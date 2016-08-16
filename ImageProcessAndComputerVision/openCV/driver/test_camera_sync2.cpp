
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

const int width = 1920;
const int height = 1080;

typedef std::chrono::high_resolution_clock Clock;
auto ts = std::chrono::steady_clock::now();
auto duration = std::chrono::steady_clock::now() - ts;
auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();

int main(int argc, char **argv)
{
	VideoCapture cap(0);
	if (!cap.isOpened()) {
		cout << "Failed to open camera " << endl;
		return -1;
	}
	cap.set(CV_CAP_PROP_FRAME_WIDTH, width);
	cap.set(CV_CAP_PROP_FRAME_HEIGHT, height);

	cv::Mat frame;

	for (int i = 0; i < 10; i++) {
		cap >> frame;
	}
	while (1) {
		ts = std::chrono::steady_clock::now();

		//cap->grab();
		//cap->retrieve(frame);
		cout << "read one frame." << endl;
		cap >> frame;
		//std::stringstream ss;
		//ss << ms/10 << " * 10 ms";
		duration = std::chrono::steady_clock::now() - ts;
		ms = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
		cout << ms << "ms" << endl;
		cv::waitKey(100);
		
		for (int i = 0; i < 20; i++) {
			ts = std::chrono::steady_clock::now();
			//cap >> frame;
			cap.grab();
			cap.retrieve(frame);
			duration = std::chrono::steady_clock::now() - ts;
			ms = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
			cout << ms << "ms" << endl;
		}
	}

}
