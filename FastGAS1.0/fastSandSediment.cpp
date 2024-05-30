#include"fastSandSediment.h"
#include <direct.h>
#include <io.h>
#include<cmath>
#define DELETE_FIRST_UNTRUST_NUM 2

std::multimap<double, double> fastSandSediment(const cv::Mat& colorPicture, SeedSet& sSet, const cv::Mat& Outline, const std::string& workFaddress, double singlePixeLength, std::vector<double>& sandMinDiameter)
{
	std::multimap<double, double>diameterAndArea;
	std::string sandDiameterAddresd = workFaddress + "sandDiamter.txt";
	std::string sandDiameterAddresdSetPoint = workFaddress + "sandDiamterSetPoint.txt";
	std::ofstream sandAreaOutput(sandDiameterAddresd), sandAreaOutputSetPoint(sandDiameterAddresdSetPoint);
	cv::Mat  new_colorSandGrain1, bgr_newPicture;
	new_colorSandGrain1 = colorPicture.clone();
	bgr_newPicture = new_colorSandGrain1.clone();
	sSet.mark = cv::Mat::zeros(sSet.mark.rows, sSet.mark.cols, CV_32S);
	std::vector<cv::Mat>sandSet;
	std::vector<cv::Point>polygonPointSet;
	int count = 0;
	for (int i=0;i<sSet.seedPSet.size();i++)
	{
		cv::Mat mark = cv::Mat::zeros(sSet.mark.rows, sSet.mark.cols, CV_32S);
		if (sSet.mark.at<int>(sSet.seedPSet[i].location) != 0)
		{
			continue;
		}
		cv::RotatedRect sandRect;
		Seed& seed = sSet.seedPSet[i];
		cv::Mat oldOutline;
		Outline.copyTo(oldOutline);
		polygonPointSet.clear();
		cv::Size pictureSize(oldOutline.cols, oldOutline.rows);
		//ȷ������Χ��Χ
		getPolygonSixTeenPoint(seed.location, oldOutline, polygonPointSet);
		deleteNewUntrustPoint(seed.location, polygonPointSet, DELETE_FIRST_UNTRUST_NUM, pictureSize);
		cv::RotatedRect tempSeedRect;
		tempSeedRect = cv::minAreaRect(polygonPointSet);
		cv::Point tsp_center = tempSeedRect.center;
		getPolygonSixTeenPoint(tsp_center, oldOutline, polygonPointSet);
		deleteNewUntrustPoint(seed.location, polygonPointSet, DELETE_FIRST_UNTRUST_NUM, pictureSize);
		std::vector<cv::Point>newPositionSeed;
		fastExpansionPolygonPointSet(EXPANSION_CONTROL_POINT_SEED_NUMBER, oldOutline, polygonPointSet, newPositionSeed);
		if (polygonPointSet.size() < 3)
		{
			continue;
		}
		std::vector<cv::Point>con_hull;
		cv::convexHull(polygonPointSet, con_hull);
		////********************************text*************************************************************************
		//***************************************************************************************************************
		//cv::Mat polygonOutline;
		//polygonOutline = drawEightPolygonPointInOutline(oldOutline, seed.location, polygonPointSet);
		//drawNewPositionSeed(polygonOutline, newPositionSeed);
		//cv::Mat rotateOutline;
		//rotateOutline = drawRotateOutline(oldOutline, sandRect, con_hull);
		//std::string  outline_color_number;
		//std::string outline_picture_address = workFaddress + "newOutPicture\\";
		//if (0 != ::_access(outline_picture_address.c_str(), 0))
		//{
		//	_mkdir(outline_picture_address.c_str());
		//}
		//outline_picture_address += "outline\\";
		//if (0 != ::_access(outline_picture_address.c_str(), 0))
		//{
		//	_mkdir(outline_picture_address.c_str());
		//}
		//std::string rot_outline_address = outline_picture_address;
		//std::string outline_rotate_number = std::to_string(i) + "_OutlineRgbPicture.JPG";
		//rot_outline_address += outline_rotate_number;
		//outline_color_number = std::to_string(i) + "_d5-10_OutlineRgbPicture.JPG";
		//outline_picture_address += outline_color_number;
		//cv::imwrite(outline_picture_address, polygonOutline);
		//***************************************************************************************************************
		//***************************************************************************************************************
		//�������ذ߿�������ʶ��
		//��ȡ���������ذ߿�
		count++;
		std::cout << "count=" << count << std::endl;
		int markNumber = i + 1;
		Cell seedCell = fromPointToCell(colorPicture, sSet.seedPSet[i].location);
		std::vector<Cell> cellSet,firstCellSet,nextCellSet;
		cellSet.push_back(seedCell);
		firstCellSet.push_back(seedCell);
		fastGetNeighbor(colorPicture, firstCellSet,nextCellSet, markNumber, sSet);
		Color sandColor;
		adjustColor(sandColor, seedCell);
		std::vector<Cell>;
		//��ʼ��ʯ���ذ߿��������ж�
		while (nextCellSet.size() != 0)
		{
			fastGetGravelCellSet(sandColor, seed,  colorPicture, cellSet, nextCellSet, con_hull,  sSet);
			firstCellSet = nextCellSet;
			fastGetNeighbor(colorPicture,firstCellSet,nextCellSet,markNumber,sSet);
			//���mark����
			fastMarkCellSetMark(mark, nextCellSet,markNumber);
		}
		cv::Mat bgr_sand;
		formSandPicture(bgr_sand, cellSet, mark, new_colorSandGrain1);
		std::cout << "cell_count=" << cellSet.size() << std::endl;
		//�������ɰ��ͼ��
		//*************************************test

		//******************************************
		//������ת����
		returnRotateRect(mark,sandRect);
		cv::Rect boundingR = sandRect.boundingRect();
		
		double sandDiameterMin =std::min(sandRect.size.height, sandRect.size.width)*10 * singlePixeLength;
		double sandArea = cellSet.size() * 9 * singlePixeLength * singlePixeLength * 100;
		diameterAndArea.insert(std::pair<double,double>(sandDiameterMin,sandArea ));
		sandAreaOutput <<"i:" <<i<<"   " << "Diameter:" << sandDiameterMin << std::endl;
		sandMinDiameter.push_back(sandDiameterMin);
		//
		cv::Point textPos = sandRect.center;
		std::string label = std::to_string(i);
		// ��ͼ���ϻ��ƺ�ɫ���  
		cv::putText(bgr_newPicture, label, textPos, cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(255, 255, 255), 2, cv::LINE_AA);
		cv::ellipse(bgr_newPicture, sandRect, cv::Scalar(0, 0, 255), 12);
		sandSet.push_back(bgr_sand);
	}
	std::string color_number;
	//*********************�����ļ���*************************
	std::string picture_address = workFaddress + "newOutPicture\\picture\\";

	if (0 != ::_access(picture_address.c_str(), 0))
	{
		::_mkdir(picture_address.c_str());   // ���� 0 ��ʾ�����ɹ���-1 ��ʾʧ��
	}
	//********************************************************
	
	color_number = "_rgbPicture.JPG";
	picture_address += color_number;
	cv::imwrite(picture_address, bgr_newPicture);
	std::string segementAllSandAddress = workFaddress + "newOutPicture\\sumColorOutlinePicture\\";
	if (0 != ::_access(segementAllSandAddress.c_str(), 0))
	{
		::_mkdir(segementAllSandAddress.c_str());
	}
	segementAllSandAddress += "allColorSand.JPG";
	cv::imwrite(segementAllSandAddress, new_colorSandGrain1);
	return diameterAndArea;
};
//������Ƶ�����
void fastExpansionPolygonPointSet(int expansionSeedNumber, const cv::Mat& oldOutline, std::vector<cv::Point>& polygonPointSet
	, std::vector<cv::Point>& newPositionSeed)
{
	std::vector<cv::Point>zeroSet, singleSet;
	cv::RNG seedRng = cv::theRNG();
	std::vector<cv::Point> orignal_polygonPointSet = polygonPointSet;
	cv::RotatedRect rotatedR = cv::minAreaRect(orignal_polygonPointSet);
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
	int count_1 = 0;
	int count_2 = 0;
	for (int i = 0; i < expansionSeedNumber; i++)
	{
		cv::Point position;
		do
		{
			count_2++;
			do
			{
				count_1++;
				singleSet.clear();
				fastRandPoint(seedRng, rotatedR, rotatedRectFourPoint, position, oldOutline);
				if (count_1 > SEARCH_MAX_NUMBER)
				{
					break;
				}
			} while (!isPointInsideEllipse(position, cv::minAreaRect(zeroSet)));
			if (count_2 > SEARCH_MAX_NUMBER)
			{
				break;
			}
			if ((oldOutline.at<uchar>(position) != EMPTY))
			{
				getPolygonSixTeenPoint(position, oldOutline, singleSet);
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
		getPolygonSixTeenPoint(newPositionSeed[i], oldOutline, singleSet);
		deleteNewUntrustPoint(newPositionSeed[i], singleSet, POP_CONTROL_POINT_NUMBER/*, pictureSize*/);
		for (int i = 0; i < singleSet.size()/*-POP_CONTROL_POINT_NUMBER*/; i++)
		{
			zeroSet.push_back(singleSet[i]);
		}
	}
	cv::convexHull(zeroSet, polygonPointSet);
};
//��ָ��������������µ�
void fastRandPoint(cv::RNG& seedRng, cv::RotatedRect& rotatedR, const std::vector<cv::Point>& rotatedRectFourPoint, cv::Point& position, const cv::Mat& oldOutline)
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
		if (x >= oldOutline.cols)
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
	} while (!isPointInsideEllipse(position, rotatedR) || (oldOutline.at<uchar>(position) == EMPTY));
};
//�ж�һ���Ƿ�����Բ��
bool isPointInsideEllipse(const cv::Point2f& point, const cv::RotatedRect& ellipse)
{
	// ����Բ����ת��Ϊ��Բ��һ�㷽�̲���  
	float center_x = ellipse.center.x;
	float center_y = ellipse.center.y;
	float axes_length_a = ellipse.size.width / 2.0f;
	float axes_length_b = ellipse.size.height / 2.0f;
	float angle = ellipse.angle * CV_PI / 180.0f;

	// �����ͼ������ת������Բ�ֲ�����  
	float x_prime = (point.x - center_x) * std::cos(angle) + (point.y - center_y) * std::sin(angle);
	float y_prime = -(point.x - center_x) * std::sin(angle) + (point.y - center_y) * std::cos(angle);

	// �жϵ��Ƿ�����Բ�ڲ�  
	return (x_prime * x_prime) / (axes_length_a * axes_length_a) +
		(y_prime * y_prime) / (axes_length_b * axes_length_b) <= 1.0f;
}
//��ȡ���������ذ߿�
void fastGetNeighbor(const cv::Mat& colorPicture, std::vector<Cell>& firstCellSet, std::vector<Cell>& nextCellSet, int markNumber, SeedSet& sSet)
{
	cv::Mat& nerghbourMark = sSet.mark;
	nextCellSet.clear();
	//���Mark����
	for (int i = 0; i < firstCellSet.size(); i++)
	{
		fastMarkPointMark(nerghbourMark, firstCellSet[i].position,  markNumber);
	}
	for (int i = 0; i < firstCellSet.size(); i++)
	{
		fastGetFourCellNeighbors(colorPicture, firstCellSet[i].position, nextCellSet, sSet.mark, markNumber);
	}
	firstCellSet.clear();
};
//���Mark����
void fastMarkPointMark(cv::Mat& mark, const cv::Point& pt, int markNumber)
{

	for (int i = 0; i < EIGHT_NEIGHBOR_AND_CENTER; i++)
	{
		cv::Point tempPt = pt+ EIGHT_NEIGHBOR_AND_CENTER_IN_CELL[i];
		isOverflowBoundary(mark, tempPt);
		mark.at<int>(tempPt) =markNumber;
	}
};
//�õ����������ذ߿�
void fastGetFourCellNeighbors(const cv::Mat& colorPicture ,const cv::Point& pt, std::vector<Cell>& nextCellSet, cv::Mat& mark, int markNumber)
{
	int x = mark.cols;
	int y = mark.rows;
	for (int i = 0; i <EIGHT_FOUR_NEIGHBOR; i++)
	{
		cv::Point nPt = pt + FOUR_CELL_NEIGHBOR[i];
		if ((mark.at<int>(nPt)==EMPTY)&&(nPt.x >= 0 && nPt.x < x && nPt.y >= 0 && nPt.y < y))
		{
			nextCellSet.push_back(fromPointToCell(colorPicture,nPt));
			fastMarkPointMark(mark, nPt, markNumber);
		}
	}
};
//�ж����ذ߿��Ƿ�������ʯ�����ذ߿�
void fastGetGravelCellSet(Color& sandColor, const Seed& seed, const cv::Mat& colorPicture, std::vector<Cell>& cellSet, std::vector<Cell>& nextCellSet, std::vector<cv::Point>& con_hull,SeedSet&sSet)
{
	std::vector<Cell>remainCellSet;
	for (int i=0;i<nextCellSet.size();i++)
	{
		Cell cell = nextCellSet[i];
		double threshold=isFastGravelCell(colorPicture,  sandColor,seed,cell);
		if ((cv::pointPolygonTest(con_hull, cell.position, true)>=0)&&(threshold>= FAST_IS_GRAVEL_CELL_THRESHOLD))
		{
			cellSet.push_back(cell);
			remainCellSet.push_back(cell);
			//������ɫ�ռ�
			adjustColor(sandColor, cell);
		}
		else//Ϊ�߽����sSet��mark����Ϊ-1
		{
			cv::Mat& mark = sSet.mark;
			fastMarkPointMark(mark, cell.position, OUTLINE_MARK);
		}
	}
	nextCellSet.clear();
	nextCellSet = remainCellSet;
};
//�ж��Ƿ�������ʯ���ذ߿�
double isFastGravelCell(const cv::Mat& colorPicture, Color& sandColor, const Seed& seed, const Cell& cell)
{
	int m = calculateBelongToGravelPixelNumber(colorPicture, sandColor, cell);
	double w = std::min(seed.scope.width, seed.scope.height) / 2.0;
	double x = cell.position.x;
	double y = cell.position.y;
	double x_s = seed.location.x;
	double y_s = seed.location.y;
	double factor = sqrtf((x - x_s) * (x - x_s) + (y - y_s) * (y - y_s)) / (FAST_ALPHA * w) - 1;
	double e = exp(FAST_BETA*factor);
	double phi = (1 + e * m / FAST_NUMBER_IN_CELL) / (1 + e);
	return phi;
};
//����������ʯ����������
int calculateBelongToGravelPixelNumber(const cv::Mat &colorPicture,const Color& sandColor, const Cell& cell)
{
	int count=0; 
	cv::Point center = cell.position;
	for (int i = 0; i < EIGHT_NEIGHBOR_AND_CENTER; i++)
	{
		cv::Point pt = center + EIGHT_NEIGHBOR_AND_CENTER_IN_CELL[i];

		Scalar_Color cellColor =colorPicture.at<cv::Vec3b>(pt);
		if (
			(sandColor.minColor[0] <= cellColor[0] && cellColor[0] <= sandColor.maxColor[0]) &&
			(sandColor.minColor[1] <= cellColor[1] && cellColor[1] <= sandColor.maxColor[1]) &&
			(sandColor.minColor[2] <= cellColor[2] && cellColor[2] <= sandColor.maxColor[2])
			)
		{
			count++;
		}
	}
	return count;
};
//���mark����
void fastMarkCellSetMark(cv::Mat& mark, const std::vector<Cell>& tempCellSet, int markNumber)
{
	for (int i = 0; i < tempCellSet.size(); i++)
	{
		fastMarkPointMark(mark, tempCellSet[i].position, markNumber);
	}
};
//������ת����
void returnRotateRect(const cv::Mat& mark, cv::RotatedRect& sandRect)
{
	// ��ֵ��ͼ�񣬷���ֵ��Ϊ��ɫ����ֵ��Ϊ��ɫ  
	cv::Mat binaryImage;
	binaryImage.create(mark.size(), CV_8U);

	// ����Դͼ���ÿ�����أ�������ֵ��Ϊ255����ֵ��Ϊ0  
	for (int y = 0; y < mark.rows; ++y) {
		for (int x = 0; x < mark.cols; ++x) {
			int32_t pixelValue = mark.at<int>(y, x);
			binaryImage.at<uint8_t>(y, x) = (pixelValue != 0) ? 255 : 0;
		}
	}
	// 1. �ҵ���Ĥ�е���������  
	std::vector<std::vector<cv::Point>> contours;
	cv::findContours(binaryImage, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

	// 2. �����������ҵ�����һ��������������С�������ת����  
	double maxArea = 0;
	for (const auto& contour : contours) {
		// �������������  
		double area = cv::contourArea(contour);
		if (area > maxArea) {
			maxArea = area;
			// ������С�������ת����  
			sandRect = cv::minAreaRect(contour);
		}
	}
};