#include "State.h"
#include <string>
#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include "Constants.h"
using namespace std;


bool SortPair(std::pair<int,int> a,std::pair<int,int> b)
{
	if(a.first!=b.first){
		return a.first<b.first;
	}else{
		return a.second<b.second;
	}
}

State::State(std::vector<std::string>* map, std::string path, State* parent, std::set<std::pair<int,int>> boxes, std::pair<int,int> player) : map(map), path(path), parent(parent), boxes(boxes), player(player)
{

	pathLength =  (parent == NULL ? 0 : parent->getPathLength()+1);
	heuristicValue = -1;

	
	//Computing hash

	// initialise the upper left point with player coordinates.
	pair<int,int> min =player;

	//retrieving the upper left point of the player's area.

	vector<vector<bool>> visitedSpot;
	for (unsigned int line = 0; line < map->size(); line++)
	{
		vector<bool> visitedSpotLine;
		
		for (unsigned int column= 0; column < (*map)[line].size()-1; column++)
		{
			visitedSpotLine.push_back(0);
		}
		
		visitedSpot.push_back(visitedSpotLine);
	}
	visitedSpot[player.second][player.first] = 1;
	queue<pair<int,int>> spots;
	spots.push(player);
	int a=0;
	while (!spots.empty())
	{
		//cout << "visiting " <<a++<<endl;
		pair<int,int> observedSpot = spots.front();
		spots.pop();
		list<pair<int,int>> nexts;
		nexts.push_back(make_pair(observedSpot.first,observedSpot.second+1));
		nexts.push_back(make_pair(observedSpot.first,observedSpot.second-1));
		nexts.push_back(make_pair(observedSpot.first+1,observedSpot.second));
		nexts.push_back(make_pair(observedSpot.first-1,observedSpot.second));
		for(list<pair<int,int>>::iterator it=nexts.begin();it!=nexts.end();it++){
			if(!visitedSpot[it->second][it->first] && (*map)[it->second][it->first]!=WALL && boxes.find(*it) == boxes.end()){
				visitedSpot[it->second][it->first] = 1;
				spots.push((*it));
				if(min.second>it->second || (min.second==it->second && min.first>it->first)){
					min = (*it);
				}
			}
		}
	}
	//*/
	int count = boxes.size();
	hash = min.first+(min.second*29);
	int i=0;
	
	vector<pair<int,int>> sortedBoxes;
	for(auto it=boxes.begin();i<count;it++,i++)
	{
		sortedBoxes.push_back(*it);
	}
	std::sort(sortedBoxes.begin(),sortedBoxes.end(),SortPair);
	for(int j=0;j<count;j++){
		hash = 65599 * hash + sortedBoxes[j].first+(sortedBoxes[j].second*29);
	}
	/*

	for(auto it=boxes.begin();i<count;it++,i++)
	{

		hash = 65599 * hash + (*it).first+((*it).second*29);
	}*/
	upperLeftReachable = min;
}


State::~State(void)
{
}

bool State::operator == (const State &b) const
{
	/*if(hash != b.hash){
		return false;
	}*/

	return upperLeftReachable==b.upperLeftReachable && b.boxes == boxes;
}

std::vector<State*> State::getChildStates()
{
	std::vector<State*> children;
	int numberOfChild = 0; //contains the number of children generated. can be print out at the end.
	
	//node x coordinate (first in node struct) is the line, is vertical, go from top to bottom
	//node y coordinate (second in node struct) is the column, is horizontal, go from left to right
	
	
	// Start of the search
	StateNode start = {player,""};

	
	queue<StateNode> frontier;
	
	frontier.push(start);
	
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
		StateNode observedNode = frontier.front();
		frontier.pop();
		
		//keeping track of the upper left point visited to compute hash later on.
		

		if ((*map)[observedNode.node.second][observedNode.node.first]!='#' && boxes.find(observedNode.node) == boxes.end() && visitedStates[observedNode.node.second][observedNode.node.first]!=1)
		{
			//UP CASE
			pair<int,int> upNodePosition(observedNode.node.first,observedNode.node.second-1);
			StateNode upNode = {upNodePosition,observedNode.path+'U'};
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
			frontier.push(upNode);
			
			//RIGHT CASE
			pair<int,int> rightPosition(observedNode.node.first+1,observedNode.node.second);
			StateNode rightNode = {rightPosition,observedNode.path+'R'};
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
			frontier.push(rightNode);
			
			//DOWN CASE
			pair<int,int> downPosition(observedNode.node.first,observedNode.node.second+1);
			StateNode downNode = {downPosition,observedNode.path+'D'};
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
			frontier.push(downNode);
			
			//LEFT CASE
			pair<int,int> leftPosition(observedNode.node.first-1,observedNode.node.second);
			StateNode leftNode = {leftPosition,observedNode.path+'L'};
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
			frontier.push(leftNode);
			
		}
		
		visitedStates[observedNode.node.second][observedNode.node.first] = 1;
	}
	
	//cout << "number of child states "<< numberOfChild << endl;

	return children;
}

