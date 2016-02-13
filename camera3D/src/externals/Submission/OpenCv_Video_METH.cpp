#include "stdafx.h"

//#### METHODS OF THE IMAGE_OP CLASS

void Image_OP::Set_Mouse_Callback_for_Image(IplImage *image_with_callback)
{
  
	// sets up mouse callback, enables user to draw rectangle on first frame
	cvSetMouseCallback( "choose area", my_Mouse_Handler, (void*)image_with_callback );
}

void Image_OP::Load_Image(char* file_name)
{
  this->my_loaded_image = cvLoadImage(file_name);
  cvNamedWindow("choose area",CV_WINDOW_AUTOSIZE);
  cvShowImage("choose area",my_loaded_image);

}

void Image_OP::Save_Image(IplImage *img, string file_path)
{
	cvSaveImage(file_path.c_str(),img);
}

void Image_OP::Blur(int square_size )
{

  this->Blur(square_size,this->my_loaded_image);
	
}

void Image_OP::Blur(int square_size,IplImage* orig_img, IplImage* manipulated_img)
{
	if(square_size > 0)
	{
     this->Reset_Manipulators();
	  this->my_pic_manipulators.blur = true;
	  
	if (manipulated_img == NULL)
	{
	  manipulated_img = cvCreateImage(cvSize(orig_img->width,
		         orig_img->height),IPL_DEPTH_8U,3);

	  // parameter 1 => source image; 2 => destination image; 3 => smoothing type;
      // here:CV_BLUR ; alternatives => CV_MEDIAN, CV_GAUSSIAN etc.
	  // 4 and 5 => area on which operation works on 
	  cvSmooth(orig_img, manipulated_img, CV_BLUR,square_size,square_size);
                   
      cvShowImage("blur",manipulated_img); 
	
	  cvReleaseImage(&manipulated_img);
	}
	else
	{
	  cvSmooth(orig_img, manipulated_img, CV_BLUR,square_size,square_size);
	}

	this->my_manipulation_applied = square_size;
	
	}
}

void Image_OP::Erode(int iter)
{
	
  this->Erode(iter,this->my_loaded_image);
}

void Image_OP::Erode(int iter, IplImage * orig_img, IplImage* manipulated_img)
{
	 if( iter > 0)
	{
	 this->Reset_Manipulators();
	  this->my_pic_manipulators.erode = true;
	  
	 if (manipulated_img == NULL)
	 {
       manipulated_img = cvCreateImage(cvSize(orig_img->width,
		                  orig_img->height),IPL_DEPTH_8U,3);

	   // third parameter is structuring element for operations;
	   // if null then 3x3 area; cna be defined by using cvCreatingStructuringElementEx
	   // iter (4th parameter) is the number of applied iterations
	   cvErode(orig_img, manipulated_img, NULL,iter);
                   
       cvShowImage("erode",manipulated_img); 

	   cvReleaseImage(&manipulated_img);
	 }
	 else
       cvErode(orig_img, manipulated_img, NULL,iter);

	 this->my_manipulation_applied = iter;

	 
	 }

}

void Image_OP::Dilate(int iter)
{
	 
		this->Dilate(iter,this->my_loaded_image);	
 
}

void Image_OP::Dilate(int iter, IplImage * orig_img, IplImage* manipulated_img)
{
	if( iter > 0)
	{
	 this->Reset_Manipulators();
	  this->my_pic_manipulators.dilate = true;
	  
    if (manipulated_img == NULL)
	{
       manipulated_img = cvCreateImage(cvSize(orig_img->width,
		                  orig_img->height),IPL_DEPTH_8U,3);

	   // third parameter is structuring element for operations;
	   // if null then 3x3 area; can be defined by using cvCreatingStructuringElementEx
	   // iter (4th parameter) is the number of applied iterations
	   cvDilate(orig_img, manipulated_img, NULL,iter);
                   
       cvShowImage("dilate",manipulated_img); 

	   cvReleaseImage(&manipulated_img);
	}
	else
		cvDilate(orig_img, manipulated_img, NULL,iter);

	 this->my_manipulation_applied = iter;
	 
	 
	 }

}

void Image_OP::Draw_Contours(int threshold)
{
	 
   this->Draw_Contours(threshold,this->my_loaded_image);
	
}

