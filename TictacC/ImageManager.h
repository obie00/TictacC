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
	bool isT = false;
	Mat src;
	Mat src_opt;
	Mat dst;
	Rect2i* cells = NULL;
	vector<Point> approx;
	vector<Vec4i> hierarchy;
	vector<vector<Point> > contours;

	void findGrid();
	void showContours();
	void displayCells();
	playerOptions detectImage(int cell);

	ImageManager(char file[40]);
	~ImageManager();

private:
	bool findLine(Mat snippet);
	bool findCircle(Mat snippet);
	Mat makeMat(int x, int y, int width, int length);
	Rect2i makeCell(int x, int y, int width, int length);
	Rect2i* getCells(vector<Point> scontours);
	bool checkborders(Rect box);
	Mat optimizeImage();
	void compareGrids();
};

