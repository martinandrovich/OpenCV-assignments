#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <tuple>
#include "src/bug.h"





void insert_start(cv::Mat&  image, cv::Mat& output, coords coordinate, ROBtek::RGB8b color)
{
    output = image.clone();
    output.ptr<ROBtek::RGB8b>(coordinate.y)[coordinate.x] = color;
}

void draw_path(cv::Mat& image, cv::Mat& output, std::vector<coords> data_set)
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
    ROBtek::terrrain grass = {terrain_col_green,1.5,true};
    ROBtek::terrrain water = {terrain_col_blue,10,false};
    ROBtek::terrrain fire = {terrain_col_red,100,false};
    ROBtek::terrrain lemon = {terrain_col_yellow,1,true};
    grass.print_terain_info();
    water.print_terain_info();
    fire.print_terain_info();
    lemon.print_terain_info();
    Bug UUT("Test1", map, grass, lemon);
    for(int x = 0; x < 100; x++)
    {
        std::cout << "Size of map is: "<< UUT.get_known_terain_size() << std::endl; 
        UUT.insert_terrain(grass,coords{20,x} );
        UUT.insert_terrain(water,coords{30,x} );
        UUT.insert_terrain(fire,coords{40,x} );
        UUT.insert_terrain(lemon,coords{50,x} );
    }
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
        insert_start(new_map,new_map, coords{idx,idx}, { 0,0,0} );
    }
    
    //cv::imshow("rotated in_imag twice", new_map);
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