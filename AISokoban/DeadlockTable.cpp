#include <iostream>
#include <vector>
#include <set>
#include <queue>
#include <sstream>
#include "DeadlockTable.h"

struct DeadlockNode
{
	std::string pos;
	int numBoxes;
};

class CompareDeadlockNodeQueue
{
public:
  bool operator() (const DeadlockNode& a, const DeadlockNode& b) const
  {
	  return a.numBoxes>b.numBoxes;
  }
};

std::vector<int> DeadlockTable::generateCircularPositions()
{
	std::vector<int> res;
	int genPositions = 0;
	int lX,lY,pP;
	pP = lX = DT_W % 2 == 0 ? DT_W / 2 : (DT_W-1) / 2;
	lY = 1;
	while(genPositions < (DT_H-1) * DT_W - 1)
	{
		std::pair<int,int> pos = Constants::computeNextCircularPosition(lX,lY-1,pP);
		lX = pos.first;
		lY = pos.second+1;

		if(lX >= 0 && lX < DT_W && lY < DT_H)
		{
			genPositions++;
			res.push_back(lX + lY*DT_W);
		}
	}

	for(int i=0;i<DT_W;i++)
	{
		if(i!=pP) res.push_back(i);
	}

	return res;
}

bool DeadlockTable::isDeadlock(std::string dl, int w)
{
	std::stringstream ss;
	std::string wdl(w+1,' ');
	
	std::set<std::string> visited;

	std::priority_queue<DeadlockNode,std::vector<DeadlockNode>,CompareDeadlockNodeQueue> q;

	// Pad with spaces because of eventual walls
	ss << wdl;
	for(int i=0;i<dl.length();i+=w)
	{
		ss << "  " << dl.substr(i,w);
	}
	ss << "  " << wdl;
	dl = ss.str();
	w+=2;

	DeadlockNode node = {dl,0};
	q.push(node);

	int expanded = 0;
	while(!q.empty())
	{
		DeadlockNode p = q.top();
		q.pop();

		if(visited.find(p.pos) != visited.end()) continue;

		visited.insert(p.pos);

		if(p.pos.find('$') == p.pos.npos) return false;

		int pPos = p.pos.find('@');
		int pX = pPos % w;
		int pY = pPos / w;

		if(pX > 0)
		{
			if(p.pos[pPos-1] == ' ') 
			{
				DeadlockNode nNode;
				std::string n(p.pos);
				n[pPos-1] = '@';
				n[pPos] = ' ';
				nNode.pos = n;
				nNode.numBoxes = p.numBoxes;
				q.push(nNode);
			}
			else if(p.pos[pPos-1] == '$' && (pX == 1 || p.pos[pPos-2] == ' ')) 
			{
				DeadlockNode nNode;
				std::string n(p.pos);
				if(pX>2) n[pPos-2] = '$';
				n[pPos-1] = '@';
				n[pPos] = ' ';
				nNode.pos = n;
				nNode.numBoxes = pX <= 2 ? p.numBoxes-1 : p.numBoxes;
				q.push(nNode);
			}
		}
		if(pY > 0)
		{
			if(p.pos[(pY-1)*w + pX] == ' ') 
			{
				DeadlockNode nNode;
				std::string n(p.pos);
				n[(pY-1)*w + pX] = '@';
				n[pPos] = ' ';
				nNode.pos = n;
				nNode.numBoxes = p.numBoxes;
				q.push(nNode);
			}
			else if(p.pos[(pY-1)*w + pX] == '$' && (pY == 1 || p.pos[(pY-2)*w + pX] == ' ')) 
			{
				DeadlockNode nNode;
				std::string n(p.pos);
				if(pY>2) n[(pY-2)*w + pX] = '$';
				n[(pY-1)*w + pX] = '@';
				n[pPos] = ' ';
				nNode.pos = n;
				nNode.numBoxes = pY <= 2 ? p.numBoxes-1 : p.numBoxes;
				q.push(nNode);
			}
		}
		if(pX < w-1)
		{
			if(p.pos[pPos+1] == ' ') 
			{
				DeadlockNode nNode;
				std::string n(p.pos);
				n[pPos+1] = '@';
				n[pPos] = ' ';
				nNode.pos = n;
				nNode.numBoxes = p.numBoxes;
				q.push(nNode);
			}
			else if(p.pos[pPos+1] == '$' && (pX == w-2 || p.pos[pPos+2] == ' ')) 
			{
				DeadlockNode nNode;
				std::string n(p.pos);
				if(pX<w-3) n[pPos+2] = '$';
				n[pPos+1] = '@';
				n[pPos] = ' ';
				nNode.pos = n;
				nNode.numBoxes = pX >= w-3 ? p.numBoxes-1 : p.numBoxes;
				q.push(nNode);
			}
		}
		if(pY < p.pos.length()/w - 1)
		{
			if(p.pos[(pY+1)*w + pX] == ' ') 
			{
				DeadlockNode nNode;
				std::string n(p.pos);
				n[(pY+1)*w + pX] = '@';
				n[pPos] = ' ';
				nNode.pos = n;
				nNode.numBoxes = p.numBoxes;
				q.push(nNode);
			}
			else if(p.pos[(pY+1)*w + pX] == '$' && (pY == p.pos.length()/w - 2 || p.pos[(pY+2)*w + pX] == ' ')) 
			{
				DeadlockNode nNode;
				std::string n(p.pos);
				if(pY<p.pos.length()/w - 3) n[(pY+2)*w + pX] = '$';
				n[(pY+1)*w + pX] = '@';
				n[pPos] = ' ';
				nNode.pos = n;
				nNode.numBoxes = pX >= p.pos.length()/w - 3 ? p.numBoxes-1 : p.numBoxes;
				q.push(nNode);
			}
		}
	}

	return true;
}

struct Node
{
	std::string pos;
	int posToCompute;
};

void DeadlockTable::computeDeadlocks(int w, int h, std::set<std::string>& deadlocks)
{
	std::queue<Node> toCompute;
	std::string nStr(w*h,' ');
	
	nStr[w % 2 == 0 ? w / 2 : (w-1) / 2] = '@';
	nStr[w + (w % 2 == 0 ? w / 2 : (w-1) / 2)] = '$';
	
	Node n = {nStr,0};
	toCompute.push(n);

	int computed = 0;
	while(!toCompute.empty())
	{
		Node node = toCompute.front();
		toCompute.pop();

		computed++;

		if(DeadlockTable::isDeadlock(node.pos,w))
		{
			deadlocks.insert(node.pos);
			continue;
		}

		if(node.posToCompute >= Constants::gridPositions.size()) continue;

		Node newNode = {node.pos,node.posToCompute+1};
		toCompute.push(newNode);

		if(node.pos[Constants::gridPositions[node.posToCompute]] == ' ')
		{
			newNode.pos[Constants::gridPositions[node.posToCompute]] = '$';
			toCompute.push(newNode);
			newNode.pos[Constants::gridPositions[node.posToCompute]] = '#';
			toCompute.push(newNode);
		}
	}

	std::cout << "Computed " << deadlocks.size() << " deadlocks from " << computed << " positions" << std::endl;
}