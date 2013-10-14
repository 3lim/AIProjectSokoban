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
    
    //each element of the vector distancesToGoal is a map of int. Each int express the distance to a specific goal. There is a map of distances for each goal
    std::vector<std::vector<std::vector<int>>> distancesToGoal = findDistancesToGoal(board, goals);
    
    //print of the maps containing the distances to the goals
    /*for (int i = 0; i < distancesToGoal.size(); i++)
    {
        printDistancesMap(distancesToGoal[i]);
    }*/
    
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

std::vector<std::vector<std::vector<int>>> findDistancesToGoal(std::vector<std::string> const board, std::set<std::pair<int, int>> const goals)
{
    //each element of the vector distancesToGoal is a map of int. Each int express the distance to the goal. There is a map of distances in distancesToGoal for each goal
    std::vector<std::vector<std::vector<int>>> distancesToGoal;
    
    //set containing the goals from where to start the BFS/flood search
    std::set<std::pair<int, int>> startList= goals;
    
    //the frontier containing the next slot to explore
    std::queue<Node> frontier;
    
    while (startList.size() != 0)
    {
        //BFS Search
        
        //map of bool: every visited position is marked true once visited
        std::vector<std::vector<bool>> visitedStates;
        
        //map of int: every visited position is marked with its distances to goal value
        std::vector<std::vector<int>> mapDistances;
        
        Node start = {*startList.begin(),""};
        
       // std::cout << "(" << start.node.first << "," << start.node.second << ")" << std::endl;
        
        frontier.push(start);
        
        //initialization of the visitedStates map and distances map
        for (unsigned int line = 0; line < board.size(); line++)
        {
            std::vector<bool> visitedStatesLine;
            std::vector<int> mapDistancesLine;
            
            for (unsigned int column= 0; column < board[line].size()-1; column++)
            {
                visitedStatesLine.push_back(0);
                mapDistancesLine.push_back(0); //ATTENTION WALL HAVE A VALUE OF 0!
            }
            
            visitedStates.push_back(visitedStatesLine);
            mapDistances.push_back(mapDistancesLine);
        }
        
        while (frontier.size() != 0)
        {
            Node observedNode = frontier.front();
            frontier.pop();
            
            if (board[observedNode.node.second][observedNode.node.first]!='#' && visitedStates[observedNode.node.second][observedNode.node.first]!=1)
            {
                //the length of the path travelled from the goal to the map position is written in mapDistances
                mapDistances[observedNode.node.second][observedNode.node.first] = observedNode.path.length();
                
                //UP CASE
                std::pair<int,int> upNodePosition(observedNode.node.first,observedNode.node.second-1);
                Node upNode = {upNodePosition,observedNode.path+'U'};
                frontier.push(upNode);
                
                //RIGHT CASE
                std::pair<int,int> rightPosition(observedNode.node.first+1,observedNode.node.second);
                Node rightNode = {rightPosition,observedNode.path+'R'};
                frontier.push(rightNode);
                
                //DOWN CASE
                std::pair<int,int> downPosition(observedNode.node.first,observedNode.node.second+1);
                Node downNode = {downPosition,observedNode.path+'D'};
                frontier.push(downNode);
                
                //LEFT CASE
                std::pair<int,int> leftPosition(observedNode.node.first-1,observedNode.node.second);
                Node leftNode = {leftPosition,observedNode.path+'L'};
                frontier.push(leftNode);
            }
            
            visitedStates[observedNode.node.second][observedNode.node.first] = 1;
        }
        
        startList.erase(startList.begin());
        
        //the map is pushed to the vector distancesToGoal
        distancesToGoal.push_back(mapDistances);
    }
    
    return distancesToGoal;
};

//debugging print function for map of integer
void printDistancesMap(std::vector<std::vector<int>> map)
{
    for (int i = 0; i < map.size(); i++)
    {
        for (int j = 0; j < map[i].size(); j++)
        {
            std::cout << " " << map[i][j] << " ";
        }
        
        std::cout << std::endl;
    }
    
    std::cout << std::endl;
    std::cout << std::endl;
}
