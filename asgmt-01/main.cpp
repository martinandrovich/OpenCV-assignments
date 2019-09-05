#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>



void rotate(cv::Mat image, int rotation)
{
    for(int x = 0; x < image.rows ; x++)
    {
        image.at<cv::Vec3b>(x,0) = 0;

    }
}


void InvertColour( cv::Mat& input_image, cv::Mat& output_image )
{
 CV_Assert( input_image.type() == CV_8UC3 );
 output_image = input_image.clone();
 for (int row=0; row < input_image.rows; row++)
 for (int col=0; col < input_image.cols; col++)
 for (int channel=0; channel <
 input_image.channels(); channel++)
 output_image.at<cv::Vec3b>(row,col)[channel] = 255 - input_image.at<cv::Vec3b>(row,col)[channel];
}

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