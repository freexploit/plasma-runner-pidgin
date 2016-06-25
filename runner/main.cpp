


#include <iostream>
#include "pidgin_dbus.h"
#include <QCoreApplication>
#include <QObject>
#include <QDebug>
#include <QMap>
#include <QVariant>
#include <QVariantMap>
#include <QAction>
#include <QDebug>

using namespace std;

int main(int argc,char* argv[])
{
    Q_UNUSED(argc);
    Q_UNUSED(argv);

    QCoreApplication app(argc,argv);

    PidginDbus p;
    QStringList l;

    if (argc > 1)
    {
        QString param = app.arguments().at(1);

        if (!param.isEmpty()){
            if(param.contains("--search")){
                QString pattern = app.arguments().at(2);
                l = p.search(pattern);
                for(int i = 0;i< l.size();i++) {
                      qDebug() << l.at(i);
                }
                QCoreApplication::exit(0);
            }
            if(param.contains("--startChat")){
                QString pattern = app.arguments().at(2);
                p.startChat(pattern);
            }

        }
        else{
            QCoreApplication::exit(1);
        }


    }
    else{

        QCoreApplication::exit(1);

    }

    //return app.exec();



}


