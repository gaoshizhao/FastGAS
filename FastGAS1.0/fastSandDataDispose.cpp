#pragma once
#include <limits>

#include"fastSandDataDispose.h"



//#define FIELD_DATA
void seiveingSandData()
{
	//input data
	std::cout << "Please enter the path to the GAS file!" << std::endl;
	std::string inputGAS;
	std::cin >> inputGAS;
	std::cout << "Please enter the path to seiving method file!" << std::endl;
	std::string inputSieving;
	std::cin >> inputSieving;
	std::multiset<double> GASFirstDiameter, GASSecondCumulationMass, SievingFirstDiameter
		, SievingSecondCumulationMass;
	readGASAndSievingData(inputGAS, GASFirstDiameter, GASSecondCumulationMass);
	readGASAndSievingData(inputSieving, SievingFirstDiameter, SievingSecondCumulationMass);
	//end input data
	//caculate relation coefficient
	auto pos = inputGAS.find_last_of("//\\");
	std::string workAddress = inputGAS.substr(0, pos);
	std::string allGASAddress = workAddress + "\\GASAndSieving.txt";
	double  rou_GASAndSieving = calculateTwoCurveCorrelation(GASFirstDiameter, GASSecondCumulationMass,
		SievingFirstDiameter, SievingSecondCumulationMass, workAddress);
	std::ofstream rou(allGASAddress);
	rou << "Correlation coefficient for GAS and Sieving =" << rou_GASAndSieving << std::endl;
	double rou_wrongRate_allMax = calculateWrongRate(GASFirstDiameter, GASSecondCumulationMass,
		SievingFirstDiameter, SievingSecondCumulationMass);
	rou << "Relative error in  for GAS and Sieving =" << rou_wrongRate_allMax << std::endl;
};

void outputFeatureGrain(std::ofstream& feature)
{
#ifdef  TEST_OUTPUT_FEATURE_GRAIN
	sandArea_address = "D:\\工作\\科研\\基于GAS算法的砾石自动识别应用研究\\计算结果\\多参数分析\\newOutPicture\\sandArea\\sandArea.txt";
#endif
	std::string inputAllSandA;
	inputAllSandA = sandArea_address;
	std::map<int, std::tuple<double, double, double>> allData;
	readTextData(inputAllSandA, allData);
	std::multiset<double>allMaxD, allMinD, allAverageD;
	std::multiset<double>allMaxDMassA, allMinDMassA, allAverageDMassA;
	std::multiset<double>allMaxDMass, allMinDMass, allAverageDMass;
	double allMaxV = 0;
	double allMinV = 0;
	double allAverageV = 0;
	for (auto it = allData.begin(); it != allData.end(); it++)
	{
		double valueMax = std::get<0>(it->second) * 10;
		double valueMin = std::get<1>(it->second) * 10;
		double valueAverage = std::get<2>(it->second) * 10;
		allAverageV += valueAverage * valueAverage * PI / 4;
		allMaxV += valueMax * valueMax * PI / 4;
		allMinV += valueMin * valueMin * PI / 4;
		allMaxD.insert(valueMax);
		allMinD.insert(valueMin);
		allAverageD.insert(valueAverage);
	}
	//calculateAccumulateCurveHorizon(allMaxD, allMaxDMassA, allMaxDMass, allMaxV);
	calculateAccumulateCurveHorizon(allMinD, allMinDMassA, allMinDMass, allMinV);
	//calculateAccumulateCurveHorizon(allAverageD, allAverageDMassA, allAverageDMass, allAverageV);
	std::string workAddress, allAverageAddress;
	auto pos = inputAllSandA.find_last_of("//\\");
	workAddress = inputAllSandA.substr(0, pos);
	allAverageAddress = workAddress + "\\allAverage.txt";
	std::map<double, double> AccumulateDiameter;
	writeDiameter(AccumulateDiameter, allMinDMassA, allMinDMass);
	getFeatureDiamter(feature, featureD, AccumulateDiameter);
};
void sandMinDiameterData()
{
	//read all data to the ".txt"file.
	std::cout << "Please enter the path to the GAS diameter file!" << std::endl;
	std::string inputGASSandA, inputImageJMinA;
	std::cin >> inputGASSandA;
	std::cout << "Please enter the file path for the  minimum grain size of the manually measured sand grains!" << std::endl;
	std::cin >> inputImageJMinA;
	std::multiset<double>minGASD, minImageJD;
	readManualMeasureDiameter(inputGASSandA, minGASD);
	readManualMeasureDiameter(inputImageJMinA, minImageJD);
	//calculate the total area
	double GASTolalArea = 0, ImageJTotalArea = 0;
	for (auto it = minGASD.begin(); it != minGASD.end(); it++)
	{
		GASTolalArea += (*it) * (*it) * PI / 4;
	}
	for (auto it = minImageJD.begin(); it != minImageJD.end(); it++)
	{
		ImageJTotalArea += (*it) * (*it) * PI / 4;
	}
	// calculate the cumulation curve
	std::multiset<double>allGASMassA, allImageJMassA;
	calculateAccumulateCurveHorizon(allGASMassA, minGASD, GASTolalArea);
	calculateAccumulateCurveHorizon(allImageJMassA, minImageJD, ImageJTotalArea);
	//output address
	std::string GASMinAddress, ImageJMinAddress, workAddress;
	auto pos = inputGASSandA.find_last_of("//\\");
	auto pos_Last = inputGASSandA.find_last_of("\.");
	std::string GASorAIForBaseGrain = inputGASSandA.substr(pos, pos_Last);
	workAddress = inputGASSandA.substr(0, pos);
	GASMinAddress = workAddress + "\\" + GASorAIForBaseGrain + "Cumulation.txt";
	ImageJMinAddress = workAddress + "\\ImageJCumulation.txt";
	writeTextDiameter(GASMinAddress, allGASMassA, minGASD);
	writeTextDiameter(ImageJMinAddress, allImageJMassA, minImageJD);
	//calculate the relation coefficient and the relation error
	std::string rouAddress = workAddress + "\\relation_coefficient_and error.txt";
	std::ofstream rou(rouAddress);
	double  rou_GASD =
		calculateTwoCurveCorrelation(minGASD, allGASMassA, minImageJD, allImageJMassA, workAddress);
	rou << "Correlation coefficient for the" + GASorAIForBaseGrain + " particle size of sand = " << rou_GASD << std::endl;
	double rou_wrongRate_GAS = calculateWrongRate(minGASD, allGASMassA, minImageJD, allImageJMassA);
	rou << "Relative error in the minimum particle size of sand =" << rou_wrongRate_GAS << std::endl;
};

