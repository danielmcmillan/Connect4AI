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
        static const int transpositionTableSize = 262144; // 2^18 - 6MB
        BoardEvaluation *table;

        // Statistics for last solve
        int nodesExamined;
        int tableHits; // Times required position was in table
        int tableReplacements; // Collisions where old value was replaced
        int tableIgnores; // Collisions where old value was left

        /// @brief Get the best move and minimax value for the given board
        /// @param board A board position.
        /// @param[out] outValue Pointer to integer to write minimax value to.
        /// @param height The maximum height for the search tree. Must not extend beyond a filled board.
        /// @param alpha Lower bound for value to search for.
        /// @param beta Upper board for value to search for.
        /// @return The move to take from the given board, or -1 if no move was determined.
        int bestMove(const Board &board, int *outValue, int depth, int alpha, int beta);

        /// @brief Play each playable column from the given board
        /// @param[out] boards Array to store the resulting boards for move in each column
        /// @param[out] columns Array to store column numbers of moves to explore
        /// @return Column for a move resulting in a win, or -1 if there is none.
        int playAllMoves(const Board &board, std::tr1::array<Board, Board::width> &boards, std::tr1::array<int, Board::width> &columns);

        /// @brief Sort the columns to play so that more promising moves appear first.
        /// @param boards Array of boards for each possible move.
        /// @param columns Array of columns to play in that should be sorted.
        void orderMoves(const std::tr1::array<Board, Board::width> &boards, std::tr1::array<int, Board::width> &columns);

        /// @breif Store a board evaluation in the transposition table.
        ///        If there is a collision, keep the evaluation with the greatest height.
        void storeInTable(const Board &board, int move, int value, int height, EvaluationType type);
        
        /// @breif Get a pointer to the transposition table entry for the given board.
        BoardEvaluation *tableEntryFor(const Board &board)
            { return &table[board.getHash() % transpositionTableSize]; }

        /// @brief Compute the score for the current player.
        static int score(const Board &board);
    };
}