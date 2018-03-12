#include "board.hpp"

#define SET(num, i) ((num) |= (1ull << (i)))
#define GET(num, i) ((num) &  (1ull << (i)))
#define SET_ZERO(num, i) ((num) &= ~(1ull << (i)))

#include <iostream>
using namespace std;

/*
 * Make a standard 8x8 othello board and initialize it to the standard setup.
 */
Board::Board() {
    taken = 0ull;
    black = 0ull;

/*
    SET(taken, 1);
    SET(taken, 2);
    SET(taken, 3);
    SET(black, 3);

    cerr << "taken 1 " << GET(taken, 1) << endl;
    cerr << "taken 2 " << GET(taken, 2) << endl;
    cerr << "taken 5 " << GET(taken, 5) << endl;
*/

    SET(taken, 3 + 8 * 3);
    SET(taken, 3 + 8 * 4);
    SET(taken, 4 + 8 * 3);
    SET(taken, 4 + 8 * 4);
    SET(black, 4 + 8 * 3);
    SET(black, 3 + 8 * 4);

/*
    cerr << "taken:" << endl;
    for (int i = 0; i < 8; i++) {
        for (int k = 0; k < 8; k++) {
            cerr << (GET(taken, i + 8*k) ? 1 : 0);
        }
        cerr << endl;
    }

    cerr << endl;

    cerr << "black:" << endl;
    for (int i = 0; i < 8; i++) {
        for (int k = 0; k < 8; k++) {
            cerr << (GET(black, i + 8*k) ? 1 : 0);
        }
        cerr << endl;
    }

    cerr << endl;
*/
}

/*
 * Destructor for the board.
 */
Board::~Board() {
}

/*
 * Returns a copy of this board.
 */
Board *Board::copy() {
    Board *newBoard = new Board();
    newBoard->black = black;
    newBoard->taken = taken;
    return newBoard;
}

bool Board::occupied(int x, int y) {
    //return taken[x + 8*y];
    return GET(taken, x + 8*y);
}

bool Board::get(Side side, int x, int y) {
    //return occupied(x, y) && (black[x + 8*y] == (side == BLACK));
    return occupied(x, y) && (!GET(black, x + 8*y) == (side != BLACK));
}

void Board::set(Side side, int x, int y) {
    SET(taken, x + 8*y);

    if (side == BLACK) {
        SET(black, x + 8*y);
    }
    else {
        SET_ZERO(black, x + 8*y);
    }
    //black.set(x + 8*y, side == BLACK);
}

bool Board::onBoard(int x, int y) {
    return(0 <= x && x < 8 && 0 <= y && y < 8);
}


/*
 * Returns true if the game is finished; false otherwise. The game is finished
 * if neither side has a legal move.
 */
bool Board::isDone() {
    return !(hasMoves(BLACK) || hasMoves(WHITE));
}

/*
 * Returns true if there are legal moves for the given side.
 */
bool Board::hasMoves(Side side) {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            Move move(i, j);
            if (checkMove(&move, side)) return true;
        }
    }
    return false;
}

/*
 * Returns true if a move is legal for the given side; false otherwise.
 */
bool Board::checkMove(Move *m, Side side) {
    // Passing is only legal if you have no moves.
    if (m == nullptr) return !hasMoves(side);

    int X = m->getX();
    int Y = m->getY();

    // Make sure the square hasn't already been taken.
    if (occupied(X, Y)) return false;

    Side other = (side == BLACK) ? WHITE : BLACK;
    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            if (dy == 0 && dx == 0) continue;

            // Is there a capture in that direction?
            int x = X + dx;
            int y = Y + dy;
            if (onBoard(x, y) && get(other, x, y)) {
                do {
                    x += dx;
                    y += dy;
                } while (onBoard(x, y) && get(other, x, y));

                if (onBoard(x, y) && get(side, x, y)) return true;
            }
        }
    }
    return false;
}

/*
 * Modifies the board to reflect the specified move.
 */
