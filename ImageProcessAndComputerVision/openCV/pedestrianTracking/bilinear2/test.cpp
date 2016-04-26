#include <opencv2/opencv.hpp>
#include <iostream>
#include "process.h"
#include "initialization.h"
using namespace std;
using namespace cv;

int main()
{
	Mat x_left, y_left;

	string file1 = "x_left.yml";
	FileStorage fs(file1, 0);
	fs["Xmapl"] >> x_left;//x_left CV_32FC1
	fs.release();

	string file2 = "y_left.yml";
	FileStorage fs1(file2, 0);
	fs1["Ymapl"] >> y_left;
	fs1.release();

	int cols = x_left.cols;
	int rows = x_left.rows;

	Mat ImgSrc = imread("left1.png", 1);//ImgSrc CV_8UC3
	Mat ImgDst = Mat(Size(cols, rows), ImgSrc.type(), Scalar::all(0));

	int StepSrc = ImgSrc.step;
	int channels = ImgSrc.channels();
	int num = cols * rows;

	short* u_v = new short[num];
	short* u_v_16 = new short[num];
	short* u_16_v= new short[num];
	short* u_16_v_16 = new short[num];
	int* pos[4];
    
    for (int i = 0; i < 4; i++)
	{
		pos[i] = new int[num];
	}
	
    initialize(u_v, u_v_16, u_16_v, u_16_v_16, pos, x_left, y_left, StepSrc, channels);
        
	process(u_v, u_v_16, u_16_v, u_16_v_16, pos, ImgSrc, ImgDst, 4);
         
         
	delete []u_v;
	delete []u_v_16;
	delete []u_16_v;
	delete []u_16_v_16;
	
    for(int i = 0;i < 4;i++)
	{
		delete []pos[i];
	}
	
    namedWindow("Src",1);	
	imshow("Src", ImgSrc);
	namedWindow("Dst",1);	
	imshow("Dst", ImgDst);
	waitKey();

	return 0;
}
