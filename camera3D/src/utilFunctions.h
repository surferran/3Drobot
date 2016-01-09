// main ref in : http://opencv-srf.blogspot.co.il/2010/09/object-detection-using-color-seperation.html
/*
Usually, one can think that BGR color space is more suitable for color based segmentation. 
But HSV color space is the most suitable color space for color based image segmentation. 
So, in the above application, I have converted the color space of original image of the video from BGR to HSV image
*/

int iLastX = -1;
int iLastY = -1;

void set_controls_gui()
{
	namedWindow("Control", CV_WINDOW_AUTOSIZE); //create a window called "Control"

	int iLowH	= 0;
	int iHighH	= 179;

	int iLowS	= 0;
	int iHighS	= 255;

	int iLowV	= 0;
	int iHighV	= 255;

	//Create trackbars in "Control" window
	cvCreateTrackbar("LowH"	, "Control", &iLowH	, 179); //Hue (0 - 179)
	cvCreateTrackbar("HighH", "Control", &iHighH, 179);

	cvCreateTrackbar("LowS"	, "Control", &iLowS	, 255); //Saturation (0 - 255)
	cvCreateTrackbar("HighS", "Control", &iHighS, 255);

	cvCreateTrackbar("LowV"	, "Control", &iLowV	, 255); //Value (0 - 255)
	cvCreateTrackbar("HighV", "Control", &iHighV, 255);
}

void check_for_tracking(Mat imgThresholded) {

	//Calculate the moments of the thresholded image
	Moments oMoments = moments(imgThresholded);

	double dM01 = oMoments.m01;
	double dM10 = oMoments.m10;
	double dArea = oMoments.m00;

	// if the area <= 10000, I consider that the there are no object in the image and it's because of the noise, the area is not zero 
	if (dArea > 10000)
	{
		//calculate the position of the ball
		int posX = dM10 / dArea;
		int posY = dM01 / dArea;

		if (iLastX >= 0 && iLastY >= 0 && posX >= 0 && posY >= 0)
		{
			//Draw a red line from the previous point to the current point
			////line(imgLines, Point(posX, posY), Point(iLastX, iLastY), Scalar(0, 0, 255), 2);
		}

		iLastX = posX;
		iLastY = posY;
	}

	imshow("Thresholded Image", imgThresholded); //show the thresholded image

	//imgOriginal = imgOriginal + imgLines;

}
