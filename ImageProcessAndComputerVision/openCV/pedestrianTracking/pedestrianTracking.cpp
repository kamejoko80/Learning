
#include "stdafx.h"

#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

void compute(InputArray leftarr, InputArray rightarr, OutputArray disparr);
void disp_fg_extract(Mat& disp, Mat& bg);

void test_show_disparity(Mat disp);

int main()
{
	Mat img1, img2, disp;
	Mat bg;
	for (int i = 1; i <= 203; i++)
	{
		stringstream str_in1, str_in2;
		str_in1 << "h:\\CODING\\data\\pedestrainTrackingFrame\\left" << i << ".png";
		str_in2 << "h:\\CODING\\data\\pedestrainTrackingFrame\\right" << i << ".png";
		img1 = imread(str_in1.str(), 1);
		img2 = imread(str_in2.str(), 1);
		
		
		// compute the disparity between img1 and img2
		compute(img1, img2, disp);
		disp = abs(disp);

		//// compute the depth
		//Mat depth;
		//disp.convertTo(depth, CV_32FC3);
		//// depth = 0.119 * 211 / (disp / 16)
		//depth = 0.119 * 211.0 * 16.0 / depth;

		disp.convertTo(disp, CV_8UC1);

		//test_show_disparity(disp);
		
		if (i == 1)
		{
			disp.copyTo(bg);
		}
		else
		{
			disp_fg_extract(disp, bg);
		}

	} // end of frames, for (int i = 1; i <= 203; i++)

	return 0;
}

