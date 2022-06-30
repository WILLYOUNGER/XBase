#ifndef X_SERVERBASE_H
#define X_SERVERBASE_H

#include "XBaseNet.h"
#include "XStruct.h"
#include "XPthreadPool.h"
#include "XHttp.h"

class XServerBase
{
public:
	XServerBase();

	XServerBase(std::string ip, int port);
protected:
	void init(std::string ip, int port);

	virtual void ConnectCallback(XNETSTRUCT::XSocket socket);

	virtual void CloseCallback(void);

	virtual void ReadCallback(XNETSTRUCT::XMsgPtr msg);

	virtual bool WriteCallback(XNETSTRUCT::XSocket epollfd, XNETSTRUCT::XSocket socket);
private:
	XNETBASE::XServer* m_server;

public:
	static std::map<XNETSTRUCT::XSocket, std::list<XNETSTRUCT::XResponse>*> m_reply;
};

#endif