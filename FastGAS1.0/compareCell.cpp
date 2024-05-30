#pragma once
#include"SandSediment.h"
#include"compareCell.h"
void CompareCellColor(const cv::Mat& colorPicture, Color& sandColor, const Cell& tempCell,
	IsSandPlaque& isSandPlaque,const cv::Mat&bourdaryMark,const cv::Mat&outline)
{   
	cv::Point position = tempCell.position;
	int value = (int)outline.at<uchar>(position);
	if (value == EMPTY)
	{
		isSandPlaque.isColorPlaque = false;
		isSandPlaque.isSandPlaque = false;
		return;
	}
	else if (comparePixelColor(colorPicture, sandColor, tempCell))
	{
		isSandPlaque.isColorPlaque=true;
		isSandPlaque.isSandPlaque = true;
		return;
	}
	else
	{
		
		if (eightNeighborHasBourdary(bourdaryMark,tempCell.position))
		{
			isSandPlaque.isColorPlaque = false;
			isSandPlaque.isSandPlaque = false;
			
		}
		int count = 0;
		std::vector<Cell> eightNeightorSet;
		get8NeighborCell( colorPicture
			,  eightNeightorSet, tempCell.position);
		for (int i = 0; i < eightNeightorSet.size(); i++)
		{
			if (comparePixelColor(colorPicture, sandColor, eightNeightorSet[i]))
			{
			count++;
			}
		}
		if (count >= SIMILAR_COLOR_NUMBER)
		{
			isSandPlaque.isColorPlaque=true;
			isSandPlaque.isSandPlaque = true;
			return;
		}
		else if (count >= SIMILAR_SAND_NUMBER)
		{
			isSandPlaque.isSandPlaque = true;
			isSandPlaque.isColorPlaque = false;
		}
		else
		{
			isSandPlaque.isColorPlaque = false;
			isSandPlaque.isSandPlaque = false;
			return;
		}
	}
};
void adjustColor(Color& color, const Cell& cell)
{
	for (int i = 0; i < CELL_NUMBER; i++)
	{
		for (int j = 0; j < COLOR_CHANNELS; j++)
		{

			color.maxColor[j] = max(color.maxColor[j],
				cell.cell.at<cv::Vec3b>(Cell_NEIGHBOR_COORDINATE[i])[j]);
			color.minColor[j] = min(color.minColor[j], 
				cell.cell.at<cv::Vec3b>(Cell_NEIGHBOR_COORDINATE[i])[j]);
		}
	}
};
void adjustColor(Color& color, const std::vector<Cell>& cellSet)
{
	int count = cellSet.size();
	for (int i = 0; i < count; i++)
	{
		adjustColor(color, cellSet[i]);
	}
};
uchar max(const uchar& lhs, const uchar& rhs)
{
	if (lhs > rhs)
		return lhs;
	else
		return rhs;
};
uchar min(const uchar& lhs, const uchar& rhs)
{
	if (lhs < rhs)
		return lhs;
	else
		return rhs;
};
bool comparePixelColor(const cv::Mat& colorPicture, Color& sandColor, const Cell& tempCell)
{
	bool isSandCell = false;
	int count = 0;
	cv::Point pt = tempCell.position;
	for (int i = 0; i < CELL_NUMBER; i++)
	{
		Scalar_Color cellColor =
			colorPicture.at<cv::Vec3b>(cv::Point(pt.x + EIGHT_NEIGHBOR_CELL[i].x, pt.y + EIGHT_NEIGHBOR_CELL[i].y));
		if (
			(sandColor.minColor[0] <= cellColor[0] && cellColor[0] <= sandColor.maxColor[0]) &&
			(sandColor.minColor[1] <= cellColor[1] && cellColor[1] <= sandColor.maxColor[1]) &&
			(sandColor.minColor[2] <= cellColor[2] && cellColor[2] <= sandColor.maxColor[2])
			)
		{
			count++;
		}
	}
	if (count >= CELL_NUMBER)
		return true;
	else
		return false;
};
void get4Neighbor(const cv::Mat& picture, std::vector<Cell>& sCell, const cv::Point position)
{

	for (int i = 0; i < FOUR_NEIGHBOR; i++)
	{
		cv::Point newPoint;
		newPoint.x = position.x + 3 * (2 - i - 1) % 2;
		newPoint.y = position.y + 3 * (2 - i) % 2;
		Cell newCell = fromPointToCell(picture, newPoint);
		sCell.push_back(newCell);
	}
};
void setColor(Color& sandColor, const Cell& cell)
{
	for (int k = 0; k < COLOR_CHANNELS; k++)
	{
		sandColor.maxColor[k] =
			cell.cell.at<cv::Vec3b>(Cell_NEIGHBOR_COORDINATE[0])[k];
		sandColor.minColor[k] =
			cell.cell.at<cv::Vec3b>(Cell_NEIGHBOR_COORDINATE[0])[k];
	}
};
void get4NeighborCell(const cv::Mat& colorPicture
	, std::vector<Cell>& fourNeightorSet, cv::Point position)
{
	for (int i = 0; i < FOUR_NEIGHBOR; i++)
	{
		cv::Point newPoint;
		newPoint.x = position.x + FOUR_CELL_NEIGHBOR[i].x;
		newPoint.y = position.y + FOUR_CELL_NEIGHBOR[i].y;
		Cell newCell = fromPointToCell(colorPicture, newPoint);
		fourNeightorSet.push_back(newCell);
	}
};
void get8NeighborCell(const cv::Mat& colorPicture
	, std::vector<Cell>& eightNeightorSet, cv::Point position)
{
	for (int i = 0; i < EIGHT_NEIGHBOR; i++)
	{
		cv::Point newPoint;
		newPoint.x = position.x + EIGHT_CELL_NEIGHBOR[i].x;
		newPoint.y = position.y + EIGHT_CELL_NEIGHBOR[i].y;
		Cell newCell = fromPointToCell(colorPicture, newPoint);
		eightNeightorSet.push_back(newCell);
	}

};
bool eightNeighborHasBourdary(const cv::Mat& bourdaryMark, const cv::Point& position)
{
	bool result=false;
	cv::Point newPosition = position;
	for (int i = 0; i < COMPARE_EIGHT_NEIGHBOR; i++)
	{
		newPosition.x += COMPARE_EIGHT_CELL_NEIGHBOR[i].x;
		newPosition.y += COMPARE_EIGHT_CELL_NEIGHBOR[i].y;
		if (bourdaryMark.at<int>(newPosition) == -1)
		{
			result = true;
			break;
		}
	}
	return result;
};
