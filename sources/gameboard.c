#include <gameboard.h>
#include <stdlib.h>
#include <stdio.h>

GameBoard CreateGameBoard(int WindowWidth, int WindowHeight)
{
    GameBoard board;

    board.windowWidth  =  WindowWidth;
    board.windowHeight =  WindowHeight;
    board.CenterX      =  WindowWidth / 2;
    board.CenterY      =  WindowHeight / 2;

    int i = 0;
    for (int y = board.CenterY + 264; y >= board.CenterY - 352; y -= 88)
    {
        for (int x = board.CenterX - 352; x < board.CenterX + 352; x += 88)
        {
            board.Grid[i] = (Rectangle){x, y, 88, 88};
            i++;
        }
    }

    return board;
}

char *ConcatString(char* folder, char* path){
    size_t length = snprintf(NULL, 0, "%s%s", folder, path) + 1;
    char *FullPath = (char *)malloc(length);
    if (FullPath)
    {
        snprintf(FullPath, length, "%s%s", folder, path);
        return FullPath;
    }
    else
    {
        printf("Error! Failed to open '%s' !\n", path);
        exit(EXIT_FAILURE);
    }
}

void LoadTextureFromPath(char *folder, char *path, Texture *texture)
{
    char *fullPath = ConcatString(folder, path);
    Image img = LoadImage(fullPath);
    *texture = LoadTextureFromImage(img);
    UnloadImage(img);
    free(fullPath);
}   

Assets InitializeAsset(char* folder){
    Assets assets;

    char *boardPath       = "board_alt.png";
    char *alphabetPath    = "text.png";
    char *numbersPath     = "numbers.png";

    char *wPawnPath       = "white_pawn.png";
    char *wRookPath       = "white_rook.png";
    char *wKnightPath     = "white_knight.png";
    char *wBishopPath     = "white_bishop.png";
    char *wQueenPath      = "white_queen.png";
    char *wKingPath       = "white_king.png";

    char *bPawnPath       = "black_pawn.png";
    char *bRookPath       = "black_rook.png";
    char *bKnightPath     = "black_knight.png";
    char *bBishopPath     = "black_bishop.png";
    char *bQueenPath      = "black_queen.png";
    char *bKingPath       = "black_king.png";

    LoadTextureFromPath(folder, boardPath, &assets.boardTexture);
    LoadTextureFromPath(folder, alphabetPath, &assets.alphabetTexture);
    LoadTextureFromPath(folder, numbersPath, &assets.numbersTexture);

    LoadTextureFromPath(folder, wPawnPath, &assets.wPawnTexture);
    LoadTextureFromPath(folder, wRookPath, &assets.wRookTexture);
    LoadTextureFromPath(folder, wKnightPath, &assets.wKnightTexture);
    LoadTextureFromPath(folder, wBishopPath, &assets.wBishopTexture);
    LoadTextureFromPath(folder, wQueenPath, &assets.wQueenTexture);
    LoadTextureFromPath(folder, wKingPath, &assets.wKingTexture);

    LoadTextureFromPath(folder, bPawnPath, &assets.bPawnTexture);
    LoadTextureFromPath(folder, bRookPath, &assets.bRookTexture);
    LoadTextureFromPath(folder, bKnightPath, &assets.bKnightTexture);
    LoadTextureFromPath(folder, bBishopPath, &assets.bBishopTexture);
    LoadTextureFromPath(folder, bQueenPath, &assets.bQueenTexture);
    LoadTextureFromPath(folder, bKingPath, &assets.bKingTexture);

    return assets;
}

