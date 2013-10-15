// AISokoban.cpp : Definiert den Einstiegspunkt für die Konsolenanwendung.
//
#include "State.h"
#include "Constants.h"
#include <queue>
#include <iostream>
#include <unordered_set>
#include <set>
#include <string>
#include <map>

typedef struct {
	std::pair<int,int> pos;
	int origin;
	int dist;
} square_t;

namespace std {
  template <>
  struct hash<State*>
  {
	std::size_t operator()(State* const& state) const
	{
	  return state->getHash();
	}
  };

  template <> struct equal_to<State*> : binary_function <State*,State*,bool> {
	  bool operator() (State* const & x,  State* const & y) const 
	  {
		  return (*x)==(*y);
	  }
	};
}

class comparison
{
public:
  bool operator() (State* const &  lhs, State* const & rhs) const
  {
	 // std::cout << rhs->getHeuristicValue() <<lhs->getHeuristicValue() << std::endl;
	  return lhs->getHeuristicValue()+lhs->getPathLength()>rhs->getHeuristicValue()+rhs->getPathLength();
  }
};

//function that creates a vector of maps containing integers which have the value of the distances to the goals. one map per goal.
std::vector<std::vector<std::vector<int>>> findDistancesToGoal(std::vector<std::string> const board, std::set<std::pair<int, int>> const goals);

//function that prints a map of integer. used for deubbging.
void printDistancesMap(std::vector<std::vector<int>> map);


bool moveable(char c)
{
	return c != '#';
}

// Compute data for all Positions from where a box can be pushed to any goal
// Data consists of a map from position to goal id to shortest distance to that goal

void computePushablePositionData(std::set<std::pair<int,int>> goals, std::vector<std::string>& map, std::map<std::pair<int,int>,std::map<int,int>>& positions)
{
	std::queue<square_t> q;
	std::set<std::pair<int,std::pair<int,int>>> visited;
	int i=0;
	for(auto it = goals.begin();it != goals.end();it++,i++) 
	{
		square_t n = {*it,i,0};
		q.push(n);
	}

	while(!q.empty())
	{
		square_t node = q.front();
		std::pair<int,int> pos = node.pos;
		q.pop();
		
		if(visited.find(std::pair<int,std::pair<int,int>>(node.origin,pos)) != visited.end()) 
		{
			int* dist = &positions[node.pos][node.origin];

			if(*dist<=node.dist) continue;
			else *dist = node.dist;
		}
		else
		{
			visited.insert(std::pair<int,std::pair<int,int>>(node.origin,pos));
		}

		bool pushable = false;
		
		if(moveable(map[pos.second-1][pos.first]) && moveable(map[pos.second-2][pos.first])) 
		{
			pushable = true;
			square_t n = {std::pair<int,int>(pos.first,pos.second-1),node.origin,node.dist+1};
			q.push(n);
		}
		if(moveable(map[pos.second+1][pos.first]) && moveable(map[pos.second+2][pos.first])) 
		{
			pushable = true;
			square_t n = {std::pair<int,int>(pos.first,pos.second+1),node.origin,node.dist+1};
			q.push(n);
		}
		if(moveable(map[pos.second][pos.first-1]) && moveable(map[pos.second][pos.first-2])) 
		{
			pushable = true;
			square_t n = {std::pair<int,int>(pos.first-1,pos.second),node.origin,node.dist+1};
			q.push(n);
		}
		if(moveable(map[pos.second][pos.first+1]) && moveable(map[pos.second][pos.first+2])) 
		{
			pushable = true;
			square_t n = {std::pair<int,int>(pos.first+1,pos.second),node.origin,node.dist+1};
			q.push(n);
		}

		if(pushable) positions[pos][node.origin] = node.dist;
	}
}

int main(void)
{
	std::vector<std::string> board;
	for (std::string line; std::getline(std::cin, line);)
		board.push_back(line);
		
	std::set<std::pair<int,int>> boxes;
	std::set<std::pair<int,int>> goals;
	std::pair<int,int> player;
	for(unsigned int y = 0; y < board.size(); y++)
	{
		for(unsigned int x = 0; x < board[y].size(); x++)
		{
			std::pair<int,int> pos(x,y);
			switch(board[y][x])
			{
				case '@': player = pos;
						  board[y][x] = ' ';
						  break;
				case '.': goals.insert(pos);
						  break;
				case '+': player = pos;
						  goals.insert(pos);
						  board[y][x] = '.';
						  break;
				case '$': boxes.insert(pos);
						  board[y][x] = ' ';
						  break;
				case '*': boxes.insert(pos);
						  goals.insert(pos);
						  board[y][x] = '.';
						  break;
			}
		}
	}
	
	computePushablePositionData(goals,board,Constants::pushablePositions);

	State* initState = new State(&board,"",NULL,boxes,player);
	Constants::Goals = goals;
	
	State* endState = NULL;

	//list of seen, but not not expanded states
	std::priority_queue<State*,std::vector<State*>,comparison> currentStates;

	//list of all seen states

	std::unordered_set<State*> visitedStates;

	//currentStates.push_back(initState);
	currentStates.push(initState);
	visitedStates.insert(initState);
	while(!currentStates.empty() && endState==NULL)
	{
		//selecting the best state and removing it from the list.
		State* state = currentStates.top();

		currentStates.pop();
		
		visitedStates.insert(state);

		std::vector<State*> childs  = state->getChildStates();

		std::vector<State*>::iterator it;
		//looping through all childs of the selected state.
		for(it=childs.begin();it!=childs.end();it++)
		{
			//check if state is win
			if((*it)->isWin())
			{
				//std::cout<<"win"<< std::endl;
				endState=(*it);
				break;
			}

			//check if state already visited;
			std::unordered_set<State*>::const_iterator got = visitedStates.find (*it);
			if(got!=visitedStates.end())// state already seen
			{
				//std::cout<<"already visited (" << (*got)->getHeuristicValue() << ")"<< std::endl;
				delete (*it);
				// TODO: delete useless state.
			}
			else
			{
				//std::cout<<"adding a state"<< std::endl;
				//check if lock state
				if(!(*it)->isLocked()) currentStates.push(*it);
			}
		}
		
		//std::cout << currentStates.size() << std::endl;
		
	}
	
	// Output answer
	if(endState==NULL)
	{
		std::cout << "no path" << std::endl;
	}
	else
	{
		std::cout << endState->getPath() << std::endl;
	}
	
	return 0;
}
