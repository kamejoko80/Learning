
#include <opencv2/opencv.hpp>

#include <iostream>
#include <fstream>
#include <ctime>
#include <chrono>

#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

using namespace std;
using namespace cv;

char inst[50];
char folder[260];
char folder_name[260];
pthread_mutex_t inst_mutex;
const int width = 1920;
const int height = 1080;

typedef std::chrono::high_resolution_clock Clock;
auto ts = std::chrono::steady_clock::now();
auto duration = std::chrono::steady_clock::now() - ts;
auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();

void GetTime(int &tm_mon, int &tm_day, int &tm_hour, int &tm_min, int &tm_sec)
{
    time_t t = time(0);   // get time now
    struct tm * now = localtime( & t );
	tm_mon = now->tm_mon + 1;
	tm_day = now->tm_mday;
	tm_hour = now->tm_hour;
	tm_min = now->tm_min;
	tm_sec = now->tm_sec;
}

void SendMessage(char *inst, const char *msg, pthread_mutex_t *mutex)
{
	pthread_mutex_lock(mutex);
	strcpy(inst, msg);
	pthread_mutex_unlock(mutex);
	cout << "send to thread sucessful" << endl;
}

void ReadOneCamera(VideoCapture *cap, Mat *img)
{
	std::stringstream ss;
	for (int i = 0; i < 1; i++) {
		cap->grab();
	}
    cap->retrieve(*img);
	duration = std::chrono::steady_clock::now() - ts;
	ms = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
	ss << ms/10 << " * 10 ms";
	putText(*img, ss.str(), cv::Point(100, 100), 1, 4, cv::Scalar(0,0,255), 4);
}

void ReadCameras(const vector<VideoCapture *> &caps, vector<Mat> &imgs)
{
	assert(caps.size() == imgs.size());
	for (size_t i = 0; i < caps.size(); i++)
		caps[i]->grab();
	for (size_t i = 0; i < caps.size(); i++)
		caps[i]->retrieve(imgs[i]);
}

void SaveImgs(const vector<Mat> &imgs, const string &folder, char *msg)
{
    char path[260];
    for (int i = 0; i < int(imgs.size()); i++) {
        sprintf(path, "%s/imgs/%s_cam%d.jpg", folder.c_str(), msg, i);
        imwrite(path, imgs[i]);
    }
}

void CopyImgs(const vector<Mat> &srcImgs, vector<Mat> &dstImgs)
{
	assert(srcImgs.size() == dstImgs.size());
	for (size_t i = 0; i < srcImgs.size(); i++) {
		srcImgs[i].copyTo(dstImgs[i]);
	}
}

void StopWatch(cv::Mat &win)
{
	cv::Mat stopWatch;
	stopWatch = win.clone();
	duration = std::chrono::steady_clock::now() - ts;
	ms = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
	std::stringstream ss;
	ss << ms/10;
	putText(stopWatch, ss.str(), cv::Point(10, 540), 1, 16, cv::Scalar(0,0,255), 8);
	imshow("win", stopWatch);
	cv::waitKey(50);
}

void *TakePicture(void *caps)
{
	int start = 0;
    char t_inst[50];
    vector<VideoCapture *> *cameras = (vector<VideoCapture *> *)caps;  
    int num_cam = cameras->size();
	vector<Mat> imgs(num_cam), records(num_cam);
	for (int i = 0;i < num_cam;i ++)
	{
		imgs[i].create(height, width, CV_8UC3);
		records[i].create(height, width, CV_8UC3);
	}

    while (1)
    {
		pthread_mutex_lock(&inst_mutex);
		if (strlen(inst) > 0)
		{
			strcpy(t_inst, inst);
			strcpy(inst, "\0");
		}
        pthread_mutex_unlock(&inst_mutex);
		if (strncmp(t_inst, "start",5) != 0 && start != 1)
		{
			continue;
		}

		cout << "receive start command\n" << endl;
		start = 1;
		while (1)
		{
	        for (int i = 0;i < num_cam;i ++)
	        {
	            ReadOneCamera((*cameras)[i], &imgs[i]);
	            pthread_mutex_lock(&inst_mutex);
            	strcpy(t_inst, inst);
		        strcpy(inst, "\0");
		        pthread_mutex_unlock(&inst_mutex);
	            if (strlen(t_inst) >= 0)
                {
                    if (strcmp(t_inst,"copy") == 0)
                    {
                        printf("receive copy command\n");
                        CopyImgs(imgs, records);
                    }
                    else if (strncmp(t_inst, "save", 4) == 0)
                    {
                        printf("receive save command\n");
                        SaveImgs(records, folder, t_inst+5);
                    }
                    else if (strcmp(t_inst, "end") == 0)
                    {
                        start = 0;
                        break;
                    }
                    else if (strcmp(t_inst, "exit") == 0)
                    {
                        return 0;
                    }
                }
			}
			if (start == 0)
				break;
		}
	}
	pthread_exit(NULL); 
}

