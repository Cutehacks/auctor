
#include <QtCore/QCoreApplication>
#include <QtCore/QThreadPool>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QHostAddress>
#include <QtQml/qqml.h>

#include <QDebug>

#include "connection.h"
#include "callbackserver.h"
#include "enums.h"

namespace com { namespace cutehacks { namespace auctor {

CallbackServer::CallbackServer(QObject *parent) :
    QTcpServer(parent),
    m_port(9100),
    m_address(QHostAddress::LocalHost)
{
    start();
}

QString CallbackServer::address() const {
    if (m_address == QHostAddress::LocalHost)
        return QStringLiteral("localhost");
    return m_address.toString();
}

void CallbackServer::start()
{
    bool success = listen(m_address, (quint16)m_port);
    if (!success)
        qWarning("Could not bind to port: %s", qUtf8Printable(errorString()));
}

void CallbackServer::stop()
{
    if (isListening())
        close();
}

void CallbackServer::restartServer()
{
    stop();
    start();
}

void CallbackServer::incomingConnection(qintptr socketDescriptor)
{
    // Hand off the handling to a separate thread
    Connection *conn = new Connection(this, socketDescriptor);
    connect(conn, SIGNAL(receivedParams(QVariantMap)), this, SLOT(emitCallback(QVariantMap)));
    QThreadPool::globalInstance()->start(conn);
}

void CallbackServer::setPort(int port)
{
    if (m_port == port)
        return;

    m_port = port;
    emit portChanged(port);
    restartServer();
}

void CallbackServer::setAddress(QString address)
{
    QHostAddress newAddress(address);
    if (m_address == newAddress)
        return;

    m_address = newAddress;
    emit addressChanged(m_address.toString());
    restartServer();
}

void CallbackServer::emitCallback(QVariantMap params)
{
    emit callbackCalled(params);
}



static void registerTypes() {
    qmlRegisterType<CallbackServer>(
        "com.cutehacks.auctor",
        1, 0,
        "CallbackServer");
    qmlRegisterUncreatableType<GrantType>(
        "com.cutehacks.auctor",
        1, 0,
        "GrantType", "Class just contains an enumeration");
}

Q_COREAPP_STARTUP_FUNCTION(registerTypes)

} } }
