#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
int main()
{
    cv::Mat image = cv::imread("assets/Astronaut1.jpg");
    int test = 0;
    if (image.empty())
    {
        std::cerr << "Could not load image";
        return 1;
    }
    std::cout<<"Hello world xD "<<test<<std::endl;
    cv::namedWindow("Image");
    cv::imshow("Image", image);
    cv::waitKey();
    return test;


}
/* 
 
 
 return 0;*/