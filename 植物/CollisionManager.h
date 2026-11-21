#pragma once
#include<unordered_map>
#include<unordered_set>
#include<string>


//碰撞类型
enum class CollisionType :uint8_t
{
	Default,
	Plant,
	Bullet,
	Zombie
};
class CollisionManager
{
public:
	CollisionManager();

	void Initialize();

	bool FindMapping(CollisionType type1, CollisionType type2);

private:
	std::unordered_set<int>collisionMap;

	void AddMapping(CollisionType type1, CollisionType type2);

};
