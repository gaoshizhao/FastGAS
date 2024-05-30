#include "pixelSizeDetermination.h"
#include <windows.h>
#include <limits>
#define TEST_PHRASE
// ȫ�ֱ��������ڴ洢ѡ��ľ��������ͼ��  
cv::Rect selection;
cv::Mat selectedRegion;
bool selecting = false;
bool isRed(const cv::Vec3b& hsv, const cv::Vec3b& lowerRed, const cv::Vec3b& upperRed) {
	// ���ɫ�������ͶȺ������Ƿ��ں�ɫ��Χ��  
	return (hsv[0] >= lowerRed[0] && hsv[0] <= upperRed[0] &&
		hsv[1] >= lowerRed[1] && hsv[1] <= upperRed[1] &&
		hsv[2] >= lowerRed[2] && hsv[2] <= upperRed[2]);
}



// ����¼��ص�����  
void mouseHandler(int event, int x, int y, int flags, void* userdata)
{
		
	cv::Mat image = (*(cv::Mat*)userdata).clone();

	switch (event)
	{
	case cv::EVENT_LBUTTONDOWN:
		// ����������ʱ��ʼѡ��  
		selecting = true;
		selection = cv::Rect(x, y, 0, 0);
		break;

	case cv::EVENT_MOUSEMOVE:
		// ����ƶ�ʱ����ѡ������  
		if (selecting)
		{
			selection.width = x - selection.x;
			selection.height = y - selection.y;
			// ��ԭͼ���ϻ���ѡ��ľ�������  
			cv::rectangle(image, selection, cv::Scalar(0, 255, 0), 2);
			cv::imshow("Image Selection", image);
			cv::waitKey(20);

		}
		break;

	case cv::EVENT_LBUTTONUP:
		// �������ͷ�ʱ����ѡ��  
		selecting = false;
		if (selection.width > 0 && selection.height > 0)
		{
			// ��ԭͼ������ȡѡ�������  
			selectedRegion = image(selection);
			// ��ԭͼ���ϻ���ѡ��ľ�������  
			cv::rectangle(image, selection, cv::Scalar(0, 255, 0), 2);
			cv::imshow("Image Selection", image);
			cv::waitKey(20);
		}
		break;
	}
}


