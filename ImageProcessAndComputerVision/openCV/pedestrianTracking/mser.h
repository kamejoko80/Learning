#ifndef _MSER_
#define _MSER_
#include <iostream>
#include <opencv2/opencv.hpp>
#include "centroid.h"
#include "contour.h"

using namespace std;
using namespace cv;

#define MAX_ITERATOR 9
#define THRES_STEP 7
centroid mser(Mat m, int t, int init_area)
{
	centroid c;
	int cur_num_now, num0=0, num1=0, num2=0, num3=0, num4=0, count=1;
	for (int th = t; th <= 240; th = th + THRES_STEP)
	{
		threshold(m, m, th, 255, THRESH_TOZERO);
		contours(m, 100,c);
		/*waitKey(1000);*/
		if (c.length >= count)//每找到一个人，记录其质心
		{
			count = count + (c.length - count);
		}
	}
	return c;
}

#endif