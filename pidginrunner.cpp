#include "pidginrunner.h"
#include "pidgindbusconnector.h"

#include <QtCore/QDebug>
#include <QtGui/QIcon>
#include <KDebug>

#include <algorithm>

namespace
{
    QString g_sTriggerWord = "pidgin";
}

class PidginRunnerPrivate
{
public:
    PidginDBusConnector m_connector ;

};

PidginRunner::PidginRunner(QObject *parent, const QVariantList &args) :
    Plasma::AbstractRunner(parent),
    d_ptr(new PidginRunnerPrivate)
{
    kWarning() << Q_FUNC_INFO ;
    setSpeed(SlowSpeed);
    setPriority(HighPriority);
    setHasRunOptions(false);
}

void PidginRunner::match(Plasma::RunnerContext &context)
{

    kDebug() << Q_FUNC_INFO ;
    QString query = context.query();
    if ( !query.isEmpty() && !query.contains("status"))
    {
        kDebug() << " Pidgin runner trigger word";
        QString contactName = query ;

        // so now let's filter all contacts from
        // pidgin
        QStringList contacts = d_func()->m_connector.filter(query);
        QList<Plasma::QueryMatch> matches;
        std::for_each(contacts.begin(), contacts.end(), [&contacts, &matches, this](const QString& c )
        {
            Plasma::QueryMatch match(this);
            QVariantMap map = d_func()->m_connector.buddyId(c);
            match.setText(c);
            match.setSubtext(map["buddyStatus"].toString());
            match.setData(c);
            match.setId(c);
            match.setType(Plasma::QueryMatch::ExactMatch);
            QIcon icon(map["buddyIconPath"].toString());
            match.setIcon(icon);
            matches.append(match);

        });
        context.addMatches(context.query(), matches);
    }
    else{
     
      QStringList statuses = {"online","offline","busy","invisible","away"};
      QString status=query.remove("status");
      //show matches
    }

}

void PidginRunner::run(const Plasma::RunnerContext &context, const Plasma::QueryMatch &match)
{
    Q_UNUSED(context);
    kDebug() << match.text() << match.data().toString();
    if(match.data().toString().contains("status ")){
      QString status= match.data().toString().remove("status ");
      d_func()->m_connector.setStatus(status);
    }
    else{
      d_func()->m_connector.startChatWith(match.data().toString());
    }
}

QStringList PidginRunner::contacts()
{
    return QStringList() ;
}

void PidginRunner::init()
{
    kDebug() << Q_FUNC_INFO ;
    // initialize dbus connection
    d_func()->m_connector.init();
}

void PidginRunner::prepareForMatchSession()
{
}

void PidginRunner::matchSessionFinished()
{
}

K_EXPORT_PLASMA_RUNNER(pidgin-runner, PidginRunner)
#include "pidginrunner.moc"
