#ifndef MESSAGE_H
#define MESSAGE_H

#include "stanza.h"

namespace qtxmpp {

class Message : public Stanza
{

public:
    explicit Message();

    QString getMessage();
    QString getSubject();
    User* getFrom();
    User* getTo();

    void setMessage(QString message);
    void setSubject(QString subject);
    void setFrom(User *from);
    void setTo(User *to);

protected:
    QString m_message;
    QString m_subject;
    User* m_from;
    User* m_to;
};

}

#endif // MESSAGE_H