int main(int argc, char **argv)
{
	if (5 != argc) {
		cout << "Usage: ./fixed-head folder_img num_cam #master #slaves_per_master" << endl;
		return -1;
	}
	int tm_mon, tm_day, tm_hour, tm_min, tm_sec;
	GetTime(tm_mon, tm_day, tm_hour, tm_min, tm_sec);
	sprintf(folder, "%s/%d_%d_%d_%d_%d", argv[1], tm_mon, tm_day, tm_hour, tm_min, tm_sec);
	const string folder_img(folder);

	sprintf(folder_name, "%d_%d_%d_%d_%d", tm_mon, tm_day, tm_hour, tm_min, tm_sec);
	const int num_cam = atoi(argv[2]);
	const int num_master = atoi(argv[3]);
	const int num_slave = atoi(argv[4]);
	const int clock_period = 400;
	system(("mkdir -p " + folder_img).c_str());
	system(("mkdir -p " + folder_img + "/imgs").c_str());
	const int minNum = 3;
	const int maxNum = 5;
	const int range_slave = 1000;
	pthread_t camera;

	//open cameras
	vector<VideoCapture *> caps(num_cam);
	for (int i = 0; i < num_cam; i++) {
		caps[i] = new VideoCapture(i);
		if (!caps[i]->isOpened()) {
			cout << "Failed to open camera " << i << endl;
			return -1;
		}
		caps[i]->set(CV_CAP_PROP_FRAME_WIDTH, width);
		caps[i]->set(CV_CAP_PROP_FRAME_HEIGHT, height);
	}

	// prepare showing window
	cv::Mat win(1080, 1920, CV_8UC3, Scalar(0,0,0));
	cv::namedWindow("win1", cv::WINDOW_NORMAL);
	cv::moveWindow("win1", 0, 0);
	cv::setWindowProperty("win1", cv::WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN);
	//cv::Mat win2(height, width, CV_8UC3);
	//cv::namedWindow("win2", cv::WINDOW_NORMAL);
	//cv::moveWindow("win2", width, 0);
	//cv::setWindowProperty("win2", cv::WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN);

	ts = std::chrono::steady_clock::now();
	// create socket
    char send_buf[20];
	// create take picture thread
	pthread_mutex_init(&inst_mutex, NULL);
	int rc;
	rc = pthread_create(&camera, NULL, TakePicture, (void *)(&caps));
	if (rc)
	{ 
	 printf("ERROR; return code from pthread_create() is %d\n", rc); 
	 exit(-1); 
	} 

	// begin record
	int key = -1;
	int pid = 0;
	while (true) {//iterate over person
		int id = 0;
		std::cout << "Pid="<<pid<<std::endl;
		while (id < num_master) 
		{   // iterate over marker positions
			std::cout << "MasterId="<<id<<std::endl;
			// check headpose
			SendMessage(inst, (char *)"end", &inst_mutex);
			key = cv::waitKey(3000);
			
			//check headpose code here
			SendMessage(inst, (std::string("start")+std::string(folder_name)).c_str(), &inst_mutex);

			// show slave point
			int s = 0;

			while( s < num_slave ) {
//			for (int s = 0; s < num_slave; s++) {
				//std::cout << "SlaveId="<<s<<", UniqueId="<<id*num_slave+s<<std::endl;	
				//random choose a slave position
				
				//show nontarget chars
				const int num_nontarget = rand() % (maxNum - minNum + 1) + minNum;
				for (int i = 0; i < num_nontarget; i++) {
					//record images
					if (i == num_nontarget - 1)
					{

						// inform the other thread and the other machine to copy imgs
						SendMessage(inst, (char *)"copy", &inst_mutex);
					}
					for (int sw = 0; sw < 100; sw++) {
						StopWatch(win);
					}
				}
		
				//show target chars
				for (int sw = 0; sw < 200; sw++) {
					StopWatch(win);
				}
				// inform the other thread and the other machine to save imgs
				char msg[50];
				sprintf(msg, "save_pid_%did_%d", pid, id*num_slave+s);
				// wait for edit
				SendMessage(inst, msg, &inst_mutex);

				for (int sw = 0; sw < 100; sw++) {
					StopWatch(win);
				}

				s++;
				if (key == 27) {
					SendMessage(inst, (char *)"exit", &inst_mutex);
					return 0;
				}
			} // while ( s < num_slave )
			id ++; // next master point
		} // while( id < num_master )
		pid++;
	}

	// exit
	SendMessage(inst, (char *)"exit", &inst_mutex);
}
