#pragma once
#include <iostream>
#include<vector>
#include<opencv2/opencv.hpp>
#include"SeedPoint.h"
#include"SandSediment.h"
#define OFFSET_FACTOR 0.4
#define DISCARD_FACTOR 0.3
#define END_LOOP   60
#define NUMBER_DIRECTIONS 8
const std::vector<double> angles_12 = {/* 0,*/ 30, 60, /*90,*/ 120, 150, /*180,*/ 210, 240, /*270,*/ 300, 330 };
//#define GET_PLOYGON_SIXTEEN_POINT
//�������ӵ�
SeedSet generateSeedPoint(const cv::Mat& picture, int seedNumber, const std::string& pastWorkFolder, const cv::Mat& colorPicture);
//���������߽��
void getFourPolygonPoint(cv::Point& seedP, const cv::Mat& outline, std::vector<cv::Point>& polygonPointSet);
//�ж�
bool fastIs_outline(const cv::Mat& picture, const int rows, const int cols);
bool fastIs_child_outline(const cv::Mat& child_picture);
//���ɵ����ӵ�ƫ�������������ĵ���Դ�С
double generateOffsetFactor(cv::Point&position,const cv::Mat&outline);
// �Ӹ����㷢�����߲���¼����������ĺ���  
void castRaysFromPoint(const cv::Mat& binaryImage, const cv::Point& startPoint, std::vector<cv::Point>& polygonPointSet, int numDirections = NUMBER_DIRECTIONS);
//�ж����ӵ��λ���Ƿ�Ϊ����ѩ
bool is_iceAndSnow(cv::Mat& newColorPicture,const cv::Mat& luvPicture,int row,int col);
//��ȡ12����������������
std::vector<cv::Point> getRayIntersections_12(const cv::Mat& binaryImage, const cv::Point& center);
