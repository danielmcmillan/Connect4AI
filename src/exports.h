extern "C"
{
    /**
     * Configure the AI. Must be called before using the computeMove function.
     */
    void configure();

    /**
     * Use the AI to determine which column to play in.
     *
     * @param board C string representing the board.
     * @param yellow Whether the current player is yellow rather than red.
     * @return The index of column to play in.
     */
    int computeMove(const char *board, bool yellow);

    /**
     * Calculate which row a piece would land in for the given column.
     *
     * @param board C string representing the board.
     * @param column The column being played into.
     * @return Index of the row from the bottom. May be a row above the playing field.
     */
    int rowForMove(const char *board, int column);

    /**
     * Get a board containing only winning pieces for the given player.
     *
     * @param board C string representing the board.
     * @param yellow Whether to get the winning pieces for yellow rather than red.
     * @return C string representation of the board containing only the winning pieces,
     *  or empty string if the player has not won. Must be freed if not NULL.
     */
    const char *winningPieces(const char *board, bool yellow);
}
