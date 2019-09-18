#include "dbmanager.h"
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>

namespace dbmanager{
    void executeQuery(const QString& query_str, bool ignore_error = false)
    {
        QSqlQuery query;
        if(!query.exec(query_str) && !ignore_error)
        {
            qDebug() << query_str << " error:\n"
                     << query.lastError();
        }
    }
    inline void createUsersTable(){
        executeQuery("CREATE TABLE IF NOT EXISTS users (id integer PRIMARY KEY)");
    }
    inline void createCollectedCoinsTable(){
        executeQuery("CREATE TABLE IF NOT EXISTS collected_coins (id integer PRIMARY KEY AUTOINCREMENT, user_id INTEGER, value INTEGER, FOREIGN KEY(user_id) REFERENCES users(id))");
    }
    inline void createMonstersTable(){
        executeQuery("CREATE TABLE IF NOT EXISTS monsters (id integer PRIMARY KEY AUTOINCREMENT, name TEXT UNIQUE)");
        executeQuery("INSERT INTO monsters (name) VALUES ('turtle')", true);
        executeQuery("INSERT INTO monsters (name) VALUES ('bowser')", true);
    }
    inline void createKilledMonsterTable(){
        executeQuery("CREATE TABLE IF NOT EXISTS killed_monsters (id integer PRIMARY KEY AUTOINCREMENT, user_id INTEGER, monster_id INTEGER, FOREIGN KEY(user_id) REFERENCES users(id), FOREIGN KEY(monster_id) REFERENCES monsters(id))");
    }
    inline void createDeathsTable(){
        executeQuery("CREATE TABLE IF NOT EXISTS deaths (id integer PRIMARY KEY AUTOINCREMENT, user_id INTEGER, timestamp DATETIME DEFAULT CURRENT_TIMESTAMP, FOREIGN KEY(user_id) REFERENCES users(id))");
    }
    inline void createTrophiesTable(){
        executeQuery("CREATE TABLE IF NOT EXISTS trophies (id integer PRIMARY KEY AUTOINCREMENT, user_id INTEGER, trophy_type_id INTEGER, trophy_level INTEGER, timestamp DATETIME DEFAULT CURRENT_TIMESTAMP, FOREIGN KEY(user_id) REFERENCES users(id), UNIQUE(user_id, trophy_type_id, trophy_level))");
    }
}

DbManager::DbManager(const QString &path)
{
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(path);

    if (!m_db.open()) {
        qDebug() << "Error: connection with database fail";
    } else {
        qDebug() << "Database: connection ok";
    }
}

DbManager::~DbManager()
{
    if(m_db.open()){
        m_db.close();
    }
}

int DbManager::addOrReturnUser(const QString& id_string)
{
    bool success = false;
    int id = id_string.toInt(&success);
    if(!success || id < 0){
        return -1;
    }
    // you should check if args are ok first...
    QSqlQuery query;
    query.prepare("INSERT INTO users (id) VALUES (:id)");
    query.bindValue(":id", id);
    if(!query.exec())
    {
        qDebug() << "addOrReturnUser error:"
                 << query.lastError();
    }
    return id;
}

void DbManager::printAllUsers()
{
    QSqlQuery query("SELECT * FROM users");
    int idId = query.record().indexOf("id");
    while(query.next())
    {
        int id = query.value(idId).toInt();
        qDebug() << id;
    }
}

bool DbManager::collectCoin(int userId, int coinAmount)
{
    if(coinAmount < 0){
        return false;
    }
    QSqlQuery query;
    query.prepare("INSERT INTO collected_coins (user_id, value) SELECT :user_id, :value WHERE EXISTS( SELECT * FROM users WHERE id=:user_id )");
    query.bindValue(":user_id", userId);
    query.bindValue(":value", coinAmount);
    if(query.exec()){
        triggerCoinsCollectedEvent(userId);
        return true;
    }
    return false;
}

int DbManager::coinsCollected(int userId)
{
    QSqlQuery query;
    query.prepare("SELECT SUM(value) from collected_coins WHERE user_id=:user_id");
    query.bindValue(":user_id", userId);
    if(!query.exec()){
        qDebug() << "coinsCollected error:"
                 << query.lastError();
        return -1;
    }
    query.next();
    return query.value(0).toInt();
}

bool DbManager::killUser(int userId)
{
    QSqlQuery query;
    query.prepare("INSERT INTO deaths (user_id) SELECT :user_id WHERE EXISTS( SELECT * FROM users WHERE id=:user_id )");
    query.bindValue(":user_id", userId);
    if(query.exec()){
       triggerUserDiedEvent(userId);
       return true;
    }
    return false;
}

bool DbManager::killUserMultipleTimes(int userId, int amount)
{
    QSqlQuery query;
    query.exec("BEGIN TRANSACTION");
    QString command = "INSERT INTO deaths (user_id) SELECT "+QString::number(userId)+" WHERE EXISTS( SELECT * FROM users WHERE id="+QString::number(userId)+" )";
    for(int i =0; i < amount; i++){
        query.exec(command);
    }
    if(query.exec("COMMIT")){
       triggerUserDiedEvent(userId);
       return true;
    }
    return false;
}

