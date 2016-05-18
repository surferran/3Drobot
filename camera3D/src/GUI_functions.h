/*
GUI_functions.h file
*/
//////////

#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;

void show_vidSource_options_gui()
;

#include "utilFunctions.h" 

//general app options
enum USER_CHOISE  {
					USR_PLAY_VID  = 0,  
					USR_STOP_PLAY = 1,  // PAUSE FOR OPTION TO SIGN TARGET AND SO. FOR VID FLIE OR LIVE CAPTURING(SHOW ANOTHER WINDOW OF CONTINUING FEED)
					USR_ASIGN_TRACKING_TARGET_MANUALLY = 2 //WHEN VIDEO PAUSED, OR BY 1 PRESS ON MIDDLE BODY WHEN VIDEO PLAYS
};

struct Operation_flags{
	bool make_stereo_calibration=	false;
	bool calc_background_subs	=	false;
	bool show_vid_source_selection =false;

	bool show_stereo			=	false;
	bool play_on				=	false;

	bool make_camshift			=	false;

	bool reset_vid_file_location =	false;
	bool reset_identification	=	false;

	bool show_img_hist			=	false;
	bool proces_img_frame		=	false;
}op_flags;

// GLOBALS for the GUI stuff
static const int thumb_num = 10;
static const int buttons_num = 15;
String	plotWindowsNames[thumb_num];
Mat		plotImages[thumb_num]; 
Point	LastMousePressPos;
int		userMouseBtnSelection = -1; // 1..buttons_num
Rect	boundRect[buttons_num];

/* functions headers . partial.. */
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

// checks where the user pressed on gui window, and returns the
// number of relevant button indexed it pressed on it area.
static void onMousePress( int event, int x, int y, int, void* )
{
 	switch( event )
	{
	case EVENT_LBUTTONDOWN:
		LastMousePressPos		= Point(x,y);
		userMouseBtnSelection	= -1;

		for (int i=0 ; i<4; i++){   //buttons_num//TODO: seperate for each gui window.
			if ( LastMousePressPos.inside (boundRect[i]) )
			{
				printf("indeed %d index \n",i); //TODO: update in 'status'  line/section
				userMouseBtnSelection	= i+1;//
			}
		}		
		break;
	case EVENT_LBUTTONUP: 
		break;
	}
}
// for the video source selection window 
static void onMousePress2( int event, int x, int y, int, void* )
{
	if (event == EVENT_LBUTTONDOWN){
		LastMousePressPos		= Point(x,y);
		userMouseBtnSelection	= -1;

		for (int i=4 ; i<buttons_num; i++){
			if ( LastMousePressPos.inside (boundRect[i]) )
			{
				printf("indeed %d index \n",i); //TODO: update in 'status'  line/section
				userMouseBtnSelection	= 30+  i-2;//
			}
		}		
	}
}

// return user input , or 0(zero) if pressed ESC, -1 as default for no special input.
// p - start/stop play video file. in window 1
// s - set identification to/by current frame 
// a - operate/turn-off the camshift algorithm on given frame
// r - re-init video reading.
//
// mouse pressing result treatment. result of user pressing on button 1, or 2 (at the moment only 2 buttons).
bool check_user_input(int* waiting_delay, char* c)   
{
	/*keyboard*/
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
	}

	/*mouse*/
	switch (userMouseBtnSelection)
	{
	case 1: 
		op_flags.make_stereo_calibration = true;
		userMouseBtnSelection=-1;
		break;
	case 2: // start utils.
		////////////////////////////////////////////
		op_flags.calc_background_subs = true;
		userMouseBtnSelection=-1;
		////////////////////////////////////////////
		break;
	case 3:// choose vid source
		op_flags.show_vid_source_selection = true;
		show_vidSource_options_gui();		
		userMouseBtnSelection=-1;
		break;
	case 4:// show video panels
		   //// start the displays and wait for tracking		
		op_flags.show_stereo = true;
		userMouseBtnSelection=-1;
		break;
	case 31:
		thisStereo.input_source = LIVE_CAM;		
		cout << "change video source to LIVE " << thisStereo.input_source << '\n';
		userMouseBtnSelection=-1;
		break;
	case 32:
		thisStereo.input_source = RECORDED_VIDEO_FILE;		
		cout << "change video source to FILE " << thisStereo.input_source << '\n';
		userMouseBtnSelection=-1;
		break;
	case 33:
		thisStereo.input_source = IMAGES_LIST;		
		cout << "change video source to IMAGES_LIST " << thisStereo.input_source << '\n';
		userMouseBtnSelection=-1;
		break;
	default: //for calibration or else
		break;
	}


	return true;
}


