#include <iostream>
#include <vector>
#include <set>
#include <queue>
#include <sstream>
#include "DeadlockTable.h"

bool DeadlockTable::isDeadlock(std::string dl, int w)
{
	std::stringstream ss;
	std::string wdl(w+1,' ');
	
	std::set<std::string> visited;
	std::queue<std::string> q;

	// Pad with spaces because of eventual walls
	ss << wdl;
	for(int i=0;i<dl.length();i+=w)
	{
		ss << "  " << dl.substr(i,w);
	}
	ss << wdl;
	dl = ss.str();
	w+=2;

	q.push(dl);

	while(!q.empty())
	{
		std::string p = q.front();
		q.pop();

		if(visited.find(p) != visited.end()) continue;

		visited.insert(p);

		if(p.find('$') == p.npos) return false;

		int pPos = p.find('@');
		int pX = pPos % w;
		int pY = pPos / w;

		if(pX > 0)
		{
			if(p[pPos-1] == ' ') 
			{
				std::string n(p);
				n[pPos-1] = '@';
				n[pPos] = ' ';
				q.push(n);
			}
			else if(p[pPos-1] == '$' && (pX == 1 || p[pPos-2] == ' ')) 
			{
				std::string n(p);
				if(pX>2) n[pPos-2] = '$';
				n[pPos-1] = '@';
				n[pPos] = ' ';
				q.push(n);
			}
		}
		if(pY > 0)
		{
			if(p[(pY-1)*w + pX] == ' ') 
			{
				std::string n(p);
				n[(pY-1)*w + pX] = '@';
				n[pPos] = ' ';
				q.push(n);
			}
			else if(p[(pY-1)*w + pX] == '$' && (pY == 1 || p[(pY-2)*w + pX] == ' ')) 
			{
				std::string n(p);
				if(pY>2) n[(pY-2)*w + pX] = '$';
				n[(pY-1)*w + pX] = '@';
				n[pPos] = ' ';
				q.push(n);
			}
		}
		if(pX < w-1)
		{
			if(p[pPos+1] == ' ') 
			{
				std::string n(p);
				n[pPos+1] = '@';
				n[pPos] = ' ';
				q.push(n);
			}
			else if(p[pPos+1] == '$' && (pX == w-2 || p[pPos+2] == ' ')) 
			{
				std::string n(p);
				if(pX<w-3) n[pPos+2] = '$';
				n[pPos+1] = '@';
				n[pPos] = ' ';
				q.push(n);
			}
		}
		if(pY < p.length()/w - 1)
		{
			if(p[(pY+1)*w + pX] == ' ') 
			{
				std::string n(p);
				n[(pY+1)*w + pX] = '@';
				n[pPos] = ' ';
				q.push(n);
			}
			else if(p[(pY+1)*w + pX] == '$' && (pY == p.length()/w - 2 || p[(pY+2)*w + pX] == ' ')) 
			{
				std::string n(p);
				if(pY<p.length()/w - 3) n[(pY+2)*w + pX] = '$';
				n[(pY+1)*w + pX] = '@';
				n[pPos] = ' ';
				q.push(n);
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
	std::string nStr(w*w,' ');
	
	nStr[w % 2 == 0 ? w / 2 : (w-1) / 2] = '@';
	nStr[w + (w % 2 == 0 ? w / 2 : (w-1) / 2)] = '$';
	
	Node n = {nStr,0};
	toCompute.push(n);

	while(!toCompute.empty())
	{
		Node node = toCompute.front();
		toCompute.pop();

		if(DeadlockTable::isDeadlock(node.pos,w))
		{
			deadlocks.insert(node.pos);
			continue;
		}

		if(node.posToCompute >= node.pos.length()) continue;

		Node newNode = {node.pos,node.posToCompute+1};
		toCompute.push(newNode);

		if(node.pos[node.posToCompute] == ' ')
		{
			newNode.pos[node.posToCompute] = '$';
			toCompute.push(newNode);
			newNode.pos[node.posToCompute] = '#';
			toCompute.push(newNode);
		}
	}
}