void Image_OP::Draw_Contours(int threshold, IplImage * orig_img, IplImage* manipulated_img)
{
	if( threshold > 0)
	{
	  this->Reset_Manipulators();
	  this->my_pic_manipulators.contour_threshold = true;
	  bool already_exists = true;
     
	  // linked lists of memory blocks (for fast allocation or de-allocation)
	  CvMemStorage* mem_storage = cvCreateMemStorage(0);

	  // found contours are stored in a sequence 
	  CvSeq* contours = 0;

	  // allocates mem for grey-scale image
	  IplImage* gray_img = cvCreateImage(cvSize(orig_img->width,orig_img->height), 
              IPL_DEPTH_8U, 1);
	  int found_contours =0;

	  if (manipulated_img == NULL)
	  {
	  already_exists = false;
	  manipulated_img = cvCreateImage(cvSize(orig_img->width,orig_img->height), 
            IPL_DEPTH_8U, 3);
	       
	  }	
		cvNamedWindow("contours only");
	       
            // converts frame into grey-scale frame
		     cvCvtColor( orig_img, gray_img, CV_RGB2GRAY );

	 			
            // extends threshold range
			int	g_thresh = threshold *5;
	       
		   // defines a threshold for operations
           // creates binary image (only 0 and 1 as pixel values)
           // pixels will be set to 0, to the source value 
           // or to max value depending on threshold type
           // here: CV_THRESH_BINARY => destination 
           // value = if source > threshold then MAX else 0
           // Parameters => 1) source- and 2) destination image
           // 3) threshold, 4) MAX value (255 in 8 bit grayscale) 5) threshold type 
           cvThreshold( gray_img, gray_img, g_thresh, 255, CV_THRESH_BINARY );

            // findings contours; return value is number of found contours
			// Parameters => 1) Image, that is used for computations
            // 2) memory to store recorded contours, 3) pointer for stored contours 4) rest
			// of parameters are optional
	        found_contours =  cvFindContours(gray_img,mem_storage, &contours);

		   // sets all elements of an array to Null
	         cvZero( gray_img );
	       if( contours ){
           // drawing contours: Parameters => 1) Image to draw on, 2) is sequence in which
           // found contours were stored, 3) color of contour, 4) contours marked as a hole
		   // are  drawn ins this color 5) depending on the number of max level contours of 
           // different levels are drawn; rest are optional arguments
		    cvDrawContours(gray_img,contours,cvScalarAll(255),cvScalarAll(255),100 );
		   }
			 
		   this->my_manipulation_applied = threshold;
			
	        cvShowImage("contours only", gray_img);
			
			// turn 1 channel image into 3 channel image (important for CvVideoWriter)
			cvCvtColor( gray_img, manipulated_img, CV_GRAY2RGB );
			// or: cvMerge(gray_img,gray_img,gray_img,NULL,manipulated_img);
		
		cvReleaseImage(&gray_img);
	    
		cvReleaseMemStorage(&mem_storage);

		if(already_exists == false)
			cvReleaseImage(&manipulated_img);

	 }

}



void Image_OP::Reset_Manipulators()
{

  this->my_pic_manipulators.blur = false;
  this->my_pic_manipulators.erode = false;
  this->my_pic_manipulators.dilate = false;
  this->my_pic_manipulators.contour_threshold = false;

}

bool Image_OP::Applies_Manipulator(IplImage *orig_image, IplImage* manipulated_img)
{
  if(this->my_pic_manipulators.blur == true)
     this->Blur(this->my_manipulation_applied,orig_image,manipulated_img);
  else if(this->my_pic_manipulators.erode == true)
     this->Erode(this->my_manipulation_applied,orig_image,manipulated_img);
  else if(this->my_pic_manipulators.dilate == true)
     this->Dilate(this->my_manipulation_applied,orig_image,manipulated_img);
  else if(this->my_pic_manipulators.contour_threshold == true)
     this->Draw_Contours(this->my_manipulation_applied,orig_image, manipulated_img);
  else return false;
  
  return true;

}

//CODE FOR EVENT HANDLING (static functions and variables see Video_OP class definitions)

//Intializing static variables

  CvPoint Image_OP::my_point;
  CvRect  Image_OP::my_ROI;
  //CvCapture *Video_OP::my_capture_static;

