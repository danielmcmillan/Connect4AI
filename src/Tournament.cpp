// #define NDEBUG

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
        
        TournamentSolver solver(950, 7, 1);
        std::cout << solver.solve(board) << std::endl;
    }
    catch (std::invalid_argument &e)
    {
        std::cerr << "Invalid argument: " << e.what() << std::endl;
        return -1;
    }
}
