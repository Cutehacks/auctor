#include <QtCore/QUrlQuery>
#include <QtCore/QDateTime>
#include <QtCore/QStringBuilder>
#include <QtNetwork/QTcpSocket>

#include "connection.h"

namespace com { namespace cutehacks { namespace auctor {

static const char* RESPONSE_HEADER =
    "HTTP/1.1 200 OK\n"
    "content-type: text/html; charset=utf-8\n"
    "Cache-Control: no-cache\n"
    "Expires: Fri, 01 Jan 1990 00:00:00 GMT\n"
    "Content-Length: %1\n"
    "Server: Auctor/1.0\n"
    "Date: %2\n"
    "\n";

static const char* RESPONSE_BODY =
    "<!DOCTYPE html>\n"
    "<html lang=\"en\">\n"
    "<head>\n"
    "</head>\n"
    "<body>\n"
    "    <p>You may now close this tab.</p>\n"
    "</body>\n"
    "</html>\n";

Connection::Connection(QObject *parent, qintptr socketDescriptor) :
    QObject(parent),
    m_socketDescriptor(socketDescriptor)
{}

void Connection::run()
{
    QTcpSocket *socket = new QTcpSocket();
    socket->setSocketDescriptor(m_socketDescriptor);

    if (socket->waitForReadyRead()) {
        QList<QByteArray> requestLine = socket->readLine().split(QChar::Space);
        QString method = requestLine.at(0);
        QString uri = requestLine.at(1);
        int start = uri.indexOf("?") + 1;
        if (start > 0) {
            QVariantMap params;
            QUrlQuery query(uri.mid(start));
            QList<QPair<QString, QString> > items = query.queryItems();
            QList<QPair<QString, QString> >::const_iterator it = items.constBegin();
            while (it != items.constEnd()) {
                params.insert((*it).first, (*it).second);
                it++;
            }
            emit receivedParams(params);
        }

        // Write a response
        const QDateTime t = QDateTime::currentDateTime();
        QString response = QString(RESPONSE_HEADER)
                .arg(qstrlen(RESPONSE_BODY))
                .arg(t.toString("ddd, dd MMM yyyy HH:mm:ss GMT"));
        response = response % QString(RESPONSE_BODY);

        socket->write(response.toUtf8());
        socket->flush();
        socket->waitForBytesWritten();
    }

    socket->close();
    delete socket;
}

} } }
