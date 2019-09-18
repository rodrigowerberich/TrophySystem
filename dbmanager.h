#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QString>
#include <QSqlDatabase>
#include <QSet>
#include "dblistener.h"
#include "trophy.h"
#include <QVector>

class DbManager
{
public:
    DbManager(const QString& path);
    ~DbManager();
    void createAllTablesIfNotExists();
    // User management
    int addOrReturnUser(const QString& id);
    void printAllUsers();
    // Coins management
    bool collectCoin(int userId, int coinAmount);
    int coinsCollected(int userId);
    // Player death management
    bool killUser(int userId);
    bool killUserMultipleTimes(int userId, int amount);
    int userDeathCount(int userId);
    // Monster death management
    bool killMonster(int userId, int monsterId);
    bool killMonsters(int userId, int monsterId, int amount);
    int monsterKilled(int userId, int monsterId);
    // Trophy management
    bool addTrophy(Trophy trophy);
    QVector<Trophy> getTrophiesOfType(int userId, Trophy::Type trophyType);
    QVector<Trophy> getTrophies(int userId);
    // Db listeners management
    void addDbListener(DbListener* dbListener);
    void removeDbListener(DbListener* dbListener);
private:
    QSqlDatabase m_db;
    QSet<DbListener*> m_event_listeners;
    void triggerEvent(DbEvent event, DbListenerArgumentList argument);
    void triggerCoinsCollectedEvent(int userId);
    void triggerUserDiedEvent(int userId);
    void triggerMonsterKilledEvent(int userId, int monsterId);
};

#endif // DBMANAGER_H
