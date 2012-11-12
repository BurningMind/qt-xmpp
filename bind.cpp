#include "bind.h"

namespace qtxmpp {

Bind::Bind()
{
    setText(false);
    IQ iq;
    Serializable serializable;

    setName(iq.getName());
    iq.setType(IQ::Set);

    QMapIterator<QString, QString> i(iq.getAttributes());
    while(i.hasNext())
    {
        i.next();
        m_attributes[i.key()] = i.value();
    }
    serializable.setName("bind");
    serializable.setXmlns("urn:ietf:params:xml:ns:xmpp-bind");
    addChild(serializable);
}

}
