#pragma once
#include <string>
#include <bitset>

class Board
{
public:
	static const int width = 6;
	static const int height = 7;

	/*
	 * Construct a Board based on the specified description.
	 * 
	*/
	Board(std::string decription, std::string player);

protected:
	/*
	 * Data representing the positions of each of the players pieces.
	 * A 1 represents piece existing, 0 represents no piece (for the player).
	 * Order of bits is column-major order with most significant bit representing the lower left piece.
	*/
	std::bitset<42> currentPlayer, otherPlayer;
};

