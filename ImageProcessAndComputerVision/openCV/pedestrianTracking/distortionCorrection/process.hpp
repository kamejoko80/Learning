#ifndef _PROCESS_
#define _PROCESS_
#include<opencv2/opencv.hpp>
#include<iostream>
using namespace std;
using namespace cv;

//	The struct is defined for the used of multiple-threads. For the method of using multiple-threads
//you can search on the Internet.In fact, the method is easy.
//Because we use multiple-threads to accelerate, the segment [start , end) is the range of data a thread need to process. 
struct argment_p   
{
	int start;
	int end;
	short* u_v;
	short* u_v_16;
	short* u_16_v; 
	short* u_16_v_16;
	int** pos;
	uchar* DataSrc;  //the picture will be processed
	uchar* DataDst;  //the picture already processed
    int StepSrc;
	
    argment_p(int _start = 0, int _end = 0, short* _u_v = NULL, short* _u_v_16 = NULL, short* _u_16_v = NULL, short* _u_16_v_16 = NULL, int** _pos = NULL,uchar* _DataSrc = NULL, uchar* _DataDst = NULL):
	start(_start),end(_end),u_v(_u_v),u_v_16(_u_v_16),u_16_v(_u_16_v),u_16_v_16(_u_16_v_16),pos(_pos),DataSrc(_DataSrc),DataDst(_DataDst){}
};


void process( short* u_v, short* u_v_16, short* u_16_v, short* u_16_v_16, int** pos, Mat& ImgSrc, Mat& ImgDst,int cnt = 4);

#endif
