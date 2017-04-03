#include "board.h"
#include <cassert>
#include <stdexcept>
#include <sstream>

namespace ConnectFour
{
	Board::Board(const std::string &description, char current, char other)
	{
		// Set the bits for the board, starting at the most significant
		int bit = 0;
		for (std::string::const_iterator i = description.begin(); i != description.end(); ++i)
		{
			if (*i == current)
			{
				currentPlayer |= 1;
			}
			else if (*i == other)
			{
				otherPlayer |= 1;
			}
			#ifndef NDEBUG
			else if (*i == rowSeperatorChar)
			{
				// Check that the row was correct length (Board::width + 1 including comma)
				if ((bit + 1) % (Board::width + 1) != 0)
				{
					throw std::invalid_argument("Description has row of wrong length");
				}
			}
			else if (*i != noPieceChar)
			{
				throw std::invalid_argument("Description has invalid characters");
			}
			#endif

			// Move to the next bit
			currentPlayer <<= 1;
			otherPlayer <<= 1;
			++bit;
		}

		#ifndef NDEBUG
		// Check description was correct length
		if (bit + 1 != (Board::width + 1)*Board::height)
		{
			throw std::invalid_argument("Description is wrong length");
		}
		#endif

		assert((currentPlayer & otherPlayer) == 0);
	}

	void Board::setSpace(int row, int column, bool forOtherPlayer, bool empty)
	{
		int bit = (Board::height - row)*(Board::width + 1) - column - 1;
		currentPlayer[bit] = !empty && !forOtherPlayer;
		otherPlayer[bit] = !empty && forOtherPlayer;
	}

	std::string Board::getDescription(int row) const
	{
		std::ostringstream oss;
		
		assert(row < Board::height);

		int bit = (Board::width + 1)*(row < 0 ? Board::height : (Board::height - row)) - 1;
		int endBit = row < 0 ? 0 : (bit - Board::width);
		for (; bit > endBit; --bit)
		{
			if (bit % (Board::width + 1) == 0)
			{
				// Row seperator bit
				oss << rowSeperatorChar;
			}
			else if (currentPlayer[bit])
			{
				oss << currentPlayerChar;
			}
			else if (otherPlayer[bit])
			{
				oss << otherPlayerChar;
			}
			else
			{
				oss << noPieceChar;
			}
		}

		return oss.str();
	}

	std::ostream& operator<<(std::ostream& os, const Board& b)  
	{  
		os << b.getDescription(); 
		return os;
	}

	bool Board::isWin(bool forOtherPlayer) const
	{
		Board::bitset board = forOtherPlayer ? otherPlayer : currentPlayer;

		// Horizontal connections
		Board::bitset b = board & (board >> 1);
		b = b & (b >> 1);
		b = b & (b >> 1);
		if (b.any()) return true;

		// Vertical connections
		b = board & board >> (Board::width + 1);
		b = b & b >> (Board::width + 1);
		b = b & b >> (Board::width + 1);
		if (b.any()) return true;

		// Diagonal / connections
		b = board & board >> (Board::width + 2);
		b = b & b >> (Board::width + 2);
		b = b & b >> (Board::width + 2);
		if (b.any()) return true;

		// Diagonal \ connections
		b = board & board >> (Board::width);
		b = b & b >> (Board::width);
		b = b & b >> (Board::width);
		if (b.any()) return true;

		return false;
	}
}
