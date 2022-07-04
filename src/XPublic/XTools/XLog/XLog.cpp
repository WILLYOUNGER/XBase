#include "XLog.h"
#include <cstring>
#include <cstdarg>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "sys/time.h"

using namespace std;

XLog::XLog()
{
	m_ll_count = 0;
	m_b_isAsync = false;
    memset(m_c_dirName_s, '\0', sizeof(m_c_dirName_s));
    strcpy(m_c_dirName_s, "./log");
}

XLog::~XLog()
{
	if (m_file_fp_ptr != NULL)
	{
		fclose(m_file_fp_ptr);
	}
}

bool XLog::init(const char* fileName, int closeLog, int level, int logBufSize, int splitLines, int maxQueueSize)
{
    memset(m_c_fileName_s, '\0', sizeof(m_c_fileName_s));
    strcpy(m_c_fileName_s, fileName);
    m_i_closeLog = closeLog;
    m_i_splitLines = splitLines;
    m_i_logBufSizes = logBufSize;
    m_i_level = level;
	if (maxQueueSize > 0)
	{
		m_b_isAsync = true;
		m_blockQueueXLC_logQueue = new XBlockQueue<XLOGCONTENT>(maxQueueSize);
		pthread_t pid;
		pthread_create(&pid, NULL, flush_log_thread, NULL);
	}
	m_c_buf_ptr = new char[m_i_logBufSizes];
	memset(m_c_buf_ptr, '\0', m_i_logBufSizes);

	time_t t = time(NULL);
	struct tm *sys_tm = localtime(&t);
	struct tm my_tm = *sys_tm;

    std::string file_full_name = std::string(m_c_dirName_s) + "/" + to_string(my_tm.tm_year + 1900) + "_" + to_string(my_tm.tm_mon + 1) + "_" + to_string(my_tm.tm_mday) + std::string(m_c_fileName_s);

    int ret = access(m_c_dirName_s, F_OK);
    if (!ret)
    {
        ret = mkdir(m_c_dirName_s, S_IRWXU);
        if (!ret)
        {
            return false;
        }
    }

    const char* file_full_name_c = file_full_name.c_str();

	m_i_today = my_tm.tm_mday;

	m_file_fp_ptr = fopen(file_full_name_c, "a");

	if (m_file_fp_ptr == NULL)
	{
		return false;
	}
	return true;
}

