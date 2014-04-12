#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\core\core.hpp>
#include <iostream>

using namespace std;
using namespace cv;


Mat Gray(Mat& src);
//灰度图求梯度
void Gradient(Mat& src,Mat&  xGraid,Mat& yGraid,Mat& Margin,Mat& Thera);
//非极大值抑制
void NonmaxSuppress(Mat& src,Mat& Margin, Mat xGraid, Mat yGraid, Mat& Thera);
Mat Canny(Mat src);