#include "kernel.h"

constexpr auto KERNEL_WIDTH 	= 3;
constexpr auto KERNEL_HEIGHT 	= 3;

//constexpr auto is_black_pixel(cv::Vec3b& v) { return ((v[0] + v[1] + v[2]) == 255 * 3); }
//constexpr bool is_white_pixel(cv::Vec3b& v) { return (v[0] + v[1] + v[2]) == 0; };

#define white_pixel ((v[0] + v[1] + v[2]) == 255 * 3)
#define is_black_pixel ((v[0] + v[1] + v[2]) == 0)


namespace kernel
{
	cv::Mat img;
	int num_steps = 0;
}

void kernel::load_img(cv::Mat& img)
{
	kernel::img = img;
}

void kernel::scan(cv::Point pos_start, std::function<void(cv::Point&, cv::Vec3b&)> callback)
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

	bound_row = (pos_start.y >= (kernel::img.rows - 1)) ? kernel::img.rows : pos_start.y + 2;
	bound_col = (pos_start.x >= (kernel::img.cols - 1)) ? kernel::img.cols : pos_start.x + 2;
	start_col = pos_start.x - ((pos_start.x <= 0) ? 0 : 1);
	start_row = pos_start.y - ((pos_start.y <= 0) ? 0 : 1);

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

void
kernel::scan_pixel(cv::Point pos, std::function<void(cv::Point&, cv::Vec3b&)> callback)
{
	// make sure that there is an image to scan
	assert(!kernel::img.empty());
	
	cv::Point p = {pos.x, pos.y};
	cv::Vec3b v = kernel::img.at<cv::Vec3b>(p);

	callback(p, v);
}

bool
kernel::check_pixel(cv::Point pos, std::function<bool(cv::Point&, cv::Vec3b&)> callback)
{
	// make sure that there is an image to scan
	assert(!kernel::img.empty());
	
	cv::Point p = {pos.x, pos.y};
	cv::Vec3b v = kernel::img.at<cv::Vec3b>(p);

	return callback(p, v);
}

inline float
kernel::distance(cv::Point pos_start, cv::Point pos_target)
{
	return sqrt(pow(abs(pos_target.x - pos_start.x), 2) + pow(abs(pos_target.y - pos_start.y), 2));
}

void
kernel::move(cv::Point& pos_start, cv::Point& pos_target)
{
	std::cout << ansi::kernel << "moving from: " << pos_start << " to " << pos_target << " with direction " << (pos_target-pos_start) << "\n";
	pos_start = pos_target;
	num_steps++;
}

int
kernel::get_num_steps()
{
	return kernel::num_steps;
}


cv::Point
kernel::find_nearest(cv::Point pos_start, cv::Point pos_target)
{
	std::map<float, cv::Point> pts;
	float dist;

	kernel::scan(pos_start, [&](cv::Point& p, cv::Vec3b& v) {
		//dist = sqrt(pow(abs(pos_target.x - p.x), 2) + pow(abs(pos_target.y - p.y), 2));
		dist = kernel::distance(pos_target, p);
		pts[dist] = cv::Point(p.x, p.y);

	});

	// return the value of the first pair in map
	return (pts.begin()->second);
}

bool
kernel::is_reachable(cv::Point pos_target)
{	
	return kernel::check_pixel(pos_target, [&](auto& p, auto& v){ return !is_black_pixel; } );
}

std::optional<cv::Point>
kernel::check_obstacle(cv::Point pos_start, cv::Point pos_target)
{
	// check if position is reachable
	// diagonal movement allowed
	if (kernel::check_pixel(pos_target, [&](auto& p, auto& v){ return !is_black_pixel; } ))
		return std::nullopt;

	cv::Point pos_obstacle;

	// points of interst

	// [ ][T][ ]
	// [L][S][R]
	// [ ][B][ ]

	std::vector<cv::Point> poi = 
	{
		{pos_start.x, pos_start.y - 1},		// T
		{pos_start.x, pos_start.y + 1},		// B
		{pos_start.x - 1, pos_start.y},		// L
		{pos_start.x + 1, pos_start.y},		// R
	};

	// run kernel algoritm
	// increment match if a point of interest is black pixel
	kernel::scan(pos_target, [&](cv::Point& p, cv::Vec3b& v) {

		if (std::any_of(poi.begin(), poi.end(), [&](auto& pt) {
			return (pt == p && is_black_pixel);
		}))
		{
			pos_obstacle = p;
			std::cout << ansi::kernel << "found obstacle at: " << pos_obstacle << "\n";
		}

	});

	return pos_obstacle;
	
}

