#include "opencv2/highgui/highgui.hpp"
#include <iostream>


using namespace cv;
using namespace std;

int main(int argc, const char **argv)
{
    // create an image (3 channels, 16 bit image depth,
    // 650 high, 600 wide, (0, 50000, 50000)) assigned for
    // Blue, Green and Red plane respectively.)
    Mat img(650, 600, CV_16UC3, Scalar(0, 50000, 50000));

    if (img.empty())
    {
        cout << "ERROR : Image cannot be loaded..!!" << endl;
        return -1;
    }

    // vector that stores the compression parameters of the image
    vector<int> compression_params;
    
    // specify the compression technique
    compression_params.push_back(CV_IMWRITE_JPEG_QUALITY);
    // specify the compression quality
    compression_params.push_back(98);

    // write the image to file
    bool bSuccess = imwrite("./testImage.jpg", img, compression_params);

    if (!bSuccess)
    {
        cout << "ERROR : Failed to save the image" << endl;
    }

    // create a window with the name "MyWindow"
    namedWindow("MyWindow", CV_WINDOW_AUTOSIZE);
    // display the image which is stored in the 'img' in the "MyWindow" window
    imshow("MyWindow", img);

    waitKey(0);

    destroyWindow("MyWindow");
    

    // write video to file
    VideoCapture cap(0); // open the video camera no. 0

    if (!cap.isOpened())  // if not success, exit program
    {
        cout << "ERROR: Cannot open the video file" << endl;
        return -1;
    }

    namedWindow("MyVideo",CV_WINDOW_AUTOSIZE); //create a window called "MyVideo"
     
    double dWidth = cap.get(CV_CAP_PROP_FRAME_WIDTH); //get the width of frames of the video
    double dHeight = cap.get(CV_CAP_PROP_FRAME_HEIGHT); //get the height of frames of the video

    cout << "Frame Size = " << dWidth << "x" << dHeight << endl;

    Size frameSize(static_cast<int>(dWidth), static_cast<int>(dHeight));

    VideoWriter oVideoWriter ("./MyVideo.avi", CV_FOURCC('P','I','M','1'), 20, frameSize, true); //initialize the VideoWriter object 

    if ( !oVideoWriter.isOpened() ) //if not initialize the VideoWriter successfully, exit the program
    {
        cout << "ERROR: Failed to write the video" << endl;
        return -1;
    }
    
    while (1)
    {
        Mat frame;

        bool bSuccess = cap.read(frame); // read a new frame from video

        if (!bSuccess) //if not success, break loop
        {
            cout << "ERROR: Cannot read a frame from video file" << endl;
            break;
        }
        
        oVideoWriter.write(frame); //writer the frame into the file
        
        imshow("MyVideo", frame); //show the frame in "MyVideo" window

        if (waitKey(10) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
        {
            cout << "esc key is pressed by user" << endl;
            break; 
        }
    }

    return 0;
}
