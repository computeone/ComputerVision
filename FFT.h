#include <iostream>
#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\highgui\highgui.hpp>
using namespace std;
using namespace cv;

#define PI 3.1415926


double realCompute(Mat src,double u,double v);
double imgCompute(Mat src,double u,double v);
/*
DFTÀ„∑®
*/
Mat DFT(Mat src);

/*
FFTÀ„∑®
*/
Mat FFT(Mat src);