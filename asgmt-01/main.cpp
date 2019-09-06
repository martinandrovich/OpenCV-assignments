#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <tuple>



cv::Mat& rotate(cv::Mat &image, cv::Mat &output, int rotation=0)
{
    cv::Mat cp_image = image.clone();
    for(int x = 0; x < image.rows ; x++)
    {
        for(int y = 0; y < image.cols; y++)
        {
            for(int channel = 0; channel < image.channels(); channel++)
            {
                cp_image.at<cv::Vec3b>(image.rows-1 -y,x)[channel] = image.at<cv::Vec3b>(x,y)[channel];
            }            
        }    
    }
    output = cp_image;
    return output;
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

cv::Mat& Segment_spoons(cv::Mat& img,int threshold)
{
   for(int x = 0; x < img.rows ; x++)
    {
        for(int y = 0; y < img.cols; y++)
        {
            if ((img.at<cv::Vec3b>(x,y)[2] < threshold) ||
               img.at<cv::Vec3b>(x,y)[0] > 30  && img.at<cv::Vec3b>(x,y)[1] > 30
               )
            {
                img.at<cv::Vec3b>(x,y)[0] = 0;
                img.at<cv::Vec3b>(x,y)[1] = 0;
                img.at<cv::Vec3b>(x,y)[2] = 0;
            }
        }    
    }
    return img;
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
    // rotate(out_image, out_image);
    // cv::imshow("rotated in_imag twice", out_image);
    
    Segment_spoons(in_image,120);
    cv::imshow("Segmented Image", in_image);
    cv::waitKey();
    return test;


}
/* 
 
 
 return 0;*/