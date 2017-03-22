#pragma once
#include "ofMain.h"

double mult3(ofVec2f a, ofVec2f b, ofVec2f c)
{
    return (a.x - c.x)*(b.y - c.y) - (b.x - c.x)*(a.y - c.y);
}

//aa, bb为一条线段两端点 cc, dd为另一条线段的两端点 相交返回true, 不相交返回false  
bool lineIntersect(ofVec2f aa, ofVec2f bb, ofVec2f cc, ofVec2f dd)
{
    if(max(aa.x, bb.x)<min(cc.x, dd.x))
    {
        return false;
    }
    if(max(aa.y, bb.y)<min(cc.y, dd.y))
    {
        return false;
    }
    if(max(cc.x, dd.x)<min(aa.x, bb.x))
    {
        return false;
    }
    if(max(cc.y, dd.y)<min(aa.y, bb.y))
    {
        return false;
    }
    if(mult3(cc, bb, aa)*mult3(bb, dd, aa)<0)
    {
        return false;
    }
    if(mult3(aa, dd, cc)*mult3(dd, bb, cc)<0)
    {
        return false;
    }
    return true;
}

// 线段相交测试并求交点
bool segmentsIntersect(ofVec2f& result, ofVec2f a, ofVec2f b, ofVec2f c, ofVec2f d)
{

    // 三角形abc 面积的2倍  
    float area_abc = (a.x - c.x) * (b.y - c.y) - (a.y - c.y) * (b.x - c.x);

    // 三角形abd 面积的2倍  
    float area_abd = (a.x - d.x) * (b.y - d.y) - (a.y - d.y) * (b.x - d.x);

    // 面积符号相同则两点在线段同侧,不相交 (对点在线段上的情况,本例当作不相交处理);  
    if(area_abc * area_abd >= 0)
    {
        return false;
    }

    // 三角形cda 面积的2倍  
    float area_cda = (c.x - a.x) * (d.y - a.y) - (c.y - a.y) * (d.x - a.x);
    // 三角形cdb 面积的2倍  
    // 注意: 这里有一个小优化.不需要再用公式计算面积,而是通过已知的三个面积加减得出.  
    float area_cdb = area_cda + area_abc - area_abd;
    if(area_cda * area_cdb >= 0)
    {
        return false;
    }

    //计算交点坐标  
    float t = area_cda / (area_abd - area_abc);
    float dx = t*(b.x - a.x),
        dy = t*(b.y - a.y);

    result.x = a.x + dx;
    result.y = a.y + dy;

    return true;
}
