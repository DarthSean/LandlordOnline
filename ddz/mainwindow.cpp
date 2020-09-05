#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <algorithm>
#include <QTime>
#include <QMessageBox>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));
    ui->groupBoxJiao->hide();
    ui->frame->hide();
    ui->groupBoxReckon->hide();
    ui->labelWin->hide();
    ui->labelLose->hide();
    memset(hasPassed, 0, sizeof(hasPassed));
}

MainWindow::~MainWindow()
{

    delete ui;
}

void MainWindow::on_pushButtonConnect_clicked()
{
    ui->pushButtonConnect->setEnabled(false);
    listenSocket = new QWebSocketServer(QStringLiteral("HOST_A"), QWebSocketServer::NonSecureMode, this);
    if(listenSocket->listen(QHostAddress::LocalHost, 9000)) {
        role = 0;
        qDebug() << "role = " << role;
        QObject::connect(listenSocket, &QWebSocketServer::newConnection, this, &MainWindow::serverConnected);
    } else {
        delete listenSocket;
        rwSocket[0] = new QWebSocket;
        QObject::connect(rwSocket[0], &QWebSocket::connected, this, &MainWindow::clientConnected);
        QObject::connect(rwSocket[0],&QWebSocket::textMessageReceived,this, &MainWindow::recvMsgFromS);
        rwSocket[0]->open(QUrl("ws://127.0.0.1:9000"));
    }
}

void MainWindow::toS(QString msg) {
    rwSocket[0]->sendTextMessage(msg);
}

void MainWindow::toC1(QString msg) {
    if(role == 0) {
        rwSocket[0]->sendTextMessage(msg);
    } else if(role == 2) {
        rwSocket[1]->sendTextMessage(msg);
    }
}

void MainWindow::toC2(QString msg) {
    rwSocket[1]->sendTextMessage(msg);
}


void MainWindow::serverConnected() {
    qDebug() << "server connected!";
    static int cnt = 0;
    rwSocket[cnt] = listenSocket->nextPendingConnection();
    if(cnt == 0) {
        QObject::connect(rwSocket[cnt],&QWebSocket::textMessageReceived,this, &MainWindow::recvMsgFromC1);
    } else {
        QObject::connect(rwSocket[cnt],&QWebSocket::textMessageReceived,this, &MainWindow::recvMsgFromC2);
    }
    rwSocket[cnt]->sendTextMessage("role#" + QString::number(cnt + 1).toUtf8());
    ++cnt;
}

void MainWindow::clientConnected() {
    qDebug() << "client connected!";
}

void MainWindow::client1ConnectedClient2() {
    rwSocket[1] = listenSocket->nextPendingConnection();
    QObject::connect(rwSocket[1], &QWebSocket::textMessageReceived, this, &MainWindow::recvMsgFromC2);
}

void MainWindow::recvMsgFromS(QString msg) {
    SBuf = msg;
    onSMsg(SBuf);
}

void MainWindow::recvMsgFromC1(QString msg) {
    if(role == 0) {
        C1Buf = msg;
    } else if(role == 2) {
        C1Buf = msg;
    } else C1Buf = "crap";
    onC1Msg(C1Buf);
}

void MainWindow::recvMsgFromC2(QString msg) {
    if(role != 2) {
        C2Buf = msg;
    }
    else C2Buf = "crap";
    onC2Msg(C2Buf);
}





















QRegExp r_role("^role#(\\d)$");
QRegExp r_deck("^deck#(\\d)(\\d+)#(\\d)(\\d+)#(\\d)(\\d+)#(\\d)(\\d+)#(\\d)(\\d+)#(\\d)(\\d+)#(\\d)(\\d+)#(\\d)(\\d+)#(\\d)(\\d+)#(\\d)(\\d+)#(\\d)(\\d+)#(\\d)(\\d+)#(\\d)(\\d+)#(\\d)(\\d+)#(\\d)(\\d+)#(\\d)(\\d+)#(\\d)(\\d+)$");
QRegExp r_cover("^cover#(\\d)(\\d+)#(\\d)(\\d+)#(\\d)(\\d+)$");
QRegExp r_assign("^assign#(\\d)$");
QRegExp r_call("^call#(\\d)$");
QRegExp r_lord("^lord#(\\d)$");
QRegExp r_card("card");

