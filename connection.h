#ifndef CONNECTION_H
#define CONNECTION_H

#include <QtCore/QObject>
#include <QtCore/QRunnable>
#include <QtCore/QVariantMap>

namespace com { namespace cutehacks { namespace auctor {

class Connection : public QObject, public QRunnable
{
    Q_OBJECT

public:
    explicit Connection(QObject *parent, qintptr socketDescriptor);
    void run();

signals:
    void receivedParams(QVariantMap);

private:
    qintptr m_socketDescriptor;
};

} } }

#endif // CONNECTION_H
