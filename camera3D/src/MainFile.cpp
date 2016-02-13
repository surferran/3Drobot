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


#include "frameFunctions.h"		// general definitions and functions. that's why it is first to include.
#include "working_consts.h"		// my added definitions, constants
#include "utilFunctions.h"		// utility functions for the application.

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
	
	
	show_user_gui();

	waitKey();
	return 0;



	//////////////////////////////

//
//	//only 1st time needed : 
//	//// 	
//	//stero_calib( cams);
//	//CalibStruct localMatrices;
//	//load_calibration_file_matrices( STEREO_CALIBRATION_FILE_NAME, &localMatrices );
//	//do_complete_rectification(Mat img1, Mat img2, CalibStruct* givenCalibMatrices);
//	//return 1;
//
//	//Create a black image with the size as the camera output
//	//Mat imgLines = Mat::zeros(raw_frame[0].size(), CV_8UC3);;
//	int currentFrameIndex[numOfActiveCams];
//	set_controls_gui(frame_max);
//
//	/// dont main1();
//	int frames_index = 0;
//	Mat *register_frames[2] ; register_frames[0] = &raw_frame[0]; // using only the 1st camera , if any
//	Mat output_blended_img;
//
//	stringstream  fileName;
//	int output_frame_index = 0;
//
//	while (1)
//	{
//		for (j = 0; j < numOfActiveCams; j++) {
//
//			cams[j] >> raw_frame[j];					// get a new frame from camera #j
//
//			if (raw_frame[j].empty())
//			{
//				printf(" --(!) No captured frame -- Break!");
//				break;
//			}
//			else
//			{
//				if (j == 0) frames_index++;
//				currentFrameIndex[j] = cams[j].get(CV_CAP_PROP_POS_FRAMES);
//				if (j == 0) Foo(currentFrameIndex[j]);
//
//				process_frame(&raw_frame[j], &modeified_frame);
//
//				imshow(window_name[j], modeified_frame);		// TODO: set those on top
//
//				videos[j].write(modeified_frame); 			// add 	if (working_mode == REG_and_REC) ?
//				if (working_mode == PLAYBACK)				waitKey(2.0*loop_DELAY) ;
//// for calibration : for specific frame indeces  - save the images. of both videoes.
//				if (working_mode == PLAYBACK) {
//					if ((frames_index == 38) || 
//						(frames_index == 45) || 
//						(frames_index == 95) ||
//						(frames_index == 111) ||
//						(frames_index == 140) ||
//						(frames_index == 159) ||
//						(frames_index == 198) ||
//						(frames_index == 252) ||
//						(frames_index == 261) ||
//						(frames_index == 300)) {
//						// save images //https://cal-linux.com/tutorials/strings.html
//						//http://stackoverflow.com/questions/15870612/saving-an-image-sequence-from-video-using-opencv2 
//						fileName.str("");
//						//fileName << "../src/output_images/out_chess_" << j << "_"<< output_frame_index<< ".jpg" ;  //frameindex+j
//						fileName << "../run_outputs/output_images/out_chess_" << output_frame_index << "_"<< j<< ".jpg" ;  //frameindex+j
//						imwrite(fileName.str(), raw_frame[j]);
//						
//						if (j == numOfActiveCams - 1) {
//							output_frame_index++;////
//							do_complete_rectification(raw_frame[0], raw_frame[1], &localMatrices);
//						////
//						}
//					}
//				}
//				if (numOfActiveCams == 1) {
//					if ((frames_index > 1)
//						&& (frames_index % stitching_frame_rate == 0))
//					{
//						register_frames[1] = &raw_frame[0];
//						char* argv_inside[5 + 2] = { "..exe name..",
//							"--features","orb" ,
//							"--warp","plane",
//							"fig_a","fig_b" };
//						int argc_local = size(argv_inside);
//
//						do_stitching_detailed(argc_local, &argv_inside[0], register_frames, frames_index, &output_blended_img);
//
//						*register_frames[0] = output_blended_img;
//					}
//					else
//						register_frames[0] = &raw_frame[0];
//				}
//			}
//		}
//
//		if ((working_mode == 11) && (numOfActiveCams == 2) && (!raw_frame[0].empty()) && (!raw_frame[1].empty())) {
//			if ((frames_index % stereo_stitching_frame_rate == 0))
//			{
//				register_frames[0] = &raw_frame[0];
//				register_frames[1] = &raw_frame[1];
//				// ref by : http://stackoverflow.com/questions/25264009/difference-between-different-warp-methods-in-opencv 
//				char* argv_inside[5 + 2] = { "..exe name..",
//					"--features","orb" ,					
//					"--warp","cylindrical",    //"--warp","stereographic",
//					"fig_a","fig_b" };
//				int argc_local = size(argv_inside);
//
//				do_stitching_detailed(argc_local, &argv_inside[0], register_frames, frames_index, &output_blended_img);
//
//				///*register_frames[0] = output_blended_img;  // runs-over the frame data. not good
//
//			}
//		}
//
//		if ( (raw_frame[0].empty()) || (waitKey(loop_DELAY) >= 0) )	
//			break;
//
//	}
//
//	destroyAllWindows();	// despite the camera will be deinitialized AUTOmatically in VideoCapture destructor
//							// using c:\OpenCV\build\include\opencv2\highgui.hpp
	return 0;
}