#ifndef MINI_RPG_OBSERVER_H
#define MINI_RPG_OBSERVER_H

#include "Model/Notification.h"

class Subject;

class Observer {

public:
    virtual void update (Notification) = 0;

};


#endif //MINI_RPG_OBSERVER_H
