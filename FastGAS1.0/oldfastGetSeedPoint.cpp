#include"fastGetSeedPoint.h"
#define FAST_OUTLINE_NUMBER 3
//生成种子点
SeedSet generateSeedPoint(const cv::Mat& picture, int seedNumber, const std::string& pastWorkFolder)
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
	for (int i = 1; i < border_number; i++)
	{
		for (int j = 1; j < border_number; j++)
		{
			row = seedRng.uniform((double)0, (double)1.0) * rows / border_number + (i - 1) * rows / border_number;
			col = seedRng.uniform((double)0, (double)1.0) * cols / border_number + (j - 1) * cols / border_number;
			uchar pp = picture.at<uchar>(row, col);
			std::vector<cv::Point> polygonPointSet;
			cv::Point position(col, row);
			int count = 0;
#ifdef GET_PLOYGON_SIXTEEN_POINT
			getPolygonSixTeenPoint(position, picture, polygonPointSet);
#else
			getPolygonPoint(position, picture, polygonPointSet);
			/*castRaysFromPoint(picture,position,  polygonPointSet);*/
#endif
			while (fastIs_outline(picture, row, col) || !(cv::isContourConvex(polygonPointSet)) || (picture.at<uchar>(position) == 0)|| (generateOffsetFactor(position,picture)>=OFFSET_FACTOR)/*判断该种子点是否可以产出8个控制点凸集*/)
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
				/*castRaysFromPoint(picture, position, polygonPointSet);*/
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
void getFourPolygonPoint(cv::Point& seedP, const cv::Mat& outline, std::vector<cv::Point>& polygonPointSet)
{
	int rows = outline.rows;
	int cols = outline.cols;
	cv::Point xy_plus;

	for (int i = 0; i < FOUR_NEIGHBOR /*EIGHT_NEIGHBOR*/; i++)
	{
		xy_plus = seedP;
		while (/*(xy_plus.x > 0 && xy_plus.x < cols) &&*/ (EMPTY != (int)outline.at<uchar>(xy_plus)))
		{
			/* xy_plus += EIGHT_CELL_NEIGHBOR[i]/3;*/
			xy_plus += FOUR_CELL_NEIGHBOR[i] / 3;
		}
		polygonPointSet.push_back(xy_plus);
	}
};

bool fastIs_outline(const cv::Mat& picture, const int rows, const int cols)
{
	cv::Point center(cols, rows);
	cv::Point top_left, bottom_right;
	top_left.x = center.x - 3 * FAST_OUTLINE_NUMBER - 1;
	top_left.y = center.y - 3 * FAST_OUTLINE_NUMBER - 1;
	bottom_right.x = center.x + 3 * FAST_OUTLINE_NUMBER + 2;
	bottom_right.y = center.y + 3 * FAST_OUTLINE_NUMBER + 2;
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
	if (fastIs_child_outline(child_picture))
	{
		return true;
	}
	else
	{
		return false;
	}
};
bool fastIs_child_outline(const cv::Mat& child_picture)
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
//生成的种子点偏移其左右域中心的相对大小
double generateOffsetFactor(const cv::Point& position, const cv::Mat& outline)
{
	cv::Point pt = position;
	cv::Rect scope= FormScope(outline, pt);
	cv::Point  center=(scope.br()+scope.tl())/2;
	double x, y;
	x = center.x - position.x;
	y = center.y - position.y;
	double offset=sqrtf(x*x+y*y)/(min(scope.height,scope.width)/2);
	
	return offset;

};
// 从给定点发射射线并记录与轮廓交点的函数  
void castRaysFromPoint(const cv::Mat& binaryImage, const cv::Point& startPoint, std::vector<cv::Point>& polygonPointSet, int numDirections ) {
	const double angleStep = 360.0 / numDirections;
	const int rayLength = 1; // 每步射线的长度  

	polygonPointSet.clear(); // 清空容器，以存储新的交点  

	for (int i = 0; i < numDirections; ++i) {
		double angle = i * angleStep;
		double radian = angle * CV_PI / 180.0;
		cv::Point currentPoint = startPoint;

		// 当射线在图像内时继续  
		while (currentPoint.x >= 0 && currentPoint.x < binaryImage.cols &&
			currentPoint.y >= 0 && currentPoint.y < binaryImage.rows) {
			// 如果当前点在轮廓上（值为0），记录交点并退出循环  
			if (binaryImage.at<uchar>(currentPoint) == 0) {
				polygonPointSet.push_back(currentPoint); // 直接在容器中记录交点  
				break;
			}
			// 计算下一个点  
			int dx = static_cast<int>(std::round(std::cos(radian) * rayLength));
			int dy = static_cast<int>(std::round(std::sin(radian) * rayLength));
			currentPoint += cv::Point(dx, dy);
		}
	}
}
