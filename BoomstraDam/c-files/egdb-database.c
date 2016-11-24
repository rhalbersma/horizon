//free_

/*
 * Copyright 1996-2004 by Michel D. Grimminck
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/*
   Endgame database generation program, based upon the Wu-Beal algorithm.
   This program can produce both 'Win/Draw/Lose' and 'Distance to Win' databases.
   
        
   The WU-BEAL algorithm
   =====================
   
   The algoritm was taken from:
   
   'A Memory Efficient Retrograde Algorithm and Its Application To Chinese Chess Endgames'
                             REN WU AND DONALD F. BEAL
   More Games of No Chance - MSRI Publications - Volume 42, 2002
   http://www.msri.org/publications/books/Book42/files/wu.pdf
   
   <start of quote>
   DATABASE W, B; // full databases (i.e. depth to win/loss for each position),
    // W for White-to-move positions, B for Black-to-move,
    // sequential access only
    SBITS S; // sequential access only bitmap
    RBITS R; // random access bitmap
    void TopLevel
        {
        DoInitialize();
        n = 0; // depth to mate or conversion
        while (!DoneWhite() && !DoneBlack())
            {
            if (!DoneWhite()) // last pass added new positions
            {
                S = Load(W, WIN_IN_N(n)); // S = WTM win_in_n
                R = Predecessor(S); // R = BTM predecessors of S
                S = Load(B, UNKNOWN); // S = BTM unknown
                S = S & R; // S = BTM maylose_in_n
                R = Load(W, WIN_<=_N(n)); // R = WTM win_in_n or less
                S = ProveSuccessor(S, R); // S = BTM lose_in_n
                B = Add(S, LOSE_IN_N(n)); // B += S
                if (dtm) // distance_to_mate?
                S = Load(B, LOSE_IN_N(n)); // S = BTM lose_in_n
                R = Predecessor(S); // R = WTM maybe win_in_n+1
                S = Load(W, UNKNOWN); // S = WTM unknown
                S = S & R; // S = WTM win_in_n+1
                W = Add(S, WIN_IN_N(n+1)); // W += S
                }
            if (!DoneBlack()) // done for BTM?
            {
                S = Load(B, WIN_IN_N(n)); // S = BTM win_in_n
                R = Predecessor(S); // R = WTM predecessors of S
                S = Load(W, UNKNOWN); // S = WTM unknown
                S = S & R; // S = WTM maylose_in_n
                R = Load(B, WIN_<=_N(n)); // R = BTM win_in_n or less
                S = ProveSuccessor(S, R); // S = WTM lose_in_n
                W = Add(S, LOSE_IN_N(n)); // W += S
                if (dtm) // distance_to_mate?
                S = Load(W, LOSE_IN_N(n)); // S = WTM lose_in_n
                R = Predecessor(S); // R = BTM maybe win_in_n+1
                S = Load(B, UNKNOWN); // S = BTM unknown
                S = S & R; // S = BTM win_in_n+1
                B = Add(S, WIN_IN_N(n+1)); // B += S
            }
            n = n + 1;
        }
    }
    <end of quote>
    
    It is all a bit more complicated than the basic algorithm, because the program
    needs to split up the databases in smaller fragments because of memory constraints;
    This causes a whole lot of complications.
    
    For the same reason, dragon also sometimes does some forward searches intermixed with
    the wu-beal algorithm.
    
    DATABASE STRUCTURE
    ==================
    
    A databases consist of a set of positions with a given number of white men,black men,
    white kings, black kings and the rank of the most leading white and black men.     
    
    This division is similar to chinook's databases.
    
    Program generates database with the following values:
    0  lose in 0
    1  win in 1 move
    2  lose in 2 moves
    3  win in 3
    ..
    etc
    253 temporary
    254 definite draw
    255 unknown
    
    ACCESSING THE DATABASE
    ======================
    To access the databases you need relavant functions and variables from
    the following files:
    
        index.c         The indexing function
        database.c      Various functions
        mem64.c         Memory management 
        const.h         Types and constants
        var.c,var.h     Some variables
        
    Example for retreiving a raw value from the database.
    init_databases();
    
    wm=bm=wc=bc=0;
    BOARD[F1]=black|man; bm++;
    BOARD[F2]=black|man; bm++;
    BOARD[F14]=white|crown; wc++;
    BOARD[F49]=white|man; wm++;
    BOARD[F50]=white|man; wm++;
    ws=findWS(wm,bm,bm,bc);
    bs=findBS(wm,bm,bm,bc);
    value=database_retrieve_value(white,wm,bm,bm,bc,ws,bs)
    

    PERFORMANCE INFO
    ================
    (previous versions, for historically reasons only)
    XXvXX log: pentium 100: adapted scheaffer algorithm, WDL
    db:0202, found=139512, time=80
    db:0202, found=10415, time=245
    db:0202, found=115, time=238
    db:0202, found=0, time=240

    XXvXX log: amd 1667: adapted scheaffer algorithm, WDL
    db:0202, found=139512, time=3
    db:0202, found=10415, time=9
    db:0202, found=115, time=1
    db:0202, found=0, time=0
    2v2 tot OOvOO: 160 sec

    Generation time for 3v2 + 4v1=4.8 hour

    (this version)
    XXvXX, amd 2000 MHz, wu-beal, WDL   13.5 sec
    XXXvXX wu-beal, WDL  423 sec
    XXXXvXX  wu-beal, WDL  8150 sec
*/



