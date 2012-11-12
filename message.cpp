#include "message.h"

namespace qtxmpp {

Message::Message()
{
    m_attributes["type"] = "chat";

    Serializable serializable;
    serializable.setName("body");
    serializable.setText(false);

    Serializable serializable2;
    serializable2.setName("");
    serializable2.setText(true);

    setText(false);
    setName("message");
    serializable.addChild(serializable2);
    addChild(serializable);
}

QString Message::getMessage()
{
    return m_message;
}

QString Message::getSubject()
{
    return m_subject;
}

User *Message::getFrom()
{
    return m_from;
}

User *Message::getTo()
{
    return m_to;
}

void Message::setMessage(QString message)
{
    m_message = message;

    deleteChildren();

    Serializable serializable;
    serializable.setName("body");
    serializable.setText(false);

    Serializable serializable2;
    serializable2.setName(message);
    serializable2.setText(true);

    setText(false);
    setName("message");
    serializable.addChild(serializable2);
    addChild(serializable);
}

void Message::setSubject(QString subject)
{
    m_subject = subject;
}

void Message::setTo(User *to)
{
    m_to = to;
    m_attributes["to"] = to->getJid() + "/" + to->getRessource();
}

void Message::setFrom(User *from)
{
    m_from = from;
    m_attributes["from"] = from->getJid() + "/" + from->getRessource();
}

}
