#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <tuple>

struct coords
{
    int x, y;
};

struct RGB_col
{
    int R,G,B;
};

cv::Mat& insert_goal(cv::Mat& image, coords coordinate, RGB_col color)
{
    image.at<cv::Vec3b>(coordinate.y, coordinate.x)[2] = color.R;
    image.at<cv::Vec3b>(coordinate.y, coordinate.x)[1] = color.G;
    image.at<cv::Vec3b>(coordinate.y, coordinate.x)[0] = color.B;
}
cv::Mat& insert_start(cv::Mat&  image, coords coordinate, uchar color)
{

}

cv::Mat& draw_path(cv::Mat& image, coords start, coords stop)
{

}

int main()
{
    cv::Mat in_image = cv::imread("assets/BabyFood/BabyFood-Sample1.JPG");
    int test = 0;
    if (in_image.empty())
    {
        std::cerr << "Could not load image";
        return 1;
    }
    std::cout<<"Hello world xD "<<test<<std::endl;
    cv::namedWindow("Image");
    // cv::namedWindow("Image");
    // cv::Mat out_image;
    // InvertColour(in_image, out_image);
    // cv::imshow("Image", in_image);
    // cv::imshow("Inverted Image", out_image);
    // rotate(out_image, out_image);
    // cv::imshow("rotated Image", out_image);
    rotate(in_image, in_image);
    cv::imshow("rotated in_imag twice", in_image);
    
    Segment_spoons(in_image,120);
    cv::imshow("Segmented Image", in_image);
    cv::waitKey();
    return test;


}
/* 
 
 
 return 0;*/