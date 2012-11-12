#ifndef USER_H
#define USER_H

#include <QString>
#include <QVariant>

namespace qtxmpp {

class User
{

public:
    explicit User();

    enum State{
        Disconnected,
        Connected,
        Busy
    };

    QString getJid();
    QString getNick();
    QString getRessource();
    QString getPassword();
    QString getPassPrefix();
    QString getStatus();
    State getState();

    void setJid(QString jid);
    void setNick(QString nick);
    void setRessource(QString ressource);
    void setPassword(QString password);
    void setPassPrefix(QString passPrefix);
    void setStatus(QString status);
    void setState(State state);
    
protected:
    QString m_jid;
    QString m_nick;
    QString m_ressource;
    QString m_password;
    QString m_passPrefix;
    QString m_status;
    State m_state;
};

}

#endif // USER_H
