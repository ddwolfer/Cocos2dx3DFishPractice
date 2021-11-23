#ifndef __ENEMY_CONTROLLER__
#define __ENEMY_CONTROLLER__

#include "cocos2d.h"
#include "EnemySetting.h"
#include <cocos/3d/CCBundle3D.h>

USING_NS_CC;

class EnemyNode : public cocos2d::Node
{
public:
    EnemyNode(EnemySetting::EnemyData& enemySetting);
    ~EnemyNode() {};

    Sprite3D* getMonsterSprite3D() { return m_mosterSpr3D; }
    Sprite3D* getBaseSprite3D() { return m_baseSpr3D; }
    float   getDeadRate() { return m_deadrate; }
    int     getScore() { return m_score; }

    void playAnimate(EnemySetting::AnimType type, bool loop, Action* callback = nullptr);

private:
    Sprite3D*   m_mosterSpr3D;
    Sprite3D*   m_baseSpr3D;
    float       m_deadrate;
    int         m_score;
    std::string m_monsterC3bPath;
    std::string m_baseC3bPath;
    std::map<EnemySetting::AnimType, EnemySetting::AnimaInfo>    m_monsterAnimInfo;
    std::map<EnemySetting::AnimType, EnemySetting::AnimaInfo>    m_baseAnimInfo;

};

class EnemyController : public cocos2d::Node
{
public:
    EnemyNode* getNewEnemy(EnemySetting::MosterType inputType);
    static EnemyController* getInstance();
    static std::map<EnemySetting::MosterType, EnemySetting::EnemyData> getEnemySettingList() { return m_enemySettingList; }
private:
    EnemyController();
    static EnemyController* m_instance;
    static std::map<EnemySetting::MosterType, EnemySetting::EnemyData> m_enemySettingList;
};

#endif // !__ENEMY_CONTROLLER__