#include <raylib.h>
#include <rlgl.h>
#include <gameboard.h>
#include <bitboard.h>


int main(){
#pragma region EarlyInit
    // ------------------ BOARD ------------------
    GameBoard board = CreateGameBoard(1920, 1080);
    InitWindow(board.windowWidth, board.windowHeight, "Chess Game");
    SetTargetFPS(60);
    Assets assets = InitializeAsset("../assets/chess_green/");
    // ------------------ BOARD ------------------

    // ------------------ COLOR ------------------
    Color GreenBoard = {5, 113, 55, 255};
    // ------------------ COLOR ------------------

    // ------------------ CHESS ------------------
    BitBoard bitboard = InitBoard();

    // char posBoard[65] = {
    //     '.', '.', '.', '.', '.', '.', '.', '.',
    //     '.', '.', '.', '.', '.', '.', '.', '.',
    //     '.', '.', '.', '.', '.', '.', '.', '.',
    //     '.', '.', '.', '.', '.', '.', '.', '.',
    //     '.', '.', '.', '.', '.', '.', '.', '.',
    //     '.', '.', '.', '.', '.', '.', '.', '.',
    //     '.', '.', '.', '.', '.', '.', '.', '.',
    //     '.', '.', '.', '.', '.', '.', '.', '.'};
    // GetCurrentPos(&bitboard, posBoard);
    // ------------------ CHESS ------------------

    // ------------------ MOUSE ------------------
    Vector2 mouseOffset = {0, 0};
    
    int8_t curCell = -1; 
    char curPiece = '.';
    int8_t lastPiecePos = -1;
    uint64_t curPiecePosMoveset[2] = {0ULL, 0ULL};
    // ------------------ MOUSE ------------------

#pragma endregion
    
#pragma region GameLoop

    while (!WindowShouldClose())
    {
        // --------------------------------------------
        // ------------------ EVENTS ------------------
        // --------------------------------------------
        Vector2 mousePos = GetMousePosition();
        
        // Handle mouse click
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){   
            for (int8_t i = 0; i < 64; i++){
                if(CheckCollisionPointRec(mousePos, board.Grid[i])){
                    if ((bitboard.playerTurn && IS_BIT(bitboard.bPosition, i) 
                        && !IS_BIT(curPiecePosMoveset[1], i))
                        ||
                        (!bitboard.playerTurn && IS_BIT(bitboard.wPosition, i) 
                        && !IS_BIT(curPiecePosMoveset[1], i)))
                    {
                        break;
                    }
                    curCell = i;
                    curPiece = GetCurrentPiece(&bitboard, i);
                    printf("Current Piece: %c, Current Cell: %d\n", curPiece, curCell);
                    
                    if(IS_BIT(curPiecePosMoveset[0], i)){
                        PieceMove(&bitboard, lastPiecePos, i);
                        bitboard.playerTurn = !bitboard.playerTurn;
                    }
                    else if(IS_BIT(curPiecePosMoveset[1], i))
                    {
                        PieceCapture(&bitboard, lastPiecePos, i);
                        bitboard.playerTurn = !bitboard.playerTurn;
                    }

                    if(curPiece == '.'){
                        curPiecePosMoveset[0] = 0ULL;
                        curPiecePosMoveset[1] = 0ULL;
                    }
                    else{
                        lastPiecePos = i;
                        GetPossibleMoves(&bitboard, i, curPiecePosMoveset, curPiece);
                    }

                    break;
                }
            }
        }


        // --------------------------------------------
        // ------------------ UPDATE ------------------
        // --------------------------------------------

        
        
        // --------------------------------------------
        // ------------------  DRAW  ------------------
        // --------------------------------------------

        BeginDrawing();

        // ------------------  BASE  ------------------
        ClearBackground(GreenBoard);
        DrawTextureEx(assets.boardTexture, (Vector2){board.CenterX - (assets.boardTexture.width * 2), 
                      board.CenterY - (assets.boardTexture.height * 2)}, .0f, 4.f, WHITE);
        DrawTextureEx(assets.alphabetTexture, (Vector2){board.CenterX - 322, board.CenterY + 450}, .0f, 4.f, WHITE);
        DrawTextureEx(assets.numbersTexture, (Vector2){board.CenterX - 410, board.CenterY - 330}, .0f, 4.f, WHITE);
        // ------------------  BASE  ------------------

        // ------------------ PIECES ------------------
        DrawMouseClick(&board, curCell, curPiece, curPiecePosMoveset, BEIGE);
        DrawChessBoard(&bitboard, &assets, &board);
        // ------------------ PIECES ------------------

        EndDrawing();
    }

#pragma endregion

    CloseWindow();
}
