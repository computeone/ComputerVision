#include "Gussian.h"
#include "Canny.h"
using namespace std;
using namespace cv;
int main(){
	double d=NormalCurve(0,-1,1.5);
	Mat image = imread("C:\\Users\\niubaisui-pc\\Desktop\\6512695.jpg");
	//image=Gussian(image, 3, 4.5);
	image = Canny(image);
	imshow("display window", image);
	cout << sizeof(uchar) << endl;
	cout << "ά����" << image.dims << endl;
	cout << "ͨ��:" << image.channels() << endl;
	cout << "���:" << image.depth() << endl;
	cout << "����:" << image.rows << endl;
	cout << "����:" << image.cols << endl;
	cout << "���С��" << image.step[0] << endl;
	cout << "�еĴ�С��" << image.step[1] << endl;
	cout << "Ԫ�صĴ�С��" << image.step[2] << endl;
	cout << asin(0.85) << endl;
	waitKey(0);
}