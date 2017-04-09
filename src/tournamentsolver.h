#pragma once
#include "solver.h"
#include <ctime>

namespace ConnectFour
{
    class TournamentSolver : public Solver
    {
    public:
        /// @brief  Construct a solver that uses techniques such as iterative deepening, transposition table to improve performance
        /// @param  targetSolveTime The time in milliseconds that the solver should take to predict the best move.
        /// @param  startDepth The depth of the search tree in the first iteration
        /// @param  depthStep The increase in depth after each iteration
        TournamentSolver(int maxSolveTime, int startDepth, int depthStep, int maxDepth = -1);

        int solve(const Board &board);
        void printStatistics(std::ostream &out) const;

    private:
        const int maxSolveTime;

        // Search depth parameters
        const int startDepth;
        const int depthStep;
        const int maxDepth;

        // The number of clock ticks to end computation at
        std::clock_t endTicks;
        static const int clocksPerMillisecond = CLOCKS_PER_SEC / 1000;
        // Whether to complete computation as soon as possible
        bool outOfTime;

        // Enum for types of board evaluation
        enum EvaluationType
        {
            evaluation_exact,
            evaluation_belowAlpha, // Value is an upper bound
            evaluation_aboveBeta // Value is a lower bound
        };
        // Struct to store data about a board evaluation for future use
        struct BoardEvaluation
        {
            Board::Hash hash;
            int move; // Move determined to be best for current player
            int value; // Minimax value for the position based on current player
            int height; // Height of the subtree rooted at this position (depends on iteration)
            EvaluationType type;
        };
        // Transposition table
        static const int transpositionTableSize = 1048576; // 2^20 - 24MB
        BoardEvaluation *table;

        // Statistics for last solve
        int nodesExamined;
        int tableHits; // Times required position was in table
        int tableReplacements; // Collisions where old value was replaced
        int tableIgnores; // Collisions where old value was left

        int bestMove(const Board &board, int *outValue, int depth, int alpha, int beta);
        inline static int score(const Board &board);
    };
}