#include "ImageManager.h"
#include "cstdlib"

using namespace std;

ImageManager::ImageManager(Mat source)
{
	src = source;
	src_opt = optimizeImage(src);
	findContours(src_opt.clone(), contours, hierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);
	src.copyTo(dst);
	findGrid();
	if (cells != NULL) {
		//displayCells();
	}
	else {
		cout << "can't find board";
	}
}


ImageManager::~ImageManager()
{
	cvDestroyAllWindows();
}

void ImageManager::ContinueGame(Mat source) {

}

bool ImageManager::findLine(Mat snippet) {
	Mat dst, color_dst;
	dst = snippet.clone();
	cvtColor(dst, color_dst, COLOR_GRAY2BGR);
	Mat cdstP = snippet.clone();
	vector<Vec4i> lines;
	HoughLinesP(dst, lines, 1, CV_PI / 180, 30, 30, 10);
	for (size_t i = 0; i < lines.size(); i++)
	{
		line(color_dst, Point(lines[i][0], lines[i][1]),
			Point(lines[i][2], lines[i][3]), Scalar(0, 0, 255), 3, 8);
	}
	if (lines.size() != 0) {
		return true;
	}
	else {
		return false;
	}
}
bool ImageManager::findCircle(Mat snippet) {
	vector<Vec3f> circles;
	HoughCircles(snippet, circles, CV_HOUGH_GRADIENT, 1, snippet.rows / 2, 100, 60, 0, 0);

	return false;
}
Mat ImageManager::makeMat(int x, int y, int width, int length) {
	if (x < 0) {
		x = 0;
	}
	if (y < 0) {
		y = 0;
	}
	if ((x + width) > src_opt.size().width) {
		width = src_opt.size().width - x - 1;
	}
	if ((y + length) > src_opt.size().height) {
		length = src_opt.size().height - y - 1;
	}

	Mat cellM = Mat(src_opt, cv::Rect(x, y, width, length));
	cvDestroyAllWindows();
	return cellM;
}
Rect2i ImageManager::makeCell(int x, int y, int width, int length) {
	if (x < 0) {
		x = 0;
	}
	if (y < 0) {
		y = 0;
	}
	if ((x + width) > src.size().width) {
		width = src.size().width - x - 1;
	}
	if ((y + length) > src.size().height) {
		width = src.size().height - y - 1;
	}
	Rect2i cellM = Rect(x, y, width, length);
	cvDestroyAllWindows();
	return cellM;
}
Rect2i* ImageManager::getCells(vector<Point> scontours) {
	int t1 = 0, r1 = 0, b1 = 1000, l1 = 1000, t2 = 0, r2 = 0, b2 = 1000, l2 = 1000;

	for (int i = 0; i < contours.size(); i++) {
		for (int j = 0; j < contours[i].size(); j++) {
			if (contours[i][j].y > t1) {
				t1 = contours[i][j].y;
			}
			if (contours[i][j].x > r1) {
				r1 = contours[i][j].x;
			}
			if (contours[i][j].y < b1) {
				b1 = contours[i][j].y;
			}
			if (contours[i][j].x < l1) {
				l1 = contours[i][j].x;
			}

		}
	}
	for (int i = 0; i < scontours.size(); i++) {
		if (scontours[i].y > t2) {
			t2 = scontours[i].y;
		}
		if (scontours[i].x > r2) {
			r2 = scontours[i].x;
		}
		if (scontours[i].y < b2) {
			b2 = scontours[i].y;
		}
		if (scontours[i].x < l2) {
			l2 = scontours[i].x;
		}
	}
	static Rect2i splitCells[9];
	splitCells[0] = makeCell(l1, b1, l2 - l1, b2 - b1);
	splitCells[1] = makeCell(l2, b1, r2 - l2, b2 - b1);
	splitCells[2] = makeCell(r2, b1, r1 - r2, b2 - b1);
	splitCells[3] = makeCell(l1, b2, l2 - l1, t2 - b2);
	splitCells[4] = makeCell(l2, b2, r2 - l2, t2 - b2);
	splitCells[5] = makeCell(r2, b2, r1 - r2, t2 - b2);
	splitCells[6] = makeCell(l1, t2, l2 - l1, t1 - t2);
	splitCells[7] = makeCell(l2, t2, r2 - l2, t1 - t2);
	splitCells[8] = makeCell(r2, t2, r1 - r2, t1 - t2);


	return splitCells;
}

