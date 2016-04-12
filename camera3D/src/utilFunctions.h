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

#include "stereo_calib.h"
#include "stereo_match.hpp"

/* functions headers */
void on_BarChange_user_selection(int newPos);
void on_BarChange_launch(int newPos);
void show_calibration_gui();
void launch_calibration();
void set_controls_gui(int frameMax[]);
int  initialize_vid_source();
void display_L_R_stream();
Size setLabel(Mat& im, const std::string label, const Point& or );
bool find_and_draw_chess(Mat img, String display_window_name , bool do_draw);
void mouse_cb(int event, int x, int y, int flags, void* userdata);

/* globals */   // TODO : add explanations
int operation_option	= 0;
int launch_status		= 0;
int stream_frame_index	= 0;
bool user_requested		= true;

struct btn_strct
{
	int index;
	String text;
	int Val;
	int x_min,x_max,y_min,y_max;
};
btn_strct btn_arr[5];
///./btn_arr = new btn_strct[5];

/* functions */
void on_BarChange_user_selection(int newPos)// when changed by user or by software
{
	//newPos = 1;
	operation_option	= newPos;
	std::cout << "user changed selection " << operation_option << std::endl;
}
void on_BarChange_launch(int newPos)// when changed by user or by software
{

	int		argc;
	char*	argv[11];  //6

	launch_status		= newPos;
	std::cout << "user changed launching control" << launch_status << std::endl;
	if ((CAPTURE_CALIBRATION_IMAGES == operation_option) && (1 == launch_status))
	{
		std::cout << "user selected to capture calibration images . starting.. " << std::endl;

		initialize_vid_source();
		display_L_R_stream();
	}
	else
		if ((CALIBRATION_STEREO == operation_option) && (1 == launch_status))
		{
			std::cout << "user selected to launch calibration . starting.. " << std::endl;
			// using saved files of calibration images. and saving matrices to files.

			argc = 6;

			argv[1] = "../src/imList.xml";
			argv[2] = "-w";
			argv[3] = "6";// "9";
			argv[4] = "-h";
			argv[5] = "5";//  "6"; 

			/*argv[1] = "../src/imList_OpenCV.xml";
			argv[2] = "-w";
			argv[3] =   "9";
			argv[4] = "-h";
			argv[5] =   "6"; */

			do_stereo_calib(argc, argv); // openCV demo code. abit manipulated
		} 	
		else
			if ( (STREAM_WITH_DISPARITY_AND_DEPTH == operation_option) && (1 == launch_status)) // rectification display
			{
				argc = 11;
				argv[1] = "../run_inputs/captured_calibration_images/out_chess_3_1.jpg";  // left image
				argv[2] = "../run_inputs/captured_calibration_images/out_chess_3_0.jpg";  // right image

				//int color_mode = alg == STEREO_BM ? 0 : -1;
				Mat img1 = imread(argv[1], -1);
				Mat img2 = imread(argv[2], -1);
				argv[3] = "-i";
				argv[4] = "../data/intrinsics.yml";
				argv[5] = "-e";
				argv[6] = "extrinsics.yml";
				argv[7] = "-o";
				argv[8] = "../data/disp_out.jpg";
				argv[9] = "-p";
				argv[10] = "../data/points_out.yml";
				
				do_stereo_match(argc, argv, img1, img2); // openCV demo as base code
			
				return;
				initialize_vid_source();
				display_L_R_stream();

				//argv[1] = "../run_inputs/opencv_example_frames/left03.jpg";
				//argv[2] = "../run_inputs/opencv_example_frames/right03.jpg";
				//argv[3] = "-i";
				//argv[4] = "../data/intrinsics.yml";
				//argv[5] = "-e";
				//argv[6] = "extrinsics.yml";
				//argv[7] = "-o";
				//argv[8] = "../data/disp_out.jpg";
				//argv[9] = "-p";
				//argv[10] = "../data/points_out.yml";
				//
				//do_stereo_match(argc, argv); // openCV demo code
				//waitKey();//6 0
				//return 0;
			}
			else
				if ((STREAM_LIVE_FROM_STEREO == operation_option) && (1 == launch_status)) // rectification display
				{
					initialize_vid_source();
					display_L_R_stream();
				}
				std::cout << " the given selection is not defined : " << operation_option << std::endl;
}

