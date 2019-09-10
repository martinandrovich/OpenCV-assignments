#include "kernel.h"

constexpr auto KERNEL_WIDTH 	= 3;
constexpr auto KERNEL_HEIGHT 	= 3;

namespace kernel
{
	cv::Mat img;
	void scan(cv::Point start_pos, std::function<void(cv::Point&, cv::Vec3b&)> callback);
}

void kernel::load_img(cv::Mat& img)
{
	kernel::img = img;
}

void kernel::scan(cv::Point start_pos, std::function<void(cv::Point&, cv::Vec3b&)> callback)
{
	// make sure that there is an image to scan
	assert(!kernel::img.empty());

	// variables
	cv::Point cur_pixel;
	cv::Vec3b cur_color;
	std::map<float, cv::Point> pts;
	int bound_row, bound_col, start_col, start_row;

	// absolute bounds and starting position for seeking pixel
	// if seeking pixel starts in a corner or near image edge

	// [ ][ ][ ]
	// [ ][X][ ]
	// [ ][ ][ ]

	bound_row = (start_pos.x >= (kernel::img.rows - 1)) ? kernel::img.rows : start_pos.x + 2;
	bound_col = (start_pos.y >= (kernel::img.cols - 1)) ? kernel::img.cols : start_pos.y + 2;
	start_col = start_pos.x - ((start_pos.x <= 0) ? 0 : 1);
	start_row = start_pos.y - ((start_pos.y <= 0) ? 0 : 1);

	// seek the kernel grid
	for(int row = start_row; row < bound_row; row++)
	{
		for(int col = start_col; col < bound_col; col++)
		{
			cv::Point p = {col, row};
			cv::Vec3b v = kernel::img.at<cv::Vec3b>(p);

			callback(p, v);
		}
	}	
}

cv::Point kernel::find_nearest(cv::Point start_pos, cv::Point target_pos)
{
	std::map<float, cv::Point> pts;
	float dist;

	kernel::scan(start_pos, [&](cv::Point& p, cv::Vec3b& v) {
		dist = sqrt(pow(target_pos.x - p.x, 2) + pow(target_pos.y - p.y, 2));
		pts[dist] = cv::Point(p.x, p.y);
	});

	// return the value of the first pair in map
	return (pts.begin()->second);
}

bool kernel::is_reachable(cv::Point p)
{	
	return true;
}

void kernel::move_to(cv::Point p)
{
	return;
}