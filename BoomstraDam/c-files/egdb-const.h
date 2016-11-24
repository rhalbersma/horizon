/* strings */




#define VERSION "1.2 (3-1-2004)"
#define WORKDIR "/usr/local/lib/dragon/"
 
#define true 1
#define false 0

/* kill method */
#define KILLER 1
#define HISTORY 2
#define PROBKILL 3

/* maxima */
#define MAXPLY 64
#define MAXREF 5
#define NHASH 10000 /* 160000 40000 */
#define MAXBOOK 50000 /* 25000  5000 */
#define MAXEVAL 300000 /* 25000 10000 */
#define MAXCOMMENT 256
#define MOVEL 64
#define MPV 25
#define MAXNM 128
#define MAXEXTEND 5
#define NPAT 1200
#define PATTREE 70000
#define REPHASH 300

/* piece encodings */
#define white 0
#define black 1
#define empty 0
#define man 2
#define crown 4
#define edge 8
#define taken 16
#define invalid 92
#define OPTION 127
#define OWN 0
#define OPP 1
#define FREE 2
#define BORDER 3

/* directions */
#define forleft 0
#define forright 1
#define backright 2
#define backleft 3

/* evaluation encoding */
#define LOSE -200000
#define WIN 200000
#define DRAW 0
#define INF 400000
#define NONE -1
#define UNKNOWN (INF+22354)
#define PN_INF 5000000
#define PN_PROVEN 0
#define PN_DISPROVEN 1
#define EXACTDRAW 0
#define ATLEASTDRAW 1
#define ATMOSTDRAW 2
#define NOTEXACT 3

/* database entries */
#define DB_WIN 1
#define DB_DRAW 254
#define DB_LOSE 0

/* search types for pn */
#define WINTHEO 0
#define WINHEUR 1
#define DRAWTHEO 2

/* evaluation types */
#define NORMAL 0
#define BASIC 1
#define AB 2
#define PN 3
#define PNTWO 4
#define EBOOK 5

/* search flags */
#define EXTEND_TAKE 63

/* tables */
#define BOOK 0
#define P6 1
#define P8 2
#define P10 3
#define MISC 4
#define NTABLE 5

#define T_LOCKED 0
#define T_UNLOADED 1
#define T_UNCHANGED 2
#define T_CHANGED 3

/* fields: enter 'makef' to generate this list! */
#define F1 14
#define F2 15
#define F3 16
#define F4 17
#define F5 18
#define F6 20
#define F7 21
#define F8 22
#define F9 23
#define F10 24
#define F11 27
#define F12 28
#define F13 29
#define F14 30
#define F15 31
#define F16 33
#define F17 34
#define F18 35
#define F19 36
#define F20 37
#define F21 40
#define F22 41
#define F23 42
#define F24 43
#define F25 44
#define F26 46
#define F27 47
#define F28 48
#define F29 49
#define F30 50
#define F31 53
#define F32 54
#define F33 55
#define F34 56
#define F35 57
#define F36 59
#define F37 60
#define F38 61
#define F39 62
#define F40 63
#define F41 66
#define F42 67
#define F43 68
#define F44 69
#define F45 70
#define F46 72
#define F47 73
#define F48 74
#define F49 75
#define F50 76

#define BTYPE unsigned char
#define INT64 long long
#define DBINDEX INT64

#define MAPPEDMEMORY

#define RBOARD(c,p,x,y) board[nextall[c][p][10+x][10+y]]
#define LBOARD(c,p,x,y) local[nextall[c][p][10+x][10+y]]
#define LWBOARD(p,x,y) local[nextall[0][p][10+x][10+y]]

#define PAT1 1
#define PAT2 2
#define PAT3 4
#define PAT4 8
#define PAT5 16
#define PAT6 32
#define PAT7 64
#define PAT8 128
#define PAT9 256
#define PAT10 512
#define PAT11 1024
#define PAT12 2048
#define PAT13 4096
#define PAT14 8192

/* time control options */
#define TCM_TIME_PER_MOVE 0
#define TCM_MAXPLY 1
#define TCM_TIME_PER_GAME 2


