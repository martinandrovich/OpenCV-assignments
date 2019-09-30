#include <iostream>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/opencv.hpp"

cv::Size
scaled_size(const cv::Mat& img, uint width) { return cv::Size(width, width * (img.rows / (float)img.cols)); }

int main()
{

	// open image and zoom in 800%
	auto img = cv::imread("assets/gvd_map.jpg");	
	cv::resize(img, img, cv::Size(), 8.f, 8.f, cv::INTER_NEAREST);
	cv::imshow("map (zoomed 800%)", img);
	cv::waitKey();

	// exit
	return 0;
}