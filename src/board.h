#pragma once
#include <string>
#include <bitset>
#include <iostream>
#include <tr1/array>

namespace ConnectFour
{
	/// @class Board
	/// @brief Class for representing and manipulating the state of a Connect Four board.
	class Board
	{
	public:
		/// @brief Board width
		static const int width = 7;
		/// @brief Board height
		static const int height = 6;

		/// @brief Swap turns with the other player (i.e. swap all pieces).
		void swap();

		/// @brief Sets whether the specified space is occupied by the current player or empty.
		void setSpace(int column, int row, bool occupied);

		/// @brief Clear all pieces of both players from the board.
		void clear() { currentPlayer = 0; otherPlayer = 0; currentHash = 0; otherHash = 0; }

		/// @brief Check whether the current player has connected 4.
		bool isWin() const;

		/// @brief Get a count of the current player's pieces on the board.
		int count() const { return currentPlayer.count(); }

		/// @brief Get a count of both player's pieces on the board.
		int totalCount() const { return currentPlayer.count() + otherPlayer.count(); }

		/// @brief Check whether playing in the specified column is a legal move.
		/// @param column The column to check. Must be a valid column.
		bool canPlay(int column) const;

		/// @brief Play in the specified column.
		/// @param column The column to play in. It must exist and canPlay(column) must be true.
		void play(int column);

		/// @brief Type for representing a Board hash.
		typedef long unsigned int Hash;
		
		/// @brief Gets the hash value for the current board state.
		Hash getHash() const { return currentHash; }

		/// @brief Array for storing the number of connections of size 2 and 3.
		typedef std::tr1::array<int, 3> connectionsArray;

		/// @brief Count the numbers of any connection of size 2, 3 and 4+.
		/// @return A connectionsArray: { exactly 2, exactly 3, atleast 4 }.
		connectionsArray countConnections() const;

		/// @brief Get a string representation for the board with red as the current player.
		/// @param row Optional parameter specifying a single row to limit the description to.
		std::string getDescription(int row = -1) const;

		/// @brief Set the board state based on the given description
		/// @param description String description of a board. invalid_argument is thrown if it is invalid.
		void setFromDescription(const std::string &description);

		/// @brief Output board descriptions from a Board into an output stream.
		friend std::ostream& operator<<(std::ostream& os, const Board& b);

		/// @brief Read board descriptions from an input stream into a Board.
		friend std::istream& operator>>(std::istream& is, Board& b);

	private:
		// Data representing the positions of each of the players pieces.
		// A 1 represents piece existing, 0 represents no piece (for the player).
		// Order of bits is row-major order with most significant bit representing the lower left piece.
		// There is a zero bit after each row to avoid erroneous matches between adjacent rows.
		typedef std::bitset<(width + 1)*height> bitset;
		bitset currentPlayer, otherPlayer;

		// Hash for each player, which can be used to compute a hash for new moves
		Hash currentHash;
		Hash otherHash;

		// The amount to shift for each direction when finding connections (horizontal, vertical, forward/backward diagonal)
		static const int shiftDirections = 4;
		static const int shiftAmounts[shiftDirections];

		// Representation for players in the description.
		static const char currentPlayerChar = 'r', otherPlayerChar = 'y', noPieceChar = '.';
		static const char rowSeperatorChar = ',';

		// Recalculate hash values
		void resetHashes();
	};
}