//输出累计曲线数据
void fastSandMinDiameterData(std::vector<double> sandMinDiameter, std::string pastWorkFolder,std::multimap<double, double> diameterAndArea)
{
	//****************************************************************************test
	std::multiset<double>minFastGASD;
	for (auto it = sandMinDiameter.begin(); it != sandMinDiameter.end(); it++)
	{
		minFastGASD.insert(*it);
	}
	double FastGASTolalArea = 0;
	for (auto it = minFastGASD.begin(); it != minFastGASD.end(); it++)
	{
		FastGASTolalArea += (*it) * (*it) * PI / 4;
	}
	std::multiset<double>allFastGASMassA;
	calculateAccumulateCurveHorizon(allFastGASMassA, minFastGASD, FastGASTolalArea);
	std::string accumulationCurveAddress = pastWorkFolder + "\\FastGASD_AccumulateCurve.txt";
	writeTextDiameter(accumulationCurveAddress, allFastGASMassA, minFastGASD);
	//********************************************************************************end
	std::string accumulationCurveAddressArea = pastWorkFolder + "\\FastGASD_AccumulateCurveArea.txt";
	std::ofstream allSandD(accumulationCurveAddressArea);
	double sum = 0;
	for (auto it = diameterAndArea.begin(); it != diameterAndArea.end(); it++)
	{
		sum += it->second;
	}
	double sum2 = 0;
	for (auto it = diameterAndArea.begin(); it != diameterAndArea.end(); it++)
	{
		sum2 += it->second;
		allSandD << (*it).first << "    " << sum2/sum << std::endl;
	}
};
//计算两条曲线的均方根误差
void calculateRMSE()
{
	std::cout << "Please enter the path to the FastGAS diameter file!" << std::endl;
	std::string inputGASSandA, inputImageJMinA;
	std::cin >> inputGASSandA;
	std::cout << "Please enter the file path for the  minimum grain size of the manually measured sand grains!" << std::endl;
	std::cin >> inputImageJMinA;
	std::multiset<double>minGASD, minImageJD;
	readManualMeasureDiameter(inputGASSandA, minGASD);
	readManualMeasureDiameter(inputImageJMinA, minImageJD);
	//calculate the total area
	double GASTolalArea = 0, ImageJTotalArea = 0;
	for (auto it = minGASD.begin(); it != minGASD.end(); it++)
	{
		GASTolalArea += (*it) * (*it) * PI / 4;
	}
	for (auto it = minImageJD.begin(); it != minImageJD.end(); it++)
	{
		ImageJTotalArea += (*it) * (*it) * PI / 4;
	}
	// calculate the cumulation curve
	std::multiset<double>allGASMassA, allImageJMassA;
	calculateAccumulateCurveHorizon(allGASMassA, minGASD, GASTolalArea);
	calculateAccumulateCurveHorizon(allImageJMassA, minImageJD, ImageJTotalArea);
	//output address
	std::string GASMinAddress, ImageJMinAddress, workAddress;
	auto pos = inputGASSandA.find_last_of("//\\");
	auto pos_Last = inputGASSandA.find_last_of("\.");
	std::string GASorAIForBaseGrain = inputGASSandA.substr(pos, pos_Last);
	workAddress = inputGASSandA.substr(0, pos);
	GASMinAddress = workAddress + "\\" + GASorAIForBaseGrain + "Cumulation.txt";
	ImageJMinAddress = workAddress + "\\ImageJCumulation.txt";
	writeTextDiameter(GASMinAddress, allGASMassA, minGASD);
	writeTextDiameter(ImageJMinAddress, allImageJMassA, minImageJD);
	//calculate the relation coefficient and the relation error
	std::string rsmeAddress = workAddress + "\\RMSE.txt";
	std::ofstream rsmeOfStream(rsmeAddress);
	double twoCurveRmse=calculateTwoCurveRMSE(minGASD, allGASMassA, minImageJD, allImageJMassA);
	rsmeOfStream << "Root mean square error of two curves=" << twoCurveRmse;

};
double calculateTwoCurveRMSE(std::multiset<double>firstHorizontalCurve, std::multiset<double> firstCurveVertical,
	std::multiset<double>secondHorizontalCurve, std::multiset<double>secondCurveVertical)
{
	std::map<double, double>firstCurveM;
	std::map<double, double>secondCurveM;
	mergeData(firstCurveVertical, firstHorizontalCurve, firstCurveM);
	mergeData(secondCurveVertical, secondHorizontalCurve, secondCurveM);
	double average_x = 0;
	double average_y = 0;
	double begin_p = 0;
	double cov = 0;
	double var_x = 0;
	double var_y = 0;
	int firstSize = firstCurveM.size();
	double interval = firstSize;
	std::vector<double> curve1, curve2;
	getRelationDiameterBetweenTwoCurves(firstCurveM, secondCurveM, curve1, curve2);
    
	double sumOfSquaredErrors = 0.0;
	int numPoints = curve1.size();

	// 遍历每个数据点  
	for (int i = 0; i < numPoints; ++i) {
		double diff = curve1[i] - curve2[i]; // 计算差值  
		sumOfSquaredErrors += diff * diff; // 累加差值的平方  
	}

	// 计算均方误差MSE  
	double mse = sumOfSquaredErrors / numPoints;

	// 计算均方根误差RMSE  
	double rmse = std::sqrt(mse);

	return rmse;






};


