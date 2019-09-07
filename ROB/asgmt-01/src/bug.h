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
        << "\nTerrain weight: "<< this->weight 
        << "\nTerrain walkable? "<< std::boolalpha <<this->walkable << std::endl;  }
    };

    struct tile
    {
        terrrain tile_t;
        struct size
        {
            int x,y;
        };
    };

};
struct coords
{
    int x, y;
};






class Bug
{
    public:
    Bug(const std::string& name,cv::Mat& map, ROBtek::terrrain start_terrain, ROBtek::terrrain goal_terrain);
    
    
    int get_walked_distance(){ return m_walked_distance;}
    const cv::Mat& get_map()const {return m_map;}
    const std::string& get_name() const {return m_name;}
    void insert_terrain(ROBtek::terrrain terrain, coords coordinate);
    int get_known_terain_size(){return m_terrains.size();} 
    void add_terrain(ROBtek::terrrain terrain);
    void print_known_terains();


    private:
    virtual void _path_planning();
    virtual void _reconstruct_closest_route();
    virtual int _get_slope_to_goal();
    virtual void _inspect_adjacent_terrain();

    int m_walked_distance;
    ROBtek::terrrain adjacent_terrain[4];
    int m_mline;
    int m_current_distance_from_goal;
    const std::string& m_name;
    coords m_coordinate;
    std::unordered_map<uint, std::pair<double,bool>> m_terrains;
    cv::Mat m_map;


};