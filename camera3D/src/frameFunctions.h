#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;

void copy_frame(Mat *inFrame, Mat *outFrame)
{
	*outFrame = *inFrame;
}

void rgb_frame_to_Edges(Mat *inFrame, Mat *outFrame)
{
	cvtColor(*inFrame, *outFrame, COLOR_BGR2GRAY);
	GaussianBlur(*outFrame, *outFrame, Size(7, 7), 1.5, 1.5);
	Canny(*outFrame, *outFrame, 0, 30, 3);
}

void rgb_frame_reduce_Blue(Mat *inFrame, Mat *outFrame)
{ // tried to copy from Python Code. it doesn't outcome similar..
// need to copy from :
	// http://stackoverflow.com/questions/9018906/detect-rgb-color-interval-with-opencv-and-c 

	//# Convert BGR to HSV
	cvtColor(*inFrame, *outFrame, COLOR_BGR2HSV);  // hsv
	//# define range of blue color in HSV
	//int	lower_blue[3] = { 110,  50,  50 };
	Scalar lower_blue = Scalar( 110, 50, 50,0 );
	//int	upper_blue[3] = { 130, 255, 255 };
	Scalar upper_blue = Scalar(130, 250, 250, 0);
	//# Threshold the HSV image to get only blue colors
	inRange(*inFrame, lower_blue, upper_blue, *outFrame );   //mask

	//# Bitwise - AND mask and original image
	bitwise_and(*outFrame, *outFrame, *outFrame); // res
}


/* 

used tutorials:
 http://docs.opencv.org/3.0-beta/modules/videoio/doc/reading_and_writing_video.html
possible examples to check :
 C:\OpenCV_Source\samples\cpp\tutorial_code\objectDetection

*/