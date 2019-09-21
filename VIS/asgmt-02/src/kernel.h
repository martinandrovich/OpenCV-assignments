#pragma once

#include <iostream>
#include <functional>
#include <map>
#include <array>
#include <algorithm>
#include <tuple>
#include <optional>

#include <opencv2/core.hpp>
#include <opencv2/highgui/highgui.hpp>

namespace kernel
{

	void
	scan(cv::Mat& img, size_t size, cv::Point pos_start, std::function<void(cv::Point&, cv::Vec3b&)> callback);

	void
	scanimg(cv::Mat& img, size_t size, std::function<void(cv::Point&, cv::Vec3b&)> callback);

	void
	test();

}