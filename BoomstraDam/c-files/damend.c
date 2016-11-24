/*
  This file is Copyright (C) 1999 Harm Jetten.
  Modifications Copyright (C) 2010 Feike Boomstra.
  Distributed under the Boost Software License, Version 1.0.
  (See accompanying file LICENSE_1_0.txt or copy at
  http://www.boost.org/LICENSE_1_0.txt)
*/


#include "damdef.h"
#include "damend.h"
#include "BoomstraDam.v1.2.h"

ULONG *end_tabl;                /* the end game position table */
char mm[2500], mk[2500], km[2500], kk[2500]; /* 2-piece end game tables */
BOOL valid_2pc = FALSE;         /* 2-piece end game tables loaded */
char enddbdir[MAX_PATH] = "C:\\Endgame\\cpr\\";        /* directory of end game database files */
int  end_maxpc = 6;             /* end game maximum nr of pieces on board */
const int inv_pc[] = { 0, E, MB, MW, KB, KW };        /* inverted pieces */
const long catch_bonus[] = { 0, XBONUS, 4*XBONUS, 9*XBONUS, 16*XBONUS };
endhash *hash_end;              /* >=3-piece cache hash table */
ULONG stored_hashkey;
ULONG stored_hashval;
ULONG combi[51][8];             /* combination lookup table */
endhf *end_ref[EF*EF*EF*EF];    /* references to endgame file info */
endhf *end4x;                   /* 4-piece index file ref. */
const int pcorder[4] = { MW, KW, MB, KB }; /* piece order for db indexing */
endhf ext_set[115];             /* endgame files from external list */

//fb toegevoegd
static char gp_text[256];
//int board[63];
//int piece_list[2][21];
//int piece_count[2];
//int side;
DWORD magic;
DWORD bytes;

/* endgame files, with size, piece count, table index, CRC, name */
endhf end_set[] = {
{  125000, 3, -1, 0x9965, "OOvO.bin" },
{  125000, 3, -1, 0x3184, "OOvX.bin" },
{  125000, 3, -1, 0x13ea, "OvOO.bin" },
{  125000, 3, -1, 0xec9f, "OvXO.bin" },
{  125000, 3, -1, 0x74bd, "OvXX.bin" },
{  125000, 3, -1, 0xf732, "XOvO.bin" },
{  125000, 3, -1, 0x6cab, "XOvX.bin" },
{  125000, 3, -1, 0x9752, "XvOO.bin" },
{  125000, 3, -1, 0xa0f7, "XvXO.bin" },
{  125000, 3, -1, 0xf548, "XvXX.bin" },
{  125000, 3, -1, 0xc2ae, "XXvO.bin" },
{  125000, 3, -1, 0x703d, "XXvX.bin" },
{  347326, 4,  0, 0x9444, "OOOvO.cpr" },
{  185557, 4,  1, 0x8659, "OOOvX.cpr" },
{  431432, 4,  2, 0xf6fb, "OOvOO.cpr" },
{  960067, 4,  3, 0x6fe2, "OOvXO.cpr" },
{  648003, 4,  4, 0x369a, "OOvXX.cpr" },
{  280344, 4,  5, 0xc0ee, "OvOOO.cpr" },
{ 1027656, 4,  6, 0x45d1, "OvXOO.cpr" },
{ 1324595, 4,  7, 0x5b3d, "OvXXO.cpr" },
{  508845, 4,  8, 0x5897, "OvXXX.cpr" },
{ 1369691, 4,  9, 0x85d6, "XOOvO.cpr" },
{  960613, 4, 10, 0xec0b, "XOOvX.cpr" },
{ 1484517, 4, 11, 0x4648, "XOvOO.cpr" },
{ 1530033, 4, 12, 0xe1e6, "XOvXO.cpr" },
{  357807, 4, 13, 0x11e1, "XOvXX.cpr" },
{  298268, 4, 14, 0xdc0d, "XvOOO.cpr" },
{  517263, 4, 15, 0xe205, "XvXOO.cpr" },
{  375668, 4, 16, 0xd23b, "XvXXO.cpr" },
{  128906, 4, 17, 0x5695, "XvXXX.cpr" },
{ 1579352, 4, 18, 0xdf97, "XXOvO.cpr" },
{ 1339555, 4, 19, 0x1584, "XXOvX.cpr" },
{  862757, 4, 20, 0x05d3, "XXvOO.cpr" },
{ 1180204, 4, 21, 0x8282, "XXvXO.cpr" },
{  262388, 4, 22, 0xc7a1, "XXvXX.cpr" },
{  559505, 4, 23, 0x4d97, "XXXvO.cpr" },
{  548310, 4, 24, 0xb61a, "XXXvX.cpr" },
{ 1831050, 4, -1, 0x91d7, "end4.idx"  },
};

/* CRC table, for checking database file correctness */
USHORT crctab[256] = {
    0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50a5, 0x60c6, 0x70e7,
    0x8108, 0x9129, 0xa14a, 0xb16b, 0xc18c, 0xd1ad, 0xe1ce, 0xf1ef,
    0x1231, 0x0210, 0x3273, 0x2252, 0x52b5, 0x4294, 0x72f7, 0x62d6,
    0x9339, 0x8318, 0xb37b, 0xa35a, 0xd3bd, 0xc39c, 0xf3ff, 0xe3de,
    0x2462, 0x3443, 0x0420, 0x1401, 0x64e6, 0x74c7, 0x44a4, 0x5485,
    0xa56a, 0xb54b, 0x8528, 0x9509, 0xe5ee, 0xf5cf, 0xc5ac, 0xd58d,
    0x3653, 0x2672, 0x1611, 0x0630, 0x76d7, 0x66f6, 0x5695, 0x46b4,
    0xb75b, 0xa77a, 0x9719, 0x8738, 0xf7df, 0xe7fe, 0xd79d, 0xc7bc,
    0x48c4, 0x58e5, 0x6886, 0x78a7, 0x0840, 0x1861, 0x2802, 0x3823,
    0xc9cc, 0xd9ed, 0xe98e, 0xf9af, 0x8948, 0x9969, 0xa90a, 0xb92b,
    0x5af5, 0x4ad4, 0x7ab7, 0x6a96, 0x1a71, 0x0a50, 0x3a33, 0x2a12,
    0xdbfd, 0xcbdc, 0xfbbf, 0xeb9e, 0x9b79, 0x8b58, 0xbb3b, 0xab1a,
    0x6ca6, 0x7c87, 0x4ce4, 0x5cc5, 0x2c22, 0x3c03, 0x0c60, 0x1c41,
    0xedae, 0xfd8f, 0xcdec, 0xddcd, 0xad2a, 0xbd0b, 0x8d68, 0x9d49,
    0x7e97, 0x6eb6, 0x5ed5, 0x4ef4, 0x3e13, 0x2e32, 0x1e51, 0x0e70,
    0xff9f, 0xefbe, 0xdfdd, 0xcffc, 0xbf1b, 0xaf3a, 0x9f59, 0x8f78,
    0x9188, 0x81a9, 0xb1ca, 0xa1eb, 0xd10c, 0xc12d, 0xf14e, 0xe16f,
    0x1080, 0x00a1, 0x30c2, 0x20e3, 0x5004, 0x4025, 0x7046, 0x6067,
    0x83b9, 0x9398, 0xa3fb, 0xb3da, 0xc33d, 0xd31c, 0xe37f, 0xf35e,
    0x02b1, 0x1290, 0x22f3, 0x32d2, 0x4235, 0x5214, 0x6277, 0x7256,
    0xb5ea, 0xa5cb, 0x95a8, 0x8589, 0xf56e, 0xe54f, 0xd52c, 0xc50d,
    0x34e2, 0x24c3, 0x14a0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405,
    0xa7db, 0xb7fa, 0x8799, 0x97b8, 0xe75f, 0xf77e, 0xc71d, 0xd73c,
    0x26d3, 0x36f2, 0x0691, 0x16b0, 0x6657, 0x7676, 0x4615, 0x5634,
    0xd94c, 0xc96d, 0xf90e, 0xe92f, 0x99c8, 0x89e9, 0xb98a, 0xa9ab,
    0x5844, 0x4865, 0x7806, 0x6827, 0x18c0, 0x08e1, 0x3882, 0x28a3,
    0xcb7d, 0xdb5c, 0xeb3f, 0xfb1e, 0x8bf9, 0x9bd8, 0xabbb, 0xbb9a,
    0x4a75, 0x5a54, 0x6a37, 0x7a16, 0x0af1, 0x1ad0, 0x2ab3, 0x3a92,
    0xfd2e, 0xed0f, 0xdd6c, 0xcd4d, 0xbdaa, 0xad8b, 0x9de8, 0x8dc9,
    0x7c26, 0x6c07, 0x5c64, 0x4c45, 0x3ca2, 0x2c83, 0x1ce0, 0x0cc1,
    0xef1f, 0xff3e, 0xcf5d, 0xdf7c, 0xaf9b, 0xbfba, 0x8fd9, 0x9ff8,
    0x6e17, 0x7e36, 0x4e55, 0x5e74, 0x2e93, 0x3eb2, 0x0ed1, 0x1ef0,
};

/* table for de-compression of 4-piece file contents (result value) */
char end_val[256] = {
  0,  1,  -2,  3,  -4,  5,  -6,  7,  -8,  9, -10, 11, -12,  13, -14,  15,
-16, 17, -18, 19, -20, 21, -22, 23, -24, 25, -26, 27, -28,  29, -30,  31,
-32, 33, -34, 35, -36, 37, -38, 39, -40, 41, -42, 43, -44,  45, -46,  47,
-48, 49, -50, 51, -52, 53, -54, 55, -56, 57, -58, 59, -60, 100, 100, 100,
  0,  1,  -2,  3,  -4,  5,  -6,  7,  -8,  9, -10, 11, -12,  13, -14,  15,
-16, 17, -18, 19, -20, 21, -22, 23, -24, 25, -26, 27, -28,  29, -30,  31,
-32, 33, -34, 35, -36, 37, -38, 39, -40, 41, -42, 43, -44,  45, -46,  47,
-48, 49, -50, 51, -52, 53, -54, 55, -56, 57, -58, 59, -60, 100, 100, 100,
  0,  1,  -2,  3,  -4,  5,  -6,  7,  -8,  9, -10, 11, -12,  13, -14,  15,
-16, 17, -18, 19, -20, 21, -22, 23, -24, 25, -26, 27, -28,  29, -30,  31,
-32, 33, -34, 35, -36, 37, -38, 39, -40, 41, -42, 43, -44,  45, -46,  47,
-48, 49, -50, 51, -52, 53, -54, 55, -56, 57, -58, 59, -60, 100, 100,   0,
  0,  1,  -2,  3,  -4,  5,  -6,  7,  -8,  9, -10, 11, -12,  13, -14,  15,
-16, 17, -18, 19, -20, 21, -22, 23, -24, 25, -26, 27, -28,  29, -30,  31,
-32, 33, -34, 35, -36, 37, -38, 39, -40, 41, -42, 43, -44,  45, -46,  47,
-48, 49, -50, 51, -52, 53, -54, 55, -56, 57, -58, 59, -60, 100, 100,   0,
};

/* table for de-compression of 4-piece file contents (repeat count) */
char end_amt[256] = {
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 5, 9,
2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 6, 10,
3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 7, 0,
4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 8, 0,
};

/* heuristics tables for value approximation */
UCHAR diag_king[51] = { 0,
   0100, 0022, 0014, 0040, 0200,
0000, 0102, 0030, 0044, 0200,
   0002, 0110, 0060, 0204, 0000,
0002, 0010, 0140, 0220, 0004,
   0010, 0040, 0300, 0020, 0004,
0010, 0040, 0200, 0100, 0020,
   0040, 0200, 0000, 0100, 0020,
0040, 0200, 0000, 0000, 0100,
   0200, 0000, 0000, 0000, 0100,
0200, 0000, 0000, 0000, 0000  };

UCHAR diag_man[51] = { 0,
   0252, 0350, 0360, 0324, 0124,
0252, 0250, 0340, 0320, 0124,
   0250, 0240, 0300, 0120, 0124,
0250, 0240, 0200, 0100, 0120,
   0240, 0200, 0000, 0100, 0120,
0240, 0200, 0000, 0000, 0100,
   0200, 0000, 0000, 0000, 0100,
0200, 0000, 0000, 0000, 0000,
   0000, 0000, 0000, 0000, 0000,
0000, 0000, 0000, 0000, 0000  };

