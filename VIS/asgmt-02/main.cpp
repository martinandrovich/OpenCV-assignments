#include <iostream>
#include <string>
#include <algorithm>

#include "src/filter.h"

int main()
{

	// opencv testing
	//filter::playground();

	// initialize rand()
	srand(time(NULL));

	// SNR test
	//filter::snr_test(50.f);

	// 1.1
	// load and display image
	//cv::Mat img = cv::imread("../assets/Astronaut1.jpg");
	cv::Mat img = cv::imread("../assets/ORings/ORing01.jpg");
	cv::namedWindow("original");
	cv::imshow("original", img);
	cv::waitKey();

	// generate gauss noise with SNR = 10
	// perform a deep copy
	auto img_gauss = img.clone();
	filter::gauss_noise(img_gauss, 10);
	cv::namedWindow("gauss noise");
	cv::imshow("gauss noise", img_gauss);
	cv::waitKey();

	// 1.2
	// implement conv filter

	// conv: box filter
	auto img_gauss_1 = img_gauss.clone();
	filter::box_filter(img_gauss_1, 3);
	filter::log_info(img, img_gauss, img_gauss_1);
	cv::namedWindow("gauss box filt");
	cv::imshow("gauss box filt", img_gauss_1);
	cv::waitKey();

	// conv:: gauss filter
	auto img_gauss_2 = img_gauss.clone();
	filter::gauss_filter(img_gauss_2);
	filter::log_info(img, img_gauss, img_gauss_2);
	cv::namedWindow("gauss gauss filt");
	cv::imshow("gauss gauss filt", img_gauss_2);
	cv::waitKey();

	// extra: rotmask filer
	auto img_gauss_3 = img_gauss.clone();
	filter::rot_mask(img_gauss_3);
	filter::log_info(img, img_gauss, img_gauss_3);
	cv::namedWindow("gauss rotmask filt");
	cv::imshow("gauss rotmask filt", img_gauss_3);
	cv::waitKey();

	// 1.3
	// implement salt and pepper noise
	auto img_sap = img.clone();
	filter::sap_noise(img_sap, 10.f);
	cv::namedWindow("sap noise");
	cv::imshow("sap noise", img_sap);
	cv::waitKey();

	// 1.4
	// implement rotmask filter
	auto img_sap_1 = img_sap.clone();
	filter::rot_mask(img_sap_1);
	filter::log_info(img, img_sap, img_sap_1);
	cv::namedWindow("sap rotmask filt");
	cv::imshow("sap rotmask filt", img_sap_1);
	cv::waitKey();

	// 1.5
	// implement median filter
	auto img_sap_2 = img_sap.clone();
	filter::median_filter(img_sap_2);
	filter::log_info(img, img_sap, img_sap_2);
	cv::namedWindow("sap median filt");
	cv::imshow("sap median filt", img_sap_2);
	cv::waitKey();

	// exit
	return 0;
}