#pragma once
#include<unordered_map>
#include<unordered_set>
#include<string>

class CollisionManager
{
	std::unordered_map<std::string, std::unordered_set<std::string>>collisionmaps;
public:
	CollisionManager() {
		collisionmaps.insert({ "Default",std::unordered_set<std::string>{"Default"} });
	}
	void Initialize();
	bool FindMapping(const std::string& key1,const std::string& key2);
	void AddMapping(const std::string& key1, const std::string& key2);

};
