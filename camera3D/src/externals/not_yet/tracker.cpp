#include <iostream>

#include<opencv/cvaux.h>
#include<opencv/highgui.h>
#include<opencv/cxcore.h>

#include<stdio.h>
#include<stdlib.h>

// Need to include this for serial port communication
#include <Windows.h>

///////////////////////////////////////////////////////////////////////////////////////////////////
int main(int argc, char* argv[])
{
	// Setup serial port connection and needed variables.
	HANDLE hSerial = CreateFile(L"COM3", GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if (hSerial !=INVALID_HANDLE_VALUE)
    {
        printf("Port opened! \n");

        DCB dcbSerialParams;
        GetCommState(hSerial,&dcbSerialParams);

        dcbSerialParams.BaudRate = CBR_9600;
        dcbSerialParams.ByteSize = 8;
        dcbSerialParams.Parity = NOPARITY;
        dcbSerialParams.StopBits = ONESTOPBIT;

        SetCommState(hSerial, &dcbSerialParams);
	}
	else
	{
		if (GetLastError() == ERROR_FILE_NOT_FOUND)
		{
			printf("Serial port doesn't exist! \n");
		}

		printf("Error while setting up serial port! \n");
	}

	char outputChars[] = "c";
	DWORD btsIO;

	// Setup OpenCV variables and structures
	CvSize size640x480 = cvSize(640, 480);			// use a 640 x 480 size for all windows, also make sure your webcam is set to 640x480 !!

	CvCapture* p_capWebcam;						// we will assign our web cam video stream to this later . . .

	IplImage* p_imgOriginal;			// pointer to an image structure, this will be the input image from webcam
	IplImage* p_imgProcessed;			// pointer to an image structure, this will be the processed image
	IplImage* p_imgHSV;                 // pointer to an image structure, this will hold the image after the color has been changed from RGB to HSV
										// IPL is short for Intel Image Processing Library, this is the structure used in OpenCV 1.x to work with images

	CvMemStorage* p_strStorage;			// necessary storage variable to pass into cvHoughCircles()

	CvSeq* p_seqCircles;				// pointer to an OpenCV sequence, will be returned by cvHough Circles() and will contain all circles
										// call cvGetSeqElem(p_seqCircles, i) will return a 3 element array of the ith circle (see next variable)
	
	float* p_fltXYRadius;				// pointer to a 3 element array of floats
										// [0] => x position of detected object
										// [1] => y position of detected object
										// [2] => radius of detected object

	int i;								// loop counter
	char charCheckForEscKey;			// char for checking key press (Esc exits program)

	p_capWebcam = cvCaptureFromCAM(0);	// 0 => use 1st webcam, may have to change to a different number if you have multiple cameras

	if(p_capWebcam == NULL) {			// if capture was not successful . . .
		printf("error: capture is NULL \n");	// error message to standard out . . .
		getchar();								// getchar() to pause for user see message . . .
		return(-1);								// exit program
	}

											            // declare 2 windows
	cvNamedWindow("Original", CV_WINDOW_AUTOSIZE);		// original image from webcam
	cvNamedWindow("Processed", CV_WINDOW_AUTOSIZE);		// the processed image we will use for detecting circles

	p_imgProcessed = cvCreateImage(size640x480,			// 640 x 480 pixels (CvSize struct from earlier)
								   IPL_DEPTH_8U,		// 8-bit color depth
								   1);					// 1 channel (grayscale), if this was a color image, use 3

	p_imgHSV = cvCreateImage(size640x480, IPL_DEPTH_8U, 3); 

	// Variables for Arduino Control
	int servoPosition = 90;
	int servoOrientation = 0;

	// Main program loop
	while(1) {								// for each frame . . .
		p_imgOriginal = cvQueryFrame(p_capWebcam);		// get frame from webcam
		
		if(p_imgOriginal == NULL) {					// if frame was not captured successfully . . .
			printf("error: frame is NULL \n");		// error message to std out
			getchar();
			break;
		}

		// Change the color model from RGB (BGR) to HSV. This makes it easier to choose a color based on Hue
		cvCvtColor(p_imgOriginal, p_imgHSV, CV_BGR2HSV);

		cvInRangeS(p_imgHSV,				// function input
				   cvScalar(38,  60,  70),			// min filtering value (if color is greater than or equal to this)
				   cvScalar(75, 181, 256),			// max filtering value (if color is less than this)
				   p_imgProcessed);				// function output

		p_strStorage = cvCreateMemStorage(0);	// allocate necessary memory storage variable to pass into cvHoughCircles()

										// smooth the processed image, this will make it easier for the next function to pick out the circles
		cvSmooth(p_imgProcessed,		// function input
				 p_imgProcessed,		// function output
				 CV_GAUSSIAN,			// use Gaussian filter (average nearby pixels, with closest pixels weighted more)
				 9,						// smoothing filter window width
				 9);					// smoothing filter window height

													// fill sequential structure with all circles in processed image
		p_seqCircles = cvHoughCircles(p_imgProcessed,		// input image, nothe that this has to be grayscale (no color)
									  p_strStorage,			// provide function with memory storage, makes function return a pointer to a CvSeq
									  CV_HOUGH_GRADIENT,	// two-pass algorithm for detecting circles, this is the only choice available
									  2,					// size of image / 2 = "accumulator resolution", i.e. accum = res = size of image / 2
									  p_imgProcessed->height / 4,	// min distance in pixels between the centers of the detected circles
									  100,						// high threshold of Canny edge detector, called by cvHoughCircles
									  50,						// low threshold of Canny edge detector, called by cvHoughCircles
									  10,	 //10					// min circle radius, in pixels
									  400);						// max circle radius, in pixels

		// Run this if the camera doesn't detect any circles
		if (p_seqCircles->total == 0)
		{
			// Initialize orientation
			// This just makes it so the camera first goes to the side that it's leaning towards
			// So if the camera is already mostly facing the left side it goes to the left end
			// before going to the right. And the other way around.
			if (servoOrientation == 0)
			{
				if (servoPosition >= 90)
					servoOrientation = 1;
				else
					servoOrientation = -1;
			}

			if (servoOrientation == 1)
			{
				outputChars[0] = 'l';
				WriteFile(hSerial, outputChars, strlen(outputChars), &btsIO, NULL);

				// This code is identical to the one on the Arduino side
				servoPosition+=5;

				if (servoPosition > 180)
				{
					servoPosition = 180;
					servoOrientation = -1;
				}
			}
			else
			{
				outputChars[0] = 'r';
				WriteFile(hSerial, outputChars, strlen(outputChars), &btsIO, NULL);

				// This code is identical to the one on the Arduino side
				servoPosition-=5;

				if (servoPosition < 0)
				{
					servoPosition = 0;
					servoOrientation = 1;
				}
			}
		}

		// Run this if the camera can see at least one circle
		for(i=0; i < p_seqCircles->total; i++) {		// for each element in sequential circles structure (i.e. for each object detected)

			p_fltXYRadius = (float*)cvGetSeqElem(p_seqCircles, i);	// from the sequential structure, read the ith value into a pointer to a float

			printf("ball position x = %f, y = %f, r = %f \n", p_fltXYRadius[0],		// x position of center point of circle
															  p_fltXYRadius[1],		// y position of center point of circle
															  p_fltXYRadius[2]);	// radius of circle

			// Reset servo orientation as the camera now has focus of a circle
			// Servo orientation is important only when the camera doesn't see a circle
			servoOrientation = 0;

			// Check whether camera should turn to its left if the circle gets near the right end of the screen
			if (p_fltXYRadius[0] > 540)
			{
				outputChars[0] = 'l';
				WriteFile(hSerial, outputChars, strlen(outputChars), &btsIO, NULL);

				servoPosition+=5;

				if (servoPosition > 180)
					servoPosition = 180;
			}

			// Check whether camera should turn to its right if the circle gets near the left end of the screen
			if (p_fltXYRadius[0] < 100)
			{
				outputChars[0] = 'r';
				WriteFile(hSerial, outputChars, strlen(outputChars), &btsIO, NULL);

				servoPosition-=5;

				if (servoPosition < 0)
					servoPosition = 0;
			}

										// draw a small green circle at center of detected object
			cvCircle(p_imgOriginal,										// draw on the original image
					 cvPoint(cvRound(p_fltXYRadius[0]), cvRound(p_fltXYRadius[1])),		// center point of circle
					 3,													// 3 pixel radius of circle
					 CV_RGB(0,255,0),									// draw pure green
					 CV_FILLED);										// thickness, fill in the circle
			
										// draw a red circle around the detected object
			cvCircle(p_imgOriginal,										// draw on the original image
					 cvPoint(cvRound(p_fltXYRadius[0]), cvRound(p_fltXYRadius[1])),		// center point of circle
					 cvRound(p_fltXYRadius[2]),							// radius of circle in pixels
					 CV_RGB(255,0,0),									// draw pure red
					 3);												// thickness of circle in pixels
		}	// end for

		cvShowImage("Original", p_imgOriginal);			// original image with detectec ball overlay
		cvShowImage("Processed", p_imgProcessed);		// image after processing

		cvReleaseMemStorage(&p_strStorage);				// deallocate necessary storage variable to pass into cvHoughCircles

		charCheckForEscKey = cvWaitKey(10);				// delay (in ms), and get key press, if any
		if(charCheckForEscKey == 27) break;				// if Esc key (ASCII 27) was pressed, jump out of while loop
	}	// end while

	cvReleaseCapture(&p_capWebcam);					// release memory as applicable

	cvDestroyWindow("Original");
	cvDestroyWindow("Processed");

	// This closes the Serial Port
    CloseHandle(hSerial);

	return(0);
}