bool goalDeadlockCheck(std::string s)
{
	int p = s.find_first_of('*');

	if(p != s.npos)
	{
		std::string n(s);
		n[p] = '$';
		bool boxDl = goalDeadlockCheck(n);
		n[p] = '#';
		bool wallDl = goalDeadlockCheck(n);

		return boxDl && wallDl;
	}
	else
	{
		return Constants::deadlockTable.find(s) != Constants::deadlockTable.end();
	}
}

bool State::isLocked()
{
	// Pick moved box
	std::pair<int,int> movedBoxPos;
	int xX,yY,xY,yX;
	switch(path.back())
	{
	case 'U': movedBoxPos = std::pair<int,int>(player.first,player.second-1);
		xX = -1;
		yY = -1;
		xY = 0;
		yX = 0;
		break;
	case 'D': movedBoxPos = std::pair<int,int>(player.first,player.second+1);
		xX = 1;
		yY = 1;
		xY = 0;
		yX = 0;
		break;
	case 'L': movedBoxPos = std::pair<int,int>(player.first-1,player.second);
		xX = 0;
		yY = 0;
		xY = 1;
		yX = -1;
		break;
	case 'R': movedBoxPos = std::pair<int,int>(player.first+1,player.second);
		xX = 0;
		yY = 0;
		xY = -1;
		yX = 1;
		break;
	}
	
	
	if(Constants::Goals.find(movedBoxPos)!=Constants::Goals.end()) return false;

	// State is locked if pushed box is in unpushable position (can't reach any goal)
	if(Constants::pushablePositions.find(movedBoxPos) == Constants::pushablePositions.end()) return true;

	std::string dlCheck(DT_H * DT_W,' ');
	
	int xDiff = (DT_W % 2 == 0 ? DT_W : DT_W-1) / 2;
	int yDiff = DT_H;
	int j = 0;
	for(int y = 0; y < yDiff; y++)
	{
		for(int x = - xDiff; x <= xDiff; x++)
		{
			int newY = player.second + y * yY + x * xY;
			int newX = player.first + y * yX + x * xX;
			dlCheck[j] = (newY < 0 || newY >= (*map).size() || newX < 0 || newX >= (*map)[newY].length()) ? ' ' : (*map)[newY][newX];
			
			if(boxes.find(make_pair(newX,newY)) != boxes.end()) dlCheck[j] = Constants::Goals.find(make_pair(newX,newY)) != Constants::Goals.end() ? '*' : '$';
			if(y == 0 && x == 0) dlCheck[j] = '@';
			if(dlCheck[j] == BOX_GOAL) dlCheck[j] = '*';
			if(dlCheck[j] == PLAYER_GOAL) dlCheck[j] = '@';
			if(dlCheck[j] == GOAL) dlCheck[j] = ' ';
			j++;
		}
	}

	std::string dlCheckReal(DT_H * DT_W,' ');
	dlCheckReal[xDiff] = dlCheck[xDiff];
	dlCheckReal[xDiff + DT_W] = dlCheck[xDiff + DT_W];

	std::pair<std::string,int> start(dlCheckReal,0);
	std::queue<std::pair<std::string,int>> dlQ;

	dlQ.push(start);
	while(!dlQ.empty())
	{
		std::pair<std::string,int> dl = dlQ.front();
		dlQ.pop();

		if(goalDeadlockCheck(dl.first)) 
		{
					/*std::cout << path.back() << "->" << movedBoxPos.first << "," << movedBoxPos.second << ": Found deadlock!" << std::endl;
					Constants::printPos(dl.first,DT_W);
					Constants::printPos(dlCheck,DT_W);*/
			return true;
		}

		if(dl.second >= Constants::gridPositions.size()) continue;

		if(dl.first[Constants::gridPositions[dl.second]] == ' ') 
		{
			std::pair<std::string,int> ndl(dl.first,dl.second+1);

			ndl.first[Constants::gridPositions[dl.second]] = dlCheck[Constants::gridPositions[dl.second]];
			dlQ.push(ndl);
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

bool SortBoxes(Position a,Position b)
{
	return a.second.size()<b.second.size();
}


bool findMatching(int i,const std::vector<Position>& pBoxes,std::vector<int>& matching)
{
	if(i>=pBoxes.size()) return true;

	for(std::pair<int,int> reachable:pBoxes[i].second)
	{
		int j=0;
		for(j=0;j<i;j++) if(matching[j] == reachable.first) break;

		if(j!=i) continue;

		matching[i] = reachable.first;

		if(findMatching(i+1,pBoxes,matching)) return true;
	}
	return false;
}

bool SortDistances(std::pair<int,int> a,std::pair<int,int> b)
{
	return a.second<b.second;
}

int State::getHeuristicValue()
{
	if(heuristicValue != -1) return heuristicValue;

	//std::cout << heuristicValue << std::endl;
	return heuristicValue = matchingHeuristic();
}

int State::minManhattanHeuristic()
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

int State::minPushableHeuristic()
{
	int ret = 0;
		for(auto it=boxes.begin();it!=boxes.end();it++){
		int min = -1;
		for(auto p:Constants::pushablePositions[*it])
		{
			if(min == -1) 
			{
				min = p.second;
			}
			else 
			{
				min = p.second < min ? p.second : min;
			}
		}
		ret += min;
	}
		return ret;
}

int State::matchingHeuristic()
{
	int ret = 0;
	
	std::vector<Position> pBoxes;

	for(auto b:boxes)
	{
		std::vector<std::pair<int,int>> distances;

		for(std::pair<int,int> r:Constants::pushablePositions[b]) distances.push_back(r);

		sort(distances.begin(),distances.end(),SortDistances);

		pBoxes.push_back(make_pair(b,distances));
	}

	std::sort(pBoxes.begin(),pBoxes.end(),SortBoxes);

	std::vector<int> matching;

	for(int i=0;i<pBoxes.size();i++) matching.push_back(-1);

	bool m = findMatching(0,pBoxes,matching);
	
	//if(!m) std::cout << "No matching found!" << std::endl;

	

	for(int i=0;i<matching.size();i++) ret += Constants::pushablePositions[pBoxes[i].first][matching[i]];

	return ret;
}

int State::getHash() const
{
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
	int count =boxes.size();
	int i=0;
	vector<pair<int,int>> sortedBoxes;
	for(auto it=boxes.begin();i<count;it++,i++)
	{
		sortedBoxes.push_back(*it);
	}
	/*std::cout<<upperLeftReachable.first<< " "<<upperLeftReachable.second<<endl;
	std::cout<<"--------------------"<<endl;;
	std::sort(sortedBoxes.begin(),sortedBoxes.end(),SortPair);
	for(int j=0;j<count;j++){
		std::cout<<sortedBoxes[j].first<< " "<<sortedBoxes[j].second<<endl;
	}
	std::cout<<"--------------------"<<endl;
	int h = upperLeftReachable.first+(upperLeftReachable.second*29);
	for(int j=0;j<count;j++){
		std::cout<<h<<endl;
		h = 65599 * h + sortedBoxes[j].first+(sortedBoxes[j].second*29);
		//h =( 65599 * h + sortedBoxes[j].first+(sortedBoxes[j].second*29))%3000;
	}
	std::cout<<h<<endl;
	std::cout<<"--------------------"<<endl;*/
}
