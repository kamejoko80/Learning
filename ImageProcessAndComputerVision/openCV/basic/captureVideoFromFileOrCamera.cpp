#include "opencv2/highgui/highgui.hpp"
#include <iostream>

using namespace cv;
using namespace std;

int main(int argc, char *argv[])
{
    // open the video camera no. 0
    VideoCapture cap(0);
    // open the video file
    //VideoCapture cap("./samples/data/768x576.avi");

    if (!cap.isOpened())
    {
        cout << "Cannot open the video file" << endl;
        return -1;
    }

    // start the video at 300ms
    //cap.set(CV_CAP_PROP_POS_MSEC, 300); 

    // get the frames per seconds of the video
    double fps = cap.get(CV_CAP_PROP_FPS);
    cout << "Frame per seconds : " << fps << endl;
    
    //get the width of frames of the video
    double dWidth = cap.get(CV_CAP_PROP_FRAME_WIDTH);
    //get the height of frames of the video
    double dHeight = cap.get(CV_CAP_PROP_FRAME_HEIGHT);

    cout << "Frame size : " << dWidth << " x " << dHeight << endl;
    
    // create a window called "MyVideo"
    // WINDOW_AUTOSIZE
    // CV_WINDOW_NORMAL
    namedWindow("MyVieo", CV_WINDOW_AUTOSIZE);

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
        
        // wait for 'esc' key press for 30 ms.
        // the ASCII value of 'esc' key is 27.
        // if 'esc' key is pressed, break the loop.
        if (waitKey(30) == 27)
        {
            cout << "esc key is pressed by user" << endl;
            break;
        }
    }

    return 0;
}
