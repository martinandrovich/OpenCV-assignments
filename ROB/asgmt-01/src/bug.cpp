#include "bug.h"
#include <iostream>
namespace ROBtek
{
    RGB8b make_RGB(uchar red, uchar green, uchar blue )
    {
        return RGB8b{blue,green,red};
    }
}

Bug::Bug(const std::string& name,cv::Mat& map, ROBtek::tile start_tile, ROBtek::tile goal_tile):
    m_name{name},
    m_map{map.clone()}
    {
        this->insert_tile(start_tile);
        this->insert_tile(goal_tile);

    }

void Bug::insert_tile(ROBtek::tile tile)
{    
    for(int x = 0; x < tile.size.x; x++)
    {
        for(int y = 0; y < tile.size.y; y++)
        {
            this->m_map.at<ROBtek::RGB8b>(cv::Point2i(tile.coordinate.y + x , tile.coordinate.x + y)) = tile.terrain_t.color;    
        }
    }
    this->m_terrains.insert(std::make_pair( tile.terrain_t.ret_val(), std::pair<double,bool>(tile.terrain_t.weight,tile.terrain_t.walkable) ));         
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

ROBtek::tile Bug::_determine_tile(ROBtek::coords coordinate)
{
    //std::


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
