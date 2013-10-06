#include "State.h"


State::State(std::vector<std::string>* map, std::string path, State* parent, std::set<std::pair<int,int>> boxes, std::pair<int,int> player) : map(map), path(path), parent(parent), boxes(boxes), player(player)
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

	for(auto it=boxes.begin();it!=boxes.end();it++)
	{
		if(b.boxes.find(*it) == b.boxes.end()) return false;
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
	for(auto it=boxes.begin();it!=boxes.end();it++)
	{
		// Box in corner
		if((*map)[it->second][it->first-1] == '#')
		{
			if((*map)[it->second-1][it->first] == '#' || (*map)[it->second+1][it->first] == '#') return true;
		}
		if((*map)[it->second][it->first+1] == '#')
		{
			if((*map)[it->second-1][it->first] == '#' || (*map)[it->second+1][it->first] == '#') return true;
		}
	}

	return false;
}

bool State::isWin()
{
	// TODO: Needs to be efficient, as this is checked every expansion
	// Shouldn't the heurisitc value be 0 for a winning state if we just consider the distance of each box to a goal?
	
	return this->getHeuristicValue() == 0;

	// Alternative 

	for(unsigned int i=0;i<Constants::Goals.size();i++) 
	{
		if(this->boxes.find(Constants::Goals[i]) == this->boxes.end()) return false;
	}
	
	return true;
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
	int i=0;
	for(auto it=boxes.begin();i<count;it++,i++)
	{
		hash = 65599 * hash + (*it).first+((*it).second*3);
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
