#ifndef __3DTEST__
#define __3DTEST__

#include "Sprite3DTest.h"
#include "cocostudio//CocoStudio.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"


USING_NS_CC;

Scene* Sprite3DTest::createScene()
{
    return Sprite3DTest::create();
}
// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}
bool Sprite3DTest::init()
{
    // 1. super init first
    if (!Scene::init())
    {
        return false;
    }

    const auto visibleSize = Director::getInstance()->getVisibleSize();
    const Vec2 origin = Director::getInstance()->getVisibleOrigin();

    srand(time(0));
    m_createCount = 0;

    //載入場景
    auto rootNode = CSLoader::createNode("AnimationNode/ShootTowerScene.csb");
    this->addChild(rootNode);
    m_enemyLayer = rootNode->getChildByName("Enemy");
    //CSLoader::createTimeline("");

    // 生成玩家
    PlayerNode* playerNode = new PlayerNode();
    m_player = playerNode;
    m_player->setCameraMask((unsigned short)CameraFlag::USER1);
    rootNode->addChild(m_player);

    // 生成敵人
    EnemyController* enemyController = EnemyController::getInstance();
    auto enemyNode = enemyController->getNewEnemy(EnemySetting::MosterType::goblin);
    m_enemyNode = enemyNode;
    m_enemyLayer->addChild(enemyNode, 0, "EnemyNode");

    auto tmpGoblin = Sprite3D::create("monster/Fish01_Goblin_Archer/monster_SP_003.c3b");

    m_createCount++;
    // 鍵盤偵測
    auto Keylistener = EventListenerKeyboard::create();
    Keylistener->onKeyPressed = CC_CALLBACK_2(Sprite3DTest::onKeyPressed, this);
    Keylistener->onKeyReleased = CC_CALLBACK_2(Sprite3DTest::onKeyReleased, this);

    _eventDispatcher->addEventListenerWithSceneGraphPriority(Keylistener, this);

    // 滑鼠偵測
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan = CC_CALLBACK_2(Sprite3DTest::onTouchBegan, this);
    touchListener->onTouchEnded = CC_CALLBACK_2(Sprite3DTest::onTouchEnd, this);

    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

    this->scheduleUpdate();

    return true;
}


void Sprite3DTest::update(float dt)
{

}
// 觸控點下
bool Sprite3DTest::onTouchBegan(Touch* touch, Event* event) 
{
    log("Attack!");
    if (m_player->getShootFlag()) 
    {
        // 射擊過程中，暫停射擊功能
        m_player->changeShootFlag();
        m_player->playAttackAnim();
        m_weapon = new WeaponNode();
        this->addChild(m_weapon);
        log("enemy dead rate in main cpp : %f", m_enemyNode->getDeadRate());
        float tmpFLT = m_enemyNode->getDeadRate();
        m_weapon->moveToTarget(tmpFLT, m_enemyNode, m_enemyLayer, );
    }

    return true;
}
// 觸控離開
bool Sprite3DTest::onTouchEnd(Touch* touch, Event* event)
{
    log("touch release");
    return true;
}
// 鍵盤案鍵按下
void Sprite3DTest::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
    CCLOG("key pressed");
    m_keys[keyCode] = true;
    // 敵人受傷
    if (keyCode == EventKeyboard::KeyCode::KEY_1)
    {
        // 受傷後回到idle狀態
        auto backToIdle = CallFunc::create([&]() {
            m_enemyNode->playAnimate(EnemySetting::AnimType::idle, true);
            log("from hit to idle");
            }
        );
        m_enemyNode->playAnimate(EnemySetting::AnimType::hit, false, backToIdle);
    }
    // 敵人死亡
    if (keyCode == EventKeyboard::KeyCode::KEY_2)
    {
        // 死亡後 哥布林、騎士 交替生產
        auto backToIdle = CallFunc::create([&]() {
            m_enemyNode->removeFromParent();
            auto newEnemyNode = EnemyController::getInstance()->getNewEnemy((EnemySetting::MosterType)(int)(m_createCount % 2));
            m_enemyNode = newEnemyNode;
            m_enemyLayer->addChild(newEnemyNode, 0, "enemyNode");
            countPlusOne();
            log("call func action dead end");
            }
        );
        m_enemyNode->playAnimate(EnemySetting::AnimType::dead, false, backToIdle);
    }
}
// 鍵盤案鍵放開
void Sprite3DTest::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
    CCLOG("key released");
    m_keys[keyCode] = false;

}
#endif // !__3DTEST__