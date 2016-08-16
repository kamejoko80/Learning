#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include "cal_cloud.h"
#include "Dataset.h"
#include "coordTrans.h"

using namespace std;
using namespace cv;


void cropEye(const cv::Mat &img_u, const cv::Mat &et, const cv::Mat hR, const Mat target,
			const Mat &cam, cal_cloud* cal,
			 cv::Mat &img_warped, cv::Mat &hrnew, cv::Size roiSize,
			 string save_path, string img_name, ofstream &log, Point2f eye_align, bool use_align, int isEye)
{
	float focal_new, distance_new;
	if (isEye > 0)
	{
		focal_new = 1920.f;
		distance_new = 600.f;
	} else
	{
		focal_new = 800.f;
		distance_new = 900.f;
	}
    // ---------- normalize image ----------
    const float distance = cv::norm(et);
    const float z_scale = distance_new / distance;

	Mat Zv = hR.col(2);
	assert(Zv.at<float>(2) > 0);
	float head_la, head_lo;
	cal_lo_la(-Zv, eye(3, 3, CV_32F), head_lo, head_la);

	if (head_la < 80 && head_la > -80 && head_lo < 80 && head_lo > -80)
	{

		Mat eye = cam * et;
		int width = roiSize.width * z_scale;
		int height = roiSize.height * z_scale;
		int x, y;
		if (use_align)
		{
			x = eye_align.x - width/2;
			y = eye_align.y - height/2;
		} else
		{
			x = eye.at<float>(0) / eye.at<float>(2) - width/2;
			y = eye.at<float>(1) / eye.at<float>(2) - height/2;
		}
		if (x > 0 && x + width < 1920 && y > 0 && y + height < 1080)
		{
			img_warped = img_u(Rect(x, y, width, height));
			resize(img_warped, img_warped, roiSize);
			// ---------- normalize gaze vector ----------
			float eye_la, eye_lo;
			cal_lo_la(target - et, eye(3, 3, CV_32F), eye_lo, eye_la);

			if (isEye == 1)
				log << img_name << " " << eye_lo << " " << eye_la << endl;
			else
			{
				cv::flip(img_warped, img_warped, 1);
				log << img_name << " " << - eye_lo << " " << eye_la << endl;
				
			}
			//char str[100];
			//sprintf(str, "lo: %.f; la: %.f", head_lo, head_la);
			//putText(img_warped, str, cv::Point(10,50), FONT_HERSHEY_COMPLEX, 0.8, cv::Scalar(0,255,0), 2);
			string path = save_path + img_name;
			cv::imwrite(path, img_warped);
			//cout << "hrnew = " << endl << hrnew << endl;
			//cout << "htnew = " << endl << htnew << endl;
		} else
		{
			cout << "eye x = " << x << "   y = " << y << endl;
			cout << "eye area out of img" << endl;
		}
	} else
	{
		cout << "head_la" << head_la << "    head_lo" << head_lo << endl;
		cout << "...... big headpose pass." << endl;
	}
}


void normalizeData(const Mat &img_u, const Mat &face, const Mat target, const Mat &cam,
                    cal_cloud* cal, string save_path, string img_name, ofstream &log, Point2f re_align, Point2f le_align, bool use_align)
{
    // undistort image
    //cv::cvtColor(img_u, img_u, COLOR_BGR2GRAY);
    //cv::imshow("eye_ori", img_u);  //cv::waitKey(0);
    // compute estimated 3D positions of the landmarks
    cv::Mat hR;
    Mat re = face(cv::Rect(0, 17, face.cols, 1));
    Mat le = face(cv::Rect(0, 16, face.cols, 1));
    cv::Mat mid_eye = (re + le) * 0.5;

    Mat mid_mouth = 0.5 * (face(cv::Rect(0, 19, face.cols, 1)) + face(cv::Rect(0, 20, face.cols, 1)));
	getHeadpose(re, le, mid_mouth, hR);

	hR = hR.t();
	Mat mid_head = mid_eye + cv::norm(re - le) * 0.9 * hR.col(2).t();
    // normalize each eye
    cv::Mat img_warped, hrnew;
	cv::Size eye_roiSize(120, 72);
	save_path = save_path + "/eye/";
	string left_img_name = "left_" + img_name;
	cropEye(img_u, le.t(), hR, target, cam, cal, img_warped, hrnew, eye_roiSize, save_path, left_img_name, log, le_align, use_align, 1);
	string right_img_name = "right_" + img_name;
	cropEye(img_u, re.t(), hR, target, cam, cal, img_warped, hrnew, eye_roiSize, save_path, right_img_name, log, re_align, use_align, 2);
}

