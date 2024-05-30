// PictureRecognize.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#pragma once
#include <iostream>
#include<string>
#include<fstream>
#include<opencv2/opencv.hpp>
#include"SeedPoint.h"
#include"PictureOutline.h"
#include"SandSediment.h"
#include"test.h"
#include"catchPicture.h"
#include"sandDataDispose.h"
//#define GAS_AND_SIEVING
//#define POST_PROCESSING
//#ifdef FEATURE_GRAIN
#define SAND_GAS_DATA
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
int main()
{
#ifdef GAS_AND_SIEVING
	seiveingSandData();
#endif
#ifdef POST_PROCESSING
	sandDataD();
	return 0;
#endif
#ifdef PICTURE_MERGE_
	mergeTwoColorPicture();
#endif
#ifdef SAND_GAS_DATA
	sandMinDiameterData();
#endif
#ifdef MAIN_GG
	cv::Mat Orig_picture;

	std::cout << "Please enter the image to be processed!";
	std::string inputPicture;
	std::cin >> inputPicture;
	Orig_picture = cv::imread(inputPicture);
	cv::Mat Blur_picture, color_HSV, old_Blur_picture;
	cv::GaussianBlur(Orig_picture, Blur_picture, cv::Size(5, 5), 1);

	cv::cvtColor(Blur_picture, color_HSV, cv::COLOR_RGB2HSV);
	std::string workFaddress = inputPicture;
	std::string pastWorkFolder;
	auto pos = workFaddress.find_last_of("\\//");
	workFaddress.erase(++pos);
	pastWorkFolder = workFaddress;
	workFaddress += "control.JPG";
	double boardArea = 0;
	do {
		std::cout << "Please enter the area of the calibration plate (in square centimetres):" << std::endl;
		std::cin >> boardArea;
		if (boardArea > 0)
		{
			break;
		}
		else
		{
			std::cout << "The area you entered for the calibration plate is incorrect, please re-enter it!" << std::endl;
		}
	} while (true);
	double singlePixeArea = cp::getSinglePixelArea(Blur_picture, boardArea, workFaddress);
	std::cout << "singlePixeArea=" << singlePixeArea << std::endl;
	std::string pixTolengthA = pastWorkFolder + "pixtToLength.txt";
	std::ofstream pixToL(pixTolengthA);
	pixToL << "Single sand length is :" << sqrtf(singlePixeArea) * 10 << "mm/pix";
	pixToL.close();
	int seednumber = SEED_NUMBER_SUM;
#ifdef FEATURE_GRAIN
	std::string FeatureGrainAddress = pastWorkFolder + "\\featureGrain.txt";
	std::ofstream feature(FeatureGrainAddress);
	for (gre = 50; gre <= 130; gre += 20)
	{
		for (can1 = 240; can1 <= 255; can1 += 15)
		{
			for (can2 = 5; can2 <= 45; can2 += 5)
			{
#endif
#ifndef TEST_OUTPUT_FEATURE_GRAIN


#ifdef NEED_INPUT_SEED_NUMBER_SUM
				do {
					std::cout << "Please enter the number of seed points (generally between 50 and 1000 times the estimated number of sand grains, the larger the grains the greater the multiplier):" << std::endl;
					std::cin >> seednumber;
					if (seednumber > 0)
					{
						break;
					}
					else
					{
						std::cout << "You have entered the incorrect number of grains of sand, please re-enter!" << std::endl;
					}
				} while (true);
#endif
				cv::Mat outline;
#ifdef OUTLINE_METHOD
				int outlineMethod = 0;
				std::cout << "Please select the contour determination method ("0" threshold method, other values indicate edge detection method):";
				std::cin >> outlineMethod;
#endif
				outline = Outline(Blur_picture, pastWorkFolder
#ifdef OUTLINE_METHOD
					, outlineMethod
#endif
				);
				cv::copyMakeBorder(outline, outline, BORDER_WIDTH, BORDER_WIDTH
					, BORDER_WIDTH, BORDER_WIDTH, cv::BORDER_CONSTANT, cv::Vec3b(0, 0, 0));
				cv::copyMakeBorder(color_HSV, color_HSV, BORDER_WIDTH, BORDER_WIDTH
					, BORDER_WIDTH, BORDER_WIDTH, cv::BORDER_CONSTANT, cv::Vec3b(0, 0, 0));
				global_outline = outline;
				SeedSet  seedS = Seed3Point(outline, seednumber);
				cv::Mat seedPicture;
				seedPicture = drawSeed(outline, seedS, cv::Scalar(125, 125, 125));
				std::string seedSA = pastWorkFolder;
				seedSA += "seedPicture.JPG";
				cv::imwrite(seedSA, seedPicture);
				SeedSet  seedMergeS = newSeedPointMergeS(outline, seedS, pastWorkFolder);
				cv::Mat newSeedPicture = drawSeed(outline, seedMergeS, cv::Scalar(125, 125, 125));
				std::string newSeedSA = pastWorkFolder;
				newSeedSA += "newSeedPicture.JPG";
				cv::imwrite(newSeedSA, newSeedPicture);
				std::vector<cv::Mat> sandSet;
				sandSet = SandSediment(color_HSV, seedMergeS, outline, pastWorkFolder, singlePixeArea);
				cv::Mat seedMergePicture;
				seedMergePicture = drawSeed(outline, seedMergeS, cv::Scalar(125, 125, 125));
				cv::Mat colorPicture = Orig_picture;
				colorPicture = drawSeed(colorPicture, seedMergeS, cv::Scalar(255, 255, 255));
				cv::namedWindow("seedMergePicture", cv::WINDOW_NORMAL);
				cv::imshow("seedMergePicture", seedMergePicture);
				cv::imwrite("seedMergePicture.JPG", seedMergePicture);
				cv::namedWindow("outline", cv::WINDOW_NORMAL);
				cv::imshow("outline", outline);
				cv::imwrite("outline.JPG", outline);
				cv::namedWindow("seedPicture", cv::WINDOW_NORMAL);
				cv::imshow("seedPicture", seedPicture);
				cv::imwrite("seedPicture.JPG", seedPicture);
				cv::namedWindow("colorPicture", cv::WINDOW_NORMAL);
				cv::imshow("colorPicture", colorPicture);

				cv::imwrite("colorPictur.JPG", colorPicture);
#endif
#ifdef FEATURE_GRAIN
				outputFeatureGrain(feature);
			}
		}
	}
#endif
	return 0;
#endif
};

