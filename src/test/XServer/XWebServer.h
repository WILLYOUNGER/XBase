#ifndef X_WEB_SERVER_H
#define X_WEB_SERVER_H

#include "XServerBase.h"
#include "XPthreadPool.h"
#include "XHttp.h"

class XWebServer : public XServerBase
{
public:
	XWebServer(std::string ip = "0.0.0.0", int port = 23333);
private:
	void init(std::string ip, int port);

	virtual void ConnectCallback(XNETSTRUCT::XSocket socket) override;

	virtual void CloseCallback(void) override;

	virtual void ReadCallback(XNETSTRUCT::XMsgPtr msg) override;

	virtual bool WriteCallback(XNETSTRUCT::XSocket epollfd, XNETSTRUCT::XSocket socket) override;
private:
	XPthreadPool<XNETSTRUCT::XMsgPtr, XHttp>* pool;
};

#endif