#pragma once
#include "ofMain.h"

class dgCoordinate
{
public:
    dgCoordinate();

    ~dgCoordinate();
    
    void setOrigin(float x, float y);

    // 显式格间距(pixel)
    void setInterval(float inteval);

    // 设定坐标轴长度(像素)
    void setAxisLength(float xLength, float yLength);

    // 分段函数。被归一化为[0, 1]后渲染
    void drawPiecewise(vector<float> function);

    // 线性函数。
    void drawLinear(vector<float> function);

    // 坐标轴
    void drawAxis();

private:
    friend class dgDistribution1D;

    friend class dgDistribution2D;

    ofVec2f origin;

    float interval;

    float xLength, yLength;

    float xVisualLength, yVisualLength;
};