void MainWindow::onSMsg(QString msg) {
    qDebug() <<"from S: " << msg;
    if(r_role.indexIn(msg) > -1) {
        role = r_role.cap(1).toInt();
        qDebug() << "role = " << role;
        if(role == 1) {
            listenSocket = new QWebSocketServer("HOST_B", QWebSocketServer::NonSecureMode, this);
            QObject::connect(listenSocket, &QWebSocketServer::newConnection, this, &MainWindow::client1ConnectedClient2);
            listenSocket->listen(QHostAddress::Any, 10000);
        } else if(role == 2) {
            rwSocket[1] = new QWebSocket;
            QObject::connect(rwSocket[1], &QWebSocket::connected, this, &MainWindow::clientConnected);
            QObject::connect(rwSocket[1],&QWebSocket::textMessageReceived,this, &MainWindow::recvMsgFromC1);
            rwSocket[1]->open(QUrl("ws://127.0.0.1:10000"));
        }
        toS("enter");
    }
    if(msg == "start") {
        ui->pushButtonConnect->hide();
        ui->frame->show();
        ui->groupBoxFire->hide();
        ui->lcdNumber_1->display(17);
        ui->lcdNumber_2->display(17);
        ui->lcdNumber_3->display(17);
        ui->labelBuchu1->hide();
        ui->labelBuchu2->hide();
        ui->labelBuchu3->hide();
        //        ui->toolButton->setIcon(QIcon(":img/resource/img/BO.png"));
        //        ui->toolButton->setIcon(QIcon(Card::getfilename(Card(1, 25))));

        if(role == 0) {
            labelJob[0] = ui->labelJob2;
            labelJob[1] = ui->labelJob3;
            labelJob[2] = ui->labelJob1;
            labelJiao[0] = ui->labelJiao2;
            labelJiao[1] = ui->labelJiao3;
            labelJiao[2] = ui->labelJiao1;
            lcd[0] = ui->lcdNumber_2;
            lcd[1] = ui->lcdNumber_3;
            lcd[2] = ui->lcdNumber_1;
            labelBuchu[0] = ui->labelBuchu2;
            labelBuchu[1] = ui->labelBuchu3;
            labelBuchu[2] = ui->labelBuchu1;
        } else if(role == 1) {
            labelJob[0] = ui->labelJob1;
            labelJob[1] = ui->labelJob2;
            labelJob[2] = ui->labelJob3;
            labelJiao[0] = ui->labelJiao1;
            labelJiao[1] = ui->labelJiao2;
            labelJiao[2] = ui->labelJiao3;
            lcd[0] = ui->lcdNumber_1;
            lcd[1] = ui->lcdNumber_2;
            lcd[2] = ui->lcdNumber_3;
            labelBuchu[0] = ui->labelBuchu1;
            labelBuchu[1] = ui->labelBuchu2;
            labelBuchu[2] = ui->labelBuchu3;
        } else if(role == 2) {
            labelJob[0] = ui->labelJob3;
            labelJob[1] = ui->labelJob1;
            labelJob[2] = ui->labelJob2;
            labelJiao[0] = ui->labelJiao3;
            labelJiao[1] = ui->labelJiao1;
            labelJiao[2] = ui->labelJiao2;
            lcd[0] = ui->lcdNumber_3;
            lcd[1] = ui->lcdNumber_1;
            lcd[2] = ui->lcdNumber_2;
            labelBuchu[0] = ui->labelBuchu3;
            labelBuchu[1] = ui->labelBuchu1;
            labelBuchu[2] = ui->labelBuchu2;
        }
        //show interface
    } else if(r_deck.indexIn(msg) > -1) {
        for(int i = 0; i <= 16; ++i) {
            deck.push_back(Card(r_deck.cap(2 * i + 1).toInt(), r_deck.cap(2 * i + 2).toInt()));
            //            qDebug() << r_deck.cap(2 * i + 1).toInt() << r_deck.cap(2 * i + 2).toInt();
        }
        std::sort(deck.begin(), deck.end());
        for(int i = 0; i <= 2; ++i) {
            cardNum[i] = 17;
        }
//        updateHand();
        hand = new QToolButton*[deck.size()];
        for(int i = 0; i < deck.size(); ++i) {
            hand[i] = new QToolButton(this);
            hand[i]->move(90 + i * 510 / deck.size(), 360);
            hand[i]->setCheckable(true);
            hand[i]->resize(90, 120);
            hand[i]->setIcon(QIcon(Card::getfilename(deck[i])));
            hand[i]->setIconSize(QSize(150, 180));
            hand[i]->show();
        }
        //        qDebug() << "got deck";
    } else if(r_cover.indexIn(msg) > -1) {
        for(int i = 0; i <= 2; ++i) {
            rest[i] = Card(r_cover.cap(2 * i + 1).toInt(), r_cover.cap(2 * i + 2).toInt());
            //            qDebug() << r_cover.cap(2 * i + 1).toInt() << r_cover.cap(2 * i + 2).toInt();
        }
        //        qDebug() << "got cover";
    } else if(r_assign.indexIn(msg) > -1) {
        memset(hasCalled, 0, sizeof(hasCalled));
        whosLord = r_assign.cap(1).toInt();
        qDebug() << whosLord;
        //show who was called
        if(whosLord == role) {
            ui->groupBoxJiao->show();
        }
    } else if(r_call.indexIn(msg) > -1) {
        hasCalled[0] = r_call.cap(1).toInt();
        qDebug() << "captured:" << r_call.cap(0).toInt() << r_call.cap(1).toInt() << r_assign.cap(2).toInt();
        if(hasCalled[0] == 1)whosLord = 0;
        qDebug() << "who's lord: " << whosLord;

        //show result

        if(hasCalled[0] == 1)labelJiao[0]->setText("叫");
        else labelJiao[0]->setText("不叫");


        if(role == 1) {
            if(hasCalled[1] != 0) {
                toS("decide");
            } else {
                ui->groupBoxJiao->show();
            }
        }
    } else if(r_lord.indexIn(msg) > -1) {//to be broadcasted
        //show cover(rest) and lord and who's turn this is
        ui->labelJiao1->hide();
        ui->labelJiao2->hide();
        ui->labelJiao3->hide();
        for(int i = 0; i < 3; ++i) {
            labelJob[i]->setText("农民");
        }
        labelJob[whosLord]->setText("地主");

        qDebug() << "lord:" << whosLord;
        cardNum[whosLord] = 20;
        lcd[whosLord]->display(cardNum[whosLord]);
        turn = whosLord;
        labelJob[whosLord]->setStyleSheet("color:red;");
        memset(job, 0, sizeof(job));
        job[whosLord] = 1;

        ui->labelCover1->setPixmap(Card::getfilename(rest[0]));
        ui->labelCover2->setPixmap(Card::getfilename(rest[1]));
        ui->labelCover3->setPixmap(Card::getfilename(rest[2]));

        if(whosLord == role) {
            for(int i = 0; i < deck.size(); ++i) {
                delete hand[i];
            }
            delete[] hand;
            deck.push_back(rest[0]);
            deck.push_back(rest[1]);
            deck.push_back(rest[2]);
            std::sort(deck.begin(), deck.end());


//            updateHand();
            hand = new QToolButton*[deck.size()];
            for(int i = 0; i < deck.size(); ++i) {
                hand[i] = new QToolButton(this);
                hand[i]->move(90 + i * 510 / deck.size(), 360);
                hand[i]->setCheckable(true);
                hand[i]->resize(90, 120);
                hand[i]->setIcon(QIcon(Card::getfilename(deck[i])));
                hand[i]->setIconSize(QSize(150, 180));
                hand[i]->show();
            }
            //update view
            fire(true);
            //begin turn
        }
    } else if(r_card.indexIn(msg) > -1) {
        labelJob[0]->setStyleSheet("color:black;");
        labelJob[1]->setStyleSheet("color:red;");
        int type = msg[5].toLatin1() - 'a';
        qDebug() << "b1";
        if(type == 15) {
            //guo
            labelBuchu[0]->show();
            hasPassed[0] = true;
        } else {
            //da_chu
            if(heap != nullptr) {
                for(int i = 0; i < present.size(); ++i) {
                    heap[i]->hide();
                }
                delete[] heap;
            }
            qDebug() << "b2";
            int feat = msg[7].toLatin1() - 'a';
            int quantity = msg[9].toLatin1() - 'a';
            if(type == 13) {
                feat = feat + 15 * quantity;
            } else if(type == 14) {
                feat = feat + 15 * quantity / 2;
            }
            qDebug() << "combo: " << type << feat << quantity;

            nowCard = qMakePair(type, feat);
            present.clear();
            for(int i = 0; i < quantity; ++i) {
                present.push_back(Card(msg[11 + i * 3].toLatin1() - 'a', msg[12 + i * 3].toLatin1() - 'a'));
            }
            //show present
           heap = new QLabel*[present.size()];
           QPalette label_palette;
           label_palette.setColor(QPalette::Background, QColor(0, 0, 0));
           for(int i = 0; i < present.size(); ++i) {
               heap[i] = new QLabel(this);
               heap[i]->resize(60, 80);

               heap[i]->setPalette(label_palette);
               heap[i]->setPixmap(Card::getfilename(present[i]));
               heap[i]->setScaledContents(true);
               heap[i]->move(160 + 225 - 30 * present.size() + i * 60, 160);
               heap[i]->show();
           }
//            if(present.size() <= 5) {
//                for(int i = 0; i < present.size(); ++i) {
//                    heap[i]->move(160 + 225 - 30 * present.size() + i * 60, 160);
//                }
//            } else {
//                for(int i = 0; i < present.size(); ++i) {
//                    heap[i]->move(90 + i * 360 / present.size(), 160);
//                }
//            }

            cardNum[0] -= quantity;
            lcd[0]->display(cardNum[0]);
            if(cardNum[0] == 0) {
                if(job[0] == job[role]) {
//                    QMessageBox::information(this, "游戏结束", "你赢了！");
                    ui->labelWin->show();
                } else {
//                    QMessageBox::information(this, "游戏结束", "你输了！");
                    ui->labelLose->show();
                }
                //restart stuff
                reckon();
            }
        }
        //next player
        bool trigger = hasPassed[0] && hasPassed[2];
        if(trigger) {
            for(int i = 0; i <= 2; ++i) {
                labelBuchu[i]->hide();
                hasPassed[i] = false;
            }
        }
        if(role == 1) {
            fire(trigger);
        }
    } else if(msg == "restart") {
        ++readyNum;
        if(readyNum == 3) {
            server_start();
        }
    }
}