//static method for handling mouse events 
void Image_OP::my_Mouse_Handler(int events, int x, int y, int flags, void* param)
{       
	    
    	IplImage *img_orig;	
		// Operations are done on a cloned image
		// so original settings can be restored 
		// if operations need to be repeated
        IplImage *img_clone;
		img_orig = (IplImage*) param;
		int x_ROI =0, y_ROI =0 , wi_ROI =0, he_ROI =0; 
		
   switch(events)
   {//begin switch
	     // event, when user presses left button 
         case CV_EVENT_LBUTTONDOWN:
           { 
             // saves mouse coordinates (x,y) in static (CvPoint) variable 
			 // (by typecasting with cvPoint)
             my_point = cvPoint(x, y);
		   }
		 break;
    
         // user moves the mouse
         case CV_EVENT_MOUSEMOVE:
	       {
		   // user moves mouse and presses left button
	       if (flags == CV_EVENT_FLAG_LBUTTON )
              {
		       // makes a copy of original image 
			   // over which the mouse moves 
               img_clone = cvCloneImage(img_orig);
       
			   // draws green (see CV_RGB(0-255,0-255,0-255) rectangle 
			   // using point coordinates from 
			   // CV_EVENT_LBUTTONDOWN as origin (= upper left corner)
			   // and coordinates of this event as bottom right corner
               cvRectangle(img_clone,my_point,cvPoint(x , y ),
				    CV_RGB(0,255, 0),1, 8, 0);
			   // shows image with rectangle drawn on it
               cvShowImage("choose area", img_clone);
              }
	       }
         break;

         // user releases left button 
         case CV_EVENT_LBUTTONUP: 
	       {
	  
            img_clone = cvCloneImage(img_orig);

			// checks position of starting point
			// stored in CV_EVENT_LBUTTONWDOWN)
			// in relation to end point, in order
			// to avoid negative values

			if(my_point.x > x)
			{
			  x_ROI = x;
			  wi_ROI = my_point.x -x;
			}
			else 
			{
			  x_ROI = my_point.x;
			  wi_ROI = x - my_point.x ;
			}

			if(my_point.y > y)
			{
			  y_ROI = y;
			  he_ROI = my_point.y -y;
			}
			else 
			{
			  y_ROI = my_point.y;
			  he_ROI = y - my_point.y ;
			}

			//stores coordinates of Region of Interest
			// in static variable
			my_ROI.x = x_ROI;
			my_ROI.y = y_ROI;
			my_ROI.width = wi_ROI;
			my_ROI.height = he_ROI;

            // set region of interest; for cvNot operation below
            cvSetImageROI(img_clone,cvRect(x_ROI,
                y_ROI,wi_ROI, he_ROI));
       
		    // inverts color information of image (bitwise inversion of array)
		    // makes selected area clearly visible
            cvNot(img_clone, img_clone);    
            
			// resets region of interest
			cvResetImageROI(img_clone);

			cvShowImage("choose area", img_clone);
			
            
	        }
	     break;
     }//end switch
	
}//end mousehandler

// for handling Trackbar events // 
// Also, requires the function cvCreateTrackBar();
// e.g. 
// cvCreateTrackbar(char* trackbarname,char* window_name, //=> window to which trackbar will be attached
// int* value,int count,CvTrackbarCallback my_Track_Bar_Handler)
/*
void Video_OP::my_Track_Bar_Handler(int pos)
{
	 cvSetCaptureProperty(
        Video_OP::my_capture_static,
        CV_CAP_PROP_POS_FRAMES,
        pos
    );
  

}
*/
/* //OVERVIEW  MOUSEEVENTS
 void mouseEvent(int evt, int x, int y, int flags, void* param)
 The 1st parameter can be one of these ( mouse callback has to be implemented with cvSetMouseCallback())
CV_EVENT_MOUSEMOVE - when the mouse pointer moves over the window (window with mouse callback)
CV_EVENT_LBUTTONDOWN - when the left button of the mouse is pressed 
CV_EVENT_RBUTTONDOWN - when the right button of the mouse is pressed 
CV_EVENT_MBUTTONDOWN - when the middle button of the mouse is pressed 
CV_EVENT_LBUTTONUP - when the left button of the mouse is released 
CV_EVENT_RBUTTONUP - when the right button of the mouse is released 
CV_EVENT_MBUTTONUP - when the middle button of the mouse is released 

 The 2nd and 3rd parameters provide x and y coordinates of the position of mouse pointer 
 The 4th parameter can be one of the following
CV_EVENT_FLAG_LBUTTON - when the left button of the mouse is pressed 
CV_EVENT_FLAG_RBUTTON - when the right button of the mouse is pressed 
CV_EVENT_FLAG_MBUTTON - when the middle button of the mouse is pressed 
CV_EVENT_FLAG_CTRLKEY - when the 'CTRL' key is pressed during a mouse event
CV_EVENT_FLAG_SHIFTKEY - when the 'SHIFT' key is pressed plus any other mouse event 
CV_EVENT_FLAG_ALTKEY - when the 'ALT' key is pressed plus any other mouse event   

*/

