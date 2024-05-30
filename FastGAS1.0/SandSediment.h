#pragma once
#include<vector>
#include<opencv2/opencv.hpp>
#include"SeedPoint.h"
#include"compareCell.h"
#include"xlnt/xlnt.hpp"
#define PREDICT_NUMUBER 5
#define MAT_CHANNEL    3
#define MAT_ROW        3
#define MAT_COL        3 
#define MATX_COL       3
#define MATX_ROW       9
#define FOUR_NEIGHBOR  4//Number of neighbours in a quadruple neighbourhood
#define EIGHT_NEIGHBOR 8//Number of domains in eight neighbourhoods
#define CELL_W_H       3//Length and width of pixel spots
#define EIGHT_FOUR_NEIGHBOR  4//Number of pixel points at four opposite corners in the eight neighbourhood
#define SIMILAR_DEGREE    0.8//Similarity Threshold
#define OUTLINE_MARK    -1//Boundary markers
#define EMPTY           0//No visited markers
#define SAND            1//Sand grains
#define NO_DETERMINE_MARK 0//Sand is the searched area marked with grains
#define INCREMENT_CELL   3//Incremental values in mark for scanning pixel patches horizontally or vertically
#define TEST_CELL_POSITION_MARK 1//TestCellPosition search direction visited tags
#define EIGHT_NEIGHBOR_NUMBER   9//Relative positions of the nine elements in the eight neighbourhoods with respect to their centroids
#define SIXTEEN_NEIGHBOR 8//Number of 1/16 directions
#define UP        0
#define LEFT      1
#define DOWN      2
#define RIGHT     3
#define INCREMENT 1
#define VISITED_MARK -2//Already visited markers
#define BORDER_WIDTH 30//Additional boundary width
#define END_SEARCH_TESTING_NUMBER 15000//Search end detection threshold if the total number of image spots exceeds this value
                                       //means that the boundaries do not converge and end-of-search detection is performed
