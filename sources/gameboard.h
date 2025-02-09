#pragma once

#ifndef GAMEBOARD_H
#define GAMEBOARD_H    

#include <raylib.h>
#include <bitboard.h>

typedef struct GameBoard
{
    int windowWidth;
    int windowHeight;
    int CenterX;
    int CenterY;
    Rectangle Grid[64];
    Rectangle PromotionSelectionGrid[5];
} GameBoard;

typedef struct Assets{
    // Board
    Texture2D boardTexture;
    Texture2D alphabetTexture;
    Texture2D numbersTexture;
    // White Pieces
    Texture2D wPawnTexture;
    Texture2D wRookTexture;
    Texture2D wKnightTexture;
    Texture2D wBishopTexture;
    Texture2D wQueenTexture;
    Texture2D wKingTexture;
    // Black Pieces
    Texture2D bPawnTexture;
    Texture2D bRookTexture;
    Texture2D bKnightTexture;
    Texture2D bBishopTexture;
    Texture2D bQueenTexture;
    Texture2D bKingTexture;
} Assets;

GameBoard CreateGameBoard(int WindowWidth, int WindowHeight);

Assets InitializeAsset(char *folder);
char *ConcatString(char *folder, char *path);
void LoadTextureFromPath(char *folder, char *path, Texture *texture);

void DrawChessBoard(const BitBoard *bitboard, const Assets *assets, const GameBoard *board, bool Drag, int8_t draggedPos, Vector2 draggedPiecePos);
void DrawPossibleMoves(const GameBoard *board, const uint64_t posMoves, Color color);
void DrawPossibleCaptures(const GameBoard *board, const uint64_t posCaptures, Color color);
void DrawMouseClick(const GameBoard *board, const BitBoard *bitboard, int8_t Cell, Color highlight);
void DrawPromotionSelectionGrid(GameBoard *board, BitBoard *bitboard);
#endif