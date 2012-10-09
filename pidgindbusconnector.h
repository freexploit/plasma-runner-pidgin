#ifndef PIDGINDBUSCONNECTOR_H
#define PIDGINDBUSCONNECTOR_H

#include <QObject>
#include <QVariantMap>

class PidginDBusConnectorPrivate ;
class PidginDBusConnector : public QObject
{
    Q_OBJECT
public:
    explicit PidginDBusConnector(QObject *parent = 0);
    
    QStringList allContacts() ;
    QStringList filter(const QString& contactName = "");
    QVariantMap buddyId( const QString& buddyName );
signals:
    
public slots:
    void init() ;
    void deinit();
    void startChatWith(const QString& contactName) ;
protected:
    PidginDBusConnectorPrivate* const d_ptr ;
    Q_DECLARE_PRIVATE(PidginDBusConnector)
    
};

#endif // PIDGINDBUSCONNECTOR_H