void MainWindow::onC1Msg(QString msg) {
    qDebug() << "from C1: " << msg;
    if(msg == "enter") {
        ++readyNum;
        if(readyNum == 2) {
            server_start();
        }
    } else if(r_call.indexIn(msg) > -1) {
        hasCalled[1] = r_call.cap(1).toInt();
        qDebug() << "captured: " << r_call.cap(1).toInt();
        if(hasCalled[1] == 1)whosLord = 1;
        qDebug() << "who's lord: " << whosLord;

        //show result
        if(hasCalled[1] == 1) labelJiao[1]->setText("叫");
        else labelJiao[1]->setText("不叫");


        if(role == 2) {
            if(hasCalled[2] != 0) {
                toS("decide");
            } else {
                ui->groupBoxJiao->show();
            }
        }
    } else if(msg == "decide") {
        server_decide();
    } else if(r_card.indexIn(msg) > -1) {
        labelJob[1]->setStyleSheet("color:black;");
        labelJob[2]->setStyleSheet("color:red;");
        int type = msg[5].toLatin1() - 'a';
        qDebug() << "b1";
        if(type == 15) {
            //guo
            labelBuchu[1]->show();
            hasPassed[1] = true;
        } else {
            //da_chu
            if(heap != nullptr) {
                for(int i = 0; i < present.size(); ++i) {
                    heap[i]->hide();
                }
                delete[] heap;
            }
            qDebug() << "b2";
            int feat = msg[7].toLatin1() - 'a';
            int quantity = msg[9].toLatin1() - 'a';
            if(type == 13) {
                feat = feat + 15 * quantity;
            } else if(type == 14) {
                feat = feat + 15 * quantity / 2;
            }
            qDebug() << "combo: " << type << feat << quantity;

            nowCard = qMakePair(type, feat);
            present.clear();
            for(int i = 0; i < quantity; ++i) {
                present.push_back(Card(msg[11 + i * 3].toLatin1() - 'a', msg[12 + i * 3].toLatin1() - 'a'));
            }
            //show present
            heap = new QLabel*[present.size()];
            QPalette label_palette;
            label_palette.setColor(QPalette::Background, QColor(0, 0, 0));
            for(int i = 0; i < present.size(); ++i) {
                heap[i] = new QLabel(this);
                heap[i]->resize(60, 80);
                heap[i]->setPalette(label_palette);
                heap[i]->setPixmap(Card::getfilename(present[i]));
                heap[i]->setScaledContents(true);
                heap[i]->move(160 + 225 - 30 * present.size() + i * 60, 160);
                heap[i]->show();
            }
//            if(present.size() <= 5) {
//                for(int i = 0; i < present.size(); ++i) {
//                    heap[i]->move(160 + 225 - 45 * present.size() + i * 90, 160);
//                }
//            } else {
//                for(int i = 0; i < present.size(); ++i) {
//                    heap[i]->move(90 + i * 360 / present.size(), 160);
//                }
//            }

            cardNum[1] -= quantity;
            lcd[1]->display(cardNum[1]);
            if(cardNum[1] == 0) {
                if(job[1] == job[role]) {
//                    QMessageBox::information(this, "游戏结束", "你赢了！");
                    ui->labelWin->show();
                } else {
//                    QMessageBox::information(this, "游戏结束", "你输了！");
                    ui->labelLose->show();
                }
                //restart stuff
                reckon();
            }
        }
        //next player
        bool trigger = hasPassed[0] && hasPassed[1];
        if(trigger) {
            for(int i = 0; i <= 2; ++i) {
                labelBuchu[i]->hide();
                hasPassed[i] = false;
            }
        }
        if(role == 2) {
            fire(trigger);
        }
    } else if(msg == "restart") {
        ++readyNum;
        if(readyNum == 3) {
            server_start();
        }
    }
}

