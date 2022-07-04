/**
*  @file     XMath.h
*  @brief    数学公式库
*  Details.
*
*  @author   wangxinxiao
*  @email    wxx1035@163.com
*  @version  1.0.0.0
*  @date     2022/6/2
*
*  Change History :
*  <Date>     | <Version> | <Author>       | <Description>
*  2022/6/2 | 1.0.0.0  | wangxinxiao      | Create file
*-
*/

#ifndef X_PUBLIC_TOOLS_UTILS_Math_H
#define X_PUBLIC_TOOLS_UTILS_Math_H

namespace XMATHTOOL {

const float cg_f_pai = 3.1415926;

/**
* @brief 求两点间的距离
* @param 开始点：（x0, y0）结束点（x1,y1）
* @return float 距离
*/
float PUB_twoPointDistance(float x0, float y0, float x1, float y1);

/**
* @brief 求出直线的函数后，从开始点截取制定距离后的点
* 注意，精度为1 ，若两点的x与y距离小于1则，视为同一个点
* @param 开始点：（x0, y0）结束点（x1,y1）distance：需要截取的距离
* @return
*   <em> true <em> 截取成功
*   <em> false <em> 截取失败
*/
bool PUB_cutPosByDistanceFloat(float x0, float y0, float x1, float y1, float distance, float &resX, float &resY);

/**
* @brief 求出直线的函数后，从开始点截取制定距离后的点
* 注意，精度为1 ，若两点的x与y距离小于1则，视为同一个点
* @param 开始点：（x0, y0）结束点（x1,y1）distance：需要截取的距离
* @return
*   <em> true <em> 截取成功
*   <em> false <em> 截取失败
*/
bool PUB_cutPosByDistanceInt(float x0, float y0, float x1, float y1, float distance, int &resX, int &resY);

}



#endif /* X_PUBLIC_TOOLS_UTILS_Math_H */
