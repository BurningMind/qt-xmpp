#ifndef PRESENCE_H
#define PRESENCE_H

#include "stanza.h"

namespace qtxmpp {

class Presence : public Stanza
{

public:
    explicit Presence();

    User* getFrom();
    User* getTo();

    void setFrom(User *from);
    void setTo(User *to);
    
protected:
    User* m_from;
    User* m_to;
    
};

}

#endif // PRESENCE_H
