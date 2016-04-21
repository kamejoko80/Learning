#ifndef _KALMAN_
#define _KALMAN__
#include <opencv2/opencv.hpp>
#include <iostream>
#include <cmath>
using namespace std;
using namespace cv;
double array0[16] = {1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1};
double array1[8] = {1, 0, 0, 0, 0, 1, 0, 0};
double array2[16] = {100, 0, 0, 0, 0, 100, 0, 0, 0, 0, 100, 0, 0, 0, 0, 100};
Mat A = Mat(4,4,CV_32F,array0);   
Mat H = Mat(2,4,CV_32F,array1);
Mat P = Mat(4,4,CV_32F,array2);   
Mat Q = eye(4,4,CV_32F);
Mat R = eye(2,2,CV_32F);
int find_pos(int* temp, int val,int length)
{
	for(int i = 0;i < length;i++)
	{
		if(temp[i] == val)
		{
			return i;
		}
	}
	return length;
}
void  kalman(Obj* last_obj, Obj* cur_obj, Obj& pre_obj, int last_num, int cur_num, Mat& P,Mat& A, Mat& H, Mat& Q, Mat& R, M, Mat& P_new)
{
   P = A*P*A.t()+Q;
   kf_gain = P*H.t()/(H*P*H.t()+R);
   Mat x = Mat(4, 1, CV_64F);
   for(int i = 0; i < last_num;i++)
   { 
	  x.at<double>(0,0) = last_obj[i].X;
	  x.at<double>(1,0) = last_obj[i].Y;
	  x.at<double>(2,0) = last_obj[i].Vx;
	  x.at<double>(3,0) = last_obj[i].Vy;
      x = A*x;
      last_obj[i].X = x.at<double>(0,0);
      last_obj[i].Y = x.at<double>(1,0);
   }
   int* temp = new int[cur_num];
   for(int i = 0; i < cur_num;i++)
   {
	   temp[i] = i;
   }
   for(int i = 0; i < last_num; i++)
   {
	   for(int j = 0;j < cur_num; j++)
	   {
		    double value = imsimilar(last_obj[i].hist,cur_obj[j].hist,2);
			d = (min(last_obj[i].X,cur_obj[j].X)/max(last_obj[i].X,cur_obj[j].X))*(min(last_obj[i].Y,cur_obj[j].Y)/max(last_obj[i].Y,cur_obj[j].Y));
            double thr = d*value*100;
			if(thr >= 45)
            {  
		       last_obj[i].behind_match = j;
               int index = find_pos(temp,last_obj[i].No,cur_num);
               temp[index] = cur_obj[j].No;
               temp[j] = last_obj[i].No;
			}
           else
		   {
			  if(value >= 0.7)
			  {   
		        last_obj[i].behind_match = j;
                int index = find_pos(temp,last_obj[i].No,cur_num);
                temp[index] = cur_obj[j].No;
                temp[j] = last_obj[i].No;
              }  
		   }
	   }
	   
   }
   for (int i = 0;i < cur_num; i++)
   {
	   cur_obj[i].No = temp[i];
   }
   
   
   pre_obj = cur_obj;
   for(int i = 0;i < last_num;i++) 
   {   int index = last_obj(i).behind_match;
       Mat _t = Mat(4,1,CV_32F);
	   Mat _s = Mat(2,1,CV_32F);
	   _t.at<double>(0,0) = last_obj[i].X;
	   _t.at<double>(1,0) = last_obj[i].Y;
	   _t.at<double>(2,0) = last_obj[i].Vx;
	   _t.at<double>(3,0) = last_obj[i].Vy;
	   _s.at<double>(0,0) = cur_obj[index].X;
	   _s.at<double>(1,0) = cur_obj[index].Y;
       if(index != 0) 
	   {  
          z = H * _t;
          r = _s - z;
          x = _t + kf_gain * r;
          pre_obj[index].X = x[1];
          pre_obj[index].Y = x[2];
          pre_obj[index].Vx = x[3];
          pre_obj[index].Vy = x[4];
	   }
 }
    P_new = P - kf_gain*H*P;


}
#endif