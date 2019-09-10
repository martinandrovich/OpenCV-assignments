#pragma once

#include <functional>
#include <map>

#include <opencv2/core.hpp>
#include <opencv2/highgui/highgui.hpp>

namespace kernel
{
	enum movedir
	{
		STEP,
		DIAGONAL
	};

	void 		load_img(cv::Mat& img);
	cv::Point 	find_nearest(cv::Point start_pos, cv::Point target_pos);
	bool		is_reachable(cv::Point p);
	void		move_to(cv::Point p);
}