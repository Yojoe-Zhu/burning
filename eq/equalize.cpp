#include <opencv2/opencv.hpp>

using namespace cv;

void equalize(Mat &src, Mat &dst)
{
	int gray[0xff+1] = {0};
	float prob[0xff+1];

	dst.create(src.size(), src.type());

	for (int i = 0; i < src.rows; i++) {
		uchar * row = src.ptr<uchar>(i);
		for (int j = 0; j < src.cols; j++) {
			gray[row[j]]++; 
		}
	}

	int total = src.rows * src.cols;
	prob[0] = (float)gray[0]/total;
	for (int i = 1; i < 0xff + 1; i++) {
		prob[i] = (float)gray[i]/total + prob[i-1];
	//	printf("[%d]%f\n", i, prob[i]);
	}

	for (int i = 0; i < src.rows; i++) {
		uchar * row = src.ptr<uchar>(i);
		uchar * drow = dst.ptr<uchar>(i);
		for (int j = 0; j < src.cols; j++) {
			drow[j] = saturate_cast<uchar>(prob[row[j]] * 0xff);
		}
	}
}

int main(int argc, char **argv)
{
	Mat src = imread(argv[1], 0);
	Mat dst;

	imshow("Origin", src);
	equalize(src, dst);
	imshow("Equalizes", dst);
	waitKey(0);
	return 0;
}