UCHAR maxdiag[51] = { 0, 8, 8, 7, 5, 4, 4 };

char center[51] = { 0,
   0,  0,  0,  0,  0,
 0,  1,  1,  1,  1,
   1,  2,  2,  2,  0,
 0,  2,  3,  3,  1,
   1,  3,  4,  2,  0,
 0,  1,  2,  2,  1,
   0,  1,  1,  0,  0,
 0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,
 0,  0,  0,  0,  0 };

char main_diag[51] = { 0,
   1,  2,  3,  4,  5,
 1,  2,  3,  4,  5,
   2,  3,  4,  5,  6,
 2,  3,  4,  5,  6,
   3,  4,  5,  6,  7,
 3,  4,  5,  6,  7,
   4,  5,  6,  7,  8,
 4,  5,  6,  7,  8,
   5,  6,  7,  8,  9,
 5,  6,  7,  8,  9 };

char cross_diag[51] = { 0,
   5,  4,  3,  2,  1,
 6,  5,  4,  3,  2,
   6,  5,  4,  3,  2,
 7,  6,  5,  4,  3,
   7,  6,  5,  4,  3,
 8,  7,  6,  5,  4,
   8,  7,  6,  5,  4,
 9,  8,  7,  6,  5,
   9,  8,  7,  6,  5,
10,  9,  8,  7,  6 };

char strat_edge[51] = { 0,
   1,  1,  1,  1,  1,
 1,  0,  0,  0,  0,
   0,  0,  0,  0,  1,
 1,  0,  0,  0,  0,
   0,  0,  0,  0,  1,
 1,  0,  0,  0,  0,
   0,  0,  0,  0,  1,
 1,  0,  0,  0,  0,
   0,  0,  0,  0,  1,
 1,  1,  1,  1,  1 };

char main_catch[51] = { 0,
   1,  0,  0,  0,  0,
 1,  1,  0,  0,  0,
   1,  1,  0,  0,  0,
 1,  1,  1,  0,  0,
   1,  1,  1,  0,  0,
 0,  1,  1,  1,  0,
   0,  1,  1,  1,  3,
 0,  0,  1,  1,  2,
   0,  0,  1,  2,  1,
 0,  0,  0,  2,  1 };

char strat_capt[51] = { 0,
   1,  0,  0,  0,  1,
 1,  0,  0,  0,  0,
   0,  0,  0,  0,  0,
 0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,
 0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,
 0,  0,  0,  0,  0,
   0,  0,  0,  0,  1,
 1,  0,  0,  0,  1 };

char strat_1grp[51] = { 0,
   1,  0,  0,  2,  2,
 1,  0,  0,  0,  0,
   0,  1,  0,  2,  2,
 0,  1,  0,  0,  0,
   0,  0,  0,  0,  0,
 0,  0,  0,  0,  0,
   0,  0,  0,  4,  0,
 3,  3,  0,  4,  0,
   0,  0,  0,  0,  4,
 3,  3,  0,  0,  4 };

char strat_2grp[51] = { 0,
   0,  0,  0,  5,  5,
 0,  0,  0,  0,  0,
   0,  0,  5,  5,  0,
 0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,
 0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,
 0,  6,  6,  0,  0,
   0,  0,  0,  0,  0,
 6,  6,  0,  0,  0 };

char strat_3grp[51] = { 0,
   0,  0,  0,  0,  7,
 0,  0,  0,  0,  0,
   0,  0,  0,  7,  7,
 0,  0,  0,  0,  0,
   0,  0,  0,  7,  0,
 0,  8,  0,  0,  0,
   0,  0,  0,  0,  0,
 8,  8,  0,  0,  0,
   0,  0,  0,  0,  0,
 8,  0,  0,  0,  0 };

char mirr_main[51] = { 0,
  45, 35, 25, 15,  5,
50, 40, 30, 20, 10,
  44, 34, 24, 14,  4,
49, 39, 29, 19,  9,
  43, 33, 23, 13,  3,
48, 38, 28, 18,  8,
  42, 32, 22, 12,  2,
47, 37, 27, 17,  7,
  41, 31, 21, 11,  1,
46, 36, 26, 16,  6 };

char mirr_tt[51] = { 0,
   6, 16, 26, 36, 46,
 1, 11, 21, 31, 41,
   7, 17, 27, 37, 47,
 2, 12, 22, 32, 42,
   8, 18, 28, 38, 48,
 3, 13, 23, 33, 43,
   9, 19, 29, 39, 49,
 4, 14, 24, 34, 44,
  10, 20, 30, 40, 50,
 5, 15, 25, 35, 45 };

char strat_rank[51] = { 0,
   0,  0,  0,  0,  0,
 1,  1,  1,  1,  1,
   2,  2,  2,  2,  2,
 3,  3,  3,  3,  3,
   4,  4,  4,  4,  4,
 5,  5,  5,  5,  5,
   6,  6,  6,  6,  6,
 7,  7,  7,  7,  7,
   8,  8,  8,  8,  8,
 9,  9,  9,  9,  9 };

char strat_31[51] = { 0,
   4,  3,  2,  1,  0,
 4,  3,  1,  2,  1,
   3,  0,  1,  0,  1,
 3,  0,  0,  0,  2,
   1,  0,  0,  1,  2,
 2,  1,  0,  0,  1,
   2,  0,  0,  0,  3,
 1,  0,  1,  0,  3,
   1,  2,  1,  3,  4,
 0,  1,  2,  3,  4 };

#ifdef EXTRA
BOOL end_check = FALSE;         /* using selective end game databases */
char end_dir_spec[MAX_PATH];    /* current directory for end game files */
char end_file_sel[MAX_FNAME];   /* current file name for end game files */
char end_file_spec[MAX_PATH];   /* complete path name for end game files */
char end_filter[128];           /* description and pattern of endfile filter */
OPENFILENAME end_ofn;           /* open / save as info */
ULONG end_added;                /* nr of end game positions added */
ULONG end_collis;               /* nr of table entries already occupied */
ULONG end_total;                /* total nr of end game table entries */
ULONG end_acc[20];              /* endgame db access statistics */
int end_pos[8];                 /* for endgame verification */
int end_pc[8] = {MW, MW, MW, MW, MB, E, E, E};
DWORD end_tick;
#endif

/************************ approximation of board value ************************/
/******************** by heuristics (in absence of database *******************/
/********************** files for that number of pieces) **********************/