bool is_reachable_old(cv::Point pos_start, cv::Point pos_target)
{	
	int match = 0;

	// points of interst

	// [ ][T][ ]
	// [L][S][R]
	// [ ][B][X]

	std::vector<cv::Point> pts = 
	{
		{pos_start.x, pos_start.y - 1},		// T
		{pos_start.x, pos_start.y + 1},		// B
		{pos_start.x - 1, pos_start.y},		// L
		{pos_start.x + 1, pos_start.y},		// R
		{pos_target.x, pos_target.y},		// X
	};

	// run kernel algoritm
	// increment match if a point of interest is black pixel
	kernel::scan(pos_start, [&](cv::Point& p, cv::Vec3b& v) {

		if (std::any_of(pts.begin(), pts.end(), [&](auto& p2) {
			return (p2 == p && !(v[0] + v[1] + v[2]));
		}))
		{
			match++;
			std::cout << ansi::kernel << "found obstacle at: " << p << "\n";
		}

	});

	return !match;
}

std::tuple<cv::Point, cv::Point>
kernel::check_corner(cv::Point pos_start, cv::Point pos_next)
{
	
	std::vector<cv::Point> pos_srfaces;
	std::vector<cv::Point> pos_corners;

	// points of interst

	// [ ][T][ ]  [A][ ][B]
	// [L][S][R]  [ ][S][ ] -->
	// [ ][B][ ]  [C][ ][D]

	std::vector<cv::Point> poi_black = 
	{
		{pos_start.x, pos_start.y - 1},			// T
		{pos_start.x, pos_start.y + 1},			// B
		{pos_start.x - 1, pos_start.y},			// L
		{pos_start.x + 1, pos_start.y},			// R
	};

	std::vector<cv::Point> poi_white = 
	{
		{pos_start.x - 1, pos_start.y - 1},		// A
		{pos_start.x + 1, pos_start.y - 1},		// B
		{pos_start.x - 1, pos_start.y + 1},		// C
		{pos_start.x + 1, pos_start.y + 1},		// D
	};

	kernel::scan(pos_start, [&](cv::Point& p, cv::Vec3b& v) {

		//std::cout << p << " - " << v << " - " << white_pixel << "\n";

		// black pixel matching points of interst
		if(std::find(poi_black.begin(), poi_black.end(), p) != poi_black.end() && is_black_pixel)
			pos_srfaces.push_back(p);

		// white pixel matching points of interst
		if(std::find(poi_white.begin(), poi_white.end(), p) != poi_white.end() && !is_black_pixel)
			pos_corners.push_back(p);
		
	});

	// find intersection
	// initialize to default values in case none found
	cv::Point intersection = pos_next;
	cv::Point move_dir = pos_next - pos_start;

	for (auto& sur : pos_srfaces)
	{
		for (auto& cor : pos_corners)
		{
			if ((cor.x == sur.x && cor.y == pos_next.y) || (cor.y == sur.y && cor.x == pos_next.x))
			{
				std::cout << ansi::kernel << "found corner at: " << cor << std::endl;
				intersection = {cor.x, cor.y};
				move_dir = sur - pos_start;
			}
		}
	}

	// return tuple
	return {intersection, move_dir};

}

cv::Point
kernel::check_wall(cv::Point pos_start, cv::Point move_dir)
{

	// check if pixel in moving direction is black
	if (check_pixel(pos_start + move_dir, [&](auto& p, auto& v){ return is_black_pixel; } ))
	{

		std::cout << ansi::kernel << "found wall at: " << (pos_start + move_dir) << std::endl;

		move_dir = { move_dir.y, move_dir.x };
		move_dir *= (check_pixel(pos_start + move_dir, [&](auto& p, auto& v){ return is_black_pixel; })) ? -1 : 1;
	}

	return move_dir;

}