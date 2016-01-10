/*#include "opencv2/core/core.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "opencv2/contrib/contrib.hpp"
*/
#include "opencv2/opencv.hpp"
#include <stdio.h>

using namespace cv;
using namespace std;

int main_stero_calib(int argc, char* argv[], VideoCapture	cams[] )
{
	int numBoards	= 10;// atoi(argv[1]);
	int board_w		= 5;// atoi(argv[2]);
	int board_h		= 4;// atoi(argv[3]);

    Size board_sz = Size(board_w, board_h);
    int board_n = board_w*board_h;

    vector<vector<Point3f> > object_points;
    vector<vector<Point2f> > imagePoints1, imagePoints2;
    vector<Point2f> corners1, corners2;

    vector<Point3f> obj;
    for (int j=0; j<board_n; j++)
    {
        obj.push_back(Point3f(j/board_w, j%board_w, 0.0f));
    }

    Mat img1, img2, gray1, gray2;
    VideoCapture cap1 = cams[0];
    VideoCapture cap2 = cams[1];

    int success = 0, k = 0, empty=0;
    bool found1 = false, found2 = false;
	Size imSize = { 0,0 };

    while (success < numBoards)
    {
        cap1 >> img1;
        cap2 >> img2;
		empty = (img1.empty() || img2.empty());
		if (empty) 
			break;
		imSize = img1.size();
        //resize(img1, img1, Size(320, 280));
        //resize(img2, img2, Size(320, 280));
        cvtColor(img1, gray1, CV_BGR2GRAY);
        cvtColor(img2, gray2, CV_BGR2GRAY);

        found1 = findChessboardCorners(img1, board_sz, corners1, CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FILTER_QUADS);
        found2 = findChessboardCorners(img2, board_sz, corners2, CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_FILTER_QUADS);

        if (found1)
        {
            cornerSubPix(gray1, corners1, Size(11, 11), Size(-1, -1), TermCriteria(CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 30, 0.1));
            drawChessboardCorners(gray1, board_sz, corners1, found1);
        }

        if (found2)
        {
            cornerSubPix(gray2, corners2, Size(11, 11), Size(-1, -1), TermCriteria(CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 30, 0.1));
            drawChessboardCorners(gray2, board_sz, corners2, found2);
        }
        
        imshow("image1", gray1);
        imshow("image2", gray2);

       
		//k = waitKey(10);
		k = waitKey(1);
        if (found1 && found2)
        {
            k = waitKey(0);
        }
        if (k == 27)
        {
            break;
        }
        if (k == ' ' && found1 !=0 && found2 != 0)
        {
            imagePoints1.push_back(corners1);
            imagePoints2.push_back(corners2);
            object_points.push_back(obj);
            printf ("Corners stored\n");
            success++;

            if (success >= numBoards)
            {
                break;
            }
        }
    }

	k = waitKey(0);//wait

    destroyAllWindows();
    printf("Starting Calibration\n");
    Mat CM1 = Mat(3, 3, CV_64FC1);
    Mat CM2 = Mat(3, 3, CV_64FC1);
    Mat D1, D2;
    Mat R, T, E, F;
	
    stereoCalibrate(object_points, imagePoints1, imagePoints2, 
                    CM1, D1, CM2, D2, imSize, R, T, E, F,
                    CALIB_FIX_ASPECT_RATIO +
					CALIB_ZERO_TANGENT_DIST +
					CALIB_SAME_FOCAL_LENGTH +
					CALIB_RATIONAL_MODEL +
					CALIB_FIX_K3 + CALIB_FIX_K4 + CALIB_FIX_K5,
					TermCriteria(TermCriteria::COUNT + TermCriteria::EPS, 100, 1e-5) );

	///*cvTermCriteria*/(CV_TERMCRIT_ITER+CV_TERMCRIT_EPS, 100, 1e-5), 
	//CV_CALIB_SAME_FOCAL_LENGTH | CV_CALIB_ZERO_TANGENT_DIST);
	/*
	http://docs.opencv.org/3.0-beta/modules/calib3d/doc/camera_calibration_and_3d_reconstruction.html#stereocalibrate

	C++: double stereoCalibrate(InputArrayOfArrays objectPoints, InputArrayOfArrays imagePoints1, InputArrayOfArrays imagePoints2, 
	InputOutputArray cameraMatrix1, InputOutputArray distCoeffs1, InputOutputArray cameraMatrix2, InputOutputArray distCoeffs2, 
	Size imageSize, OutputArray R, OutputArray T, OutputArray E, OutputArray F, 
	int flags=CALIB_FIX_INTRINSIC , 
	TermCriteria criteria=TermCriteria(TermCriteria::COUNT+TermCriteria::EPS, 
	30, 1e-6))
	*/
    FileStorage fs1("mystereocalib.yml", FileStorage::WRITE);
    fs1 << "CM1" << CM1;
    fs1 << "CM2" << CM2;
    fs1 << "D1" << D1;
    fs1 << "D2" << D2;
    fs1 << "R" << R;
    fs1 << "T" << T;
    fs1 << "E" << E;
    fs1 << "F" << F;

    printf("Done Calibration\n");

    printf("Starting Rectification\n");

    Mat R1, R2, P1, P2, Q;
    stereoRectify(CM1, D1, CM2, D2, imSize/* img1.size()*/, R, T, R1, R2, P1, P2, Q);
    fs1 << "R1" << R1;
    fs1 << "R2" << R2;
    fs1 << "P1" << P1;
    fs1 << "P2" << P2;
    fs1 << "Q" << Q;

    printf("Done Rectification\n");

    printf("Applying Undistort\n");

    Mat map1x, map1y, map2x, map2y;
    Mat imgU1, imgU2;

    initUndistortRectifyMap(CM1, D1, R1, P1, imSize/* img1.size()*/, CV_32FC1, map1x, map1y);
    initUndistortRectifyMap(CM2, D2, R2, P2, imSize/*img2.size()*/, CV_32FC1, map2x, map2y);

    printf("Undistort complete\n");
	fs1.release();
	/*
    while(1)
    {    
        cap1 >> img1;
        cap2 >> img2;

        remap(img1, imgU1, map1x, map1y, INTER_LINEAR, BORDER_CONSTANT, Scalar());
        remap(img2, imgU2, map2x, map2y, INTER_LINEAR, BORDER_CONSTANT, Scalar());

        imshow("image1", imgU1);
        imshow("image2", imgU2);

        k = waitKey(5);

        if(k==27)
        {
            break;
        }
    }
	*/
    cap1.release();
    cap2.release();

    return(0);
}
