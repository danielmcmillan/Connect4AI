#include <iostream>
#include <cassert>
#include <stdexcept>
#include <sstream>

#include "board.h"
#include "automarkedsolver.h"

using namespace ConnectFour;

int main(int argc, char **argv)
{
    if (argc <= 4)
    {
        std::cerr << "Expected more arguments" << std::endl;
        return -1;
    }

    bool yellow = argv[2][0] == 'y';
    bool enablePruning = argv[3][0] == 'A';
    int depth = 0;
    std::istringstream(argv[4]) >> depth;

    try
    {
        // Create the game board
        Board board;
        board.setFromDescription(argv[1]);
        if (yellow)
        {
            board.swap();
        }

        AutomarkedSolver solver(depth, enablePruning);
        std::cout << solver.solve(board) << std::endl;
        std::cout << solver.numberOfNodesExamined() << std::endl;
    }
    catch (std::invalid_argument &e)
    {
        std::cerr << "Invalid argument: " << e.what() << std::endl;
        return -1;
    }
}