/*********************************************/
/* check for king holding men above diagonal */
/*********************************************/
/* in: col = color of king                           */
/*     kp = position of king                         */
/*     valp = pointer to resulting value of position */
/* returns: TRUE if king's side has winning position */
//BOOL
//king_diag(int col, int kp, long *valp)
//{
//    USEDEF int mp[51];
//    UINT mask;
//    int  mc, i, j, tot;
//
//    if (col == W)
//    {
//        mc = piece_count[B];
//        for (j = 0; j < mc; j++)
//            mp[j] = piece_list[B][j];
//        if ((kp == 46 || kp == 14) && mc <= 3 && board[5] == MB)
//        {
//            for (j = 0; j < mc; j++)
//                if (mp[j] != 5
//                && ((diag_man[mp[j]] & 0020) != 0 ||
//                    (diag_man[mp[j]] & 0200) == 0))
//                    return FALSE;
//            *valp = INFIN/4*3 - 15*mc;
//            return TRUE;
//        }
//    }
//    else
//    {
//        kp = 51 - kp;
//        mc = piece_count[W];
//        for (j = 0; j < mc; j++)
//            mp[j] = 51 - piece_list[W][j];
//        if ((kp == 46 || kp == 14) && mc <= 3 && board[46] == MW)
//        {
//            for (j = 0; j < mc; j++)
//                if (mp[j] != 5
//                && ((diag_man[mp[j]] & 0020) != 0 ||
//                    (diag_man[mp[j]] & 0200) == 0))
//                    return FALSE;
//            *valp = 15*mc - INFIN/4*3;
//            return TRUE;
//        }
//    }
//    tot = 0377;
//    for (j = 0; j < mc; j++)
//        tot &= diag_man[mp[j]];
//    mask = 1;
//    for (i = 2; i <= maxdiag[mc]; i++)
//    {
//        mask <<= 1;
//        if ((diag_king[kp] & mask & tot) != 0)
//        {                               /* all men above king diagonal */
//            if (i == maxdiag[mc])
//            {
//                if (mc == 4)
//                    for (j = 0; j < mc; j++)
//                        if (mp[j] == 5)
//                            return FALSE;   /* watch for capture of king  */
//                while (kp > 0)
//                {
//                    for (j = 0; j < mc; j++)
//                    {
//                        if (kp == mp[j])    /* watch for sacrifice to get */
//                            return FALSE;   /* king off diagonal          */
//                        if (kp > 10 && kp - 10 == mp[j])
//                            return FALSE;
//                    }
//                    kp = neighbor[kp][i & 1];
//                }
//            }
//            if (col == W)
//            {
//                for (j = 0; j < piece_count[W]; j++)
//                    if (board[piece_list[W][j]] != KW
//                    && (diag_king[piece_list[W][j]] & mask) != 0)
//                        return FALSE;
//            }
//            else
//            {
//                for (j = 0; j < piece_count[B]; j++)
//                    if (board[piece_list[B][j]] != KB
//                    && (diag_king[piece_list[B][51 - j]] & mask) != 0)
//                        return FALSE;
//            }
//            *valp = INFIN/4*3;
//            for (j = 0; j < mc; j++)        /* estimate nr. of moves left */
//                *valp -= 2*(i + 1 - strat_rank[mp[j]]);
//            if (col == B)
//                *valp = -*valp;
//            return TRUE;
//        }
//    }
//    return FALSE;
//}
//
///***********************************************/
///* estimate value of simple end game positions */
///***********************************************/
///* in: valp = pointer to evaluation result (for white) */
///*     ply = ply depth of position                     */
///* returns TRUE if no further evaluation needed        */
///* val may be set to evaluation result for white       */
//BOOL
//estim_end(long *valp, int ply)
//{
//    USEDEF int wkp[51], bkp[51];
//    int cnt[9];
//    int i, wc, bc, wk, bk, x;
//    char diag;
//
//    cnt[1] = cnt[2] = cnt[3] = cnt[4] = 0;
//    cnt[5] = cnt[6] = cnt[7] = cnt[8] = 0;
//    wc = piece_count[W];
//    bc = piece_count[B];
//    wk = bk = 0;
//    for (i = 0; i < wc; i++)                /* get list of white kings */
//        if (board[piece_list[W][i]] == KW)
//            wkp[wk++] = piece_list[W][i];
//    for (i = 0; i < bc; i++)                /* get list of black kings */
//        if (board[piece_list[B][i]] == KB)
//            bkp[bk++] = piece_list[B][i];
//    if (wc >= bc + 3)
//    {                                       /* white can capture black */
//        if (wk < 2 && board[15] == MW && board[25] == MW)
//            return FALSE;
//        if (wk > 0)
//            *valp += INFIN/2 - LBONUS*ply;
//        else
//            *valp += 3*VAL_MAN - LBONUS*ply;
//        diag = 11;
//        for (i = 0; i < wc; i++)
//        {                                   /* keep men near edge */
//            x = piece_list[W][i];
//            if (board[x] == KW)
//            {
//                if (wk < 3)
//                {
//                    if (board[24] != E && board[20] != E)
//                        *valp += HBONUS*main_catch[x];
//                }
//                else
//                {
//                    cnt[(int) strat_1grp[x]]++;
//                    cnt[(int) strat_2grp[x]]++;
//                    cnt[(int) strat_3grp[x]]++;
//                    if (strat_capt[x] != 0)
//                        *valp += HBONUS;
//                }
//            }
//            else
//            {
//                *valp -= HBONUS*(center[51 - x] + strat_rank[x]);
//                diag = min(diag, main_diag[x]);
//            }
//        }
//        x = 0;
//        for (i = 1; i <= 8; i++)
//            if (cnt[i] > x)
//                x = cnt[i];
//        *valp += catch_bonus[x];
//        if (bk == 1 && main_diag[bkp[0]] == diag - 1)
//        {
//            *valp -= HBONUS;
//            for (i = 0; i < wk; i++)
//                if (main_diag[wkp[i]] == diag - 1)
//                    return FALSE;
//            *valp -= XBONUS;
//        }
//        return FALSE;
//    }
//    if (bc >= wc + 3)
//    {                                       /* black can capture white */
//        if (bk < 2 && board[36] == MB && board[26] == MB)
//            return FALSE;
//        if (bk > 0)
//            *valp -= INFIN/2 + LBONUS*ply;
//        else
//            *valp -= 3*VAL_MAN + LBONUS*ply;
//        diag = 11;
//        for (i = 0; i < bc; i++)
//        {                                   /* keep men near edge */
//            x = piece_list[B][i];
//            if (board[x] == KB)
//            {
//                if (bk < 3)
//                {
//                    if (board[27] != E && board[31] != E)
//                        *valp -= HBONUS*main_catch[51 - x];
//                }
//                else
//                {
//                    cnt[(int) strat_1grp[x]]++;
//                    cnt[(int) strat_2grp[x]]++;
//                    cnt[(int) strat_3grp[x]]++;
//                    if (strat_capt[x] != 0)
//                        *valp -= HBONUS;
//                }
//            }
//            else
//            {
//                *valp += HBONUS*(center[x] + strat_rank[51 - x]);
//                diag = min(diag, main_diag[51 - x]);
//            }
//        }
//        x = 0;
//        for (i = 1; i <= 8; i++)
//            if (cnt[i] > x)
//                x = cnt[i];
//        *valp -= catch_bonus[x];
//        if (wk == 1 && main_diag[51 - wkp[0]] == diag - 1)
//        {
//            *valp += HBONUS;
//            for (i = 0; i < bk; i++)
//                if (main_diag[51 - bkp[i]] == diag - 1)
//                    return FALSE;
//            *valp += XBONUS;
//        }
//        return FALSE;
//    }
//    if (wk > 0 && bk > 0 && wc <= 3 && bc <= 3)
//    {
//        *valp = 0;                          /* draw */
//        if (wk < bk)
//        {
//            for (i = 0; i < wk; i++)
//                *valp -= LBONUS*strat_31[wkp[i]];
//            for (i = 0; i < bk; i++)
//                *valp -= LBONUS*strat_edge[bkp[i]];
//        }
//        else if (wk > bk)
//        {
//            for (i = 0; i < bk; i++)
//                *valp += LBONUS*strat_31[bkp[i]];
//            for (i = 0; i < wk; i++)
//                *valp += LBONUS*strat_edge[wkp[i]];
//        }
//        return FALSE;
//    }
//    if (bk == 0)
//    {
//        for (i = 0; i < wk; i++)            /* check for men above king */
//            if (king_diag(W, wkp[i], valp))
//                return TRUE;
//        for (i = 0; i < bc; i++)            /* drive men to center */
//            *valp += HBONUS*center[piece_list[B][i]];
//    }
//    if (wk == 0)
//    {
//        for (i = 0; i < bk; i++)            /* check for men above king */
//            if (king_diag(B, bkp[i], valp))
//                return TRUE;
//        for (i = 0; i < wc; i++)            /* drive men to center */
//            *valp -= HBONUS*center[51 - piece_list[W][i]];
//    }
//    return FALSE;
//}
//
///********************* exact determination of board value *********************/
///******************** by algorithm (in absence of database ********************/
///********************** files for that number of pieces) **********************/
//
///**********************************************/
///* determine if king can capture single piece */
///**********************************************/
///* in: wp = position of king         */
///*     bp = position of piece        */
///* returns: TRUE if king can capture */
//BOOL
//cancapt(int wp, int bp)
//{
//    return (main_diag[wp] == main_diag[bp] ||
//            cross_diag[wp] == cross_diag[bp])
//           && strat_edge[bp] == 0;
//}
//
///***********************************/
///* value of 2 kings, can't capture */
///***********************************/
///* in: wp = position of one king   */
///*     bp = position of other king */
///* returns: evaluation result      */
//long
//valkk(int wp, int bp)
//{
//    if ((wp == 46 && bp == 5) || (wp == 5 && bp == 46))
//        return -INFIN + 2;  /* kings are at ends of main diagonal */
//    else if (main_diag[wp] == 5 && main_diag[bp] == 5)
//        return INFIN - 3;   /* "side" moves to end of main diag.  */
//    return 0;               /* draw, kings are somewhere else     */
//}
//
///******************************************/
///* determine if kings can capture a piece */
///******************************************/
///* in: wp1, wp2 = positions of kings */
///*     bp       = position of piece  */
///* returns: TRUE if capture possible */
//BOOL
//can2capt(int wp1, int wp2, int bp)
//{
//    char md, xd;
//
//    if (strat_edge[bp] != 0)
//        return FALSE;
//    md = main_diag[bp];
//    xd = cross_diag[bp];
//    if (main_diag[wp1] == md && wp2 != neighbor[bp][(wp1 < bp) ? 2 : 1])
//        return TRUE;
//    if (cross_diag[wp1] == xd && wp2 != neighbor[bp][(wp1 < bp) ? 3 : 0])
//        return TRUE;
//    if (main_diag[wp2] == md && wp1 != neighbor[bp][(wp2 < bp) ? 2 : 1])
//        return TRUE;
//    if (cross_diag[wp2] == xd && wp1 != neighbor[bp][(wp2 < bp) ? 3 : 0])
//        return TRUE;
//    return FALSE;
//}
//
///***************************************************/
///* see if black king can be caught along main axis */
///***************************************************/
///* in: wk1, wk2 = positions of white kings */
///*     bk       = position of black king   */
///* returns: TRUE if caught  */
//BOOL
//can2main(int wk1, int wk2, int bk)
//{
//    int d1, d2, dm, dc, x;
//
//    d1 = main_diag[wk1];
//    d2 = main_diag[wk2];
//    dm = main_diag[bk];
//    if (d2 == dm)
//    {
//        if (d1 == dm)
//        {
//            if (bk == 49 && wk1 != 40 && wk2 != 40)
//                return FALSE;
//            else
//                return TRUE;
//        }
//        else
//        {
//            d2 = d1, x = wk2, wk2 = wk1, wk1 = x;
//        }
//    }
//    else if (d1 != dm)
//    {
//        return FALSE;
//    }
//    dc = cross_diag[bk];
//    x  = cross_diag[wk2];
//    if (wk1 == mirr_tt[bk] && wk2 != mirr_main[bk]
//    && (d2 == 10 - dm || x == dc))
//        return TRUE;
//    d1 = cross_diag[wk1];
//    if ((d1 == 11 - dc && x  >= 13 - dc && x  < dc)
//    ||  (x  == 11 - dc && d1 >= 13 - dc && d1 < dc))
//        return TRUE;
//    return FALSE;
//}
//
///*****************************************************/
///* see if black king can be caught across cross axis */
///*****************************************************/
///* in: wk1, wk2 = positions of white kings */
///*     bk       = position of black king   */
///* returns: TRUE if caught  */
//BOOL
//can2cross(int wk1, int wk2, int bk)
//{
//    int d1, d2, dm, dc, x;
//
//    d1 = cross_diag[wk1];
//    d2 = cross_diag[wk2];
//    dc = cross_diag[bk];
//    if (d2 == dc)
//    {
//        if (d1 == dc && bk != 47)
//            return TRUE;
//        else
//            d2 = d1, x = wk2, wk2 = wk1, wk1 = x;
//    }
//    else if (d1 != dc)
//    {
//        return FALSE;
//    }
//    dm = main_diag[bk];
//    x  = main_diag[wk2];
//    if (wk1 == mirr_main[bk] && wk2 != mirr_tt[bk]
//    && (x == dm || d2 == 11 - dc))
//        return TRUE;
//    d1 = main_diag[wk1];
//    if ((d1 == 10 - dm && x  >= 12 - dm && x  < dm)
//    ||  (x  == 10 - dm && d1 >= 12 - dm && d1 < dm))
//        return TRUE;
//    return FALSE;
//}
//
///************************************/
///* determine value of 2 white kings */
///* against 1 black, white to move   */
///************************************/
///* in: wk1, wk2 = positions of white kings */
///*     bk       = position of black king   */
///* returns: the value                      */
//long
//valwkkk(int wk1, int wk2, int bk)
//{
//    int x, y;
//
//    if (wk1 > wk2)
//        x = wk1, wk1 = wk2, wk2 = x;
//    if ((bk == 28 && wk1 == 23 && wk2 == 32)
//    ||  (bk == 32 && wk1 == 28 && wk2 == 37)
//    ||  (bk == 37 && wk1 == 32 && wk2 == 41))
//        return INFIN - 5;
//    if (bk == 46)
//    {
//        if (wk2 == 41)
//        {
//            if (wk1 != 37
//            && (main_diag[wk1] == 5 || cross_diag[wk1] == 8))
//                return INFIN - 1;
//            else
//                return INFIN - 3;
//        }
//        if ((wk1 == 41 && (wk2 == 42 || wk2 == 48))
//        ||  (wk1 == 37 && wk2 == 47)
//        ||  (wk1 == 36 && wk2 == 37))
//            return INFIN - 1;
//        else if (main_diag[wk1] == 5 || main_diag[wk2] == 5)
//            return INFIN - 3;
//    }
//    else if (bk == 50)
//    {
//        if (can2cross(wk1, wk2, bk))
//        {
//            return INFIN - 3;
//        }
//        else
//        {
//            if ((wk1 == 1 && wk2 == 7)
//            ||  (wk1 == 2 && wk2 == 6)
//            ||  (wk1 == 6 && wk2 == 16))
//                return INFIN - 7;
//            x = cross_diag[wk1];
//            if (wk2 == 45)
//            {
//                if (wk1 == 6)
//                    return INFIN - 5;
//                else if (wk1 == 1 || x == 6)
//                    return INFIN - 3;
//            }
//            y = cross_diag[wk2];
//            if (wk1 == 1 && y == 5)
//                return INFIN - 5;
//            else if (wk1 == 7 && y == 6)
//                return INFIN - 7;
//            else if ((x == 5 && y == 6) || (x == 6 && y == 5))
//                return INFIN - 5;
//            return 0;
//        }
//    }
//    else if (bk >= 47)
//    {
//            if (can2main(wk1, wk2, bk)
//            ||  can2cross(wk1, wk2, bk))
//                return INFIN - 3;
//    }
//    else if (can2capt(wk1, wk2, bk))
//    {
//        return INFIN - 1;
//    }
//    return 0;
//}
//
///************************************/
///* determine value of 2 white kings */
///* against 1 black, black to move   */
///************************************/
///* in: wk1, wk2 = positions of white kings */
///*     bk       = position of black king   */
///* returns: the value                      */
//long
//valbkkk(int wk1, int wk2, int bk)
//{
//    BOOL capt;
//    int  x, y, dir, dy, wp1, wp2, bp;
//    long val, value;
//
//    value = INFIN;
//    capt = FALSE;
//    dir = 0;
//    while (dir < 4)
//    {
//        x = neighbor[bk][dir];
//        while (x > 0)
//        {
//            if (x == wk1 || x == wk2)
//            {
//                if (x == wk1)
//                    y = wk2;
//                else
//                    y = wk1;
//                x = neighbor[x][dir];
//                if (x > 0 && x != y)
//                {
//                    if (!capt)
//                        value = INFIN, capt = TRUE;
//                    if (dir == 0 || dir == 3)
//                        dy = main_diag[y];
//                    else
//                        dy = cross_diag[y];
//                    do {
//                        if (strat_edge[y] == 0)
//                        {
//                            if (dir == 0 || dir == 3)
//                            {
//                                if (dy == main_diag[x])
//                                    return INFIN - 1;
//                            }
//                            else
//                            {
//                                if (dy == cross_diag[x])
//                                    return INFIN - 1;
//                            }
//                        }
//                        if (x == y)
//                            break;
//                        if (cancapt(y, x))
//                            val = INFIN - 1;
//                        else
//                            val = valkk(y, x);
//                        if (val < value)
//                            value = val;
//                        x = neighbor[x][dir];
//                    } while (x > 0);
//                }
//                else
//                {
//                    break;
//                }
//            }
//            if (!capt)
//            {
//                wp1 = wk1, wp2 = wk2, bp = x;
//                if (cross_diag[bp] <= 5)
//                {                           /* normalize range of black */
//                    wp1 = mirr_tt[wp1];
//                    wp2 = mirr_tt[wp2];
//                    bp  = mirr_tt[bp ];
//                }
//                if (main_diag[bp] <= 4)
//                {
//                    wp1 = mirr_main[wp1];
//                    wp2 = mirr_main[wp2];
//                    bp  = mirr_main[bp ];
//                }
//                val = valwkkk(wp1, wp2, bp);
//                if (val < value)
//                    value = val;
//            }
//            x = neighbor[x][dir];
//        }
//        dir++;
//    }
//    if (value == INFIN)
//        return -value;
//    else if (value > 0)
//        return -value + 1;
//    else if (value < 0)
//        return -value - 1;
//    return 0;
//}
//
/********************* exact determination of board value *********************/
/************************* by accessing database files ************************/