void show_user_gui()
{
	String WinName = "User Controls";
	namedWindow(WinName, CV_WINDOW_AUTOSIZE); //create a window 
	Mat b_im(120, 340, CV_8UC3, Scalar(10, 10, 10));  // hight, width,type,.. //create_empty_image() 

	int posTrackBar = 0;

	//Create trackbars in this window
	posTrackBar = cvCreateTrackbar("User_selection"			 , "User Controls", &operation_option	, 4, on_BarChange_user_selection);
	posTrackBar = cvCreateTrackbar("change to launch program", "User Controls", &launch_status		, 1, on_BarChange_launch);
	// TODO: add text to show working directory. 
			// and calibration files directory.
			// and options possibilities (the enum)

	std::string text = " show stereo stream ";
	Size boundary = setLabel(b_im, text, cvPoint(10, 30));  // return is w,h + l,top
	//attach callback. or set boundaries for buttons table
	
	btn_arr[0].index = 0;
	btn_arr[0].text = text;
	btn_arr[0].Val = 0;
	btn_arr[0].x_min = 10;
	btn_arr[0].x_max = 10 + boundary.width;
	btn_arr[0].y_min = 30;
	btn_arr[0].y_max = 30 + boundary.height;
	/**/


	 text = " capture calibration images from BW stream ";
	 boundary = setLabel(b_im, text, cvPoint(10, 60));
	//attach callback. or set boundaries for buttons table
	 btn_arr[1].index = 1;
	 btn_arr[1].text = text;
	 btn_arr[1].Val = 0;
	 btn_arr[1].x_min = 10;
	 btn_arr[1].x_max = 10 + boundary.width;
	 btn_arr[1].y_min = 30;
	 btn_arr[1].y_max = 30 + boundary.height;
	 /* when hover above - change background. (redisplay label with different bckg color )*/

	imshow(WinName, b_im);

}

int initialize_vid_source()
{
	int	j = 0 ;   // general use cameras loop counter

	if (thisStereo.input_source == STREAM_STEREO_CAMS)
	{
		for (j = 0; j < numOfActiveCams; j++)
		{
			thisStereo.cams[j] = VideoCapture(camIndexes[j]);		namedWindow(window_name[j], 1);
			if (!thisStereo.cams[j].isOpened())  // check if we succeeded
				return -1;

		//	if (CAPTURE_CALIBRATION_IMAGES == operation_option) {
		//		thisStereo.cams[j].set(CV_CAP_PROP_FRAME_WIDTH	, 640);
		//		thisStereo.cams[j].set(CV_CAP_PROP_FRAME_HEIGHT	, 480);
		//	}
		//	else
			{
				thisStereo.cams[j].set(CV_CAP_PROP_FRAME_WIDTH	, working_FRAME_WIDTH);
				thisStereo.cams[j].set(CV_CAP_PROP_FRAME_HEIGHT	, working_FRAME_HIGHT);
			}
			//thisStereo.cams[j].set(CV_CAP_PROP_FPS, 30);
			thisStereo.cams[j].set(CV_CAP_PROP_FPS, 15);
			double tmp = thisStereo.cams[j].get(CV_CAP_PROP_FPS);
			cout << "frame rate given as : " << tmp; // gives zero.. no support..

			// record the stream and/or it's results
			thisStereo.rec_videos[j].open(rec_file_name[j], REC_CODEC, REC_FPS, Size(working_FRAME_WIDTH, working_FRAME_HIGHT), true);  // bool isColor=true

			// add mouse capture event. to engage 'user_request' as 'true'			 
			setMouseCallback(window_name[j] , mouse_cb, NULL); // img for later - check area of image as button area.. (by adding relevant labels..)
		}
	}
	else
		if (thisStereo.input_source == RECORDED_VIDEOS_COUPLE)
		{
			for (j = 0; j < numOfActiveCams; j++)
			{
				thisStereo.cams[j] = VideoCapture(rec_file_name[j]);	namedWindow(window_name[j], 1);
				if (!thisStereo.cams[j].isOpened())  // check if we succeeded
					return -1;
				
				thisStereo.vid_res[j].width		= thisStereo.cams[j].get(CV_CAP_PROP_FRAME_WIDTH);
				thisStereo.vid_res[j].height	= thisStereo.cams[j].get(CV_CAP_PROP_FRAME_HEIGHT);
				thisStereo.vid_len[j]			= thisStereo.cams[j].get(CV_CAP_PROP_FRAME_COUNT);				
			}
		}
		else
		{
			std::cout << " option of images list will be treated elsewhre " << std::endl;
		}
}