void DrawChessBoard(const BitBoard *bitboard, const Assets *assets, const GameBoard *board,
                    bool Drag, int8_t draggedPiece, Vector2 draggedPiecePos)
{
    if(bitboard->GameContinue){
        if(bitboard->bCheckMap != 0 || bitboard->bCheckMap != 0){
            DrawText(bitboard->playerTurn ? "White is in check!" : "Black is in check!",
                     board->CenterX - (assets->boardTexture.width * 2),
                     board->CenterY - (assets->boardTexture.height * 2),
                     40, RED);
        }
        else{
            DrawText(bitboard->playerTurn ? "White's Turn" : "Black's Turn",
                    board->CenterX - (assets->boardTexture.width * 2),
                    board->CenterY - (assets->boardTexture.height * 2),
                    40, bitboard->playerTurn ? WHITE : BLACK);
        }
    }
    else{
        DrawText(!bitboard->playerTurn ? "White won!" : "Black won!" ,
                 board->CenterX - (assets->boardTexture.width * 2),
                 board->CenterY - (assets->boardTexture.height * 2),
                 40, bitboard->playerTurn ? WHITE : BLACK);
    }

    for (uint8_t i = 0; i < 64; i++)
    {
        if(Drag && i == draggedPiece)
            continue;
        Texture2D texture;
        if (IS_BIT(bitboard->wPosition, i))
        {
            if (IS_BIT(bitboard->wPawn, i))
                texture = assets->wPawnTexture;
            else if (IS_BIT(bitboard->wRook, i))
                texture = assets->wRookTexture;
            else if (IS_BIT(bitboard->wBishop, i))
                texture = assets->wBishopTexture;
            else if (IS_BIT(bitboard->wKnight, i))
                texture = assets->wKnightTexture;
            else if (IS_BIT(bitboard->wQueen, i))
                texture = assets->wQueenTexture;
            else if (IS_BIT(bitboard->wKing, i))
                texture = assets->wKingTexture;
        }
        // Check Black BitBoards
        else if (IS_BIT(bitboard->bPosition, i))
        {
            if (IS_BIT(bitboard->bPawn, i))
                texture = assets->bPawnTexture;
            else if (IS_BIT(bitboard->bRook, i))
                texture = assets->bRookTexture;
            else if (IS_BIT(bitboard->bBishop, i))
                texture = assets->bBishopTexture;
            else if (IS_BIT(bitboard->bKnight, i))
                texture = assets->bKnightTexture;
            else if (IS_BIT(bitboard->bQueen, i))
                texture = assets->bQueenTexture;
            else if (IS_BIT(bitboard->bKing, i))
                texture = assets->bKingTexture;
        }
        else
        {
            continue;
        }
        DrawTextureEx(texture, (Vector2){board->Grid[i].x + 44 - (float)(texture.width * 2), 
                      board->Grid[i].y + 44 - (float)(texture.height * 2)}, 0.f, 4.f, WHITE);
    }

    if (Drag)
    {
        Texture2D texture;
        if (IS_BIT(bitboard->wPosition, draggedPiece))
        {
            if (IS_BIT(bitboard->wPawn, draggedPiece))
                texture = assets->wPawnTexture;
            else if (IS_BIT(bitboard->wRook, draggedPiece))
                texture = assets->wRookTexture;
            else if (IS_BIT(bitboard->wBishop, draggedPiece))
                texture = assets->wBishopTexture;
            else if (IS_BIT(bitboard->wKnight, draggedPiece))
                texture = assets->wKnightTexture;
            else if (IS_BIT(bitboard->wQueen, draggedPiece))
                texture = assets->wQueenTexture;
            else if (IS_BIT(bitboard->wKing, draggedPiece))
                texture = assets->wKingTexture;
        }
        // Check Black BitBoards
        else if (IS_BIT(bitboard->bPosition, draggedPiece))
        {
            if (IS_BIT(bitboard->bPawn, draggedPiece))
                texture = assets->bPawnTexture;
            else if (IS_BIT(bitboard->bRook, draggedPiece))
                texture = assets->bRookTexture;
            else if (IS_BIT(bitboard->bBishop, draggedPiece))
                texture = assets->bBishopTexture;
            else if (IS_BIT(bitboard->bKnight, draggedPiece))
                texture = assets->bKnightTexture;
            else if (IS_BIT(bitboard->bQueen, draggedPiece))
                texture = assets->bQueenTexture;
            else if (IS_BIT(bitboard->bKing, draggedPiece))
                texture = assets->bKingTexture;
        }
        DrawTextureEx(texture, draggedPiecePos, .0f, 4.f, WHITE);
    }
    
    return;
}

void DrawPossibleMoves(const GameBoard *board, const uint64_t posMoves, Color color)
{
    for (uint8_t i = 0; i < 64; i++){
        if (IS_BIT(posMoves, i))
            DrawRectangleRec(board->Grid[i], color);
    }
}

void DrawPossibleCaptures(const GameBoard *board, const uint64_t posCaptures, Color color)  
{
    for (uint8_t i = 0; i < 64; i++)
    {
        if (IS_BIT(posCaptures, i))
            DrawRectangleRec(board->Grid[i], color);
    }
}

void DrawMouseClick(const GameBoard *board, const BitBoard *bitboard, int8_t Cell, Color highlight){
    if(Cell > -1){
        DrawRectangleRec(board->Grid[Cell], highlight);
        if(bitboard->playerTurn){
            DrawPossibleMoves(board, bitboard->wMoveMap[Cell+1][0], WHITE);
            DrawPossibleCaptures(board, bitboard->wMoveMap[Cell+1][1], RED);
        }
        else{
            DrawPossibleMoves(board, bitboard->bMoveMap[Cell + 1][0], WHITE);
            DrawPossibleCaptures(board, bitboard->bMoveMap[Cell + 1][1], RED);
        }

    }
}

void DrawPromotionSelectionGrid(GameBoard *board, BitBoard *bitboard){
    // White promotion
    if(bitboard->PromotionSquare > 55){
        board->PromotionSelectionGrid[0] = (Rectangle){board->Grid[bitboard->PromotionSquare].x - 66,
                                                       board->Grid[bitboard->PromotionSquare].y - 44, 44, 44};
        DrawRectangleRec(board->PromotionSelectionGrid[0], YELLOW);
        for (int8_t i = 1; i < 5; i++){
            board->PromotionSelectionGrid[i] = (Rectangle){board->PromotionSelectionGrid[i-1].x + 44,
                                                           board->PromotionSelectionGrid[i-1].y, 44, 44};
            DrawRectangleRec(board->PromotionSelectionGrid[i], YELLOW);
        }
    }
    else if(bitboard->PromotionSquare < 8){
        board->PromotionSelectionGrid[0] = (Rectangle){board->Grid[bitboard->PromotionSquare].x + 66,
                                                       board->Grid[bitboard->PromotionSquare].y + 44, 44, 44};
        DrawRectangleRec(board->PromotionSelectionGrid[0], YELLOW);
        for (int8_t i = 1; i <= 5; i++)
        {
            board->PromotionSelectionGrid[i] = (Rectangle){board->PromotionSelectionGrid[i - 1].x + 44,
                                                           board->PromotionSelectionGrid[i - 1].y, 44, 44};
            DrawRectangleRec(board->PromotionSelectionGrid[i], YELLOW);
        }
    }


}