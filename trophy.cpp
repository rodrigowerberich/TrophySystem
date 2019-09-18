#include "trophy.h"

int Trophy::userId() const
{
    return m_userId;
}

void Trophy::setUserId(int userId)
{
    m_userId = userId;
}

Trophy::Type Trophy::type() const
{
    return m_type;
}

void Trophy::setType(const Trophy::Type &type)
{
    m_type = type;
}

int Trophy::level() const
{
    return m_level;
}

void Trophy::setLevel(int level)
{
    m_level = level;
}

std::tuple<int, Trophy::Type, int> Trophy::values() const
{
    return {m_userId, m_type, m_level};
}

Trophy::Trophy():
    Trophy{0, Trophy::Type::Undefined, 0}
{

}

Trophy::Trophy(int userId, Trophy::Type type, int level):
    m_userId{userId},
    m_type{type},
    m_level{level}
{

}

Trophy::Trophy(int userId, int type, int level):
    Trophy{userId, static_cast<Trophy::Type>(type), level}
{

}

Trophy::Trophy(std::tuple<int, int, int> values):
    Trophy{std::get<0>(values), std::get<1>(values), std::get<2>(values)}
{

}

Trophy::Trophy(std::tuple<int, Trophy::Type, int> values):
    Trophy{std::get<0>(values), std::get<1>(values), std::get<2>(values)}
{

}
