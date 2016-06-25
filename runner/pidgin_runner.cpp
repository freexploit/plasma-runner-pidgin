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

#include <QDir>
#include <QtXml/QDomDocument>
#include <QFile>
#include <QFileInfo>
#include <QHash>
#include <QMutexLocker>
#include <QProcess>
#include <QIcon>
#include <QAction>
#include <QDebug>

#include <KLocalizedString>
#include <krun.h>

#include "pidgin_runner.h"
#include "pidgin_dbus.h"

//constructor
PidginRunner::PidginRunner(QObject *parent, const QVariantList& args)
    : Plasma::AbstractRunner(parent,args)
{
    Q_UNUSED(args);
    setObjectName( "Pidgin Contact Finder Runner" );
    setSpeed( AbstractRunner::SlowSpeed );
}

PidginRunner::~PidginRunner(){


}

void PidginRunner::match(Plasma::RunnerContext &context)
{
    qDebug() << Q_FUNC_INFO ;
    QString query = context.query();
    if ( !query.isEmpty() )
    {
        qDebug() << " Pidgin runner trigger word";
        QString contactName = query ;

        // so now let's filter all contacts from
        // pidgin
        QStringList contacts = pidgin_d.search(query);
        QList<Plasma::QueryMatch> matches;
        std::for_each(contacts.begin(), contacts.end(), [&contacts, &matches, this](const QString& c )
        {
            Plasma::QueryMatch match(this);
            QVariantMap map = pidgin_d.buddyId(c);
            match.setText(c);
            match.setSubtext(map["buddyStatus"].toString());
            match.setData(c);
            match.setId(c);
            match.setType(Plasma::QueryMatch::ExactMatch);
            QIcon icon(map["buddyIconPath"].toString());
            match.setIcon(icon);
            matches.append(match);

        });
        context.addMatches(matches);
    }
}




void PidginRunner::run(const Plasma::RunnerContext &context, const Plasma::QueryMatch &match){
    Q_UNUSED(context);
    qDebug() << match.text() << match.data().toString();
    pidgin_d.startChat(match.data().toString());
}
#include "moc_pidgin_runner.cpp"
