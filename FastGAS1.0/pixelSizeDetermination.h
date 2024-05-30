#pragma once
#include<opencv2/opencv.hpp>
#include <iostream>
#include <iostream>  
#include <vector>  
#include <cmath>
#define AUTO_RECOGNITION
#define TEST
////#define HOUGH_CIRCLES
////Obtain single pixel size
double getCircleSinglePixelArea(cv::Mat Image, double circleArea, std::string workFaddress);
// 自定义的圆度指标计算函数  
float calculateRadiusConsistency(const cv::Mat& edges, float cx, float cy, float radius);
// 计算图像中圆形区域内像素数量的函数  
int countPixelsInCircle(const cv::Mat& image, float cx, float cy, float radius);
//缩放显示大幅图像
void  showLargeImage(const cv::Mat&image);
