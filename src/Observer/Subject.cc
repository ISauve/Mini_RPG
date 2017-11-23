#include "Subject.h"
#include "Observer.h"

void Subject::subscribe ( Observer * newView ) {
    observers_.insert( newView );
}


void Subject::unsubscribe ( Observer * formerView ) {
    observers_.erase( formerView );
}


void Subject::notify( Notification n ) {
    Observers::iterator i;
    for ( i = observers_.begin(); i != observers_.end(); ++i )
        ( * i )->update(n);
}