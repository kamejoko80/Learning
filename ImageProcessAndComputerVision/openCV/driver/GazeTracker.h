//
// Created by 金宇超 on 16/7/12.
//

#ifndef CALIBRATION_SCREEN_GAZETRACKER_H
#define CALIBRATION_SCREEN_GAZETRACKER_H

#include <cmath>
#include <fstream>
#include <opencv2/opencv.hpp>

#include "cal_cloud.h"
#include "Tracker_NoCvFace.h"
#include "util.hpp"
#include "coordTrans.h"
#include "PostProcess.hpp"

using namespace cv;
using namespace std;
using namespace tracking;

// RESULT STRUCTURE
struct Gaze_t {
    float gaze_longitude;
    float gaze_latitude;
    float gaze_vector[3];

    float head_coordinate[3]; // XYZ
    float head_angle[3]; // Pitch, Yaw, Roll

    float head_coordinate_cam1[3]; // XYZ
    float head_angle_cam1[3]; // Pitch, Yaw, Roll
    float head_coordinate_cam2[3]; // XYZ
    float head_angle_cam2[3]; // Pitch, Yaw, Roll
    // DEBUG ONLY!
    cv::Mat cloud;
    cv::Mat axis;
};

struct GazeEstimator{
public:
    GazeEstimator(){
        img_input_ = NULL;
    }
    
    ~GazeEstimator(){
        if(img_input_ != NULL)
        delete [] img_input_;
        img_input_ = NULL;
    }

    void Set_Model(){
        set_model(conf.GPU_id);
        if(img_input_ != NULL)
        delete [] img_input_;
        // img_input = new float[]; //?! set the input size
    }

    void Set_Conf(const string conf_path){
        LOG(INFO) << conf_path.c_str() << endl;
        conf = Config(conf_path);
    }

    std::pair<float, float> Predict(const cv::Mat& input_img){ // TODO: should we check the input image?
        cv::Mat imgin;
        cv::resize(input_img, imgin, cv::Size(conf.new_width, conf.new_height));
        
        cv::Mat imgout;
        cv::Mat imgcrop;
        imgin.convertTo(imgcrop, CV_32F); // TODO: should we handle data type?
        // resize & crop
        cv::Rect crop_area(
            (conf.new_width-conf.crop_width)/2,
            (conf.new_height-conf.crop_height)/2,
            conf.crop_width, conf.crop_height);
        imgout = cv::Mat(imgcrop,crop_area).clone(); //! unconsistent 

        vector<int> img_shape_(4,0);
        img_shape_[0] = 1;
        img_shape_[1] = imgout.channels();
        img_shape_[2] = imgout.rows;
        img_shape_[3] = imgout.cols;

        int buffer_size = (int)(imgout.channels() * imgout.cols * imgout.rows); // the size of img_input_
        if( buffer_size != old_img_input_size ) {
            if( img_input_ != NULL ) { // already allocated
                delete[] img_input_; // release old data
            }
            img_input_ = new float[buffer_size]; // allocate new data
        }

        CHECK(imgout.isContinuous()) << "crop image is not continous! " << endl;
        for (int i=0; i<imgout.rows*imgout.cols; i++) { 
            for( int c=0; c<imgout.channels(); c++) {
                img_input_[imgout.cols*imgout.rows*c + i] = ((float*)imgout.data)[i*imgout.channels()+c] - conf.pixel_means_[c];
            }
            // img_input_[imgout.cols*imgout.rows*1 + i] = ((float*)imgout.data)[i*3+1] - conf.pixel_means_[1];
            // img_input_[imgout.cols*imgout.rows*2 + i] = ((float*)imgout.data)[i*3+2] - conf.pixel_means_[2];
        } // attention! this step might be incorrect.
        const vector<Blob<float>*>& input_blobs = Alex_caffe_net->input_blobs();
        CHECK ( input_blobs.size() == 1) << "illegal input layer size!" << input_blobs.size() << endl;
        input_blobs[0]->Reshape(img_shape_);
        memcpy(input_blobs[0]->mutable_cpu_data(), img_input_, sizeof(float) * input_blobs[0]->count());
        Alex_output = Alex_caffe_net->ForwardPrefilled();
        float longitude = Alex_output[0]->cpu_data()[0];
        float latitude = Alex_output[0]->cpu_data()[1];
        return make_pair(longitude, latitude);
    }

    string Image_list(){return conf.Image_list;}
    string Output_fea(){return conf.Output_fea;}
    string getImgDir(){return conf.Image_Dir;}
private:
    boost::shared_ptr<Net<float> > Alex_caffe_net;
    vector<Blob<float>*> Alex_output;

    int old_img_input_size;
    float *img_input_;
        
    struct Config{
        string Image_Dir;
        string Image_list;
        string Output_fea;
        string prototxt;
        string model;
        float pixel_means_[3];
        int GPU_id;
        int new_height;
        int new_width;
        int crop_height;
        int crop_width;
        
