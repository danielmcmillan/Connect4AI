#include "tournamentsolver.h"
#include <cassert>
#include <limits>
#include <cstring>
#include <algorithm>
#include <tr1/array>

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
        assert(maxDepth == -1 || maxDepth >= startDepth);
    }

    int TournamentSolver::solve(const Board &board)
    {
        nodesExamined = 0;
        tableHits = 0;
        tableReplacements = 0;
        tableIgnores = 0;

        // Initialise timing
        endTicks = std::clock() + maxSolveTime * clocksPerMillisecond;
        outOfTime = false;

        // Initialise transposition table
        table = new BoardEvaluation[transpositionTableSize]();

        // Find the range of heights to iterate
        int movesToDraw = Board::width*Board::height - board.totalCount();
        int maxHeight = (maxDepth != -1) ? std::min(maxDepth, movesToDraw) : movesToDraw;
        int height = std::min(startDepth, maxHeight);

        int value;
        int move = -1;
        for (; height <= maxHeight; height += depthStep)
        {
            int newMove = bestMove(board, &value, height, std::numeric_limits<int>::min() + 1, std::numeric_limits<int>::max() - 1);
            if (newMove == -1)
            {
                // Ran out of time or no possible moves
                break;
            }
            move = newMove;
        }

        delete [] table;
        return move;
    }

    void TournamentSolver::printStatistics(std::ostream &out) const
    {
        out << "Nodes examined: " << nodesExamined  << std::endl
            << "Table hit/replace/ignore: " << tableHits << "/" << tableReplacements << "/" << tableIgnores << std::endl;
    }

    int TournamentSolver::bestMove(const Board &board, int *outValue, int height, int alpha, int beta)
    {
        // Check whether result is in the transposition table
        BoardEvaluation *eval = tableEntryFor(board);
        if (eval->height == height && eval->hash == board.getHash())
        {
            ++tableHits;
            switch (eval->type)
            {
            case evaluation_belowAlpha:
                // Value is an upper bound. Previous player will trim this move unless it is above alpha.
                if (eval->value < alpha)
                {
                    *outValue = alpha;
                    return -1;
                }
                // beta = eval->value??? it is known that move is no better than that
                break;
            case evaluation_aboveBeta:
                // Value is a lower bound. Possible early beta cutoff.
                if (eval->value >= beta)
                {
                    *outValue = beta;
                    return eval->move;
                }
                // alpha = eval->value-1??? since it is known that this move is atleast that good
                break;
            default:
                // Value is exact and best move is already known.
                *outValue = eval->value;
                return eval->move;
            }
        }

        ++nodesExamined;

        // Handle leaf nodes
        if (height == 0)
        {
            if (board.totalCount() == Board::width*Board::height)
            {
                // Draw (full board)
                *outValue = 0;
            }
            else
            {
                // Non-terminal leaf node, use heuristic
                *outValue = score(board);
            }
            storeInTable(board, -1, *outValue, 0, evaluation_exact);
            return -1;
        }

        // Get the boards and columns of moves to explore
        std::tr1::array<Board, Board::width> boards;
        std::tr1::array<int, Board::width> moveOrder;
        int winningMove = playAllMoves(board, boards, moveOrder);
        if (winningMove != -1)
        {
            // Utility function prefers sooner wins
            *outValue = (Board::width*Board::height - board.totalCount() + 1) * 1000;
            // Return from winning moves without exploring any other moves
            storeInTable(board, winningMove, *outValue, height, evaluation_exact);
            return winningMove;
        }
        orderMoves(boards, moveOrder);

        // Check whether out of time
        if ((height % 4) == 0) // TODO Only check the time occasionally
        {
            outOfTime = std::clock() >= endTicks;
        }
        
        // Compute the move in the next level with best minimax value for the current player
        int move = -1;
        *outValue = std::numeric_limits<int>::min();
        EvaluationType evalType = evaluation_belowAlpha;
        for (int i = 0; i < Board::width; ++i)
        {
            int column = moveOrder[i];
            if (column == -1) continue;

            int value;
            bestMove(boards[column], &value, height - 1, -beta, -alpha);

            if (outOfTime)
            {
                // Return the best move found so far
                return move;
            }

            // The move is evaluated in terms of the other player, so invert it
            value = -value;

            // if (height == maxDepth)
            // {
            //     std::cout << "Column " << column << " has value " << value << std::endl;
            // }

            // Update maximum
            if (value > *outValue)
            {
                *outValue = value;
                move = column;
            }
            
            // Pruning
            if (value > alpha)
            {
                alpha = value;
                // value>alpha => -value<-alpha, so value<beta in child. Therefore atleast one child is exact, and
                // the value returned will be exact unless beta cutoff occurs.
                evalType = evaluation_exact;
            }
            if (alpha >= beta)
            {
                // This branch will be too high for the previous player to choose it.
                // The returned value is not exact, only a lower bound
                evalType = evaluation_aboveBeta;
                break;
            }
        }

        storeInTable(board, move, *outValue, height, evalType);
        return move;
    }

    int TournamentSolver::playAllMoves(const Board &board, std::tr1::array<Board, Board::width> &boards, std::tr1::array<int, Board::width> &columns)
    {
        // Play each column
        for (int i = 0; i < Board::width; ++i)
        {
            if (board.canPlay(i))
            {
                columns[i] = i;
                boards[i] = board;
                boards[i].play(i);
                if (boards[i].isWin())
                {
                    return i;
                }
                boards[i].swap();
            }
            else
            {
                columns[i] = -1;
            }
        }
        return -1;
    }

    // Comparison function to sort by descending move value
    struct MoveCompare
    {
        const std::tr1::array<int, Board::width> &moveValues;
        MoveCompare(const std::tr1::array<int, Board::width> &values) : moveValues(values) {}
        bool operator()(int col1, int col2)
        {
            if (col1 == -1) return false;
            if (col2 == -1) return true;
            return moveValues[col1] > moveValues[col2];
        }
    };

    void TournamentSolver::orderMoves(const std::tr1::array<Board, Board::width> &boards, std::tr1::array<int, Board::width> &columns)
    {
        // Get values for each move
        std::tr1::array<int, Board::width> moveValues;
        for (int i = 0; i < Board::width; ++i)
        {
            int column = columns[i];
            if (column != -1)
            {
                const Board &board = boards[column];
                BoardEvaluation *eval = tableEntryFor(board);
                if (eval->hash == board.getHash() && eval->type == evaluation_exact)
                {
                    // Value based on stored value from previous iteration
                    moveValues[column] = eval->value;
                }
                else
                {
                    moveValues[column] = 0;
                }
                // Adjust value based on closeness to centre position (assume centre is better)
                moveValues[column] += 100000 * ((Board::width/2) - std::abs(columns[i] - (Board::width/2)));
            }
        }

        std::sort(columns.begin(), columns.end(), MoveCompare(moveValues));
    }

    void TournamentSolver::storeInTable(const Board &board, int move, int value, int height, EvaluationType type)
    {
        BoardEvaluation *eval = tableEntryFor(board);
        if (height > eval->height)
        {
            if (eval->height != 0)
            {
                ++tableReplacements;
            }
            // Store the move in the transposition table
            eval->hash = board.getHash();
            eval->move = move;
            eval->value = value;
            eval->height = height;
            eval->type = type;
        }
        else
        {
            ++tableIgnores;
        }
    }

    int TournamentSolver::score(const Board &board)
    {
        Board::ThreatInfo info = board.getThreatInfo();
        return board.countPossibleConnections(false) - board.countPossibleConnections(true)
            + 70*(info.allThreats[0] - info.allThreats[1])
            + 100*(info.groundedThreats[0] - info.groundedThreats[1])
            + 150*(info.doubleThreats[0] - info.doubleThreats[1]);
    }
}
