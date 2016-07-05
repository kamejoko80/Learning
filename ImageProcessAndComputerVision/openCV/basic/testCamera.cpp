#include "opencv2/highgui/highgui.hpp"
#include <stdio.h>
#include <iostream>

using namespace cv;
using namespace std;

void help()
{
    cout << "Usage: testCamera <device> <width> <height>" << endl;
    cout << "E.g.\n ./testCamera \n ./testCamera 0 \n ./testCamera 0 640 480 \n";
}

int main(int argc, char *argv[])
{
    int i, k;
    int device;
    int width, height;
    bool setsize = false;

    switch(argc)
    {
        case 1: device = 0;
                break;
        case 2: device = atoi(argv[1]);
                break;
        case 4: device = atoi(argv[1]);
                sscanf(argv[2], "%d", &width);
                sscanf(argv[3], "%d", &height);
                setsize = true;
                break;
        default: help(); return 1;
    }
    // open the video camera no. 0
    VideoCapture cap(0);
    if (setsize)
    {
        cap.set(CV_CAP_PROP_FRAME_WIDTH, width);
        cap.set(CV_CAP_PROP_FRAME_HEIGHT, height);
    }
    //get the width of frames of the video
    double dWidth = cap.get(CV_CAP_PROP_FRAME_WIDTH);
    //get the height of frames of the video
    double dHeight = cap.get(CV_CAP_PROP_FRAME_HEIGHT);

    if (!cap.isOpened())
    {
        cout << "Cannot open the video file" << endl;
        return -1;
    }
    // get the frames per seconds of the video
    double fps = cap.get(CV_CAP_PROP_FPS);
    cout << "Frame per seconds : " << fps << endl;
    cout << "Frame size : " << dWidth << " x " << dHeight << endl;
    //namedWindow("MyVideo", CV_WINDOW_NORMAL);
    while(1)
    {
        Mat frame;
        // read a new frame from video
        // cap>>frame;
        bool bSuccess = cap.read(frame);
        if (!bSuccess)
        {
            cout << "Cannot read the frame from video file" << endl;
            break;
        }
        // show the frame in "MyVideo" window
        imshow("MyVideo", frame);
        // if 'esc' key is pressed, break the loop.
        if (waitKey(30) == 27)
        {
            cout << "esc key is pressed by user" << endl;
            break;
        }
    }

    return 0;
}
