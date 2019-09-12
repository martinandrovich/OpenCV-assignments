#include "src/bug.h"

int main()
{
	std::cout << "bug algorithms, v. 1.0\n\n";
	
	cv::Mat img = cv::imread("assets/environ-01.png");
	init_bug(img);

	return 0;
}
