#include "XTool.h"

#include "iostream"
#include <chrono>
#include <ctime>

#include "time.h"

namespace XNORMALTOOL {

static unsigned int g_i_time = (unsigned int)time(0);

int PUB_getRandInt(int begin, int end)
{
    if (time(0) != g_i_time)
    {
        g_i_time = (unsigned int)time(0);
        srand((unsigned int)g_i_time);
    }
    int _i_res = begin + rand() % (end - begin + 1);
    
    return _i_res;
}


float PUB_getRandFloat(float begin, float end)
{
    if (time(0) != g_i_time)
    {
        srand((unsigned int)time(0));
    }
    float _f_res = begin + (float)(rand()) / RAND_MAX * (end - begin);
    
    return _f_res;
}


float PUB_adjustmentRange(float adjustmentNum, float begin, float end)
{
    if (adjustmentNum > end)
    {
        adjustmentNum = end;
    }
    else if (adjustmentNum < begin)
    {
        adjustmentNum = begin;
    }
    return adjustmentNum;
}

void PUB_getNowTime(long &s, long &ms)
{
    std::chrono::system_clock::time_point time_point_now = std::chrono::system_clock::now(); // 获取当前时间点
    std::chrono::system_clock::duration duration_since_epoch
            = time_point_now.time_since_epoch(); // 从1970-01-01 00:00:00到当前时间点的时长
    time_t microseconds_since_epoch
            = std::chrono::duration_cast<std::chrono::microseconds>(duration_since_epoch).count(); // 将时长转换为微秒数
    time_t seconds_since_epoch = microseconds_since_epoch / 1000000; // 将时长转换为秒数
    std::tm current_time = *std::localtime(&seconds_since_epoch); // 获取当前时间（精确到秒）
    time_t tm_microsec = microseconds_since_epoch % 1000; // 当前时间的微妙数
    time_t tm_millisec = microseconds_since_epoch / 1000 % 1000; // 当前时间的毫秒数
    
    s = current_time.tm_sec;
    ms = tm_millisec;
}

void PUB_log(std::string log)
{
    long _l_sec, _l_microsec;
    PUB_getNowTime(_l_sec, _l_microsec);
    std::cout << log << " sec: " << _l_sec << " microsec: " << _l_microsec << std::endl;
}
}
