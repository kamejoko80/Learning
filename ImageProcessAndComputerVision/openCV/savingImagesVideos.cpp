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

    return 0;
}
