#include <bitboard.h>

BitBoard InitBoard(){
    BitBoard board;
    
    // xING POSITIONS
    // White
    board.wPawn     = 65280ULL;
    board.wRook     = 129ULL;
    board.wKnight   = 66ULL;
    board.wBishop   = 36ULL;
    board.wQueen    = 8ULL;
    board.wKing     = 16ULL;
    board.wPosition = 65535ULL;
    for (int8_t i = 0; i <= 64; i++){
        board.wMoveMap[i][0] = 0ULL;
        board.wMoveMap[i][1] = 0ULL;
    }
        // Black
    board.bPawn     = 71776119061217280ULL;
    board.bRook     = 9295429630892703744ULL;
    board.bKnight   = 4755801206503243776ULL;
    board.bBishop   = 2594073385365405696ULL;
    board.bQueen    = 576460752303423488ULL;
    board.bKing     = 1152921504606846976ULL;
    board.bPosition = 18446462598732840960ULL;
    for (int8_t i = 0; i <= 64; i++)
    {
        board.bMoveMap[i][0] = 0ULL;
        board.bMoveMap[i][1] = 0ULL;
    }
    // Castle
    board.wCastled  = 0;
    board.bCastled  = 0;
    // Promotion
    board.SelectPromotion = false;
    board.PromotionSquare = -1;
    board.PromotionSelection = '.'; // Promotion doesnt happen if not changed
    // En Passant
    board.enPassant = -1; // -1 if enpassant is illegible, collumn number if enpassant is legible (0-7)
    // Game State
    board.playerTurn = 0; // 1 if white, 0 if black, x with black because initial GameStateUpdater() call
    board.GameContinue = 1;
    board.wCheckMap = 0ULL;
    board.bCheckMap = 0ULL;

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

void PieceMove(BitBoard *board, int8_t x, int8_t target){
    uint64_t *Piece = GetCurrentPieceEx(board, x);

    // char piece = GetCurrentPiece(board, x);
    // printf("Moving piece: %c\n", piece);
    // for (int rank = 7; rank >= 0; rank--)
    // { 
    //     for (int file = 0; file < 8; file++)
    //     { 
    //         int square = rank * 8 + file;
    //         uint64_t mask = 1ULL << square;
    //         printf("%c ", (*Piece & mask) ? '1' : '.');
    //     }
    //     printf("\n");
    // }
    
    // Check and update if en passant
    if ((*Piece == board->wPawn && target - x == 16 && x < 16) ||
        (*Piece == board->bPawn && x - target == 16 && x > 47)){
        // printf("Updated enPassant data! Old : %d ", board->enPassant);
        board->enPassant = target % 8;
        // printf("New: %d\n", board->enPassant);
        }
    else{
        board->enPassant = -1;
    }

    // Check and update status of castling
    if(*Piece == board->wKing){
        if (target > x && target - x == 2){
            CLEAR_BIT(board->wRook, 7);
            SET_BIT(board->wRook, 5);
            CLEAR_BIT(board->wPosition, 7);
            SET_BIT(board->wPosition, 5);
        }
        else if(x > target && x - target == 2){
            CLEAR_BIT(board->wRook, 0);
            SET_BIT(board->wRook, 3);
            CLEAR_BIT(board->wPosition, 0);
            SET_BIT(board->wPosition, 3);
        }
    }
    else if(*Piece == board->bKing){
        if (target > x && target - x == 2){
            CLEAR_BIT(board->bRook, 63);
            SET_BIT(board->bRook, 61);
            CLEAR_BIT(board->bPosition, 63);
            SET_BIT(board->bPosition, 61);
        }
        else if (x > target && x - target == 2)
        {
            CLEAR_BIT(board->bRook, 56);
            SET_BIT(board->bRook, 59);
            CLEAR_BIT(board->bPosition, 56);
            SET_BIT(board->bPosition, 59);
        }
    }
    
    // Check and prompt if promotion
    if(*Piece == board->wPawn && target/8 == 7 && x/8 == 6){
        CLEAR_BIT(*Piece, x);
        CLEAR_BIT(board->wPosition, x);
        board->SelectPromotion = true;
        board->PromotionSquare = target;
        GameStateUpdater(board);
        return;
    }
    else if (*Piece == board->bPawn && target / 8 == 0 && x / 8 == 1){
        CLEAR_BIT(*Piece, x);
        CLEAR_BIT(board->bPosition, x);
        board->SelectPromotion = true;
        board->PromotionSquare = target;
        GameStateUpdater(board);
        return;
    }

    CLEAR_BIT(*Piece, x);
    SET_BIT(*Piece, target);
    if(IS_BIT(board->wPosition, x)){
        CLEAR_BIT(board->wPosition, x);
        SET_BIT(board->wPosition, target);
    }
    else{
        CLEAR_BIT(board->bPosition, x);
        SET_BIT(board->bPosition, target);
    }

    // piece = GetCurrentPiece(board, target);
    // printf("Moved piece: %c\n", piece);
    // for (int rank = 7; rank >= 0; rank--)
    // {
    //     for (int file = 0; file < 8; file++)
    //     {
    //         int square = rank * 8 + file;
    //         uint64_t mask = 1ULL << square;
    //         printf("%c ", (*Piece & mask) ? '1' : '.');
    //     }
    //     printf("\n");
    // }
    // printf("Resulting Chess Board:\nWhite\n");
    // for (int rank = 7; rank >= 0; rank--)
    // {
    //     for (int file = 0; file < 8; file++)
    //     {
    //         int square = rank * 8 + file;
    //         uint64_t mask = 1ULL << square;
    //         printf("%c ", (board->wPosition & mask) ? '1' : '.');
    //     }
    //     printf("\n");
    // }
    // printf("Black\n");
    // for (int rank = 7; rank >= 0; rank--)
    // {
    //     for (int file = 0; file < 8; file++)
    //     {
    //         int square = rank * 8 + file;
    //         uint64_t mask = 1ULL << square;
    //         printf("%c ", (board->bPosition & mask) ? '1' : '.');
    //     }
    //     printf("\n");
    // }
    GameStateUpdater(board);
}

void PieceCapture(BitBoard *board, int8_t x, int8_t target)
{
    uint64_t *Capturer = GetCurrentPieceEx(board, x);
    uint64_t *Captured;
    int8_t temp = target;
    // if en passant
    if (Capturer == &board->wPawn && GetCurrentPiece(board, target-8) == 'p' && 
        x / 8 == 4 && target % 8 == board->enPassant)
    {
        temp = target - 8;
        Captured = GetCurrentPieceEx(board, temp);
        board->enPassant = -1;
    }
    else if (Capturer == &board->bPawn && GetCurrentPiece(board, target + 8) == 'P' && 
        x / 8 == 3 && target % 8 == board->enPassant)
    {
        temp = target + 8;
        Captured = GetCurrentPieceEx(board, temp);
        board->enPassant = -1;
    }
    else{
        Captured = GetCurrentPieceEx(board, target);
    }

    // if with promotion
    if (*Capturer == board->wPawn && target / 8 == 7 && x / 8 == 6)
    {
        CLEAR_BIT(*Capturer, x);
        CLEAR_BIT(*Captured, target);
        CLEAR_BIT(board->wPosition, x);
        CLEAR_BIT(board->bPosition, target);
        board->SelectPromotion = true;
        board->PromotionSquare = target;
        GameStateUpdater(board);
        return;
    }
    else if (*Capturer == board->bPawn && target / 8 == 0 && x / 8 == 1)
    {
        CLEAR_BIT(*Capturer, x);
        CLEAR_BIT(*Captured, target);
        CLEAR_BIT(board->bPosition, x);
        CLEAR_BIT(board->wPosition, target);
        board->SelectPromotion = true;
        board->PromotionSquare = target;
        GameStateUpdater(board);
        return;
    }

    // char capturer = GetCurrentPiece(board, x);
    // char captured = GetCurrentPiece(board, target);
    // printf("Capturing... \n");
    // printf("Capturer: %c\n", capturer);
    // for (int rank = 7; rank >= 0; rank--)
    // {
    //     for (int file = 0; file < 8; file++)
    //     {
    //         int square = rank * 8 + file;
    //         uint64_t mask = 1ULL << square;
    //         printf("%c ", (*Capturer & mask) ? '1' : '.');
    //     }
    //     printf("\n");
    // }
    // printf("Captured: %c\n", captured);
    // for (int rank = 7; rank >= 0; rank--)
    // {
    //     for (int file = 0; file < 8; file++)
    //     {
    //         int square = rank * 8 + file;
    //         uint64_t mask = 1ULL << square;
    //         printf("%c ", (*Captured & mask) ? '1' : '.');
    //     }
    //     printf("\n");
    // }

    

    CLEAR_BIT(*Capturer, x);
    SET_BIT(*Capturer, target);
    CLEAR_BIT(*Captured, temp);
    if (IS_BIT(board->wPosition, x))
    {
        printf("White is the capturer!\n");
        CLEAR_BIT(board->wPosition, x);
        SET_BIT(board->wPosition, target);
        CLEAR_BIT(board->bPosition, temp);
    }
    else
    {
        printf("Black is the capturer!\n");
        CLEAR_BIT(board->bPosition, x);
        SET_BIT(board->bPosition, target);
        CLEAR_BIT(board->wPosition, temp);
    }

    // capturer = GetCurrentPiece(board, x);
    // captured = GetCurrentPiece(board, target);
    // printf("Captured! \n");
    // printf("Prev Cell: %c\n", capturer);
    // for (int rank = 7; rank >= 0; rank--)
    // {
    //     for (int file = 0; file < 8; file++)
    //     {
    //         int square = rank * 8 + file;
    //         uint64_t mask = 1ULL << square;
    //         printf("%c ", (*Capturer & mask) ? '1' : '.');
    //     }
    //     printf("\n");
    // }
    // printf("Captured Cell: %c\n", captured);
    // for (int rank = 7; rank >= 0; rank--)
    // {
    //     for (int file = 0; file < 8; file++)
    //     {
    //         int square = rank * 8 + file;
    //         uint64_t mask = 1ULL << square;
    //         printf("%c ", (*Captured & mask) ? '1' : '.');
    //     }
    //     printf("\n");
    // }

    // printf("Resulting Chess Board:\nWhite\n");
    // for (int rank = 7; rank >= 0; rank--)
    // {
    //     for (int file = 0; file < 8; file++)
    //     {
    //         int square = rank * 8 + file;
    //         uint64_t mask = 1ULL << square;
    //         printf("%c ", (board->wPosition & mask) ? '1' : '.');
    //     }
    //     printf("\n");
    // }
    // printf("Black\n");
    // for (int rank = 7; rank >= 0; rank--)
    // {
    //     for (int file = 0; file < 8; file++)
    //     {
    //         int square = rank * 8 + file;
    //         uint64_t mask = 1ULL << square;
    //         printf("%c ", (board->bPosition & mask) ? '1' : '.');
    //     }
    //     printf("\n");
    // }

    GameStateUpdater(board);
}

void PiecePromotion(BitBoard *board){
    uint64_t *Piece = GetPromotionPrompt(board);
    SET_BIT(*Piece, board->PromotionSquare);
    if(board->playerTurn)
        SET_BIT(board->wPosition, board->PromotionSquare);
    else
        SET_BIT(board->bPosition, board->PromotionSquare);
    board->SelectPromotion = false;
    GameStateUpdater(board);
    return;
}

uint64_t *GetPromotionPrompt(BitBoard *board){
    if(board->PromotionSelection == 'P')
        return &board->wPawn;
    if (board->PromotionSelection == 'R')
        return &board->wRook;
    if (board->PromotionSelection == 'N')
        return &board->wKnight;
    if (board->PromotionSelection == 'Q')
        return &board->wQueen;
    if (board->PromotionSelection == 'p')
        return &board->bPawn;
    if (board->PromotionSelection == 'r')
        return &board->bRook;
    if (board->PromotionSelection == 'n')
        return &board->bKnight;
    if (board->PromotionSelection == 'q')
        return &board->bQueen;
}

void GetPossibleMoves(BitBoard *board, int8_t square, bool isWhite)
{
    if ((!IS_BIT(board->wPosition, square) && isWhite) ||
        (!IS_BIT(board->bPosition, square) && !isWhite))
        return;

    if (isWhite)
    {
        board->wMoveMap[square + 1][0] = 0ULL;
        board->wMoveMap[square + 1][1] = 0ULL;
        // -------------------------- PAWN --------------------------
        if (IS_BIT(board->wPawn, square))
        {
            if (square + 8 < 64 && !IS_BIT(board->wPosition, square + 8) && !IS_BIT(board->bPosition, square + 8))
                SET_BIT(board->wMoveMap[square + 1][0], square + 8);

            // Double Square Edge Case
            if (square < 16 && !IS_BIT(board->wPosition, square + 16) && !IS_BIT(board->bPosition, square + 16))
                SET_BIT(board->wMoveMap[square + 1][0], square + 16);

            // Find Possible captures and protects
            // Right
            if (square + 9 < 64 && (square + 9) % 8 == (square + 1) % 8 && square % 8 < 7)
            {
                if (IS_BIT(board->bPosition, square + 9)){
                    SET_BIT(board->wMoveMap[square + 1][1], square + 9);
                    SET_BIT(board->wMoveMap[0][0], square + 9);
                }
                else if (IS_BIT(board->wPosition, square + 9)){
                    SET_BIT(board->wMoveMap[0][1], square + 9);
                }
            }
            // Left
            if (square + 7 < 64 && (square + 7) % 8 == (square - 1) % 8 && square % 8 > 0)
            {
                if (IS_BIT(board->bPosition, square + 7))
                {
                    SET_BIT(board->wMoveMap[square + 1][1], square + 7);
                    SET_BIT(board->wMoveMap[0][0], square + 7);
                }
                else if (IS_BIT(board->wPosition, square + 7))
                {
                    SET_BIT(board->wMoveMap[0][1], square + 7);
                }
            }
            // En Passant
            // (square%8) obtains col number and (square/8) obtains row number
            if ((board->enPassant == (square % 8) - 1 || board->enPassant == (square % 8) + 1) && (square / 8) == 4)
                SET_BIT(board->wMoveMap[square + 1][1], board->enPassant + 8 * ((square / 8) + 1));

            return;
        }
        // -------------------------- PAWN --------------------------

        // -------------------------- ROOK --------------------------
        // And Queen (partially)
        if(IS_BIT(board->wRook, square) || IS_BIT(board->wQueen, square)){
            // UP
            for (int8_t i = square + 8; i < 64; i += 8)
            {
                // Search protects
                if (IS_BIT(board->wPosition, i))
                {
                    SET_BIT(board->wMoveMap[0][1], i);
                    break;
                }

                // Search possible captures
                else if(IS_BIT(board->bPosition, i))
                {
                    SET_BIT(board->wMoveMap[square + 1][1], i);
                    SET_BIT(board->wMoveMap[0][0], i);
                    break;
                }

                // Search for moves
                else{
                    SET_BIT(board->wMoveMap[square + 1][0], i);
                    SET_BIT(board->wMoveMap[0][0], i);
                }
            }
            
            // DOWN
            for (int8_t i = square - 8; i >= 0; i -= 8)
            {
                // Search protects
                if (IS_BIT(board->wPosition, i))
                {
                    SET_BIT(board->wMoveMap[0][1], i);
                    break;
                }

                // Search possible captures
                else if (IS_BIT(board->bPosition, i))
                {
                    SET_BIT(board->wMoveMap[square + 1][1], i);
                    SET_BIT(board->wMoveMap[0][0], i);
                    break;
                }

                // Search for moves
                else
                {
                    SET_BIT(board->wMoveMap[square + 1][0], i);
                    SET_BIT(board->wMoveMap[0][0], i);
                }
            }
            
            // RIGHT
            // (square + (7 - (square % 8))) obtains the most right position of square's rank
            for (int8_t i = square + 1; i <= (square + (7 - (square % 8))); i++)
            {
                // Search protects
                if (IS_BIT(board->wPosition, i))
                {
                    SET_BIT(board->wMoveMap[0][1], i);
                    break;
                }

                // Search possible captures
                else if (IS_BIT(board->bPosition, i))
                {
                    SET_BIT(board->wMoveMap[square + 1][1], i);
                    SET_BIT(board->wMoveMap[0][0], i);
                    break;
                }

                // Search for moves
                else
                {
                    SET_BIT(board->wMoveMap[square + 1][0], i);
                    SET_BIT(board->wMoveMap[0][0], i);
                }
            }
            
            // LEFT
            // (square - (square%8)) obtains the most left position of square's rank
            for (int8_t i = square - 1; i >= (square - (square % 8)); i--)
            {
                // Search protects
                if (IS_BIT(board->wPosition, i))
                {
                    SET_BIT(board->wMoveMap[0][1], i);
                    break;
                }

                // Search possible captures
                else if (IS_BIT(board->bPosition, i))
                {
                    SET_BIT(board->wMoveMap[square + 1][1], i);
                    SET_BIT(board->wMoveMap[0][0], i);
                    break;
                }

                // Search for moves
                else
                {
                    SET_BIT(board->wMoveMap[square + 1][0], i);
                    SET_BIT(board->wMoveMap[0][0], i);
                }
            }

            if (IS_BIT(board->wRook, square))
                return;
        }
        // -------------------------- ROOK --------------------------

        // ------------------------- KNIGHT -------------------------
        if (IS_BIT(board->wKnight, square))
        {
            // UP RIGHT
            if ((square + 17 < 64) && (square + 17) / 8 == (square / 8) + 2){
                // Search for protects
                if (IS_BIT(board->wPosition, square + 17)){
                    SET_BIT(board->wMoveMap[0][1], square + 17);
                }
                // Search for captures
                else if(IS_BIT(board->bPosition, square + 17)){
                    SET_BIT(board->wMoveMap[square+1][1], square + 17);
                    SET_BIT(board->wMoveMap[0][0], square + 17);
                }
                // Search for moves
                else{
                    SET_BIT(board->wMoveMap[square + 1][0], square + 17);
                    SET_BIT(board->wMoveMap[0][0], square + 17);
                }
            }
            
            // UP LEFT
            if ((square + 15 < 64) && (square + 15) / 8 == (square / 8) + 2){
                // Search for protects
                if (IS_BIT(board->wPosition, square + 15))
                {
                    SET_BIT(board->wMoveMap[0][1], square + 15);
                }
                // Search for captures
                else if (IS_BIT(board->bPosition, square + 15))
                {
                    SET_BIT(board->wMoveMap[square + 1][1], square + 15);
                    SET_BIT(board->wMoveMap[0][0], square + 15);
                }
                // Search for moves
                else
                {
                    SET_BIT(board->wMoveMap[square + 1][0], square + 15);
                    SET_BIT(board->wMoveMap[0][0], square + 15);
                }
            }

            // DOWN RIGHT
            if ((square >= 15) && (square - 15) / 8 == (square / 8) - 2){
                // Search for protects
                if (IS_BIT(board->wPosition, square - 15))
                {
                    SET_BIT(board->wMoveMap[0][1], square - 15);
                }
                // Search for captures
                else if (IS_BIT(board->bPosition, square - 15))
                {
                    SET_BIT(board->wMoveMap[square + 1][1], square - 15);
                    SET_BIT(board->wMoveMap[0][0], square - 15);
                }
                // Search for moves
                else
                {
                    SET_BIT(board->wMoveMap[square + 1][0], square - 15);
                    SET_BIT(board->wMoveMap[0][0], square - 15);
                }
            }

            // DOWN LEFT
            if ((square >= 17) && (square - 17) / 8 == (square / 8) - 2){
                // Search for protects
                if (IS_BIT(board->wPosition, square - 17))
                {
                    SET_BIT(board->wMoveMap[0][1], square - 17);
                }
                // Search for captures
                else if (IS_BIT(board->bPosition, square - 17))
                {
                    SET_BIT(board->wMoveMap[square + 1][1], square - 17);
                    SET_BIT(board->wMoveMap[0][0], square - 17);
                }
                // Search for moves
                else
                {
                    SET_BIT(board->wMoveMap[square + 1][0], square - 17);
                    SET_BIT(board->wMoveMap[0][0], square - 17);
                }
            }

            // RIGHT UP
            if ((square + 10 < 64) && (square + 10) / 8 == (square / 8) + 1){
                // Search for protects
                if (IS_BIT(board->wPosition, square + 10))
                {
                    SET_BIT(board->wMoveMap[0][1], square + 10);
                }
                // Search for captures
                else if (IS_BIT(board->bPosition, square + 10))
                {
                    SET_BIT(board->wMoveMap[square + 1][1], square + 10);
                    SET_BIT(board->wMoveMap[0][0], square + 10);
                }
                // Search for moves
                else
                {
                    SET_BIT(board->wMoveMap[square + 1][0], square + 10);
                    SET_BIT(board->wMoveMap[0][0], square + 10);
                }
            }

            // RIGHT DOWN
            if ((square >= 6) && (square - 6) / 8 == (square / 8) - 1){
                // Search for protects
                if (IS_BIT(board->wPosition, square - 6))
                {
                    SET_BIT(board->wMoveMap[0][1], square - 6);
                }
                // Search for captures
                else if (IS_BIT(board->bPosition, square - 6))
                {
                    SET_BIT(board->wMoveMap[square + 1][1], square - 6);
                    SET_BIT(board->wMoveMap[0][0], square - 6);
                }
                // Search for moves
                else
                {
                    SET_BIT(board->wMoveMap[square + 1][0], square - 6);
                    SET_BIT(board->wMoveMap[0][0], square - 6);
                }
            }
            
            // LEFT UP
            if ((square + 6 < 64) && (square + 10) / 8 == (square / 8) + 1)
            {
                // Search for protects
                if (IS_BIT(board->wPosition, square + 6))
                {
                    SET_BIT(board->wMoveMap[0][1], square + 6);
                }
                // Search for captures
                else if (IS_BIT(board->bPosition, square + 6))
                {
                    SET_BIT(board->wMoveMap[square + 1][1], square + 6);
                    SET_BIT(board->wMoveMap[0][0], square + 6);
                }
                // Search for moves
                else
                {
                    SET_BIT(board->wMoveMap[square + 1][0], square + 6);
                    SET_BIT(board->wMoveMap[0][0], square + 6);
                }
            }
            
            // LEFT DOWN
            if ((square >= 10) && (square - 10) / 8 == (square / 8) - 1)
            {
                // Search for protects
                if (IS_BIT(board->wPosition, square - 10))
                {
                    SET_BIT(board->wMoveMap[0][1], square - 10);
                }
                // Search for captures
                else if (IS_BIT(board->bPosition, square - 10))
                {
                    SET_BIT(board->wMoveMap[square + 1][1], square - 10);
                    SET_BIT(board->wMoveMap[0][0], square - 10);
                }
                // Search for moves
                else
                {
                    SET_BIT(board->wMoveMap[square + 1][0], square - 10);
                    SET_BIT(board->wMoveMap[0][0], square - 10);
                }
            }

            return;
        }
        // ------------------------- KNIGHT -------------------------

        // ------------------------- BISHOP -------------------------
        // And Queen (partially)
        if (IS_BIT(board->wBishop, square) || IS_BIT(board->wQueen, square)){
            
            // UP RIGHT
            for (int8_t i = square + 9; i < 64 && i % 8 != 0; i += 9)
            {   
                // Search for protects
                if (IS_BIT(board->wPosition, i))
                {
                    SET_BIT(board->wMoveMap[0][1], i);
                    break;
                }
                // Search for captures
                else if (IS_BIT(board->bPosition, i))
                {
                    SET_BIT(board->wMoveMap[square+1][1], i);
                    SET_BIT(board->wMoveMap[0][0], i);
                    break;
                }
                // Search for moves
                else{   
                    SET_BIT(board->wMoveMap[square + 1][0], i);
                    SET_BIT(board->wMoveMap[0][0], i);
                }
            }

            // UP LEFT
            for (int8_t i = square + 7; i < 64 && i % 8 != 7; i += 7)
            {
                // Search for protects
                if (IS_BIT(board->wPosition, i))
                {
                    SET_BIT(board->wMoveMap[0][1], i);
                    break;
                }
                // Search for captures
                else if (IS_BIT(board->bPosition, i))
                {
                    SET_BIT(board->wMoveMap[square + 1][1], i);
                    SET_BIT(board->wMoveMap[0][0], i);
                    break;
                }
                // Search for moves
                else
                {
                    SET_BIT(board->wMoveMap[square + 1][0], i);
                    SET_BIT(board->wMoveMap[0][0], i);
                }
            }

            // DOWN RIGHT
            for (int8_t i = square - 7; i >= 0 && i % 8 != 0; i -= 7)
            {
                // Search for protects
                if (IS_BIT(board->wPosition, i))
                {
                    SET_BIT(board->wMoveMap[0][1], i);
                    break;
                }
                // Search for captures
                else if (IS_BIT(board->bPosition, i))
                {
                    SET_BIT(board->wMoveMap[square + 1][1], i);
                    SET_BIT(board->wMoveMap[0][0], i);
                    break;
                }
                // Search for moves
                else
                {
                    SET_BIT(board->wMoveMap[square + 1][0], i);
                    SET_BIT(board->wMoveMap[0][0], i);
                }
            }
            
            // DOWN LEFT
            for (int8_t i = square - 9; i >= 0 && i % 8 != 7; i -= 9)
            {
                // Search for protects
                if (IS_BIT(board->wPosition, i))
                {
                    SET_BIT(board->wMoveMap[0][1], i);
                    break;
                }
                // Search for captures
                else if (IS_BIT(board->bPosition, i))
                {
                    SET_BIT(board->wMoveMap[square + 1][1], i);
                    SET_BIT(board->wMoveMap[0][0], i);
                    break;
                }
                // Search for moves
                else
                {
                    SET_BIT(board->wMoveMap[square + 1][0], i);
                    SET_BIT(board->wMoveMap[0][0], i);
                }
            }

            return;
        }
        // ------------------------- BISHOP -------------------------

        // -------------------------- KING --------------------------
        if (IS_BIT(board->wKing, square))
        {
            // Castling to right Rook
            if (!board->wCastled && IS_BIT(board->wRook, 7))
            {
                if (!(board->wPosition & ((1ULL << 5) | (1ULL << 6)))) 
                {
                    SET_BIT(board->wMoveMap[square + 1][0], 6);
                    SET_BIT(board->wMoveMap[0][0], 6);
                }
            }
            
            // Castling to left Rook
            if (!board->wCastled && IS_BIT(board->wRook, 0))
            {
                if (!(board->wPosition & ((1ULL << 1) | (1ULL << 2) | (1ULL << 3)))){
                    SET_BIT(board->wMoveMap[square + 1][0], 2);
                    SET_BIT(board->wMoveMap[0][0], 2);
                }
            }
            
            // UP
            for (int8_t i = square + 7; i < 64 && i < square + 10; i++)
            {
                // Search for protects
                if(IS_BIT(board->wPosition, i)){
                    SET_BIT(board->wMoveMap[0][1], i);
                }
                // Search for captures
                else if(IS_BIT(board->bPosition, i)){
                    SET_BIT(board->wMoveMap[square + 1][1], i);
                    SET_BIT(board->wMoveMap[0][0], i);
                }
                // Search for moves
                else{
                    SET_BIT(board->wMoveMap[square + 1][0], i);
                    SET_BIT(board->wMoveMap[0][0], i);
                }
            }
            
            // DOWN
            for (int8_t i = square - 7; i >= 0 && i > square - 10; i--)
            {
                // Search for protects
                if (IS_BIT(board->wPosition, i))
                {
                    SET_BIT(board->wMoveMap[0][1], i);
                }
                // Search for captures
                else if (IS_BIT(board->bPosition, i))
                {
                    SET_BIT(board->wMoveMap[square + 1][1], i);
                    SET_BIT(board->wMoveMap[0][0], i);
                }
                // Search for moves
                else
                {
                    SET_BIT(board->wMoveMap[square + 1][0], i);
                    SET_BIT(board->wMoveMap[0][0], i);
                }
            }
            
            // RIGHT
            if (square + 1 < 64)
            {
                // Search for protects
                if (IS_BIT(board->wPosition, square+1))
                {
                    SET_BIT(board->wMoveMap[0][1], square+1);
                }
                // Search for captures
                else if (IS_BIT(board->bPosition, square+1))
                {
                    SET_BIT(board->wMoveMap[square + 1][1], square+1);
                    SET_BIT(board->wMoveMap[0][0], square+1);
                }
                // Search for moves
                else
                {
                    SET_BIT(board->wMoveMap[square + 1][0], square+1);
                    SET_BIT(board->wMoveMap[0][0], square+1);
                }
            }

            // LEFT
            if (square - 1 >= 0 && !IS_BIT(board->wPosition, square - 1) && !IS_BIT(board->bPosition, square - 1))
            {
                // Search for protects
                if (IS_BIT(board->wPosition, square - 1))
                {
                    SET_BIT(board->wMoveMap[0][1], square - 1);
                }
                // Search for captures
                else if (IS_BIT(board->bPosition, square - 1))
                {
                    SET_BIT(board->wMoveMap[square + 1][1], square - 1);
                    SET_BIT(board->wMoveMap[0][0], square - 1);
                }
                // Search for moves
                else
                {
                    SET_BIT(board->wMoveMap[square + 1][0], square - 1);
                    SET_BIT(board->wMoveMap[0][0], square - 1);
                }
            }

            return;
        }
        // -------------------------- KING --------------------------
    }

    else
    {
        board->bMoveMap[square + 1][0] = 0ULL;
        board->bMoveMap[square + 1][1] = 0ULL;
        // -------------------------- PAWN --------------------------
        if (IS_BIT(board->bPawn, square))
        {
            if (square - 8 >= 0 && !IS_BIT(board->bPosition, square - 8) && !IS_BIT(board->wPosition, square - 8))
                SET_BIT(board->bMoveMap[square + 1][0], square - 8);

            // Double Square Edge Case
            if (square > 47 && !IS_BIT(board->bPosition, square - 16) && !IS_BIT(board->wPosition, square - 16))
                SET_BIT(board->bMoveMap[square + 1][0], square - 16);

            // Search possible captures and protects
            // Right
            if (square >= 7 && (square - 7) % 8 == (square + 1) % 8)
            {
                if (IS_BIT(board->wPosition, square - 7)){
                    SET_BIT(board->bMoveMap[square + 1][1], square - 7);
                    SET_BIT(board->bMoveMap[0][0], square - 7);
                }
                else if (IS_BIT(board->bPosition, square - 7)){
                    SET_BIT(board->bMoveMap[0][1], square - 7);
                }
            }
            // Left
            if (square >= 9 && (square - 9) % 8 == (square - 1) % 8)
            {
                if (IS_BIT(board->wPosition, square - 9))
                {
                    SET_BIT(board->bMoveMap[square + 1][1], square - 9);
                    SET_BIT(board->bMoveMap[0][0], square - 9);
                }
                else if (IS_BIT(board->bPosition, square - 9))
                {
                    SET_BIT(board->bMoveMap[0][1], square - 9);
                }
            }
            // En Passant
            // (square%8) obtains col number and (square/8) obtains row number
            if ((board->enPassant == (square % 8) - 1 || board->enPassant == (square % 8) + 1) && (square / 8) == 3)
                SET_BIT(board->bMoveMap[square + 1][1], board->enPassant + 8 * ((square / 8) - 1));

            return;
        }
        // -------------------------- PAWN --------------------------

        // -------------------------- ROOK --------------------------
        // And Queen (partially)
        if (IS_BIT(board->bRook, square) || IS_BIT(board->bQueen, square))
        {
            // UP
            for (int8_t i = square + 8; i < 64; i += 8)
            {
                // Search protects
                if (IS_BIT(board->bPosition, i))
                {
                    SET_BIT(board->bMoveMap[0][1], i);
                    break;
                }

                // Search possible captures
                else if (IS_BIT(board->wPosition, i))
                {
                    SET_BIT(board->bMoveMap[square + 1][1], i);
                    SET_BIT(board->bMoveMap[0][0], i);
                    break;
                }

                // Search for moves
                else
                {
                    SET_BIT(board->bMoveMap[square + 1][0], i);
                    SET_BIT(board->bMoveMap[0][0], i);
                }
            }

            // DOWN
            for (int8_t i = square - 8; i >= 0; i -= 8)
            {
                // Search protects
                if (IS_BIT(board->bPosition, i))
                {
                    SET_BIT(board->bMoveMap[0][1], i);
                    break;
                }

                // Search possible captures
                else if (IS_BIT(board->wPosition, i))
                {
                    SET_BIT(board->bMoveMap[square + 1][1], i);
                    SET_BIT(board->bMoveMap[0][0], i);
                    break;
                }

                // Search for moves
                else
                {
                    SET_BIT(board->bMoveMap[square + 1][0], i);
                    SET_BIT(board->bMoveMap[0][0], i);
                }
            }

            // RIGHT
            // (square + (7 - (square % 8))) obtains the most right position of square's rank
            for (int8_t i = square + 1; i <= (square + (7 - (square % 8))); i++)
            {
                // Search protects
                if (IS_BIT(board->bPosition, i))
                {
                    SET_BIT(board->bMoveMap[0][1], i);
                    break;
                }

                // Search possible captures
                else if (IS_BIT(board->wPosition, i))
                {
                    SET_BIT(board->bMoveMap[square + 1][1], i);
                    SET_BIT(board->bMoveMap[0][0], i);
                    break;
                }

                // Search for moves
                else
                {
                    SET_BIT(board->bMoveMap[square + 1][0], i);
                    SET_BIT(board->bMoveMap[0][0], i);
                }
            }

            // LEFT
            // (square - (square%8)) obtains the most left position of square's rank
            for (int8_t i = square - 1; i >= (square - (square % 8)); i--)
            {
                // Search protects
                if (IS_BIT(board->bPosition, i))
                {
                    SET_BIT(board->bMoveMap[0][1], i);
                    break;
                }

                // Search possible captures
                else if (IS_BIT(board->wPosition, i))
                {
                    SET_BIT(board->bMoveMap[square + 1][1], i);
                    SET_BIT(board->bMoveMap[0][0], i);
                    break;
                }

                // Search for moves
                else
                {
                    SET_BIT(board->bMoveMap[square + 1][0], i);
                    SET_BIT(board->bMoveMap[0][0], i);
                }
            }

            if (IS_BIT(board->bRook, square))
                return;
        }
        // -------------------------- ROOK --------------------------

        // ------------------------- KNIGHT -------------------------
        if (IS_BIT(board->bKnight, square))
        {
            // UP RIGHT
            if ((square + 17 < 64) && (square + 17) / 8 == (square / 8) + 2)
            {
                // Search for protects
                if (IS_BIT(board->bPosition, square + 17))
                {
                    SET_BIT(board->bMoveMap[0][1], square + 17);
                }
                // Search for captures
                else if (IS_BIT(board->wPosition, square + 17))
                {
                    SET_BIT(board->bMoveMap[square + 1][1], square + 17);
                    SET_BIT(board->bMoveMap[0][0], square + 17);
                }
                // Search for moves
                else
                {
                    SET_BIT(board->bMoveMap[square + 1][0], square + 17);
                    SET_BIT(board->bMoveMap[0][0], square + 17);
                }
            }

            // UP LEFT
            if ((square + 15 < 64) && (square + 15) / 8 == (square / 8) + 2)
            {
                // Search for protects
                if (IS_BIT(board->bPosition, square + 15))
                {
                    SET_BIT(board->bMoveMap[0][1], square + 15);
                }
                // Search for captures
                else if (IS_BIT(board->wPosition, square + 15))
                {
                    SET_BIT(board->bMoveMap[square + 1][1], square + 15);
                    SET_BIT(board->bMoveMap[0][0], square + 15);
                }
                // Search for moves
                else
                {
                    SET_BIT(board->bMoveMap[square + 1][0], square + 15);
                    SET_BIT(board->bMoveMap[0][0], square + 15);
                }
            }

            // DOWN RIGHT
            if ((square >= 15) && (square - 15) / 8 == (square / 8) - 2)
            {
                // Search for protects
                if (IS_BIT(board->bPosition, square - 15))
                {
                    SET_BIT(board->bMoveMap[0][1], square - 15);
                }
                // Search for captures
                else if (IS_BIT(board->wPosition, square - 15))
                {
                    SET_BIT(board->bMoveMap[square + 1][1], square - 15);
                    SET_BIT(board->bMoveMap[0][0], square - 15);
                }
                // Search for moves
                else
                {
                    SET_BIT(board->bMoveMap[square + 1][0], square - 15);
                    SET_BIT(board->bMoveMap[0][0], square - 15);
                }
            }

            // DOWN LEFT
            if ((square >= 17) && (square - 17) / 8 == (square / 8) - 2)
            {
                // Search for protects
                if (IS_BIT(board->bPosition, square - 17))
                {
                    SET_BIT(board->bMoveMap[0][1], square - 17);
                }
                // Search for captures
                else if (IS_BIT(board->wPosition, square - 17))
                {
                    SET_BIT(board->bMoveMap[square + 1][1], square - 17);
                    SET_BIT(board->bMoveMap[0][0], square - 17);
                }
                // Search for moves
                else
                {
                    SET_BIT(board->bMoveMap[square + 1][0], square - 17);
                    SET_BIT(board->bMoveMap[0][0], square - 17);
                }
            }

            // RIGHT UP
            if ((square + 10 < 64) && (square + 10) / 8 == (square / 8) + 1)
            {
                // Search for protects
                if (IS_BIT(board->bPosition, square + 10))
                {
                    SET_BIT(board->bMoveMap[0][1], square + 10);
                }
                // Search for captures
                else if (IS_BIT(board->wPosition, square + 10))
                {
                    SET_BIT(board->bMoveMap[square + 1][1], square + 10);
                    SET_BIT(board->bMoveMap[0][0], square + 10);
                }
                // Search for moves
                else
                {
                    SET_BIT(board->bMoveMap[square + 1][0], square + 10);
                    SET_BIT(board->bMoveMap[0][0], square + 10);
                }
            }

            // RIGHT DOWN
            if ((square >= 6) && (square - 6) / 8 == (square / 8) - 1)
            {
                // Search for protects
                if (IS_BIT(board->bPosition, square - 6))
                {
                    SET_BIT(board->bMoveMap[0][1], square - 6);
                }
                // Search for captures
                else if (IS_BIT(board->wPosition, square - 6))
                {
                    SET_BIT(board->bMoveMap[square + 1][1], square - 6);
                    SET_BIT(board->bMoveMap[0][0], square - 6);
                }
                // Search for moves
                else
                {
                    SET_BIT(board->bMoveMap[square + 1][0], square - 6);
                    SET_BIT(board->bMoveMap[0][0], square - 6);
                }
            }

            // LEFT UP
            if ((square + 6 < 64) && (square + 10) / 8 == (square / 8) + 1)
            {
                // Search for protects
                if (IS_BIT(board->bPosition, square + 6))
                {
                    SET_BIT(board->bMoveMap[0][1], square + 6);
                }
                // Search for captures
                else if (IS_BIT(board->wPosition, square + 6))
                {
                    SET_BIT(board->bMoveMap[square + 1][1], square + 6);
                    SET_BIT(board->bMoveMap[0][0], square + 6);
                }
                // Search for moves
                else
                {
                    SET_BIT(board->bMoveMap[square + 1][0], square + 6);
                    SET_BIT(board->bMoveMap[0][0], square + 6);
                }
            }

            // LEFT DOWN
            if ((square >= 10) && (square - 10) / 8 == (square / 8) - 1)
            {
                // Search for protects
                if (IS_BIT(board->bPosition, square - 10))
                {
                    SET_BIT(board->bMoveMap[0][1], square - 10);
                }
                // Search for captures
                else if (IS_BIT(board->wPosition, square - 10))
                {
                    SET_BIT(board->bMoveMap[square + 1][1], square - 10);
                    SET_BIT(board->bMoveMap[0][0], square - 10);
                }
                // Search for moves
                else
                {
                    SET_BIT(board->bMoveMap[square + 1][0], square - 10);
                    SET_BIT(board->bMoveMap[0][0], square - 10);
                }
            }

            return;
        }
        // ------------------------- KNIGHT -------------------------

        // ------------------------- BISHOP -------------------------
        // And Queen (partially)
        if (IS_BIT(board->bBishop, square) || IS_BIT(board->bQueen, square))
        {

            // UP RIGHT
            for (int8_t i = square + 9; i < 64 && i % 8 != 0; i += 9)
            {
                // Search for protects
                if (IS_BIT(board->bPosition, i))
                {
                    SET_BIT(board->bMoveMap[0][1], i);
                    break;
                }
                // Search for captures
                else if (IS_BIT(board->wPosition, i))
                {
                    SET_BIT(board->bMoveMap[square + 1][1], i);
                    SET_BIT(board->bMoveMap[0][0], i);
                    break;
                }
                // Search for moves
                else
                {
                    SET_BIT(board->bMoveMap[square + 1][0], i);
                    SET_BIT(board->bMoveMap[0][0], i);
                }
            }

            // UP LEFT
            for (int8_t i = square + 7; i < 64 && i % 8 != 7; i += 7)
            {
                // Search for protects
                if (IS_BIT(board->bPosition, i))
                {
                    SET_BIT(board->bMoveMap[0][1], i);
                    break;
                }
                // Search for captures
                else if (IS_BIT(board->wPosition, i))
                {
                    SET_BIT(board->bMoveMap[square + 1][1], i);
                    SET_BIT(board->bMoveMap[0][0], i);
                    break;
                }
                // Search for moves
                else
                {
                    SET_BIT(board->bMoveMap[square + 1][0], i);
                    SET_BIT(board->bMoveMap[0][0], i);
                }
            }

            // DOWN RIGHT
            for (int8_t i = square - 7; i >= 0 && i % 8 != 0; i -= 7)
            {
                // Search for protects
                if (IS_BIT(board->bPosition, i))
                {
                    SET_BIT(board->bMoveMap[0][1], i);
                    break;
                }
                // Search for captures
                else if (IS_BIT(board->wPosition, i))
                {
                    SET_BIT(board->bMoveMap[square + 1][1], i);
                    SET_BIT(board->bMoveMap[0][0], i);
                    break;
                }
                // Search for moves
                else
                {
                    SET_BIT(board->bMoveMap[square + 1][0], i);
                    SET_BIT(board->bMoveMap[0][0], i);
                }
            }

            // DOWN LEFT
            for (int8_t i = square - 9; i >= 0 && i % 8 != 7; i -= 9)
            {
                // Search for protects
                if (IS_BIT(board->bPosition, i))
                {
                    SET_BIT(board->bMoveMap[0][1], i);
                    break;
                }
                // Search for captures
                else if (IS_BIT(board->wPosition, i))
                {
                    SET_BIT(board->bMoveMap[square + 1][1], i);
                    SET_BIT(board->bMoveMap[0][0], i);
                    break;
                }
                // Search for moves
                else
                {
                    SET_BIT(board->bMoveMap[square + 1][0], i);
                    SET_BIT(board->bMoveMap[0][0], i);
                }
            }
            return;
        }
        // ------------------------- BISHOP -------------------------

        // -------------------------- KING --------------------------
        if (IS_BIT(board->bKing, square))
        {
            // Castling to right Rook
            if (!board->bCastled && IS_BIT(board->bRook, 63))
            {
                if (!(board->bPosition & ((1ULL << 61) | (1ULL << 62))))
                {
                    SET_BIT(board->bMoveMap[square + 1][0], 62);
                    SET_BIT(board->bMoveMap[0][0], 62);
                }
            }

            // Castling to left Rook
            if (!board->bCastled && IS_BIT(board->bRook, 56))
            {
                if (!(board->bPosition & ((1ULL << 59) | (1ULL << 58) | (1ULL << 57))))
                {
                    SET_BIT(board->bMoveMap[square + 1][0], 58);
                    SET_BIT(board->bMoveMap[0][0], 58);
                }
            }

            // UP
            for (int8_t i = square + 7; i < 64 && i < square + 10; i++)
            {
                // Search for protects
                if (IS_BIT(board->bPosition, i))
                {
                    SET_BIT(board->bMoveMap[0][1], i);
                }
                // Search for captures
                else if (IS_BIT(board->bPosition, i))
                {
                    SET_BIT(board->bMoveMap[square + 1][1], i);
                    SET_BIT(board->bMoveMap[0][0], i);
                }
                // Search for moves
                else
                {
                    SET_BIT(board->bMoveMap[square + 1][0], i);
                    SET_BIT(board->bMoveMap[0][0], i);
                }
            }

            // DOWN
            for (int8_t i = square - 7; i >= 0 && i > square - 10; i--)
            {
                // Search for protects
                if (IS_BIT(board->bPosition, i))
                {
                    SET_BIT(board->bMoveMap[0][1], i);
                }
                // Search for captures
                else if (IS_BIT(board->wPosition, i))
                {
                    SET_BIT(board->bMoveMap[square + 1][1], i);
                    SET_BIT(board->bMoveMap[0][0], i);
                }
                // Search for moves
                else
                {
                    SET_BIT(board->bMoveMap[square + 1][0], i);
                    SET_BIT(board->bMoveMap[0][0], i);
                }
            }

            // RIGHT
            if (square + 1 < 64)
            {
                // Search for protects
                if (IS_BIT(board->bPosition, square + 1))
                {
                    SET_BIT(board->bMoveMap[0][1], square + 1);
                }
                // Search for captures
                else if (IS_BIT(board->wPosition, square + 1))
                {
                    SET_BIT(board->bMoveMap[square + 1][1], square + 1);
                    SET_BIT(board->bMoveMap[0][0], square + 1);
                }
                // Search for moves
                else
                {
                    SET_BIT(board->bMoveMap[square + 1][0], square + 1);
                    SET_BIT(board->bMoveMap[0][0], square + 1);
                }
            }

            // LEFT
            if (square - 1 >= 0 && !IS_BIT(board->wPosition, square - 1) && !IS_BIT(board->bPosition, square - 1))
            {
                // Search for protects
                if (IS_BIT(board->bPosition, square - 1))
                {
                    SET_BIT(board->bMoveMap[0][1], square - 1);
                }
                // Search for captures
                else if (IS_BIT(board->wPosition, square - 1))
                {
                    SET_BIT(board->bMoveMap[square + 1][1], square - 1);
                    SET_BIT(board->bMoveMap[0][0], square - 1);
                }
                // Search for moves
                else
                {
                    SET_BIT(board->bMoveMap[square + 1][0], square - 1);
                    SET_BIT(board->bMoveMap[0][0], square - 1);
                }
            }

            return;
        }
        // -------------------------- KING --------------------------
    }

}

#pragma endregion

#pragma region Game State

void GenerateMoveMap(BitBoard *board){
    board->wMoveMap[0][0] = 0ULL;
    board->wMoveMap[0][1] = 0ULL;
    board->bMoveMap[0][0] = 0ULL;
    board->bMoveMap[0][1] = 0ULL;
    for (int8_t i = 0; i < 64; i++){
        board->wMoveMap[i + 1][0] = 0ULL;
        board->wMoveMap[i + 1][1] = 0ULL;
        board->bMoveMap[i + 1][0] = 0ULL;
        board->bMoveMap[i + 1][1] = 0ULL;

        GetPossibleMoves(board, i, true);
        GetPossibleMoves(board, i, false);
    }
}

void GenerateCheckMap(BitBoard *board){
    board->wCheckMap = 0ULL;
    board->bCheckMap = 0ULL;

    if (!AND_BIT(board->bMoveMap[0][0], board->wKing) &&
        !AND_BIT(board->wMoveMap[0][0], board->bKing))
        return;

    if(board->playerTurn){
        int8_t wKingPos = __builtin_ctzll(board->wKing);
        for (int8_t i = 1; i <= 64; i++)
        {
            if (AND_BIT(board->bMoveMap[i][1], board->wKing))
            {
                SET_BIT(board->wCheckMap, i-1);

                int8_t h = i-1 > wKingPos ? i-1 : wKingPos;
                int8_t l = i-1 < wKingPos ? i-1 : wKingPos;

                // if Rook or Queen (Vertical)
                if ((i-1 - wKingPos) % 8 == 0)
                {
                    for (int8_t x = l + 8; x < h; x += 8)
                    {
                        SET_BIT(board->wCheckMap, x);
                    }
                    continue;
                }
                // if Rook or Queen (Horizontal)
                if ((i-1) / 8 == wKingPos / 8)
                {
                    for (int8_t x = l + 1; x < h; x++)
                    {
                        SET_BIT(board->wCheckMap, x);
                    }
                    continue;
                }
                // if Bishop or Queen (Right)
                if ((i-1 - wKingPos) % 9 == 0)
                {
                    for (int8_t x = l + 9; x < h; x += 9)
                    {
                        SET_BIT(board->wCheckMap, x);
                    }
                    continue;
                }
                if ((i-1 - wKingPos) % 7 == 0)
                {
                    for (int8_t x = l + 7; x < h; x += 7)
                    {
                        SET_BIT(board->wCheckMap, x);
                    }
                    continue;
                }
            }
        }
    }
    
    else{
        int8_t bKingPos = __builtin_ctzll(board->bKing);

        for (int8_t i = 1; i <= 64; i++)
        {
            if (AND_BIT(board->wMoveMap[i][1], board->bKing))
            {
                SET_BIT(board->bCheckMap, i-1);

                int8_t h = i-1 > bKingPos ? i-1 : bKingPos;
                int8_t l = i-1 < bKingPos ? i-1 : bKingPos;

                // if Rook or Queen (Vertical)
                if ((i-1 - bKingPos) % 8 == 0)
                {
                    for (int8_t x = l + 8; x < h; x += 8)
                    {
                        SET_BIT(board->bCheckMap, x);
                    }
                    continue;
                }
                // if Rook or Queen (Horizontal)
                if ((i-1) / 8 == bKingPos / 8)
                {
                    for (int8_t x = l + 1; x < h; x++)
                    {
                        SET_BIT(board->bCheckMap, x);
                    }
                    continue;
                }
                // if Bishop or Queen (Right)
                if ((i-1 - bKingPos) % 9 == 0)
                {
                    for (int8_t x = l + 9; x < h; x += 9)
                    {
                        SET_BIT(board->bCheckMap, x);
                    }
                    continue;
                }
                if ((i-1 - bKingPos) % 7 == 0)
                {
                    for (int8_t x = l + 7; x < h; x += 7)
                    {
                        SET_BIT(board->bCheckMap, x);
                    }
                    continue;
                }
            }
        }
    }

}

void ClearIllegalMoves(BitBoard *board){
    
    if(board->playerTurn){
        int8_t wKingPos = __builtin_ctzll(board->wKing);
        
        // Clear king's illegal moves
        board->wMoveMap[wKingPos+1][1] &= ~(board->bMoveMap[0][1]); // king cannot capture protected pieces
        board->wMoveMap[wKingPos+1][0] &= ~(board->bMoveMap[0][0]); // king cannot move to a possible check square

        board->wMoveMap[0][0] = board->wMoveMap[wKingPos+1][1] | board->wMoveMap[wKingPos+1][0]; // Reset move map to match cleared illegal moves
        for (int8_t i = 1; i <= 64; i++){
            if(i-1 == wKingPos || !IS_BIT(board->wPosition, i-1))
                continue;
            if(board->wCheckMap != 0){

                board->wMoveMap[i][0] &= board->wCheckMap;  // Remove any moves that is not to block check
                board->wMoveMap[i][1] &= board->wCheckMap;  // Remove any captures that is not to stop check
            }
            // Prevent piece protecting the king to be moved
            if(IS_BIT(board->bMoveMap[0][0], i-1) && i == 12){
                bool protecting = true;

                // Vertical
                if ((i - 1 - wKingPos) % 8 == 0)
                {
                    if(i-1 > wKingPos){
                        for (int8_t x = wKingPos + 8; x < i-1; x += 8)
                        {
                            if(IS_BIT(board->wPosition, x)){
                                protecting = false;
                                break;
                            }
                        }
                        bool temp = false;
                        for(int8_t x = i + 7; x < 64; x += 8){
                            // If the attacker is right above
                            if (IS_BIT(board->bRook, x) || IS_BIT(board->bQueen, x))
                            {
                                temp = true;
                                break;
                            }
                        }
                        if(!temp)
                            protecting = false;
                    }
                    else{
                        for (int8_t x = wKingPos - 8; x > i-1; x -= 8)
                        {
                            if (IS_BIT(board->wPosition, x))
                            {
                                protecting = false;
                                break;
                            }
                        }
                        bool temp = false;
                        for (int8_t x = i - 9; x >= 0; x -= 8)
                        {
                            // If the attacker is right below
                            if (IS_BIT(board->bRook, x) || IS_BIT(board->bQueen, x))
                            {
                                temp = true;
                                break;
                            }
                        }
                        if (!temp)
                            protecting = false;
                    }

                }
                
                // Horizontal
                else if ((i - 1) / 8 == wKingPos / 8)
                {
                    if (i - 1 > wKingPos)
                    {
                        for (int8_t x = wKingPos + 1; x < i - 1; x++)
                        {
                            if (IS_BIT(board->wPosition, x))
                            {
                                protecting = false;
                                break;
                            }
                        }
                        bool temp = false;
                        for (int8_t x = i; x < 64; x++)
                        {
                            // If the attacker is right above
                            if (IS_BIT(board->bRook, x) || IS_BIT(board->bQueen, x))
                            {
                                temp = true;
                                break;
                            }
                        }
                        if (!temp)
                            protecting = false;
                    }
                    else
                    {
                        for (int8_t x = wKingPos - 1; x > i-1; x--)
                        {
                            if (IS_BIT(board->wPosition, x))
                            {
                                protecting = false;
                                break;
                            }
                        }
                        bool temp = false;
                        for (int8_t x = i - 2; x >= 0; x--)
                        {
                            // If the attacker is right above
                            if (IS_BIT(board->bRook, x) || IS_BIT(board->bQueen, x))
                            {
                                temp = true;
                                break;
                            }
                        }
                        if (!temp)
                            protecting = false;
                    }
                }
                
                // Diagonal Right
                else if ((i - 1 - wKingPos) % 9 == 0)
                {
                    if (i - 1 > wKingPos)
                    {
                        for (int8_t x = wKingPos + 9; x < i - 1; x += 9)
                        {
                            if (IS_BIT(board->wPosition, x))
                            {
                                protecting = false;
                                break;
                            }
                        }
                        bool temp = false;
                        for (int8_t x = i + 8; x < 64; x += 9)
                        {
                            // If the attacker is right above
                            if (IS_BIT(board->bBishop, x) || IS_BIT(board->bQueen, x))
                            {
                                temp = true;
                                break;
                            }
                        }
                        if (!temp)
                            protecting = false;
                    }
                    else
                    {
                        for (int8_t x = wKingPos - 9; x > i - 1; x -= 9)
                        {
                            if (IS_BIT(board->wPosition, x))
                            {
                                protecting = false;
                                break;
                            }
                        }
                        bool temp = false;
                        for (int8_t x = i - 10; x >= 0; x -= 9)
                        {
                            // If the attacker is right above
                            if (IS_BIT(board->bBishop, x) || IS_BIT(board->bQueen, x))
                            {
                                temp = true;
                                break;
                            }
                        }
                        if (!temp)
                            protecting = false;
                    }
                }
                
                // Diagonal Left
                else if ((i - 1 - wKingPos) % 7 == 0)
                {
                    if (i - 1 > wKingPos)
                    {
                        for (int8_t x = wKingPos + 7; x < i - 1; x += 7)
                        {
                            if (IS_BIT(board->wPosition, x))
                            {
                                protecting = false;
                                break;
                            }
                        }
                        bool temp = false;
                        for (int8_t x = i + 6; x < 64; x += 7)
                        {
                            // If the attacker is right above
                            if (IS_BIT(board->bBishop, x) || IS_BIT(board->bQueen, x))
                            {
                                temp = true;
                                break;
                            }
                        }
                        if (!temp)
                            protecting = false;
                    }
                    else
                    {
                        for (int8_t x = wKingPos - 7; x > i - 1; x -= 7)
                        {
                            if (IS_BIT(board->wPosition, x))
                            {
                                protecting = false;
                                break;
                            }
                        }
                        bool temp = false;
                        for (int8_t x = i - 8; x >= 0; x -= 7)
                        {
                            // If the attacker is right above
                            if (IS_BIT(board->bBishop, x) || IS_BIT(board->bQueen, x))
                            {
                                temp = true;
                                break;
                            }
                        }
                        if (!temp)
                            protecting = false;
                    }
                }

                else
                {
                    protecting = false;
                }

                if(protecting){
                    board->wMoveMap[i][0] = 0ULL;
                    board->wMoveMap[i][1] = 0ULL;
                }
            }

            board->wMoveMap[0][0] |= board->wMoveMap[i][0] | board->wMoveMap[i][1]; // Add back moves after removing
        }
    }
    
    else
    {
        int8_t bKingPos = __builtin_ctzll(board->bKing);
        // Clear king's illegal moves

        board->bMoveMap[bKingPos+1][1] &= ~(board->wMoveMap[0][1]); // king cannot capture protected pieces
        board->bMoveMap[bKingPos+1][0] &= ~(board->wMoveMap[0][0]); // king cannot move to a possible check square

        board->bMoveMap[0][0] = board->bMoveMap[bKingPos+1][1] | board->bMoveMap[bKingPos+1][0]; // Reset move map to match cleared illegal moves


        for (int8_t i = 1; i <= 64; i++)
        {
            if (i-1 == bKingPos || !IS_BIT(board->bPosition, i-1))
                continue;
            if(board->bCheckMap != 0){
                board->bMoveMap[i][0] &= board->bCheckMap;  // Remove any moves that is not to block check
                board->bMoveMap[i][1] &= board->bCheckMap;  // Remove any captures that is not to stop check
            }

            if (IS_BIT(board->wMoveMap[0][0], i - 1))
            {
                bool protecting = true;

                // Vertical
                if ((i - 1 - bKingPos) % 8 == 0)
                {
                    if (i - 1 > bKingPos)
                    {
                        for (int8_t x = bKingPos + 8; x < i - 1; x += 8)
                        {
                            if (IS_BIT(board->wPosition, x))
                            {
                                protecting = false;
                                break;
                            }
                        }
                        bool temp = false;
                        for (int8_t x = i + 7; x < 64; x += 8)
                        {
                            // If the attacker is right above
                            if (IS_BIT(board->wRook, x) || IS_BIT(board->wQueen, x))
                            {
                                temp = true;
                                break;
                            }
                        }
                        if (!temp)
                            protecting = false;
                    }
                    else
                    {
                        for (int8_t x = bKingPos - 8; x > i - 1; x -= 8)
                        {
                            if (IS_BIT(board->wPosition, x))
                            {
                                protecting = false;
                                break;
                            }
                        }
                        bool temp = false;
                        for (int8_t x = i - 9; x >= 0; x -= 8)
                        {
                            // If the attacker is right above
                            if (IS_BIT(board->wRook, x) || IS_BIT(board->wQueen, x))
                            {
                                temp = true;
                                break;
                            }
                        }
                        if (!temp)
                            protecting = false;
                    }
                }

                // Horizontal
                else if ((i - 1) / 8 == bKingPos / 8)
                {
                    if (i - 1 > bKingPos)
                    {
                        for (int8_t x = bKingPos + 1; x < i - 1; x++)
                        {
                            if (IS_BIT(board->wPosition, x))
                            {
                                protecting = false;
                                break;
                            }
                        }
                        bool temp = false;
                        for (int8_t x = i; x < 64; x++)
                        {
                            // If the attacker is right above
                            if (IS_BIT(board->wRook, x) || IS_BIT(board->wQueen, x))
                            {
                                temp = true;
                                break;
                            }
                        }
                        if (!temp)
                            protecting = false;
                    }
                    else
                    {
                        for (int8_t x = bKingPos - 1; x > i - 1; x--)
                        {
                            if (IS_BIT(board->wPosition, x))
                            {
                                protecting = false;
                                break;
                            }
                        }
                        bool temp = false;
                        for (int8_t x = i - 2; x >= 0; x--)
                        {
                            // If the attacker is right above
                            if (IS_BIT(board->wRook, x) || IS_BIT(board->wQueen, x))
                            {
                                temp = true;
                                break;
                            }
                        }
                        if (!temp)
                            protecting = false;
                    }

                    
                }

                // Diagonal Right
                else if ((i - 1 - bKingPos) % 9 == 0)
                {
                    if (i - 1 > bKingPos)
                    {
                        for (int8_t x = bKingPos + 9; x < i - 1; x += 9)
                        {
                            if (IS_BIT(board->wPosition, x))
                            {
                                protecting = false;
                                break;
                            }
                        }
                        bool temp = false;
                        for (int8_t x = i + 8; x < 64; x += 9)
                        {
                            // If the attacker is right above
                            if (IS_BIT(board->wBishop, x) || IS_BIT(board->wQueen, x))
                            {
                                temp = true;
                            }
                        }
                        if (!temp)
                            protecting = false;
                    }
                    else
                    {
                        for (int8_t x = bKingPos - 9; x > i - 1; x -= 9)
                        {
                            if (IS_BIT(board->wPosition, x))
                            {
                                protecting = false;
                                break;
                            }
                        }
                        bool temp = false;
                        for (int8_t x = i - 10; x >= 0; x -= 9)
                        {
                            // If the attacker is right above
                            if (IS_BIT(board->wBishop, x) || IS_BIT(board->wQueen, x))
                            {
                                temp = true;
                                break;
                            }
                        }
                        if (!temp)
                            protecting = false;
                    }
                }

                // Diagonal Left
                else if ((i - 1 - bKingPos) % 7 == 0)
                {
                    if (i - 1 > bKingPos)
                    {
                        for (int8_t x = bKingPos + 7; x < i - 1; x += 7)
                        {
                            if (IS_BIT(board->wPosition, x))
                            {
                                protecting = false;
                                break;
                            }
                        }
                        bool temp = false;
                        for (int8_t x = i + 6; x < 64; x += 7)
                        {
                            // If the attacker is right above
                            if (IS_BIT(board->wBishop, x) || IS_BIT(board->wQueen, x))
                            {
                                temp = true;
                            }
                        }
                        if (!temp)
                            protecting = false;
                    }
                    else
                    {
                        for (int8_t x = bKingPos - 7; x > i - 1; x -= 7)
                        {
                            if (IS_BIT(board->wPosition, x))
                            {
                                protecting = false;
                                break;
                            }
                        }
                        bool temp = false;
                        for (int8_t x = i - 8; x >= 0; x -= 7)
                        {
                            // If the attacker is right above
                            if (IS_BIT(board->wBishop, x) || IS_BIT(board->wQueen, x))
                            {
                                temp = true;
                            }
                        }
                        if (!temp)
                            protecting = false;
                    }
                }

                else{
                    protecting = false;
                }

                if (protecting)
                {
                    printf("error found here maybe\n");
                    board->bMoveMap[i][0] = 0ULL;
                    board->bMoveMap[i][1] = 0ULL;
                }
            }
            
            board->bMoveMap[0][0] |= board->bMoveMap[i][0] | board->bMoveMap[i][1]; // Add back moves after removing
        }
    }

    // printf("White's Move Map: \n");
    // for (int rank = 7; rank >= 0; rank--)
    // {
    //     for (int file = 0; file < 8; file++)
    //     {
    //         int square = rank * 8 + file;
    //         uint64_t mask = 1ULL << square;
    //         printf("%c ", (board->wMoveMap[0][0] & mask) ? '1' : '.');
    //     }
    //     printf("\n");
    // }
    // printf("Black's Move Map: \n");
    // for (int rank = 7; rank >= 0; rank--)
    // {
    //     for (int file = 0; file < 8; file++)
    //     {
    //         int square = rank * 8 + file;
    //         uint64_t mask = 1ULL << square;
    //         printf("%c ", (board->bMoveMap[56][0] & mask) ? '1' : '.');
    //     }
    //     printf("\n");
    // }
}

void CheckmateChecker(BitBoard *board){
    if ((board->wCheckMap != 0 && board->wMoveMap[0][0] == 0) || 
        (board->bCheckMap != 0 && board->bMoveMap[0][0] == 0))
        board->GameContinue = false;
}

void GameStateUpdater(BitBoard *board){
    if(board->SelectPromotion){
        return;
    }
    board->playerTurn = !board->playerTurn;
    GenerateMoveMap(board);
    GenerateCheckMap(board);
    ClearIllegalMoves(board);
    CheckmateChecker(board);
}

#pragma endregion