#ifndef TCP_SOCKET_CLIENT_H
#define TCP_SOCKET_CLIENT_H

#include <QRunnable>
#include <QTcpSocket>
#include <QEventLoop>

class TcpSocketClient : public QObject,
                        public QRunnable {
    Q_OBJECT
public:
    TcpSocketClient(QString ip, short port);
    ~TcpSocketClient();

private:
    virtual void run() override;

    bool connect();

signals:

private:
    QString ip_;
    short port_;
    QTcpSocket *socket_client_;
    QEventLoop *event_loop_;
};

#endif // TCP_SOCKET_CLIENT_H
