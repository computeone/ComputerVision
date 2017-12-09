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


	//���radius����10���˳�
	if (radius > 10){
		return image;
	}
	//����ģ���뾶
	Vector<cv::Point2f> points;
	for (int i = 0; i < radius * 2 + 1; i++){
		for (int j = 0; j < radius * 2 + 1; j++){
			points.push_back(Point2f(i - radius, j - radius));
		}
	}
	for (Point2f ff : points){
		//cout << ff << endl;
	}
	//Ȩ�ؾ������
	Mat weight(radius * 2 + 1, radius * 2 + 1, CV_64FC1, 1);
	for (int i = 0; i <radius * 2 + 1; i++){
		double* p = weight.ptr<double>(i);
		for (int j = 0; j < radius * 2 + 1; j++){
			p[j] = NormalCurve(points[i*(radius*2+1)+j].x,points[i*(radius*2+1)+j].y,u);
		}
	}

	//�����˹ģ����Ȩƽ��ֵ
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
	
	//��ͼ����д���
	Mat dst_image = image.clone();
	uchar(*p[21]);
	for (int i = radius; i < image.rows-radius; i++){

		//ÿ���漰����������
		for (int ii = 0; ii < radius * 2 + 1; ii++){
			p[ii]= image.ptr<uchar>(i-radius+ii);
		}

		uchar* pp = dst_image.ptr<uchar>(i);
		for (int j = radius; j < image.cols-radius; j++){

			//����ͨ���ֱ���
			for (int n = 0; n < 3; n++){
				double sum = 0;
				//Ӧ�ø�˹�����㷨
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