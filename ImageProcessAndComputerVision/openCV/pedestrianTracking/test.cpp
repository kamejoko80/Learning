#include "stdafx.h"
#include<opencv2/opencv.hpp>
#include <iostream>
using namespace cv;
using namespace std;

void test_show_disparity(Mat disp)
{
	Mat disp8;
	namedWindow("disparity", 1);
	normalize(disp, disp8, 0, 255, CV_MINMAX, CV_8U);
	imshow("disparity", disp8);
}

void test_show_fg(Mat fg)
{
	namedWindow("disparity", 1);
	imshow("disparity", fg);
}

void test_show_contours(vector<vector<Point>> contours, vector<Vec4i> hierarchy, Mat bg_img)
{
	for (int m = 0; m < contours.size(); m++)
	{
		Scalar color = Scalar(0, 0, 255);
		drawContours(bg_img, contours, m, color, 2, 8, hierarchy, 0, Point());
	
		Rect roi = boundingRect(contours[m]);
		rectangle(bg_img, roi, (255, 255, 255), 3);
	}
	cvNamedWindow("fg_contours");
	imshow("fg_contours", bg_img);

	cvWaitKey(10);
}




void test_mat_operation()
{
	//Mat a(3, 5, CV_32FC1, Scalar(1));
	Mat a;
	Mat b(3, 5, CV_8UC1, Scalar(3));
	b.convertTo(a, CV_32FC1);
	a = 2.0 / a;
	cout << a;
	cout << b;
}