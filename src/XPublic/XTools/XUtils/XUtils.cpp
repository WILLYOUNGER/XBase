#include "XUtils.h"

#include <fcntl.h>
#include <signal.h>
#include <errno.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <unistd.h>	//close(int sockfd)
#include <cstring>
#include <chrono>
#include <ctime>
#include <time.h>

using namespace std;
using namespace XUTILSTOOL;
using namespace XNETSTRUCT;

NetUtils* NetUtils::m_netUtils_instance = nullptr;

NetUtils* NetUtils::getInstance()
{
	if (m_netUtils_instance == nullptr)
	{
		m_netUtils_instance->init();
	}
	return m_netUtils_instance;
}

NetUtils::~NetUtils()
{
}

NetUtils::NetUtils()
{
}

void NetUtils::init()
{
}

int NetUtils::setnonblocking(XSocket fd)
{
	int old_option = fcntl(fd, F_GETFL);
	int new_option = old_option |= O_NONBLOCK;
	fcntl(fd, F_SETFL, new_option);
	return old_option;
}

void NetUtils::addfd(XSocket epollfd, XSocket fd, bool one_shot, int mode)
{
	epoll_event event;
	event.data.fd = fd;
	event.events = EPOLLIN | EPOLLRDHUP;
	if (one_shot)
	{
		event.events |= EPOLLONESHOT;
	}
	if (mode == 1)
	{
		event.events |= EPOLLET;
	}
	epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &event);
	setnonblocking(fd);
}

void NetUtils::removefd(XSocket epollfd, XSocket fd)
{
	epoll_ctl(epollfd, EPOLL_CTL_DEL, fd, 0);
	close(fd);
}

void NetUtils::modfd(XSocket epollfd, XSocket fd, int ev, int mode)
{
	epoll_event event;
	event.data.fd = fd;
    if (1 == mode)
        event.events = ev | EPOLLET | EPOLLONESHOT | EPOLLRDHUP;
    else
        event.events = ev | EPOLLONESHOT | EPOLLRDHUP;

    epoll_ctl(epollfd, EPOLL_CTL_MOD, fd, &event);
}

void NetUtils::addsig(int sig, void (handler)(int), bool restart)
{
	struct sigaction sa;
	memset(&sa, '\0', sizeof(sa));
	sa.sa_handler = handler;
	if (restart)
	{
		sa.sa_flags |= SA_RESTART;
	}
	sigfillset(&sa.sa_mask);
	assert(sigaction(sig, &sa, NULL) != -1);
}

void NetUtils::senderror(XSocket connfd, const char* info)
{
	XLOG_ERROR("%s", info);
	send(connfd, info, strlen(info), 0);
	close(connfd);
}

void NetUtils::setFdCloseType(XNETSTRUCT::XSocket fd, int isCloseNow, int time)
{
	struct linger tmp = {isCloseNow, time};
        setsockopt(fd, SOL_SOCKET, SO_LINGER, &tmp, sizeof(tmp));
}

namespace XNETSTRUCT {

	static unsigned int g_i_time = (unsigned int)time(0);

int PUB_getRandInt(int begin, int end)
{
    if (time(0) != g_i_time)
    {
        g_i_time = (unsigned int)time(0);
        srand((unsigned int)g_i_time);
    }
    int _i_res = begin + rand() % (end - begin + 1);
    
    return _i_res;
}


float PUB_getRandFloat(float begin, float end)
{
    if (time(0) != g_i_time)
    {
        srand((unsigned int)time(0));
    }
    float _f_res = begin + (float)(rand()) / RAND_MAX * (end - begin);
    
    return _f_res;
}


float PUB_adjustmentRange(float adjustmentNum, float begin, float end)
{
    if (adjustmentNum > end)
    {
        adjustmentNum = end;
    }
    else if (adjustmentNum < begin)
    {
        adjustmentNum = begin;
    }
    return adjustmentNum;
}

void PUB_getNowTime(long &s, long &ms)
{
    std::chrono::system_clock::time_point time_point_now = std::chrono::system_clock::now(); // 获取当前时间点
    std::chrono::system_clock::duration duration_since_epoch
            = time_point_now.time_since_epoch(); // 从1970-01-01 00:00:00到当前时间点的时长
    time_t microseconds_since_epoch
            = std::chrono::duration_cast<std::chrono::microseconds>(duration_since_epoch).count(); // 将时长转换为微秒数
    time_t seconds_since_epoch = microseconds_since_epoch / 1000000; // 将时长转换为秒数
    std::tm current_time = *std::localtime(&seconds_since_epoch); // 获取当前时间（精确到秒）
    time_t tm_microsec = microseconds_since_epoch % 1000; // 当前时间的微妙数
    time_t tm_millisec = microseconds_since_epoch / 1000 % 1000; // 当前时间的毫秒数
    
    s = current_time.tm_sec;
    ms = tm_millisec;
}
}
