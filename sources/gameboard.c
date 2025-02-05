#include <gameboard.h>
#include <stdlib.h>
#include <stdio.h>

GameBoard CreateGameBoard(int WindowWidth, int WindowHeight, int GridWidth, int GridHeight)
{
    GameBoard board;

    board.windowWidth  =  WindowWidth;
    board.windowHeight =  WindowHeight;
    board.gridWidth    =  GridWidth;
    board.gridHeight   =  GridHeight;
    board.CenterX      =  WindowWidth / 2;
    board.CenterY      =  WindowHeight / 2;
    board.OriginX      =  (board.CenterX) - (4 * GridWidth);
    board.EndX         =  (board.CenterX) + (4 * GridWidth);
    board.OriginY      =  (board.CenterY) - (4 * GridHeight);
    board.EndY         =  (board.CenterY) + (4 * GridHeight);
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

void DrawChessBoard(const BitBoard *bitboard, const Assets *assets, const GameBoard *board){
    int i = 0;
    for (int y = board->CenterY + 264; y >= board->CenterY - 352; y -= 88)
    {
        for (int x = board->CenterX - 352; x < board->CenterX + 352; x += 88)
        {   
            Texture2D texture;
            if (IS_BIT(bitboard->wPosition, i)){
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
            else if (IS_BIT(bitboard->bPosition, i)){
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
            else{
                i++;
                continue;
            }
            DrawTextureEx(texture, (Vector2){x + 44 - (float)(texture.width * 2), 
                          y + 44 - (float)(texture.height * 2)}, 0.f, 4.f, WHITE);
            i++;
        }
    }
}