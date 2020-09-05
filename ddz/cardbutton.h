#ifndef CARDBUTTON_H
#define CARDBUTTON_H

#include <QToolButton>
#include "card.h"
class CardButton : public QToolButton
{
    Q_OBJECT
public:
    explicit CardButton(QWidget *parent = nullptr, int _suit = 0, int _num = 0);
    bool selected = false;
    void mousePressEvent(QMouseEvent *) override;
    Card c;
private:

signals:

};

#endif // CARDBUTTON_H
