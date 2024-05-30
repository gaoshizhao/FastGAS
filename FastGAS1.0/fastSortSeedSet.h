#pragma once
#include"seedPoint.h"
#define SORT_SEED_RATE   0.5
//对种子点的质量进行排序，优先处理相对位置靠近作用域中心的种子点
void sortSeed(SeedSet& seedMergeS, const cv::Mat& outline);
//计算种子点相对作用域中心的相对位置
double calculateRelativePosition(Seed& s, const cv::Mat& outline);
// 判断矩形是否为空的函数  
bool isRectEmpty(const cv::Rect& rect);
//显示排序后的种子点
cv::Mat drawSortSeed(const cv::Mat& outline, const SeedSet& seedS, cv::Scalar s);
