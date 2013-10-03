#include "State.h"


State::State(std::vector<std::string>* map, std::string path, State* parent, std::vector<std::pair<int,int>> boxes, std::pair<int,int> player) : map(map), path(path), parent(parent), boxes(boxes), player(player)
{
	pathLength = path.size() + parent->getPathLength();
}


State::~State(void)
{
}

bool State::operator == (const State &b) const
{
	//useless ?
	/*if(b.boxes.size()!=boxes.size()){
		return false;
	}*/

	for(unsigned int i=0;i<boxes.size();i++)
	{
		if(boxes[i].first!=b.boxes[i].first || boxes[i].second!=b.boxes[i].second){
			return false;
		}
	}

	if(player.first!=b.player.first || player.second!=b.player.second){
		return false;
	}

	return true;
}

std::vector<State*> State::getChildStates()
{
	std::vector<State*> childs;
	//TODO: implement me
	return childs;
}

bool State::isLocked()
{
	//TODO: implement me
	return false;
}

bool State::isWin()
{
	//TODO: implement me
	return false;
}

int State::getHeuristicValue()
{
	//TODO: implement me
	return 0;
}

int State::getHash()
{
	int count = boxes.size()<15 ? boxes.size() : 15; // do not take into account more than 15 box so make hash computation faster.
	int hash = player.first+(player.second*3);
	for(int i=0;i<count;i++)
	{
		hash = 65599 * hash + boxes[i].first+(boxes[i].second*3);
	}
	return hash;
}

std::string State::getPath()
{
	return parent->getPath().append(path);
}

State* State::getParent()
{
	//needed ?
	return parent;

}

int State::getPathLength()
{
	return parent->getPathLength()+path.size();
}
