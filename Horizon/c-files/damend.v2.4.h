/*
  This file is Copyright (C) 1999 Harm Jetten.
  Modifications Copyright (C) 2010 Feike Boomstra.
  Distributed under the Boost Software License, Version 1.0.
  (See accompanying file LICENSE_1_0.txt or copy at
  http://www.boost.org/LICENSE_1_0.txt)
*/


#define ENDSIZE 16384L          /* size of end game position table (longs) */
#define ENDMASK (ENDSIZE - 1)   /* mask for end game position table index */
#define XBONUS   1048576L

//HINSTANCE hInst;
#define EHSIZE 8192L            /* size of end game database cache */
#define EHMASK (EHSIZE - 1)     /* mask for cache index */

#define EF 6                    /* endgame ref. table dimension per piece */

#define IDS_ENDLST "DAMEND.LST"
#define IDS_FEND "DAM.END"

#define INFIN 30000
#define E_MAGIC 0xDECAFF

#define DWORD off_t
#define USHORT unsigned short int
#define UCHAR unsigned char
#define ULONG unsigned int		// 64 bits system, not portable
#define BOOL int
#define UINT unsigned int

typedef struct {                /* end game info file structure */
    DWORD length;
    int pc_count;
    int idx;
    USHORT crc;
    char name[14];
    int mat_ofs;
    int fd;
    UCHAR *f_ptr;
} endhf;

typedef struct {                /* >=3-piece hash cache structure */
    ULONG hashval;
    long  eval;
} endhash;

extern char enddbdir[];         /* directory of end game database files */
extern int end_maxpc;           /* end game maximum nr of pieces on board */
extern const int inv_pc[];      /* inverted pieces */
extern endhf *end_ref[EF*EF*EF*EF]; /* references to endgame file info */

extern BOOL end_check;          /* using selective end game databases */
extern ULONG end_added;         /* nr of end game positions added */
extern ULONG end_collis;        /* nr of table entries already occupied */
extern ULONG end_total;         /* total nr of end game table entries */
extern ULONG end_acc[];         /* endgame db access statistics */
extern int end_pos[], end_pc[]; /* for endgame verification */
extern char file_spec[256];
extern char gp_text[];
extern int side;
extern int piece_count[2];
extern int piece_list[2][21];
extern int board[63];
extern UINT magic;
extern DWORD bytes;


extern BOOL estim_end(long *val, int ply);
extern void reset_enddb(void);
extern int check_enddb(endhf *ep);
extern BOOL endgame(long *valp);
extern BOOL endgame_5_6(long *valp);
extern BOOL find_end(void);
extern void add_end(void);
extern BOOL save_end(char *fname);
extern BOOL load_end(char *fname);
extern BOOL init_end(void);
extern void exit_end(void);
extern void comp_chkend(void);
