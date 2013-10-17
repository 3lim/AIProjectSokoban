#pragma once
#include <string>
#include <set>
#include <unordered_set>
#include "Constants.h"

class DeadlockTable
{
public: static bool isDeadlock(std::string dl, int w);
		static void computeDeadlocks(int w, int h, std::unordered_set<std::string>& deadlocks);
		static std::vector<int> generateCircularPositions();
		static void compileDeadlockTable(const std::string table,std::unordered_set<std::string>& deadlocks);
		static bool checkForDeadlock(std::string s);
};