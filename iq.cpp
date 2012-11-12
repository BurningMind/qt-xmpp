#include "iq.h"

namespace qtxmpp {

IQ::IQ()
{
    setText(false);
    setName("iq");

    if(m_attributes["type"] == "get")
        setType(Get);
    else if(m_attributes["type"] == "set")
        setType(Set);
    else if(m_attributes["type"] == "result")
        setType(Result);
}

void IQ::setType(IQ::Type type)
{
    m_type = type;

    switch(m_type)
    {
        case Get:
            m_attributes["type"] =  "get";
            break;

        case Set:
            m_attributes["type"] =  "set";
            break;

        case Result:
            m_attributes["type"] =  "result";
            break;
    }
}

void IQ::setFrom(User *from)
{
    m_from = from;
    m_attributes["from"] = from->getJid();
}

}
