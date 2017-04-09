#include "board.h"
#include "zobristnumbers.h"
#include <cassert>
#include <stdexcept>
#include <sstream>

namespace ConnectFour
{
	const int Board::shiftAmounts[] = {1, Board::width + 1, Board::width + 2, Board::width};

	void Board::swap()
	{
		currentPlayer ^= otherPlayer;
		otherPlayer ^= currentPlayer;
		currentPlayer ^= otherPlayer;
		currentHash ^= otherHash;
		otherHash ^= currentHash;
		currentHash ^= otherHash;
	}

	void Board::setSpace(int column, int row, bool occupied)
	{
		assert(column >= 0 && row >= 0 && column < Board::width && row < Board::height);
		
		int bit = (Board::height - row)*(Board::width + 1) - column - 1;
		currentPlayer[bit] = occupied;

		Hash currentRandom = zobristNumbers[Board::width*Board::height - 1 - row*Board::width + column];
		Hash otherRandom = zobristNumbers[2*Board::width*Board::height - 1 - row*Board::width + column];
		// XOR in/out updated currentPlayer slot 
		currentHash ^= currentRandom;
		otherHash ^= otherRandom;

		if (otherPlayer[bit])
		{
			otherPlayer[bit] = false;
			// XOR out otherPlayer slot
			currentHash ^= otherRandom;
			otherHash ^= currentRandom;
		}
	}

	bool Board::isWin() const
	{
		// Check for connections in each direction
		for (int shift = 0; shift < shiftDirections; ++shift)
		{
			Board::bitset b = currentPlayer & (currentPlayer >> shiftAmounts[shift]);
			b = b & (b >> shiftAmounts[shift]);
			b = b & (b >> shiftAmounts[shift]);
			if (b != 0) return true;
		}

		return false;
	}

	bool Board::canPlay(int column) const
	{
		assert(column >= 0 && column < Board::width);

		// Check that the corresponding bit in the top row is zero
		return ((currentPlayer ^ otherPlayer) & (Board::bitset(1) << (Board::width - column))) == 0;
	}

	void Board::play(int column)
	{
		assert(canPlay(column));

		Board::bitset board = currentPlayer ^ otherPlayer;
		// Initialise mask with bit corresponding to the column set in bottom row
		Board::bitset mask = Board::bitset(1) << ((Board::width + 1)*Board::height - column - 1);
		int row = 0;

		// Move bit up 1 row at a time until it is in a free spot
		while ((mask & board) != 0)
		{
			mask >>= (Board::width + 1);
			++row;
		}

		// Add the bit to the current player
		currentPlayer |= mask;

		// Update hash
		currentHash ^= zobristNumbers[Board::width*Board::height - 1 - row*Board::width + column];
		otherHash ^= zobristNumbers[2*Board::width*Board::height - 1 - row*Board::width + column];
	}

	Board::connectionsArray Board::countConnections() const
	{
		connectionsArray connections = {0};

		// Count the connections in each direction
		for (int shift = 0; shift < shiftDirections; ++shift)
		{
			Board::bitset b = currentPlayer;
			std::tr1::array<int, 4> counts = {0};

			// Count the number of pieces remaining after each shift
			// 1 is removed from each remaining group of pieces, so the difference in counts can be used to calculate how many groups are atleast a particular size
			for (std::tr1::array<int, 4>::iterator count = counts.begin(); count != counts.end(); ++count)
			{
				b = b & (b >> shiftAmounts[shift]);
				*count = b.count();
			}

			int atleast2 = counts[0] - counts[1];
			int atleast3 = counts[1] - counts[2];
			int atleast4 = counts[2] - counts[3];

			connections[0] += atleast2 - atleast3; // Exactly 2
			connections[1] += atleast3 - atleast4; // Exactly 3
			connections[2] += atleast4;
		}

		return connections;
	}

	void Board::resetHashes()
	{
		currentHash = 0;
		otherHash = 0;
		bitset c = currentPlayer;
		bitset o = otherPlayer;

		for (int bit = 0; bit < Board::width*Board::height; ++bit)
		{
			// Skip the row-separating bits
			if ((bit % Board::width) == 0)
			{
				c >>= 1;
				o >>= 1;
			}
			// If a piece exists, xor in the corresponding random number
			if ((c & bitset(1)) != 0)
			{
				currentHash ^= zobristNumbers[bit];
				otherHash ^= zobristNumbers[Board::width*Board::height + bit];
			}
			if ((o & bitset(1)) != 0)
			{
				otherHash ^= zobristNumbers[bit];
				currentHash ^= zobristNumbers[Board::width*Board::height + bit];
			}
			c >>= 1;
			o >>= 1;
		}
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

	void Board::setFromDescription(const std::string &description)
	{
		clear();
		
		// Set the bits for the board, starting at the most significant
		int bit = 0;
		for (std::string::const_iterator i = description.begin(); i != description.end(); ++i)
		{
			if (*i == currentPlayerChar)
			{
				currentPlayer |= 1;
			}
			else if (*i == otherPlayerChar)
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

		resetHashes();
	}

	std::ostream& operator<<(std::ostream& os, const Board& b)
	{  
		os << b.getDescription(); 
		return os;
	}

	std::istream& operator>>(std::istream& is, Board& b)
	{
		std::string description;
		is >> description;
		b.setFromDescription(description);
		return is;
	}
}
