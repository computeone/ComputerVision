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


	//如果radius大于10，退出
	if (radius > 10){
		return image;
	}
	//计算模糊半径
	Vector<cv::Point2f> points;
	for (int i = 0; i < radius * 2 + 1; i++){
		for (int j = 0; j < radius * 2 + 1; j++){
			points.push_back(Point2f(i - radius, j - radius));
		}
	}
	for (Point2f ff : points){
		//cout << ff << endl;
	}
	//权重矩阵计算
	Mat weight(radius * 2 + 1, radius * 2 + 1, CV_64FC1, 1);
	for (int i = 0; i <radius * 2 + 1; i++){
		double* p = weight.ptr<double>(i);
		for (int j = 0; j < radius * 2 + 1; j++){
			p[j] = NormalCurve(points[i*(radius*2+1)+j].x,points[i*(radius*2+1)+j].y,u);
		}
	}

	//计算高斯模糊加权平均值
	double avg_sum=0;
	for (int i = 0; i < radius * 2 + 1; i++){
		double* p = weight.ptr<double>(i);
		for (int j = 0; j < radius * 2 + 1; j++){
			avg_sum = avg_sum + p[j];
		}
	}

	for (int i = 0; i < radius * 2 + 1; i++){
		double* p = weight.ptr<double>(i);
		for (int j = 0; j < radius * 2 + 1; j++){
			p[j] = p[j] / avg_sum;
		}
	}
	
	//cout << weight << endl;
	
	//对图像进行处理
	Mat dst_image = image.clone();
	uchar(*p[21]);
	for (int i = radius; i < image.rows-radius; i++){

		//每次涉及到数据行数
		for (int ii = 0; ii < radius * 2 + 1; ii++){
			p[ii]= image.ptr<uchar>(i-radius+ii);
		}

		uchar* pp = dst_image.ptr<uchar>(i);
		for (int j = radius; j < image.cols-radius; j++){

			//三个通道分别处理
			for (int n = 0; n < 3; n++){
				double sum = 0;
				//应用高斯过滤算法
				for (int m = 0; m < radius * 2 + 1; m++){

					for (int q = 0; q < radius * 2 + 1; q++){
						sum = (int)p[q][(j-radius+m)*3+n] * weight.ptr<double>(q)[m]+sum;
					}
				}
				pp[j * 3 + n] = sum;
			}
			
		}
	}

	return dst_image;
}