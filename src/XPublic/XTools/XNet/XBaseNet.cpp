#include "XBaseNet.h"
#include <sys/types.h>	//socket's types
#include <sys/socket.h>	//socket
#include <netinet/in.h>	//socket's len about inet_ntop
#include <arpa/inet.h>	//inet_ntop and inet_pton
#include <sys/epoll.h>
#include <errno.h>
#include <signal.h>
#include <unistd.h>		//close
#include <stdlib.h>
#include <assert.h>
#include <memory.h>
#include <pthread.h>

#include "XUtils.h"
#include "XLog.h"

using namespace std;
using namespace XNETBASE;
using namespace XNETSTRUCT;
using namespace XUTILSTOOL;

XServer::XServer()
{
	XServer("0.0.0.0", 23333);
}

XServer::XServer(std::string ip, int port, int mode, PROTOTYPE protoType)
{
	m_str_ip = ip;
	m_i_port = port;
	m_i_mode = mode;
}

XServer::~XServer()
{
	stopListen();
}

void XServer::beginListen()
{
	struct sockaddr_in address;
	memset(&address, 0, sizeof(address));
	address.sin_family = AF_INET;
	inet_pton(AF_INET, m_str_ip.c_str(), &address.sin_addr);
	address.sin_port = htons(m_i_port);

	m_xsocket_sockfd = socket(PF_INET, SOCK_STREAM, 0);
	assert(m_xsocket_sockfd >= 0);

	NETUTILS->setFdCloseType(m_xsocket_sockfd, 1, 1);

	int ret = bind(m_xsocket_sockfd, (struct sockaddr*) &address, sizeof(address) );
	assert(ret != -1);

	ret = listen(m_xsocket_sockfd, m_i_maxConnectNum);
	assert(ret != -1);

	m_xsocket_epollfd = epoll_create(MAX_FD);

	assert(m_xsocket_epollfd != -1);
	NETUTILS->setnonblocking(m_xsocket_epollfd);
	NETUTILS->addfd(m_xsocket_epollfd, m_xsocket_sockfd, false, m_i_mode);

	NETUTILS->addsig(SIGPIPE, SIG_IGN);

	assert(m_xsocket_epollfd >= 0);

	int reuse = 1;
	setsockopt(m_xsocket_sockfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));

	m_b_stop = false;
	pthread_create(&m_pthread_pthreadId, NULL, loop, (void*)this);
}

void XServer::sig_handler(int sig)
{
	int save_errno = errno;
	int msg = sig;
	errno = save_errno;
}

void* XServer::loop(void* _this)
{
	XServer* object = (XServer*)_this;
	object->run();
	return _this;
}

