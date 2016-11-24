/*
 * Copyright 1996 by Michel D. Grimminck
 *
 * All Rights Reserved
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose and without fee is hereby granted,
 * provided that the above copyright notice appear in all copies and that
 * both that copyright notice and this permission notice appear in
 * supporting documentation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */

/* global variables */

#include "egdb-const.h"

#ifndef POS
#define POS
#endif

//char *db33;
//int db33a;
//int db33b;
//
//POS int lmap[10][10];
//POS char next[2][93][4];
///*  index 1=piece color
//    index 2=board position
//    index 3=direction:
//        forleft=0
//        forright=1
//        backright=2
//        backleft=3
//    value: position of next field in the specified direction or 'invalid (92)' if of the board
//*/
//POS char nextall[2][93][20][20];
///*
//    index 1=piece color
//    index 2=board position
//    index 3=direction left/right         (dx+10)   10=0,9=-1,8=-2,15=+5, etc
//    index 3=direction forward/backward   (dy+10)   10=0,9=-1,8=-2,15=+5, etc
//    value: position of next field in the specified direction or 'invalid (92)' if of the board
//    see macro's RBOARD() and LBOARD
// */
//
POS BTYPE board[93];
//POS BTYPE blocked[93];
//POS char takeback[4][4][4][4][4][4];
///* see patsearc.init_takeback for documentation */
//
//POS int pieces[8];
//POS char promote[2][93];
//POS char movelist[MAXPLY][MAXNM][MOVEL];
//POS char killer [MAXPLY][MOVEL];
//POS struct _movescore {
//    char move[MOVEL];
//    int value;
//} movescore[MAXNM];
//POS struct _tpat
//{
//    char board[50];
//    int use_succes;
//    int use_fail;
//    int nmoves;
//    char movelist[12][32];
//} tpat[NPAT];
//
//
//POS int tomove;
//
//POS INT64 nsort,neval,ngen,ndat,nmat,nmovelist,nquiet,nquietfail,precount,dbfail,pat_try,pat_found,pat_succes;
//POS INT64 tneval;  /* total evaluations during this move */
//POS char workdir[256]=WORKDIR;
//POS char version[128]=VERSION;
//POS char PV[MPV][MPV][MOVEL];
//POS int maxpv=7;
//POS int deval[MAXPLY];
//POS int quiescence=true;
//POS int do_order=true;
//POS int do_res=true;
//#ifdef MAPPEDMEMORY
    POS int mem64db[4096*81];
//#else
//    POS unsigned char *database[4096*81];
//#endif
////POS int db_usage[4096];
////POS int loadDatabaseOnDemand[4096*81];
////POS int dtwStatus[4096*81];   //0= don't know, 1=not available, 2=available
//
//POS int use_db=true;
//typedef struct  {
//    unsigned int hashkey;
//    int min_score;
//    int max_score;
//    unsigned char board[18];
//    char flags;
//    char move[2];
//    short int depth;
//} tpTranspos;
//tpTranspos *transpos;
//POS struct _evalcache {
//    unsigned int hashkey;
//    unsigned char board[18];
//    int score;
//} evalcache[MAXEVAL],rephash[REPHASH];
//POS int tablesize=NHASH;
//POS int use_hash=true;
//POS INT64 inhash,outhash,ineval,outeval;
//POS int hash_rnd[50][6];
//POS int eval_type=NORMAL;
//POS int pattern_use[49];
//POS int default_search=800000;
//POS int default_eval=0,stage=0;
//POS unsigned int history[MAXPLY+1][150][20];
//POS unsigned int countermove[2][150][150];
POS int invmap[93] ={
                        -1, -1, -1, -1, -1, -1, -1,
                        -1, -1, -1, -1, -1, -1,
                        -1,  0,  1,  2,  3,  4, -1,
                        5,  6,  7,  8,  9, -1,
                        -1, 10, 11, 12, 13, 14, -1,
                        15, 16, 17, 18, 19, -1,
                        -1, 20, 21, 22, 23, 24, -1,
                        25, 26, 27, 28, 29, -1,
                        -1, 30, 31, 32, 33, 34, -1,
                        35, 36, 37, 38, 39, -1,
                        -1, 40, 41, 42, 43, 44, -1,
                        45, 46, 47, 48, 49, -1,
                        -1, -1, -1, -1, -1, -1, -1,
                        -1, -1, -1, -1, -1, -1, -1,-1

                    };
