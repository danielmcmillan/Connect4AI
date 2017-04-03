// #define NDEBUG

#include <iostream>
#include <cassert>
#include <stdexcept>
#include <cstdlib>

#include "board.h"

using namespace ConnectFour;

int main(int argc, char **argv)
{
    if(argc <= 2)
    {
        std::cerr << "Expected more arguments" << std::endl;
        std::exit(-1);
    }

    // Find the character representing the current and other player
    std::string player = argv[2];
    char playerChar = 'r';
    char otherChar = 'y';
    if (player == "yellow")
    {
        playerChar = 'y';
        otherChar = 'r';
    }
    else if (player != "red")
    {
        std::cerr << "Invalid player" << std::endl;
        std::exit(-1);
    }

    // Create the game board
    try
    {
        Board board(argv[1], playerChar, otherChar);
        std::cout << board << std::endl;
    }
    catch (std::invalid_argument &e)
    {
        std::cerr << "Invalid argument: " << e.what() << std::endl;
        std::exit(-1);
    }
}
