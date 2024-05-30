#pragma once
#include<vector>
#include<opencv2/opencv.hpp>
#include"fastSandSediment.h"
#include"SandSediment.h"
#define EIGHT_NEIGHBOR_AND_CENTER 9
#define FAST_ALPHA 0.4
#define FAST_BETA 5.0
#define FAST_NUMBER_IN_CELL 9
#define FAST_IS_GRAVEL_CELL_THRESHOLD 0.5
const cv::Point EIGHT_NEIGHBOR_AND_CENTER_IN_CELL[EIGHT_NEIGHBOR_AND_CENTER]
= { cv::Point(-1,-1),cv::Point(0, -1),cv::Point(1, -1),
	cv::Point(-1, 0),cv::Point(0, 0),cv::Point(1, 0),
	cv::Point(-1, 1),cv::Point(0, 1),cv::Point(1, 1)
};
//砾石分割
std::multimap<double, double> fastSandSediment(const cv::Mat& colorPicture, SeedSet& sSet, const cv::Mat& Outline, const std::string& workFaddress, double singlePixeLength, std::vector<double> &sandMinDiameter);
//扩大控制点数量
void fastExpansionPolygonPointSet(int expansionSeedNumber, const cv::Mat& oldOutline, std::vector<cv::Point>& polygonPointSet, std::vector<cv::Point>& newPositionSeed);
//在指定区域随机生成新点
void fastRandPoint(cv::RNG& seedRng, cv::RotatedRect& rotatedR, const std::vector<cv::Point>& rotatedRectFourPoint, cv::Point& position, const cv::Mat& oldOutline);
//判断一点是否在椭圆内
bool isPointInsideEllipse(const cv::Point2f& point, const cv::RotatedRect& ellipse);
//获取四邻域像素斑块
void fastGetNeighbor(const cv::Mat& colorPicture, std::vector<Cell>& firstCellSet, std::vector<Cell>& nextCellSet,  int markNumber, SeedSet& sSet);
//标记Mark矩阵
void fastMarkPointMark(cv::Mat& mark, const cv::Point& pt, int markNumber);
//得到四邻域像素斑块
void fastGetFourCellNeighbors(const cv::Mat& colorPicture, const cv::Point& pt, std::vector<Cell>& nextCellSet,cv::Mat& mark,int markNumber);
//判断像素斑块是否属于砾石的像素斑块
void fastGetGravelCellSet(Color& sandColor, const Seed& seed, const cv::Mat& colorPicture, std::vector<Cell>& cellSet,  std::vector<Cell>& nextCellSet, std::vector<cv::Point>& con_hull,  SeedSet& sSet);
//判断是否属于砾石像素斑块
double isFastGravelCell(const cv::Mat& colorPicture, Color& sandColor, const Seed& seed, const Cell& cell);
//计算属于砾石的像素数量
int calculateBelongToGravelPixelNumber(const cv::Mat& colorPicture, const Color& sandColor, const Cell& cell);
//标记mark矩阵
void fastMarkCellSetMark(cv::Mat& mark, const std::vector<Cell>& tempCellSet, int markNumber);
//返回旋转矩形
void returnRotateRect(const cv::Mat& mark, cv::RotatedRect& sandRect);