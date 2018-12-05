#include "ImageManager.h"
#include "cstdlib"

using namespace std;

ImageManager::ImageManager(Mat source)
{
	src = source;
	src_opt = optimizeImage(src);

	findContours(src_opt.clone(), contours, hierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);

	src.copyTo(dst);
	findGrid(src_opt);

	if (cells != NULL) {
		//displayCells();
		cout << "Found Board!" << endl << endl;
	}
	else {
		cout << "Can't find board!" << endl << endl;
	}
}


ImageManager::~ImageManager()
{
	//cvDestroyAllWindows();
}

void ImageManager::ContinueGame(Mat source) {

}

bool ImageManager::findLine(Mat snippet) {

	if (snippet.rows == 0 || snippet.cols == 0) {
		return false;
	}

	Mat dst = snippet.clone();
	Mat t_dst = snippet.clone();

	vector<Vec4i> lines;

	Canny(dst, t_dst, 50, 150);

	HoughLinesP(t_dst, lines, 1.0, CV_PI / 180, 30, 30, 20);

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

	// If the x and y coordinates are negative, set them to zero.
	if (x < 0) {
		x = 0;
	}
	if (y < 0) {
		y = 0;
	}

	if (width < 0) {
		width = 0;
	}
	if (length < 0) {
		length = 0;
	}

	if ((x + width) > src_opt.size().width) {
		width = src_opt.size().width - x - 1;
	}
	if ((y + length) > src_opt.size().height) {
		length = src_opt.size().height - y - 1;
	}

	Mat cellM = Mat(src_opt, cv::Rect(x, y, width, length));
	//cvDestroyAllWindows();

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

	// Top left corner coordinates of box.
	int x = box.x;
	int y = box.y;

	// Height and width of box.
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
	else if (findLine(makeMat(x + width + 10, y + height - 10, width, 20)) == false) {
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

void ImageManager::findGrid(Mat opt_src) {

	vector<Point> approx;

	for (int i = 0; i < contours.size(); i++)
	{
		approxPolyDP(Mat(contours[i]), approx, cv::arcLength(cv::Mat(contours[i]), true) * 0.1, true);

		if (std::fabs(contourArea(contours[i])) < 2500 || !cv::isContourConvex(approx)) {
			continue;
		}

		if (approx.size() == 4) {

			if (checkborders(boundingRect(contours[i])) == true) {
				foundBoard = true;
				cout << "TTT Exists!" << endl;

				cells = getCells(contours[i]);

				break;

			}
			else {
				foundBoard = false;
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
	//cvDestroyAllWindows();
}

Mat ImageManager::optimizeImage(Mat src) {
	Mat src_opt;
	Mat kernel = (Mat_<int>(3, 3) << 1, 1, 1, 1, 1, 1, 1, 1, 1);

	cvtColor(src, src_opt, CV_BGR2GRAY);
	blur(src_opt, src_opt, Size(3, 3));

	Canny(src_opt, src_opt, 50, 150, 3);

	// Removes potential holes that can be found inside lines.
	dilate(src_opt, src_opt, kernel, Point(-1, -1), 5);
	erode(src_opt, src_opt, kernel, Point(-1, -1), 5);

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

		bool notConvex = !isContourConvex(snipApprox);
		bool notSmall = (fabs(contourArea(snipContours[i])) < 100);

		if (snipApprox.size() == 8) {
			return true;
		}
	}

	return false;
}

bool ImageManager::isO(Mat snippet) {

	Mat c_snippet = optimizeImage(snippet.clone());
	vector<vector<Point> > snipContours;
	vector<Vec4i> snipHierarchy;
	Mat drawSnipContours = Mat::zeros(c_snippet.size(), CV_8UC3);
	vector<Point> approx;

	findContours(c_snippet.clone(), snipContours, snipHierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

	for (int i = 0; i < snipContours.size(); i++)
	{
		approxPolyDP(Mat(snipContours[i]), approx, arcLength(Mat(snipContours[i]), true)*0.01, true);

		if (!isContourConvex(approx) || (fabs(contourArea(snipContours[i])) < 100)) {
			continue;
		}

		double area = cv::contourArea(snipContours[i]);
		cv::Rect r = cv::boundingRect(snipContours[i]);
		int radius = r.width / 2;

		double first = abs(1 - ((double)r.width / r.height));
		double second = abs(1 - (area / (CV_PI * (radius*radius))));

		if (first <= 0.4 && second <= 0.4) {
			return true;
		}
	}

	return false;
}

playerOptions ImageManager::detectImage(int cell) {

	Mat snippet = Mat(dst, cells[cell]);

	if (isO(snippet)) {
		cout << "Test O" << endl;
		return O;
	}
	else if (isX(snippet)) {
		cout << "Test X" << endl;
		return X;
	}

	return E;
}

Mat ImageManager::getOutputImg(int chosen_cell, playerOptions cpu_symbol) {
	outputImg = src.clone();
	Mat roi; // Region of interest: region where the X or O may be drawn on.

	roi = Mat(outputImg, cells[chosen_cell]);

	if (cpu_symbol == X) {
		roi = drawX(roi);
	}
	else if (cpu_symbol == O) {
		roi = drawCircle(roi);
	}

	return outputImg;
}

Mat ImageManager::drawCircle(Mat snippet) {

	int length = snippet.rows;
	int width = snippet.cols;
	int radius = 0;

	if (width < length) {
		radius = (width - 40) / 2;
	}
	else {
		radius = (length - 40) / 2;
	}

	// Find the center point of the circle.
	width = width / 2;
	length = length / 2;

	circle(snippet, Point(width, length), radius, Scalar(0, 0, 255), 3);

	return snippet;
}

Mat ImageManager::drawX(Mat snippet) {

	int length = snippet.rows;
	int width = snippet.cols;
	int c_length = 0;
	int c_width = 0;

	// Corners of the X.
	Point tl_corner;
	Point br_corner;
	Point bl_corner;
	Point tr_corner;

	// Find the center point of the snippet. Will be used
	// to find the corner points of the X.
	c_width = width / 2;
	c_length = length / 2;

	length = (length / 2) - 40;
	width = (width / 2) - 40;

	// Make length == width so the drawn X is symmetrical.
	if (length < width) {
		width = length;
	}
	else {
		length = width;
	}

	tl_corner = Point(c_width - width, c_length + length);
	br_corner = Point(c_width + width, c_length - length);

	line(snippet, tl_corner, br_corner, Scalar(0, 0, 255), 3);

	tr_corner = Point(c_width + width, c_length + length);
	bl_corner = Point(c_width - width, c_length - length);

	line(snippet, tr_corner, bl_corner, Scalar(0, 0, 255), 3);

	return snippet;
}

void ImageManager::compareGrids() {
	//for(int i = 0; i < TTT.){}
}