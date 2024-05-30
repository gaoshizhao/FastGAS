#pragma once
#include"SandSediment.h"
#include"compareCell.h"
#include"xlnt/xlnt.hpp"
#define ENLARGE_WIDTH   10//For magnification of image patches
#define CENTER_LAYER_NUMBER 1//Number of layers of centre pixels from outermost pixels
const cv::Vec3b CENTER_COLOR({ 255,255,255 });
const cv::Vec3b BORDERARY_COLOR({255,0,0});
const cv::Vec3b EMPTY_COLOR({0,0,0});
const cv::Vec3b NEXT_COLOR({125,125,125});
const uchar SINGE_EMPTY_COLOR(0);
//Show Similar data
void displaySimilar(const Similar& similar);
//Display Cell data
void displayCell(const Cell& cell);
//Show maxCell
void displayMaxCell(const Cell& cell);
//Show minCell
void displayMinCell(const Cell& cell);
//Show markers
void displayMark(cv::Mat mark, const std::vector<Cell>& cellSet);
//Show images
void displayPicture(const cv::Mat& picture, const std::string& name);
//Show marker on colour image
void displayColorMark(cv::Mat ColorPicture, const std::vector<Cell>& tempCellSet,std::vector<Cell>cellSet);
//Display of pixel values
std::ostream& operator<<(std::ostream&os,const cv::Vec3b&pix);
//Display of coordinate values
std::ostream& operator<<(std::ostream&os,const cv::Point&coordination);
//Pixel points within a neighborhood of marker centroid 8
void markColorPicture(cv::Mat& newColorPicture, const cv::Point& location, int layerNumber = CENTER_LAYER_NUMBER);
//In a more pronounced colour
void displayMark(const cv::Mat& mark);
//Determine if the matrix is empty
bool is_empty(const cv::Mat& TestCellPosition_Mark);
//Display of non-energy position coordinates and values in the mark matrix
void writeMarkValue(const cv::Mat& mark, const std::string& name);
//Output images
void writePicture(const cv::Mat& mark, const std::string& name);
//Output the location, properties and direction of the searched spots
void writeCellPositionAndAttributeAndDirection(xlnt::worksheet&dataSheet, const cv::Point&position,const int &direction,const int &attribution);
//Output the location, properties and direction of the searched spots
void writeCellPositionAndAttributeAndDirection_partial(xlnt::worksheet& dataSheet, const cv::Point& position, const int& direction, const int sequence);
//Output the location, properties and direction of the searched spots
void writeCellPositionAndAttributeAndDirection_partial(xlnt::worksheet& dataSheet, const cv::Point& position,
	const int& direction, const int& attribution, const int sequence);
//Output the location, properties and direction of the searched spots
void writeCellPositionAndAttributeAndDirection(xlnt::worksheet& dataSheet,
	const cv::Point& position,
	const int& direction, const int& attribution, const int sequence);
//Update workSheet
void renewWorkSheet(const AdjustPosition& adPosition, xlnt::workbook& dataBook
	, xlnt::worksheet& dataSheet);
//Writing data to a cell
void writeOrClearWrite(xlnt::worksheet& dataSheet,const cv::Point& position,std::string content);
//Merge images
void mergeTwoColorPicture();