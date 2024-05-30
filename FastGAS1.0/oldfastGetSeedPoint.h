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
//生成种子点
SeedSet generateSeedPoint(const cv::Mat& picture, int seedNumber, const std::string& pastWorkFolder);
//获得四邻域边界点
void getFourPolygonPoint(cv::Point& seedP, const cv::Mat& outline, std::vector<cv::Point>& polygonPointSet);
//判断
bool fastIs_outline(const cv::Mat& picture, const int rows, const int cols);
bool fastIs_child_outline(const cv::Mat& child_picture);
//生成的种子点偏移其左右域中心的相对大小
double generateOffsetFactor(const cv::Point&position,const cv::Mat&outline);
// 从给定点发射射线并记录与轮廓交点的函数  
void castRaysFromPoint(const cv::Mat& binaryImage, const cv::Point& startPoint, std::vector<cv::Point>& polygonPointSet, int numDirections = NUMBER_DIRECTIONS);
