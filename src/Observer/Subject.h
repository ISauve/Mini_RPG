#ifndef MINI_RPG_SUBJECT_H
#define MINI_RPG_SUBJECT_H

#include "../Model/Notification.h"
#include <set>
class Observer;

class Subject {
    typedef std::set< Observer* > Observers;
    Observers observers_;

public:
    void subscribe( Observer* );
    void unsubscribe( Observer* );

protected:
    void notify(Notification);
};



#endif //MINI_RPG_SUBJECT_H
