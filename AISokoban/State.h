#pragma once
#include <vector>
#include <list>
#include <set>
#include <string>
#include "Constants.h"

class State
{
public:
	State(std::vector<std::string>* map, std::string path, State* parent, std::set<std::pair<int,int>> boxes, std::pair<int,int> player);
	~State(void);
	bool operator == (const State &b) const;
	std::vector<State*> getChildStates();
	bool isLocked();
	bool isWin();
	int getHeuristicValue();
	int getHash();
	std::string getPath();
	State* getParent();
	int getPathLength();

	static std::vector<std::pair<int,int>> goals;
private:
	std::set<std::pair<int,int>> boxes;
	std::pair<int,int> player;
	std::string path;
	State* parent;
	std::vector<std::string>* map;
	int pathLength;
};

struct Node
{
    std::pair<int,int> node;
    std::string path; // path travelled to reach this node
};