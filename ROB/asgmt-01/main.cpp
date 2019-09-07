#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <tuple>
#include "src/bug.h"





void insert_start(cv::Mat&  image, cv::Mat& output, ROBtek::coords coordinate, ROBtek::RGB8b color)
{
    output = image.clone();
    output.ptr<ROBtek::RGB8b>(coordinate.y)[coordinate.x] = color;
}

void draw_path(cv::Mat& image, cv::Mat& output, std::vector<ROBtek::coords> data_set)
{
    output = image.clone();
    for(auto& point:data_set)
    {
        //output.ptr<RGB_col>(point.y)[]

    }

}

void test_Bug(cv::Mat& map)
{
    ROBtek::RGB8b starcol = {255,0,0};
    ROBtek::RGB8b goalcol = {0,255,0};
    ROBtek::RGB8b terrain_col_green = ROBtek::make_RGB(0,255,0);
    ROBtek::RGB8b terrain_col_blue = ROBtek::make_RGB(0,0,255);
    ROBtek::RGB8b terrain_col_red = ROBtek::make_RGB(255,0,0);
    ROBtek::RGB8b terrain_col_yellow = ROBtek::make_RGB(255,255,0);
    ROBtek::tile grass = {{terrain_col_green,1.5,true}, {0,0},{20,60} };
    ROBtek::tile water = {{terrain_col_blue,1.5,true}, {20,20},{20,20} };
    ROBtek::tile fire = {{terrain_col_red,1.5,true}, {40,40},{20,20} };
    ROBtek::tile lemon = {{terrain_col_yellow,1.5,true}, {60,60},{20,20} };

    std::cout<<"Tile size:" << grass.size.x << " X "<< grass.size.y << std::endl;
    Bug UUT("Test1", map, grass, lemon);
        
    
    UUT.insert_tile(grass);
    UUT.insert_tile(water);
    UUT.insert_tile(fire);
    UUT.insert_tile(lemon);

    UUT.print_known_terains();
    
    cv::imshow("Bug_map",UUT.get_map());
    
}

int main()
{
    
    cv::Mat map = cv::imread("assets/environ-01.png");
    cv::Mat test_map = cv::imread("assets/test_100x100.png");
    if (map.empty())
    {
        std::cerr << "Could not load image";
        return 1;
    }
    cv::Mat new_map = map.clone();
    test_Bug(test_map);
    //insert_goal(map,new_map, coords{idx,idx}, RGB_col{.R = 128, .G=0, .B=0} );
    for(int idx = 0; idx < 100; idx++)
    {
        //insert_goal(new_map,new_map, coords{idx,idx}, RGB_col{.R = 128, .G=0, .B=0} );
        insert_start(new_map,new_map, ROBtek::coords{idx,idx}, { 0,0,0} );
    }
    
    cv::imshow("rotated in_imag twice", map);
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