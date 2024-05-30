#include"test.h"
#include <windows.h>
void displaySimilar(const Similar& similar)
{
	std::cout << "similar.maxSimilar=" << similar.maxSimilar << std::endl;
	std::cout << "similar.minSimilar=" << similar.minSimilar << std::endl;
};
void displayCell(const Cell& cell)
{
	displayMaxCell(cell);
	displayMinCell(cell);
};
void displayMaxCell(const Cell& cell)
{
	const Matx91b& maxCell = cell.maxCell;
	std::cout << "cell.maxCell:" << std::endl;
	for (int i = 0; i < MATX_ROW; i++)
	{
		for (int j = 0; j < MAT_CHANNEL; j++)
		{
			std::cout << std::setw(12) << (int)maxCell.val[i][j] << " ";
		}
		std::cout << std::endl;
	}

};
void displayMinCell(const Cell& cell)
{
	const Matx91b& minCell = cell.minCell;
	std::cout << "cell.minCell:" << std::endl;
	for (int i = 0; i < MATX_ROW; i++)
	{
		for (int j = 0; j < MAT_CHANNEL; j++)
		{
			std::cout << std::setw(12) << (int)minCell.val[i][j] << " ";
		}
		std::cout << std::endl;
	}
};
void displayMark(cv::Mat mark, const std::vector<Cell>& cellSet)
{
	int rows = mark.rows;
	int cols = mark.cols;
	cv::Mat newMark=cv::Mat::zeros(rows,cols,CV_8UC3);
	int count = cellSet.size();
	for (int i = 0; i < count; i++)
	{
		newMark.at<cv::Vec3b>(cellSet[i].position) = CENTER_COLOR;
	}
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			if ((mark.at<int>(i, j) != SINGE_EMPTY_COLOR)
				&& (newMark.at<cv::Vec3b>(i, j) != CENTER_COLOR))
			{
				newMark.at<cv::Vec3b>(i, j) = BORDERARY_COLOR;
			}
		}
	}
	cv::namedWindow("markPicture", cv::WINDOW_NORMAL);
	cv::imshow("markPicture", newMark);
	cv::waitKey(0);
};
void displayPicture(const cv::Mat& picture, const std::string& name)
{
	cv::namedWindow(name, cv::WINDOW_NORMAL);
	cv::imshow(name, picture);
	cv::waitKey(0);

};
void displayColorMark(cv::Mat ColorPicture, const std::vector<Cell>& tempCellSet, std::vector<Cell>cellSet)
{
	cv::Mat newColorPicture;
    ColorPicture.copyTo(newColorPicture);
	int tempCount = tempCellSet.size();
	int count = cellSet.size();
	for (int i = 0; i < count; i++)
	{
		markColorPicture(newColorPicture, cellSet[i].position);
	}
	for (int i = 0; i < tempCount; i++)
	{
		markColorPicture(newColorPicture, cellSet[i].position);
	}
	cv::namedWindow("newColorPicture", cv::WINDOW_NORMAL);
	cv::imshow("newColorPicture", newColorPicture);
	cv::waitKey(0);

};
std::ostream& operator<<(std::ostream& os, const cv::Vec3b& pix)
{
	os << (int)pix[0] << " " << (int)pix[1] << " " << (int)pix[2] ;
	return os;
};
std::ostream& operator<<(std::ostream& os, const cv::Point& coordination)
{
	os << "[" << (int)coordination.x << " " << (int)coordination.y<<"]";
	return os;
};
void markColorPicture(cv::Mat& newColorPicture, const cv::Point& location, int layerNumber)
{
	cv::Point topLeft(location.x- layerNumber,location.y- layerNumber);
	int rows = layerNumber * 2 + 2, cols = layerNumber * 2 + 2;//行后和列后
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			newColorPicture.at<cv::Vec3b>(cv::Point(topLeft.x + j, topLeft.y + i)) = CENTER_COLOR;
		}
	}
};
void displayMark(const cv::Mat& mark)
{
	cv::Mat newMark;
	newMark=cv::Mat::zeros(mark.rows,mark.cols,CV_8U);
	int rows = mark.rows;
	int cols = mark.cols;
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			if (mark.at<int>(i, j)!=0 )
			{
				newMark.at<uchar>(i, j) = 255;
			}
		}
	}
	cv::namedWindow("Mark Picture", CV_WINDOW_NORMAL);
	cv::imshow("Mark Picture", newMark);
	cv::waitKey(0);

};
bool is_empty(const cv::Mat& TestCellPosition_Mark)
{
	bool empty = true;
	int rows = TestCellPosition_Mark.rows;
	int cols = TestCellPosition_Mark.cols;
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			if (TestCellPosition_Mark.at<int>(i, j) != 0)
			{
				empty = false;
			}
		}
	}
	return empty;
};
void writeMarkValue(const cv::Mat& mark,const std::string& name)
{
	int rows = mark.rows;
	int cols = mark.cols;
	int value = 0;
	int count = 0;
	std::ofstream output(name,std::ofstream::app);
	if (output.is_open() == 0)
	{
		std::cout << name << "It didn't open successfully!" << std::endl;
	}

	SYSTEMTIME sys;
	GetLocalTime(&sys);
	output << "**********************Start time**************************" << std::endl;
	output << "Date." << sys.wMonth<<"Month"<<sys.wDay<<"Day"<<"Time."<<sys.wHour << ":" << sys.wMinute << ":" << sys.wSecond << std::endl;
	
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			value = mark.at<int>(i, j);
			if (value != 0)
			{
				count++;
				output <<std::setw(6)<< "[" << i << " " << j << "]=" << value;
				if (count % 9 == 0)
					output << std::endl;
			}
		}
	}
	output << "**********************end**************************" << std::endl;
};
void writePicture(const cv::Mat& mark, const std::string& name)
{
	
	cv::imwrite(name, mark);
};
void writeCellPositionAndAttributeAndDirection(xlnt::worksheet& dataSheet, const cv::Point& position, 
	const int& direction, const int& attribution)
{
	std::string place = "[" + std::to_string(position.x) + " " + std::to_string(position.y) + "]";
	std::string dir_s = "dir:" + std::to_string(direction) + " ";
	std::string attr_s = "attr:" + std::to_string(attribution);
	std::string content = place + dir_s + attr_s;
	writeOrClearWrite(dataSheet, position, content);
	if (attribution == -1)
	{
		xlnt::font run_font;
		run_font.bold();
		run_font.color(xlnt::color::black());
		run_font.name("SimSun");
		run_font.size(9);
        dataSheet.cell(position.x, position.y).font(run_font);
	}
	else
	{
		xlnt::font run_font;
		run_font.color(xlnt::color::blue());
		run_font.name("SimSun");
		run_font.bold();
		run_font.size(9);
		dataSheet.cell(position.x, position.y).font(run_font);
	}
};
void writeCellPositionAndAttributeAndDirection_partial(xlnt::worksheet& dataSheet, const cv::Point& position, const int& direction,const int sequence)
{
	std::string place = "[" + std::to_string(position.x) + " " + std::to_string(position.y) + "]";
	std::string dir_s = "dir:" + std::to_string(direction) + " ";
	std::string seq_s = "seq:" + std::to_string(sequence);
	std::string content = place + dir_s+seq_s;
	writeOrClearWrite(dataSheet,  position, content);
	xlnt::font run_font;
	run_font.color(xlnt::color::green());
	run_font.name("SimSun");
	run_font.size(9);
	dataSheet.cell(position.x, position.y).font(run_font);
};
void writeCellPositionAndAttributeAndDirection_partial(xlnt::worksheet& dataSheet, const cv::Point& position,
	const int& direction, const int& attribution,const int sequence)
{
	std::string place = "[" + std::to_string(position.x) + " " + std::to_string(position.y) + "]";
	std::string dir_s = "dir:" + std::to_string(direction) + " ";
	std::string attr_s = "attr:" + std::to_string(attribution)+" ";
	std::string seq_s = "seq:" + std::to_string(sequence);
	std::string content = place + dir_s + attr_s+seq_s;
	writeOrClearWrite(dataSheet, position, content);
	if (attribution == -1)
	{
		xlnt::font run_font;
		run_font.bold();
		run_font.color(xlnt::color::black());
		run_font.name("SimSun");
		run_font.size(9);
		dataSheet.cell(position.x, position.y).font(run_font);
	}
	else
	{
		xlnt::font run_font;
		run_font.color(xlnt::color::blue());
		run_font.name("SimSun");
		run_font.bold();
		run_font.size(9);
		dataSheet.cell(position.x, position.y).font(run_font);
	}
};
void writeCellPositionAndAttributeAndDirection(xlnt::worksheet& dataSheet,
	const cv::Point& position,
	const int& direction, const int& attribution, const int sequence)
{
	std::string place = "[" + std::to_string(position.x) + " " + std::to_string(position.y) + "]";
	std::string dir_s = "dir:" + std::to_string(direction) + " ";
	std::string attr_s = "attr:" + std::to_string(attribution) + " ";
	std::string seq_s = "seq:" + std::to_string(sequence);
	std::string content = place + dir_s + attr_s + seq_s;
	writeOrClearWrite(dataSheet, position, content);

	if (attribution == -1)
	{
		xlnt::font run_font;
		run_font.bold();
		run_font.color(xlnt::color::black());
		run_font.name("SimSun");
		run_font.size(9);
		dataSheet.cell(position.x, position.y).font(run_font);
	}
	else
	{
		xlnt::font run_font;
		run_font.color(xlnt::color::blue());
		run_font.name("SimSun");
		run_font.bold();
		run_font.size(9);
		dataSheet.cell(position.x, position.y).font(run_font);
	}
}
void renewWorkSheet(const AdjustPosition& adPosition, xlnt::workbook& dataBook
	, xlnt::worksheet& dataSheet)
{
	std::string place = "[" + std::to_string(adPosition.position.x) + " " + std::to_string(adPosition.position.y) + "]";
	std::string attr_s = "attr:" + std::to_string(adPosition.attribution ) + " ";
	std::string content = place +attr_s ;
	if (adPosition.attribution == EMPTY && (dataSheet.cell(adPosition.position.x, adPosition.position.y).has_value()))
	{
	dataSheet.cell(adPosition.position.x, adPosition.position.y).clear_comment();
	}
	else if (adPosition.attribution == -1)
	{
		if (dataSheet.cell(adPosition.position.x, adPosition.position.y).has_value())
		{
			dataSheet.cell(adPosition.position.x, adPosition.position.y).clear_comment();
			dataSheet.cell(adPosition.position.x, adPosition.position.y).value(content);
		}
		else
		{
			dataSheet.cell(adPosition.position.x, adPosition.position.y).value(content);
		}
	};
	int attribution = adPosition.attribution;
	cv::Point position = adPosition.position;
	if (attribution == -1)
	{
		xlnt::font run_font;
		run_font.bold();
		run_font.color(xlnt::color::black());
		run_font.name("SimSun");
		run_font.size(9);
		dataSheet.cell(position.x, position.y).font(run_font);
	}
	else
	{
		xlnt::font run_font;
		run_font.color(xlnt::color::blue());
		run_font.name("SimSun");
		run_font.bold();
		run_font.size(9);
		dataSheet.cell(position.x, position.y).font(run_font);
	}
};
void writeOrClearWrite(xlnt::worksheet& dataSheet, const cv::Point& position, std::string content)
{
	if (dataSheet.cell(position.x, position.y).has_value())
	{
		dataSheet.cell(position.x, position.y).clear_comment();
		dataSheet.cell(position.x, position.y).value(content);
	}
	else
	{
		dataSheet.cell(position.x, position.y).value(content);
	}
};
void mergeTwoColorPicture()
{
	while (true)
	{
		std::string orig_picture_str, contour_picture_str;
		std::cout << "Please input original picture!" << std::endl;
		std::cin >> orig_picture_str;
		std::cout << "Please input contour picture!" << std::endl;
		std::cin >> contour_picture_str;
		cv::Mat orig_picture, contour_picture, merge_picture;
		orig_picture = cv::imread(orig_picture_str);
		contour_picture = cv::imread(contour_picture_str);
		std::string workFaddress = orig_picture_str;
		std::string pastWorkFolder;
		auto pos = workFaddress.find_last_of("\\//");
		workFaddress.erase(++pos);
		pastWorkFolder = workFaddress;
		workFaddress += "ori_merge_contour.JPG";
		int x_cols = min(orig_picture.cols, contour_picture.cols);
		int y_rows = min(orig_picture.rows, contour_picture.rows);
		merge_picture = cv::Mat::zeros(y_rows, x_cols, CV_8UC3);
		for (int x = 0; x < x_cols; x++)
		{
			for (int y = 0; y < y_rows; y++)
			{
				cv::Point pt(x, y);
				if (contour_picture.at<cv::Vec3b>(pt) == cv::Vec3b(255, 255, 255))
				{
					merge_picture.at<cv::Vec3b>(pt) = orig_picture.at<cv::Vec3b>(pt);
				}
				else
				{
					merge_picture.at<cv::Vec3b>(pt) = contour_picture.at<cv::Vec3b>(pt);
				}
			}
		}
		cv::imwrite(workFaddress, merge_picture);
	}

};
