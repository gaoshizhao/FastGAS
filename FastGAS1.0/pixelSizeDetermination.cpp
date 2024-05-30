#include "pixelSizeDetermination.h"
#include <windows.h>
#include <limits>
#define TEST_PHRASE
// 全局变量，用于存储选择的矩形区域和图像  
cv::Rect selection;
cv::Mat selectedRegion;
bool selecting = false;
bool isRed(const cv::Vec3b& hsv, const cv::Vec3b& lowerRed, const cv::Vec3b& upperRed) {
	// 检查色调、饱和度和明度是否在红色范围内  
	return (hsv[0] >= lowerRed[0] && hsv[0] <= upperRed[0] &&
		hsv[1] >= lowerRed[1] && hsv[1] <= upperRed[1] &&
		hsv[2] >= lowerRed[2] && hsv[2] <= upperRed[2]);
}



// 鼠标事件回调函数  
void mouseHandler(int event, int x, int y, int flags, void* userdata)
{
		
	cv::Mat image = (*(cv::Mat*)userdata).clone();

	switch (event)
	{
	case cv::EVENT_LBUTTONDOWN:
		// 鼠标左键按下时开始选择  
		selecting = true;
		selection = cv::Rect(x, y, 0, 0);
		break;

	case cv::EVENT_MOUSEMOVE:
		// 鼠标移动时更新选择区域  
		if (selecting)
		{
			selection.width = x - selection.x;
			selection.height = y - selection.y;
			// 在原图像上绘制选择的矩形区域  
			cv::rectangle(image, selection, cv::Scalar(0, 255, 0), 2);
			cv::imshow("Image Selection", image);
			cv::waitKey(20);

		}
		break;

	case cv::EVENT_LBUTTONUP:
		// 鼠标左键释放时结束选择  
		selecting = false;
		if (selection.width > 0 && selection.height > 0)
		{
			// 从原图像中提取选择的区域  
			selectedRegion = image(selection);
			// 在原图像上绘制选择的矩形区域  
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
		第一，需要用户框选出标定物的大致范围（主要是为了提高标定物检测速度和检测的准确性）；
		第二，用颜色选择出符合颜色域要求的所有像素点；
		第三，用Canny法检测边界，（待定）然后用findContours寻找组装轮廓，；

		第四，用霍夫圆法检测圆形；
		第五，对检测出的圆形按照圆度进行排序，找出最完美的那个圆，即为标定物的轮廓；}

		第六，显示该检测出的区域；
		第七，计算圆形区域内的像素数量，然后计算出单个像素的实际面积值；
	*/

	// 转换到HSV颜色空间  
	cv::Mat image = Image.clone();
	cv::Mat hsvImage;
	cv::cvtColor(image, hsvImage, cv::COLOR_BGR2HSV);
	// 定义红色的HSV范围  
	cv::Scalar lowerRed = cv::Scalar(150,100,  50);
	cv::Scalar upperRed = cv::Scalar(180,255,  255);
	// 创建红色区域的掩码  
	cv::Mat mask;
	cv::inRange(hsvImage,lowerRed,upperRed,mask);
#ifdef TEST_PHRASE
	std::string outCircleMask = workFaddress + "circleMask.jpg";
	cv::imwrite(outCircleMask, mask);
#endif
	// 对原图像和掩码进行位运算，得到只包含红色区域的图像  
	// 查找轮廓  
	std::vector<std::vector<cv::Point>> contours;
	cv::findContours(mask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

	// 初始化最大斑块的像素数量为0  
	double maxPixels = 0;
	cv::Mat maxContourMask = cv::Mat::zeros(mask.size(), CV_8UC1);
	std::vector<cv::Point> largestContour;
	// 遍历所有轮廓，找到最大的那个  
	for (size_t i = 0; i < contours.size(); ++i) 
	{
		int contourPixels = cv::contourArea(contours[i]);

		if (contourPixels > maxPixels)
		{
			maxPixels = contourPixels;
			// 绘制最大斑块的掩膜  
			cv::drawContours(maxContourMask, contours, static_cast<int>(i), cv::Scalar(255), cv::FILLED);
			largestContour = contours[i];
		}
	}
	//************************************用圆形拟合提高标定物精度*************************************************
	// 确定最大范围（边界框）  
	cv::Rect boundingBox = cv::boundingRect(largestContour);
	cv::Point center = (boundingBox.br() + boundingBox.tl()) / 2;
	int radius = (boundingBox.height>boundingBox.width? boundingBox.height: boundingBox.width) / 2;
	// 创建与原始图像相同大小和类型的空白遮罩图像  
	cv::Mat maxContour = cv::Mat::zeros(image.size(), CV_8UC1);

	// 在遮罩上绘制圆形区域  
	cv::circle(maxContour, center, radius, cv::Scalar(255), cv::FILLED);

	// 计算圆形区域内像素的数量  
	int pixelCount = cv::countNonZero(maxContour);
	//*************************************************************************************
	// 如果找到最大斑块，则输出并显示  
	if (maxPixels > 0) 
	{
		cv::Mat maxPatch;
		cv::bitwise_and(image, image, maxPatch, maxContour);
#ifdef TEST_PHRASE
		std::string outCircle = workFaddress + "circle.jpg";
		cv::imwrite(outCircle, maxPatch);
#endif
		// 输出最大斑块的像素数量  
		std::cout << "Number of pixels in the maximum patch: " << pixelCount << std::endl;
	}
	else 
	{
		std::cout << "No contours found in the image." << std::endl;
		exit(-1);
	}

	return circleArea / pixelCount;
};
// 自定义的圆度指标计算函数  
float calculateRadiusConsistency(const cv::Mat& edges, float cx, float cy, float radius) {
	float sum = 0.0f;
	int count = 0;

	// 遍历图像中的每个像素  
	for (int y = 0; y < edges.rows; ++y) {
		for (int x = 0; x < edges.cols; ++x) {
			// 计算当前像素到圆心的距离  
			float dist = std::sqrt((x - cx) * (x - cx) + (y - cy) * (y - cy));

			// 如果当前像素在圆上或接近圆上  
			if (edges.at<uchar>(y, x) == 255 && std::abs(dist - radius) < radius * 0.1) {
				// 累加距离与平均半径的差值  
				sum += std::abs(dist - radius);
				count++;
			}
		}
	}

	// 如果圆上的点数不够多，则返回一个大值表示圆度不好  
	if (count < 10) {
	}

	// 计算差值的平均值作为圆度指标  
	return sum / count;
}
// 计算图像中圆形区域内像素数量的函数  
int countPixelsInCircle(const cv::Mat& image, float cx, float cy, float radius) {
	int count = 0;

	// 遍历图像中的每个像素  
	for (int y = 0; y < image.rows; ++y) {
		for (int x = 0; x < image.cols; ++x) {
			// 计算当前像素到圆心的距离  
			float dist = std::sqrt((x - cx) * (x - cx) + (y - cy) * (y - cy));

			// 如果当前像素在圆内或圆上  
			if (dist <= radius) {
				// 增加像素计数  
				count++;
			}
		}
	}

	return count;
}
//缩放显示大幅图像
void  showLargeImage(const cv::Mat& image)
{
	// 获取主显示器的宽度  
	int screenWidth = GetSystemMetrics(SM_CXSCREEN) * 0.8;	// 1228 691获取主显示器的高度  
	int screenHeight = GetSystemMetrics(SM_CYSCREEN) * 0.8;

	// 获取图像大小  
	cv::Size imageSize = image.size();
	//缩放系数
	double widthScale = imageSize.width / screenWidth;
	double heightScale = imageSize.height / screenHeight;

	/*分三种情况：1.是图像的宽高均小于屏幕的尺寸；2.图像的宽高均大于屏幕尺寸；
	3.图像的宽高有一个大于屏幕的宽高，有一个小屏幕的宽高。都取大的就合适	*/
	double imageScale = max(widthScale, heightScale);

	// 缩放图像以适应窗口  
	cv::Mat resizedImage;
	cv::resize(image, resizedImage, cv::Size(), 1 / imageScale, 1 / imageScale, cv::INTER_LINEAR);
	cv::Size resizedImageSize = resizedImage.size();

// 缩放图像以适应窗口  
	std::string windowName = "Image Selection";
	cv::namedWindow(windowName, cv::WINDOW_NORMAL);
	cv::resizeWindow(windowName, resizedImageSize.width, resizedImageSize.height);
	cv::imshow(windowName, resizedImage);
	cv::waitKey(0);
};