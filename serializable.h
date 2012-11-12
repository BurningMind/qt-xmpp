#ifndef SERIALIZABLE_H
#define SERIALIZABLE_H

#include <QString>
#include <QMap>
#include <QStringList>
#include <QPair>

#include "user.h"

namespace qtxmpp {

class Serializable
{
    public:
        explicit Serializable();
        static QByteArray serialize(Serializable *serializable);
        static Serializable* deserialize();

        QString getXmlns();
        void setXmlns(QString xmlns);
        QString getName();
        void setName(QString name);
        QMap<QString, QString> getAttributes();
        QList<Serializable> getChildren();
        void addChild(Serializable serializable);
        Serializable getChild(int child);
        void deleteChildren();

        bool isText();
        void setText(bool text);

    protected:
        QString m_xmlns;
        QString m_name;

        QMap<QString, QString> m_attributes;
        QList<Serializable> m_children;

        bool m_text;
};

}

#endif // SERIALIZABLE_H
