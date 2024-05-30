#include"fastSortSeedSet.h"
//对种子点的质量进行排序，优先处理相对位置靠近作用域中心的种子点
/*
建立一个以相对种子点相对作用域中心的相对位置为索引的map数组，计算所有vector中的种子点，然后在将该种子点拷贝到SeedSet中的vector数据结构中
*/
void sortSeed(SeedSet& seedMergeS, const cv::Mat& outline)
{
	std::map<double, Seed>mapSeed;
	std::vector<Seed> vectorSeed = seedMergeS.seedPSet;
	for (auto it = vectorSeed.begin(); it != vectorSeed.end(); it++)
	{
		//计算种子点相对作用域中心的相对位置
		double relativePosition = calculateRelativePosition(*it, outline);
		mapSeed.insert(std::pair<double,Seed>(relativePosition, *it));
	}
	//清除所用种子点的位置，赋予容器新位置
	seedMergeS.seedPSet.clear();
	for (auto it = mapSeed.begin(); it != mapSeed.end(); it++)
	{
		seedMergeS.seedPSet.push_back(it->second);
	}
};
//计算种子点相对作用域中心的相对位置
/*
判断种子点的作用域是否为空，如果为空则重新生成作用域，再计算作用域的中心点位置
*/
double calculateRelativePosition(Seed& s, const cv::Mat& outline)
{
	double relativePosition = 0;
	if (isRectEmpty(s.scope))//如果作用域为空则重新生成作用域
	{
		s.scope = FormScope(outline, s.location);
	}
	else
	{
		cv::Point center = cv::Point(s.scope.x + s.scope.width / 2, s.scope.y + s.scope.height / 2);
		cv::Point tl = s.scope.tl();
		cv::Point location = s.location;
		relativePosition = double(abs(location.x - center.x) + abs(location.y - center.y)) / double(abs(center.x - tl.x) + abs(center.y - tl.y));
		//if (relativePosition > 0)
		//{
		//	std::cout << "种子点与其作用域中心的相对位置超过了1，种子点和作用域的位置不匹配！";
		//	exit(-1);
		//}
	}
	return relativePosition;
};
// 判断矩形是否为空的函数  
bool isRectEmpty(const cv::Rect& rect) {
	return rect.width <= 0 || rect.height <= 0;
}
//显示排序后的种子点
cv::Mat drawSortSeed(const cv::Mat& outline, const SeedSet& seedS, cv::Scalar s)
{
	int rows = seedS.position.rows, cols = seedS.position.cols;
	cv::Mat drawSeedPicture;
	cv::Point p;
	outline.copyTo(drawSeedPicture);
	int count = seedS.seedPSet.size()*SORT_SEED_RATE;
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
		cv::circle(drawSeedPicture, p, 5, s, 5);
	}
	return drawSeedPicture;

};