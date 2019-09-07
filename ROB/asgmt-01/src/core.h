#pragma once

#include <vector>
#include <string>
#include <iostream>

#include <opencv2/core.hpp>
#include <opencv2/highgui/highgui.hpp>

struct coords
{
	int x, y;
};

struct path
{
	coords begin;
	coords end;
	std::vector<coords> path;
};

void test();