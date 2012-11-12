#include "presence.h"

namespace qtxmpp {

Presence::Presence()
{
    setText(false);
    setName("presence");
}

User *Presence::getFrom()
{
    return m_from;
}

User *Presence::getTo()
{
    return m_to;
}

void Presence::setTo(User *to)
{
    m_to = to;
    m_attributes["to"] = m_to->getJid();
}

void Presence::setFrom(User *from)
{
    m_from = from;
    m_attributes["from"] = m_from->getJid();
}

}
