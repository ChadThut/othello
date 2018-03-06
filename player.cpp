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
}

/*
 * Destructor for the player.
 */
Player::~Player() {
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
    /*
     * TODO: Implement how moves your AI should play here. You should first
     * process the opponent's opponents move before calculating your own move
     */

     /* Update the board with the opponent's move. */
     board.doMove(opponentsMove, (side == WHITE ? BLACK : WHITE));

     Move *best = nullptr;
     int bestValue = INT_MIN;

     for (int i = 0; i < 64; i++) {
         Move *move = new Move(i / 8, i % 8);

         if (board.checkMove(move, side)) {
             /* Use the board heuristic to find the best move. */
             Board *copy = board.copy();
             copy->doMove(move, side);
             int value = copy->value(side);

//             fprintf(stderr, "value: %d, (x, y): (%d, %d)\n", value, move->getX(), move->getY());

             if (value > bestValue) {
                 bestValue = value;
                 best = move;
             }

            delete copy;
         }

        // delete move;
     }

     if (best) {
//         fprintf(stderr, "best move: (%d, %d)\n", best->getX(), best->getY());
     }
     else {
//         fprintf(stderr, "pass");
     }

//     fprintf(stderr, "-------------------------------------\n");

     /* We don't need 'move' anymore. */
     //delete move;

     /* Do the best move. */
     board.doMove(best, side);

    return best;
}

int miniMax(int depth, Side side, Board board)
{
	if (depth == 0)
	{
		return board.value(side);
	}
	int bestVal , v; 
	if (depth % 2 == 0) //even number of depth --> you are the maximizing player
	{
		bestVal = -1e8;
		for(int i = 0; i < 64; i++)
		{
			Move *move = new Move(i / 8, i % 8);
			if (board.checkMove(move, side))
			{
				Board *copy = board.copy();
				copy->doMove(move, side);
				v = miniMax(depth - 1, (side == WHITE ? BLACK : WHITE), *copy);
				bestVal = max(v, bestVal);
				delete copy;
			}

		}
		return bestVal;
	}
	else
	{
	    bestVal = 1e8;
	    for(int i = 0; i < 64; i ++)
	    {
			Move *move = new Move(i / 8, i % 8);
			if(board.checkMove(move, side))
			{
				Board *copy = board.copy();
				copy->doMove(move, side);
				v = miniMax(depth -1, side, *copy);
				bestVal = min(bestVal, v);
				delete copy;
			}

		}
		return bestVal;
	    
	}
}
