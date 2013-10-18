#include "Constants.h"

std::set<std::pair<int,int>> Constants::Goals;
std::map<std::pair<int,int>,std::map<int,int>> Constants::pushablePositions;
std::unordered_set<std::string> Constants::deadlockTable;
std::vector<int> Constants::gridPositions;
std::map<std::pair<int,int>,std::vector<Tunnel>> Constants::tunnels;
std::set<std::pair<std::pair<int,int>,bool>> Constants::tunnelP;

std::pair<int,int> Constants::computeNextCircularPosition(int lX,int lY,int pPos)
{
	if(lY == 0)
	{
		if(lX <= pPos)
		{
			return (pPos - lX) % 2 == 0 ? std::make_pair(lX-1,0) : std::make_pair(lX+1,1);
		}
		else
		{
			return (lX - pPos) % 2 == 1 ? std::make_pair(lX+1,0) : std::make_pair(lX-1,1);
		}
	}
	else if(lX == pPos)
	{
		return std::make_pair(lY % 2 == 1 ? lX + 1 : lX - 1,lY-1);
	}
	else
	{
		if(lX < pPos)
		{
			return ((pPos - lX) % 2 == 0 && lY % 2 == 1) || ((pPos - lX) % 2 == 1 && lY % 2 == 0) ? std::make_pair(lX+1,lY+1) : std::make_pair(lX-1,lY-1);
		}
		else
		{
			return ((lX - pPos) % 2 == 0 && lY % 2 == 0) || ((lX - pPos) % 2 == 1 && lY % 2 == 1) ? std::make_pair(lX-1,lY+1) : std::make_pair(lX+1,lY-1);
		}
	}
}

void Constants::printPos(std::string p, int w)
{
	std::cout << "-" << std::endl;
	for(int i=0;i<p.length();i+=w)
	{
		std::cout << p.substr(i,w) << "-" << std::endl;
	}
	std::cout << "-" << std::endl;
}