        Config(){}
        Config(const string confpath){
            // load config
            ifstream fin(confpath.c_str());
            string HASH;

            for (int index = 0; index < 7; index++ ){
                fin >> HASH;
                cout << "READ=" << HASH << endl;
                if(index == 0){
                    fin >> HASH;
                    CHECK(HASH == "Configure_AlexNet");
                    LOG(INFO) << HASH;
                } else if(index == 1) {
                    CHECK(HASH == "#Caffe_Prototxt");
                    fin >> prototxt;
                } else if(index == 2) {
                    CHECK(HASH == "#Caffe_Model");
                    fin >> model;
                } else if(index == 3) {
                    CHECK(HASH == "#GPU_ID");
                    fin >> GPU_id;
                } else if(index == 4) {
                    CHECK(HASH == "#MEANS");
                    fin >> pixel_means_[0] >> pixel_means_[1] >> pixel_means_[2];
                } else if(index == 5) {
                    CHECK(HASH == "#Image_Size");
                    fin >> new_height >> new_width;
                } else if(index == 6) {
                    CHECK(HASH == "#Crop_Size");
                    fin >> crop_height >> crop_width;
                }
            }
        }
    }conf;
    
    void set_model(const int GPU_id=-1){
        if(GPU_id == -1){
            Caffe::set_mode(Caffe::CPU);
        }else{
            Caffe::SetDevice(GPU_id);
            Caffe::set_mode(Caffe::GPU);
        }
        cout << "GPU ID: " << GPU_id << endl;

        Alex_caffe_net.reset(new Net<float>(conf.prototxt , TEST ));
        Alex_caffe_net->CopyTrainedLayersFrom(conf.model);
        cout << "Test Net Init Done!" <<endl;

        cout << "CONF:" << endl;
        cout << conf.new_width << ", " << conf.new_height << endl;
        cout << conf.crop_width << ", " << conf.crop_height << endl;
    }
};

// PIPELINE CODE
class GazeTracker {
public:
    int input_width, input_height;
    int camera_id;

    struct camera {
        cv::Mat P, K;
        cv::Mat dist;
        cv::Mat R, rvec, tvec;
    };

    std::vector<camera> cameras;

private:
    float eye_width, eye_height;
    int frameIdx;

    tracking::Tracker_NoCvFace* tracker;
    GazeEstimator* gaze_estimator;

    vector<PostProcess> posts;

    cv::Mat avg_pose; // Recent Average Cloud
    float avg_pose_live = 0.0f;

    cv::Mat mean_pose; // Mean Head Cloud, put at ZERO
    float mean_pose_raw[21*3];

    cv::Point2f rotate2d(const cv::Point2f& inPoint, const double& angRad) {
        cv::Point2f outPoint;
        //CW rotation
        outPoint.x = std::cos(angRad)*inPoint.x - std::sin(angRad)*inPoint.y;
        outPoint.y = std::sin(angRad)*inPoint.x + std::cos(angRad)*inPoint.y;
        return outPoint;
    }

    cv::Point2f rotatePoint(const cv::Point2f& inPoint, const cv::Point2f& center, const double& angRad) {
        return rotate2d(inPoint - center, angRad) + center;
    }

    cv::Mat crop_one_eye(cv::Mat warpedImg, const cv::Point2f& pt,
                       const double& dist, const double& eye_wid, const double& eye_height) {
        if(pt.x-dist/2 < 0 || pt.x-dist/2+dist>input_width) return cv::Mat();
        if(pt.y-dist/4 < 0 || pt.y-dist/4+dist/2>input_height) return cv::Mat();
        cv::Mat ret = warpedImg(Rect(pt.x-dist/2,pt.y-dist/4,dist,dist/2));
        cv::resize(ret, ret, cv::Size(eye_wid, eye_height));
        ret.convertTo(ret, CV_8UC3);
        return ret;
    }

    bool crop_eyes( cv::Mat input_img, std::vector<cv::Point2f> landmarks,
                       std::pair<cv::Mat,cv::Mat> &eyes ) {

        cv::Point2f left = landmarks[6];
        cv::Point2f right = landmarks[9];
        double dist = cv::norm(left - right);

        //get angle
        const double theta = asin((right.y - left.y) / dist) * 180 / PI;
        cv::Mat H = getRotationMatrix2D(left, theta, 1);

        cv::Mat warpedImg;
        warpAffine(input_img, warpedImg, H, input_img.size());

        cv::Point2f mid_left = rotatePoint(landmarks[16], left, -theta/180.0*PI); //(landmarks[6]+landmarks[7])*0.5
        cv::Point2f mid_right = rotatePoint(landmarks[17], left, -theta/180.0*PI); //(landmarks[8]+landmarks[9])*0.5
        eyes.first = crop_one_eye(warpedImg, mid_left, dist/1.5, 
            eye_width, eye_height);
        eyes.second = crop_one_eye(warpedImg, mid_right, dist/1.5, 
            eye_width, eye_height);
        return (eyes.first.size().width != 0) && (eyes.second.size().width != 0);
        // TODO: add width, height scale, add scale as argument
    }