void MainWindow::onC2Msg(QString msg) {
    qDebug() << "from C2: " << msg;
    if(msg == "enter") {
        ++readyNum;
        if(readyNum == 2) {
            server_start();
        }
    } else if(r_call.indexIn(msg) > -1) {
        hasCalled[2] = r_call.cap(1).toInt();
        qDebug() << "captured: " << r_call.cap(1).toInt();
        if(hasCalled[2] == 1)whosLord = 2;
        qDebug() << "who's lord: " << whosLord;
        //show result
        if(hasCalled[2] == 1) labelJiao[2]->setText("叫");
        else labelJiao[2]->setText("不叫");

        if(role == 0) {
            if(hasCalled[0] != 0) {
                server_decide();
            } else {
                ui->groupBoxJiao->show();
            }
        }
    } else if(msg == "decide") {
        server_decide();
    } else if(r_card.indexIn(msg) > -1) {
        labelJob[2]->setStyleSheet("color:black;");
        labelJob[0]->setStyleSheet("color:red;");
        int type = msg[5].toLatin1() - 'a';
        qDebug() << "b1";
        if(type == 15) {
            //guo
            labelBuchu[2]->show();
            hasPassed[2] = true;
        } else {
            //da_chu
            if(heap != nullptr) {
                for(int i = 0; i < present.size(); ++i) {
                    heap[i]->hide();
                }
                delete[] heap;
            }
            qDebug() << "b2";
            int feat = msg[7].toLatin1() - 'a';
            int quantity = msg[9].toLatin1() - 'a';
            if(type == 13) {
                feat = feat + 15 * quantity;
            } else if(type == 14) {
                feat = feat + 15 * quantity / 2;
            }
            qDebug() << "combo: " << type << feat << quantity;

            nowCard = qMakePair(type, feat);
            present.clear();
            for(int i = 0; i < quantity; ++i) {
                present.push_back(Card(msg[11 + i * 3].toLatin1() - 'a', msg[12 + i * 3].toLatin1() - 'a'));
            }
            //show present
            heap = new QLabel*[present.size()];
            QPalette label_palette;
            label_palette.setColor(QPalette::Background, QColor(0, 0, 0));
            for(int i = 0; i < present.size(); ++i) {
                heap[i] = new QLabel(this);
                heap[i]->resize(60, 80);
                heap[i]->setPalette(label_palette);
                heap[i]->setPixmap(Card::getfilename(present[i]));
                heap[i]->setScaledContents(true);
                heap[i]->move(160 + 225 - 30 * present.size() + i * 60, 160);
                heap[i]->show();
            }
//            if(present.size() <= 5) {
//                for(int i = 0; i < present.size(); ++i) {
//                    heap[i]->move(160 + 225 - 30 * present.size() + i * 60, 160);
//                }
//            } else {
//                for(int i = 0; i < present.size(); ++i) {
//                    heap[i]->move(90 + i * 360 / present.size(), 160);
//                }
//            }

            cardNum[2] -= quantity;
            lcd[2]->display(cardNum[2]);
            if(cardNum[2] == 0) {
                if(job[2] == job[role]) {
//                    QMessageBox::information(this, "游戏结束", "你赢了！");
                    ui->labelWin->show();
                } else {
//                    QMessageBox::information(this, "游戏结束", "你输了！");
                    ui->labelLose->show();
                }
                //restart stuff
                reckon();
            }
        }
        //next player
        bool trigger = hasPassed[1] && hasPassed[2];
        if(trigger) {
            for(int i = 0; i <= 2; ++i) {
                labelBuchu[i]->hide();
                hasPassed[i] = false;
            }
        }
        if(role == 0) {
            fire(trigger);
        }
    } else if(msg == "restart") {
        ++readyNum;
        if(readyNum == 3) {
            server_start();
        }
    }
}


