#include <opencv2/opencv.hpp>
#include "coordTrans.h"

#define PI (3.1415926)

using namespace cv;
using namespace std;

void getHeadNormal(const cv::Mat &cloud, cv::Mat &normal) {
	cv::Mat matrW, matrU, matrV;
	cv::Mat row_mean;
	cv::reduce(cloud, row_mean, 0, CV_REDUCE_AVG);
	cv::Mat new_cloud = cloud - cv::repeat(row_mean, cloud.rows, 1);
	cv::SVD::compute(new_cloud, matrW, matrU, matrV);
	matrV.row(2).copyTo(normal);
	// if (normal.at<float>(0, 2) > 0)
		// normal = -normal;
}

void getHeadpose(const cv::Mat &cloud, cv::Mat &hR,  float adjust_angle)
{
	cv::Mat cloud_21_3;
	if (cloud.cols > cloud.rows)
		cloud_21_3 = cloud.t();
	else
		cloud_21_3 = cloud;
	assert(cloud_21_3.cols == 3);
	assert(cloud_21_3.rows == 21);

	cv::Mat re = cloud_21_3.row(17);
	cv::Mat le = cloud_21_3.row(16);
	cv::Mat mid_eye = (re + le) * 0.5;
	cv::Mat mid_mouth = 0.5 * (cloud_21_3.row(19) + cloud_21_3.row(20));
	getHeadpose(re, le, mid_mouth, hR, adjust_angle);
}

void getHeadpose(cv::Mat &re, cv::Mat &le, cv::Mat &mid_mouth, cv::Mat &hR, float adjust_angle)
{
	cv::Mat dirx = re - le; dirx /= cv::norm(dirx);
	cv::Mat mid_eye = (re + le) * 0.5;
	cv::Mat diry = (mid_mouth - mid_eye);
	cv::Mat Rx_15deg;
	cv::Mat rx_15deg = adjust_angle / 180.0 * PI * dirx;
	cv::Rodrigues(rx_15deg, Rx_15deg);
	diry = (Rx_15deg * diry.t()).t();
	// force diry to be vertical to dirx
	diry = diry - (dirx.dot(diry))*dirx;
	diry /= cv::norm(diry);
	cv::Mat dirz = dirx.cross(diry);

	cv::vconcat(dirx, diry, hR);
	cv::vconcat(hR, dirz, hR);
}


void cal_lo_la(const cv::Mat &vector, const cv::Mat &coordinate, float &lo, float &la)
{
	// the coordinate matrix should be the vconcat of three row vector x, y, z
	float x_proj = coordinate.row(0).dot(vector.t());
    float y_proj = coordinate.row(1).dot(vector.t());
    float z_proj = coordinate.row(2).dot(vector.t());
    lo = atan(x_proj / z_proj) * 180.0f / PI;
    la = asin(y_proj / cv::norm(vector)) * 180.0f / PI;
}


void camPointTo(const cv::Mat &img, cv::Mat &img_warped, cv::Mat &rotMat, const cv::Size &roiSize, 
			const cv::Mat &dirx, const cv::Mat &center, const cv::Mat &cam, const float distance_new, const float focal_new)
{
	cv::Mat cam_new, scaleMat;
	camPointTo(img, img_warped, rotMat, cam_new, scaleMat, roiSize, dirx, center, cam, distance_new, focal_new);
}

void camPointTo(const cv::Mat &img, cv::Mat &img_warped, cv::Mat &rotMat, cv::Mat &cam_new, cv::Mat &scaleMat, const cv::Size &roiSize, 
			const cv::Mat &dirx, const cv::Mat &center, const cv::Mat &cam, const float distance_new, const float focal_new)
{
	float pcamNew[] = {
			focal_new, 0, roiSize.width / 2.f,
			0, focal_new, roiSize.height / 2.f,
			0, 0, 1.f
	};
	cam_new = cv::Mat(3, 3, CV_32FC1, pcamNew);
	// ---------- normalize image ----------
	camPointTo(dirx, center, distance_new, rotMat, scaleMat);
	cv::Mat warpMat = cam_new * scaleMat * rotMat *  cam.inv();
	warpPerspective(img, img_warped, warpMat, roiSize, INTER_LINEAR, BORDER_CONSTANT, Scalar(128,128,128));
}

void camPointTo(const cv::Mat &dirx, const cv::Mat &center, const float &distance_new, cv::Mat &rotMat, cv::Mat &scaleMat)
{
	const float distance = cv::norm(center);
	const float z_scale = distance_new / distance;
	float pscaleMat[] = {
			1.f, 0.f, 0.f,
			0.f, 1.f, 0.f,
			0.f, 0.f, z_scale,
	};
	scaleMat = cv::Mat(3, 3, CV_32FC1, pscaleMat);
	// compute the virtual camera coordinate
	cv::Mat forward = center / distance;
	cv::Mat down = forward.cross(dirx); // just X axis
	down /= cv::norm(down);
	cv::Mat right = down.cross(forward);
	right /= cv::norm(right);

	cv::vconcat(right.t(), down.t(), rotMat);
	cv::vconcat(rotMat, forward.t(), rotMat);
}
