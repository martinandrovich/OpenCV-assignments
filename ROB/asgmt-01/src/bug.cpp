#include "bug.h"

#define delay_base 50000
#define delay usleep(delay_base * 0.05)

void bug1a(cv::Mat& img)
{
	// states
	enum state
	{
		MOVE_TOWARDS_GOAL,
		CIRCUMNAVIGATE_OBSTACLE,
		RETURN_TO_SAVED_POINT
	};

	// variables
	state 		bug_state = MOVE_TOWARDS_GOAL;
	cv::Point 	pos_cur = {10, 10};
	cv::Point 	pos_next;
	cv::Point 	pos_goal = {20, 400};
	cv::Point 	move_dir;

	cv::Point 	pos_wall_start;
	cv::Point 	pos_wall_saved;

	std::vector<std::pair<cv::Point, float>> circnav;
	
	// prepare kernel
	kernel::load_img(img);

	// state machine
	while(pos_cur != pos_goal) switch (bug_state)
	{
		case MOVE_TOWARDS_GOAL:
		{
			// draw image
			draw_img(img, pos_cur);

			// determine direction of shortest distance
			pos_next = kernel::find_nearest(pos_cur, pos_goal);

			// check if desired point is reachable (no obstacles)
			// change state if not
			if(!kernel::is_reachable(pos_next))
			{
				bug_state = CIRCUMNAVIGATE_OBSTACLE;
				// calculate initial moveaxis
				move_dir = {1,0};
				pos_next = pos_cur;
				pos_wall_start = pos_cur;
				break;
			}

			// step
			std::cout << ansi::kernel << "moving from: " << pos_cur << " to " << pos_next << "\n";
			pos_cur = pos_next;

			// delay
			delay;
			
			break;
		}

		case CIRCUMNAVIGATE_OBSTACLE:
		{	

			// draw image
			draw_img(img, pos_cur);

			// check wall
			// if next = black -> change direction
			// dir = flip(dir) *  :
			// check current pos, if nextpos blocked, find white pixel 	wher
			//kernel::check_wall();
			pos_next += move_dir;

			// check corner -> check which side has surface -> change direction/move axis
			// if next = blank -> jump diagonal
			std::tie(pos_next, move_dir) = kernel::check_corner(pos_cur, pos_next);

			// move in specified direction
			// check full circle
			if (pos_next == pos_wall_start)
			{
				bug_state = RETURN_TO_SAVED_POINT;

				// find lowest point
				pos_wall_saved = (std::min_element(circnav.cbegin(), circnav.cend(), [](auto& a, auto& b) {
        			return a.second < b.second;    
    			}))->first;

				// reverse vector
				std::reverse(circnav.begin(), circnav.end());

				break;
			}

			std::cout << ansi::kernel << "moving from: " << pos_cur << " to " << pos_next << " with direction " << move_dir << "\n";
			pos_cur = pos_next;

			// record point with shortest distance to goal
			circnav.emplace_back(pos_cur, kernel::distance(pos_cur, pos_goal));

			delay;

			break;
		}

		case RETURN_TO_SAVED_POINT:
		{

			for (auto pos = circnav.begin(); pos->first != pos_wall_saved; pos++ )
			{
				pos_next = pos->first;
				std::cout << ansi::kernel << "moving from: " << pos_cur << " to " << pos_next << " with direction " << move_dir << "\n";
				pos_cur = pos_next;
				draw_img(img, pos_cur);
				delay;
			}

			bug_state = MOVE_TOWARDS_GOAL;

			break;
		}
	
		default: break;
	}
}

void init_bug(cv::Mat& img)
{

	cv::Point pos_start 	= {-1,-1};
	cv::Point pos_target 	= {-1,-1};

	// prepare kernel
	kernel::load_img(img);

	// show image
	cv::imshow("bug", img);
	cv::waitKey(1);

	// set start position
	std::cout << "click on the image to select a START position." << std::endl;

	cv::setMouseCallback("bug", [](int e, int x, int y, int s, void *p){
		if (e == 1) { *((cv::Point*)p) = {x, y}; } 
		
	}, &pos_start);

	wait_start:
	while(pos_start.x == -1) cv::waitKey(1);

	if(!kernel::is_reachable(pos_start))
	{
		std::cout << "please select a reachable pixel." << std::endl;
		pos_start = {-1, -1};
		goto wait_start;
	}

	std::cout << "start: " << pos_start << std::endl;
	cv::circle(img, pos_start, 4, {255, 0, 0}, -1, cv::LINE_AA);
	cv::imshow("bug", img);
	cv::waitKey(1);

	// set target position
	std::cout << "click on the image to select a TARGET position." << std::endl;

	cv::setMouseCallback("bug", [](int e, int x, int y, int s, void *p){
		if (e == 1) { *((cv::Point*)p) = {x, y}; } 
		
	}, &pos_target);

	wait_target:
	while(pos_target.x == -1) cv::waitKey(1);

	if(!kernel::is_reachable(pos_target))
	{
		std::cout << "please select a reachable pixel." << std::endl;
		pos_target = {-1, -1};
		goto wait_target;
	}

	std::cout << "target: " << pos_start << std::endl;
	cv::circle(img, pos_target, 4, {255, 0, 0}, -1, cv::LINE_AA);
	cv::imshow("bug", img);
	cv::waitKey(1);

	// select algorithm
	;

	// run
	std::cout << "Press ANY KEY to begin." << std::endl;
	cv::waitKey();
	bug1(img, pos_start, pos_target);
}

