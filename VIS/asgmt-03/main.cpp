#include <iostream>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "src/hist.h"

cv::Size
scaled_size(const cv::Mat& img, uint width) { return cv::Size(width, width * (img.rows / (float)img.cols)); }

int main()
{

	// initialize rand()
	srand(time(NULL));

	// // 1.1
	// // grayscale (1 channel) histogram
	// cv::Mat img1_gray = cv::imread("../assets/LicencePlates/LicencePlates06.jpg", cv::IMREAD_GRAYSCALE);
	// cv::resize(img1_gray, img1_gray, scaled_size(img1_gray, 800));
	// cv::imshow("org gray", img1_gray);
	// cv::waitKey();
	// hist::plot_histogram(img1_gray, hist::type::hist1C);

	// // perform graysale equalization
	// cv::Mat img1_gray_eq = img1_gray.clone();
	
	// hist::equalize(img1_gray_eq);
	// cv::imshow("org gray eq", img1_gray_eq);
	// cv::waitKey();
	// hist::plot_histogram(img1_gray_eq, hist::type::hist1C);

	// // color (3 channel) histogram
	// cv::Mat img1_colr = cv::imread("../assets/LicencePlates/LicencePlates06.jpg");
	// cv::resize(img1_colr, img1_colr, scaled_size(img1_colr, 800));
	// cv::imshow("org color", img1_colr);
	// cv::waitKey();
	// hist::plot_histogram(img1_colr, hist::type::hist3C, cv::COLOR_BGR2HLS, false);

	// // perform graysale equalization
	// cv::Mat img1_colr_eq = img1_colr.clone();
	
	// hist::equalize(img1_colr_eq);
	// cv::imshow("org color eq", img1_colr_eq);
	// cv::waitKey();
	// hist::plot_histogram(img1_colr_eq, hist::type::hist3C, cv::COLOR_BGR2HLS, false);

	// 1.2
	// histogram comparison
	// cv::Mat img_astr1 = cv::imread("../assets/Astronaut1.jpg");
	// cv::Mat img_astr2 = cv::imread("../assets/Astronaut2.jpg");
	// cv::Mat img_oring1 = cv::imread("../assets/ORings/ORing01.jpg");

	// hist::compare(img_astr1, img_astr2, img_oring1);

	// 1.3
	// back projection
	// cv::Mat img_spoon_sample = cv::imread("../assets/BabyFood/BabyFood-Sample-Hist1.JPG");
	// cv::Mat img_spoon1 = cv::imread("../assets/BabyFood/BabyFood-Sample1.JPG");

	// hist::back_proj(img_spoon_sample, img_spoon1, 4);

	// 1.4
	// k-means clutersing
	cv::Mat img_glue = cv::imread("../assets/Astronaut1.jpg");
	//cv::Mat img_glue = cv::imread("../assets/Astronaut1.jpg");
	cv::imshow("img glue", img_glue);
	cv::waitKey();
	hist::k_means_clust<cv::Vec3b>(img_glue, 8, 30);

	// exit
	return 0;
}