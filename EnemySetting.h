#ifndef __ENEMY_SETTING__
#define __ENEMY_SETTING__

#include "cocos2d.h"

USING_NS_CC;

namespace EnemySetting 
{
	enum class MosterType
	{
		goblin = 0,
		knight = 1
	};
	enum class AnimType
	{
		idle = 0,
		hit = 1,
		dead = 2
	};
	struct AnimaInfo
	{
		float Start;
		float End;
	};
	// �ĤH��Ƶ��c
	struct EnemyData
	{
		std::string     monsterC3bPath;
		std::string     baseC3bPath;
		Vec3            monsterPosition;
		Vec3            basePosition;
		Vec3            monsterRotation;
		Vec3            baseRotation;
		float			monsterScale;
		float			baseScale;
		float           deadRate;
		int				score;
		std::map<AnimType, AnimaInfo>    monsterAnimInfo;
		std::map<AnimType, AnimaInfo>    baseAnimInfo;
	};

	// �]�w�ĤH��T
	// �ĤH�ʵe�]�w
	const std::map<AnimType, AnimaInfo> goblinAnim = {
		{ AnimType::idle,	{0.f,		1.5f} },
		{ AnimType::hit,	{8.63f,		9.f} },
		{ AnimType::dead,	{11.33f,	11.83f} }
	};
	const std::map<AnimType, AnimaInfo> knightAnim = {
		{ AnimType::idle,	{0.f,		1.33f} },
		{ AnimType::hit,	{5.33f,		5.83f} },
		{ AnimType::dead,	{13.33f,	13.83f} }
	};
	const std::map<AnimType, AnimaInfo> brickAnim = {
		{ AnimType::idle,	{1.23f,		1.26f} },
		{ AnimType::hit,	{3.9f,		3.93f} },
		{ AnimType::dead,	{1.26f,		6.73f} }
	};
	const std::map<AnimType, AnimaInfo> woodAnim = {
		{ AnimType::idle,	{5.5f,		5.53f} },
		{ AnimType::hit,	{2.5f,		2.53f} },
		{ AnimType::dead,	{1.33f,		5.53f} }
	};
	//�����L
	const EnemyData goblinData = {
		"/monster/Fish01_Goblin_Archer/monster_SP_003.c3b",
		"/base/wood/wood_break.c3b",
		Vec3(0.f,15.5f,0.f),
		Vec3(0.f,0.f,0.f),
		Vec3(0.f,0.f,0.f),
		Vec3(0.f,0.f,0.f),
		1.5f,
		1.f,
		0.8f,
		1000,
		goblinAnim,
		woodAnim
	};
	//�M�h
	const EnemyData knightData = {
		"/monster/Fish02_Knife_Soldier/monster_S_004_01.c3b",
		"/base/brick/brick_break.c3b",
		Vec3(0.f,15.5f,0.f),
		Vec3(0.f,0.f,0.f),
		Vec3(0.f,0.f,0.f),
		Vec3(0.f,0.f,0.f),
		1.5f,
		1.f,
		0.8f,
		2000,
		knightAnim,
		brickAnim
	};
	
}


#endif // !__ENEMY_SETTING__