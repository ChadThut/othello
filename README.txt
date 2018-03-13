Andrew made the bit board and, setup the move to use alpha beta pruning after
Chad created the alpha beta pruning method. He also made the endgame solver
and made the depth of the search depend on the time left.

'Chad kinda sat around, did some pruning, complained about how arbitrary
the improvements we made were, and generally just kinda held me back. He
also threw down some nonsense in the heuristic about mobility, and
it made us better against BetterPlayer, but made us lose  in the
tournament.'-Chad


To make the bot tournament worthy, we made the depth of search depend on how
much time we have left so that it would probably not timeout. We also used
alpha beta pruning to make the search a little faster and we used a pretty
simple heuristic so we could search deeper. We tried to sort the moves in
alpha beta pruning by the one that gives us the best score immediately so that
the pruning would be more effective, but this ended up taking more time so we
removed it.
