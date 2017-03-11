#pragma once

#include "ofMain.h"

class dglAnimation
{
public:
	dglAnimation();

	//直接读入一整个XXX文件夹的帧动画图片(文件夹位于of工程的bin/data内) 从-1开始直到-sum为止
	//需要命名方式为"XXX-1.png/bmp..."等等
	//文件夹名 后缀名(所有文件的格式都需要统一为同一格式) 帧数(文件个数)
	void loadFrames(string folderName, string suffixFormat, int sumNumber);

	//添加帧动画图
	void addFrame(ofImage image);

	//在指定位置绘制当前帧
	void drawCurrentFrame(ofVec3f position);

	//帧动画统一宽高
	int width, height;

	//当前帧
	int currentFrame;
	//总帧数
	int sumFrames;

	//帧动画集
	vector<ofImage> pictures;
};