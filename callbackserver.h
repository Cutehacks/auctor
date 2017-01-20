#ifndef CALLBACKSERVER_H
#define CALLBACKSERVER_H

#include <QtCore/QObject>
#include <QtCore/QVariantMap>
#include <QtNetwork/QHostAddress>
#include <QtNetwork/QTcpServer>

namespace com { namespace cutehacks { namespace auctor {

class CallbackServer : public QTcpServer
{
    Q_OBJECT

    Q_PROPERTY(int port READ port WRITE setPort NOTIFY portChanged)
    Q_PROPERTY(QString address READ address WRITE setAddress NOTIFY addressChanged)

public:
    explicit CallbackServer(QObject *parent = 0);

    int port() const { return m_port; }
    QString address() const;

protected:
    void start();
    void stop();
    void restartServer();
    void incomingConnection(qintptr socketDescription);

signals:
    void portChanged(int port);
    void addressChanged(QString address);
    void callbackCalled(QVariantMap params);

public slots:
    void setPort(int port);
    void setAddress(QString address);
    void emitCallback(QVariantMap);

private:
    int m_port;
    QHostAddress m_address;
    QString m_code;
};

} } }

#endif // CALLBACKSERVER_H
