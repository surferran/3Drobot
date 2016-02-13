
// IMAGE_MANIPULATORS_CLASS (by MKopp)
class Image_OP
{
public:
	Image_OP():my_manipulation_applied(0)
	{this->Reset_Manipulators();}
	virtual ~Image_OP(){}

	//methods
	//  loads an image
	void Load_Image(char* file_name);
	// saves an image
	void Save_Image(IplImage *img, string file_path);
	 // depicts contours of image
	void Depict_Contours();
	// applies simple blur filter (mean of square_size pixel area)
	void Blur(int square_size);
	// applies simple blur  filter and specifies image
	void Blur(int square_size, IplImage *orig_img, IplImage* manipulated_img = NULL);
	// erodes image
	void Erode(int iter);
	// applies erode proceduere to specified image
	void Erode(int iter, IplImage *orig_img, IplImage* manipulated_img = NULL);
	// dilates image
	void Dilate(int iter);
    // applies erode proceduere to specified image
	void Dilate(int iter, IplImage *orig_img,IplImage* manipulated_img = NULL);
	// draws contours
	void Draw_Contours(int threshold);
    // applies erode proceduere to specified image
	void Draw_Contours(int threshold, IplImage *orig_img, IplImage* manipulated_img = NULL);

    // Reset manipulators (all to false)
	void Reset_Manipulators();
	// Applies image manipulator in Play_Video or Write_Video
	bool Applies_Manipulator(IplImage *orig_image,IplImage* manipulated_img = NULL); 

	// struct for selecting a "manipulator"
	   struct my_manipulators_state{
		   bool blur;
		   bool erode;
		   bool dilate;
		   bool contour_threshold;
	   }my_pic_manipulators;

	   // static variables for mouse events 
	   //point mouse operations start from
	   static CvPoint my_point;
	   //saves Region of Interest determined by mouse operations
	   static CvRect my_ROI;

protected:

	void Set_Mouse_Callback_for_Image(IplImage *image_with_callback);
	// static function to handle mouse events
	static void my_Mouse_Handler( int events, int x, int y, int flags, void* param);
	// static function to handle trackbar events
	//static void my_Track_Bar_Slide(int pos);

	int my_manipulation_applied;
	IplImage *my_loaded_image;
	
};


// VIDEO_OP CLASS (by MKopp) ##############################
class Video_OP:public Image_OP
{

//Constructor & Destructor
public:
	Video_OP():my_on_off(true)
	{}
	virtual ~Video_OP(){}

//Methods

	// loads videofile and assigns a pointer to it, returns true, when successful
	bool Get_Video_from_File(char* file_name);
	// returns frame rate (frames per sec) 
	int Get_Frame_Rate();
	// returns total number of frames
	int Get_Total_Frames();
	// returns width of captured video
	int Get_Width();
	// returns height of captured video
	int Get_Height();
	// returns index of current frame
	int Get_Current_Frame_Index();
	// moves video to frame; error prone for some formats
	void Go_to_Frame(int);
	// breaks loop presenting movies
	void Stop_Video();
	// sets up loop to present video
	void Play_Video();
	// sets up loop that runs through movie 'from 'a frame 'to' a frame
	void Play_Video(int from, int to);
	// Plays area of a certain video (from to);CvRect = selected area of video 
	void Play_Video(int from, int to, CvRect area);
    // turns video into avi file; from=first frame to=last frame; 
	bool Write_Video(int from, int to, char* path);
	// turns video into avi file; from=first from to= last frame; CvRect = area selected
	bool Write_Video(int from, int to,CvRect area, char* path);

	// EXTRA METHODS I 
	// turns video into single pictures
	bool Turn_Video_into_Pics(string path);
	// turns video into single pictures
	bool Turn_Video_into_Pics(int from, int to,string path);
	// turns pics into video
	void Turn_Pics_into_Video(string * file_names_arr, int arr_size, int frame_rate);
   

	//EXTRA METHODS II
	//alternative Go_to_frame method; is slow, but accurate
	void Go_to_Frame_via_loop(int frame_pos);
	//calculates contours of image
	void Unload_Video();
	//gets current frame (as image) from loaded movie and returns index
	int Query_Frame();
	//returns pointer to cvCapture structure
	CvCapture* Get_Pointer_to_Video();
	//returns pointer to grabbed frame
	IplImage* Get_Pointer_of_grabbed_frame();
	//grabs frame of loaded movie 
    void Grab_frame(int at_pos);
	//Create a destination folder
	void Create_Dest_Folder(char *folder_name);

   
//class Variables
protected:
	
	IplImage *my_first_frame;
	CvCapture * my_p_capture;

	CvSize captured_size;
	string my_current_file;
	bool my_on_off;
	
	int my_total_frame;
	IplImage  *my_grabbed_frame; 
	
	string destination_folder;
	

};


