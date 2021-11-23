#ifndef __SPRITE_3DTEST__
#define __SPRITE_3DTEST__

#include "cocos2d.h"
#include "EnemyController.h"
#include "PlayerController.h"
#include "cocostudio//CocoStudio.h"

USING_NS_CC;

#define KEYPRESS_CHECK(Key) \
if (isKeyPressed(Key)) { \
    keyPressedDuration(Key); \
}


class Sprite3DTest : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    void update(float dt);
    // Ĳ���ƥ�
    bool onTouchBegan(Touch* touch, Event* event);
    bool onTouchEnd(Touch* touch, Event* event);
    bool onTouchMoved(Touch* touch, Event* event);
    // ��L�ƥ�
    void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);
    void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);
    //�ĤH�P�w�ƥ�
    void checkEnemyDeath();
    void enemyChangeHitToIdle();
    void enemyDead();
    void generateNewEnemy();
    // ���y�ƥ�
    void showRewardCircle();
    void setRewardValue(int inputValue);
    void playRewardAnim(std::string animName);
    // ���private�ܼ�
    void setEnemyNode(EnemyNode* inputNode ) { m_enemyNode = inputNode; }
    Node* getEnemyLayer() { return m_enemyLayer; }
    bool getRewardFlag() { return m_rewardFalg; }
    void setRewardFalg(bool inputBool) { m_rewardFalg = inputBool; }
    // �p��ĤH�ƶq debug��
    void countPlusOne() { m_createCount++; }
    int getCreateCount() { return m_createCount; }
    // implement the "static create()" method manually
    CREATE_FUNC(Sprite3DTest);

private:
    std::map<EventKeyboard::KeyCode, bool> m_keys;
    Node*           m_rewardCircle;
    Node*           m_enemyLayer;
    EnemyNode*      m_enemyNode;
    PlayerNode*     m_player;
    WeaponNode*     m_weapon;
    cocostudio::timeline::ActionTimeline* m_rewardAction;

    bool        m_fireBullet;
    bool        m_rewardFalg;
    int         m_createCount; 
};

#endif // __SPRITE_3DTEST__