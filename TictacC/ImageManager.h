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
	vector<vector<Point> > contours;

	void findGrid();
	void showContours();
	void displayCells();
	playerOptions detectImage(int cell);
	void displayOutputImg();

	ImageManager(Mat source);
	void ContinueGame(Mat source);
	~ImageManager();

private:
	Point imgCenter;//Point(w / 2.0, w / 2.0);
	bool findLine(Mat snippet);
	bool findCircle(Mat snippet);
	Mat makeMat(int x, int y, int width, int length);
	Rect2i makeCell(int x, int y, int width, int length);
	Rect2i* getCells(vector<Point> scontours);
	bool checkborders(Rect box);
	Mat optimizeImage(Mat src);
	bool isX(Mat snippet);
	bool isO(Mat snippet);
	void compareGrids();
};

