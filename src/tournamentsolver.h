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
        int numberOfNodesExamined() const;

    private:
        const int maxSolveTime;
        const int startDepth;
        const int depthStep;
        const int maxDepth;

        // The number of clock ticks to end computation at
        std::clock_t endTicks;
        bool outOfTime;
        static const int clocksPerMillisecond = CLOCKS_PER_SEC / 1000;

        int nodesExamined;

        int bestMove(const Board &board, int *outValue, int depth, int alpha, int beta);
        inline static int score(const Board &board);
    };
}