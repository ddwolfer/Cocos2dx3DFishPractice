#ifndef __ENEMY_SETTING__
#define __ENEMY_SETTING__

#include "cocos2d.h"

USING_NS_CC;

namespace EnemySetting 
{
	// 怪物型態
	enum class MosterType
	{
		goblin = 0,
		knight = 1
	};
	// 動畫狀態
	enum class AnimType
	{
		idle = 0,
		hit = 1,
		dead = 2
	};
	// 動畫起迄
	struct AnimaInfo
	{
		float Start;
		float End;
	};
	// 敵人資料結構
	struct EnemyData
	{
		std::string     monsterC3bPath;						// 怪物路徑
		std::string     baseC3bPath;						// 基底路徑
		Vec3            monsterPosition;					// 怪物位置
		Vec3            basePosition;						// 基底位置
		Vec3            monsterRotation;					// 怪物旋轉
		Vec3            baseRotation;						// 基底旋轉
		float			monsterScale;						// 怪物縮放
		float			baseScale;							// 基底縮放
		float           deadRate;							// 死亡概率(0~1)
		int				score;								// 擊殺後的分數
		std::map<AnimType, AnimaInfo>    monsterAnimInfo;	// 怪物的動畫資訊
		std::map<AnimType, AnimaInfo>    baseAnimInfo;		// 基底的動畫資訊
	};

	// 設定敵人資訊
	// 敵人動畫設定
	const std::map<AnimType, AnimaInfo> goblinAnim = {
		{ AnimType::idle,	{0.f,		1.5f} },
		{ AnimType::hit,	{8.63f,		9.f} },
		{ AnimType::dead,	{11.33f,	11.83f} }
	};
	const std::map<AnimType, AnimaInfo> knightAnim = {
		{ AnimType::idle,	{0.f,		1.33f} },
		{ AnimType::hit,	{5.37f,		5.83f} },
		{ AnimType::dead,	{13.33f,	13.83f} }
	};
	const std::map<AnimType, AnimaInfo> brickAnim = {
		{ AnimType::idle,	{1.23f,		1.26f} },
		{ AnimType::hit,	{1.23f,		1.26f} }, // 也可以改{ AnimType::hit,	{3.9f,		3.93f}
		{ AnimType::dead,	{1.26f,		6.73f} }
	};
	const std::map<AnimType, AnimaInfo> woodAnim = {
		{ AnimType::idle,	{5.5f,		5.53f} },
		{ AnimType::hit,	{5.5f,		5.53f} }, // 也可以改{ AnimType::hit,	{2.5f,		2.53f}
		{ AnimType::dead,	{1.33f,		5.53f} }
	};
	//哥布林
	const EnemyData goblinData = {
		"/monster/Fish01_Goblin_Archer/monster_SP_003.c3b",
		"/base/wood/wood_break.c3b",
		Vec3(0.f,15.5f,0.f),
		Vec3(0.f,0.f,0.f),
		Vec3(0.f,0.f,0.f),
		Vec3(0.f,0.f,0.f),
		1.5f,
		1.f,
		0.5f,
		1000,
		goblinAnim,
		woodAnim
	};
	//騎士
	const EnemyData knightData = {
		"/monster/Fish02_Knife_Soldier/monster_S_004_01.c3b",
		"/base/brick/brick_break.c3b",
		Vec3(0.f,15.5f,0.f),
		Vec3(0.f,0.f,0.f),
		Vec3(0.f,0.f,0.f),
		Vec3(0.f,0.f,0.f),
		1.5f,
		1.f,
		0.25f,
		2000,
		knightAnim,
		brickAnim
	};
	
}


#endif // !__ENEMY_SETTING__