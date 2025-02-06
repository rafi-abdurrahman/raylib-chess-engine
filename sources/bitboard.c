#include <bitboard.h>

BitBoard InitBoard(){
    BitBoard board;
    
    // STARTING POSITIONS
    // White
    board.wPawn     = 65280ULL;
    board.wRook     = 129ULL;
    board.wKnight   = 66ULL;
    board.wBishop   = 36ULL;
    board.wQueen    = 8ULL;
    board.wKing     = 16ULL;
    board.wPosition = 65535ULL;
    // Black
    board.bPawn     = 71776119061217280ULL;
    board.bRook     = 9295429630892703744ULL;
    board.bKnight   = 4755801206503243776ULL;
    board.bBishop   = 2594073385365405696ULL;
    board.bQueen    = 576460752303423488ULL;
    board.bKing     = 1152921504606846976ULL;
    board.bPosition = 18446462598732840960ULL;
    // Castle
    board.wCastled  = 0;
    board.bCastled  = 0;
    // En Passant
    board.enPassant = 8; // 8 if enpassant is illegible, collumn number if enpassant is legible (0-7)
    // Game State
    board.playerTurn = 1; // 1 if white, 0 if black
    board.wInCheck = 0;
    board.bInCheck = 0;

    return board;
}

#pragma region Get Board Position

void GetCurrentPos(const BitBoard* board, char* posBoard){
    for (int i = 0; i < 64; i++){
        // Check White BitBoards
        if(IS_BIT(board->wPosition, i)){
            if(IS_BIT(board->wPawn, i))
                posBoard[i] = 'P';
            else if (IS_BIT(board->wRook, i))
                posBoard[i] = 'R';
            else if (IS_BIT(board->wBishop, i))
                posBoard[i] = 'B';
            else if (IS_BIT(board->wKnight, i))
                posBoard[i] = 'N';
            else if (IS_BIT(board->wQueen, i))
                posBoard[i] = 'Q';
            else if (IS_BIT(board->wKing, i))
                posBoard[i] = 'K';
        }
        // Check Black BitBoards
        else if (IS_BIT(board->bPosition, i))
        {
            if (IS_BIT(board->bPawn, i))
                posBoard[i] = 'p';
            else if (IS_BIT(board->bRook, i))
                posBoard[i] = 'r';
            else if (IS_BIT(board->bBishop, i))
                posBoard[i] = 'b';
            else if (IS_BIT(board->bKnight, i))
                posBoard[i] = 'n';
            else if (IS_BIT(board->bQueen, i))
                posBoard[i] = 'q';
            else if (IS_BIT(board->bKing, i))
                posBoard[i] = 'k';
        }
        else
            posBoard[i] = '.';
    }
    return;
}

char GetCurrentPiece(const BitBoard *board, int8_t square){
    if (IS_BIT(board->wPosition, square))
    {
        if (IS_BIT(board->wPawn, square))
            return('P');
        else if (IS_BIT(board->wRook, square))
            return('R');
        else if (IS_BIT(board->wBishop, square))
            return('B');
        else if (IS_BIT(board->wKnight, square))
            return('N');
        else if (IS_BIT(board->wQueen, square))
            return('Q');
        else if (IS_BIT(board->wKing, square))
            return('K');
    }
    // Check Black BitBoards
    else if (IS_BIT(board->bPosition, square))
    {
        if (IS_BIT(board->bPawn, square))
            return('p');
        else if (IS_BIT(board->bRook, square))
            return('r');
        else if (IS_BIT(board->bBishop, square))
            return('b');
        else if (IS_BIT(board->bKnight, square))
            return('n');
        else if (IS_BIT(board->bQueen, square))
            return('q');
        else if (IS_BIT(board->bKing, square))
            return('k');
    }
    else
        return('.');
}

