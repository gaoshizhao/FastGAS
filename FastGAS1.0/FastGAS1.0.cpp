// PictureRecognize.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#pragma once
#include <iostream>
#include<string>
#include<fstream>
#include<opencv2/opencv.hpp>
#include <filesystem>
#include <vector>  
#include <algorithm> 
#include <chrono> 
#include <sys/stat.h> 

#include"SeedPoint.h"
#include"PictureOutline.h"
#include"SandSediment.h"
#include"test.h"
#include"catchPicture.h"
#include"pixelSizeDetermination.h"
#include"fastSandSediment.h"
#include"sandSediment.h"
#include"fastSortSeedSet.h"
#include"fastGetSeedPoint.h"
#include"fastSandDataDispose.h"
//#define ROOT_MEAN_SQUARE_ERROR
//#define GAS_AND_SIEVING
//#define POST_PROCESSING
//#ifdef FEATURE_GRAIN
//#define SAND_GAS_DATA
#define FAST_GAS
#define SEED_POINT_TEST
int gre;
int can1, can2;
double featureD = 0.9;
std::string sandArea_address, featureGrainAddress;
//#endif
#ifndef POST_PROCESSING
#ifndef PICTURE_MERGE_
#ifndef GAS_AND_SIEVING
#ifndef SAND_GAS_DATA
#define MAIN_GG
#endif
#endif
#endif
#endif
#define SEED_NUMBER_SUM 20000
#define SEEDNUMBER 100
cv::Mat global_outline;
namespace fs = std::filesystem;
fs::path moveFileToNewFolder(const fs::path& sourceFilePath, const fs::path& destinationFolderPath) {
	fs::path newFilePath = destinationFolderPath / sourceFilePath.filename();
	fs::rename(sourceFilePath, newFilePath); // 移动文件到新位置  
	return newFilePath;
}
std::string getFileNameWithoutExtension(const std::string& filePath) {
	size_t lastDotPos = filePath.rfind('.');
	size_t lastSlashPos = filePath.find_last_of("/\\");
	if (lastSlashPos == std::string::npos) {
		// 如果没有路径，只有文件名  
		return filePath.substr(0, lastDotPos);
	}
	if (lastDotPos == std::string::npos || lastDotPos < lastSlashPos) {
		// 如果没有扩展名或扩展名在路径中而非文件名中  
		return filePath.substr(lastSlashPos + 1);
	}
	return filePath.substr(lastSlashPos + 1, lastDotPos - lastSlashPos - 1);
}
int main()
{
#ifndef ROOT_MEAN_SQUARE_ERROR
	double circleArea = 0;
	//Single pixel size determination.
	double singlePixeLength = 0;
	//Single pixel size available
	std::cout << "Is there already a single pixel size?(y/n)" << std::endl;
	char have_pixelSize;
	std::cin >> have_pixelSize;
	if (have_pixelSize == 'y' || have_pixelSize == 'Y')
	{
		std::cout << "Please enter the single pixel size (in meters):";
		std::cin >> singlePixeLength;
	}
	else
	{
		do {
			std::cout << "Please enter the area of the calibration plate (in square centimetres):" << std::endl;
			std::cin >> circleArea;
			if (circleArea > 0)
			{
				break;
			}
			else
			{
				std::cout << "The area you entered for the calibration plate is incorrect, please re-enter it!" << std::endl;
			}
		} while (true);
	}
	cv::Mat Orig_picture;
	std::cout << "Please enter the working directory!";
	std::string sourceFolderPath;
	std::cin >> sourceFolderPath;
	std::string pixelLongAddress= sourceFolderPath + "\\pixel_long.txt";
	std::string timeAddress = sourceFolderPath + "\\execution_time.txt";
	std::string fileSizeAddress= sourceFolderPath + "\\file_size.txt";
	// 写入到.txt文件中  
	std::ofstream outfile(timeAddress);
	std::ofstream outPixelLong(pixelLongAddress);
	std::ofstream outFileSize(fileSizeAddress);
	std::string  file_n;
	std::vector<std::string> validExtensions = { ".jpg", ".jpeg", ".png", ".gif",".tiff",".tif"}; // 可以添加更多图片格式 
	for (const auto& entry : fs::directory_iterator(sourceFolderPath)) 
	{
		if (fs::is_regular_file(entry))
		{
			std::string extension = entry.path().extension().string();
			std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower); // 转换为小写以进行比较
			if (std::ranges::any_of(validExtensions, [extension](const std::string& ext) {return ext == extension; }))
			{
				// 创建一个以图片名称命名的新文件夹  
				fs::path newFolderPath = sourceFolderPath / entry.path().stem();
				file_n = entry.path().stem().string();
				if (!fs::exists(newFolderPath))
				{
					fs::create_directories(newFolderPath);
				}
				// 将图片移动到新创建的文件夹中 

				fs::path newFilePath= moveFileToNewFolder(entry.path(), newFolderPath);

				// 获取开始时间  
				auto start = std::chrono::high_resolution_clock::now();

				Orig_picture = cv::imread(newFilePath.string());
				cv::Mat Blur_picture, color_HSV, old_Blur_picture;
				//cv::GaussianBlur(Orig_picture, Blur_picture, cv::Size(5, 5), 1);
				Blur_picture = Orig_picture;
				//cv::cvtColor(Blur_picture, color_HSV, cv::COLOR_RGB2HSV);
				struct stat statbuf;
				stat(newFilePath.string().c_str(), &statbuf);
				double filePictureSize= statbuf.st_size / 1024.0;
				outFileSize << filePictureSize << std::endl;
				std::string pastWorkFolder, workFaddress= newFilePath.string();
				auto pos = newFilePath.string().find_last_of("\\//");
				workFaddress.erase(++pos);
				pastWorkFolder = workFaddress;
				if (have_pixelSize != 'y' && have_pixelSize != 'Y')
				{
					//通过circleArea是否为零判断单个像素尺寸是否已给定
					double singlePixeArea = getCircleSinglePixelArea(Orig_picture, circleArea, pastWorkFolder);
					singlePixeLength = sqrtf(singlePixeArea);
					std::string fileName = getFileNameWithoutExtension(newFilePath.string());
					outPixelLong << "第" << fileName <<": " << singlePixeLength * 10 << "mm/pix"<<std::endl;					
				}
				//生成砾石轮廓
				cv::Mat outline;
				outline = Outline(Blur_picture, pastWorkFolder);
				cv::copyMakeBorder(outline, outline, BORDER_WIDTH, BORDER_WIDTH
					, BORDER_WIDTH, BORDER_WIDTH, cv::BORDER_CONSTANT, cv::Vec3b(0, 0, 0));
				cv::copyMakeBorder(Blur_picture, Blur_picture, BORDER_WIDTH, BORDER_WIDTH
					, BORDER_WIDTH, BORDER_WIDTH, cv::BORDER_CONSTANT, cv::Vec3b(0, 0, 0));
				SeedSet  seedS = generateSeedPoint(outline, SEED_NUMBER_SUM, pastWorkFolder, Blur_picture);
				cv::Mat seedPicture;
				seedPicture = drawSeed(outline, seedS, cv::Scalar(125, 125, 125));
				std::string seedSA = pastWorkFolder;
				seedSA += "seedPicture.JPG";
				cv::imwrite(seedSA, seedPicture);
				sortSeed(seedS, outline);
				//显示排序结果
				cv::Mat sortSeedPicture = drawSortSeed(outline, seedS, cv::Scalar(125, 125, 125));
				std::string sortSeedSA = pastWorkFolder;
				sortSeedSA += "sortSeedPicture.JPG";
				cv::imwrite(sortSeedSA, sortSeedPicture);
				//图像分割
				std::multimap<double, double> diameterAndArea;
				std::vector<double> sandMinDiameter;
				diameterAndArea = fastSandSediment(Blur_picture, seedS, outline, pastWorkFolder, singlePixeLength, sandMinDiameter);
				//输出累计曲线数据
				fastSandMinDiameterData(sandMinDiameter, pastWorkFolder, diameterAndArea);
				// 获取结束时间  
				auto end = std::chrono::high_resolution_clock::now();
				// 计算运行时间  
				auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
				double execution_time_in_seconds = duration.count() / 1000000.0;

				if (outfile.is_open()) {
					outfile <<file_n<<"  "<< filePictureSize << "  " << execution_time_in_seconds /*<< "秒"*/ << std::endl;
				}
				else {
					std::cerr << "无法打开文件" << std::endl;
				}
			}
		}
	}



#endif 
#ifdef ROOT_MEAN_SQUARE_ERROR
	    //计算两条曲线的均方根误差
     calculateRMSE();
#endif
};

