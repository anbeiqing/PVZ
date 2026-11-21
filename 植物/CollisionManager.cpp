#include "CollisionManager.h"

void CollisionManager::Initialize()
{
	AddMapping("sun", "Platform");
}

bool CollisionManager::FindMapping(const std::string& key1, const std::string& key2)
{
	auto it = collisionmaps.find(key1);
	if (it != collisionmaps.end()) {
		return it->second.find(key2) != it->second.end();
	}
	return false;
}

void CollisionManager::AddMapping(const std::string& key1, const std::string& key2)
{
	if (collisionmaps.find(key1) == collisionmaps.end()) {
		collisionmaps.insert({ key1,std::unordered_set<std::string>() });
	}
	if (collisionmaps.find(key2) == collisionmaps.end()) {
		collisionmaps.insert({ key2,std::unordered_set<std::string>() });
	}
	collisionmaps[key1].insert(key2);
	collisionmaps[key2].insert(key1);
}
