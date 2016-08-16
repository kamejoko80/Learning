#include <opencv2/opencv.hpp>

// IN
// cloud: 21 3D face points matrix
// OUT
// hR: the headpose rotation matrix
void getHeadpose(const cv::Mat &cloud, cv::Mat &hR, float adjust_angle = 10.0f);
// IN
// re: the right eye 3D points in camera coordinate
// le: the left eye 3D points in camera coordinate
// mid_mouth: the middle of mouth conner 3D points in camera coordinate
// OUT
// hR: the headpose rotation matrix
void getHeadpose(cv::Mat &re, cv::Mat &le, cv::Mat &mid_mouth, cv::Mat &hR, float adjust_angle = 10.0f);

void getHeadNormal(const cv::Mat &cloud, cv::Mat &normal);

void cal_lo_la(const cv::Mat &vector, const cv::Mat &coordinate, float &lo, float &la);

void camPointTo(const cv::Mat &img, cv::Mat &img_warped, cv::Mat &rotMat, const cv::Size &roiSize, 
			const cv::Mat &dirx, const cv::Mat &center, const cv::Mat &cam, const float distance_new, const float focal_new = 1920.f);

void camPointTo(const cv::Mat &img, cv::Mat &img_warped, cv::Mat &rotMat, cv::Mat &cam_new, cv::Mat &scaleMat, const cv::Size &roiSize, 
			const cv::Mat &dirx, const cv::Mat &center, const cv::Mat &cam, const float distance_new, const float focal_new = 1920.f);

void camPointTo(const cv::Mat &dirx, const cv::Mat &center, const float &distance_new, cv::Mat &rotMat, cv::Mat &scaleMat);

