#pragma once
class Sector
{
public:
	Sector();
	~Sector();
	std::mutex sLock;
	std::unordered_set<int> player;
	std::unordered_set<int> object;
	//std::map<int, Object*> object;
};

