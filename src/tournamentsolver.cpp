#include "tournamentsolver.h"
#include <cassert>
#include <limits>

namespace ConnectFour
{
    TournamentSolver::TournamentSolver(int maxSolveTime, int startDepth, int depthStep, int maxDepth) :
        maxSolveTime(maxSolveTime),
        startDepth(startDepth),
        depthStep(depthStep),
        maxDepth(maxDepth),
        nodesExamined(0)
    {
        assert(maxSolveTime > 0);
        assert(startDepth > 0);
        assert(depthStep > 0);
    }

    // Get the best move from the given board, and updates outValue with the value for the best move
    // Returns -1 if there are no legal moves or it is unknown
    int TournamentSolver::bestMove(const Board &board, int *outValue, int height, int alpha, int beta)
    {
        ++nodesExamined;

        // Get the board from the opposite player's perspective
        Board other(board);
        other.swap();

        // Check for draw
        if (board.count() + other.count() == Board::width*Board::height)
        {
            *outValue = 0;
            return -1;
        }

        // Check for leaf node
        if (height == 0)
        {
            *outValue = score(board) - score(other);
            return -1;
        }

        // Compute the move in the next level with best minimax value for the current player
        int move = -1;
        int bestValue = std::numeric_limits<int>::min();
        // Check each possible move from left to right
        for (int col = 0; col < Board::width; ++col)
        {
            if (!board.canPlay(col)) continue;
            // Create copy of board with move performed
            Board b(board);
            b.play(col);
            // Check for winning move
            if (b.isWin())
            {
                // Utility
                *outValue = 10000;
                return col;
            }
            b.swap(); // Current player is now the other player
            int value;
            bestMove(b, &value, height - 1, -beta, -alpha);
            // Stop if out of time
            if (col == 0 && (height % 4) == 0) // Only check the time occasionally
            {
                outOfTime = std::clock() >= endTicks;
            }
            if (outOfTime)
            {
                // Return worst possible value
                // TODO break so that best found so far is chosen. Then outermost will return best move so far,
                // which is ok because it checks move that was best in the last iteration first
                *outValue = std::numeric_limits<int>::min() - 1;
                return -1;
            }
            // The move is evaluated in terms of the other player, so invert it
            value = -value;
            if (value > bestValue)
            {
                bestValue = value;
                move = col;
            }
            
            // Pruning
            if (value > alpha)
            {
                alpha = value;
            }
            if (alpha >= beta)
            {
                // This branch will be too high for the previous player to choose it
                break;
            }
        }

        *outValue = bestValue;
        return move;
    }

    int TournamentSolver::solve(const Board &board)
    {
        nodesExamined = 0;
        endTicks = std::clock() + maxSolveTime * clocksPerMillisecond;
        outOfTime = false;

        int value;
        int move = -1;
        for (int height = startDepth; height != maxDepth + 1; ++height)
        {
            int newMove = bestMove(board, &value, height, std::numeric_limits<int>::min() + 1, std::numeric_limits<int>::max() - 1);
            if (newMove == -1)
            {
                // Ran out of time or no possible moves
                return move;
            }
            move = newMove;
        }
        return move;
    }

    void TournamentSolver::printStatistics(std::ostream &out) const
    {
        out << "Nodes examined: " << nodesExamined << std::endl;
    }

    // Compute score for current player
    inline int TournamentSolver::score(const Board &board)
    {
        Board::connectionsArray connections = board.countConnections();
        return board.count() + 10*connections[0] + 100*connections[1] + 1000*connections[2];
    }
}
