#ifndef CARD_H
#define CARD_H

#include <QVector>
#include <QDebug>
class Card
{
public:
    Card(int _suit = 0, int _num = 0);
    int getNum();
    int getSuit();
    bool operator<(Card& other);
    static QPair<int, int> getSet(QVector<Card>& set);
    static QString getfilename(Card c, bool solid = false);

private:
    int num;//3456789 10 11/J 12/Q 13/K 14/A 20/2 25/BJoker 26/RJoker
    int suit;//1heart 2diamond 3spade 4club
};

#endif // CARD_H
