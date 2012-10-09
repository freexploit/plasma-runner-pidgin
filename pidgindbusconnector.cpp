#include "pidgindbusconnector.h"

#include <QtDBus/QDBusConnection>
#include <QtDBus/QDBusMessage>
#include <QtDBus/QDBusInterface>
#include <QtDBus/QDBusReply>
#include <QtCore/QDebug>
#include <QtCore/QStringList>
#include <QtCore/QMutex>
#include <QtCore/QMutexLocker>
#include <QtGui/QIcon>

#include <algorithm>


class PidginDBusConnectorPrivate
{
public:
    PidginDBusConnectorPrivate():
        m_iface("im.pidgin.purple.PurpleService","/im/pidgin/purple/PurpleObject","im.pidgin.purple.PurpleInterface",QDBusConnection::sessionBus())
    {}

    template<typename T>
    T call(const QString& methodName,const QVariant & arg1 = QVariant(),const QVariant & arg2 = QVariant(),const QVariant & arg3 = QVariant())
    {
        T ret = T() ;
        if ( m_iface.isValid() )
        {
            QDBusReply<T> reply = m_iface.call(methodName,arg1,arg2,arg3);
            if ( reply.isValid() )
            {
                ret = reply.value();
            }
            else{
                qWarning() << "Metod name:" << methodName<< "reply not valid " <<reply.error().message();
            }
        }
        else
        {
            qWarning() << " IFACE not valid";
        }
        return ret ;
    }

    QDBusInterface m_iface;
    typedef QVariantMap BuddiesList ;
    QMap<QString,QVariantMap> m_buddies ;
    QMutex m_mutex ;

};

PidginDBusConnector::PidginDBusConnector(QObject *parent) :
    QObject(parent),
    d_ptr( new PidginDBusConnectorPrivate)
{
}

void PidginDBusConnector::init()
{
    QMutexLocker ls(&(d_func()->m_mutex));

    typedef QList<int> IntList ;
    IntList accounts = d_func()->call<IntList>("PurpleAccountsGetAllActive");
    std::for_each(accounts.begin(), accounts.end(), [&](int accId ){

        QString accountName=d_func()->call<QString>("PurpleAccountGetUsername",accId);
        IntList buddies = d_func()->call<IntList>("PurpleFindBuddies",accId,"");
        std::for_each( buddies.begin(),buddies.end(),[&](int buddyID){
            QString buddyName = d_func()->call<QString>("PurpleBuddyGetAlias",buddyID);
            QVariantMap map ;
            map["accountID"] = accId ;
            map["accountName"] = accountName ;
            map["buddyID"] = buddyID ;
            map["buddyAlias"] = buddyName ;
            map["buddyName"] = d_func()->call<QString>("PurpleBuddyGetName",buddyID);
            QVariant pres = d_func()->call<int>("PurpleBuddyGetPresence",buddyID);
            QString pres1 = d_func()->call<QString>("PurplePresenceGetChatUser",pres);
            qDebug() << pres1 ;
            map["buddyPresence"] = pres1 ;
            d_func()->m_buddies[buddyName] = map ;
        });
    });
}

void PidginDBusConnector::deinit()
{
}

void PidginDBusConnector::startChatWith(const QString &contactName)
{
    QStringList keys = d_func()->m_buddies.keys();

    QString name;
    std::for_each(keys.begin(),keys.end(),[&contactName,&name](const QString& s){
        if ( s.contains(contactName,Qt::CaseInsensitive) )
        {
            name = s ;
        }
    });

    if(d_func()->m_buddies.contains(name))
    {
        QVariantMap contact = d_func()->m_buddies[name];
        qDebug() << contact ;
        d_func()->call<int>("PurpleConversationNew",
                             1,
                             contact["accountID"],
                             contact["buddyName"]);
    }
}

QStringList PidginDBusConnector::allContacts()
{
    return d_func()->m_buddies.keys();
}

QStringList PidginDBusConnector::filter(const QString &contactName)
{
    QMutexLocker ls(&(d_func()->m_mutex));

    QStringList filtered;
    if ( contactName.isEmpty())
    {
        filtered = allContacts();
    }
    else
    {
        QStringList keys = d_func()->m_buddies.keys();
        std::for_each(keys.begin(),keys.end(),[&contactName,&filtered](const QString& s){
            if ( s.contains(contactName,Qt::CaseInsensitive) )
            {
                filtered.append(s);
            }
        });
    }
    return filtered ;
}

QVariantMap PidginDBusConnector::buddyId(const QString &buddyName)
{
    QMutexLocker ls(&(d_func()->m_mutex));
    QVariantMap ret ;

    if ( d_func()->m_buddies.contains(buddyName))
    {
        ret = d_func()->m_buddies[buddyName];

    }

    return ret ;

}

Q_DECLARE_METATYPE(QList<int>)
#include "pidgindbusconnector.moc"
