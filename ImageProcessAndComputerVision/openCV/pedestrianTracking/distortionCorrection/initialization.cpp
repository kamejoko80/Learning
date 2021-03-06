#include <opencv2/opencv.hpp>
#include <iostream>
#include "initialization.hpp"

using namespace std;
using namespace cv;

//this function is used to calculate the proc_data
void fill_u_v(float* pre_data, short* proc_data1, short* proc_data2, int num)
{
	for(int i = 0;i < num;i++)
	{
		short src = floor(pre_data[i]);
        // the proc_data1 is equal to u or v in the formulation
		proc_data1[i] = (pre_data[i] - src) * 16;
        // the proc_data2 is equal to 16 - u or 16 - v in the formulation
		proc_data2[i] = 16 - proc_data1[i];
	}
}

//this function is defined for the use of multi_thread.
void* fill(void* arg)
{
	argment *pa = (argment*)arg;
	fill_u_v(pa->pre_data, pa->proc_data1, pa->proc_data2, pa->num);
	pthread_exit(NULL);
}

//this function is used to calculate the four position needed for the interpolation
void pos_cac(float* x, float* y,int **pos,int rows, int cols, int num, int StepSrc, int channels)
{   
    int _max = channels * num;
    // StepSrc is equal to (the num of pixls of a line)* (channels of per pixl) 
	for(int i = 0; i < num; i++)
	{
		short xsrc = floor(x[i]);
	    short ysrc = floor(y[i]);
	    //base is the position of the first channel of the pixl in the picture 
        int base = ysrc*StepSrc + xsrc*channels;
		pos[0][i] = base - StepSrc - channels;
		pos[1][i] = base - StepSrc;
		pos[2][i] = base - channels;
		pos[3][i] = base;
        
        // According to the formulation, we need 4 positions, but in some cases,several       
        // positions maybe out of range.For example,if a pixl's base is 0,then pos0~pos2 will
        //out of range.If a pos is out of range,we don't use it in the formulation, and we  
        //use -1 to record.
        if(pos[0][i] >= _max || pos[0][i] < 0) {pos[0][i] = -1;cout<<"out of range"<<endl;}
		if(pos[1][i] >= _max || pos[1][i] < 0) {pos[1][i] = -1;cout<<"out of range"<<endl;}
		if(pos[2][i] >= _max || pos[2][i] < 0) {pos[2][i] = -1;cout<<"out of range"<<endl;}
		if(pos[3][i] >= _max || pos[3][i] < 0) {pos[3][i] = -1;cout<<"out of range"<<endl;}
                
	}
}


//the function  is used to calculate the parameter used in the formulation.
//u_v is equal to u*v, u_v_16 is equal to u*(16 - v), u_16_v is equal to (16-u)*v, u_16_v_16 is equal to (16 - u)*(16 - v)
void parameter_cac(short* u, short* v, short* u_16, short* v_16,short* u_v, short* u_v_16, short* u_16_v, short* u_16_v_16, int num)
{                              
    for(int i = 0;i < num;i++)
	{
		u_v[i] = u[i] * v[i];
		u_v_16[i] = u[i] * v_16[i];
		u_16_v[i] = u_16[i] * v[i];
		u_16_v_16[i] = u_16[i] * v_16[i];
	}
}

void initialize(short* _u_v, short* _u_v_16, short* _u_16_v, short* _u_16_v_16, int** _pos, Mat& mat_x, Mat& mat_y, int StepSrc, int channels)
{   
    float* x = (float*)mat_x.data;
	float* y = (float*)mat_y.data;
    int cols = mat_x.cols;
	int rows = mat_x.rows;
	int num =  cols * rows; 
	short* u = new short[num];
	short* v = new short[num];
	short* u_16 = new short[num];
	short* v_16 = new short[num];
   
    // multiple-thread, there are 2 threads.
	pthread_t pt;
	argment arg0(x, u, u_16, num), arg1(y, v, v_16, num);
    
    int err;
	err = pthread_create(&pt, NULL, fill, &arg0);
	if(err != 0)
	{
        cerr<<"Create pthread failed!\n";
    }
    
    fill_u_v(arg1.pre_data, arg1.proc_data1, arg1.proc_data2, arg1.num);
	pthread_join(pt, NULL);
	pos_cac(x, y, _pos, rows, cols, num, StepSrc, channels);	
	parameter_cac(u, v, u_16, v_16, _u_v, _u_v_16, _u_16_v, _u_16_v_16, num);
	
	delete []u;
	delete []v;
	delete []u_16;
	delete []v_16;
}
