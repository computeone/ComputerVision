#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <iostream>


#define pi 3.1415926
#define e  2.718281828459
using namespace std;
using namespace cv;


//g(x,y)=(1/2*pi*u^2)*e^-(x^2+y^2)/2u^2
double NormalCurve(double x, double y, double u);
Mat Gussian(Mat image, int radius, double u);