/*****************************************/
/* initialize endgame db reference entry */
/*****************************************/
/* in: ep = ptr to endfile info structure */
void
init_endref(endhf *ep)
{
    int i, mw, mb, kw, kb;

    i = mw = mb = kw = kb = 0;
    while (ep->name[i] != 'v' && ep->name[i] != '\0')
    {
        if (ep->name[i] == 'X')
            kw++;
        if (ep->name[i] == 'O')
            mw++;
        i++;
    }
    while (ep->name[i] != '.' && ep->name[i] != '\0')
    {
        if (ep->name[i] == 'X')
            kb++;
        if (ep->name[i] == 'O')
            mb++;
        i++;
    }
    ep->mat_ofs = 2*kw + mw - 2*kb - mb;
    end_ref[EF*EF*EF*kw + EF*EF*mw + EF*kb + mb] = ep;
}

/************************************/
/* initialize endgame database data */
/************************************/
void
init_enddb(void)
{
    FILE   *f;
    int    i, j;
    USEDEF int lng, cnt, idx, crc;

//    find_full(IDS_ENDLST);      /* load external endgame db file info */
	
	errno_t err = fopen_s(&f,"C:\\Endgame\\cpr\\DAMEND.LST", "r");
    if (!(err))
    {
        for (i = 0; i < sizeof ext_set / sizeof ext_set[0]; i++)
        {
            j = fscanf_s(f, "%i,%i,%i,%i,%s\n", &lng, &cnt, &idx, &crc, gp_text, 255);
            if (j != 5 || strlen(gp_text) >= 14)
                break;
            ext_set[i].length = lng;
            ext_set[i].pc_count = cnt;
            ext_set[i].idx = idx;
            ext_set[i].crc = crc;
            strcpy_s(ext_set[i].name, gp_text);
            init_endref(&ext_set[i]);
        }
        fclose(f);
    } else printf("Can't open DAMEND.LST\n");
    for (i = 0; i < sizeof end_set / sizeof end_set[0]; i++)
        init_endref(&end_set[i]);
    end4x = end_ref[0];

    combi[0][0] = 1;            /* set up combination lookup table */
    for (i = 1; i <= 50; i++)
    {
        combi[i][0] = 1;
        for (j = 1; j < 8; j++)
            combi[i][j] = combi[i - 1][j - 1] + combi[i - 1][j];
    }
}

/******************************************/
/* close end game database file if needed */
/******************************************/
/* in: ep = ptr to endfile info structure */
void
close_endfile(endhf *ep)
{
    if (ep != NULL && ep->hf != NULL)
    {
        if (ep->hf == INVALID_HANDLE_VALUE)
            ep->hf = NULL;
        else if (ep->f_ptr != NULL)
        {
            UnmapViewOfFile(ep->f_ptr);
            ep->f_ptr = NULL;
            CloseHandle(ep->hmap);
            ep->hmap = NULL;
            CloseHandle(ep->hf);
            ep->hf = NULL;
        }
    }
}

/********************************/
/* reset end game database info */
/********************************/
void
reset_enddb(void)
{
    int i;

    for (i = 0; i < sizeof end_ref / sizeof end_ref[0]; i++)
        close_endfile(end_ref[i]);
}

/************************************************/
/* get full path of end game database directory */
/************************************************/
/* in: section = which part of semicolon-separated  */
/*               path to select                     */
/* returns: TRUE = requested part copied to gp_text */
/*          FALSE = requested part not found        */
//BOOL
//getdbdir(int section)
//{
//    char *end_ptr, *ch_ptr;
//    int end_len, gp_len, tot_len;
//
//    end_ptr = enddbdir;
//    while (section > 0)                     /* locate requested section */
//    {
//        end_ptr = strchr(end_ptr, ';');
//        if (end_ptr == NULL)
//            return FALSE;
//        end_ptr++;
//        section--;
//    }
//    ch_ptr = strchr(end_ptr, ';');          /* remove successive sections */
//    if (ch_ptr != NULL)
//        end_len = (int) (ch_ptr - end_ptr);
//    else
//        end_len = (int) strlen(end_ptr);
//    gp_text[0] = '\0';
//    if (end_len > 0 && end_ptr[0] != '\\' && end_ptr[1] != ':')
//    {
//        GetModuleFileName(hInst, gp_text, MAX_PATH);   /* relative path,    */
//        ch_ptr = strrchr(gp_text, '\\');               /* prepend directory */
//        if (ch_ptr != NULL)                            /* of executable     */
//            ch_ptr[1] = '\0';
//    }
//    gp_len = strlen(gp_text);
//    tot_len = min(MAX_PATH - 1, gp_len + end_len);     /* append end dir */
//    strncpy(gp_text + gp_len, end_ptr, tot_len - gp_len);
//    gp_text[tot_len] = '\0';
//    if (tot_len == 0 || gp_text[tot_len - 1] != '\\')
//        strcat(gp_text, "\\");                         /* final backslash */
//    return TRUE;
//}
//
/*****************************************/
/* open end game database file if needed */
/*****************************************/
/* in: ep = ptr to endfile info structure */
/* returns: 0 is success       */
/*          1 file not found   */
/*          2 incorrect length */
/*          3 other            */
int
open_endfile(endhf *ep)
{
    int section;

    if (ep->hf == INVALID_HANDLE_VALUE)     /* got error opening file before */
        return 3;
    section = 0;
    if (ep->hf == NULL)                     /* file not opened before */
    {
        do {
            //if (!getdbdir(section++))       /* try next dir in endgame path */
            //    return 1;
			strcpy_s(gp_text, 256, "C:\\Endgame\\cpr\\");
            strcat_s(gp_text, 256, ep->name);      /* construct full path */
			//printf("looking for: %s\n", gp_text);
            ep->hf = CreateFile(gp_text, GENERIC_READ, FILE_SHARE_READ, NULL,
                            OPEN_EXISTING,
                            FILE_ATTRIBUTE_NORMAL | FILE_FLAG_RANDOM_ACCESS,
                            NULL);
        } while (ep->hf == INVALID_HANDLE_VALUE);
#ifdef EXTRA
        if (ep->length == 0)
            ep->length = GetFileSize(ep->hf, NULL);
#endif
        //printf("File opened\n");
		if (GetFileSize(ep->hf, NULL) != ep->length)
        {
            CloseHandle(ep->hf);
            ep->hf = INVALID_HANDLE_VALUE;
            return 2;
        }
        ep->hmap = CreateFileMapping(ep->hf, NULL, PAGE_READONLY, 0, 0, ep->name);
        if (ep->hmap == NULL)
        {
#ifdef _DEBUG
            //MessageBox(hwndMain, "CreateFileMapping failed", szAppName, MB_OK);
			printf("CreateFileMapping failed\n");
#endif
            CloseHandle(ep->hf);
            ep->hf = INVALID_HANDLE_VALUE;
            return 3;
        }
        ep->f_ptr = (UCHAR *) MapViewOfFile(ep->hmap, FILE_MAP_READ, 0, 0, 0);
        if (ep->f_ptr == NULL)
        {                               /* virtual memory exhausted?       */
            reset_enddb();              /* close other files and try again */
            ep->f_ptr = (UCHAR *) MapViewOfFile(ep->hmap, FILE_MAP_READ, 0, 0, 0);
            if (ep->f_ptr == NULL)
            {
#ifdef _DEBUG
                //MessageBox(hwndMain, "MapViewOfFile failed", szAppName, MB_OK);
				printf("MapViewOfFile failed\n");
#endif
                CloseHandle(ep->hmap);
                ep->hmap = NULL;
                CloseHandle(ep->hf);
                ep->hf = INVALID_HANDLE_VALUE;
                return 3;
            }
        }
    }
    return 0;
}

/**************************************************/
/* check presence & correct size of end game file */
/**************************************************/
/* in: ep = ptr to endgame file info */
/* returns: see open_endfile(), plus: */
/*          4 = incorrect CRC */
int
check_enddb(endhf *ep)
{
    int ret;

    ret = open_endfile(ep);
    if (ret == 0)
    {
        UCHAR *fp;
        UINT  crc;
        DWORD i;

        crc = 0xffff;
        fp = ep->f_ptr;
#ifdef EXTRA
        if (end_check)   /* leave file open to check max. virtual memory use */
            return ret;
        if (ep->crc != 0xffff)      /* don't check the files marked like this */
#endif
        for (i = 0; i < ep->length; i++)
            crc = ((crc<<8)&0xff00) ^ crctab[((crc>>8)&0xff) ^ *fp++];
        if (crc != ep->crc)
            ret = 4;
#ifdef _DEBUG
        if (crc != ep->crc)
        {
            //char line[100];
            //wsprintf(line,"%s: %x", ep->name, crc);
            //MessageBox(hwndMain, line, szAppName, MB_ICONEXCLAMATION | MB_OK);
			printf("%s: %x\n", ep->name, crc);
        }
#endif
        close_endfile(ep);
    }
    return ret;
}

/***************************************************/
/* search for position value in endgame hash table */
/***************************************************/
/* in: valp = pointer to returned value of position */
/* returns: TRUE if found in table                  */
//BOOL
//find_endhash(long *valp)
//{
//    endhash *eh;
//
//    if (hash_end == NULL)
//        return FALSE;
//    eh = &hash_end[hashkey & EHMASK];
//    if (eh->hashval != (hashval ^ side))
//        return FALSE;
//    *valp = eh->eval;
//    return TRUE;
//}
//
/**********************************************/
/* store position value in endgame hash table */
/**********************************************/
/* in: val = value to store */
//void
//store_endhash(long val)
//{
//    endhash *eh;
//
//    if (hash_end == NULL)
//    {
//        hash_end = (endhash *) mymalloc(EHSIZE*sizeof(endhash));
//        if (hash_end == NULL)
//            return;
//    }
//    eh = &hash_end[hashkey & EHMASK];
//    eh->hashval = hashval ^ side;
//    eh->eval = val;
//    stored_hashkey = hashkey;
//    stored_hashval = hashval;
//}
//
/**********************************/
/* sort a list of piece positions */
/**********************************/
/* in: a = array of integers to sort   */
/*     n = number of elements in array */
__inline
void
sort_list(int *a, int n)
{
    int i, j, t;

    for (i = 0; i < n - 1; i++)
        for (j = i + 1; j < n; j++)
            if (a[i] > a[j])
                t = a[i], a[i] = a[j], a[j] = t;
}

/*********************************/
/* prepare for database indexing */
/*********************************/
/* out: list = position list of pieces */
/*      np = piece counts              */
/* returns: ptr to db info structure,  */
/*          or NULL if error/notfound  */
//endhf *
//prep_db(int list[][8], int np[])
//{
//    int i, pc, pos;
//    endhf *ep;
//
//    np[MW] = 0;
//    np[MB] = 0;
//    np[KW] = 0;
//    np[KB] = 0;
//    if (side == W)
//    {
//        for (i = 0; i < piece_count[W]; i++)
//        {
//            pos = piece_list[W][i];
//            pc = board[pos];
//            list[pc][np[pc]++] = pos - 1;
//        }
//        for (i = 0; i < piece_count[B]; i++)
//        {
//            pos = piece_list[B][i];
//            pc = board[pos];
//            list[pc][np[pc]++] = pos - 1;
//        }
//    }
//    else
//    {
//        for (i = 0; i < piece_count[W]; i++)
//        {
//            pos = piece_list[W][i];
//            pc = inv_pc[board[pos]];
//            list[pc][np[pc]++] = 50 - pos;
//        }
//        for (i = 0; i < piece_count[B]; i++)
//        {
//            pos = piece_list[B][i];
//            pc = inv_pc[board[pos]];
//            list[pc][np[pc]++] = 50 - pos;
//        }
//    }
//	// fb: in list[MW/MB/KW/KB][i] staat de positie 0..49 net als bij MG
//	// in np[MW/MB/KW/KB] staat het aantal
//
//    /* find database file */
//    ep = end_ref[EF*EF*EF*np[KW] + EF*EF*np[MW] + EF*np[KB] + np[MB]];
//    if (ep != NULL)
//    {
//        if (open_endfile(ep) != 0)
//        {
//            PEGCOUNT(end_acc[0]);
//            return NULL;
//        }
//        sort_list(list[MW], np[MW]);
//        sort_list(list[MB], np[MB]);
//        sort_list(list[KW], np[KW]);
//        sort_list(list[KB], np[KB]);
//    }
//    return ep;
//}
//
endhf *
alt_prep_db(int list[][8], int np[], int color,int wman,  unsigned long * wman_idx, 
							                   int wcrown,unsigned long * wcrown_idx, 
								               int bman,  unsigned long * bman_idx, 
								               int bcrown,unsigned long * bcrown_idx )

