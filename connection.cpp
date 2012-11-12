#include "connection.h"

namespace qtxmpp {

Connection::Connection()
{
    m_state = None;

    m_socket = new QSslSocket;

    m_user = new User;

    m_domDocument = new QDomDocument;

    connect(m_socket, SIGNAL(encrypted()), this, SLOT(socketEncrypted()));
    connect(m_socket, SIGNAL(sslErrors(QList<QSslError>)), this, SLOT(sslErrors(QList<QSslError>)));
    connect(m_socket, SIGNAL(readyRead()), this, SLOT(socketReadyRead()));
}

void Connection::connectEncrypted()
{
    m_socket->connectToHostEncrypted(m_ip, m_port);
}

QSslSocket *Connection::getSocket()
{
    return m_socket;
}

User *Connection::getUser()
{
    return m_user;
}

QDomDocument *Connection::getDomDocument()
{
    return m_domDocument;
}

Extension *Connection::getExtension(int extension)
{
    if(m_extensions.size() > extension)
        return m_extensions[extension];
    else
        return 0;
}

QList<Extension *> Connection::getExtensions()
{
    return m_extensions;
}

User* Connection::getContact(int contact)
{
    if(m_contacts.size() > contact)
        return m_contacts[contact];
    else
        return 0;
}

QList<User*> Connection::getContacts()
{
    return m_contacts;
}
Authentification *Connection::getAuthentification()
{
    return m_authentification;
}

void Connection::setSocket(QSslSocket *socket)
{
    m_socket = socket;
}

void Connection::setUser(User *user)
{
    m_user = user;
}

void Connection::setDomDocument(QDomDocument *document)
{
    m_domDocument = document;
}

void Connection::setAuthentification(Authentification *authentification)
{
    m_authentification = authentification;
}

void Connection::setIp(QString ip)
{
    m_ip = ip;
}

void Connection::setPort(int port)
{
    m_port = port;
}

void Connection::setDomain(QString domain)
{
    m_domain = domain;
}

void Connection::addExtension(Extension *extension)
{
    m_extensions.append(extension);
}

void Connection::addExtensions(QList<Extension *> extensions)
{
    QListIterator<Extension*> i(extensions);
    while (i.hasNext())
        m_extensions.append(i.next());
}

void Connection::addContact(User* contact)
{
    if(!m_contacts.contains(contact))
        m_contacts.append(contact);
}

void Connection::addContacts(QList<User*> contacts)
{
    QListIterator<User*> i(contacts);
    while (i.hasNext())
    {
        User* contact = i.next();
        if(!m_contacts.contains(contact))
            m_contacts.append(contact);
    }
}

void Connection::removeExtension(Extension *extension)
{
    m_extensions.removeAll(extension);
}

void Connection::removeContact(User* contact)
{
    if(m_contacts.contains(contact))
        m_contacts.removeAll(contact);
}

void Connection::removeContacts(QList<User*> contacts)
{
    QListIterator<User*> i(contacts);
    while (i.hasNext())
    {
        User* contact = i.next();
        if(m_contacts.contains(contact))
            m_contacts.removeAll(contact);
    }
}

void Connection::removeExtensions(QList<Extension *> extensions)
{
    QListIterator<Extension*> i(extensions);
    while (i.hasNext())
        m_extensions.removeAll(i.next());
}

void Connection::sendExtension(Extension *extension)
{
    qDebug() << Serializable::serialize(extension);
    m_socket->write(Serializable::serialize(extension));
}

void Connection::sendStanza(Stanza *stanza)
{
    m_socket->write(Serializable::serialize(stanza));
}

QByteArray Connection::getHeader()
{
    return QString("<?xml version='1.0'?><stream:stream version=\"1.0\" xmlns:stream=\"http://etherx.jabber.org/streams\" xmlns=\"jabber:client\" to=\"" + m_domain + "\">").toUtf8();
}

QByteArray Connection::getFooter()
{
    return "</stream:stream>";
}

void Connection::socketEncrypted()
{
    m_socket->write(getHeader());
    m_state = Connected;
}

void Connection::socketReadyRead()
{
    QByteArray content = m_socket->readAll();
    m_buffer += content;

    m_domDocument->setContent(m_buffer, false);

    switch(m_state)
    {
        case None:
            break;

        case Connected:
            qDebug() << content;
            if(content.contains("<stream:features"))
            {
                if(!m_domDocument->namedItem("stream:stream").isNull())
                {
                    QDomElement streamElement = m_domDocument->namedItem("stream:stream").toElement();
                    if(!streamElement.namedItem("stream:features").isNull())
                    {
                        QDomElement featuresElement = streamElement.namedItem("stream:features").toElement();
                        if(!featuresElement.namedItem("mechanisms").isNull())
                        {
                            QDomElement mechanismsElement = featuresElement.namedItem("mechanisms").toElement();
                            if(mechanismsElement.firstChildElement().text() == "PLAIN")
                            {
                                setAuthentification(new Plain);
                                addExtension(new Auth(m_domain, getAuthentification(), getUser()));
                            }
                        }
                    }
                }

                m_domDocument->clear();
                m_buffer.clear();
            }
            else if(content.contains("<success"))
            {
                m_state = Authed;
                m_socket->write(getHeader());

                m_domDocument->clear();
                m_buffer.clear();
            }

            break;

        case Authed:
            qDebug() << content;
            if(content.contains("<stream:features"))
            {
                if(!m_domDocument->namedItem("stream:stream").isNull())
                {
                    QDomElement streamElement = m_domDocument->namedItem("stream:stream").toElement();
                    if(!streamElement.namedItem("stream:features").isNull())
                    {
                        QDomElement featuresElement = streamElement.namedItem("stream:features").toElement();
                        if(!featuresElement.namedItem("bind").isNull())
                        {
                            addExtension(new Bind);
                        }
                        if(!featuresElement.namedItem("session").isNull())
                        {
                            addExtension(new Session);
                        }
                    }
                }

                m_domDocument->clear();
                m_buffer.clear();
            }
            if(content.contains("<iq type='result'><bind"))
            {
                if(!m_domDocument->namedItem("iq").isNull())
                {
                    QDomElement iqElement = m_domDocument->namedItem("iq").toElement();
                    if(!iqElement.namedItem("bind").isNull())
                    {
                        QDomElement bindElement = iqElement.namedItem("bind").toElement();
                        if(!bindElement.namedItem("jid").isNull())
                        {
                            QDomElement jidElement = bindElement.namedItem("jid").toElement();
                            m_user->setJid(jidElement.firstChild().toText().data().split("/").value(0));
                            m_user->setRessource(jidElement.firstChild().toText().data().split("/").value(1));
                        }
                    }
                }

                m_state = AwaitingSession;

                m_domDocument->clear();
                m_buffer.clear();
            }
            if(content.contains("<iq type='result'><session"))
            {
                m_state = AwaitingBind;

                m_domDocument->clear();
                m_buffer.clear();
            }

            break;

        case AwaitingBind:
            qDebug() << content;
            if(content.contains("<iq type='result'><bind"))
            {
                if(!m_domDocument->namedItem("iq").isNull())
                {
                    QDomElement iqElement = m_domDocument->namedItem("iq").toElement();
                    if(!iqElement.namedItem("bind").isNull())
                    {
                        QDomElement bindElement = iqElement.namedItem("bind").toElement();
                        if(!bindElement.namedItem("jid").isNull())
                        {
                            QDomElement jidElement = bindElement.namedItem("jid").toElement();
                            m_user->setJid(jidElement.firstChild().toText().data());
                        }
                    }
                }

                m_state = Ready;

                Serializable query;
                query.setName("query");
                query.setXmlns("jabber:iq:roster");

                IQ iq;
                iq.setType(IQ::Get);
                iq.setFrom(m_user);
                iq.addChild(query);

                sendStanza(&iq);

                m_domDocument->clear();
                m_buffer.clear();
            }
            break;

        case AwaitingSession:
            qDebug() << content;
            if(content.contains("<iq type='result'><session"))
            {
                m_state = Ready;

                Serializable query;
                query.setName("query");
                query.setXmlns("jabber:iq:roster");

                IQ iq;
                iq.setType(IQ::Get);
                iq.setFrom(m_user);
                iq.addChild(query);

                sendStanza(&iq);

                m_domDocument->clear();
                m_buffer.clear();
            }
            break;

        case Ready:
            qDebug() << content;

            if(content.contains("<presence"))
            {
                if(!m_domDocument->namedItem("presence").isNull())
                {
                    QDomElement presenceElement = m_domDocument->namedItem("presence").toElement();
                    if(!presenceElement.namedItem("status").isNull())
                    {
                        QDomElement statusElement = presenceElement.namedItem("status").toElement();
                        QListIterator<User*> i(m_contacts);
                        for(int j = 0; i.hasNext(); j++)
                        {
                            User *user = i.next();
                            if(presenceElement.attribute("from").split("/").value(0) == user->getJid())
                                m_contacts.value(j)->setStatus(statusElement.firstChild().toText().data());
                        }
                    }
                    if(!presenceElement.namedItem("show").isNull())
                    {
                        QDomElement showElement = presenceElement.namedItem("show").toElement();
                        QListIterator<User*> i(m_contacts);
                        for(int j = 0; i.hasNext(); j++)
                        {
                            User *user = i.next();
                            if(presenceElement.attribute("from").split("/").value(0) == user->getJid())
                            {
                                if(showElement.firstChild().toText().data() == "dnd")
                                    m_contacts.value(j)->setState(User::Busy);
                                else
                                    m_contacts.value(j)->setState(User::Connected);

                                m_contacts.value(j)->setRessource(presenceElement.attribute("from").split("/").value(1));

                            }
                        }
                    }
                    else if(presenceElement.attribute("type") == "unavailable")
                    {
                        QListIterator<User*> i(m_contacts);
                        for(int j = 0; i.hasNext(); j++)
                        {
                            if(presenceElement.attribute("from").split("/").value(0) == i.next()->getJid())
                            {
                                m_contacts.value(j)->setState(User::Disconnected);
                            }
                        }
                    }
                }

                emit newContactList(m_contacts);

                Serializable query;
                query.setName("query");
                query.setXmlns("jabber:iq:roster");

                IQ iq;
                iq.setType(IQ::Get);
                iq.setFrom(m_user);
                iq.addChild(query);

                sendStanza(&iq);

                m_domDocument->clear();
                m_buffer.clear();
            }

            if(content.contains("<query xmlns='jabber:iq:roster'"))
            {
                bool exists = false;
                for(QDomElement n = m_domDocument->namedItem("iq").namedItem("query").firstChildElement("item"); !n.isNull(); n = n.nextSiblingElement("item"))
                {
                    exists = false;
                    QListIterator<User*> i(m_contacts);
                    while(i.hasNext())
                    {
                        if(n.attribute("jid") == i.next()->getJid())
                        {
                            exists = true;
                        }
                    }

                    if(!exists)
                    {
                        User *user = new User;
                        user->setJid(n.attribute("jid"));
                        user->setNick(n.attribute("name"));
                        m_contacts.append(user);
                    }
                }

                emit newContactList(m_contacts);

                m_domDocument->clear();
                m_buffer.clear();

                if(m_user->getState() == User::Disconnected)
                {
                    m_user->setState(User::Connected);
                    sendStanza(new Presence);
                }
            }

            if(content.contains("<message"))
            {
                int contactId = 0;
                QListIterator<User*> i(m_contacts);
                for(int j = 0; i.hasNext(); j++)
                {
                    User* contact = i.next();
                    if(contact->getJid() == m_domDocument->namedItem("message").toElement().attribute("from").split("/").value(0))
                        contactId = j;
                }

                Message message;
                message.setFrom(getContact(contactId));
                message.setTo(m_user);
                message.setMessage(m_domDocument->namedItem("message").namedItem("body").firstChild().toText().data());
                message.setSubject(m_domDocument->namedItem("message").namedItem("subject").firstChild().toText().data());

                emit messageReceived(message);

                m_domDocument->clear();
                m_buffer.clear();
            }

            break;

    }

    handleExtensions();
}

void Connection::sendMessage(Message message)
{
    qDebug() << Serializable::serialize(&message);
    m_socket->write(Serializable::serialize(&message));
}

void Connection::sslErrors(QList<QSslError> list)
{
    for(int i = 0; i < list.size(); i++)
        qDebug() << "Error: " << list.value(i).errorString();
}

void Connection::handleExtensions()
{
    QListIterator<Extension*> i(m_extensions);
    while(i.hasNext())
    {
        sendExtension(i.next());
    }

    m_extensions.clear();
}

}