#define SAND_RADIUS_NUMBER 100//Obtain sample size of sand grain radius for calculation of mean equivalent radius
#define PI    3.1412926//Define the value of ¦Ð
#define AREA_NUMBER  2//Equivalent number of sand grains area
#define EXPANSION_CONTROL_POINT_SEED_NUMBER 40//Control the number of seed points
#define POP_CONTROL_POINT_NUMBER  4//Delete the number of untrusted control points
#define OUTLINE_CELL_NUMBER  1//Define the number of layers of the machine algorithm bounding the training set, i.e. enclosing the smallest rectangle by three turns
//#define GET_PLOYGON_SIXTEEN_POINT
#define TERMINATION_LOOP  100000
#define SEARCH_MAX_NUMBER 20
//#define WRITE_EXCEL
#define THRESHOLD_SVM     25000//SVM Threshold
#define FOUR_DIMENSIONAL_SPACE
#define THREE_DIMENSIONAL_SPACE
#define RGB_PICTURE
//Scan relative position data type of the centroids of the four neighbourhood patches
enum class CenterRelativePosition{up=0,right=1,down=2,left=3};
const cv::Point centerPosition({1,1});//Coordinates of the centre of the 3*3 pixel block
//Rotation matrix of the pixel point positions in the four diagonals of the eight neighbourhoods
const cv::Point DIG_FORU_NEIGHBOR[EIGHT_FOUR_NEIGHBOR]
= { cv::Point(1,-1),cv::Point(-1,-1),cv::Point(-1,1),cv::Point(1,1) };
//Offset relative to the centroid in the four neighbourhoods of TestCellPosition
const cv::Point FOUR_CELL_NEIGHBOR[EIGHT_FOUR_NEIGHBOR]
= { cv::Point(3,0),cv::Point(0,-3),cv::Point(-3,0),cv::Point(0,3)  };
//Offset relative to the centroid in the eight neighbourhoods of TestCellPosition
const cv::Point EIGHT_CELL_NEIGHBOR[EIGHT_NEIGHBOR]
= { cv::Point(3,0),cv::Point(3,-3),cv::Point(0,-3),cv::Point(-3,-3),cv::Point(-3,0),cv::Point(-3,3),cv::Point(0,3) , cv::Point(3,3)};
std::vector<cv::Mat> SandSediment(const cv::Mat& colorPicture, SeedSet& sSet,const cv::Mat&Outline,const std::string&workFaddress, double singlePixeArea);
//Data type of a pixel point
typedef cv::Vec3i Matx13b;
//maxCell,minCell data type
typedef cv::Matx < Matx13b, MATX_ROW, 1>Matx91b;
//Sand Pixel Dots
struct SandPixel 
{
	cv::Point position;
	cv::Vec3b sColor;
};
//Produces a sand pixel point
SandPixel  setSandPixel(const cv::Point& position, const cv::Vec3b& sColor);
//3*3 pixel block
struct Cell
{
	cv::Mat cell;//Store 3*3 original image pixels
	Matx91b maxCell;//Store the sorted pixel space in positive order
	Matx91b minCell;//Store the sorted pixel space in reverse order
	cv::Point position;//Coordinates of the centre of the pixel block
	//Cell's default constructor, set to zero for all pixel points
	Cell() :cell(MAT_ROW, MAT_COL, CV_8UC3), maxCell({ {0,0,0}, {0,0,0} ,{0,0,0}, {0,0,0},{0,0,0}, {0,0,0} ,{0,0,0}, {0,0,0},  {0,0,0} }), 
		minCell({ {0,0,0}, {0,0,0} ,{0,0,0}, {0,0,0},{0,0,0}, {0,0,0} ,{0,0,0}, {0,0,0},{0,0,0} }), position(0, 0) {};
	//Mainly used to facilitate testing of individual functions
	Cell(const std::initializer_list<int>&size, std::initializer_list<uchar>& li)
		:cell(size,li), position(0, 0)
	{
	//Initialize maxCell and minCell
	//Forming a positive order pixel space maxCell
	FormMaxCell();
	//Forming a reverse-order pixel space minCell
	FormMinCell();
	}
	Cell(const cv::Mat& ce,const cv::Point& p) 
	{
		ce.copyTo(cell);
		position = p;
	FormMaxCell();
	FormMinCell();
	}
	//Forming a positive order pixel space maxCell
	void FormMaxCell();
	//Forming a reverse-order pixel space minCell
	void FormMinCell();
};
//Maximum and minimum values of pixel patch similarity
struct Similar
{
	double maxSimilar;
	double minSimilar;
};
//Data on the location and value of pixel patch centroids;
struct PositionValue
{
	//Pixel plaque centroid location
	cv::Point position;
	//Pixel plaque centroid mark value
	int markValue;
	PositionValue() :position({}), markValue(0) {}
	PositionValue(const  cv::Mat&mark,const cv::Point &pt)
	{
		position = pt;
		markValue = mark.at<int>(pt);
	}
};
//Form the data type of the temporary cell container to be tested
struct TestCellPosition
{
	int direction;
	PositionValue center;
	std::vector<PositionValue> Four_Neighbor;
	TestCellPosition() :center({}),direction(0),Four_Neighbor() {}
};
//Recording of adjustment position information
struct AdjustPosition
{
	cv::Point position;
	int attribution;
};
//Generate TestCellPosition
void formTestCellPosition(TestCellPosition& nextTCP, const cv::Mat& mark,
	const cv::Point& center, int direction);
