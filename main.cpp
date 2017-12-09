#include <cv.h>
#include <cxcore.h>
#include <highgui.h>
#include <stdlib.h>
#include <conio.h>
#include <iostream>

using namespace std;
using namespace cv;


int main(int argc, char** argv)
{
	Mat src, src_gray, detected_edges, distances, integral_im;

	int low_threshold = 50;
	char* window_src_name = "Source Image";
	char* window_src_gray_name = "Source Gray Image";
	char* window_result_name = "Result Image";

	src = imread("4.png");

	if (!src.data)
	{
		return -1;
	}

	cvtColor(src, src_gray, CV_BGR2GRAY);

	blur(src_gray, detected_edges, Size(3, 3));
	Canny(detected_edges, detected_edges, low_threshold, low_threshold * 3, 3, true);

	detected_edges = Scalar::all(255) - detected_edges;

	distanceTransform(detected_edges, distances, CV_DIST_L2, 3);

	Mat result(src_gray.rows, src_gray.cols, src_gray.type());
	integral(src_gray, integral_im);
	for (int i = 1; i < integral_im.rows; i++)
		for (int j = 1; j < integral_im.cols; j++)
		{
			int size = 1 * (int)distances.at<float>(i - 1, j - 1);
			if (size % 2 == 0) size--;

			int i_top = max(i - size / 2, 1);
			int i_bot = min(i + size / 2, integral_im.rows - 1);
			int j_left = max(j - size / 2, 1);
			int j_right = min(j + size / 2, integral_im.cols - 1);

			int count = (i_bot - i_top + 1)*(j_right - j_left + 1);
			double sum = integral_im.at<int>(i_bot, j_right)
				+ integral_im.at<int>(i_top - 1, j_left - 1)
				- integral_im.at<int>(i_top - 1, j_right)
				- integral_im.at<int>(i_bot, j_left - 1);

			result.at<uchar>(i - 1, j - 1) = sum / count;
		}

	namedWindow(window_src_name, CV_WINDOW_AUTOSIZE);
	namedWindow(window_src_gray_name, CV_WINDOW_AUTOSIZE);
	namedWindow(window_result_name, CV_WINDOW_AUTOSIZE);

	imshow(window_src_name, src);
	imshow(window_src_gray_name, src_gray);
	imshow(window_result_name, result);

	waitKey(0);
	return 0;
}