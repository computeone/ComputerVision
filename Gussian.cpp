#include "Gussian.h"
#include <opencv2\core\mat.hpp>
#include <opencv2\flann\lsh_table.h>
using namespace std;
using namespace cv;


//高斯分布函数  
//g(x,y)=(1/2*pi*u^2)*e^-(x^2+y^2)/2u^2
double NormalCurve(double x, double y, double u){
	double v = powf(e, -1 * (powf(x, 2) + powf(y, 2)) / (2 * powf(u, 2)));
	double g = (1 / (2 * pi*powf(u, 2)))*v;
	return g;
}

//高斯模糊算法
Mat Gussian(Mat image, const int radius, double u){

	//计算模糊半径
	Vector<cv::Point2f> points;
	for (int i = 0; i < radius * 2 + 1; i++){
		for (int j = 0; j < radius * 2 + 1; j++){
			points.push_back(Point2f(i - radius, j - radius));
		}
	}
	for (Point2f ff : points){
		cout << ff << endl;
	}
	//权重矩阵计算
	Mat m(radius * 2 + 1, radius * 2 + 1, CV_64FC2, 1);
	for (int i = 0; i <radius * 2 + 1; i++){
		double* p = m.ptr<double>(i);
		for (int j = 0; j < radius * 2 + 1; j++){
			p[j] = NormalCurve(points[i*(radius*2+1)+j].x,points[i*(radius*2+1)+j].y,u);
		}
	}

	//计算高斯模糊加权平均值
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

	//对图像进行处理

	return m;
}