//Copy the data from Mat to Matx93
void MatCopyToMatx91b(const cv::Mat&m, Matx91b&mtx);
//Exchange of two data
void swap(Matx13b& lhs, Matx13b& rhs);
//Comparing the similarity of two pixel speckle blocks
Similar CellSimilar(const Cell& lhs, const Cell& rhs);
//The four neighbourhoods immediately adjacent to the seed point are pixel patches within the same sand grain
void get4Neighbor(const cv::Mat& picture, std::vector<Cell>& sCell, const Seed& seed);
//Generating new pixel blocks by coordinate position
Cell fromPointToCell(const cv::Mat& picture, cv::Point& pt);
//Perform four-neighbourhood image speckle tagging
void markSeedSetMark(SeedSet& sSet, const std::vector<Cell>cellSet);
//Perform four-neighbourhood image speckle tagging
void markSeedSetMark(SeedSet& sSet, const std::vector<Cell>cellSet,int markNumber);
// Perform centroid quad-neighbourhood image speckle tagging
void markPointMark(cv::Mat&mark, const cv::Point& pt);
// Perform centroid quad-neighbourhood image speckle tagging
void markPointMark(cv::Mat& mark, const cv::Point& pt,int markNumber);
//Get the seed points within the four diagonal field image patches in the eight neighbourhoods of the seed point and make a judgement
void get4_8Neighbor(const cv::Mat& picture, std::vector<Cell>& sCell, const Seed& seed);
//Pixel size comparison
bool lessOperator(const Matx13b& left, const Matx13b& right);
//Pixel size comparison operation
bool operator<(const Matx13b& left, const Matx13b& right);
//Compute the dot product between two vectors of type Matx91b (typedef cv::Matx < Matx13b, MATX_ROW, 1>Matx91b)
double ddot(const Matx91b& lhs, const Matx91b& rhs);
//Multiplication of data of type Matx13b
double operator*(const Matx13b& left, const Matx13b& right);
//Initialisation of texture information, by seed points and by the four neighbourhoods of the sample points
void adjustTextureSimilar(Similar& sandSimilar, const std::vector<Cell>& cellSet);
//Adjustment of individual pixel speckles
void adjustTextureSimilar(Similar& sandSimilar, const std::vector<Cell>& cellSet, Cell& cell);
//Compare the pixel degree between individual pixel patches and the identified patches to see if the threshold is met, if the threshold is met then the patches are judged to be sand patches, if not then they are not sand patches
bool compareCellTexture(const Similar& sandSimilar, const std::vector<Cell>& cellSet, const Cell& cell);
//	Search for possible pixel patches to be determined
#ifdef WRITE_EXCEL
void formTempCellSet(const cv::Mat& colorPicture, cv::Mat& mark
	, std::vector<Cell>& tempCellSet, const Seed& seed, const std::vector<cv::Point>& polygonPointSet);
#endif
//	Search for possible pixel patches to be determined
void formTempCellSet(const cv::Mat& colorPicture, cv::Mat& mark
	, std::vector<Cell>& tempCellSet, std::vector<Cell>& cellSet
	, int search_end_test, const Seed& seed
#ifdef WRITE_EXCEL
	, xlnt::worksheet& dataSheet, xlnt::workbook& dataBook
#endif
	, const std::vector<cv::Point>& polygonPointSet);
//Given a point to determine the TestCellPosition in the four neighbourhoods of the change point
void formTestCellPosition(TestCellPosition& testCellPosition, const  cv::Mat& mark, const cv::Point& pt);
//Get the TestCellPosition of the first position to be tested
void getFirstTestCellPosition(TestCellPosition& firstTCP, const cv::Mat& mark, const Seed& seed,cv::Mat& TestCellPosition_Mark);
//Go to the next pixel plaque point to be tested TestCellPosition
void getNextTestCellPosition(TestCellPosition& nextTCP, TestCellPosition& lastTCP
	,  cv::Mat& mark, const Seed& seed/*, cv::Mat& TestCellPositionMark*/,const std::vector<cv::Point>&polygonPointSet);
//Get Directions
void getTestCellPositionUp(TestCellPosition& nextTCP, TestCellPosition& lastTCP
	,  cv::Mat& mark, const Seed& seed/* cv::Mat& TestCellPositionMark*/, const std::vector<cv::Point>& polygonPointSet);
//Generate TestCellPosition
void formTestCellPosition(TestCellPosition& nextTCP, const cv::Mat& mark,
	const cv::Point& center, int direction);
//lastTCP direction is pointing left
void getTestCellPositionLEFT(TestCellPosition& nextTCP,
	TestCellPosition& lastTCP,  cv::Mat& mark, const Seed& seed, const std::vector<cv::Point>& polygonPointSet);