#include <stdio.h>
#include "egdb-const.h"
#include "egdb-var.h"
//#include "functions.h"
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include "BoomstraDam.v1.2.h"


#ifdef USE_ZLIB
#include "/usr/include/zlib.h"
#endif

#define DB_INDEX_FILE "C:\\Endgame\\endgame2.ini"
#define DB_UNKNOWN 3
#define DB_ALL 100
#define CAPTURE 100(clock()-startTime)/CLOCKS_PER_SEC
#define PROMOTE 101
#define DB_VERIFY 200
#define QMAX 700000000000LLU
#define QNAME "tmpgen/q%i"

#define WDL 0
#define DTW 1

#define MPLY 252   // maximal plydepth in database
/* uncomment to create databases by forward searches only. This is very slow
   and for testing only.
   
   #define FORWARDONLY
*/

int availableOnDisk(int,int,int,int,int,int);
int availableOnDiskExt(int,int,int,int,int,int,int);
int database_nr(int,int,int,int,int,int,int);
extern DBINDEX initDatabase(int,int,int,int,int,int,int,int,int,int,int);
extern DBINDEX db_count(int,int,int,int,int,int);
extern int mem64_allocate(INT64);
extern char *mem64_pointer(int,INT64,int);
extern int mem64_load(int,char *);
extern void init_index();
extern DBINDEX database_linear_index(int, int, unsigned long *, int, unsigned long *, int, unsigned long *, int, unsigned long *);
extern DBINDEX alt_database_linear_index(int, int, unsigned long *, int, unsigned long *, int, unsigned long *, int, unsigned long *);



FILE *logfile;
// number of positions in a database, index=database_nr()

int pos_count[4096*81];    
// size of database in bytes
DBINDEX bytesize[4096*81];   

// flag set if database passed verification
int verified[4096*81];       

/* The queues contains all positions that have a certain database value

   There are 4 different queue types
   0-255      positions with player 0 to move
   256-512    positions which have at least one successor of value n-256. Player 0 to move.
   512-767    positions with player 1 to move
   768-1023   positions which have at least one successor of value n-768. Player 1 to move
   
   notice: qsize must be signed
 */
//INT64 qsize[1024],qmax[2];

/* file pointer to queues */
//FILE *qfile[1024];  

// total amount of allocated memory in the memory handler
DBINDEX allocatedMemory;   

// maximal depth to win in any database, DTW metric
int maxDepth=0;   

// minimal number of iterations to complete current database
int minIteration=0; 

/* database metric mode
   WDL=Win/Draw/Lose, 2 bits per position
   DTW=Depth to win based, 8 bits per position
 */
int mode=WDL;

/* 'database' points to all databases with a maximum of 8 man and 8 crowns
   or contains NULL if the database is not present.

   All databases are computed for 'white to move'. If black is to move
   the board is reversed.
*/

