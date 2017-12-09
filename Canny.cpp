#include <opencv2\imgproc\imgproc.hpp>
#include "Canny.h"
#include "Gussian.h"
#include <cmath>
using namespace std;
using namespace cv;


Mat Gray(Mat& src){
	//ԭʼͼ��ҶȻ�
	for (int i = 0; i < src.rows; i++){

		uchar* p = src.ptr<uchar>(i);
		for (int j = 0; j < src.cols; j++){
			//uchar tmp = (p[j * 3] + p[j * 3 + 1] + p[j * 3 + 2]) / 3;
			uchar tmp = 0.3*p[j * 3] + 0.59*p[j * 3 + 1] + 0.11*p[j * 3 + 2];
			p[j * 3] = tmp;
			p[j * 3 + 1] = tmp;
			p[j * 3 + 2] = tmp;
		}
	}
	return src;
}
void Gradient(Mat& src, Mat&  xGraid, Mat& yGraid, Mat& Margin, Mat& Thera){
	//���޲��ȷ���ݶȷ�ֵ�ͷ���
	for (int i = 0; i < src.rows - 1; i++){
		double* p = src.ptr<double>(i);
		double* p1 = src.ptr<double>(i + 1);
		double* x = xGraid.ptr<double>(i);
		double* y = yGraid.ptr<double>(i);
		double* m = Margin.ptr<double>(i);
		double* a = Thera.ptr<double>(i);
		for (int j = 0; j < src.cols - 1; j++){
			x[j] = (p[j + 1] - p[j] + p1[j + 1] - p1[j]) / 2;
			y[j] = (p[j ] - p1[j ] + p[j + 1] - p1[j + 1]) / 2;
			m[j] = sqrt(pow(x[j ], 2) + pow(y[j ], 2));
			if (x[j] == 0){
				//a[j] = -1;
				//cout <<"dfdf"<< x[j] << endl;
			}
			else{
				a[j] = atan(y[j ] / x[j]);
			}
			
		}
	}
}
void NonmaxSuppress(Mat& src, Mat& Margin, Mat xGraid, Mat yGraid, Mat& Thera){
	//�Ǽ���ֵ����
	//��β�лҶ�����Ϊ0
	double* p = src.ptr<double>(0);
	double* pp = src.ptr<double>(src.rows - 1);
	for (int i = 0; i < src.cols; i++){
		p[i] = 0;
		pp[i] = 0;
	}



	double g1, g2, g3, g4;
	double dTmp1=0, dTmp2=0;
	double weight;
	//��һ�к����һ������Ϊ0
	for (int i = 0; i < src.rows; i++){
		src.ptr<double>(i)[0] = 0;
		src.ptr<double>(i)[src.cols  - 1] = 0;
	}


	//���зǼ���ֵ����
	for (int i = 1; i < Margin.rows - 1; i++){

		double* d = src.ptr<double>(i);
		double* m1 = Margin.ptr<double>(i - 1);
		double* m0 = Margin.ptr<double>(i);
		double* m2 = Margin.ptr<double>(i + 1);
		double* x = xGraid.ptr<double>(i);
		double* y = yGraid.ptr<double>(i);
		double* thera = Thera.ptr<double>(i);
		for (int j = 1; j < Margin.cols - 1; j++){

			//����ݶȷ�ֵΪ0����Ǳ�Ե
			if (m0[j] == 0){
				d[j] = 0;
			}
			else{
				//������s���
				/*
				��һ�����
				g1  g2
				c
				g3  g4
				*/
				if ((thera[j] >= (0.5*pi)) && (thera[j]<(0.75*pi)) ||
					((thera[j] >= (2*pi/3)) && (thera[j]<(1.75*pi)))){
					g1 = m1[j - 1];
					g2 = m1[j];
					g3 = m2[j ];
					g4 = m2[j + 1];

					weight = abs(x[j]) / abs(y[j]);
					dTmp1 = g1*weight + g2*(1 - weight);
					dTmp2 = g4*weight + g3*(1 - weight);
				}
				/*
				�ڶ������
				g1
				g2  c	g3
				g4
				*/
				else if (((thera[j] >= (0.75*pi)) && (thera[j]<pi)) ||
					((thera[j] >= (1.75*pi)) && (thera[j]<(2*pi)))){
					g1 = m1[j - 1];
					g2 = m0[j - 1];
					g3 = m0[j + 1];
					g4 = m2[j + 1];
					weight = abs(x[j ]) / abs(y[j]);
					dTmp1 = g2*weight + g1*(1 - weight);
					dTmp2 = g4*weight + g3*(1 - weight);

				}
				/*
				���������
				g1	g2
				c
				g4	g3
				*/
				else if (((thera[j] >= (0.25*pi)) && (thera[j]<(0.5*pi))) ||
					((thera[j] >= (1.25*pi)) && (thera[j]<(1.75*pi)))){
					g1 = m1[j];
					g2 = m1[j + 1];
					g3 = m2[j];
					g4 = m2[j - 1];
					weight = abs(y[j]) / abs(x[j]);
					dTmp1 = g2*weight + g1*(1 - weight);
					dTmp2 = g3*weight + g4*(1 - weight);
				}

				/*
				���������
				g1
				g4	c	g2
				g3
				*/
				else if (((thera[j] >= 0) && (thera[j]<(0.25*pi))) ||
					((thera[j] >= pi) && (thera[j]<(1.25*pi)))){
					g1 = m1[j + 1];
					g2 = m0[j + 1];
					g3 = m2[j - 1];
					g4 = m0[j - 1];
					weight = abs(y[j]) / abs(x[j]);
					dTmp1 = g1*weight + g2*(1 - weight);
					dTmp2 = g3*weight + g4*(1 - weight);
				}

				//���� �ֲ����ֵ�жϣ���д�� �����
				if (dTmp1 < m0[j] && dTmp2 < m0[j]){
					m0[j] = 128;
				}
				else{
					m0[j] = 0;
				}
			}

		}
	}
}
Mat Canny(Mat src){

	//ԭʼͼ��ҶȻ�
	Mat dst = Gray(src);
	//��˹�˲�
	dst = Gussian(dst, 1, 1);	
	//���޲��ȷ���ݶȷ�ֵ�ͷ���
	Mat m(dst.rows , dst.cols , CV_64FC1);
	Mat xGraid(dst.rows, dst.cols, CV_64FC1);
	Mat yGraid(dst.rows, dst.cols, CV_64FC1);
	Mat Margin(dst.rows, dst.cols, CV_64FC1);
	Mat Thera(dst.rows, dst.cols, CV_64FC1);

	//����dstͼ���е�����ͨ���е�һ����m��
	for (int i = 0; i < dst.rows; i++){
		uchar* p = dst.ptr<uchar>(i);
		double* mm = m.ptr<double>(i);
		for (int j = 0; j < dst.cols; j++){
			mm[j] = p[j * 3];
		}
	}


	Gradient(m, xGraid, yGraid, Margin, Thera);
	//�Ǽ���ֵ����
	NonmaxSuppress(m, Margin, xGraid, yGraid, Thera);
	//����ͼ��߽粻�Ǳ�Ե

	//˫��ֵ���ͱ�Ե����

	//��ԭͼ��Ϊrgb��ʽ
	for (int i = 0; i < dst.rows; i++){
		uchar* p = dst.ptr<uchar>(i);
		double* mm = m.ptr<double>(i);
		for (int j = 0; j < dst.cols; j++){
			p[j * 3] = mm[j];
			p[j * 3 + 1] = mm[j];
			p[j * 3 + 2] = mm[j];
		}
	}
	return dst;
}