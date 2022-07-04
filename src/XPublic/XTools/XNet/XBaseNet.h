/**
*  @file XBaseNet.h
*  @brief    基础网络类
*  Details.
*
*  @author   wangxinxiao
*  @email    wxx1035@163.com
*  @version  1.0.0.0
*  @date     2022/5/2
*
*  Change History :
*  <Date>     | <Version> | <Author>       | <Description>
*  2022/5/2 | 1.0.0.0  | wangxinxiao      | Create file
*-
*/

#ifndef X_PUBLIC_TOOLS_NET_BASENET_H
#define X_PUBLIC_TOOLS_NET_BASENET_H

#include <pthread.h>
#include <netinet/in.h>
#include "XNetStruct.h"

/**
 * @brief 基础网络命名空间
 * 
 */
namespace XNETBASE{

/**
 * @brief socket服务端类
 * 
 */
class XServer final
{
public:
	XServer();

	XServer(XServer &server) = delete;

	~XServer();

	XServer(std::string ip, int port, int mode = 0, XNETSTRUCT::PROTOTYPE protoType = XNETSTRUCT::TCPNET);

	void setConnectionCallback(XNETSTRUCT::XSConCB _fun) {m_xsConCB_connectCb = _fun; }

	void setCloseCallback(XNETSTRUCT::XSCloseCB _fun) {m_xsCloseCB_closeCb = _fun; }

	void setReadCallback(XNETSTRUCT::XSReadCB _fun) {m_xsReadCB_readCb = _fun; }

	void setWriteCallback(XNETSTRUCT::XSWriteCB _fun) {m_xsWriteCB_writeCb = _fun; }

	void setMessageType(XNETSTRUCT::MESSAGETYPE _type) {m_messageType_messageType = _type; }

	void beginListen();

	void sendMessage(XNETSTRUCT::XMsgPtr _msg);

	void setMaxConnectNum(int _num) {m_i_maxConnectNum = _num;};

	void stopListen();

private:
	static void* loop(void* _this);

	void run();

	static void sig_handler(int sig);
private:
	std::string m_str_ip {"0.0.0.0"};
	int m_i_port {23333};
	int m_i_maxConnectNum {100};
	XNETSTRUCT::XSocket m_xsocket_sockfd{-1};
	XNETSTRUCT::XSocket m_xsocket_epollfd{-1};
	int m_i_mode{0};

	XNETSTRUCT::MESSAGETYPE m_messageType_messageType {XNETSTRUCT::BYTESTREAM};

	XNETSTRUCT::PROTOTYPE m_protoType_protoType {XNETSTRUCT::TCPNET};

	XNETSTRUCT::XSConCB m_xsConCB_connectCb {nullptr};
	XNETSTRUCT::XSCloseCB m_xsCloseCB_closeCb {nullptr};
	XNETSTRUCT::XSReadCB m_xsReadCB_readCb {nullptr};
	XNETSTRUCT::XSWriteCB m_xsWriteCB_writeCb {nullptr};

	bool m_b_stop {true};

	pthread_t m_pthread_pthreadId;

	int m_i_sockfdNum {0};
};

/**
 * @brief socket客户端类
 * 
 */
class XClient final
{
public:

	XClient(XClient &client) = delete;

	~XClient();

	XClient(std::string ip, int port, int mode = 0, XNETSTRUCT::PROTOTYPE protoType = XNETSTRUCT::TCPNET);

	void setConnectionCallback(XNETSTRUCT::XSConCB _fun) {m_xsConCB_connectCb = _fun; }

	void setCloseCallback(XNETSTRUCT::XSCloseCB _fun) {m_xsCloseCB_closeCb = _fun; }

	void setReadCallback(XNETSTRUCT::XSReadCB _fun) {m_xsReadCB_readCb = _fun; }

	void setWriteCallback(XNETSTRUCT::XSWriteCB _fun) {m_xsWriteCB_writeCb = _fun; }

	void setMessageType(XNETSTRUCT::MESSAGETYPE _type) {m_messageType_messageType = _type; }

	void beginConnect();

	void sendMessage(XNETSTRUCT::XMsgPtr _msg);

	void stopConnect();

private:
	XClient();

	static void* loop(void* _this);

	void run();

	static void sig_handler(int sig);
private:
	std::string m_str_ip;
	int m_i_port;
	XNETSTRUCT::XSocket m_xsocket_sockfd{-1};
	XNETSTRUCT::XSocket m_xsocket_epollfd{-1};
	int m_mode;

	XNETSTRUCT::MESSAGETYPE m_messageType_messageType {XNETSTRUCT::BYTESTREAM};

	XNETSTRUCT::PROTOTYPE m_protoType_protoType {XNETSTRUCT::TCPNET};

	XNETSTRUCT::XSConCB m_xsConCB_connectCb {nullptr};
	XNETSTRUCT::XSCloseCB m_xsCloseCB_closeCb {nullptr};
	XNETSTRUCT::XSReadCB m_xsReadCB_readCb {nullptr};
	XNETSTRUCT::XSWriteCB m_xsWriteCB_writeCb {nullptr};

	bool m_b_stop {true};

	bool m_b_connected{false};

	pthread_t m_pthread_pthreadId;

	sockaddr_in m_sockaddr_address;
};


}

#endif /* X_PUBLIC_TOOLS_NET_BASENET_H */