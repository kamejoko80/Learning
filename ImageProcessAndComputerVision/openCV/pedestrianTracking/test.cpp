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