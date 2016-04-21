#ifndef _STRUCT_OBJ_
#define _STRUCT_OBJ_

#include <opencv2/opencv.hpp>
using namespace cv;
struct Obj
{	
	Mat hist;
    double X ;
    double Y ;
    double Vx ;
    double Vy ;
    double W;
    double H;
    int No ;
    int behind_match;
	Obj() :Vx(0), Vy(15), behind_match(0){}
};
#endif