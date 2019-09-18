#include "dblistener.h"
#include <tuple>

namespace dblistener {

template<>
int getEventArguments(DbListenerArgumentList arguments){
    if(arguments.size() != 1){
        return -1;
    }
    bool success = false;
    int int0 = arguments[0].toInt(&success);
    return success? int0:-1;
}

template<>
std::tuple<int, int> getEventArguments(DbListenerArgumentList arguments){
    if(arguments.size() != 2){
        return {-1,-1};
    }
    bool success0 = false;
    bool success1 = false;
    int int0 = arguments[0].toInt(&success0);
    int int1 = arguments[1].toInt(&success1);
    return (success0 && success1)? (std::make_tuple(int0, int1)):(std::make_tuple(-1, -1));
}

}

DbListener::~DbListener()
{

}
