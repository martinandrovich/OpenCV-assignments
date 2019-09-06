#include "bug.h"


Bug::Bug(const std::string& name,cv::Mat& map, RGB8b start_tile_color, RGB8b goal_tile_color):
    //m_goal_tile_color{goal_tile_color},
    //m_start_tile_color{start_tile_color},
    m_name{name}
    //m_map{map.clone()}
    {
        //this->m_goal_tile_color = goal_tile_color;
        //this->m_name = name;

    }

void Bug::insert_tile(terrrain tile, coords coordinate)
{
    this->m_map.at<cv::Vec3b>(cv::Point2i(coordinate.y, coordinate.x)) = tile.color;
    //this->m_terrain_tiles.insert(tile.color)
    //output.at<cv::Vec3b>(coordinate.y, coordinate.x)[1] = color.G;
    //output.at<cv::Vec3b>(coordinate.y, coordinate.x)[0] = color.B;
    //output.at<cv::Vec3b>(cv::Point2i(coordinate.y, coordinate.x)) = cv::Vec3b{1,2,3};
}

void Bug::_inspect_adjacent_tiles()
{

}

void Bug::_path_planning()
{

}
void Bug::_reconstruct_closest_route(){

}
int Bug::_get_slope_to_goal(){
    return 0;

}