#pragma once
#include <string>
#include <bitset>
#include <iostream>
#include <tr1/array>

namespace ConnectFour
{
	class Board
	{
	public:
		static const int width = 7;
		static const int height = 6;

		// Swap turns with the other player (i.e. swap all pieces)
		void swap()
		{
			currentPlayer ^= otherPlayer;
			otherPlayer ^= currentPlayer;
			currentPlayer ^= otherPlayer;
		}

		// Sets whether the specified space is occupied by the current player or empty
		void setSpace(int column, int row, bool occupied);

		// Clear all pieces of both players from the board
		void clear() { currentPlayer = 0; otherPlayer = 0; }

		// Check whether the current player has connected 4
		bool isWin() const;

		// Get the total number of pieces for the current player
		int count() const { return currentPlayer.count(); }

		// Count the number connections of different sizes
		// Returns an array: { exactly 2, exactly 3, atleast 4 }
		typedef std::tr1::array<int, 3> connectionsArray;
		connectionsArray countConnections() const;

		// Return a string representation for the board with red as the current player
		std::string getDescription(int row = -1) const;

		// Set the board state based on the given description
		void setFromDescription(const std::string &description);

		friend std::ostream& operator<<(std::ostream& os, const Board& b);

	protected:
		/*
		 * Data representing the positions of each of the players pieces.
		 * A 1 represents piece existing, 0 represents no piece (for the player).
		 * Order of bits is row-major order with most significant bit representing the lower left piece.
		 * There is a zero bit after each row to avoid erroneous matches between adjacent rows.
		*/
		typedef std::bitset<(width + 1)*height> bitset;
		bitset currentPlayer, otherPlayer;

		// The amount to shift for each direction when finding connections (horizontal, vertical, forward/backward diagonal)
		static const int shiftAmounts[4];

		// Representation for players in the description.
		static const char currentPlayerChar = 'r', otherPlayerChar = 'y', noPieceChar = '.';
		static const char rowSeperatorChar = ',';
	};
}
