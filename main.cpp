#include <iostream>

#include "pidgindbusconnector.h"

#include <QtCore/QDebug>
#include <QtCore/QCoreApplication>
#include <QtCore/QStringList>

using namespace std;

int main(int argc, char* argv[])
{
    Q_UNUSED(argc);
    Q_UNUSED(argv);

    QCoreApplication app(argc,argv);

    PidginDBusConnector conn ;
    conn.init();


    if ( argc > 1 )
    {
        const std::string param=argv[1];
        if ( param.compare("--list") == 0 )
        {
            conn.allContacts();
//            conn.startChatWith("glaza");
        }
    }

    return app.exec();
}

