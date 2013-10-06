#pragma once
#include <vector>

#define FREE_SPACE ' '
#define WALL '#'
#define GOAL '.'
#define PLAYER '@'
#define PLAYER_GOAL '+'
#define BOX '$'
#define BOX_GOAL '*'

class Constants
{
public:
	static std::vector<std::pair<int,int>> Goals;
};