#define DEFMAX 200000
int defMem64[DEFMAX];
DBINDEX defPOS[DEFMAX];
unsigned char defValue[DEFMAX];
int defCnt=0;


int countSliceWhite(int wman,int wcrown,int bman,int bcrown)
// returns the maximal number of white slices in a database-group
{
    if (wman+wcrown+bman+bcrown<=5) return(1);
	if (wman>0) return(9);
    
    return(1);
}

int countSliceBlack(int wman,int wcrown,int bman,int bcrown)
// returns the maximal number of black slices in a database-group
{
    if (wman+wcrown+bman+bcrown<=5) return(1);
    if (bman>0) return(9);
    return(1);
}

char *database_nameExt(int wman,int wcrown,int bman,int bcrown,int ws,int bs,int metric)
// returns the core filename of a database (without directory or extension)
// example filename: wld7-XXOvOOOO-45
{
    static char name[40];
    int a=0,i;

	if (metric==WDL) {
		name[a++]='w';
		name[a++]='d';
		name[a++]='l';
		name[a++]=48+wcrown+wman+bcrown+bman;
		name[a++]='-';
	} else {
		name[a++]='d';
		name[a++]='t';
		name[a++]='w';
		name[a++]=48+wcrown+wman+bcrown+bman;
		name[a++]='-';
	} 
	
    for(i=0;i<wcrown;i++) name[a++]='X';
    for(i=0;i<wman;i++) name[a++]='O';
    name[a++]='v';
    for(i=0;i<bcrown;i++) name[a++]='X';
    for(i=0;i<bman;i++) name[a++]='O';
    name[a++]='-';
    name[a++]=48+ws;
    name[a++]=48+bs;
    name[a]='\0';
    return(name);
}

char *database_name(int wman,int wcrown,int bman,int bcrown,int ws,int bs)
{
    return(database_nameExt(wman,wcrown,bman,bcrown,ws,bs,mode));
}

int load_database(int wman,int wcrown,int bman,int bcrown,int ws,int bs)
// loads a database into memory
// returns true if succesfull
{
    char db_file[100];
	char *id;
    DBINDEX i;
    int nr;
    DBINDEX size;
    int use_compression=false;
    char *p;

    id=database_name(wman,wcrown,bman,bcrown,ws,bs);
#ifdef USE_ZLIB
    sprintf(db_file,"databases/%s.raw.gz",id);
#else
	sprintf_s(db_file,100,"C:\\Endgame\\%s.raw",id);
#endif
    size=db_count(wman,wcrown,bman,bcrown,ws,bs);
    nr=database_nr(white,wman,wcrown,bman,bcrown,ws,bs);
    if (mem64db[nr] !=-1) return (true);  //already loaded

    pos_count[nr]=(int)size;
    bytesize[nr]=(size+3);
    if (mode==WDL) {
        bytesize[nr]=(size+3)/4;
		//printf("bytesize = %d\n", bytesize[nr]);
    }

    if (!availableOnDisk(wman,wcrown,bman,bcrown,ws,bs)) {
        printf("database %s not found\n",db_file);
        mem64db[nr]=-1;
        return(false);
    }
        
    mem64db[nr]=mem64_allocate(bytesize[nr]);
    if (mem64db[nr]<0) {
        printf("fatal: no memory for database (%i Mb)\n",bytesize[nr]/1024/1024);
        return false;
    }
    allocatedMemory+=sizeof(unsigned char)*bytesize[nr];

    strcat_s(id,40, "             ");
    id[16]=0;  // fix length: 123v5678-90.
    
    //printf("loading %s         \r",id);
    //fflush(stdout);
    /* make sure that the memory is mapped in ram to prevent excessive swapping */


    for(i=0;i<bytesize[nr];i+=256) {
        p=mem64_pointer(mem64db[nr],i,true);
        if (p==NULL) return (false);
		*p=0;
    }
	//printf("memory pointers set\n");

    if (use_compression==false) {
        mem64_load(mem64db[nr],db_file);
    }
	
	//printf("memory loaded\n");

    //if (windows==true) {
    //    printf("\n");
    //    printf("PROGRESS|*|*|*|*|loading %s\n",id);
    //}

    //printf("loading %s      (Size = %4u Mb, total memory = %4u Mb)      \n",id,(int) (bytesize[nr]/1024/1024),(int) (allocatedMemory/1024/1024));
    //printf("                                     \r");
    //fflush(stdout);
    return(true);
}