    void crop_eye_normalize(const cv::Mat &img_u,
                 const cv::Mat &et,  // 3D position of the eye in camera coordinate
                 const cv::Mat &cam,
				 const cv::Mat &dirx,
                 cv::Mat &img_warped,
                 cv::Mat &rotMat) //left = 1, right = 2
    {
        cv::Size roiSize(eye_width, eye_height);
        float focal_new, distance_new;
        focal_new = 1920.f;
        distance_new = 600.f;
		camPointTo(img_u, img_warped, rotMat, roiSize, dirx, et, cam, distance_new, focal_new);
		img_warped.convertTo(img_warped, CV_8UC3);
    }


public:
    GazeTracker() {
        this->tracker = new tracking::Tracker_NoCvFace();
        this->gaze_estimator = new GazeEstimator();
        this->cameras.resize(2);
		this->posts.resize(2);
        frameIdx = 0;
    }

    void Init(string conf) {
        std::ifstream fin(conf);
        string ID;
        // Camera Id
        fin >> ID;
        fin >> camera_id;

        // TrackerPath
        fin >> ID; cout << ID << endl;
        string track_path;
        fin >> track_path;
        cout << ID << track_path << endl;
        this->tracker->Init(track_path, 2); // , batch_size=2; For stereo
        this->tracker->SetMaxObjectCount(3);

        // GazePath
        string gaze_path;
        fin >> ID; cout << ID << endl;
        fin >> gaze_path;
        this->gaze_estimator->Set_Conf(gaze_path);
        this->gaze_estimator->Set_Model();

        //InputSize
        fin >> ID; cout << ID << endl;
        fin >> input_width >> input_height;

        // EyeSize
        fin >> ID; cout << ID << endl;
        fin >> eye_width >> eye_height;

        for(int cam=0; cam<2; cam++) {
            cv::Mat K, dist;
            float Kraw[9];
            float distraw[5];

            // K
            fin >> ID; cout << ID << endl;
            for( int i=0;i<9;i++ ) {
                fin >> Kraw[i];
            }
            K = cv::Mat(3,3,CV_32F,Kraw).clone();

            // DistCoef
            fin >> ID; cout << ID << endl;

            for( int i=0;i<5;i++ ) {
                fin >> distraw[i];
            }
            dist = cv::Mat(1,5,CV_32F,distraw).clone();

            // R
            float rvec_raw[3];
            cv::Mat rvec;
            fin >> ID; cout << ID << endl;
            for( int i=0;i<3;i++) {
                fin >> rvec_raw[i];
            }
            rvec = cv::Mat(1,3,CV_32F,rvec_raw);

            // t
            float trans_raw[3];
            cv::Mat tvec;
            fin >> ID; cout << ID << endl;
            for( int i=0;i<3;i++) {
                fin >> trans_raw[i];
            }
            tvec = cv::Mat(1,3,CV_32F,trans_raw);

            this->cameras[cam].K = K.clone();
            this->cameras[cam].dist = dist.clone();
            this->cameras[cam].rvec = rvec.clone();
            this->cameras[cam].tvec = tvec.clone();

            cv::Rodrigues(this->cameras[cam].rvec,
                          this->cameras[cam].R);

            cv::hconcat(this->cameras[cam].R,
                        this->cameras[cam].tvec.t(),
                        this->cameras[cam].P);
            this->cameras[cam].P = this->cameras[cam].K *
                    this->cameras[cam].P;
        }

        //Mean pose 21x3
        fin >> ID;

        for( int i=0;i<21;i++ ) {
            fin >> mean_pose_raw[i*3+0]
                >> mean_pose_raw[i*3+1]
                >> mean_pose_raw[i*3+2];
        }        
        mean_pose = cv::Mat(21,3,CV_32F,mean_pose_raw).clone();
        cout << ID << endl;
        cout << "INIT DATA ALL LOADED" << endl;
        fin.close();
    }

    void get3D( const vector<vector<cv::Point2f> > &pt_2d,
                const vector<camera> &cameras,
                const int& pt_idx,
                cv::Mat &pt_3d) {
        int numViews = 2;
        cv::Mat matrA(numViews * 2, 4, CV_32FC1);
        for (int i = 0; i < numViews; ++i) {
            const cv::Mat &P = cameras[i].P;
            const cv::Point2f &pt = pt_2d[i][pt_idx];
            matrA.row(2 * i) = pt.x * P.row(2) - P.row(0);
            matrA.row(2 * i + 1) = pt.y * P.row(2) - P.row(1);
        }
        cv::Mat matrW, matrU, matrV;
        cv::SVD::compute(matrA, matrW, matrU, matrV);
        pt_3d = matrV(cv::Range(3, 4), cv::Range(0, 3)) / matrV.at<float>(3, 3);
    }

