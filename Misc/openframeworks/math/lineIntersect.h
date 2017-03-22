#pragma once
#include "ofMain.h"

double mult(ofVec2f a, ofVec2f b, ofVec2f c)
{
    return (a.x - c.x)*(b.y - c.y) - (b.x - c.x)*(a.y - c.y);
}

//aa, bb为一条线段两端点 cc, dd为另一条线段的两端点 相交返回true, 不相交返回false  
bool intersect(ofVec2f aa, ofVec2f bb, ofVec2f cc, ofVec2f dd)
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
    if(mult(cc, bb, aa)*mult(bb, dd, aa)<0)
    {
        return false;
    }
    if(mult(aa, dd, cc)*mult(dd, bb, cc)<0)
    {
        return false;
    }
    return true;
}