{
    int p;
    endhf *ep;

    np[MW] = 0;
    np[MB] = 0;
    np[KW] = 0;
    np[KB] = 0;

	if (color == W)
	{	for (p=0; p<wman;  p++)  list[MW][np[MW]++]  = wman_idx[p];
		for (p=0; p<wcrown; p++) list[KW][np[KW]++]  = wcrown_idx[p];
		for (p=0; p<bman;  p++)  list[MB][np[MB]++]  = bman_idx[p];
		for (p=0; p<bcrown; p++) list[KB][np[KB]++]  = bcrown_idx[p];
	}
	else
	{	for (p=bman-1; p>=0;  p--)  list[MW][np[MW]++]  = 49 - bman_idx[p];
		for (p=bcrown-1; p>=0; p--) list[KW][np[KW]++]  = 49 - bcrown_idx[p];
		for (p=wman-1; p>=0;  p--)  list[MB][np[MB]++]  = 49 - wman_idx[p];
		for (p=wcrown-1; p>=0; p--) list[KB][np[KB]++]  = 49 - wcrown_idx[p];
	}

	// fb: in list[MW/MB/KW/KB][i] staat de positie 0..49 net als bij MG
	// in np[MW/MB/KW/KB] staat het aantal

    /* find database file */
    ep = end_ref[EF*EF*EF*np[KW] + EF*EF*np[MW] + EF*np[KB] + np[MB]];
	//printf("ep = %p\n", ep);
    if (ep != NULL)
    {	int ret;
        if ((ret = open_endfile(ep)) != 0)
        {	printf("problem ret = %d\n", ret);
            PEGCOUNT(end_acc[0]);
            return NULL;
        }
        sort_list(list[MW], np[MW]);
        sort_list(list[MB], np[MB]);
        sort_list(list[KW], np[KW]);
        sort_list(list[KB], np[KB]);
    }
    return ep;
}

/*********************************/
/* evaluate board w.r.t end game */
/*********************************/
/* in: valp = pointer to resulting value of position */
/* returns: TRUE if outcome is certain               */
//BOOL
//endgame(long *valp)
//{
//    int     wc, bc, x;
//    int     wp1, bp1;
//    char    v;
//    endhf  *ep;
//    UCHAR  *pb, *pz;
//    ULONG   ipos, li, idx;
//    int list[6][8], np[6];
//    int i, j, pc, ofs, c;
//
//    wc = piece_count[W];
//    bc = piece_count[B];
//    if (wc == 0 || bc == 0)
//    {
//        *valp = -INFIN;             /* lose, no piece left to move with */
//        return TRUE;
//    }
//    switch (wc + bc)
//    {
//    case 2:                         /* 1 piece against 1 piece */
//        if (!valid_2pc)
//            return FALSE;
//        PEGCOUNT(end_acc[2]);
//        wp1 = piece_list[W][0];
//        bp1 = piece_list[B][0];
//        if (side == W)
//            x = 50*wp1 + bp1 - 51;
//        else
//            x = 2550 - 50*bp1 - wp1;
//        if (board[wp1] == MW)
//        {
//            if (board[bp1] == MB)
//            {
//                v = mm[x];
//            }
//            else
//            {
//                if (side == B)
//                    v = km[x];
//                else
//                    v = mk[x];
//            }
//        }
//        else
//        {
//            if (board[bp1] == KB)
//            {
//                v = kk[x];
//            }
//            else
//            {
//                if (side == W)
//                    v = km[x];
//                else
//                    v = mk[x];
//            }
//        }
//        if (v != 0)
//            *valp = -INFIN - 1 - v;
//        else
//            *valp = 0;
//        return TRUE;
//
//    case 3:                             /* 2 pieces against 1 piece */
//        if (enddbdir[0] == '\0')
//        {  return FALSE;                             /* end game data base not available */
//
//			//if (wc == 2)
//   //         {
//   //             wp1 = piece_list[W][0];
//   //             wp2 = piece_list[W][1];
//   //             bp1  = piece_list[B][0];
//   //             w1  = board[wp1];
//   //             w2  = board[wp2];
//   //             b1   = board[bp1];
//   //             sd = side;
//   //         }
//   //         else
//   //         {                           /* normalize to 2 white - 1 black */
//   //             wp1 = 51 - piece_list[B][0];
//   //             wp2 = 51 - piece_list[B][1];
//   //             bp1  = 51 - piece_list[W][0];
//   //             w1  = inv_pc[board[piece_list[B][0]]];
//   //             w2  = inv_pc[board[piece_list[B][1]]];
//   //             b1   = inv_pc[board[piece_list[W][0]]];
//   //             sd = W + B - side;
//   //         }
//   //         if (b1 == KB)
//   //         {
//   //             if (w1 == KW)
//   //             {
//   //                 if (w2 == KW)
//   //                 {                                /* all kings, normalize */
//   //                     if (cross_diag[bp1] <= 5)    /* range of black       */
//   //                     {
//   //                         wp1 = mirr_tt[wp1];
//   //                         wp2 = mirr_tt[wp2];
//   //                         bp1  = mirr_tt[bp1];
//   //                     }
//   //                     if (main_diag[bp1] <= 4)
//   //                     {
//   //                         wp1 = mirr_main[wp1];
//   //                         wp2 = mirr_main[wp2];
//   //                         bp1  = mirr_main[bp1];
//   //                     }
//   //                     if (sd == W)
//   //                         *valp = valwkkk(wp1, wp2, bp1);
//   //                     else
//   //                         *valp = valbkkk(wp1, wp2, bp1);
//   //                     PEGCOUNT(end_acc[3]);
//   //                     return TRUE;
//   //                 }
//   //             }
//   //         }
//   //         return FALSE;
//        }                                   /* end game database available */
//        //if (find_endhash(valp))
//        //{
//        //    PEGCOUNT(end_acc[13]);
//        //    return TRUE;
//        //}
//        ep = prep_db(list, np);
//        if (ep == NULL)
//            return FALSE;
//        ipos = 0;
//        for (i = 0; i < 4; i++)
//        {
//            pc = pcorder[i];
//            for (j = 0; j < np[pc]; j++)
//                ipos = 50*ipos + list[pc][j];
//        }
//        if (ipos >= ep->length)
//        {
//            PEGCOUNT(end_acc[0]);
//            return FALSE;
//        }
//        v = ep->f_ptr[ipos];
//        if (v == 100)
//            *valp = ep->mat_ofs;    /* draw, return material value */
//        else if (v > 0)
//            *valp = INFIN - v;      /* distance to win */
//        else
//            *valp = -INFIN - v;     /* distance to loss */
//        //store_endhash(*valp);
//        PEGCOUNT(end_acc[3]);
//        return TRUE;
//
//    case 4:                             /* 2 pieces against 2, or 3 against 1 */
//        if (enddbdir[0] == '\0')
//            return FALSE;               /* end game data base not available */
//        if (end4x->hf == INVALID_HANDLE_VALUE)
//            return FALSE;               /* index file not available */
//        //if (find_endhash(valp))
//        //{
//        //    PEGCOUNT(end_acc[14]);
//        //    return TRUE;
//        //}
//        if (open_endfile(end4x) != 0)
//        {
//            PEGCOUNT(end_acc[0]);
//            return FALSE;
//        }
//        ep = prep_db(list, np);
//        if (ep == NULL)
//            return FALSE;
//        ipos = 0;
//        for (i = 0; i < 4; i++)
//        {
//            pc = pcorder[i];
//            for (j = 0; j < np[pc]; j++)
//                ipos = 50*ipos + list[pc][j];
//        }
//        li = ipos >> 8;
//        ofs = (int) ipos & 255;
//        idx = 0;
//        if (li > 0)
//        {
//            pb = &end4x->f_ptr[ep->idx*73242 + li*3 - 3];
//            if (pb > end4x->f_ptr + end4x->length - 3)
//            {
//                PEGCOUNT(end_acc[0]);
//                return FALSE;
//            }
//            idx  = *pb++;               /* get 3-byte index, is little-endian */
//            idx += *pb++*256;
//            idx += *pb*65536;
//        }
//        pb = &ep->f_ptr[idx];           /* starting point for decompression */
//        pz = ep->f_ptr + ep->length;    /* end of file marker */
//        do {
//            if (pb >= pz)
//            {
//                PEGCOUNT(end_acc[0]);
//                return FALSE;
//            }
//            c = *pb++;
//            if (c == 255)
//            {
//                if (pb >= pz - 1)
//                {
//                    PEGCOUNT(end_acc[0]);
//                    return FALSE;
//                }
//                ofs -= *pb++ + 1;
//                c = end_val[(int) *pb++];
//            }
//            else if (c == 191)
//            {
//                if (pb >= pz)
//                {
//                    PEGCOUNT(end_acc[0]);
//                    return FALSE;
//                }
//                ofs -= *pb++ + 1;
//                c = 100;
//            }
//            else
//            {
//                ofs -= end_amt[c];
//                c = end_val[c];
//            }
//        } while (ofs >= 0);
//        if (c == 100)
//            *valp = ep->mat_ofs;    /* draw, return material value */
//        else if (c > 0)
//            *valp = INFIN - c;      /* distance to win */
//        else
//            *valp = -INFIN - c;     /* distance to loss */
//        //store_endhash(*valp);
//        PEGCOUNT(end_acc[4]);
//        return TRUE;
//
//    default:                            /* more pieces than supported */
//        break;
//    }
//    return FALSE;
//}
//
/*********************************/
/* alt_evaluate board w.r.t end game */
/*********************************/
/* in: valp = pointer to resulting value of position */
/* returns: TRUE if outcome is certain               */
BOOL
alt_endgame(long *valp, int color,int wman,  unsigned long * wman_idx, 
							      int wcrown,unsigned long * wcrown_idx, 
								  int bman,  unsigned long * bman_idx, 
								  int bcrown,unsigned long * bcrown_idx )
{
    int     wc, bc, x;
    int     wp0, bp0;
    char    v;
    endhf  *ep;
    UCHAR  *pb, *pz;
    ULONG   ipos, li, idx;
    int list[6][8], np[6];
    int i, j, pc, ofs, c;

    wc = wman + wcrown;
    bc = bman + bcrown;
    if (wc == 0 || bc == 0)
    {
        *valp = -INFIN;             /* lose, no piece left to move with */
        return TRUE;
    }
    switch (wc + bc)
    {
    case 2:                         /* 1 piece against 1 piece */
        if (!valid_2pc)
            return FALSE;
        PEGCOUNT(end_acc[2]);
		if (wman) wp0 = wman_idx[0]; else wp0 = wcrown_idx[0];
		if (bman) bp0 = bman_idx[0]; else bp0 = bcrown_idx[0];

        if (color == W) x = 50*wp0 + bp0;
        else            x = 2499 - 50*bp0 - wp0;
		//printf("wp0 = %d, bp0 = %d, x = %d\n", wp0, bp0, x);
		if (wman)
        {   if (bman)
            {   v = mm[x];
            }
            else
            {   if (color == B) v = km[x];
                else            v = mk[x];
            }
		}
        else
        {   if (bcrown)
            {   v = kk[x];
            }
            else
            {   if (color == W) v = km[x];
                else            v = mk[x];
            }
		}
        if (v > 0)      *valp = INFIN - v;
		else if (v < 0) *valp = -INFIN + v;
        else            *valp = 1;

        return TRUE;

    case 3:                             /* 2 pieces against 1 piece */
        if (enddbdir[0] == '\0')
		{	printf("end game data base not available\n");return FALSE;}                             /* end game data base not available */
		//printf("alt prep\n");
        ep = alt_prep_db(list, np, color, wman,   wman_idx, 
		                                  wcrown, wcrown_idx, 
										  bman,   bman_idx,
										  bcrown, bcrown_idx);
        if (ep == NULL)
		{   printf("error prep db\n"); return FALSE; }
		//printf("alt prep done\n");
        ipos = 0;
        for (i = 0; i < 4; i++)
        {
            pc = pcorder[i];
            for (j = 0; j < np[pc]; j++)
                ipos = 50*ipos + list[pc][j];
        }
        if (ipos >= ep->length)
        {	printf("too long\n");
            PEGCOUNT(end_acc[0]);
            return FALSE;
        }
		//printf("ipos = %d\n", ipos);
        v = ep->f_ptr[ipos];
		//printf("ipos = %d, v = %d\n", ipos, v);
        if (v == 100)
            *valp = ep->mat_ofs;    /* draw, return material value */
        else if (v > 0)
            *valp = INFIN - v;      /* distance to win */
        else
            *valp = -INFIN - v;     /* distance to loss */
        //store_endhash(*valp);
        PEGCOUNT(end_acc[3]);
        return TRUE;

    case 4:                             /* 2 pieces against 2, or 3 against 1 */
        if (enddbdir[0] == '\0')
            return FALSE;               /* end game data base not available */
        if (end4x->hf == INVALID_HANDLE_VALUE)
            return FALSE;               /* index file not available */
        //if (find_endhash(valp))
        //{
        //    PEGCOUNT(end_acc[14]);
        //    return TRUE;
        //}
        if (open_endfile(end4x) != 0)
        {
            PEGCOUNT(end_acc[0]);
            return FALSE;
        }
        ep = alt_prep_db(list, np, color, wman,   wman_idx, 
		                                  wcrown, wcrown_idx, 
										  bman,   bman_idx,
										  bcrown, bcrown_idx);
        if (ep == NULL)
            return FALSE;
        ipos = 0;
        for (i = 0; i < 4; i++)
        {
            pc = pcorder[i];
            for (j = 0; j < np[pc]; j++)
                ipos = 50*ipos + list[pc][j];
        }
        li = ipos >> 8;
        ofs = (int) ipos & 255;
        idx = 0;
        if (li > 0)
        {
            pb = &end4x->f_ptr[ep->idx*73242 + li*3 - 3];
            if (pb > end4x->f_ptr + end4x->length - 3)
            {
                PEGCOUNT(end_acc[0]);
                return FALSE;
            }
            idx  = *pb++;               /* get 3-byte index, is little-endian */
            idx += *pb++*256;
            idx += *pb*65536;
        }
        pb = &ep->f_ptr[idx];           /* starting point for decompression */
        pz = ep->f_ptr + ep->length;    /* end of file marker */
        do {
            if (pb >= pz)
            {
                PEGCOUNT(end_acc[0]);
                return FALSE;
            }
            c = *pb++;
            if (c == 255)
            {
                if (pb >= pz - 1)
                {
                    PEGCOUNT(end_acc[0]);
                    return FALSE;
                }
                ofs -= *pb++ + 1;
                c = end_val[(int) *pb++];
            }
            else if (c == 191)
            {
                if (pb >= pz)
                {
                    PEGCOUNT(end_acc[0]);
                    return FALSE;
                }
                ofs -= *pb++ + 1;
                c = 100;
            }
            else
            {
                ofs -= end_amt[c];
                c = end_val[c];
            }
        } while (ofs >= 0);
        if (c == 100)
            *valp = ep->mat_ofs;    /* draw, return material value */
        else if (c > 0)
            *valp = INFIN - c;      /* distance to win */
        else
            *valp = -INFIN - c;     /* distance to loss */
        //store_endhash(*valp);
        PEGCOUNT(end_acc[4]);
        return TRUE;

    default:                            /* more pieces than supported */
        break;
    }
    return FALSE;
}