void load_databaseFull(int wman,int wcrown,int bman,int bcrown)
// loads all slices of the given database
{
    int ws,bs;
    int maxws,maxbs;
    
    maxws=countSliceWhite(wman,wcrown,bman,bcrown);
    maxbs=countSliceBlack(wman,wcrown,bman,bcrown);
    for (ws=0;ws<maxws;ws++) {
        for (bs=0;bs<maxbs;bs++) {
            load_database(wman,wcrown,bman,bcrown,ws,bs);
        }
    }
}


void init_databases(void)
{
    int i;

	sprintf_s(pagefile,255,"%s","c:\\Endgame\\temp\\mem64-%i-%i.page");

	for(i=0;i<4096*81;i++) {
        mem64db[i]=-1;
        verified[i]=false;
        //loadDatabaseOnDemand[i]=false;
        //dtwStatus[i]=0;
    }
    for(int p=0;p<93;p++) if (invmap[p]>=0) map[invmap[p]]=p;

    init_index();
}

void read_all_databases(int maxpieces)
{
	for (int tot_pieces=2; tot_pieces <= maxpieces; tot_pieces++)
	{	//printf("tot pieces: %d\n", tot_pieces);
		for (int white_pieces = 1; white_pieces < tot_pieces; white_pieces++)
		{	//printf("white pieces: %d\n", white_pieces);
			int black_pieces = tot_pieces - white_pieces;
			for (int white_man = 0; white_man <= white_pieces; white_man++)
			{	//printf("white man: %d\n", white_man);
				int white_king = white_pieces - white_man;
				for (int black_man = 0; black_man <= black_pieces; black_man++)
				{	int black_king = black_pieces - black_man;
					//printf("%d %d %d %d\n", white_man, white_king, black_man, black_king);
					int wsmax = countSliceWhite(white_man, white_king, black_man, black_king);
					int bsmax = countSliceBlack(white_man, white_king, black_man, black_king);
					for (int ws=0;ws<wsmax;ws++) 
					    for (int bs=0;bs<bsmax;bs++) 
						{	//printf("%d %d %d %d\n", white_man, white_king, black_man, black_king);
							load_database(white_man, white_king, black_man, black_king, ws, bs);
						}
				}
			}
		}
	}
}
int database_nr(int color,int wman,int wcrown,int bman,int bcrown,int ws,int bs)
/* returns the number of the requested database */
{
    int nr;

    if (color==white) nr=81*(wman+8*wcrown+64*bman+512*bcrown)+9*ws+bs;
    else nr=81*(bman+8*bcrown+64*wman+512*wcrown)+9*bs+ws;
    return(nr);
}

