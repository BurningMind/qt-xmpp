#ifndef AUTH_H
#define AUTH_H

#include "extension.h"
#include "authentification.h"
#include "user.h"

namespace qtxmpp {

class Auth : public Extension
{

public:
    explicit Auth(QString domain, Authentification *authentification, User *user);

    void setDomain(QString domain);
    void setMechanism(Authentification *mechanism);

protected:
    QString m_domain;
    Authentification *m_mechanism;
};

}

#endif // AUTH_H