void display_L_R_stream()
{
	int		j = 0;
	String	framesCounterStr = "";
	bool	found_chess, total_chess_found;

	stringstream	fileName;
	int				output_frame_index = 0;

	int		argc;
	char*	argv[11];  

	while (1)	// consider put in separate thread
	{
		total_chess_found = true;
		for (j = 0; j < numOfActiveCams; j++) {

			thisStereo.cams[j] >> thisStereo.raw_frame[j];					// get a new frame from camera #j

			cvtColor(thisStereo.raw_frame[j], thisStereo.raw_frame[j], CV_BGR2GRAY); // change to GRAY

			if (thisStereo.raw_frame[j].empty())
			{
				printf(" --(!) No captured frame -- Break!");
				// add print on image screen
				break;
			}
			else
			{
				if (j == 0) stream_frame_index++;
				thisStereo.modeified_frame[j] = thisStereo.raw_frame[j].clone();

				std::string text = "frames counter ";
				text += std::to_string(stream_frame_index);
				framesCounterStr = text;
				// add seconds + mili since started , + FPS? 
				//display it as text label
				setLabel(thisStereo.modeified_frame[j], framesCounterStr , cvPoint(10, 30));


				if ((CAPTURE_CALIBRATION_IMAGES == operation_option)) {
					// add check and display for the chessboard.
			//		found_chess = find_and_draw_chess(thisStereo.modeified_frame[j], window_name[j], true);
			//		if (!found_chess)
			//			total_chess_found = false;

					thisStereo.rec_videos[j].write(thisStereo.raw_frame[j]);  // do the recording
				}
			
			//		thisStereo.rec_videos[j].write(thisStereo.modeified_frame[j]);  // do the recording

				imshow(window_name[j], thisStereo.modeified_frame[j]);		// TODO: consider displaying a modified frame

			}
		}
			
		if ((STREAM_WITH_DISPARITY_AND_DEPTH == operation_option) && numOfActiveCams==2)
		{
			argc = 11;
			argv[1] = "../run_inputs/captured_calibration_images/out_chess_3_1.jpg";  // left image
			argv[2] = "../run_inputs/captured_calibration_images/out_chess_3_0.jpg";  // right image

			//int color_mode = alg == STEREO_BM ? 0 : -1;
			Mat img1 = thisStereo.modeified_frame[0];
			Mat img2 = thisStereo.modeified_frame[1];
			argv[3] = "-i";			argv[4]  = "../data/intrinsics.yml";
			argv[5] = "-e";			argv[6]  = "extrinsics.yml";
			argv[7] = "-o";			argv[8]  = "../data/disp_out.jpg";
			argv[9] = "-p";			argv[10] = "../data/points_out.yml";

			cout << "\n matching.. ";
			do_stereo_match(argc, argv, img1, img2); // openCV demo code
			cout << "\n done & displayed matches ";
		}

		// ask user to save pictures. using added mouse capture 
		if (total_chess_found && user_requested) 
		{			
			user_requested = false;  // will need to press again mouse left button , to allow capture again

			//cout << "\n user - please choose to keep those images or not \n";
			//int c = waitKey();			//121-yes , 110 - not
			//cout << "\n you chose : " << c;

			/* save the images with serial number in file name */
			output_frame_index++;
			for (j = 0; j < numOfActiveCams; j++) { 
				fileName.str(""); 
				fileName << "../run_outputs/output_images/out_chess_" << output_frame_index << "_"<< j<< ".jpg" ;  //frameindex+j
				imwrite(fileName.str(), thisStereo.raw_frame[j]);			//	 save the captured raw frames. without any drawings on it.
			}
			cout << "\n found and saved images "<< output_frame_index ;

		}
		if ( (waitKey(loop_DELAY) >= 0) ||
			 (thisStereo.raw_frame[j].empty() && j < numOfActiveCams) )
			break;
	}

	printf(" -- finished streaming -- ");
}
Size setLabel(Mat& im, const std::string label, const Point& or )
{
	// from : http://answers.opencv.org/question/27695/puttext-with-black-background/
	int fontface = cv::FONT_HERSHEY_SIMPLEX;
	double scale = 0.4;
	int thickness = 1;
	int baseline = 0;

	CvScalar black,red,green,blue,white;
	black = CV_RGB(0, 0, 0);
	red = CV_RGB(255, 0, 0);
	green = CV_RGB(0, 255, 0);
	blue = CV_RGB(0, 0, 255);
	white = CV_RGB(0, 0, 0);


	cv::Size text = cv::getTextSize(label, fontface, scale, thickness, &baseline);
	cv::rectangle(im, or +cv::Point(0, baseline), or +cv::Point(text.width, -text.height), green, CV_FILLED);
	cv::putText(im, label, or , fontface, scale, CV_RGB(255, 255, 255), thickness, 8);
	// consider:
	//			putText(thisStereo.raw_frame[j], framesCounterStr, cvPoint(30, 30),
	//				FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(200, 200, 250), 1, CV_AA);
	return text;
}


