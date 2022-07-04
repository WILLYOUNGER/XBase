/**
*  @file XServerBase.h
*  @brief    服务器基类
*  Details.
*
*  @author   wangxinxiao
*  @email    wxx1035@163.com
*  @version  1.0.0.0
*  @date     2022/6/30
*
*  Change History :
*  <Date>     | <Version> | <Author>       | <Description>
*  2022/6/30 | 1.0.0.0  | wangxinxiao      | Create file
*-
*/

#ifndef X_PUBLIC_TOOLS_SERVERBASE_SERVERBASE_H
#define X_PUBLIC_TOOLS_SERVERBASE_SERVERBASE_H

#include "XBaseNet.h"
#include "XNetStruct.h"
#include "XPthreadPool.h"
#include "XHttp.h"

/**
 * @brief 服务器基类
 * 声明了连接 关闭 读取 发送回调
 * 需在派生类调用init(ip, port)方可使用
 * 
 */
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
	XNETBASE::XServer* m_xserver_server;

public:
	static std::map<XNETSTRUCT::XSocket, std::list<XNETSTRUCT::XResponse>*> m_reply;
};

#endif /* X_PUBLIC_TOOLS_SERVERBASE_SERVERBASE_H */