#include <string>
#include <set>
#include "Constants.h"

class DeadlockTable
{
public: static bool isDeadlock(std::string dl, int w);
		static void computeDeadlocks(int w, int h, std::set<std::string>& deadlocks);
		static std::vector<int> generateCircularPositions();
		static void compileDeadlockTable(const std::string table,std::set<std::string>& deadlocks);
};