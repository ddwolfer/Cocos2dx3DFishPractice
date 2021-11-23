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
	// 大砲攻擊動畫
	auto playerAnimation = Animation3D::create(playerData.c3bPath);
	auto playerAnimate = Animate3D::create(playerAnimation);
	// 炮擊的火花動畫&Sprite3D
	SET_SETTING(m_sprBulletDegg, bulletDeggData)

	auto bulletDeggAnimation = Animation3D::create(bulletDeggData.c3bPath);
	auto bulletDeggAnimate = Animate3D::create(bulletDeggAnimation);

	m_sprPlayer->addChild(m_sprBulletDegg);

	// 砲擊結束 火光消失
	auto deleteBulletDegg = RemoveSelf::create();
	/*CallFunc::create([&]() {
		m_sprBulletDegg->removeFromParent();
		}
	);*/
	// 砲擊結束後 重新允許射擊
	auto shootCoolDownEnd = CallFunc::create([&]() {
		m_shootCoolDown = !m_shootCoolDown;
		}
	);
	// 進行行動
	auto bulletSeqAction = Sequence::create(bulletDeggAnimate, deleteBulletDegg, NULL);
	auto playerSeqAction = Sequence::create(playerAnimate, shootCoolDownEnd, NULL);

	m_sprBulletDegg->runAction(bulletSeqAction);
	m_sprPlayer->runAction(playerSeqAction);
}

WeaponNode::WeaponNode()
{
	// 砲彈基本設定
	SET_SETTING(m_sprStone, weaponStoneData)
	// 火焰基本設定
	SET_SETTING(m_sprFire, weaponFireData)

	m_sprStone->addChild(m_sprFire);
	this->addChild(m_sprStone);
}
// 傳一個position 跟 std::function
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
				// 刪除節點
				targetEnemy->removeFromParent();
				// 隨機生成新的敵人
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
