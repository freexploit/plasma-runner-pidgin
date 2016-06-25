/*
 *   Copyright (C) 2016 Christopher Valerio <christopher@valerio.guru>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details
 *
 *   You should have received a copy of the GNU General Public
 *   License along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include <QtDBus>
#include <QDBusConnection>
#include <QDBusInterface>
#include <QDBusReply>
#include <QDBusMessage>
#include <QMutex>
#include <QMutexLocker>

#include "pidgin_dbus.h"

namespace
{
        const QString pidgin_icon="/usr/share/pixmaps/pidgin/status/48";
}



PidginDbus::PidginDbus ():
pidgin_d("im.pidgin.purple.PurpleService","/im/pidgin/purple/PurpleObject","im.pidgin.purple.PurpleService")
{
    QMutexLocker ml (&mutex);
    typedef QList<int> IntList;
    IntList accounts = call<IntList>("PurpleAccountsGetAllActive");

    std::for_each(accounts.begin(),accounts.end(),[&](int accId){
        QString accountName=call<QString>("PurpleAccountGetUsername",accId);
        IntList buddies = call<IntList>("PurpleFindBuddies",accId,"");
        std::for_each( buddies.begin(),buddies.end(),[&](int buddyID){
            QVariantMap map;
            QString buddyName = call<QString>("PurpleBuddyGetAlias",buddyID);
            map["accountID"] = accId ;
            map["accountName"] = accountName ;
            map["buddyID"] = buddyID ;
            map["buddyAlias"] = buddyName ;
            map["buddyName"] = call<QString>("PurpleBuddyGetName",buddyID);
            this->people[buddyName]=map;
        });


    });
}

PidginDbus::~PidginDbus(){
}


QStringList PidginDbus::getContacts()
{
    return this->people.keys();
}

QVariantMap PidginDbus::getStatus(int buddy)
{
    QVariantMap statusMap;
    statusMap["status"]= "Status not found";
    statusMap["icon"]="";

    int presence = call<int>("PurpleBuddyGetPresence",buddy);
    int isOnline = call<int>("PurplePresenceIsOnline",presence);
    int isAvailable = call<int>("PurplePresenceIsAvailable",presence);
    int isIdle = call<int>("PurplePresenceIsIdle",presence);

    if( isOnline)
    {
        statusMap["status"] = "Away";
        statusMap["icon"] = pidgin_icon + "/away.png";
    }
    else
    {
        statusMap["status"] = "Offline";
        statusMap["icon"] = pidgin_icon + "/offline.png";
    }

    if ( isAvailable )
    {
        statusMap["status"] = "Available";
        statusMap["icon"] = pidgin_icon + "/available.png";
    }

    if(isIdle)
    {
        statusMap["status"] = "Away";
        statusMap["icon"] = pidgin_icon + "/away.png";
    }

    return statusMap;


}

void PidginDbus::startChat(const QString &contactName)
{

    QStringList  keys = this->people.keys();
    QString name;

    std::for_each(keys.begin(),keys.end(),[&contactName,&name](const QString &s){
        if(s.contains(contactName,Qt::CaseInsensitive)){
            name = s;

        }
    });

    if (this->people.contains(name)){

        QVariantMap contact = this->people[name];
        qDebug() << contact;
        call<int>("PurpleConversationNew",
                     1,
                     contact["accountID"],
                     contact["buddyName"]);
    }
}

QStringList PidginDbus::search(const QString &contactName)
{
    QMutexLocker ml(&mutex);
    QStringList filtered;
    if (contactName.isEmpty())
    {

        filtered = this->getContacts();
    }
    else
    {
        QStringList keys = this->people.keys();
        std::for_each(keys.begin(),keys.end(),[&contactName,&filtered](const QString& s){
            if ( s.contains(contactName,Qt::CaseInsensitive) )
            {
                filtered.append(s);
            }
        });
    
    }
        

    return filtered;

}
QVariantMap PidginDbus::buddyId(const QString &buddyName)
{
    QMutexLocker ml(&mutex);
    QVariantMap buddy ;

    if ( people.contains(buddyName))
    {
        buddy = people[buddyName];

    }

    return buddy ;

}



#include "moc_pidgin_dbus.cpp"



