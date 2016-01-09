
enum WORKING_MODES	{ CALIBRATION=0, REGULAR  , REG_and_REC , PLAYBACK }; //  REGULAR(=no recording) 
enum FRAME_SIZES	{ SMALL=0, MEDIUM , LARGE};

const int numOfActiveCams		= 2;// 2;// use the first # of the next indeces list :

const int camIndexes[3]			= { 0, 1 , 2 };       // default : { 0, 1 , 2 };

const int working_FRAME_WIDTH	= 320;// 640;// 160;
const int working_FRAME_HIGHT	= 240;// 480;// 120;

const int REC_FPS				= 30;	// 10 - will not record
const int REC_CODEC				= CV_FOURCC('D', 'I', 'V', 'X') ; //CV_FOURCC('P', 'I', 'M', '1') ;
const int loop_DELAY			= 3;	//  (int)1000 / REC_FPS; //33;// 10; // [ mS ]  -> 1000/# [Hz]

const int stitching_frame_rate			= 1 ; // 10;	// every # number of frames  - do stitch with previous one.
const int stereo_stitching_frame_rate	= 50 ; // 10;	// every # number of frames  - do stitch between the two stereo ones.

const int CHAR_LEN				= 100;

const char rec_file_name[3][CHAR_LEN]	= { "output_1.avi" , "output_2.avi", "output_3.avi" };
//const char rec_file_name[3][CHAR_LEN] = { "output_1.mp4" , "output_2.avi", "output_3.avi" };
const char window_name[3][CHAR_LEN]		= { "out_1" , "out_2", "out_3" };

const WORKING_MODES working_mode = REGULAR; // PLAYBACK;// REG_and_REC;
const FRAME_SIZES	frame_size   = SMALL;

//---------------------------------------------------

class CMyCam
{
public:
	CMyCam(int HW_cam_index);					// size by default application definition.
	CMyCam(int HW_cam_index, FRAME_SIZES size); // overloading. size by var input
	~CMyCam();

	//get_cam_properties(j);

private:
	VideoCapture	cam;
	int				HW_cam_index;
	int				w, h;
	char			window_name[CHAR_LEN];
};

CMyCam::CMyCam(int HW_cam_index)
{

}

CMyCam::CMyCam(int HW_cam_index, FRAME_SIZES size)
{

}

CMyCam::~CMyCam()
{
}