//Obtain single pixel size
double getCircleSinglePixelArea(cv::Mat Image, double circleArea, std::string workFaddress)
{
	//This function identifies the area of the circular calibration object in the
	//figure to obtain the number of pixels in that area, which can further obtain the size of a single pixel.
	//Identify a red sphere in the image and display the recognition result, allowing the user to determine 
	//if the recognition is correct. If the input is correct, press the ESC key. If it is incorrect,
	//ask the user to use the mouse to frame the position of the sphere and re recognize it
	/*
		��һ����Ҫ�û���ѡ���궨��Ĵ��·�Χ����Ҫ��Ϊ����߱궨�����ٶȺͼ���׼ȷ�ԣ���
		�ڶ�������ɫѡ���������ɫ��Ҫ����������ص㣻
		��������Canny�����߽磬��������Ȼ����findContoursѰ����װ��������

		���ģ��û���Բ�����Բ�Σ�
		���壬�Լ�����Բ�ΰ���Բ�Ƚ��������ҳ����������Ǹ�Բ����Ϊ�궨���������}

		��������ʾ�ü���������
		���ߣ�����Բ�������ڵ�����������Ȼ�������������ص�ʵ�����ֵ��
	*/

	// ת����HSV��ɫ�ռ�  
	cv::Mat image = Image.clone();
	cv::Mat hsvImage;
	cv::cvtColor(image, hsvImage, cv::COLOR_BGR2HSV);
	// �����ɫ��HSV��Χ  
	cv::Scalar lowerRed = cv::Scalar(150,100,  50);
	cv::Scalar upperRed = cv::Scalar(180,255,  255);
	// ������ɫ���������  
	cv::Mat mask;
	cv::inRange(hsvImage,lowerRed,upperRed,mask);
#ifdef TEST_PHRASE
	std::string outCircleMask = workFaddress + "circleMask.jpg";
	cv::imwrite(outCircleMask, mask);
#endif
	// ��ԭͼ����������λ���㣬�õ�ֻ������ɫ�����ͼ��  
	// ��������  
	std::vector<std::vector<cv::Point>> contours;
	cv::findContours(mask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

	// ��ʼ�����߿����������Ϊ0  
	double maxPixels = 0;
	cv::Mat maxContourMask = cv::Mat::zeros(mask.size(), CV_8UC1);
	std::vector<cv::Point> largestContour;
	// ���������������ҵ������Ǹ�  
	for (size_t i = 0; i < contours.size(); ++i) 
	{
		int contourPixels = cv::contourArea(contours[i]);

		if (contourPixels > maxPixels)
		{
			maxPixels = contourPixels;
			// �������߿����Ĥ  
			cv::drawContours(maxContourMask, contours, static_cast<int>(i), cv::Scalar(255), cv::FILLED);
			largestContour = contours[i];
		}
	}
	//************************************��Բ�������߱궨�ﾫ��*************************************************
	// ȷ�����Χ���߽��  
	cv::Rect boundingBox = cv::boundingRect(largestContour);
	cv::Point center = (boundingBox.br() + boundingBox.tl()) / 2;
	int radius = (boundingBox.height>boundingBox.width? boundingBox.height: boundingBox.width) / 2;
	// ������ԭʼͼ����ͬ��С�����͵Ŀհ�����ͼ��  
	cv::Mat maxContour = cv::Mat::zeros(image.size(), CV_8UC1);

	// �������ϻ���Բ������  
	cv::circle(maxContour, center, radius, cv::Scalar(255), cv::FILLED);

	// ����Բ�����������ص�����  
	int pixelCount = cv::countNonZero(maxContour);
	//*************************************************************************************
	// ����ҵ����߿飬���������ʾ  
	if (maxPixels > 0) 
	{
		cv::Mat maxPatch;
		cv::bitwise_and(image, image, maxPatch, maxContour);
#ifdef TEST_PHRASE
		std::string outCircle = workFaddress + "circle.jpg";
		cv::imwrite(outCircle, maxPatch);
#endif
		// ������߿����������  
		std::cout << "Number of pixels in the maximum patch: " << pixelCount << std::endl;
	}
	else 
	{
		std::cout << "No contours found in the image." << std::endl;
		exit(-1);
	}

	return circleArea / pixelCount;
};
// �Զ����Բ��ָ����㺯��  
float calculateRadiusConsistency(const cv::Mat& edges, float cx, float cy, float radius) {
	float sum = 0.0f;
	int count = 0;

	// ����ͼ���е�ÿ������  
	for (int y = 0; y < edges.rows; ++y) {
		for (int x = 0; x < edges.cols; ++x) {
			// ���㵱ǰ���ص�Բ�ĵľ���  
			float dist = std::sqrt((x - cx) * (x - cx) + (y - cy) * (y - cy));

			// �����ǰ������Բ�ϻ�ӽ�Բ��  
			if (edges.at<uchar>(y, x) == 255 && std::abs(dist - radius) < radius * 0.1) {
				// �ۼӾ�����ƽ���뾶�Ĳ�ֵ  
				sum += std::abs(dist - radius);
				count++;
			}
		}
	}

	// ���Բ�ϵĵ��������࣬�򷵻�һ����ֵ��ʾԲ�Ȳ���  
	if (count < 10) {
	}

	// �����ֵ��ƽ��ֵ��ΪԲ��ָ��  
	return sum / count;
}
// ����ͼ����Բ�����������������ĺ���  
int countPixelsInCircle(const cv::Mat& image, float cx, float cy, float radius) {
	int count = 0;

	// ����ͼ���е�ÿ������  
	for (int y = 0; y < image.rows; ++y) {
		for (int x = 0; x < image.cols; ++x) {
			// ���㵱ǰ���ص�Բ�ĵľ���  
			float dist = std::sqrt((x - cx) * (x - cx) + (y - cy) * (y - cy));

			// �����ǰ������Բ�ڻ�Բ��  
			if (dist <= radius) {
				// �������ؼ���  
				count++;
			}
		}
	}

	return count;
}
//������ʾ���ͼ��
void  showLargeImage(const cv::Mat& image)
{
	// ��ȡ����ʾ���Ŀ��  
	int screenWidth = GetSystemMetrics(SM_CXSCREEN) * 0.8;	// 1228 691��ȡ����ʾ���ĸ߶�  
	int screenHeight = GetSystemMetrics(SM_CYSCREEN) * 0.8;

	// ��ȡͼ���С  
	cv::Size imageSize = image.size();
	//����ϵ��
	double widthScale = imageSize.width / screenWidth;
	double heightScale = imageSize.height / screenHeight;

	/*�����������1.��ͼ��Ŀ�߾�С����Ļ�ĳߴ磻2.ͼ��Ŀ�߾�������Ļ�ߴ磻
	3.ͼ��Ŀ����һ��������Ļ�Ŀ�ߣ���һ��С��Ļ�Ŀ�ߡ���ȡ��ľͺ���	*/
	double imageScale = max(widthScale, heightScale);

	// ����ͼ������Ӧ����  
	cv::Mat resizedImage;
	cv::resize(image, resizedImage, cv::Size(), 1 / imageScale, 1 / imageScale, cv::INTER_LINEAR);
	cv::Size resizedImageSize = resizedImage.size();

// ����ͼ������Ӧ����  
	std::string windowName = "Image Selection";
	cv::namedWindow(windowName, cv::WINDOW_NORMAL);
	cv::resizeWindow(windowName, resizedImageSize.width, resizedImageSize.height);
	cv::imshow(windowName, resizedImage);
	cv::waitKey(0);
};