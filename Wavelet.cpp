#include "Wavelet.h"
#include <iostream>

using namespace std;
using namespace cv;

Mat waveLet(Mat src){
	
	Mat dst = src.clone();
	int m = 0;

	//进行行变换
	for (int i = 0; i < src.rows; i++){

		uchar* p = src.ptr<uchar>(i);
		uchar* d = dst.ptr<uchar>(i);
		int mid = src.cols / 2;
		for (int j = 0; j < src.cols-2; j=j+2){
			d[m * 3] = (p[j * 3] + p[(j+1) * 3 ]) / 2;
			d[m * 3 + 1] = (p[j * 3 + 1] + p[(j + 1) * 3 + 1]) / 2;
			d[m * 3 + 2] = (p[j * 3 + 2] + p[(j + 1) * 3 + 2]) / 2;

			d[(mid + m) * 3] = abs(d[m * 3] - p[j * 3]);
			d[(mid + m) * 3 + 1] = abs(d[m * 3 + 1] - p[j * 3 + 1]);
			d[(mid + m) * 3 + 2] = abs(d[m * 3 + 2] - p[j * 3 + 2]);
			m++;
		}
		m = 0;
	}


	//进行列变换
	for (int i = 0; i < src.cols; i++){
		int mid = src.rows / 2;
		for (int j = 0; j < src.rows-2; j = j + 2){
			src.ptr<uchar>(m)[i * 3] = (dst.ptr<uchar>(j)[i * 3] + 
				dst.ptr<uchar>(j+1)[i * 3]) / 2;
			src.ptr<uchar>(m)[i * 3 + 1] = (dst.ptr<uchar>(j)[i * 3 + 1] + 
				dst.ptr<uchar>(j + 1)[i * 3 + 1]) / 2;
			src.ptr<uchar>(m)[i * 3 + 2] = (dst.ptr<uchar>(j)[i * 3 + 2] +
				dst.ptr<uchar>(j + 1)[i * 3 + 2]) / 2;

			src.ptr<uchar>(mid + m)[i * 3] = abs(dst.ptr<uchar>(j)[i * 3] -
				src.ptr<uchar>(m)[i * 3]);
			src.ptr<uchar>(mid + m)[i * 3+1] = abs(dst.ptr<uchar>(j)[i * 3+1] -
				src.ptr<uchar>(m)[i * 3+1]);
			src.ptr<uchar>(mid + m)[i * 3+2] = abs(dst.ptr<uchar>(j)[i * 3+2] -
				src.ptr<uchar>(m)[i * 3+2]);
			m++;
		}
		m = 0;
	}
	return src;
}