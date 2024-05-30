#pragma once
#include <direct.h>
#include <io.h>
#include "PictureOutline.h"
#include"SandSediment.h"
#include"sandDataDispose.h"
#define KERNEL_CORE 3
#define CLASSIFY_MORPHOLOGY 2
#define CANNY_UP_THRESHOLD  255
#define CANNY_DOWN_THRESHOLD 5
#define NEED_INPUT_CANNY_UP_AND_DOWN_THRESHOLD
#define MAX_OUTLINE_PIXELS_NUMBER 50 
#define MAX_OUTLINE_PIXELS_NUMBER_LARGE 500 
#define MAX_OUTLINE_PIXELS_NUMBER_THRESHOLD 600 
#define MAX_SAND_PIXELS_NUMBER 500
#define LOW_THRESHOLD 160
#define HIGH_THRESHOLD 255
#define K 0.01
#define TEST_PHRASE
const cv::Point EIGHT_POINT_NEIGHBOR[EIGHT_NEIGHBOR]
= { cv::Point(1,0),cv::Point(1,-1),cv::Point(0,-1),cv::Point(-1,-1),cv::Point(-1,0),cv::Point(-1,1),cv::Point(0,1) , cv::Point(1,1) };
cv::Mat Outline(const cv::Mat& picture, std::string address)
{
	cv::Mat  equalizedImage, grayImage;
	cv::cvtColor(picture, grayImage, cv::COLOR_BGR2GRAY);
	// 对图像进行直方图均衡化  
	cv::equalizeHist(grayImage, equalizedImage);
	cv::Mat Bi_picture,Canny_picture,tempM;
    thresholdMethod(equalizedImage, Bi_picture);
	morphologyAndMethod( Bi_picture, CLASSIFY_MORPHOLOGY,1,false);
#ifdef TEST_PHRASE
	writePicture(Bi_picture, "thresh.jpg", address);
#endif
	//确定Canny法上下阈值
	cv::Mat gray = grayImage.clone();
	double otsuThreshold = cv::threshold(gray, gray, 0, 255, cv::THRESH_BINARY + cv::THRESH_OTSU);
	double lowThreshold = otsuThreshold * 0.2;
	double highThreshold = otsuThreshold*1.5;
	if (lowThreshold <LOW_THRESHOLD )
	{
		lowThreshold = LOW_THRESHOLD;
	}
	else if (highThreshold>HIGH_THRESHOLD)
	{
		highThreshold = HIGH_THRESHOLD;
	}
	cv::Canny(grayImage, Canny_picture, lowThreshold, highThreshold);
	Canny_picture = deleteUntrustOutlines(Canny_picture, 0, MAX_OUTLINE_PIXELS_NUMBER);
	morphologyAndMethod(Canny_picture, CLASSIFY_MORPHOLOGY, 3, true);
	tempM = mergeThresholdAndCannyMethod(Bi_picture, Canny_picture);
#ifdef TEST_PHRASE
	writePicture(Canny_picture, "Canny.jpg", address);
	writePicture(tempM, "merge.jpg", address);
#endif
	tempM = deleteUntrustOutlines(tempM,255, MAX_OUTLINE_PIXELS_NUMBER_LARGE);
#ifdef TEST_PHRASE
	writePicture(tempM, "merge_delete_outline.jpg", address);
#endif
	//tempM = deleteUntrustOutlines(tempM, 0, MAX_SAND_PIXELS_NUMBER);
#ifdef TEST_PHRASE
	//writePicture(tempM, "merge_delete_sand.jpg", address);
#endif
	morphologyAndMethod(tempM, CLASSIFY_MORPHOLOGY, 1, false);

	return tempM;
};
cv::Mat Scharr(const cv::Mat& picture)
{
	cv::Mat Blur_picture;
	cv::GaussianBlur(picture, Blur_picture, cv::Size(5, 5), 1);
	cv::Mat Gray_picture;
	cv::cvtColor(Blur_picture, Gray_picture, cv::COLOR_BGR2GRAY);
	cv::Mat ScharrX_picture, ScharrY_picture,Scharr_picture;
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
	cv::Mat tempM,tempM_1;
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
			std::cout << "Please enter the type of morphological operation:"<<std::endl;
			std::cout << "0: indicates swell operation + open operation" << std::endl;
			std::cout << "1: indicates corrosion operation + closed operation" << std::endl;
			std::cout << "2: indicates swelling operation" << std::endl;
			std::cout << "3: Indicates corrosion operations" << std::endl;
			std::cout << "4: indicates expansion operation + corrosion operation" << std::endl;
			std::cout << "5: indicates corrosion operation + expansion operation" << std::endl;
			std::cin >> classifyN;
			if (classifyN == 0|| classifyN == 1|| classifyN == 2|| classifyN == 3 || classifyN == 4 || classifyN == 5)
			{
				break;
			}
			else
			{
				std::cout << "The value entered does not match the requirement!";
			}
		} while (true);
		if (classifyN==0)
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
		}else if (classifyN == 2)
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
		if (morphologyNumber>0)
		{
			if (classifyN == 0)
			{
				cv::dilate(Canny_picture, tempM_1, Kernel, center,dilateN);
				cv::morphologyEx(tempM_1, tempM, cv::MORPH_OPEN, Kernel, center, morphologyNumber);
			}
			else if(classifyN == 1)
			{
				cv::erode(Canny_picture, tempM_1, Kernel, center, erodeN);
				cv::morphologyEx(tempM_1, tempM, cv::MORPH_CLOSE, Kernel, center, morphologyNumber);
			}
			else if (classifyN == 2)
			{
				cv::dilate(Canny_picture, tempM,  Kernel, center, dilateN);
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
		}else
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
	Bi_picture = localThreshold(Gray_picture, 30, K);	
	int kernelC = KERNEL_CORE;
	cv::Mat tempM;
	cv::Point center = cv::Point(-1, -1);
	cv::Mat Kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(kernelC, kernelC));
	cv::dilate(Bi_picture, tempM, Kernel, center, CLASSIFY_MORPHOLOGY);
	Bi_picture = deleteUntrustOutlines(tempM, 255, MAX_OUTLINE_PIXELS_NUMBER_THRESHOLD);
};
// 自定义的局部阈值函数  
cv::Mat localThreshold(const cv::Mat& src, int windowSize, double k)
{
	cv::Mat dst = cv::Mat::zeros(src.size(), src.type());
	int halfWindowSize = windowSize / 2;
	cv::Mat window;
	for (int i = 0; i < src.rows; i++) 
	{
		for (int j = 0; j < src.cols; j++) 
		{
			int row_u= i - halfWindowSize, row_d= i + halfWindowSize + 1, col_l= j - halfWindowSize, col_r= j + halfWindowSize + 1;
			if (row_u<0)
			{
				row_u = 0;
				row_d = halfWindowSize*2 + 1;
			}
			else if(row_d>src.rows)
			{
				row_u = src.rows - halfWindowSize * 2;
				row_d = src.rows -1 ;
			}

			if (col_l < 0)
			{
				col_l = 0;
				col_r = halfWindowSize * 2 + 1;
			}
			else if (col_r > src.cols)
			{
				col_r = src.cols -1;
				col_l = src.cols - halfWindowSize * 2;
			}
			window = src(cv::Range(row_u, row_d),cv::Range(col_l, col_r));

			cv::Scalar mean, stddev;
			meanStdDev(window, mean, stddev);
			double threshold = mean[0] + k * stddev[0];

			if (src.at<uchar>(i, j) > threshold) {
				dst.at<uchar>(i, j) = 255;
			}
			else {
				dst.at<uchar>(i, j) = 0;
			}
		}
	}
	return dst;
};

