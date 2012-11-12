#ifndef IQ_H
#define IQ_H

#include "stanza.h"

namespace qtxmpp {

class IQ : public Stanza
{

public:
    explicit IQ();

    enum Type{
        Get,
        Set,
        Result
    };

    void setType(Type type);
    void setFrom(User *from);

protected:
    Type m_type;
    User* m_from;
    
};

}

#endif // IQ_H
