/*#include "opencv2/video/tracking.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/video/background_segm.hpp"
#include "opencv2/features2d/features2d.hpp"
#include <stdio.h>*/
//using namespace cv;

// use this one for better implementation/demonstartion:
// http://docs.opencv.org/master/d1/dc5/tutorial_background_subtraction.html#gsc.tab=0 

#include "opencv2/opencv.hpp"

// the function draws all the squares in the image
static void drawShapesContours(Mat& image, const vector<vector<Point> >& ShapesContours)
{
	for (size_t i = 0; i < ShapesContours.size(); i++)
	{
		const Point* p	= &ShapesContours[i][0];
		int n			= (int)ShapesContours[i].size();
		polylines(image, &p, &n, 1, true, Scalar(0, 255, 0), 3, LINE_AA);
	}

	imshow("Capture ", image);
}

int show_forgnd_and_bgnd()
{
    VideoCapture cap;
	String vidName = ""; 
	//String vidName = "pool.avi";
	String StatusText = "NAN";


	if (vidName =="")
		//TODO: need to define resulution
		cap.open(0); // Bu þekliyle uygun olan Web kamerasýný açar ve görüntüyü ordan alýr bu astýrý açarsanýz alttaki satýrý kapatýn
	else
		cap.open(vidName);
	cvWaitKey(100); // initial delay for init
    
    if( !cap.isOpened() )
    {
        puts("***Could not initialize capturing...***\n");
        return 0;
    }

    namedWindow( "Capture "		, CV_WINDOW_AUTOSIZE);
    namedWindow( "Foreground "	, CV_WINDOW_AUTOSIZE );
    Mat frame,foreground,image;
	//utils
	vector<vector<Point> > contours;
	vector<vector<Point> > shapes_contours_obj;
	vector<vector<Point> > shapes_contours;
	int size;

	cv::Ptr<BackgroundSubtractorMOG2> mog = createBackgroundSubtractorMOG2(3/*0*/, 16.0, false);

    int fps=cap.get(CV_CAP_PROP_FPS);
	int loopWait = 0;

    if(fps<=0)
        loopWait=33;
    else
        loopWait=1000/fps; // 1000/30=33; 1000/15=67

    for(;;)
    {
		shapes_contours.clear();

        cap>>frame;   // Bir frame alýyoruz
        if( frame.empty() )
                break;

        image=frame.clone();

		// TODO: resize image if not set in init. or use different resulutions for capture and for analysis.
		//	// to gray, 
		//gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
		//	gray = cv2.GaussianBlur(gray, (21, 21), 0)

        // Arka plan çýkarma kýsmý
		mog->apply(frame,foreground); //foreground = frame;//  ~absdiff()  //using c:\OpenCV\sources\modules\video\src\bgfg_gaussmix2.cpp
        // Ufak tefek temizlik


        threshold	(foreground,	foreground,	128,	255,THRESH_BINARY);
        medianBlur	(foreground,	foreground,	9);
        erode		(foreground,	foreground,	Mat());
        dilate		(foreground,	foreground,	Mat());

		// set min size to moved area
		// draw cursers on screen
		// draw distance of target from screenn center
		// set text strings and text layer to display

		

		/// my addition

		// find contours and store them all as a list
		//findContours(gray, contours, RETR_LIST, CHAIN_APPROX_SIMPLE);
		findContours(foreground.clone(), contours,RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);// See squares.c in the OpenCV sample directory.
		/*# 
			# compute the bounding box for the contour, draw it on the frame,
			# and update the text
			(x, y, w, h) = cv2.boundingRect(c)
			cv2.rectangle(frame, (x, y), (x + w, y + h), (0, 255, 0), 2)
			text = "Occupied"*/
		vector<Point> approx;

			// test each contour
		for (size_t i = 0; i < contours.size(); i++)
		{
			// approximate contour with accuracy proportional to the contour perimeter
			approxPolyDP(Mat(contours[i]), approx, arcLength(Mat(contours[i]), true)*0.02, true);

			// contours should have relatively large area (to filter out noisy contours)
			// and be convex.
			// Note: absolute value of an area is used because
			// area may be positive or negative - in accordance with the
			// contour orientation

			////size = fabs(contourArea(Mat(approx)));
			//if (size > 1000 && size < 151000 && isContourConvex(Mat(approx)))
				//shapes_contours.push_back(approx);
			size = fabs(contourArea(Mat(contours[i])));
			//if (size > 100 && size < 151000 && isContourConvex(Mat(contours[i])))
				shapes_contours.push_back(contours[i]);
		}
        imshow( "Capture "	,image );
		drawShapesContours(image, shapes_contours);
			///////

        // Ekranda gösterme kýsmý
        // image.copyTo(foreground,foreground); // birde böyle deneyin bakalým ne olacak
        imshow("Foreground ",foreground);

        char c = (char)waitKey(loopWait);
        if( c == 27 )   // ESC tuþuna basýlana kadar çalýþ
            break;

    }


}
