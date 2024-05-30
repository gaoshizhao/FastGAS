#pragma once
#include <iostream>
#include<vector>
#include<opencv2/opencv.hpp>
#include"SeedPoint.h"
#include"SandSediment.h"
#define OFFSET_FACTOR 0.6
#define END_LOOP   60
#define NUMBER_DIRECTIONS 8
//#define GET_PLOYGON_SIXTEEN_POINT
//�������ӵ�
SeedSet generateSeedPoint(const cv::Mat& picture, int seedNumber, const std::string& pastWorkFolder);
//���������߽��
void getFourPolygonPoint(cv::Point& seedP, const cv::Mat& outline, std::vector<cv::Point>& polygonPointSet);
//�ж�
bool fastIs_outline(const cv::Mat& picture, const int rows, const int cols);
bool fastIs_child_outline(const cv::Mat& child_picture);
//���ɵ����ӵ�ƫ�������������ĵ���Դ�С
double generateOffsetFactor(const cv::Point&position,const cv::Mat&outline);
// �Ӹ����㷢�����߲���¼����������ĺ���  
void castRaysFromPoint(const cv::Mat& binaryImage, const cv::Point& startPoint, std::vector<cv::Point>& polygonPointSet, int numDirections = NUMBER_DIRECTIONS);
