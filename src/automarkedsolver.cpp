#include "automarkedsolver.h"
#include <cassert>
#include <limits>
#include <iostream>

namespace ConnectFour
{
    AutomarkedSolver::AutomarkedSolver(int maxDepth, bool prune) :
        maxDepth(maxDepth),
        prune(prune),
        nodesExamined(0)
    {
        assert(maxDepth >= 0);
    }

    // Get the best move from the given board, and updates outValue with the value for the best move
    // Returns -1 if there are no legal moves or it is unknown
    int AutomarkedSolver::bestMove(const Board &board, int *outValue, int depth, int alpha, int beta)
    {
        ++nodesExamined;

        // Get the board from the opposite player's perspective
        Board other(board);
        other.swap();

        // Check for terminal node
        if (board.isWin())
        {
            // This won't ever happen
            *outValue = 10000;
            return -1;
        }
        else if (other.isWin())
        {
            *outValue = -10000;
            return -1;
        }
        else if (board.count() + other.count() == Board::width*Board::height)
        {
            // Draw
            *outValue = 0;
            return -1;
        }

        // Check for leaf node
        if (depth == maxDepth)
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
            b.swap(); // Current player is now the other player
            int value;
            bestMove(b, &value, depth + 1, -beta, -alpha);
            // The move is evaluated in terms of the other player, so invert it
            value = -value;
            if (value > bestValue)
            {
                bestValue = value;
                move = col;
            }
            // Update alpha
            if (prune)
            {
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
        }

        *outValue = bestValue;
        return move;
    }

    int AutomarkedSolver::solve(const Board &board)
    {
        nodesExamined = 0;
        int value;
        return bestMove(board, &value, 0, std::numeric_limits<int>::min() + 1, std::numeric_limits<int>::max() - 1);
    }

    int AutomarkedSolver::numberOfNodesExamined() const
    {
        return nodesExamined;
    }

    void AutomarkedSolver::printStatistics(std::ostream &out) const
    {
        out << "Nodes examined: " << nodesExamined << std::endl;
    }

    // Compute score for current player
    int AutomarkedSolver::score(const Board &board)
    {
        Board::connectionsArray connections = board.countConnections();
        return board.count() + 10*connections[0] + 100*connections[1] + 1000*connections[2];
    }
}
