#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\core\core.hpp>
#include <iostream>

using namespace std;
using namespace cv;


Mat Gray(Mat& src);
//�Ҷ�ͼ���ݶ�
void Gradient(Mat& src,Mat&  xGraid,Mat& yGraid,Mat& Margin,Mat& Thera);
//�Ǽ���ֵ����
void NonmaxSuppress(Mat& src,Mat& Margin, Mat xGraid, Mat yGraid, Mat& Thera);
Mat Canny(Mat src);