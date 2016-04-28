#ifndef _INItIALIZATION_
#define _INItIALIZATION_

#include<opencv2/opencv.hpp>
#include<iostream>

using namespace std;
using namespace cv;

//the struct is defined for the used of multiple-threads.
//For the method of using multiple-threads
//you can search on the net. In fact, the method is easy.
struct argment              
{   
	float* pre_data;        //the data of the matrix X or Y
	short* proc_data1;      //proc_data1[i] = (pre_data[i] - floor(pre_data[i])) * 16;
	short* proc_data2;      //proc_data2[i] = 16 - proc_data1[i];
	int num;                //the num of the elements of matrix X or Y 
	argment(float *pre_d = NULL, short* pro_d1 = NULL, short *pro_d2 = NULL, int n = 0):\
    pre_data(pre_d), proc_data1(pro_d1),proc_data2(pro_d2),num(n){}
};

void initialize(short* _u_v, short* _u_v_16, short* _u_16_v, short* _u_16_v_16, int** _pos, Mat& mat_x, Mat& mat_y, int StepSrc, int channels);

#endif