void XLog::write_log(int level, string pos, const char *format, ...)
{
    if (level < m_i_level || m_i_closeLog == 0)
    {
        return;
    }
	struct timeval now = {0, 0};
    gettimeofday(&now, NULL);
    time_t t = now.tv_sec;
    struct tm *sys_tm = localtime(&t);
    struct tm my_tm = *sys_tm;
    XLOGCONTENT _logContent_content;
    char s[16] = {0};
    _logContent_content._str_logContent = pos;
    switch (level)
    {
    case 0:
        strcpy(s, "\033[1;32;40m");
        _logContent_content._str_logContent += "[debug] ";
        break;
    case 1:
        strcpy(s, "\033[1;34;40m");
        _logContent_content._str_logContent += "[info] ";
        break;
    case 2:
        strcpy(s, "\033[1;33;40m");
        _logContent_content._str_logContent += "[warn] ";
        break;
    case 3:
        strcpy(s, "\033[1;31;40m");
        _logContent_content._str_logContent += "[error] ";
        break;
    }
    _logContent_content._str_beginColor = s;

    //写入一个log，对m_count++, m_split_lines最大行数
    m_locker_mutex.lock();
    m_ll_count++;

    if (m_i_today != my_tm.tm_mday || m_ll_count % m_i_splitLines == 0) //everyday log
    {
        fflush(m_file_fp_ptr);
        fclose(m_file_fp_ptr);

        if (m_i_today != my_tm.tm_mday)
        {
            std::string file_full_name = string(m_c_dirName_s) + "/" + to_string(my_tm.tm_year + 1900) + "_" + to_string(my_tm.tm_mon + 1) + "_" + to_string(my_tm.tm_mday) + string(m_c_dirName_s);
            const char* file_full_name_c = file_full_name.c_str();
            m_file_fp_ptr = fopen(file_full_name_c, "a");
        }
        else
        {
            std::string file_full_name = string(m_c_dirName_s) + "/" + to_string(my_tm.tm_year + 1900) + "_" + to_string(my_tm.tm_mon + 1) + "_" + to_string(my_tm.tm_mday) + "_" + to_string(m_ll_count / m_i_splitLines) + std::string(m_c_fileName_s);
            const char* file_full_name_c = file_full_name.c_str();
            m_file_fp_ptr = fopen(file_full_name_c, "a");
        }
    }

    m_locker_mutex.unlock();

    va_list valst;
    va_start(valst, format);

    m_locker_mutex.lock();

    char _c_buffer_ptr[48] = {0};

    //写入的具体时间内容格式
    int n = snprintf(_c_buffer_ptr, 48, "%d-%02d-%02d %02d:%02d:%02d.%06ld  ",
                     my_tm.tm_year + 1900, my_tm.tm_mon + 1, my_tm.tm_mday,
                     my_tm.tm_hour, my_tm.tm_min, my_tm.tm_sec, now.tv_usec);
    _logContent_content._str_time = string(_c_buffer_ptr, 0, n);

    int m = vsnprintf(m_c_buf_ptr, m_i_logBufSizes - 1, format, valst);
    va_end(valst);
    m_c_buf_ptr[m] = '\n';
    m_c_buf_ptr[m + 1] = '\0';
    _logContent_content._str_logContent += string(m_c_buf_ptr, 0, m + 1);

    _logContent_content._str_endColor = "\033[0m";

    m_locker_mutex.unlock();

    if (m_i_closeLog != 3)
    {
        if (m_b_isAsync && !m_blockQueueXLC_logQueue->full())
        {
            m_blockQueueXLC_logQueue->push(_logContent_content);
        }
        else
        {
            m_locker_mutex.lock();
			fputs((_logContent_content._str_time + _logContent_content._str_logContent).c_str(), m_file_fp_ptr);
			m_locker_mutex.unlock();
        }
    }
    if (m_i_closeLog != 2)
    {
        if (m_b_isAsync && !m_blockQueueXLC_logQueue->full())
        {
        }
        else
        {
            m_locker_mutex.lock();
            std::string _str_logWithColor = _logContent_content._str_time + _logContent_content._str_beginColor + _logContent_content._str_logContent + _logContent_content._str_endColor;
   			printf("%s", _str_logWithColor.c_str());
            m_locker_mutex.unlock();
        }
    }

    va_end(valst);
}

string XLog::getFileLineFunctionName(const char *format, ...)
{
    va_list valst;
    va_start(valst, format);
    char _char_pos_ptr[201] = {0};
    int m = vsnprintf(_char_pos_ptr, 200, format, valst);
    va_end(valst);

    string _str_pos = string(_char_pos_ptr, 0, m);

    size_t _size_found = _str_pos.rfind("/");
    if (_size_found != std::string::npos)
    {
        string _str_posTemp_ptr = _str_pos.substr(0, _size_found);
        _str_pos = _str_pos.substr(_size_found, m - _size_found);
        size_t _size_foundTemp = _str_posTemp_ptr.rfind("/");
        if (_size_foundTemp != std::string::npos)
        {
            _str_pos = _str_posTemp_ptr.substr(_size_foundTemp, _str_posTemp_ptr.length() - _size_foundTemp) + _str_pos;
        }
    }

    return _str_pos;
}

void XLog::flush(void)
{
	m_locker_mutex.lock();
	fflush(m_file_fp_ptr);
	m_locker_mutex.unlock();
}