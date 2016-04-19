#ifndef _PROCESS_
#define _PROCESS_
#include<opencv2/opencv.hpp>
#include<iostream>
using namespace std;
using namespace cv;

struct argment_p   //	The struct is defined for the used of multiple-threads. For the method of using multiple-threads
{                  //you can search on the Internet.In fact, the method is easy.
	int start;     //Because we use multiple-threads to accelerate, the segment [start , end) is the range of data a thread need to process. 
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

void _compute(int start, int end, short* u_v, short* u_v_16, short* u_16_v, short* u_16_v_16, int** pos,uchar* DataSrc, uchar* DataDst)
{
	for(int i = start; i < end;i++)
	{
		
		short _u_v = u_v[i];
		short _u_16_v = u_16_v[i];
		short _u_v_16 = u_v_16[i];
		short _u_16_v_16 = u_16_v_16[i];
                int pos0 = pos[0][i];
                int pos1 = pos[1][i];
                int pos2 = pos[2][i];
                int pos3 = pos[3][i];                
	 	for (int k = 0; k < 3; k++)
	 	{   int temp = 0;
                   
                    if(pos0 >= 0) temp += DataSrc[pos0 + k]*_u_16_v_16;
		    if(pos1 >= 0) temp += DataSrc[pos1 + k]*_u_v_16;
	            if(pos2 >= 0) temp += DataSrc[pos2 + k]*_u_16_v;
		    if(pos3 >= 0) temp += DataSrc[pos3 + k]*_u_v;
                    DataDst[3*i + k] = temp>>8;
                   
		    
	 	}
	}
}
void *compute(void* arg)
{ 
	argment_p* data= (argment_p*) arg;
	_compute(data->start, data->end, data->u_v, data->u_v_16, data->u_16_v, data->u_16_v_16, data->pos, data->DataSrc, data->DataDst);
	pthread_exit(NULL);
}
void process( short* u_v, short* u_v_16, short* u_16_v, short* u_16_v_16, int** pos, Mat& ImgSrc, Mat& ImgDst,int cnt = 4)
{          //cnt is the num of threads, 4 is the default value,which may be fastest.
        int gap = ImgDst.rows/cnt;
	pthread_t* ptp = new pthread_t[cnt];
	argment_p* argp = new argment_p[cnt];
        uchar* DataSrc = ImgSrc.data;
        uchar* DataDst = ImgDst.data;   
        int cols = ImgDst.cols;
        int rows = ImgDst.rows;
        int num = cols*rows; 
        for(int i = 0;i < cnt;i++)
	{
		argp[i].start = gap*i*cols;
		argp[i].end = argp[i].start + gap*cols;
		argp[i].u_v = u_v;
		argp[i].u_v_16 = u_v_16;
		argp[i].u_16_v = u_16_v;
		argp[i].u_16_v_16 = u_16_v_16;
		argp[i].pos = pos;
		argp[i].DataSrc = DataSrc;
		argp[i].DataDst =DataDst;
	}
	argp[cnt-1].end = num; 
        
        int err;
        for(int i = 0; i < cnt-1;i++)
	{ 
           err = pthread_create(&ptp[i],NULL,compute,&argp[i]);
	   if(err!=0)
	   {cerr<<"Create pthread failed!\n";}
	}
        _compute(argp[cnt-1].start, argp[cnt-1].end, argp[cnt-1].u_v, argp[cnt-1].u_v_16, argp[cnt-1].u_16_v, argp[cnt-1].u_16_v_16, argp[cnt-1].pos,argp[cnt-1].DataSrc, argp[cnt-1].DataDst); 
        
        for(int i = 0; i < cnt-1;i++)
	{
	  pthread_join(ptp[i],NULL);
	}

}
#endif