void Board::doMove(Move *m, Side side) {
    // A nullptr move means pass.
    if (m == nullptr) return;

    // Ignore if move is invalid.
    if (!checkMove(m, side)) return;

    int X = m->getX();
    int Y = m->getY();
    Side other = (side == BLACK) ? WHITE : BLACK;
    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            if (dy == 0 && dx == 0) continue;

            int x = X;
            int y = Y;
            do {
                x += dx;
                y += dy;
            } while (onBoard(x, y) && get(other, x, y));

            if (onBoard(x, y) && get(side, x, y)) {
                x = X;
                y = Y;
                x += dx;
                y += dy;
                while (onBoard(x, y) && get(other, x, y)) {
                    set(side, x, y);
                    x += dx;
                    y += dy;
                }
            }
        }
    }
    set(side, X, Y);
}

/*
 * Current count of given side's stones.
 */
int Board::count(Side side) {
    return (side == BLACK) ? countBlack() : countWhite();
}

/*
 * Current count of black stones.
 */
int Board::countBlack() {
    //return black.count();

    int res = 0;

    for (int i = 0; i < 64; i++) {
        res += GET(black, i);
    }

    return res;
}

/*
 * Current count of white stones.
 */
int Board::countWhite() {
    //return taken.count() - black.count();

    int res = 0;

    for (int i = 0; i < 64; i++) {
        res += GET(taken, i);
        res -= GET(black, i);
    }

    return res;
}

/*
 * Sets the board state given an 8x8 char array where 'w' indicates a white
 * piece and 'b' indicates a black piece. Mainly for testing purposes.
 */
void Board::setBoard(char data[]) {
    //taken.reset();
    //black.reset();
    taken = 0ll;
    black = 0ll;

    for (int i = 0; i < 64; i++) {
        if (data[i] == 'b') {
            //taken.set(i);
            //black.set(i);
            SET(taken, i);
            SET(black, i);
        } if (data[i] == 'w') {
            //taken.set(i);
            SET(taken, i);
        }
    }
}

/*
 * Use a naive approach to the board value by finding the net number of pieces
 * for a given side.
 */
int Board::simpleValue(Side side) {
    return count(side) - count(side == WHITE ? BLACK : WHITE);
}
/*
 *  Calculate the 'mobility' of a given side (number of available moves)
 */

int Board::mobility(Side side)
{
	int count = 0;
	Move *move = new Move(0, 0);
	for(int i = 0; i < 64; i ++)
	{
		move->setX(i / 8);
		move->setY(i % 8);
			if(checkMove(move, side))
				count ++;
	}
	return count;
				
}
/*
 * Use a heuristic to approximate the value of the board.
 */
int Board::value(Side side) {
    /* Calculate the value for black, then negate the value if it's white. */
    int res = 0;

    for (int i = 0; i < 64; i++) {
        if (i == 0 || i == 7 || i == 56 || i == 63) {
            /* These are the corners. */
            if (GET(taken, i)) {
                //res += 50 * (black[i] ? 1 : -1);
                res += 50 * (GET(black, i) ? 1 : -1);
            }
        }
        else if (i == 9 || i == 14 || i == 49 || i == 54) {
            /* These are the squares just inside the corners. */
            if (GET(taken, i)) {
                //res += 10 * (black[i] ? -1 : 1);
                res += 10 * (GET(black, i) ? -1 : 1);
            }
        }
        else if (i == 1 || i == 6 || i == 8 || i == 15 || i == 48 || i == 55
                    || i == 57 || i == 62) {
            /* These are the edge squares right next to a corner. */
            //if (taken[i]) {
            if (GET(taken, i)) {
                //res += 20 * (black[i] ? -1 : 1);
                res += 20 * (GET(black, i) ? -1 : 1);
            }
        }
        else if (i / 8 == 0 || i / 8 == 7 || i % 8 == 0 || i % 8 == 7) {
            /* These are the other edge squares. */
            //if (taken[i]) {
            if (GET(taken, i)) {
                //res += 7 * (black[i] ? 1 : -1);
                res += 7 * (GET(black, i) ? 1 : -1);
            }
        }
        else {
            /* These are the middle squares. */
            //if (taken[i]) {
            if (GET(taken, i)) {
                //res += (black[i] ? 1 : -1);
                res += (GET(black, i) ? 1 : -1);
            }
        }
    }

    /* The value was calculated for black. */
    return (side == BLACK ? res : -res);
}
