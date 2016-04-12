/*#include "opencv2/video/tracking.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/video/background_segm.hpp"
#include "opencv2/features2d/features2d.hpp"
#include <stdio.h>*/
//using namespace cv;

// use this one for better implementation/demonstartion:
// http://docs.opencv.org/master/d1/dc5/tutorial_background_subtraction.html#gsc.tab=0 

// ref by http://docs.opencv.org/2.4/doc/tutorials/imgproc/shapedescriptors/bounding_rects_circles/bounding_rects_circles.html 
// http://docs.opencv.org/3.1.0/dd/d49/tutorial_py_contour_features.html#gsc.tab=0

#include "opencv2/opencv.hpp"
using namespace cv;


// the function draws all the squares in the image
//static void drawShapesContours(Mat& image, const vector<vector<Point> >& ShapesContours)
static void drawShapesContours(Mat& image, const vector<vector<Point> >& ShapesContours,
	const vector<Moments> & curves_moments, vector<Point2f>& mass_centers)
{
	for (size_t i = 0; i < ShapesContours.size(); i++)
	{
		const Point*	p = &ShapesContours[i][0];
		int				n = (int)ShapesContours[i].size();
		polylines(image, &p, &n, 1, true, Scalar(255, 255, 0), 3, LINE_AA);

		if (!mass_centers.empty()) {
			circle(image, mass_centers[i], 4, Scalar(0, 255, 255), -1, 8, 0);
		}
		if (!curves_moments.empty()) {
			//draw text of data about it, near the mass center 
		}
	}

	imshow("Capture ", image);
}

int show_forgnd_and_bgnd()
{
    VideoCapture			cap;
	String					vidName = ""; 
	//String				vidName = "pool.avi";
	String					StatusText = "NAN";

	int						fps;
	Mat						frame, foreground, image;
	vector<vector<Point> >	contours;
	vector<vector<Point> >	selected_shapes_contours;
	int						area;
	Rect					rect;

	cv::Ptr<BackgroundSubtractorMOG2> mog = createBackgroundSubtractorMOG2(330, 16.0, false);
	Scalar					random_color;
	int						loopWait = 0;
	RNG						rng(12345);
	int						found_contours_num ;

	if (vidName =="")
		//TODO: need to define resulution, and frame rate
		cap.open(0); // Bu þekliyle uygun olan Web kamerasýný açar ve görüntüyü ordan alýr bu astýrý açarsanýz alttaki satýrý kapatýn
	else
		cap.open(vidName);

	cvWaitKey(1000); // initial delay for init
    
    if( !cap.isOpened() )
    {
        puts("***Could not initialize capturing...***\n");
        return 0;
    }

	fps = cap.get(CV_CAP_PROP_FPS);
    if(fps<=0)
        loopWait=33;
    else
        loopWait=1000/fps; // 1000/30=33; 1000/15=67

    namedWindow( "Capture "		, CV_WINDOW_AUTOSIZE);
    namedWindow( "Foreground "	, CV_WINDOW_AUTOSIZE );

    for(;;)
    {
		/* get new frame */
        cap	>>	frame;   
        if( frame.empty() )
                break;

		// clear list, copy image
		selected_shapes_contours.clear();
        image	=	frame.clone();

		/* apply background substraction and manipulate the resultant frame */
		mog->apply(frame,foreground); 

        threshold	(foreground,	foreground,	128,	255,THRESH_BINARY);//28,128,198
        medianBlur	(foreground,	foreground,	3);//9
        erode		(foreground,	foreground,	Mat());
        dilate		(foreground,	foreground,	Mat());

		// find contours and store them all as a list
		findContours(foreground.clone(), contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);// See squares.c in the OpenCV sample directory.
		found_contours_num = contours.size();

		vector<Point> approx;

		/// Approximate contours to polygons + get bounding rects and circles
		vector<vector<Point> >	contours_poly	(found_contours_num);
		vector<Rect>			boundRect		(found_contours_num);
		vector<Point2f>			center			(found_contours_num);
		vector<float>			radius			(found_contours_num);
		vector<Moments>			curves_moments	(found_contours_num);
		vector<Point2f>			mass_centers	(found_contours_num);
		// draw cursers on screen
		// draw distance of target from screenn center
		// set text strings and text layer to display		

		// TODO: resize image if not set in init. or use different resulutions for capture and for analysis.
		//	//TODO:  to gray, and low freq noise reduction
		//gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
		//	gray = cv2.GaussianBlur(gray, (21, 21), 0)

		//////////////////////////
//			 test each contour
		StatusText = "";
		if (found_contours_num > 0) {
			StatusText = " sensed some movement ";
		}
		vector<Vec4i> hierarchy;
		for (size_t i = 0; i <found_contours_num ; i++)
		{
			// approximate contour with accuracy proportional to the contour perimeter
		    approxPolyDP(Mat(contours[i]), approx, arcLength(Mat(contours[i]), true)*0.02, true);
			boundRect[i] = boundingRect(approx);
			random_color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
			if (SHOW_MOVING_CONTOURS)
				//drawContours(image, approx, i, random_color, 1, 8, vector<Vec4i>(), 0, Point());
				drawContours(image, contours, i, random_color, 1, 8, vector<Vec4i>(), 0, Point());
			if (SHOW_MOVING_RECTANGLES)
				rectangle(image,
					boundRect[i].tl(), boundRect[i].br(),
					random_color,		1, 8, 0);
			// contours should have relatively large area (to filter out noisy contours)
			// and be convex.
			// Note: absolute value of an area is used because area may be positive or negative - 
			// in accordance with the contour orientation
			
			area = fabs(contourArea(Mat(contours[i])));
			if (area > MIN_CURVE_AREA && area < MAX_CURVE_AREA && isContourConvex(Mat(contours[i]))) {
				StatusText = "--Major Movement detected--";

				///// .push_back(area);->weighted area->center of movement, and avereged
				selected_shapes_contours	.push_back(contours[i]);///approx
				curves_moments				.push_back(moments(contours[i], false));  // not for display. just for info or other use.
				mass_centers				.push_back(Point2f(	curves_moments[i].m10 / curves_moments[i].m00,
																curves_moments[i].m01 / curves_moments[i].m00));
				if (SHOW_MOVING_BIG_CONTOURS)
					//drawContours(image, approx, i, random_color, 1, 8, vector<Vec4i>(), 0, Point());
					drawContours(image, contours, i, random_color, 1, 8, vector<Vec4i>(), 0, Point());

			}
		}
		// keep history of points.
		// dispay a line of done trajectory in 3D space, and show prediction for planned trajectory.
		
		/* displays and drawings */
		putText(image, StatusText, Point(10, 30), FONT_HERSHEY_COMPLEX, 0.5, (0, 0, 255), 2);
		drawShapesContours(image, selected_shapes_contours, curves_moments, mass_centers);
       // imshow( "Capture "		,image );
        imshow( "Foreground "	,foreground);

		/* wait shortly for optional user input */
        char c = (char)waitKey(loopWait);
        if( c == 27 )   // ESC key
            break;

    }


}