void show_buttons_gui()
{
	String WinName	= "User controls"; // buttons,status text
	int num_of_displayed_btns = 4;
	std::string text[buttons_num];
	text[0]			= " stereo calibration procedure ";// for stereo. by pre-set images and xml file";
		//" show stereo stream "; 
	text[1]			= " calculate background substruction ";//maximum length string
	text[2]			= " select video source ";
				/// 4->num_of_displayed_btns
	Mat all_btns_im	(10+30*1.5*4,  250*1.5 , CV_8UC3, Scalar(10 , 10 , 10 ));   // hight, width,type,.. //create_empty_image() 
	Mat btn_im		( 20*1.5	,  240*1.5 , CV_8UC3, Scalar(30, 30, 30));  // hight, width,type,.. //create_empty_image() 
	Mat clear_btn_im ;
	btn_im.copyTo(clear_btn_im);
	int originX	,	originY	;
	int text_in_label_location_X	,	text_in_label_location_Y;
	Rect roi;

	namedWindow(WinName, CV_WINDOW_AUTOSIZE); //create a window 
	moveWindow(WinName, 5,155);

	// TODO: add text to show working directory. 
	// and calibration files directory.
	// and options possibilities (the enum)

	setMouseCallback(WinName , onMousePress, 0 );

	text_in_label_location_X	=	5;
	text_in_label_location_Y	=	18;
		//gives x,y as left lower // return is w,h + l,top	//. or set boundaries for buttons table	
	setLabel(btn_im, text[0], cvPoint(text_in_label_location_X, text_in_label_location_Y));		
	originX			= 10 ;
	originY			= 10 ;
	boundRect[0]	= Rect (originX - 3, originY - 5, originX + 310*1.5, originY + 10*1.5);	//x,y,w,h //boundRect[0]	= Rect (+ 7, + 5,320,20);
	roi				= Rect ( Point( originX, originY ), btn_im.size() );
	btn_im.copyTo( all_btns_im( roi ) );
	clear_btn_im.copyTo(btn_im);

	setLabel(btn_im, text[1], cvPoint(text_in_label_location_X, text_in_label_location_Y));	
	originX			= 10 ;
	originY			= 10+30*1.5 ;
	boundRect[1]	= Rect (originX - 3, originY - 5, originX + 310*1.5, originY + 10*1.5);	//x,y,w,h
	roi				= Rect( Point( originX, originY ), btn_im.size() );
	btn_im.copyTo( all_btns_im( roi ) );
	clear_btn_im.copyTo(btn_im);

	setLabel(btn_im, text[2], cvPoint(text_in_label_location_X, text_in_label_location_Y));	
	originX			= 10 ;
	originY			= 10+30*1.5*2 ;
	boundRect[2]	= Rect (originX - 3, originY - 5, originX + 310*1.5, originY + 10*1.5);	//x,y,w,h
	roi				= Rect( Point( originX, originY ), btn_im.size() );
	btn_im.copyTo( all_btns_im( roi ) );
	clear_btn_im.copyTo(btn_im);

	text[3]			= " start video panel ";
	setLabel(btn_im, text[3], cvPoint(text_in_label_location_X, text_in_label_location_Y));	
	originX			= 10 ;
	originY			= 10+30*1.5*3 ;
	boundRect[3]	= Rect (originX - 3, originY - 5, originX + 310*1.5, originY + 10*1.5);	//x,y,w,h
	roi				= Rect( Point( originX, originY ), btn_im.size() );
	btn_im.copyTo( all_btns_im( roi ) );
	clear_btn_im.copyTo(btn_im);

	imshow(WinName, all_btns_im);

}


