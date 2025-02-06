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

enum BoardSquare{a1, b1, c1, d1, e1, f1, g1, h1,
                 a2, b2, c2, d2, e2, f2, g2, h2,
                 a3, b3, c3, d3, e3, f3, g3, h3,
                 a4, b4, c4, d4, e4, f4, g4, h4,
                 a5, b5, c5, d5, e5, f5, g5, h5,
                 a6, b6, c6, d6, e6, f6, g6, h6,
                 a7, b7, c7, d7, e7, f7, g7, h7,
                 a8, b8, c8, d8, e8, f8, g8, h8};

#define SET_BIT(bb, square) ((bb) |= (1ULL << (square)))
#define CLEAR_BIT(bb, square) ((bb) &= ~(1ULL << (square)))
#define FLIP_BIT(bb, square) ((bb) ^= (1ULL << (square)))
#define IS_BIT(bb, square) ((bb) & (1ULL << (square)))

typedef struct BitBoard
{
    // White
    uint64_t wPawn;
    uint64_t wRook;
    uint64_t wKnight;
    uint64_t wBishop;
    uint64_t wQueen;
    uint64_t wKing;
    uint64_t wPosition;
    // Black
    uint64_t bPawn;
    uint64_t bRook;
    uint64_t bKnight;
    uint64_t bBishop;
    uint64_t bQueen;
    uint64_t bKing;
    uint64_t bPosition;
    // Castle
    bool wCastled;
    bool bCastled;
    // En Passant
    int8_t enPassant;
    // Game State
    bool playerTurn;
    bool wInCheck;
    bool bInCheck;
} BitBoard;

BitBoard InitBoard(); // Initialize starting positions

void GetCurrentPos(const BitBoard* board, char* posBoard);
char GetCurrentPiece(const BitBoard *board, int8_t square);
uint64_t *GetCurrentPieceEx(BitBoard *board, int8_t square);

void PieceMove(BitBoard* board, int8_t start, int8_t target);
void PieceCapture(BitBoard* board, int8_t start, int8_t target);

void GetPossibleMoves(BitBoard *board, int8_t square, uint64_t *possiblePos, char Piece);
void GetPossibleCaptures(BitBoard *board, int8_t square, uint64_t *possiblePos, char Piece);

#endif