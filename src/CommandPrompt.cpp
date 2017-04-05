#define NDEBUG

#include <iostream>
#include <sstream>
#include <cassert>
#include <cstdlib>
#include <ctime>
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
        std::cout << "Invalid description: " << e.what() << std::endl;
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
        std::cout << "Invalid arguments: <column> <row>" << std::endl;
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

void save(const string &name)
{
    if (name.empty())
    {
        std::cout << "No name given" << std::endl;
        return;
    }
    std::ofstream file((name + ".stash").c_str());
    if (file.good())
    {
        file << board;
    }
    else
    {
        std::cout << "Failed to save board" << std::endl;
    }
}

void load(const string &name)
{
    if (name.empty())
    {
        std::cout << "No name given" << std::endl;
        return;
    }
    std::ifstream file((name + ".stash").c_str());
    if (file.good())
    {
        string description;
        file >> description;
        board.setFromDescription(description);
    }
    else
    {
        std::cout << "Failed to load board" << std::endl;
    }
}

void play(int column, bool checkOnly = false)
{
    if (column >= 0 && column < Board::width && board.canPlay(column))
    {
        std::cout << "Valid move" << std::endl;
        if (!checkOnly)
        {
            board.play(column);
        }
    }
    else
    {
        std::cout << "Invalid move" << std::endl;
    }
}

void random(int pieces)
{
    if (pieces < 0)
    {
        pieces = std::rand() % (Board::width*Board::height);
    }
    while (pieces > 0)
    {
        int column = std::rand() % Board::width;
        if (board.canPlay(column))
        {
            board.play(column);
        }
        --pieces;

        board.swap();
    }
}

int main(int argc, char **argv)
{
    std::srand(std::time(NULL));

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
                std::cout << "No description given" << std::endl;
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
        else if (command == "save")
        {
            string name;
            iss >> name;
            save(name);
        }
        else if (command == "load")
        {
            string name;
            iss >> name;
            load(name);
        }
        else if (command == "canplay" || command == "play")
        {
            int column = -1;
            iss >> column;
            play(column, command == "canplay");
        }
        else if (command == "random")
        {
            int pieces = -1;
            iss >> pieces;
            random(pieces);
        }
        else if (command != "")
        {
            std::cout << "Invalid command" << std::endl;
        }
        printBoard();
        std::cout << "> ";
    }
}
