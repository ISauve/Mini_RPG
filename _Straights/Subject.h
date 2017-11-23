/*
 * MVC example of GTKmm program
 *
 * Subject class.  Abstract class for Observer Pattern
 *
 */


#ifndef MVC_SUBJECT_H
#define MVC_SUBJECT_H

#include <set>

class Observer;

class Subject {
    typedef std::set< Observer * > Observers;
    Observers observers_;

public:
       void subscribe( Observer * );
       void unsubscribe( Observer * );

protected:
       void notify();
}; // Subject


#endif
