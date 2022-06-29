#ifndef X_WEB_SERVER_H
#define X_WEB_SERVER_H

#include "XBaseNet.h"
#include "XStruct.h"
#include "XPthreadPool.h"
#include "XHttp.h"

using namespace XNETSTRUCT;
class XWebServer
{
public:
	XWebServer();
private:
	void ConnectCallback(XNETSTRUCT::XSocket socket);

	void CloseCallback(void);

	void ReadCallback(XNETSTRUCT::XMsgPtr msg);

	bool WriteCallback(XNETSTRUCT::XSocket epollfd, XNETSTRUCT::XSocket socket);
private:
	XNETBASE::XServer* m_web_server;

	XPthreadPool<XNETSTRUCT::XMsgPtr, XHttp>* pool;
};

#endif