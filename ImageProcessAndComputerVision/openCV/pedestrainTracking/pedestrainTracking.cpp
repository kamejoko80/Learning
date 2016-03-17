
#include "stdafx.h"

#include <opencv2/opencv.hpp>
#include <iostream>

// to avoid the warning about the function fopen
//#pragma warning(disable: 4996)

using namespace std;
using namespace cv;

void compute(InputArray leftarr, InputArray rightarr, OutputArray disparr);
void test_show_disparity(Mat disp);

//void saveDisp(const char* filename, const Mat& mat) {
//
//	FILE* fp = fopen(filename, "wt");
//
//	for (int y = 0; y < mat.rows; y++)
//	{
//		for (int x = 0; x < mat.cols; x++)
//		{
//			// The disparity matrix is in CV_16S format, so read it as short type
//			short disp = mat.at<short>(y, x);
//			// if the disparity matrix is in CV_32F format, read it as float type
//			fprintf(fp, "%d ", disp);
//		}
//		fprintf(fp, "\n");
//	}
//	fclose(fp);
//}

int main()
{
	Mat img1, img2, disp;
	for (int i = 1; i <= 203; i++)
	{
		stringstream str_in1, str_in2;
		str_in1 << "h:\\CODING\\data\\pedestrainTrackingFrame\\left" << i << ".png";
		str_in2 << "h:\\CODING\\data\\pedestrainTrackingFrame\\right" << i << ".png";
		img1 = imread(str_in1.str(), 1);
		img2 = imread(str_in2.str(), 1);
		
		int64 t = getTickCount();
		compute(img1, img2, disp);
		t = getTickCount() - t;
		

		test_show_disparity(disp);


		// compute the depth
		//Scalar::all(0): initialize as all 0 matrix
		//Mat depth = Mat(Size(disp.rows, disp.cols), CV_32FC1, Scalar::all(0));

		//for (int j = 0; j < disp.rows * disp.cols; j++)
		//{
		//	float a = abs(*(disp.data + j) / 16);
		//	*(depth.data + j) = 0.119 * 211 /a;
		//}
		//cout << float(*(depth.data + 10));


		// save the disparity data in TXT files
		//stringstream str_out;
		//str_out << "disp" << i << ".txt";
		//string s = string(str_out.str());
		//saveDisp(s.c_str(), disp);

		cout << "Run time(s):" << t / getTickFrequency() << endl;
		waitKey(1);
	}
	return 0;
}

void test_show_disparity(Mat disp)
{
	Mat disp8;
	namedWindow("disparity", 1);
	normalize(disp, disp8, 0, 255, CV_MINMAX, CV_8U);
	imshow("disparity", disp8);
}