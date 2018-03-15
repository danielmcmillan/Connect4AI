#pragma once
#include "solver.h"

namespace ConnectFour
{
    class AutomarkedSolver : public Solver
    {
    public:
        // Construct a solver that satisfies specified conditions defined in the assignment
        // Searches the game tree to the specified maxDepth with alpha-beta pruning optionally enabled
        AutomarkedSolver(int maxDepth, bool prune);

        int solve(const Board &board);
        int numberOfNodesExamined() const;
        void printStatistics(std::ostream &out) const;

    private:
        const bool prune;
        const int maxDepth;
        int nodesExamined;

        int bestMove(const Board &board, int *outValue, int depth, int alpha, int beta);
        static int score(const Board &board);
    };
}