void XServer::run()
{
	while (!m_b_stop)
	{
		epoll_event events[MAX_EVENT_NUMBER];
		int ret = epoll_wait(m_xsocket_epollfd, events, MAX_EVENT_NUMBER, -1);
		for (int i = 0; i < ret; i++)
		{
			XSocket sockfd = events[i].data.fd;
			if (sockfd == m_xsocket_sockfd)
			{
				struct sockaddr_in client_address;
				socklen_t client_addresslength = sizeof(client_address);
				XSocket connfd = accept(m_xsocket_sockfd, (struct sockaddr*) &client_address, &client_addresslength);
				if (connfd < 0)
				{
					XLOG_ERROR("error:%d", errno);
				}
				if (m_i_sockfdNum >= MAX_FD)
				{
					NETUTILS->senderror(connfd, "this server is busy");
					continue;
				}
				if (m_xsConCB_connectCb == nullptr)
				{
					NETUTILS->senderror(connfd, "not find server");
				}
				else
				{
					m_xsConCB_connectCb(connfd);
					//_Debug
					int reuse = 1;
					setsockopt(connfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
					NETUTILS->addfd(m_xsocket_epollfd, connfd, true, m_i_mode);
					m_i_sockfdNum++;
				}
			}
			else if (events[i].events & (EPOLLRDHUP | EPOLLHUP | EPOLLERR))
			{
				NETUTILS->removefd(m_xsocket_epollfd, sockfd);
				m_i_sockfdNum--;
			}
			else if (events[i].events & EPOLLIN)
			{
				if (m_xsReadCB_readCb == nullptr)
				{
					NETUTILS->senderror(sockfd, "server cant recv message");
				}
				else
				{
					int _read_bytes_num = 0;
					int _read_bytes_idx = 0;
					char _read_buf[READ_BUFFER_SIZE];
					memset(_read_buf, '\0', READ_BUFFER_SIZE);
					int _isError = false;
					while (true)
					{
						if (_read_bytes_idx > READ_BUFFER_SIZE)
						{
							break;
						}
						_read_bytes_num = recv(sockfd, _read_buf + _read_bytes_idx, READ_BUFFER_SIZE - _read_bytes_idx, 0);
						if (_read_bytes_num == -1)
						{
							if (errno == EAGAIN || errno == EWOULDBLOCK)
							{
								break;
							}
							NETUTILS->senderror(sockfd, "socket is error");
							_isError = true;
							break;
						}
						else if (_read_bytes_num == 0)
						{
							NETUTILS->senderror(sockfd, "recv a null requset");
							_isError = true;
							break;
						}
						_read_bytes_idx += _read_bytes_num;
					}
					if (!_isError)
					{
						auto mes = make_shared<XMessage>(new XMessage(_read_buf, m_xsocket_epollfd, sockfd));
						m_xsReadCB_readCb(mes);
					}
				}
			}
			else if (events[i].events & EPOLLOUT)
			{
				if (m_xsWriteCB_writeCb == nullptr)
				{
					NETUTILS->senderror(sockfd, "server cant send message");
				}
				else
				{
					if (m_xsWriteCB_writeCb(m_xsocket_epollfd, sockfd))
					{
						NETUTILS->removefd(m_xsocket_epollfd, sockfd);
						m_i_sockfdNum--;
					}
				}
			}
		}
	}
}

void XServer::stopListen()
{
	if (m_b_stop == false)
	{
		m_b_stop = true;
		pthread_join(m_pthread_pthreadId, NULL);
	}
}

XClient::XClient(string ip, int port, int mode, PROTOTYPE protoType)
	:m_str_ip(ip), m_i_port(port), m_mode(mode), m_protoType_protoType(protoType)
{
}

XClient::~XClient()
{
	stopConnect();
}

void XClient::beginConnect()
{
	memset(&m_sockaddr_address, 0, sizeof(m_sockaddr_address));
	m_sockaddr_address.sin_family = AF_INET;
	inet_pton(AF_INET, m_str_ip.c_str(), &m_sockaddr_address.sin_addr);
	m_sockaddr_address.sin_port = htons(m_i_port);

	m_xsocket_sockfd = socket(PF_INET, SOCK_STREAM, 0);
	assert(m_xsocket_sockfd >= 0);

	m_xsocket_epollfd = epoll_create(5);

	m_b_stop = false;
	m_b_connected  = false;
	pthread_create(&m_pthread_pthreadId, NULL, loop, (void*)this);
}

void XClient::sendMessage(XMsgPtr _msg)
{
	;
}

void XClient::stopConnect()
{
	if (m_b_stop == false)
	{
		m_b_stop = true;
		pthread_join(m_pthread_pthreadId, NULL);
	}
}

void* XClient::loop(void* _this)
{
	XClient* _client = (XClient*) _this;
	_client->run();
	return _this;
}

void XClient::run()
{
	while (!m_b_stop)
	{
		if (m_b_connected)
		{
			epoll_event events[MAX_EVENT_NUMBER];
			int number = epoll_wait(m_xsocket_epollfd, events, MAX_EVENT_NUMBER, -1);
			if (number < 0 && errno != EINTR)
			{
				XLOG_ERROR("epoll failure");
				break;
			}
			for (int i = 0; i < number; i++)
			{
				int sockfd = events[i].data.fd;
				if (events[i].events & (EPOLLRDHUP | EPOLLHUP | EPOLLERR))
				{
					NETUTILS->removefd(m_xsocket_epollfd, m_xsocket_sockfd);
					m_b_connected = false;
				}
				else if (events[i].events & EPOLLIN)
				{
					if (m_xsReadCB_readCb)
					{
						char _read_buf[READ_BUFFER_SIZE];
						int _read_bytes_num = 0;
						int _read_bytes_idx = 0;
						bool _isError = false;
						memset(_read_buf, '\0', sizeof(_read_buf));
						while (true)
						{
							if (_read_bytes_idx > READ_BUFFER_SIZE)
							{
								break;
							}
							_read_bytes_num = recv(m_xsocket_sockfd, _read_buf + _read_bytes_idx, READ_BUFFER_SIZE - _read_bytes_idx, 0);
							if (_read_bytes_num < 0)
							{
								if (errno == EAGAIN || errno == EWOULDBLOCK)
								{
									break;
								}
								_isError = true;
								break;
							}
							else if (_read_bytes_num == 0)
							{
								_isError = true;
								break;
							}
							_read_bytes_idx += _read_bytes_num;
						}
						if (!_isError)
						{
							auto _msg = make_shared<XMessage>(new XMessage(_read_buf, m_xsocket_epollfd, sockfd));
							m_xsReadCB_readCb(_msg);
						}
					}
					else
					{
						XLOG_ERROR("client's readCallback is null.");
					}
				}
				else if (events[i].events & EPOLLIN)
				{
					if (m_xsWriteCB_writeCb)
					{
						m_xsWriteCB_writeCb(m_xsocket_epollfd, sockfd);
					}
					else
					{
						XLOG_ERROR("client's wirteCallback is null.");
					}
				}
			}
		}
		else
		{
			if (connect(m_xsocket_sockfd, (struct sockaddr*)&m_sockaddr_address, sizeof(m_sockaddr_address)) >= 0)
			{
				if (m_xsConCB_connectCb)
				{
					NETUTILS->addfd(m_xsocket_epollfd, m_xsocket_sockfd, true, m_mode);
					m_xsConCB_connectCb(m_xsocket_sockfd);
					m_b_connected = true;

				}
				else
				{
					close(m_xsocket_sockfd);
					XLOG_ERROR("client's connectCallback is null.");
				}
			}
		}
	}
}