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

char GetCurrentPiece(const BitBoard *board, uint8_t square){
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

uint64_t *GetCurrentPieceEx(BitBoard *board, uint8_t square)
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

#pragma region Piece Moves

void PieceMove(BitBoard *board, uint64_t *Piece, uint8_t start, uint8_t target){
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
}

void GetPossibleMoves(BitBoard *board, uint8_t square, uint64_t *possiblePos, char Piece)
{
    *possiblePos = 0ULL;
    if(Piece = '.')
        return;

    // -----------------------------------------------------------
    // -------------------------- WHITE --------------------------
    // -----------------------------------------------------------

    // -------------------------- PAWN --------------------------
    if(Piece == 'P'){
        if(square+8 < 64 && !IS_BIT(board->wPosition, square + 8) && !IS_BIT(board->bPosition, square + 8))
            SET_BIT(*possiblePos, square + 8);

        // Double Square Edge Case
        if (square < 16 && !IS_BIT(board->wPosition, square + 16) && !IS_BIT(board->bPosition, square + 16))
            SET_BIT(*possiblePos, square + 16);

        return;
    }
    // -------------------------- PAWN --------------------------

    // -------------------------- ROOK --------------------------
    // And Queen (Partially)
    if(Piece == 'R' || Piece == 'Q'){
        // UP
        for (uint8_t i = square + 8; i < 64; i += 8){
            if (!IS_BIT(board->wPosition, i) && !IS_BIT(board->bPosition, i))
                SET_BIT(*possiblePos, i);
        }
        // DOWN
        for (uint8_t i = square - 8; i <= 0; i -= 8){
            if (!IS_BIT(board->wPosition, i) && !IS_BIT(board->bPosition, i))
                SET_BIT(*possiblePos, i);
        }
        // RIGHT
        // (square + (7 - (square % 8))) obtains the most right position of square's rank
        for (uint8_t i = square + 1; i <= (square + (7 - (square % 8))); i++)
        {
            if (!IS_BIT(board->wPosition, i) && !IS_BIT(board->bPosition, i))
                SET_BIT(*possiblePos, i);
        }
        // LEFT
        // (square - (square%8)) obtains the most left position of square's rank
        for (uint8_t i = square - 1; i >= (square - (square % 8)); i--)
        {
            if (!IS_BIT(board->wPosition, i) && !IS_BIT(board->bPosition, i))
                SET_BIT(*possiblePos, i);
        }

        if(Piece == 'R')
            return;
    }
    // -------------------------- ROOK --------------------------

    // ------------------------- KNIGHT -------------------------
    if(Piece == 'N'){
        // UP RIGHT
        if ((square + 17 < 64) && !IS_BIT(board->bPosition, square + 17) && !IS_BIT(board->wPosition, square + 17)){
            SET_BIT(*possiblePos, square + 17);
        }
        // UP LEFT
        if ((square + 15 < 64) && !IS_BIT(board->bPosition, square + 15) && !IS_BIT(board->wPosition, square + 15))
        {
            SET_BIT(*possiblePos, square + 15);
        }
        // DOWN RIGHT
        if ((square - 15 < 64) && !IS_BIT(board->bPosition, square - 15) && !IS_BIT(board->wPosition, square - 15))
        {
            SET_BIT(*possiblePos, square - 15);
        }
        // DOWN LEFT
        if ((square - 17 < 64) && !IS_BIT(board->bPosition, square - 17) && !IS_BIT(board->wPosition, square - 17))
        {
            SET_BIT(*possiblePos, square - 17);
        }
        // RIGHT UP
        if ((square + 10 < 64) && !IS_BIT(board->bPosition, square + 10) && !IS_BIT(board->wPosition, square + 10))
        {
            SET_BIT(*possiblePos, square + 10);
        }
        // RIGHT DOWN
        if ((square - 6 < 64) && !IS_BIT(board->bPosition, square - 6) && !IS_BIT(board->wPosition, square - 6))
        {
            SET_BIT(*possiblePos, square - 6);
        }
        // LEFT UP
        if ((square + 6 < 64) && !IS_BIT(board->bPosition, square + 6) && !IS_BIT(board->wPosition, square + 6))
        {
            SET_BIT(*possiblePos, square + 6);
        }
        // LEFT DOWN
        if ((square - 10 < 64) && !IS_BIT(board->bPosition, square - 10) && !IS_BIT(board->wPosition, square - 10))
        {
            SET_BIT(*possiblePos, square - 10);
        }

        return;
    }
    // ------------------------- KNIGHT -------------------------

    // ------------------------- BISHOP -------------------------
    // And Queen (Partially)
    if (Piece == 'B' || Piece == 'Q')
    {
        // UP RIGHT
        for (uint8_t i = square + 9; i < 64; i += 9){
            if (!IS_BIT(board->wPosition, i) && !IS_BIT(board->bPosition, i))
                SET_BIT(*possiblePos, i);
        }
        // UP LEFT
        for (uint8_t i = square + 7; i < 64; i += 7)
        {
            if (!IS_BIT(board->wPosition, i) && !IS_BIT(board->bPosition, i))
                SET_BIT(*possiblePos, i);
        }
        // DOWN RIGHT
        for (uint8_t i = square - 7; i >= 0; i -= 7)
        {
            if (!IS_BIT(board->wPosition, i) && !IS_BIT(board->bPosition, i))
                SET_BIT(*possiblePos, i);
        }
        // DOWN LEFT
        for (uint8_t i = square - 9; i >= 0; i -= 9)
        {
            if (!IS_BIT(board->wPosition, i) && !IS_BIT(board->bPosition, i))
                SET_BIT(*possiblePos, i);
        }

        return;
    }
    // ------------------------- BISHOP -------------------------

    // -------------------------- KING --------------------------
    if (Piece == 'K')
    {
        // Castling to right Rook
        if(board->wCastled && IS_BIT(board->wRook, 7)){
            _Bool canCastle = 1;
            for (uint8_t i = square+1; i < 7; i++){
                if(IS_BIT(board->wPosition, i))
                    canCastle = 0;
            }
            if(canCastle)
                SET_BIT(*possiblePos, 6);
        }
        // Castling to left Rook
        if (board->wCastled && IS_BIT(board->wRook, 0))
        {
            _Bool canCastle = 1;
            for (uint8_t i = square-1; i > 0; i--)
            {
                if (IS_BIT(board->wPosition, i))
                    canCastle = 0;
            }
            if (canCastle)
                SET_BIT(*possiblePos, 6);
        }
        // UP
        for (uint8_t i = square + 7; i < 64 && i < square + 10; i++){
            if (!IS_BIT(board->wPosition, i) && !IS_BIT(board->bPosition, i))
                SET_BIT(*possiblePos, i);
        }
        // DOWN
        for (uint8_t i = square - 7; i >= 0 && i > square - 10; i--)
        {
            if (!IS_BIT(board->wPosition, i) && !IS_BIT(board->bPosition, i))
                SET_BIT(*possiblePos, i);
        }
        // RIGHT
        if (square + 1 < 64 && !IS_BIT(board->wPosition, square + 1) && !IS_BIT(board->bPosition, square + 1))
            SET_BIT(*possiblePos, square + 1);
        // LEFT
        if (square - 1 >= 0 && !IS_BIT(board->wPosition, square - 1) && !IS_BIT(board->bPosition, square - 1))
            SET_BIT(*possiblePos, square - 1);

        return;
    }
    // -------------------------- KING --------------------------

    // -----------------------------------------------------------
    // -------------------------- WHITE --------------------------
    // -----------------------------------------------------------



    // -----------------------------------------------------------
    // -------------------------- BLACK --------------------------
    // -----------------------------------------------------------

    // -------------------------- PAWN --------------------------
    if (Piece == 'p')
    {
        if (square - 8 >= 0 && !IS_BIT(board->bPosition, square - 8) && !IS_BIT(board->wPosition, square - 8))
            SET_BIT(*possiblePos, square - 8);

        // Double Square Edge Case
        if (square > 47 && !IS_BIT(board->bPosition, square - 16) && !IS_BIT(board->wPosition, square - 16))
            SET_BIT(*possiblePos, square - 16);

        return;
    }
    // -------------------------- PAWN --------------------------

    // -------------------------- ROOK --------------------------
    // And Queen (Partially)
    if (Piece == 'r' || Piece == 'q')
    {
        // UP
        for (uint8_t i = square + 8; i < 64; i += 8)
        {
            if (!IS_BIT(board->bPosition, i) && !IS_BIT(board->wPosition, i))
                SET_BIT(*possiblePos, i);
        }
        // DOWN
        for (uint8_t i = square - 8; i <= 0; i -= 8)
        {
            if (!IS_BIT(board->bPosition, i) && !IS_BIT(board->wPosition, i))
                SET_BIT(*possiblePos, i);
        }
        // RIGHT
        // (square + (7 - (square % 8))) obtains the most right position of square's rank
        for (uint8_t i = square + 1; i <= (square + (7 - (square % 8))); i++)
        {
            if (!IS_BIT(board->bPosition, i) && !IS_BIT(board->wPosition, i))
                SET_BIT(*possiblePos, i);
        }
        // LEFT
        // (square - (square%8)) obtains the most left position of square's rank
        for (uint8_t i = square - 1; i >= (square - (square % 8)); i--)
        {
            if (!IS_BIT(board->bPosition, i) && !IS_BIT(board->wPosition, i))
                SET_BIT(*possiblePos, i);
        }

        if (Piece == 'r')
            return;
    }
    // -------------------------- ROOK --------------------------

    // ------------------------- KNIGHT -------------------------
    if (Piece == 'n')
    {
        // UP RIGHT
        if ((square + 17 < 64) && !IS_BIT(board->bPosition, square + 17) && !IS_BIT(board->wPosition, square + 17))
        {
            SET_BIT(*possiblePos, square + 17);
        }
        // UP LEFT
        if ((square + 15 < 64) && !IS_BIT(board->bPosition, square + 15) && !IS_BIT(board->wPosition, square + 15))
        {
            SET_BIT(*possiblePos, square + 15);
        }
        // DOWN RIGHT
        if ((square - 15 < 64) && !IS_BIT(board->bPosition, square - 15) && !IS_BIT(board->wPosition, square - 15))
        {
            SET_BIT(*possiblePos, square - 15);
        }
        // DOWN LEFT
        if ((square - 17 < 64) && !IS_BIT(board->bPosition, square - 17) && !IS_BIT(board->wPosition, square - 17))
        {
            SET_BIT(*possiblePos, square - 17);
        }
        // RIGHT UP
        if ((square + 10 < 64) && !IS_BIT(board->bPosition, square + 10) && !IS_BIT(board->wPosition, square + 10))
        {
            SET_BIT(*possiblePos, square + 10);
        }
        // RIGHT DOWN
        if ((square - 6 < 64) && !IS_BIT(board->bPosition, square - 6) && !IS_BIT(board->wPosition, square - 6))
        {
            SET_BIT(*possiblePos, square - 6);
        }
        // LEFT UP
        if ((square + 6 < 64) && !IS_BIT(board->bPosition, square + 6) && !IS_BIT(board->wPosition, square + 6))
        {
            SET_BIT(*possiblePos, square + 6);
        }
        // LEFT DOWN
        if ((square - 10 < 64) && !IS_BIT(board->bPosition, square - 10) && !IS_BIT(board->wPosition, square - 10))
        {
            SET_BIT(*possiblePos, square - 10);
        }

        return;
    }
    // ------------------------- KNIGHT -------------------------

    // ------------------------- BISHOP -------------------------
    // And Queen (Partially)
    if (Piece == 'b' || Piece == 'q')
    {
        // UP RIGHT
        for (uint8_t i = square + 9; i < 64; i += 9)
        {
            if (!IS_BIT(board->bPosition, i) && !IS_BIT(board->wPosition, i))
                SET_BIT(*possiblePos, i);
        }
        // UP LEFT
        for (uint8_t i = square + 7; i < 64; i += 7)
        {
            if (!IS_BIT(board->bPosition, i) && !IS_BIT(board->wPosition, i))
                SET_BIT(*possiblePos, i);
        }
        // DOWN RIGHT
        for (uint8_t i = square - 7; i >= 0; i -= 7)
        {
            if (!IS_BIT(board->bPosition, i) && !IS_BIT(board->wPosition, i))
                SET_BIT(*possiblePos, i);
        }
        // DOWN LEFT
        for (uint8_t i = square - 9; i >= 0; i -= 9)
        {
            if (!IS_BIT(board->bPosition, i) && !IS_BIT(board->wPosition, i))
                SET_BIT(*possiblePos, i);
        }

        return;
    }
    // ------------------------- BISHOP -------------------------

    // -------------------------- KING --------------------------
    if (Piece == 'K')
    {
        // Castling to right Rook
        if (board->bCastled && IS_BIT(board->bRook, 63))
        {
            _Bool canCastle = 1;
            for (uint8_t i = square + 1; i < 63; i++)
            {
                if (IS_BIT(board->bPosition, i))
                    canCastle = 0;
            }
            if (canCastle)
                SET_BIT(*possiblePos, 62);
        }
        // Castling to left Rook
        if (board->bCastled && IS_BIT(board->bRook, 56))
        {
            _Bool canCastle = 1;
            for (uint8_t i = square - 1; i > 56; i--)
            {
                if (IS_BIT(board->bPosition, i))
                    canCastle = 0;
            }
            if (canCastle)
                SET_BIT(*possiblePos, 58);
        }
        // UP
        for (uint8_t i = square + 7; i < 64 && i < square + 10; i++)
        {
            if (!IS_BIT(board->bPosition, i) && !IS_BIT(board->wPosition, i))
                SET_BIT(*possiblePos, i);
        }
        // DOWN
        for (uint8_t i = square - 7; i >= 0 && i > square - 10; i--)
        {
            if (!IS_BIT(board->bPosition, i) && !IS_BIT(board->wPosition, i))
                SET_BIT(*possiblePos, i);
        }
        // RIGHT
        if (square + 1 < 64 && !IS_BIT(board->bPosition, square + 1))
            SET_BIT(*possiblePos, square + 1);
        // LEFT
        if (square - 1 >= 0 && !IS_BIT(board->bPosition, square - 1))
            SET_BIT(*possiblePos, square - 1);

        return;
    }
    // -------------------------- KING --------------------------

    // -----------------------------------------------------------
    // -------------------------- BLACK --------------------------
    // -----------------------------------------------------------
}

#pragma endregion

#pragma region Piece Captures

void PieceCapture(BitBoard *board, uint64_t *Capturer, uint64_t *Captured, uint8_t start, uint8_t target)
{
    CLEAR_BIT(*Capturer, start);
    SET_BIT(*Capturer, target);
    if (IS_BIT(board->wPosition, start))
    {
        CLEAR_BIT(board->wPosition, start);
        SET_BIT(board->wPosition, target);
    }
    else
    {
        CLEAR_BIT(board->bPosition, start);
        SET_BIT(board->bPosition, target);
    }

    CLEAR_BIT(*Captured, target);
    if (IS_BIT(board->wPosition, target))
    {
        CLEAR_BIT(board->wPosition, target);
    }
    else
    {
        CLEAR_BIT(board->bPosition, target);
    }
}

void GetPossibleCaptures(BitBoard *board, uint8_t square, uint64_t *possiblePos, char Piece){
    *possiblePos = 0ULL;
    if (Piece = '.')
        return;

    // -----------------------------------------------------------
    // -------------------------- WHITE --------------------------
    // -----------------------------------------------------------

    // -------------------------- PAWN --------------------------
    if (Piece == 'P')
    {
        // Right
        if (square + 9 < 64 && IS_BIT(board->bPosition, square + 9))
            SET_BIT(*possiblePos, square + 9);
        // Left
        if (square + 7 < 64 && IS_BIT(board->bPosition, square + 7))
            SET_BIT(*possiblePos, square + 7);
        // En Passant
        // (square%8) obtains col number and (square/8) obtains row number 
        if ((board->enPassant == (square%8) - 1 || board->enPassant == (square%8) + 1) && (square/8) == 4)
            SET_BIT(*possiblePos, board->enPassant + 8);
        
        return;
    }
    // -------------------------- PAWN --------------------------

    // -------------------------- ROOK --------------------------
    // And Queen (Partially)
    if (Piece == 'R' || Piece == 'Q')
    {
        // UP
        for (uint8_t i = square + 8; i < 64; i += 8)
        {
            if (IS_BIT(board->bPosition, i))
                SET_BIT(*possiblePos, i);
            break;
        }
        // DOWN
        for (uint8_t i = square - 8; i <= 0; i -= 8)
        {
            if (IS_BIT(board->bPosition, i))
                SET_BIT(*possiblePos, i);
            break;
        }
        // RIGHT
        // (square + (7 - (square % 8))) obtains the most right position of square's rank
        for (uint8_t i = square + 1; i <= (square + (7 - (square % 8))); i++)
        {
            if (IS_BIT(board->bPosition, i))
                SET_BIT(*possiblePos, i);
            break;
        }
        // LEFT
        // (square - (square%8)) obtains the most left position of square's rank
        for (uint8_t i = square - 1; i >= (square - (square % 8)); i--)
        {
            if (IS_BIT(board->bPosition, i))
                SET_BIT(*possiblePos, i);
            break;
        }

        if (Piece == 'R')
            return;
    }
    // -------------------------- ROOK --------------------------

    // ------------------------- KNIGHT -------------------------
    if (Piece == 'N')
    {
        // UP RIGHT
        if ((square + 17 < 64) && IS_BIT(board->bPosition, square + 17))
        {
            SET_BIT(*possiblePos, square + 17);
        }
        // UP LEFT
        if ((square + 15 < 64) && IS_BIT(board->bPosition, square + 15))
        {
            SET_BIT(*possiblePos, square + 15);
        }
        // DOWN RIGHT
        if ((square - 15 < 64) && IS_BIT(board->bPosition, square - 15))
        {
            SET_BIT(*possiblePos, square - 15);
        }
        // DOWN LEFT
        if ((square - 17 < 64) && IS_BIT(board->bPosition, square - 17))
        {
            SET_BIT(*possiblePos, square - 17);
        }
        // RIGHT UP
        if ((square + 10 < 64) && IS_BIT(board->bPosition, square + 10))
        {
            SET_BIT(*possiblePos, square + 10);
        }
        // RIGHT DOWN
        if ((square - 6 < 64) && IS_BIT(board->bPosition, square - 6))
        {
            SET_BIT(*possiblePos, square - 6);
        }
        // LEFT UP
        if ((square + 6 < 64) && IS_BIT(board->bPosition, square + 6))
        {
            SET_BIT(*possiblePos, square + 6);
        }
        // LEFT DOWN
        if ((square - 10 < 64) && IS_BIT(board->bPosition, square - 10))
        {
            SET_BIT(*possiblePos, square - 10);
        }

        return;
    }
    // ------------------------- KNIGHT -------------------------

    // ------------------------- BISHOP -------------------------
    // And Queen (Partially)
    if (Piece == 'B' || Piece == 'Q')
    {
        // UP RIGHT
        for (uint8_t i = square + 9; i < 64; i += 9)
        {
            if (IS_BIT(board->bPosition, i))
                SET_BIT(*possiblePos, i);
            break;
        }
        // UP LEFT
        for (uint8_t i = square + 7; i < 64; i += 7)
        {
            if (IS_BIT(board->bPosition, i))
                SET_BIT(*possiblePos, i);
            break;
        }
        // DOWN RIGHT
        for (uint8_t i = square - 7; i >= 0; i -= 7)
        {
            if (IS_BIT(board->bPosition, i))
                SET_BIT(*possiblePos, i);
            break;
        }
        // DOWN LEFT
        for (uint8_t i = square - 9; i >= 0; i -= 9)
        {
            if (IS_BIT(board->bPosition, i))
                SET_BIT(*possiblePos, i);
            break;
        }

        return;
    }
    // ------------------------- BISHOP -------------------------

    // -------------------------- KING --------------------------
    if (Piece == 'K')
    {
        // UP
        for (uint8_t i = square + 7; i < 64 && i < square + 10; i++)
        {
            if (IS_BIT(board->bPosition, i))
                SET_BIT(*possiblePos, i);
        }
        // DOWN
        for (uint8_t i = square - 7; i >= 0 && i > square - 10; i--)
        {
            if (IS_BIT(board->bPosition, i))
                SET_BIT(*possiblePos, i);
        }
        // RIGHT
        if (square + 1 < 64 && IS_BIT(board->bPosition, square + 1))
            SET_BIT(*possiblePos, square + 1);
        // LEFT
        if (square - 1 >= 0 && IS_BIT(board->bPosition, square - 1))
            SET_BIT(*possiblePos, square - 1);
        return;
    }
    // -------------------------- KING --------------------------

    // -----------------------------------------------------------
    // -------------------------- WHITE --------------------------
    // -----------------------------------------------------------
}

#pragma endregion

#pragma region Game State

#pragma endregion