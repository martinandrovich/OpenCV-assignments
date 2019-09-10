#include "src/core.h"

int main()
{
	std::cout << "Hello world\n";

	//draw_test();
	
	cv::Mat img = cv::imread("assets/environ-01.png");
	bug1(img);

	return 0;
}
