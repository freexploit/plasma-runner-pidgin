#ifndef PIDGINRUNNER_H
#define PIDGINRUNNER_H

#include <plasma/abstractrunner.h>
#include <QStringList>

class PidginRunnerPrivate;
class PidginRunner : public Plasma::AbstractRunner
{
    Q_OBJECT
public:
    explicit PidginRunner(QObject *parent, const QVariantList &args);

    void match(Plasma::RunnerContext &context);
    void run(const Plasma::RunnerContext &context, const Plasma::QueryMatch &match);

    QStringList contacts() ;
//    void createRunOptions(QWidget *widget);
//    void reloadConfiguration();
    
public Q_SLOTS:
    /*virtual */ void init() ;
    /*virtual */ void prepareForMatchSession() ;
    /*virtual */ void matchSessionFinished() ;
protected:
    PidginRunnerPrivate * const d_ptr;
private:
    Q_DECLARE_PRIVATE(PidginRunner);
};

#endif // PIDGINRUNNER_H
