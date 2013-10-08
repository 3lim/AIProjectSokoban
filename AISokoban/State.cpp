#include "State.h"
#include <string>
#include <iostream>
#include <vector>
using namespace std;


State::State(std::vector<std::string>* map, std::string path, State* parent, std::set<std::pair<int,int>> boxes, std::pair<int,int> player) : map(map), path(path), parent(parent), boxes(boxes), player(player)
{
	// Think of pathLength more of a total heuristic value, as we aren't interested in the shortest path, only in the one 
	// that leads us to the goal the fastest, i.e. lowest total heuristic value (fewest box-moves?)
	//pathLength = this->getHeuristicValue() + (parent == NULL ? 0 : parent->getHeuristicValue());
	pathLength =  (parent == NULL ? 0 : parent->getPathLength()+1);
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
	std::vector<State*> children;
	int numberOfChild = 0; //contains the number of children generated. can be print out at the end.
	
	//node x coordinate (first in node struct) is the line, is vertical, go from top to bottom
	//node y coordinate (second in node struct) is the column, is horizontal, go from left to right
	
	
	// Start of the search
	Node start = {player,""};
	
	vector<Node> frontier;
	
	frontier.push_back(start);
	
	//map of bool: every visited position is marked true once visited
	vector<vector<bool>> visitedStates;
	for (unsigned int line = 0; line < map->size(); line++)
	{
		vector<bool> visitedStatesLine;
		
		for (unsigned int column= 0; column < (*map)[line].size()-1; column++)
		{
			visitedStatesLine.push_back(0);
		}
		
		visitedStates.push_back(visitedStatesLine);
	}
	
	//cout << visitedStates.size() << endl;
	
	while (frontier.size() != 0)
	{
		
		//BFS SEARCH: first element of the list is taken
		Node observedNode = frontier.front();
		frontier.erase(frontier.begin());
		
		if ((*map)[observedNode.node.second][observedNode.node.first]!='#' && boxes.find(observedNode.node) == boxes.end() && visitedStates[observedNode.node.second][observedNode.node.first]!=1)
		{
			//UP CASE
			pair<int,int> upNodePosition(observedNode.node.first,observedNode.node.second-1);
			Node upNode = {upNodePosition,observedNode.path+'U'};
			pair<int, int> doubleUpPosition(observedNode.node.first,observedNode.node.second-2);
			if (boxes.find(upNode.node) != boxes.end() && (*map)[doubleUpPosition.second][doubleUpPosition.first] !='#' && boxes.find(doubleUpPosition) == boxes.end()) //If the slot above the player position contains a box, a child state is created where the box has been pushed up
			{
				pair<int,int> boxFound= upNode.node;
				set<pair<int,int>> newBoxes = boxes;
				newBoxes.erase(boxFound);
				newBoxes.insert(doubleUpPosition);
				State* child = 0;
				child = new State(map,upNode.path,this,newBoxes,boxFound);
				children.push_back(child);
			   // child->print();
				 /*cout << child->path << endl;
				cout << "play pos (" << boxFound.first << "," << boxFound.second <<")" << endl;
				set<pair<int,int>>::iterator itt;
				for (itt = newBoxes.begin(); itt != newBoxes.end(); itt++)
				{
					cout << "pos of box (" << (*itt).first <<","<< (*itt).second<<")" << endl;
				}*/
				numberOfChild++;
			}
			frontier.push_back(upNode);
			
			//RIGHT CASE
			pair<int,int> rightPosition(observedNode.node.first+1,observedNode.node.second);
			Node rightNode = {rightPosition,observedNode.path+'R'};
			pair<int, int> doubleRightPosition(observedNode.node.first+2,observedNode.node.second);
			if (boxes.find(rightNode.node) != boxes.end() && (*map)[doubleRightPosition.second][doubleRightPosition.first] !='#' && boxes.find(doubleRightPosition) == boxes.end()) //If the slot on the right of the player position contains a box and the move is possible a child state is created where the box has been pushed right
			{
				pair<int,int> boxFound = rightNode.node;
				set<pair<int,int>> newBoxes = boxes;
				newBoxes.erase(boxFound);
				newBoxes.insert(doubleRightPosition);
				State* child = 0;
				child = new State(map,rightNode.path,this,newBoxes,boxFound);
				children.push_back(child);
			   // child->print();
			   // cout << child->path << endl;
			   // cout << "play pos (" << boxFound.first << "," << boxFound.second <<")" << endl;
				/*set<pair<int,int>>::iterator itt;
				for (itt = newBoxes.begin(); itt != newBoxes.end(); itt++)
				{
					cout << "pos of box (" << (*itt).first <<","<< (*itt).second<<")" << endl;
				}*/
				numberOfChild++;
			}
			frontier.push_back(rightNode);
			
			//DOWN CASE
			pair<int,int> downPosition(observedNode.node.first,observedNode.node.second+1);
			Node downNode = {downPosition,observedNode.path+'D'};
			pair<int,int> doubleDownPosition(observedNode.node.first,observedNode.node.second+2);
			if (boxes.find(downPosition) != boxes.end() && (*map)[doubleDownPosition.second][doubleDownPosition.first] !='#' && boxes.find(doubleDownPosition) == boxes.end()) //If the slot on the right of the player position contains a box and the move is possible a child state is created where the box has been pushed right
			{
				pair<int,int> boxFound = downNode.node;
				set<pair<int,int>> newBoxes = boxes;
				newBoxes.erase(boxFound);
				newBoxes.insert(doubleDownPosition);
				State* child = 0;
				child = new State(map,downNode.path,this,newBoxes,boxFound);
				children.push_back(child);
			   // child->print();
			   /* cout << child->path << endl;
				cout << "play pos (" << boxFound.first << "," << boxFound.second <<")" << endl;
				set<pair<int,int>>::iterator itt;
				for (itt = newBoxes.begin(); itt != newBoxes.end(); itt++)
				{
					cout << "pos of box (" << (*itt).first <<","<< (*itt).second<<")" << endl;
				}*/
				numberOfChild++;
			}
			frontier.push_back(downNode);
			
			//LEFT CASE
			pair<int,int> leftPosition(observedNode.node.first-1,observedNode.node.second);
			Node leftNode = {leftPosition,observedNode.path+'L'};
			pair<int,int> doubleLeftPosition(observedNode.node.first-2,observedNode.node.second);
			if (boxes.find(leftPosition) != boxes.end() && (*map)[doubleLeftPosition.second][doubleLeftPosition.first] !='#' && boxes.find(doubleLeftPosition) == boxes.end()) //If the slot on the right of the player position contains a box and the move is possible a child state is created where the box has been pushed right
			{
			   // cout << "dat " << endl;
				pair<int,int> boxFound = leftNode.node;
				set<pair<int,int>> newBoxes = boxes;
				newBoxes.erase(boxFound);
				newBoxes.insert(doubleLeftPosition);
				State* child = 0;
				child = new State(map,leftNode.path,this,newBoxes,boxFound);
				children.push_back(child);
			  //  child->print();
			   /* cout << child->path << endl;
				cout << "play pos (" << boxFound.first << "," << boxFound.second <<")" << endl;
				set<pair<int,int>>::iterator itt;
				for (itt = newBoxes.begin(); itt != newBoxes.end(); itt++)
				{
					cout << "pos of box (" << (*itt).first <<","<< (*itt).second<<")" << endl;
				}*/
				numberOfChild++;
			}
			frontier.push_back(leftNode);
			
		}
		
		visitedStates[observedNode.node.second][observedNode.node.first] = 1;
	}
	
	//cout << "number of child states "<< numberOfChild << endl;
	return children;
}

