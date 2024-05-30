#pragma once
#include<opencv2/opencv.hpp>
#include"compareCell.h"

//Mouse drawing box
namespace cp {

	//Mouse callback functions
	void my_mouse_callback(int event, int x, int y, int flags, void* param);
	//Mouse draws rectangular box range
	void draw_box(cv::Mat& img, cv::Rect box);
	//Show the range of colour changes of the mouse circle box
	void displayColorRange(cv::Mat& colorPictrue, const cv::Rect& box);
	//Mouse Driver
	void mouseCallBackDrive(const cv::Mat& colorPictrue, const std::string& windowName);
	//Get the area of a single pixel
	double getSinglePixelArea(const cv::Mat&colorPicture,double area/*Square control panel area (in square centimetres by default)*/, const std::string& workFolderAddress);
	//Clear Color
	void clearColor(Color&color);
	//Filter the image range of a standard template, display the results and write the results to a specified folder
	int filterStandardTemplateImageRange(const cv::Mat& picture, const Color& color, const std::string& workFolderAddress);
	//Not a standard template pixel judgement function
	bool noStantardTemplateImage(const cv::Vec3b& tempColor, const Color& color);
	//cp namespace end position
}
