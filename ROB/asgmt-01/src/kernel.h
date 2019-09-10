#pragma once

#include <iostream>
#include <functional>
#include <map>
#include <array>
#include <algorithm>

#include <opencv2/core.hpp>
#include <opencv2/highgui/highgui.hpp>

namespace ansi
{
    constexpr auto reset	= "\033[0m";
    constexpr auto red		= "\033[31m";
    constexpr auto green	= "\033[32m";
	constexpr auto kernel	= "\033[32mkernel:\033[0m ";
}

namespace kernel
{
	enum movedir
	{
		STEP,
		DIAGONAL
	};

	void 		load_img(cv::Mat& img);
	cv::Point 	find_nearest(cv::Point start_pos, cv::Point target_pos);
	bool		is_reachable(cv::Point start_pos, cv::Point target_pos);
}