bool State::isLocked()
{
	for(auto it=boxes.begin();it!=boxes.end();it++)
	{
		// Box on goal
		if(Constants::Goals.find(*it)!=Constants::Goals.end()) continue;
		
		bool blockedRight = (*map)[it->second][it->first+1] == '#';
		bool blockedLeft = (*map)[it->second][it->first-1] == '#';
		bool blockedUp = (*map)[it->second-1][it->first] == '#';
		bool blockedDown = (*map)[it->second+1][it->first] == '#';
		
		bool boxRight = boxes.find(std::pair<int,int>(it->first+1,it->second))!=boxes.end();
		bool boxLeft = boxes.find(std::pair<int,int>(it->first-1,it->second))!=boxes.end();
		bool boxUp = boxes.find(std::pair<int,int>(it->first,it->second-1))!=boxes.end();
		bool boxDown = boxes.find(std::pair<int,int>(it->first,it->second+1))!=boxes.end();

		// Box in corner
		/* #
		   $# */
		if(blockedLeft)
		{
			if(blockedUp || blockedDown) return true;
		}
		if(blockedRight)
		{
			if(blockedUp || blockedDown) return true;
		}

		if(boxRight)
		{
			// Box blocked by other box
			/* #$
			   #$ */
			if((blockedUp || blockedDown) && ((*map)[it->second-1][it->first+1] == '#' || (*map)[it->second+1][it->first+1] == '#')) return true;
			
			// 4 Boxes in square or with a wall
			/* $$     #$
			   $$     $$ */
			if((blockedDown || boxDown) && boxes.find(std::pair<int,int>(it->first+1,it->second+1))!=boxes.end()) return true;

			if((blockedUp || boxUp) && boxes.find(std::pair<int,int>(it->first+1,it->second-1))!=boxes.end()) return true;
		}

		if(boxLeft)
		{
			// Box blocked by other box
			/* #$
			   #$ */
			if((blockedUp || blockedDown) && ((*map)[it->second-1][it->first-1] == '#' || (*map)[it->second+1][it->first-1] == '#')) return true;

			// 4 Boxes in square or with a wall
			/* $$     #$
			   $$     $$ */
			if((blockedDown || boxDown) && boxes.find(std::pair<int,int>(it->first-1,it->second+1))!=boxes.end()) return true;

			if((blockedUp || boxUp) && boxes.find(std::pair<int,int>(it->first-1,it->second-1))!=boxes.end()) return true;
		}

		if(boxDown)
		{
			// Box blocked by other box
			/* #$
			   #$ */
			if((blockedLeft || blockedRight) && ((*map)[it->second+1][it->first+1] == '#' || (*map)[it->second+1][it->first-1] == '#')) return true;
		}
		
		if(boxUp)
		{
			// Box blocked by other box
			/* #$
			   #$ */
			if((blockedLeft || blockedRight) && ((*map)[it->second-1][it->first+1] == '#' || (*map)[it->second-1][it->first-1] == '#')) return true;
		}

	}

	return false;
}