/*******************************************/
/* get index value for a single piece type */
/*******************************************/
/* in: n = number of pieces of this type        */
/*     fields = number of allowed board squares */
/*     pos = array of sorted piece positions    */
/* returns: the index value                     */
ULONG
index_singletype(int n, int fields, int *pos)
{
    int i, leading_pc, offset = 0;
    ULONG result = 0;

    for (i = 0; i < n; i++)
    {
        leading_pc = pos[i] - offset;
        result += combi[fields][n - i] - combi[fields - leading_pc][n - i];
        fields -= leading_pc + 1;
        offset = pos[i] + 1;
    }
    return result;
}

/*******************************************************/
/* find the value of a 5- or 6-piece endgame position. */
/* note: no capture must be possible in this position. */
/*******************************************************/
/* in: valp = pointer to resulting value of position */
/* returns: TRUE if outcome is certain               */
//BOOL
//endgame_5_6(long *valp)
//{
//    int i, j, jumpover;
//    ULONG ipos, p1, p2, p3;
//    int np[6] = {0, 0, 0, 0, 0, 0};
//    int list[6][8];
//    USEDEF int decrease[6][8];
//    UCHAR  cval, *blkptr, *pb, *pz;
//    endhf *ep;
//    static UINT pow3[] = { 1, 3, 9, 27, 81 };
//
//#ifdef _DEBUG
//    if (piece_count[W] + piece_count[B] < 5
//    ||  piece_count[W] + piece_count[B] > 6)
//    {
//        PEGCOUNT(end_acc[0]);
//        return FALSE;
//    }
//#endif
//    if (enddbdir[0] == '\0')
//        return FALSE;               /* end game data base not available */
//    //if (find_endhash(valp))
//    //{
//    //    PEGCOUNT(end_acc[10 + piece_count[W] + piece_count[B]]);
//    //    return TRUE;
//    //}
//    ep = prep_db(list, np);
//    if (ep == NULL)
//        return FALSE;
//    /* set jumpovers white man */
//    for (i = 0; i < np[MW]; i++)
//    {
//        jumpover = 0;
//        for (j = 0; j < np[MB]; j++)
//            if (list[MB][j] < list[MW][i] && list[MB][j] >= 5)
//                jumpover++;
//        decrease[MW][i] = jumpover;
//    }
//    for (i = 0; i < np[KB]; i++)    /* set jumpovers black king */
//    {
//        jumpover = 0;
//        for (j = 0; j < np[MB]; j++)
//            if (list[MB][j] < list[KB][i])
//                jumpover++;
//        for (j = 0; j < np[MW]; j++)
//            if (list[MW][j] < list[KB][i])
//                jumpover++;
//        decrease[KB][i] = jumpover;
//    }
//    for (i = 0; i < np[KW]; i++)    /* set jumpovers white king */
//    {
//        jumpover = 0;
//        for (j = 0; j < np[MB]; j++)
//            if (list[MB][j] < list[KW][i])
//                jumpover++;
//        for (j = 0; j < np[MW]; j++)
//            if (list[MW][j] < list[KW][i])
//                jumpover++;
//        for (j = 0; j < np[KB]; j++)
//            if (list[KB][j] < list[KW][i])
//                jumpover++;
//        decrease[KW][i] = jumpover;
//    }
//    for (i = 0; i < np[KB]; i++)    /* apply decreases */
//        list[KB][i] -= decrease[KB][i];
//    for (i = 0; i < np[KW]; i++)
//        list[KW][i] -= decrease[KW][i];
//    for (i = 0; i < np[MW]; i++)
//        list[MW][i] -= decrease[MW][i] + 5;
//    p3 = combi[50 - np[MB] - np[MW] - np[KB]][np[KW]];
//    p2 = p3*combi[50 - np[MB] - np[MW]][np[KB]];
//    p1 = p2*combi[45][np[MW]];
//    ipos  = index_singletype(np[MB], 45, list[MB])*p1;
//    ipos += index_singletype(np[MW], 45, list[MW])*p2;
//    ipos += index_singletype(np[KB], 50 - np[MB] - np[MW], list[KB])*p3;
//    ipos += index_singletype(np[KW], 50 - np[MB] - np[MW] - np[KB], list[KW]);
//    blkptr = ep->f_ptr + ep->idx*(ipos/1024);
//    pz = ep->f_ptr + ep->length;    /* end of file marker */
//    if (blkptr > pz - ep->idx)
//    {
//        PEGCOUNT(end_acc[0]);
//        return FALSE;
//    }
//    pb = ep->f_ptr + *blkptr++;     /* find start of segment */
//    pb += *blkptr++*256;            /* note: is little-endian */
//    pb += *blkptr++*65536;
//    if (ep->idx > 3)
//        pb += *blkptr*16777216;     /* for 4-byte index */
//    i = ipos%1024;
//    do {
//        if (pb >= pz)                   /* bounds check */
//        {
//            PEGCOUNT(end_acc[0]);
//            return FALSE;
//        }
//        cval = *pb++;
//        if (cval <= 242)                /* non-repeat code */
//        {
//            i -= 5;
//        }
//        else if (cval <= 246)           /* repeat code for win */
//        {
//            if (cval == 246)
//            {
//                if (pb >= pz)
//                {
//                    PEGCOUNT(end_acc[0]);
//                    return FALSE;
//                }
//                i -= *pb++*5;           /* next byte has repeat count */
//            }
//            else
//            {
//                 i -= (cval - 241)*5;   /* repeat count 2,3,4 */
//            }
//            cval = 0;
//        }
//        else if (cval <= 250)           /* repeat code for draw */
//        {
//            if (cval == 250)
//            {
//                if (pb >= pz)
//                {
//                    PEGCOUNT(end_acc[0]);
//                    return FALSE;
//                }
//                i -= *pb++*5;           /* next byte has repeat count */
//            }
//            else
//            {
//                i -= (cval - 245)*5;    /* repeat count 2,3,4 */
//            }
//            cval = 121;
//        }
//        else if (cval <= 254)           /* repeat code for loss */
//        {
//            if (cval == 254)
//            {
//                if (pb >= pz)
//                {
//                    PEGCOUNT(end_acc[0]);
//                    return FALSE;
//                }
//                i -= *pb++*5;           /* next byte has repeat count */
//            }
//            else
//            {
//                i -= (cval - 249)*5;    /* repeat count 2,3,4 */
//            }
//            cval = 242;
//        }
//        else                            /* 255 = repeat code for other */
//        {
//            if (pb >= pz - 1)
//            {
//                PEGCOUNT(end_acc[0]);
//                return FALSE;
//            }
//            i -= *pb++*5;               /* next byte has repeat count */
//            cval = *pb++;               /* next byte has repeated value */
//        }
//    } while (i >= 0);
//    cval = (cval/pow3[4 + (i + 1)%5])%3;
//    if (cval == 0)                      /* win */
//        *valp = INFIN - 64*(ep->pc_count - 3);
//    else if (cval == 1)                 /* draw */
//        *valp = 0;
//    else                                /* loss */
//        *valp = 64*(ep->pc_count - 3) - INFIN;
//    *valp += ep->mat_ofs;               /* add small material offset */
//    //store_endhash(*valp);
//    PEGCOUNT(end_acc[piece_count[W] + piece_count[B]]);
//    return TRUE;
//}

BOOL
alt_endgame_5_6(long *valp, int color,	int wman,  unsigned long * wman_idx, 
										int wcrown,unsigned long * wcrown_idx, 
										int bman,  unsigned long * bman_idx, 
										int bcrown,unsigned long * bcrown_idx )