void bug1(cv::Mat& img, cv::Point pos_start, cv::Point pos_target)
{
	// states
	enum state
	{
		DETERMINE_SLOPE,
		MOVE_TOWARDS_GOAL,
		CIRCUMNAVIGATE_OBSTACLE,
		RETURN_TO_SAVED_POINT
	};

	// variables
	state 		bug_state = DETERMINE_SLOPE;

	double 		slope, remainder;

	cv::Point 	pos_cur = pos_start;
	cv::Point 	pos_next, move_dir, pos_wall_start, pos_wall_saved;

	std::vector<std::pair<cv::Point, float>> circnav;

	// state machine
	while(pos_cur != pos_target) switch (bug_state)
	{
		case DETERMINE_SLOPE:
		{
			// determine slope and initial remainder
			auto delta_x = pos_target.x - pos_cur.x;
			auto delta_y = pos_target.y - pos_cur.y;
			slope = (delta_x) ? ((float)delta_y / delta_x) : 1;
			remainder = slope;

			// determine direction of x & y
			move_dir.x = (delta_x ? (delta_x < 0 ? -1 : 1) : 0);
			move_dir.y = (delta_y ? (delta_y < 0 ? -1 : 1) : 0);

			// begin moving
			pos_next = pos_cur;
			bug_state = MOVE_TOWARDS_GOAL;

			//
			// !break;
			//
		}
		case MOVE_TOWARDS_GOAL:
		{

			// modification of shortest path algorithm
			// to only move one pixel per iteration

			// draw image
			draw_img(img, pos_cur);

			// update remainder if needed
			if (abs(remainder) < 1)
			{
				remainder = slope + remainder;
			}

			// move x if needed
			if (abs(remainder) >= abs(slope) && pos_cur.x != pos_target.x)
			{
				pos_next.x = pos_cur.x + move_dir.x;
			}			

			// move y if needed
			if (abs(remainder) >= 1)
			{
				pos_next.y = pos_cur.y + move_dir.y;
				remainder += (remainder > 0) ? -1 : 1;
			}

			// check if desired point is reachable (no obstacles)
			// change state if not
			if(auto pos_obstacle = kernel::check_obstacle(pos_cur, pos_next))
			{
				// calculate initial moveaxis
				move_dir = pos_obstacle.value() - pos_cur;

				// prepare circumnavigation
				pos_next = pos_cur;
				pos_wall_start = pos_cur;
				circnav.clear();
				
				bug_state = CIRCUMNAVIGATE_OBSTACLE;
				break;
			}

			// move
			kernel::move(pos_cur, pos_next);

			// delay
			delay;
			
			break;
		}

		case CIRCUMNAVIGATE_OBSTACLE:
		{	

			// draw image
			draw_img(img, pos_cur);

			// check wall
			// change direction if blocked
			move_dir = kernel::check_wall(pos_cur, move_dir);
			pos_next += move_dir;

			// check for corners
			// move diagonally if encourtered + change direction
			std::tie(pos_next, move_dir) = kernel::check_corner(pos_cur, pos_next);

			// check if reached beginning point
			if (pos_next == pos_wall_start)
			{
				// find shortest distance from recorded points
				pos_wall_saved = (std::min_element(circnav.cbegin(), circnav.cend(), [](auto& a, auto& b) {
        			return a.second < b.second;    
    			}))->first;

				// reverse vector
				std::reverse(circnav.begin(), circnav.end());

				bug_state = RETURN_TO_SAVED_POINT;
				break;
			}

			// move
			kernel::move(pos_cur, pos_next);

			// record point with shortest distance to goal
			circnav.emplace_back(pos_cur, kernel::distance(pos_cur, pos_target));

			delay;

			break;
		}

		case RETURN_TO_SAVED_POINT:
		{

			for (auto pos = circnav.begin(); pos_cur != pos_wall_saved; pos++ )
			{
				pos_next = pos->first;
				kernel::move(pos_cur, pos_next);
				draw_img(img, pos_cur);
				delay;
			}

			bug_state = DETERMINE_SLOPE;

			break;
		}
	
		default: break;
	}

	// done, show image and number of steps
	std::cout << "\ncompleted bug1 using " << kernel::get_num_steps() << " steps." << std::endl;
	cv::waitKey();
}
