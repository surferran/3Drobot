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


#include "frameFunctions.h"  
#include "working_consts.h"
#include "utilFunctions.h"
// dont #include "C:\Users\Ran_the_User\Documents\Technion_Studies\2016_A_winter\01_STEREO_cam_car_PROJECT\CODE\example\objectDetection.cpp"
#include "externals\BackgroundSub.cpp" // tried
#include "externals\stitching_detailed.cpp" // tried
//#include "C:\Users\Ran_the_User\Documents\Technion_Studies\2016_A_winter\01_STEREO_cam_car_PROJECT\Code\camera3D\src\externals\tracker.cpp"
////#include ".\stereo_calib.cpp"  // the openCV code example
///#include ".\stereo_match.cpp"  // the openCV code example
#include "externals\not_yet\optical_flow_demo.cpp"
//#include "externals\stereocalibrate.cpp" - included in project workspace
///#include "stereo_calibrate_from_PUSHBROOM.cpp" - included
//int stero_calib( VideoCapture c[]); // declare for the CPP already included file
////////extern struct CalibStruct; 
//void load_calibration_file_matrices(String caliFile, CalibStruct* outputMatrices_p);
//void do_complete_rectification(Mat img1, Mat img2, CalibStruct* givenCalibMatrices);
//#include "externals\libviso2\src\demo.cpp";
//#include "externals\Submission\UsingOpenCV.cpp"
//int APIENTRY _tWinMain(HINSTANCE hInstance,
//	HINSTANCE hPrevInstance,
//	LPTSTR    lpCmdLine,
//	int       nCmdShow);

// Need to include this for serial port communication
//#include <Windows.h>
//#include <ser>

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