{
    int i, j, jumpover;
    ULONG ipos, p1, p2, p3;
    int np[6] = {0, 0, 0, 0, 0, 0};
    int list[6][8];
    USEDEF int decrease[6][8];
    UCHAR  cval, *blkptr, *pb, *pz;
    endhf *ep;
    static UINT pow3[] = { 1, 3, 9, 27, 81 };

    if (enddbdir[0] == '\0')
        return FALSE;               /* end game data base not available */

    ep = alt_prep_db(list, np, color, wman,   wman_idx, 
	                                  wcrown, wcrown_idx, 
									  bman,   bman_idx,
									  bcrown, bcrown_idx);
    if (ep == NULL)
        return FALSE;
    /* set jumpovers white man */
    for (i = 0; i < np[MW]; i++)
    {
        jumpover = 0;
        for (j = 0; j < np[MB]; j++)
            if (list[MB][j] < list[MW][i] && list[MB][j] >= 5)
                jumpover++;
        decrease[MW][i] = jumpover;
    }
    for (i = 0; i < np[KB]; i++)    /* set jumpovers black king */
    {
        jumpover = 0;
        for (j = 0; j < np[MB]; j++)
            if (list[MB][j] < list[KB][i])
                jumpover++;
        for (j = 0; j < np[MW]; j++)
            if (list[MW][j] < list[KB][i])
                jumpover++;
        decrease[KB][i] = jumpover;
    }
    for (i = 0; i < np[KW]; i++)    /* set jumpovers white king */
    {
        jumpover = 0;
        for (j = 0; j < np[MB]; j++)
            if (list[MB][j] < list[KW][i])
                jumpover++;
        for (j = 0; j < np[MW]; j++)
            if (list[MW][j] < list[KW][i])
                jumpover++;
        for (j = 0; j < np[KB]; j++)
            if (list[KB][j] < list[KW][i])
                jumpover++;
        decrease[KW][i] = jumpover;
    }
    for (i = 0; i < np[KB]; i++)    /* apply decreases */
        list[KB][i] -= decrease[KB][i];
    for (i = 0; i < np[KW]; i++)
        list[KW][i] -= decrease[KW][i];
    for (i = 0; i < np[MW]; i++)
        list[MW][i] -= decrease[MW][i] + 5;
    p3 = combi[50 - np[MB] - np[MW] - np[KB]][np[KW]];
    p2 = p3*combi[50 - np[MB] - np[MW]][np[KB]];
    p1 = p2*combi[45][np[MW]];
    ipos  = index_singletype(np[MB], 45, list[MB])*p1;
    ipos += index_singletype(np[MW], 45, list[MW])*p2;
    ipos += index_singletype(np[KB], 50 - np[MB] - np[MW], list[KB])*p3;
    ipos += index_singletype(np[KW], 50 - np[MB] - np[MW] - np[KB], list[KW]);
    blkptr = ep->f_ptr + ep->idx*(ipos/1024);
    pz = ep->f_ptr + ep->length;    /* end of file marker */
    if (blkptr > pz - ep->idx)
    {
        PEGCOUNT(end_acc[0]);
        return FALSE;
    }
    pb = ep->f_ptr + *blkptr++;     /* find start of segment */
    pb += *blkptr++*256;            /* note: is little-endian */
    pb += *blkptr++*65536;
    if (ep->idx > 3)
        pb += *blkptr*16777216;     /* for 4-byte index */
    i = ipos%1024;
    do {
        if (pb >= pz)                   /* bounds check */
        {
            PEGCOUNT(end_acc[0]);
            return FALSE;
        }
        cval = *pb++;
        if (cval <= 242)                /* non-repeat code */
        {
            i -= 5;
        }
        else if (cval <= 246)           /* repeat code for win */
        {
            if (cval == 246)
            {
                if (pb >= pz)
                {
                    PEGCOUNT(end_acc[0]);
                    return FALSE;
                }
                i -= *pb++*5;           /* next byte has repeat count */
            }
            else
            {
                 i -= (cval - 241)*5;   /* repeat count 2,3,4 */
            }
            cval = 0;
        }
        else if (cval <= 250)           /* repeat code for draw */
        {
            if (cval == 250)
            {
                if (pb >= pz)
                {
                    PEGCOUNT(end_acc[0]);
                    return FALSE;
                }
                i -= *pb++*5;           /* next byte has repeat count */
            }
            else
            {
                i -= (cval - 245)*5;    /* repeat count 2,3,4 */
            }
            cval = 121;
        }
        else if (cval <= 254)           /* repeat code for loss */
        {
            if (cval == 254)
            {
                if (pb >= pz)
                {
                    PEGCOUNT(end_acc[0]);
                    return FALSE;
                }
                i -= *pb++*5;           /* next byte has repeat count */
            }
            else
            {
                i -= (cval - 249)*5;    /* repeat count 2,3,4 */
            }
            cval = 242;
        }
        else                            /* 255 = repeat code for other */
        {
            if (pb >= pz - 1)
            {
                PEGCOUNT(end_acc[0]);
                return FALSE;
            }
            i -= *pb++*5;               /* next byte has repeat count */
            cval = *pb++;               /* next byte has repeated value */
        }
    } while (i >= 0);
    cval = (cval/pow3[4 + (i + 1)%5])%3;
    if (cval == 0)                      /* win */
        *valp = INFIN - 64*(ep->pc_count - 3);
    else if (cval == 1)                 /* draw */
        *valp = 0;
    else                                /* loss */
        *valp = 64*(ep->pc_count - 3) - INFIN;
    *valp += ep->mat_ofs;               /* add small material offset */
    //store_endhash(*valp);
    PEGCOUNT(end_acc[piece_count[W] + piece_count[B]]);
    return TRUE;
}

/*********************************/
/* clear end game position table */
/*********************************/
void
clear_end(void)
{
    long i;
    ULONG *et;

    et = end_tabl;
    for (i = 0; i < ENDSIZE; i++)
        *et++ = 0;
}

/********************************************/
/* find position in end game position table */
/********************************************/
/* returns: TRUE if found in table */
//BOOL
//find_end(void)
//{
//    ULONG hk, val;
//
//#ifdef EXTRA
//    if (end_check)
//        return FALSE;
//#endif
//    /* skip position if just found by 5/6-piece endgame databases */
//    if (stored_hashkey == hashkey && stored_hashval == hashval)
//        return FALSE;
//    hk = hashkey;
//    if (side == B)
//        hk++;
//    do {
//        val = end_tabl[hk & ENDMASK];
//        if (val == hashval)
//            return TRUE;
//        hk += 2;
//    } while (val != 0);
//    return FALSE;
//}

#ifdef EXTRA
/******************************************/
/* store board position in end game table */
/******************************************/
/* black to move, position is winning for white         */
/* returns: FALSE if all table entries already occupied */
BOOL
store_end(void)
{
    hvalkey *hp;
    ULONG *et;
    ULONG hk, hv;
    int i, j, p;
    long x;

    x = 1;
    while (TRUE)
    {
        et = &end_tabl[(hashkey + x) & ENDMASK];
        if (*et == 0 || *et == hashval)
        {
            *et = hashval;                  /* store for straight board */
            break;
        }
        x += 2;
        if (x >= ENDSIZE)
            return FALSE;
    }
    hk = 0;
    hv = 0;
    for (i = 0; i < 2; i++)
    {
        for (j = 0; j < piece_count[i]; j++)
        {
            p = piece_list[i][j];
            hp = &htab[51 - p][inv_pc[board[p]] - M];
            hk ^= hp->key;
            hv ^= hp->val;
        }
    }
    x = 0;
    while (TRUE)
    {
        et = &end_tabl[(hk + x) & ENDMASK];
        if (*et == 0 || *et == hv)
        {
            *et = hv;                         /* store for inverted board */
            break;
        }
        x += 2;
        if (x >= ENDSIZE)
            return FALSE;
    }
    return TRUE;
}

/*********************************/
/* mirror board against diagonal */
/*********************************/
/* in: maindiag = TRUE for mirroring against main diagonal */
void
mirror_board(BOOL maindiag)
{
    USEDEF int brd[51];
    int i, j, pos, pc;
    char *mirr_ptr;
    hvalkey *hp;

    if (maindiag)
        mirr_ptr = mirr_main;
    else
        mirr_ptr = mirr_tt;
    for (i = 1; i <= 50; i++)
        brd[i] = E;
    hashkey = 0;
    hashval = 0;
    for (i = 0; i < 2; i++)
    {
        for (j = 0; j < piece_count[i]; j++)
        {
            pos = piece_list[i][j];
            pc = board[pos];
            pos = mirr_ptr[pos];
            hp = &htab[pos][pc - M];
            hashkey ^= hp->key;
            hashval ^= hp->val;
            piece_list[i][j] = pos;
            brd[pos] = pc;
        }
    }
    for (i = 1; i <= 50; i++)
        board[i] = brd[i];
}

/**************************************************/
/* store set of board positions in end game table */
/**************************************************/
/* black to move, position is winning for white   */
void
store_pos(void)
{
    int i, j;

    for (i = 0; i < 2; i++)
    {                               /* check if only kings on the board */
        for (j = 0; j < piece_count[i]; j++)
        {
            if (board[piece_list[i][j]] < K)
            {
                if (store_end())    /* man found, store only actual position */
                    end_added++;
                else
                    end_collis++;
                return;
            }
        }
    }                               /* all kings, store actual and  */
    mirror_board(TRUE);             /* mirrored positions           */
    if (store_end())
        end_added++;
    else
        end_collis++;
    mirror_board(FALSE);
    if (store_end())
        end_added++;
    else
        end_collis++;
    mirror_board(TRUE);
    if (store_end())
        end_added++;
    else
        end_collis++;
    mirror_board(FALSE);
    if (store_end())
        end_added++;
    else
        end_collis++;
}

/*********************************************/
/* add a game to the end game position table */
/*********************************************/
/* in: fname = name of save file     */
/* returns: TRUE = OK, FALSE = abort */
BOOL
end_add(char *fname)
{
    int col, h_move, move;

    if (!load_game(fname))          /* load the game */
        return FALSE;
    if (!unmake_all())              /* undo all moves */
        return FALSE;
    copy_d_board();
    copy_board();
    h_move = game_record[3];
    while (h_move > 0)
    {
        init_eval();
        col = (piece_count[B] == 2) ? B : W;
        if (side == B && game_record[h_move + 2] == 0
        && piece_count[W] >= 1 && piece_count[B] >= 1
        && (piece_count[W] + piece_count[B] > 3
            || (piece_count[W] + piece_count[B] == 3 &&
                !(board[piece_list[W][0]] == KW &&
                  board[piece_list[B][0]] == KB &&
                  board[piece_list[col][1]] == KW + col)))
        && piece_count[W] + piece_count[B] <= end_maxpc)
        {
            store_pos();
        }
        move = getpack_gr(h_move);
        if (move <= 0)
            return FALSE;
        make_move(move);
        relpacket(move);
        h_move = game_record[h_move + 1];
    }
    init_eval();
    col = (piece_count[B] == 2) ? B : W;
    if (side == B
    && piece_count[W] >= 1 && piece_count[B] >= 1
    && (piece_count[W] + piece_count[B] > 3
            || (piece_count[W] + piece_count[B] == 3 &&
                !(board[piece_list[W][0]] == KW &&
                  board[piece_list[B][0]] == KB &&
                  board[piece_list[col][1]] == KW + col)))
    && piece_count[W] + piece_count[B] <= end_maxpc)
    {
        gen_moves(XMVTREE, side, 1, 0);
        move = move_tree[XMVTREE];
        if (move > 0 && move_tree[move + 4] == 0)
            store_pos();
    }
    return TRUE;
}

/*************************************/
/* count positions in end game table */
/*************************************/
void
count_end(void)
{
    long i;
    ULONG *et;

    end_total = 0;
    et = end_tabl;
    for (i = 0; i < ENDSIZE; i++)
        if (*et++ != 0)
            end_total++;
}

/***********************************/
/* add positions to end game table */
/***********************************/
void
add_end(void)
{
    end_added = end_collis = 0;
    forall_dam(end_add);            /* add all indicated files */
    count_end();
    sound_move(TRUE, TRUE);         /* alert operator          */
}

