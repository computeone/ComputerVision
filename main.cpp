#include "Gussian.h"

using namespace std;
using namespace cv;
int main(){
	double d=NormalCurve(0,-1,1.5);
	Mat image = imread("C:\\Users\\niubaisui-pc\\Pictures\\6512695.jpg");
	image=Gussian(image, 5, 1.5);
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