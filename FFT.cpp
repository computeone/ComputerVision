#include <iostream>
#include "Canny.h"
#include "FFT.h"
using namespace std;
using namespace cv;


double realCompute(Mat src, double u, double v){
	int M = src.rows;
	int N = src.cols;
	double real = 0;
	//
	for (int i = 0; i < src.rows; i++){
		uchar* p = src.ptr<uchar>(i);
		for (int j = 0; j < src.cols; j++){
			double tmp1 = -1 * u*i / M;
			double tmp2 = -1 * v*j / N;
			double tmp0 = tmp1 + tmp2;
			real=p[j*3] * cos(2 * PI*tmp0)+real;
		}
	}
	return real;
}

double imgCompute(Mat src, double u, double v){
	int M = src.rows;
	int N = src.cols;
	double img = 0;
	//
	for (int i = 0; i < src.rows; i++){
		uchar* p = src.ptr<uchar>(i);
		for (int j = 0; j < src.cols; j++){
			double tmp0 = -1*u*i / M - v*j / N;
			img= p[j*3] * sin(2 * PI*tmp0) + img;
		}
	}
	return img;
}
Mat DFT(Mat src){
	int rows = src.rows;
	int cols = src.cols;
	src = Gray(src);
	Mat dst = src.clone();
	double c = rows*cols;
	cout << src << endl;
	//
	for (int i = 0; i < rows; i++){
		uchar* p = src.ptr<uchar>(i);
		uchar* d = dst.ptr<uchar>(i);
		for (int j = 0; j < cols; j++){
			double real=realCompute(src, i, j);
			double img=imgCompute(src, i, j);
			double tmp = sqrt(pow(real/c, 2) + pow(img/c, 2));
			d[j*3] = (uchar)tmp;
		}
	}
	cout << dst << endl;
	
	//
	for (int i = 0; i < dst.rows; i++){
		uchar* p = dst.ptr<uchar>(i);
		for (int j = 0; j < dst.cols; j++){
			p[j * 3 + 1] = p[j * 3];
			p[j * 3 + 2] = p[j * 3];
		}
	}

	cout << dst << endl;

	//
	return dst;
}

Mat FFT(Mat src){

	return src;
}