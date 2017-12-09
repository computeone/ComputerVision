#include "Gussian.h"
#include <opencv2\core\mat.hpp>
#include <opencv2\flann\lsh_table.h>
using namespace std;
using namespace cv;


//��˹�ֲ�����  
//g(x,y)=(1/2*pi*u^2)*e^-(x^2+y^2)/2u^2
double NormalCurve(double x, double y, double u){
	double v = powf(e, -1 * (powf(x, 2) + powf(y, 2)) / (2 * powf(u, 2)));
	double g = (1 / (2 * pi*powf(u, 2)))*v;
	return g;
}

//��˹ģ���㷨
Mat Gussian(Mat image, const int radius, double u){

	//����ģ���뾶
	Vector<cv::Point2f> points;
	for (int i = 0; i < radius * 2 + 1; i++){
		for (int j = 0; j < radius * 2 + 1; j++){
			points.push_back(Point2f(i - radius, j - radius));
		}
	}
	for (Point2f ff : points){
		cout << ff << endl;
	}
	//Ȩ�ؾ������
	Mat m(radius * 2 + 1, radius * 2 + 1, CV_64FC2, 1);
	for (int i = 0; i <radius * 2 + 1; i++){
		double* p = m.ptr<double>(i);
		for (int j = 0; j < radius * 2 + 1; j++){
			p[j] = NormalCurve(points[i*(radius*2+1)+j].x,points[i*(radius*2+1)+j].y,u);
		}
	}

	//�����˹ģ����Ȩƽ��ֵ
	double sum=0;
	for (int i = 0; i < radius * 2 + 1; i++){
		double* p = m.ptr<double>(i);
		for (int j = 0; j < radius * 2 + 1; j++){
			sum = sum + p[j];
		}
	}

	for (int i = 0; i < radius * 2 + 1; i++){
		double* p = m.ptr<double>(i);
		for (int j = 0; j < radius * 2 + 1; j++){
			p[j] = p[j] / sum;
		}
	}
	
	cout << m << endl;

	//��ͼ����д���

	return m;
}