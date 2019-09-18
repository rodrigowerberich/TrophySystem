#ifndef TROPHYSYSTEM_H
#define TROPHYSYSTEM_H

#include "dblistener.h"
#include "dbmanager.h"
#include <QDebug>

class TrophySystem:public DbListener
{
private:
    DbManager& m_dbManager;
    void coinsCollected(int userId);
    void playerDied(int userId);
    void monsterKilled(int userId, int monsterId);
    template <typename TrophyType, typename UserIdType, typename LevelType, typename AmountType, typename RequirementType, typename SetType>
    void checkAndAddTrophy(TrophyType trophyType, UserIdType userId, LevelType level, AmountType amount, RequirementType requirement, const SetType& obtainedTrophiesSet){
        if( amount >= requirement ){
            if (!obtainedTrophiesSet.contains(level)){
                m_dbManager.addTrophy({userId, trophyType, level});
                qDebug() << "Added trophy " << trophyType << level;
            }
        }
    }
    template <typename TrophyType, typename UserIdType, typename AmountType, typename LevelRequirementsType>
    void processTrophies(TrophyType trophyType, UserIdType userId, AmountType amount, const LevelRequirementsType& levelRequirement){
        auto trophies = m_dbManager.getTrophiesOfType(userId, trophyType);
        QSet<int> trophies_levels;
        for(const auto& trophy: trophies){
            trophies_levels.insert(trophy.level());
        }
        for(int i=0; i < levelRequirement.size(); i++){
            checkAndAddTrophy(trophyType, userId, i+1, amount, levelRequirement[i], trophies_levels);
        }
    }
public:
    TrophySystem(DbManager& dbManager);
    ~TrophySystem() override;
    void eventHappened(DbEvent event, const DbListenerArgumentList) override;
};

#endif // TROPHYSYSTEM_H
