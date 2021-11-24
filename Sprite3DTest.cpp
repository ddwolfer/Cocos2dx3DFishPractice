#ifndef __3DTEST__
#define __3DTEST__

#include "Sprite3DTest.h"

#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

#define ENEMY_NODE_NAME "EnemyNode"
#define ENEMY_LAYER_PRIORITY 0

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
    // 變數初始化
    m_createCount = 0;
    m_fireBullet = false;
    m_rewardFalg = false;
    //載入場景
    auto rootNode = CSLoader::createNode("AnimationNode/ShootTowerScene.csb");
    this->addChild(rootNode);
    m_enemyLayer = rootNode->getChildByName("Enemy");
    log("init background success");
    // 載入獎勵圈圈
    m_rewardCircle = CSLoader::createNode("AnimationNode/Reward_Big.csb");
    m_rewardAction = CSLoader::createTimeline("AnimationNode/Reward_Big.csb");
    m_rewardCircle->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    m_rewardCircle->setVisible(false);
    rootNode->addChild(m_rewardCircle);
    m_rewardCircle->runAction(m_rewardAction);
    // 生成玩家
    PlayerNode* playerNode = new PlayerNode();
    m_player = playerNode;
    m_player->setCameraMask((unsigned short)CameraFlag::USER1);
    rootNode->addChild(m_player);
    log("init player(cannon) success");
    // 生成敵人
    EnemyController* enemyController = EnemyController::getInstance();
    auto enemyNode = enemyController->getNewEnemy(EnemySetting::MosterType::goblin);
    m_enemyNode = enemyNode;
    m_enemyLayer->addChild(enemyNode, 0, ENEMY_NODE_NAME);
    log("init enemy(should be goblin) success");
    // 鍵盤偵測
    auto Keylistener = EventListenerKeyboard::create();
    Keylistener->onKeyPressed = CC_CALLBACK_2(Sprite3DTest::onKeyPressed, this);
    Keylistener->onKeyReleased = CC_CALLBACK_2(Sprite3DTest::onKeyReleased, this);

    _eventDispatcher->addEventListenerWithSceneGraphPriority(Keylistener, this);
    log("add keyboard event success");

    // 觸控偵測
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan = CC_CALLBACK_2(Sprite3DTest::onTouchBegan, this);
    touchListener->onTouchEnded = CC_CALLBACK_2(Sprite3DTest::onTouchEnd, this);
    touchListener->onTouchMoved = CC_CALLBACK_2(Sprite3DTest::onTouchMoved, this);

    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
    log("add touch event success");

    this->scheduleUpdate();
    log("add update schedule success");

    return true;
}
// 子彈打到敵人後 判定敵人是否死亡 
// random 0~1之間的數字，小於死亡概率->死亡 大於->存活
void Sprite3DTest::checkEnemyDeath()
{
    log("start check dead rate");
    float randomBulletDeadRate = (float)rand() / RAND_MAX;
    // 敵人存活
    if (randomBulletDeadRate > m_enemyNode->getDeadRate())
    {
        log("enemy live, shoot more bullet!");
        enemyHit();
    }
    // 敵人死亡
    else
    {
        log("congratulation! enemy die!");
        enemyDead();
    }
    return;
}
// 子彈判定為存活時，敵人撥放擊中動畫後回到idle
void Sprite3DTest::enemyHit()
{
    log("enemy should play hit animation, then back to idle status.");
    auto animTypeHitBackToIdle = CallFunc::create([=]() {
        m_enemyNode->playAnimate(EnemySetting::AnimType::idle, true);
        log("enemy animate type from hit back to idle");
        }
    );
    m_enemyNode->playAnimate(EnemySetting::AnimType::hit, false, animTypeHitBackToIdle);
}
// 子彈判定為死亡時，敵人需死亡，然後創建新的敵人
// (目前現有的TYPE為 {0:哥布林, 1:騎士})
void Sprite3DTest::enemyDead()
{
    // 設定分數
    setRewardValue(m_enemyNode->getScore());
    // 死亡動作
    log("enemy play dead animation, then remove itself.\n");
    Action* deadCallBack = CallFunc::create(std::bind(&Sprite3DTest::generateNewEnemy, this));
    m_enemyNode->playAnimate(EnemySetting::AnimType::dead, false, deadCallBack);
    //跳獎勵
    showRewardCircle();
}
// 創建新的敵人
void Sprite3DTest::generateNewEnemy()
{
    // 移除
    m_enemyNode->removeFromParent();
    // 新增
    auto generateNewEnemy = EnemyController::getInstance()->getNewEnemy((EnemySetting::MosterType)((int)(rand() % 2)));
    m_enemyNode = generateNewEnemy;
    m_enemyLayer->addChild(generateNewEnemy, 0, ENEMY_NODE_NAME);
    log("create new enemy success");
}
// 顯示獎勵圈圈
void Sprite3DTest::showRewardCircle()
{
    m_rewardCircle->setVisible(true);
    setRewardFalg(true);
    playRewardAnim("start");
}
// 播放獎勵動畫
void Sprite3DTest::playRewardAnim(std::string animName)
{    
    if (animName == "start") 
    {
        m_rewardAction->play("start", false);
        m_rewardAction->setLastFrameCallFunc(std::bind(&Sprite3DTest::playRewardAnim, this, "loop"));
    }
    else if (animName == "loop")
    {
        m_rewardAction->play("loop", false);
        m_rewardAction->setLastFrameCallFunc(std::bind(&Sprite3DTest::playRewardAnim, this, "end"));
    }
    else if (animName == "end")
    {
        m_rewardAction->play("end", false);
        m_rewardAction->setLastFrameCallFunc(std::bind(&Sprite3DTest::playRewardAnim, this, "quit"));
    }
    // 三段動畫結束後，隱藏獎圈並結束動作
    else if (animName == "quit")
    {
        m_rewardAction->clearLastFrameCallFunc();
        m_rewardCircle->setVisible(false);
        setRewardFalg(false);
    } 
}
// 設定獎勵數值
void Sprite3DTest::setRewardValue(int inputValue)
{
    // 找到設定數值的"Coney_Num"，並改變字串
    m_rewardCircle->enumerateChildren("//Coney_Num", [=](Node* foundNode) -> bool {
        static_cast<ui::TextBMFont*>(foundNode)->setString(std::to_string(inputValue));
        return true;
        });
}

