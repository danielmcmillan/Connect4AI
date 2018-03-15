#include <iostream>
#include <cassert>
#include <stdexcept>
#include <sstream>

#include "board.h"
#include "tournamentsolver.h"

using namespace ConnectFour;

int main(int argc, char **argv)
{
    if (argc <= 2)
    {
        std::cerr << "Expected more arguments" << std::endl;
        return -1;
    }

    bool yellow = argv[2][0] == 'y';

    try
    {
        // Create the game board
        Board board;
        board.setFromDescription(argv[1]);
        if (yellow)
        {
            board.swap();
        }

        TournamentSolver solver(970, 8, 1, -1);
        int move = solver.solve(board);
        if (move == 7)
        {
            // Skip move
            return 0;
        }
        else if (move == -1)
        {
            // Failed to complete iteration,
            move = 3;
        }
        std::cout << move << std::endl;
    }
    catch (std::invalid_argument &e)
    {
        std::cerr << "Invalid argument: " << e.what() << std::endl;
        return -1;
    }
}
