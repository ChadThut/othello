#ifndef __PLAYER_H__
#define __PLAYER_H__

#include <iostream>
#include "common.hpp"
#include "board.hpp"
using namespace std;

class Player {

public:
    Player(Side side);
    ~Player();

    Move *doMove(Move *opponentsMove, int msLeft);
    int miniMax(int depth, Side side, Board *board, bool maxPlayer, int a, int b);
    void loadBoard(Board *b);

    // Flag to tell if the player is running within the test_minimax context
    bool testingMinimax;

private:
    bool endgame;
    Side side;
    Board board;
};

#endif
