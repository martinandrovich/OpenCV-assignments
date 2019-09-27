#pragma once

#include <string>
#include <vector>
#include <iostream>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/opencv.hpp"
#include <opencv2/plot.hpp>

#define WITHOUT_NUMPY
#pragma GCC diagnostic ignored "-Wregister"
#include "../lib/matplotlibcpp.h"

namespace hist
{
	enum class type
	{
		hist1C,
		hist3C
	};

	void
	plot_histogram(const cv::Mat& img, hist::type hist_type, cv::ColorConversionCodes cs = cv::COLOR_BGR2RGB, bool normalize = true);

	void
	smooth_1d(const cv::Mat& img);

	void
	equalize(cv::Mat& img);

	void
	compare(const cv::Mat& img1, const cv::Mat& img2, int method = CV_COMP_CORREL);

	template <class ... Args> void
	compare(const cv::Mat& img_org, Args ... imgs)
	{
		const auto list_imgs = {imgs...};
		int counter = 1;

		std::cout << "performing multiple matches:" << std::endl;

		for (auto& img : list_imgs)
		{
			std::cout << " img " << counter++ << ": ";
			hist::compare(img_org, img);
		}
	}

	void
	back_proj(cv::Mat& img_sample, cv::Mat& img, int bin_size = 8);
}