//######## Video_OP Class Methods ##################################

bool Video_OP::Get_Video_from_File(char* file_name)
{
       //checks if filename is available
	    if(!file_name)
		  return false;
	    this->my_current_file = file_name;
		// captures video file and sets pointer to CvCapture structure
        my_p_capture = cvCreateFileCapture(file_name); //or cvCaptureFromFile(char*)

       // checks if capturing has been successful; 
       // e.g. function returns NULL if codec for video type is not installed on machine
        if (!my_p_capture)
	      return false;
		
       //gets first frame, to have access to video properties (e.g. for Play_Video() method)
		 my_grabbed_frame = cvQueryFrame( my_p_capture);

       // gets width and height of grabbed frame
		 captured_size.width =(int)cvGetCaptureProperty(my_p_capture, CV_CAP_PROP_FRAME_WIDTH);
		 captured_size.height =(int)cvGetCaptureProperty(my_p_capture, CV_CAP_PROP_FRAME_HEIGHT);

      // resets Image_OP::my_ROI to original size of image
		 my_ROI.x = 0;
         my_ROI.y = 0;
		 my_ROI.width = captured_size.width;
         my_ROI.height = captured_size.height;

      // creates window to display first frame
        cvNamedWindow("choose area",CV_WINDOW_AUTOSIZE);
	     
	 
	  // displays first frame in window
		 cvShowImage("choose area",my_grabbed_frame);

      // gets video's number of frames 
         my_total_frame =  ( int )cvGetCaptureProperty( my_p_capture,CV_CAP_PROP_FRAME_COUNT );

	  // set Mouse callback for first grabbed frame
		 this->Set_Mouse_Callback_for_Image(this->my_grabbed_frame);

		 this->my_loaded_image = my_grabbed_frame;

		

      return true;
}

int Video_OP::Get_Frame_Rate()
{
  // OpenCV code that returns framerate (frames per second )
   return ( int )cvGetCaptureProperty( my_p_capture, CV_CAP_PROP_FPS );
}

int Video_OP::Get_Total_Frames()
{
  //returns total length of video (in frames)
   return my_total_frame;
}

int Video_OP::Get_Current_Frame_Index()
{
  // returns the index of next frame to be decoded (as frame number)
   return captured_size.width;
}

int Video_OP::Get_Height()
{
  // returns height of video in pixels
   return captured_size.height;
}


int Video_OP::Get_Width()
{
	// returns width of video in pixels
	 return (int)cvGetCaptureProperty(my_p_capture, CV_CAP_PROP_FRAME_WIDTH);
}

// does not work without errors for every videoformat
void Video_OP::Go_to_Frame(int frame)
{
   // OpenCV code that drives movie to a particular frame
    int x = cvSetCaptureProperty( my_p_capture, CV_CAP_PROP_POS_FRAMES, (double) frame );

}

void Video_OP::Grab_frame(int at_pos)
{
   // grabs current frame of captured video
   // and pushes pointer to the next frame
   // so that a subsequent call returns the next frame
   this->Go_to_Frame(at_pos);
   cvNamedWindow( "grabbed frame", 0 );
   my_grabbed_frame = cvQueryFrame( my_p_capture);
   cvShowImage( "grabbed frame", my_grabbed_frame );


}


void Video_OP::Stop_Video()
{
  //Interrupts 'while' loop
  //in Play_Movie, Opitcal flow etc.
  my_on_off = false;

}

void Video_OP::Play_Video()
{

  this->Play_Video(0,this->my_total_frame);
	 
}


void Video_OP::Play_Video(int from, int to)
{
    if(to < 0 || to <= from || from < 0){
	    return;
	    }
	if( to ==0) 
		  to = this->Get_Total_Frames();

	CvRect equal_size = cvRect(0,0,my_grabbed_frame->width,my_grabbed_frame->height);
    
	this->Play_Video(from,to,equal_size);

}

