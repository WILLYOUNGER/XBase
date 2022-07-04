/**
*  @file     XUtils.h
*  @brief    通用工具库
*  Details.
*
*  @author   wangxinxiao
*  @email    wxx1035@163.com
*  @version  1.0.0.0
*  @date     2022/7/2
*
*  Change History :
*  <Date>     | <Version> | <Author>       | <Description>
*  2022/6/4   | 1.0.0.0   | wangxinxiao    | Create file
*  2022/7/2   | 1.0.1.0   | wangxinxiao    | Add NetTool
*-
*/

#ifndef X_PUBLIC_TOOLS_UTILS_UTILS_H
#define X_PUBLIC_TOOLS_UTILS_UTILS_H

#include "XNetStruct.h"

/**
 * @brief 通用工具类命名空间，主要是网络类，和数字类工具
 */
namespace XUTILSTOOL{

/**
 * @brief 网络类的通用操作单例
 * 诸如给epollfd添删改fd 与 sig
 */
class NetUtils
{
public:
	static NetUtils* getInstance();

	/**
	 * @brief 将fd设置为不阻塞
	 * 
	 * @param fd 套接字
	 * @return int 老的设置
	 */
	int setnonblocking(XNETSTRUCT::XSocket fd);

	/**
	 * @brief 给epollfd添加fd
	 * 
	 * @param epollfd 
	 * @param fd 被添加的套接字
	 * @param one_shot 一个事件只处理一次
	 * @param mode 模式 1:ET 0：LT
	 */
	void addfd(XNETSTRUCT::XSocket epollfd, XNETSTRUCT::XSocket fd, bool one_shot, int mode);

	/**
	 * @brief 从epollfd中移除监听fd
	 * 
	 * @param epollfd 
	 * @param fd 被移除的套接字
	 */
	void removefd(XNETSTRUCT::XSocket epollfd, XNETSTRUCT::XSocket fd);

	/**
	 * @brief 修改在epollfd中的fd的设置
	 * 
	 * @param epollfd 
	 * @param fd 被修改的套接字
	 * @param ev 需要改成的配置
	 * @param mode 模式 1:ET 0：LT
	 */
	void modfd(XNETSTRUCT::XSocket epollfd, XNETSTRUCT::XSocket fd, int ev, int mode);

	/**
	 * @brief 设置信号处理函数
	 * 
	 * @param sig 信号标号
	 * @param handler 处理函数
	 * @param restart 是否设置自动重启，默认重启
	 */
	void addsig(int sig, void (handler)(int), bool restart = true );

	/**
	 * @brief 给socket发送错误信息
	 * 
	 * @param connfd 对应socket
	 * @param info 错误信息
	 */
	void senderror(XNETSTRUCT::XSocket connfd, const char* info);
	
	/**
	 * @brief 设置关闭方式
	 * 
	 * @param connfd 对应socket
	 * @param isCloseNow true：立刻关闭 false：优雅关闭
	 * @param time 优雅关闭时间（值在 isloseNow关键字为false时生效）
	 */
	void setFdCloseType(XNETSTRUCT::XSocket fd, int isCloseNow, int time);
private:
	NetUtils();

	NetUtils(NetUtils &utils) = delete;

	~NetUtils();

	void init();
private:
	static NetUtils* m_netUtils_instance;
};

/**
* @brief 获取随机整数
* @param begin：前范围（包括begin）后范围（包括end）
* @return int 随机整数
*/
int PUB_getRandInt(int begin, int end);

/**
* @brief 获取随机有理数
* @param begin：前范围（包括begin）后范围（包括end）
* @return float 随机有理数
*/
float PUB_getRandFloat(float begin, float end);

/**
* @brief 调整范围，限定数的最大最小
* @param begin：前范围（包括begin）后范围（包括end）
* @return 调整后的数
*/
float PUB_adjustmentRange(float adjustmentNum, float begin, float end);

/**
* @brief 获取当前时间
* @param s：秒  ms：毫秒
*/
void PUB_getNowTime(long &s, long &ms);

#define NETUTILS NetUtils::getInstance()
}

#endif /* X_PUBLIC_TOOLS_UTILS_UTILS_H */