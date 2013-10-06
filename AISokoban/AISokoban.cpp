// AISokoban.cpp : Definiert den Einstiegspunkt f�r die Konsolenanwendung.
//
#include "State.h"
#include "Constants.h"
#include <queue>
#include <iostream>
#include <unordered_set>
#include <set>
#include <string>

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
	  std::cout << rhs->getHeuristicValue() <<lhs->getHeuristicValue() << std::endl;
	  return lhs->getHeuristicValue()>rhs->getHeuristicValue();
  }
};

int main(void)
{
	std::vector<std::string> board;
	for (std::string line; std::getline(std::cin, line);)
		board.push_back(line);
		
	std::set<std::pair<int,int>> boxes;
	std::vector<std::pair<int,int>> goals;
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
				case '.': goals.push_back(pos);
						  break;
				case '+': player = pos;
						  goals.push_back(pos);
						  board[y][x] = '.';
						  break;
				case '$': boxes.insert(pos);
						  board[y][x] = ' ';
						  break;
				case '*': boxes.insert(pos);
						  goals.push_back(pos);
						  board[y][x] = '.';
						  break;
			}
		}
	}

	State* initState = new State(&board,"",NULL,boxes,player);
	Constants::Goals = goals;
	
	State* endState = NULL;

	//list of seen, but not not expanded states
	std::priority_queue<State*,std::vector<State*>,comparison> currentStates;

	//list of all seen states

	std::unordered_set<State*> visitedStates;

	//currentStates.push_back(initState);
	currentStates.push(initState);
	visitedStates.emplace(initState);
	while(!currentStates.empty() && endState==NULL)
	{
		//selecting the best state and removing it from the list.
		State* state = currentStates.top();

		std::cout << "expanding node : " <<state->getHeuristicValue() << std::endl;
		currentStates.pop();
		visitedStates.emplace(state);

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

				// TODO: delete useless state.
			}
			else
			{
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