void show_vidSource_options_gui()
{
	String WinName	= "video source"; // buttons,status text
	std::string text[3];
	text[0]			= " live stream "; 
	text[1]			= " pre-captured file";
	text[2]			= " images list - not feasable yet..";
	Mat all_btns_im	(100*1.5,  250*1.5 , CV_8UC3, Scalar(10 , 10 , 10 ));   // hight, width,type,.. //create_empty_image() 
	Mat btn_im		( 20*1.5,  240*1.5 , CV_8UC3, Scalar(30, 30, 30));  // hight, width,type,.. //create_empty_image() 
	Mat clear_btn_im ;
	btn_im.copyTo(clear_btn_im);
	int originX	,	originY	;
	int text_in_label_location_X	,	text_in_label_location_Y;
	Rect roi;

	namedWindow(WinName, CV_WINDOW_AUTOSIZE); 
	moveWindow(WinName, 355,155);

	// TODO: add text to show working directory. 
	// and calibration files directory.
	// and options possibilities (the enum)

	setMouseCallback(WinName , onMousePress2, 0 );

	text_in_label_location_X	=	5;
	text_in_label_location_Y	=	18;
	//gives x,y as left lower // return is w,h + l,top	//. or set boundaries for buttons table	
	setLabel(btn_im, text[0], cvPoint(text_in_label_location_X, text_in_label_location_Y));		
	originX			= 10 ;
	originY			= 10 ;
	boundRect[3]	= Rect (originX - 3, originY - 5, originX + 310*1.5, originY + 10*1.5);	//x,y,w,h //boundRect[0]	= Rect (+ 7, + 5,320,20);
	roi				= Rect ( Point( originX, originY ), btn_im.size() );
	btn_im.copyTo( all_btns_im( roi ) );
	clear_btn_im.copyTo(btn_im);

	setLabel(btn_im, text[1], cvPoint(text_in_label_location_X, text_in_label_location_Y));	
	originX			= 10 ;
	originY			= 10+30*1.5 ;
	boundRect[4]	= Rect (originX - 3, originY - 5, originX + 310*1.5, originY + 10*1.5);	//x,y,w,h
	roi				= Rect( Point( originX, originY ), btn_im.size() );
	btn_im.copyTo( all_btns_im( roi ) );
	clear_btn_im.copyTo(btn_im);

	setLabel(btn_im, text[2], cvPoint(text_in_label_location_X, text_in_label_location_Y));	
	originX			= 10 ;
	originY			= 10+30*1.5*2 ;
	boundRect[5]	= Rect (originX - 3, originY - 5, originX + 310*1.5, originY + 10*1.5);	//x,y,w,h
	roi				= Rect( Point( originX, originY ), btn_im.size() );
	btn_im.copyTo( all_btns_im( roi ) );
	clear_btn_im.copyTo(btn_im);

	imshow(WinName, all_btns_im);

}

/////////////////////////////

//int to string helper function
string _intToString(int number){

	//this function has a number input and string output
	std::stringstream ss;
	ss << number;
	return ss.str();
}
/////////////////////////////

/*
add sign of cross on the x,y point, on the delivered image.
*/
void add_Target_Cross_to_Image(int x, int y, Mat &cameraFeed){
	//draw some crosshairs around the object
	circle(cameraFeed,Point(x,y),20,Scalar(0,255,0),2);
	line(cameraFeed,Point(x,y),Point(x,y-25),Scalar(0,255,0),2);
	line(cameraFeed,Point(x,y),Point(x,y+25),Scalar(0,255,0),2);
	line(cameraFeed,Point(x,y),Point(x-25,y),Scalar(0,255,0),2);
	line(cameraFeed,Point(x,y),Point(x+25,y),Scalar(0,255,0),2);

	// TODO: consider print it in the imaage corner. in status bar for example.
	//write the position of the object to the screen
	putText(cameraFeed,"Tracking object at (" + _intToString(x)+","+_intToString(y)+")",Point(x,y),1,1,Scalar(255,0,0),2);

}
