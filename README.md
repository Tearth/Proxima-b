# Proxima b
Chess engine based on min-max algorithm and transposition tables. 

# Supported modes
* Player vs AI
* AI vs AI
* AI vs FICS (Free Internet Chess Server)

# Used algorithms
* negamax search - simplify implementation of minmax algorithm
* alpha-beta pruning - decreases the number of nodes parsed by the negamax
* transposition tables - cache for parsed positions, prevents from double calculation of node
* iterative deepening - iterative increasing of maximal depth
* quiescence search - prevents from horizon problem, calculates very deep nodes (only captures, promotions or checks)
* Zobrist hashing - calculates unique number for the board

# Console commands
| Name        	| Description   			| 
| ------------- | ------------------------- | 
| Hello (Hi)          | writes "Hello" on the console | 
| AddPiece s x y      	| adds the specific piece [s] on position [x, y] | 
| Save x      	| saves the board to the specific file [x] | 
| Load x      	| loads the board from the specific file [x] | 
| Heuristic (Heur)     	|  gets an heuristic value of the board | 
| MinMax (n) c      	|  calls AI algorithm for the specific color [c] | 
| GetHash (Hash)       	|  gets hash of the board | 
| IsMate (Mate) c      	|  writes "True" if the king with color [c] is mates | 
| MarkPos (MPos) x y      	|  marks the specific position | 
| DoMove (DM) a b x y      	|  does move from [a, b] to [x, y] | 
| SetMaxTimeAI (MaxTime) s x y      	|  sets max time for AI algorithm | 
| SetGameMode (GameMode) x      	|  sets the game mode [x] | 
| StopFICS      	| stops fics thread  | 
| ResetBoard (Reset)     	|  resets board to default values | 
| Help (H)       	|  writes list of commands  | 
| SendFICSCommand (SendFICS) x      	|  sends specific text [x] to the FICS server | 
| FISCLoop(GameMode) x      	|  sets FICS games loop | 