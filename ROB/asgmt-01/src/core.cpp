#include "core.h"

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
			draw_pixel(img, current, color);
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

void bug1(cv::Mat& img)
{
	// states
	enum state
	{
		MOVE_TOWARDS_GOAL,
		CIRCUMNAVIGATE_OBSTACLE,
		RETURN_TO_SAVED_POINT
	};

	// variables
	state bug_state = MOVE_TOWARDS_GOAL;
	cv::Point pos_cur = {25, 25};
	cv::Point pos_next;
	cv::Point pos_goal = {100, 100};

	// prepare kernel
	kernel::load_img(img);

	// state machine
	while(pos_cur != pos_goal) switch (bug_state)
	{
		case MOVE_TOWARDS_GOAL:
		{
			// determine direction of shortest distance
			pos_next = kernel::find_nearest(pos_cur, pos_goal);

			// check if desired point is reachable (no obstacles)
			// change state if not
			if(!kernel::is_reachable(pos_next))
			{
				bug_state = CIRCUMNAVIGATE_OBSTACLE;
				break;
			}

			// step
			std::cout << "kernel: moving from: " << pos_cur << " to " << pos_next << "\n";
			draw_pixel(img, pos_cur, {255, 0, 0});
			pos_cur = pos_next;

			// delay
			usleep(100000);
			
			break;
		}

		case CIRCUMNAVIGATE_OBSTACLE:
		{
			std::cout << "CIRCUM YEEE\n";
			break;
		}

		case RETURN_TO_SAVED_POINT:
		{
			std::cout << "Testing\n";
			break;
		}
	
		default: break;
	}
}
