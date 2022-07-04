/**
*  @file XServlet.h
*  @brief    servlet基类
*  Details.
*
*  @author   wangxinxiao
*  @email    wxx1035@163.com
*  @version  1.0.0.0
*  @date     2022/5/4
*
*  Change History :
*  <Date>     | <Version> | <Author>       | <Description>
*  2022/5/4 | 1.0.0.0  | wangxinxiao      | Create file
*-
*/

#ifndef X_SERVLET_H
#define X_SERVLET_H

#include "XHttpStruct.h"
#include <map>

class XServlet;

typedef XServlet* (*funcPtr)();

/**
 * @brief Servlet基类
 * 
 */
class XServlet
{
public:
	virtual void doGet(XNETSTRUCT::XRequest &req, XNETSTRUCT::XResponse &res) {}

	virtual void doPost(XNETSTRUCT::XRequest &req, XNETSTRUCT::XResponse &res) {}

	std::string getPath() {return m_path;}

protected:
	void setPath(std::string str) { m_path = str;}

	static std::map<std::string, std::string> m_secion;

	std::string m_path;
};

/**
 * @brief Servlet工厂，供用宏生成单例
 * 
 */
class XServletFactory
{
public:
	static XServlet* getInstance(const std::string &className);
	static void registerClass(const std::string &className, funcPtr func);
};

class Register
{
public:
    Register(const char* className, funcPtr fp);
};

#endif