void MainWindow::server_start() {//server
    onSMsg("start");
    toC1("start");
    toC2("start");

    int nums[] = {3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 20};
    for(auto i : nums) {
        for(int j = 1; j <= 4; ++j) {
            pile.push_back(Card(j, i));
        }
    }
    srand(static_cast<unsigned int>(time(0)));
    pile.push_back(Card(1, 26));
    pile.push_back(Card(3, 25));
    std::random_shuffle(pile.begin(), pile.end());

    QString card0 = "deck", card1 = "deck", card2 = "deck";
    for(int i = 0; i <= 16; ++i) {
        card0 = card0 + "#" + QString::number(pile[i].getSuit()) + QString::number(pile[i].getNum());
    }
    for(int i = 17; i <= 33; ++i) {
        card1 = card1 + "#" + QString::number(pile[i].getSuit()) + QString::number(pile[i].getNum());
    }
    for(int i = 34; i <= 50; ++i) {
        card2 = card2 + "#" + QString::number(pile[i].getSuit()) + QString::number(pile[i].getNum());
    }
    onSMsg(card0);
    toC1(card1);
    toC2(card2);
    QString cover = "cover";
    for(int i = 0; i <= 2; ++i) {
        //        rest[i] = pile[51 + i];
        cover = cover + "#" + QString::number(pile[51 + i].getSuit()) + QString::number(pile[51 + i].getNum());
    }
    onSMsg(cover);
    toC1(cover);
    toC2(cover);
    int rand = qrand() % 3;
    whosLord = rand;
    QString assign = "assign#" + QString::number(rand);
    onSMsg(assign);
    toC1(assign);
    toC2(assign);
}

