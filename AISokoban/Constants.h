#pragma once
#include <set>
#include <map>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

#define FREE_SPACE ' '
#define WALL '#'
#define GOAL '.'
#define PLAYER '@'
#define PLAYER_GOAL '+'
#define BOX '$'
#define BOX_GOAL '*'
#define DT_W 3
#define DT_H 3

class Constants
{
public:
	static std::set<std::pair<int,int>> Goals;
	static std::map<std::pair<int,int>,std::map<int,int>> pushablePositions;
	static std::set<std::string> deadlockTable;
	static std::vector<int> gridPositions;
	static void printPos(std::string p,int w);
	static std::pair<int,int> computeNextCircularPosition(int lX,int lY,int pPos);
};