uint64_t *GetCurrentPieceEx(BitBoard *board, int8_t square)
{
    if (IS_BIT(board->wPosition, square))
    {
        if (IS_BIT(board->wPawn, square))
            return (&board->wPawn);
        else if (IS_BIT(board->wRook, square))
            return (&board->wRook);
        else if (IS_BIT(board->wBishop, square))
            return (&board->wBishop);
        else if (IS_BIT(board->wKnight, square))
            return (&board->wKnight);
        else if (IS_BIT(board->wQueen, square))
            return (&board->wQueen);
        else if (IS_BIT(board->wKing, square))
            return (&board->wKing);
    }
    // Check Black BitBoards
    else if (IS_BIT(board->bPosition, square))
    {
        if (IS_BIT(board->bPawn, square))
            return (&board->bPawn);
        else if (IS_BIT(board->bRook, square))
            return (&board->bRook);
        else if (IS_BIT(board->bBishop, square))
            return (&board->bBishop);
        else if (IS_BIT(board->bKnight, square))
            return (&board->bKnight);
        else if (IS_BIT(board->bQueen, square))
            return (&board->bQueen);
        else if (IS_BIT(board->bKing, square))
            return (&board->bKing);
    }
    else
        return (NULL);
}

#pragma endregion

#pragma region Piece Moveset

void PieceMove(BitBoard *board, int8_t start, int8_t target){
    char piece = GetCurrentPiece(board, start);
    printf("Moving piece: %c\n", piece);
    uint64_t *Piece = GetCurrentPieceEx(board, start);
    for (int rank = 7; rank >= 0; rank--)
    { 
        for (int file = 0; file < 8; file++)
        { 
            int square = rank * 8 + file;
            uint64_t mask = 1ULL << square;
            printf("%c ", (*Piece & mask) ? '1' : '.');
        }
        printf("\n");
    }
    CLEAR_BIT(*Piece, start);
    SET_BIT(*Piece, target);
    if(IS_BIT(board->wPosition, start)){
        CLEAR_BIT(board->wPosition, start);
        SET_BIT(board->wPosition, target);
    }
    else{
        CLEAR_BIT(board->bPosition, start);
        SET_BIT(board->bPosition, target);
    }
    piece = GetCurrentPiece(board, target);
    printf("Moved piece: %c\n", piece);
    for (int rank = 7; rank >= 0; rank--)
    {
        for (int file = 0; file < 8; file++)
        {
            int square = rank * 8 + file;
            uint64_t mask = 1ULL << square;
            printf("%c ", (*Piece & mask) ? '1' : '.');
        }
        printf("\n");
    }
    printf("Resulting Chess Board:\nWhite\n");
    for (int rank = 7; rank >= 0; rank--)
    {
        for (int file = 0; file < 8; file++)
        {
            int square = rank * 8 + file;
            uint64_t mask = 1ULL << square;
            printf("%c ", (board->wPosition & mask) ? '1' : '.');
        }
        printf("\n");
    }
    printf("Black\n");
    for (int rank = 7; rank >= 0; rank--)
    {
        for (int file = 0; file < 8; file++)
        {
            int square = rank * 8 + file;
            uint64_t mask = 1ULL << square;
            printf("%c ", (board->bPosition & mask) ? '1' : '.');
        }
        printf("\n");
    }
}

void PieceCapture(BitBoard *board, int8_t start, int8_t target)
{
    uint64_t *Capturer = GetCurrentPieceEx(board, start);
    uint64_t *Captured = GetCurrentPieceEx(board, target);
    char capturer = GetCurrentPiece(board, start);
    char captured = GetCurrentPiece(board, target);
    printf("Capturing... \n");
    printf("Capturer: %c\n", capturer);
    for (int rank = 7; rank >= 0; rank--)
    {
        for (int file = 0; file < 8; file++)
        {
            int square = rank * 8 + file;
            uint64_t mask = 1ULL << square;
            printf("%c ", (*Capturer & mask) ? '1' : '.');
        }
        printf("\n");
    }
    printf("Captured: %c\n", captured);
    for (int rank = 7; rank >= 0; rank--)
    {
        for (int file = 0; file < 8; file++)
        {
            int square = rank * 8 + file;
            uint64_t mask = 1ULL << square;
            printf("%c ", (*Captured & mask) ? '1' : '.');
        }
        printf("\n");
    }
    CLEAR_BIT(*Capturer, start);
    SET_BIT(*Capturer, target);
    CLEAR_BIT(*Captured, target);
    if (IS_BIT(board->wPosition, start))
    {
        printf("White is the capturer!\n");
        CLEAR_BIT(board->wPosition, start);
        SET_BIT(board->wPosition, target);
        CLEAR_BIT(board->bPosition, target);
    }
    else
    {
        printf("Black is the capturer!\n");
        CLEAR_BIT(board->bPosition, start);
        SET_BIT(board->bPosition, target);
        CLEAR_BIT(board->wPosition, target);
    }
    capturer = GetCurrentPiece(board, start);
    captured = GetCurrentPiece(board, target);
    printf("Captured! \n");
    printf("Prev Cell: %c\n", capturer);
    for (int rank = 7; rank >= 0; rank--)
    {
        for (int file = 0; file < 8; file++)
        {
            int square = rank * 8 + file;
            uint64_t mask = 1ULL << square;
            printf("%c ", (*Capturer & mask) ? '1' : '.');
        }
        printf("\n");
    }
    printf("Captured Cell: %c\n", captured);
    for (int rank = 7; rank >= 0; rank--)
    {
        for (int file = 0; file < 8; file++)
        {
            int square = rank * 8 + file;
            uint64_t mask = 1ULL << square;
            printf("%c ", (*Captured & mask) ? '1' : '.');
        }
        printf("\n");
    }

    printf("Resulting Chess Board:\nWhite\n");
    for (int rank = 7; rank >= 0; rank--)
    {
        for (int file = 0; file < 8; file++)
        {
            int square = rank * 8 + file;
            uint64_t mask = 1ULL << square;
            printf("%c ", (board->wPosition & mask) ? '1' : '.');
        }
        printf("\n");
    }
    printf("Black\n");
    for (int rank = 7; rank >= 0; rank--)
    {
        for (int file = 0; file < 8; file++)
        {
            int square = rank * 8 + file;
            uint64_t mask = 1ULL << square;
            printf("%c ", (board->bPosition & mask) ? '1' : '.');
        }
        printf("\n");
    }
}

