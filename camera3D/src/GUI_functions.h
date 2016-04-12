/*
GUI_functions.h file
*/
//////////

#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;

enum USER_CHOISE  {
					USR_PLAY_VID  = 0,  
					USR_STOP_PLAY = 1,  // PAUSE FOR OPTION TO SIGN TARGET AND SO. FOR VID FLIE OR LIVE CAPTURING(SHOW ANOTHER WINDOW OF CONTINUING FEED)
					USR_ASIGN_TRACKING_TARGET_MANUALLY = 2 //WHEN VIDEO PAUSED, OR BY 1 PRESS ON MIDDLE BODY WHEN VIDEO PLAYS
};

struct Operation_flags{
	bool play_on				= false;
	bool make_camshift			= false;
	bool reset_vid_file_location = false;
	bool reset_identification	= false;
	bool show_img_hist			= false;
	bool proces_img_frame		= false;
}op_flags;

// GLOBALS
static const int thumb_num = 10;
String plotWindows[thumb_num];
Mat    images[thumb_num]; 

/* functions headers */
void draw_output_frames(String* WinNames, Mat* images);
//void check_user_input();

/* functions */
void draw_output_frames(String* WinNames, Mat* images)
{
	for (int i=0; i<thumb_num ; i++)
	{
		imshow( WinNames[i]	, images[i] );
	}
	
}

// return user input , or 0(zero) if pressed ESC, -1 as default for no special input.
// p - start/stop play video file. in window 1
// s - set identification to/by current frame 
// a - operate/turn-off the camshift algorithm on given frame
// r - re-init video reading.
bool check_user_input(int* waiting_delay, char* c)   
{
	*c = (char)waitKey(*waiting_delay);
	if( *c == 27 )
		return false;
	switch(*c)
	{
	case 'p':
		op_flags.play_on					= ! op_flags.play_on ;
		break;
	case 's':
		op_flags.reset_identification		= true;
		break;
	case 'a':
		op_flags.make_camshift				= !op_flags.make_camshift ;
		break;
	case 'r':
		op_flags.reset_vid_file_location	= true;
		break;
	case 'd':
		
		break;

	//default: ;
	}
	return true;
}