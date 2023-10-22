#include <QApplication>

#include "tcp_socket_client.h"

#include <QThreadPool>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QThreadPool::globalInstance()->setMaxThreadCount(4);

    QString ip = "192.168.31.229";
    short port = 8891;
    for (int index = 0; index < 5; index++) {
        TcpSocketClient *client = new TcpSocketClient(ip, port);
        client->setAutoDelete(true);
        QThreadPool::globalInstance()->start(client);
    }

    return a.exec();
}
