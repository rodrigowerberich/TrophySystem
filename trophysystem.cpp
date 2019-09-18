#include "trophysystem.h"
#include <QDebug>
#include "trophy.h"
#include <QSet>
#include "constants.h"



void TrophySystem::coinsCollected(int userId)
{
    int coinsCollected = m_dbManager.coinsCollected(userId);
    processTrophies(Trophy::Type::CoinCollection, userId, coinsCollected, QVector<int>{1, 100, 1000, 10000, 100000});
}

void TrophySystem::playerDied(int userId)
{
    int userDeaths = m_dbManager.userDeathCount(userId);
    processTrophies(Trophy::Type::DeathCount, userId, userDeaths, QVector<int>{1, 10, 25, 50, 100});
}

void TrophySystem::monsterKilled(int userId, int monsterId)
{
    int monsterKillCount = m_dbManager.monsterKilled(userId, monsterId);
    switch (monsterId) {
    case constants::TURTLE_ID:{
        processTrophies(Trophy::Type::TurtleKiller, userId, monsterKillCount, QVector<int>{1, 100, 1000, 10000, 100000});
        break;
    }
    case constants::BOWSER_ID:{
        processTrophies(Trophy::Type::BowserKiller, userId, monsterKillCount, QVector<int>{1, 100, 1000, 10000, 100000});
        break;
    }
    default:
        break;
    }
}

TrophySystem::TrophySystem(DbManager &dbManager):
    m_dbManager{dbManager}
{
    m_dbManager.addDbListener(this);
}

TrophySystem::~TrophySystem()
{
    m_dbManager.removeDbListener(this);
}



void TrophySystem::eventHappened(DbEvent event, const DbListenerArgumentList arguments)
{
    switch (event) {
    case DbEvent::CoinsCollected:{
        int userId = dblistener::getEventArguments<int>(arguments);
        if(userId == -1){
            return;
        }else{
            coinsCollected(userId);
        }
        break;
    }
    case DbEvent::PlayerDied:{
        int userId = dblistener::getEventArguments<int>(arguments);
        if(userId == -1){
            return;
        }else{
            playerDied(userId);
        }
        break;
    }
    case DbEvent::MonsterKilled:{
        auto [userId, monsterId] = dblistener::getEventArguments<std::tuple<int, int>>(arguments);
        if((userId == -1) || (monsterId == -1)){
            return;
        }else{
            monsterKilled(userId, monsterId);
        }
        break;
    }

    }
}
