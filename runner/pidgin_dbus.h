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
 *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#ifndef PIDGIN_DBUS_H
#define PIDGIN_DBUS_H
#include <QObject>
#include <QVariant>
#include <QDBusConnection>
#include <QDBusInterface>
#include <QDBusReply>
#include <QDebug>
#include <QMutex>

class PidginDbus : public QObject
{
    Q_OBJECT;
    private:
        QDBusInterface pidgin_d;
        QMutex mutex;
        QMap<QString,QVariantMap> people;
        QVariantMap getStatus(int);
        QString getStatusMessage(int);


        //start template
        template<typename T>
        T call(const QString& methodName,const QVariant & arg1 = QVariant(),const QVariant & arg2 = QVariant(),const QVariant & arg3 = QVariant())
        {
            T ret = T() ;
            if ( pidgin_d.isValid() )
            {
                QDBusReply<T> reply = pidgin_d.call(methodName,arg1,arg2,arg3);
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
    //end template
    public:
        PidginDbus();
        ~PidginDbus();
        void startChat(const QString &contact);
        QStringList getContacts();
        QStringList search(const QString &contactName);
        QVariantMap buddyId(const QString &buddyName);
    //signals:

    //public slots:
        //void init() ;
        //void deinit();
        //void startChatWith(const QString& contactName) ;
    





};







#endif
