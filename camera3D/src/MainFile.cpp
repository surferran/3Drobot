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
// look at : https://www.youtube.com/watch?v=kxsvG4sSuvA 

#include "frameFunctions.h"		// general definitions and functions. that's why it is first to include.
#include "working_consts.h"		// my added definitions, constants
#include "utilFunctions.h"		// utility functions for the application.

////#include "externals\stereoBMTuner-1.0\StereoBMTuner\stereoBMTuner.h"

void process_frame(Mat *inFrame, Mat *outFrame)
{
	//rgb_frame_to_Edges(&frame[j], &edges);  // (in, out) Mat order
	//rgb_frame_reduce_Blue(&frame[j], &edges);  // (in, out) Mat order
	copy_frame	(inFrame, outFrame);
	// color_to_gray (..)
	// smooth image
	// find edges
	// check main areas
	// remove background
}

int main(int argc, char** argv)
{
	/* global partial init */
	thisStereo.input_source = STREAM_STEREO_CAMS;
	
	show_user_gui();

	waitKey();

	destroyAllWindows();
	return 0;
}