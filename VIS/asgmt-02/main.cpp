#include <iostream>
#include <string>
#include <algorithm>

#include "src/filter.h"
#include "src/kernel.h"

int main()
{


	// kernel test
	//kernel::test();

	// initialize rand()
	srand(time(NULL));

	// SNR test
	//kernel::test();
	//filter::snr_test(50.f);

	// 1.1
	// load and display Orings/Oring01.jpg
	//cv::Mat img = cv::imread("../assets/Astronaut1.jpg");
	cv::Mat img = cv::imread("../assets/ORings/ORing01.jpg");
	cv::namedWindow("original");
	cv::imshow("original", img);
	cv::waitKey();

	// generate Gaussian noise with SNR = 10
	auto img_gauss = img.clone();
	filter::gauss_noise(img_gauss, 10);
	cv::namedWindow("gauss noise");
	cv::imshow("gauss noise", img_gauss);
	cv::waitKey();

	// 1.2
	// ...
	auto img_gauss_1 = img_gauss.clone();
	filter::conv_filter(img_gauss_1, 3);
	cv::namedWindow("gauss locavg filt");
	cv::imshow("gauss locavg filt", img_gauss_1);
	cv::waitKey();

	auto img_gauss_2 = img_gauss.clone();
	filter::conv_filter(img_gauss_2, 3);
	cv::namedWindow("gauss conv filt");
	cv::imshow("gauss conv filt", img_gauss_2);
	cv::waitKey();

	auto img_gauss_3 = img_gauss.clone();
	filter::rot_mask(img_gauss_3);
	cv::namedWindow("gauss rotmask filt");
	cv::imshow("gauss rotmask filt", img_gauss_3);
	cv::waitKey();


	// 1.3
	// ...
	auto img_sap = img.clone();
	filter::sap_noise(img_sap, 10.f);
	cv::namedWindow("sap noise");
	cv::imshow("sap noise", img_sap);
	cv::waitKey();

	// 1.4
	// ..
	auto img_sap_1 = img_sap.clone();
	filter::rot_mask(img_sap_1);
	cv::namedWindow("sap rotmask filt");
	cv::imshow("sap rotmask filt", img_sap_1);
	cv::waitKey();

	// exit
	return 0;
}