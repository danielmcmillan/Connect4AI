#pragma once
#include <string>
#include <bitset>
#include <iostream>

namespace ConnectFour
{
	class Board
	{
	public:
		static const int width = 7;
		static const int height = 6;

		/*
		 * Construct a Board based on the specified description.
		 * Current and other specify the character representing the players in the description.
		*/
		Board(const std::string &decription, char current, char other);

		// Return a string representation for the board
		std::string getDescription() const;
		friend std::ostream& operator<<(std::ostream& os, const Board& b);

	protected:
		/*
		 * Data representing the positions of each of the players pieces.
		 * A 1 represents piece existing, 0 represents no piece (for the player).
		 * Order of bits is row-major order with most significant bit representing the lower left piece.
		 * There is a zero bit after each row to avoid erroneous matches between adjacent rows.
		*/
		std::bitset<(width + 1)*height> currentPlayer, otherPlayer;

		// Representation for players in the description.
		static const char currentPlayerChar = 'r', otherPlayerChar = 'y', noPieceChar = '.';
		static const char rowSeperatorChar = ',';
	};
}
