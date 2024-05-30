#pragma once
#include<vector>
#include"SeedPoint.h"
#include"SandSediment.h"
struct Cell;
#define COLOR_CHANNELS   3//Define the number of pixel channels
#define SIMILAR_COLOR_NUMBER 5//The pixels of this patch point can be modified in the pixel colour space
#define SIMILAR_SAND_NUMBER  3// Minimum number of similar patches in the 8-neighbourhood
#define FOUR_SAND_NUMBER 3//Number of 4-neighbours
#define CELL_NUMBER      5//Number of pixel points within a sand patch
#define EIGHT_NEIGHBOR   9//Number of pixel points at four opposite corners in the 8-neighbourhood
#define COMPARE_EIGHT_NEIGHBOR  8//Number of pixel patches in the 8-neighbourhoods compared
//Rotation matrix of pixel point positions for eight neighbourhoods
const cv::Point EIGHT_NEIGHBOR_CELL[EIGHT_NEIGHBOR]
= { cv::Point(-1,-1),cv::Point(0, -1),cv::Point(1, -1),
    cv::Point(-1, 0),cv::Point( 0, 0),cv::Point( 1, 0), 
	cv::Point(-1, 1),cv::Point( 0, 1),cv::Point( 1, 1)
  };
//Location of all coordinates of Cell patches
const cv::Point Cell_NEIGHBOR_COORDINATE[EIGHT_NEIGHBOR]
= { cv::Point( 0, 0),cv::Point(1, 0),cv::Point(2, 0),
	cv::Point( 0, 1),cv::Point(1, 1),cv::Point(2, 1),
	cv::Point( 0, 2),cv::Point(1, 2),cv::Point(2, 2)
};
//Offsets relative to the centroid in the 8-neighbourhood
const cv::Point COMPARE_EIGHT_CELL_NEIGHBOR[COMPARE_EIGHT_NEIGHBOR]
= { cv::Point(3,0),cv::Point(3,-3),cv::Point(0,-3)
    ,cv::Point(-3,-3),cv::Point(-3,0),
cv::Point(-3,3),cv::Point(0,3) , cv::Point(3,3) };
typedef cv::Vec3b Scalar_Color;
//Colour data type
struct Color
{
	Scalar_Color minColor;//Lower limit value in colour space
	Scalar_Color maxColor;//Upper limit in the colour space
	Color()  {};
};
//Colour judgment data type
struct IsSandPlaque
{
	bool isSandPlaque;
	bool isColorPlaque;
};
//Adjustment of individual pixel patches
void adjustColor(Color& color, const Cell& cell);
//Adjustment of multiple pixel patches
void adjustColor(Color& color, const std::vector<Cell>& cellSet);
//Determine the colour of pixel patches
void CompareCellColor(const cv::Mat& colorPicture, Color& sandColor, const Cell& tempCell, IsSandPlaque& isSandPlaque, const cv::Mat& bourdaryMark,const cv::Mat&outline);
//Return the maximum channel pixel value
uchar max(const uchar& lhs, const uchar& rhs);
//Return the minimum channel pixel value
uchar min(const uchar& lhs, const uchar& rhs);
//Determine the colour of a pixel point
bool comparePixelColor(const cv::Mat& colorPicture, Color& sandColor, const Cell& tempCell);
//The 4-neighbourhoods immediately adjacent to the seed point are pixel patches within the same sand grain
void get4Neighbor(const cv::Mat& picture, std::vector<Cell>& sCell, const cv::Point position);
//Make colour assignments
void setColor(Color& sandColor, const Cell& cell);
//The 4-neighbourhoods immediately adjacent to the seed point are pixel patches within the same sand grain
void get4NeighborCell(const cv::Mat& colorPicture
	, std::vector<Cell>& fourNeightorSet, cv::Point position);
//The 8-neighbourhoods immediately adjacent to the seed point are pixel patches within the same sand grain
void get8NeighborCell(const cv::Mat& colorPicture
	, std::vector<Cell>& eightNeightorSet, cv::Point position);
//Determine whether there is a boundary point in the eight neighbourhoods of the point to be measured, if so, the point is also determined to be a boundary point
bool eightNeighborHasBourdary(const cv::Mat& bourdaryMark, const cv::Point& position);