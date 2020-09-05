#ifndef THREAD_H
#define THREAD_H
#include <QThread>
#include <QtNetwork>
#include <QDebug>
class Thread : public QObject
{
    Q_OBJECT
public:
    Thread(QObject* parent = nullptr);
    ~Thread();
    QTcpSocket * rwSocket[2];
    QTcpServer * listenSocket[2];
    QString SBuf;
    QString C1Buf;
    QString C2Buf;
private:

public:
    int role = 0;//0:server, 1:client of 9000, 2:client of 10000
    void run();
public slots:
    void serverConnected1();
    void serverConnected2();
    void client1ConnectedClient2();
    void clientConnected();

    void recvMsgFromS();
    void recvMsgFromC1();
    void recvMsgFromC2();
signals:
    void onSMsg(QString msg);
    void onC1Msg(QString msg);
    void onC2Msg(QString msg);
};

#endif // THREAD_H
