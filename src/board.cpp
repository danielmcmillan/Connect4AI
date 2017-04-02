#include "board.h"
#include <cassert>
#include <sstream>

Board::Board(std::string decription, std::string player)
{
	currentPlayer = otherPlayer = 0;
	char playerc = player[0];
	std::istringstream iss(decription);
	std::string row;

	// Set the pieces for each row
	for (int i = 0; i < Board::height; ++i)
	{
		// Get the next comma-separated row
		assert(std::getline(iss, row, ','));

		for (int c = 0; c < row.length(); ++c)
		{
			if (row[c] == playerc)
			{
				currentPlayer[c*Board::height + i] = true;
			}
			else if (row[c] != '.')
			{
				otherPlayer[c*Board::height + i] = true;
			}
		}
	}
}