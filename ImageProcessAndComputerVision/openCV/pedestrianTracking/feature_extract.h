#ifndef _FEATURE_EXTRACT_
#define _FEATURE_EXTRACT_
//#include <opencv2/opencv.hpp>
//#include <iostream>
#include <cmath>
#include "struct_obj.h"
#include "hist.h"
#include "mser.h"

//using namespace std;
//using namespace cv;

Obj* feature_extract(vector<vector<Point>>& contour,Mat& cur_frame,Mat& fg_t)
{   
	Mat mat;
	/*Obj* obj = new Obj[contours.size()];*/
	/*Obj* obj;*/
	Obj obj[5];
	int count = 0;
	for (int i = 0; i < contour.size(); i++)
	{
		Rect rectangle = boundingRect(contour[i]);
		Point point1(rectangle.x, rectangle.y);
		Point point2(rectangle.x + rectangle.width, rectangle.y + rectangle.height);
		IplImage *p = &IplImage(fg_t);
		cvSetImageROI(p, rectangle);
		mat = cvarrToMat(p);
		double tmparea = fabs(contourArea(contour[i]));
		if (tmparea > 4650 || mat.rows>100 || mat.cols>100)//多于一个人,确定人数，找到每个人的质心
		{
			centroid c;
			int thres = 150;
			normalize(mat, mat, 0, 255, CV_MINMAX, CV_8U);
			c = mser(mat, thres, tmparea);//1-2ms，阈值上升找到人的个数，并返回每个人的质心
			Node *p = c.get_head();
			for (int j = 1; j <= c.length;j++)
			{
				obj[count + j - 1].X = p->x + rectangle.x;
				obj[count + j - 1].Y = p->y + rectangle.y;
				obj[count + j - 1].No = count + j;
				int start_Y = round(obj[count + j - 1].Y - 30), end_Y = round(obj[count + j - 1].Y + 30);
				int start_X = round(obj[count + j - 1].X - 35), end_X = round(obj[count + j - 1].X + 25);
				int width = 0, height = 0;
				if (start_Y < 0){
					start_Y = 0;
					height = round(2 * obj[count + j - 1].Y);
					end_Y = start_Y + height - 1;
				}
				else if (end_Y > 202){
					end_Y = 202;
					height = round(2 * (202 - obj[count + j - 1].Y));
					start_Y = end_Y - width + 1;
				}
				else
					height = 60;
				if (start_X < 0){
					start_X = 0;
					width = round(2 * obj[count + j - 1].X);
					end_X = start_X + width - 1;
				}
				else if (end_X > 290){
					end_X = 290;
					width = round(2 * (290 - obj[count + j - 1].X));
					start_X = 290 - width + 1;
				}
				else
					width = 70;
				Rect rect(start_X, start_Y,width,height);
				/*Mat A = cur_frame(Range(start_Y, end_Y),Range(start_X, end_X));*/
				Mat A = cur_frame(rect);
				namedWindow("A",1);
				imshow("A",A);
				waitKey(1);
				destroyWindow("A");
				obj[count + j - 1].hist = hist(A);
				obj[count + j - 1].W = width;
				obj[count + j - 1].H = height;
				p = p->next;
			}
			count += c.length;
		}
		else//一个人，存入结构体，提取特征
		{
			Moments mom = moments(contour[i]);
			obj[count].X = mom.m10 / mom.m00;
			obj[count].Y = mom.m01 / mom.m00;
			obj[count].No = count + 1;
			Mat A = cur_frame(Range(round(rectangle.y), round(rectangle.y + rectangle.height - 1) + 1),
							  Range(round(rectangle.x), round(rectangle.x + rectangle.width - 1) + 1));
			namedWindow("2",1);
			imshow("2", A);
			waitKey(1);
			destroyWindow("2");
			obj[count].hist = hist(A);
			obj[count].W = round(rectangle.width);
			obj[count].H = round(rectangle.height);
			count++;
		}
	}
		/*Moments mom = moments(Mat(contours[i]));  
        obj[i].X = mom.m10/mom.m00;
		obj[i].Y = mom.m01/mom.m00;
        obj[i].Vx = 0;
        obj[i].Vy = 15;
		obj[i].No = i;
        obj[i].behind_match = 0;
		if(rectangle.height < 40 && rectangle.y + rectangle.height/2 - 20 >= 1 && rectangle.y + rectangle.height/2 + 20 <= 200) 
		{
		   rectangle.y = rectangle.y + rectangle.height/2 - 20;
           rectangle.height = 40;
		} 
		if(rectangle.width < 40 && rectangle.x + rectangle.width/2 - 30 >= 1 && rectangle.x +  rectangle.width/2 + 30 <= 291) 
		{
		   rectangle.x = rectangle.x + rectangle.width/2 - 30;
           rectangle.width = 60;
		}
		Mat A = cur_frame(Range(round(rectangle.y), round(rectangle.y + rectangle.height - 1) + 1),
						  Range(round(rectangle.x), round(rectangle.x + rectangle.width - 1) + 1));
        obj[i].hist = hist(A);
		obj[i].W = round(rectangle.x + rectangle.width - round(rectangle.x));
        obj[i].H = round(rectangle.y + rectangle.height - round(rectangle.y));*/
	Obj* obj_t = new Obj[count];
	for (int i = 0; i < count; i++)
	{
		obj_t[i] = obj[i];
	}
	return obj_t;
}
#endif