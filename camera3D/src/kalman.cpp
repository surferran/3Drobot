#include "opencv2/video/tracking.hpp"
#include "opencv2/highgui/highgui.hpp"

#include <stdio.h>

using namespace cv;

int main2();

static inline Point calcPoint(Point2f center, double R, double angle)
{
    return center + Point2f((float)cos(angle), (float)-sin(angle))*(float)R;
}

static void help()
{
    printf( "\nExample of c calls to OpenCV's Kalman filter.\n"
"   Tracking of rotating point.\n"
"   Rotation speed is constant.\n"
"   Both state and measurements vectors are 1D (a point angle),\n"
"   Measurement is the real point angle + gaussian noise.\n"
"   The real and the estimated points are connected with yellow line segment,\n"
"   the real and the measured points are connected with red line segment.\n"
"   (if Kalman filter works correctly,\n"
"    the yellow segment should be shorter than the red one).\n"
            "\n"
"   Pressing any key (except ESC) will reset the tracking with a different speed.\n"
"   Pressing ESC will stop the program.\n"
            );
}

int main(int, char**)
{

	main2();


    help();
    Mat img(500, 500, CV_8UC3);
    KalmanFilter KF(2, 1, 0);
    Mat state(2, 1, CV_32F); /* (phi, delta_phi) */
    Mat processNoise(2, 1, CV_32F);
    Mat measurement = Mat::zeros(1, 1, CV_32F);
    char code = (char)-1;

    for(;;)
    {
        randn( state, Scalar::all(0), Scalar::all(0.1) );
        KF.transitionMatrix = (Mat_<float>(2, 2) << 1, 1, 0, 1);

        setIdentity(KF.measurementMatrix);
        setIdentity(KF.processNoiseCov, Scalar::all(1e-5));
        setIdentity(KF.measurementNoiseCov, Scalar::all(1e-1));
        setIdentity(KF.errorCovPost, Scalar::all(1));

        randn(KF.statePost, Scalar::all(0), Scalar::all(0.1));

        for(;;)
        {
            Point2f center(img.cols*0.5f, img.rows*0.5f);
            float R = img.cols/3.f;
            double stateAngle = state.at<float>(0);
            Point statePt = calcPoint(center, R, stateAngle);

            Mat prediction = KF.predict();
            double predictAngle = prediction.at<float>(0);
            Point predictPt = calcPoint(center, R, predictAngle);

            randn( measurement, Scalar::all(0), Scalar::all(KF.measurementNoiseCov.at<float>(0)));

            // generate measurement
            measurement += KF.measurementMatrix*state;

            double measAngle = measurement.at<float>(0);
            Point measPt = calcPoint(center, R, measAngle);

            // plot points
            #define drawCross( center, color, d )                                        \
                line( img, Point( center.x - d, center.y - d ),                          \
                             Point( center.x + d, center.y + d ), color, 1, LINE_AA, 0); \
                line( img, Point( center.x + d, center.y - d ),                          \
                             Point( center.x - d, center.y + d ), color, 1, LINE_AA, 0 )

            img = Scalar::all(0);
            drawCross( statePt, Scalar(255,255,255), 3 );
            drawCross( measPt, Scalar(0,0,255), 3 );
            drawCross( predictPt, Scalar(0,255,0), 3 );
            line( img, statePt, measPt, Scalar(0,0,255), 3, LINE_AA, 0 );
            line( img, statePt, predictPt, Scalar(0,255,255), 3, LINE_AA, 0 );

            if(theRNG().uniform(0,4) != 0)
                KF.correct(measurement);

            randn( processNoise, Scalar(0), Scalar::all(sqrt(KF.processNoiseCov.at<float>(0, 0))));
            state = KF.transitionMatrix*state + processNoise;

            imshow( "Kalman", img );
            code = (char)waitKey(100);

            if( code > 0 )
                break;
        }
        if( code == 27 || code == 'q' || code == 'Q' )
            break;
    }

    return 0;
}


/////////////////////////////////////////
/////http://opencvexamples.blogspot.com/2014/01/kalman-filter-implementation-tracking.html 

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/video/tracking.hpp"
///#include <Windows.h>

#define drawCross( center, color, d )                                 \
line( img, Point( center.x - d, center.y - d ), Point( center.x + d, center.y + d ), color, 2, CV_AA, 0); \
line( img, Point( center.x + d, center.y - d ), Point( center.x - d, center.y + d ), color, 2, CV_AA, 0 )

using namespace cv;
using namespace std;

Point mousePos;

static void onMouseHover( int event, int x, int y, int, void* ){
	mousePos.x = x;
	mousePos.y = y;
}

int main2( )
{ 

	KalmanFilter KF(4, 2, 0);
	///POINT 
//		Point mousePos;

	///GetCursorPos(&mousePos);

	namedWindow("mouse kalman",0);
	setMouseCallback( "mouse kalman", onMouseHover, 0 );

	// intialization of KF...
	KF.transitionMatrix = (Mat_<float>(4, 4) << 1,0,1,0,   0,1,0,1,  0,0,1,0,  0,0,0,1);
	Mat_<float> measurement(2,1); measurement.setTo(Scalar(0));


	KF.statePre.at<float>(0) = mousePos.x;
	KF.statePre.at<float>(1) = mousePos.y;
	KF.statePre.at<float>(2) = 0;
	KF.statePre.at<float>(3) = 0;
	setIdentity(KF.measurementMatrix);
	setIdentity(KF.processNoiseCov, Scalar::all(1e-4));
	setIdentity(KF.measurementNoiseCov, Scalar::all(10));
	setIdentity(KF.errorCovPost, Scalar::all(.1));
	// Image to show mouse tracking
	Mat img(600, 800, CV_8UC3);
	vector<Point> mousev,kalmanv;
	mousev.clear();
	kalmanv.clear();

	while(1)
	{
		// First predict, to update the internal statePre variable
		Mat prediction = KF.predict();
		Point predictPt(prediction.at<float>(0),prediction.at<float>(1));

		// Get mouse point
		///GetCursorPos(&mousePos);
		measurement(0) = mousePos.x;
		measurement(1) = mousePos.y; 

		// The update phase 
		Mat estimated = KF.correct(measurement);

		Point statePt(estimated.at<float>(0),estimated.at<float>(1));
		Point measPt(measurement(0),measurement(1));
		// plot points
		imshow("mouse kalman", img);
		img = Scalar::all(0);

		mousev.push_back(measPt);
		kalmanv.push_back(statePt);
		drawCross( statePt, Scalar(255,255,255), 5 );
		drawCross( measPt, Scalar(0,0,255), 5 );

		for (int i = 0; i < mousev.size()-1; i++) 
			line(img, mousev[i], mousev[i+1], Scalar(255,255,0), 1);

		for (int i = 0; i < kalmanv.size()-1; i++) 
			line(img, kalmanv[i], kalmanv[i+1], Scalar(0,155,255), 1);

		
		int code = (char)waitKey(10);

		if( code > 0 )
			break;
	}

	return 0;
}
