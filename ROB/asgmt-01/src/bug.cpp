#include "bug.h"
#include <iostream>
namespace ROBtek
{
    RGB8b make_RGB(uchar red, uchar green, uchar blue )
    {
        return RGB8b{blue,green,red};
    }
}

Bug::Bug(const std::string& name,cv::Mat& map, ROBtek::terrrain start_terrain, ROBtek::terrrain goal_terrain):
    m_name{name},
    m_map{map.clone()}
    {
        this->add_terrain(start_terrain);
        this->add_terrain(goal_terrain);

    }

void Bug::insert_terrain(ROBtek::terrrain terrain, coords coordinate)
{
    this->m_map.at<ROBtek::RGB8b>(cv::Point2i(coordinate.y, coordinate.x)) = terrain.color;
    this->m_terrains.insert(std::make_pair( terrain.ret_val(), std::pair<double,bool>(terrain.weight,terrain.walkable) ));
}

void Bug::add_terrain(ROBtek::terrrain terrain)
{
    this->m_terrains.insert(std::make_pair( terrain.ret_val(), std::pair<double,bool>(terrain.weight,terrain.walkable) ));
}


void Bug::print_known_terains()
{
    std::cout << "Printing all known terrains..." << std::endl << std::endl;
    for(auto const& [color,val]: this->m_terrains)
    {
        std::cout << "Terain colors: "<< color << /* " (r,g,b) -> " << (uchar*)(color & (uint)0x000000ff)  << " , "<< (uchar)(color & (uint)0x0000ff00) <<" , " << (uchar)((uint)(color) & (uint)0x00ff0000)*/
         "\n Weight : " << val.first <<"\n Walkable? " << val.second<<std::endl;
    }
}

void Bug::_inspect_adjacent_terrain()
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
