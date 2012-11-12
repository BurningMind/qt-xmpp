#include "auth.h"

namespace qtxmpp {

Auth::Auth(QString domain, Authentification *authentification, User *user)
{
    setText(false);
    setName("auth");
    setDomain(domain);
    //setAttribute("xmlns:stream", "http://etherx.jabber.org/streams");
    setXmlns("urn:ietf:params:xml:ns:xmpp-sasl");
    setMechanism(authentification);

    Serializable serializable;
    serializable.setText(true);

    QByteArray array;
    array.append(user->getNick());
    array.append('@');
    array.append(m_domain);
    array.append('\0');
    array.append(user->getNick());
    array.append('\0');
    array.append(user->getPassPrefix() + user->getPassword());

    serializable.setName(QString(array.toBase64()));

    addChild(serializable);
}

void Auth::setDomain(QString domain)
{
    m_domain = domain;
    m_attributes["to"] = m_domain;
}

void Auth::setMechanism(Authentification *mechanism)
{
    m_mechanism = mechanism;
    m_attributes["mechanism"] = mechanism->getName();
}

}
