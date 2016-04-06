#include "stdafx.h"
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

void test_show_contours(vector<vector<Point>> contours, vector<Vec4i> hierarchy, Mat bg_img);
void test_show_fg(Mat fg);

void disp_fg_extract(Mat& disp, Mat& bg)
{
	Mat diff, fg;

	absdiff(disp, bg, diff);

	int64 t = getTickCount();

    threshold(diff, diff, 30, 255, THRESH_TOZERO);

	normalize(diff, fg, 0, 255, CV_MINMAX, CV_8U);
	// test_show_fg(fg);

	vector<vector<Point>>contours;
	vector<Vec4i>hierarchy;
	findContours(fg, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
	double minarea = 650;

	for (int m = 0; m < contours.size(); m++)
	{
		double tmparea = fabs(contourArea(contours[m]));
		if (tmparea < minarea)
		{
			//delete the contours whose area less than minarea
			contours.erase(contours.begin() + m);
			m = m - 1;
		}
	}

	t = getTickCount() - t;
	cout << "Run time(ms):" << t / getTickFrequency() * 1000 << endl;

	test_show_contours(contours, hierarchy, disp);
	
}
