#pragma once
#include"SandSediment.h"
#include"SeedPoint.h"
#include"test.h"
#include <direct.h>
#include <io.h>
#include"sandDataDispose.h"
#define WHETHER_EXCEL false
#define SAND_NUMBER 551000
//#define WRITE_EXCEL
#define NEGATIVE_ML  400
#define TWENTY_SEVEN_POINT
#ifndef TWENTY_SEVEN_POINT
#define THREE_POINT
#endif
#define COLOR_HSV
#ifndef COLOR_HSV
#define MACHINE_LEARNING_SVM
#ifndef MACHINE_LEARNING_SVM
#ifndef MACHINE_LEARNING_RTREES
#ifndef MACHINE_LEARNING_BOOST
#ifndef MACHINE_LEARNING_BAYES
#define MACHINE_LEARNING_LOGISTIC_REGRESSION
#endif
#endif
#endif
#endif
#endif
#define SEED_CONTROL_POSITION
#define TEST_0  200000
#define TEST_1  2690000
#define POSITION_X  3812
#define POSITION_Y   72
#define OUTLINE_THRESHOLD  -9
#define CELL_SET_COUNT 10
#define REWARD
#define ROTATE_OUTLINE
#define MEASURE_D
int  pre_cellSetSize;
int next_cellSetSize;
int cellSetCount;
bool cellSetBool;
const double DELETE_IN_MULTIMAP = 0.001;
extern cv::Mat global_outline;
std::string address_global;
cv::RNG seedRngColor = cv::theRNG();
int first_mark = 0;
cv::Point beginSearch;
cv::Point searchSeed;
int seed_number_globle = 0;
struct DirectionRangeXY
{
	cv::Range x_range;
	cv::Range y_range;
};
struct SearchDirection 
{
	int y0; 
	int x1; 
	int y2; 
	int x3;
	int count;
};
struct PreviousCellSet 
{
	std::vector<Cell>preSet;
	bool isContinue;
};
PreviousCellSet previousCellSet;
void setSearchDirection(SearchDirection& searchDirection, cv::Point& seed);
void renew_searchSeed(cv::Point& searchSeed, DirectionRangeXY& directionrangeXY);
SearchDirection searchDirection;
int sandMarkNumber ;
DirectionRangeXY directionRangeXY;
void renewDirectionRangeXY(DirectionRangeXY& directionRangeXY, const TestCellPosition& lastTCP);
cv::Mat TestCellPosition_Mark;
cv::Mat TestCellDirection_Mark;
int test_0 = 0;
std::ofstream output("123.txt");
std::ofstream newOutput("new123.txt");
int sn = 0;
cv::Mat* ad_globel=0;