int DbManager::userDeathCount(int userId)
{
    QSqlQuery query;
    query.prepare("SELECT COUNT(id) from deaths WHERE user_id=:user_id");
    query.bindValue(":user_id", userId);
    if(!query.exec()){
        qDebug() << "userDeathCount error:"
                 << query.lastError();
        return -1;
    }
    query.next();
    return query.value(0).toInt();
}

bool DbManager::killMonster(int userId, int monsterId)
{
    QSqlQuery query;
    query.prepare("INSERT INTO killed_monsters (user_id, monster_id) SELECT :user_id, :monster_id WHERE EXISTS (SELECT * FROM users WHERE id=:user_id) AND EXISTS (SELECT * FROM monsters WHERE id=:monster_id)");
    query.bindValue(":user_id", userId);
    query.bindValue(":monster_id", monsterId);
    if(query.exec()){
       triggerMonsterKilledEvent(userId, monsterId);
       return true;
    }
    return false;
}

bool DbManager::killMonsters(int userId, int monsterId, int amount)
{
     QSqlQuery query;
     query.exec("BEGIN TRANSACTION");
     QString command = "INSERT INTO killed_monsters (user_id, monster_id) SELECT "+QString::number(userId)+ ", "+QString::number(monsterId)+" WHERE EXISTS (SELECT * FROM users WHERE id="+QString::number(userId)+") AND EXISTS (SELECT * FROM monsters WHERE id="+QString::number(monsterId)+")";
     for(int i =0; i < amount; i++){
         query.exec(command);
     }
     if(query.exec("COMMIT")){
        triggerMonsterKilledEvent(userId, monsterId);
        return true;
     }
     return false;
}

int DbManager::monsterKilled(int userId, int monsterId)
{
    QSqlQuery query;
    query.prepare("SELECT COUNT(id) from killed_monsters WHERE user_id=:user_id AND monster_id=:monster_id");
    query.bindValue(":user_id", userId);
    query.bindValue(":monster_id", monsterId);
    if(!query.exec()){
        qDebug() << "monsterKilled error:"
                 << query.lastError();
        return -1;
    }
    query.next();
    return query.value(0).toInt();
}

bool DbManager::addTrophy(Trophy trophy)
{
    auto [userId, trophyType, trophyLevel] = trophy.values();
    QSqlQuery query;
    query.prepare("INSERT INTO trophies (user_id, trophy_type_id, trophy_level) SELECT :user_id, :trophy_type_id, :trophy_level WHERE EXISTS (SELECT * FROM users WHERE id=:user_id)");
    query.bindValue(":user_id", userId);
    query.bindValue(":trophy_type_id", trophyType);
    query.bindValue(":trophy_level", trophyLevel);
    if(!query.exec()){
        qDebug() << "addTrophy error:"
                 << query.lastError();
        return false;
    }
    return true;
}

QVector<Trophy> DbManager::getTrophiesOfType(int userId, Trophy::Type trophyType)
{
    QSqlQuery query;
    query.prepare("SELECT user_id, trophy_type_id, trophy_level FROM trophies WHERE user_id=:user_id AND trophy_type_id=:trophy_type_id");
    query.bindValue(":user_id", userId);
    query.bindValue(":trophy_type_id", trophyType);
    if(!query.exec()){
        qDebug() << "getTrophiesOfType error:"
                 << query.lastError();
        return {};
    }
    QVector<Trophy> trophies;
    while(query.next()){
        trophies.push_back({query.value(0).toInt(), query.value(1).toInt(), query.value(2).toInt()});
    }
    return trophies;
}

QVector<Trophy> DbManager::getTrophies(int userId)
{
    QSqlQuery query;
    query.prepare("SELECT user_id, trophy_type_id, trophy_level FROM trophies WHERE user_id=:user_id");
    query.bindValue(":user_id", userId);
    if(!query.exec()){
        qDebug() << "getTrophiesOfType error:"
                 << query.lastError();
        return {};
    }
    QVector<Trophy> trophies;
    while(query.next()){
        trophies.push_back({query.value(0).toInt(), query.value(1).toInt(), query.value(2).toInt()});
    }
    return trophies;
}

void DbManager::addDbListener(DbListener *dbListener)
{
    m_event_listeners.insert(dbListener);
}

void DbManager::removeDbListener(DbListener *dbListener)
{
    m_event_listeners.remove(dbListener);
}

void DbManager::triggerEvent(DbEvent event, DbListenerArgumentList argument)
{
    for(const auto& listener:m_event_listeners){
        if(listener != nullptr){
            listener->eventHappened(event, argument);
        }
    }
}

void DbManager::triggerCoinsCollectedEvent(int userId)
{
    DbListenerArgumentList list{QString::number(userId)};
    triggerEvent(DbEvent::CoinsCollected, list);
}

void DbManager::triggerUserDiedEvent(int userId)
{
    DbListenerArgumentList list{QString::number(userId)};
    triggerEvent(DbEvent::PlayerDied, list);
}

void DbManager::triggerMonsterKilledEvent(int userId, int monsterId)
{
    DbListenerArgumentList list{QString::number(userId), QString::number(monsterId)};
    triggerEvent(DbEvent::MonsterKilled, list);
}


void DbManager::createAllTablesIfNotExists()
{
    dbmanager::createUsersTable();
    dbmanager::createCollectedCoinsTable();
    dbmanager::createMonstersTable();
    dbmanager::createKilledMonsterTable();
    dbmanager::createDeathsTable();
    dbmanager::createTrophiesTable();
}
