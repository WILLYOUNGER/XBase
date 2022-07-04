/**
*  @file XLog.h
*  @brief    日志类
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

#ifndef X_PUBLIC_TOOLS_LOG_LOG_H
#define X_PUBLIC_TOOLS_LOG_LOG_H

#include <string>
#include <cstdio>
#include "XLock.h"
#include "XBlockQueue.h"

/**
 * @brief 日志内容结构体
 * 
 */
typedef struct XLOGCONTENT
{
	std::string _str_time;
	std::string _str_beginColor;
	std::string _str_logContent;
	std::string _str_endColor;
} XLOGCONTENT;

/**
 * @brief 日志类
 * 
 */
class XLog
{
public:
	enum XLevel
	{
		DEBUG = 0,
		INFO = 1,
		WARN = 2,
		ERROR = 3,
	};
public:
	static XLog* getInstance()
	{
		static XLog instance;
		return &instance;
	}

	static void *flush_log_thread(void *args)
	{
		return XLog::getInstance()->async_write_log();
	}

	/**
	 * @brief 初始化日志类
	 * 
	 * @param file_name 文件全称 如："XWebLog.txt"
	 * @param close_log 开启状态：0:close 1:file and cmd 2: file 3: cmd
	 * @param level 	打印大于这个等级的日志 0：debug 1：info 2：warn 3：error
	 * @param log_buf_size 每行日志最大长度
	 * @param split_lines  每个文件最大行数，超出会自动创建新文件
	 * @param max_queue_size 如果大于0为异步日志，输入为队列长度，否则为同步日志
	 * @return true 
	 * @return false 失败，这里可以检查一下./log文件夹有没有创建
	 */
	bool init(const char* fileName, int closeLog, int level = 0, int logBufSize = 8192, int splitLines = 5000000, int maxQueueSize = 0);

	/**
	 * @brief 输出日志
	 * 
	 * @param level 日志等级
	 * @param pos 需要打印日志的代码位置
	 * @param format 日志内容
	 * @param ... 
	 */
	void write_log(int level, std::string pos, const char *format, ...);

	/**
	 * @brief 获取对应格式的字符串
	 * 
	 * @param format 
	 * @param ... 
	 * @return std::string 
	 */
	std::string getFileLineFunctionName(const char *format, ...);

	bool isOpen()
	{
		return m_i_closeLog;
	}

	void flush(void);

	~XLog();
private:
	XLog();

	void *async_write_log()
	{
		XLOGCONTENT single_log;
		while (m_blockQueueXLC_logQueue->pop(single_log))
		{
			m_locker_mutex.lock();
			fputs((single_log._str_time + single_log._str_logContent).c_str(), m_file_fp_ptr);
			if (m_i_closeLog != 2)
   			{
				std::string _str_logWithColor = single_log._str_time + single_log._str_beginColor + single_log._str_logContent + single_log._str_endColor;
   				printf("%s", _str_logWithColor.c_str());
			}
			m_locker_mutex.unlock();
		}
		char* _c_res_ptr = nullptr;
		return _c_res_ptr;
	}
private:
	char m_c_dirName_s[128];
	char m_c_fileName_s[128];
	int m_i_splitLines;
	int m_i_logBufSizes;
	long long m_ll_count;
	int m_i_today;
	FILE* m_file_fp_ptr;
	char* m_c_buf_ptr;
	XBlockQueue<XLOGCONTENT> *m_blockQueueXLC_logQueue;
	bool m_b_isAsync;
	Locker m_locker_mutex;
	int m_i_closeLog; 	//0:close 1:file and cmd 2: file 3: cmd
	int m_i_level;
};

#define XLOG XLog::getInstance()

#define XLOG_DEBUG(format, ...) if(XLOG->isOpen()) {XLOG->write_log(0, XLOG->getFileLineFunctionName("%s(%d)-<%s> ", __FILE__, __LINE__, __FUNCTION__), format, ##__VA_ARGS__); XLOG->flush();}
#define XLOG_INFO(format, ...) if(XLOG->isOpen()) {XLOG->write_log(1, XLOG->getFileLineFunctionName("%s(%d)-<%s> ", __FILE__, __LINE__, __FUNCTION__), format, ##__VA_ARGS__); XLOG->flush();}
#define XLOG_WARN(format, ...) if(XLOG->isOpen()) {XLOG->write_log(2, XLOG->getFileLineFunctionName("%s(%d)-<%s> ", __FILE__, __LINE__, __FUNCTION__), format, ##__VA_ARGS__); XLOG->flush();}
#define XLOG_ERROR(format, ...) if(XLOG->isOpen()) {XLOG->write_log(3, XLOG->getFileLineFunctionName("%s(%d)-<%s> ", __FILE__, __LINE__, __FUNCTION__), format, ##__VA_ARGS__); XLOG->flush();}


#endif /* X_PUBLIC_TOOLS_LOG_LOG_H */