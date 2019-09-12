#include <unistd.h>
#include <vector>
#include <string>
#include <iostream>

#include <opencv2/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "kernel.h"

void draw_test();
void draw_pixel(cv::Mat& img, cv::Point pos, cv::Vec3b color);
void draw_line(cv::Mat& img, cv::Point2i begin, cv::Point2i end, cv::Vec3b color);
void draw_img(cv::Mat& img, cv::Point cur_pos);