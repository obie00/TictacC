#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "iostream"

#include <vector>
using namespace cv;
using namespace std;

bool findLine(Mat snippet) {
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
bool findCircle(Mat snippet) {
	vector<Vec3f> circles;

	HoughCircles(snippet, circles, CV_HOUGH_GRADIENT, 1, snippet.rows / 8, 200, 100, 0, 0);

	return false;
}
Mat makeMat(Mat board, int x, int y, int width, int length) {
	if (x < 0) {
		x = 0;
	}
	if (y < 0) {
		y = 0;
	}
	if ((x + width) > board.size().width) {
		width = board.size().width - x - 1;
	}
	if ((y + length) > board.size().height) {
		length = board.size().height - y - 1;
	}

	Mat cellM = Mat(board, cv::Rect(x, y, width, length));
	cvDestroyAllWindows();
	return cellM;
}
Rect2i makeCell(Mat board, int x, int y, int width, int length) {
	if (x < 0) {
		x = 0;
	}
	if (y < 0) {
		y = 0;
	}
	if ((x + width) > board.size().width) {
		width = board.size().width - x - 1;
	}
	if ((y + length) > board.size().height) {
		width = board.size().height - y - 1;
	}
	Rect2i cellM = Rect(x, y, width, length);
	cvDestroyAllWindows();
	return cellM;
}
Rect2i* getCells(Mat board, vector<vector<Point> > contours, vector<Point> scontours) {
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
	splitCells[0] = makeCell(board, l1, b1, l2 - l1, b2 - b1);
	splitCells[1] = makeCell(board, l2, b1, r2 - l2, b2 - b1);
	splitCells[2] = makeCell(board, r2, b1, r1 - r2, b2 - b1);
	splitCells[3] = makeCell(board, l1, b2, l2 - l1, t2 - b2);
	splitCells[4] = makeCell(board, l2, b2, r2 - l2, t2 - b2);
	splitCells[5] = makeCell(board, r2, b2, r1 - r2, t2 - b2);
	splitCells[6] = makeCell(board, l1, t2, l2 - l1, t1 - t2);
	splitCells[7] = makeCell(board, l2, t2, r2 - l2, t1 - t2);
	splitCells[8] = makeCell(board, r2, t2, r1 - r2, t1 - t2);


	return splitCells;
}

bool checkborders(Rect box, Mat snippet) {
	int x = box.x;
	int y = box.y;
	int height = box.height;
	int width = box.width;
	if (findLine(makeMat(snippet, x - 10, y - height - 10, 20, height)) == false) {
		return false;
	}
	else if (findLine(makeMat(snippet, x - width - 10, y - 10, width - 10, 20)) == false) {
		return false;
	}
	else if (findLine(makeMat(snippet, x + width - 10, y - height - 10, 20, height)) == false) {
		return false;
	}
	else if (findLine(makeMat(snippet, x + width + 10, y - 10, width, 20)) == false) {
		return false;
	}
	else if (findLine(makeMat(snippet, x - 10, y + height + 10, 20, height)) == false) {
		return false;
	}
	else if (findLine(makeMat(snippet, x - width - 10, y + height - 10, width - 10, 20)) == false) {
		return false;
	}
	else if (findLine(makeMat(snippet, x + width - 10, y + height + 10, 20, height)) == false) {
		return false;
	}
	 if (findLine(makeMat(snippet, x + width + 10, y + height - 10, width, 20)) == false) {
		return false;
	}
	return true;
}

void displayCells(Mat dst, Rect2i cells[9]) {
	cout << "cells are";
	for (int i = 0; i < 9; i++) {
		cout << cells[i] << "->";
		Mat thiscell = Mat(dst, cells[i]);
		imshow("thiscell", thiscell);
		cvWaitKey(0);
		cvDestroyAllWindows();
	}
}

Rect2i* findGrid(Mat src, Mat src_opt, vector<vector<Point> > contours, Rect2i* cells) {
	vector<Point> approx;
	for (int i = 0; i < contours.size(); i++)
	{
		approxPolyDP(cv::Mat(contours[i]), approx, cv::arcLength(cv::Mat(contours[i]), true)*0.02, true);
		if (std::fabs(cv::contourArea(contours[i])) < 100 || !cv::isContourConvex(approx)) {
			continue;
		}
		if (approx.size() == 4) {
			if (checkborders(boundingRect(contours[i]), src_opt) == true) {
				cout << "TTT exists";
				cells = getCells(src, contours, contours[i]);
				break;
			}
		}
	}
	return cells;
}

void drawContours(Mat src_opt, vector<vector<Point> > contours, vector<Vec4i> hierarchy) {
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

Mat optimizeImage(Mat src) {
	Mat src_opt;
	cvtColor(src, src_opt, CV_BGR2GRAY);
	blur(src_opt, src_opt, Size(3, 3));
	Canny(src_opt, src_opt, 80, 240, 3);
	imshow("src_opt", src_opt);
	cvWaitKey(0);
	cvDestroyAllWindows();

	return src_opt;
}

int main()
{
	bool isT = false;
	Mat src;
	Mat src_opt;
	Mat dst;
	Rect2i* cells = NULL;
	vector<Point> approx;
	vector<Vec4i> hierarchy;
	vector<vector<Point> > contours;
	src = imread("TestImages\\ti0.png");
	if (src.data == NULL) {
		cout << "image not found\n";
		system("pause");
		return 0;
	}
	src_opt = optimizeImage(src);

	findContours(src_opt.clone(), contours, hierarchy, CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE);

	src.copyTo(dst);
	
	cells = findGrid(src, src_opt, contours, cells);

	if (cells != NULL) {
		displayCells(dst, cells);
	}
	
	drawContours(src_opt, contours, hierarchy);
	
	cvDestroyAllWindows();
	return 0;
}