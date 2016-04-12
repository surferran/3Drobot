//// 3D stero robot (RoboDog)
// Usage :
//	remark below the desired frame function : taken from '"frameFunctions.h"  '
//  all main constants are set in the file '"working_consts.h"'

// TODO : 
//  26/11/15 -  set working_consts by UI, asspacialy choosing REC or PLAYBACK
//				set correct recordings rate		
//				do calibration !!
// DONE :
//	14/11/15 - performance issue..  -> 25/11/15 - smaller resultion to reduce bus trafic !
//  14/02/16 - stereo calibration seems to work finally. smaller resultution captures are bad for calibration.
// 
// helpers:
//   http://answers.opencv.org/question/9503/how-to-increase-the-quality-of-disparity-map/
//   http://www.pyimagesearch.com/2015/05/25/basic-motion-detection-and-tracking-with-python-and-opencv/
// https://singhgaganpreet.wordpress.com/tag/cvboundingrect-example/
//  navigation ref:
//https://www.youtube.com/watch?v=4Y7zG48uHRo 
// tracking ref by: 
// http://blog.christianperone.com/2015/01/real-time-drone-object-tracking-using-python-and-opencv/ !, add gradient hist/info of the object
// look at : https://www.youtube.com/watch?v=kxsvG4sSuvA 

 /* my  constants and parameters */
// bounds in percent from image size
#define MIN_MOVED_AREA_in_image 33//.0//23
#define MAX_MOVED_AREA_in_image 95.0
#define NUM_OF_PIXELS_IN_FRAME	(640.0*480.0)
#define MIN_CURVE_AREA			MIN_MOVED_AREA_in_image/100*NUM_OF_PIXELS_IN_FRAME
#define MAX_CURVE_AREA			MAX_MOVED_AREA_in_image/100*NUM_OF_PIXELS_IN_FRAME
#define SHOW_MOVING_CONTOURS	true//true
#define SHOW_MOVING_RECTANGLES	false//true
#define SHOW_MOVING_BIG_CONTOURS	true
#define SHOW_MOVING_BIG_RECTANGLES	false//true

// below or above those limits it will be treated as noise. (or too close movement to camera)

#include "frameFunctions.h"		// general definitions and functions. that's why it is first to include.
#include "working_consts.h"		// my added definitions, constants
#include "utilFunctions.h"		// utility functions for the application.

#include "BackgroundSub.hpp"

#include "GUI_functions.h"



//#include "externals\objectTrackingTut.cpp"
//#include "externals\multipleObjectTracking.cpp"
//#include "externals\motionTracking_myChanges.cpp"
#include "externals\motionTracking_ver14.cpp"
////#include "externals\stereoBMTuner-1.0\StereoBMTuner\stereoBMTuner.h"

#include "camshiftdemo.cpp"



int main(int argc, char** argv){
	/* global partial init */
	//do_track_subject(argc, argv);
	//do_track_mult();
	//main_track();
	//main_14

	//ask_user_for_video_source();	// get file name or open camera0 (+1 later for stereo)
	string	base_out_file_path	= "C:/Users/Ran_the_User/Documents/Technion_Studies/IP_STUFF/video_4_testing/out";
	string	framesCounterStr	= ""	, base_file_name = "" , file_full_name="", file_suffix = ".*";	
	int		stream_frame_index	= 0;
	char	rec_file_name[150] = "C:/Users/Ran_the_User/Documents/Technion_Studies/IP_STUFF/video_4_testing/in/VID_3D_scenario/output_1.avi";
	VideoCapture vid = VideoCapture(rec_file_name);	
	namedWindow("sensing user keys here", 1);

	int		loop_delay = 33 ; //[ms]	// need to fit the file recording value
	char	user_pressing=0;	// just optional.

	if( !vid.isOpened() )
		return -1;

	plotWindows[0] = "win1 - raw image";

	while (1)
	{
		if (op_flags.reset_vid_file_location)
		{
			vid.set(CAP_PROP_POS_FRAMES,0);	// go back to first frame
			op_flags.reset_identification	 = true;
			op_flags.reset_vid_file_location = false;
		}
		if (op_flags.reset_identification){
			reset_camshift_vars();			// a funciton to reset the camshift main variables. 
			op_flags.reset_identification	= false;
		}
		if (op_flags.play_on){
			vid >> images[0];
			if (images[0].empty())
				op_flags.play_on = false;
			else
			{					
				imshow(plotWindows[0], images[0]); 
				if (op_flags.make_camshift)
					do_camshift_on_Single_current_frame(&images[0]); 
			}
		}
		
		if (!check_user_input(&loop_delay, &user_pressing))
			break;
	}


	return 0;



	do_camshift_main();
	return 0;


	show_forgnd_and_bgnd();
	return 0;

	/////////////////////////////////////////////////////

	thisStereo.input_source = STREAM_STEREO_CAMS;
	
	show_user_gui();

	waitKey();

	destroyAllWindows();
	return 0;
	//////////////////////////////////////////////////////
}