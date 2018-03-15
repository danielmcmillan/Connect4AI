#pragma once

#include <iostream>
#include "board.h"

namespace ConnectFour
{
    /// @class Solver
    /// @brief Abstract class for an object that will predict the best move for a given board
    class Solver
    {
    public:
        virtual ~Solver() {};

        /// @brief Gets the best move to play for a specific board state.
        /// @param board Board object representing a board state.
        /// @return Number of the column for the move found, or -1 if no move could be found.
        virtual int solve(const Board &board) = 0;

        /// @brief Gets statistics for the last solve if they have been recorded.
        /// @return A string describing the statistics, or empty string if no statistics are recorded.
        virtual void printStatistics(std::ostream &out) const = 0;

    protected:
        Solver() {};
    };
}