/***********************/
/* save end game table */
/***********************/
/* in: fname = name of file, or null */
/* returns: TRUE if OK               */
BOOL
save_end(char *fname)
{
    char    *str;

    if (fname == NULL)
    {
        strcpy(end_file_spec, end_file_sel);
        end_ofn.hwndOwner = hwndMain;
        end_ofn.Flags = OFN_OVERWRITEPROMPT | OFN_HIDEREADONLY
                       | OFN_NOREADONLYRETURN;
        if (!GetSaveFileName(&end_ofn))
            return FALSE;
        strcpy(end_dir_spec, end_file_spec);
        str = strrchr(end_dir_spec, '\\');
        if (str != NULL)
            *str = '\0';
        fname = end_file_spec;
    }
    set_cursor(hCurWait);
    hf = CreateFile(fname, GENERIC_WRITE, 0, NULL,
                    CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    switch (hf == INVALID_HANDLE_VALUE)
    {
    case TRUE:
        break;
    default:
        magic = E_MAGIC;
        if (!WriteFile(hf, &magic, sizeof magic, &bytes, NULL)
        ||  bytes != sizeof magic
        ||  !WriteFile(hf, mm, sizeof mm, &bytes, NULL)
        ||  bytes != sizeof mm
        ||  !WriteFile(hf, mk, sizeof mk, &bytes, NULL)
        ||  bytes != sizeof mk
        ||  !WriteFile(hf, km, sizeof km, &bytes, NULL)
        ||  bytes != sizeof km
        ||  !WriteFile(hf, kk, sizeof kk, &bytes, NULL)
        ||  bytes != sizeof kk
        ||  !WriteFile(hf, end_tabl, ENDSIZE*sizeof(ULONG), &bytes, NULL)
        ||  bytes != ENDSIZE*sizeof(ULONG))
            break;
        if (!CloseHandle(hf))
            break;
        set_cursor(hCurArrow);
        return TRUE;
    }
    if (hf != INVALID_HANDLE_VALUE)
        CloseHandle(hf);
    set_cursor(hCurArrow);
    file_error(OF_WRITE, fname);
    return FALSE;
}
#endif

/***********************/
/* load end game table */
/***********************/
/* in: fname = name of file, or null */
/* returns: TRUE if OK               */
BOOL
load_end(char *fname)
{
#ifdef EXTRA
    char    *str;

    if (fname == NULL)
    {
        strcpy(end_file_spec, end_file_sel);
        end_ofn.hwndOwner = hwndMain;
        end_ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
        if (!GetOpenFileName(&end_ofn))
            return FALSE;
        strcpy(end_dir_spec, end_file_spec);
        str = strrchr(end_dir_spec, '\\');
        if (str != NULL)
            *str = '\0';
        fname = end_file_spec;
    }
#endif

    //set_cursor(hCurWait);
    HANDLE hf = CreateFile(fname, GENERIC_READ, FILE_SHARE_READ, NULL,
                    OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    switch (hf == INVALID_HANDLE_VALUE)
    {
    case TRUE:
		printf("Invalid handle value: %p\n", hf);
        break;
    default:
        if (!ReadFile(hf, &magic, sizeof magic, &bytes, NULL)
        ||  bytes != sizeof magic
        ||  magic != E_MAGIC
        ||  !ReadFile(hf, mm, sizeof mm, &bytes, NULL)
        ||  bytes != sizeof mm
        ||  !ReadFile(hf, mk, sizeof mk, &bytes, NULL)
        ||  bytes != sizeof mk
        ||  !ReadFile(hf, km, sizeof km, &bytes, NULL)
        ||  bytes != sizeof km
        ||  !ReadFile(hf, kk, sizeof kk, &bytes, NULL)
        ||  bytes != sizeof kk)
            break;
        valid_2pc = TRUE;
        if (!ReadFile(hf, end_tabl, ENDSIZE*sizeof(ULONG), &bytes, NULL)
        || bytes != ENDSIZE*sizeof(ULONG))
            break;
#ifdef EXTRA
        count_end();
#endif
        if (!CloseHandle(hf))
            break;
        //set_cursor(hCurArrow);
        return TRUE;
    }
    if (hf != INVALID_HANDLE_VALUE)
        CloseHandle(hf);
    //set_cursor(hCurArrow);
    //file_error(OF_READ, fname);
	printf("File error: %s, magic = %X\n", fname, magic);
    return FALSE;
}

/*******************************/
/* load initial end game table */
/*******************************/
/* returns: TRUE if OK         */
BOOL
init_end(void)
{
#ifdef EXTRA
    char    *str;
    char    ch;
    int     x;

    x = load_l_string(IDS_XENDFILT, end_filter, 128);
    ch = end_filter[x - 1];
    while (--x >= 0)
        if (end_filter[x] == ch)
            end_filter[x] = '\0';
    GetModuleFileName(hInst, end_dir_spec, MAX_PATH);
    str = strrchr(end_dir_spec, '\\');
    if (str != NULL)
        *str = '\0';
    end_ofn.lStructSize = sizeof end_ofn;
    end_ofn.lpstrFilter = end_filter;
    end_ofn.lpstrFile = end_file_spec;
    end_ofn.nMaxFile = MAX_PATH;
    end_ofn.lpstrFileTitle = end_file_sel;
    end_ofn.nMaxFileTitle = MAX_FNAME;
    end_ofn.lpstrInitialDir = end_dir_spec;
    end_ofn.lpstrDefExt = "end";
#endif
    end_tabl = (ULONG *) mymalloc(ENDSIZE*sizeof(ULONG));
    if (end_tabl == NULL)
        return FALSE;
    //find_full(IDS_FEND);
    //if (!load_end(file_spec))
	if (!load_end("C:\\Endgame\\cpr\\DAM.END"))
        clear_end();
    init_enddb();
    return TRUE;
}

/*************************************/
/* clean-up actions for program exit */
/*************************************/
void
exit_end(void)
{
    reset_enddb();
    //if (hash_end != NULL)
    //    myfree(hash_end);
    //if (end_tabl != NULL)
    //    myfree(end_tabl);
}

#ifdef EXTRA
/****************/
/* report error */
/****************/
/* in: msg = error message string */
/*     v1 = first value to dump   */
/*     v2 = second value to dump  */
void
error_msg(const char *msg, long v1, long v2)
{
    char line[100];

    repaint_board();
    wsprintf(line, "%s %ld %ld\n%d %d %d %d %d %d %d",
             msg, v1, v2, end_pos[1], end_pos[2], end_pos[3],
             end_pos[4], end_pos[5], end_pos[6], end_pos[7]);
    MessageBox(hwndMain, line, szAppName, MB_ICONEXCLAMATION | MB_OK);
}

/**********************************/
/* check position's value against */
/* minimax value of valid moves   */
/**********************************/
void
comp_ce(void)
{
    BOOL b;
    int  i;
    long v, pvv;

    side = side_moving = col(end_pc[0]);
    side_pc = W + B - side;
    for (i = 1; i <= 50; i++)
        d_board[i] = E;
    for (i = 1; i < 8; i++)
        if (end_pc[i] != E)
            d_board[end_pos[i]] = end_pc[i];
    copy_board();
    init_eval();
    if (piece_count[W] + piece_count[B] > 4)
    {
        gen_moves(XMVTREE, side, 0, 0); /* generate captures only */
        if (move_tree[XMVTREE] > 0)
            return;                     /* skip when capture position */
        b = endgame_5_6(&v);
    }
    else
    {
        b = endgame(&v);
    }
    if (!b)
    {
        error_msg("direct false", 0, 0);
        multi_ret = DRAWPOS;
        return;
    }
    thinktime = 1;
    last_tick = GetTickCount();
    pc_lev = 1;
    maxreached = FALSE;
    maxply = MAXPLY;
    nearinfin = INFIN - MAXPLY;
    inh_hash = FALSE;
    reltree(XMVTREE);
    pvv = pv_search(0, XMVTREE, 1, -INFIN, INFIN, PV);
    reltree(XMVTREE);
    if (multi_ret != 0 || escapekey)
        return;
    if (piece_count[W] + piece_count[B] > 4)
    {
        b = ((v >= nearinfin && pvv >= nearinfin)
             || (abs(v) <= MAXPLY && abs(pvv) <= MAXPLY)
             || (v <= -nearinfin && pvv <= -nearinfin));
    }
    else    /* exact distance to win only available with <= 4 pc */
    {
        b = (v == pvv || (abs(v) <= MAXPLY && abs(pvv) <= MAXPLY));
    }
    if (!b)
    {
        error_msg("mismatch", v, pvv);
        multi_ret = DRAWPOS;
        return;
    }
}

/**********************************/
/* check position's value against */
/* approx. endgame evaluation     */
/**********************************/
void
comp_ap(void)
{
    BOOL b;
    int  i;
    long v, pvv;

    side = side_moving = col(end_pc[0]);
    side_pc = W + B - side;
    for (i = 1; i <= 50; i++)
        d_board[i] = E;
    for (i = 1; i < 8; i++)
        if (end_pc[i] != E)
            d_board[end_pos[i]] = end_pc[i];
    copy_board();
    init_eval();
    gen_moves(XMVTREE, side, 0, 0); /* generate captures only */
    if (move_tree[XMVTREE] > 0)
        return;                     /* skip when capture position */
    if (piece_count[W] + piece_count[B] > 4)
    {
        b = endgame_5_6(&v);
    }
    else
    {
        b = endgame(&v);
    }
    if (!b)
    {
        error_msg("direct false", 0, 0);
        multi_ret = DRAWPOS;
        return;
    }
    pvv = get_eval(ENDPLY);
    if ((pvv >  INFIN/3 && v <  INFIN/3)
    ||  (pvv < -INFIN/3 && v > -INFIN/3))
    {
        error_msg("mismatch", v, pvv);
        multi_ret = DRAWPOS;
        return;
    }
}

/*******************************************/
/* iterate one piece through all positions */
/*******************************************/
/* in: i = index of the piece to iterate */
void
comp_iterate(int i)
{
    int p;

    for (end_pos[i] = 1; end_pos[i] <= 50; end_pos[i]++)
    {
        if ((end_pos[i] <=  5 && end_pc[i] == MW)
        ||  (end_pos[i] >= 46 && end_pc[i] == MB))
            continue;
        for (p = 1; p < i; p++)
            if (end_pos[i] == end_pos[p]
            || (end_pos[i] < end_pos[p] && end_pc[i] == end_pc[p]))
                break;
        if (p < i)
            continue;
        if (end_pc[i + 1] != E)
            comp_iterate(i + 1);
        else if (!end_check)
            comp_ce();
        else
            comp_ap();
        if (multi_ret != 0 || escapekey)
            return;
        if (GetTickCount() - end_tick >= 500)
        {
            end_tick += ((GetTickCount() - end_tick)/500)*500;
            repaint_board();
            multi(0);
        }
    }
}

/*****************************************/
/* check consistency of endgame database */
/*****************************************/
void
comp_chkend(void)
{
    end_tick = GetTickCount();
    board_empty();
    clock_running = TRUE;

    if (do_dialog(IDD_ENDTEST) != IDOK)
        return;
    escapekey = FALSE;
    comp_iterate(1);
    repaint_board();
}
#endif
extern int evaluate_endgame_position(char color_white, 
									 int nr_of_white_man, unsigned long * white_man_idx, 
									 int nr_of_white_king, unsigned long * white_king_idx, 
									 int nr_of_black_man, unsigned long * black_man_idx, 
									 int nr_of_black_king, unsigned long * black_king_idx)

	// white_man_idx etc. in internal BoomstraDam representation: 0..9, 11..20, 22..31, 33..42, 44..53

{	int i,color;
	unsigned long loc_white_man_idx[21], loc_white_king_idx[21], loc_black_man_idx[21], loc_black_king_idx[21];
	long return_value;
	
for (i=0; i<nr_of_white_man; i++){  loc_white_man_idx [i] = int_to_ext[white_man_idx [i]] - 1; /*printf("white man: %d\n", white_man_idx [i]);*/}
	for (i=0; i<nr_of_white_king; i++) loc_white_king_idx[i] = int_to_ext[white_king_idx[i]] - 1;
	for (i=0; i<nr_of_black_man; i++)  {loc_black_man_idx [i] = int_to_ext[black_man_idx [i]] - 1;/*printf("black man: %d\n", black_man_idx [i]);*/}
	for (i=0; i<nr_of_black_king; i++) loc_black_king_idx[i] = int_to_ext[black_king_idx[i]] - 1;


	if (color_white) color = W; else color = B;

	//printf("tot pieces: %d\n", nr_of_white_man + nr_of_white_king + nr_of_black_man + nr_of_black_king );

	if (nr_of_white_man + nr_of_white_king + nr_of_black_man + nr_of_black_king >= 5)
	{	BOOL result = alt_endgame_5_6(&return_value, color, nr_of_white_man,  loc_white_man_idx, 
		                                  nr_of_white_king, loc_white_king_idx, 
										  nr_of_black_man,  loc_black_man_idx,
										  nr_of_black_king, loc_black_king_idx);
		if (result) {{ if (return_value == 0) return_value = 1;} return (int) return_value;} else return 0; 
	}
	else
	{	BOOL result = alt_endgame(&return_value, color, nr_of_white_man,  loc_white_man_idx, 
		                                  nr_of_white_king, loc_white_king_idx, 
										  nr_of_black_man,  loc_black_man_idx,
										  nr_of_black_king, loc_black_king_idx);
		if (result) {{ if (return_value == 0) return_value = 1;} return (int) return_value;} else return 0; 
	}
}

extern void Initialize_endgame()
{	init_end();
}
