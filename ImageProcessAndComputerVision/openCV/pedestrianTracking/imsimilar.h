#ifndef _IMSIMILAR_
#define _IMSIMILAR_
#include <opencv2/opencv.hpp>
#include <iostream>
#include <cmath>
using namespace std;
using namespace cv;
double issimilar(Mat& count0, Mat& count1 ,int type)
{
	int n1 = count0.cols;
	int n2 = count1.cols;
	if(n1 != n2)
	{
		cout<<"维度出错！"<<endl;
		return 0；
	}
	double result = 0;
	double value = 0;
	if(type == 1)
	{   double den;
        float* _count0 = (float*)count0.data;
		float* _count1 = (float*)count1.data;
		for(int i = 0; i < n1; i++)
		{
			den = _count0[i] < _count1[i] ? _count1[i]:_count0[i];
			if(!den)
			{
				den = 1;
			}
			result = result+(1-abs(_count1[x] - _count2[x])/den);   
		}
		value = 100*result/n1;
	}
	else
	{
		for(int i = 0;i < n1;i ++)
		{result = result+min(count1[x],count2[x]);}
         value = result;
	}
	return value;
}
#endif