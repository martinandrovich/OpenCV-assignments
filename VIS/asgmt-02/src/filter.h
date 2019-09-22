#pragma once

#include <string>
#include <vector>
#include <numeric>
#include <map>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/opencv.hpp"

namespace filter
{
	void
	playground();

	void
	log_info(const cv::Mat& img_org, const cv::Mat& img_noisy, const cv::Mat& img_filt);

	void
	border(cv::Mat& img);
	
	void
	gauss_noise(cv::Mat& img, float snr = 10.0f);

	void
	gauss_noise(cv::Mat& img, size_t size, float sigma);

	void
	sap_noise(cv::Mat& img, float snr = 10.0f);

	void
	convolve(cv::Mat& img, cv::Mat1i mask, float weight = 1);

	void
	rot_mask(cv::Mat& img);
	
	void
	rot_mask2(cv::Mat& img);

	void
	box_filter(cv::Mat& img, size_t size = 3);

	void
	median_filter(cv::Mat& img, size_t size = 3);

	void
	mediangray_filter(cv::Mat& img, size_t size = 3);

	void
	gauss_filter(cv::Mat& img, float sigma = 10.f);

	float
	snr(const cv::Mat& img_org, const cv::Mat& img_noisy);

	float
	psnr(const cv::Mat& img1, const cv::Mat& img2);

	void
	snr_test(float noise_pct);
}