    Gaze_t* detect_stereo(cv::Mat image_in_left, cv::Mat image_in_right) {
        
        frameIdx ++;

        Gaze_t* result = new Gaze_t();
        std::vector<cv::Mat> imgs(2);
        
        imgs[0] = image_in_left;
        imgs[1] = image_in_right;

        std::vector<std::vector<TrackInstance>> instances = this->tracker->Track(imgs);
        if( instances.size() != 2 ) {
            delete result;
            return NULL;
        }
        LOG(ERROR) << "Track Done";

        std::vector<std::vector<cv::Point2f>> pts(2);
        bool have_face[2] = {true, true};
        unsigned int face_flag = 0x00;
        for(int t=0; t<2; t++) {
			pts[t].clear();
            std::vector<TrackInstance> temp = instances[t]; // Only one image
			//if( temp == null ) {
			//	have_face[t] = false;
			//	continue;
			//}
            std::cout << "T=" << t << ", temp.size()=" << temp.size() << std::endl;
            int idx = -1;
            if( temp.size() == 0 ) {
				std::cout << "temp.size()=0" << std::endl;
                have_face[t] = false;
				posts[t].Reset();
                continue;
            } else if( temp.size() == 1 ) {
                idx = 0;
            } else { // MORE THAN ONE FACE
                float biggest_area = 0.0f;
                int biggest_idx = -1;
                for( int inst_id=0; inst_id < temp.size(); inst_id++) {
                    cv::Rect face_rect = LandmarksToRect(temp[inst_id].landmarks);
                    float area = face_rect.height * face_rect.width;
					std::cout << "area=" << area << std::endl;
                    if ( area > biggest_area ) { // Pick the one with the Largest face
                        biggest_area = area;
                        biggest_idx = inst_id;
                    }
                }
                idx = biggest_idx;
            }
			std::cout << "Pick Idx = " << idx << std::endl;
			if( idx == -1 ) {
				have_face[t] = false;
				continue;
			}
            tracking::TrackInstance instance = temp[idx];
            pts[t] = instance.landmarks;
            if(isnan(pts[t][0].x)) {
                have_face[t] = false;	
				posts[t].Reset();
                continue;
            }
            posts[t].Process(pts[t]);
            face_flag |= 1 << t;
        }
        LOG(ERROR) << "Face Select Done, Face Flag = " << face_flag;

        cv::Mat face_R, face_t;
        cv::Mat cloud( 21,3,CV_32F );
        vector<cv::Mat> img_patches(4); // Two images, 4 eyes

        if( face_flag == 0x00 ) { // NO FACE
			this->tracker->Reset();
            delete result;
            return NULL;
        } else if( face_flag != 3 ) {// ONLY ONE FACE
            LOG(ERROR) << "Find Only One Face";
            cv::Mat rvec, tvec;

            cv::Mat pose;
            int face_camera_Idx = -1;
            vector<cv::Point2f> pts_2d;
            if( avg_pose_live > 0 ) {
                pose = avg_pose;
                LOG(ERROR) << "Choose avg_pose";
            } else {
                pose = mean_pose;
                LOG(ERROR) << "Choose mean_pose";
            }
            for( int t=0; t<2; t++ ){
                if( (face_flag & 1<<t) ) {
                    face_camera_Idx = t;
                    pts_2d = pts[t];
                    break;
                }
            }
            LOG(ERROR) << "face_camera_Idx = " << face_camera_Idx;
            LOG(ERROR) << "pts_2d.size() = " << pts_2d.size();
            LOG(ERROR) << "pose.shape() = " << pose.size();

            static cv::Mat raux, taux;
            static bool useExtrinsicGuess = false;

            cv::solvePnP(pose, pts_2d,
                         this->cameras[face_camera_Idx].K,
                         this->cameras[face_camera_Idx].dist,
                         raux, taux, useExtrinsicGuess, CV_ITERATIVE);

            if (!useExtrinsicGuess) {
               useExtrinsicGuess = true;
            }

            raux.convertTo(rvec, CV_32F); // solvePnP get [double] result, convert to float
            taux.convertTo(tvec, CV_32F);

            cv::Mat R, hR;

            if( tvec.at<float>(2) < 0.0f ) {
                cv::Mat _comp_rvec, _comp_tvec;
                std::cout << "Below zero" << std::endl;
                cv::solvePnP(pose, pts_2d,
                         this->cameras[face_camera_Idx].K,
                         this->cameras[face_camera_Idx].dist,
                         _comp_rvec, _comp_tvec, false, CV_EPNP);

                cv::Rodrigues(rvec, R); // Here we get R, t of Head
                R.convertTo(R, CV_32F);

                std::cout << R << std::endl;
                std::cout << rvec << std::endl;
                std::cout << tvec << std::endl;

                cv::Rodrigues(_comp_rvec, R); // Here we get R, t of Head
                R.convertTo(R, CV_32F);

                std::cout << R << std::endl;
                std::cout << _comp_rvec << std::endl;
                std::cout << _comp_tvec << std::endl;

                cv::waitKey(0);
            }
            
            cv::Rodrigues(rvec, R); // Here we get R, t of Head
            R.convertTo(R, CV_32F);

            cloud = pose * R.t() + cv::repeat(tvec.t(),21,1);
            cloud = (cloud - cv::repeat(this->cameras[face_camera_Idx].tvec, 21, 1)) * this->cameras[face_camera_Idx].R;
            Mat re = 0.25*(cloud.row(9)+cloud.row(5)+cloud.row(8)+cloud.row(3));
            Mat le = 0.25*(cloud.row(6)+cloud.row(0)+cloud.row(7)+cloud.row(2));
            cv::Mat mid_eye = (re + le) * 0.5;
            Mat mid_mouth = 0.5 * (cloud.row(19) + cloud.row(20));
            face_t = mid_eye;
            getHeadpose(re, le, mid_mouth, hR, 0.0f);
            face_R = hR;
        } else {
            LOG(ERROR) << "Find Two Face";
            // Calculate the 3D Cloud
            for( int i=0; i<21; i++) {
                cv::Mat row;
                get3D( pts, this->cameras, i, row);
                row.copyTo(cloud.row(i));
            }

            cv::Mat R, head_cloud0, head_cloud1;
            cv::Mat rvec, tvec;
            static cv::Mat raux_cam0, taux_cam0, raux_cam1, taux_cam1;
            static bool useExtrinsicGuess = false;

			if (avg_pose_live > 0) {
				Mat re = 0.25*(cloud.row(9)+cloud.row(5)+cloud.row(8)+cloud.row(3));
				Mat le = 0.25*(cloud.row(6)+cloud.row(0)+cloud.row(7)+cloud.row(2));
				cv::Mat mid_eye = (re + le) * 0.5;
				Mat mid_mouth = 0.5 * (cloud.row(19) + cloud.row(20));
				getHeadpose(re, le, mid_mouth, R, 0.0f);
				
				Mat current_cloud = (cloud - cv::repeat(mid_eye, 21, 1)) * R.t();
				float alpha = 0.01;  // a parameter defined as the ratio of current cloud in avg_pose
				avg_pose = alpha * current_cloud.clone() + (1 - alpha) * avg_pose;
				
				cv::solvePnP(avg_pose, pts[0],
							 this->cameras[0].K,
							 this->cameras[0].dist,
							 raux_cam0, taux_cam0, useExtrinsicGuess, CV_ITERATIVE);
				cv::solvePnP(avg_pose, pts[1],
							 this->cameras[1].K,
							 this->cameras[1].dist,
							 raux_cam1, taux_cam1, useExtrinsicGuess, CV_ITERATIVE);
				if (!useExtrinsicGuess) {
				   useExtrinsicGuess = true;
				}
				raux_cam0.convertTo(rvec, CV_32F); // solvePnP get [double] result, convert to float
				taux_cam0.convertTo(tvec, CV_32F);
				cv::Rodrigues(rvec, R); // Here we get R, t of Head
				R.convertTo(R, CV_32F);
				head_cloud0 = avg_pose * R.t() + cv::repeat(tvec.t(),21,1);
				head_cloud0 = (head_cloud0 - cv::repeat(this->cameras[0].tvec, 21, 1)) * this->cameras[0].R;

				raux_cam1.convertTo(rvec, CV_32F); // solvePnP get [double] result, convert to float
				taux_cam1.convertTo(tvec, CV_32F);
				cv::Rodrigues(rvec, R); // Here we get R, t of Head
				R.convertTo(R, CV_32F);
				head_cloud1 = avg_pose * R.t() + cv::repeat(tvec.t(),21,1);
				head_cloud1 = (head_cloud1 - cv::repeat(this->cameras[1].tvec, 21, 1)) * this->cameras[1].R;
				
				cloud = (head_cloud0 + head_cloud1) / 2;

			} else {
				avg_pose = cloud.clone();
			}
			avg_pose_live += 1.0f;
	
			Mat re = 0.25*(cloud.row(9)+cloud.row(5)+cloud.row(8)+cloud.row(3));
			Mat le = 0.25*(cloud.row(6)+cloud.row(0)+cloud.row(7)+cloud.row(2));
			cv::Mat mid_eye = (re + le) * 0.5;
			Mat mid_mouth = 0.5 * (cloud.row(19) + cloud.row(20));
			getHeadpose(re, le, mid_mouth, R, 0.0f);

            face_R = R;
            face_t = mid_eye;
        }

        result->cloud = cloud;

        cv::Scalar euler = eulerAngles(face_R);
        LOG(ERROR) << "Euler Angle Done";
        stringstream ss;
        for( int i=0; i<3; i++) {
            result->head_angle[i] = (float) euler.val[i];       // copy euler angle to result
            result->head_coordinate[i] = face_t.at<float>(0,i);   // copy translation
        }

		cv::Mat face_R_cam = this->cameras[0].R * face_R;
		cv::Mat face_t_cam = face_t + this->cameras[0].tvec;
		cv::Scalar euler_cam = eulerAngles(face_R_cam);
        for( int i=0; i<3; i++) {
            result->head_angle_cam1[i] = (float) euler_cam.val[i];       // copy euler angle to result
            result->head_coordinate_cam1[i] = face_t_cam.at<float>(0,i);   // copy translation
        }
		face_R_cam = this->cameras[1].R * face_R;
		face_t_cam = face_t + this->cameras[1].tvec;
		euler_cam = eulerAngles(face_R_cam);
        for( int i=0; i<3; i++) {
            ss << (float) euler.val[i]/PI*180.0f << " ";
            result->head_angle_cam2[i] = (float) euler_cam.val[i];       // copy euler angle to result
            result->head_coordinate_cam2[i] = face_t_cam.at<float>(0,i);   // copy translation
        }
		
		std::vector<cv::Mat> eye_patches;
        bool is_crop_success;
		bool crop_MPII = true;
		if (crop_MPII) {
			is_crop_success = true;
			std::vector<cv::Mat> rotMat;
			for (int t = 0; t < 4; t++) {
				if (have_face[t/2]) {
					std::cout << "Crop " << t << std::endl;
					cv::Mat center, img_warped, rot, headX;
					if (t%2==1) {
						center = this->cameras[t/2].R * cloud.row(17).t() + this->cameras[t/2].tvec.t();
					} else {
						center = this->cameras[t/2].R * cloud.row(16).t() + this->cameras[t/2].tvec.t();
					}
					headX = this->cameras[t/2].R * face_R.row(0).t();
					crop_eye_normalize(imgs[t/2], center, this->cameras[t/2].K, headX, img_warped, rot);
					if (t%2==1) cv::flip(img_warped, img_warped, 1);
					eye_patches.push_back(img_warped);
					rotMat.push_back(rot);
				}
			}
		} else {
			is_crop_success = false;
			std::vector<std::pair<cv::Mat, cv::Mat>> eye_patch_pairs(2);
			for (int t = 0; t < 2 ; t++) {
				if (have_face[t]) {
					std::cout << "Crop " << t << std::endl;
					bool current_success = crop_eyes(imgs[t], pts[t], eye_patch_pairs[t]); // Get Eye Patch
					is_crop_success |= current_success;
					eye_patches.push_back(eye_patch_pairs[t].first);
					cv::Mat temp_right;
					cv::flip(eye_patch_pairs[t].second, temp_right, 1);
					eye_patches.push_back(temp_right);
				}
			}
		}

		if( is_crop_success ) {
            cv::Mat eye_grey;
            std::pair<float, float> gaze_angle = std::make_pair(0.0f,0.0f);
            for(int ei=0; ei<eye_patches.size(); ei++) {
                cv::cvtColor(eye_patches[ei], eye_grey, CV_BGR2GRAY);
                std::pair<float, float> current_gaze_angle = this->gaze_estimator->Predict(eye_grey);
				cout << current_gaze_angle.first << "," << current_gaze_angle.second << endl;
                if(ei%2==1) current_gaze_angle.first = -current_gaze_angle.first;
                gaze_angle.first += current_gaze_angle.first;
                gaze_angle.second += current_gaze_angle.second;
//                cout << current_gaze_angle.first << "," << current_gaze_angle.second << endl;
            }
            result->gaze_longitude = (gaze_angle.first/eye_patches.size())/180.0f*PI;
            result->gaze_latitude = (gaze_angle.second/eye_patches.size())/180.0f*PI;
            std::cout << "Angle:" << result->gaze_longitude << " " << result->gaze_latitude << std::endl;

            float local_gaze_vector[3];
            local_gaze_vector[0] = -cos(result->gaze_latitude)*sin(result->gaze_longitude);
            local_gaze_vector[1] = sin(result->gaze_latitude);
            local_gaze_vector[2] = -cos(result->gaze_latitude)*cos(result->gaze_longitude);
            std::cout << "Local Vector: " << local_gaze_vector[0] << " " << local_gaze_vector[1] << " " << local_gaze_vector[2] << std::endl;
            cv::Mat gaze_vector_mat(3,1,CV_32F,local_gaze_vector);
            gaze_vector_mat = face_R.t() * gaze_vector_mat;
            for(int i=0; i<3; i++) {
                result->gaze_vector[i] = gaze_vector_mat.at<float>(i);
            }
			std::cout << "fill gaze vector" << std::endl;
            cv::Mat eye_debug(100, eye_patches.size()*200, eye_patches[0].type());
            for(int p=0; p<eye_patches.size(); p++) {
                eye_patches[p].copyTo(eye_debug(cv::Rect(p*200,0,200,100)));
            }
            cv::imshow("eye",eye_debug);
        }

        cv::Mat landmark_debug(540,960*2,image_in_left.type());

        for(int p=0;p<21;p++) {
			if( have_face[0] ) 
	            cv::circle(image_in_left, pts[0][p], 2, Scalar(0,255,0), 2);
			if( have_face[1] )
	            cv::circle(image_in_right, pts[1][p], 2, Scalar(0,255,0), 2);
        }

        Mat re = cloud.row(6);
        Mat le = cloud.row(9);
        cv::Mat mid_eye = (re + le) * 0.5;
        cv::Mat axis_mat(5,3,CV_32F);
        mid_eye.copyTo(axis_mat.row(0));

        cv::Mat normal_alt;
        getHeadNormal(cloud, normal_alt);

        for(int a=0; a<3; a++) {
            cv::Mat temp(1,3,CV_32F);
            temp = mid_eye + face_R.row(a)*300;
            temp.copyTo(axis_mat.row(a+1));
        }
        cv::Mat normal_alt_ext = mid_eye + normal_alt*300;
        normal_alt_ext.copyTo(axis_mat.row(4));

        result->axis = axis_mat;
		
		std::cout << "Axis fill done" << std::endl;
				
        for(int t=0; t<2; t++) {
            cv::Mat cloud_c = (cloud * cameras[t].R.t() +
                                cv::repeat(cameras[t].tvec,21,1));

            cv::Mat cloud_pt = cameras[t].K * cloud_c.t();
            vector<cv::Point2f> axis_2d_pt;
            cv::projectPoints(axis_mat, cameras[t].rvec, cameras[t].tvec, cameras[t].K, cameras[t].dist,
                              axis_2d_pt);

            if( is_crop_success ) {
                cv::Mat gaze_vector_mat = cv::Mat(1,3,CV_32F);
                gaze_vector_mat.at<float>(0) = result->gaze_vector[0];
                gaze_vector_mat.at<float>(1) = result->gaze_vector[1];
                gaze_vector_mat.at<float>(2) = result->gaze_vector[2];
                gaze_vector_mat *= 150;
                gaze_vector_mat += mid_eye;
                vector<cv::Point2f> gaze_2d_pt;
                cv::projectPoints(gaze_vector_mat, cameras[t].rvec, cameras[t].tvec, cameras[t].K, cameras[t].dist,
                                  gaze_2d_pt);
                if( t==0) {
                    cv::line(image_in_left, axis_2d_pt[0], gaze_2d_pt[0], Scalar(255, 255, 255), 2);
                } else {
                    cv::line(image_in_right, axis_2d_pt[0], gaze_2d_pt[0], Scalar(255, 255, 255), 2);
                }
            }

            if( t==0) {
                cv::line(image_in_left, axis_2d_pt[0], axis_2d_pt[1], Scalar(0,0,255), 2);
                cv::line(image_in_left, axis_2d_pt[0], axis_2d_pt[2], Scalar(0,255,0), 2);
                cv::line(image_in_left, axis_2d_pt[0], axis_2d_pt[3], Scalar(255,0,0), 2);
                cv::line(image_in_left, axis_2d_pt[0], axis_2d_pt[4], Scalar(255,128,0), 2);
                for(int p=0; p<21; p++) {
                    cv::Mat pt = cloud_pt.col(p);
                    cv::circle(image_in_left, cv::Point2f(pt.at<float>(0)/pt.at<float>(2), pt.at<float>(1)/pt.at<float>(2)), 2 , Scalar(255,0,255), 2);
                }
            } else {
                cv::line(image_in_right, axis_2d_pt[0], axis_2d_pt[1], Scalar(0,0,255), 2);
                cv::line(image_in_right, axis_2d_pt[0], axis_2d_pt[2], Scalar(0,255,0), 2);
                cv::line(image_in_right, axis_2d_pt[0], axis_2d_pt[3], Scalar(255,0,0), 2);
                cv::line(image_in_right, axis_2d_pt[0], axis_2d_pt[4], Scalar(255,128,0), 2);
                for(int p=0; p<21; p++) {
                    cv::Mat pt = cloud_pt.col(p);
                    cv::circle(image_in_right, cv::Point2f(pt.at<float>(0)/pt.at<float>(2), pt.at<float>(1)/pt.at<float>(2)), 2 , Scalar(255,0,255), 2);
                }
            }

        }
        cv::Mat small;
        cv::resize(image_in_left, small, cv::Size(960,540));
        small.copyTo(landmark_debug(cv::Rect(0,0,960,540)));
        cv::resize(image_in_right, small, cv::Size(960,540));
        small.copyTo(landmark_debug(cv::Rect(960,0,960,540)));

        cv::Mat debug(540+30,960*2+30,CV_8UC3);
        debug = cv::Scalar(0,0,0);
        landmark_debug.copyTo(debug(cv::Rect(0,0,960*2,540)));
        if( is_crop_success ) {
            cv::rectangle(debug,
                          Point2f(960*2,270),
                          Point2f(960*2+30,270+540*(result->gaze_latitude)/PI), cv::Scalar(0,255,0), -1);
            cv::rectangle(debug,
                          Point2f(960,540),
                          Point2f(960-960*2*(- result->gaze_longitude*2/PI),960+30), cv::Scalar(0,255,0), -1);
        }
        cv::putText(debug,
                    ss.str(),
                    Point2f(30,30),
                    FONT_HERSHEY_PLAIN,
                    2, Scalar(0,255,0)
        );
        cv::imshow("landmark", debug);

        return result;
    }

