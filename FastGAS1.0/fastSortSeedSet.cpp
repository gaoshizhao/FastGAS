#include"fastSortSeedSet.h"
//�����ӵ�����������������ȴ������λ�ÿ������������ĵ����ӵ�
/*
����һ����������ӵ�������������ĵ����λ��Ϊ������map���飬��������vector�е����ӵ㣬Ȼ���ڽ������ӵ㿽����SeedSet�е�vector���ݽṹ��
*/
void sortSeed(SeedSet& seedMergeS, const cv::Mat& outline)
{
	std::map<double, Seed>mapSeed;
	std::vector<Seed> vectorSeed = seedMergeS.seedPSet;
	for (auto it = vectorSeed.begin(); it != vectorSeed.end(); it++)
	{
		//�������ӵ�������������ĵ����λ��
		double relativePosition = calculateRelativePosition(*it, outline);
		mapSeed.insert(std::pair<double,Seed>(relativePosition, *it));
	}
	//����������ӵ��λ�ã�����������λ��
	seedMergeS.seedPSet.clear();
	for (auto it = mapSeed.begin(); it != mapSeed.end(); it++)
	{
		seedMergeS.seedPSet.push_back(it->second);
	}
};
//�������ӵ�������������ĵ����λ��
/*
�ж����ӵ���������Ƿ�Ϊ�գ����Ϊ�������������������ټ�������������ĵ�λ��
*/
double calculateRelativePosition(Seed& s, const cv::Mat& outline)
{
	double relativePosition = 0;
	if (isRectEmpty(s.scope))//���������Ϊ������������������
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
		//	std::cout << "���ӵ��������������ĵ����λ�ó�����1�����ӵ���������λ�ò�ƥ�䣡";
		//	exit(-1);
		//}
	}
	return relativePosition;
};
// �жϾ����Ƿ�Ϊ�յĺ���  
bool isRectEmpty(const cv::Rect& rect) {
	return rect.width <= 0 || rect.height <= 0;
}
//��ʾ���������ӵ�
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