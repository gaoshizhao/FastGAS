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
//生成种子点
SeedSet generateSeedPoint(const cv::Mat& picture, int seedNumber, const std::string& pastWorkFolder, const cv::Mat& colorPicture);
//获得四邻域边界点
void getFourPolygonPoint(cv::Point& seedP, const cv::Mat& outline, std::vector<cv::Point>& polygonPointSet);
//判断
bool fastIs_outline(const cv::Mat& picture, const int rows, const int cols);
bool fastIs_child_outline(const cv::Mat& child_picture);
//生成的种子点偏移其左右域中心的相对大小
double generateOffsetFactor(cv::Point&position,const cv::Mat&outline);
// 从给定点发射射线并记录与轮廓交点的函数  
void castRaysFromPoint(const cv::Mat& binaryImage, const cv::Point& startPoint, std::vector<cv::Point>& polygonPointSet, int numDirections = NUMBER_DIRECTIONS);
//判断种子点的位置是否为冰或雪
bool is_iceAndSnow(cv::Mat& newColorPicture,const cv::Mat& luvPicture,int row,int col);
//获取12条射线与轮廓交点
std::vector<cv::Point> getRayIntersections_12(const cv::Mat& binaryImage, const cv::Point& center);
