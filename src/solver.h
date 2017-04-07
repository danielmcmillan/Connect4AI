#pragma once
#include "board.h"

namespace ConnectFour
{
    // Abstract class for an object that will predict the best move for a given board
    class Solver
    {
    public:
        virtual ~Solver() {};

        // Returns the board column to play for the specified board
        virtual int solve(const Board &board) = 0;

        // Returns the number of tree nodes examined by the solver when computing the last move,
        // if the solver implementation counts them. Otherwise returns 0.
        virtual int numberOfNodesExamined() const { return 0; }

    protected:
        Solver() {};
    };
}