int main(int argc, char **argv)
{
	if (argc != 6)
	{
		cout << "usage: ./test_nomalize [calib_file] [landmarks.txt] [path_to_origin_imgs] [path_to_save_imgs] [log.txt]\n";
		return -1;
	}

	cal_cloud* cal = new cal_cloud(argv[1], 1920, 1080, 10.0);
	std::vector<dataset::Case> cases = dataset::read(argv[2]);
	string img_folder(argv[3]);
	string save_path(argv[4]);
	string log_file_path(argv[5]);
	//int isEye = atoi(argv[6]);

	system(("mkdir -p " + save_path).c_str());
	system(("mkdir -p " + save_path + "/eye").c_str());
	string img_path, img_name;
	int pid, cid, camId;


	ofstream log_file(log_file_path, fstream::app | fstream::out);

	cout << cases.size() << endl;
	cv::Mat face_world, face_cam, target, cam, dist, camR, camT;
	cv::Mat marker_mat(3, 1,CV_32F);
	const float factor = 1080.0/745.0;
	const cv::Point3f tv_zero = TV_ZERO;
	//#pragma omp parallel for
	for (int case_i = 0; case_i < cases.size(); case_i++)
	{
		bool is_success = cal->getCloudRansac(cases[case_i].pts, cases[case_i].camera_Idx, face_world, 2);
		if (! is_success)
			continue;
		pid = cases[case_i].pid;
		cid = cases[case_i].cid;
		cout << "pid:" << pid << "  cid:" << cid << endl;
		for (int cam_i = 0; cam_i < cases[case_i].camera_Idx.size(); cam_i++)
		{
			camId = cases[case_i].camera_Idx[cam_i];
			//cout << "  camId:" << camId << endl;

			// load calibration data
			cam = cal->cam[camId].MatIntrinsics; 
			dist = cal->cam[camId].distCoeffs;
			camR = cal->cam[camId].R;
			camT = cal->cam[camId].tvec;
			
			float rep_error = 0.f;
			bool use_align = false;
			// evaluate the reconstruction error
			vector<cv::Point2f> rep = cal->reproject_kpts(face_world, camId);
			for (int p = 0; p < 21; p++)
			{
				rep_error += cv::norm(rep[p] - cases[case_i].pts[camId][p]);
			}
			if( rep_error > 150 && rep_error < 3000 )
			{
				use_align = true;
			}
			cv::Point2f re_align =  cases[case_i].pts[camId][17];
			cv::Point2f le_align = cases[case_i].pts[camId][16];

			// read 3d position of 21 point on face
			face_cam = face_world * camR.t() + cv::repeat(camT.t(), 21, 1);
			marker_mat.at<float>(0) = tv_zero.x - cases[case_i].slave_pt.x/factor; // TODO: gap
			marker_mat.at<float>(1) = tv_zero.y + cases[case_i].slave_pt.y/factor;
			marker_mat.at<float>(2) = tv_zero.z;
			target = camR * marker_mat + camT;
			
			stringstream ss;
			ss << "pid_" << pid << "id_" << cid << "_cam" << camId << ".jpg";
			ss >> img_name;
			img_path = img_folder + "/" + img_name;
			cv::Mat img = cv::imread(img_path);
			if (img.empty())
			{
				cout << "Failed to read image" << img_name << endl;
				continue;
			}
			Mat re = face_cam(cv::Rect(0, 17, face_cam.cols, 1));
			Mat le = face_cam(cv::Rect(0, 16, face_cam.cols, 1));
			Mat re_2d = cam * re.t();
			Mat le_2d = cam * le.t();
			Point2f re_pt(re_2d.at<float>(0) / re_2d.at<float>(2), re_2d.at<float>(1) / re_2d.at<float>(2));
			Point2f le_pt(le_2d.at<float>(0) / le_2d.at<float>(2), le_2d.at<float>(1) / le_2d.at<float>(2));

			// cv::circle(img, re_pt, 4, Scalar(0,255,0), 2);
			// cv::circle(img, le_pt, 4, Scalar(0,255,0), 2);
			// cv::line(img, re_pt, le_pt, Scalar(0, 0, 0), 5);
			// imshow("img", img);
			//	cv::waitKey(0);
			string img_name_prefix = img_folder.substr(0, img_folder.find_last_of("/"));
			img_name_prefix = img_name_prefix.substr(img_name_prefix.find_last_of("/") + 1, img_name_prefix.size());
			img_name = img_name_prefix + "_" + img_name;

			cv::Mat img_u;
			undistort(img, img_u, cam, dist);
			if (re_pt.x > le_pt.x)
				normalizeData(img_u, face_cam, target, cam, cal, save_path, img_name, log_file, re_align, le_align, use_align);
			else
				cout << "......re < le: big pose pass." << endl;
		}
    }
	log_file.close();
    return 0;
}
