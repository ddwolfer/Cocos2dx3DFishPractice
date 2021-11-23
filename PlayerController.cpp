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

	auto bulletDeggAnimation = Animation3D::create(bulletDeggData.c3bPath);
	auto bulletDeggAnimate = Animate3D::create(bulletDeggAnimation);

	m_sprPlayer->addChild(m_sprBulletDegg);

	// �������� ��������
	auto deleteBulletDegg = RemoveSelf::create();
	/*CallFunc::create([&]() {
		m_sprBulletDegg->removeFromParent();
		}
	);*/
	// ���������� ���s���\�g��
	auto shootCoolDownEnd = CallFunc::create([&]() {
		m_shootCoolDown = !m_shootCoolDown;
		}
	);
	// �i����
	auto bulletSeqAction = Sequence::create(bulletDeggAnimate, deleteBulletDegg, NULL);
	auto playerSeqAction = Sequence::create(playerAnimate, shootCoolDownEnd, NULL);

	m_sprBulletDegg->runAction(bulletSeqAction);
	m_sprPlayer->runAction(playerSeqAction);
}

WeaponNode::WeaponNode()
{
	// ���u�򥻳]�w
	SET_SETTING(m_sprStone, weaponStoneData)
	// ���K�򥻳]�w
	SET_SETTING(m_sprFire, weaponFireData)

	m_sprStone->addChild(m_sprFire);
	this->addChild(m_sprStone);
}
// �Ǥ@��position �� std::function
void WeaponNode::moveToTarget(float deadrate, EnemyNode* targetEnemy, Node* enemyLayer)
{


	auto weaponMoveTo = MoveTo::create(2, targetEnemy->getBaseSprite3D()->getPosition3D());
	auto checkEnemyDeath = CallFunc::create([=]() {
		float rollDice = (float) rand() / RAND_MAX;
		log("enemy dead rate: %f / player dice value: %f ", deadrate, rollDice);
		if (rollDice > deadrate)
		{
			auto hitBackToIdle = CallFunc::create([=]() {
				targetEnemy->playAnimate(EnemySetting::AnimType::idle, true);
				log("from hit to idle");
				}
			);
			log("enemy live");
			targetEnemy->playAnimate(EnemySetting::AnimType::hit, false, hitBackToIdle);
		}
		else
		{
			auto deadAndCreateNew = CallFunc::create([=]() {
				// �R���`�I
				targetEnemy->removeFromParent();
				// �H���ͦ��s���ĤH
				auto newEnemyNode = EnemyController::getInstance()->getNewEnemy((EnemySetting::MosterType)(int)(rand() % 2));
				enemyLayer->addChild(newEnemyNode,0,"EnemyNode");

				log("dead and create new one");
				}
			);
			log("enemy dead");
			targetEnemy->playAnimate(EnemySetting::AnimType::dead, false, deadAndCreateNew);
		}
		}
	);
	auto removeWeapon = RemoveSelf::create();
	auto weaponSeqAction = Sequence::create(weaponMoveTo, checkEnemyDeath,removeWeapon, NULL);

	m_sprStone->runAction(weaponSeqAction);
}
