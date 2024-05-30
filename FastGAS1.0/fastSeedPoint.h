#pragma once
#include <iostream>
#include<vector>
#include<opencv2/opencv.hpp>
#define OUTLINE_NUMBER   7//Seed spot periphery width (refers to 3 triple patch widths 3*7 (7 is seven patches))

struct Seed
{
	cv::Point location;//Coordinates of the location of the seed point
	cv::Rect scope;//Seed point sphere of influence
	Seed() :location(cv::Point(0, 0)), scope(cv::Rect(0, 0, 0, 0)) {};
	Seed(cv::Point p, cv::Rect s) :location(p), scope(s) {}
};
struct SeedSet
{
	std::vector<Seed> seedPSet;//Set of coordinates for seed point locations
	cv::Mat position;//Location information for seed points
	cv::Mat mark;//种子点被访问后的标记数组
	SeedSet() {};
	SeedSet(const std::vector<Seed>& sv, const cv::Mat& p, const cv::Mat& m)
		:seedPSet(sv), position(p), mark(m) {}
};
//When defining the number of seed points, it should be about 10 times the actual number of sand grains
SeedSet SeedPoint(const cv::Mat& picture, int seedNumber);
//Generate a range of action for each seed point
cv::Rect FormScope(const cv::Mat& picture, cv::Point& position);
//Seed point merging
SeedSet MergeSeed(const cv::Mat& picture, SeedSet& sSet);
//Find a Sphere of Influence
std::vector<cv::Point> findScopeSeed(const SeedSet& sSet, const Seed& s);
//Obtain the average seed point within the area of influence of the seed point
cv::Point averagePoint(const std::vector<cv::Point>& vectorPoint);
//Set marker matrix value
void setMark(SeedSet& sSet, const cv::Rect& rect);
//Seeking the largest of the seed points in the area of influence
Seed findMaxSeed(const SeedSet& sSet, const cv::Rect& rect, Seed averageS
	, const cv::Mat& picture);
//Plotting seed points
cv::Mat drawSeed(const cv::Mat& picture, const SeedSet& seedS, const cv::Scalar& color);
//Find the location of the seed point with the largest area of influence within the group
cv::Point maxScope(const cv::Mat& picture, std::vector<cv::Point>& seedSTemp);
//Produces 3-ring seed points
SeedSet Seed3Point(const cv::Mat& picture, int seedNumber, const std::string& pastWorkFolder);
//Perform non-edge seed point determination
bool is_outline(const cv::Mat& picture, const int rows, const int cols);
//Determining whether there are edge points within the area of influence of a seed point
bool is_child_outline(const cv::Mat& child_picture);
//Display the eight control points in the OldOutline image
cv::Mat drawEightPolygonPointInOutline(const cv::Mat& OldOutline, const cv::Point& seedP, const std::vector<cv::Point>& polygonPointSet);
//Set the control range for each seed point
void setSeedPositionMarkValue(cv::Mat& seed_mark, std::vector<cv::Point>& polygonPointSet);
//New combined seed points
SeedSet newSeedMergeS(const cv::Mat& outline, SeedSet& seedS, std::string pastWorkFolder);
//Plotting seed points
cv::Mat drawSeed(const cv::Mat& picture, const SeedSet& seedS, const cv::Scalar& color, const int diameter);
//Find a new random point near the centre of the rotating rectangle
cv::Point getNewPointInRect(std::vector<cv::Point>& temp_polygonGS_i);
//New combined seed points
SeedSet newSeedPointMergeS(const cv::Mat& outline, SeedSet& seedS, std::string pastWorkFolder);
//Copy the seeds from the seed container to a new associated container for
void copySeedSetToSeedSetPair(SeedSet& seedS, std::multimap<int, std::pair<int, cv::Point>>& seedSPair, const cv::Mat& outline);
//Enter the appropriate percentage of censored seed points
void InputRightDeleteBadSeedPoint(const cv::Mat& outline, std::multimap<int, std::pair<int, cv::Point>>& seedSPair);
//Draw the seed point at the specified location
void drawSeedPointInOutline(cv::Mat& newOutline, std::multimap<int, std::pair<int, cv::Point>>::const_iterator it,
	const std::multimap<int, std::pair<int, cv::Point >>& seedSPair);
//Find the optimal seed point within the specified influence range and eliminate unsuitable seed points
void findBestSeedPointInScopeAndDeleteBadSeedPoint(std::multimap<int, std::pair<int, cv::Point>>& seedSPair
	, std::multimap<int, std::pair<int, cv::Point>>::iterator& It, const cv::Mat& outline);
//Copy the corresponding seed elements from the associated container into the SeedSet container
void fromMapCopyToSeedSet(std::multimap<int, std::pair<int, cv::Point>>& seedSPair, SeedSet& returnSeedS, const cv::Mat& outline);
//Update the seedSPair ordering to sort by the size of the largest distance from the boundary
void renewOrder(std::multimap<int, std::pair<int, cv::Point>>& seedSPair, const cv::Mat& outline);
//Get the maximum distance of the seed point from the boundary
int getMaxDistancePP(const cv::Point& point, const std::vector<cv::Point>& polygonPointSet);