//The lastTCP direction is pointing down
void getTestCellPositionDOWN(TestCellPosition& nextTCP,
	TestCellPosition& lastTCP,  cv::Mat& mark, const Seed& seed, const std::vector<cv::Point>& polygonPointSet);
//lastTCP direction is to the right
void getTestCellPositionRIGHT(TestCellPosition& nextTCP,
	TestCellPosition& lastTCP,  cv::Mat& mark, const Seed& seed,const std::vector<cv::Point>&polygonPointSet);
//Set this sand boundary to zero
void setMarkZero(cv::Mat& mark);
//Press the segmented sand image into the sandSet
void formSandPicture(cv::Mat& sand, const std::vector<Cell>& cellSet,const cv::Mat&mark,cv::Mat& colorSandGrain);
//Copy the cells in the vector one by one into the image sand
void copyCellToPicture(cv::Mat& sand,const  Cell& cell, int minRows, int minCols,cv::Mat& sandColorPicture,cv::Vec3b& color);
//Make similarity assignments
void setSimilar(Similar& sandSimilar, const std::vector<Cell>& cellSet);
//Get the pixel patches in the seed point field
void getNeighbor(const cv::Mat& colorPicture, std::vector<Cell>& cellSet
	, const Seed& seed, int outline_number,SeedSet&Sset,const int markNumber);
//Get access tokens
int getVisitMarkValue(const cv::Point& center, const int direction);
//Initialize TestCellDirection_Mark.
void InitializationTestCellDirection_Mark(cv::Mat& testCellDirection_Mark);
//Zeroing out TestCellDirection_Mark
void setTestCellDirection_MarkZero(cv::Mat& TestCellDirection_Mark);
//Three directional TestCellDirection_Mark values
int  getTestCellDirection_MarkValue(cv::Point & center, int direction);
//Display the border in the picture
void drawMarkAtColorPicture(cv::Mat& colorPicture, const cv::Mat& mark);
//Determining whether the general direction of up left down and right is correct
bool isRightOrient(const cv::Point& position, const cv::Point& seedPosition);
//Adjustment of untrue boundary values in mark, i.e. the true boundary must be immediately adjacent to one edge of the sand grain, otherwise it is a false boundary and should be eliminated
std::vector<AdjustPosition> adjustBoundaryMark(const cv::Mat& colorPicture, cv::Mat& mark, const int sandMarkNumber,std::vector<Cell>&cellSet,const cv::Rect& minRect);
//Write information like the spot to the bookSheet
void writeInformationToBookdata(const cv::Mat& mark, xlnt::workbook& dataBook, xlnt::worksheet& dataSheet
	, const std::vector<Cell>& tempCellSet, const cv::Mat& TestCellDirection_Mark);
//Pressing real sand patches into the cellSet
void pushbackCellSet(std::vector<Cell>& cellSet, const std::vector<Cell>& tempCellSet, const cv::Mat& mark, const int sandMarkNumber);
//The adjusted and changed boundary marks and the corresponding internal sand patches are reflected in the worksheet
void  adjuestMarkAndCellSet(const std::vector<AdjustPosition>& AdSet, xlnt::workbook& dataBook
	, xlnt::worksheet& dataSheet, cv::Mat& mark, std::vector<Cell>& cellSet);
