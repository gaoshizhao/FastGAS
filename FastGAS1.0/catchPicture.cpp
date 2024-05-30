#include "catchPicture.h"
#include"test.h"
namespace cp {
	cv::Rect box;
	Color rectColor;
	
	bool drawing_box=false;
	void my_mouse_callback(int event, int x, int y, int flags, void* param)
	{
		cv::Mat& img = *(cv::Mat*)param;
		switch (event)
		{
		case CV_EVENT_LBUTTONDOWN:
		{
			drawing_box = true;
			box = cv::Rect(x, y, 0, 0);
			break;
		}
		case CV_EVENT_MOUSEMOVE:
		{
			drawing_box = false;
			box.width = x - box.x;
			box.height = y - box.y;
			break;
		}
		case CV_EVENT_LBUTTONUP:
		{
			drawing_box = false;
			if (box.height < 0)
			{
				box.x += box.width;
				box.height = -1 * box.height;
			}
			if (box.width < 0)
			{
				box.y += box.height;
				box.width = -1 * box.width;
			}
			displayColorRange(img,box);
			break;
		}
		case CV_EVENT_RBUTTONDOWN:
		{
			cv::Vec3b pix = img.at<cv::Vec3b>(x, y);
			for (int k = 0; k < COLOR_CHANNELS; k++)
			{
				std::cout << (int)pix[k] << " ";
			}
			std::cout << std::endl;
			break;
		
		}

		}
	};
	void draw_box(cv::Mat& img, cv::Rect box)
	{
		cv::rectangle(img, box.tl(), box.br(), cv::Vec3b(0, 120, 25),10);
	};
	void displayColorRange(cv::Mat& colorPictrue, const cv::Rect& box)
	{
		cv::Mat childPicture;
		colorPictrue(box).copyTo(childPicture);
		draw_box(colorPictrue, box);
		Color boxColor;
		for (int k = 0; k < COLOR_CHANNELS; k++)
		{
			boxColor.minColor[k] = childPicture.at<cv::Vec3b>(0,0)[k];
			boxColor.maxColor[k] = childPicture.at<cv::Vec3b>(0,0)[k];
		}
		for (int i = 0; i < childPicture.rows; i++)
		{
			for (int j = 0; j < childPicture.cols; j++)
			{
				for (int k = 0; k < COLOR_CHANNELS; k++)
				{
					int a = boxColor.maxColor[k];
					int b = boxColor.minColor[k];
					boxColor.maxColor[k] = max(boxColor.maxColor[k], childPicture.at<cv::Vec3b>(i, j)[k]);
					boxColor.minColor[k] = min(boxColor.minColor[k], childPicture.at<cv::Vec3b>(i, j)[k]);
				}
			}
		}
		for (int i = 0; i < COLOR_CHANNELS; i++)
		{
			std::cout << "boxColor[" << i << "]=" <<(int) boxColor.maxColor[i] 
				<<"--"<< (int)boxColor.minColor[i]<< std::endl;	
		}
		rectColor.maxColor = boxColor.maxColor;
		rectColor.minColor = boxColor.minColor;
	};
	void mouseCallBackDrive(const cv::Mat& colorPictrue,const std::string&windowName)
	{
		cv::Mat picture;
	    colorPictrue.copyTo(picture);
		clearColor(rectColor);
		cv::namedWindow(windowName, CV_WINDOW_NORMAL);
		cv::setMouseCallback(windowName, my_mouse_callback,(void*)&picture);
		cv::Mat temp;
		for (;;)
		{
			picture.copyTo(temp);
			if (drawing_box)
			{
				draw_box(temp,box);
			}
			cv::imshow(windowName,temp);
			if ((int)cv::waitKey(15) == 27)
			{
				break;
			}
		}
	};
	double getSinglePixelArea(const cv::Mat& colorPicture, double area,const std::string &workFolderAddress)
	{
		std::string windowName("Square control box to select a colour");
		std::cout << "In the window below, use the right mouse button to drag the selection square to control the colour range of the empty space!" << std::endl;
		std::cout << "When you've made your selection, press esc to exit!" << std::endl;
		cv::Mat picture;
		colorPicture.copyTo(picture);
		mouseCallBackDrive(picture, windowName);
		cv::Mat colorMark = cv::Mat::zeros(colorPicture.rows,colorPicture.cols,CV_8UC1);
		double count= filterStandardTemplateImageRange(picture,rectColor,workFolderAddress);
		cv::destroyAllWindows();
		return area/count;
	
	};
	void clearColor(Color& color)
	{
		color.maxColor = color.minColor = 0;
	};
	int filterStandardTemplateImageRange(const cv::Mat& picture, const Color& color, const std::string& workFolderAddress)
	{
		cv::Mat newColorPicture;
		picture.copyTo(newColorPicture);
		cv::Vec3b zeroColor(255,255,255);
		int count = 0;
		for (int i = 0; i < picture.rows; i++)
		{
			for (int j = 0; j < picture.cols; j++)
			{
				cv::Vec3b& tempColor = newColorPicture.at<cv::Vec3b>(i, j);
				if ( noStantardTemplateImage(tempColor,color))
				{
					tempColor = zeroColor;
				}
				else 
				{
					count++;
				}
			}
		}
 		writePicture(newColorPicture,workFolderAddress);
		return count;
	};
	bool noStantardTemplateImage(const cv::Vec3b& tempColor, const Color& color)
	{
		bool result = true;
             if (((tempColor[0] >= color.minColor[0]) && (tempColor[0] <= color.maxColor[0]))&&
				((tempColor[1] >= color.minColor[1]) && (tempColor[1] <= color.maxColor[1]))&&
				((tempColor[2] >= color.minColor[2]) && (tempColor[2] <= color.maxColor[2]))
				)
			{
				result = false;

			}

		return result;
	};
}