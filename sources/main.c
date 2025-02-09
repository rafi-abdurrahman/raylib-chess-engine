#include <raylib.h>
#include <rlgl.h>
#include <raymath.h>
#include <gameboard.h>
#include <bitboard.h>


int main(){
#pragma region EarlyInit
    // ------------------ BOARD ------------------
    GameBoard board = CreateGameBoard(1920, 1080);
    InitWindow(board.windowWidth, board.windowHeight, "Chess Game");
    SetTargetFPS(60);
    Assets assets = InitializeAsset("assets/chess_green/");
    // ------------------ BOARD ------------------

    // ------------------ COLOR ------------------
    Color GreenBoard = {5, 113, 55, 255};
    // ------------------ COLOR ------------------

    // ------------------ CHESS ------------------
    BitBoard bitboard = InitBoard();
    GameStateUpdater(&bitboard);
    // ------------------ CHESS ------------------

    // ------------------ MOUSE ------------------
    Vector2 textureDragPos;
    Vector2 mouseOffset;

    int8_t curCell = -1; 
    char curPiece = '.';
    int8_t lastPiecePos = -1;
    // ------------------ MOUSE ------------------

#pragma endregion
    
#pragma region GameLoop

    while (!WindowShouldClose())
    {
        // --------------------------------------------
        // ------------------ EVENTS ------------------
        // --------------------------------------------
        board.mousePos = GetMousePosition();

        // ------------------ MOUSE -------------------
        if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
            if(bitboard.SelectPromotion){
                for (int8_t i = 0; i < 5; i++){
                    if(CheckCollisionPointRec(board.mousePos, board.PromotionSelectionGrid[i])){
                        if(bitboard.playerTurn){
                            if (i == 0)
                                bitboard.PromotionTarget = 'P';
                            if (i == 1)
                                bitboard.PromotionTarget = 'N';
                            if (i == 2)
                                bitboard.PromotionTarget = 'B';
                            if (i == 3)
                                bitboard.PromotionTarget = 'R';
                            if (i == 4)
                                bitboard.PromotionTarget = 'Q';
                        }
                        else{
                            if (i == 0)
                                bitboard.PromotionTarget = 'p';
                            if (i == 1)
                                bitboard.PromotionTarget = 'n';
                            if (i == 2)
                                bitboard.PromotionTarget = 'b';
                            if (i == 3)
                                bitboard.PromotionTarget = 'r';
                            if (i == 4)
                                bitboard.PromotionTarget = 'q';
                        }
                        break;
                    }
                }
            }
            bool insideGrid = false;
            for (int8_t i = 0; i < 64; i++){
                if(CheckCollisionPointRec(board.mousePos, board.Grid[i])){
                    if ((bitboard.playerTurn && IS_BIT(bitboard.bPosition, i) 
                        && !IS_BIT(bitboard.wMoveMap[lastPiecePos+1][1], i) && lastPiecePos > -1)
                        ||
                        (!bitboard.playerTurn && IS_BIT(bitboard.wPosition, i) 
                        && !IS_BIT(bitboard.bMoveMap[lastPiecePos+1][1], i))&& lastPiecePos > -1)
                    {
                        break;
                    }

                    insideGrid = true;

                    textureDragPos = (Vector2){board.Grid[i].x, board.Grid[i].y};
                    mouseOffset.x = board.mousePos.x - board.Grid[i].x;
                    mouseOffset.y = board.mousePos.y - board.Grid[i].y;

                    lastPiecePos = curCell;
                    curCell = i;
                    curPiece = GetCurrentPiece(&bitboard, i);
                    // printf("Current Piece: %c, Current Cell: %d, Last Cell: %d\n", curPiece, curCell, lastPiecePos);
                    break;
                }
            }
            if(!insideGrid){
                curCell = -1;
            }
        }
        
        else if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && curCell > -1)
        {
            textureDragPos.x = board.mousePos.x - mouseOffset.x;
            textureDragPos.y = board.mousePos.y - mouseOffset.y;
        }

        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT) && curCell > -1)
        {
            for (int8_t i = 0; i < 64; i++){
                if(CheckCollisionPointRec(board.mousePos, board.Grid[i])){
                    if ((bitboard.playerTurn && IS_BIT(bitboard.bPosition, i) 
                        && !IS_BIT(bitboard.wMoveMap[lastPiecePos+1][1], i) && lastPiecePos > -1)
                        ||
                        (!bitboard.playerTurn && IS_BIT(bitboard.wPosition, i) 
                        && !IS_BIT(bitboard.bMoveMap[lastPiecePos+1][1], i) && lastPiecePos > -1))
                    {
                        break;
                    }
                    lastPiecePos = curCell;
                    curCell = i;
                    curPiece = GetCurrentPiece(&bitboard, i);
                    // printf("Current Piece: %c, Current Cell: %d, Last Cell: %d\n", curPiece, curCell, lastPiecePos);
                    break;
                }
            }
        }
        // ------------------ MOUSE -------------------

        // --------------------------------------------
        // ------------------ UPDATE ------------------
        // --------------------------------------------

        // ------------------ MOVES -------------------
        if(lastPiecePos > -1){
            // printf("Processing Move from %d to %d\n", lastPiecePos, curCell);
            // If white
            if(bitboard.playerTurn){
                if(IS_BIT(bitboard.wMoveMap[lastPiecePos+1][0], curCell)){
                    PieceMove(&bitboard, lastPiecePos, curCell);
                }
                else if (IS_BIT(bitboard.wMoveMap[lastPiecePos + 1][1], curCell)){
                    PieceCapture(&bitboard, lastPiecePos, curCell);
                }
            }
            // If black
            else{
                if(IS_BIT(bitboard.bMoveMap[lastPiecePos+1][0], curCell)){
                    PieceMove(&bitboard, lastPiecePos, curCell);
                }
                else if (IS_BIT(bitboard.bMoveMap[lastPiecePos + 1][1], curCell)){
                    PieceCapture(&bitboard, lastPiecePos, curCell);
                }
            }
            lastPiecePos = curCell;
        }
        if(bitboard.SelectPromotion && bitboard.PromotionTarget != '.'){   
            PiecePromotion(&bitboard);
        }
        // ------------------ MOVES -------------------

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
        DrawMouseClick(&board, &bitboard, curCell, BEIGE);
        DrawChessBoard(&bitboard, &assets, &board, (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && curCell > -1), curCell, textureDragPos);
        if(bitboard.SelectPromotion){
            DrawPromotionSelectionGrid(&board, &bitboard, &assets);
        }
        // ------------------ PIECES ------------------

        EndDrawing();
    }

#pragma endregion

    CloseWindow();
}
