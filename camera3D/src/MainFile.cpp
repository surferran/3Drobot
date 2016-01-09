//// 3D stero robot (RoboDog)
// Usage :
//	remark below the desired frame function : taken frin '"frameFunctions.h"  '
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
#include "C:\Users\Ran_the_User\Documents\Technion_Studies\2016_A_winter\01_STEREO_cam_car_PROJECT\Code\camera3D\src\externals\BackgroundSub.cpp" // tried
#include "C:\Users\Ran_the_User\Documents\Technion_Studies\2016_A_winter\01_STEREO_cam_car_PROJECT\Code\camera3D\src\externals\stitching_detailed.cpp" // tried
//#include "C:\Users\Ran_the_User\Documents\Technion_Studies\2016_A_winter\01_STEREO_cam_car_PROJECT\Code\camera3D\src\externals\tracker.cpp"
#include "externals\not_yet\stereo_calib.cpp"
#include "externals\not_yet\optical_flow_demo.cpp"

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
/*
void open_com() {          //serial
	// Setup serial port connection and needed variables.
	HANDLE hSerial = CreateFile(L"COM3", GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if (hSerial != INVALID_HANDLE_VALUE)
	{
		printf("Port opened! \n");

		DCB dcbSerialParams;
		GetCommState(hSerial, &dcbSerialParams);

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
}
*/

int main(int argc, char** argv)
{
	int	j = 0;   // general use cameras loop counter
	VideoCapture	cams[numOfActiveCams];			// set the cameras buffers
	VideoWriter     videos[numOfActiveCams];		// set the recordings buffers
	Mat				raw_frame[numOfActiveCams];
	Mat				modeified_frame;

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

	do_example_for_optical_flow();
	cvWaitKey(0);
	return 0;

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
		}
		else
		{
			cams[j].set(CV_CAP_PROP_FRAME_WIDTH , working_FRAME_WIDTH);
			cams[j].set(CV_CAP_PROP_FRAME_HEIGHT, working_FRAME_HIGHT);

			if (working_mode == REG_and_REC)
				videos[j].open(rec_file_name[j], REC_CODEC, REC_FPS, Size(working_FRAME_WIDTH, working_FRAME_HIGHT), true);  // bool isColor=true
		}
	}


	do_stereo_calib(argc, argv);

	//Create a black image with the size as the camera output
	//Mat imgLines = Mat::zeros(raw_frame[0].size(), CV_8UC3);;

	set_controls_gui();

	/// dont main1();
	int frames_index = 0;
	Mat *register_frames[2] ; register_frames[0] = &raw_frame[0]; // using only the 1st camera , if any
	Mat output_blended_img;
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

				process_frame(&raw_frame[j], &modeified_frame);

				imshow(window_name[j], modeified_frame);

				videos[j].write(modeified_frame); 			// add 	if (working_mode == REG_and_REC) ?

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