#include <raylib.h>
#include <rlgl.h>
#include <gameboard.h>
#include <bitboard.h>

int main(){
#pragma region EarlyInit
    // ------------------ BOARD ------------------
    GameBoard board = CreateGameBoard(1920, 1080, 100, 100);
    InitWindow(board.windowWidth, board.windowHeight, "Chess Game");
    SetTargetFPS(60);
    Assets assets = InitializeAsset("../assets/chess_green/");
    // ------------------ BOARD ------------------

    // ------------------ COLOR ------------------
    Color GreenBoard = {5, 113, 55, 255};
    // ------------------ COLOR ------------------

    // ------------------ CHESS ------------------
    BitBoard bitboard = InitBoard();
    char posBoard[65] = {
        '.', '.', '.', '.', '.', '.', '.', '.',
        '.', '.', '.', '.', '.', '.', '.', '.',
        '.', '.', '.', '.', '.', '.', '.', '.',
        '.', '.', '.', '.', '.', '.', '.', '.',
        '.', '.', '.', '.', '.', '.', '.', '.',
        '.', '.', '.', '.', '.', '.', '.', '.',
        '.', '.', '.', '.', '.', '.', '.', '.',
        '.', '.', '.', '.', '.', '.', '.', '.'};
    GetCurrentPos(&bitboard, posBoard);
    // ------------------ CHESS ------------------

#pragma endregion
    
#pragma region GameLoop

    while (!WindowShouldClose())
    {
        // --------------------------------------------
        // ------------------ EVENTS ------------------
        // --------------------------------------------


        
        // --------------------------------------------
        // ------------------ UPDATE ------------------
        // --------------------------------------------

        
        
        // --------------------------------------------
        // ------------------  DRAW  ------------------
        // --------------------------------------------

        BeginDrawing();

        ClearBackground(GreenBoard);
        DrawTextureEx(assets.boardTexture, (Vector2){board.CenterX - (assets.boardTexture.width * 2), 
                      board.CenterY - (assets.boardTexture.height * 2)}, .0f, 4.f, WHITE);
        DrawTextureEx(assets.alphabetTexture, (Vector2){board.CenterX - 322, board.CenterY + 450}, .0f, 4.f, WHITE);
        DrawTextureEx(assets.numbersTexture, (Vector2){board.CenterX - 410, board.CenterY - 330}, .0f, 4.f, WHITE);
        DrawChessBoard(&bitboard, &assets, &board);

        EndDrawing();
    }

#pragma endregion

    CloseWindow();
}
