/*#include "opencv2/core/core.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "opencv2/contrib/contrib.hpp"
*/
#include "opencv2/opencv.hpp"
#include <stdio.h>

#include "..\working_consts.h"

using namespace cv;
using namespace std;

/* 
	recieve 2 cameras as VideoCapture array. 
	constant definitions are correct for my chessboard.
*/
int stero_calib( VideoCapture	cams[] )
{
	// correct for my existing chessboard.
	int numBoards	= 10;
	int board_w		= 5;
	int board_h		= 4;

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
		if (k == ' ' && found1 != 0 && found2 != 0)
		{
			imagePoints1.push_back(corners1);
			imagePoints2.push_back(corners2);
			object_points.push_back(obj);
			printf("Corners stored\n");
			success++;

			if (success >= numBoards)
			{
				break;
			}
		}
	}

	//k = waitKey(0);//wait

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
		TermCriteria(TermCriteria::COUNT + TermCriteria::EPS, 100, 1e-5));
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
	fs1 << "imSize" << imSize;
	fs1 << "R" << R;
	fs1 << "T" << T;
	fs1 << "E" << E;
	fs1 << "F" << F;

	printf("Done cameras Calibration\n");

	// 2nd part of calibration, calc rectification params:
	printf("Starting Rectification\n");

	Mat R1, R2, P1, P2, Q;
	stereoRectify(CM1, D1, CM2, D2, imSize/* img1.size()*/, R, T, R1, R2, P1, P2, Q);
		// Compute rectification mappings
	//stereoRectify(cameraMatrix0, distCoeffs0, cameraMatrix1, distCoeffs1, imageSize, RInitial, TInitial, R1, R2, P1, P2, Q, CALIB_ZERO_DISPARITY, 0, imageSize, &validRoi[0], &validRoi[1]);
	
	fs1 << "R1" << R1;
	fs1 << "R2" << R2;
	fs1 << "P1" << P1;
	fs1 << "P2" << P2;
	fs1 << "Q" << Q;

	printf("Done images Rectification calculation \n");

	fs1.release();		// Close file
	cap1.release();		// close cameras/videos
	cap2.release();		// -"-

	return(0);
}


void load_calibration_file_matrices(String caliFile, CalibStruct* outputMatrices_p)
// Read calibration file , and load the calibrattion matrices to memory.
{
	if (caliFile.empty()) {
		cout << "ERROR: Calibration file not found." << endl;
		return;
	}

	CalibStruct outputMatrices;
	FileStorage fs(caliFile, FileStorage::READ);

	if (!fs.isOpened())
	{
		cout << "Calibration file was not found" << endl;
		return;
	}
	else 
	{		
		fs["CM1"] >> outputMatrices.CM1;
		fs["CM2"] >> outputMatrices.CM2;
		fs["D1"]  >> outputMatrices.D1; 
		fs["D2"]  >> outputMatrices.D2;
		fs["imSize"] >> outputMatrices.imSize;
		fs["R"]   >> outputMatrices.R;
		fs["T"]   >> outputMatrices.T;
		fs["E"]   >> outputMatrices.E;
		fs["F"]   >> outputMatrices.F;
		fs["R1"]  >> outputMatrices.R1;
		fs["R2"]  >> outputMatrices.R2;
		fs["P1"]  >> outputMatrices.P1;
		fs["P2"]  >> outputMatrices.P2;
		fs["Q"]   >> outputMatrices.Q;
	}
	*outputMatrices_p = outputMatrices;
}

void do_complete_rectification(Mat img1, Mat img2 , CalibStruct* givenCalibMatrices)
{
	bool empty = (img1.empty() || img2.empty());
	if (empty)
		return;//+print

	printf("Applying Undistort\n");

	Mat map1x, map1y, map2x, map2y;  // required ONCE after loading calibration matrices.
	Mat imgU1, imgU2;

	Size imSize = givenCalibMatrices->imSize;

	// consider do it only once:
	initUndistortRectifyMap(givenCalibMatrices->CM1, givenCalibMatrices->D1, givenCalibMatrices->R1, 
		givenCalibMatrices->P1, givenCalibMatrices->imSize/* img1.size()*/, CV_16SC2/*CV_32FC1*/, map1x, map1y);
	initUndistortRectifyMap(givenCalibMatrices->CM2, givenCalibMatrices->D2, givenCalibMatrices->R2, 
		givenCalibMatrices->P2, givenCalibMatrices->imSize/* img2.size()*/, CV_16SC2/*CV_32FC1*/, map2x, map2y);
	//initUndistortRectifyMap(cameraMatrix0, distCoeffs0, R1, P1, imageSize, CV_16SC2, rmap[0][0], rmap[0][1]);
	//initUndistortRectifyMap(cameraMatrix1, distCoeffs1, R2, P2, imageSize, CV_16SC2, rmap[1][0], rmap[1][1]);


	printf("Undistort complete\n");

	//while(1)
	{
	remap(img1, imgU1, map1x, map1y, INTER_LINEAR, BORDER_CONSTANT, Scalar());
	remap(img2, imgU2, map2x, map2y, INTER_LINEAR, BORDER_CONSTANT, Scalar());

	/**/
	imshow("image1", imgU1);
	imshow("image2", imgU2);

	/*k = waitKey(5);

	if(k==27)
	{
	break;
	}*/
	}
	
}