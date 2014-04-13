#include <opencv2\imgproc\imgproc.hpp>
#include "Canny.h"
#include "Gussian.h"
#include <cmath>
using namespace std;
using namespace cv;


Mat Gray(Mat& src){
	//原始图像灰度化
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
	//有限差分确定梯度幅值和方向
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
				a[j] = atan(y[j ] / x[j])*57.3;
				if (a[j] < 0){
					a[j] = a[j] + 360;
				}
			}
			
		}
	}
}
void NonmaxSuppress(Mat& src, Mat& Margin, Mat xGraid, Mat yGraid, Mat& Thera){
	//非极大值抑制
	//首尾行灰度设置为0
	double* p = src.ptr<double>(0);
	double* pp = src.ptr<double>(src.rows - 1);
	for (int i = 0; i < src.cols; i++){
		p[i] = 0;
		pp[i] = 0;
	}



	double g1, g2, g3, g4;
	double dTmp1=0, dTmp2=0;
	double weight;
	//第一列和最后一列设置为0
	for (int i = 0; i < src.rows; i++){
		src.ptr<double>(i)[0] = 0;
		src.ptr<double>(i)[src.cols  - 1] = 0;
	}


	//进行非极大值抑制
	for (int i = 1; i < Margin.rows - 1; i++){

		double* d = src.ptr<double>(i);
		double* m1 = Margin.ptr<double>(i - 1);
		double* m0 = Margin.ptr<double>(i);
		double* m2 = Margin.ptr<double>(i + 1);
		double* x = xGraid.ptr<double>(i);
		double* y = yGraid.ptr<double>(i);
		double* thera = Thera.ptr<double>(i);
		for (int j = 1; j < Margin.cols - 1; j++){

			//如果梯度幅值为0，则非边缘
			if (m0[j] == 0){
				d[j] = 0;
			}
			else{
				//分四种s情况
				/*
				第一种情况
				g1  g2
				c
				g3  g4
				*/
				if ((thera[j] >= 90) && (thera[j]<135) ||
					((thera[j] >= 270) && (thera[j]<315))){
					g1 = m1[j - 1];
					g2 = m1[j];
					g3 = m2[j ];
					g4 = m2[j + 1];

					weight = abs(x[j]) / abs(y[j]);
					dTmp1 = g1*weight + g2*(1 - weight);
					dTmp2 = g4*weight + g3*(1 - weight);
				}
				/*
				第二种情况
				g1
				g2  c	g3
				g4
				*/
				else if (((thera[j] >= 135) && (thera[j]<180)) ||
					((thera[j] >= 315) && (thera[j]<360))){
					g1 = m1[j - 1];
					g2 = m0[j - 1];
					g3 = m0[j + 1];
					g4 = m2[j + 1];
					weight = abs(x[j ]) / abs(y[j]);
					dTmp1 = g2*weight + g1*(1 - weight);
					dTmp2 = g4*weight + g3*(1 - weight);

				}
				/*
				第三种情况
				g1	g2
				c
				g4	g3
				*/
				else if (((thera[j] >= 45) && (thera[j]<90)) ||
					((thera[j] >= 225) && (thera[j]<270))){
					g1 = m1[j];
					g2 = m1[j + 1];
					g3 = m2[j];
					g4 = m2[j - 1];
					weight = abs(y[j]) / abs(x[j]);
					dTmp1 = g2*weight + g1*(1 - weight);
					dTmp2 = g3*weight + g4*(1 - weight);
				}

				/*
				第四种情况
				g1
				g4	c	g2
				g3
				*/
				else if (((thera[j] >= 0) && (thera[j]<45)) ||
					((thera[j] >= 180) && (thera[j]<225))){
					g1 = m1[j + 1];
					g2 = m0[j + 1];
					g3 = m2[j - 1];
					g4 = m0[j - 1];
					weight = abs(y[j]) / abs(x[j]);
					dTmp1 = g1*weight + g2*(1 - weight);
					dTmp2 = g3*weight + g4*(1 - weight);
				}

				//进行 局部最大值判断，并写入 检测结果
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
void DualThresholdDetect(Mat& src,Mat& Margin){

	int nHist[1024];   //灰度直方图
	int nEdgeNum;      //可能边界数
	int nMaxMag = 0;	//最大梯度数
	int nHighCount;

	//构造灰度图的统计直方图
	for (int i = 0; i < 1024; i++){
		nHist[i] = 0;
	}

	for (int i = 0; i < src.rows; i++){
		double* p = src.ptr<double>(i);
		for (int j = 0; j < src.cols; j++){
			if (p[j] == 128){
				nHist[(int)p[j]]++;
			}
		}
	}


	//获取最大梯度幅值及潜在的边缘点个数
	nEdgeNum = nHist[0];
	nMaxMag = 0;
	for (int i = 0; i < 1024; i++){
		if (nHist[i] != 0){
			nMaxMag = i;
		}
		nEdgeNum += nHist[i];
	}

	cout << "MaxMag:" << nMaxMag << endl;
	cout << "nEdgeNum:" << nEdgeNum << endl;

	//计算双阀值
	double RatHigh = 0.79;
	double ThrHigh;
	double ThrLow;
	double RatLow = 0.5;
	nHighCount = (int)(RatHigh*nEdgeNum + 0.5);
	int k = 1;
	nEdgeNum = nHist[1];
	while ((k < nMaxMag - 1) && (nEdgeNum < nHighCount)){
		k++;
		nEdgeNum += nHist[k];
	}

	ThrHigh = k;
	ThrLow = (int)((ThrHigh)*RatLow + 0.5);


	//进行边缘检测
	for (int i = 0; i < src.rows; i++){
		double* p = src.ptr<double>(i);
		double* m = Margin.ptr<double>(i);
		for (int j = 0; j < src.cols; j++){
			if ((p[j]== 128) && (m[j]>ThrHigh)){
				p[j] = 255;
				TraceEdge(j, i, ThrLow, src, Margin);
			}
		}
	}

	//删除非边界点
	for (int i = 0; i < src.rows; i++){
		double* p = src.ptr<double>(i);
		for (int j = 0; j < src.cols; j++){
			if (p[j] != 255){
				//p[j] = 0;
			}
		}
	}
}
void TraceEdge(int y, int x, int nThrLow, Mat& src,Mat& Margin){
	
	//对8邻域像素进行查询
	int xNum[8] = { 1, 1, 0, -1, -1, 0, 1};
	int yNum[8] = { 0, 1, 1, 1, 0, -1, -1, -1 };

	long yy, xx, k;
	for (k = 0; k < 8; k++){

		yy = y + yNum[k];
		xx = x + xNum[k];
		double* p = src.ptr<double>(x);
		double* m = Margin.ptr<double>(x);
		if ((p[y]==128)&&(m[y]>nThrLow)){

			//以该点为边界点
			p[y] = 255;
			//以该点为中心访问继续访问8邻域
			TraceEdge(yy, xx, nThrLow, src, Margin);
		}
	}
}
Mat Canny(Mat src){

	//原始图像灰度化
	Mat dst = Gray(src);
	//高斯滤波
	dst = Gussian(dst, 1, 1);	
	//有限差分确定梯度幅值和方向
	Mat gray(dst.rows , dst.cols , CV_64FC1);
	Mat xGraid(dst.rows, dst.cols, CV_64FC1);
	Mat yGraid(dst.rows, dst.cols, CV_64FC1);
	Mat Margin(dst.rows, dst.cols, CV_64FC1);
	Mat Thera(dst.rows, dst.cols, CV_64FC1);

	//复制dst图像中的三个通道中的一个到m中
	for (int i = 0; i < dst.rows; i++){
		uchar* p = dst.ptr<uchar>(i);
		double* mm = gray.ptr<double>(i);
		for (int j = 0; j < dst.cols; j++){
			mm[j] = p[j * 3];
		}
	}


	Gradient(gray, xGraid, yGraid, Margin, Thera);
	//非极大值抑制
	NonmaxSuppress(gray, Margin, xGraid, yGraid, Thera);

	//双阀值检测和边缘连接
	DualThresholdDetect(gray,Margin);

	//复原图像为rgb格式
	for (int i = 0; i < dst.rows; i++){
		uchar* p = dst.ptr<uchar>(i);
		double* mm = gray.ptr<double>(i);
		for (int j = 0; j < dst.cols; j++){
			p[j * 3] = mm[j];
			p[j * 3 + 1] = mm[j];
			p[j * 3 + 2] = mm[j];
		}
	}
	return dst;
}