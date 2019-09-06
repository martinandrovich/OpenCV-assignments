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
    uchar R,G,B;
};

void insert_goal(cv::Mat& image,cv::Mat& output, coords coordinate, RGB_col color)
{
    output = image.clone();
    output.at<cv::Vec3b>(coordinate.y, coordinate.x)[2] = color.R;
    output.at<cv::Vec3b>(coordinate.y, coordinate.x)[1] = color.G;
    output.at<cv::Vec3b>(coordinate.y, coordinate.x)[0] = color.B;
    output.at<cv::Vec3b>(cv::Point2i(coordinate.y, coordinate.x)) = cv::Vec3b{1,2,3};
}
void insert_start(cv::Mat&  image, cv::Mat& output, coords coordinate, RGB_col color)
{
    output = image.clone();
    output.ptr<RGB_col>(coordinate.y)[coordinate.x] = color;
}

void draw_path(cv::Mat& image, cv::Mat& output, std::vector<coords> data_set)
{
    output = image.clone();
    for(auto& point:data_set)
    {
        //output.ptr<RGB_col>(point.y)[]

    }

}

int main()
{
    cv::Mat map = cv::imread("assets/environ-01.png");
    if (map.empty())
    {
        std::cerr << "Could not load image";
        return 1;
    }
    cv::Mat new_map = map.clone();
    
    //insert_goal(map,new_map, coords{idx,idx}, RGB_col{.R = 128, .G=0, .B=0} );
    for(int idx = 0; idx < 100; idx++)
    {
        //insert_goal(new_map,new_map, coords{idx,idx}, RGB_col{.R = 128, .G=0, .B=0} );
        insert_start(new_map,new_map, coords{idx,idx}, RGB_col{.R = 0, .G=0, .B=0} );
    }
    
    cv::imshow("rotated in_imag twice", new_map);
    // cv::imshow("Image", in_image);
    // cv::imshow("Inverted Image", out_image);
    // rotate(out_image, out_image);
    // cv::imshow("rotated Image", out_image);
    
    
    //Segment_spoons(in_image,120);
    //cv::imshow("Segmented Image", in_image);
    cv::waitKey();
    return 0;


}
/* 
 
 
 return 0;*/