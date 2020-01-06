#include <opencv2/opencv.hpp>

using namespace cv;

#define DOSQRT 0

static void roberts(Mat &src, Mat &dst)
{
	int rows = src.rows;
	int cols = src.cols;

	dst = src.clone();

	for (int i = 0; i < rows - 1; i++) {
		uchar *up = src.ptr<uchar>(i);
		uchar *down = src.ptr<uchar>(i+1);
		uchar *p = dst.ptr<uchar>(i);
		for (int j = 0; j < cols - 1; j++) {
			int Gx = down[j+1]-up[j];
			int Gy = down[j]-up[j+1];
#if DOSQRT
			p[j] = saturate_cast<uchar>(sqrt(Gx * Gx + Gy * Gy));
#else
			p[j] = saturate_cast<uchar>(abs(Gx) + abs(Gy));
#endif
		}
	}	
}

/*
 * Gx = [1 0 -1
 *       1 0 -1
 *       1 0 -1] * A
 *
 * Gy = [1  1  1
 *       0  0  0 
 *      -1 -1 -1] * A
 */
static void prewitt(Mat &src, Mat &dst)
{
	int rows = src.rows;
	int cols = src.cols;

	dst = src.clone();

	for (int i = 1; i < rows - 1; i++) {
		uchar *up = src.ptr<uchar>(i-1);
		uchar *cur = src.ptr<uchar>(i);
		uchar *down = src.ptr<uchar>(i+1);
		uchar *p = dst.ptr<uchar>(i);
		for (int j = 1; j < cols - 1; j++) {
			int Gx = (up[j-1] + cur[j-1] + down[j-1])
				- (up[j+1] + cur[j+1] + down[j+1]);
			int Gy = (up[j-1] + up[j] + up[j+1])
				- (down[j-1] + down[j] + down[j+1]);
#if DOSQRT
			p[j] = saturate_cast<uchar>(sqrt(Gx * Gx + Gy * Gy));
#else
			p[j] = saturate_cast<uchar>(abs(Gx) + abs(Gy));
#endif
		}
	}	
}

/*
 * Gx = [1 0 -1
 *       2 0 -2
 *       1 0 -1] * A
 *
 * Gy = [1  2  1
 *       0  0  0 
 *      -1 -2 -1] * A
 */
static void sobel(Mat &src, Mat &dst)
{
	int rows = src.rows;
	int cols = src.cols;

	dst = src.clone();

	for (int i = 1; i < rows - 1; i++) {
		uchar *up = src.ptr<uchar>(i-1);
		uchar *cur = src.ptr<uchar>(i);
		uchar *down = src.ptr<uchar>(i+1);
		uchar *p = dst.ptr<uchar>(i);
		for (int j = 1; j < cols - 1; j++) {
			int Gx = (up[j-1] + 2 * cur[j-1] + down[j-1])
				- (up[j+1] + 2 * cur[j+1] + down[j+1]);
			int Gy = (up[j-1] + 2 * up[j] + up[j+1])
				- (down[j-1] + 2 * down[j] + down[j+1]);
#if DOSQRT
			p[j] = saturate_cast<uchar>(sqrt(Gx * Gx + Gy * Gy));
#else
			p[j] = saturate_cast<uchar>(abs(Gx) + abs(Gy));
#endif
		}
	}	
}

/*
 * Gx = [ -3 0 +3
 *       -10 0 +10 
 *        -3 0 +3] * A
 *
 * Gy = [-3 -10 -3
 *        0  0  0 
 *       +3 +10 +3] * A
 */
static void scharr(Mat &src, Mat &dst)
{
	int rows = src.rows;
	int cols = src.cols;

	dst = src.clone();

	for (int i = 1; i < rows - 1; i++) {
		uchar *up = src.ptr<uchar>(i-1);
		uchar *cur = src.ptr<uchar>(i);
		uchar *down = src.ptr<uchar>(i+1);
		uchar *p = dst.ptr<uchar>(i);
		for (int j = 1; j < cols - 1; j++) {
			int Gx = - (3 * up[j-1] + 10 * cur[j-1] + 3 * down[j-1])
				+ (3 * up[j+1] + 10 * cur[j+1] + 3 * down[j+1]);
			int Gy = - (3 * up[j-1] + 10 * up[j] + 3 * up[j+1])
				+ (3 * down[j-1] + 10 * down[j] + 3 * down[j+1]);
#if DOSQRT
			p[j] = saturate_cast<uchar>(sqrt(Gx * Gx + Gy * Gy));
#else
			p[j] = saturate_cast<uchar>(abs(Gx) + abs(Gy));
#endif
		}
	}	
}

/*
 * 0  1 0
 * 1 -4 1
 * 0  1 0
 */
static void laplace(Mat &src, Mat &dst)
{
	int rows = src.rows;
	int cols = src.cols;

	dst = src.clone();

	for (int i = 1; i < rows - 1; i++) {
		uchar *up = src.ptr<uchar>(i-1);
		uchar *cur = src.ptr<uchar>(i);
		uchar *down = src.ptr<uchar>(i+1);
		uchar *p = dst.ptr<uchar>(i);
		for (int j = 1; j < cols - 1; j++) {
			int G = up[j] + cur[j-1] + cur[j+1] + down[j]
					- 4 * cur[j];
			p[j] = saturate_cast<uchar>(G);
		}
	}	
}

/*
 * 1  1 1 
 * 1 -8 1
 * 1  1 1 
 */
static void laplace_ex(Mat &src, Mat &dst)
{
	int rows = src.rows;
	int cols = src.cols;

	dst = src.clone();

	for (int i = 1; i < rows - 1; i++) {
		uchar *up = src.ptr<uchar>(i-1);
		uchar *cur = src.ptr<uchar>(i);
		uchar *down = src.ptr<uchar>(i+1);
		uchar *p = dst.ptr<uchar>(i);
		for (int j = 1; j < cols - 1; j++) {
			int G = up[j-1] + up[j] + up[j+1]
				+ cur[j-1] - 8 * cur[j] + cur[j+1]
				+ down[j-1] + down[j] + down[j+1];
			p[j] = saturate_cast<uchar>(G);
		}
	}	
}

int main(int argc, char **argv)
{
	Mat img = imread(argv[1]);
	Mat dst;

	if (img.channels() > 1)
		cv::cvtColor(img, img, CV_RGB2GRAY);
	imshow(argv[1], img);

	roberts(img, dst);	
	imshow("Roberts", dst);

	prewitt(img, dst);	
	imshow("Pewitt", dst);

	sobel(img, dst);	
	imshow("Sobel", dst);

	scharr(img, dst);	
	imshow("Scharr", dst);

	laplace(img, dst);	
	imshow("Laplace", dst);

	laplace_ex(img, dst);	
	imshow("LaplaceEx", dst);

	waitKey(0);
	return 0;
}
