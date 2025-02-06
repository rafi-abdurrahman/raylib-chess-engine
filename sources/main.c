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
    Vector2 textureDragPos = {0, 0};

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
            bool insideGrid = false;
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
                    textureDragPos = (Vector2){board.Grid[i].x, board.Grid[i].y};
                    insideGrid = true;
                    mouseOffset.x = mousePos.x - board.Grid[i].x;
                    mouseOffset.y = mousePos.y - board.Grid[i].y;
                    curCell = i;
                    curPiece = GetCurrentPiece(&bitboard, i);
                    printf("Current Piece: %c, Current Cell: %d\n", curPiece, curCell);
                    break;
                }
            }
            if(!insideGrid){
                curCell = -1;
            }
        }
        else if(IsMouseButtonDown(MOUSE_BUTTON_LEFT) && curCell > -1){
            // printf("Im being pressed!\n");

            textureDragPos.x = mousePos.x - mouseOffset.x;
            textureDragPos.y = mousePos.y - mouseOffset.y;
        }
        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && curCell > -1)
        {
            for (int8_t i = 0; i < 64; i++){
                if(CheckCollisionPointRec(mousePos, board.Grid[i])){
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
                    break;
                }
                }
            }
        }

        // --------------------------------------------
        // ------------------ UPDATE ------------------
        // --------------------------------------------

        if(curCell >-1){
            if (IS_BIT(curPiecePosMoveset[0], curCell))
            {
                PieceMove(&bitboard, lastPiecePos, curCell);
                curPiecePosMoveset[0] = 0ULL;
                curPiecePosMoveset[1] = 0ULL;
                bitboard.playerTurn = !bitboard.playerTurn;
                curCell = -1;
            }
            else if (IS_BIT(curPiecePosMoveset[1], curCell))
            {
                PieceCapture(&bitboard, lastPiecePos, curCell);
                curPiecePosMoveset[0] = 0ULL;
                curPiecePosMoveset[1] = 0ULL;
                bitboard.playerTurn = !bitboard.playerTurn;
                curCell = -1;
            }
            else if (!IS_BIT(bitboard.wPosition, curCell) && !IS_BIT(bitboard.bPosition, curCell))
            {
                curPiecePosMoveset[0] = 0ULL;
                curPiecePosMoveset[1] = 0ULL;
            }
            else
            {
                lastPiecePos = curCell;
                GetPossibleMoves(&bitboard, curCell, curPiecePosMoveset);
            }

        }

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
        DrawChessBoard(&bitboard, &assets, &board, (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && curCell > -1), curCell, textureDragPos);
        // ------------------ PIECES ------------------

        EndDrawing();
    }

#pragma endregion

    CloseWindow();
}
