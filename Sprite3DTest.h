#ifndef __SPRITE_3DTEST__
#define __SPRITE_3DTEST__

#include "cocos2d.h"
#include "EnemyController.h"
#include "PlayerController.h"

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

    bool onTouchBegan(Touch* touch, Event* event);
    bool onTouchEnd(Touch* touch, Event* event);
    void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);
    void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);
    
    void setEnemyNode(EnemyNode* inputNode ) { m_enemyNode = inputNode; }
    Node* getEnemyLayer() { return m_enemyLayer; }

    void countPlusOne() { m_createCount++; }
    int getCreateCount() { return m_createCount; }
    // implement the "static create()" method manually
    CREATE_FUNC(Sprite3DTest);

private:
    std::map<EventKeyboard::KeyCode, bool> m_keys;
    Node*       m_enemyLayer;
    EnemyNode*  m_enemyNode;
    PlayerNode* m_player;
    WeaponNode* m_weapon;

    int m_createCount; 
};

#endif // __SPRITE_3DTEST__