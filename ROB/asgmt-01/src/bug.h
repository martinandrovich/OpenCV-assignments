#pragma once
#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <map>
#include <unordered_map>
#include <iostream>
typedef unsigned char uchar;

namespace ROBtek
{
    struct RGB8b
    {   
        union{
            struct 
            {
                uchar B,G,R;
            };
            uchar color[3];  
        };
    };

    RGB8b make_RGB(uchar red, uchar green, uchar blue );
   
    struct terrrain
    {
        RGB8b color;
        double weight;
        bool walkable;
        uint ret_val(){return( *(uint*)&this->color.color);}
        void print_terain_info(){ std::cout 
        << "Terrain color: "
        << this->ret_val()
        << " \nTerrain weight: "<< this->weight 
        << " \nTerrain walkable? "<< std::boolalpha <<this->walkable << std::endl;  }
    };

    struct coords
    {
        int x, y;
    };

   

    struct tile
    {
        terrrain terrain_t;       
        coords coordinate;
        struct 
        {
            int x,y;
        }size;
    };
};







class Bug
{
    public:
    Bug(const std::string& name,cv::Mat& map, ROBtek::tile start_tile, ROBtek::tile goal_tile);
    
    
    int get_walked_distance(){ return m_walked_distance;}
    const cv::Mat& get_map()const {return m_map;}
    const std::string& get_name() const {return m_name;}
    void insert_tile(ROBtek::tile tile);
    int get_known_terain_size(){return m_terrains.size();} 
    void add_terrain(ROBtek::terrrain terrain);
    void print_known_terains();


    private:
    virtual void _path_planning();
    virtual void _reconstruct_closest_route();
    virtual int _get_slope_to_goal();
    virtual void _inspect_adjacent_tiles();
    ROBtek::tile _determine_tile(ROBtek::coords coordinate);
    

    int m_walked_distance;
    ROBtek::tile adjacent_tiles[4];
    int m_mline;
    int m_current_distance_from_goal;
    int m_tile_size_x, m_tile_size_y;
    const std::string& m_name;
    ROBtek::coords m_position;
    std::unordered_map<uint, std::pair<double,bool>> m_terrains;
    cv::Mat m_map;


};