void Sprite3DTest::update(float dt)
{
    // 冷卻時間過 & 觸控點下
    if (m_fireBullet && m_player->getShootFlag() && !m_rewardFalg)
    {
        // 射擊冷卻中，暫停射擊功能
        m_player->setShootFlag(false);
        m_player->playAttackAnim();
        m_weapon = new WeaponNode();
        this->addChild(m_weapon);
        std::function<void()> bulletCallBack = std::bind(&Sprite3DTest::checkEnemyDeath, this);
        m_weapon->moveToTarget(
            Vec3(m_enemyNode->getPosition3D().x, m_enemyNode->getPosition3D().y, m_enemyNode->getPosition3D().z + 7),
            bulletCallBack);
    }
}
// 觸控點下
bool Sprite3DTest::onTouchBegan(Touch* touch, Event* event) 
{
    log("Attack!");
    m_fireBullet = true;
    return true;
}
// 觸控離開
bool Sprite3DTest::onTouchEnd(Touch* touch, Event* event)
{
    log("touch release");
    m_fireBullet = false;
    return true;
}
// 觸控移動
bool Sprite3DTest::onTouchMoved(Touch* touch, Event* event)
{
    return true;
}

// 鍵盤案鍵按下
void Sprite3DTest::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
    //以下為測試用
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
    if (keyCode == EventKeyboard::KeyCode::KEY_3)
    {
        
    }
}

// 鍵盤案鍵放開
void Sprite3DTest::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
    CCLOG("key released");
    m_keys[keyCode] = false;
}
#endif // !__3DTEST__