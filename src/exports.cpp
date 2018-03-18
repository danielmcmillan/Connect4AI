#include "exports.h"
#include <cstdlib>
#include <cstring>
#include <string>
#include "board.h"
#include "mainsolver.h"

static ConnectFour::MainSolver solver(20000, 13, 1, 13);

/// @brief Set board based on description in a cstring
static void setBoardFromCString(ConnectFour::Board &board, const char *cstring, bool yellow = false)
{
    std::string boardString(cstring);
    board.setFromDescription(boardString);
    if (yellow)
    {
        board.swap();
    }
}

void configure()
{

}

int computeMove(const char *boardString, bool yellow)
{
    ConnectFour::Board board;
    setBoardFromCString(board, boardString, yellow);
    return solver.solve(board);
}

int rowForMove(const char *boardString, int column)
{
    ConnectFour::Board board;
    setBoardFromCString(board, boardString);
    return board.getFreeRow(column);
}

const char *winningPieces(const char *boardString, bool yellow)
{
    ConnectFour::Board board;
    setBoardFromCString(board, boardString);
    std::string winners = board.getWinningPiecesDescription(yellow);
    if (winners.empty())
    {
        return NULL;
    }
    else
    {
        return strdup(winners.c_str());
    }
}
