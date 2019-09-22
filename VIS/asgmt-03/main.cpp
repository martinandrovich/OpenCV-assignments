#include <iostream>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

int main()
{

	// initialize rand()
	srand(time(NULL));

	// 1.1
	// load and display image
	cv::Mat img = cv::imread("../assets/ORings/ORing01.jpg");
	cv::namedWindow("original");
	cv::imshow("original", img);
	cv::waitKey();

	// exit
	return 0;
}