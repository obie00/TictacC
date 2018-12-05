#pragma once
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "iostream"
#include <vector>
#include "GameManager.h"

using namespace cv;
using namespace std;

class ImageManager
{
public:
	bool foundBoard = false;
	Mat src;
	Mat src_opt;
	Mat dst;
	Mat outputImg;

	Rect2i* cells = NULL;
	vector<Point> approx;
	vector<Vec4i> hierarchy;
	vector<vector<Point>> contours;

	void findGrid(Mat opt_src);
	void showContours();
	void displayCells();
	Mat getOutputImg(int chosen_cell, playerOptions cpu_symbol);

	playerOptions detectImage(int cell);

	ImageManager(Mat source);
	void ContinueGame(Mat source);
	~ImageManager();

private:
	Point imgCenter;
	bool findLine(Mat snippet);
	bool findCircle(Mat snippet);
	Mat makeMat(int x, int y, int width, int length);
	Rect2i makeCell(int x, int y, int width, int length);
	Rect2i* getCells(vector<Point> scontours);
	bool checkborders(Rect box);
	bool isX(Mat snippet);
	bool isO(Mat snippet);
	Mat optimizeImage(Mat src);
	Mat drawCircle(Mat snippet);
	Mat drawX(Mat snippet);
	void compareGrids();
};