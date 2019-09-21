#include "kernel.h"

/*
+--------+----+----+----+----+------+------+------+------+
|        | C1 | C2 | C3 | C4 | C(5) | C(6) | C(7) | C(8) |
+--------+----+----+----+----+------+------+------+------+
| CV_8U  |  0 |  8 | 16 | 24 |   32 |   40 |   48 |   56 |
| CV_8S  |  1 |  9 | 17 | 25 |   33 |   41 |   49 |   57 |
| CV_16U |  2 | 10 | 18 | 26 |   34 |   42 |   50 |   58 |
| CV_16S |  3 | 11 | 19 | 27 |   35 |   43 |   51 |   59 |
| CV_32S |  4 | 12 | 20 | 28 |   36 |   44 |   52 |   60 |
| CV_32F |  5 | 13 | 21 | 29 |   37 |   45 |   53 |   61 |
| CV_64F |  6 | 14 | 22 | 30 |   38 |   46 |   54 |   62 |
+--------+----+----+----+----+------+------+------+------+
*/

void
kernel::scan(cv::Mat& img, size_t size, cv::Point pos_start, std::function<void(cv::Point&, cv::Vec3b&)> callback)
{

	// variables
	cv::Point cur_pixel;
	cv::Vec3b cur_color;
	int bound_row, bound_col, start_col, start_row;

	// absolute bounds and starting position for seeking pixel
	// if seeking pixel starts in a corner or near image edge

	// [ ][ ][ ]
	// [ ][X][ ]
	// [ ][ ][ ]

	bound_row = (pos_start.y >= (img.rows - 1)) ? img.rows : pos_start.y + (size - 1);
	bound_col = (pos_start.x >= (img.cols - 1)) ? img.cols : pos_start.x + (size - 1);
	start_col = pos_start.x - ((pos_start.x <= 0) ? 0 : 1);
	start_row = pos_start.y - ((pos_start.y <= 0) ? 0 : 1);

	// log kernel size reduction
	if (size < size)
	{

	}

	// seek the kernel grid
	for(int row = start_row; row < bound_row; row++)
	{
		for(int col = start_col; col < bound_col; col++)
		{
			cv::Point p = {col, row};
			cv::Vec3b v = img.at<cv::Vec3b>(p);

			callback(p, v);
		}
	}
}

void
scanimg(cv::Mat& img, size_t size, std::function<void(cv::Point&, cv::Vec3b&)> callback)
{

	// assert existence of image and odd kernel size
	assert(!img.empty()); assert(size % 2 == 1);

	return;
}

void
kernel::test()
{
	// matrix creation
	cv::Mat ones = cv::Mat::ones(8, 8, CV_8U);

	// 3D matrix creation
	cv::Mat mat_3d = cv::Mat(cv::Size(3, 3), CV_8UC3, cv::Scalar(1, 1, 1));

	// scalar multiplications
	ones = ones * 2;

	// mask
	// format: col, row, width, height
	auto mask = ones(cv::Rect(5,1,3,2));

	// submatrix test
	std::cout << "is mask sub of ones : " << std::boolalpha << mask.isSubmatrix() << std::endl;

	// edit area
	ones(cv::Rect(2, 2, 4, 4)) = 0;

	// print
	std::cout << "\n\nones:\n\n" << ones << std::endl;
	std::cout << "\n\nmask:\n\n" << mask << std::endl;

	// parallel traverse
	mask.forEach<uchar>([](auto& pxl, auto pos) -> void {
		auto row = pos[0]; auto col = pos[1];
		std::cout << "col: " << col << " | row: " << row << " | val: " << (int)pxl << std::endl;
	});

	// matrices
	auto rect1  = cv::Mat::ones(3,3, CV_8U);
	auto rect2  = cv::Mat::ones(3,3, CV_8U) * 2;

	// dot product
	auto dot    = rect1.dot(rect2);

	// matrix element wise multiplication
	auto rect3  = rect1.mul(rect2);

	std::cout
		<< "\n\nrect1:\n\n" 	<< rect1
		<< "\n\nrect2:\n\n" 	<< rect2
		<< "\n\ndot:\n\n" 		<< dot
		<< "\n\nrect3:\n\n" 	<< rect3
		<< std::endl;

	// for more: operations on arrays
	// https://docs.opencv.org/2.4/modules/core/doc/operations_on_arrays.html
}