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
// �Զ����Բ��ָ����㺯��  
float calculateRadiusConsistency(const cv::Mat& edges, float cx, float cy, float radius);
// ����ͼ����Բ�����������������ĺ���  
int countPixelsInCircle(const cv::Mat& image, float cx, float cy, float radius);
//������ʾ���ͼ��
void  showLargeImage(const cv::Mat&image);
