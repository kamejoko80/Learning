#ifndef _HIST_
#define _HIST_
#include <opencv2/opencv.hpp>
#include <iostream>
#include <cmath>
using namespace std;
using namespace cv;
Mat hist(Mat& m_rgb)
{
	int rows = m_rgb.rows;
	int cols = m_rgb.cols;
	Mat m_rgb_32;
	m_rgb.convertTo(m_rgb_32, CV_32FC3, 1.0/255.0, 0);
	Mat m_hsv;
	cvtColor(m_rgb_32, m_hsv, CV_BGR2HSV);
	for(int i = 0;i < m_hsv.rows;i++)
	{
		Vec3f * p = m_hsv.ptr<Vec3f>(i); 
		for(int j = 0; j < m_hsv.cols; j++)
		{
		//h
		if(p[j][0]<=15||p[j][0]>345) p[j][0] = 0;

        if(p[j][0]<=25&&p[j][0]>15)  p[j][0] = 1;              

        if(p[j][0]<=45&&p[j][0]>25)  p[j][0] = 2;

        if(p[j][0]<=55&&p[j][0]>45) p[j][0] = 3;                  

        if(p[j][0]<=80&&p[j][0]>55) p[j][0] = 4;                    

        if(p[j][0]<=108&&p[j][0]>80) p[j][0] = 5;                   

        if(p[j][0]<=140&&p[j][0]>108) p[j][0] = 6;                   

        if(p[j][0]<=165&&p[j][0]>140) p[j][0] = 7;                    

        if(p[j][0]<=190&&p[j][0]>165) p[j][0] = 8;                   

        if(p[j][0]<=220&&p[j][0]>190) p[j][0] = 9;                    

        if(p[j][0]<=255&&p[j][0]>220)  p[j][0] = 10;                   

        if(p[j][0]<=275&&p[j][0]>255) p[j][0] = 11; 
		
        if(p[j][0]<=290&&p[j][0]>275) p[j][0] = 12;
                    
        if(p[j][0]<=316&&p[j][0]>290)  p[j][0] = 13;
                   
        if(p[j][0]<=330&&p[j][0]>316) p[j][0] = 14;
                    
        if(p[j][0]<=345&&p[j][0]>330)  p[j][0] = 15;
           
		   //s
        if(p[j][1]<=0.15&&p[j][1]>0) p[j][1] = 0;
                   
        if(p[j][1]<=0.4&&p[j][1]>0.15) p[j][1] = 1;
                   
        if(p[j][1]<=0.75&&p[j][1]>0.4) p[j][1] = 2;
                    
        if(p[j][1]<=1&&p[j][1]>0.75) p[j][1] = 3;
            
			//v
	    if(p[j][2]<=0.15&&p[j][2]>0)  p[j][2] = 0;
                  
        if(p[j][2]<=0.4&&p[j][2]>0.15) p[j][2] = 1;
            
        if(p[j][2]<=0.75&&p[j][2]>0.4) p[j][2] = 2;
            
        if( p[j][2]<=1&&p[j][2]>0.75) p[j][2] = 3;       
		}
	}
	Mat m_temp = Mat(rows,cols,CV_8UC1);
	for(int i = 0;i < m_temp.rows;i++)
	{
		Vec3f * p = m_hsv.ptr<Vec3f>(i); 
		uchar * _p = m_temp.ptr<uchar>(i);  
		for(int j = 0; j < m_temp.cols; j++)
		{
			_p[j] = p[j][0]*16+p[j][1]*4+p[j][2];
		}
	}
	Mat histogram;
	const int histSize = 256;  
    float range[] = {0, 255};  
    const float *ranges[] = {range};  
    const int channels = 0;  
	calcHist(&m_temp, 1, &channels, cv::Mat(), histogram, 1, &histSize, &ranges[0], true, false);  
    return histogram;
}
#endif