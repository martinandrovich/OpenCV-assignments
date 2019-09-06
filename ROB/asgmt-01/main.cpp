#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <tuple>
#include "src/bug.h"





void insert_start(cv::Mat&  image, cv::Mat& output, coords coordinate, RGB8b color)
{
    output = image.clone();
    output.ptr<RGB8b>(coordinate.y)[coordinate.x] = color;
}

void draw_path(cv::Mat& image, cv::Mat& output, std::vector<coords> data_set)
{
    output = image.clone();
    for(auto& point:data_set)
    {
        //output.ptr<RGB_col>(point.y)[]

    }

}

void test_map()
{
    std::map<int,int> test;
    
    
    test[1] = 5;
    std::map<int,int>::iterator it = test.begin();
    std::cout<<"Value of key: "<< test[1] << std::endl;
    test[2] = 5;
    std::cout<<"Value of key: "<< test[2] << std::endl;
    test.insert(std::pair<int,int>(2,2));
    std::cout<<"Value of key: "<< test[2] << std::endl;
}

int main()
{
    test_map();
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
        insert_start(new_map,new_map, coords{idx,idx}, { 0,0,0} );
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