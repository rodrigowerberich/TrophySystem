#ifndef TROPHY_H
#define TROPHY_H

#include <algorithm>
#include <tuple>

class Trophy
{
public:
    enum Type{
        Undefined,
        CoinCollection,
        DeathCount,
        TurtleKiller,
        BowserKiller
    };
private:
    int m_userId;
    Trophy::Type m_type;
    int m_level;
public:
    Trophy();
    Trophy(int userId, Trophy::Type type, int level);
    Trophy(int userId, int type, int level);
    template <typename T>
    Trophy(const T & collection){
        auto size = std::distance(std::begin(collection), std::end(collection));
        if( 2 < size ){
            auto begin_iterator = std::begin(collection);
            m_userId = *begin_iterator;
            m_type = *(begin_iterator+1);
            m_level = *(begin_iterator+2);
        }
    }
    Trophy(std::tuple<int,int,int> values);
    Trophy(std::tuple<int,Trophy::Type,int> values);
    int userId() const;
    void setUserId(int userId);
    Trophy::Type type() const;
    void setType(const Trophy::Type &type);
    int level() const;
    void setLevel(int level);
    std::tuple<int, Trophy::Type, int> values() const;

};

#endif // TROPHY_H