//int main_notNow(int argc, char** argv)
int main(int argc, char** argv)
{
	int	j = 0;   // general use cameras loop counter
	VideoCapture	cams[numOfActiveCams];			// set the cameras buffers
	VideoWriter     videos[numOfActiveCams];		// set the recordings buffers
	Mat				raw_frame[numOfActiveCams];
	Mat				modeified_frame;
	int				frame_max[numOfActiveCams];		// used for playback  // TODO: put in a struct of CamProperties 
	/*HINSTANCE hInstance, hPrevInstance;
	LPTSTR    lpCmdLine;
	int nCmdShow;
	_tWinMain( hInstance,
		 hPrevInstance,
		    lpCmdLine,
		       nCmdShow);*/
	int maxVals[] = { 100,100 };
	/*set_controls_gui(maxVals);
	gui_example();*/
	show_calibration_gui();

	waitKey();
	return 0;
	//
	//.. / src / imList.xml - w 9 - h 6
	//
	//do_stereo_calib(argc, argv); // openCV demo code
	argc = 11;
	argv[1] = "../run_inputs/opencv_example_frames/left03.jpg";
	argv[2] = "../run_inputs/opencv_example_frames/right03.jpg";
	argv[3] = "-i";
	argv[4] = "../data/intrinsics.yml";
	argv[5] = "-e";
	argv[6] = "extrinsics.yml";
	argv[7] = "-o";
	argv[8] = "../data/disp_out.jpg";
	argv[9] = "-p";
	argv[10] = "../data/points_out.yml";
	/**/
	//do_stereo_match(argc, argv); // openCV demo code
	waitKey();//6 0
	return 0;
	///main_PushBroom(argc, argv);

	// from 'BackgroundSub.cpp'
	///show_forgnd_and_bgnd();

	// from 'stitching_detailed.cpp' 
	/*
	printUsage(); 
	string a = "C:\Users\Ran_the_User\Documents\Technion_Studies\2016_A_winter\01_STEREO_cam_car_PROJECT\Code\camera3D\src\externals\bgnd_sub\pic_set\0";
	cout << a;
	do_stitching_detailed(argc, argv); 	
	//	..\src\externals\bgnd_sub\pic_set\20151212_075744.jpg ..\src\externals\bgnd_sub\pic_set\20151212_075748.jpg ..\src\externals\bgnd_sub\pic_set\20151212_075751.jpg ..\src\externals\bgnd_sub\pic_set\20151212_075753.jpg

	*/
	//////do_stitching_detailed( argc,  argv);
	/*Mat3b visibleResult;
	convertScaleAbs(result, visibleResult);
	imshow("visibleResult", visibleResult);*/
	//waitKey(); 

	/*
	do_example_for_optical_flow();
	cvWaitKey(0);
	return 0;
	*/

	//while (waitKey(loop_DELAY) == 0) break;
	//////return 0;



	//////////////////////////////

	/* init the video sources , and set it's properties */
	for (j = 0; j < numOfActiveCams; j++) 
	{
		//cams[j] = new CMyCam(j); // TODO : complete this option
		///CMyCam cam1(1, SMALL);

		if (working_mode == PLAYBACK)
			{	cams[j] = VideoCapture(rec_file_name[j]);	namedWindow(window_name[j], 1);	}
		else
			{	cams[j] = VideoCapture(camIndexes[j]);		namedWindow(window_name[j], 1);	}
		
		if (!cams[j].isOpened())  // check if we succeeded
			return -1;					// TODO : add error notice

		if (working_mode == PLAYBACK) // set same resulution for all frames
		{
			double frame_width  = cams[0].get(CV_CAP_PROP_FRAME_WIDTH);
			double frame_height = cams[0].get(CV_CAP_PROP_FRAME_HEIGHT);
			frame_max[j]		= cams[j].get(CV_CAP_PROP_FRAME_COUNT);
		}
		else
		{
			cams[j].set(CV_CAP_PROP_FRAME_WIDTH , working_FRAME_WIDTH);
			cams[j].set(CV_CAP_PROP_FRAME_HEIGHT, working_FRAME_HIGHT);

			if (working_mode == REG_and_REC)
				videos[j].open(rec_file_name[j], REC_CODEC, REC_FPS, Size(working_FRAME_WIDTH, working_FRAME_HIGHT), true);  // bool isColor=true
		}
	}

	//only 1st time needed : 
	//// 	
	//stero_calib( cams);
	//CalibStruct localMatrices;
	//load_calibration_file_matrices( STEREO_CALIBRATION_FILE_NAME, &localMatrices );
	//do_complete_rectification(Mat img1, Mat img2, CalibStruct* givenCalibMatrices);
	//return 1;

	//Create a black image with the size as the camera output
	//Mat imgLines = Mat::zeros(raw_frame[0].size(), CV_8UC3);;
	int currentFrameIndex[numOfActiveCams];
	set_controls_gui(frame_max);

	/// dont main1();
	int frames_index = 0;
	Mat *register_frames[2] ; register_frames[0] = &raw_frame[0]; // using only the 1st camera , if any
	Mat output_blended_img;

	stringstream  fileName;
	int output_frame_index = 0;

	while (1)
	{
		for (j = 0; j < numOfActiveCams; j++) {

			cams[j] >> raw_frame[j];					// get a new frame from camera #j

			if (raw_frame[j].empty())
			{
				printf(" --(!) No captured frame -- Break!");
				break;
			}
			else
			{
				if (j == 0) frames_index++;
				currentFrameIndex[j] = cams[j].get(CV_CAP_PROP_POS_FRAMES);
				if (j == 0) Foo(currentFrameIndex[j]);

				process_frame(&raw_frame[j], &modeified_frame);

				imshow(window_name[j], modeified_frame);		// TODO: set those on top

				videos[j].write(modeified_frame); 			// add 	if (working_mode == REG_and_REC) ?
				if (working_mode == PLAYBACK)				waitKey(2.0*loop_DELAY) ;
// for calibration : for specific frame indeces  - save the images. of both videoes.
				if (working_mode == PLAYBACK) {
					if ((frames_index == 38) || 
						(frames_index == 45) || 
						(frames_index == 95) ||
						(frames_index == 111) ||
						(frames_index == 140) ||
						(frames_index == 159) ||
						(frames_index == 198) ||
						(frames_index == 252) ||
						(frames_index == 261) ||
						(frames_index == 300)) {
						// save images //https://cal-linux.com/tutorials/strings.html
						//http://stackoverflow.com/questions/15870612/saving-an-image-sequence-from-video-using-opencv2 
						fileName.str("");
						//fileName << "../src/output_images/out_chess_" << j << "_"<< output_frame_index<< ".jpg" ;  //frameindex+j
						fileName << "../run_outputs/output_images/out_chess_" << output_frame_index << "_"<< j<< ".jpg" ;  //frameindex+j
						imwrite(fileName.str(), raw_frame[j]);
						
						if (j == numOfActiveCams - 1) {
							output_frame_index++;////
							do_complete_rectification(raw_frame[0], raw_frame[1], &localMatrices);
						////
						}
					}
				}
				if (numOfActiveCams == 1) {
					if ((frames_index > 1)
						&& (frames_index % stitching_frame_rate == 0))
					{
						register_frames[1] = &raw_frame[0];
						char* argv_inside[5 + 2] = { "..exe name..",
							"--features","orb" ,
							"--warp","plane",
							"fig_a","fig_b" };
						int argc_local = size(argv_inside);

						do_stitching_detailed(argc_local, &argv_inside[0], register_frames, frames_index, &output_blended_img);

						*register_frames[0] = output_blended_img;
					}
					else
						register_frames[0] = &raw_frame[0];
				}
			}
		}

		if ((working_mode == 11) && (numOfActiveCams == 2) && (!raw_frame[0].empty()) && (!raw_frame[1].empty())) {
			if ((frames_index % stereo_stitching_frame_rate == 0))
			{
				register_frames[0] = &raw_frame[0];
				register_frames[1] = &raw_frame[1];
				// ref by : http://stackoverflow.com/questions/25264009/difference-between-different-warp-methods-in-opencv 
				char* argv_inside[5 + 2] = { "..exe name..",
					"--features","orb" ,					
					"--warp","cylindrical",    //"--warp","stereographic",
					"fig_a","fig_b" };
				int argc_local = size(argv_inside);

				do_stitching_detailed(argc_local, &argv_inside[0], register_frames, frames_index, &output_blended_img);

				///*register_frames[0] = output_blended_img;  // runs-over the frame data. not good

			}
		}

		if ( (raw_frame[0].empty()) || (waitKey(loop_DELAY) >= 0) )	
			break;

	}

	destroyAllWindows();	// despite the camera will be deinitialized AUTOmatically in VideoCapture destructor
							// using c:\OpenCV\build\include\opencv2\highgui.hpp
	return 0;
}