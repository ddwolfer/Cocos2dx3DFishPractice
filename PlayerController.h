#ifndef __PLAYER_CONTROLLER__
#define __PLAYER_CONTROLLER__

#include "cocos2d.h"
#include "EnemySetting.h"
#include "EnemyController.h"

USING_NS_CC;

#define SET_SETTING(spr3D, sprData) \
spr3D = Sprite3D::create(sprData.c3bPath);\
spr3D->setPosition3D(sprData.position);\
spr3D->setRotation3D(sprData.rotation);\
spr3D->setScale(sprData.scale);\
spr3D->setCameraMask((unsigned short)CameraFlag::USER1);\


struct settingData 
{
	const std::string	c3bPath;
	const Vec3			position;
	const Vec3			rotation;
	const float			scale;
};

const settingData playerData = 
{
	"weapon/ConnonLv_01/gun_01.c3b",
	Vec3(0,3,42),
	Vec3(0,0,0),
	1.0
};
const settingData bulletDeggData =
{
	"weapon/bullet/Bullet_Degg.c3b",
	Vec3(0,0,-5),
	Vec3(0,180,0),
	0.06
};
const settingData weaponStoneData =
{
	"weapon/bullet/Weapon_Stone_01.c3b",
	Vec3(0,5,35),
	Vec3(90,0,180),
	0.13
};
const settingData weaponFireData =
{
	"weapon/bullet/Fire_small.c3b",
	Vec3(0,10,0),
	Vec3(-90,90,0),
	1
};



class PlayerNode : public cocos2d::Node
{
public:
	PlayerNode();
	~PlayerNode() {};
	Sprite3D*	getSprite3D() { return m_sprPlayer; }
	bool		getShootFlag() { return m_shootCoolDown; }
	void		changeShootFlag() {	m_shootCoolDown = !m_shootCoolDown;	}
	void		playAttackAnim();

private:
	Sprite3D*	m_sprPlayer;
	Sprite3D*	m_sprBulletDegg; //可以刪掉 function 內用就好
	bool		m_shootCoolDown;
};

class WeaponNode : public cocos2d::Node
{
public:
	WeaponNode();
	void moveToTarget(float deadrate, EnemyNode* targetEnemy, Node* enemyLayer);
private:
	Sprite3D*	m_sprStone;
	Sprite3D*	m_sprFire;
};


#endif // !__PLAYER_CONTROLLER__