void Video_OP::Play_Video(int from, int to, CvRect area)
{
       // checks if file has been loaded successfully
	  if( !my_p_capture ) return;      

      if( area.height == 0 || area.width == 0)
	  {
		  area.height = this->my_grabbed_frame->height;
		  area.width = this->my_grabbed_frame->width;
	  }

	  my_on_off = true;
	  int key =0;
	  int frame_counter = from;
	  
	    // checks  if frame is available
	     if( !my_grabbed_frame ) return;

	   
	    // retrieves frames per second (fps) to set speed of presentation
	    int fps = this->Get_Frame_Rate();
		
		// creates window, in which movie is displayed //see below (A)
		cvNamedWindow( "video", CV_WINDOW_AUTOSIZE ); 
		cvNamedWindow("video area",CV_WINDOW_AUTOSIZE);
		
		// sets pointer to frame where the video should start from 
		 this->Go_to_Frame(from); //or alternative see below
		//int x = cvSetCaptureProperty( my_p_capture, CV_CAP_PROP_POS_FRAMES, (double) from );

		
		
		// allocates memory for image containing Region of Interest (=ROI)
		IplImage * area_frame = cvCreateImage(cvSize(area.width,area.height),my_grabbed_frame->depth, my_grabbed_frame->nChannels);
		 IplImage *after_manipulation_img = cvCreateImage(cvSize(area.width,area.height),IPL_DEPTH_8U, my_grabbed_frame->nChannels);

		 // loop, which is stopped by setting my_on_off = false; 
	     while(my_on_off == true && frame_counter <= to) { 
             // gets a frame 
	          my_grabbed_frame = cvQueryFrame( my_p_capture);
		   
	         // checks if frame is available
	            if( !my_grabbed_frame ) break;
	        
			 // Select region of interest (area that is selected from original image)
                cvSetImageROI(my_grabbed_frame, cvRect(area.x, area.y, area_frame->width, area_frame->height));
              
             // Attention: cvGetSize now returns the width and the height of ROI 
				 
             // copy subimage from original image to destination image
                 cvCopy(my_grabbed_frame, area_frame, NULL);

             // uses manipulator (e.g. blur) that has been invoked before
				if(this->Applies_Manipulator(area_frame,after_manipulation_img))
                 cvCopy(after_manipulation_img,my_grabbed_frame,NULL);
                      
             // resets Region of Interest 
                 cvResetImageROI(my_grabbed_frame);

	         // displays Region of Interest and grabbed frame (=original)
	             cvShowImage( "video area", area_frame);//area_frame );
	             cvShowImage("video",my_grabbed_frame);
				 
				 frame_counter++;

			// if 25 frames per second; 1000 (-> 1000 ms -> 1sec) /25 = 40 ms; 	
		    // breaks loop if 'q' is pressed
			     key = cvWaitKey(1000 /fps);
				 if (key == 'q') 
					 break;

		  }// end while

		  // releases pointer for movie capturing 
	       cvReleaseCapture( &my_p_capture ); 
		
		  // frees memory used for image pointer
		   cvReleaseImage(&area_frame);
		   cvReleaseImage(&after_manipulation_img);
		  // closes windows, in which images are depicted 
	       cvDestroyWindow("video" );
		   cvDestroyWindow("video area" );
		   cvDestroyWindow("choose area");


}


void Video_OP::Create_Dest_Folder(char *folder_name)
{
    destination_folder = folder_name;
	destination_folder += "/DATA/";
}


bool Video_OP::Write_Video(int from, int to, char* path)
{

	 if(to < 0 || to <= from || from < 0){
	    return false;
	    }
	if( to ==0) 
		  to = this->Get_Total_Frames();

	CvRect equal_size = cvRect(0,0,my_grabbed_frame->width,my_grabbed_frame->height);
    
	
	return this->Write_Video(from,to,equal_size,path);

	
		
}

