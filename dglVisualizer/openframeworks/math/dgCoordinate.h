#pragma once
#include "ofMain.h"

class dgCoordinate
{
public:
    dgCoordinate();

    ~dgCoordinate();
    
    // �趨������ԭ��
    void setOrigin(float x, float y);

    // ��ʽ����(pixel)
    void setInterval(float inteval);

    // �趨�����᳤��(����)
    void setAxisLength(float xLength, float yLength);

    // �ֶκ���������һ��Ϊ[0, 1]����Ⱦ
    void drawPiecewise(vector<float> function);

    // ���Ժ�����
    void drawLinear(vector<float> function);

    // ������
    void drawAxis();

private:
    ofVec2f origin;

    float interval;

    float xLength, yLength;

    float xVisualLength, yVisualLength;
};