int alt_findWS(int wman, int lowest_white_man, int wcrown,int bman,int bcrown)
/* returns the white slice number of the current position
*/
{
	//int wm=white|man;
	
	if (wman==0) return(0);
	if (wman+wcrown+bman+bcrown<=5) return(0);
	
	switch (lowest_white_man)
	{
		case 5: return 8;
		case 6: return 8;
		case 7: return 8;
		case 8: return 8;
		case 9: return 8;
		case 10: return 7;
		case 11: return 7;
		case 12: return 7;
		case 13: return 7;
		case 14: return 7;
		case 15: return 6;
		case 16: return 6;
		case 17: return 6;
		case 18: return 6;
		case 19: return 6;
		case 20: return 5;
		case 21: return 5;
		case 22: return 5;
		case 23: return 5;
		case 24: return 5;
		case 25: return 4;
		case 26: return 4;
		case 27: return 4;
		case 28: return 4;
		case 29: return 4;
		case 30: return 3;
		case 31: return 3;
		case 32: return 3;
		case 33: return 3;
		case 34: return 3;
		case 35: return 2;
		case 36: return 2;
		case 37: return 2;
		case 38: return 2;
		case 39: return 2;
		case 40: return 1;
		case 41: return 1;
		case 42: return 1;
		case 43: return 1;
		case 44: return 1;
		case 45: return 0;
		case 46: return 0;
		case 47: return 0;
		case 48: return 0;
		case 49: return 0;
	}

	//if (board[F6]==wm || board[F7]==wm || board[F8]==wm || board[F9]==wm || board[F10]==wm) return(8);
	//if (board[F11]==wm || board[F12]==wm || board[F13]==wm || board[F14]==wm || board[F15]==wm) return(7);
	//if (board[F16]==wm || board[F17]==wm || board[F18]==wm || board[F19]==wm || board[F20]==wm) return(6);
	//if (board[F21]==wm || board[F22]==wm || board[F23]==wm || board[F24]==wm || board[F25]==wm) return(5);
	//if (board[F26]==wm || board[F27]==wm || board[F28]==wm || board[F29]==wm || board[F30]==wm) return(4);
	//if (board[F31]==wm || board[F32]==wm || board[F33]==wm || board[F34]==wm || board[F35]==wm) return(3);
	//if (board[F36]==wm || board[F37]==wm || board[F38]==wm || board[F39]==wm || board[F40]==wm) return(2);
	//if (board[F41]==wm || board[F42]==wm || board[F43]==wm || board[F44]==wm || board[F45]==wm) return(1);
	//if (board[F46]==wm || board[F47]==wm || board[F48]==wm || board[F49]==wm || board[F50]==wm) return(0);
	return (0);
}

int findWS(int wman, int lowest_white_man, int wcrown,int bman,int bcrown)
/* returns the white slice number of the current position
*/
{
	int wm=white|man;
	
	if (wman==0) return(0);
	if (wman+wcrown+bman+bcrown<=5) return(0);
	
	if (board[F6]==wm || board[F7]==wm || board[F8]==wm || board[F9]==wm || board[F10]==wm) return(8);
	if (board[F11]==wm || board[F12]==wm || board[F13]==wm || board[F14]==wm || board[F15]==wm) return(7);
	if (board[F16]==wm || board[F17]==wm || board[F18]==wm || board[F19]==wm || board[F20]==wm) return(6);
	if (board[F21]==wm || board[F22]==wm || board[F23]==wm || board[F24]==wm || board[F25]==wm) return(5);
	if (board[F26]==wm || board[F27]==wm || board[F28]==wm || board[F29]==wm || board[F30]==wm) return(4);
	if (board[F31]==wm || board[F32]==wm || board[F33]==wm || board[F34]==wm || board[F35]==wm) return(3);
	if (board[F36]==wm || board[F37]==wm || board[F38]==wm || board[F39]==wm || board[F40]==wm) return(2);
	if (board[F41]==wm || board[F42]==wm || board[F43]==wm || board[F44]==wm || board[F45]==wm) return(1);
	if (board[F46]==wm || board[F47]==wm || board[F48]==wm || board[F49]==wm || board[F50]==wm) return(0);
	return (0);
}