bool Video_OP::Write_Video(int from, int to,CvRect area, char* path)
{

	
    int key =0;
	int frame_counter = from;
	my_on_off = true;

	if( !my_p_capture ) 
			 return false;

	 if( area.height == 0 || area.width == 0)
	  {
		  area.height = this->my_grabbed_frame->height;
		  area.width = this->my_grabbed_frame->width;
	  }

	 

	 // checks  if frame is available
	 if( !my_grabbed_frame ) return false;

	 // sets pointer to position from (cvQuery starts there)
	 this->Go_to_Frame(from );
	 
     // determines frame rate of movie
	 // needed for cvVideoWriter 
	 double fps = this->Get_Frame_Rate();

	// alternative code to get size of video frames
    //  CvSize size_orig = cvSize(
    //   (int)cvGetCaptureProperty( this->my_p_capture, CV_CAP_PROP_FRAME_WIDTH) ,
    //  (int)cvGetCaptureProperty(this->my_p_capture, CV_CAP_PROP_FRAME_HEIGHT) );
   
    // CvSize size = cvSize(area.width,area.height); //OR
	   CvSize size = cvSize(this->Get_Width(),this->Get_Height());

	 cvNamedWindow("write to avi",CV_WINDOW_AUTOSIZE);
	    
	 
        //Creates Video Writer; parameters: (1) path where video is stored; (2) Codec for video-output;
		//(3) frame rate; (4) size of image to be stored; (5) is_color is optional; if 0 => black/white
	    CvVideoWriter *video_writer = cvCreateVideoWriter(path, CV_FOURCC('M','J','P','G'), //corresponding codec must be installed on machine
         fps,size);
		

      // allocates memory for image containing Region of Interest
		IplImage *area_frame = cvCreateImage(cvSize(area.width,area.height),my_grabbed_frame->depth, my_grabbed_frame->nChannels);
	    IplImage *after_manipulation_img = cvCreateImage(cvSize(area.width,area.height),IPL_DEPTH_8U, my_grabbed_frame->nChannels);
	/*
	Examples of Codecs CvVideoWriter supports; provided they are installed on the machine

	CV_FOURCC('P','I','M','1')    = MPEG-1 codec
    CV_FOURCC('M','J','P','G')    = motion-jpeg codec 
    CV_FOURCC('M', 'P', '4', '2') = MPEG-4.2 codec
    CV_FOURCC('D', 'I', 'V', '3') = MPEG-4.3 codec
    CV_FOURCC('D', 'I', 'V', 'X') = MPEG-4 codec
    CV_FOURCC('U', '2', '6', '3') = H263 codec
    CV_FOURCC('I', '2', '6', '3') = H263I codec
    CV_FOURCC('F', 'L', 'V', '1') = FLV1 codec

	*/
	    
		
	//creates loop, which is stopped by setting my_on_off = false; see Stop_Video() method
	     while(my_on_off == true && frame_counter < to) { 
                    
			
		  // for demonstration purposes; frame capturing done by two function calls
		  // this can be done in one step by using cvQueryFrame (see Play_Movie) 
          cvGrabFrame(this->my_p_capture); // captures a frame
		   my_grabbed_frame =cvRetrieveFrame(my_p_capture);  // retrieves the captured frame 
		   if( !my_grabbed_frame) return false;

		    // Select region of interest (area that is selected from original image)
                cvSetImageROI(my_grabbed_frame, cvRect(area.x,area.y, area.width, area.height));
              
             // Attention: cvGetSize now returns the width and the height of ROI 
				 
             // copy subimage from original image to destination image
                 cvCopy(my_grabbed_frame, area_frame, NULL);


				 // uses manipulator that has been invoked before (e.g.blur)
			    if(this->Applies_Manipulator(area_frame, after_manipulation_img))
                   cvCopy(after_manipulation_img,my_grabbed_frame,NULL);

				 // adds the frame to the file; return value is not used here
				   cvWriteFrame(video_writer,my_grabbed_frame);
                // int x = cvWriteFrame(video_writer,after_manipulation_img); 
                 
				
             // resets Region of Interest 
                 cvResetImageROI(my_grabbed_frame);
		  	
		  
		  frame_counter++;
		 
		  cvShowImage("write to avi",my_grabbed_frame); 
          key=cvWaitKey(20);          
         }
      
		 // releasing the video writer plus cvCapture structure plus Image plus closing window

		  cvReleaseVideoWriter(&video_writer);
		  cvDestroyWindow( "write to avi" );
		  cvDestroyWindow("choose area");

		  cvReleaseImage(&area_frame);
		  cvReleaseImage(&after_manipulation_img);
		  cvReleaseCapture(&my_p_capture);
		
       
		  return true;
}

