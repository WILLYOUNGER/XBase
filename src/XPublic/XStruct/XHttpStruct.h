#ifndef X_PUBLIC_STRUCT_HTTPSTRUCT_H
#define X_PUBLIC_STRUCT_HTTPSTRUCT_H

#include <map>
#include <list>
#include <string>
#include <unistd.h>
#include <sys/mman.h>
#include "XNetStruct.h"

namespace XNETSTRUCT {

typedef enum _method {
	NONE = 0,
	GET,
	POST,
	HEAD,
	PUT,
	DELETE,
	TRACE,
	OPTIONS,
	CONNECT,
	PATCH
} METHOD;

typedef enum _http_code {
	NO_REQUEST,
	GET_REQUEST,
	BAD_REQUEST,
	NO_RESOURCE,
	FORBIDDEN_REQUEST,
	FILE_REQUEST,
	CSS_REQUEST,
	ICO_REQUEST,
	JPG_REQUEST,
	GIF_REQUEST,
	INTERNAL_ERROR,
	CLOSED_CONNECTION
} HTTP_CODE;

class XRequest
{
public:
	XRequest()
	{
		m_method_method = NONE;
	}

	void setMethod(METHOD method)
	{
		m_method_method = method;
	}

	METHOD getMethod() { return m_method_method;}

	std::string getAttribute(std::string key) {return m_mapStrStr_attribute[key];}

	void setAttribute(std::string key, std::string value) {m_mapStrStr_attribute.insert(std::pair<std::string, std::string>(key, value));}

	void clear()
	{
		m_mapStrStr_attribute.clear();
		m_method_method = NONE;
		m_str_path = "/";
		m_str_version = "HTTP/1.0";
	}

	std::string getPath() {return m_str_path;}

	void setPath(std::string str) { m_str_path = str;}

	std::string getVersion() {return m_str_version;}

	void setVersion(std::string str) { m_str_version = str;}

	int getAttributeNum()
	{
		return m_mapStrStr_attribute.size();
	}
private:
	METHOD m_method_method;

	std::string m_str_path;

	std::string m_str_version;

	std::map<std::string, std::string> m_mapStrStr_attribute;
};

class XResponse
{
public:
	XResponse()
	{
		clear();
	}

	~XResponse()
	{
		munmap(m_c_fileAddress_ptr, m_i_contentLength);
        m_c_fileAddress_ptr = 0;
	}

	void setHttpCode(HTTP_CODE code) {m_httpCode_code = code;}

	HTTP_CODE getHttpCode() {return m_httpCode_code;}

	void clear()
	{
		m_httpCode_code = NO_RESOURCE;
		m_b_isEmpty = true;
		m_i_contentLength = 0;
	}

	void setNotEmpty()
	{
		m_b_isEmpty = false;
	}

	bool isEmpty()
	{
		return m_b_isEmpty;
	}

	void setFileAddress(char* address)
	{
		m_c_fileAddress_ptr = address;
	}

	char* getFileAddress()
	{
		return m_c_fileAddress_ptr;
	}

	void setContentLength(int length)
	{
		m_i_contentLength = length;
	}

	int getContentLength()
	{
		return m_i_contentLength;
	}

	void setHeadString(std::string _head)
	{
		m_str_head = _head;
	}

	std::string getHeadString()
	{
		return m_str_head;
	}

	void setNeedClose(bool needClose)
	{
		m_b_isNeedClose = needClose;
	}

	bool getNeedClose()
	{
		return m_b_isNeedClose;
	}

private:
	HTTP_CODE m_httpCode_code;

	bool m_b_isEmpty;

	char* m_c_fileAddress_ptr;

	int m_i_contentLength;

	bool m_b_isNeedClose { true };

	std::string m_str_head;
};
}

#endif /* X_PUBLIC_STRUCT_HTTPSTRUCT_H */