#include "Constants.h"

std::set<std::pair<int,int>> Constants::Goals;
std::map<std::pair<int,int>,std::map<int,int>> Constants::pushablePositions;
std::set<std::string> Constants::deadlockTable;