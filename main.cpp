#include "Gussian.h"

using namespace std;
using namespace cv;
int main(){
	double d=NormalCurve(0,-1,1.5);
	cout << d << endl;
	CvPoint3D64f point;
	//Mat image(5, 5, CV_8UC3, 1);
	//randu(image,Scalar::all(0), Scalar::all(255));
	//cout << image << endl;
	Mat image = imread("C:\\Users\\niubaisui-pc\\Pictures\\6512695.jpg");
	image=Gussian(image, 3, 1.5);
	imshow("display window", image);
	cout << sizeof(uchar) << endl;
	cout << "维数：" << image.dims << endl;
	cout << "通道:" << image.channels() << endl;
	cout << "深度:" << image.depth() << endl;
	cout << "行数:" << image.rows << endl;
	cout << "列数:" << image.cols << endl;
	cout << "面大小：" << image.step[0] << endl;
	cout << "行的大小：" << image.step[1] << endl;
	cout << "元素的大小：" << image.step[2] << endl;
	waitKey(0);
}