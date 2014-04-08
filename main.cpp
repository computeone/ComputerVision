#include "Gussian.h"

using namespace std;
using namespace cv;
int main(){
	double d=NormalCurve(0,-1,1.5);
	cout << d << endl;
	Mat image;
	Gussian(image, 2, 1.5);
}