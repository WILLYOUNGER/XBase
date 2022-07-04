/**
*  @file XServletDefine.h
*  @brief    servlet单例自动创建类
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

#ifndef X_PUBLIC_SERVLET_SERVLETDEFINE_H
#define X_PUBLIC_SERVLET_SERVLETDEFINE_H

#include <map>
#include <string>
#include "XServlet.h"

extern std::map<const std::string, funcPtr> XServletMap;

#define REGISETRSERVLET(ClassName) \
	class Register##ClassName \
	{	\
		public: \
		static XServlet* getInstance() \
		{	\
			return new ClassName; \
		}	\
		private:	\
		static const Register _staticRegister;	\
	};	\
	const Register Register##ClassName::_staticRegister(#ClassName, Register##ClassName::getInstance);

#endif /* X_PUBLIC_SERVLET_SERVLETDEFINE_H */