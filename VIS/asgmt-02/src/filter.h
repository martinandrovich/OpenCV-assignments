#pragma once

#include <string>
#include <vector>
#include <numeric>
#include <map>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/opencv.hpp"

#include "kernel.h"

namespace filter
{
	void
	gauss_noise(cv::Mat& img, float snr = 10.0f);

	void
	sap_noise(cv::Mat& img, float snr = 10.0f);

	void
	conv_filter(cv::Mat& img, size_t size, cv::Mat1i mask = cv::Mat::ones(3, 3, CV_32SC1));

	void
	rot_mask(cv::Mat& img);

	float
	snr(const cv::Mat& img_org, const cv::Mat& img_noisy);

	float
	psnr(const cv::Mat& img1, const cv::Mat& img2);

	void
	log_info(const cv::Mat& img);

	void
	snr_test(float noise_pct);
}