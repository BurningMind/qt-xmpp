#ifndef CONNECTION_H
#define CONNECTION_H

#include <QObject>
#include <QSslSocket>
#include <QDomDocument>
#include <QList>
#include <QByteArray>
#include <QStack>
#include <QApplication>

#include "user.h"
#include "extension.h"
#include "authentification.h"
#include "stanza.h"
#include "plain.h"
#include "auth.h"
#include "bind.h"
#include "session.h"
#include "presence.h"
#include "message.h"

namespace qtxmpp {

class Connection : public QObject
{
    Q_OBJECT
public:
    explicit Connection();

    enum State {
        None,
        Connected,
        Authed,
        AwaitingBind,
        AwaitingSession,
        Ready
    };
    
    void connectEncrypted();
    QSslSocket* getSocket();
    User* getUser();
    QDomDocument* getDomDocument();
    Extension* getExtension(int extension);
    QList<Extension*> getExtensions();
    User* getContact(int contact);
    QList<User*> getContacts();
    Authentification* getAuthentification();

    void setSocket(QSslSocket *socket);
    void setUser(User *user);
    void setDomDocument(QDomDocument *document);
    void setAuthentification(Authentification *authentification);
    void setIp(QString ip);
    void setPort(int port);
    void setDomain(QString domain);

    void addExtension(Extension *extension);
    void addExtensions(QList<Extension*> extensions);
    void addContact(User* contact);
    void addContacts(QList<User*> contacts);

    void removeExtension(Extension *extension);
    void removeExtensions(QList<Extension*> extensions);
    void removeContact(User* contact);
    void removeContacts(QList<User*> contacts);

    void sendExtension(Extension *extension);
    void sendStanza(Stanza *stanza);

    QByteArray getHeader();
    QByteArray getFooter();

protected slots:
    void socketEncrypted();
    void socketReadyRead();
    void sendMessage(Message message);
    void sslErrors(QList<QSslError> list);

signals:
    void chatReady();
    void messageReceived(Message);
    void newContactList(QList<User*>);

protected:
    QSslSocket *m_socket;
    User *m_user;
    QDomDocument *m_domDocument;
    QList<Extension*> m_extensions;
    QList<User*> m_contacts;
    Authentification *m_authentification;
    QByteArray m_buffer;
    State m_state;
    QString m_ip;
    int m_port;
    QString m_domain;

    void handleExtensions();
};

}

#endif // CONNECTION_H
