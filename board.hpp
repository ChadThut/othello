#ifndef __BOARD_H__
#define __BOARD_H__

#include <bitset>
#include "common.hpp"
using namespace std;

class Board {

private:
    unsigned long long taken;
    unsigned long long black;

    bool occupied(int x, int y);
    bool get(Side side, int x, int y);
    void set(Side side, int x, int y);
    bool onBoard(int x, int y);

public:
    Board();
    ~Board();
    Board *copy();

    bool isDone();
    bool hasMoves(Side side);
    bool checkMove(Move *m, Side side);
    void doMove(Move *m, Side side);
    int count(Side side);
    int countBlack();
    int countWhite();
	long long getTaken();
	long long getBlack();
    void setBoard(char data[]);
    void setInt(Side side, int a);

    int simpleValue(Side side);
    int value(Side side);
    int mobility(Side side);
};

#endif
