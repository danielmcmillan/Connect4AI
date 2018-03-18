#pragma once

#include <cstdint>
#include <string>
#include <bitset>
#include <iostream>
#include <array>

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

		/// @brief Get the row of the bottom-most free spot in a column.
		/// @param column The column to check for a free spot within.
		/// @return Index of the row from the bottom. May be outside range of rows if there is no free spot.
		int getFreeRow(int column);

		/// @brief Type for representing a Board hash.
		typedef unsigned int Hash;

		/// @brief Gets the hash value for the current board state.
		Hash getHash() const { return currentHash; }

		/// @brief Array for storing the number of connections of size 2 and 3.
		typedef std::array<int, 3> connectionsArray;

		/// @brief Count the numbers of any connection of size 2, 3 and 4+.
		/// @return A connectionsArray: { exactly 2, exactly 3, atleast 4 }.
		connectionsArray countConnections() const;

		/// @brief Stores counts of different types of threats for both players.
		struct ThreatInfo
		{
			int allThreats[2]; // Threats that may be possible to exploit
			int groundedThreats[2]; // Threats with a piece by any player below them (they can be played immediately)
			int doubleThreats[2]; // Threats with another threat below them, they can always be exploited
		};

		/// @brief Get information about the
		/// @return The difference in number of threats.
		ThreatInfo getThreatInfo() const;

		/// @brief Get a string representation for the board with red as the current player.
		/// @param row Optional parameter specifying a single row to limit the description to.
		std::string getDescription(int row = -1, bool showThreats = false) const;

		/// @brief Get a string representation for the current player's board pieces that won the game.
		/// @return String description, or empty string if the current player hasn't won the game.
		std::string getWinningPiecesDescription(bool forYellow = false) const;

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

		/// @breif Get a bitmask for a board with piece in every slot
		static const bitset &getBoardMask();

		/// @breif Get a bitmask for the bottom row
		static const bitset &getBottomMask();

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

		/// @brief Get a board of possible threats: slots that would allow connecting-four.
		/// @param bad Whether to find threats against current player. Otherwise finds threats against other player.
		bitset getThreats(bool bad) const;

		/// @brief Unset bits in threats that correspond to threats that can't be exploited
		/// @param[in,out] threats The threats to filter
		/// @param otherThreats The other player's threats which may prevent exploiting our own threats.
		static void filterThreats(Board::bitset &threats, const Board::bitset &otherThreats);
	};
}