cv::Mat mergeThresholdAndCannyMethod(cv::Mat & Bi_picture, cv::Mat & Canny_picture)
{
	if (Bi_picture.size()!=Canny_picture.size())
	{
		std::cout << "The two input sand boundary images do not match in size!" << std::endl;
		exit(-1);
	}
	cv::Mat outlineP = cv::Mat::zeros(Bi_picture.size(),CV_8UC1);
	for (int i = 0; i < Bi_picture.rows; i++)
	{
		for (int j=0;j<Bi_picture.cols;j++)
		{
			if ((Bi_picture.at<uchar>(i, j) == 255) && (Canny_picture.at<uchar>(i, j) == 255))
			{
				outlineP.at<uchar>(i, j) = 255;
			}
		}
	}
	return outlineP;
};
void morphologyAndMethod(cv::Mat& Canny_picture,int morphologyC,int morphologyN,bool isReverseColor)
{
	int morphologyNumber = morphologyN;
	int classifyN = morphologyC;
	int kernelC = KERNEL_CORE;
	cv::Mat tempM, tempM_1;
	cv::Point center = cv::Point(-1, -1);	
	cv::Mat Kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(kernelC, kernelC));
    cv::dilate(Canny_picture, tempM, Kernel, center, morphologyN);
	if (isReverseColor)
	{
		cv::threshold(tempM, Canny_picture, 20, 255, cv::THRESH_BINARY_INV);
		//cv::namedWindow("Canny_picturePicture", cv::WINDOW_NORMAL);
		//cv::imshow("Canny_picturePicture", Canny_picture);
		//cv::waitKey(4000);
		//cv::destroyWindow("Canny_picturePicture");
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
cv::Mat deleteUntrustOutlines(cv::Mat& tempM,int outlineOrSand,int maxUntrustNumber)
{
	cv::Mat mark = cv::Mat::zeros(tempM.rows,tempM.cols,CV_8UC1);
	cv::Mat outline=cv::Mat(tempM.rows, tempM.cols, CV_8UC1,cv::Scalar(outlineOrSand));
	for (int col =0; col < tempM.cols; col++)
	{
		for (int row =0; row < tempM.rows; row++)
		{
			cv::Point pt(col, row);
			std::vector<cv::Point> outSet;
			if (mark.at<uchar>(pt) == 1)
			{
				continue;
			}
			else if(tempM.at<uchar>(pt)== outlineOrSand)
			{
				mark.at<uchar>(pt) = 1;
				continue;
			}
			else
			{
				searchOutlines( tempM,  mark, pt,  outSet, outlineOrSand);
			}
			if (outSet.size() >= maxUntrustNumber)
			{
				for (int i = 0; i != outSet.size(); i++)
				{
					outline.at<uchar>(outSet[i]) = 255- outlineOrSand;
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
			getEightNeighbors(pt,eightNeighbor,mark);
			for (int k= 0; k < eightNeighbor.size(); k++)
			{
				if (mark.at<uchar>(eightNeighbor[k]) == 0)
				{
					mark.at<uchar>(eightNeighbor[k]) = 1;
					if (tempM.at<uchar>(eightNeighbor[k]) == 255- outlineOrSand)
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
void getEightNeighbors(const cv::Point& pt, std::vector<cv::Point>& eightNeighbor,const cv::Mat&mark)
{
	int x = mark.cols;
	int y = mark.rows;
	for (int i = 0; i < EIGHT_NEIGHBOR; i++)
	{
		cv::Point nPt=pt+ EIGHT_POINT_NEIGHBOR[i];
		if (nPt.x >= 0 && nPt.x < x && nPt.y >= 0 && nPt.y < y)
		{
			eightNeighbor.push_back(nPt);
		}
	
	}
};
// 函数用于绘制直方图  
void plotHistogram(const cv::Mat& hist, int histSize, int histHeight, const std::string& equlizedImageAddress)
{
	int binW = 256 / histSize; // 每个直方图条目的像素宽度  
	cv::Mat histImg(histHeight, 256, CV_8UC1, cv::Scalar(255, 255, 255)); // 创建一个白色背景的图像  

	// 归一化直方图数据到0-histHeight范围  
	cv::Mat normalizedHist;
	cv::normalize(hist, normalizedHist, 0, histHeight, cv::NORM_MINMAX, -1, cv::Mat());

	// 在直方图图像上绘制直方图条目  
	for (int i = 1; i < histSize; i++) {
		cv::line(histImg, cv::Point(binW * (i - 1), histHeight - cvRound(normalizedHist.at<float>(i - 1))),
			cv::Point(binW * (i), histHeight - cvRound(normalizedHist.at<float>(i))),
			cv::Scalar(0), 2, 8, 0);
	}
	cv::imwrite(equlizedImageAddress, normalizedHist);
};

