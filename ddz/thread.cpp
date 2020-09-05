#include "thread.h"

Thread::Thread(QObject* parent) : QObject(parent)
{
    bool becomeServer1 = false, becomeServer2 = false, becomeServer3 = false;
    QTcpSocket socket(0);
//    socket.abort();
//    socket.connectToHost(QHostAddress("127.0.0.1"), 9000);
//    if(!socket.waitForConnected(1))
//    {
//        qDebug() << "no 9000!";
//        becomeServer1 = true;
//    }
//    socket.abort();
//    socket.connectToHost("127.0.0.1", 10000);
//    if(!socket.waitForConnected(1))
//    {
//        qDebug() <<"no 10000!";
//        becomeServer2 = true;
//    }
//    socket.abort();
//    socket.connectToHost("127.0.0.1", 20000);
//    if(!socket.waitForConnected(1))
//    {
//        qDebug() <<"no 20000!";
//        becomeServer3 = true;
//    }
//    socket.abort();
    socket.abort();
    socket.connectToHost("127.0.0.1", 20000);
    if(!socket.waitForConnected(1))
    {
        qDebug() <<"no 20000!";
        becomeServer3 = true;
    }
    socket.abort();
    if(!becomeServer3) {
        role = 2;
    } else {
        socket.connectToHost("127.0.0.1", 9000);
        if(!socket.waitForConnected()) {
            qDebug() << "no 900!";
            becomeServer1 = true;
        }
        socket.abort();
        if(becomeServer1) {
            role = 0;
        } else {
            role = 1;
        }
    }


//    if(becomeServer1 && becomeServer3) {
//        role = 0;
//    } else if(!becomeServer1 && becomeServer3) {
//        role = 1;
//    } else if(!becomeServer3) {
//        role = 2;
//    }
    qDebug() << "role = " << role;
}

Thread::~Thread() {
//    if(listenSocket[0] != nullptr)delete listenSocket[0];
//    if(listenSocket[1] != nullptr)delete listenSocket[1];
//    if(rwSocket[0] != nullptr)delete rwSocket[0];
//    if(rwSocket[1] != nullptr)delete rwSocket[1];
}

void Thread::run() {
    if(role == 0) {
        qDebug() << "i am server";
        listenSocket[0] = new QTcpServer;
        listenSocket[0]->listen(QHostAddress::Any,9000);
        connect(listenSocket[0], &QTcpServer::newConnection, this, &Thread::serverConnected1);

        listenSocket[1] = new QTcpServer;
        connect(listenSocket[1], &QTcpServer::newConnection,this, &Thread::serverConnected2);
        listenSocket[1]->listen(QHostAddress::Any, 10000);
    } else if(role == 1){
        rwSocket[0] = new QTcpSocket;
        connect(rwSocket[0], &QTcpSocket::connected, this, &Thread::clientConnected);
        rwSocket[0]->connectToHost(QHostAddress("127.0.0.1"),9000);
        connect(rwSocket[0],SIGNAL(readyRead()),this,SLOT(recvMsgFromS()));
        rwSocket[0]->waitForConnected();
        qDebug() << rwSocket[0]->state() << "aha!";
        listenSocket[0] = new QTcpServer;
        connect(listenSocket[0], &QTcpServer::newConnection,this, &Thread::client1ConnectedClient2);
        listenSocket[0]->listen(QHostAddress::Any, 20000);
    } else if(role == 2) {
        rwSocket[0] = new QTcpSocket;
        connect(rwSocket[0], &QTcpSocket::connected, this, &Thread::clientConnected);
        rwSocket[0]->connectToHost(QHostAddress("127.0.0.1"),10000);
        connect(rwSocket[0],SIGNAL(readyRead()),this,SLOT(recvMsgFromS()));
        rwSocket[0]->waitForConnected();
        rwSocket[1] = new QTcpSocket;
        connect(rwSocket[1], &QTcpSocket::connected, this, &Thread::clientConnected);
        rwSocket[1]->connectToHost(QHostAddress("127.0.0.1"),20000);
        connect(rwSocket[1],SIGNAL(readyRead()),this,SLOT(recvMsgFromC1()));
        rwSocket[1]->waitForConnected();
    }

    if(role != 0) {
        rwSocket[0]->write("enter");
    }
//    exec();
}

void Thread::serverConnected1() {
    qDebug() << "server connected in 9000!";
    rwSocket[0] = listenSocket[0]->nextPendingConnection();
    connect(rwSocket[0],SIGNAL(readyRead()),this,SLOT(recvMsgFromC1()));
//    connect(rwSocket[0], &QTcpSocket::disconnected, [=](){rwSocket[0]->deleteLater();});
}

void Thread::serverConnected2() {
    qDebug() << "server connected in 10000!";
    rwSocket[1] = listenSocket[1]->nextPendingConnection();
    connect(rwSocket[1],SIGNAL(readyRead()),this,SLOT(recvMsgFromC2()));
//    connect(rwSocket[1], &QTcpSocket::disconnected, [=](){rwSocket[1]->deleteLater();});
}

void Thread::client1ConnectedClient2() {
    qDebug() << "client1 connected client2!";
    rwSocket[1] = listenSocket[0]->nextPendingConnection();
    connect(rwSocket[1],SIGNAL(readyRead()),this,SLOT(recvMsgFromC2()));
//    connect(rwSocket[1], &QTcpSocket::disconnected, [=](){rwSocket[1]->deleteLater();});
}

void Thread::clientConnected() {
    qDebug() << "client connected!";
}

void Thread::recvMsgFromS() {
    if(role != 0) {
//        rwSocket[0]->waitForReadyRead();
        SBuf = rwSocket[0]->readAll();

    }
    else SBuf = "crap";
    emit onSMsg(SBuf);
}

void Thread::recvMsgFromC1() {
    if(role == 0) {
//        rwSocket[0]->waitForReadyRead();
        C1Buf = rwSocket[0]->readAll();

    } else if(role == 2) {
//        rwSocket[1]->waitForReadyRead();
        C1Buf = rwSocket[1]->readAll();

    } else C1Buf = "crap";
    emit onC1Msg(C1Buf);
}

void Thread::recvMsgFromC2() {
    if(role != 2) {
//        rwSocket[1]->waitForReadyRead();
        C2Buf = rwSocket[1]->readAll();

    }
    else C2Buf = "crap";
    emit onC2Msg(C2Buf);
}
