#include "State.h"


State::State(std::vector<std::string>* map, std::string path, State* parent, std::vector<std::pair<int,int>> boxes, std::pair<int,int> player) : map(map), path(path), parent(parent), boxes(boxes), player(player)
{
	pathLength = path.size() + parent->getPathLength();
}


State::~State(void)
{
}