int alt_findBS(int wman,int wcrown,int bman,int highest_black_man, int bcrown)
/* returns the black slice number of the current position
*/
{
//	int bm=black|man;
	
	if (bman==0) return(0);
	if (wman+wcrown+bman+bcrown<=5) return(0);

	switch (highest_black_man)
	{
		case 0: return 0;
		case 1: return 0;
		case 2: return 0;
		case 3: return 0;
		case 4: return 0;
		case 5: return 1;
		case 6: return 1;
		case 7: return 1;
		case 8: return 1;
		case 9: return 1;
		case 10: return 2;
		case 11: return 2;
		case 12: return 2;
		case 13: return 2;
		case 14: return 2;
		case 15: return 3;
		case 16: return 3;
		case 17: return 3;
		case 18: return 3;
		case 19: return 3;
		case 20: return 4;
		case 21: return 4;
		case 22: return 4;
		case 23: return 4;
		case 24: return 4;
		case 25: return 5;
		case 26: return 5;
		case 27: return 5;
		case 28: return 5;
		case 29: return 5;
		case 30: return 6;
		case 31: return 6;
		case 32: return 6;
		case 33: return 6;
		case 34: return 6;
		case 35: return 7;
		case 36: return 7;
		case 37: return 7;
		case 38: return 7;
		case 39: return 7;
		case 40: return 8;
		case 41: return 8;
		case 42: return 8;
		case 43: return 8;
		case 44: return 8;
	}
	
	//if (board[F41]==bm || board[F42]==bm || board[F43]==bm || board[F44]==bm || board[F45]==bm) return(8);
	//if (board[F36]==bm || board[F37]==bm || board[F38]==bm || board[F39]==bm || board[F40]==bm) return(7);
	//if (board[F31]==bm || board[F32]==bm || board[F33]==bm || board[F34]==bm || board[F35]==bm) return(6);
	//if (board[F26]==bm || board[F27]==bm || board[F28]==bm || board[F29]==bm || board[F30]==bm) return(5);
	//if (board[F21]==bm || board[F22]==bm || board[F23]==bm || board[F24]==bm || board[F25]==bm) return(4);
	//if (board[F16]==bm || board[F17]==bm || board[F18]==bm || board[F19]==bm || board[F20]==bm) return(3);
	//if (board[F11]==bm || board[F12]==bm || board[F13]==bm || board[F14]==bm || board[F15]==bm) return(2);
	//if (board[F6]==bm || board[F7]==bm || board[F8]==bm || board[F9]==bm || board[F10]==bm) return(1);
	//if (board[F1]==bm || board[F2]==bm || board[F3]==bm || board[F4]==bm || board[F5]==bm) return(0);
	return (0);
}

int findBS(int wman,int wcrown,int bman,int highest_black_man, int bcrown)
/* returns the black slice number of the current position
*/
{
	int bm=black|man;
	
	if (bman==0) return(0);
	if (wman+wcrown+bman+bcrown<=5) return(0);

	if (board[F41]==bm || board[F42]==bm || board[F43]==bm || board[F44]==bm || board[F45]==bm) return(8);
	if (board[F36]==bm || board[F37]==bm || board[F38]==bm || board[F39]==bm || board[F40]==bm) return(7);
	if (board[F31]==bm || board[F32]==bm || board[F33]==bm || board[F34]==bm || board[F35]==bm) return(6);
	if (board[F26]==bm || board[F27]==bm || board[F28]==bm || board[F29]==bm || board[F30]==bm) return(5);
	if (board[F21]==bm || board[F22]==bm || board[F23]==bm || board[F24]==bm || board[F25]==bm) return(4);
	if (board[F16]==bm || board[F17]==bm || board[F18]==bm || board[F19]==bm || board[F20]==bm) return(3);
	if (board[F11]==bm || board[F12]==bm || board[F13]==bm || board[F14]==bm || board[F15]==bm) return(2);
	if (board[F6]==bm || board[F7]==bm || board[F8]==bm || board[F9]==bm || board[F10]==bm) return(1);
	if (board[F1]==bm || board[F2]==bm || board[F3]==bm || board[F4]==bm || board[F5]==bm) return(0);
	return (0);
}

int database_retrieve_value(int color,int wman,unsigned long * wman_idx, 
							          int wcrown,unsigned long * wcrown_idx, 
									  int bman,unsigned long * bman_idx, 
									  int bcrown,unsigned long * bcrown_idx, 
									  int ws,int bs)
