#include "EnemyController.h"

USING_NS_CC;

std::map<EnemySetting::MosterType, EnemySetting::EnemyData> EnemyController::m_enemySettingList = {};
EnemyController* EnemyController::m_instance = nullptr;

EnemyNode::EnemyNode(EnemySetting::EnemyData& _enemySetting)
{
	// 讀取基本設定
	m_mosterSpr3D		= Sprite3D::create(_enemySetting.monsterC3bPath);
	m_monsterC3bPath	= _enemySetting.monsterC3bPath;
	m_baseSpr3D			= Sprite3D::create(_enemySetting.baseC3bPath);
	m_baseC3bPath		= _enemySetting.baseC3bPath;
	m_deadrate			= _enemySetting.deadRate;
	m_score				= _enemySetting.score;
	m_monsterAnimInfo	= _enemySetting.monsterAnimInfo;
	m_baseAnimInfo		= _enemySetting.baseAnimInfo;
	// 設定基本設定
	m_mosterSpr3D->setPosition3D(_enemySetting.monsterPosition);
	m_baseSpr3D->setPosition3D(_enemySetting.basePosition);
	m_mosterSpr3D->setRotation3D(_enemySetting.monsterRotation);
	m_baseSpr3D->setRotation3D(_enemySetting.baseRotation);
	m_mosterSpr3D->setScale(_enemySetting.monsterScale);
	m_baseSpr3D->setScale(_enemySetting.baseScale);
	m_baseSpr3D->setCullFaceEnabled(false);

	// 初始動畫
	playAnimate(EnemySetting::AnimType::idle, true);

	this->addChild(m_mosterSpr3D, 2, "monster");
	this->addChild(m_baseSpr3D, 1, "base");
}

void EnemyNode::playAnimate(EnemySetting::AnimType type, bool loop, Action* callback)
{
	// 先停掉上一個動作
	m_mosterSpr3D->stopAllActions();
	m_baseSpr3D->stopAllActions();

	// 建立新的動作
	auto monsterAnimation3D = Animation3D::create(m_monsterC3bPath);
	auto monsterAnimate3D = Animate3D::create(
		monsterAnimation3D,
		m_monsterAnimInfo[type].Start,
		m_monsterAnimInfo[type].End - m_monsterAnimInfo[type].Start
	);
	log("moster play frame time from start: %f to end: %f", m_monsterAnimInfo[type].Start, m_monsterAnimInfo[type].End);
	auto baseAnimation3D = Animation3D::create(m_baseC3bPath);
	auto baseAnimate3D = Animate3D::create(
		baseAnimation3D,
		m_baseAnimInfo[type].Start,
		m_baseAnimInfo[type].End - m_baseAnimInfo[type].Start
	);
	// 執行動作 
	if (loop) 
	{
		m_mosterSpr3D->runAction(RepeatForever::create(monsterAnimate3D));
		m_baseSpr3D->runAction(RepeatForever::create(baseAnimate3D));
	}
	else
	{
		auto _action = Sequence::create(monsterAnimate3D, callback, NULL);

		m_baseSpr3D->runAction((baseAnimate3D));
		m_mosterSpr3D->runAction(_action);
	}

}


// 初始化敵人設定
EnemyController::EnemyController()
{
	m_enemySettingList.insert(std::pair<EnemySetting::MosterType, EnemySetting::EnemyData>(
		EnemySetting::MosterType::goblin, EnemySetting::goblinData));
	m_enemySettingList[EnemySetting::MosterType::goblin] = EnemySetting::goblinData;
	m_enemySettingList[EnemySetting::MosterType::knight] = EnemySetting::knightData;
}
// 獲取新的敵人
EnemyNode* EnemyController::getNewEnemy(EnemySetting::MosterType inputType)
{
	// 創建新的敵人
	EnemyNode* newEnemy = new EnemyNode(m_enemySettingList[inputType]);
	newEnemy->setCameraMask((unsigned short)CameraFlag::USER1);
	return newEnemy;
}
// 獲取EnemyController
EnemyController* EnemyController::getInstance()
{
	if (m_instance == nullptr)
	{
		return new EnemyController();
	}
	return m_instance;
}
