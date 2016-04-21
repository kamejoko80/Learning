#ifndef _CONTOUR_
#define _CONTOUR_
#include <iostream>
#include <opencv2/opencv.hpp>
#include "centroid.h"

using namespace std;
using namespace cv;

void contours(Mat m, double minarea, centroid& c)
{
	vector<vector<Point>>contours;
	vector<Vec4i>hierarchy;
	/*vector<float>centroids;    */
	Mat img1, img2;
	m.copyTo(img1);
	m.copyTo(img2);

	findContours(img1, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
	int n;
	for (n = 0; n < contours.size(); n++)
	{
		double tmparea = fabs(contourArea(contours[n]));
		if (tmparea < minarea)
		{
			contours.erase(contours.begin() + n);//删除面积小于设定值的轮廓
			n = n - 1;
		}
	}

	if (n >c.length)
	{
		for (n = 0; n < contours.size(); n++)
		{
			if (contourArea(contours[n])<1800)
			{
				Moments mom = moments(Mat(contours[n]));
				double x = mom.m10 / mom.m00;
				double y = mom.m01 / mom.m00;
				c.add(x, y);
			}
		}
	}

	int k;
	double contour_area = 0;
	for (k = 0; k < contours.size(); k++)
	{
		Scalar color = Scalar(255, 255, 255);

		drawContours(img2, contours, k, color, 1, 8, hierarchy, 0, Point());
		contour_area = contour_area + fabs(contourArea(contours[k]));
	}
	/*namedWindow("contour");
	imshow("contour", img2);*/
	/*return contours.size();*/
}
#endif