/* retreives the value of a database entry for the current board position. The the piece count
   and slice numbers need to be specified for performance reasons.
   
   See also: read_value()
   
   Returns a value from the perspective of 'color'.
   */
{
    DBINDEX index,dindex;
    int score;
    unsigned char *db;
    static int counter=0;
    int handle;
    
    /* no pieces: return LOSE */
    if (color==white) if (wman==0 && wcrown==0) return(0);
    if (color==black) if (bman==0 && bcrown==0) return(0);
    handle=mem64db[database_nr(color,wman,wcrown,bman,bcrown,ws,bs)];
	if (handle == -1)
	{	open_log_file();
		fprintf_s(log_file, "can't load database %d, %d, %d, %d\n",wman,wcrown,bman,bcrown);
		_fcloseall();
		return 0;
	}
	//printf("nr:%i\n",database_nr(color,wman,wcrown,bman,bcrown,ws,bs));
    index=database_linear_index(color, wman, wman_idx, wcrown, wcrown_idx, bman, bman_idx, bcrown, bcrown_idx);
    if (index<0) {
        printf("fatal: exception 1, %llu\n", index);
        return 0;
    }//else return index; //temp
//  	if (mode==WDL) {
        dindex=index/4;
	    db=(unsigned char *)mem64_pointer(handle,dindex,false);
		if (db==NULL) return (0);
        switch(index-4*dindex) {
            case 0:{score=(*db >>6)&3;break;}
            case 1:{score=(*db >>4)&3;break;}
            case 2:{score=(*db >>2)&3;break;}
            case 3:{score=*db &3;break;}
        }
        if (score==0) score = -30000;
        else if (score==1) score = +30000;
		else if (score==2) score = 1;
		else if (score==3) score = 0;
        return(score);
	//} else {
	//    db=(unsigned char *)mem64_pointer(handle,index,false);  //read-only access
	//    return(*db);
	//}
}
int alt_database_retrieve_value(int color,int wman,unsigned long * wman_idx, 
							          int wcrown,unsigned long * wcrown_idx, 
									  int bman,unsigned long * bman_idx, 
									  int bcrown,unsigned long * bcrown_idx, 
									  int ws,int bs)
/* retreives the value of a database entry for the current board position. The the piece count
   and slice numbers need to be specified for performance reasons.
   
   See also: read_value()
   
   Returns a value from the perspective of 'color'.
   */
{
    DBINDEX index,dindex;
    int score;
    unsigned char *db;
    static int counter=0;
    int handle;
    
    /* no pieces: return LOSE */
    if (color==white) if (wman==0 && wcrown==0) return(0);
    if (color==black) if (bman==0 && bcrown==0) return(0);
    handle=mem64db[database_nr(color,wman,wcrown,bman,bcrown,ws,bs)];
	if (handle == -1)
	{	open_log_file();
		fprintf_s(log_file, "can't load database %d, %d, %d, %d\n",wman,wcrown,bman,bcrown);
		_fcloseall();
		return 0;
	}
	//printf("nr:%i\n",database_nr(color,wman,wcrown,bman,bcrown,ws,bs));
    index=alt_database_linear_index(color, wman, wman_idx, wcrown, wcrown_idx, bman, bman_idx, bcrown, bcrown_idx);
    if (index<0) {
        printf("fatal: exception 1, %llu\n", index);
        return 0;
    }//else return index;
//  	if (mode==WDL) {
        dindex=index/4;
	    db=(unsigned char *)mem64_pointer(handle,dindex,false);
		if (db==NULL) return (0);
        switch(index-4*dindex) {
            case 0:{score=(*db >>6)&3;break;}
            case 1:{score=(*db >>4)&3;break;}
            case 2:{score=(*db >>2)&3;break;}
            case 3:{score=*db &3;break;}
        }
        if (score==0) score = -30000;
        else if (score==1) score = +30000;
		else if (score==2) score = 1;
		else if (score==3) score = 0;
        return(score);
	//} else {
	//    db=(unsigned char *)mem64_pointer(handle,index,false);  //read-only access
	//    return(*db);
	//}
}

int availableOnDiskExt(int wman,int wcrown,int bman,int bcrown,int ws,int bs,int metric)
/* returns true if database is available on disk */
{
	char db_filename[100],*id;

	FILE *in;

    id=database_nameExt(wman,wcrown,bman,bcrown,ws,bs,metric);
	
    #ifdef USE_ZLIB
        sprintf(db_filename,"databases/%s.raw.gz",id);
    #else
		sprintf_s(db_filename,100,"C:\\Endgame\\%s.raw",id);
    #endif
    errno_t err = fopen_s(&in, db_filename, "rb");
    if ((err)) return (false);
    fclose(in);
    return (true);
}

int availableOnDisk(int wman,int wcrown,int bman,int bcrown,int ws,int bs)
{
    return(availableOnDiskExt(wman,wcrown,bman,bcrown,ws,bs,mode));
}

int availableOnDiskFull(int wman,int wcrown,int bman,int bcrown)
/* returns true if the given database is fully available on disk, for all its slices */
{
    int ws,bs;
    
    ws=countSliceWhite(wman,wcrown,bman,bcrown)-1;
    bs=countSliceBlack(wman,wcrown,bman,bcrown)-1;
    return availableOnDisk(wman,wcrown,bman,bcrown,0,0);
}
extern void Initialize_endgame(void)
{	init_databases();
	read_all_databases(4);
}

