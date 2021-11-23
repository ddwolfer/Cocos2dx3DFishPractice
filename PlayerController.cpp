#include "PlayerController.h"

PlayerNode::PlayerNode()
{
	// ���\�g��
	m_shootCoolDown = true;
	// �j���򥻳]�w
	SET_SETTING(m_sprPlayer, playerData)

	this->addChild(m_sprPlayer, 0, "Player");
}

void PlayerNode::playAttackAnim()
{
	// �j�������ʵe
	auto playerAnimation = Animation3D::create(playerData.c3bPath);
	auto playerAnimate = Animate3D::create(playerAnimation);
	// ����������ʵe&Sprite3D
	SET_SETTING(m_sprBulletDegg, bulletDeggData)
	m_sprBulletDegg->setBlendFunc(BlendFunc::ADDITIVE);

	auto bulletDeggAnimation = Animation3D::create(bulletDeggData.c3bPath);
	auto bulletDeggAnimate = Animate3D::create(bulletDeggAnimation);

	m_sprPlayer->addChild(m_sprBulletDegg);

	// �������� ��������
	auto deleteBulletDegg = RemoveSelf::create();

	// ���������� ���s���\�g��
	auto shootCoolDownEnd = CallFunc::create([&]() {
		setShootFlag(true);
		}
	);
	// �i����
	auto bulletSeqAction = Sequence::create(bulletDeggAnimate, deleteBulletDegg, NULL);
	auto playerSeqAction = Sequence::create(playerAnimate, shootCoolDownEnd, NULL);

	m_sprBulletDegg->runAction(bulletSeqAction);
	m_sprPlayer->runAction(playerSeqAction);
}

WeaponNode::WeaponNode(float _shootTime)
{
	// ���u�򥻳]�w
	SET_SETTING(m_sprStone, weaponStoneData);
	// ���K�򥻳]�w
	SET_SETTING(m_sprFire, weaponFireData);
	
	m_sprFire->setBlendFunc(BlendFunc::ADDITIVE);

	m_sprStone->addChild(m_sprFire);
	this->addChild(m_sprStone);
	this->m_shootTime = _shootTime;
}
void WeaponNode::moveToTarget(Vec3 targetPosition, std::function<void()> callFunc)
{
	auto weaponMoveTo = MoveTo::create(m_shootTime, targetPosition);

	auto weaponMoveEase = EaseIn::create(weaponMoveTo, 2);

	auto removeWeapon = RemoveSelf::create();

	auto weaponSeqAction = Sequence::create(weaponMoveEase, CallFunc::create(callFunc), removeWeapon, NULL);

	m_sprStone->runAction(weaponSeqAction);
}
