#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork>
#include <QWebSocket>
#include <QWebSocketServer>
#include <QLabel>
#include <QLCDNumber>
#include <QGroupBox>
#include <QToolButton>
#include "card.h"
//#include "cardbutton.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    //general
    void on_pushButtonConnect_clicked();

    void serverConnected();
    void clientConnected();
    void client1ConnectedClient2();
    void recvMsgFromS(QString msg);
    void recvMsgFromC1(QString msg);
    void recvMsgFromC2(QString msg);

    void onSMsg(QString msg);//process
    void onC1Msg(QString msg);//process
    void onC2Msg(QString msg);//process
    void server_start();
    void server_decide();
    void fire(bool forced);
    void reckon();
    void on_pushButtonJiao_clicked();
    void on_pushButtonBuJiao_clicked();
//    void updateHand();
    void handSelectable(bool selectable);
    void on_pushButtonChu_clicked();

    void on_pushButtonBuyao_clicked();

    void on_pushButtonQuit_clicked();

    void on_pushButtonRestart_clicked();

private:
    Ui::MainWindow *ui;

    //connection
    QWebSocket * rwSocket[2];
    QWebSocketServer * listenSocket;
    QString SBuf;
    QString C1Buf;
    QString C2Buf;
    void toS(QString msg);
    void toC1(QString msg);
    void toC2(QString msg);

    //general
    int role = 0;//0:server, 1:client 1, 2:client 2
    int job[3];//1:lord 0:farmer   ordered by role
    int cardNum[3];//0:server 1.client1 2 client2
    int whosLord;//lord, by default the 1st one to call
    int hasCalled[3];//jiaodizhu, 0:none 1:jiao 2:bujiao
    int turn;//based on role
    QPair<int, int> nowCard;//the combo to beat
    QVector<Card> present;//current card to show
    QLabel** heap = nullptr;//show current cards;
    bool forced;//forced to play
    bool hasPassed[3];//record who buChu-ed
    QVector<Card> deck;
    QVector<Card> pile;
    Card rest[3];
    QToolButton** hand = nullptr;

    //server
    int readyNum = 0;//used by server to decide when to start

    //client1

    //client2

    //interface
    QLabel* labelJob[3];
    QLabel* labelJiao[3];
    QLabel* labelBuchu[3];
    QLCDNumber* lcd[3];
signals:
    void terminate();
};
#endif // MAINWINDOW_H
