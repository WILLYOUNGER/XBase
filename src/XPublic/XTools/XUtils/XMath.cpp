#include "XMath.h"

#include "math.h"

namespace XMATHTOOL {

float PUB_twoPointDistance(float x0, float y0, float x1, float y1)
{
    return sqrtf( pow(x0 - x1, 2) + pow(y0 - y1, 2) );
}

bool PUB_cutPosByDistanceFloat(float x0, float y0, float x1, float y1, float distance, float &resX, float &resY)
{
//    两点式：已知直线l上的两点P1(x1，y1)、P2(x2，y2)，(x1≠x2)
//
//    直线方程是(y-y1)/(y2-y1)=(x-x1)/(x2-x1)
//
//    也要注意两个特例：
//
//    A、当x1=x2时，直线方程是x=x1
//
//    B、当y1=y2时，直线方程是y=y1。
//
//    斜截式：已知直线l在y轴上的截距为b，斜率为b；
//
//    直线方程为y=kx+b。
    if (x1 - x0 < 1 && x1 - x0 > -1 && y1 - y0 < 1 && y1 - y0 > -1)
    {
        return false;
    }
    else if (x1 - x0 < 1 && x1 - x0 > -1)
    {
        resX = x0;
        
        if (y1 - y0 < 1 && y1 - y0 > -1)
        {
            resY = y0;
        }else if (y1 > y0)
        {
            resY = y0 + distance;
        }
        else{
            resY = y0 - distance;
        }
    }
    else if (y1 - y0 < 1 && y1 - y0 > -1)
    {
        if (x1 - x0 < 1 && x1 - x0 > -1)
        {
            resX = x0;
        }
        else if (x1 > x0)
        {
            resX = x0 + distance;
        }
        else{
            resX = x0 - distance;
        }
        resY =y0;
    }
    else{
        //两点求一般直线方程如下 ：(y2 - y1)x + (x1 - x2)y - x1(y2 - y1) + y1(x2 - x1) = 0
        // y = (y1 - y2) / (x1 - x2) * x + (x1 * (y2 - y1) - y1 * (x2 - x1)) / (x1 - x2)
        float _f_b = (x0 * (y1 - y0) - y0 * (x1 - x0)) / (x0 - x1);
        float _f_k = (y0 - y1) / (x0 - x1);
        float _f_pow2Distance = distance * distance;
        float _f_A = (pow(_f_k, 2) + 1);
        float _f_B = 2 * _f_k * (_f_b - y0) - 2 * x0;
        float _f_C = x0 * x0 + pow((_f_b - y0), 2) - _f_pow2Distance;
        float _f_delta = pow(_f_B, 2) - 4 * _f_A * _f_C;
        if (x1 > x0)
        {
            resX = (-_f_B + sqrtf(_f_delta)) / (2 * _f_A);
        }
        else{
            resX = (-_f_B - sqrtf(_f_delta)) / (2 * _f_A);
        }
        
        resY = _f_k * resX + _f_b;
    }
    return true;
}

bool PUB_cutPosByDistanceInt(float x0, float y0, float x1, float y1, float distance, int &resX, int &resY)
{
    float _f_resX, _f_resY;
    bool _b_res = PUB_cutPosByDistanceFloat(x0, y0, x1, y1, distance, _f_resX, _f_resY);
    resX = _f_resX;
    resY = _f_resY;

    return _b_res;
}

}
