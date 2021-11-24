#include "PlayerController.h"

PlayerNode::PlayerNode()
{
	// 允許射擊
	m_shootCoolDown = true;
	// 大砲基本設定
	SET_SETTING(m_sprPlayer, playerData)

	this->addChild(m_sprPlayer, 0, "Player");
}

void PlayerNode::playAttackAnim()
{
	// 玩家(大砲)
	// 大砲攻擊動畫
	auto playerAnimation = Animation3D::create(playerData.c3bPath);
	auto playerAnimate = Animate3D::create(playerAnimation);
	// 砲擊動畫結束後 重新允許射擊
	auto shootCoolDownEnd = CallFunc::create([&]() {
		setShootFlag(true);
		}
	);
	// 進行行動
	auto playerSeqAction = Sequence::create(playerAnimate, shootCoolDownEnd, NULL);
	m_sprPlayer->runAction(playerSeqAction);

	// 砲擊火花
	// Sprite and Animation
	SET_SETTING(m_sprBulletDegg, bulletDeggData)
	m_sprBulletDegg->setBlendFunc(BlendFunc::ADDITIVE);
	auto bulletDeggAnimation = Animation3D::create(bulletDeggData.c3bPath);
	auto bulletDeggAnimate = Animate3D::create(bulletDeggAnimation);
	m_sprPlayer->addChild(m_sprBulletDegg);
	// 砲擊結束 火光消失
	auto deleteBulletDegg = RemoveSelf::create();
	// 進行行動
	auto bulletSeqAction = Sequence::create(bulletDeggAnimate, deleteBulletDegg, NULL);
	m_sprBulletDegg->runAction(bulletSeqAction);
}

WeaponNode::WeaponNode(float _shootTime)
{
	// 砲彈基本設定
	SET_SETTING(m_sprStone, weaponStoneData);
	// 火焰基本設定
	SET_SETTING(m_sprFire, weaponFireData);
	// 去掉黑邊
	m_sprFire->setBlendFunc(BlendFunc::ADDITIVE);

	m_sprStone->addChild(m_sprFire);
	this->addChild(m_sprStone);
	this->m_shootTime = _shootTime;
}
void WeaponNode::moveToTarget(Vec3 targetPosition, std::function<void()> callFunc)
{
	// 移動子彈
	auto weaponMoveTo = MoveTo::create(m_shootTime, targetPosition);
	// 子彈變速
	auto weaponMoveEase = EaseIn::create(weaponMoveTo, 2);
	// 子彈移除
	auto removeWeapon = RemoveSelf::create();
	// 串聯子彈動作
	auto weaponSeqAction = Sequence::create(weaponMoveEase, CallFunc::create(callFunc), removeWeapon, NULL);
	m_sprStone->runAction(weaponSeqAction);
}