void capture_10_calibration_stereo_images() 
{

};

bool find_and_draw_chess(Mat img, String display_window_name= "corners", bool do_draw=true) // TODO: use pointers instead..
{
	if (img.empty())
		return false;
	Size imageSize = img.size();
	Size boardSize(6, 5);	// for my toy chessboard card   // (9, 6); - for the openCV demo files

	vector<Point2f> imagePoints;

	bool found = false;
	vector<Point2f>& corners =  imagePoints;// num of given frames
	 
	Mat timg;
	timg = img;
		
	found = findChessboardCorners(timg, boardSize, corners,
		CALIB_CB_ADAPTIVE_THRESH | CALIB_CB_NORMALIZE_IMAGE);
	/*
	if (found)
		color the display windows frame in green, otherwise red or gray 
		*/
 
	/* displayCorners */
	if (found && do_draw) // found, and user desire to draw the findings
	{
		Mat cimg, cimg1;
		//cvtColor(img, cimg, COLOR_GRAY2BGR); // openCV example photos are gray ones
		cimg = img;
		drawChessboardCorners(cimg, boardSize, corners, found);
		double sf = 640. / MAX(img.rows, img.cols);
		resize(cimg, cimg1, Size(), sf, sf);
		imshow(display_window_name, cimg1); 
	}
	return found;
}

void mouse_cb(int event, int x, int y, int flags, void* userdata)
{
	if (event == EVENT_LBUTTONDOWN)
	{
		user_requested = true;	// for initiating calibration
	}
}
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
		//cout << "Left button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
		/*printf("%d %d: %d, %d, %d\n",
			x, y,
			(int)(*rgb).at<Vec3b>(y, x)[0],
			(int)(*rgb).at<Vec3b>(y, x)[1],
			(int)(*rgb).at<Vec3b>(y, x)[2]);*/

		user_requested = true;	// for initiating calibration
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