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

void test_Bug(cv::Mat& map)
{
    RGB8b starcol = {255,0,0};
    RGB8b goalcol = {0,255,0};
    RGB8b terrain_col = {.B = 0, .G = 255, .R=0 };
    terrrain dirt = {terrain_col,1,false};
    std::cout << "ret rgb " << dirt.ret_rgb() << "Ret val:" << dirt.ret_val() << std::endl;
    Bug UUT("Test1", map, starcol, goalcol);
    for(int x = 0; x < 100; x++)
    {
        UUT.insert_tile(dirt,coords{20,x} );
    }
    
    cv::imshow("Bug_map",UUT.get_map());
    
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
    test_Bug(map);
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