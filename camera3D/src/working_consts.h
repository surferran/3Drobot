#ifndef WORKING_CONSTS_H
#define WORKING_CONSTS_H

using namespace cv;

enum WORKING_MODES	{ CALIBRATION=0, REGULAR  , REG_and_REC , PLAYBACK }; //  REGULAR(=no recording) 
enum FRAME_SIZES	{ SMALL=0, MEDIUM , LARGE};

const int numOfActiveCams		= 2;// 2;// use the first # of the next indeces list :

const int camIndexes[3]			= { 0, 1 , 2 };       // default : { 0, 1 , 2 };

const int working_FRAME_WIDTH	= 320;// 640;// 160;
const int working_FRAME_HIGHT	= 240;// 480;// 120;

const int REC_FPS				= 15;// 30;	// 10 - will not record
const int REC_CODEC				= CV_FOURCC('D', 'I', 'V', 'X') ; //CV_FOURCC('P', 'I', 'M', '1') ;
const int loop_DELAY = 75;// for 15fps ;// 34; // for camera highest frame rate	//  (int)1000 / REC_FPS; //33;// 10; // [ mS ]  -> 1000/# [Hz]

const int stitching_frame_rate			= 1 ; // 10;	// every # number of frames  - do stitch with previous one.
const int stereo_stitching_frame_rate	= 50 ; // 10;	// every # number of frames  - do stitch between the two stereo ones.

const int CHAR_LEN				= 100;

const char rec_file_name[3][CHAR_LEN]	= { "output_1.avi" , "output_2.avi", "output_3.avi" };
//const char rec_file_name[3][CHAR_LEN] = { "output_1.mp4" , "output_2.avi", "output_3.avi" };
const char window_name[3][CHAR_LEN]		= { "out_1" , "out_2", "out_3" };

const FRAME_SIZES	frame_size   = SMALL;

/* for util functions : */
const String STEREO_CALIBRATION_IMAGES_LIST = "fileList.txt";	// will include files names in left-right pairs order
const String STEREO_CALIBRATION_VIDEO_PAIR  = "output_#.avi";  // '#' will be 1,2 , respectivley for cams 0,1

															   /* constants */
enum USER_STATUS_SELECTION {
	JUST_INITIALIZED	=	0 ,		 // mode 0 is the initialization. as NULL
	CAPTURE_CALIBRATION_IMAGES	=	1,			// right - index 0, window title as 1. 	// left - index 1, window title as 2. 
	CALIBRATION_STEREO,
	STREAM_LIVE_FROM_STEREO,
	STREAM_WITH_DISPARITY_AND_DEPTH
};
 
enum VIDEO_SOURCE {					// each 2 images will be populated from 
	STREAM_STEREO_CAMS = 1,		// real-time capture 
	RECORDED_VIDEOS_COUPLE,		// ready-made couple of video files
	IMAGES_LIST							// pairs of pre-captured Left-Right images
};

struct CalibStruct {
	Size imSize;
	Mat
		CM1, CM2,	// cameraMatrix1,2
		D1, D2,		// distCoeffs1,2   //need to add calibrated imageSize
		R,
		T,
		E,
		F,
		R1, R2,
		P1, P2,
		Q;
};

struct StereoCams
{
	VIDEO_SOURCE	input_source	;
	VideoCapture	cams			[numOfActiveCams];			// set the cameras buffers. 0 for Left, 1 for Right
	CalibStruct		calib_mtx		[numOfActiveCams];			// the calculated/pre-calculated cammeras parameters
	VideoWriter     rec_videos		[numOfActiveCams];		// recordings buffers for stream_input recording

	Mat				raw_frame		[numOfActiveCams];		// intermidiate buffer for capturing
	Mat				modeified_frame	[numOfActiveCams];		// modified buffer for optional processing for each step

	int				vid_len			[numOfActiveCams];		// relevant for pre-recorded input video
	Size			vid_res			[numOfActiveCams];		// relevant for pre-recorded input video

};

StereoCams thisStereo;				// global 


//---------------------------------------------------
//
//class CMyCam
//{
//public:
//	CMyCam(int HW_cam_index);					// size by default application definition.
//	CMyCam(int HW_cam_index, FRAME_SIZES size); // overloading. size by var input
//	~CMyCam();
//
//	//get_cam_properties(j);
//
//private:
//	VideoCapture	cam;
//	int				HW_cam_index;
//	int				w, h;
//	char			window_name[CHAR_LEN];
//};
//
//CMyCam::CMyCam(int HW_cam_index)
//{
//
//}
//
//CMyCam::CMyCam(int HW_cam_index, FRAME_SIZES size)
//{
//
//}
//
//CMyCam::~CMyCam()
//{
//}

#endif //WORKING_CONSTS_H