bool Video_OP::Turn_Video_into_Pics( string path)
{

	return this->Turn_Video_into_Pics(0,this->Get_Total_Frames(),path);

}

bool Video_OP::Turn_Video_into_Pics(int from, int to, string path)
{


	 if(to < 0 || to <= from || from < 0){
	    return false;
	    }
	if( to ==0) 
		  to = this->Get_Total_Frames();

	 int key =0;
	 int frame_counter = from;
	
	 // check if movie has been captured
	 if( !my_p_capture ) 
			 return false;

	 this->Go_to_Frame(from );
	 
     
	  string str_num = "";
	  string str = "";
	 
	 double fps = this->Get_Frame_Rate();
     
	 // get frame size 
	 CvSize size = cvSize(this->Get_Width(),this->Get_Height());
	    
		
	//creates loop, which is stopped by setting my_on_off = false; see Stop_Video() method
	     while(my_on_off == true && frame_counter < to) { 
                     
            str = path;
		  // captures video frame
          my_grabbed_frame = cvQueryFrame(this->my_p_capture);
		   if( !my_grabbed_frame) return false;	
		  
		   // add path, filename, "_", and number to output string
		   char buf[9];
	       sprintf(buf,"%d",(int) frame_counter);
           str_num = buf; 
		   str += "\\frame_";
		   str += str_num;
		   str += ".jpg";
		   // save current frame; use above string as filename
		   this->Save_Image(my_grabbed_frame,str);
		  
		   // counts how many frames have been processed
		  frame_counter++;
		 
		  cvShowImage("write to avi",my_grabbed_frame); 
          key=cvWaitKey(20);          
         }
      
		 // releasing the video writer plus cvCapture structure plus Image plus closing window

		  
		  cvDestroyWindow( "write to avi" );

		  
		  
		  cvReleaseCapture(&my_p_capture);
		
       
		  return true;
	

}

void Video_OP::Turn_Pics_into_Video(string *file_names_arr, int arr_size, int frame_rate)
{
	    // load first image of filename array
        IplImage *img = cvLoadImage(file_names_arr[0].c_str());

		// determine size of image for CvVideoWriter
		CvSize size = cvSize(img->width,img->height);

		string file_name_movie = file_names_arr[0];
		// file-ending avi is added to filename of video
		file_name_movie += ".avi";
		
        //Creates Video Writer; parameters: (1) path where video is stored; (2) Codec for videooutput;
		  //(3) frame rate; (4) size of image to be stored; (5) is_color is optional; if 0 => black/white
	    CvVideoWriter *video_writer = cvCreateVideoWriter(file_name_movie.c_str(), CV_FOURCC('M','J','P','G'), //corresponding codec must be installed on machine
         frame_rate,size);

		// adds first image to videofile
		int x = cvWriteFrame(video_writer,img);

		// loops that loads images whose names are stored in array
		// and adds images to avi file
		for(int i = 1; i < arr_size; i++)
		{
           img = cvLoadImage(file_names_arr[i].c_str());
		   x = cvWriteFrame(video_writer,img);
		
		}

		cvReleaseVideoWriter(&video_writer);
		cvReleaseImage(&img);

}


void Video_OP::Go_to_Frame_via_loop(int frame_pos)
{
	int counter = 0;
	if (frame_pos == 0) 
		return;

   while(counter < frame_pos -1)
   {
	   counter++;
	   my_grabbed_frame = cvQueryFrame( my_p_capture);
   }
      
}

void Video_OP::Unload_Video()
{
    // releases memory 
	   if(!my_p_capture) return ;
	    cvReleaseCapture( &my_p_capture ); 
		cvReleaseImage(&this->my_grabbed_frame);
	    
}


int Video_OP::Query_Frame()
{

   
	  // checks if file has been loaded successfully
	  if( !my_p_capture ) return  -1;   	
	     
            // gets a frame 
	        my_grabbed_frame = cvQueryFrame( my_p_capture);
			// check  if frame is available
	        if( !my_grabbed_frame ) return -1;
			//returns current frame index

			return this->Get_Current_Frame_Index();
	       
}

IplImage * Video_OP::Get_Pointer_of_grabbed_frame()
{

  return my_grabbed_frame;
}

CvCapture * Video_OP::Get_Pointer_to_Video()
{

  return my_p_capture;
}



