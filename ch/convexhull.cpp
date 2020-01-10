#include <vector>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

static int iscounterclockwise(Point p0, Point p1, Point p2);
Mat g_image;

void convex_hull(vector<Point>&points, vector<Point>&hull)
{
	int size = points.size();
	Point p0 = points[0];
	int idx = 0;

	/* 找出 Y 轴值最小的点 */
	for (int i = 1; i < size; i++) {
		if (points[i].y < p0.y) {
			p0 = points[i];
			idx = i;
		}
	}

	cout << "Y 轴最小点：" << p0 << endl;

	/* 剩余的点以p0为中心，按与p0的极角逆时针排序 */
	if (idx != 0) {
		points[idx] = points[0];
		points[0] = p0;
	}

	vector<float>coses;
	coses.push_back(1.0);
	for (int i = 1; i < size; i++) {
		int dx = points[i].x - p0.x;
		int dy = points[i].y - p0.y;
		float coss = dx/sqrt(dx * dx + dy * dy);
		coses.push_back(coss);
	}

	cout <<"排序前的点: "<< points << endl;

	// bubble sorting
    for(int i = 1; i < size; i++) {
        for(int j = 1; j < size - i; j++) {
            if (coses[j] < coses[j + 1]) {
                float temp_cos = coses[j];
                coses[j] = coses[j + 1];
                coses[j + 1] = temp_cos;

                Point temp_point = points[j];
                points[j] = points[j + 1];
                points[j + 1] = temp_point;
            }
        }
    }

	cout <<"排序后的点: "<< points << endl;

#if 0
	for (int i = 0; i < size; i++) {
		circle(g_image, points[i], (i+1)*2,
			Scalar(255, 255, 255),
			FILLED, LINE_AA);
		line(g_image, points[0], points[i],
			Scalar(255, 0, 0), 2, LINE_AA);
		imshow("Debug", g_image);
	}
#endif

	/* 将前两个点P0,P1入栈，然后从p2开始,计算栈顶两个点与该点
	 * 三点向量是否是逆时针转动，若是，则将该点压入栈中;
	 * 否则将栈顶元素推出。
	 */
	hull.push_back(points[0]);
	hull.push_back(points[1]);
	for (int i = 2; i < size; ) {
		if (iscounterclockwise(
				hull[hull.size()-2],
				hull[hull.size()-1],
				points[i]) > 0) {
			hull.push_back(points[i]);
			i++;
		} else {
			cout << "顺时针或共线:" << hull[hull.size()-1] << endl;
			hull.pop_back();
		}
	}
}

/* 向量的叉乘>0为逆时针，<0为顺时针，=0同线 */
static int iscounterclockwise(Point p0, Point p1, Point p2)
{
	return ((p1.x - p0.x) * (p2.y - p0.y)
		- (p1.y - p0.y) * (p2.x - p0.x));
}

int main(int argc, char **argv)
{
	int rows = 300;
	int cols = 300;
	Mat src(rows, cols, CV_8UC3);
	vector<Point> points;
	vector<Point> hull;	

	g_image = src.clone();

	for (int i = 0; i < atoi(argv[1]); i++) {
		Point pt = Point(rand()%rows, rand()%cols);
		circle(src, pt, 3,
			Scalar(rand()%255, rand()%255, rand()%255),
			FILLED, LINE_AA);
		points.push_back(pt);
	}

	cout<<"输入点坐标: " << points << endl;
	convex_hull(points, hull);
	cout << "最后得到的凸包: " << hull << endl;

#if 1
	Point pt = hull[hull.size()-1];
	for (int i = 0; i < hull.size(); i++) {
		line(src, pt, hull[i], Scalar(255, 0, 0), 2, LINE_AA);
		pt = hull[i];
	}
#endif

	imshow("XX", src);
	waitKey(0);
	return 0;
}
