#include "player.hpp"

/*
 * Constructor for the player; initialize everything here. The side your AI is
 * on (BLACK or WHITE) is passed in as "side". The constructor must finish
 * within 30 seconds.
 */
Player::Player(Side s) {
    // Will be set to true in test_minimax.cpp.
    testingMinimax = false;

    /*
     * TODO: Do any initialization you need to do here (setting up the board,
     * precalculating things, etc.) However, remember that you will only have
     * 30 seconds.
     */

     board = Board();
     side = s;
     endgame = false;
}

/*
 * Destructor for the player.
 */
Player::~Player() {
}

/*
 * Sets the player's board to the given board.
 */
void Player::loadBoard(Board *b) {
     board = *b;
 }

/*
 * Compute the next move given the opponent's last move. Your AI is
 * expected to keep track of the board on its own. If this is the first move,
 * or if the opponent passed on the last move, then opponentsMove will be
 * nullptr.
 *
 * msLeft represents the time your AI has left for the total game, in
 * milliseconds. doMove() must take no longer than msLeft, or your AI will
 * be disqualified! An msLeft value of -1 indicates no time limit.
 *
 * The move returned must be legal; if there are no valid moves for your side,
 * return nullptr.
 */
Move *Player::doMove(Move *opponentsMove, int msLeft) {

    /* Update the board with the opponent's move. */
    board.doMove(opponentsMove, (side == WHITE ? BLACK : WHITE));

    Move *best = new Move(-1, -1);
    Move *move = new Move (0, 0);
    int bestValue = -1e9;

    /* Check if the game is near the end. */
    if (board.countBlack() + board.countWhite() >= 50) {
        endgame = true;
    }

    for (int i = 0; i < 64; i++) {
        move->setX(i / 8);
        move->setY(i % 8);

        if (board.checkMove(move, side)) {
            /* Use the board heuristic to find the best move. */
            Board *copy = board.copy();
            copy->doMove(move, side);

            int value = 0;
            //value = copy->value(side); // heuristic

            if (endgame) {
                value = miniMax(20, (side == WHITE ? BLACK : WHITE), copy, false, -1e8, 1e8);
            }

            else {
                if (testingMinimax) {
                    value = miniMax(1, (side == WHITE ? BLACK : WHITE), copy, false, -1e8, 1e8);
                }
                else {
                    value = miniMax(7, (side == WHITE ? BLACK : WHITE), copy, false, -1e8, 1e8);
                }
            }

            /* Update the best move. */
            if (value > bestValue) {
                bestValue = value;
                best->setX(move->getX());
                best->setY(move->getY());
            }

            delete copy;
        }
    }

    /* We don't need 'move' anymore. */
    delete move;

    /* Do the best move. */
    if(best->getX() == -1)
    {
        delete best;
        best = nullptr;
    }
    board.doMove(best, side);

    return best;
}

int Player::miniMax(int depth, Side side, Board *board, bool maxPlayer, int alpha, int beta)
{
    int inf = 1e8;

	if (depth == 0 || board->isDone())
	{
        /* Use the naive heuristic for testing minimax and for endgame. */
        if (testingMinimax || endgame) {
            return board->simpleValue(side);
        }
		return board->value(side);
	}

	int v;
	if (maxPlayer)
	{
		v = -inf;
		Move *move = new Move(0, 0);
		for(int i = 0; i < 64; i++)
		{
		 move->setX(i / 8);
		 move->setY(i % 8);
			if (board->checkMove(move, side))
			{
				Board *copy = board->copy();
			    copy->doMove(move, side);
				v = max(v, miniMax(depth - 1, (side == WHITE ? BLACK : WHITE), copy, false, alpha, beta));
				alpha = max(alpha, v);
				delete copy;
				if(beta <= alpha)
					break;
			}
		}

        delete move;

		return (v == -inf ? board->value(side) : v);
	}
	else
	{
	    v = inf;
	    Move *move = new Move(0, 0);
	    for(int i = 0; i < 64; i ++)
	    {
		 move->setX(i / 8);
		 move->setY(i % 8);
			if(board->checkMove(move, side))
			{
				Board *copy = board->copy();
				copy->doMove(move, side);
				v = min(v, miniMax(depth -1, (side == WHITE ? BLACK : WHITE), copy, true, alpha, beta));
				beta = min(beta, v);
				delete copy;
				if(beta <= alpha)
					break;
			}
		}

        delete move;

		return (v == inf ? board->value(side) : v);
	}
}
