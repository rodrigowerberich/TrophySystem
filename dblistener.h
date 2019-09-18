#ifndef DBLISTENER_H
#define DBLISTENER_H

#include <QVector>
#include <QString>

using DbListenerArgumentList = QVector<QString>;

enum class DbEvent{
    CoinsCollected,
    PlayerDied,
    MonsterKilled
};

class DbListener{
public:
    virtual ~DbListener();
    virtual void eventHappened(DbEvent event, const DbListenerArgumentList arguments) = 0;


};

namespace dblistener {
template <typename ReturnType>
ReturnType getEventArguments(DbListenerArgumentList arguments);
}

#endif // DBLISTENER_H
