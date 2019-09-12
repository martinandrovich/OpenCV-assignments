#include "draw.h"

void draw_test()
{
	// load image
	cv::Mat img = cv::imread("assets/environ-01.png");

	// check if image has been loaded
	if (img.empty())
	{
		std::cerr << "Could not load the specified image.";
		return;
	}

	// draw pixel test
	draw_pixel(img, cv::Point(100, 100), cv::Vec3b(255, 0, 0));

	// draw line test
	draw_line(img, {200, 200}, {100, 100}, {0, 0, 255});

	// create window and display img
	cv::namedWindow("Image");
	cv::imshow("Image", img);
	cv::waitKey();
}

void draw_pixel(cv::Mat &img, cv::Point pos, cv::Vec3b color)
{
	img.at<cv::Vec3b>(pos) = color;
}

void draw_line(cv::Mat &img, cv::Point2i begin, cv::Point2i end, cv::Vec3b color)
{
	// variables
	int delta_x, delta_y, dir_x, dir_y;
	double slope, remainder;

	// set current pos to begin point
	// and draw initial pixel
	cv::Point2i current = begin;
	draw_pixel(img, current, color);

	// determine slope
	delta_x = end.x - begin.x;
	delta_y = end.y - begin.y;
	slope = (delta_x) ? (delta_y / delta_x) : 1;

	// determine direction of x & y
	dir_x = (delta_x ? (delta_x < 0 ? -1 : 1) : 0);
	dir_y = (delta_y ? (delta_y < 0 ? -1 : 1) : 0);

	// iterate until target reached
	while (current != end)
	{
		// move x if needed
		if (current.x != end.x)
		{
			current.x += dir_x;
			//draw_pixel(img, current, color);
		}

		// std::cout << "current: " << current << " | target: " << end << "\n"; 
		// usleep(10000);

		// move y if needed
		if (current.y != end.y)
		{
			// update remainder
			remainder = slope + remainder;

			// move y while abs(remainder) > 1
			while (abs(remainder) > 1)
			{
				current.y += dir_y;
				draw_pixel(img, current, color);
				remainder += (remainder > 0) ? -1 : 1;
			}
		}
	}
}

void draw_img(cv::Mat& img, cv::Point cur_pos)
{	
	cv::namedWindow("bug");

	cv::Vec3b 	red 	= {0, 0, 255};
	cv::Vec3b 	purple 	= {255, 0, 240};
	cv::Vec3b 	green 	= {0, 255, 0};
	static auto color 	= red;
	
	kernel::scan_pixel(cur_pos, [&](auto& p, auto& v){
		color = (v == red) ? green : red;
	});
	
	draw_pixel(img, cur_pos, color);

	cv::imshow("bug", img);
	cv::waitKey(1);
}