void GetPossibleMoves(BitBoard *board, int8_t square, uint64_t *possiblePos)
{
    possiblePos[0] = 0ULL;
    if (!IS_BIT(board->wPosition, square) && !IS_BIT(board->bPosition, square))
        return;

    // ------------------------- WPAWN --------------------------
    if (IS_BIT(board->wPawn, square))
    {
        if(square + 8 < 64 && !IS_BIT(board->wPosition, square + 8) && !IS_BIT(board->bPosition, square + 8))
            SET_BIT(possiblePos[0], square + 8);

        // Double Square Edge Case
        if (square < 16 && !IS_BIT(board->wPosition, square + 16) && !IS_BIT(board->bPosition, square + 16))
            SET_BIT(possiblePos[0], square + 16);

        // Find Possible Captures
        // Right
        if (square + 9 < 64 && IS_BIT(board->bPosition, square + 9))
            SET_BIT(possiblePos[1], square + 9);
        // Left
        if (square + 7 < 64 && IS_BIT(board->bPosition, square + 7))
            SET_BIT(possiblePos[1], square + 7);
        // En Passant
        // (square%8) obtains col number and (square/8) obtains row number
        if ((board->enPassant == (square % 8) - 1 || board->enPassant == (square % 8) + 1) && (square / 8) == 4)
            SET_BIT(possiblePos[1], board->enPassant + 8);

        return;
    }
    // ------------------------- WPAWN --------------------------

    // ------------------------- BPAWN --------------------------
    if (IS_BIT(board->bPawn, square))
    {
        if (square - 8 >= 0 && !IS_BIT(board->bPosition, square - 8) && !IS_BIT(board->wPosition, square - 8))
            SET_BIT(possiblePos[0], square - 8);

        // Double Square Edge Case
        if (square > 47 && !IS_BIT(board->bPosition, square - 16) && !IS_BIT(board->wPosition, square - 16))
            SET_BIT(possiblePos[0], square - 16);

        // Search possible Captures
        // Right
        if (square - 7 < 64 && IS_BIT(board->wPosition, square - 7))
            SET_BIT(possiblePos[1], square - 7);
        // Left
        if (square - 9 < 64 && IS_BIT(board->wPosition, square - 9))
            SET_BIT(possiblePos[1], square - 9);
        // En Passant
        // (square%8) obtains col number and (square/8) obtains row number
        if ((board->enPassant == (square % 8) - 1 || board->enPassant == (square % 8) + 1) && (square / 8) == 3)
            SET_BIT(possiblePos[1], board->enPassant - 8);

        return;
    }
    // ------------------------- BPAWN --------------------------

    // -------------------------- ROOK --------------------------
    // And Queen (Partially)
    if (IS_BIT(board->wRook, square) || IS_BIT(board->wQueen, square) || IS_BIT(board->bRook, square) || IS_BIT(board->bQueen, square))
    {
        // UP
        for (int8_t i = square + 8; i < 64; i += 8){
            if (!IS_BIT(board->wPosition, i) && !IS_BIT(board->bPosition, i))
                SET_BIT(possiblePos[0], i);
            
            // Search possible captures
            else if (((IS_BIT(board->wRook, square) || IS_BIT(board->wQueen, square)) && IS_BIT(board->bPosition, i)) ||
                     ((IS_BIT(board->bRook, square) || IS_BIT(board->bQueen, square)) && IS_BIT(board->wPosition, i)))
            {
                SET_BIT(possiblePos[1], i);
                break;
            }

            else
                break;
        }
        // DOWN
        for (int8_t i = square - 8; i >= 0; i -= 8){
            if (!IS_BIT(board->wPosition, i) && !IS_BIT(board->bPosition, i))
                SET_BIT(possiblePos[0], i);

            // Search possible captures
            else if (((IS_BIT(board->wRook, square) || IS_BIT(board->wQueen, square)) && IS_BIT(board->bPosition, i)) ||
                     ((IS_BIT(board->bRook, square) || IS_BIT(board->bQueen, square)) && IS_BIT(board->wPosition, i)))
            {
                SET_BIT(possiblePos[1], i);
                break;
            }

            else
                break;
        }
        // RIGHT
        // (square + (7 - (square % 8))) obtains the most right position of square's rank
        for (int8_t i = square + 1; i <= (square + (7 - (square % 8))); i++)
        {
            if (!IS_BIT(board->wPosition, i) && !IS_BIT(board->bPosition, i))
                SET_BIT(possiblePos[0], i);

            // Search possible captures
            else if (((IS_BIT(board->wRook, square) || IS_BIT(board->wQueen, square)) && IS_BIT(board->bPosition, i)) ||
                     ((IS_BIT(board->bRook, square) || IS_BIT(board->bQueen, square)) && IS_BIT(board->wPosition, i)))
            {
                SET_BIT(possiblePos[1], i);
                break;
            }

            else
                break;
        }
        // LEFT
        // (square - (square%8)) obtains the most left position of square's rank
        for (int8_t i = square - 1; i >= (square - (square % 8)); i--)
        {
            if (!IS_BIT(board->wPosition, i) && !IS_BIT(board->bPosition, i))
                SET_BIT(possiblePos[0], i);

            // Search possible captures
            else if (((IS_BIT(board->wRook, square) || IS_BIT(board->wQueen, square)) && IS_BIT(board->bPosition, i)) ||
                     ((IS_BIT(board->bRook, square) || IS_BIT(board->bQueen, square)) && IS_BIT(board->wPosition, i)))
            {
                SET_BIT(possiblePos[1], i);
                break;
            }

            else
                break;
        }

        if (IS_BIT(board->wRook, square) || IS_BIT(board->bRook, square))
            return;
    }
    // -------------------------- ROOK --------------------------

    // ------------------------- KNIGHT -------------------------
    if (IS_BIT(board->wKnight, square) || IS_BIT(board->bKnight, square))
    {
        // UP RIGHT
        if ((square + 17 < 64) && (square + 17) / 8 == (square / 8) + 2 && !IS_BIT(board->bPosition, square + 17) && !IS_BIT(board->wPosition, square + 17))
        {
            SET_BIT(possiblePos[0], square + 17);
        }
        // UP LEFT
        if ((square + 15 < 64) && (square + 15) / 8 == (square / 8) + 2 && !IS_BIT(board->bPosition, square + 15) && !IS_BIT(board->wPosition, square + 15))
        {
            SET_BIT(possiblePos[0], square + 15);
        }
        // DOWN RIGHT
        if ((square >= 15) && (square - 15) / 8 == (square / 8) - 2 && !IS_BIT(board->bPosition, square - 15) && !IS_BIT(board->wPosition, square - 15))
        {
            SET_BIT(possiblePos[0], square - 15);
        }
        // DOWN LEFT
        if ((square >= 17) && (square - 17) / 8 == (square / 8) - 2 && !IS_BIT(board->bPosition, square - 17) && !IS_BIT(board->wPosition, square - 17))
        {
            SET_BIT(possiblePos[0], square - 17);
        }
        // RIGHT UP
        if ((square + 10 < 64) && (square + 10) / 8 == (square / 8) + 1 && !IS_BIT(board->bPosition, square + 10) && !IS_BIT(board->wPosition, square + 10))
        {
            SET_BIT(possiblePos[0], square + 10);
        }
        // RIGHT DOWN
        if ((square >= 6) && (square - 6) / 8 == (square / 8) - 1 && !IS_BIT(board->bPosition, square - 6) && !IS_BIT(board->wPosition, square - 6))
        {
            SET_BIT(possiblePos[0], square - 6);
        }
        // LEFT UP
        if ((square + 6 < 64) && (square + 10) / 8 == (square / 8) + 1 && !IS_BIT(board->bPosition, square + 6) && !IS_BIT(board->wPosition, square + 6))
        {
            SET_BIT(possiblePos[0], square + 6);
        }
        // LEFT DOWN
        if ((square >= 10) && (square - 10) / 8 == (square / 8) - 1 && !IS_BIT(board->bPosition, square - 10) && !IS_BIT(board->wPosition, square - 10))
        {
            SET_BIT(possiblePos[0], square - 10);
        }

        // Search possible Captures
        if (IS_BIT(board->wKnight, square))
        {
            // UP RIGHT
            if ((square + 17 < 64) && (square + 17) / 8 == (square / 8) + 2 && IS_BIT(board->bPosition, square + 17))
            {
                SET_BIT(possiblePos[1], square + 17);
            }
            // UP LEFT
            if ((square + 15 < 64) && (square + 15) / 8 == (square / 8) + 2 && IS_BIT(board->bPosition, square + 15))
            {
                SET_BIT(possiblePos[1], square + 15);
            }
            // DOWN RIGHT
            if ((square >= 15) && (square - 15) / 8 == (square / 8) - 2 && IS_BIT(board->bPosition, square - 15))
            {
                SET_BIT(possiblePos[1], square - 15);
            }
            // DOWN LEFT
            if ((square >= 17) && (square - 17) / 8 == (square / 8) - 2 && IS_BIT(board->bPosition, square - 17))
            {
                SET_BIT(possiblePos[1], square - 17);
            }
            // RIGHT UP
            if ((square + 10 < 64) && (square + 10) / 8 == (square / 8) + 1 && IS_BIT(board->bPosition, square + 10))
            {
                SET_BIT(possiblePos[1], square + 10);
            }
            // RIGHT DOWN
            if ((square >= 6) && (square - 6) / 8 == (square / 8) - 1 && IS_BIT(board->bPosition, square - 6))
            {
                SET_BIT(possiblePos[1], square - 6);
            }
            // LEFT UP
            if ((square + 6 < 64) && (square + 10) / 8 == (square / 8) + 1 && IS_BIT(board->bPosition, square + 6))
            {
                SET_BIT(possiblePos[1], square + 6);
            }
            // LEFT DOWN
            if ((square >= 10) && (square - 10) / 8 == (square / 8) - 1 && IS_BIT(board->bPosition, square - 10))
            {
                SET_BIT(possiblePos[1], square - 10);
            }
        }
        else{
            // UP RIGHT
            if ((square + 17 < 64) && (square + 17) / 8 == (square / 8) + 2 && IS_BIT(board->wPosition, square + 17))
            {
                SET_BIT(possiblePos[1], square + 17);
            }
            // UP LEFT
            if ((square + 15 < 64) && (square + 15) / 8 == (square / 8) + 2 && IS_BIT(board->wPosition, square + 15))
            {
                SET_BIT(possiblePos[1], square + 15);
            }
            // DOWN RIGHT
            if ((square >= 15) && (square - 15) / 8 == (square / 8) - 2 && IS_BIT(board->wPosition, square - 15))
            {
                SET_BIT(possiblePos[1], square - 15);
            }
            // DOWN LEFT
            if ((square >= 17) && (square - 17) / 8 == (square / 8) - 2 && IS_BIT(board->wPosition, square - 17))
            {
                SET_BIT(possiblePos[1], square - 17);
            }
            // RIGHT UP
            if ((square + 10 < 64) && (square + 10) / 8 == (square / 8) + 1 && IS_BIT(board->wPosition, square + 10))
            {
                SET_BIT(possiblePos[1], square + 10);
            }
            // RIGHT DOWN
            if ((square >= 6) && (square - 6) / 8 == (square / 8) - 1 && IS_BIT(board->wPosition, square - 6))
            {
                SET_BIT(possiblePos[1], square - 6);
            }
            // LEFT UP
            if ((square + 6 < 64) && (square + 10) / 8 == (square / 8) + 1 && IS_BIT(board->wPosition, square + 6))
            {
                SET_BIT(possiblePos[1], square + 6);
            }
            // LEFT DOWN
            if ((square >= 10) && (square - 10) / 8 == (square / 8) - 1 && IS_BIT(board->wPosition, square - 10))
            {
                SET_BIT(possiblePos[1], square - 10);
            }
        }
        return;
    }
    // ------------------------- KNIGHT -------------------------

    // ------------------------- BISHOP -------------------------
    // And Queen (Partially)
    if (IS_BIT(board->wBishop, square) || IS_BIT(board->wQueen, square) || IS_BIT(board->bBishop, square) || IS_BIT(board->bQueen, square))
    {
        // UP RIGHT
        for (int8_t i = square + 9; i < 64 && i%8 != 0; i += 9){
            if (!IS_BIT(board->wPosition, i) && !IS_BIT(board->bPosition, i))
                SET_BIT(possiblePos[0], i);

            // Search possible captures
            else if (((IS_BIT(board->wBishop, square) || IS_BIT(board->wQueen, square)) && IS_BIT(board->bPosition, i)) ||
                     ((IS_BIT(board->bBishop, square) || IS_BIT(board->bQueen, square)) && IS_BIT(board->wPosition, i)))
            {
                SET_BIT(possiblePos[1], i);
                break;
            }

            else
                break;
        }
        // UP LEFT
        for (int8_t i = square + 7; i < 64 && i%8 != 7; i += 7)
        {
            if (!IS_BIT(board->wPosition, i) && !IS_BIT(board->bPosition, i))
                SET_BIT(possiblePos[0], i);

            // Search possible captures
            else if (((IS_BIT(board->wBishop, square) || IS_BIT(board->wQueen, square)) && IS_BIT(board->bPosition, i)) ||
                     ((IS_BIT(board->bBishop, square) || IS_BIT(board->bQueen, square)) && IS_BIT(board->wPosition, i)))
            {
                SET_BIT(possiblePos[1], i);
                break;
            }

            else
                break;
        }
        // DOWN RIGHT
        for (int8_t i = square - 7; i >= 0 && i % 8 != 0; i -= 7)
        {
            if (!IS_BIT(board->wPosition, i) && !IS_BIT(board->bPosition, i))
                SET_BIT(possiblePos[0], i);

            // Search possible captures
            else if (((IS_BIT(board->wBishop, square) || IS_BIT(board->wQueen, square)) && IS_BIT(board->bPosition, i)) ||
                     ((IS_BIT(board->bBishop, square) || IS_BIT(board->bQueen, square)) && IS_BIT(board->wPosition, i)))
            {
                SET_BIT(possiblePos[1], i);
                break;
            }

            else
                break;
        }
        // DOWN LEFT
        for (int8_t i = square - 9; i >= 0 && i % 8 != 7; i -= 9)
        {
            if (!IS_BIT(board->wPosition, i) && !IS_BIT(board->bPosition, i))
                SET_BIT(possiblePos[0], i);

            // Search possible captures
            else if (((IS_BIT(board->wBishop, square) || IS_BIT(board->wQueen, square)) && IS_BIT(board->bPosition, i)) ||
                     ((IS_BIT(board->bBishop, square) || IS_BIT(board->bQueen, square)) && IS_BIT(board->wPosition, i)))
            {
                SET_BIT(possiblePos[1], i);
                break;
            }

            else
                break;
        }

        return;
    }
    // ------------------------- BISHOP -------------------------

    // ------------------------- WKING --------------------------
    if (IS_BIT(board->wKing, square))
    {
        // Castling to right Rook
        if(board->wCastled && IS_BIT(board->wRook, 7)){
            bool canCastle = 1;
            for (int8_t i = square+1; i < 7; i++){
                if(IS_BIT(board->wPosition, i))
                    canCastle = 0;
            }
            if(canCastle)
                SET_BIT(possiblePos[0], 6);
        }
        // Castling to left Rook
        if (board->wCastled && IS_BIT(board->wRook, 0))
        {
            bool canCastle = 1;
            for (int8_t i = square-1; i > 0; i--)
            {
                if (IS_BIT(board->wPosition, i))
                    canCastle = 0;
            }
            if (canCastle)
                SET_BIT(possiblePos[0], 6);
        }
        // UP
        for (int8_t i = square + 7; i < 64 && i < square + 10; i++){
            if (!IS_BIT(board->wPosition, i) && !IS_BIT(board->bPosition, i))
                SET_BIT(possiblePos[0], i);
        }
        // DOWN
        for (int8_t i = square - 7; i >= 0 && i > square - 10; i--)
        {
            if (!IS_BIT(board->wPosition, i) && !IS_BIT(board->bPosition, i))
                SET_BIT(possiblePos[0], i);
        }
        // RIGHT
        if (square + 1 < 64 && !IS_BIT(board->wPosition, square + 1) && !IS_BIT(board->bPosition, square + 1))
            SET_BIT(possiblePos[0], square + 1);
        // LEFT
        if (square - 1 >= 0 && !IS_BIT(board->wPosition, square - 1) && !IS_BIT(board->bPosition, square - 1))
            SET_BIT(possiblePos[0], square - 1);

        // Search possible Captures
        // UP
        for (int8_t i = square + 7; i < 64 && i < square + 10; i++)
        {
            if (IS_BIT(board->bPosition, i))
                SET_BIT(possiblePos[1], i);
        }
        // DOWN
        for (int8_t i = square - 7; i >= 0 && i > square - 10; i--)
        {
            if (IS_BIT(board->bPosition, i))
                SET_BIT(possiblePos[1], i);
        }
        // RIGHT
        if (square + 1 < 64 && IS_BIT(board->bPosition, square + 1))
            SET_BIT(possiblePos[1], square + 1);
        // LEFT
        if (square - 1 >= 0 && IS_BIT(board->bPosition, square - 1))
            SET_BIT(possiblePos[1], square - 1);

        return;
    }
    // ------------------------- WKING --------------------------

    // ------------------------- BKING --------------------------
    if (IS_BIT(board->bKing, square))
    {
        // Castling to right Rook
        if (board->bCastled && IS_BIT(board->bRook, 63))
        {
            bool canCastle = 1;
            for (int8_t i = square + 1; i < 63; i++)
            {
                if (IS_BIT(board->bPosition, i))
                    canCastle = 0;
            }
            if (canCastle)
                SET_BIT(possiblePos[0], 62);
        }
        // Castling to left Rook
        if (board->bCastled && IS_BIT(board->bRook, 56))
        {
            bool canCastle = 1;
            for (int8_t i = square - 1; i > 56; i--)
            {
                if (IS_BIT(board->bPosition, i))
                    canCastle = 0;
            }
            if (canCastle)
                SET_BIT(possiblePos[0], 58);
        }
        // UP
        for (int8_t i = square + 7; i < 64 && i < square + 10; i++)
        {
            if (!IS_BIT(board->bPosition, i) && !IS_BIT(board->wPosition, i))
                SET_BIT(possiblePos[0], i);
        }
        // DOWN
        for (int8_t i = square - 7; i >= 0 && i > square - 10; i--)
        {
            if (!IS_BIT(board->bPosition, i) && !IS_BIT(board->wPosition, i))
                SET_BIT(possiblePos[0], i);
        }
        // RIGHT
        if (square + 1 < 64 && !IS_BIT(board->bPosition, square + 1))
            SET_BIT(possiblePos[0], square + 1);
        // LEFT
        if (square - 1 >= 0 && !IS_BIT(board->bPosition, square - 1))
            SET_BIT(possiblePos[0], square - 1);

        // Search possible Captures
        // UP
        for (int8_t i = square + 7; i < 64 && i < square + 10; i++)
        {
            if (IS_BIT(board->wPosition, i))
                SET_BIT(possiblePos[1], i);
        }
        // DOWN
        for (int8_t i = square - 7; i >= 0 && i > square - 10; i--)
        {
            if (IS_BIT(board->wPosition, i))
                SET_BIT(possiblePos[1], i);
        }
        // RIGHT
        if (square + 1 < 64 && IS_BIT(board->wPosition, square + 1))
            SET_BIT(possiblePos[1], square + 1);
        // LEFT
        if (square - 1 >= 0 && IS_BIT(board->wPosition, square - 1))
            SET_BIT(possiblePos[1], square - 1);

        return;
    }
    // ------------------------- BKING --------------------------

}

#pragma endregion

#pragma region Game State

#pragma endregion