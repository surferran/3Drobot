/* main ref in : http://opencv-srf.blogspot.co.il/2010/09/object-detection-using-color-seperation.html
//				 https://learningopencv.wordpress.com/2011/05/29/example-2-3-playing-video-file-with-trackbar-position/ 
					http://docs.opencv.org/2.4/modules/highgui/doc/user_interface.html
					http://stackoverflow.com/questions/2974625/opencv-seek-function-rewind
					http://docs.opencv.org/2.4/modules/highgui/doc/qt_new_functions.html#createbutton

/*
Usually, one can think that BGR color space is more suitable for color based segmentation. 
But HSV color space is the most suitable color space for color based image segmentation. 
So, in the above application, I have converted the color space of original image of the video from BGR to HSV image
*/
//////////
//#include "c:\Users\Ran_the_User\Documents\GitHub\3Drobot\camera3D\src\stereo_calib.cpp"

//#include "C:\Users\Ran_the_User\Documents\GitHub\3Drobot\camera3D\src\stereo_calib.cpp"
////int do_stereo_calib(int argc, char** argv); // declare for the CPP already included file
//int do_stereo_calib(int argc, char** argv);

#include ".\stereo_calib.h"

/* constants */
enum USER_STATUS_SELECTION { CALIBRATION_RIGHT = 1,		// don't use it as a seperate mode
							 CALIBRATION_LEFT	,		// don't use it as a seperate mode
							 CALIBRATION_STEREO	,	// just use this one
							 STREAM_LIVE_FROM_STEREO
							};

/* globals */
int operation_option = 0;
int launch_status	 = 0;

/* functions */
void on_BarChange_user_selection(int newPos)// when changed by user or by software
{
	//newPos = 1;
	operation_option	= newPos;
	std::cout << "user changed selection " << operation_option << std::endl;
}
void on_BarChange_launch(int newPos)// when changed by user or by software
{
	launch_status		= newPos;
	std::cout << "user changed launching control" << launch_status << std::endl;
	if ((CALIBRATION_STEREO == operation_option) && (1 == launch_status))
	{
		std::cout << "user selected to launch calibration . starting.. " << std::endl;

	}

}
void show_calibration_gui()
{
	String WinName = "User Controls";
	namedWindow(WinName, CV_WINDOW_AUTOSIZE); //create a window 

	int posTrackBar = 0;

	//Create trackbars in this window
	posTrackBar = cvCreateTrackbar("User_selection"			 , "User Controls", &operation_option	, 3, on_BarChange_user_selection);
	posTrackBar = cvCreateTrackbar("change to launch program", "User Controls", &launch_status		, 1, on_BarChange_launch);
	// TODO: add text to show working directory. 
			// and calibration files directory.
			//

}
void launch_calibration()
{
	enum USER_STATUS_SELECTION {
		CALIBRATION_RIGHT = 1,		// don't use it as a seperate mode
		CALIBRATION_LEFT,		// don't use it as a seperate mode
		CALIBRATION_STEREO,	// just use this one
		STREAM_LIVE_FROM_STEREO
	};

	if (CALIBRATION_RIGHT == operation_option)
	{

	}
	else if (CALIBRATION_LEFT == operation_option)
		{

		}
		else if (CALIBRATION_STEREO == operation_option)
			{
				//int argc;
				//char* argv[6];

				//argc = 6;

				//argv[] = malloc(sizeof(char*) * (argc + 1));
				//argv[1] = "../src/imList.xml";
				//argv[2] = "-w";
				//argv[3] = "9";
				//argv[4] = "-h";
				//argv[5] = "6";
				//argv[6] = NULL;
				//// not. argv[6] = "L";
				//do_stereo_calib(argc, argv); // openCV demo code

			}
			else 
				std::cout << " the given selection is not defined : " << operation_option << std::endl;
	

}
//int new_argv()
//{
//	int i = 0;
//	int new_argc = 0;
//	char** tmp = new_args;
//
//	while (*tmp) {
//		++new_argc;
//		++tmp;
//	}
//
//	tmp = malloc(sizeof(char*) * (new_argc + 1));
//	// if (!tmp) error_fail();
//
//	for (i = 0; i < new_argc; ++i) {
//		tmp[i] = strdup(new_args[i]);
//	}
//	tmp[i] = NULL;
//
//	*pargv = tmp;
//
//	return new_argc;
//}

////////

int iLastX = -1;
int iLastY = -1;

int posTrackBar = 0;

void onBarChange(int newPos);

