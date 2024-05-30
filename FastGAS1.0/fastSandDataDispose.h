#pragma once
#include <iostream>
#include<string>
#include<vector>
#include<map>
#include<tuple>
#include<fstream>
#include<sstream>
#include<set>
#define SAND_DATA_NUMBER 4
#define PI 3.1415926
#define DIVIDE_NUMBER  100
//#define FEATURE_GRAIN
//#define  TEST_OUTPUT_FEATURE_GRAIN
//#ifdef FEATURE_GRAIN
extern int gre;
extern int can1, can2;
extern double featureD;
extern std::string sandArea_address, featureGrainAddress;
//#endif


struct SandAccumulateCurveDivide
{
	double x_i;
	double y_i;
};
//Handling sand data
void seiveingSandData();
void readGASAndSievingData(const std::string& inputGAS, std::multiset<double>& GASFirstDiameter
	, std::multiset<double>& GASSecondCumulationMass);
void sandDataD();
//Read the corresponding data from the specified path
void readTextData(const std::string& inputAllSandD, const std::string& inputWrongSandD,
	std::map<int, std::tuple<double, double, double>>& allData, std::vector<int>& wrongeData);
//Reading of sand grain serial numbers
void readSandNumber(const std::string& intermediateData, std::string& sandNumber);
//Write the average particle size data to the specified txt file
void writeTextDiameter(const std::string& address, std::multiset<double>& MaxDMassA, const std::multiset<double>& rightV);
void writeTextDiameter(const std::string& GASMinAddress, const std::multiset<double>& allGASMassA);
//Obtain maximum, minimum particle size
void getMaxMinDiameter(const std::multiset<double>& allD, const std::multiset<double>& rightD, double& maxTwoD, double& minTwoD);
//Reading of manual particle size data
void readManualMeasureDiameter(const std::string& inputManualMaxMinA, std::multiset<double>& minManualD,
	std::multiset<double>& maxManualD, std::multiset<double>& averageManualD);
//Calculation of the total coordinates of the accumulation curve and the sand mass values for each stage
double calculateTwoCurveCorrelation(std::multiset<double>firstHorizontalCurve, std::multiset<double> firstCurveVertical,
	std::multiset<double>secondHorizontalCurve, std::multiset<double>secondCurveVertical
	, const std::string& workAddress);
//Consolidated data
void mergeData(const std::multiset<double>& horizontalCurve, const std::multiset<double>& AccumulateCurve,
	std::map<double, double>& CurveM);
//Determine the interval where the interpolation location points are located
void findIntervalInCurveM(const std::map<double, double>& CurveM, std::pair<double, double>& first,
	std::pair<double, double>& second, double position);
//Calculation of average particle size
void calculateAverageDiameter(const std::map<double, double>& firstCurveM, const std::map<double, double>& secondCurveM,
	double& average_x, double& average_y, const double& maxTwoD, const double& minTwoD);
//Read the corresponding data from the specified path
void readTextData(const std::string& inputAllSandD,
	std::map<int, std::tuple<double, double, double>>& allData);
//Combining manual measurement data
void mergeManualData(std::map<int, std::tuple<double, double, double>>& rightAllData, const std::multiset<double>& maxManualD, const std::multiset<double>& minManualD
	, const std::multiset<double>& averageManualD);
//Calculation of the total coordinates of the accumulation curve and the sand mass values for each stage
void calculateAccumulateCurveHorizon(const std::multiset<double>& MaxD, std::multiset<double>& MaxDMassA, std::multiset<double>& MaxDMass,
	const double totalValue);
void calculateAccumulateCurveHorizon(std::multiset<double>& allGASMassA, const std::multiset<double>& minGASD, double GASTolalArea);
//Calculation error rate
double calculateWrongRate(std::multiset<double>& firstHorizontalCurve, std::multiset<double>& firstCurveVertical,
	std::multiset<double>& secondHorizontalCurve, std::multiset<double>& secondCurveVertical);
//Identify inside that zone
void  findIntervalInCurve(const std::map<double, double>& allData, const double position, std::pair<double, double>& bg, std::pair<double, double>& ed);
//Output error radius values
void writeWrongText(const std::string& Address, const std::multiset<double>& wrongData);
//Output all error data
void writeWtongText(const std::string& address, const std::map<int, std::tuple<double, double, double>>& wrongAllData);
void getRelationDiameterBetweenTwoCurves(const std::map<double, double>& firstCurveM
	, const std::map<double, double>& secondCurveM, std::vector<double>& firstDiameterVector,
	std::vector<double>& secondDiameterVector);
double getRelationSecondCurveDiameter(double firstCumulationValue,
	const std::map<double, double>& secondCurveM);
void calculateAverageDiameter(double& average_x, double& average_y, const std::vector<double>& firstDiameterVector,
	const std::vector<double>& secondDiameterVector);
void outputFeatureGrain(std::ofstream& feature);
void readTextData(const std::string& inputAllSandD,
	std::map<int, std::tuple<double, double, double>>& allData);
void writeTextDiameter(std::ofstream& feature, std::multiset<double>& MaxDMassA, const std::multiset<double>& rightV);

void  writeDiameter(std::map<double, double>& AccumulateDiameter, const std::multiset<double>& allAverageDMassA,
	const std::multiset<double>& allAverageD);
void getFeatureDiamter(std::ofstream& feature, double featureGrain, const std::map<double, double>& AccumulateDiameter);
void sandMinDiameterData();
void readManualMeasureDiameter(const std::string& inputGASSandA, std::multiset<double>& minGASD);
//输出累计曲线数据
void fastSandMinDiameterData(std::vector<double> sandMinDiameter,std::string pastWorkFolder, std::multimap<double, double> diameterAndArea);
//计算两条曲线的均方根误差
void calculateRMSE();
double calculateTwoCurveRMSE(std::multiset<double>firstHorizontalCurve, std::multiset<double> firstCurveVertical,
	std::multiset<double>secondHorizontalCurve, std::multiset<double>secondCurveVertical);
