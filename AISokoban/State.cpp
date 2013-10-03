#include "State.h"


State::State(std::vector<std::string>* map, std::string path, State* parent, std::vector<std::pair<int,int>> boxes, std::pair<int,int> player) : map(map), path(path), parent(parent), boxes(boxes), player(player)
{
	// Think of pathLength more of a total heuristic value, as we aren't interested in the shortest path, only in the one 
	// that leads us to the goal the fastest, i.e. lowest total heuristic value (fewest box-moves?)

	pathLength = this->getHeuristicValue() + (parent == NULL ? 0 : parent->getHeuristicValue());
	
	//pathLength = path.size() + (parent == NULL ? 0 : parent->getPathLength());
}


State::~State(void)
{
}

// IMPORTANT
// I think we shouldn't compare the player position, instead we should compare if the movable area is identical
// So if in several states the position of the boxes is identical but the states are reached by pushing different boxes
// the state aren't regarded as identical even if the same child states could be reached

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
	// TODO: Needs to be efficient, as this is checked every expansion
	// Shouldn't the heurisitc value be 0 if we just consider the distance of each box to a goal?
	
	return this->getHeuristicValue() == 0;
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
	return parent == NULL ? path : parent->getPath().append(path);
}

State* State::getParent()
{
	//needed ?
	return parent;

}

int State::getPathLength()
{
	return pathLength;

	// Should be computed just one time as this doesn't change during search
	//return parent->getPathLength()+path.size();
}
