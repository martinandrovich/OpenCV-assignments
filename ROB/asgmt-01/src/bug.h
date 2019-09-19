#pragma once

#include <unistd.h>
#include <vector>
#include <string>
#include <iostream>
#include <tuple>
#include <set>

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "draw.h"
#include "kernel.h"

void init_bug(cv::Mat& img);

void bug1(cv::Mat& img, cv::Point pos_start, cv::Point pos_target);
void bug1_old(cv::Mat& img, cv::Point pos_start, cv::Point pos_target);