//Find a matching iterator for later deletion
 std::vector<Cell>::iterator fromCellSetfindIterator( std::vector<Cell> & cellSet, const cv::Point & pt);
 //Are the elements in both vector<Cell>s the same
 bool isContinueTempCellSet(const std::vector<Cell>& preSet, const std::vector<Cell>& tempCellSet);
 //Determine the average equivalent radius of the existing search boundary
  int getSandRadius(const std::vector<Cell> & cellSet, const cv::Mat& mark);
  //Get individual sand grain radius values
  int getSingleSandRadius(const std::vector<Cell>& cellSet, const cv::Mat& mark, int& count);
  //Obtain sand diameter values
  bool getSandRadiusValue(cv::Point& pt, const cv::Mat& mark, int& radius, int& count);
  //Get the eight control points for this sand grain
  void getPolygonPoint(cv::Point& seedP, const cv::Mat& outline, std::vector<cv::Point>& polygonPointSet);
  //Determine whether the given point lies below the diagonal diagonal
  bool isLocateLineDown(const cv::Point& firsP, const cv::Point& secondP, const cv::Point& center);
  //Expand the number of control vertices in the polygonPointSet array
  void expansionPolygonPointSet(int expansionSeedNumber, const cv::Mat& oldOutline, std::vector<cv::Point>& polygonPointSet, std::vector<cv::Point>&newPositionSeed);
  //Remove untrustworthy control points, the four points furthest from the position are untrustworthy
  void deleteUntrustPoint(const cv::Point& position, std::vector<cv::Point>& singleSet);
  //Compare lh with rh for distance from position
  bool compareDistance(const cv::Point& position, const cv::Point& lh, const cv::Point& rh);
  //Removal of unrealistic boundaries within mark
  void cullUnrealContour(cv::Mat& mark);
  //Determine if the image is a true contour point at that point
  bool is_realOutline(const cv::Mat& mark, const cv::Point& p, cv::Range& X_range, cv::Range& Y_range);
  //Generation of sand grains from sand contours
  void fromMarkToSand(const cv::Mat& mark, cv::Mat& colorSandGrain, cv::Mat& bgr_sand);
  //Generation of random colours;
  cv::Vec3b randomColor();
  //Formation of a sand grain image
  cv::Mat formSandPixel(const std::vector<SandPixel>& spSet);
  //Finding the range of variation in sand grains
  void findMinMaxRange(const cv::Mat& mark, cv::Range& X_range, cv::Range& Y_range);
  //Find all the contour points of a grain of sand
  void findOutlineSet(const cv::Mat& mark, std::vector<cv::Point>& pSet);
  //Is the specified point in the sand
  bool is_inSand(const cv::Mat& mark, const cv::Point& p, const cv::Range& X_range, const cv::Range& Y_range);
  //Is the specified point in the sand
  bool is_inSand(const cv::Mat& sand, const cv::Point& p,const cv::Vec3b& color);
  //Find the next position within the control polygon
 cv::Point findNextPosition(const TestCellPosition & lastTCP, const std::vector<cv::Point> & polygonPointSet, const cv::Point& center,const cv::Point&seed_location);
 //Search range check to see if the polygon enclosed by the 8 control points is exceeded
 void adjustNewPosition(cv::Point& center,cv::Mat& mark, const std::vector<cv::Point>& polygonPointSet,
	 TestCellPosition& lastTCP, const int serial, const cv::Point& seed);
 //Get access tokens
 int getVisitMarkValue_8(const cv::Point& center, const int i);
 //Perform four-neighbourhood image speckle tagging
 void markSeedSetMark(SeedSet& sSet, const std::vector<Cell>cellSet, int markNumber, cv::Mat& mark);
 //Draw the extent of the rotating rectangle
 cv::Mat drawRotateOutline(const cv::Mat& oldOutline, const cv::RotatedRect& sandRect, const std::vector<cv::Point>& con_hull);
 //Show new points
 void  drawNewPositionSeed(cv::Mat& polygonOutline, const std::vector<cv::Point>& newPositionSeed);
 void drawNewPointSetOnPolygonOutline(cv::Mat& polygonOutline, std::vector<cv::Point>& newPolgonPointSet);
 //Removing points of no confidence
 void  deleteNewUntrustPoint(cv::Point& seed_location, std::vector<cv::Point>& polygonPointSet, int deleteNumber, const cv::Size& pictureSize);
 //Removing points of no confidence
 void  deleteNewUntrustPoint(cv::Point& seed_location, std::vector<cv::Point>& polygonPointSet, int deleteNumber);
 //Get the four control points of the matching tilted oval adaptation rectangle
 std::vector<cv::Point> getFillEllipseRotatedRect(const std::vector<cv::Point>& singleSet, int deleteNumber, const cv::Size& pictureSize);
 //Get the sixteen control points of this sand grain
 void getPolygonSixTeenPoint(cv::Point& seedP, const cv::Mat& outline, std::vector<cv::Point>& polygonPointSet);
 //Get boundary pixel patches
 void getOutline(const cv::Mat& colorPicture, const cv::Rect& minRect, std::vector<Cell>& outlineSet);
 //Converting data from cell to Mat
 void fromCellToMat(const std::vector<Cell>& cellSet, const std::vector<Cell>& outlineSet, cv::Mat& trainingData, cv::Mat& responses);
 //Conversion of image patches to Mat data format
 cv::Mat cellTransactionMat(const Cell& cell);
 //Determine if the sand grain is at the edge of the picture
 bool is_MarginSand(const cv::Point& seed, const cv::Rect& minRect, const cv::Mat& newPicture);
 //Deleting seed points from near-neighbour boundaries
 void deleteUntrustPointInZeroSet(const cv::Mat& oldOutline, std::vector<cv::Point>& zeroSet);
 //Remove seed points immediately adjacent to the boundary in the association container
 void deleteUntrustInMultimap(std::multimap<int, std::pair<int, cv::Point>>& seedSPair, double DELETE_IN_MULTIMAP);
 //Re-determine if the seed point has already been processed
 bool isDispose(cv::Mat& sSet_mark, std::vector<Cell>& cellSet);
 //Do a visit_mark to see if it exceeds the image border and set it to VISITED_MARK if it does
 void determinePictureFrame(const cv::Mat& mark, const cv::Point& pt, const int direction,int &visit_mark);
 //Showing the range of influence of sand particles in the mark matrix
 void drawMarkPolygon(cv::Mat& mark, const std::vector<cv::Point>& con_hull);
 //Show mark matrix
 void displaySeedMark(const cv::Mat& mark);
 //Write non-zero data in the mark matrix to the sheet form
 void writeMark(xlnt::worksheet& markSheet, const cv::Point& p, int value);
 //Make orientation adjustments for one direction for borders, one direction for sand grains, one direction for empty, and one visited direction
 void adjustDirection(const int visit_mark0, const int visit_mark1, const int visit_mark2, const int visit_mark3,
	 const int direction_0, const int direction_1, const int direction_2, const int direction_3,
	 const int& outline, const int& empty, const int& sand,
	 cv::Point& direction, int& serial);
 //break if all four directions of lastTCP.center.position have been visited
 bool is_visitedFourDirection(const cv::Mat& TestCellPosition_Mark, const cv::Point& position);
 //Determining support vector machine one classification positive class training data generation
 void  formSVMOneClass(const std::vector<Cell>& cellSet, cv::Mat& trainingData, cv::Mat&responses);
 //Copy the elements in cell to Mat and add variance values
 cv::Mat fromCellToMat(const Cell& it);
 //Determining whether an image patch is a patch in a sand grain
 bool OneClassSVMPredict(const cv::Mat& Outline, const cv::Ptr<cv::ml::SVM>& svm, const Cell& tempCell,std::ofstream & thresholdSVM);
 //Determining if there is a boundary overflow
 inline void isOverflowBoundary(const cv::Mat& mark, const cv::Point& pt);
 //Determining support vector machine one classification positive class training data generation
 void formSVMClassFour(const std::vector<Cell>& cellSet, cv::Mat& trainingData, cv::Mat& responses);
 //Copy the elements in cell to Mat and add variance values
 cv::Mat fromCellToMatFour(const Cell& it);
 //Determining whether an image patch is a patch in a sand grain
 bool OneClassSVMPredictFour(const cv::Ptr<cv::ml::SVM>& svm, const Cell& tempCell);
 //Generate random location points
 inline void RandPoint(cv::RNG& seedRng, cv::RotatedRect& rotatedR, const std::vector<cv::Point>& rotatedRectFourPoint, cv::Point& position, const cv::Mat& oldOutline);
 //Re-determine if the seed point has already been processed
 bool isDispose(cv::Mat& sSet_mark, std::vector<cv::Point>& cellSet);