void MainWindow::server_decide() {
    onSMsg("lord#" + QString::number(whosLord));
    toC1("lord#" + QString::number(whosLord));
    toC2("lord#" + QString::number(whosLord));
}

void MainWindow::on_pushButtonJiao_clicked()
{
    ui->groupBoxJiao->hide();
    hasCalled[role] = 1;
    whosLord = role;
    ui->labelJiao2->setText("叫");
    //show
    if(role != 0) toS("call#1");
    if(role != 1) toC1("call#1");
    if(role != 2) toC2("call#1");
}

void MainWindow::on_pushButtonBuJiao_clicked()
{
    ui->groupBoxJiao->hide();
    hasCalled[role] = 2;
    ui->labelJiao2->setText("不叫");

    //show
    if(role != 0) toS("call#2");
    if(role != 1) toC1("call#2");
    if(role != 2) toC2("call#2");
}

void MainWindow::fire(bool _forced) {
    ui->groupBoxFire->show();
    forced = _forced;
//    updateHand();
    if(_forced) ui->pushButtonBuyao->setEnabled(false);
    else ui->pushButtonBuyao->setEnabled(true);
    handSelectable(true);
}

void MainWindow::reckon() {
    ui->frame->hide();
    for(int i = 0; i < deck.size(); ++i) {
        hand[i]->hide();
    }
    for(int i = 0; i < present.size(); ++i) {
        heap[i]->hide();
    }
    delete[] hand;
    delete[] heap;
    heap = nullptr;
    hand = nullptr;
    memset(cardNum, 0, sizeof(cardNum));
    memset(hasPassed, 0, sizeof(hasPassed));
    deck.clear();
    pile.clear();
    readyNum = 0;
    ui->labelJob1->setText("");
    ui->labelJob2->setText("");
    ui->labelJob3->setText("");
    ui->labelJob1->setStyleSheet("color:black;");
    ui->labelJob2->setStyleSheet("color:black;");
    ui->labelJob3->setStyleSheet("color:black;");
    ui->labelJiao1->setText("");
    ui->labelJiao2->setText("");
    ui->labelJiao3->setText("");
    ui->labelJiao1->show();
    ui->labelJiao2->show();
    ui->labelJiao3->show();
    QPixmap pixmap;
    pixmap.load(":img/resource/img/PADDING.png");
    ui->labelCover1->setPixmap(pixmap);
    ui->labelCover2->setPixmap(pixmap);
    ui->labelCover3->setPixmap(pixmap);
    ui->groupBoxReckon->show();
}