std::vector<cv::Mat> SandSediment(const cv::Mat& colorPicture, SeedSet& sSet,const cv::Mat&Outline,const std::string& workFaddress, double singlePixeArea)
{

	cv::Mat colorSandGrain, new_colorSandGrain,new_colorSandGrain1;
	colorPicture.copyTo(new_colorSandGrain);
	cv::cvtColor(new_colorSandGrain, colorSandGrain, cv::COLOR_HSV2RGB);
	colorSandGrain.copyTo(new_colorSandGrain1);
	std::string thresholdAddress = workFaddress + "threshold.txt";
	std::ofstream thresholdSVM(thresholdAddress);
	std::string sandAreaAddress = workFaddress + "newOutPicture\\";
#ifdef GET_NEIGHBOR_FIRST
	address_global = sandAreaAddress;
#endif
	if (0 != ::_access(sandAreaAddress.c_str(), 0))
	{
		::_mkdir(sandAreaAddress.c_str());   
	}
	sandAreaAddress +=  "sandArea\\";
	if (0 != ::_access(sandAreaAddress.c_str(), 0))
	{
		::_mkdir(sandAreaAddress.c_str());
	}
	sandAreaAddress += "sandArea.txt";
#ifdef FEATURE_GRAIN
	sandArea_address = sandAreaAddress;
#endif

	std::ofstream sandAreaOutput(sandAreaAddress);
#ifdef WRITE_EXCEL
		xlnt::workbook dataBook;
		dataBook.load("C://work//PictureRecognize//PictureRecognize//PictureRecognize//picture.xlsx");
		xlnt::worksheet dataSheet = dataBook.active_sheet();
#endif
	std::vector<cv::Mat>sandSet;
	std::vector<Cell>cellSet;
	std::vector<Cell>tempCellSet;
	std::vector<Cell>FourNeighborSet;
	std::vector<Cell>outline;
	cv::Mat sumOutlinePicture,bgr_sumOutlinePicture, bgr_sumOutlinePictureNew;
	colorPicture.copyTo(sumOutlinePicture);
	cv::cvtColor(sumOutlinePicture, bgr_sumOutlinePicture, cv::COLOR_HSV2RGB);
	bgr_sumOutlinePicture.copyTo(bgr_sumOutlinePictureNew);
#ifdef TEST_BREAK_POINT
	std::string markAdress = "C:\\work\\ scientific_research\\digital_particle_size_identification method\\work\\schematic\\newOutPicture\\mark\\1988_mark.JPG";
	if (markAdress == "")
	{
		std::cout << "The address entered is empty!" << std::endl;
		exit(-1);
	}
	cv::Mat ttMM;
	ttMM = cv::imread(markAdress);
	ttMM.convertTo(sSet.mark,CV_32SC1);
	if (sSet.mark.data== NULL)
	{
		std::cout << "The image was not read successfully!" << std::endl;
		exit(-1);
	}
#else
	sSet.mark = cv::Mat::zeros(sSet.mark.rows, sSet.mark.cols, CV_32S);
#endif
	cv::Mat& seed_mark = sSet.mark;
	cv::Mat mark= cv::Mat::zeros(sSet.mark.rows, sSet.mark.cols, CV_32S);
	ad_globel = &mark;
	TestCellDirection_Mark = cv::Mat::zeros(sSet.mark.rows, sSet.mark.cols, CV_32S);
	InitializationTestCellDirection_Mark(TestCellDirection_Mark);
	std::vector<cv::Point>polygonPointSet;
#ifdef REWARD
	for (int i =
#ifndef TEST_BREAK_POINT
		sSet.seedPSet.size()-1
#endif
		; i >= 0; i--)
#else
	for (int i =
#ifndef TEST_BREAK_POINT
		0
#endif
		; i != sSet.seedPSet.size(); i++)
#endif
	{
		if (sSet.mark.at<int>(sSet.seedPSet[i].location) != 0)
		{
			continue;
		}
		cv::RotatedRect sandRect;
		cv::Mat oldOutline;
		Outline.copyTo(oldOutline);
		pre_cellSetSize = 0;
		next_cellSetSize = 0;
		cellSetCount = 0;
		cellSetBool = false;
		Seed& seed = sSet.seedPSet[i];
		cv::Size pictureSize(oldOutline.cols, oldOutline.rows);
		polygonPointSet.clear();
#ifdef GET_PLOYGON_SIXTEEN_POINT
		getPolygonSixTeenPoint(seed.location, oldOutline, polygonPointSet);
#else
		getPolygonPoint(seed.location, oldOutline, polygonPointSet);
#endif
		deleteNewUntrustPoint(seed.location, polygonPointSet, 1, pictureSize);
		cv::RotatedRect tempSeedRect;
		tempSeedRect = cv::minAreaRect(polygonPointSet);
		cv::Point tsp_center = tempSeedRect.center;
#ifdef GET_PLOYGON_SIXTEEN_POINT
		getPolygonSixTeenPoint(seed.location, oldOutline, polygonPointSet);
#else
		getPolygonPoint(seed.location, oldOutline, polygonPointSet);
#endif
		deleteNewUntrustPoint(seed.location, polygonPointSet,1, pictureSize);
		std::vector<cv::Point>newPolgonPointSet = polygonPointSet;
		std::vector<cv::Point>newPositionSeed;
#ifdef SEED_CONTROL_POSITION
		cv::Mat seedPositionOutline;
		oldOutline.copyTo(seedPositionOutline);
		drawNewPositionSeed(seedPositionOutline, newPolgonPointSet);
		if (isDispose(sSet.mark, newPolgonPointSet))
		{
			continue;
		}
		std::string seed_position_address = workFaddress + "newOutPicture\\";
		if (0 != ::_access(seed_position_address.c_str(), 0))
		{
			_mkdir(seed_position_address.c_str()); 
		}
#ifndef TEST_BREAK_POINT
		seed_position_address += "mark\\";
#else
		seed_position_address += "seed_position\\";
#endif
		if (0 != ::_access(seed_position_address.c_str(), 0))
		{
			_mkdir(seed_position_address.c_str());   
		}
#ifndef TEST_BREAK_POINT
		std::string seed_position_number = std::to_string(i) + "_mark.JPG";
#else
		std::string seed_position_number = std::to_string(i) + "_OutlinePicture.JPG";
#endif
		seed_position_address += seed_position_number;
#ifndef TEST_BREAK_POINT
		/*cv::imwrite(seed_position_address, sSet.mark);*/
#else
		/*cv::imwrite(seed_position_address, seedPositionOutline);*/
#endif
#endif
		expansionPolygonPointSet(EXPANSION_CONTROL_POINT_SEED_NUMBER, oldOutline, polygonPointSet, newPositionSeed);
		if (polygonPointSet.size() < 3)
		{
			continue;
		}
		std::vector<cv::Point>con_hull;
		cv::convexHull(polygonPointSet, con_hull);
		sandRect = cv::minAreaRect(con_hull);
		cv::Rect minRect;
		minRect = cv::boundingRect(con_hull);
		drawMarkPolygon(oldOutline, con_hull);
#ifdef ROTATE_OUTLINE
		cv::Mat polygonOutline;
		polygonOutline = drawEightPolygonPointInOutline(oldOutline, seed.location, polygonPointSet);
		drawNewPositionSeed(polygonOutline, newPositionSeed);
		cv::Mat rotateOutline;
		rotateOutline = drawRotateOutline(oldOutline, sandRect, con_hull);
		std::string  outline_color_number;
		std::string outline_picture_address = workFaddress + "newOutPicture\\";
		if (0 != ::_access(outline_picture_address.c_str(), 0))
		{
			_mkdir(outline_picture_address.c_str());  
		}
		outline_picture_address += "outline\\";
		if (0 != ::_access(outline_picture_address.c_str(), 0))
		{
			_mkdir(outline_picture_address.c_str());
		}
		std::string rot_outline_address = outline_picture_address;
		std::string outline_rotate_number = std::to_string(i) + "_OutlineRgbPicture.JPG";
		rot_outline_address += outline_rotate_number;
		outline_color_number = std::to_string(i) + "_d5-10_OutlineRgbPicture.JPG";
		outline_picture_address += outline_color_number;
		cv::imwrite(outline_picture_address, polygonOutline);
#endif
		sSet.seedPSet[i].location = sandRect.center;
		sSet.seedPSet[i].scope = FormScope(oldOutline, sSet.seedPSet[i].location);
		int search_end_test = 0;
		int scope_min = 0;
		if (sandRect.size.height > sandRect.size.width)
		{
			scope_min = sandRect.size.width * 0.07;
		}
		else
		{
			scope_min = sandRect.size.height * 0.07;
		}
		int outline_n = 0;
		if (scope_min > OUTLINE_MARK)
		{
			outline_n = scope_min;
		}
		else
		{
			outline_n = OUTLINE_MARK;
		}
		int markNumber = i + 1;
		getNeighbor(colorPicture, cellSet, sSet.seedPSet[i], outline_n, sSet, markNumber);
		if (isDispose(sSet.mark, cellSet))
		{
			cellSet.clear();
			continue;
		}
		thresholdSVM << "*****************************************************************" << std::endl;
		thresholdSVM << "****************************" << "No." << i << "Sand grains" << "****************" << std::endl;
		std::vector<Cell>outlineSet;
#ifndef  COLOR_HSV
		try {
			getOutline(colorPicture, minRect, outlineSet);
		}
		catch(std::runtime_error(err))
		{
			cellSet.clear();
			output << err.what() << "\n";
			continue;
			continue;
		}
#endif
		cv::Mat trainingData;
		cv::Mat responses;
#ifndef MACHINE_LEARNING_SVM
		fromCellToMat(cellSet, outlineSet, trainingData, responses);
#else
#ifndef FOUR_DIMENSIONAL_SPACE
		 formSVMOneClass(cellSet, trainingData, responses);
#else
		formSVMClassFour( cellSet,trainingData, responses);
#endif

#endif
#ifdef MACHINE_LEARNING_SVM
		cv::Ptr<cv::ml::SVM> svm = cv::ml::SVM::create();
		svm->setType(cv::ml::SVM::ONE_CLASS);
		svm->setCoef0(40000);
		svm->setDegree(1);
		svm->setGamma(0.0000000001);
		svm->setTermCriteria(cv::TermCriteria(cv::TermCriteria::MAX_ITER + cv::TermCriteria::EPS, 1000, 1e-3));
	
		svm->setKernel(cv::ml::SVM::POLY);
		svm->setNu(0.8);
		svm->setC(0.001); 
		svm->train(trainingData, cv::ml::ROW_SAMPLE, responses);
#endif
#ifdef MACHINE_LEARNING_RTREES
		cv::Ptr<cv::ml::RTrees> forest = cv::ml::RTrees::create();
		forest->setMaxDepth(28);
		forest->setMinSampleCount(50);
		forest->setMaxCategories(2);
		forest->setCalculateVarImportance(true);
		forest->setActiveVarCount(2);
		forest->setTermCriteria(cv::TermCriteria(cv::TermCriteria::MAX_ITER + cv::TermCriteria::EPS, 100, 0.01));
		forest->train(trainingData, cv::ml::ROW_SAMPLE, responses);
#endif
#ifdef MACHINE_LEARNING_BOOST
		cv::Ptr<cv::ml::Boost> boosting = cv::ml::Boost::create();
		std::vector<double>priors(2);
		priors[0] = 1;
		priors[1] = 5;
		boosting->setBoostType(cv::ml::Boost::REAL);
		boosting->setWeakCount(10);
		boosting->setWeightTrimRate(0.95);
		boosting->setMaxDepth(5);
		boosting->setUseSurrogates(false);
		boosting->setPriors(cv::Mat(priors));
		boosting->train(trainingData, cv::ml::ROW_SAMPLE, responses);
#endif
#ifdef MACHINE_LEARNING_BAYES
		cv::Ptr<cv::ml::NormalBayesClassifier> bayes = cv::ml::NormalBayesClassifier::create();
		bayes->train(trainingData, cv::ml::ROW_SAMPLE, responses);
#endif
#ifdef MACHINE_LEARNING_LOGISTIC_REGRESSION
		cv::Ptr<cv::ml::LogisticRegression> lr1 = cv::ml::LogisticRegression::create();
		lr1->setLearningRate(0.001);
		lr1->setIterations(10);
		lr1->setRegularization(cv::ml::LogisticRegression::REG_L2);
		lr1->setTrainMethod(cv::ml::LogisticRegression::BATCH);
		lr1->setMiniBatchSize(1);
		lr1->train(trainingData, cv::ml::ROW_SAMPLE, responses);
#endif
#ifdef COLOR_HSV
		Color sandColor;
		setColor(sandColor, cellSet[0]);
		adjustColor(sandColor, cellSet);
#endif

		test_0 = 0;
		sn++;
		seed_number_globle = i;
		output << "No." << i << "Sand Grain." << std::endl;
#ifdef COLOR_HSV
		Similar sandSimilar;
#endif
		int seedNumber = i;
		sandMarkNumber = markNumber;
#ifdef WRITE_EXCEL
		if (test_0 >= TEST_0 && seed_number_globle == SAND_NUMBER)
		{
			for (int i = 0; i < cellSet.size(); i++)
			{
				writeCellPositionAndAttributeAndDirection(dataSheet,
					cellSet[i].position, 0,
					sandMarkNumber);
			}
		}
#endif
		markSeedSetMark(sSet, cellSet, markNumber, mark);
		markPointMark(sSet.mark, sSet.seedPSet[i].location, markNumber);
		markPointMark(mark, sSet.seedPSet[i].location, markNumber);
		TestCellPosition_Mark = cv::Mat::zeros(mark.rows, mark.cols, CV_32S);
		previousCellSet.isContinue = true;
		previousCellSet.preSet = tempCellSet;
		formTempCellSet(colorPicture, mark, tempCellSet, cellSet
			, search_end_test, seed,
#ifdef WRITE_EXCEL
			dataSheet, dataBook,
#endif
			polygonPointSet);
		int sand_over_count = 0;
	try{
		while (tempCellSet.size() != 0 && sand_over_count <= TERMINATION_LOOP)
		{

			std::cout << "tempCellSet.size() = " << tempCellSet.size() << std::endl;
			output << "tempCellSet.size() = " << tempCellSet.size() << std::endl;
			pre_cellSetSize = cellSet.size();
			for (int i = 0; i < tempCellSet.size(); i++)
			{
				sand_over_count++;
				test_0++;

				if (true)
				{
					output << "******************test_0=" << test_0 << std::endl;
				}
#ifdef PICTURE_OUTPUT
				if (test_0 == 1760 && i == 25)
				{
					std::cout << "";
				}
				if (test_0 == 10944)
				{
#endif
#ifdef WRITE_EXCEL
					/*dataBook.save("C://work//PictureRecognize//PictureRecognize//PictureRecognize//picture.xlsx");*/
#endif
#ifdef PICTURE_OUTPUT
					cv::Mat newPicture, bgr_newPicture;
					colorPicture.copyTo(newPicture);
					cv::cvtColor(newPicture, bgr_newPicture, cv::COLOR_HSV2RGB);
					drawMarkAtColorPicture(bgr_newPicture, mark);
					std::string picture_address, color_number;
					picture_address = "C:\\work\\PictureRecognize\\PictureRecognize\\PictureRecognize\\outputPicture\\new\\";
					color_number = std::to_string(i) + "_d5-10_rgbPicture.JPG";
					picture_address += color_number;
					cv::imwrite(picture_address, bgr_newPicture);
					exit(-1);
				}
#endif
				if (test_0 == 26825)
				{

					int a = mark.at<int>(tempCellSet[i].position);
					std::cout << "";
				}
#ifdef COLOR_HSV
				IsSandPlaque iSandPlaque;
				CompareCellColor(colorPicture, sandColor, tempCellSet[i], iSandPlaque, mark, oldOutline);
				if (iSandPlaque.isSandPlaque)
				{
#endif
#ifdef MACHINE_LEARNING_SVM
#ifndef FOUR_DIMENSIONAL_SPACE
					if (OneClassSVMPredict( Outline, svm,  tempCellSet[i], thresholdSVM))
#else
					if (OneClassSVMPredictFour(svm, tempCellSet[i]))
#endif
					{
#endif
#ifdef MACHINE_LEARNING_RTREES
						if ((int)(forest->predict(cellTransactionMat(tempCellSet[i]))) == 1)
						{
#endif
#ifdef MACHINE_LEARNING_BOOST
							if ((int)(boosting->predict(cellTransactionMat(tempCellSet[i]))) == 1)
							{
#endif
#ifdef MACHINE_LEARNING_BAYES
								if ((int)(bayes->predict(cellTransactionMat(tempCellSet[i]))) == 1)
								{
#endif
#ifdef MACHINE_LEARNING_LOGISTIC_REGRESSION
									if ((int)(lr1->predict(cellTransactionMat(tempCellSet[i])) == 1))
									{
#endif
										cellSet.push_back(tempCellSet[i]);
#ifdef COLOR_HSV
										if (iSandPlaque.isColorPlaque)
										{
											adjustColor(sandColor, tempCellSet[i]);
											adjustTextureSimilar(sandSimilar, cellSet, tempCellSet[i]);
										}
#endif
										markPointMark(mark, tempCellSet[i].position, markNumber);
										markPointMark(seed_mark, tempCellSet[i].position, markNumber);
										cv::Point temp_a = tempCellSet[i].position;
										int dire = TestCellDirection_Mark.at<int>(temp_a);
										continue;
				}
									else 
									{
										if (compareCellTexture(sandSimilar, cellSet, tempCellSet[i]))
										{
											cellSet.push_back(tempCellSet[i]);
											adjustColor(sandColor, tempCellSet[i]);									
											adjustTextureSimilar(sandSimilar, cellSet, tempCellSet[i]);
											markPointMark(sSet.mark, tempCellSet[i].position, markNumber);
											continue;
										}
									}
									cv::Point temp_a = tempCellSet[i].position;
									int dire = TestCellDirection_Mark.at<int>(temp_a);
									markPointMark(sSet.mark, tempCellSet[i].position, OUTLINE_MARK);
									markPointMark(mark, tempCellSet[i].position, OUTLINE_MARK);
#ifdef WRITE_EXCEL
									if (test_0 >= TEST_0 && seed_number_globle == SAND_NUMBER)
									{
										int value_m = sSet.mark.at<int>(tempCellSet[i].position);
										writeCellPositionAndAttributeAndDirection(dataSheet,
											tempCellSet[i].position, dire,
											value_m);
										/*dataBook.save("C://work//PictureRecognize//PictureRecognize//PictureRecognize//picture.xlsx");*/
									}
#endif
			}
#ifdef WRITE_EXCEL
			/*if (test_0 >= TEST_0)
			{
				dataBook.save("C://work//PictureRecognize//PictureRecognize//PictureRecognize//picture.xlsx");
			}*/
#endif

								next_cellSetSize = cellSet.size();
								std::vector<AdjustPosition> AdSet;
#ifdef WRITE_EXCEL
								if (test_0 >= TEST_0 && seed_number_globle == SAND_NUMBER)
								{
									dataBook.save("C://work//PictureRecognize//PictureRecognize//PictureRecognize//picture.xlsx");
								}
#endif
								
								AdSet = adjustBoundaryMark(colorPicture, mark, sandMarkNumber, cellSet, minRect);
								if (cellSet.size() == 0)
								{
									break;
								}

#ifdef WRITE_EXCEL
								if (test_0 >= TEST_0 && seed_number_globle == SAND_NUMBER)
								{
									adjuestMarkAndCellSet(AdSet, dataBook, dataSheet, mark, cellSet);
									writeInformationToBookdata(mark, dataBook, dataSheet
										, tempCellSet, TestCellDirection_Mark);

								}
#endif
								TestCellPosition_Mark = cv::Mat::zeros(mark.rows, mark.cols, CV_32S);
								previousCellSet.preSet = tempCellSet;
								tempCellSet.clear();
								formTempCellSet(colorPicture, mark, tempCellSet
									, cellSet, search_end_test, seed,
#ifdef WRITE_EXCEL
									dataSheet, dataBook,
#endif
									polygonPointSet);
								if (isContinueTempCellSet(previousCellSet.preSet, tempCellSet))
								{
									previousCellSet.isContinue = false;
									break;
								}
								if (next_cellSetSize == pre_cellSetSize)
								{
									cellSetCount++;
									if (cellSetCount == CELL_SET_COUNT)
									{
										cellSetBool = true;
										break;
									}
								}
								else
								{
									cellSetBool = false;
								}

		}
	}
catch (std::runtime_error err)
	{
	setTestCellDirection_MarkZero(TestCellDirection_Mark);
#ifdef  SET_MARK_ZERO
	setMarkZero(mark);
#else
	mark = cv::Mat::zeros(sSet.mark.rows, sSet.mark.cols, CV_32S);
#endif
	cellSet.clear();
	output << err.what()<<"\n";
	continue;
	}
		setTestCellDirection_MarkZero(TestCellDirection_Mark);
		cv::Mat newPicture, bgr_newPicture;
		cv::Mat bgr_sand;
		colorPicture.copyTo(newPicture);
		cv::cvtColor(newPicture, bgr_newPicture, cv::COLOR_HSV2RGB);
		if (cellSetBool&&(!is_MarginSand(sSet.seedPSet[i].location,minRect,newPicture)))
		{
            fromMarkToSand( mark,  new_colorSandGrain1,bgr_sand);
		}
		else
		{
			cullUnrealContour(mark);
			cullUnrealContour(seed_mark);
			formSandPicture(bgr_sand, cellSet, mark, new_colorSandGrain1);
		}
		cv::ellipse(bgr_newPicture, sandRect, cv::Scalar(0, 0, 255), 12);
		cv::ellipse(bgr_sumOutlinePicture, sandRect,cv::Scalar(0,0,255) ,12);
		cv::ellipse(bgr_sumOutlinePictureNew, sandRect, cv::Scalar(0, 0, 255), 12);
		cv::putText(bgr_sumOutlinePictureNew,std::to_string(i),sandRect.center,cv::FONT_HERSHEY_SIMPLEX,2,cv::Scalar(255,255,255),12);
		std::string color_number;
#ifdef RGB_PICTURE
		//*********************创建文件夹*************************
		std::string picture_address = workFaddress + "newOutPicture\\picture\\";
		//创建文件夹
		if (0 != ::_access(picture_address.c_str(), 0))
		{
			// if this folder not exist, create a new one.
			::_mkdir(picture_address.c_str());   // 返回 0 表示创建成功，-1 表示失败
			//换成 ::_mkdir  ::_access 可以创建多级目录，没测试
		}
		//********************************************************
		/*picture_address = "C:\\work\\PictureRecognize\\PictureRecognize\\PictureRecognize\\outputPicture\\d5-10\\picture\\";*/
		color_number = std::to_string(i) + "_rgbPicture.JPG";
		picture_address += color_number;
		cv::imwrite(picture_address, bgr_newPicture);
#endif
#ifdef  SET_MARK_ZERO
		setMarkZero(mark);
#else
		mark = cv::Mat::zeros(sSet.mark.rows, sSet.mark.cols, CV_32S);
#endif
		std::string number_address;
		std::string sand_address = workFaddress + "newOutPicture\\sand\\";
		if (0 != ::_access(sand_address.c_str(), 0))
		{
			::_mkdir(sand_address.c_str());
		}
		number_address = std::to_string(i) + ".JPG";
		sand_address += number_address;
		cv::imwrite(sand_address, bgr_sand);
		sandSet.push_back(bgr_sand);
		double sandDiameterMin = sandRect.size.height*std::sqrtf(singlePixeArea );
		double sandDiameterMax = sandRect.size.width * std::sqrtf(singlePixeArea );
		if (sandDiameterMin > sandDiameterMax)
		{
			std::swap(sandDiameterMin, sandDiameterMax);
		}
		double sandDiameter = std::sqrtf(singlePixeArea * cellSet.size() * 9 * 4 / PI);
		sandAreaOutput <<"No."<<i<<"Sand_grains"<<" " << sandDiameterMax<<" " << sandDiameterMin <<" "<<sandDiameter<<std::endl;
		cellSet.clear();
	}
	std::string sumOutlinePictureAddress = workFaddress + "newOutPicture\\sumOutlinePicture\\";
	std::string sumOutlinePictureAddressNew = workFaddress + "newOutPicture\\sumOutlinePicture\\";
	if (0 != ::_access(sumOutlinePictureAddress.c_str(), 0))
	{
		::_mkdir(sumOutlinePictureAddress.c_str()); 
	}
	sumOutlinePictureAddress += "sumOutlinePictrue.JPG";
	sumOutlinePictureAddressNew += "sumOutlinePictrueNumber.JPG";
	cv::imwrite(sumOutlinePictureAddress,bgr_sumOutlinePicture);
	cv::imwrite(sumOutlinePictureAddressNew, bgr_sumOutlinePictureNew);
	std::string segementAllSandAddress= workFaddress + "newOutPicture\\sumColorOutlinePicture\\";
	if (0 != ::_access(segementAllSandAddress.c_str(), 0))
	{
		::_mkdir(segementAllSandAddress.c_str()); 
	}
	segementAllSandAddress += "allColorSand.JPG";
	cv::imwrite(segementAllSandAddress, new_colorSandGrain1);
	return sandSet;
}
inline void Cell::FormMaxCell()
{
	MatCopyToMatx91b(cell, maxCell);
	for (int i = 0; i < MATX_ROW; i++)
	{
		for (int j = i; j < MATX_ROW; j++)
		{
			if (maxCell.val[i] < maxCell.val[j])
			{
				Matx13b temp;
				temp = this->maxCell.val[i];
				this->maxCell.val[i] = this->maxCell.val[j];
				this->maxCell.val[j] = temp;
			}
		}
	}
};
inline void Cell::FormMinCell()
{
	MatCopyToMatx91b(cell, minCell);
	for (int i = 0; i < MATX_ROW; i++)
	{
		for (int j = i; j < MATX_ROW; j++)
		{
			if (!(minCell.val[i] < minCell.val[j]))
			{
				Matx13b temp;
				temp = this->minCell.val[i];
				this->minCell.val[i] = this->minCell.val[j];
				this->minCell.val[j] = temp;
			}
		}
	}
};
bool operator<(const Matx13b& left, const Matx13b& right)
{
	int L = 0, R = 0;
	for (int i = 0; i < left.channels; i++)
	{
		L += left(i);
		R += right(i);
	}
	return L < R;
};
bool lessOperator(const Matx13b& left, const Matx13b& right)
{
	int L = 0, R = 0;
	for (int i = 0; i < left.channels; i++)
	{
		L += left(i);
		R += right(i);
	}
	return L < R;
};
void swap(Matx13b& lhs, Matx13b& rhs)
{
	Matx13b temp;
	temp = lhs;
	lhs = rhs;
	rhs = temp;
}
void MatCopyToMatx91b(const cv::Mat& m, Matx91b& mtx)
{

	CV_Assert((m.rows*m.cols== MATX_ROW)&&(m.channels()==MAT_CHANNEL));
	for (int i = 0; i < MAT_ROW; i++)
	{
		for (int j = 0; j < MAT_COL; j++)
		{
			mtx.val[i * MAT_ROW + j] = m.at<cv::Vec3b>(i, j);
		}
	}
};
Similar CellSimilar(const Cell& lhs, const Cell& rhs)
{
	Similar similar;
	double maxInnerProduct=0,modulusLhs=0, modulusRhs=0;
	maxInnerProduct = ddot(lhs.maxCell,rhs.maxCell);
	modulusLhs = std::sqrtf(ddot(lhs.maxCell,lhs.maxCell));
	modulusRhs = std::sqrtf(ddot(rhs.maxCell,rhs.maxCell));

	CV_DbgAssert(modulusLhs!=0|| modulusRhs!=0,"Denominator is zero");
	similar.maxSimilar = maxInnerProduct / (modulusLhs * modulusRhs);
	double minInnerProduct = 0;
	minInnerProduct = ddot(lhs.maxCell,rhs.minCell);
	similar.minSimilar = minInnerProduct / (modulusLhs * modulusRhs);
	return similar;
};
void get4Neighbor(const cv::Mat&picture,std::vector<Cell>& sCell, const Seed& seed)
{

	for (int i = 0; i < FOUR_NEIGHBOR; i++)
	{
		cv::Point newPoint;
		newPoint.x=seed.location.x+ 3 * ((2 - i - 1) % 2);
		newPoint.y=seed.location.y+ 3 * ((2 - i) % 2);
		Cell newCell= fromPointToCell(picture,newPoint);
		sCell.push_back(newCell);
	}
};
Cell fromPointToCell(const cv::Mat& picture, cv::Point& pt)
{
	CV_DbgAssert(picture.channels()==3 && ((pt.x<picture.cols-1)
              &&(pt.y<picture.rows-1)&&(pt.x>0)&&(pt.y>0)),"fromPointToCell（）Seed point in function over edge");
	cv::Point topLeft(pt.x - 1, pt.y - 1);
	cv::Point bottomRight(pt.x + 2, pt.y + 2);
	if (topLeft.x<0 || topLeft.x>picture.cols||
		topLeft.y<0 || topLeft.y>picture.rows||
		bottomRight.x<0|| bottomRight.x>picture.cols||
		bottomRight.y<0 || bottomRight.y>picture.rows 
		)
	{
		throw std::runtime_error("Overflow boundary!");
	}
	cv::Rect rectCell(topLeft, bottomRight);
	cv::Mat cellPicture;
	cellPicture=picture(rectCell);
	Cell cell(cellPicture, pt);
	return cell;

};
void markSeedSetMark(SeedSet& sSet, const std::vector<Cell>cellSet)
{
	cv::Mat& mark = sSet.mark;
	int count = cellSet.size();
	for (int i = 0; i < count; i++)
	{
		markPointMark(mark,cellSet[i].position);
	}
};
void markPointMark(cv::Mat&mark, const cv::Point& pt)
{
	cv::Point topLeft(pt.x - 1, pt.y - 1);
	for (int i = 0; i < CELL_W_H; i++)
	{
		for (int j = 0; j < CELL_W_H; j++)
		{
		isOverflowBoundary(mark, topLeft);
			mark.at<uchar>(topLeft) = 1;
		}
	}
};
void get4_8Neighbor(const cv::Mat& picture, std::vector<Cell>& sCell, const Seed& seed)
{
	for (int i = 0; i < EIGHT_FOUR_NEIGHBOR; i++)
	{
		cv::Point newPoint;
		newPoint.x = seed.location.x + 3 * DIG_FORU_NEIGHBOR[i].x;
		newPoint.y = seed.location.y + 3 * DIG_FORU_NEIGHBOR[i].x;
		Cell newCell = fromPointToCell(picture, newPoint);
		sCell.push_back(newCell);
	}
};
double ddot(const Matx91b&lhs, const Matx91b& rhs) 
{
	double s = 0;
	for (int i = 0; i < lhs.channels; i++) 
		s += lhs.val[i] * rhs.val[i];

	return s;
}
double operator*(const Matx13b& left, const Matx13b& right)
{
	int s=0;
	for (int i = 0; i < left.channels; i++)
	{
		s += left[i] * right[i];
	}
	return s;
};
void adjustTextureSimilar(Similar& sandSimilar, const std::vector<Cell>& cellSet)
{
	Similar tempSimilar;
	for (int i = 0; i < cellSet.size(); i++)
	{
		for (int j = i; j < cellSet.size(); j++)
		{
			tempSimilar = CellSimilar(cellSet[i], cellSet[j]);
			if (sandSimilar.maxSimilar < tempSimilar.maxSimilar)
			{
				sandSimilar.maxSimilar = tempSimilar.maxSimilar;
			}
			if (sandSimilar.minSimilar > tempSimilar.minSimilar)
			{
				sandSimilar.minSimilar = tempSimilar.minSimilar;
			}
		}
	}
};
void adjustTextureSimilar(Similar& sandSimilar, const std::vector<Cell>& cellSet, Cell& cell)
{
	Similar tempSimilar;
	for (int i = 0; i < cellSet.size(); i++)
	{
		tempSimilar = CellSimilar(cellSet[i], cell);
		if (sandSimilar.maxSimilar < tempSimilar.maxSimilar)
		{
			sandSimilar.maxSimilar = tempSimilar.maxSimilar;
		}
		if (sandSimilar.minSimilar > tempSimilar.minSimilar)
		{
			sandSimilar.minSimilar = tempSimilar.minSimilar;
		}
	}

};
bool compareCellTexture(const Similar& sandSimilar, const std::vector<Cell>& cellSet, const Cell& cell)
{
	int sandCellCount = cellSet.size() * SIMILAR_DEGREE;
	int count = 0;
	Similar tempSimilar;
	for (int i = 0; i < cellSet.size(); i++)
	{
		tempSimilar = CellSimilar(cellSet[i], cell);
		if (tempSimilar.minSimilar >= sandSimilar.minSimilar)
		{
			count++;
		}
	}
	return(count>= sandCellCount);
}
void markSeedSetMark(SeedSet& sSet, const std::vector<Cell>cellSet, int markNumber)
{
	cv::Mat& mark = sSet.mark;
	int count = cellSet.size();
	for (int i = 0; i < count; i++)
	{
		markPointMark(mark, cellSet[i].position,markNumber);
	}
};
void markPointMark(cv::Mat& mark, const cv::Point& pt, int markNumber)
{
	cv::Point topLeft(pt.x - 1, pt.y - 1);
	for (int i = 0; i < CELL_W_H; i++)
	{
		for (int j = 0; j < CELL_W_H; j++)
		{
			cv::Point p = cv::Point(topLeft.x + i, topLeft.y + j);
			if (p.x<0||p.x>mark.cols-1||p.y<0||p.y>mark.rows-1)
			{
				throw std::runtime_error("overrun range!");
			}
				mark.at<int>(p) = markNumber;
		}
	}
};
#ifdef WRITE_EXCEL
void formTempCellSet(const cv::Mat& colorPicture, cv::Mat& mark
	, std::vector<Cell>& tempCellSet, const Seed& seed, const std::vector<cv::Point>&polygonPointSet)
{
	tempCellSet.clear();
	TestCellPosition firstTCP,nextTCP,lastTCP;
	getFirstTestCellPosition(firstTCP, mark,  seed, TestCellPosition_Mark);
	nextTCP = firstTCP;
	cv::Point position = seed.location;
	int markValue = mark.at<int>(position);
	int ij = 0;
	do
	{
		ij++;
		if(test_0==1057&&
        lastTCP.center.position.x == 2315 && lastTCP.center.position.y == 2413)
		std::cout << "ij=" <<ij<< std::endl;
		std::cout<< "[" << lastTCP.center.position.x << " " << lastTCP.center.position.y << "]" << std::endl;
		output << "[" << lastTCP.center.position.y <<" "<< lastTCP.center.position.x<<"]"<<std::endl ;
		lastTCP = nextTCP;
		if (lastTCP.center.markValue== OUTLINE_MARK|| lastTCP.center.markValue == sandMarkNumber)
		{
			getNextTestCellPosition(nextTCP, lastTCP
				, mark, seed/*,cv::Mat& TestCellPositionMark*/,polygonPointSet);
			if(lastTCP.center.position!=firstTCP.center.position)
            markPointMark(TestCellPosition_Mark, lastTCP.center.position, VISITED_MARK);
		}
		else if(lastTCP.center.markValue == EMPTY)
		{
			Cell tempCell= fromPointToCell(colorPicture, lastTCP.center.position);;
			tempCellSet.push_back(tempCell);
			getNextTestCellPosition(nextTCP, lastTCP
				, mark, seed/*,cv::Mat& TestCellPositionMark*/, polygonPointSet);
			if (lastTCP.center.position != firstTCP.center.position)
			markPointMark(TestCellPosition_Mark, lastTCP.center.position, VISITED_MARK);
		}
		else
		{

			std::cout << "The mark mark value in formTempCellSet is an exception value";
			exit(-1);
		}
	} while (nextTCP.center.position != firstTCP.center.position);
};
#endif
void formTestCellPosition(TestCellPosition& testCellPosition, const cv::Mat& mark,const cv::Point& pt)
{
	testCellPosition.center.position = pt;
	testCellPosition.center.markValue = mark.at<int>(pt);
	for (int i = 0; i < EIGHT_FOUR_NEIGHBOR; i++)
	{
		std::vector<PositionValue>& temp= testCellPosition.Four_Neighbor;
		temp[i].position.x = pt.x + FOUR_CELL_NEIGHBOR[i].x;
		temp[i].position.y = pt.y + FOUR_CELL_NEIGHBOR[i].y;
		if (temp[i].position.x > mark.cols-1 || temp[i].position.x <0 ||
			temp[i].position.y>mark.rows-1 || temp[i].position.y < 0)
		{
			throw std::runtime_error("Overflow boundary!");
		}
		temp[i].markValue = mark.at<int>(temp[i].position);
	}
};
void getFirstTestCellPosition(TestCellPosition& firstTCP, const cv::Mat& mark, const Seed& seed,cv::Mat& TestCellPosition_Mark)
{
	cv::Point pt = seed.location;
	cv::Point next_pt = cv::Point(seed.location.x + INCREMENT_CELL, seed.location.y);
	cv::Point previous_pt = cv::Point(seed.location.x - INCREMENT_CELL, seed.location.y);
	int markValue = mark.at<int>(pt);
	int next_markValue = mark.at<int>(next_pt);
	while ((!(markValue == OUTLINE_MARK|| markValue == NO_DETERMINE_MARK))
		||(!(next_markValue== NO_DETERMINE_MARK))
		)
	{	
		pt.x += INCREMENT_CELL;
		next_pt.x += INCREMENT_CELL;
		previous_pt.x+= INCREMENT_CELL;
		markValue = mark.at<int>(pt);
		next_markValue = mark.at<int>(next_pt);
		TestCellPosition_Mark.at<int>(previous_pt) = VISITED_MARK;
	}
	firstTCP.direction = UP;
	TestCellPosition_Mark.at<int>(pt) = EMPTY;
	formTestCellPosition(firstTCP, mark, pt,UP);
	TestCellDirection_Mark.at<int>(pt) = UP;
};
void getNextTestCellPosition(TestCellPosition& nextTCP, TestCellPosition& lastTCP
	,  cv::Mat& mark, const Seed& seed/*, cv::Mat& TestCellPositionMark*/,const std::vector<cv::Point>& polygonPointSet)
{
	try{
	switch (lastTCP.direction)
	{
	    case UP:
		{
			getTestCellPositionUp( nextTCP, lastTCP
				, mark,seed/*, cv::Mat & TestCellPositionMark*/,polygonPointSet);
		    break;
		}
		case LEFT:
		{
			getTestCellPositionLEFT(nextTCP,lastTCP, mark, seed,polygonPointSet);
			break;
		}
		case DOWN:
		{
			getTestCellPositionDOWN(nextTCP,lastTCP, mark, seed,polygonPointSet);

			break;
		}
		case RIGHT:
		{
			getTestCellPositionRIGHT(nextTCP,lastTCP, mark, seed,polygonPointSet);
			break;
		}

	}
}
catch (...)
{
	throw;
}
};
void getTestCellPositionUp(TestCellPosition& nextTCP,
	TestCellPosition& lastTCP,  cv::Mat& mark, const Seed& seed
	/*, cv::Mat& TestCellPositionMark*/, const std::vector<cv::Point>& polygonPointSet)
{
	 int serial = -1;
	cv::Point direction;
	nextTCP.direction = UP;
	int test_direction_0 = getTestCellDirection_MarkValue(lastTCP.center.position, UP);
	int test_direction_1 = getTestCellDirection_MarkValue(lastTCP.center.position, LEFT);
	int test_direction_2 = getTestCellDirection_MarkValue(lastTCP.center.position, DOWN);
	if ((lastTCP.center.position.y > directionRangeXY.y_range.start))
	{
		lastTCP.direction = UP;
		TestCellDirection_Mark.at<int>(lastTCP.center.position) = UP;
		if ((lastTCP.center.position.y  > directionRangeXY.y_range.start)
			&& (test_direction_2 != -1))
		{
			test_direction_2 = UP;
		}
		TestCellDirection_Mark.at<int>(lastTCP.center.position+ FOUR_CELL_NEIGHBOR[DOWN]) = UP;
		if ((lastTCP.center.position.y - INCREMENT_CELL > directionRangeXY.y_range.start)
			&& (test_direction_1 != -1))
		{
			test_direction_1 = UP;
		}
		if ((lastTCP.center.position.y  > directionRangeXY.y_range.start)
			&& (test_direction_0 != -1))
		{
			test_direction_0 = UP;
		}
		
	}
	int visit_mark0 =getVisitMarkValue(lastTCP.center.position,UP) ;
	int visit_mark1 = getVisitMarkValue(lastTCP.center.position, LEFT);
	int visit_mark2 = getVisitMarkValue(lastTCP.center.position, DOWN);
	int direction_0 = lastTCP.Four_Neighbor[0].markValue;
	int direction_1= lastTCP.Four_Neighbor[1].markValue;
	int direction_2 = lastTCP.Four_Neighbor[2].markValue;
	int direction_3 = lastTCP.Four_Neighbor[3].markValue;
	int visit_mark3 = getVisitMarkValue(lastTCP.center.position, RIGHT);
	int test_direction_3 = getTestCellDirection_MarkValue(lastTCP.center.position, RIGHT);
	determinePictureFrame(mark, lastTCP.center.position, UP, visit_mark0);
	determinePictureFrame(mark, lastTCP.center.position, LEFT, visit_mark1);
	determinePictureFrame(mark, lastTCP.center.position, DOWN, visit_mark2);
	determinePictureFrame(mark, lastTCP.center.position,  RIGHT, visit_mark3);
	int outline = OUTLINE_MARK;
	int empty = EMPTY;
	int sand = mark.at<int>(seed.location);
	if ((((direction_0 == sand)&&(direction_1 == sand) && (direction_2 == sand)) &&(visit_mark1 == VISITED_MARK) &&(visit_mark2 == VISITED_MARK))
		||(visit_mark0!= VISITED_MARK)&&((direction_0 == outline) ||
		(direction_0 == empty && ((direction_1 == sand)||(visit_mark1 == VISITED_MARK)) && (((direction_2 == sand) || (direction_2 == empty))||(visit_mark2 == VISITED_MARK))))
		)
	{
        if (((direction_2 == outline) && (visit_mark2 != VISITED_MARK) && (direction_1 == outline) &&  (direction_0 == outline)))
		{
			direction = FOUR_CELL_NEIGHBOR[2];
			serial = 2;
		}
		else if (((direction_1 == outline) && (visit_mark1 != VISITED_MARK) && (direction_0 == outline)) ||
	            ((direction_1 == empty) && (visit_mark1 != VISITED_MARK) && (visit_mark3 == VISITED_MARK) && (direction_2 == outline) && (direction_0 == outline)))
		{
			direction = FOUR_CELL_NEIGHBOR[1];
			serial = 1;
		}
		else
		{
			direction = FOUR_CELL_NEIGHBOR[0];
			serial = 0;
		}
	}
	else if (
		(((visit_mark0 == VISITED_MARK)&&(direction_2 == sand)&& (visit_mark1 != VISITED_MARK)&&(direction_1!=sand))

		||(visit_mark1 != VISITED_MARK)&&((direction_1 == outline) ||
		((direction_0 == empty && direction_1 == empty && direction_2 == sand) ||
		(direction_0 == sand && direction_1 == empty && direction_2 == empty) ||
		(direction_0 == sand && direction_1 == empty && direction_2 == sand)
		) && ((direction_2 != outline) || (visit_mark2 != VISITED_MARK)))||
		((visit_mark2==VISITED_MARK)&&(visit_mark1!=VISITED_MARK)&&(direction_1!=sand)))		
		&&
		!((visit_mark2!= VISITED_MARK) && (direction_2 == outline) && (visit_mark1 != VISITED_MARK)
			&& (direction_1== outline) && (direction_0== empty))
		)
	{
		if ((direction_3== outline) && (visit_mark3 != VISITED_MARK) && (direction_2 == outline)&& (direction_1 == outline))
		{
			direction = FOUR_CELL_NEIGHBOR[3];
			serial = 3;
		}
		else if (((direction_2 == outline) && (visit_mark2 != VISITED_MARK) && (direction_1 == outline))||
			    ((direction_2 == empty) && (visit_mark2 != VISITED_MARK) && (visit_mark0 == VISITED_MARK) && (direction_1 == outline) && (direction_3 == outline)))
		{
			direction = FOUR_CELL_NEIGHBOR[2];
			serial = 2;
		}
		else
		{
			direction = FOUR_CELL_NEIGHBOR[1];
			serial = 1;
		}
	}
	else if (
		((visit_mark2 != VISITED_MARK)&&(((((direction_0 != outline)||(visit_mark0 == VISITED_MARK)) && ((direction_1 != outline)||(visit_mark1 == VISITED_MARK)) && (direction_2 == outline)))
      ||((visit_mark0 == VISITED_MARK) && (direction_1 == sand) && (direction_2 == empty))))
		||
		((visit_mark2 != VISITED_MARK) && (direction_2 == outline) && (visit_mark1 != VISITED_MARK)
			&& (direction_1 == outline) && (direction_0 == empty))
		)
	{	
		if ((visit_mark3 != VISITED_MARK) && (direction_3 == outline))
		{

			direction = FOUR_CELL_NEIGHBOR[3];
			serial = 3;
		}
		else
		{
			direction = FOUR_CELL_NEIGHBOR[2];
			serial = 2;
		}
	}
	else if( (((visit_mark2 != VISITED_MARK) &&(direction_0 == empty && direction_1 == empty && direction_2 == empty))) && ((direction_3 != outline) || (visit_mark3 == VISITED_MARK))
		||(((visit_mark0 == VISITED_MARK)||(direction_0 == sand))&& ((visit_mark1 == VISITED_MARK) || (direction_1 == sand))
			&& ((visit_mark3 == VISITED_MARK) || (direction_3 == sand)) &&(direction_2==empty) && (visit_mark2 != VISITED_MARK)) ||
			(visit_mark2 != VISITED_MARK) && ((((visit_mark0 == VISITED_MARK) && (direction_0 == outline))&&((visit_mark1 == VISITED_MARK)|| (direction_1 == empty))&&(direction_2 == empty)&&(direction_3 == sand)))
		|| ((visit_mark2 != VISITED_MARK) && (direction_2 == outline) && (direction_1 == outline) && (direction_0 == outline))
		|| ((visit_mark2 != VISITED_MARK) && (visit_mark3 == VISITED_MARK) && (visit_mark0 == VISITED_MARK) && (visit_mark1 == VISITED_MARK))
		)
	{
			if (((direction_3 == empty)|| (direction_3 == outline))&& visit_mark3 != VISITED_MARK &&direction_2==empty&&direction_1==empty&& visit_mark0== VISITED_MARK)
			{
				direction = FOUR_CELL_NEIGHBOR[3];
				serial = 3;
			}
			else
			{
				direction = FOUR_CELL_NEIGHBOR[2];
				serial = 2;
			}
	}
	else if ((visit_mark0!= VISITED_MARK)/*&&(test_direction_3==-1)*/ && (direction_0 != sand))
	{
		direction = FOUR_CELL_NEIGHBOR[0];
		serial = 0;
	}
	else if (((visit_mark2 == VISITED_MARK) || (direction_2 != sand)) && (visit_mark1 != VISITED_MARK)&& (direction_1!= sand) && ((direction_0 == sand) || (visit_mark0 == VISITED_MARK)) && ((direction_3 == sand) || (visit_mark3 == VISITED_MARK)))
	{
	if (direction_0 == empty && direction_1 == empty)
	{
		direction = FOUR_CELL_NEIGHBOR[0];
		serial = 0;
	}
	else
	{
		direction = FOUR_CELL_NEIGHBOR[1];
		serial = 1;
	}
	}
	else if (((direction_2 == sand)||(visit_mark2 == VISITED_MARK))&&
	((direction_1 == sand) || (visit_mark1 == VISITED_MARK))&&(direction_0!=sand)
	&& (visit_mark0 != VISITED_MARK))
	{
	       direction = FOUR_CELL_NEIGHBOR[0];
		   serial = 0;
	}
	else if (((((visit_mark3 != VISITED_MARK) && (direction_3 == outline) && ((direction_2 != outline) || (visit_mark2 == VISITED_MARK)) && ((direction_1 != outline) || (visit_mark1 == VISITED_MARK)) && ((direction_0 != outline) || (visit_mark0 == VISITED_MARK))) && (lastTCP.center.position != beginSearch)) ||
		((visit_mark3 != VISITED_MARK) && (direction_3 == empty) && ((visit_mark2 == VISITED_MARK) || (direction_2 == sand)) && ((visit_mark1 == VISITED_MARK) || (direction_1 == sand)) && ((visit_mark0 == VISITED_MARK) || (direction_0 == sand))))	
		|| (visit_mark3 != VISITED_MARK) && (((direction_0 == sand) || (visit_mark0 == VISITED_MARK)) && ((direction_1 == sand) || (visit_mark1 == VISITED_MARK)) && (direction_3 == empty))
		|| ((visit_mark3 != VISITED_MARK) && (direction_3 == empty) &&
		((direction_0 == outline) || (direction_0 == sand)) && (direction_1 == empty)
		&& (direction_2 == empty))
		)
	{
		direction = FOUR_CELL_NEIGHBOR[3];
		serial = 3;
	}
	else if (((visit_mark0 == VISITED_MARK) || (direction_0 == sand)) && ((visit_mark1 == VISITED_MARK) || (direction_1 == sand)) &&
	((visit_mark2 == VISITED_MARK) || (direction_2 == sand)) && ((visit_mark3 == VISITED_MARK) || (direction_3 == sand)))
	{
	    if (isRightOrient(lastTCP.center.position, searchSeed))
	       {
		      direction = FOUR_CELL_NEIGHBOR[0];
			  serial = 0;
	       }
	     else
	       {
	        	direction = FOUR_CELL_NEIGHBOR[1];
				serial = 1;
	       }
	 }
	else
	{
		std::cout << "Wrong combination of four neighbourhoods" << seed_number_globle << std::endl;
		if (isRightOrient(lastTCP.center.position, searchSeed))
		{
			direction = FOUR_CELL_NEIGHBOR[0];
			serial = 0;
		}
		else
		{
			direction = FOUR_CELL_NEIGHBOR[1];
			serial = 1;
		}
		std::exit(1);
	}
	if ((lastTCP.center.position.x <searchDirection.x1))
	{
		nextTCP.direction = LEFT;
		searchDirection.count = 1;
	}
	renewDirectionRangeXY(directionRangeXY, lastTCP);
	adjustDirection(visit_mark0, visit_mark1, visit_mark2, visit_mark3,
		direction_0, direction_1, direction_2, direction_3,
		outline, empty, sand,
		direction, serial);
	cv::Point center;
	center.x = lastTCP.center.position.x + direction.x;
	center.y = lastTCP.center.position.y + direction.y;
	if (serial == -1)
	{
		std::cout << "void getTestCellPositionUp(TestCellPosition& nextTCP,"
			"TestCellPosition& lastTCP, const cv::Mat& mark, const Seed& seed)" << "输入的序号值为-1，不正确" << std::endl;
		exit(-1);
	}

#ifdef MEASURE_D
	int measureD = cv::pointPolygonTest(polygonPointSet, center, true);
	if ((lastTCP.Four_Neighbor[serial].markValue!= outline))
	{
		if (measureD < OUTLINE_THRESHOLD/*!r.contains(center)*/)
		{
			markPointMark(mark, center, OUTLINE_MARK);
		}
	};
#endif
	TestCellDirection_Mark.at<int>(lastTCP.center.position) = UP;
	try {
		formTestCellPosition(nextTCP, mark, center);
	}
	catch(...)
	{
		throw;
	}
};
void formTestCellPosition(TestCellPosition& nextTCP, const cv::Mat& mark,
	const cv::Point& center, int direction)
{
	nextTCP.direction = direction;
	nextTCP.center.position = center;
	nextTCP.center.markValue = mark.at<int>(center);
	for (int i = 0; i < EIGHT_FOUR_NEIGHBOR; i++)
	{
		PositionValue temp;
		temp.position.x = center.x + FOUR_CELL_NEIGHBOR[i].x;
		temp.position.y = center.y + FOUR_CELL_NEIGHBOR[i].y;
		if (temp.position.x<0 || temp.position.x>mark.cols || temp.position.y<0 || temp.position.y>mark.rows)
		{
			throw std::runtime_error("formTestCellPosition overflow boundaey!");
		}
		temp.markValue = mark.at<int>(temp.position);
		nextTCP.Four_Neighbor.push_back(temp);
	}

};
void getTestCellPositionLEFT(TestCellPosition& nextTCP,
	TestCellPosition& lastTCP,  cv::Mat& mark, const Seed& seed,const std::vector<cv::Point>&polygonPointSet)
{
	int serial = -1;
	cv::Point direction;
	nextTCP.direction = LEFT;
	int visit_mark1 = getVisitMarkValue(lastTCP.center.position, LEFT);
	int visit_mark2 = getVisitMarkValue(lastTCP.center.position, DOWN);
	int visit_mark3 = getVisitMarkValue(lastTCP.center.position, RIGHT);
	int direction_1 = lastTCP.Four_Neighbor[1].markValue;
	int direction_2 = lastTCP.Four_Neighbor[2].markValue;
	int direction_3 = lastTCP.Four_Neighbor[3].markValue;
	const cv::Mat* address = &mark;
	if (address == ad_globel)
		std::cout << "";
	int test_direction_1 = getTestCellDirection_MarkValue(lastTCP.center.position, LEFT);
	int test_direction_2 = getTestCellDirection_MarkValue(lastTCP.center.position, DOWN);
	int test_direction_3 = getTestCellDirection_MarkValue(lastTCP.center.position, RIGHT);
	int direction_0 = lastTCP.Four_Neighbor[0].markValue;
	int visit_mark0 = getVisitMarkValue(lastTCP.center.position, UP);
	int test_direction_0 = getTestCellDirection_MarkValue(lastTCP.center.position, UP);
	determinePictureFrame(mark, lastTCP.center.position, UP, visit_mark0);
	determinePictureFrame(mark, lastTCP.center.position, LEFT, visit_mark1);
	determinePictureFrame(mark, lastTCP.center.position, DOWN, visit_mark2);
	determinePictureFrame(mark, lastTCP.center.position, RIGHT, visit_mark3);
	int outline = OUTLINE_MARK;
	int empty = EMPTY;
	int sand = mark.at<int>(seed.location);
		if (((visit_mark3 == VISITED_MARK)&&(visit_mark2 == VISITED_MARK) && ((direction_1 == sand)&&(direction_2 == sand)/* && (direction_2 == sand)*/)) ||
			(visit_mark1!=VISITED_MARK)&&((direction_1 == outline) ||
		((direction_1 == empty) && ((direction_2 == sand)||(visit_mark2 == VISITED_MARK)) && (((direction_3 == sand)|| (direction_3 == empty)/*下方为空，1也优先*/)||(visit_mark3 == VISITED_MARK))))
			||((direction_2 == sand)&&(direction_1 == sand)&&(visit_mark3 == VISITED_MARK)&&(visit_mark0 == VISITED_MARK)))
	{

           if ((direction_3 == outline) && (visit_mark3 != VISITED_MARK) && (direction_2 == outline)  && (direction_1 == outline))
			{
				direction = FOUR_CELL_NEIGHBOR[3];
				serial = 3;
			}
			else if (((direction_2== outline) && (visit_mark2!= VISITED_MARK) && (direction_1== outline))||
				     ((direction_2== empty) && (visit_mark2 != VISITED_MARK) && (visit_mark0== VISITED_MARK) && (direction_1== outline) && (direction_3 == outline)))
			{
				direction = FOUR_CELL_NEIGHBOR[2];
				serial = 2;
			}
			else
			{
				direction = FOUR_CELL_NEIGHBOR[1];
				serial = 1;
			}
	}
	else if ((((((visit_mark2 != VISITED_MARK) &&(direction_2 != sand)) && (direction_3 == sand) && (visit_mark1 == VISITED_MARK)))||(visit_mark2 != VISITED_MARK) && ((direction_2 == outline) ||/*向左移动*/
		((direction_1 == empty && direction_2 == empty && direction_3 == sand) ||
		(direction_1 == sand && direction_2 == empty && direction_3 == empty) ||
		(direction_1 == sand && direction_2== empty && direction_3== sand)
		) && ((direction_3 != outline) || (visit_mark3 != VISITED_MARK)))||
		((visit_mark3==VISITED_MARK)&&(visit_mark2!=VISITED_MARK)&&(direction_2!=sand))
		)&&
		!((visit_mark3 != VISITED_MARK) && (direction_3 == outline) && (visit_mark2 != VISITED_MARK)
			&& (direction_2 == outline) && (direction_1 == empty)))
	{
			if ((direction_0 == outline) && (visit_mark0 != VISITED_MARK) && (direction_3 == outline)  && (direction_2 == outline))
			{
				direction = FOUR_CELL_NEIGHBOR[0];
				serial = 0;
			}
			else if (((direction_3 == outline) && (visit_mark3 != VISITED_MARK) && (direction_2== outline))||
			        ((direction_3 == empty) && (visit_mark3 != VISITED_MARK) && (visit_mark1 == VISITED_MARK) && (direction_2 == outline) && (direction_0 == outline)))
			{
				direction = FOUR_CELL_NEIGHBOR[3];
				serial = 3;
			}
			else
			{
				direction = FOUR_CELL_NEIGHBOR[2];
				serial = 2;
			}
	}
	else if (
			((visit_mark3 != VISITED_MARK) && (((((visit_mark3 != VISITED_MARK) || visit_mark1 == VISITED_MARK) && ((direction_2 != outline)||(visit_mark2 == VISITED_MARK)) && direction_3 == outline))||
		((visit_mark1 == VISITED_MARK) && (direction_2 == sand) && (direction_3 == empty))))||
		((visit_mark3 != VISITED_MARK) && (direction_3 == outline) && (visit_mark2 != VISITED_MARK)
			&& (direction_2 == outline) && (direction_1 == empty))
		)
	{
			if ((visit_mark0 != VISITED_MARK)&& (direction_0 == outline))
			{

				direction = FOUR_CELL_NEIGHBOR[0];
				serial = 0;
			}
			else
			{
				direction = FOUR_CELL_NEIGHBOR[3];
				serial = 3;
			}
	}
	else if (((visit_mark3 != VISITED_MARK) && (direction_1 == empty && direction_2 == empty && direction_3 == empty)&&(( direction_0 != outline)||(visit_mark0 == VISITED_MARK)))||
			(((visit_mark1 == VISITED_MARK) || (direction_1 == sand)) && ((visit_mark2 == VISITED_MARK)|| (direction_2 == sand)) 
				&& ((visit_mark0 == VISITED_MARK) || (direction_0 == sand)) && ((direction_3 == empty)) && (visit_mark3 != VISITED_MARK))||

			((visit_mark3 != VISITED_MARK) && (((visit_mark1 == VISITED_MARK) && (direction_1 == outline)) &&
		     ((visit_mark2 == VISITED_MARK) || (direction_2 == empty)) && 
				(direction_3 == empty) && (direction_0 == sand)))
		|| ((visit_mark3 != VISITED_MARK) && (visit_mark0 == VISITED_MARK) && (visit_mark1 == VISITED_MARK) && (visit_mark2 == VISITED_MARK))
		)
	{
			if (((direction_0== empty ) || (direction_0 == outline))&& visit_mark0 != VISITED_MARK && direction_2 == empty && direction_3 == empty && visit_mark1 == VISITED_MARK)
			{
				direction = FOUR_CELL_NEIGHBOR[0];
				serial = 0;
			}
			else
			{
				direction = FOUR_CELL_NEIGHBOR[3];
				serial = 3;
			}
	}
	else if ((visit_mark1 != VISITED_MARK) /*&& (test_direction_0 == -1)*/ && (direction_1 != sand))
	{
			direction = FOUR_CELL_NEIGHBOR[1];
			serial = 1;
	}
	else if (((visit_mark3 == VISITED_MARK)|| (direction_3!= sand)) && (visit_mark2 != VISITED_MARK) && (direction_2 != sand) && ((direction_1 == sand) || (visit_mark1 == VISITED_MARK)) && ((direction_0 == sand) || (visit_mark0 == VISITED_MARK)))
	{
	if (direction_1== empty && direction_2 == empty)
	{
		direction = FOUR_CELL_NEIGHBOR[1];
		serial = 1;
	}
	else
	{
		direction = FOUR_CELL_NEIGHBOR[2];
		serial = 2;
	}
	}
	else if (((direction_3 == sand) || (visit_mark3 == VISITED_MARK)) &&
			((direction_2 == sand) || (visit_mark2 == VISITED_MARK)) && (direction_1 != sand)
		&& (visit_mark1 != VISITED_MARK))
	{
			direction = FOUR_CELL_NEIGHBOR[1];
			serial = 1;
	}
	else if ((((visit_mark0 != VISITED_MARK) && (direction_0 == outline) && ((direction_1 != outline) || (visit_mark1 == VISITED_MARK)) && ((direction_2 != outline) || (visit_mark2 == VISITED_MARK)) && ((direction_3 != outline) || (visit_mark3 == VISITED_MARK))) ||
			((visit_mark0 != VISITED_MARK) && (direction_0 == empty) && ((visit_mark2 == VISITED_MARK) || (direction_2 == sand)) && ((visit_mark1 == VISITED_MARK) || (direction_1 == sand)) && ((visit_mark3 == VISITED_MARK) || (direction_3 == sand))))		
			    || (visit_mark0 != VISITED_MARK) && (((direction_1 == sand) || (visit_mark1 == VISITED_MARK)) && ((direction_2 == sand) || (visit_mark2 == VISITED_MARK)) && (direction_0 == empty))
			|| ((visit_mark0 != VISITED_MARK)&& (direction_0 == empty) && 
			((direction_1 == outline)|| (direction_1 == sand))&& (direction_2 == empty)
			&& (direction_3 == empty))
			)
		{
			direction = FOUR_CELL_NEIGHBOR[0];
			serial = 0;
		}
	else if ((visit_mark3 != VISITED_MARK) && (direction_3 == outline) && (direction_2 == outline) && (direction_1 == outline))
	{
	direction = FOUR_CELL_NEIGHBOR[3];
	serial = 3;
	}
	else if ((((visit_mark0 != VISITED_MARK) && (direction_0 == outline) && ((direction_1 != outline) || (visit_mark1 == VISITED_MARK)) && ((direction_2 != outline) || (visit_mark2 == VISITED_MARK)) && ((direction_3 != outline) || (visit_mark3 == VISITED_MARK))) ||
	((visit_mark0 != VISITED_MARK) && (direction_0 == empty) && ((visit_mark2 == VISITED_MARK) || (direction_2 == sand)) && ((visit_mark1 == VISITED_MARK) || (direction_1 == sand)) && ((visit_mark3 == VISITED_MARK) || (direction_3 == sand))))
	/*|| (((direction_1 == sand) || (visit_mark1 == VISITED_MARK)) && ((direction_2 == sand) || (visit_mark2 == VISITED_MARK)) && (direction_0 == empty))*/
	)
	{
	direction = FOUR_CELL_NEIGHBOR[0];
	serial = 0;
	}
	else if (((visit_mark0 == VISITED_MARK) || (direction_0 == sand)) && ((visit_mark1 == VISITED_MARK) || (direction_1 == sand)) &&
	((visit_mark2 == VISITED_MARK) || (direction_2 == sand)) && ((visit_mark3 == VISITED_MARK) || (direction_3 == sand)))
	{
	     if (isRightOrient(lastTCP.center.position, searchSeed))
	      {
		     direction = FOUR_CELL_NEIGHBOR[2];
			 serial = 2;
	       }
	      else
	       {
		    direction = FOUR_CELL_NEIGHBOR[1];
			serial = 1;
        	}
	}
	else
	{
	if (isRightOrient(lastTCP.center.position, searchSeed))
	{
		direction = FOUR_CELL_NEIGHBOR[2];
		serial = 2;
	}
	else
	{
		direction = FOUR_CELL_NEIGHBOR[1];
		serial = 1;
	}
		std::cout << "Wrong combination of four neighbourhoods" << seed_number_globle<<std::endl;
		std::exit(1);
	}
		if (lastTCP.center.position.y > searchDirection.y2)
		{
			nextTCP.direction = DOWN;
			searchDirection.count = 2;
		}
	renewDirectionRangeXY(directionRangeXY, lastTCP);
	adjustDirection(visit_mark0, visit_mark1, visit_mark2, visit_mark3,
		direction_0, direction_1, direction_2, direction_3,
		outline, empty, sand,
		direction, serial);
	cv::Point center;
	center.x = lastTCP.center.position.x + direction.x;
	center.y = lastTCP.center.position.y + direction.y;
	if (serial == -1)
	{
		std::cout << "void getTestCellPositionLEFT(TestCellPosition& nextTCP,"
			"TestCellPosition& lastTCP, const cv::Mat& mark, const Seed& seed)" << "输入的序号值为-1，不正确" << std::endl;
		exit(-1);
	}
#ifdef MEASURE_D
	int measureD = cv::pointPolygonTest(polygonPointSet, center, true);
	if ((lastTCP.Four_Neighbor[serial].markValue !=outline))
	{
		if (measureD < OUTLINE_THRESHOLD/*!r.contains(center)*/)
		{
			markPointMark(mark, center, OUTLINE_MARK);
		}
	};
#endif
	TestCellDirection_Mark.at<int>(lastTCP.center.position) = LEFT;
	try{
	formTestCellPosition(nextTCP, mark, center);
	}
	catch (...)
	{
		throw;
	}
};
void getTestCellPositionDOWN(TestCellPosition& nextTCP,
	TestCellPosition& lastTCP,  cv::Mat& mark, const Seed& seed,const std::vector<cv::Point>&polygonPointSet)
{
	int serial = -1;
	cv::Point direction;
	nextTCP.direction = DOWN;
	int test_direction_0 = getTestCellDirection_MarkValue(lastTCP.center.position, UP);
	int test_direction_3 = getTestCellDirection_MarkValue(lastTCP.center.position, RIGHT);
	int test_direction_2 = getTestCellDirection_MarkValue(lastTCP.center.position, DOWN);
	if ((lastTCP.center.position.y < directionRangeXY.y_range.end))
	{
		lastTCP.direction = DOWN;
		TestCellDirection_Mark.at<int>(lastTCP.center.position) = DOWN;
		if ((lastTCP.center.position.y  < directionRangeXY.y_range.end)
			&& (test_direction_0 != -1))
		{
			test_direction_0 = DOWN;
		}
		if ((lastTCP.center.position.y + INCREMENT_CELL < directionRangeXY.y_range.end)
			&& (test_direction_3 != -1))
		{
			test_direction_3 = DOWN;
		}
		if ((lastTCP.center.position.y  < directionRangeXY.y_range.end)
			&& (test_direction_2 != -1))
		{
			test_direction_2 = DOWN;
		}
	}
	int visit_mark2 = getVisitMarkValue(lastTCP.center.position, DOWN);
	int visit_mark3 = getVisitMarkValue(lastTCP.center.position, RIGHT);
	int visit_mark0 = getVisitMarkValue(lastTCP.center.position, UP);
	int direction_2 = lastTCP.Four_Neighbor[2].markValue;
	int direction_3 = lastTCP.Four_Neighbor[3].markValue;
	int direction_0 = lastTCP.Four_Neighbor[0].markValue;
	int direction_1 = lastTCP.Four_Neighbor[1].markValue;
	int visit_mark1 = getVisitMarkValue(lastTCP.center.position, LEFT);
	int test_direction_1 = getTestCellDirection_MarkValue(lastTCP.center.position, LEFT);
	determinePictureFrame(mark, lastTCP.center.position, UP, visit_mark0);
	determinePictureFrame(mark, lastTCP.center.position, LEFT, visit_mark1);
	determinePictureFrame(mark, lastTCP.center.position, DOWN, visit_mark2);
	determinePictureFrame(mark, lastTCP.center.position, RIGHT, visit_mark3);
	int outline = OUTLINE_MARK;
	int empty = EMPTY;
	int sand = mark.at<int>(seed.location);
	if (((visit_mark0 == VISITED_MARK) && (visit_mark3 == VISITED_MARK)&& (direction_2 == sand)&&(direction_0 == sand) && (direction_3 == sand)) || (visit_mark2!=VISITED_MARK)&&((direction_2 == outline) ||
		(direction_2 == empty && ((direction_3 == sand)||(visit_mark3 == VISITED_MARK)) && (((direction_0 == sand) || (direction_0 == empty)/*右方为空，左也优先*/)||(visit_mark0 == VISITED_MARK))))
		/*||(direction_0 == sand && direction_3 == empty && direction_2 == sand)*/||
		(direction_3 == sand && direction_2 == sand&& (visit_mark1 == VISITED_MARK)&& (visit_mark0 == VISITED_MARK))
		)
	{
		//if (direction_2 == outline)
		//{
		//	nextTCP.direction = test_direction_2;
		//}
/*		if ((direction_1 == outline) && (visit_mark1 != VISITED_MARK) && (direction_0 == outline)  && (direction_3 == outline) && (direction_2 == outline))
		{
			direction = FOUR_CELL_NEIGHBOR[1];
			serial = 1;
		}
		else */ if((direction_0 == outline) && (visit_mark0 != VISITED_MARK) && (direction_3 == outline) && (direction_2 == outline))
		{
			direction = FOUR_CELL_NEIGHBOR[0];
			serial = 0;
		}
		else if (((direction_3== outline) && (visit_mark3 != VISITED_MARK) && (direction_2 == outline))||
		     	((direction_3 == empty) && (visit_mark3 != VISITED_MARK) && (visit_mark1 == VISITED_MARK) && (direction_2 == outline) && (direction_0 == outline)))
		{
			direction = FOUR_CELL_NEIGHBOR[3];
			serial = 3;
		}
		else
		{
			direction = FOUR_CELL_NEIGHBOR[2];
			serial = 2;
		}

	}
	else if (((((visit_mark2 == VISITED_MARK) && (direction_0 == sand) && ((visit_mark3 != VISITED_MARK) && (direction_3 != sand))))
		||(visit_mark3 != VISITED_MARK) && ((direction_3 == outline) ||/*向右移动*/
		((direction_2 == empty && direction_3 == empty && direction_0 == sand) ||
		(direction_2 == sand && direction_3 == empty && direction_0 == empty) ||
		(direction_2 == sand && direction_3 == empty && direction_0 == sand)
		) && ((direction_0 != outline) || (visit_mark0 != VISITED_MARK)))||
		((visit_mark0==VISITED_MARK)&&(visit_mark3!=VISITED_MARK)&&(direction_3!=sand))
		)&&
	    !((visit_mark0 != VISITED_MARK)&&(direction_0 == outline)&& (visit_mark3 != VISITED_MARK) 
			&& (direction_3 == outline)&&(direction_2==empty)))
	{
		if ((direction_1 == outline) && (visit_mark1 != VISITED_MARK) && (direction_0 == outline)  && (direction_3 == outline))
		{
			direction = FOUR_CELL_NEIGHBOR[1];
			serial = 1;
		}
		else if (((direction_0 == outline )&& (visit_mark0 != VISITED_MARK) && (direction_3 == outline))||
			     ((direction_0 == empty) && (visit_mark0 != VISITED_MARK) && (visit_mark2 == VISITED_MARK) && (direction_3 == outline) && (direction_1 == outline)))
		{
			direction = FOUR_CELL_NEIGHBOR[0];
			serial = 0;
		}
		else
		{
			direction = FOUR_CELL_NEIGHBOR[3];
			serial = 3;
		}
	}
	else if (((visit_mark0 != VISITED_MARK) && (((((direction_2 != outline)||(visit_mark2 == VISITED_MARK)) && ((direction_3 != outline)||(visit_mark3 == VISITED_MARK)) &&( direction_0 == outline)))||
		((visit_mark2 == VISITED_MARK)&& (direction_3 == sand)&&(direction_0 == empty))))||
		((visit_mark0 != VISITED_MARK) && (direction_0 == outline) && (visit_mark3 != VISITED_MARK)
			&& (direction_3 == outline) && (direction_2 == empty)))
	{
		if ((visit_mark1 != VISITED_MARK) && (direction_1 == outline))
		{
			direction = FOUR_CELL_NEIGHBOR[1];
			serial = 1;
		}
		else
		{
			direction = FOUR_CELL_NEIGHBOR[0];
			serial = 0;
		}
	}
	else if (((visit_mark0 != VISITED_MARK) && (direction_2 == empty && direction_3 == empty && direction_0 == empty) && ((direction_1 != outline)|| (visit_mark1 == VISITED_MARK)))
		|| (((visit_mark2 == VISITED_MARK)|| (direction_2 == sand))&& ((visit_mark3 == VISITED_MARK) || (direction_3 == sand))
			&& ((visit_mark1 == VISITED_MARK) || (direction_1 == sand)) &&((direction_0==empty)) && (visit_mark0 != VISITED_MARK))||
			(visit_mark0 != VISITED_MARK) && ((((visit_mark2== VISITED_MARK) && (direction_2 == outline)) &&
		((visit_mark3 == VISITED_MARK) || (direction_3 == empty)) &&
				(direction_0== empty) && (direction_1 == sand)))||
				((visit_mark0 != VISITED_MARK) && (direction_0 == outline) && (direction_3 == outline) && (direction_2 == outline))
				||((visit_mark0 != VISITED_MARK)&& (visit_mark1 == VISITED_MARK) && (visit_mark2 == VISITED_MARK) && (visit_mark3 == VISITED_MARK))
		)
	{
			if (((direction_1 == empty)|| (direction_1 == outline)) && visit_mark1 != VISITED_MARK && direction_3 == empty && direction_0 == empty && visit_mark2 == VISITED_MARK)
			{
				direction = FOUR_CELL_NEIGHBOR[1];
				serial = 1;
			}
			else
			{
				direction = FOUR_CELL_NEIGHBOR[0];
				serial = 0;
			}
	}
	else if ((visit_mark2 != VISITED_MARK) /*&& (test_direction_1 == -1)*/ && (direction_2 != sand))
	{
		direction = FOUR_CELL_NEIGHBOR[2];
		serial = 2;
	}
	else if ((visit_mark3 != VISITED_MARK) && (direction_3 != sand) && ((visit_mark0 == VISITED_MARK)|| (direction_0 != sand)) && ((direction_1 == sand) || (visit_mark1 == VISITED_MARK)) && ((direction_2 == sand) || (visit_mark2 == VISITED_MARK)))
	{
		if (direction_3==empty&&direction_0==empty)
		{
			direction = FOUR_CELL_NEIGHBOR[0];
			serial = 0;
		}
		else
		{
			direction = FOUR_CELL_NEIGHBOR[3];
			serial = 3;
		}
	}
	else if (((direction_0 == sand) || (visit_mark0 == VISITED_MARK)) &&
		((direction_3 == sand) || (visit_mark3 == VISITED_MARK)) && (direction_2 != sand)
		&& (visit_mark2 != VISITED_MARK))
	{
		direction = FOUR_CELL_NEIGHBOR[2];
		serial = 2;
	}
	else if ((((visit_mark1 != VISITED_MARK) && (direction_1 == outline) && ((direction_2 != outline) || (visit_mark2 == VISITED_MARK)) && ((direction_3 != outline) || (visit_mark3 == VISITED_MARK)) && ((direction_0 != outline) || (visit_mark0 == VISITED_MARK))) ||
		((visit_mark1 != VISITED_MARK) && (direction_1 == empty) && ((visit_mark2 == VISITED_MARK) || (direction_2 == sand)) && ((visit_mark0 == VISITED_MARK) || (direction_0 == sand)) && ((visit_mark3 == VISITED_MARK) || (direction_3 == sand))))
	

       //|| (visit_mark1 != VISITED_MARK) && (((direction_2 == sand) || (visit_mark2== VISITED_MARK)) && ((direction_3 == sand) || (visit_mark3 == VISITED_MARK)) && ((direction_0 == sand) || (visit_mark0 == VISITED_MARK))&& (direction_1 == empty))
		             || (visit_mark1 != VISITED_MARK) && (((direction_2 == sand) || (visit_mark3 == VISITED_MARK)) && ((direction_3 == sand) || (visit_mark3 == VISITED_MARK)) && (direction_1 == empty))
					//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%8-14,7:18(END)			
		|| ((visit_mark1 != VISITED_MARK) && (direction_1 == empty) &&
		((direction_2 == outline) || (direction_2 == sand)) && (direction_3== empty)
		&& (direction_0 == empty))
		)
	{
		direction = FOUR_CELL_NEIGHBOR[1];
		serial = 1;
	}
	else if (((visit_mark0 == VISITED_MARK) || (direction_0 == sand)) && ((visit_mark1 == VISITED_MARK) || (direction_1 == sand)) &&
	((visit_mark2 == VISITED_MARK) || (direction_2 == sand)) && ((visit_mark3 == VISITED_MARK) || (direction_3 == sand)))
	{
		if (isRightOrient(lastTCP.center.position, searchSeed))
		{
			direction = FOUR_CELL_NEIGHBOR[2];
			serial = 2;
		}
		else
		{
			direction = FOUR_CELL_NEIGHBOR[3];
			serial = 3;
		}
	}
	else
	{
		if (isRightOrient(lastTCP.center.position, searchSeed))
		{
			direction = FOUR_CELL_NEIGHBOR[2];
			serial = 2;
		}
		else
		{
			direction = FOUR_CELL_NEIGHBOR[3];
			serial = 3;
		}
		std::cout << "Wrong combination of four neighbourhoods" << seed_number_globle << std::endl;
		std::exit(1);
	}
	//if ((((visit_mark1 != VISITED_MARK) && (direction_1 == outline) && ((direction_2 != outline) || (visit_mark2 == VISITED_MARK)) && ((direction_3 != outline) || (visit_mark3 == VISITED_MARK)) && ((direction_0 != outline) || (visit_mark0 == VISITED_MARK))) ||
	//((visit_mark1 != VISITED_MARK) && (direction_1 == empty) && ((visit_mark2 == VISITED_MARK) || (direction_2 == sand)) && ((visit_mark0 == VISITED_MARK) || (direction_0 == sand)) && ((visit_mark3 == VISITED_MARK) || (direction_3 == sand))))
	///*|| (((direction_2 == sand) || (visit_mark2 == VISITED_MARK)) && ((direction_3 == sand) || (visit_mark3 == VISITED_MARK)) && (direction_1 == empty))*/
	if (lastTCP.center.position.x > searchDirection.x3)
	{
		nextTCP.direction = RIGHT;
		searchDirection.count = 3;
	}
	adjustDirection(visit_mark0, visit_mark1, visit_mark2, visit_mark3,
		direction_0, direction_1, direction_2, direction_3,
		outline, empty, sand,
		direction, serial);
	cv::Point center;
	center.x = lastTCP.center.position.x + direction.x;
	center.y = lastTCP.center.position.y + direction.y;
	if (serial == -1)
	{
		std::cout << "void getTestCellPositionDOWN(TestCellPosition& nextTCP,"
			"TestCellPosition& lastTCP, const cv::Mat& mark, const Seed& seed)" << "The serial number value entered is -1, which is incorrect" << std::endl;
		exit(-1);
	}
#ifdef MEASURE_D
	int measureD = cv::pointPolygonTest(polygonPointSet, center, true);
	if ((lastTCP.Four_Neighbor[serial].markValue !=outline))
	{
		if (measureD < OUTLINE_THRESHOLD/*!r.contains(center)*/)
		{
			markPointMark(mark, center, OUTLINE_MARK);
		}
	};
#endif
	TestCellDirection_Mark.at<int>(lastTCP.center.position) = DOWN;
	try{
	formTestCellPosition(nextTCP, mark, center);
	}
	catch (...)
	{
		throw;
	}
};
void getTestCellPositionRIGHT(TestCellPosition& nextTCP,
	TestCellPosition& lastTCP,  cv::Mat& mark, const Seed& seed, const std::vector<cv::Point>& polygonPointSet)
{
	int serial = -1;
	cv::Point direction;
	nextTCP.direction = RIGHT;
	int visit_mark3 = getVisitMarkValue(lastTCP.center.position, RIGHT);
	int visit_mark0 = getVisitMarkValue(lastTCP.center.position, UP);
	int visit_mark1 = getVisitMarkValue(lastTCP.center.position, LEFT);
	int direction_3 = lastTCP.Four_Neighbor[3].markValue;
	int direction_0 = lastTCP.Four_Neighbor[0].markValue;
	int direction_1 = lastTCP.Four_Neighbor[1].markValue;
	int test_direction_0 = getTestCellDirection_MarkValue(lastTCP.center.position, UP);
	int test_direction_1 = getTestCellDirection_MarkValue(lastTCP.center.position, LEFT);
	int test_direction_3 = getTestCellDirection_MarkValue(lastTCP.center.position,RIGHT);
	int direction_2 = lastTCP.Four_Neighbor[2].markValue;
	int visit_mark2 = getVisitMarkValue(lastTCP.center.position, DOWN);
	int test_direction_2 = getTestCellDirection_MarkValue(lastTCP.center.position, DOWN);
	determinePictureFrame(mark, lastTCP.center.position, UP, visit_mark0);
	determinePictureFrame(mark, lastTCP.center.position, LEFT, visit_mark1);
	determinePictureFrame(mark, lastTCP.center.position, DOWN, visit_mark2);
	determinePictureFrame(mark, lastTCP.center.position, RIGHT, visit_mark3);
	int outline = OUTLINE_MARK;
	int empty = EMPTY;
	int sand = mark.at<int>(seed.location);
	if ((( visit_mark0 == VISITED_MARK) &&(visit_mark1 == VISITED_MARK)&& (direction_3 == sand)&& (direction_1 == sand) && (direction_0 == sand)) || (visit_mark3!=VISITED_MARK)&&((direction_3 == outline) ||
		(direction_3 == empty && ((direction_0 == sand)||(visit_mark0 == VISITED_MARK)) && (((direction_1 == sand) || (direction_1 == empty)/*上方为空，下方优先*/)||(visit_mark1 == VISITED_MARK))))
		||(direction_0 == sand && direction_3 == sand &&(visit_mark2 == VISITED_MARK)&&(visit_mark1 == VISITED_MARK)))
	{
/*		if ((direction_2 == outline) && (visit_mark2 != VISITED_MARK) && (direction_1 == outline) && (direction_0 == outline) && (direction_3 == outline))
		{
			direction = FOUR_CELL_NEIGHBOR[2];
			serial = 2;
		}
		else*/ if (((direction_1 == outline) && (visit_mark1 != VISITED_MARK) && (direction_0 == outline)  && (direction_3 == outline)))
		{
			direction = FOUR_CELL_NEIGHBOR[1];
			serial = 1;
		}
		else if (((direction_0 == outline) && (visit_mark0 != VISITED_MARK) && (direction_3 == outline)) ||
	((direction_0 == empty) && (visit_mark0 != VISITED_MARK) && (visit_mark2 == VISITED_MARK) && (direction_1 == outline) && (direction_3 == outline)))
		{
			direction = FOUR_CELL_NEIGHBOR[0];
			serial = 0;
		}
		else
		{
			direction = FOUR_CELL_NEIGHBOR[3];
			serial = 3;
		}
	}
	else if ((((((visit_mark0 != VISITED_MARK) && (direction_0 != sand)) && (direction_1 == sand) && (visit_mark3 == VISITED_MARK))) ||(visit_mark0 != VISITED_MARK) && ((direction_0 == outline) ||/*向右移动*/
		((direction_3 == empty && direction_0 == empty && direction_1 == sand) ||
		(direction_3 == sand && direction_0 == empty && direction_1== empty) ||
		(direction_3 == sand && direction_0 == empty && direction_1 == sand)
		))&&((direction_1!=outline)||( visit_mark1 != VISITED_MARK))||
		((visit_mark1==VISITED_MARK)&&(visit_mark0!=VISITED_MARK)&&(direction_0!=sand))
		)&&
		!((visit_mark1 != VISITED_MARK) && (direction_1 == outline) && (visit_mark0 != VISITED_MARK)
			&& (direction_0 == outline) && (direction_3 == empty)))
	{
		//if (direction_0 == outline)
		//{
		//	nextTCP.direction = test_direction_0;
		//}
		if ((direction_2 == outline) && (visit_mark2 != VISITED_MARK) && (direction_1 == outline) && (direction_0 == outline))
		{
			direction = FOUR_CELL_NEIGHBOR[2];
			serial = 2;
		}
		else if (((direction_1== outline) && (visit_mark1 != VISITED_MARK) && (direction_0== outline))||
			((direction_1 == empty) && (visit_mark1 != VISITED_MARK) && (visit_mark3 == VISITED_MARK) && (direction_0 == outline) && (direction_2== outline)))
		{
			direction = FOUR_CELL_NEIGHBOR[1];
			serial = 1;
		}
		else
		{
			direction = FOUR_CELL_NEIGHBOR[0];
			serial = 0;
		}
	}
	else if( 
		((visit_mark1 != VISITED_MARK) && (((((direction_3 != outline)||(visit_mark3 == VISITED_MARK)) && ((direction_0 != outline)||(visit_mark0 == VISITED_MARK)) && direction_1 == outline))
		||((visit_mark3 == VISITED_MARK) && (direction_0 == sand) && (direction_1 == empty))))||

		((visit_mark1 != VISITED_MARK) && (direction_1 == outline) && (visit_mark0 != VISITED_MARK)
			&& (direction_0 == outline) && (direction_3 == empty))
		)
	{
		//if (direction_1 == outline)
		//{
		//	nextTCP.direction = test_direction_1;
		//}
		if ((visit_mark2 != VISITED_MARK) && (direction_2== outline))
		{

			direction = FOUR_CELL_NEIGHBOR[2];
			serial = 2;
		}
		else
		{
			direction = FOUR_CELL_NEIGHBOR[1];
			serial = 1;
		}

	}
	else if (((visit_mark1 != VISITED_MARK) && (direction_3 == empty && direction_0 == empty && direction_1 == empty)&&((direction_2 != outline)||(visit_mark2 == VISITED_MARK)))
		|| (((visit_mark3 == VISITED_MARK) || (direction_3 == sand)) && ((visit_mark0 == VISITED_MARK) || (direction_0 == sand))
			&&((visit_mark2 == VISITED_MARK) || (direction_2 == sand)) && ((direction_1 == empty)) && (visit_mark1 != VISITED_MARK))||

			(visit_mark1 != VISITED_MARK) && ((((visit_mark3 == VISITED_MARK) && (direction_3 == outline)) &&
		((visit_mark0 == VISITED_MARK) || (direction_0 == empty)) &&
				(direction_1 == empty) && (direction_2 == sand)))||
				((visit_mark1 != VISITED_MARK) && (direction_1 == outline) && (direction_3 == outline) && (direction_0 == outline))
		|| ((visit_mark1 != VISITED_MARK) && (visit_mark2 == VISITED_MARK) && (visit_mark3 == VISITED_MARK) && (visit_mark0 == VISITED_MARK))
		)
	{
	/*	if ((lastTCP.center.position.x>=directionRangeXY.x_range.end)||
			(lastTCP.center.position.y<searchDirection.y0))
		{*/
			if (((direction_2 == empty)|| (direction_2 == outline)) && visit_mark2!= VISITED_MARK && direction_0 == empty && direction_1 == empty && visit_mark3 == VISITED_MARK)
			{
				direction = FOUR_CELL_NEIGHBOR[2];
				serial = 2;
			}
			else
			{
				direction = FOUR_CELL_NEIGHBOR[1];
				serial = 1;
			}
	}
	else if ((visit_mark3 != VISITED_MARK) /*&& (test_direction_2 == -1)*/&& (direction_3 !=sand ))
	{
		direction = FOUR_CELL_NEIGHBOR[3];
		serial = 3;
	}
	else if ((visit_mark0 != VISITED_MARK) && (direction_0 != sand) &&((visit_mark1 == VISITED_MARK)|| (direction_1 != sand))&&((direction_2 == sand)||(visit_mark2 == VISITED_MARK))&& ((direction_3 == sand) || (visit_mark3 == VISITED_MARK)))
	{
	if (direction_3 == empty && direction_0 == empty)
	{
		direction = FOUR_CELL_NEIGHBOR[1];
		serial = 1;
	}
	else
	{
		direction = FOUR_CELL_NEIGHBOR[0];
		serial = 0;
	}
	}
	else if (((direction_1 == sand) || (visit_mark1 == VISITED_MARK)) &&
		((direction_0== sand) || (visit_mark0 == VISITED_MARK)) && (direction_3 != sand)
		&& (visit_mark3 != VISITED_MARK))
	{
		direction = FOUR_CELL_NEIGHBOR[3];
		serial = 3;
	}
	else if (((visit_mark2 != VISITED_MARK) && (direction_2 == outline) && ((direction_3 != outline) || (visit_mark3 == VISITED_MARK)) && ((direction_1 != outline) || (visit_mark1 == VISITED_MARK)) && ((direction_0 != outline) || (visit_mark0 == VISITED_MARK))) ||
		((visit_mark2 != VISITED_MARK) && (direction_2 == empty) && ((visit_mark3 == VISITED_MARK) || (direction_3 == sand)) && ((visit_mark1 == VISITED_MARK) || (direction_1 == sand)) && ((visit_mark0 == VISITED_MARK) || (direction_0 == sand)))
		
		//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%8-14,7:18(BEIGNG)	
		//|| (visit_mark2 != VISITED_MARK) && (((direction_3 == sand) || (visit_mark3 == VISITED_MARK)) && ((direction_0 == sand) || (visit_mark0 == VISITED_MARK)) && ((direction_1 == sand) || (visit_mark1 == VISITED_MARK)) && (direction_2== empty))
		|| (visit_mark2 != VISITED_MARK) && (((direction_0 == sand) || (visit_mark0 == VISITED_MARK)) && ((direction_3 == sand) || (visit_mark3 == VISITED_MARK)) && (direction_2 == empty))
		//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%8-14,7:18(END)	
		
		|| ((visit_mark2 != VISITED_MARK) && (direction_2 == empty) &&
		((direction_3 == outline) || (direction_3 == sand)) && (direction_0 == empty)
		&& (direction_1 == empty))
		)
	{
		direction = FOUR_CELL_NEIGHBOR[2];
		serial = 2;
	}
	else if (((visit_mark0 == VISITED_MARK) || (direction_0 == sand)) && ((visit_mark1 == VISITED_MARK) || (direction_1 == sand)) &&
		((visit_mark2 == VISITED_MARK) || (direction_2 == sand)) && ((visit_mark3 == VISITED_MARK) || (direction_3 == sand)))
	{
		if (isRightOrient(lastTCP.center.position, searchSeed))
		{
			direction = FOUR_CELL_NEIGHBOR[0];
			serial = 0;
		}
		else
		{
			direction = FOUR_CELL_NEIGHBOR[3];
			serial = 3;
		}
	}
	else
	{
		if (isRightOrient(lastTCP.center.position, searchSeed))
		{
			direction = FOUR_CELL_NEIGHBOR[0];
			serial = 0;
		}
		else
		{
			direction = FOUR_CELL_NEIGHBOR[3];
			serial = 3;
		}
		std::cout << "Wrong combination of four neighbourhoods" << seed_number_globle << std::endl;
		std::exit(1);
	}
	renewDirectionRangeXY(directionRangeXY, lastTCP);
	if (lastTCP.center.position.y < searchDirection.y0)
	{
		nextTCP.direction = UP;
		searchDirection.count = 4;
	}
	adjustDirection(visit_mark0, visit_mark1, visit_mark2, visit_mark3,
		direction_0, direction_1, direction_2, direction_3,
		outline, empty, sand,
		direction, serial);
	cv::Point center;
	center.x = lastTCP.center.position.x + direction.x;
	center.y = lastTCP.center.position.y + direction.y;
	if (serial == -1)
	{
		std::cout << "void getTestCellPositionRight(TestCellPosition& nextTCP,"
			"TestCellPosition& lastTCP, const cv::Mat& mark, const Seed& seed)" << "The serial number value entered is -1, which is incorrect" << std::endl;
		exit(-1);
	}

#ifdef MEASURE_D
	int measureD = cv::pointPolygonTest(polygonPointSet, center, true);
	if ((lastTCP.Four_Neighbor[serial].markValue !=outline)/*|| measureD < 0*/)
	{
		if (measureD < OUTLINE_THRESHOLD/*!r.contains(center)*/)
		{
			markPointMark(mark, center, OUTLINE_MARK);			
		}
	};
#endif
	TestCellDirection_Mark.at<int>(lastTCP.center.position) = RIGHT;
	try{
	formTestCellPosition(nextTCP, mark, center);
	}
	catch (...)
	{
		throw;
	}
};
void setMarkZero(cv::Mat& mark)
{
	for (int i = 0; i < mark.rows; i++)
	{
		for (int j = 0; j < mark.cols; j++)
		{
			if (mark.at<int>(i, j) == OUTLINE_MARK)
			{
				mark.at<int>(i, j) = EMPTY;
			}
		}
	}

};
void formSandPicture(cv::Mat& sand, const std::vector<Cell>& cellSet,const cv::Mat&mark, cv::Mat& colorSandGrain)
{
	cv::Mat HSVpicture;
	cv::cvtColor(colorSandGrain, HSVpicture, cv::COLOR_RGB2HSV);
	int minRows= cellSet[0].position.y, maxRows= cellSet[0].position.y;
	int minCols= cellSet[0].position.x, maxCols= cellSet[0].position.x;
	int count = cellSet.size();
	for (int i = 0; i < count; i++)
	{
	   cv::Point center= cellSet[i].position;
	   if (minRows > center.y)
	   {
		  minRows = center.y;
	   }
	   if (maxRows < center.y)
	   {
		  maxRows = center.y;
	   }
	   if (minCols > center.x)
	   {
		  minCols = center.x;
	   }
	   if (maxCols < center.x)
	   {
		  maxCols = center.x;
	   }
	}
	minRows -= 1;
	minCols -= 1;
	maxRows += 1;
	maxCols += 1;
	int rows = maxRows - minRows + 1;
	int cols = maxCols - minCols + 1;
	cv::Vec3b  sandColorR = randomColor();
	sand = cv::Mat(rows, cols, CV_8UC3, cv::Vec3b(100,255,100));
	int cell_count = 0;
	for (int i = 0; i < cellSet.size(); i++)
	{
		cell_count++;
		copyCellToPicture(sand, cellSet[i],minRows,minCols, colorSandGrain, sandColorR);
	}
	cv::imwrite("C:\\work\\science\\Digital_particle_size_identification_methods\\work\\newOutPicture\\sand.JPG", sand);
	int count_cell = 0;
	for (int i = 0; i < sand.cols; i++)
	{
		for (int j = 0; j < sand.rows; j++)
		{
			int x = i;
			int y = j;
			cv::Point p(x, y);
			cv::Point p_image(minCols, minRows);
			cv::Point pp(minCols + x, minRows + y);
			if (sand.at<cv::Vec3b>(p) == cv::Vec3b(100,255,100))
			{
				bool is_in_sand = is_inSand( sand,p, cv::Vec3b(100, 255, 100));
				if (is_in_sand)
				{
					sand.at<cv::Vec3b>(p) = HSVpicture.at<cv::Vec3b>(pp);
					colorSandGrain.at<cv::Vec3b>(pp) = sandColorR;
					count_cell++;
				}		
			}
		}
	}
	std::cout << "count_cell=" << count_cell << std::endl;
	cv::Mat sand_HSV;
	sand.copyTo(sand_HSV);
	cv::cvtColor(sand_HSV, sand, cv::COLOR_HSV2RGB);
};
void copyCellToPicture(cv::Mat& sand, const Cell& cell, int minRows, int minCols, cv::Mat& sandColorPicture, cv::Vec3b& color)
{
	cv::Point position = cell.position;
	cv::Point picture_location= cell.position;
	const cv::Mat& ce = cell.cell;
	position.x -= minCols;
	position.y -= minRows;
	cv::Point pix(1,1);
	int count_i = 0;
	for (int i = 0; i < EIGHT_NEIGHBOR_NUMBER; i++)
	{
		sandColorPicture.at<cv::Vec3b>(picture_location + EIGHT_NEIGHBOR_CELL[i]) = color;
		sand.at<cv::Vec3b>(position + EIGHT_NEIGHBOR_CELL[i]) = ce.at<cv::Vec3b>(pix +EIGHT_NEIGHBOR_CELL[i]);
	}
};
void setSimilar(Similar& sandSimilar, const std::vector<Cell>& cellSet)
{
	sandSimilar = CellSimilar(cellSet[0], cellSet[1]);
};
void getNeighbor(const cv::Mat& colorPicture, std::vector<Cell>& cellSet
	, const Seed& seed, int outline_number, SeedSet& sSet,const int markNumber)
{
	int seedNumber = (2 * outline_number + 1) * (2 * outline_number + 1);
	int colNumber =  2 * outline_number + 1;
	int rowNumber = colNumber;
	cv::Point center = seed.location;
	cv::Point top_left,bottom_right;
	top_left.x = center.x - outline_number * 3;
	top_left.y = center.y - outline_number * 3;
	bottom_right.x = center.x + outline_number * 3;
	bottom_right.y = center.y + outline_number * 3;
	directionRangeXY.x_range.start = top_left.x;
	directionRangeXY.y_range.start = top_left.y;
	directionRangeXY.x_range.end = center.x + outline_number * 3;
	directionRangeXY.y_range.end = center.y + outline_number * 3;
	for (int i = 0; i < rowNumber; i++)
	{
		for (int j = 0; j < colNumber; j++)
		{
			cv::Point newPoint;
			newPoint.x = top_left.x + i* INCREMENT_CELL;
			newPoint.y = top_left.y +j* INCREMENT_CELL;
			Cell newCell = fromPointToCell(colorPicture, newPoint);
			cellSet.push_back(newCell);
		}
	}
#ifdef GET_NEIGHBOR_FIRST
	cv::Mat rectangle_outline;
	std::string new_string_address = address_global;
	colorPicture.copyTo(rectangle_outline);
	cv::rectangle(rectangle_outline,top_left,bottom_right,cv::Scalar(125,125,125),5);
	new_string_address += "trainData\\";
	if (0 != ::_access(new_string_address.c_str(), 0))
	{
		_mkdir(new_string_address.c_str());   
	}
	new_string_address +="glo_plus_" + std::to_string(seed_number_globle) + ".JPG";
	cv::imwrite(new_string_address,rectangle_outline);
#endif
};
int getVisitMarkValue(const cv::Point& center, const int direction)
{
	int visit_mark=0;
	cv::Point new_center(center);
		new_center += FOUR_CELL_NEIGHBOR[direction];
	visit_mark = TestCellPosition_Mark.at<int>(new_center);
	return visit_mark;
};
void InitializationTestCellDirection_Mark(cv::Mat& testCellDirection_Mark)
{
	cv::Mat& mark = testCellDirection_Mark;

	int rows = mark.rows;
	int cols = mark.cols/*-BORDER_WIDTH*/;
	mark = cv::Mat::ones(rows, cols, CV_32S) * -1;
	for (int r = BORDER_WIDTH; r < rows; r++)
	{
		for (int c = cols - BORDER_WIDTH; c < cols; c++)
		{
			mark.at<int>(r, c) = UP;
		}
	}
	for (int r = 0; r < BORDER_WIDTH; r++)
	{
		for (int c = BORDER_WIDTH; c < cols; c++)
		{
			mark.at<int>(r, c) = LEFT;
		}
	}
	for (int r = 0; r < rows - BORDER_WIDTH; r++)
	{
		for(int c=0;c< BORDER_WIDTH;c++)
		{
			mark.at<int>(r, c) = DOWN;
		}
	}
	for (int r = rows - BORDER_WIDTH; r < rows; r++)
	{
		for (int c = 0; c < cols - BORDER_WIDTH; c++)
		{
			mark.at<int>(r, c) = RIGHT;
		}
	}
};
void setTestCellDirection_MarkZero(cv::Mat& TestCellDirection_Mark)
{
	cv::Mat& mark = TestCellDirection_Mark;
	int rows = TestCellDirection_Mark.rows;
	int cols = TestCellDirection_Mark.cols;
	for (int r = BORDER_WIDTH; r < rows - BORDER_WIDTH; r++)
	{
		for (int c = BORDER_WIDTH; c < cols - BORDER_WIDTH; c++)
		{
			if (mark.at<int>(r, c) != -1)
			{
				mark.at<int>(r, c) = -1;
			}
		}
	}
};
int  getTestCellDirection_MarkValue(cv::Point& center, int direction)
{
	cv::Point pt=center;
	pt += FOUR_CELL_NEIGHBOR[direction];
	if (pt.x<0 || pt.x>TestCellDirection_Mark.cols || pt.y<0 || pt.y>TestCellDirection_Mark.rows)
	{
		throw std::runtime_error("getTestCellDirection_MarkValue overflow boundary!");
	}
	return TestCellDirection_Mark.at<int>(pt);

};
void renewDirectionRangeXY(DirectionRangeXY& directionRangeXY, const TestCellPosition& lastTCP)
{
	const cv::Point& pt = lastTCP.center.position;
	cv::Range& x_range = directionRangeXY.x_range;
	cv::Range& y_range = directionRangeXY.y_range;
	if (x_range.start > pt.x)
	{
		x_range.start = pt.x;
	}
	if (x_range.end < pt.x)
	{
		x_range.end = pt.x;
	}
	if (y_range.start > pt.y)
	{
		y_range.start = pt.y;
	}
	if(y_range.end<pt.y)
	{
		y_range.end = pt.y;
	}
};
void formTempCellSet(const cv::Mat& colorPicture/*彩色原图*/, cv::Mat& mark
	, std::vector<Cell>& tempCellSet, std::vector<Cell>& cellSet
	, int search_end_test, const Seed& seed
#ifdef WRITE_EXCEL
	,xlnt::worksheet&dataSheet,xlnt::workbook&dataBook
#endif
	, const std::vector<cv::Point>& polygonPointSet)
{	
	try{
	tempCellSet.clear();
	TestCellPosition firstTCP, nextTCP, lastTCP;
	searchSeed = seed.location;
	int countNumber = 0;
	getFirstTestCellPosition(firstTCP, mark, seed,  TestCellPosition_Mark);
	setSearchDirection(searchDirection, searchSeed);
	searchDirection.count = 0;
	nextTCP = firstTCP;
	beginSearch = firstTCP.center.position;
	first_mark = 1;
	cv::Point position = seed.location;
	int markValue = mark.at<int>(position);
	int ij = 0;
	int count_i = 0;
	do
	{
		countNumber++;
		if (countNumber > SEARCH_MAX_NUMBER)
		{
			throw std::runtime_error("Reach upper limit!");
		}
	/*	if (count_i++ > 0.1 * END_SEARCH_TESTING_NUMBER)
		{
			tempCellSet.clear();
			return;
		}*/
		renew_searchSeed(searchSeed,  directionRangeXY);
		setSearchDirection(searchDirection, searchSeed);
		if ((nextTCP.center.position.y<firstTCP.center.position .y) && (searchDirection.count == 4))
		{
			break;
		}
		lastTCP = nextTCP;
		seed_number_globle;
		Seed s;
		seed_number_globle;
		cv::Point s_p = seed.location;
#ifdef WRITE_EXCEL
		ij++;
		if (
			lastTCP.center.position.x == POSITION_X && lastTCP.center.position.y == POSITION_Y && test_0 >= TEST_1/* && seed_number_globle == 2182*/)
		{
			std::cout << "ij=" << ij << std::endl;
			dataBook.save("C://work//PictureRecognize//PictureRecognize//PictureRecognize//picture.xlsx");
			exit(-1);
		}
		//std::cout << "[" << lastTCP.center.position.x << " " << lastTCP.center.position.y << "]" << std::endl;
		output << "[" << lastTCP.center.position.x << " " << lastTCP.center.position.y << "]" << std::endl;
		 if (lastTCP.center.position.x == 531  && lastTCP.center.position.y ==4831&& test_0 >= 26582)
		 {
			 dataBook.save("C://work//PictureRecognize//PictureRecognize//PictureRecognize//picture.xlsx");
			 std::cout << "";

		 }
#endif
		 //*****************************************
		/* jji++;
		 if (lastTCP.center.position.x == 531 && lastTCP.center.position.y == 4831 && test_0 >= 26582)
		 {*/
#ifdef WRITE_EXCEL
			 /*dataBook.save("C://work//PictureRecognize//PictureRecognize//PictureRecognize//picture.xlsx");*/
#endif
		/*	 exit(-1);*/
	/*		 std::cout << "jji="<<jji<<std::endl;
		 }*/
		 /*int aaa = mark.at<int>(seed.location);*/
		if (lastTCP.center.markValue == OUTLINE_MARK || lastTCP.center.markValue == sandMarkNumber
			|| cv::pointPolygonTest(polygonPointSet, lastTCP.center.position, true)<0)
		{
			if (cv::pointPolygonTest(polygonPointSet, lastTCP.center.position, true) < 0)
			{
				markPointMark(mark, lastTCP.center.position, OUTLINE_MARK);
			}
			getNextTestCellPosition(nextTCP, lastTCP
				, mark, seed/*,cv::Mat& TestCellPositionMark*/, polygonPointSet);
			if (is_visitedFourDirection(TestCellPosition_Mark, position))
			{
				break;
			}
			markPointMark(TestCellPosition_Mark, lastTCP.center.position, VISITED_MARK);
#ifdef WRITE_EXCEL
			if ( test_0 >= TEST_1 && seed_number_globle == SAND_NUMBER)
			{

				writeCellPositionAndAttributeAndDirection_partial(dataSheet,
					lastTCP.center.position, searchDirection.count, lastTCP.center.markValue, ij);
				/*dataBook.save("C://work//PictureRecognize//PictureRecognize//PictureRecognize//picture.xlsx");*/
			}
#endif
			//************************************************
		}
		else /*if (lastTCP.center.markValue == EMPTY)*/
		{
#ifdef WRITE_EXCEL
			if (test_0 >= TEST_1 && seed_number_globle == SAND_NUMBER)
			{

				writeCellPositionAndAttributeAndDirection_partial(dataSheet,
					lastTCP.center.position, searchDirection.count, ij);
				/*dataBook.save("C://work//PictureRecognize//PictureRecognize//PictureRecognize//picture.xlsx");*/
			}
#endif
			//************************************************
			Cell tempCell = fromPointToCell(colorPicture, lastTCP.center.position);;
			tempCellSet.push_back(tempCell);
			getNextTestCellPosition(nextTCP, lastTCP
				, mark, seed/*,cv::Mat& TestCellPositionMark*/,  polygonPointSet);
			if (is_visitedFourDirection(TestCellPosition_Mark, position))
			{
				break;
			}
				markPointMark(TestCellPosition_Mark, lastTCP.center.position, VISITED_MARK);

		}
	if (searchDirection.count >= 2)
	{
		markPointMark(TestCellPosition_Mark, beginSearch, EMPTY);
	}
	} while ((nextTCP.center.position != firstTCP.center.position)
		&&!((nextTCP.center.position.y == searchDirection.y0) && (searchDirection.count == 4))
		);

	for (int i = 0; i < tempCellSet.size(); i++)
	{
		output << "[" << tempCellSet[i].position.y << " " << tempCellSet[i].position.x << "]" << std::endl;
		/*std::cout << "[" << tempCellSet[i].position.y << " " << tempCellSet[i].position.x << "]" << std::endl;*/
	}
#ifdef WRITE_EXCEL
	if ( test_0 >= TEST_1 && seed_number_globle == SAND_NUMBER)
	{
		dataBook.save("C://work//PictureRecognize//PictureRecognize//PictureRecognize//picture.xlsx");
	}
#endif
		}
		catch (...)
		{
			throw;
		}
};
void drawMarkAtColorPicture(cv::Mat& colorPicture, const cv::Mat& mark)
{
	int cols = mark.cols;
	int rows = mark.rows;
	int count = 0;
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			if (mark.at<int>(i, j) == -1)
			{
				count++;
				colorPicture.at < cv::Vec3b >(i, j) = cv::Vec3b{ 255,0,0 };
			}
		}
	}
	std::cout << "count:" << count << std::endl;
}
bool isRightOrient(const cv::Point& position, const cv::Point& seedPosition)
{
	int x_val, y_val;
	bool orient;
	x_val = abs(position.x - seedPosition.x);
	y_val = abs(position.y - seedPosition.y);
	if (x_val > y_val)
	{
		orient = true;
	}
	else
	{
		orient = false;
	}
	return orient;
}
void setSearchDirection(SearchDirection& searchDirection, cv::Point& seed)
{
	searchDirection.y0 = seed.y;
    searchDirection.x1 = seed.x;
    searchDirection.y2 = seed.y;
    searchDirection.x3 = seed.x; 
}
void renew_searchSeed(cv::Point& searchSeed, DirectionRangeXY& directionrangeXY)
{
	int x, y;
	x = (directionrangeXY.x_range.start + directionrangeXY.x_range.end) * 0.5;
	y = (directionrangeXY.y_range.start+directionrangeXY.y_range.end ) * 0.5;
	searchSeed = cv::Point(x, y);
};
std::vector<AdjustPosition> adjustBoundaryMark(const cv::Mat&colorPicture,cv::Mat& mark, const int sandMarkNumber,std::vector<Cell>&cellSet,const cv::Rect& minRect)
{
	if ( seed_number_globle == 1092 && test_0 == 1081)
	{
		std::cout << "";
	}
	int  cc=0;
	int value;
	int count_4;
	int count_8;
	std::vector<AdjustPosition> AdSet;
	AdjustPosition AdPosition;
	cv::Point pt;
	bool isRealBoundary ;
	int i = (cellSet[0].position.y) % INCREMENT_CELL;
	for (; i < mark.rows; i += INCREMENT_CELL)
	{
		if (cellSet.size() == 0)
		{
			break;
		}
		int j = (cellSet[0].position.x) % INCREMENT_CELL;
		for (; j < mark.cols; j+= INCREMENT_CELL)
		{
			if (!minRect.contains(cv::Point(j,i)))
			{
				continue;
			}
			count_4 = 0;
			count_8 = 0;
			pt = cv::Point(j, i);
			value = mark.at<int>(pt);
			cc++;		
			if (seed_number_globle == 1081 && test_0 == 1384&&cc==361)
			{	
				std::cout << "cc=" << cc << std::endl;
			}
			if (value == OUTLINE_MARK)
			{
				for (int k = 0; k < EIGHT_NEIGHBOR; k++)
				{
					if (mark.at<int>(pt + EIGHT_CELL_NEIGHBOR[k]) == sandMarkNumber)
					{
						if (k % 2 == 0)
						{
							count_4++;
						}
						else
						{
							count_8++;
						}
					}
				}
			   if (count_4==0&& count_8==0)
			   {
				  markPointMark(mark, pt, EMPTY);
				  markPointMark(TestCellPosition_Mark, pt, EMPTY);
				  AdPosition.position = pt;
				  AdPosition.attribution = EMPTY;
				  AdSet.push_back(AdPosition);
			   }
			   else if((count_4== FOUR_SAND_NUMBER)/*||((count_4==2)&&(count_8>=3))*/)
			   {
				   markPointMark(mark, pt, sandMarkNumber);
				   AdPosition.position = pt;
				   AdPosition.attribution = sandMarkNumber;
				   AdSet.push_back(AdPosition);
				   Cell tempC=fromPointToCell(colorPicture,pt);
				   cellSet.push_back(tempC);
			   }
			}
			else if (value == sandMarkNumber)
			{
				for (int k = 0; k < EIGHT_NEIGHBOR; k++)
				{
					if (mark.at<int>(pt + EIGHT_CELL_NEIGHBOR[k]) == OUTLINE_MARK/*|| mark.at<int>(pt + EIGHT_CELL_NEIGHBOR[k]) == EMPTY*/)
					{
						if (k % 2 == 0)
						{
							count_4++;
						}
						else
						{
							count_8++;
						}
					}
				}
				if (count_4 >= FOUR_SAND_NUMBER/*|| ((count_4 == 2) && (count_8 == 4))*/)
				{
					markPointMark(mark, pt, OUTLINE_MARK);
					AdPosition.position = pt;
					AdPosition.attribution = OUTLINE_MARK;
					AdSet.push_back(AdPosition);
					std::vector<Cell>::const_iterator it =  fromCellSetfindIterator(cellSet,  pt);
					if (it != cellSet.end())
					{
						cellSet.erase(it);
					}
				}
			}
		}
	}
	return AdSet;
};
void writeInformationToBookdata(const cv::Mat& mark,xlnt::workbook&dataBook, xlnt::worksheet& dataSheet
	, const std::vector<Cell>& tempCellSet, const cv::Mat& TestCellDirection_Mark)
{
	int size = tempCellSet.size();
	int value;
	int dire;
	cv::Point position;
	for (int i = 0; i < size; i++)
	{
		position = tempCellSet[i].position;
		value = mark.at<int>(position);
		if (value != EMPTY)
		{
			dire = TestCellDirection_Mark.at<int>(position);
			writeCellPositionAndAttributeAndDirection(dataSheet,
				position, dire
				, value);
		}
	}
	/*dataBook.save("C://work//PictureRecognize//PictureRecognize//PictureRecognize//picture.xlsx");*/
};
void pushbackCellSet(std::vector<Cell>& cellSet, const std::vector<Cell>& tempCellSet, const cv::Mat& mark, const int sandMarkNumber)
{
	int size = tempCellSet.size();
	int value;
	cv::Point position;
	for (int i = 0; i < size; i++)
	{
		position = tempCellSet[i].position;
		value = mark.at<int>(position);
		if (value == sandMarkNumber)
		{
			cellSet.push_back(tempCellSet[i]);
		}
	}
};
void  adjuestMarkAndCellSet(const std::vector<AdjustPosition>& AdSet, xlnt::workbook& dataBook
	, xlnt::worksheet& dataSheet, cv::Mat& mark, std::vector<Cell>& cellSet)
{
	int size = AdSet.size();
	for (int i = 0; i < size; i++)
	{
		renewWorkSheet( AdSet[i], dataBook	,  dataSheet);
	}
	dataBook.save("C://work//PictureRecognize//PictureRecognize//PictureRecognize//picture.xlsx");
};
 std::vector<Cell>::iterator fromCellSetfindIterator(std::vector<Cell> & cellSet, const cv::Point & pt)
{
	 std::vector<Cell>::iterator it = cellSet.begin();
	 for (it = cellSet.begin(); it != cellSet.end(); it++)
	 {
		 if ((*it).position == pt)
		 {
			 break;
		 }
	 }
	 return it;
 };
 bool isContinueTempCellSet(const std::vector<Cell>& preSet, const std::vector<Cell>& tempCellSet)
 {
	 if (preSet.size() != tempCellSet.size())
	 {
		 return false;
	 }
	 else
	 {
		 int size = preSet.size();
		 for (int i = 0; i < size; i++)
		 {
			 if (preSet[i].position != tempCellSet[i].position)
			 {
				 return false;
			 }
		 }
		 return true;
	 }
 };
 int getSandRadius(const std::vector<Cell>& cellSet, const cv::Mat& mark)
 {
	 int sum = 0;
	 int count = 0;
	 for (int i=0; i < SAND_RADIUS_NUMBER;i++ )
	 {
		 sum += getSingleSandRadius(cellSet, mark, count);
	 }
	 if (count == 0)
	 {
		 return 0;
	 }
	 else
	 {
		 return sum / count;
	 }
 
 };
 int getSingleSandRadius(const std::vector<Cell>& cellSet, const cv::Mat& mark,int&count)
 {
	 cv::RNG numberRng = cv::theRNG();
	 int count_i = 0;
	 int radius = 0;
	 while (count_i++<cellSet.size())
	 {
		 int number = numberRng.uniform(0, cellSet.size());
		 cv::Point pt = cellSet[number].position;
		 if (getSandRadiusValue(pt,mark,radius,  count))
		 {
			 break;
		 }
	 }
	 return radius;
 };
 bool getSandRadiusValue(cv::Point& pt, const cv::Mat& mark, int& radius, int& count)
 {
	 int rows = mark.rows;
	 int cols = mark.cols;
	 cv::Point x_plus, x_sub;
	 bool result=false;
	 for (int i = 0; i < EIGHT_FOUR_NEIGHBOR; i++)
	 { 
		 x_plus = x_sub = pt;
		 do
		 {
			 if ((x_plus.x >= 0 && x_plus.x < cols) && (OUTLINE_MARK != mark.at<int>(x_plus)))
			 {
				 x_plus += EIGHT_CELL_NEIGHBOR[0+i];
			 }
			 if ((x_sub.y >= 0 && x_sub.y < rows) && (OUTLINE_MARK != mark.at<int>(x_sub)))
			 {
				 x_sub += EIGHT_CELL_NEIGHBOR[4+i];
			 }

		 } while (((x_plus.x < 0 && x_plus.x >= cols) || (OUTLINE_MARK == mark.at<int>(x_plus))) &&
			 ((x_sub.y < 0 && x_sub.y >= rows) || (OUTLINE_MARK == mark.at<int>(x_sub))));
		 if ((OUTLINE_MARK == mark.at<int>(x_plus)) && (OUTLINE_MARK == mark.at<int>(x_sub)))
		 {
			 count++;
			 radius += (int)(sqrt((x_plus.x - x_sub.x) ^ 2 + (x_plus.y - x_sub.y) ^ 2));
			 result = true;
		 };
	 }
	 return result;
 };
 void getPolygonPoint(cv::Point& seedP, const cv::Mat& outline, std::vector<cv::Point>& polygonPointSet)
 {
	 int rows = outline.rows;
	 int cols = outline.cols;
	 cv::Point xy_plus;

	 for (int i = 0; i < EIGHT_NEIGHBOR; i++)
	 {
		 xy_plus  = seedP;
		 while (/*(xy_plus.x > 0 && xy_plus.x < cols) &&*/ (EMPTY !=(int)outline.at<uchar>(xy_plus)))	
		 {
				 xy_plus += EIGHT_CELL_NEIGHBOR[i]/3;
		 } 
		 polygonPointSet.push_back(xy_plus);
	 }
 };
 bool isLocateLineDown(const cv::Point& firsP, const cv::Point& secondP, const cv::Point& center)
 {
	 int value;
	 int x_1, y_1, x_2, y_2, x_c, y_c;
	 x_1 = firsP.x;
	 y_1 = firsP.y;
	 x_2 = secondP.x;
	 y_2 = secondP.y;
	 x_c = center.x;
	 y_c = center.y;
	 value = (x_c - x_2) / (x_1 - x_2) * y_1 + (x_c - x_1) / (x_2 - x_1) * y_2 - y_c;
	 return value > 0;
 };
 void expansionPolygonPointSet(int expansionSeedNumber, const cv::Mat& oldOutline, std::vector<cv::Point>& polygonPointSet, std::vector<cv::Point>& newPositionSeed)
 {
	 std::vector<cv::Point>zeroSet, /*decideSet,*/singleSet;
	 cv::RNG seedRng = cv::theRNG();
	 std::vector<cv::Point> orignal_polygonPointSet = polygonPointSet;
	 cv::RotatedRect rotatedR= cv::minAreaRect(orignal_polygonPointSet);
	 cv::Point2f pts[4] = {};
	 rotatedR.points(pts);
	 std::vector<cv::Point> rotatedRectFourPoint;
	 for (int i = 0; i < 4; i++)
	 {
		 rotatedRectFourPoint.push_back(pts[i]);
	 }
	 polygonPointSet.clear();
	 zeroSet.clear();
	 zeroSet = orignal_polygonPointSet;
	 cv::Size pictureSize(oldOutline.cols, oldOutline.rows);
	 //decideSet.clear();
	 int count_1 = 0;
	 int count_2 = 0;
	 for (int i = 0; i < expansionSeedNumber; i++)
	 {
		 cv::Point position;
		/* zeroSet=decideSet*/ /*= polygonPointSet*/;
		 do
		 {
			 count_2++;
			 do
			 {
				 count_1++;
				 singleSet.clear();
				  RandPoint( seedRng ,  rotatedR, rotatedRectFourPoint,  position, oldOutline);
				 if (count_1> SEARCH_MAX_NUMBER)
				 {
					 break;
				 }
			 } while (cv::pointPolygonTest(zeroSet,position,false)==-1);
			 if (count_2> SEARCH_MAX_NUMBER)
			 {
				 break;
			 }
			  if ((oldOutline.at<uchar>(position) != EMPTY))
			  {
#ifdef GET_PLOYGON_SIXTEEN_POINT
				  getPolygonSixTeenPoint(position, oldOutline, singleSet);
#else
				  getPolygonPoint(position, oldOutline, singleSet);
#endif				 
				  if (cv::isContourConvex(singleSet))
				  {
					  deleteNewUntrustPoint(position, singleSet, POP_CONTROL_POINT_NUMBER/*, pictureSize*/);
					  for (int i = 0; i < singleSet.size()/*-POP_CONTROL_POINT_NUMBER*/; i++)
					  {
						  zeroSet.push_back(singleSet[i]);	  
					  }
					  break;
					  
				  }
				  else
				  {
					  continue;
				  }
			  }
		 } while (true);
		 newPositionSeed.push_back(position);
	 }
	 zeroSet.clear();
	 polygonPointSet.clear();
	 deleteUntrustPointInZeroSet(oldOutline, newPositionSeed);
	 for (int i = 0; i < newPositionSeed.size(); i++)
	 {
		 singleSet.clear();
#ifdef GET_PLOYGON_SIXTEEN_POINT
		 getPolygonSixTeenPoint(newPositionSeed[i], oldOutline, singleSet);
#else
		 getPolygonPoint(position, oldOutline, singleSet);
#endif	
		 deleteNewUntrustPoint(newPositionSeed[i], singleSet, POP_CONTROL_POINT_NUMBER/*, pictureSize*/);
		 for (int i = 0; i < singleSet.size()/*-POP_CONTROL_POINT_NUMBER*/; i++)
		 {
			 zeroSet.push_back(singleSet[i]);
		 }
	 }
	 cv::convexHull(zeroSet, polygonPointSet);
 };
 void deleteUntrustPoint(const cv::Point& position, std::vector<cv::Point>& singleSet)
 {
	 for (int i = 0; i < singleSet.size(); i++)
	 {
		 for (int j = i+1; j < singleSet.size(); j++)
		 {
			 if (compareDistance(position, singleSet[i], singleSet[j]))
			 {
				 std::swap(singleSet[i], singleSet[j]);
			 }
		 }
	 }
	 std::vector<cv::Point> getFillEllipseRotatedRect(const std::vector<cv::Point> & singleSet, int deleteNumber);
	 for (int i = 0; i < POP_CONTROL_POINT_NUMBER; i++)
	 {
		 singleSet.pop_back();
	 }
 };
 bool compareDistance(const cv::Point& position, const cv::Point& lh, const cv::Point& rh)
 {
	 int leftD = (position.x - lh.x)* (position.x - lh.x) + (position.y - lh.y)* (position.y - lh.y);
	 int rightD = (position.x-rh.x)* (position.x - rh.x) +(position.y-rh.y)*(position.y - rh.y);
	 return leftD > rightD;
 };
 void cullUnrealContour(cv::Mat& mark)
 {
	 int  rows, cols;
	 rows = mark.rows - BORDER_WIDTH * 2;
	 cols = mark.cols - BORDER_WIDTH * 2;
	 cv::Range X_range, Y_range;
	 findMinMaxRange(mark, X_range, Y_range);

	 for (int i = Y_range.start; i < Y_range.end; i++)
	 {
		 for (int j = X_range.start;j <X_range.end; j++)
		 {
			 cv::Point p(j, i);
			 if ((mark.at<int>(p) == OUTLINE_MARK)&&(!is_realOutline(mark,p,X_range,Y_range)))
			 {
				 mark.at<int>(p) = EMPTY;
			 }
		 }
	 }
 };
 bool is_realOutline(const cv::Mat& mark, const cv::Point& p,cv::Range&X_range,cv::Range&Y_range)
 {
	 int negative_x, positive_x, negative_y, positive_y;
	 bool real, negative_xb, positive_xb, negative_yb, positive_yb;
	 real = negative_xb = positive_xb = negative_yb = positive_yb = false;
	 negative_x = p.x -1;
	 positive_x = p.x+1;
	 negative_y = p.y - 1;
	 positive_y = p.y+1;
	 for (; negative_x >= X_range.start; negative_x--)
	 {
		 cv::Point pp(negative_x, p.y);
		 if (mark.at<int>(pp) == OUTLINE_MARK)
		 {
			 negative_xb = false;
			 break;
		 }
	 }
	 if (negative_x < X_range.start)
	 {
		 return true;
	 }
	 for (; positive_x < X_range.end; positive_x++)
	 {
		 cv::Point pp(positive_x, p.y);
		 if (mark.at<int>(pp) == OUTLINE_MARK)
		 {
			 positive_xb = false;
			 break;
		 }
	 }
	 if (positive_x >= X_range.end)
	 {
		 return true;
	 }
	 for (; negative_y >= Y_range.start; negative_y--)
	 {
		 cv::Point pp(p.x, negative_y);
		 if (mark.at<int>(pp) == OUTLINE_MARK)
		 {
			 negative_yb = false;
			 break;
		 }
	 }
	 if (negative_y < Y_range.start)
	 {
		 return true;
	 }
	 for (; positive_y < Y_range.end; positive_y++)
	 {
		 cv::Point pp(p.x, positive_y);
		 if (mark.at<int>(pp) == OUTLINE_MARK)
		 {
			 positive_yb = false;
			 break;
		 }
	 }
	 if (positive_y >= Y_range.end)
	 {
		 return true;
	 }
	 real = negative_xb && positive_xb && negative_yb && positive_yb;
	 return real;
 };
 void fromMarkToSand(const cv::Mat& mark, cv::Mat& colorSandGrain, cv::Mat& bgr_sand)
 {
	 cv::Vec3b  sandColorR = randomColor();
	 std::vector<SandPixel> spSet;
	 int  rows, cols;
	 rows = mark.rows - BORDER_WIDTH * 2;
	 cols = mark.cols - BORDER_WIDTH * 2;
	 cv::Range X_range,Y_range;
	 findMinMaxRange(mark,  X_range,  Y_range);
	 for (int i = Y_range.start; i < Y_range.end; i++)
	 {
		 for (int j = X_range.start; j < X_range.end; j++)
		 {
			 cv::Point p(j, i);
			 cv::Vec3b sandC;
			 if ( is_inSand(mark, p,X_range,Y_range))
			 {
				 sandC = colorSandGrain.at<cv::Vec3b>(p);
				 spSet.push_back(setSandPixel(p, sandC));
				 colorSandGrain.at<cv::Vec3b>(p) = sandColorR;
			 }
		 }
	 }
	 bgr_sand= formSandPixel(spSet);
 };
 cv::Vec3b randomColor()
 {

	 uchar  b, g, r;
	 b=seedRngColor.uniform(0, 255);
	 g= seedRngColor.uniform(0, 255);
	 r = seedRngColor.uniform(0, 255);
	 cv::Vec3b color(b, g, r);
	 return color;
 };
 SandPixel  setSandPixel(const cv::Point& position, const cv::Vec3b& sColor)
 {
	 SandPixel sp;
	 sp.position = position;
	 sp.sColor = sColor;
	 return sp;
 };
 cv::Mat formSandPixel(const std::vector<SandPixel>& spSet)
 {
	 int minRows = spSet[0].position.y, maxRows = spSet[0].position.y;
	 int minCols = spSet[0].position.x, maxCols = spSet[0].position.x;
	 int count = spSet.size();
	 for (int i = 0; i < count; i++)
	 {
		 cv::Point center = spSet[i].position;
		 if (minRows > center.y)
		 {
			 minRows = center.y;
		 }
		 if (maxRows < center.y)
		 {
			 maxRows = center.y;
		 }
		 if (minCols > center.x)
		 {
			 minCols = center.x;
		 }
		 if (maxCols < center.x)
		 {
			 maxCols = center.x;
		 }
	 }
	 minRows -= 1;
	 minCols -= 1;
	 maxRows += 1;
	 maxCols += 1;
	 int rows = maxRows - minRows + 1;
	 int cols = maxCols - minCols + 1;
	 cv::Mat sand = cv::Mat(rows, cols, CV_8UC3,cv::Vec3b(255,0,0));
	 int cell_count = 0;
	 for (int i = 0; i < spSet.size(); i++)
	 {
		 cell_count++;
		 cv::Point p = spSet[i].position;
		 p.x -= minCols;
		 p.y -= minRows;
		 sand.at<cv::Vec3b>(p)[0] = spSet[i].sColor[0];
		 sand.at<cv::Vec3b>(p)[1] = spSet[i].sColor[1];
		 sand.at<cv::Vec3b>(p)[2] = spSet[i].sColor[2];
	 }
	 return sand;

 };
 void findMinMaxRange(const cv::Mat& mark, cv::Range& X_range, cv::Range& Y_range)
 {
	 if(test_0==29092)
	 std::cout << "test_0=" << test_0 << std::endl;
	 std::vector<cv::Point> pSet;
	 findOutlineSet(mark,  pSet);
	 if (pSet.size() == 0)
	 {
		 X_range = cv::Range(0, 0);
		 Y_range = cv::Range(0, 0);
		 return;
	 }
	 int minRows = pSet[0].y, maxRows = pSet[0].y;
	 int minCols = pSet[0].x, maxCols = pSet[0].x;
	 int count = pSet.size();
	 for (int i = 0; i < count; i++)
	 {
		 cv::Point center = pSet[i];
		 if (minRows > center.y)
		 {
			 minRows = center.y;
		 }
		 if (maxRows < center.y)
		 {
			 maxRows = center.y;
		 }
		 if (minCols > center.x)
		 {
			 minCols = center.x;
		 }
		 if (maxCols < center.x)
		 {
			 maxCols = center.x;
		 }
	 }
	 minRows -= 1;
	 minCols -= 1;
	 maxRows += 1;
	 maxCols += 1;
	 X_range = cv::Range(minCols, maxCols);
	 Y_range = cv::Range(minRows, maxRows);
 };
 void findOutlineSet(const cv::Mat& mark, std::vector<cv::Point>& pSet)
 {
	 int  rows, cols;
	 rows = mark.rows - BORDER_WIDTH * 2;
	 cols = mark.cols - BORDER_WIDTH * 2;
	 for (int i = BORDER_WIDTH; i < cols; i++)
	 {
		 for (int j = BORDER_WIDTH; j < rows; j++)
		 {
			 cv::Point p(i,j);
			 if (mark.at<int>(p) == OUTLINE_MARK)
			 {
				 pSet.push_back(p);
			 }
		 }
	 }
	  
 
 };
 bool is_inSand(const cv::Mat& mark, const cv::Point& p, const cv::Range& X_range, const cv::Range& Y_range)
 {
	 int negative_x, positive_x, negative_y, positive_y;
	 bool real, negative_xb, positive_xb, negative_yb, positive_yb;
	 real = negative_xb = positive_xb = negative_yb = positive_yb = false;
	 negative_x = p.x - 1;
	 positive_x = p.x+1;
	 negative_y = p.y -1;
	 positive_y = p.y+1;
	 int count_in_sand = 0;
	 for (; negative_x >= X_range.start; negative_x--)
	 {
		 cv::Point pp(negative_x, p.y);
		 if (mark.at<int>(pp) == OUTLINE_MARK)
		 {
			 negative_xb = true;
			 break;
		 }
	 }
	 for (; positive_x < X_range.end; positive_x++)
	 {
		 cv::Point pp(positive_x, p.y);
		 if (mark.at<int>(pp) == OUTLINE_MARK)
		 {
			 positive_xb = true;
			 break;
		 }
	 }
	 for (; negative_y >= Y_range.start; negative_y--)
	 {
		 cv::Point pp(p.x, negative_y);
		 if (mark.at<int>(pp) == OUTLINE_MARK)
		 {
			 negative_yb = true;
			 break;
		 }
	 }
	 for (; positive_y < Y_range.end; positive_y++)
	 {
		 cv::Point pp(p.x, positive_y);
		 if (mark.at<int>(pp) == OUTLINE_MARK)
		 {
			 positive_yb = true;
			 break;
		 }
	 }
	 real = negative_xb && positive_xb && negative_yb && positive_yb;
	 return real;
 };
 bool is_inSand(const cv::Mat& sand, const cv::Point& p,const cv::Vec3b&color)
 {
	 int negative_x, positive_x, negative_y, positive_y;

	 bool real, negative_xb, positive_xb, negative_yb, positive_yb;
	 real = negative_xb = positive_xb = negative_yb = positive_yb = false;
	 negative_x = p.x - 1;
	 positive_x = p.x + 1;
	 negative_y = p.y - 1;
	 positive_y = p.y + 1;
	 for (; negative_x > 0; negative_x--)
	 {
		 cv::Point pp(negative_x, p.y);
		 if (sand.at<cv::Vec3b>(pp) != color)
		 {
			 negative_xb = true;
			 break;
		 }
	 }
	 for (; positive_x < sand.cols; positive_x++)
	 {
		 cv::Point pp(positive_x, p.y);
		 if (sand.at<cv::Vec3b>(pp) != color)
		 {
			 positive_xb = true;
			 break;
		 }
	 }
	 for (; negative_y > 0; negative_y--)
	 {
		 cv::Point pp(p.x, negative_y);
		 if (sand.at<cv::Vec3b>(pp) != color)
		 {
			 negative_yb = true;
			 break;
		 }
	 }
	 for (; positive_y <sand.rows; positive_y++)
	 {
		 cv::Point pp(p.x, positive_y);
		 if (sand.at<cv::Vec3b>(pp) != color)
		 {
			 positive_yb = true;
			 break;
		 }
	 }
	 real = negative_xb && positive_xb && negative_yb && positive_yb;
	 return real;
 };
 cv::Point findNextPosition(const TestCellPosition& lastTCP, const std::vector<cv::Point>& polygonPointSet, const cv::Point& center, const cv::Point& seed_location)
 {

	 cv::Point previous,next;
	 int pre_value, nex_value;
	 int i = 0;
	 for (; i < 8; i++)
	 {
		 int visit_mark = getVisitMarkValue_8(center, i);
		 if (visit_mark != VISITED_MARK)
		 {
			 previous = center + EIGHT_CELL_NEIGHBOR[i];
			 pre_value = abs(previous.x - seed_location.x) + abs(previous.y - seed_location.y);
			 break;
		 }
	 }
	 if (i >= 8)
	 {
		 return center;
	 }
	 for (; i < 8; i++)
	 {

		 next = center + EIGHT_CELL_NEIGHBOR[i];
		 int visit_mark = getVisitMarkValue_8(center,  i);
		 nex_value = abs(next.x-seed_location.x)+abs(next.y-seed_location.y);
		 if ((nex_value<=pre_value)&&visit_mark!=VISITED_MARK)
		 {
			 pre_value = nex_value;
			 previous = next;
		 }
	 }
	 return previous;
 };
 void adjustNewPosition(cv::Point& center,  cv::Mat& mark,  const std::vector<cv::Point>& polygonPointSet,
	 TestCellPosition& lastTCP, const int serial, const cv::Point& seed)
 {
	 int outline = OUTLINE_MARK;
	 int empty = EMPTY;
	 int measureD = cv::pointPolygonTest(polygonPointSet, center, true);
	 int  mark_value = global_outline.at<uchar>(center);
	 if (mark_value== outline)
	 {
		 markPointMark(mark, center, OUTLINE_MARK);
		 if (measureD < OUTLINE_THRESHOLD)
		 {
			 markPointMark(TestCellPosition_Mark, center, VISITED_MARK);
			 center = findNextPosition(lastTCP, polygonPointSet, center,seed);
		 }
	 }
	 else /*if (lastTCP.Four_Neighbor[serial].markValue !=empty)*/
	 {
		 if (measureD < OUTLINE_THRESHOLD)
		 {
			 markPointMark(mark, center, OUTLINE_MARK);
			 markPointMark(TestCellPosition_Mark, center, VISITED_MARK);
			 center = findNextPosition(lastTCP, polygonPointSet, center,seed);
		 }
	 }
 };
 int getVisitMarkValue_8(const cv::Point& center, const int i)
 {
	 int visit_mark = 0;
	 cv::Point new_center(center);
	 new_center += EIGHT_CELL_NEIGHBOR[i];
	 visit_mark = TestCellPosition_Mark.at<int>(new_center);
	 return visit_mark;
 };
 void markSeedSetMark(SeedSet& sSet, const std::vector<Cell>cellSet, int markNumber, cv::Mat& mark)
 {
	 int count = cellSet.size();
	 for (int i = 0; i < count; i++)
	 {
		 markPointMark(sSet.mark, cellSet[i].position, markNumber);
		 markPointMark(mark, cellSet[i].position, markNumber);
	 }
 }
 cv::Mat drawRotateOutline(const cv::Mat& oldOutline, const cv::RotatedRect& sandRect, const std::vector<cv::Point>& con_hull)
 {
	 cv::Mat rot_outline;
	 oldOutline.copyTo(rot_outline);
	 cv::Point2f new_point[4];
	 sandRect.points(new_point);
	 for (int i = 0; i < 4; i++)
	 {
		 int j = (i + 1) % 4;
		cv::Point first = new_point[i];
		cv::Point second = new_point[j];
		 cv::line(rot_outline, first, second, 185, 3);
	 }
	 return rot_outline;
 };
 void  drawNewPositionSeed(cv::Mat& polygonOutline, const std::vector<cv::Point>& newPositionSeed)
 {
	 for (int i = 0; i < newPositionSeed.size(); i++)
	 {
		 cv::circle(polygonOutline, newPositionSeed[i], 4, 50, 4);
	 }
 
 };
 void drawNewPointSetOnPolygonOutline(cv::Mat& polygonOutline,  std::vector<cv::Point>& newPolgonPointSet)
 {
	 std::vector<cv::Point>con_hull=newPolgonPointSet;
	 cv::convexHull(newPolgonPointSet, con_hull);
	 cv::Point first, second;
	 int count = con_hull.size();
	 for (int i = 0; i < count; i++)
	 {
		 int j = (i + 1) % count;
		 first = con_hull[i];
		 second = con_hull[j];
		 cv::line(polygonOutline, first, second, 60, 5);
	 }
 }
 void  deleteNewUntrustPoint(cv::Point& seed_location, std::vector<cv::Point>& polygonPointSet, int deleteNumber)
 {
	 for (int i = 0; i < polygonPointSet.size(); i++)
	 {
		 for (int j = i + 1; j < polygonPointSet.size(); j++)
		 {
			 if (compareDistance(seed_location, polygonPointSet[i], polygonPointSet[j]))
			 {
				 std::swap(polygonPointSet[i], polygonPointSet[j]);
			 }
		 }
	 }
	 for (int i = 0; i < deleteNumber; i++)
	 {
		 polygonPointSet.pop_back();
	 }
 };
 void  deleteNewUntrustPoint(cv::Point& seed_location, std::vector<cv::Point>& polygonPointSet, int deleteNumber,const cv::Size&pictureSize)
 {
	 for (int i = 0; i < polygonPointSet.size(); i++)
	 {
		 for (int j = i + 1; j < polygonPointSet.size(); j++)
		 {
			 if (compareDistance(seed_location, polygonPointSet[i], polygonPointSet[j]))
			 {
				 std::swap(polygonPointSet[i], polygonPointSet[j]);
			 }
		 }
	 }
	 std::vector<cv::Point>RotatedRectAngle;
	 RotatedRectAngle= getFillEllipseRotatedRect(polygonPointSet,  deleteNumber,  pictureSize);
	 std::vector<cv::Point>CullRotated;
	 cv::convexHull(RotatedRectAngle, CullRotated);
	 for (int i = polygonPointSet.size()-deleteNumber; i < polygonPointSet.size(); i++)
	 {
		 if (cv::pointPolygonTest(CullRotated, polygonPointSet[i], false) == -1)
		 {
			 for (int j = i; j < polygonPointSet.size(); )
			 {
				 polygonPointSet.pop_back();
			 }
			 break;
		 }
	 }
 };
 std::vector<cv::Point> getFillEllipseRotatedRect(const std::vector<cv::Point>& singleSet, int deleteNumber, const cv::Size& pictureSize)
 {
	 std::vector<cv::Point> ellipsePointSet;
	 for (int i = 0; i < singleSet.size() - deleteNumber; i++)
	 {
		 ellipsePointSet.push_back(singleSet[i]);
	 }
	 cv::RotatedRect ellipseRect;
	 ellipseRect = cv::fitEllipse(ellipsePointSet);
	 std::vector<cv::Point>ellipseRotR;
	 cv::Point2f sRR[4];
	 ellipseRect.points(sRR);
	 for (int i = 0; i < FOUR_NEIGHBOR; i++)
	 {
		 cv::Point angle(sRR[i]);
		 if (angle.x < 0)
		 {
			 angle.x = 0;
		 }
		 if (angle.x > pictureSize.width - 1)
		 {
			 angle.x = pictureSize.width - 1;
		 }
		 if (angle.y < 0)
		 {
			 angle.y = 0;
		 }
		 if (angle.y > pictureSize.height - 1)
		 {
			 angle.y = pictureSize.height - 1;
		 }
		 ellipseRotR.push_back(angle);
	 }
	 return ellipseRotR;
 };
 void getPolygonSixTeenPoint(cv::Point& seedP, const cv::Mat& outline, std::vector<cv::Point>& polygonPointSet)
 {
	 int rows = outline.rows;
	 int cols = outline.cols;
	 cv::Point xy_plus;

	 for (int i = 0; i < EIGHT_NEIGHBOR; i++)
	 {
		 xy_plus = seedP;
		 while (/*(xy_plus.x > 0 && xy_plus.x < cols) &&*/ (EMPTY != (int)outline.at<uchar>(xy_plus)))
		 {
			 xy_plus += EIGHT_CELL_NEIGHBOR[i]/3;
		 }
		 polygonPointSet.push_back(xy_plus);
	 }
	 double theta = PI / 8;
	 double distance = 0;
	 
	 for (int j=0;j<SIXTEEN_NEIGHBOR;j++)
	 {
		 theta += j * PI / 4;
		 xy_plus = seedP;
		 if( (theta<PI / 2&&theta>0 )||(theta>PI*3/2&&theta<PI*2))
		 {
			 while (/*(xy_plus.x > 0 && xy_plus.x < cols) &&*/ (EMPTY != (int)outline.at<uchar>(xy_plus)))
			 {
				 xy_plus.y = seedP.y;
				 xy_plus += EIGHT_CELL_NEIGHBOR[0]/3;
				 distance = -(xy_plus.x - seedP.x)*tan(theta);
				 xy_plus.y += distance;
			 }
			 if (j % 2 == 0)
			 {
				 polygonPointSet.push_back(xy_plus);
			 }
		 }
		 else 
		 {
			 while (/*(xy_plus.x > 0 && xy_plus.x < cols) &&*/ (EMPTY != (int)outline.at<uchar>(xy_plus)))
			 {
				 xy_plus.y = seedP.y;
				 xy_plus.x += EIGHT_CELL_NEIGHBOR[5].x/3;
				 distance = (seedP.x- xy_plus.x) * tan(theta);
				 xy_plus.y += distance;
			 }
			 if (j % 2 == 0)
			 {
				 polygonPointSet.push_back(xy_plus);
			 }
		 }
		 theta = PI / 8;
	 };
#ifdef GET_PLOYGON_SIXTEEN_POINT_
	 if (outline.at<uchar>(seedP)!=EMPTY)
	 {
		 cv::Mat gpsp_outline;
		 outline.copyTo(gpsp_outline);
		 cv::circle(gpsp_outline, seedP, 20, 120, 10);
		 for (int i = 0; i < polygonPointSet.size(); i++)
		 {
			 cv::circle(gpsp_outline, polygonPointSet[i], 10, 120, 10);
		 };
		 cv::namedWindow("getPolygonSixteenPoint", cv::WINDOW_NORMAL);
		 cv::imshow("getPolygonSixteenPoint", gpsp_outline);
		 cv::waitKey(5000);
	 }
#endif

 };
 void getOutline(const cv::Mat& colorPicture, const cv::Rect& minRect, std::vector<Cell>& outlineSet)
 {
	 cv::Point top_left_begin =minRect.tl();
	 top_left_begin -= cv::Point(-1,-1);
	 cv::Point bottom_left_begin = cv::Point(minRect.tl().x, minRect.tl().y + minRect.height + 1);
	 cv::Point top_right_begin = cv::Point(minRect.tl().x+minRect.width+1,minRect.tl().y);
	 Cell tempC;
	 for (int i = 0; i < OUTLINE_CELL_NUMBER; i++)
	 {
		 cv::Point top_left=cv::Point(top_left_begin.x,top_left_begin.y-i * INCREMENT_CELL);
		 cv::Point bottom_left = cv::Point(bottom_left_begin.x, bottom_left_begin.y + i * INCREMENT_CELL);
		 for (int j=0;j<minRect.width;j++)
		 {
			 top_left.x++;
			 if (j % NEGATIVE_ML == 0)
			 {
				 tempC = fromPointToCell(colorPicture, top_left);
				 outlineSet.push_back(tempC);
			 }
			 bottom_left.x++;
			 if (j % NEGATIVE_ML == 0)
			 {
				 tempC = fromPointToCell(colorPicture, bottom_left);
				 outlineSet.push_back(tempC);
			 }
		 }
	 }
	 for (int i = 0; i < OUTLINE_CELL_NUMBER; i++)
	 {
		 cv::Point top_left = cv::Point(top_left_begin.x-i* INCREMENT_CELL,top_left_begin.y);
		 cv::Point top_right = cv::Point(top_right_begin.x+i * INCREMENT_CELL,top_right_begin.y);
		 for (int j=0;j<minRect.height;j++)
		 {
			 top_left.y++;
			 if (j % NEGATIVE_ML == 0)
			 {
				 tempC = fromPointToCell(colorPicture, top_left);
				 outlineSet.push_back(tempC);
			 }
			 top_right.y++;
			 if (j % NEGATIVE_ML == 0)
			 {
				 tempC = fromPointToCell(colorPicture, top_right);
				 outlineSet.push_back(tempC);
			 }
		 }
	 }
#ifdef GET_OUTLINE
	 cv::Mat colorPictureTemp;
	 colorPicture.copyTo(colorPictureTemp);
	 for (int i = 0; i < outlineSet.size(); i++)
	 {
		 cv::circle(colorPictureTemp,outlineSet[i].position,3,cv::Scalar(125,125,125),2);
	 }
	 std::string new_string_address = address_global;
	 new_string_address += "trainData\\";
	 if (0 != ::_access(new_string_address.c_str(), 0))
	 {
		 // if this folder not exist, create a new one.
		 _mkdir(new_string_address.c_str());  
	 }
	 new_string_address += "glo_negative_" + std::to_string(seed_number_globle) + ".JPG";
	 cv::imwrite(new_string_address, colorPictureTemp);
	 //cv::namedWindow("colorPictureTemp", cv::WINDOW_NORMAL);
	 //cv::imshow("colorPictureTemp",colorPictureTemp);
	 //cv::waitKey(10000);
#endif 
 };
 void fromCellToMat(const std::vector<Cell>& cellSet, const std::vector<Cell>& outlineSet, cv::Mat& trainingData, cv::Mat& responses)
 {
	 trainingData = cellTransactionMat(cellSet[0]);
	 for (int i=1;i<cellSet.size();i++)
	 {
		 trainingData.push_back(cellTransactionMat(cellSet[i]));
	 }
#ifdef MACHINE_LEARNING_SVM
	 for (int j = 0; j < outlineSet.size(); j++)
	 {
		 trainingData.push_back(cellTransactionMat(outlineSet[j]));
	 }
#endif
#ifdef THREE_POINT
	 responses = cv::Mat::zeros( (cellSet.size() + outlineSet.size())*9,1, CV_32FC1);
#endif
#ifdef TWENTY_SEVEN_POINT
	 responses = cv::Mat::zeros((cellSet.size() + outlineSet.size()), 1, CV_32FC1);
#endif
#ifdef THREE_POINT
	 for (int k = 0; k < cellSet.size()*9; k++)
#endif
#ifdef TWENTY_SEVEN_POINT
	 for (int k = 0; k < cellSet.size() ; k++)
#endif
	 {
		 responses.at<float>(k) = 1;
	 }

 };
 cv::Mat cellTransactionMat(const Cell& cell)
 {
#ifdef THREE_POINT
	 cv::Mat tempM(9,3,CV_32FC1);
#endif
#ifdef TWENTY_SEVEN_POINT
	 cv::Mat tempM(1, 27, CV_32FC1);
#endif

	 cv::Mat cellM;
	 cell.cell.copyTo(cellM);
#ifdef THREE_POINT
	 for (int i = 0; i < MAT_ROW; i++)
	 {
		 for (int j = 0; j < MAT_COL; j++)
		 {
			 for (int k = 0; k < 3; k++)
			 {
				 tempM.at<float>(i * MAT_ROW + j,k) = cellM.at<cv::Vec3b>(i, j)[k];
			 }
		 }
	 }
#endif
#ifdef TWENTY_SEVEN_POINT
	 for (int i = 0; i < MAT_ROW; i++)
	 {
		 for (int j = 0; j < MAT_COL; j++)
		 {
			 for (int k = 0; k < 3; k++)
			 {
				 tempM.at<float>(i * MAT_ROW* MAT_COL + j * MAT_COL + k) = cellM.at<cv::Vec3b>(i, j)[k];
			 }
		 }
	 }
#endif
	 return tempM;
 };
 bool is_MarginSand(const cv::Point& seed, const cv::Rect& minRect, const cv::Mat& newPicture)
 {
	 int rows = newPicture.rows;
	 int cols = newPicture.cols;
	 int r_y = minRect.height;
	 int r_x = minRect.width;
	 int top = seed.y;
	 int left = seed.x;
	 int right = cols-seed.x;
	 int bottom =rows-seed.y;
	 if (top < r_y)
	 {
		 return true;
	 }else if (bottom < r_y)
	 {
		 return true;
	 }
	 else if (left<r_x)
	 {
		 return true;
	 }
	 else if (right<r_x)
	 {
		 return true;
	 }
	 else
	 {
		 return false;
	 }
 };
 void deleteUntrustPointInZeroSet(const cv::Mat& oldOutline, std::vector<cv::Point>& zeroSet)
 {
	 std::multimap<int, std::pair<int, cv::Point>> seedSPair;
	 std::multimap<int, cv::Point>tempSeedSPair;
	 for (auto it = zeroSet.begin(); it != zeroSet.end(); it++)
	 {
		 std::vector<cv::Point>polygonPointSet;
		 getPolygonPoint((*it), oldOutline, polygonPointSet);
		 deleteNewUntrustPoint((*it), polygonPointSet, 0);
		 std::pair<int, cv::Point> tempS;
		 tempS.first = (polygonPointSet[0].x - (*it).x) * (polygonPointSet[0].x - (*it).x)
			 + (polygonPointSet[0].y - (*it).y) * (polygonPointSet[0].y - (*it).y);
		 tempS.second = (*it);
		 tempSeedSPair.insert(tempS);
	 }
	 int i = 0;
	 for (auto it = tempSeedSPair.begin(); it != tempSeedSPair.end(); it++)
	 {
		 std::pair<int, std::pair<int, cv::Point>> tempSP;
		 tempSP.first = i++;
		 tempSP.second = *it;
		 seedSPair.insert(tempSP);
	 }
	  deleteUntrustInMultimap( seedSPair,  DELETE_IN_MULTIMAP);
	  zeroSet.clear();
	  for (auto it = seedSPair.cbegin(); it != seedSPair.cend(); it++)
	  {
		  zeroSet.push_back(it->second.second);
	  }
 };
 void deleteUntrustInMultimap(std::multimap<int, std::pair<int, cv::Point>>& seedSPair, double DELETE_IN_MULTIMAP)
 {
	 int deleteNumber = seedSPair.size() * DELETE_IN_MULTIMAP;
	 std::multimap<int, std::pair<int, cv::Point>>::const_iterator it;
	 it = seedSPair.upper_bound(deleteNumber);
	 seedSPair.erase(seedSPair.cbegin(), it);
 };
 bool isDispose(cv::Mat& sSet_mark, std::vector<Cell>& cellSet)
 {
	 bool is_dispose = false;
	 int count = 0;
	 for (auto it = cellSet.begin(); it != cellSet.end(); it++)
	 {
		 if (sSet_mark.at<int>(it->position)!=0)
		 {
			 count++;
		 }
	 }
	 if (count >= cellSet.size() * 0.1)
	 {
		 is_dispose = true;
	 }
	 return is_dispose;
 };
 void determinePictureFrame(const cv::Mat& mark, const cv::Point& pt,const int direction, int& visit_mark)
 {
	 cv::Point ppt = pt;
	 int x= BORDER_WIDTH*0.5,y= BORDER_WIDTH*0.5;
	 cv::Point top_left(x,y);
	 int width=mark.cols- BORDER_WIDTH*0.5;
	 int height=mark.rows- BORDER_WIDTH*0.5;
	 cv::Point bottom_right(width,height);
	 cv::Rect frame=cv::Rect(top_left,bottom_right);
	 ppt += FOUR_CELL_NEIGHBOR[direction];
	 if (!frame.contains(ppt))
	 {
		 visit_mark = VISITED_MARK;
	 }
 };
 void drawMarkPolygon(cv::Mat& mark, const std::vector<cv::Point>& con_hull)
 {
	 cv::Point first, second;
	 int count = con_hull.size();
	 for (int i = 0; i < count; i++)
	 {
		 int j = (i + 1) % count;
		 first = con_hull[i];
		 second = con_hull[j];
		 cv::line(mark, first, second, -1, 2);
	 }
 };
 void displaySeedMark(const cv::Mat& mark)
 {
	 xlnt::workbook markBook;
	 markBook.load("C://work//PictureRecognize//PictureRecognize//PictureRecognize//mark.xlsx");
	 xlnt::worksheet markSheet = markBook.active_sheet();
	 for (int x = 0; x < mark.cols; x++)
	 {
		 for (int y = 0; y < mark.rows; y++)
		 {
			 cv::Point p(x, y);
			 int value = mark.at<int>(p);
			 if ( value!= 0)
			 {
				writeMark( markSheet,  p, value);
			 }

		 }
	 }
	 markBook.save("C://work//PictureRecognize//PictureRecognize//PictureRecognize//mark.xlsx");
 };
 void writeMark(xlnt::worksheet& markSheet, const cv::Point& p, int value)
 {
	 std::string place = "[" + std::to_string(p.x) + " " + std::to_string(p.y) + "]";
	 std::string val_s = "value:" + std::to_string(value);
	 std::string content = place + val_s;
	 writeOrClearWrite(markSheet, p, content);
	 if (value == -1)
	 {
		 xlnt::font run_font;
		 run_font.bold();
		 run_font.color(xlnt::color::black());
		 run_font.name("SimSun");
		 run_font.size(9);
		 markSheet.cell(p.x, p.y).font(run_font);
	 }
	 else
	 {
		 xlnt::font run_font;
		 run_font.color(xlnt::color::blue());
		 run_font.name("SimSun");
		 run_font.bold();
		 run_font.size(9);
		 markSheet.cell(p.x, p.y).font(run_font);
	 }
 };
 void adjustDirection(const int visit_mark0, const int visit_mark1, const int visit_mark2, const int visit_mark3,
	 const int direction_0, const int direction_1, const int direction_2, const int direction_3,
	 const int& outline, const int& empty, const int& sand,
	 cv::Point& direction, int& serial)
 {
	 if ((visit_mark2 != VISITED_MARK) && (visit_mark0 == VISITED_MARK) && (direction_1 == outline) && (direction_2 == empty) && (direction_3 == sand))
	 {
		 direction = FOUR_CELL_NEIGHBOR[2];
		 serial = 2;
	 }
	 else if ((visit_mark3 != VISITED_MARK) && (visit_mark1 == VISITED_MARK) && (direction_2 == outline) && (direction_3 == empty) && (direction_0 == sand))
	 {
		 direction = FOUR_CELL_NEIGHBOR[3];
		 serial = 3;
	 }
	 else if ((visit_mark0 != VISITED_MARK) && (visit_mark2 == VISITED_MARK) && (direction_3 == outline) && (direction_0 == empty) && (direction_1 == sand))
	 {
		 direction = FOUR_CELL_NEIGHBOR[0];
		 serial = 0;
	 }
	 else if ((visit_mark1!= VISITED_MARK) && (visit_mark3 == VISITED_MARK) && (direction_0 == outline) && (direction_1 == empty) && (direction_2 == sand))
	 {
		 direction = FOUR_CELL_NEIGHBOR[1];
		 serial = 1;
	 }
 };
 bool is_visitedFourDirection(const cv::Mat& TestCellPosition_Mark, const cv::Point& position)
 {
	 int count=0;
	 bool visitedFourBool = false;
	 for (int i = 0; i < FOUR_NEIGHBOR; i++)
	 {
		 cv::Point p = position;
		 p += FOUR_CELL_NEIGHBOR[i];
		int visit_mark = TestCellPosition_Mark.at<int>(p);
		if (visit_mark == VISITED_MARK)
		{
			count++;
		}
	 }
	 if (count == FOUR_NEIGHBOR)
	 {
		 visitedFourBool = true;
	 }
	 return visitedFourBool;
 };
 void  formSVMOneClass(const std::vector<Cell>& cellSet, cv::Mat& trainingData, cv::Mat& responses)
 {
	 cv::Mat temp;
	 for (auto it=cellSet.begin(); it!=cellSet.end(); it++)
	 {
		 if (it == cellSet.begin())
		 {
			 temp = fromCellToMat(*it);
			 temp.copyTo(trainingData);
		 }
		 else	
		 {
			 temp = fromCellToMat(*it);
			 trainingData.push_back(temp);
		 }
	 }
	 int row = 9 * cellSet.size();
	 responses = cv::Mat::ones(row, 1, CV_32FC1);
 };

 cv::Mat fromCellToMat(const Cell& it)
 {
	 float mean[3] = {};
	 cv::Mat temp = cv::Mat(9, 6, CV_32FC1);
	 for (int i = 0; i < MAT_ROW; i++)
	 {
		 for (int j = 0; j < MAT_COL; j++)
		 {
			 for (int k = 0; k < MAT_CHANNEL; k++)
			 {
				 mean[k] += it.cell.at<cv::Vec3b>(i, j)[k];
			 }
		 }
	 }
	 for (int i=0;i< MAT_CHANNEL;i++)
	 {
		 mean[i] /= MATX_ROW;
	 }
	 for (int i = 0; i < MAT_ROW; i++)
	 {
		 for (int j = 0; j < MAT_COL; j++)
		 {
			 for (int k = 0; k < 6; k++)
			 {
				 if (k < 3)
				 {
					 temp.at<float>(i * MAT_ROW + j, k) = it.cell.at<cv::Vec3b>(i, j)[k];
				 }
				 else
				 {
					 temp.at<float>(i * MAT_ROW + j, k) = (it.cell.at<cv::Vec3b>(i, j)[k - MAT_ROW] - mean[k - MAT_ROW]);
				 }
			 }
		 }
	 }
	 return temp;
 };
 bool OneClassSVMPredict(const cv::Mat& Outline, const cv::Ptr<cv::ml::SVM>& svm, const Cell& tempCell,std::ofstream & thresholdSVM)
 {
	 if (Outline.at<uchar>(tempCell.position) == EMPTY)
	 {
		 return false;
	 }
	 cv::Mat temp,tempR;
	 int count=0;
	 temp = fromCellToMat(tempCell);
	 for (int i = 0; i < temp.rows; i++)
	 {
		 tempR = temp.rowRange(i,i+1);
		 if (/*tempF*//*<THRESHOLD_SVM*/svm->predict(tempR))
		 {
			 count++;
		 }
	 }
	 thresholdSVM << std::endl;
	 if (count >= PREDICT_NUMUBER)
	 {
		 return true;
	 }
	 else
	 {
		 return false;
	 }
 };
 inline void isOverflowBoundary(const cv::Mat& mark, const cv::Point& pt)
 {
	 if (pt.x<0||pt.x>mark.cols||pt.y<0||pt.y>mark.rows)
	 {
		 throw std::runtime_error("Overflow boundary!");
	 }
 
 };
 void formSVMClassFour(const std::vector<Cell>& cellSet, cv::Mat& trainingData, cv::Mat& responses)
 {
	 cv::Mat temp;
	 for (auto it = cellSet.begin(); it != cellSet.end(); it++)
	 {
		 if (it == cellSet.begin())
		 {
			 temp = fromCellToMatFour(*it);
			 temp.copyTo(trainingData);
		 }
		 else	 
		 {
			 temp = fromCellToMatFour(*it);
			 trainingData.push_back(temp);
		 }
	 }
	 int row = 9 * cellSet.size();
	 responses = cv::Mat::ones(row, 1, CV_32FC1);
 };
 cv::Mat fromCellToMatFour(const Cell& it)
 {
#ifndef THREE_DIMENSIONAL_SPACE
	 float mean[3] = {};
	 cv::Mat temp = cv::Mat(9, 4, CV_32FC1);
	 for (int i = 0; i < MAT_ROW; i++)
	 {
		 for (int j = 0; j < MAT_COL; j++)
		 {
			 for (int k = 0; k < MAT_CHANNEL; k++)
			 {
				 mean[k] += it.cell.at<cv::Vec3b>(i, j)[k];
			 }
		 }
	 }

	 for (int i = 0; i < MAT_CHANNEL; i++)
	 {
		 mean[i] /= MATX_ROW;
	 }
#else
	 cv::Mat temp = cv::Mat(9, 3, CV_32FC1);
#endif
	 for (int i = 0; i < MAT_ROW; i++)
	 {
		 for (int j = 0; j < MAT_COL; j++)
		 {
#ifndef THREE_DIMENSIONAL_SPACE
			 for (int k = 0; k < 4; k++)
			 {
				 if (k < 3)
				 {
					 temp.at<float>(i * MAT_ROW + j, k) = it.cell.at<cv::Vec3b>(i, j)[k];
				 }
				 else
				 {
					 float variance = 0;
					 for (int m = 0; m < MAT_COL; m++)
					 {
						 variance+= (it.cell.at<cv::Vec3b>(i, j)[m] - mean[m]) *
							 (it.cell.at<cv::Vec3b>(i, j)[m] - mean[m]);
					 }
					 variance /= MAT_COL;
					 temp.at<float>(i * MAT_ROW + j, k) = variance;
				 }
			 }
#else
			 for (int k=0;k< MAT_ROW;k++)
			 {
				 temp.at<float>(i * MAT_ROW + j, k) = it.cell.at<cv::Vec3b>(i, j)[k];
			 }
#endif
		 }
	 }
	 return temp;
 };
 bool OneClassSVMPredictFour(const cv::Ptr<cv::ml::SVM>& svm, const Cell& tempCell)
 {
	 cv::Mat temp, tempR;
	 int count = 0;
	 temp = fromCellToMatFour(tempCell);
	 for (int i = 0; i < temp.rows; i++)
	 {
		 tempR = temp.rowRange(i, i + 1);
		 if (svm->predict(tempR/*,cv::noArray(),cv::ml::StatModel::RAW_OUTPUT*/))
		 {
			 count++;
		 }
	 }
	 if (count >= PREDICT_NUMUBER)
	 {
		 return true;
	 }
	 else
	 {
		 return false;
	 }
 };
 inline void RandPoint(cv::RNG& seedRng, cv::RotatedRect& rotatedR, const std::vector<cv::Point>& rotatedRectFourPoint, cv::Point& position, const cv::Mat& oldOutline)
 {
	 do
	 {
	 double coff_x = seedRng.uniform(-5.0, 5.0);
	 double coff_y = seedRng.uniform(-5.0, 5.0);
	 int x = rotatedR.center.x + (int)(coff_x * rotatedR.size.width);
	 int y = rotatedR.center.y + (int)(coff_y * rotatedR.size.height);
	 if (x < 0)
	 {
		 x = 0;
	 }
	 if (x >=oldOutline.cols)
	 {
		 x = oldOutline.cols - 1;
	 }
	 if (y < 0)
	 {
		 y = 0;
	 }
	 if (y >= oldOutline.rows)
	 {
		 y = oldOutline.rows - 1;
	 }
	 position = cv::Point(x, y);
     }while (cv::pointPolygonTest(rotatedRectFourPoint, position, false) == -1|| (oldOutline.at<uchar>(position) == EMPTY));
 };
 bool isDispose(cv::Mat& sSet_mark, std::vector<cv::Point>& cellSet)
 {
	 bool is_dispose = false;
	 int count = 0;
	 for (auto it = cellSet.begin(); it != cellSet.end(); it++)
	 {
		 if (sSet_mark.at<int>(*it) != 0)
		 {
			 count++;
		 }
	 }
	 if (count >= cellSet.size() * 0.1)
	 {
		 is_dispose = true;
	 }
	 return is_dispose;
 };