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

namespace ansi
{
	constexpr auto reset	= "\033[0m";
	constexpr auto red		= "\033[31m";
	constexpr auto green	= "\033[32m";
	constexpr auto kernel	= "\033[32mkernel:\033[0m ";
}

namespace kernel
{

	void
	load_img(cv::Mat& img);

	void
	scan(cv::Point pos_start, std::function<void(cv::Point&, cv::Vec3b&)> callback);
	
	void
	scan_pixel(cv::Point pos, std::function<void(cv::Point&, cv::Vec3b&)> callback);

	bool
	check_pixel(cv::Point pos, std::function<bool(cv::Point&, cv::Vec3b&)> callback);

	float
	distance(cv::Point pos_start, cv::Point pos_target);

	void
	move(cv::Point& pos_start, cv::Point& pos_target);

	int
	get_num_steps();

	cv::Point
	find_nearest(cv::Point pos_start, cv::Point pos_target);

	bool
	is_reachable(cv::Point pos_target);

	std::optional<cv::Point>
	check_obstacle(cv::Point pos_start, cv::Point pos_target);

	cv::Point
	check_wall(cv::Point pos_start, cv::Point move_dir);
	
	std::tuple<cv::Point, cv::Point>
	check_corner(cv::Point pos_start, cv::Point pos_next);

	cv::Point
	check_wall(cv::Point pos_start, cv::Point move_dir);
}
