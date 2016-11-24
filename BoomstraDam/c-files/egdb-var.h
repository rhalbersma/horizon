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
/*#define POS extern
#include "var.c"
#ifdef notdef*/


extern int lmap[10][10];
extern char next[2][93][4];
extern char nextall[2][93][20][20];
extern char takeback[4][4][4][4][4][4];
extern BTYPE board[93],blocked[93];
extern int pieces[8];
extern char promote[2][93];
extern char movelist[MAXPLY][MAXNM][MOVEL];
extern char killer[MAXPLY][MOVEL];
extern struct _movescore {
    char move[MOVEL];
    int value;
    } movescore[MAXNM];
extern int tomove;
extern struct _tpat
{
  char board[50];
  int use_succes;
  int use_fail;
  int nmoves;
  char movelist[12][32];
} tpat[NPAT];

extern INT64 nsort,neval,ngen,ndat,nmat,nmovelist,nquiet,nquietfail,precount,dbfail,pat_try,pat_found,pat_succes;
extern INT64 tneval;
extern char PV[MPV][MPV][MOVEL];
extern int deval[MAXPLY];
extern int quiescence;
extern int do_order;
extern int do_res,maxpv;
#ifdef MAPPEDMEMORY
    extern int mem64db[4096];
#else
    extern unsigned char *database[4096*81];
#endif
extern int db_usage[4096];
extern int loadDatabaseOnDemand[4096*81];
extern int dtwStatus[4096*81];   //0= don't know, 1=not available, 2=available
extern int use_db;
typedef struct  {
    unsigned int hashkey;
    int min_score;
    int max_score;
    unsigned char board[18];
    char flags;
    char move[2];
    short int depth;
} tpTranspos;
extern tpTranspos *transpos;
extern struct _evalcache {
    unsigned int hashkey;
    unsigned char board[18];
    int score;
  } evalcache[MAXEVAL],rephash[REPHASH];
extern int tablesize;
extern int use_hash;
extern INT64 inhash,outhash,ineval,outeval;
extern int hash_rnd[50][6];
extern int eval_type;
extern int pattern_use[49];
extern int default_search;
extern int default_eval,stage;
extern unsigned int history[20][150][20];
extern unsigned int countermove[2][150][150];
extern int invmap[128];
extern int map[50],reverse_map[50],reverse_color[16];
extern int kill_method;
extern double test_nr,test_e,test_e2;
extern double test_t,test_t2;
extern int parameters[100],param_a[100],param_b[100],param_c[100];
extern char pdnfile[100],dcpfile[100];
extern int search_min,search_max;
extern float perc;
extern char ref_board[MAXREF][50];
extern char ref_move[MAXREF][MOVEL];
extern int ref_nr;
extern struct _game_history {
    unsigned char board[25];
    char move[MOVEL];
    char comment[MAXCOMMENT];
    char tmp[MAXCOMMENT];
} game_history[200];
extern struct _pdn_info {
    char whitepl[100];
    char blackpl[100];
    char event[100];
    char date[100];
    char site[100];
    char round[100];
    char result[100];
} pdn_info;
extern int progress[50];
extern char workdir[256],version[128];
extern int game_history_nr,game_history_max,game_color,game_history_firstcolor;
extern int selective,predeepning;
extern int usexv;
extern int usecol;
extern float time_left[2];
extern float time_incr;
extern INT64 mem64_diskActivity;
extern int xray_w[93],xray_b[93];
extern int windows;
extern int stopflag;

extern float timeUsed[2];
extern int timeControlMoves;  /* number of half-moves for first time control */
extern float timeForFirstControl;  /* time to reach first time control */
extern int timeControlMoves2;  /* number of half-moves for the second and next controls */
extern float timeForSecondControl;  /* time to reach second and other time controls */
extern float time_reserve;  /* time to keep in reserve before the next time control */
extern float operator_time;  /* time to input move and press the clock in computer-computer games */

extern int lastSearchDepth;
extern int inDatabases;
extern int ignoreDB1;
extern int ignoreDB2;
extern int allowTwoPlyIncrements;
extern int blockingPlay; 
extern int useBlockingPlay;
extern int startMan;
extern int mapTo4[100];
extern int bookMode;
extern char breakThrough[2*128*6561];
extern int cntPos;
extern int varCount[10];  // various counters
extern char *db33;
extern int db33a;
extern int db33b;
extern char pagefile[255];






