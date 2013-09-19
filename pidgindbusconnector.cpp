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

namespace
{
    const QString g_scIconPath="/usr/share/pixmaps/pidgin/status/48";
}

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

    QVariantMap status(int buddy)
    {
        QVariantMap ret ;
        ret["status"] = "Status not known";
        ret["icon"]="";
        int presence = call<int>("PurpleBuddyGetPresence",buddy);

        int isOnline = call<int>("PurplePresenceIsOnline",presence);
        int isAvailable = call<int>("PurplePresenceIsAvailable",presence);
        int isIdle = call<int>("PurplePresenceIsIdle",presence);

        if( isOnline)
        {
            ret["status"] = "Away";
            ret["icon"] = g_scIconPath + "/away.png";
        }
        else
        {
            ret["status"] = "Offline";
            ret["icon"] = g_scIconPath + "/offline.png";
        }

        if ( isAvailable )
        {
            ret["status"] = "Available";
            ret["icon"] = g_scIconPath + "/available.png";
        }

        if(isIdle)
        {
            ret["status"] = "Away";
            ret["icon"] = g_scIconPath + "/away.png";
        }
        return ret;
    }

    QString statusMessage(int buddy)
    {
        int presenceid = call<int>("PurpleBuddyGetPresence",buddy);
        int statusid = call<int>("PurplePresenceGetActiveStatus",presenceid);
        QString statusMessage = call<QString>("PurpleStatusGetAttrString",statusid,"message");
        return statusMessage;
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
            QVariantMap status = d_func()->status(buddyID);
            map["buddyStatus"] =status["status"];
            map["buddyStatusMessage"] = d_func()->statusMessage(buddyID);
            map["buddyIconPath"] =status["icon"];
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
 
void PidginDBusConnector::setStatus(const QString& status)
{
   QList<int> all_statuses = d_func()->call<QList<int>>("PurpleSavedstatusesGetAll");
   int status_id;
   QHash<int,int> status_items;
   
   for (int i =0;i<all_statuses.size() ;i++){
	
     int status_type =  d_func()->call<int>("PurpleSavedstatusGetType",all_statuses.at(i));
     status_items.insert(status_type,all_statuses.at(i));
     
  }
    //STATUS_OFFLINE = 1 STATUS_AVAILABLE = 2 STATUS_UNAVAILABLE = 3 STATUS_INVISIBLE = 4 STATUS_AWAY = 5
  
  if(status.contains("offline")){
    if(status_items.contains(1)){
      status_id=status_items.value(1);
    }
  }
  
  if(status.contains("online")){
    if(status_items.contains(2)){
      status_id=status_items.value(2);
    }
  }
  
  if(status.contains("busy")){
    if(status_items.contains(3)){
      status_id=status_items.value(3);
    }
  }
  
  if(status.contains("invisible")){
    if(status_items.contains(4)){
      status_id=status_items.value(4);
    }
  }
  if(status.contains("away")){
    if(status_items.contains(5)){
      status_id=status_items.value(5);
    }
  }
  QVariant v(status_id);
  int d = d_func()->call<int>("PurpleSavedstatusActive",v);
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
