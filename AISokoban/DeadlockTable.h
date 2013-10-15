#include <string>
#include <set>

class DeadlockTable
{
public: static bool isDeadlock(std::string dl, int w);
		static void computeDeadlocks(int w, int h, std::set<std::string>& deadlocks);
};