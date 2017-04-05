#include <iostream>
#include <sstream>
#include <cassert>
#include <stdexcept>
#include <tr1/array>
#include <fstream>

#include "board.h"

using namespace ConnectFour;
using std::string;

Board board;

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
        board.setFromDescription(description);
        std::cout << "Board updated" << std::endl;
    }
    catch (std::invalid_argument &e)
    {
        std::cerr << "Invalid description: " << e.what() << std::endl;
    }
}

void setSpace(int column, int row, const string &piece)
{
    if (piece == "yellow")
    {
        board.swap();
    }
    if (row >= 0 && column >= 0 && row < Board::height && column < Board::width)
    {
        board.setSpace(column, row, piece != "clear");
    }
    else if (row == -1 && column == -1 && piece == "clear")
    {
        // Clear when no arguments are given
        board.clear();
    }
    else
    {
        std::cerr << "Invalid arguments: <column> <row>" << std::endl;
    }
    if (piece == "yellow")
    {
        board.swap();
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
        std::cout << r << board.getDescription(r) << r << std::endl;
    }
    std::cout << oss.str();
}

void printCount()
{
    Board::connectionsArray connections = board.countConnections();
    std::cout << "Total pieces: " << board.count() << std::endl;
    std::cout << "2-in-a-row: " << connections[0] << std::endl;
    std::cout << "3-in-a-row: " << connections[1] << std::endl;
    std::cout << "4+-in-a-row: " << connections[2] << std::endl;
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
            std::cout << board << std::endl;
        }
        else if (command == "swap")
        {
            board.swap();
        }
        else if (command == "red" || command == "yellow" || command == "clear")
        {
            int column = -1, row = -1;
            iss >> column; iss >> row;
            setSpace(column, row, command);
        }
        else if (command == "iswin")
        {
            std::cout << (board.isWin() ? "won" : "not won") << std::endl;
        }
        else if (command == "count")
        {
            printCount();
        }
        else if (command != "")
        {
            std::cerr << "Invalid command" << std::endl;
        }
        printBoard();
        std::cout << "> ";
    }
}
