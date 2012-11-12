#include "user.h"

namespace qtxmpp {

User::User()
{
    m_state = Disconnected;
}

QString User::getJid()
{
    return m_jid;
}

QString User::getNick()
{
    return m_nick;
}

QString User::getRessource()
{
    return m_ressource;
}

QString User::getPassword()
{
    return m_password;
}

QString User::getPassPrefix()
{
    return m_passPrefix;
}

QString User::getStatus()
{
    return m_status;
}

User::State User::getState()
{
    return m_state;
}

void User::setJid(QString jid)
{
    m_jid = jid;
}

void User::setNick(QString nick)
{
    m_nick = nick;
}

void User::setRessource(QString ressource)
{
    m_ressource = ressource;
}

void User::setPassword(QString password)
{
    m_password = password;
}

void User::setPassPrefix(QString passPrefix)
{
    m_passPrefix = passPrefix;
}

void User::setStatus(QString status)
{
    m_status = status;
}

void User::setState(User::State state)
{
    m_state = state;
}

}
