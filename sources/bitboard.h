#pragma once
#ifndef BITBOARD_H
#define BITBOARD_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

/*
############################################## BITBOARD REPRESENTATION ##############################################

WHITE
             PAWN                ROOK              KNIGHT              BISHOP               QUEEN                KING
8 0 0 0 0 0 0 0 0   8 0 0 0 0 0 0 0 0   8 0 0 0 0 0 0 0 0   8 0 0 0 0 0 0 0 0   8 0 0 0 0 0 0 0 0   8 0 0 0 0 0 0 0 0
7 0 0 0 0 0 0 0 0   7 0 0 0 0 0 0 0 0   7 0 0 0 0 0 0 0 0   7 0 0 0 0 0 0 0 0   7 0 0 0 0 0 0 0 0   7 0 0 0 0 0 0 0 0
6 0 0 0 0 0 0 0 0   6 0 0 0 0 0 0 0 0   6 0 0 0 0 0 0 0 0   6 0 0 0 0 0 0 0 0   6 0 0 0 0 0 0 0 0   6 0 0 0 0 0 0 0 0
5 0 0 0 0 0 0 0 0   5 0 0 0 0 0 0 0 0   5 0 0 0 0 0 0 0 0   5 0 0 0 0 0 0 0 0   5 0 0 0 0 0 0 0 0   5 0 0 0 0 0 0 0 0
4 0 0 0 0 0 0 0 0   4 0 0 0 0 0 0 0 0   4 0 0 0 0 0 0 0 0   4 0 0 0 0 0 0 0 0   4 0 0 0 0 0 0 0 0   4 0 0 0 0 0 0 0 0
3 0 0 0 0 0 0 0 0   3 0 0 0 0 0 0 0 0   3 0 0 0 0 0 0 0 0   3 0 0 0 0 0 0 0 0   3 0 0 0 0 0 0 0 0   3 0 0 0 0 0 0 0 0
2 1 1 1 1 1 1 1 1   2 0 0 0 0 0 0 0 0   2 0 0 0 0 0 0 0 0   2 0 0 0 0 0 0 0 0   2 0 0 0 0 0 0 0 0   2 0 0 0 0 0 0 0 0
1 0 0 0 0 0 0 0 0   1 1 0 0 0 0 0 0 1   1 0 1 0 0 0 0 1 0   1 0 0 1 0 0 1 0 0   1 0 0 0 1 0 0 0 0   1 0 0 0 0 1 0 0 0
  a b c d e f g h     a b c d e f g h     a b c d e f g h     a b c d e f g h     a b c d e f g h     a b c d e f g h

BLACK
             PAWN                ROOK              KNIGHT              BISHOP               QUEEN                KING
8 0 0 0 0 0 0 0 0   8 1 0 0 0 0 0 0 1   8 0 1 0 0 0 0 1 0   8 0 0 1 0 0 1 0 0   8 0 0 0 1 0 0 0 0   8 0 0 0 0 1 0 0 0
7 1 1 1 1 1 1 1 1   7 0 0 0 0 0 0 0 0   7 0 0 0 0 0 0 0 0   7 0 0 0 0 0 0 0 0   7 0 0 0 0 0 0 0 0   7 0 0 0 0 0 0 0 0
6 0 0 0 0 0 0 0 0   6 0 0 0 0 0 0 0 0   6 0 0 0 0 0 0 0 0   6 0 0 0 0 0 0 0 0   6 0 0 0 0 0 0 0 0   6 0 0 0 0 0 0 0 0
5 0 0 0 0 0 0 0 0   5 0 0 0 0 0 0 0 0   5 0 0 0 0 0 0 0 0   5 0 0 0 0 0 0 0 0   5 0 0 0 0 0 0 0 0   5 0 0 0 0 0 0 0 0
4 0 0 0 0 0 0 0 0   4 0 0 0 0 0 0 0 0   4 0 0 0 0 0 0 0 0   4 0 0 0 0 0 0 0 0   4 0 0 0 0 0 0 0 0   4 0 0 0 0 0 0 0 0
3 0 0 0 0 0 0 0 0   3 0 0 0 0 0 0 0 0   3 0 0 0 0 0 0 0 0   3 0 0 0 0 0 0 0 0   3 0 0 0 0 0 0 0 0   3 0 0 0 0 0 0 0 0
2 0 0 0 0 0 0 0 0   2 0 0 0 0 0 0 0 0   2 0 0 0 0 0 0 0 0   2 0 0 0 0 0 0 0 0   2 0 0 0 0 0 0 0 0   2 0 0 0 0 0 0 0 0
1 0 0 0 0 0 0 0 0   1 0 0 0 0 0 0 0 0   1 0 0 0 0 0 0 0 0   1 0 0 0 0 0 0 0 0   1 0 0 0 0 0 0 0 0   1 0 0 0 0 0 0 0 0
  a b c d e f g h     a b c d e f g h     a b c d e f g h     a b c d e f g h     a b c d e f g h     a b c d e f g h

############################################## BITBOARD REPRESENTATION ##############################################
*/

#define SET_BIT(bb, square) ((bb) |= (1ULL << (square)))
#define CLEAR_BIT(bb, square) ((bb) &= ~(1ULL << (square)))
#define FLIP_BIT(bb, square) ((bb) ^= (1ULL << (square)))
#define IS_BIT(bb, square) ((bb) & (1ULL << (square)))
#define AND_BIT(b1, b2) ((b1) & (b2))
#define LESS_THAN_BIT(b1, b2, square) ((b1) & (b2 >> square))
#define MORE_THAN_BIT(b1, b2, square) ((b1) & (b2 << square))

#define wPawn wPieces[1]
#define wRook wPieces[2]
#define wKnight wPieces[3]
#define wBishop wPieces[4]
#define wQueen wPieces[5]
#define wKing wPieces[6]
#define wPosition wPieces[0]

#define bPawn bPieces[1]
#define bRook bPieces[2]
#define bKnight bPieces[3]
#define bBishop bPieces[4]
#define bQueen bPieces[5]
#define bKing bPieces[6]
#define bPosition bPieces[0]

typedef struct BitBoard
{
    // White
    uint64_t wPieces[7];
    uint64_t wMoveMap[65][2];
    // Black
    uint64_t bPieces[7];
    uint64_t bMoveMap[65][2];
    // Castle
    bool wCastled;
    bool bCastled;
    // Promotion
    bool SelectPromotion;
    int8_t PromotionSquare;
    char PromotionTarget;
    // En Passant
    int8_t enPassant;
    // Game State
    bool playerTurn;
    bool GameContinue;
    uint64_t wCheckMap;
    uint64_t bCheckMap;
    bool doubleCheck;
} BitBoard;

BitBoard InitBoard(); // Initialize starting positions

void GetCurrentPos(const BitBoard* board, char* posBoard);
char GetCurrentPiece(const BitBoard *board, int8_t square);
uint64_t *GetCurrentPieceEx(BitBoard *board, int8_t square);

void PiecePromotion(BitBoard *board);
void PieceMove(BitBoard* board, int8_t start, int8_t target);
void PieceCapture(BitBoard* board, int8_t start, int8_t target);
uint64_t *GetPromotionPrompt(BitBoard *board);
void GetPossibleMoves(BitBoard *board, int8_t square, bool isWhite);

void GenerateMoveMap(BitBoard *board);
void GenerateCheckMap(BitBoard *board);
void CheckmateChecker(BitBoard *board);
void GameStateUpdater(BitBoard *board);
#endif