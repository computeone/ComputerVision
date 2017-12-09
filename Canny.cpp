#include <opencv2\imgproc\imgproc.hpp>
#include "Canny.h"
#include "Gussian.h"
#include <cmath>
using namespace std;
using namespace cv;

Mat Canny(Mat src){

	Mat xGrad = src.clone();
	Mat yGrad = src.clone();
	Mat Margin = src.clone();
	Mat angle = src.clone();
	//ԭʼͼ��ҶȻ�
	Mat dst = src.clone();
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

	//��˹�˲�
	src = Gussian(src, 1, 1);
	
	//���޲��ȷ���ݶȷ�ֵ�ͷ���
	for (int i = 0; i < src.rows-1; i++){
		uchar* p = src.ptr<uchar>(i);
		uchar* p1 = src.ptr<uchar>(i + 1);
		uchar* x = xGrad.ptr<uchar>(i);
		uchar* y = yGrad.ptr<uchar>(i);
		uchar* m = Margin.ptr<uchar>(i);
		uchar* a = angle.ptr<uchar>(i);
		for (int j = 0; j < src.cols-1; j++){
			x[j * 3] = (p[(j + 1) * 3] - p[j * 3] + p1[(j + 1) * 3] - p1[j * 3]) / 2;
			y[j * 3] = (p[j*3]-p1[j*3]+p[(j+1)*3]-p1[(j+1)*3]) / 2;
			m[j * 3] = sqrt(pow(x[j * 3], 2) + pow(y[j * 3], 2));
			a[j * 3] = atan(y[j * 3] / x[j * 3]);

			x[j * 3+1] = (p[j * 3] - p1[j * 3] + p[(j + 1) * 3] - p1[(j + 1) * 3]) / 2;
			m[j * 3+1] = sqrt(pow(x[j * 3], 2) + pow(y[j * 3], 2));
			a[j * 3+1] = atan(y[j * 3] / x[j * 3]);

			x[j * 3+2] = (p[(j + 1) * 3] - p[j * 3] + p1[(j + 1) * 3] - p1[j * 3]) / 2;
			y[j * 3+2] = (p[j * 3] - p1[j * 3] + p[(j + 1) * 3] - p1[(j + 1) * 3]) / 2;
			m[j * 3+2] = sqrt(pow(x[j * 3], 2) + pow(y[j * 3], 2));
			a[j * 3+2] = atan(y[j * 3] / x[j * 3]);
		}
	}

	//�Ǽ���ֵ����
	  //��β�лҶ�����Ϊ0
	uchar* p = dst.ptr<uchar>(0);
	uchar* pp = dst.ptr<uchar>(dst.row - 1);
	for (int i = 0; i < dst.cols; i++){
		p[i * 3] = 0;
		p[i * 3 + 1] = 0;
		p[i * 3 + 2] = 0;

		pp[i * 3] = 0;
		pp[i * 3 + 1] = 0;
		pp[i * 3 + 2] = 0;
	}

	  //��һ�к����һ������Ϊ0
	for (int i = 0; i < dst.rows; i++){
		dst.ptr<uchar>(i)[0] = 0;
		dst.ptr<uchar>(i)[1] = 0;
		dst.ptr<uchar>(i)[2] = 0;

		dst.ptr<uchar>(i)[dst.cols*3 - 1] = 0;
		dst.ptr<uchar>(i)[dst.cols*3 - 1] = 0;
		dst.ptr<uchar>(i)[dst.cols*3 - 1] = 0;
	}


	for (int i = 1; i < Margin.rows - 1; i++){
		
		uchar* d = dst.ptr<uchar>(i);
		uchar* p1 = Margin.ptr<uchar>(i - 1);
		uchar* p0 = Margin.ptr<uchar>(i);
		uchar* p2 = Margin.ptr<uchar>(i + 1);
		uchar* x = xGrad.ptr<uchar>(i);
		uchar* y = yGrad.ptr<uchar>(i);
		uchar* m = Margin.ptr<uchar>(i);
		uchar* a = angle.ptr<uchar>(i);
		for (int j = 1; j < Margin.cols - 1; j++){

			//����ݶȷ�ֵΪ0����Ǳ�Ե
			if (m[j * 3] == 0){
				d[j * 3] = 0;
			}
			else{

				//���������y������x������˵����������������y����
				if (abs(y[j * 3]>abs(x[j * 3]))){
					//�����ֵ����
					double weight = abs(x[j * 3]) / abs(y[j * 3]);

					//���x��y�����������ķ�����ͬ

					//���x��y�����������ķ����෴

				}

				//���������x������y������˵�������ķ���������x����
				else{

					//���x��y�����������ķ�����ͬ

					//���x��y�����������ķ����෴
				}
				
			}
			
		}
	}

	//����ͼ��߽粻�Ǳ�Ե

	

	//˫��ֵ���ͱ�Ե����
	return src;
}