//void MainWindow::updateHand() {

//    hand = nullptr;

//    hand = new QToolButton*[deck.size()];
//    for(int i = 0; i < deck.size(); ++i) {
//        hand[i] = new QToolButton(this);
//        hand[i]->move(90 + i * 510 / deck.size(), 360);
//        hand[i]->setCheckable(true);
//        hand[i]->resize(90, 120);
//        hand[i]->setIcon(QIcon(Card::getfilename(deck[i])));
//        hand[i]->setIconSize(QSize(150, 180));
//        hand[i]->show();
//    }
//    handSelectable(false);
//}

void MainWindow::handSelectable(bool selectable) {
    for(int i = 0; i < deck.size(); ++i) {
        hand[i]->setCheckable(selectable);
    }
}

void MainWindow::on_pushButtonChu_clicked()
{
    QVector<Card> combo;
    for(int i = 0; i < deck.size(); ++i) {
        if(hand[i]->isChecked()) {
            combo.push_back(deck[i]);
        }
    }
    auto a = Card::getSet(combo);
    qDebug() << a.first;
    bool valid = true;
    if(a.first == 0) {
        valid = false;
    } else if(forced) {
        //chupai
    } else {
        if(a.first != 2 && a.first != 5 && a.first != nowCard.first) {
            valid = false;
        } else {
            if(a.first == 1 || a.first == 3 || a.first == 4 || a.first == 6 || a.first == 7 || a.first == 8 || a.first == 9 || a.first == 10 || a.first == 11 || a.first == 12) {
                if(a.second <= nowCard.second) {
                    valid = false;
                }
            } else {
                if(a.first == 5) {
                    if(nowCard.first == 2) {
                        valid = false;
                    } else if(nowCard.first == 5) {
                        if(a.second <= nowCard.second) {
                            valid = false;
                        } else {
                            //chupai
                        }
                    } else {
                        //chupai
                    }
                } else if(a.first == 2) {
                    //chupai
                } else {
                    int al = a.second / 15, av = a.second % 15, nl = nowCard.second / 15, nv = nowCard.second % 15;
                    if(al != nl) {
                        valid = false;
                    } else {
                        if(av <= nv) {
                            valid = false;
                        } else {
                            //chupai
                        }
                    }
                }
            }
        }
    }
    if(!valid) {
        QMessageBox::information(this, "提示", "牌组无效");
        for(int i = 0; i < deck.size(); ++i) {
            hand[i]->setChecked(false);
        }
        return;
    }

    for(int i = 0; i < deck.size(); ++i) {
        hand[i]->hide();
    }
    QString card = "card#";
    int type = a.first;
    int eigen = a.second;
    if(type == 13 | type == 14) {
        eigen = eigen % 15;
    }
    int cardnum = combo.size();
    QString tmp = "x#y#z";
    tmp[0] = char(type) + 'a';
    tmp[2] = char(eigen) + 'a';
    tmp[4] = char(cardnum) + 'a';
    card += tmp;
    QString tmp2 = "#xy";
    for(int i = 0; i < cardnum; ++i) {
        tmp2[1] = char(combo[i].getSuit()) + 'a';
        tmp2[2] = char(combo[i].getNum()) + 'a';
        card += tmp2;
    }
    for(int i = deck.size() - 1; i >= 0; --i) {
        if(hand[i]->isChecked()) {
            deck.remove(i);
        }
    }
    hand = new QToolButton*[deck.size()];
    for(int i = 0; i < deck.size(); ++i) {
        hand[i] = new QToolButton(this);
        hand[i]->move(90 + i * 510 / deck.size(), 360);
        hand[i]->setCheckable(true);
        hand[i]->resize(90, 120);
        hand[i]->setIcon(QIcon(Card::getfilename(deck[i])));
        hand[i]->setIconSize(QSize(150, 180));
        hand[i]->show();
    }
    handSelectable(false);
//    updateHand();
    ui->groupBoxFire->hide();
    if(role != 0) {
        toS(card);
    } else {
        onSMsg(card);
    }
    if(role != 1) {
        toC1(card);
    } else {
        onC1Msg(card);
    }
    if(role != 2) {
        toC2(card);
    } else {
        onC2Msg(card);
    }
}

void MainWindow::on_pushButtonBuyao_clicked()
{
    ui->groupBoxFire->hide();
    QString card = "card#x";
    card[5] = char(15) + 'a';
    for(int i = 0; i < deck.size(); ++i) {
        hand[i]->setChecked(false);
    }
    if(role != 0) {
        toS(card);
    } else {
        onSMsg(card);
    }
    if(role != 1) {
        toC1(card);
    } else {
        onC1Msg(card);
    }
    if(role != 2) {
        toC2(card);
    } else {
        onC2Msg(card);
    }
}

void MainWindow::on_pushButtonQuit_clicked()
{
    emit terminate();
}

void MainWindow::on_pushButtonRestart_clicked()
{
    ui->labelWin->hide();
    ui->labelLose->hide();
    ui->groupBoxReckon->hide();
    if(role == 0) onSMsg("restart");
    else toS("restart");
}