//POS int progress[50]={
//                         9,   9,   9,   9,   9,
//                         8,   8,   8,   8,   8,
//                         7,   7,   7,   7,   7,
//                         6,   6,   6,   6,   6,
//                         5,   5,   5,   5,   5,
//                         4,   4,   4,   4,   4,
//                         3,   3,   3,   3,   3,
//                         2,   2,   2,   2,   2,
//                         1,   1,   1,   1,   1,
//                         0,   0,   0,   0,   0
//                     };
POS int map[50];
//POS int reverse_map[50],reverse_color[16];
//POS int kill_method=PROBKILL;
//POS double test_nr=0,test_e=0,test_e2=0;
//POS double test_t=0,test_t2=0;
//POS char pdnfile[100]={""},dcpfile[100]={""};
//POS int search_min,search_max;
//POS float perc=1.0;
//POS char ref_board[MAXREF][50];
//POS char ref_move[MAXREF][MOVEL];
//POS int ref_nr=0;
//POS int selective=true;
//POS int predeepning=300;
//POS int usexv=false;
//POS int usecol=true;
//POS int varCount[10];  // various counters
///* time control variables for time control per game */
//POS float timeUsed[2]={0,0};
//POS int timeControlMoves=130;  /* number of half-moves for first time control */
//POS float timeForFirstControl=300;  /* time to reach first time control */
//POS int timeControlMoves2=80;  /* number of half-moves for the second and next controls */
//POS float timeForSecondControl=60;  /* time to reach second and other time controls */
//POS float time_reserve=0;  /* time to keep in reserve before the next time control */
//POS float operator_time=0;  /* time to input move and press the clock in computer-computer games */
//
//
//POS struct _game_history {
//    unsigned char board[25];
//    char move[MOVEL];
//    char comment[MAXCOMMENT];
//    char tmp[MAXCOMMENT];
//} game_history[300];
//POS int game_history_nr=0;
//POS int game_history_max=0;
//POS int game_color=white;
//POS int game_history_firstcolor=0;
//
//POS struct _pdn_info {
//    char whitepl[100];
//    char blackpl[100];
//    char event[100];
//    char date[100];
//    char site[100];
//    char round[100];
//    char result[100];
//} pdn_info;
//
//#ifdef notdef
//int parameters[100]={0, 3, 0, 14, 12, 30, 32, -25, 30};
//int parameters[100]={0, 3, 2, 14, 16, 30, 20, -24, 59};
//int parameters[100]={0,  5,  8, 14,  0, 30, 20,-31, 67, 20, 49, 80, 12, 64, 0, 0, 0}; /* inital values */
//int    param_a[100]={0, 20, 10, 20, 30, 15,  6, 49, 62,  0, 87, 37,  7, 33,-16, 25,-91};
//int    param_b[100]={0, 20, 10, 10, 30, 15,  6, 49, 62,  0, 87, 37,  7, 33,  4, 25,-91};
//int    param_c[100]={0, 20, 10,  0, 30, 15,  6, 49, 62,  0, 87, 43,  7, 33, 30, 25,-91};
//int    param_a[100]={0, 20, 10, 20, 20, 15,  6, 49, 30,  15, 87, 37,  7, 40,-16, 25,-91};
//int    param_b[100]={0, 20, 10, 10, 20, 15,  6, 49, 30,  15, 87, 37,  7, 13,  4, 25,-91};
//int    param_c[100]={0, 20, 10,  0, 20, 15,  6, 49, 30,  15, 87, 43,  7, 13, 40, 25,-91};
//#endif
///*                   0, 22,  5,  8, 10,  0,  0,  0,  0,   0, 20, 62,  3, 20,  0, 28,  0*/
//POS int    param_a[100]={0, 8,  10,  3, 10,  9, 10,  0, 35,  60, 20, 40,  3, 20, 10, 30,  2};  // opening
//POS int    param_b[100]={0, 11,  15,  11, 10, 14, 10,  0, 35,  60, 20, 32,  3, 20, 34, 28,  4};  // middlegame
//POS int    param_c[100]={0, 11,  18,  11,  2, 22, 10,  0, 35,  60, 30, 25,  3,  0, 70, 25,  8};  // endgame
//
//POS int parameters[100]={0, 70,-19,-78, 30, -1, -1, 69,-52,-23, 87,-37,  7, 33,-16, 25,-91}; /* 186/384 stage b at 5 ply */
//
//int windows=true;
//int lastSearchDepth;
//int inDatabases;   /* boolean indicating whether the root position is in the databases */
//int ignoreDB1;  /* don't get theoretic value from this database_nr (used when root position is in the databases */
//int ignoreDB2;  /* don't get theoretic value from this database_nr */
//int allowTwoPlyIncrements=true;  /* allows the 'play' engine to increment search depth by 2 ply */
//int blockingPlay=0;  /* >0 mean try to avoid exchanges */
//int useBlockingPlay=false;
//int startMan=0;  /* number of man at beginning of search */
//int bookMode=1;  /* 0=no book, 1=default, 2=tournament mode */
//char breakThrough[2*128*6561];  /* breakthroug-table */
//int cntPos;
//int xray_w[93],xray_b[93];


