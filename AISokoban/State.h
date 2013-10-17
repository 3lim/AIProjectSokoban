#pragma once
#include <vector>
#include <list>
#include <set>
#include <string>
#include "DeadlockTable.h"
#include "Constants.h"

typedef std::pair<std::pair<int,int>,std::vector<std::pair<int,int>>> Position;

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
	long getHash() const;
	std::string getPath();
	State* getParent();
	int getPathLength();
	void print();
	static std::vector<std::pair<int,int>> goals;
private:
	int matchingHeuristic();
	int minPushableHeuristic();
	int minManhattanHeuristic();
	std::set<std::pair<int,int>> boxes;
	std::pair<int,int> player;
	std::string path;
	State* parent;
	std::vector<std::string>* map;
	int pathLength;
	int heuristicValue;
	long hash;
	std::pair<int,int> upperLeftReachable;
	bool matchingFound;
	
	// For debugging
	std::vector<std::pair<int,int>> boxGoalMatching;
	std::vector<Position> sortedBoxes;
	// -
};

struct StateNode
{
	std::pair<int,int> node;
	std::string path; // path travelled to reach this node
};