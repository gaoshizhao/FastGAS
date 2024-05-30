#pragma once
#include <iostream>
#include<vector>
#include<opencv2/opencv.hpp>

#define LONG 21//Length of square patches
cv::Mat Outline(const cv::Mat& picture, std::string address
#ifdef OUTLINE_METHOD	
	, const int outlineMethod
#endif
);
cv::Mat Scharr(const cv::Mat& picture);
//
struct RectSpot
{
private:
	cv::Point position;
	cv::Vec3b color;
	cv::Size size;
	cv::Mat rectSpot;
public:
	RectSpot() :position(0, 0), color(0, 0, 0), size(0, 0)
		, rectSpot(cv::Size(LONG, LONG), CV_8UC3, cv::Scalar(0, 0, 0))
	{};
	RectSpot(const cv::Mat& spot, cv::Point p) :position(p), size(LONG, LONG)
	{
		spot.copyTo(rectSpot);
		for (int i = 0; i < 3; i++)
			color[i] = (uchar)(cv::sum(rectSpot)[i] / (LONG * LONG));
	};

};
//
struct RectS_Mat
{
	cv::Mat Ori_picture;
	cv::Mat Scharr_picture;
	cv::Size size;
	std::vector<RectSpot> Ori_vector;
	std::vector<RectSpot> Sch_vector;
	RectS_Mat() = delete;
	RectS_Mat(const cv::Mat& Ori_picture) :Ori_picture(Ori_picture),
		Scharr_picture(Scharr(Ori_picture)), size(Ori_picture.cols, Ori_picture.rows)
	{

	};

};
//Perform morphological operations and edge detection operations
void morphologyAndCanny(cv::Mat& Canny_picture);
//Binary image processing method to obtain contour images
void thresholdMethod(const cv::Mat& Gray_picture, cv::Mat& Bi_picture);
//Results of merging threshold method bounds and Canny method bounds
cv::Mat mergeThresholdAndCannyMethod(cv::Mat& Bi_picture, cv::Mat& Canny_picture);
//Perform morphological processing
void morphologyAndMethod(cv::Mat& Canny_picture, int morphologyC, int morphologyN, bool isReverseColor);
//Output images
void writePicture(const cv::Mat& mark, const std::string& name, const std::string address);
//Noise rejection
cv::Mat deleteUntrustOutlines(cv::Mat& tempM, int outlineOrSand, int maxUntrustNumber);
//Search for the edge of the rubble at the designated point of exit
void searchOutlines(cv::Mat& tempM, cv::Mat& mark, const cv::Point& pt, std::vector<cv::Point>& outSet, int outlineOrSand);
//Eight neighbourhood points at past designated points
void getEightNeighbors(const cv::Point& pt, std::vector<cv::Point>& eightNeighbor, const cv::Mat& mark);