extern int evaluate_endgame_position(char color_white, 
									 int nr_of_white_man, unsigned long * white_man_idx, 
									 int nr_of_white_king, unsigned long * white_king_idx, 
									 int nr_of_black_man, unsigned long * black_man_idx, 
									 int nr_of_black_king, unsigned long * black_king_idx)
// white_man_idx etc. in internal BoomstraDam representation: 0..9, 11..20, 22..31, 33..42, 44..53
{	int i,ws,bs,color;
	for (i=0; i<50; i++) board[map[i]] = empty;
	for (i=0; i<nr_of_white_man; i++)  {board[map[int_to_ext[white_man_idx [i]] - 1]] = white|man; /*printf("from: %d mapped to: %d\n", white_man_idx [i], int_to_ext[white_man_idx [i]] - 1);*/};
	for (i=0; i<nr_of_white_king; i++) board[map[int_to_ext[white_king_idx[i]] - 1]] = white|crown;
	for (i=0; i<nr_of_black_man; i++)  {board[map[int_to_ext[black_man_idx [i]] - 1]] = black|man;/*printf("from: %d mapped to: %d\n", black_man_idx [i], int_to_ext[black_man_idx [i]] - 1);*/};
	for (i=0; i<nr_of_black_king; i++) board[map[int_to_ext[black_king_idx[i]] - 1]] = black|crown;

	ws = findWS(nr_of_white_man, white_man_idx[0], nr_of_white_king, nr_of_black_man, nr_of_black_king);
	bs = findBS(nr_of_white_man, nr_of_white_king, nr_of_black_man, black_man_idx[nr_of_black_man-1], nr_of_black_king);
	if (color_white) color = white; else color = black;

	return database_retrieve_value(color, nr_of_white_man,  white_man_idx, 
		                                  nr_of_white_king, white_king_idx, 
										  nr_of_black_man,  black_man_idx,
										  nr_of_black_king, black_king_idx,
										  ws, bs);
}

extern int alt_evaluate_endgame_position(char color_white, 
									 int nr_of_white_man, unsigned long * white_man_idx, 
									 int nr_of_white_king, unsigned long * white_king_idx, 
									 int nr_of_black_man, unsigned long * black_man_idx, 
									 int nr_of_black_king, unsigned long * black_king_idx)

	// white_man_idx etc. in internal BoomstraDam representation: 0..9, 11..20, 22..31, 33..42, 44..53

{	int i,ws,bs,color;
	unsigned long loc_white_man_idx[21], loc_white_king_idx[21], loc_black_man_idx[21], loc_black_king_idx[21];
	
for (i=0; i<nr_of_white_man; i++){  loc_white_man_idx [i] = int_to_ext[white_man_idx [i]] - 1; /*printf("white man: %d\n", white_man_idx [i]);*/}
	for (i=0; i<nr_of_white_king; i++) loc_white_king_idx[i] = int_to_ext[white_king_idx[i]] - 1;
	for (i=0; i<nr_of_black_man; i++)  {loc_black_man_idx [i] = int_to_ext[black_man_idx [i]] - 1;/*printf("black man: %d\n", black_man_idx [i]);*/}
	for (i=0; i<nr_of_black_king; i++) loc_black_king_idx[i] = int_to_ext[black_king_idx[i]] - 1;


	ws = alt_findWS(nr_of_white_man, loc_white_man_idx[0], nr_of_white_king, nr_of_black_man, nr_of_black_king);
	bs = alt_findBS(nr_of_white_man, nr_of_white_king, nr_of_black_man, loc_black_man_idx[nr_of_black_man-1], nr_of_black_king);
	if (color_white) color = white; else color = black;

	return alt_database_retrieve_value(color, nr_of_white_man,  loc_white_man_idx, 
		                                  nr_of_white_king, loc_white_king_idx, 
										  nr_of_black_man,  loc_black_man_idx,
										  nr_of_black_king, loc_black_king_idx,
										  ws, bs);
}

