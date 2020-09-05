#include "card.h"
#include <memory.h>
Card::Card(int _suit, int _num) : num(_num), suit(_suit)
{

}

int Card::getNum() {
    return num;
}

int Card::getSuit() {
    return suit;
}

bool Card::operator<(Card& other) {
    return num < other.num;
}

QPair<int, int> Card::getSet(QVector<Card>& set) {
    std::sort(set.begin(), set.end());
    int size = set.size();
    if(size == 1) {
        return qMakePair(1, set[0].getNum());//dan
    } else if(size == 2) {
        if(set[0].getNum() + set[1].getNum() == 51) {
            return qMakePair(2, 0);//wangzha
        } else if(set[0].getNum() == set[1].getNum()) {
            return qMakePair(3, set[0].getNum());//dui
        }
    } else if(size == 3) {
        if(set[0].getNum() == set[1].getNum() && set[1].getNum() == set[2].getNum()) {
            return qMakePair(4, set[0].getNum());//san
        }
    } else if(size == 4) {
        if(set[0].getNum() == set[1].getNum() && set[1].num == set[2].num && set[2].getNum() == set[3].getNum()) {
            return qMakePair(5, set[0].getNum());//zha
        }else if((set[0].getNum() == set[1].getNum() && set[1].num == set[2].num) ||
                 (set[1].num == set[2].num && set[2].getNum() == set[3].getNum())) {
            return qMakePair(6, set[1].getNum());//3+1
        }
    }
    if(size == 5) {
        if(set[0].getNum() == set[1].getNum() && set[3].getNum() == set[4].getNum() && (set[1].getNum() == set[2].getNum() ||
                                                                                        set[2].getNum() == set[3].getNum())) {
            return qMakePair(7, set[2].getNum());//3+2
        }
    }
    if(size == 6) {
        if((set[0].num == set[1].num && set[1].num == set[2].num && set[2].num == set[3].num) ||
           (set[4].num == set[1].num && set[1].num == set[2].num && set[2].num == set[3].num) ||
           (set[4].num == set[5].num && set[5].num == set[2].num && set[2].num == set[3].num)) {
            return qMakePair(8, set[2].getNum());//4+2
        } else if(set[0].getNum() == set[1].getNum() && set[1].getNum() == set[2].getNum() && set[3].getNum() == set[4].getNum() && set[4].getNum() == set[5].getNum() && set[2].num + 1 == set[3].num) {
            return qMakePair(9, set[0].getNum());//fei
        }
    }
    if(size == 8) {
        if((set[0].getNum() == set[1].getNum() && set[2].getNum() == set[3].getNum() && set[4].getNum() == set[5].getNum() && set[6].getNum() == set[7].getNum())) {
            int val = 0;
            if(set[5].getNum() == set[6].getNum()) val = set[5].getNum();
            else if(set[3].getNum() == set[4].getNum()) val = set[3].getNum();
            else if(set[1].getNum() == set[2].getNum()) val = set[1].getNum();
            if(val != 0) {
                return qMakePair(10, val);//4+2*2
            }
        } else {
            int stat[50];
            memset(stat, 0, sizeof(stat));
            for(Card& a : set) {
                ++stat[a.getNum()];
            }
            for(int i = 3; i <= 13; ++i) {
                if(stat[i] >= 3 && stat[i + 1] >= 3) {
                    return qMakePair(11, i);//feixiao
                }
            }
        }
    }
    if(size == 10) {
        int stat[50];
        memset(stat, 0, sizeof(stat));
        for(Card& a : set) {
            ++stat[a.getNum()];
        }
        for(int i = 3; i <= 13; ++i) {
            if(stat[i] >= 3 && stat[i + 1] >= 3) {
                int two = 0;
                for(int j = 3; j <= 25; ++j) {
                    if(j != i && j != i + 1) {
                        if(stat[j] == 2) {
                            ++two;
                        } else if(stat[j] == 4) {
                            two += 2;
                        }
                    }

                }
                if(two == 2) {
                    return qMakePair(12, i);//feida
                }
            }
        }
    }
    if(size >= 5) {
        bool shun = true;
        for(int i = 0; i < size - 1; ++i) {
            if(set[i + 1].getNum() != set[i].getNum() + 1) {
                shun = false;
                break;
            }
        }
        if(shun) return qMakePair(13, set[0].getNum() + size * 15);//shun
    }
    if(size >= 6 && !(size % 2)) {
        bool lian = true;
        for(int i = 0; i < size / 2 - 1; ++i) {
            if((set[2 * i].getNum() != set[2 * i + 1].getNum()) || (set[2 * i + 2].getNum() != set[2 * i].getNum() + 1)) {
                lian = false;
                break;
            }
        }
        if(lian) return qMakePair(14, set[0].getNum() + size / 2 * 15);//lian
    }
    return qMakePair(0, 0);//busted
}

QString Card::getfilename(Card c, bool solid) {
    int _num = c.getNum();
    int _suit = c.getSuit();
    QString suit, num;
    if(_num == 25) {
        num = "O";
        suit = "B";
    } else if(_num == 26) {
        num = "O";
        suit = "R";
    } else {
        if(_suit == 1) {
            suit = "H";
        } else if(_suit == 2) {
            suit = "D";
        } else if(_suit == 3) {
            suit = "S";
        } else if(_suit == 4) {
            suit = "C";
        }
        if(_num >= 3 && _num <= 10) {
            num = QString::number(_num);
        } else if(_num == 11) {
            num = "J";
        } else if(_num == 12) {
            num = "Q";
        } else if(_num == 13) {
            num = "K";
        } else if(_num == 14) {
            num = "A";
        } else if(_num == 20) {
            num = "2";
        }
    }
//    qDebug() << ":img/resource/img/" + suit + num + ".png";
    return ":img/resource/img/" + suit + num + ".png";
}
