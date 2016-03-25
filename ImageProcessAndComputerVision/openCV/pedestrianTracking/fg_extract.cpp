#include<iostream>
#include<opencv2/opencv.hpp>

using namespace std;
using namespace cv;

void extract();
Mat cur_frame = imread("right0.png");

int height = cur_frame.rows;
int width = cur_frame.cols;
double diff;
double var;
char filename[20];
Mat fg(height, width, CV_8UC1);
Mat frame_gray(height, width, CV_8UC1);
Mat cur_bg1(height, width, CV_8UC1);
Mat cur_bg2(height, width, CV_8UC1, Scalar(20));

int main()
{
	cvtColor(cur_frame, cur_bg1, CV_BGR2GRAY);
	
	for (int k = 1; k <= 203 ;k++)
	{
		sprintf_s(filename,"right%d.png",k);
	    cur_frame = imread(filename);
		cvtColor(cur_frame, frame_gray, CV_BGR2GRAY);
	
		extract();
		waitKey(50);
	}
	waitKey(0);
}


void extract()
{
	
	int defaultVarTh = 3;	// default standard deviation threshold
	int defaultVar = 20;	// guess it
	int alpha = 0.05;		// background update frequency
	
	//foreground extract
	double mean;
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			mean = cur_bg1.at<uchar>(i, j);
			diff = double(frame_gray.at<uchar>(i, j)) - mean;
			if (abs(diff) <= defaultVarTh * defaultVar)
			{
				fg.at<uchar>(i, j) = 0;
				
				//Vec3b pixel;
				//pixel[0] = 0;
				//pixel[1] = 0;
				//pixel[2] = 0;
				//fg_color.at<Vec3b>(i, j) = pixel;
			}
			else
			{
				fg.at<uchar>(i, j) = 255;
				//fg_color.at<Vec3b>(i, j) = (cur_frame.at<Vec3b>(i, j));
			}
		}
	}

	//background update
	for (i = 0; i < height; i++)
	{
		for (j = 0; j < width; j++)
		{
			mean = cur_bg1.at<uchar>(i, j);
			var = cur_bg2.at<uchar>(i, j);
			diff = double(frame_gray.at<uchar>(i, j)) - mean;
			if (abs(diff) <= defaultVarTh*defaultVar)
			{
				cur_bg1.at<uchar>(i, j) = cur_bg1.at<uchar>(i, j) + alpha*diff;
				mean = cur_bg2.at<uchar>(i, j);
				diff = double(frame_gray.at<uchar>(i, j)) - mean;
				cur_bg2.at<uchar>(i, j) = sqrt(var*var + alpha*(diff*diff - var*var));
			}
		}
	}

	cvNamedWindow("fg");
	imshow("fg", cur_bg1);

	//cvNamedWindow("fg_color");
	//imshow("fg_color", fg_color);

	//cvNamedWindow("cur_frame");
	//imshow("cur_frame", cur_frame);

	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(fg, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
	double minarea = 650;
	for (i = 0; i < contours.size(); i++)
	{
		double tmparea = fabs(contourArea(contours[i]));
		//std::cout << i << "area" << contourArea(contours[i]) << std::endl;
		if (tmparea < minarea)
		{
			//delete the contours whose area less than minarea
			contours.erase(contours.begin() + i);
			i = i - 1;
		}
	}

	for (i = 0; i < contours.size(); i++)
	{
		Scalar color = Scalar(0, 0, 255);
		drawContours(cur_frame, contours, i, color, 2, 8, hierarchy, 0, Point());
	}
	cvNamedWindow("dst");
	imshow("dst", cur_frame);
	
}