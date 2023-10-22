#include "tcp_socket_client.h"

#include <QDebug>
#include <QThread>

TcpSocketClient::TcpSocketClient(QString ip, short port)
    : ip_(ip),
      port_(port),
      socket_client_(nullptr),
      event_loop_(nullptr) {
    qDebug() << "[TcpSocketClient] TcpSocketClient current thread: " << QThread::currentThread();
}

TcpSocketClient::~TcpSocketClient() {
    qDebug() << "[TcpSocketClient] ~TcpSocketClient current thread: " << QThread::currentThread();
}

bool TcpSocketClient::connect() {
    socket_client_ = new QTcpSocket();
    QObject::connect(socket_client_, &QTcpSocket::connected, [](){
        qDebug() << "[TcpSocketClient] SocketConnected current thread: " << QThread::currentThread();
    });

    QObject::connect(socket_client_, &QTcpSocket::stateChanged, [](QAbstractSocket::SocketState socketState) {
        qDebug() << "[TcpSocketClient] StateChanged " << socketState << " current thread: " << QThread::currentThread();
    });

    QObject::connect(socket_client_, &QTcpSocket::errorOccurred, [](QAbstractSocket::SocketError socketError) {
        qDebug() << "[TcpSocketClient] ErrorOccurred " << socketError << " current thread: " << QThread::currentThread();
    });

    QObject::connect(socket_client_, &QTcpSocket::disconnected, [this]() {
        qDebug() << "[TcpSocketClient] SocketDisconnected current thread: " << QThread::currentThread();
        if (event_loop_ && event_loop_->isRunning()) {
            event_loop_->exit();
            event_loop_->deleteLater();
        }
    });

    QObject::connect(socket_client_, &QTcpSocket::readyRead, [this]{
        QByteArray data = socket_client_->readAll();
        qDebug() << "[TcpSocketClient] SocketRead current thread: " << QThread::currentThread()
                 << " data: " << data;
    });

    socket_client_->connectToHost(ip_, port_);
    return socket_client_->waitForConnected(5000);
}

void TcpSocketClient::run() {
    if (connect()) {
        event_loop_ = new QEventLoop();
        event_loop_->exec();
    }

    if (socket_client_) {
        socket_client_->disconnect(this);
        socket_client_->deleteLater();
    }
}
