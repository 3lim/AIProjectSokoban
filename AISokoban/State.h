#pragma once
#include <vector>
#include <list>
class State
{
public:
	State(std::vector<std::string>* map, std::string path, State* parent, std::vector<std::pair<int,int>> boxes, std::pair<int,int> player);
	~State(void);
	std::vector<State> getChildStates();
	bool isLocked();
	int getHeuristicValue();
	int getHash();
	std::string getPath();
	State* getParent();
	int getPathLength();
private:
	std::vector<std::pair<int,int>> boxes;
	std::pair<int,int> player;
	std::string path;
	State* parent;
	std::vector<std::string>* map;
	int pathLength;
};

