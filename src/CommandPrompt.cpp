#include <iostream>
#include <sstream>
#include <cassert>
#include <stdexcept>

#include "board.h"

using namespace ConnectFour;
using std::string;

Board b;

void makeLowerCase(string &str)
{
    for (string::iterator i = str.begin(); i != str.end(); ++i)
    {
        *i = std::tolower(*i);
    }
}

void setBoard(string &description)
{
    try
    {
        b = Board(description);
        std::cout << "Board updated" << std::endl;
    }
    catch (std::invalid_argument &e)
    {
        std::cerr << "Invalid description: " << e.what() << std::endl;
    }
}

void printBoard()
{
    // Make top/bottom string
    std::ostringstream oss;
    oss << '+';
    for (int i = 0; i < Board::width; ++i)
    {
        oss << i;
    }
    oss << '+' << std::endl;

    std::cout << std::endl << oss.str();

    for (int r = Board::height - 1; r >= 0; --r)
    {
        std::cout << r << b.getDescription(r) << r << std::endl;
    }
    std::cout << oss.str();
}

int main(int argc, char **argv)
{
    std::cout << "ConnectFour command prompt" << std::endl;

    string line;
    printBoard();
    std::cout << "> ";
    while (std::getline(std::cin, line))
    {
        makeLowerCase(line);
        std::istringstream iss(line);
        string command;
        iss >> command;

        if (command == "exit" || command == "quit")
        {
            break;
        }
        else if (command == "set")
        {
            if (iss >> command)
            {
                setBoard(command);
            }
            else
            {
                std::cerr << "No description given" << std::endl;
            }
        }
        else if (command == "print")
        {
            std::cout << b << std::endl;
        }
        else if (command == "red" || command == "yellow" || command == "clear")
        {
            int row, column;
            iss >> row;
            iss >> column;
            if (iss && row >= 0 && column >= 0 && row < Board::height && column < Board::width)
            {
                bool clear = command == "clear";
                bool otherPlayer = command == "yellow";
                b.setSpace(row, column, otherPlayer, clear);
            }
            else
            {
                std::cerr << "Invalid arguments: <row:0-5> <column:0-6>" << std::endl;
            }
        }
        else if (command == "check")
        {
            std::cout << (b.isWin() ? "won" : "not won") << std::endl;
        }
        else if (command != "")
        {
            std::cerr << "Invalid command" << std::endl;
        }
        printBoard();
        std::cout << "> ";
    }
}