    Gaze_t* detect_mono(cv::Mat image_in) {
        // INPUT
        std::vector<cv::Mat> imgs(2);
        imgs[0] = image_in;
        imgs[1] = image_in;

        Gaze_t* result = new Gaze_t();

        // DETECT & ALIGNMENT
        std::vector<std::vector<TrackInstance>> instances = this->tracker->Track(imgs);
        if( instances.size() != 2 ) {
            delete result;
            return NULL;
        }
        std::vector<TrackInstance> temp = instances[0]; // Only one image
LOG(INFO) << "Tracking Done";
        // SELECT A FACE
        int idx = -1;
        if( temp.size() == 0 ) {
            return NULL; // NO FACE
        } else if( temp.size() == 1 ) {
            idx = 0;
        } else { // MORE THAN ONE FACE
            float biggest_area = 0.0f;
            int biggest_idx = -1;
            for( int inst_id=0; inst_id < temp.size(); inst_id++) {
                cv::Rect face_rect = LandmarksToRect(temp[inst_id].landmarks);
                float area = face_rect.height * face_rect.width;
                if ( area > biggest_area ) { // Pick the one with the Largest face
                    biggest_area = area;
                    biggest_idx = inst_id;
                }
            }
            idx = biggest_idx;
        }
        tracking::TrackInstance instance = temp[idx];
        
        // cv::imshow("landmark", debug_img);
        // LOG(INFO) << "Tracking Done";

        // SOLVE PNP
        cv::Mat rvec, tvec;
        cv::solvePnP(mean_pose, instance.landmarks,
                     cameras[0].K, cameras[0].dist,
                     rvec, tvec, false, CV_EPNP);

        rvec.convertTo(rvec, CV_32F); // solvePnP get [double] result, convert to float
        tvec.convertTo(tvec, CV_32F);

        cv::Mat R;
        cv::Rodrigues(rvec, R); // Here we get R, t of Head
        cv::Scalar euler = eulerAngles(R);
LOG(INFO) << "Euler Angle Done";
        // LOG(INFO) << "Euler Angle Done";
        stringstream ss;
        for( int i=0; i<3; i++) {
            ss << (float) euler.val[i] << " ";
            result->head_angle[i] = (float) euler.val[i];       // copy euler angle to result
            result->head_coordinate[i] = tvec.at<float>(0,i);   // copy translation
        }

		cv::Mat face_R_cam = cameras[0].R * R;
		cv::Mat face_t_cam = cameras[0].tvec + tvec;
		cv::Scalar euler_cam = eulerAngles(face_R_cam);
        for( int i=0; i<3; i++) {
			result->head_angle_cam1[i] = (float) euler_cam.val[i];
			result->head_coordinate_cam1[i] = face_t_cam.at<float>(0,i);
		}

        std::pair<cv::Mat, cv::Mat> eyes_patch;
        bool is_crop_success = crop_eyes(image_in, instance.landmarks, eyes_patch); // Get Eye Patch
LOG(INFO) << "Crop Eye Done" << eyes_patch.first.size();
        if( is_crop_success ) {
            cv::Mat eye_grey;
            cv::cvtColor(eyes_patch.first, eye_grey, CV_BGR2GRAY);
            std::pair<float, float> gaze_angle = 
                this->gaze_estimator->Predict(eye_grey);
            result->gaze_longitude = gaze_angle.first;
            result->gaze_latitude = gaze_angle.second;
            float local_gaze_vector[3];
        // minus to fit our definition of head coordinate
            local_gaze_vector[0] = -cos(result->gaze_latitude)*sin(result->gaze_longitude);
            local_gaze_vector[1] = sin(result->gaze_latitude);
            local_gaze_vector[2] = -cos(result->gaze_latitude)*cos(result->gaze_longitude);

            cv::Mat gaze_vector_mat(1,3,CV_32F,local_gaze_vector);
            gaze_vector_mat = gaze_vector_mat*R.t();
            for(int i=0; i<3; i++) {
                result->gaze_vector[i] = gaze_vector_mat.at<float>(0,i);
            }

            cv::Mat eye_debug(eyes_patch.first.size().height,
                eyes_patch.first.size().width*2, eyes_patch.first.type());

            eyes_patch.first.copyTo(
                eye_debug(cv::Rect(0,0,
                eyes_patch.first.size().width, eyes_patch.first.size().height)));
            eyes_patch.second.copyTo(
                eye_debug(cv::Rect(eyes_patch.second.size().width,0,
                eyes_patch.second.size().width, eyes_patch.second.size().height)));
            cv::imshow("eye", eye_debug);

        } else {
            result->gaze_longitude = nanf("");
            result->gaze_latitude = nanf("");
            for(int i=0; i<3; i++) {
                result->gaze_vector[i] = nanf("");
            }
        }
LOG(INFO) << "Eye Predict Done";
        // Predict the gaze angle
// DEBUG PART
        
        cv::Mat landmark_debug = image_in.clone();
        for(int p=0;p<21;p++) {
            cv::circle(landmark_debug, instance.landmarks[p], 2, Scalar(0,255,0), 1);
        }
        cv::Mat landmark_debug_small;
        cv::resize(landmark_debug, landmark_debug_small, Size(960,540));
        cv::Mat debug(540+30,960+30,CV_8UC3);
        debug = cv::Scalar(0,0,0);
        landmark_debug_small.copyTo(debug(cv::Rect(0,0,960,540)));
        if( is_crop_success ) {
            cv::rectangle(debug, 
                Point2f(960,270),
                Point2f(960+30,270+540*(result->gaze_latitude)/180.0), cv::Scalar(0,255,0), -1);
            cv::rectangle(debug, 
                Point2f(480,540), 
                Point2f(480+960*(result->gaze_longitude/180.0),540+30), cv::Scalar(0,255,0), -1);
        }
        cv::putText(debug, 
            ss.str(), 
            Point2f(30,30), 
            FONT_HERSHEY_PLAIN,
            1, Scalar(255,0,0)
            );
        cv::imshow("landmark", debug);
        // LOG(INFO) << "Crop Eye Done";
        
        return result;
    }

    void free( Gaze_t* result ) {
        delete result;
    }

};

#endif //CALIBRATION_SCREEN_GAZETRACKER_H
