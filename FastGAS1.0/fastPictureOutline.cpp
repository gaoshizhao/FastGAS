#pragma once
#include <direct.h>
#include <io.h>
#include "fastPictureOutline.h"
#include"fastSandSediment.h"
#include"fastSandDataDispose.h"
#define KERNEL_CORE 3
#define CLASSIFY_MORPHOLOGY 2
#define CANNY_UP_THRESHOLD  255
#define CANNY_DOWN_THRESHOLD 5
#define NEED_INPUT_CANNY_UP_AND_DOWN_THRESHOLD
#define MAX_OUTLINE_PIXELS_NUMBER 3000 
#define MAX_SAND_PIXELS_NUMBER 3000
const cv::Point EIGHT_POINT_NEIGHBOR[EIGHT_NEIGHBOR]
= { cv::Point(1,0),cv::Point(1,-1),cv::Point(0,-1),cv::Point(-1,-1),cv::Point(-1,0),cv::Point(-1,1),cv::Point(0,1) , cv::Point(1,1) };
cv::Mat Outline(const cv::Mat& picture, std::string address
#ifdef OUTLINE_METHOD
	, const int outlineMethod
#endif
)
{
	cv::Mat Blur_picture;
	cv::GaussianBlur(picture, Blur_picture, cv::Size(5, 5), 1);
	cv::Mat Gray_picture;
	cv::cvtColor(Blur_picture, Gray_picture, cv::COLOR_BGR2GRAY);
	cv::Mat Bi_picture, Bi_Blur, Ada_Bi_Blur, Otu_Bi_Blur, Canny_picture, tempM, newTempM;
#ifdef MORPHOLOGY
	do
	{
#endif
		thresholdMethod(Gray_picture, Bi_picture);
#ifdef MORPHOLOGY
		morphologyAndCanny(Bi_picture);
#else
		morphologyAndMethod(Bi_picture, CLASSIFY_MORPHOLOGY, 1, false);
		writePicture(Bi_picture, "thresh.jpg", address);
#endif
#ifdef NEED_INPUT_CANNY_UP_AND_DOWN_THRESHOLD
		do
		{
#endif
			int up = CANNY_UP_THRESHOLD, down = CANNY_DOWN_THRESHOLD;
#ifdef NEED_INPUT_CANNY_UP_AND_DOWN_THRESHOLD
#ifndef FEATURE_GRAIN
			do {
				std::cout << "Please enter the upper threshold (0-255) for the Canny profile detection method!";
				std::cin >> up;
				std::cout << "Please enter the lower threshold value (0-255) for the Canny profile detection method!";
				std::cin >> down;
				if (up > 255 || up <= 0 || down > 255 || down <= 0 || down >= up)
				{
					std::cout << "The value entered does not match the requirement! Please re-enter!" << std::endl;
				}
				else
				{
					break;
				}

			} while (true);
#endif
#ifdef FEATURE_GRAIN
			up = can1;
			down = can2;
#endif
#endif
			cv::Canny(Gray_picture, Canny_picture, down, up);
#ifdef MORPHOLOGY
			morphologyAndCanny(Canny_picture);
#else
			morphologyAndMethod(Canny_picture, CLASSIFY_MORPHOLOGY, 3, true);
#ifdef NEED_INPUT_CANNY_UP_AND_DOWN_THRESHOLD
			tempM = mergeThresholdAndCannyMethod(Bi_picture, Canny_picture);
#ifndef FEATURE_GRAIN
			cv::namedWindow("Canny_picture", cv::WINDOW_NORMAL);
			cv::imshow("Canny_picture", tempM);
			cv::waitKey(5000);
			std::cout << "Does the threshold on Canny satisfy the requirement (y/n)?" << std::endl;
			char ch;
			std::cin >> ch;
			if (ch == 'y' || ch == 'Y')
			{
				break;
			}
#endif
#ifdef FEATURE_GRAIN
			break;
#endif
#endif
#endif
#ifdef NEED_INPUT_CANNY_UP_AND_DOWN_THRESHOLD
		} while (true);
#endif
		writePicture(Canny_picture, "Canny.jpg", address);
		writePicture(tempM, "merge.jpg", address);
		tempM = deleteUntrustOutlines(tempM, 255, MAX_OUTLINE_PIXELS_NUMBER);
		writePicture(tempM, "merge_delete_outline.jpg", address);
		tempM = deleteUntrustOutlines(tempM, 0, MAX_SAND_PIXELS_NUMBER);
		writePicture(tempM, "merge_delete_sand.jpg", address);
		cv::namedWindow("Canny_picture", cv::WINDOW_NORMAL);
		cv::imshow("Canny_picture", tempM);
		cv::waitKey(5000);
#ifdef MORPHOLOGY
		morphologyAndCanny(tempM);
#else
		morphologyAndMethod(tempM, CLASSIFY_MORPHOLOGY, 1, false);
#endif
		cv::namedWindow("Canny_picture", cv::WINDOW_NORMAL);
		cv::imshow("Canny_picture", tempM);
		cv::waitKey(5000);
#ifdef MORPHOLOGY
		std::cout << "输入轮廓图像是否满足要求，如果满足要求请输入y，如果需要更改阈值请输入n！";
		char ch;
		std::cin >> ch;
		if (ch == 'y')
		{
			cv::destroyAllWindows();
			return tempM;
		}
	} while (true);
#else
		cv::destroyAllWindows();
		return tempM;
#endif
};
	cv::Mat Scharr(const cv::Mat& picture)
	{
		cv::Mat Blur_picture;
		cv::GaussianBlur(picture, Blur_picture, cv::Size(5, 5), 1);
		cv::Mat Gray_picture;
		cv::cvtColor(Blur_picture, Gray_picture, cv::COLOR_BGR2GRAY);
		cv::Mat ScharrX_picture, ScharrY_picture, Scharr_picture;
		cv::Sobel(Gray_picture, ScharrX_picture, CV_8U, 1, 0, CV_SCHARR);
		cv::Sobel(Gray_picture, ScharrY_picture, CV_8U, 0, 1, CV_SCHARR);
		Scharr_picture = ScharrX_picture + ScharrY_picture;
		return Scharr_picture;
	};
	void morphologyAndCanny(cv::Mat& Canny_picture)
	{
		int morphologyNumber = 1;
		int classifyN = 0;
		int reverseColor = 0;
		int kernelC = 3;
		int dilateN;
		int erodeN;
		cv::Mat tempM, tempM_1;
		cv::Point center = cv::Point(-1, -1);
		do {
			do {
				std::cout << "Please enter the size of the kernel matrix for the morphological operations:" << std::endl;
				std::cin >> kernelC;
				if (kernelC >= 3)
				{
					break;
				}
				std::cout << "The size of the input kernel matrix is incorrect!" << std::endl;
			} while (true);
			cv::Mat Kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(kernelC, kernelC));
			do {
				std::cout << "Please enter the type of morphological operation:" << std::endl;
				std::cout << "0: indicates swell operation + open operation" << std::endl;
				std::cout << "1: indicates corrosion operation + closed operation" << std::endl;
				std::cout << "2: indicates swelling operation" << std::endl;
				std::cout << "3: Indicates corrosion operations" << std::endl;
				std::cout << "4: indicates expansion operation + corrosion operation" << std::endl;
				std::cout << "5: indicates corrosion operation + expansion operation" << std::endl;
				std::cin >> classifyN;
				if (classifyN == 0 || classifyN == 1 || classifyN == 2 || classifyN == 3 || classifyN == 4 || classifyN == 5)
				{
					break;
				}
				else
				{
					std::cout << "The value entered does not match the requirement!";
				}
			} while (true);
			if (classifyN == 0)
			{
				do {
					std::cout << "Please enter the number of open operations:" << std::endl;
					std::cin >> morphologyNumber;
					if (morphologyNumber >= 0)
					{
						break;
					}
					else
					{
						std::cout << "The value entered does not match the requirement!";
					}
				} while (true);
				do {
					std::cout << "Please enter the number of expansion operations:" << std::endl;
					std::cin >> dilateN;
					if (dilateN >= 1)
					{
						break;
					}
					else
					{
						std::cout << "The value entered does not match the value requested!";
					}
				} while (true);
			}
			if (classifyN == 1)
			{
				do {
					std::cout << "Please enter the number of closed operations:" << std::endl;
					std::cin >> morphologyNumber;
					if (morphologyNumber >= 0)
					{
						break;
					}
					else
					{
						std::cout << "The value entered does not match the requirement!";
					}
				} while (true);
				do {
					std::cout << "Please enter the number of corrosion operations:" << std::endl;
					std::cin >> erodeN;
					if (erodeN >= 1)
					{
						break;
					}
					else
					{
						std::cout << "The value entered does not match the requirement!";
					}
				} while (true);
			}
			else if (classifyN == 2)
			{
				do {
					std::cout << "Please enter the number of expansion operations:" << std::endl;
					std::cin >> dilateN;
					if (dilateN >= 1)
					{
						break;
					}
					else
					{
						std::cout << "The value entered does not match the requirement!";
					}
				} while (true);
			}
			else if (classifyN == 3)
			{
				do {
					std::cout << "Please enter the number of corrosion operations:" << std::endl;
					std::cin >> erodeN;
					if (erodeN >= 1)
					{
						break;
					}
					else
					{
						std::cout << "The value entered does not match the requirement!";
					}
				} while (true);
			}
			else if (classifyN == 4)
			{
				do {
					std::cout << "Please enter the number of expansion operations:" << std::endl;
					std::cin >> dilateN;
					if (dilateN >= 1)
					{
						break;
					}
					else
					{
						std::cout << "The value entered does not match the requirement!";
					}
				} while (true);
				do {
					std::cout << "Please enter the number of corrosion operations:" << std::endl;
					std::cin >> erodeN;
					if (erodeN >= 1)
					{
						break;
					}
					else
					{
						std::cout << "The value entered does not match the requirement!";
					}
				} while (true);
			}
			else if (classifyN == 5)
			{
				do {
					std::cout << "Please enter the number of expansion operations:" << std::endl;
					std::cin >> dilateN;
					if (dilateN >= 1)
					{
						break;
					}
					else
					{
						std::cout << "The value entered does not match the requirement!";
					}
				} while (true);
				do {
					std::cout << "Please enter the number of corrosion operations:" << std::endl;
					std::cin >> erodeN;
					if (erodeN >= 1)
					{
						break;
					}
					else
					{
						std::cout << "The value entered does not match the requirement!";
					}
				} while (true);
			}
			if (morphologyNumber > 0)
			{
				if (classifyN == 0)
				{
					cv::dilate(Canny_picture, tempM_1, Kernel, center, dilateN);
					cv::morphologyEx(tempM_1, tempM, cv::MORPH_OPEN, Kernel, center, morphologyNumber);
				}
				else if (classifyN == 1)
				{
					cv::erode(Canny_picture, tempM_1, Kernel, center, erodeN);
					cv::morphologyEx(tempM_1, tempM, cv::MORPH_CLOSE, Kernel, center, morphologyNumber);
				}
				else if (classifyN == 2)
				{
					cv::dilate(Canny_picture, tempM, Kernel, center, dilateN);
				}
				else if (classifyN == 3)
				{
					cv::erode(Canny_picture, tempM, Kernel, center, erodeN);
				}
				else if (classifyN == 4)
				{
					cv::dilate(Canny_picture, tempM_1, Kernel, center, dilateN);
					cv::erode(tempM_1, tempM, Kernel, center, erodeN);
				}
				else if (classifyN == 5)
				{
					cv::erode(Canny_picture, tempM_1, Kernel, center, erodeN);
					cv::dilate(tempM_1, tempM, Kernel, center, dilateN);
				}
			}
			else
			{
				Canny_picture.copyTo(tempM);
			}
			cv::namedWindow("morphologyPicture", cv::WINDOW_NORMAL);
			cv::imshow("morphologyPicture", tempM);
			cv::waitKey(5000);
			std::cout << "Whether the morphological operation meets the requirements, enter y if it does, or n if the threshold needs to be changed!";
			char ch;
			std::cin >> ch;
			if (ch == 'y')
			{
				cv::destroyWindow("morphologyPicture");
				break;
			}
		} while (true);
		std::cout << "Does it need to be counter-coloured? (y/n)";
		char ch;
		std::cin >> ch;
		if (ch == 'y')
		{
			do
			{
				cv::threshold(tempM, Canny_picture, 40, 255, cv::THRESH_BINARY_INV);
				cv::namedWindow("Canny_picturePicture", cv::WINDOW_NORMAL);
				cv::imshow("Canny_picturePicture", Canny_picture);
				cv::waitKey(5000);
				std::cout << "Does the inverse colour operation meet the requirements, enter y if it does, or n if you need to change the threshold!";
				char ch;
				std::cin >> ch;
				if (ch == 'y')
				{
					cv::destroyWindow("Canny_picturePicture");
					break;
				}
			} while (true);
		}
		else
		{
			tempM.copyTo(Canny_picture);
		}
	};
	void thresholdMethod(const cv::Mat& Gray_picture, cv::Mat& Bi_picture)
	{
#ifndef FEATURE_GRAIN
		do {
			int value;
			do {
				std::cout << "Please enter the image profile threshold (an integer between 0 and 255, you can start with a value between 15 and 140 for testing):";
				std::cin >> value;
				if (value > 1 && value <= 255)
				{
					break;
				}
				else
				{
					std::cout << "The value entered does not match the requirement!";
				}
			} while (true);
#endif
#ifdef FEATURE_GRAIN
			int value = gre;
#endif
			cv::threshold(Gray_picture, Bi_picture, value, 255, cv::THRESH_BINARY);
#ifndef FEATURE_GRAIN
			cv::namedWindow("outlinePicture", cv::WINDOW_FREERATIO);
			cv::imshow("outlinePicture", Bi_picture);
			cv::waitKey(5000);
			std::cout << "Enter whether the contour image meets the requirements, enter y if it does, or n if you need to change the threshold!";
			char ch;
			std::cin >> ch;
			if (ch == 'y')
			{
				break;
			}
		} while (true);
#endif
	};
	cv::Mat mergeThresholdAndCannyMethod(cv::Mat& Bi_picture, cv::Mat& Canny_picture)
	{
		if (Bi_picture.size() != Canny_picture.size())
		{
			std::cout << "The two input sand boundary images do not match in size!" << std::endl;
			exit(-1);
		}
		cv::Mat outlineP = cv::Mat::zeros(Bi_picture.size(), CV_8UC1);
		for (int i = 0; i < Bi_picture.rows; i++)
		{
			for (int j = 0; j < Bi_picture.cols; j++)
			{
				if ((Bi_picture.at<uchar>(i, j) == 255) && (Canny_picture.at<uchar>(i, j) == 255))
				{
					outlineP.at<uchar>(i, j) = 255;
				}
			}
		}
		return outlineP;
	};
	void morphologyAndMethod(cv::Mat& Canny_picture, int morphologyC, int morphologyN, bool isReverseColor)
	{
		int morphologyNumber = morphologyN;
		int classifyN = morphologyC;
		int kernelC = KERNEL_CORE;
		cv::Mat tempM, tempM_1;
		cv::Point center = cv::Point(-1, -1);
		cv::Mat Kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(kernelC, kernelC));
		cv::dilate(Canny_picture, tempM, Kernel, center, morphologyN);
		cv::namedWindow("morphologyPicture", cv::WINDOW_NORMAL);
		cv::imshow("morphologyPicture", tempM);
		cv::waitKey(4000);
		cv::destroyWindow("Canny_picturePicture");
		if (isReverseColor)
		{
			cv::threshold(tempM, Canny_picture, 40, 255, cv::THRESH_BINARY_INV);
			cv::namedWindow("Canny_picturePicture", cv::WINDOW_NORMAL);
			cv::imshow("Canny_picturePicture", Canny_picture);
			cv::waitKey(4000);
			cv::destroyWindow("Canny_picturePicture");
		}
	};
	void writePicture(const cv::Mat& mark, const std::string& name, const std::string address)
	{
		std::string PictureAddress = address + "newOutPicture\\";
#ifdef GET_NEIGHBOR_FIRST
		address_global = sandAreaAddress;
#endif
		if (0 != ::_access(PictureAddress.c_str(), 0))
		{
			::_mkdir(PictureAddress.c_str());
		}
		PictureAddress += "outlineP\\";
		if (0 != ::_access(PictureAddress.c_str(), 0))
		{
			::_mkdir(PictureAddress.c_str());
		}
		PictureAddress += name;
		cv::imwrite(PictureAddress, mark);
	};
	cv::Mat deleteUntrustOutlines(cv::Mat& tempM, int outlineOrSand, int maxUntrustNumber)
	{
		cv::Mat mark = cv::Mat::zeros(tempM.rows, tempM.cols, CV_8UC1);
		cv::Mat outline = cv::Mat(tempM.rows, tempM.cols, CV_8UC1, cv::Scalar(outlineOrSand));
		for (int col = 0; col < tempM.cols; col++)
		{
			for (int row = 0; row < tempM.rows; row++)
			{
				cv::Point pt(col, row);
				std::vector<cv::Point> outSet;
				if (mark.at<uchar>(pt) == 1)
				{
					continue;
				}
				else if (tempM.at<uchar>(pt) == outlineOrSand)
				{
					mark.at<uchar>(pt) = 1;
					continue;
				}
				else
				{
					searchOutlines(tempM, mark, pt, outSet, outlineOrSand);
				}
				if (outSet.size() >= maxUntrustNumber)
				{
					for (int i = 0; i != outSet.size(); i++)
					{
						outline.at<uchar>(outSet[i]) = 255 - outlineOrSand;
					}
				}
				outSet.clear();
			}
		}
		return outline;
	};
	void searchOutlines(cv::Mat& tempM, cv::Mat& mark, const cv::Point& pt, std::vector<cv::Point>& outSet, int outlineOrSand)
	{
		std::vector<cv::Point> firstSet, nextSet;
		firstSet.push_back(pt);
		do
		{
			nextSet.clear();

			for (int i = 0; i < firstSet.size(); i++)
			{
				outSet.push_back(firstSet[i]);
			}
			for (int i = 0; i < firstSet.size(); i++)
			{
				cv::Point pt = firstSet[i];
				std::vector<cv::Point> eightNeighbor;
				getEightNeighbors(pt, eightNeighbor, mark);
				for (int k = 0; k < eightNeighbor.size(); k++)
				{
					if (mark.at<uchar>(eightNeighbor[k]) == 0)
					{
						mark.at<uchar>(eightNeighbor[k]) = 1;
						if (tempM.at<uchar>(eightNeighbor[k]) == 255 - outlineOrSand)
						{
							nextSet.push_back(eightNeighbor[k]);
						}
					}
				}
			}
			firstSet.clear();
			firstSet = nextSet;
		} while (nextSet.size() != 0);
	};
	void getEightNeighbors(const cv::Point& pt, std::vector<cv::Point>& eightNeighbor, const cv::Mat& mark)
	{
		int x = mark.cols;
		int y = mark.rows;
		for (int i = 0; i < EIGHT_NEIGHBOR; i++)
		{
			cv::Point nPt = pt + EIGHT_POINT_NEIGHBOR[i];
			if (nPt.x >= 0 && nPt.x < x && nPt.y >= 0 && nPt.y < y)
			{
				eightNeighbor.push_back(nPt);
			}

		}
	};