bool State::isWin()
{
	// TODO: Needs to be efficient, as this is checked every expansion
	// Shouldn't the heurisitc value be 0 for a winning state if we just consider the distance of each box to a goal?
	
//	return this->getHeuristicValue() == 0;

	// Alternative 

	for(auto it=this->boxes.begin();it!=this->boxes.end();it++) 
	{
		if(Constants::Goals.find(*it) == Constants::Goals.end()) return false;
	}
	
	return true;
}

int State::getHeuristicValue()
{
	int ret=0;
	set<pair<int,int>>::iterator it;
	for(it=boxes.begin();it!=boxes.end();it++){
		pair<int,int> box = (*it);
		pair<int,int> goal  = *Constants::Goals.begin();
		int dist= abs(box.first-goal.first)+abs(box.second-goal.second);
		for(auto it=++Constants::Goals.begin();it!=Constants::Goals.end() && dist!=0;it++){
			goal = *it;
			dist = min(dist,abs(box.first-goal.first)+abs(box.second-goal.second));
		}
		ret+=dist;
	}
	return ret;
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
	//cout << "path " << path<<endl;
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

void State::print()
{
	for(unsigned int i=0;i<map->size();i++){
		for(unsigned int j=0;j<(*map)[i].size();j++){
			char c = (*map)[i][j];
			pair<int,int> currentPos(j,i);
			if(boxes.find(currentPos)!=boxes.end()){
				if(c==FREE_SPACE){
					c=BOX;
				}else if(c==GOAL){
					c=BOX_GOAL;
				}
			}else if( player.first==currentPos.first &&player.second==currentPos.second){
				if(c==FREE_SPACE){
					c=PLAYER;
				}else if(c==GOAL){
					c=PLAYER_GOAL;
				}
			}
			std::cout<<c;
		}
		std::cout<<endl;
	}
}