bool ImageManager::checkborders(Rect box) {
	int x = box.x;
	int y = box.y;
	int height = box.height;
	int width = box.width;
	if (findLine(makeMat(x - 10, y - height - 10, 20, height)) == false) {
		return false;
	}
	else if (findLine(makeMat(x - width - 10, y - 10, width - 10, 20)) == false) {
		return false;
	}
	else if (findLine(makeMat(x + width - 10, y - height - 10, 20, height)) == false) {
		return false;
	}
	else if (findLine(makeMat(x + width + 10, y - 10, width, 20)) == false) {
		return false;
	}
	else if (findLine(makeMat(x - 10, y + height + 10, 20, height)) == false) {
		return false;
	}
	else if (findLine(makeMat(x - width - 10, y + height - 10, width - 10, 20)) == false) {
		return false;
	}
	else if (findLine(makeMat(x + width - 10, y + height + 10, 20, height)) == false) {
		return false;
	}
	if (findLine(makeMat(x + width + 10, y + height - 10, width, 20)) == false) {
		return false;
	}
	return true;
}

void ImageManager::displayCells() {
	cout << "cells are";
	for (int i = 0; i < 9; i++) {
		cout << cells[i] << "->";
		Mat thiscell = Mat(dst, cells[i]);
		imshow("thiscell", thiscell);
		cvWaitKey(0);
		cvDestroyAllWindows();
	}
}

void ImageManager::findGrid() {
	vector<Point> approx;
	for (int i = 0; i < contours.size(); i++)
	{
		approxPolyDP(cv::Mat(contours[i]), approx, cv::arcLength(cv::Mat(contours[i]), true)*0.02, true);
		if (std::fabs(cv::contourArea(contours[i])) < 100 || !cv::isContourConvex(approx)) {
			continue;
		}
		if (approx.size() == 4) {
			if (checkborders(boundingRect(contours[i])) == true) {
				cout << "TTT exists";
				cells = getCells(contours[i]);
				break;
			}
		}
	}
}

void ImageManager::showContours() {
	Scalar color = Scalar(200, 200, 200);

	Mat drawing = Mat::zeros(src_opt.size(), CV_8UC3);
	for (size_t i = 0; i < contours.size(); i++)
	{
		Scalar color(rand() & 255, rand() & 255, rand() & 255);
		drawContours(drawing, contours, (int)i, color, 2, 8, hierarchy, 0, Point());
	}
	imshow("drawing", drawing);
	cvWaitKey(0);
	cvDestroyAllWindows();
}

Mat ImageManager::optimizeImage(Mat src) {
	Mat src_opt;
	cvtColor(src, src_opt, CV_BGR2GRAY);
	blur(src_opt, src_opt, Size(3, 3));
	Canny(src_opt, src_opt, 80, 240, 3);
	cvDestroyAllWindows();

	return src_opt;
}

bool ImageManager::isX(Mat snippet) {
	vector<Point> snipApprox;
	vector<Vec4i> snipHierarchy;
	vector<vector<Point> > snipContours;
	snippet = optimizeImage(snippet);
	findContours(snippet.clone(), snipContours, snipHierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);
	for (int i = 0; i < snipContours.size(); i++)
	{
		approxPolyDP(cv::Mat(snipContours[i]), snipApprox, cv::arcLength(cv::Mat(snipContours[i]), true)*0.02, true);
		if (snipApprox.size() == 8) {
			return true;
		}
		else {
			return false;
		}
	}
	return true;
}

bool ImageManager::isO(Mat snippet) {
	vector<Vec3f> circles;
	snippet = optimizeImage(snippet);
	HoughCircles(snippet, circles, CV_HOUGH_GRADIENT, 1, snippet.rows / 8, 200, 40, 0, 0);
	if (circles.size() == 0) {
		return false;
	}
	else {
		return true;
	}
}

playerOptions ImageManager::detectImage(int cell) {
	/*cout << "Enter a value for cell[" << cell << "]: ";
	cin >> cell;
	cout << "\n";
	if (cell == 1) {
		return X;
	}
	else if (cell == 2) {
		return O;
	}
	return E;*/
	
	Mat snippet = Mat(dst, cells[cell]);
	if (isX(snippet)) {
		return X;
	}
	else if (isO(snippet)) {
		return O;
	}
	return E;
}
void ImageManager::compareGrids() {
	//for(int i = 0; i < TTT.){}
}