void set_controls_gui(int frameMax[])
{
	String WinName = "Controls";
	namedWindow(WinName, CV_WINDOW_AUTOSIZE); //create a window called "Control"

	int iLowH	= 0;
	int iHighH	= 179;

	int iLowS	= 0;
	int iHighS	= 255;

	int iLowV	= 0; 

	int F1minFrame = 1;
	int F1maxFrame = frameMax[0];

	int F2minFrame = 1;
	int F2maxFrame = frameMax[1];

	//Create trackbars in "Control" window
	cvCreateTrackbar("LowH"	, "Controls", &iLowH	, 179); //Hue (0 - 179)
	cvCreateTrackbar("HighH", "Controls", &iHighH, 179);

	cvCreateTrackbar("LowS"	, "Controls", &iLowS	, 255); //Saturation (0 - 255)
	cvCreateTrackbar("HighS", "Controls", &iHighS, 255);

	cvCreateTrackbar("LowV", "Controls", &iLowV, 255); //Value (0 - 255)
//	cvCreateTrackbar("HighV", "Control", &iHighV, 255);

	posTrackBar=cvCreateTrackbar("F1ndx", "Controls", NULL, F1maxFrame, onBarChange );  // this trackbar is just for showing information. NOT setting one
	//cvCreateTrackbar("F2ndx", "Control", &F2minFrame, F2maxFrame); 
	///createButton("button2", NULL, NULL, CV_CHECKBOX, 0); needs QT compile support


}
void onBarChange(int newPos)  // when changed by user or by software
{
	newPos = 1;
}

void Foo(int newPos)
{
	setTrackbarPos("F1ndx", "Control", newPos);
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

void my_button_cb(int state, void* userdata);
void CallBackFunc(int event, int x, int y, int flags, void* userdata);
void gui_example()
//from http://opencvexamples.blogspot.com/2014/01/detect-mouse-clicks-and-moves-on-image.html
{
	// Read image from file 
	Mat img = imread("left01.JPG");

	//if fail to read the image
	if (img.empty())
	{
		cout << "Error loading the selected image" << endl;
		//return -1;
	}

	//Create a window
	namedWindow("ImageDisplay", 1);

	//set the callback function for any mouse event
	//setMouseCallback("ImageDisplay", CallBackFunc, NULL);
	setMouseCallback("ImageDisplay", CallBackFunc, &img);

	// declare and initialize our userdata
	int my_data[] = { 0,1 };

	createButton("dummy_button", my_button_cb, &my_data, CV_PUSH_BUTTON, 0);


	//show the image
	imshow("ImageDisplay", img);

}

void my_button_cb(int state, void* userdata)
{
	std::cout << "@my_button_cb" << std::endl;

	// convert userdata to the right type
	int* ptr = (int*)userdata;
	if (!ptr)
	{
		std::cout << "@my_button_cb userdata is empty" << std::endl;
		return;
	}

	//// lock mutex to protect data from being modified by the
	//// main() thread
	//pthread_mutex_lock(&ptr->mtx);

	//ptr->state = state;

	//// unlock mutex
	//pthread_mutex_unlock(&ptr->mtx);
}


void CallBackFunc(int event, int x, int y, int flags, void* userdata)
{
	Mat* rgb = (Mat*)userdata;

	if (event == EVENT_LBUTTONDOWN)
	{
		cout << "Left button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
		/*printf("%d %d: %d, %d, %d\n",
			x, y,
			(int)(*rgb).at<Vec3b>(y, x)[0],
			(int)(*rgb).at<Vec3b>(y, x)[1],
			(int)(*rgb).at<Vec3b>(y, x)[2]);*/
	}
	else if (event == EVENT_RBUTTONDOWN)
	{
		cout << "Right button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
	}
	else if (event == EVENT_MBUTTONDOWN)
	{
		cout << "Middle button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
	}
	else if (event == EVENT_MOUSEMOVE)
	{
		cout << "Mouse move over the window - position (" << x << ", " << y << ")" << endl;
	}
	/////
	if (flags == (EVENT_FLAG_CTRLKEY + EVENT_FLAG_LBUTTON))
	{
		cout << "Left mouse button is clicked while pressing CTRL key - position (" << x << ", " << y << ")" << endl;
	}
	else if (flags == (EVENT_FLAG_RBUTTON + EVENT_FLAG_SHIFTKEY))
	{
		cout << "Right mouse button is clicked while pressing SHIFT key - position (" << x << ", " << y << ")" << endl;
	}
	else if (event == EVENT_MOUSEMOVE && flags == EVENT_FLAG_ALTKEY)
	{
		cout << "Mouse is moved over the window while pressing ALT key - position (" << x << ", " << y << ")" << endl;
	}
}