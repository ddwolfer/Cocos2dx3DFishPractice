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
    // �ܼƪ�l��
    m_createCount = 0;
    m_fireBullet = false;
    m_rewardFalg = false;
    //���J����
    auto rootNode = CSLoader::createNode("AnimationNode/ShootTowerScene.csb");
    this->addChild(rootNode);
    m_enemyLayer = rootNode->getChildByName("Enemy");
    log("init background success");
    // ���J���y���
    m_rewardCircle = CSLoader::createNode("AnimationNode/Reward_Big.csb");
    m_rewardAction = CSLoader::createTimeline("AnimationNode/Reward_Big.csb");
    m_rewardCircle->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
    m_rewardCircle->setVisible(false);
    rootNode->addChild(m_rewardCircle);
    m_rewardCircle->runAction(m_rewardAction);
    // �ͦ����a
    PlayerNode* playerNode = new PlayerNode();
    m_player = playerNode;
    m_player->setCameraMask((unsigned short)CameraFlag::USER1);
    rootNode->addChild(m_player);
    log("init player(cannon) success");
    // �ͦ��ĤH
    EnemyController* enemyController = EnemyController::getInstance();
    auto enemyNode = enemyController->getNewEnemy(EnemySetting::MosterType::goblin);
    m_enemyNode = enemyNode;
    m_enemyLayer->addChild(enemyNode, 0, ENEMY_NODE_NAME);
    log("init enemy(should be goblin) success");
    // ��L����
    auto Keylistener = EventListenerKeyboard::create();
    Keylistener->onKeyPressed = CC_CALLBACK_2(Sprite3DTest::onKeyPressed, this);
    Keylistener->onKeyReleased = CC_CALLBACK_2(Sprite3DTest::onKeyReleased, this);

    _eventDispatcher->addEventListenerWithSceneGraphPriority(Keylistener, this);
    log("add keyboard event success");

    // Ĳ������
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
// �l�u����ĤH�� �P�w�ĤH�O�_���` 
// random 0~1�������Ʀr�A�p�󦺤`���v->���` �j��->�s��
void Sprite3DTest::checkEnemyDeath()
{
    log("start check dead rate");
    float randomBulletDeadRate = (float)rand() / RAND_MAX;
    // �ĤH�s��
    if (randomBulletDeadRate > m_enemyNode->getDeadRate())
    {
        log("enemy live, shoot more bullet!");
        enemyHit();
    }
    // �ĤH���`
    else
    {
        log("congratulation! enemy die!");
        enemyDead();
    }
    return;
}
// �l�u�P�w���s���ɡA�ĤH���������ʵe��^��idle
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
// �l�u�P�w�����`�ɡA�ĤH�ݦ��`�A�M��Ыطs���ĤH
// (�ثe�{����TYPE�� {0:�����L, 1:�M�h})
void Sprite3DTest::enemyDead()
{
    // �]�w����
    setRewardValue(m_enemyNode->getScore());
    // ���`�ʧ@
    log("enemy play dead animation, then remove itself.\n");
    Action* deadCallBack = CallFunc::create(std::bind(&Sprite3DTest::generateNewEnemy, this));
    m_enemyNode->playAnimate(EnemySetting::AnimType::dead, false, deadCallBack);
    //�����y
    showRewardCircle();
}
// �Ыطs���ĤH
void Sprite3DTest::generateNewEnemy()
{
    // ����
    m_enemyNode->removeFromParent();
    // �s�W
    auto generateNewEnemy = EnemyController::getInstance()->getNewEnemy((EnemySetting::MosterType)((int)(rand() % 2)));
    m_enemyNode = generateNewEnemy;
    m_enemyLayer->addChild(generateNewEnemy, 0, ENEMY_NODE_NAME);
    log("create new enemy success");
}
// ��ܼ��y���
void Sprite3DTest::showRewardCircle()
{
    m_rewardCircle->setVisible(true);
    setRewardFalg(true);
    playRewardAnim("start");
}
// ������y�ʵe
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
    // �T�q�ʵe������A���ü���õ����ʧ@
    else if (animName == "quit")
    {
        m_rewardAction->clearLastFrameCallFunc();
        m_rewardCircle->setVisible(false);
        setRewardFalg(false);
    } 
}
// �]�w���y�ƭ�
void Sprite3DTest::setRewardValue(int inputValue)
{
    // ���]�w�ƭȪ�"Coney_Num"�A�ç��ܦr��
    m_rewardCircle->enumerateChildren("//Coney_Num", [=](Node* foundNode) -> bool {
        static_cast<ui::TextBMFont*>(foundNode)->setString(std::to_string(inputValue));
        return true;
        });
}

void Sprite3DTest::update(float dt)
{
    // �N�o�ɶ��L & Ĳ���I�U
    if (m_fireBullet && m_player->getShootFlag() && !m_rewardFalg)
    {
        // �g���N�o���A�Ȱ��g���\��
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
// Ĳ���I�U
bool Sprite3DTest::onTouchBegan(Touch* touch, Event* event) 
{
    log("Attack!");
    m_fireBullet = true;
    return true;
}
// Ĳ�����}
bool Sprite3DTest::onTouchEnd(Touch* touch, Event* event)
{
    log("touch release");
    m_fireBullet = false;
    return true;
}
// Ĳ������
bool Sprite3DTest::onTouchMoved(Touch* touch, Event* event)
{
    return true;
}

// ��L������U
void Sprite3DTest::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
    //�H�U�����ե�
    CCLOG("key pressed");
    m_keys[keyCode] = true;
    // �ĤH����
    if (keyCode == EventKeyboard::KeyCode::KEY_1)
    {
        // ���˫�^��idle���A
        auto backToIdle = CallFunc::create([&]() {
            m_enemyNode->playAnimate(EnemySetting::AnimType::idle, true);
            log("from hit to idle");
            }
        );
        m_enemyNode->playAnimate(EnemySetting::AnimType::hit, false, backToIdle);
    }
    // �ĤH���`
    if (keyCode == EventKeyboard::KeyCode::KEY_2)
    {
        // ���`�� �����L�B�M�h ����Ͳ�
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

// ��L�����}
void Sprite3DTest::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
    CCLOG("key released");
    m_keys[keyCode] = false;
}
#endif // !__3DTEST__