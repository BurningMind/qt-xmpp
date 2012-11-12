#include "serializable.h"

namespace qtxmpp {

Serializable::Serializable()
{
    setText(false);
}

QByteArray Serializable::serialize(Serializable *serializable)
{
    if(serializable->isText())
        return serializable->getName().toUtf8();
    else
    {
        QByteArray byteArray;
        byteArray.append("<" + serializable->getName());
        QMapIterator<QString, QString> i(serializable->getAttributes());
        while(i.hasNext())
        {
            i.next();
            byteArray.append(" " + i.key() + "='" + i.value() + "'");
        }
        byteArray.append(">");

        QListIterator<Serializable> j(serializable->getChildren());
        while(j.hasNext())
        {
            Serializable ser = j.next();
            byteArray.append(Serializable::serialize(&ser));
        }

        byteArray.append("</" + serializable->getName() + ">");
        return byteArray;
    }
}

Serializable* Serializable::deserialize()
{
    return new Serializable();
}

QString Serializable::getXmlns()
{
    return m_xmlns;
}

QString Serializable::getName()
{
    return m_name;
}

void Serializable::setXmlns(QString xmlns)
{
    m_xmlns = xmlns;
    m_attributes["xmlns"] = m_xmlns;
}

void Serializable::setName(QString name)
{
    m_name = name;
}

QMap<QString, QString> Serializable::getAttributes()
{
    return m_attributes;
}

QList<Serializable> Serializable::getChildren()
{
    return m_children;
}

void Serializable::addChild(Serializable serializable)
{
    m_children.append(serializable);
}

Serializable Serializable::getChild(int child)
{
    return m_children.value(child);
}

void Serializable::deleteChildren()
{
    m_children.clear();
}

bool Serializable::isText()
{
    return m_text;
}

void Serializable::setText(bool text)
{
    m_text = text;
}

}