void sandDataD()
{
	while (true)
	{
#ifndef FIELD_DATA
		std::cout << "Please enter the path to the sand diameter file!" << std::endl;
		std::string inputAllSandA, inputManualMaxMinA;
		std::cin >> inputAllSandA;
		std::cout << "Please enter the file path for the maximum and minimum grain size of the manually measured sand grains!" << std::endl;
		std::cin >> inputManualMaxMinA;
		std::multiset<double> minManualD, maxManualD, averageManualD;
		std::map<int, std::tuple<double, double, double>> allData, rightAllData;
		readManualMeasureDiameter(inputManualMaxMinA, minManualD, maxManualD, averageManualD);
		mergeManualData(rightAllData, maxManualD, minManualD, averageManualD);
		readTextData(inputAllSandA, allData);

#endif
#ifdef FIELD_DATA

		std::cout << "Please enter the path to the sand diameter file!" << std::endl;
		std::string inputAllSandA, inputWrongSandA, inputManualMaxMinA;

		std::cin >> inputAllSandA;
		std::cout << "Please enter the wrong sand diameter file path!" << std::endl;
		std::cin >> inputWrongSandA;


		std::map<int, std::tuple<double, double, double>> allData, rightAllData, wrongAllData;
		std::vector<int>wrongData;
		std::multiset<double>wrongMaxD, wrongMinD, wrongAverageD;
		readTextData(inputAllSandA, inputWrongSandA, allData, wrongData);
		rightAllData = allData;
		for (int i = 0; i < wrongData.size(); i++)
		{
			wrongAllData.insert(std::pair<int, std::tuple<double, double, double>>(wrongData[i], rightAllData[wrongData[i]]));
			rightAllData.erase(wrongData[i]);
		}
		for (auto it = wrongAllData.begin(); it != wrongAllData.end(); it++)
		{
			double valueMax = std::get<0>(it->second) * 10;
			double valueMin = std::get<1>(it->second) * 10;
			double valueAverage = std::get<2>(it->second) * 10;
			wrongMaxD.insert(valueMax);
			wrongMinD.insert(valueMin);
			wrongAverageD.insert(valueAverage);
		}
#endif
		std::multiset<double>allMaxD, allMinD, allAverageD, rightMaxD, rightMinD, rightAverageD;
		std::multiset<double>allMaxDMassA, allMinDMassA, allAverageDMassA, rightMaxDMassA, rightMinDMassA, rightAverageDMassA;
		std::multiset<double>allMaxDMass, allMinDMass, allAverageDMass, rightMaxDMass, rightMinDMass, rightAverageDMass;
		double allMaxV = 0;
		double allMinV = 0;
		double allAverageV = 0;
		double rightMaxV = 0;
		double rightMinV = 0;
		double rightAverageV = 0;
#ifdef FIELD_DATA
		for (auto it = allData.begin(); it != allData.end(); it++)
		{
			double valueMax = std::get<0>(it->second) * 10;
			double valueMin = std::get<1>(it->second) * 10;
			double valueAverage = std::get<2>(it->second) * 10;
			allAverageV += valueAverage * valueAverage * PI / 4;
			allMaxV += valueMax * valueMax * PI / 4;
			allMinV += valueMin * valueMin * PI / 4;
			allMaxD.insert(valueMax);
			allMinD.insert(valueMin);
			allAverageD.insert(valueAverage);
		}
		for (auto it = rightAllData.begin(); it != rightAllData.end(); it++)
		{
			double valueMax = std::get<0>(it->second) * 10;
			double valueMin = std::get<1>(it->second) * 10;
			double valueAverage = std::get<2>(it->second) * 10;
			rightAverageV += valueAverage * valueAverage * PI / 4;
			rightMaxV += valueMax * valueMax * PI / 4;
			rightMinV += valueMin * valueMin * PI / 4;
			rightMaxD.insert(valueMax);
			rightMinD.insert(valueMin);
			rightAverageD.insert(valueAverage);
		}
#endif
#ifndef FIELD_DATA
		for (auto it = allData.begin(); it != allData.end(); it++)
		{
			double valueMax = std::get<0>(it->second) * 10;
			double valueMin = std::get<1>(it->second) * 10;
			double valueAverage = std::get<2>(it->second) * 10;
			allAverageV += valueAverage * valueAverage * PI / 4;
			allMaxV += valueMax * valueMax * PI / 4;
			allMinV += valueMin * valueMin * PI / 4;
			allMaxD.insert(valueMax);
			allMinD.insert(valueMin);
			allAverageD.insert(valueAverage);
		}
		for (auto it = rightAllData.begin(); it != rightAllData.end(); it++)
		{
			double valueMax = std::get<0>(it->second);
			double valueMin = std::get<1>(it->second);
			double valueAverage = std::get<2>(it->second);
			rightAverageV += valueAverage * valueAverage * PI / 4;
			rightMaxV += valueMax * valueMax * PI / 4;
			rightMinV += valueMin * valueMin * PI / 4;
			rightMaxD.insert(valueMax);
			rightMinD.insert(valueMin);
			rightAverageD.insert(valueAverage);
		}
#endif
		calculateAccumulateCurveHorizon(allMaxD, allMaxDMassA, allMaxDMass, allMaxV);
		calculateAccumulateCurveHorizon(allMinD, allMinDMassA, allMinDMass, allMinV);
		calculateAccumulateCurveHorizon(allAverageD, allAverageDMassA, allAverageDMass, allAverageV);
		calculateAccumulateCurveHorizon(rightMaxD, rightMaxDMassA, rightMaxDMass, rightMaxV);
		calculateAccumulateCurveHorizon(rightMinD, rightMinDMassA, rightMinDMass, rightMinV);
		calculateAccumulateCurveHorizon(rightAverageD, rightAverageDMassA, rightAverageDMass, rightAverageV);
		std::string workAddress, allMaxAddress, allMinAddress, allAverageAddress, rightMaxAddress
			, rightMinAddress, rightAverageAddress;
		auto pos = inputAllSandA.find_last_of("//\\");
		workAddress = inputAllSandA.substr(0, pos);
		allMaxAddress = workAddress + "\\allMax.txt";
		allMinAddress = workAddress + "\\allMin.txt";
		allAverageAddress = workAddress + "\\allAverage.txt";
		rightMaxAddress = workAddress + "\\rightMax.txt";
		rightMinAddress = workAddress + "\\rightMin.txt";
		rightAverageAddress = workAddress + "\\rightAverage.txt";
#ifdef FIELD_DATA
		std::string wrongAllAddress, wrongMaxAddress, wrongMinAddress, wrongAverageAddress;
		wrongAllAddress += workAddress + "\\wrongAll.txt";
		wrongMaxAddress += workAddress + "\\wrongMax.txt";
		wrongMinAddress += workAddress + "\\wrongMin.txt";
		wrongAverageAddress += workAddress + "\\wrongAverage.txt";
		writeWrongText(wrongMaxAddress, wrongMaxD);
		writeWrongText(wrongMinAddress, wrongMinD);
		writeWrongText(wrongAverageAddress, wrongAverageD);
		void writeWtongText(const std::string & address, const std::map<int, std::tuple<double, double, double>> &wrongAllData);
#endif

		writeTextDiameter(allMaxAddress, allMaxDMassA, allMaxD);
		writeTextDiameter(allMinAddress, allMinDMassA, allMinD);
		writeTextDiameter(allAverageAddress, allAverageDMassA, allAverageD);
		writeTextDiameter(rightMaxAddress, rightMaxDMassA, rightMaxD);
		writeTextDiameter(rightMinAddress, rightMinDMassA, rightMinD);
		writeTextDiameter(rightAverageAddress, rightAverageDMassA, rightAverageD);
		std::string rouAddress = workAddress + "\\rou.txt";
		std::ofstream rou(rouAddress);
		double  rou_allMaxD = calculateTwoCurveCorrelation(allMaxD, allMaxDMassA,
			rightMaxD, rightMaxDMassA, workAddress);
		rou << "Correlation coefficient for the maximum particle size of sand =" << rou_allMaxD << std::endl;
		double rou_allMinD = calculateTwoCurveCorrelation(allMinD, allMinDMassA,
			rightMinD, rightMinDMassA, workAddress);
		rou << "Correlation coefficient for the smallest particle size of sand =" << rou_allMinD << std::endl;
		double rou_allAverageD = calculateTwoCurveCorrelation(allAverageD, allAverageDMassA,
			rightAverageD, rightAverageDMassA, workAddress);
		rou << "Correlation coefficient for the average particle size of sand =" << rou_allAverageD << std::endl;
		rou << "Correct number of particles =" << rightAllData.size() << std::endl;
		rou << "Number of full particles =" << allData.size() << std::endl;
#ifdef FIELD_DATA
		rou << "Number of error particles =" << wrongData.size() << std::endl;
#endif
		double rou_wrongRate_allMax = calculateWrongRate(allMaxD, allMaxDMassA,
			rightMaxD, rightMaxDMassA);
		rou << "Relative error in the maximum particle size of sand =" << rou_wrongRate_allMax << std::endl;
		double rou_wrongRate_allMin = calculateWrongRate(allMinD, allMinDMassA,
			rightMinD, rightMinDMassA);
		rou << "Relative error in the minimum particle size of sand =" << rou_wrongRate_allMin << std::endl;
		double rou_wrongRate_allAverage = calculateWrongRate(allAverageD, allAverageDMassA,
			rightAverageD, rightAverageDMassA);
		rou << "Relative error in the mean particle size of sand =" << rou_wrongRate_allAverage << std::endl;
	}
};
void readTextData(const std::string& inputAllSandD, const std::string& inputWrongSandD,
	std::map<int, std::tuple<double, double, double>>& allData, std::vector<int>& wrongeData)
{
	std::ifstream allSandD(inputAllSandD);
	if (!allSandD.is_open())
	{
		std::cout << "No full data file opened!" << std::endl;
		exit(-1);
	}
	std::ifstream wrongSandD(inputWrongSandD);
	if (!wrongSandD.is_open())
	{
		std::cout << "No error sand data file opened!" << std::endl;
		exit(-1);
	}
	std::string line, sandNumber, intermediateData;
	while (std::getline(allSandD, line))
	{
		std::vector<std::string>oneSand;
		std::istringstream oneSandData(line);
		for (int i = 0; i < SAND_DATA_NUMBER; i++)
		{
			oneSandData >> intermediateData;
			if (i == 0)
			{
				readSandNumber(intermediateData, sandNumber);
				oneSand.push_back(sandNumber);
			}
			else
			{
				oneSand.push_back(intermediateData);
			}
		}
		allData.insert(std::pair<int, std::tuple<double, double, double>>
			(stoi(oneSand[0]),
				std::tuple<double, double, double>(stod(oneSand[1]), stod(oneSand[2]), stod(oneSand[3]))));
	}
	while (std::getline(wrongSandD, line))
	{
		wrongeData.push_back(stoi(line));
		std::cout << line << std::endl;
	}
};
void readSandNumber(const std::string& intermediateData, std::string& sandNumber)
{
	std::string numbers("0123456789");
	auto pos = intermediateData.find_first_of(numbers);
	auto end = intermediateData.find_last_not_of(numbers);
	sandNumber = intermediateData.substr(pos, end - pos);
};
void writeTextDiameter(const std::string& GASMinAddress, const std::multiset<double>& allGASMassA)
{
	std::ofstream allSandD(GASMinAddress);
	for (auto it = allGASMassA.begin(); it != allGASMassA.end(); it++)
	{
		allSandD << (*it) << std::endl;
	}
};
void writeTextDiameter(const std::string& address, std::multiset<double>& MaxDMassA, const std::multiset<double>& rightV)
{
	std::ofstream allSandD(address);
	auto itt = MaxDMassA.begin();
	for (auto it = rightV.begin(); it != rightV.end(); it++, itt++)
	{
		allSandD << (*it) << "    " << (*itt) << std::endl;
	}
};
void getMaxMinDiameter(const std::multiset<double>& allD, const std::multiset<double>& rightD, double& maxTwoD, double& minTwoD)
{
	double first = *(allD.begin());
	double second = *(rightD.begin());
	if (first >= second)
	{
		minTwoD = first;
	}
	else
	{
		minTwoD = second;
	}
	first = *(--allD.end());
	second = *(--rightD.end());
	if (first >= second)
	{
		maxTwoD = second;
	}
	else
	{
		maxTwoD = first;
	}
};
void readManualMeasureDiameter(const std::string& inputManualMaxMinA, std::multiset<double>& minManualD,
	std::multiset<double>& maxManualD, std::multiset<double>& averageManualD)
{
	std::ifstream ManualSandD(inputManualMaxMinA);
	if (!ManualSandD.is_open())
	{
		std::cout << "No full data file opened!" << std::endl;
		exit(-1);
	}
	std::string line, maxD, minD;
	while (std::getline(ManualSandD, line))
	{
		std::istringstream oneSandData(line);
		oneSandData >> maxD >> minD;
		maxManualD.insert(stod(maxD));
		minManualD.insert(stod(minD));
		averageManualD.insert(sqrtf(stod(maxD) * stod(minD)));
	}
};
void readManualMeasureDiameter(const std::string& inputGASSandA, std::multiset<double>& minGASD)
{
	std::ifstream ManualSandD(inputGASSandA);
	if (!ManualSandD.is_open())
	{
		std::cout << "No full data file opened!" << std::endl;
		exit(-1);
	}
	std::string line, minD;
	while (std::getline(ManualSandD, line))
	{
		std::istringstream oneSandData(line);
		oneSandData >> minD;
		minGASD.insert(stod(minD));
	}
};
double calculateTwoCurveCorrelation(std::multiset<double>firstHorizontalCurve, std::multiset<double> firstCurveVertical,
	std::multiset<double>secondHorizontalCurve, std::multiset<double>secondCurveVertical
	, const std::string& workAddress)
{
	std::map<double, double>firstCurveM;
	std::map<double, double>secondCurveM;
	mergeData(firstCurveVertical, firstHorizontalCurve, firstCurveM);
	mergeData(secondCurveVertical, secondHorizontalCurve, secondCurveM);
	double average_x = 0;
	double average_y = 0;
	double begin_p = 0;
	double cov = 0;
	double var_x = 0;
	double var_y = 0;
	int firstSize = firstCurveM.size();
	double interval = firstSize;
	std::vector<double> firstDiameterVector, secondDiameterVector;
	getRelationDiameterBetweenTwoCurves(firstCurveM, secondCurveM, firstDiameterVector, secondDiameterVector);
	calculateAverageDiameter(average_x, average_y, firstDiameterVector, secondDiameterVector);
	for (int i = 0; i < firstSize; i++)
	{
		cov += (firstDiameterVector[i] - average_x) * (secondDiameterVector[i] - average_y);
		var_x += (firstDiameterVector[i] - average_x) * (firstDiameterVector[i] - average_x);
		var_y += (secondDiameterVector[i] - average_y) * (secondDiameterVector[i] - average_y);
	}
	return cov / (sqrtf(var_x) * sqrtf(var_y));
};
void calculateAccumulateCurveHorizon(std::multiset<double>& allGASMassA, const std::multiset<double>& minGASD, double GASTolalArea)
{
	double sum = 0;
	for (auto it = minGASD.begin(); it != minGASD.end(); it++)
	{
		double value = (*it) * (*it) * PI / 4 / GASTolalArea;
		sum += value;
		if (abs(sum - 1) < 0.00000001)
		{
			sum = 1;
		}
		allGASMassA.insert(sum);
	}
};
void calculateAccumulateCurveHorizon(const std::multiset<double>& MaxD, std::multiset<double>& MaxDMassA, std::multiset<double>& MaxDMass,
	const double totalValue)
{
	double sum = 0;
	for (auto it = MaxD.begin(); it != MaxD.end(); it++)
	{
		double value = (*it) * (*it) * PI / 4 / totalValue;
#ifdef FEATURE_GRAIN
		MaxDMass.insert(*it);
#else
		MaxDMass.insert(value);
#endif

		sum += value;
		if (abs(sum - 1) < 0.00000001)
		{
			sum = 1;
		}
		MaxDMassA.insert(sum);
	}

};
void mergeData(const std::multiset<double>& horizontalCurve, const std::multiset<double>& AccumulateCurve,
	std::map<double, double>& CurveM)
{
	for (auto it = horizontalCurve.begin(), itt = AccumulateCurve.begin(); it != horizontalCurve.end(); it++, itt++)
	{
		CurveM.insert(std::pair<double, double>((*it), (*itt)));
	}
};
void findIntervalInCurveM(const std::map<double, double>& CurveM, std::pair<double, double>& first,
	std::pair<double, double>& second, double position)
{
	auto end = CurveM.end()--;
	auto next = CurveM.begin()++;
	for (auto it = CurveM.begin(); it != end; it++, next++)
	{
		if (position >= it->first && position <= next->first)
		{
			first = *it;
			second = *next;
			return;
		}
	}
	std::cout << "position=" << position << std::endl;
	std::cout << "No suitable interval found, data error occurred!";
	exit(-1);
};
void calculateAverageDiameter(const std::map<double, double>& firstCurveM, const std::map<double, double>& secondCurveM,
	double& average_x, double& average_y, const double& maxTwoD, const double& minTwoD)
{
	average_x = 0;
	for (auto it = firstCurveM.begin(); it != firstCurveM.end(); it++)
	{
		if ((it->first >= minTwoD && it->first <= maxTwoD))
		{
			average_x += it->first * it->second;
		}
	}

	average_y = 0;
	for (auto it = secondCurveM.begin(); it != secondCurveM.end(); it++)
	{
		if ((it->first >= minTwoD && it->first <= maxTwoD))
		{
			average_y += it->first * it->second;
		}
	}
};
void readTextData(const std::string& inputAllSandD,
	std::map<int, std::tuple<double, double, double>>& allData)
{
	std::ifstream allSandD(inputAllSandD);
	if (!allSandD.is_open())
	{
		std::cout << "No full data file opened!" << std::endl;
		exit(-1);
	}

	std::string line, sandNumber, intermediateData;
	while (std::getline(allSandD, line))
	{
		std::vector<std::string>oneSand;
		std::istringstream oneSandData(line);
		for (int i = 0; i < SAND_DATA_NUMBER; i++)
		{
			oneSandData >> intermediateData;
			if (i == 0)
			{
				readSandNumber(intermediateData, sandNumber);
				oneSand.push_back(sandNumber);
			}
			else
			{
				oneSand.push_back(intermediateData);
			}
		}
		allData.insert(std::pair<int, std::tuple<double, double, double>>
			(stoi(oneSand[0]),
				std::tuple<double, double, double>(stod(oneSand[1]), stod(oneSand[2]), stod(oneSand[3]))));
	}
};
void mergeManualData(std::map<int, std::tuple<double, double, double>>& rightAllData, const std::multiset<double>& maxManualD, const std::multiset<double>& minManualD
	, const std::multiset<double>& averageManualD)
{
	int number = 0;
	for (auto it_max = maxManualD.begin(), it_min = minManualD.begin(), it_average = averageManualD.begin(); it_max != maxManualD.end(); it_max++, it_min++, it_average++, number++)
	{
		rightAllData.insert(std::pair<int, std::tuple<double, double, double>>(number, std::tuple<double, double, double>(*it_max, *it_min, *it_average)));
	}
};
double calculateWrongRate(std::multiset<double>& firstHorizontalCurve, std::multiset<double>& firstCurveVertical,
	std::multiset<double>& secondHorizontalCurve, std::multiset<double>& secondCurveVertical)
{
	std::map<double, double>allData;
	std::map<double, double>rightData;
	mergeData(firstHorizontalCurve, firstCurveVertical, allData);
	mergeData(secondHorizontalCurve, secondCurveVertical, rightData);
	double minD = *firstHorizontalCurve.begin(), maxD = *(--firstHorizontalCurve.end());
	if (*secondHorizontalCurve.begin() > minD)
	{
		minD = *secondHorizontalCurve.begin();
	}
	if (*(--secondHorizontalCurve.end()) < maxD)
	{
		maxD = *(--secondHorizontalCurve.end());
	}
	std::set<double>allV, rightV;
	double interal = (maxD - minD) / DIVIDE_NUMBER;
	for (int i = 0; i < DIVIDE_NUMBER; i++)
	{
		double position = minD + interal * i;
		std::pair<double, double>A_bg, A_ed, R_bg, R_ed;
		findIntervalInCurve(allData, position, A_bg, A_ed);
		findIntervalInCurve(rightData, position, R_bg, R_ed);
		double A_value = (position - A_ed.first) / (A_bg.first - A_ed.first) * A_bg.second + (position - A_bg.first) / (A_ed.first - A_bg.first) * A_ed.second;
		allV.insert(A_value);
		double R_value = (position - R_ed.first) / (R_bg.first - R_ed.first) * R_bg.second + (position - R_bg.first) / (R_ed.first - R_bg.first) * R_ed.second;
		rightV.insert(R_value);
	}
	double error = 0;
	double sum = 0;
	for (auto it = allV.begin(), itt = rightV.begin(); it != allV.end(); it++, itt++)
	{
		error += abs(*it - *itt);
		sum += *it;
	}
	return error / sum;
};
void  findIntervalInCurve(const std::map<double, double>& allData, const double position, std::pair<double, double>& bg, std::pair<double, double>& ed)
{
	for (auto it = allData.begin(), itt = ++allData.begin(); itt != allData.end(); it++, itt++)
	{
		if (position >= it->first && position <= itt->first)
		{
			bg = *it;
			ed = *itt;
			return;
		}

	}
};
void writeWrongText(const std::string& Address, const std::multiset<double>& wrongData)
{
	std::ofstream allSandD(Address);
	for (auto it = wrongData.begin(); it != wrongData.end(); it++)
	{
		allSandD << (*it) << "    " << std::endl;
	}
};
void writeWtongText(const std::string& address, const std::map<int, std::tuple<double, double, double>>& wrongAllData)
{
	std::ofstream allSandD(address);
	for (auto it = wrongAllData.begin(); it != wrongAllData.end(); it++)
	{
		allSandD << "No." << it->first << "Grain of sand:" << " " << std::get<0>(it->second) * 10 << "   "
			<< std::get<1>(it->second) * 10 << "   " << std::get<2>(it->second) * 10 << std::endl;
	}
};
void readGASAndSievingData(const std::string& inputGAS, std::multiset<double>& GASFirstDiameter
	, std::multiset<double>& GASSecondCumulationMass)
{
	std::ifstream ManualSandD(inputGAS);
	if (!ManualSandD.is_open())
	{
		std::cout << "No full data file opened!" << std::endl;
		exit(-1);
	}
	std::string line, maxD, minD;
	while (std::getline(ManualSandD, line))
	{
		std::istringstream oneSandData(line);
		oneSandData >> maxD >> minD;
		GASFirstDiameter.insert(stod(maxD));
		GASSecondCumulationMass.insert(stod(minD));
	}
};
void getRelationDiameterBetweenTwoCurves(const std::map<double, double>& firstCurveM
	, const std::map<double, double>& secondCurveM, std::vector<double>& firstDiameterVector,
	std::vector<double>& secondDiameterVector)
{
	int cC = 0;
	for (auto it = firstCurveM.begin(); it != firstCurveM.end(); it++)
	{
		cC++;
		if (cC == 106)
		{
			std::cout << "cc";
		}
		std::cout << cC << std::endl;
		firstDiameterVector.push_back(it->second);
		secondDiameterVector.push_back(getRelationSecondCurveDiameter(it->first, secondCurveM));
	}

};
double getRelationSecondCurveDiameter(double firstCumulationValue,
	const std::map<double, double>& secondCurveM)
{
	double sDV = 0;
	auto it = secondCurveM.begin(), next = it;
	next++;
	if (secondCurveM.begin()->first >= firstCumulationValue)
	{
		return secondCurveM.begin()->second;
	};
	for (; next != secondCurveM.end(); it++, next++)
	{

		if ((it->first <= firstCumulationValue) && (next->first >= firstCumulationValue))
		{
			sDV = it->second + (next->second - it->second) * (firstCumulationValue - it->first) / (next->first - it->first);
			break;
		}
	}
	if (sDV == 0)
	{
		std::cout << "Don't get the relation diameter in the second curve!";
		std::exit(-1);
	}
	return sDV;
};
void calculateAverageDiameter(double& average_x, double& average_y, const std::vector<double>& firstDiameterVector,
	const std::vector<double>& secondDiameterVector)
{
	double sumX = 0, sumY = 0;
	for (auto it = firstDiameterVector.begin(); it != firstDiameterVector.end(); it++)
	{
		sumX += *it;
	}
	for (auto it = secondDiameterVector.begin(); it != secondDiameterVector.end(); it++)
	{
		sumY += *it;
	}
	average_x = sumX / firstDiameterVector.size();
	average_y = sumY / secondDiameterVector.size();
};
//void readTextData(const std::string& inputAllSandD,
//	std::map<int, std::tuple<double, double, double>>& allData)
//{
//	std::ifstream allSandD(inputAllSandD);
//	if (!allSandD.is_open())
//	{
//		std::cout << "No full data file opened!" << std::endl;
//		exit(-1);
//	}
//
//	std::string line, sandNumber, intermediateData;
//	while (std::getline(allSandD, line))
//	{
//		std::vector<std::string>oneSand;
//		std::istringstream oneSandData(line);
//		for (int i = 0; i < SAND_DATA_NUMBER; i++)
//		{
//			oneSandData >> intermediateData;
//			if (i == 0)
//			{
//				readSandNumber(intermediateData, sandNumber);
//				oneSand.push_back(sandNumber);
//			}
//			else
//			{
//				oneSand.push_back(intermediateData);
//			}
//		}
//		allData.insert(std::pair<int, std::tuple<double, double, double>>
//			(stoi(oneSand[0]),
//				std::tuple<double, double, double>(stod(oneSand[1]), stod(oneSand[2]), stod(oneSand[3]))));
//	}
//};
void writeTextDiameter(std::ofstream& feature, std::multiset<double>& MaxDMassA, const std::multiset<double>& rightV)
{
	auto itt = MaxDMassA.begin();
	for (auto it = rightV.begin(); it != rightV.end(); it++, itt++)
	{
		feature << (*it) << "    " << (*itt) << std::endl;
	}

};
void  writeDiameter(std::map<double, double>& AccumulateDiameter, const std::multiset<double>& allAverageDMassA,
	const std::multiset<double>& allAverageD)
{
	auto itt = allAverageDMassA.begin();
	for (auto it = allAverageD.begin(); it != allAverageD.end(); it++, itt++)
	{
		AccumulateDiameter.insert(std::pair<double, double>(*itt, *it));
	}
};
void getFeatureDiamter(std::ofstream& feature, double featureGrain, const std::map<double, double>& AccumulateDiameter)
{
	double dia;
	auto it = AccumulateDiameter.begin();
	auto next = it;
	for (next++; next != AccumulateDiameter.end(); it++, next++)
	{
		if (featureGrain >= it->first && featureGrain <= next->first)
		{
			dia = it->second + (featureGrain - it->first) / (next->first - it->first) * (next->second - it->second);
			break;
		}

	}
	feature << "gre= " << gre << "  " << "can1= " << can1 << "  " << "can2= " << can2 << "  " << dia << std::endl;
};