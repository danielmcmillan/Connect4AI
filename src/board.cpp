#include "board.h"
#include <cassert>
#include <stdexcept>
#include <sstream>

// see if I can #include <hash_map> from SGI STL
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

	std::string Board::getDescription() const
	{
		std::ostringstream oss;
		
		int bit = (Board::width + 1)*Board::height - 1;
		for (; bit > 0; --bit)
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
}
