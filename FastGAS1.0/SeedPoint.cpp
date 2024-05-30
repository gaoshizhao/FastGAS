#include "SeedPoint.h"
//#include"test.h"
#include"SandSediment.h"
#include<opencv2/opencv.hpp>
#include <direct.h>
#include <io.h>
#define OUTPUT_SEED_PICTURE 
//#define NEW_SEED_POINT_MERGE_S
//#define COPY_SEED_SET_TO_SEED_SET_PAIR
#define DELETE_DOWN_NUMBERS 0.2
#define DELETE_UP_NUMBERS   0.00
#define END_LOOP   60
#define END_DELETE_POINT 4
#define TEMP_END_NUMBER 50
#define DELETE_MAX_DISTANCE 
std::ofstream outFile("location.txt");
cv::RNG seedNewRng = cv::theRNG();
cv::Rect FormScope(const cv::Mat& picture, cv::Point& position)
{

	int x0 = position.x, y0 = position.y;
	int det_x0 = 0,det_y0=0;
	int det_x1 = 0, det_y1 = 0;
	int x1 = position.x, y1 = position.y;
	if (picture.at<uchar>(position) == 0)
	{
		std::cout << "This point is the boundary point!" << std::endl;
		return cv::Rect(0, 0, 0, 0);
	}
	uchar PP = picture.at<uchar>(y0, x0 - det_x0);
	while (x0>0&&picture.at<uchar>(y0, x0 - det_x0) == 255)
	{		
		det_x0++;
		if (x0 - det_x0 <= 0)
			break;
	}
	if (x0 == 0)
		det_x0 = 0;
	while (y0>0&&picture.at<uchar>(y0-det_y0,x0) == 255)
	{
		det_y0++;
		if (y0 - det_y0 <= 0)
			break;
	}
	if (y0 == 0)
		det_y0 = 0;
	x0 -= det_x0;
	y0 -= det_y0;
	while (x1<picture.cols-1&&picture.at<uchar>(y1,x1 +det_x1 ) == 255)
	{
		det_x1++;
		if (x1 + det_x1 >= picture.cols-1 )
		{
			
			break;
		}
	}
	if (x1 == picture.cols-1)
		det_x1 = 0;
	while (y1<picture.rows-1 &&picture.at<uchar>(y1+det_y1, x1) == 255)
	{

		det_y1++;
		if (y1 + det_y1 >= picture.rows-1)
		{
			break;
		}
	}
	if (y1 == picture.rows-1)
		det_y1 = 0;
	x1 += det_x1; 
	y1 += det_y1;
	return cv::Rect(cv::Point(x0, y0), cv::Point(x1, y1));
};
SeedSet SeedPoint(const cv::Mat& picture, int seedNumber)
{
	
	cv::RNG seedRng = cv::theRNG();
	SeedSet seedSet;
	seedSet.position = cv::Mat::zeros(picture.rows, picture.cols, CV_8U);
	seedSet.mark = cv::Mat::zeros(picture.rows, picture.cols, CV_32S);
	seedSet.seedPSet.reserve(seedNumber);
	Seed seed;
	int rows = picture.rows; 
	int cols = picture.cols;
	int row = 0, col = 0;
	for (int i = 0; i < seedNumber; i++)
	{
		row = seedRng.uniform(0,rows);
		col = seedRng.uniform(0, cols);
		uchar pp = picture.at<uchar>(row, col);

		while (picture.at<uchar>(row, col) == 0)
		{
			row = seedRng.uniform(0, rows);
			col = seedRng.uniform(0, cols);
		}
		seed.location= cv::Point(col, row);
		uchar ps = picture.at<uchar>(seed.location);
		uchar psp = picture.at<uchar>(row, col);
		
		seed.scope = FormScope(picture,seed.location);
		seedSet.seedPSet.push_back(seed);
		seedSet.position.at<uchar>(seed.location)=1;
#ifdef SEEDSET
		if (i >= 0)
		{

			if (!outFile.is_open())
				std::cout << "No files open!";
			outFile << "i=" << i << std::endl;
			outFile << "seed.location.x=" << seed.location.x << std::endl;
			outFile << "seed.location.y=" << seed.location.y << std::endl;
			outFile << "seed.scope.x=" << seed.scope.x << std::endl;
			outFile << "seed.scope.y=" << seed.scope.y << std::endl;
			outFile << "seed.scope.width=" << seed.scope.width << std::endl;
			outFile << "seed.scope.height=" << seed.scope.height << std::endl;
			outFile << "***************" << std::endl;
			outFile << "picture[" << i << "]=" <<(int)seedSet.position.at<uchar>(seed.location) << std::endl;
		}
#endif
	}
	return seedSet;
};
std::vector<cv::Point> findScopeSeed(const SeedSet& sSet, const Seed& s)
{
	cv::Point pointTest = s.location;
	cv::Rect rectTest = s.scope;
	std::vector<cv::Point> seedSTemp;
	for (int i = s.scope.x; i <= s.scope.x + s.scope.width; i++)
	{
		for (int j = s.scope.y; j <= s.scope.y + s.scope.height; j++)
		{
			if (sSet.position.at<uchar>(j, i) == 1)
			{
				seedSTemp.push_back(cv::Point(i, j));
			}		
		}
	}
	return seedSTemp;
};
cv::Point averagePoint(const std::vector<cv::Point>& vectorPoint)
{
	int size = vectorPoint.size();
	int averageX = 0, averageY = 0;
	for (int i = 0; i < size; i++)
	{
		averageX += vectorPoint[i].x;
		averageY += vectorPoint[i].y;
	}
	averageX /= size;
	averageY /= size;
	return cv::Point(averageX, averageY);
};
void setMark(SeedSet& sSet, const cv::Rect& rect)
{
	cv::Mat& m = sSet.mark;
	int x = rect.x, y = rect.y, width = rect.width, height = rect.height;
	for (int i = x; i < x + width; i++)
	{
		for (int j = y; j < y + height; j++)
		{
			m.at<int>(j, i) = 1;
		}
	}
};
Seed findMaxSeed(const SeedSet& sSet, const cv::Rect& rect, Seed averageS
	, const cv::Mat& picture)
{
	const cv::Mat& m = sSet.position;
	int x = rect.x, y = rect.y, width = rect.width, height = rect.height;
	cv::Point maxPoint = averageS.location,midP;
	cv::Rect maxRect = averageS.scope,midR;
	for (int i = x; i < x + width; i++)
	{
		for (int j = y; j < y + height; j++)
		{
			if (m.at<uchar>(j, i) == 1)
			{
				midP.x = i, midP.y = j;
				midR = FormScope(picture, midP);
				if (midR.area() > maxRect.area())
				{
					maxPoint = midP;
					maxRect = midR;
				}
			}
		}
	}
	Seed newSeed;
	newSeed.location = maxPoint;
	newSeed.scope = maxRect;
	return newSeed;
};
SeedSet MergeSeed(const cv::Mat& picture,  SeedSet& sSet)
{
	SeedSet newSet;
	int sn = sSet.seedPSet.size();
	newSet.mark = cv::Mat::zeros(picture.rows, picture.cols, CV_32S);
	newSet.position = cv::Mat::zeros(picture.rows,picture.cols,CV_8U);
	newSet.seedPSet.reserve(sn*0.4);
	Seed newSeed;
	for (int i = 0; i < sn; i++)
	{
		cv::Point pointTest = sSet.seedPSet[i].location;
		cv::Rect rectTest = sSet.seedPSet[i].scope;
		if (sSet.mark.at<int>(pointTest)==1)
		{
			continue;
		}
		std::vector<cv::Point> seedSTemp;
		seedSTemp = findScopeSeed(sSet, sSet.seedPSet[i]);
		cv::Point averageP = averagePoint(seedSTemp);
		if (picture.at<uchar>(averageP) == 0)
			averageP = maxScope(picture,seedSTemp);
		cv::Rect averageR = FormScope(picture,averageP);
		if (rectTest.area() >= averageR.area())
		{
			newSet.seedPSet.push_back(sSet.seedPSet[i]);
			newSet.position.at<uchar>(pointTest) = 1;
			setMark(sSet, sSet.seedPSet[i].scope);
		}
		else
		{
			setMark(sSet, sSet.seedPSet[i].scope);
			while (true)
			{
				setMark(sSet,averageR);	
				newSeed = findMaxSeed(sSet, averageR,Seed(averageP,averageR),picture);
				if (averageR.area() >= newSeed.scope.area())
				{
					newSet.seedPSet.push_back(Seed(averageP,averageR));
					newSet.position.at<uchar>(averageP) = 1;
					break;
				}
				setMark(sSet, newSeed.scope);
				averageP = newSeed.location;
				averageR = newSeed.scope;
			}
		}
	}
	return newSet;
};
cv::Mat drawSeed(const cv::Mat& picture, const SeedSet& seedS, const cv::Scalar&color)
{
	int rows = seedS.position.rows, cols = seedS.position.cols;
	cv::Mat drawSeedPicture;
	cv::Point p;
	picture.copyTo(drawSeedPicture);
	int count = seedS.seedPSet.size();
	for (int i = 0; i < count; i++)
	{
		p = seedS.seedPSet[i].location;
		cv::Rect r = seedS.seedPSet[i].scope;
		int x0 = p.x;
		int y0 = p.y;
		int x1 = r.x;
		int y1 = r.y;
		int width = r.width;
		int height = r.height;
		cv::circle(drawSeedPicture,p,5,color,5);
	}
	return drawSeedPicture;
};
cv::Point maxScope(const cv::Mat &picture,std::vector<cv::Point>& seedSTemp)
{
	cv::Rect maxR,tempRect;
	maxR = FormScope(picture, seedSTemp[0]);
	int count = seedSTemp.size(),maxNumber=0;
	for (int i = 1; i < count; i++)
	{
		tempRect = FormScope(picture, seedSTemp[i]);
		if (tempRect.area() > maxR.area())
		{
			maxR = tempRect;
			maxNumber = i;
		}
	}
	return seedSTemp[maxNumber];
};
SeedSet Seed3Point(const cv::Mat& picture, int seedNumber, const std::string& pastWorkFolder)
{
	cv::Mat pointP = picture.clone();
	cv::RNG seedRng = cv::theRNG();
	SeedSet seedSet;
	seedSet.position = cv::Mat::zeros(picture.rows, picture.cols, CV_8U);
	seedSet.mark = cv::Mat::zeros(picture.rows, picture.cols, CV_32S);
	seedSet.seedPSet.reserve(seedNumber);
	Seed seed;
	int rows = picture.rows;
	int cols = picture.cols;
	int row = 0, col = 0;
	cv::Mat seed_mark = cv::Mat::zeros(rows, cols, CV_8S);
	int ij = 0;
	int border_number = int(sqrtf(seedNumber)) + 1;
	for (int i = 1; i < border_number ; i++)
	{
		for (int j = 1; j < border_number ; j++)
		{
		row = seedRng.uniform((double)0, (double)1.0) * rows / border_number +(i-1)*rows/border_number;
		col = seedRng.uniform((double)0, (double)1.0) * cols / border_number +(j-1)*cols/border_number;
		uchar pp = picture.at<uchar>(row, col);
		std::vector<cv::Point> polygonPointSet;
		cv::Point position(col, row);
		int count = 0;
#ifdef GET_PLOYGON_SIXTEEN_POINT
		getPolygonSixTeenPoint(position, picture, polygonPointSet);
#else
		getPolygonPoint(position, picture, polygonPointSet);
#endif
		while ((is_outline(picture,row, col)||!(cv::isContourConvex(polygonPointSet))||(picture.at<uchar>(position) == 0)/*判断该种子点是否可以产出8个控制点凸集*/))
		{
			polygonPointSet.clear();
			row = seedRng.uniform((double)0, (double)1.0) * rows / border_number + (i - 1) * rows / border_number;
			col = seedRng.uniform((double)0, (double)1.0) * cols / border_number + (j - 1) * cols / border_number;
			position = cv::Point(col, row);
			/*cv::circle(pointP, position, 5, cv::Scalar(125, 125, 125), 5);*/
#ifdef GET_PLOYGON_SIXTEEN_POINT
			getPolygonSixTeenPoint(position, picture, polygonPointSet);
#else
			getPolygonPoint(position, picture, polygonPointSet);
#endif
			if (++count > END_LOOP)
			{
				break;
			}
			
		}
		if (count < END_LOOP)
		{
			seed.location = cv::Point(col, row);
			uchar ps = picture.at<uchar>(seed.location);
			uchar psp = picture.at<uchar>(row, col);
			seed.scope = FormScope(picture, seed.location);
			seedSet.seedPSet.push_back(seed);
			seedSet.position.at<uchar>(seed.location) = 1;
			ij++;
			std::cout << "ij=" << ij << std::endl;
		}
	    }
    }
	std::cout << "************************************************************" << std::endl;
	//std::string possibleSeedSA = pastWorkFolder;
	//possibleSeedSA += "possibleSeedPicture.JPG";
	//cv::imwrite(possibleSeedSA, pointP);
	return seedSet;
};
bool is_outline(const cv::Mat& picture, const int rows, const int cols)
{
	cv::Point center(cols, rows);
	cv::Point top_left, bottom_right;
	top_left.x = center.x - 3 * OUTLINE_NUMBER - 1;
	top_left.y = center.y - 3 * OUTLINE_NUMBER - 1;
	bottom_right.x = center.x + 3 * OUTLINE_NUMBER + 2;
	bottom_right.y = center.y + 3 * OUTLINE_NUMBER + 2;
	if (
		(top_left.x < 0 || top_left.y < 0) ||
		(bottom_right.x >= picture.cols) ||
		(bottom_right.y >= picture.rows)
		)
	{
		return true;
	}
	cv::Rect influenceSope(top_left, bottom_right);
	const cv::Mat child_picture = picture(influenceSope);
	if (is_child_outline(child_picture))
	{
		return true;
	}
	else
	{
		return false;
	}
};
bool is_child_outline(const cv::Mat& child_picture)
{
	for (int i = 0; i < child_picture.rows; i++)
	{
		for (int j = 0; j < child_picture.cols; j++)
		{
			if (child_picture.at<uchar>(i, j) == 0)
			{
				return true;
			}
		}
	}
	return false;
};
cv::Mat drawEightPolygonPointInOutline(const cv::Mat& OldOutline, const cv::Point& seedP, const std::vector<cv::Point>& polygonPointSet)
{
	int rows = seedP.y, cols = seedP.x;
	cv::Mat drawPolygonPointPicture;
	cv::Point first,second;
	OldOutline.copyTo(drawPolygonPointPicture);
	int count = polygonPointSet.size();
	for (int i = 0; i < count; i++)
	{
		int j = (i + 1) % count;
		first = polygonPointSet[i];
		second= polygonPointSet[j];
		cv::line(drawPolygonPointPicture, first, second, 185,3);
	}
	cv::circle(drawPolygonPointPicture, seedP, 40, 125, 15);
	return drawPolygonPointPicture;
};
void setSeedPositionMarkValue(cv::Mat& seed_mark, std::vector<cv::Point> &polygonPointSet)
{
	for (int x=0;x<seed_mark.cols;x++)
	{
		for (int y = 0; y < seed_mark.rows; y++)
		{
			cv::Point location(x,y);
			if (cv::pointPolygonTest(polygonPointSet, location, true)>0)
			{
				seed_mark.at<uchar>(location) = 1;
			}
		}
	}
};
SeedSet newSeedMergeS(const cv::Mat& outline, SeedSet& seedS, std::string pastWorkFolder)
{
#ifdef OUTPUT_SEED_PICTURE
	std::string outline_seed_address = pastWorkFolder + "newOutPicture\\";
	if (0 != ::_access(outline_seed_address.c_str(), 0))
	{
		// if this folder not exist, create a new one.
		_mkdir(outline_seed_address.c_str()); 
	}
	outline_seed_address += "outlineSeed\\";
	if (0 != ::_access(outline_seed_address.c_str(), 0))
	{
		// if this folder not exist, create a new one.
		_mkdir(outline_seed_address.c_str()); 
	}
	cv::Mat seedPicture;
	seedPicture = drawSeed(outline, seedS, cv::Scalar(125, 125, 125),10);
#endif

	SeedSet newSeedS;
	newSeedS.mark = cv::Mat::zeros(outline.rows, outline.cols, CV_8U);
	newSeedS.position = cv::Mat::zeros(outline.rows,outline.cols,CV_8U);
	seedS.mark = cv::Mat::zeros(outline.rows, outline.cols, CV_8U);
	for (int i = 0; i < seedS.seedPSet.size(); i++)
	{

		if (seedS.mark.at<uchar>(seedS.seedPSet[i].location) != 0)
		{
			continue;
		}
#ifdef OUTPUT_SEED_PICTURE
		std::string seed_picture_i = "i_" + std::to_string(i);
#endif
		Seed NewSeed;
		NewSeed.location = cv::Point(-1, -1);
		std::vector<cv::Point>polygonGS_i;
		std::vector<cv::Point>cull_point_ij_set;
		getPolygonPoint(seedS.seedPSet[i].location, outline, polygonGS_i);
		deleteNewUntrustPoint(seedS.seedPSet[i].location, polygonGS_i, END_DELETE_POINT);
		for (int j = 0; j < seedS.seedPSet.size(); j++)
		{
			if (i == j)
			{
				continue;
			}
			if (seedS.mark.at<uchar>(seedS.seedPSet[j].location)==0&&cv::pointPolygonTest(polygonGS_i, seedS.seedPSet[j].location, false) == 1)
			{
				seedS.mark.at<uchar>(seedS.seedPSet[j].location) = 1;
				std::vector<cv::Point>polygonGS_j;
				getPolygonPoint(seedS.seedPSet[j].location, outline, polygonGS_j);
				deleteNewUntrustPoint(seedS.seedPSet[j].location, polygonGS_j, END_DELETE_POINT);
				for (int k = 0; k < polygonGS_j.size(); k++)
				{
					polygonGS_i.push_back(polygonGS_j[k]);
				}
				cv::convexHull(polygonGS_i, cull_point_ij_set);
				cv::RotatedRect tempSeedRect;
				tempSeedRect = cv::minAreaRect(cull_point_ij_set);
				cv::Point tsp_center = tempSeedRect.center;
				NewSeed.location = tsp_center;
				NewSeed.scope = FormScope(outline, tsp_center);
#ifdef OUTPUT_SEED_PICTURE
				std::string seed_picture_j = "j_" + std::to_string(j);
				std::string output_address_ij = outline_seed_address + seed_picture_i + seed_picture_j + "outline_seed.JPG";
				std::vector<cv::Point>cull_point_set;
				cv::circle(seedPicture, seedS.seedPSet[i].location, 10, cv::Scalar(50, 50, 50));
				cv::circle(seedPicture, seedS.seedPSet[j].location, 5, cv::Scalar(100, 100, 100));
				drawNewPointSetOnPolygonOutline(seedPicture, cull_point_ij_set);
				cv::circle(seedPicture, tsp_center, 15, cv::Scalar(30, 30, 30));
				cv::imwrite(output_address_ij, seedPicture);
#endif
			}
		}
		if (NewSeed.location != cv::Point(-1, -1))
		{
			std::vector<cv::Point>temp_polygonGS_i;
			getPolygonPoint(NewSeed.location, outline, temp_polygonGS_i);
			if (cv::isContourConvex(temp_polygonGS_i))
			{
				newSeedS.seedPSet.push_back(NewSeed);
				newSeedS.position.at<uchar>(NewSeed.location) = 1;
			}
			else
			{
				cv::RotatedRect tempNewSeedRect;
				tempNewSeedRect = cv::minAreaRect(temp_polygonGS_i);
				cv::Point tsp_new_center ;
				int count = 0;
				do
				{
					 tsp_new_center=getNewPointInRect( temp_polygonGS_i);
					 temp_polygonGS_i.clear();
					 getPolygonPoint(tsp_new_center, outline, temp_polygonGS_i);
					 count++;
				} while (!cv::isContourConvex(temp_polygonGS_i)&&count<TEMP_END_NUMBER);
				if (count< TEMP_END_NUMBER)
				{
					Seed seedTemp;
					seedTemp.location = tsp_new_center;
					seedTemp.scope = FormScope(outline,tsp_new_center);
					newSeedS.seedPSet.push_back(seedTemp);
					newSeedS.position.at<uchar>(tsp_new_center) = 1;
				}
				else
				{
					newSeedS.seedPSet.push_back(seedS.seedPSet[i]);
					newSeedS.position.at<uchar>(seedS.seedPSet[i].location) = 1;
				}
			}
		}
		else
		{
			std::vector<cv::Point>new_cull_point_ij_set;
			cv::convexHull(polygonGS_i, new_cull_point_ij_set);
			cv::RotatedRect tempSeedRect;
			tempSeedRect = cv::minAreaRect(new_cull_point_ij_set);
			cv::Point tsp_center = tempSeedRect.center;
			NewSeed.location = tsp_center;
			NewSeed.scope = FormScope(outline, tsp_center);
			std::vector<cv::Point>temp_polygonGS_i;
			getPolygonPoint(NewSeed.location, outline, temp_polygonGS_i);
			if (cv::isContourConvex(temp_polygonGS_i))
			{
				newSeedS.seedPSet.push_back(NewSeed);
				newSeedS.position.at<uchar>(NewSeed.location) = 1;
			}
			else
			{
				cv::RotatedRect tempNewSeedRect;
				tempNewSeedRect = cv::minAreaRect(temp_polygonGS_i);
				cv::Point tsp_new_center;
				int count = 0;
				do
				{
					tsp_new_center = getNewPointInRect(temp_polygonGS_i);
					temp_polygonGS_i.clear();
					getPolygonPoint(tsp_new_center, outline, temp_polygonGS_i);
					count++;
				} while (!cv::isContourConvex(temp_polygonGS_i) && count < TEMP_END_NUMBER);
				if (count < TEMP_END_NUMBER)
				{
					Seed seedTemp;
					seedTemp.location = tsp_new_center;
					seedTemp.scope = FormScope(outline, tsp_new_center);
					newSeedS.seedPSet.push_back(seedTemp);
					newSeedS.position.at<uchar>(tsp_new_center) = 1;
				}
				else 
				{
					newSeedS.seedPSet.push_back(seedS.seedPSet[i]);
					newSeedS.position.at<uchar>(seedS.seedPSet[i].location) = 1;
				}
			}
		}
	}
	return newSeedS;
};
cv::Mat drawSeed(const cv::Mat& picture, const SeedSet& seedS, const cv::Scalar& color, const int diameter)
{
	int rows = seedS.position.rows, cols = seedS.position.cols;
	cv::Mat drawSeedPicture;
	cv::Point p;
	picture.copyTo(drawSeedPicture);
	int count = seedS.seedPSet.size();
	for (int i = 0; i < count; i++)
	{
		p = seedS.seedPSet[i].location;
		cv::Rect r = seedS.seedPSet[i].scope;
		int x0 = p.x;
		int y0 = p.y;
		int x1 = r.x;
		int y1 = r.y;
		int width = r.width;
		int height = r.height;
		cv::circle(drawSeedPicture, p, diameter, color, 10);
	}
	return drawSeedPicture;
};
cv::Point getNewPointInRect(std::vector<cv::Point>& temp_polygonGS_i)
{
	cv::Rect tempNewSeedRect;
	tempNewSeedRect = cv::boundingRect(temp_polygonGS_i);
	int height = tempNewSeedRect.height;
	int width = tempNewSeedRect.width;
	double coff_x = seedNewRng.uniform(0.2, 0.8);
	double coff_y = seedNewRng.uniform(0.2, 0.8);
	int x = tempNewSeedRect.x + (int)(coff_x * width);
	int y = tempNewSeedRect.y + (int)(coff_y * height);
	cv::Point position = cv::Point(x, y);
	return position;
};
SeedSet newSeedPointMergeS(const cv::Mat& outline, SeedSet& seedS, std::string pastWorkFolder)
{
	std::multimap<int, std::pair<int,cv::Point>> seedSPair;
	copySeedSetToSeedSetPair( seedS,  seedSPair,  outline);
	InputRightDeleteBadSeedPoint( outline, seedSPair);
#ifdef NEW_SEED_POINT_MERGE_S
	for (auto it = seedSPair.begin(); it != seedSPair.end(); ++it)
	{
		findBestSeedPointInScopeAndDeleteBadSeedPoint(seedSPair,it,outline);
	}
	cv::Mat outputOutline;
	outline.copyTo(outputOutline);
	for (auto it = seedSPair.begin(); it != seedSPair.end(); it++)
	{
		cv::circle(outputOutline,it->second.second,5,50,5);
	}
	std::string seedSA = pastWorkFolder;
	seedSA += "BestSeedPicture.JPG";
	cv::namedWindow("BestSeedPicture", cv::WINDOW_NORMAL);
	cv::imshow("BestSeedPicture", outputOutline);
	cv::waitKey(5000);
#endif
	SeedSet returnSeedS;
	fromMapCopyToSeedSet(seedSPair, returnSeedS,outline);
	return returnSeedS;
};
void copySeedSetToSeedSetPair(SeedSet& seedS, std::multimap<int, std::pair<int, cv::Point>>& seedSPair,const cv::Mat&outline)
{
	std::multimap<int, cv::Point>tempSeedSPair;
	for (auto it = seedS.seedPSet.begin(); it != seedS.seedPSet.end(); it++)
	{
		std::vector<cv::Point>polygonPointSet;
		getPolygonPoint((*it).location, outline, polygonPointSet);
		deleteNewUntrustPoint((*it).location, polygonPointSet, 0);
		std::pair<int, cv::Point> tempS;
		tempS.first = (polygonPointSet[0].x - (*it).location.x) * (polygonPointSet[0].x - (*it).location.x)
			+ (polygonPointSet[0].y - (*it).location.y) * (polygonPointSet[0].y - (*it).location.y);
		tempS.second = (*it).location;
		tempSeedSPair.insert(tempS);
	}
#ifdef COPY_SEED_SET_TO_SEED_SET_PAIR
	int ii = 0;
	for (auto it = tempSeedSPair.begin(); it != tempSeedSPair.end(); it++)
	{
		ii++;
		std::cout << "ii="<<ii<<" "<<(*it).first << std::endl;
	}
#endif
	int i = 0;
	for (auto it= tempSeedSPair.begin();it!= tempSeedSPair.end();it++)
	{
		std::pair<int, std::pair<int, cv::Point>> tempSP;
		tempSP.first = i++;
		tempSP.second = *it;
		seedSPair.insert(tempSP);
	}
};
void InputRightDeleteBadSeedPoint(const cv::Mat& outline, std::multimap<int, std::pair<int, cv::Point>>& seedSPair)
{
	cv::Mat newOutline;
	float value;
	int deleteNumber;
	std::multimap<int, std::pair<int, cv::Point>>::const_iterator it;
	do {
		outline.copyTo(newOutline);
#ifdef  NEED_INPUT_DELET_NUMBER
		do {
			std::cout << "Please enter the proportion of image deletion that closely follows the outline seed points (a number between 0.-0.999 is more appropriate, note that deleting too large a proportion will result in some sand grains not having seed points)：";
			std::cin >> value;
			if (value >=0 && value <1)
			{
				break;
			}
			else
			{
				std::cout << "The value entered does not match the requirement!";
			}
		} while (true);
#else
		value = DELETE_DOWN_NUMBERS;
#endif
		deleteNumber = value * seedSPair.size();
		it = seedSPair.upper_bound(deleteNumber);
		drawSeedPointInOutline( newOutline,it,seedSPair);
		cv::imwrite("Delete_down_numbers.jpg",newOutline);
		cv::namedWindow("NewOutlinePicture", cv::WINDOW_NORMAL);
		cv::imshow("NewOutlinePicture", newOutline);
		cv::waitKey(5000);
		char ch;
#ifdef  NEED_INPUT_DELET_NUMBER
		std::cout << "Whether the entered censored tightly bounded seed point ratio meets the requirement, enter y if it does, or n if the threshold needs to be changed!";
		std::cin >> ch;
#else
		ch = 'y';
#endif
		if (ch == 'y')
		{
			break;
		}
		else
		{
			continue;
		}


	} while (true);
	seedSPair.erase(seedSPair.cbegin(),it);
#ifdef DELETE_MAX_DISTANCE
	renewOrder( seedSPair, outline);
	cv::Mat newOutlineP;
	float valueP;
	int deleteNumberP;
	std::multimap<int, std::pair<int, cv::Point>>::const_iterator It;
	do {
		outline.copyTo(newOutlineP);
#ifdef  NEED_INPUT_DELET_NUMBER
		do {
			std::cout << "Please enter the percentage of deletion of implausible seed points from the image (a number between 0.-0.999 is more appropriate, note that too large a percentage of deletion will result in some sand grains having no seed points):";
			std::cin >> valueP;
			if (valueP >= 0 && valueP < 1)
			{
				break;
			}
			else
			{
				std::cout << "The value entered does not match the requirement!";
			}
		} while (true);
#else
		valueP = DELETE_UP_NUMBERS;
#endif
		deleteNumberP =(--(seedSPair.end()))->first - valueP * seedSPair.size();
		It = seedSPair.lower_bound(deleteNumberP);
		for (auto it = seedSPair.begin(); it != It; it++)
		{
			cv::circle(newOutlineP, (*it).second.second, 5, 125, 5);
		}
		cv::namedWindow("NewOutlinePicture", cv::WINDOW_NORMAL);
		cv::imshow("NewOutlinePicture", newOutlineP);
		cv::imwrite("Delete_up_numbers.jpg", newOutlineP);
		cv::waitKey(5000);
		char ch;
#ifdef  NEED_INPUT_DELET_NUMBER
		std::cout << "Does the entered percentage of untrustworthy seed censoring meet the requirement, enter y if it does, or n if you need to change the threshold!";
		std::cin >> ch;
#else
		ch = 'y';
#endif
		if (ch == 'y'||ch=='Y')
		{
			break;
		}
		else
		{
			continue;
		}
	} while (true);
	seedSPair.erase(It, seedSPair.cend());
#endif
};
void drawSeedPointInOutline(cv::Mat& newOutline, std::multimap<int, std::pair<int, cv::Point>>::const_iterator it,
	const std::multimap<int, std::pair<int, cv::Point >>& seedSPair)
{
	for (auto It=it;It!=seedSPair.end();It++)
	{
		cv::circle(newOutline, (*It).second.second, 5, 125, 5);
	}
};
void findBestSeedPointInScopeAndDeleteBadSeedPoint(std::multimap<int, std::pair<int, cv::Point>>& seedSPair
	,std::multimap<int, std::pair<int, cv::Point>>::iterator& It,const cv::Mat&outline)
{
	std::vector<cv::Point>polygonPointSet;
	getPolygonPoint((*It).second.second, outline, polygonPointSet);
	deleteNewUntrustPoint((*It).second.second, polygonPointSet, 2);
	for (auto it = It; it != seedSPair.end(); it++)
	{
		if (cv::pointPolygonTest(polygonPointSet, (*it).second.second, false) == 1)
		{
			if (it->second.first>It->second.first)
			{
				swap(it->second, It->second);
				it = seedSPair.erase(it);
				it--;
			}
		}
	}
};
void fromMapCopyToSeedSet(std::multimap<int, std::pair<int, cv::Point>>& seedSPair, SeedSet& returnSeedS, const cv::Mat& outline)
{
	returnSeedS.position = cv::Mat::zeros(outline.rows, outline.cols, CV_8U);
	for (auto it = seedSPair.begin(); it != seedSPair.end(); it++)
	{
		Seed tempSeed;
		tempSeed.scope = FormScope(outline, it->second.second);
		tempSeed.location = it->second.second;
		returnSeedS.seedPSet.push_back(tempSeed);
		returnSeedS.position.at<uchar>(tempSeed.location) = 1;
	};
	returnSeedS.mark = cv::Mat::zeros(outline.rows,outline.cols,CV_8U);
};
void renewOrder(std::multimap<int, std::pair<int, cv::Point>>& seedSPair,const cv::Mat&outline)
{
	std::multimap<int, std::pair<int, cv::Point>> tempSP=seedSPair;
	seedSPair.clear();
	int i = 0;
	for (auto it=tempSP.begin();it!= tempSP.end();it++)
	{
		std::pair<int, cv::Point> tempP;
		std::vector<cv::Point>polygonPointSet;
		getPolygonPoint((*it).second.second, outline, polygonPointSet);
		int MaxDistance = getMaxDistancePP((*it).second.second,polygonPointSet);
		i++;
		tempP = std::pair<int, cv::Point>(MaxDistance, (*it).second.second);
		std::pair<int, std::pair<int, cv::Point>> midTemp;
		midTemp.first = i;
		midTemp.second = tempP;
		seedSPair.insert(midTemp);
	}
};
int getMaxDistancePP(const cv::Point& point,  const std::vector<cv::Point>&polygonPointSet)
{
int maxD=0,temp=0;
std::vector<cv::Point>::const_iterator first = polygonPointSet.begin();
std::vector<cv::Point>::const_iterator second = polygonPointSet.begin() + 4;
	for (int i=0;i<4;i++)
	{
		temp = (first->x-second->x) * (first->x - second->x) + (first->y - second->y) * (first->y - second->y);
		if (temp > maxD)
		{
			maxD = temp;
		}
		++first;
		++second;
	}
	return maxD;
};