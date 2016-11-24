/*
  This file is Copyright (C) 2010 Feike Boomstra.
  Distributed under the Boost Software License, Version 1.0.
  (See accompanying file LICENSE_1_0.txt or copy at
  http://www.boost.org/LICENSE_1_0.txt)
*/


#define NDEBUG 1
#include <cassert>
#include <signal.h>
#include <stdio.h>
#include <cstdlib>

#define IS_WHITE_ON_MOVE 0x1					/* the color of the next move in this position, WHITE = 1, BLACK = 0 */
#define INV_WHITE_ON_MOVE 0xFFFE
#define IS_MAX_NODE 0x2						/* the nature of the node: MAX_NODE = 1, MIN_NODE = 0 */
#define IS_EVALUATED 0x4					/* is the node allready evaluated */
#define IS_USELESS_SACRIFICE 0x8					// too much losses
#define IS_NOT_IN_HASH 0x10					// node is not found in the hash (anymore)
#define IS_EXACT_VALUE 0x20					// evaluation value is coming from the endgame database
#define IS_CYCLE_POSSIBLE 0x40              // both sides a king and no move done with man
#define IS_JUST_PROMOTED 0x80               // color on move promoted in this ply
#define IS_EXTENDED 0x100					// is extended in this branch
#define HAS_EXTENSION_CONDITION 0x200       // to signal extension condition appearing for the firt time
#define IS_QUIESCENCE 0x400
#define HAD_TO_CAPTURE 0x800
#define HAD_ONCE_A_WHITE_KING 0x1000
#define HAD_ONCE_A_BLACK_KING 0x2000
#define HASH_ARRAY_SIZE 0x4000000				/* the related array size [0..HASH_ARRAY_SIZE -1] */
#define MASK_HASH HASH_ARRAY_SIZE - 1				/* the mask according to this size */
#define WHITE_HASH (0x5555555555555555 & MASK_HASH) // to distinguish between white on move or black on move
#define INFINITY  32767						/* used in the minmax routines */
#define NR_OF_ENDGAME_PIECES 6

//------------------ useless sacrifice ------------------------------

#define MIN_NODE_DID_FREE_MOVE 0
#define MAX_NODE_DID_FREE_MOVE 1

#define MIN_CAPTURED 10
#define MAX_CAPTURED 11
#define RE_CAPTURED 12

#define MAX_SACRIFICED 1
#define MIN_SACRIFICED 2
#define BOTH_SACRIFICED (MAX_SACRIFICED|MIN_SACRIFICED)

#define PROMOTION_EXTENSION 0

#define NUM_OF_THREADS 4
#define THREAD_MAX NUM_OF_THREADS

#define NR_OF_WEIGHTS 31


#define nw_dir (-6)
#define ne_dir (-5)
#define sw_dir (+5)
#define se_dir (+6)

#define BIT0 0x1ULL
#define BIT1 0x2ULL
#define BIT2 0x4ULL
#define BIT3 0x8ULL
#define BIT4 0x10ULL
#define BIT5 0x20ULL
#define BIT6 0x40ULL
#define BIT7 0x80ULL
#define BIT8 0x100ULL
#define BIT9 0x200ULL
#define BIT10 0x400ULL
#define BIT11 0x800ULL
#define BIT12 0x1000ULL
#define BIT13 0x2000ULL
#define BIT14 0x4000ULL
#define BIT15 0x8000ULL
#define BIT16 0x10000ULL
#define BIT17 0x20000ULL
#define BIT18 0x40000ULL
#define BIT19 0x80000ULL
#define BIT20 0x100000ULL
#define BIT21 0x200000ULL
#define BIT22 0x400000ULL
#define BIT23 0x800000ULL
#define BIT24 0x1000000ULL
#define BIT25 0x2000000ULL
#define BIT26 0x4000000ULL
#define BIT27 0x8000000ULL
#define BIT28 0x10000000ULL
#define BIT29 0x20000000ULL
#define BIT30 0x40000000ULL
#define BIT31 0x80000000ULL
#define BIT32 0x100000000ULL
#define BIT33 0x200000000ULL
#define BIT34 0x400000000ULL
#define BIT35 0x800000000ULL
#define BIT36 0x1000000000ULL
#define BIT37 0x2000000000ULL
#define BIT38 0x4000000000ULL
#define BIT39 0x8000000000ULL
#define BIT40 0x10000000000ULL
#define BIT41 0x20000000000ULL
#define BIT42 0x40000000000ULL
#define BIT43 0x80000000000ULL
#define BIT44 0x100000000000ULL
#define BIT45 0x200000000000ULL
#define BIT46 0x400000000000ULL
#define BIT47 0x800000000000ULL
#define BIT48 0x1000000000000ULL
#define BIT49 0x2000000000000ULL
#define BIT50 0x4000000000000ULL
#define BIT51 0x8000000000000ULL
#define BIT52 0x10000000000000ULL
#define BIT53 0x20000000000000ULL
#define BIT54 0x40000000000000ULL
#define BIT55 0x80000000000000ULL
#define BIT56 0x100000000000000ULL
#define BIT57 0x200000000000000ULL
#define BIT58 0x400000000000000ULL
#define BIT59 0x800000000000000ULL
#define BIT60 0x1000000000000000ULL
#define BIT61 0x2000000000000000ULL
#define BIT62 0x4000000000000000ULL
#define BIT63 0x8000000000000000ULL



// the bit representation of the fields of the board
#define FLD0 0x0ULL
#define FLD1 0x1ULL
#define FLD2 0x2ULL
#define FLD3 0x4ULL
#define FLD4 0x8ULL
#define FLD5 0x10ULL
#define FLD6 0x20ULL
#define FLD7 0x40ULL
#define FLD8 0x80ULL
#define FLD9 0x100ULL
#define FLD10 0x200ULL
#define FLD11 0x800ULL
#define FLD12 0x1000ULL
#define FLD13 0x2000ULL
#define FLD14 0x4000ULL
#define FLD15 0x8000ULL
#define FLD16 0x10000ULL
#define FLD17 0x20000ULL
#define FLD18 0x40000ULL
#define FLD19 0x80000ULL
#define FLD20 0x100000ULL
#define FLD21 0x400000ULL
#define FLD22 0x800000ULL
#define FLD23 0x1000000ULL
#define FLD24 0x2000000ULL
#define FLD25 0x4000000ULL
#define FLD26 0x8000000ULL
#define FLD27 0x10000000ULL
#define FLD28 0x20000000ULL
#define FLD29 0x40000000ULL
#define FLD30 0x80000000ULL
#define FLD31 0x200000000ULL
#define FLD32 0x400000000ULL
#define FLD33 0x800000000ULL
#define FLD34 0x1000000000ULL
#define FLD35 0x2000000000ULL
#define FLD36 0x4000000000ULL
#define FLD37 0x8000000000ULL
#define FLD38 0x10000000000ULL
#define FLD39 0x20000000000ULL
#define FLD40 0x40000000000ULL
#define FLD41 0x100000000000ULL
#define FLD42 0x200000000000ULL
#define FLD43 0x400000000000ULL
#define FLD44 0x800000000000ULL
#define FLD45 0x1000000000000ULL
#define FLD46 0x2000000000000ULL
#define FLD47 0x4000000000000ULL
#define FLD48 0x8000000000000ULL
#define FLD49 0x10000000000000ULL
#define FLD50 0x20000000000000ULL

// for black:
#define BLD0 FLD0
#define BLD1 FLD50
#define BLD2 FLD49
#define BLD3 FLD48
#define BLD4 FLD47
#define BLD5 FLD46
#define BLD6 FLD45
#define BLD7 FLD44
#define BLD8 FLD43
#define BLD9 FLD42
#define BLD10 FLD41
#define BLD11 FLD40
#define BLD12 FLD39
#define BLD13 FLD38
#define BLD14 FLD37
#define BLD15 FLD36
#define BLD16 FLD35
#define BLD17 FLD34
#define BLD18 FLD33
#define BLD19 FLD32
#define BLD20 FLD31
#define BLD21 FLD30
#define BLD22 FLD29
#define BLD23 FLD28
#define BLD24 FLD27
#define BLD25 FLD26
#define BLD26 FLD25
#define BLD27 FLD24
#define BLD28 FLD23
#define BLD29 FLD22
#define BLD30 FLD21
#define BLD31 FLD20
#define BLD32 FLD19
#define BLD33 FLD18
#define BLD34 FLD17
#define BLD35 FLD16
#define BLD36 FLD15
#define BLD37 FLD14
#define BLD38 FLD13
#define BLD39 FLD12
#define BLD40 FLD11
#define BLD41 FLD10
#define BLD42 FLD9
#define BLD43 FLD8
#define BLD44 FLD7
#define BLD45 FLD6
#define BLD46 FLD5
#define BLD47 FLD4
#define BLD48 FLD3
#define BLD49 FLD2
#define BLD50 FLD1




// The internal index of the fields:
#define F1 0
#define F2 1
#define F3 2
#define F4 3
#define F5 4
#define F6 5
#define F7 6
#define F8 7
#define F9 8
#define F10 9
#define F11 11
#define F12 12
#define F13 13
#define F14 14
#define F15 15
#define F16 16
#define F17 17
#define F18 18
#define F19 19
#define F20 20
#define F21 22
#define F22 23
#define F23 24
#define F24 25
#define F25 26
#define F26 27
#define F27 28
#define F28 29
#define F29 30
#define F30 31
#define F31 33
#define F32 34
#define F33 35
#define F34 36
#define F35 37
#define F36 38
#define F37 39
#define F38 40
#define F39 41
#define F40 42
#define F41 44
#define F42 45
#define F43 46
#define F44 47
#define F45 48
#define F46 49
#define F47 50
#define F48 51
#define F49 52
#define F50 53


#define BITS50  \
		   (FLD1  | FLD2  | FLD3  | FLD4  | FLD5  | FLD6  | FLD7  | FLD8  | FLD9  | FLD10 | \
			FLD11 | FLD12 | FLD13 | FLD14 | FLD15 | FLD16 | FLD17 | FLD18 | FLD19 | FLD20 | \
			FLD21 | FLD22 | FLD23 | FLD24 | FLD25 | FLD26 | FLD27 | FLD28 | FLD29 | FLD30 | \
			FLD31 | FLD32 | FLD33 | FLD34 | FLD35 | FLD36 | FLD37 | FLD38 | FLD39 | FLD40 | \
			FLD41 | FLD42 | FLD43 | FLD44 | FLD45 | FLD46 | FLD47 | FLD48 | FLD49 | FLD50 )

// some special parts:

#define DIAGONAL (FLD5 | FLD10 | FLD14 | FLD19 | FLD23 | FLD28 | FLD32 | FLD37 | FLD41 | FLD46)

#define FIELDS_01_05 (FLD1 | FLD2 | FLD3 | FLD4 | FLD5)					// mask for upper row
#define FIELDS_46_50 (FLD46 | FLD47 | FLD48 | FLD49 | FLD50)			// mask for lower row

#define NEARLY_BLACK_KING (FLD36 | FLD37 | FLD38 | FLD39 | FLD40 | FLD41 | FLD42 | FLD43 | FLD44 | FLD45)
#define NEARLY_WHITE_KING (FLD6  | FLD7  | FLD8  | FLD9  | FLD10 | FLD11 | FLD12 | FLD13 | FLD14 | FLD15)

#define ROW1  (FLD1|FLD2|FLD3|FLD4|FLD5)
#define ROW2  (FLD6|FLD7|FLD8|FLD9|FLD10)
#define ROW3  (FLD11|FLD12|FLD13|FLD14|FLD15)
#define ROW4  (FLD16|FLD17|FLD18|FLD19|FLD20)
#define ROW5  (FLD21|FLD22|FLD23|FLD24|FLD25)
#define ROW6  (FLD26|FLD27|FLD28|FLD29|FLD30)
#define ROW7  (FLD31|FLD32|FLD33|FLD34|FLD35)
#define ROW8  (FLD36|FLD37|FLD38|FLD39|FLD40)
#define ROW9  (FLD41|FLD42|FLD43|FLD44|FLD45)
#define ROW10 (FLD46|FLD47|FLD48|FLD49|FLD50)


// some macro's to deal with 64 bit operations on bits

#define count_bits_and_fill_list(bit_var, nr_of_bits,  bit_list)\
{unsigned long long all64 = bit_var; nr_of_bits=0;\
while (1) { if (all64 == 0) break; bit_list[nr_of_bits++] =  __builtin_ctzll(all64); all64 = (all64 & (all64 - 1));}}
#define count_bits(bit_var, nr_of_bits) {nr_of_bits =  __builtin_popcountll((bit_var)); }
#define add_count_bits(bit_var, nr_of_bits, amount) {BitArray temp__=bit_var; while (temp__){temp__ = temp__ & (temp__-1); nr_of_bits += amount;}}
#define all_bits_set(question, constant) (((question) & (constant)) == (constant))
#define all_white(bits) (all_bits_set(white_man,(bits)))
#define all_black(bits) (all_bits_set(black_man,(bits)))
#define all_empty(bits) (all_bits_set(empty_field, (bits)))
#define all_empty_or_white(bits) (all_bits_set((empty_field|white_man), (bits)))
#define all_empty_or_black(bits) (all_bits_set((empty_field|black_man), (bits)))
#define all_occupied(bits) (all_bits_set((white_man|black_man),(bits)))
#define occupied (white_man|black_man)
#define uint32 unsigned int
#define count_white(bit_var) (__builtin_popcountll(((bit_var) & white_man)))
#define count_black(bit_var) (__builtin_popcountll(((bit_var) & black_man)))
#define count(bit_var) (__builtin_popcountll((bit_var)))
#define	occ  (white_man | black_man)
#define empty_or_white (empty_field | white_man)
#define empty_or_black (empty_field | black_man)
#define white_or_empty empty_or_white
#define black_or_empty empty_or_black
#define any_white(bits) (white_man & (bits))
#define any_black(bits) (black_man & (bits))

#define lock_init(x, y) pthread_mutex_init(x, y)
#define lock_grab(x)    pthread_mutex_lock(x)
#define lock_release(x) pthread_mutex_unlock(x)
#define lock_destroy(x) pthread_mutex_destroy(x)

#define hash_lock_init(x, y) pthread_mutex_init(x, y)
#define hash_lock_grab(x)    pthread_mutex_lock(x)
#define hash_lock_release(x) pthread_mutex_unlock(x)
/*
#define hash_lock_init(x, y)
#define hash_lock_grab(x)
#define hash_lock_release(x)
*/
// struct definitions

struct stand_type {
	unsigned long long man_king;				/* bit array [0..] man = 1 king = 0   */
	unsigned long long white_black;				/* bit array [0..] white = 1 black = 0 . */
	unsigned long long occ_empty;				/* bit array [0..] occupied = 1 empty = 0 */
};

struct hash_node {
	unsigned long long man_king;				/* bit array [0..] man = 1 king = 0   */
	unsigned long long white_black;				/* bit array [0..] white = 1 black = 0 . */
	unsigned long long occ_empty;				/* bit array [0..] occupied = 1 empty = 0 */
	short int upperbound;						/* upperbound used in alphabeta pruning */
	short int lowerbound;						/* lowerbound used in alphabeta pruning */
	short int distance_from_root;
	unsigned short int status;
//	char search_depth;							/* to check whether the boundaries has to be re-calculated for a new depth */
	short int search_depth;
	unsigned char best_move_idx;				// the best move from a previous search depth
	//unsigned char move_status;                  // this is to debug
	char depth;									// debug

};

struct tnode {
	unsigned long long man_king;				/* bit array [0..] man = 1 king = 0   */
	unsigned long long white_black;				/* bit array [0..] white = 1 black = 0 . */
	unsigned long long occ_empty;				/* bit array [0..] occupied = 1 empty = 0 */
	unsigned int hash_key;						/* the hash key related to the board position */
	unsigned short int status;
	unsigned char nr_of_white_man;
	unsigned char nr_of_black_man;
	unsigned char nr_of_white_king;
	unsigned char nr_of_black_king;
	unsigned char total_nr_of_pieces;
//	unsigned char move_status;                  // this is to debug
	char depth;									// debug
//	char mutual_sacrifice;                      // to detect, whether bother sides are giving away pieces alternatively
	struct tnode *previous_node;
};
struct move_type {
	unsigned long long start;							/* the start position of the piece that moves (or captures) */
	unsigned long long stop;							/* the stop position of the piece that moves or captures */
	unsigned long long captured;		/* bit array of pieces that are captured in this move */
};

struct opening_book_move_type{
	int Start;
	int Stop;
	int Next;
} ;
struct game_list {
	int Start;
	int Stop;
};

struct opening_book_position_type {
	int nr_of_moves;
	struct opening_book_move_type Moves[9];
};


typedef struct tnode * NodePnt;
typedef struct hash_node * HashPnt;
typedef struct work_space_move_generator * WsPnt;
typedef unsigned long long BitArray;
typedef int (*CMPFUN)(WsPnt, int, int);

// This record is populated by the move generator
// and by init_splitpoint()
// and used by populate_node()
// and get-next-move()
// and sp_search()

struct work_space_move_generator {
	unsigned int max_nr_of_captures;				/* to keep hold of the rule: you have to capture as many pieces as possible */
	BitArray current_start_position;			/* to keep the start position during capturing with a king */
	unsigned int nr_of_legal_moves;					/* to keep track of the total number of legal moves */
	unsigned int current_move;						// to keep track of the current move
	struct move_type legal_moves[200];		// to store the legal moves
	int move_order[200];
	BitArray own_man;						/* bit arrays, the names are self explaining */
	BitArray own_king;
	BitArray other_man;
	BitArray other_king;
	BitArray other_piece;
	BitArray empty_field;
	unsigned int hash_key;
	int quiescence;							// is current position quiescence
	int capture;							// has color on move a forced capture
	//int other_side_has_to_capture;
	pthread_mutex_t ws_lock;							// lock for parallel operation
	int first_free_move;					// used for get_next_move
	int best_move;							// save the current best move
	int best_value;							// and the current best value
	int master_thread;						// to identify the owner of this split point
	NodePnt org_node;						// the node used by the move generator
	int depth;
	int alpha;
	int beta;
	int distance_from_root;
	WsPnt parent;
	bool finished;
	sig_atomic_t cpus;
	int ActiveSplitPoints;
	int slaves[THREAD_MAX];

};

typedef struct work_space_move_generator * WsPnt;




struct piece_count_record {
	char nr_of_white_man;
	char nr_of_black_man;
	char nr_of_white_king;
	char nr_of_black_king;
	char total_nr_of_pieces;
};

struct Thread {
  WsPnt splitPoint;
  int activeSplitPoints;
  int64_t nr_of_nodes_visited;
  int64_t nr_of_nodes_evaluated;
  int64_t nr_of_hits;
  int64_t nr_of_overwrites;
  int64_t avg_distance_from_root;
  int64_t nr_useless_sacrifice;
  int64_t useless_called;
  int64_t useless_endgame;
  int64_t useless_equal;
  int64_t useless_both_king;
  int64_t useless_one_king;
  int64_t useless_run_away;
  unsigned int max_distance_from_root;
  int64_t nr_of_best_move_only_hits;
  bool stop;
  bool running;
  bool idle;
  bool workIsWaiting;
  pthread_t os_thread_id;
};

struct init_boundaries {
	long low;
	long high;
};

typedef struct init_boundaries *InitPnt;
// global procedures

extern char * get_all_moves_with_value(char *);
extern char * get_best_move_fixed_depth(char *);
extern void call_init_tree();
extern void init_hash();
extern void create_final_report();
extern void generate_legal_moves(NodePnt, WsPnt, int, int);
extern unsigned char generate_limited_moves(NodePnt, WsPnt , char, BitArray, char, char);
extern void populate_node(WsPnt, NodePnt, int, NodePnt);
extern short int evaluate(NodePnt, char, char, int);
extern int white_is_locking_pieces(NodePnt, int, BitArray, BitArray);
extern int black_is_locking_pieces(NodePnt, int, BitArray, BitArray);
extern long calc_initial_hash_key(NodePnt);
extern void test_evaluator();
extern void get_best_move(char *);
extern void ponder(char *);
extern void stop_pondering();
extern void test_move_generator(char *);
extern void print_move(WsPnt , int );
extern void print_moves(WsPnt);
extern void print_fields(BitArray);
extern void calc_piece_count(NodePnt, struct piece_count_record *);
extern void Initialize_WhiteBook();
extern void Initialize_BlackBook();
extern void Initialize_frequency_tables(void);
extern void get_move_from_opening_book(char * );
extern void evaluate_board_position(char *);
extern void evaluate_board_position_2(char *);
extern char * analyse_timing(char *);
extern void print_board(struct tnode *);
extern void print_board_in_string(struct tnode *, char * );
extern void print_64(unsigned long );
extern int is_same_board_position(NodePnt, struct hash_node *);
extern short int mtdf_depth_limited(NodePnt, int, char);
extern short int mtdf_time_report(NodePnt, int, char, double);
extern void calc_patterns(BitArray, BitArray, int, int);
extern void init_game(char *);
extern char lc(struct tnode * , int);
extern BitArray nr_of_black_on_move_free_path(BitArray, BitArray, BitArray);
extern BitArray nr_of_black_not_on_move_free_path(BitArray, BitArray, BitArray);
extern BitArray nr_of_white_on_move_free_path(BitArray, BitArray, BitArray);
extern BitArray nr_of_white_not_on_move_free_path(BitArray, BitArray, BitArray);
extern short int evaluate_with_capture(NodePnt, struct piece_count_record);
extern void print_board_on_log(struct tnode *);
extern void write_levelAB(unsigned char, NodePnt);
extern void open_log_file();
extern void init_log_file();
extern int freedom(char, BitArray, BitArray, BitArray);
extern short int evaluate_endgame_position(char, int, unsigned long *, int, unsigned long *, int, unsigned long *, int, unsigned long *);
extern int alt_evaluate_endgame_position(char, int, unsigned long *, int, unsigned long *, int, unsigned long *, int, unsigned long *);
extern void print_bits(BitArray);
extern void init_hash_locks();
//extern void Qsort(WsPnt, int *, CMPFUN, uint32, uint32);
extern void swap_colors(NodePnt , NodePnt);
extern void calc_piece_count(NodePnt, struct piece_count_record);
extern int check_stand(NodePnt);
extern void print_bits(BitArray);
extern char * count_moves_at_level(char *);
extern int other_side_is_not_on_capture (NodePnt);
extern int increase_depth_white_on_move(NodePnt);
extern int increase_depth_black_on_move(NodePnt);
extern void wake_sleeping_threads(void);
extern void setup_hash(char *);
extern void find_hash_position(char *);
extern void find_hash_hashkey(char *);
extern void set_quiscence(WsPnt, NodePnt);
extern void set_capture(WsPnt, NodePnt);
extern char int_to_ext(BitArray);
extern int calc_black_voorpost(BitArray, BitArray, BitArray, int);
extern int calc_white_voorpost(BitArray, BitArray, BitArray, int);
extern int saldo(NodePnt, int);
extern int imin(int, int);
extern int imax(int, int);
extern short int search(NodePnt, short int, short int, short int, short int, int, bool);
extern NodePnt get_random_begin_postion();
extern void init_self_play_book();
extern short int mtdf_run_once(NodePnt, int, int, char);
extern void parallel_init_hash_zero(void);
extern short int min_max_search(NodePnt, unsigned int, int, int, int, bool);
extern void follow_up_position(char *);
extern short int mtdf_continue(NodePnt, int, int, char);
extern void parallel_init_hash_depth(void);
extern int more_or_equal_pieces(NodePnt);
extern int white_connections(BitArray, unsigned char);
extern int black_connections(BitArray, unsigned char);

// global variables

extern struct hash_node * hash;
extern BitArray bits[64];
extern BitArray not_bits[64];
extern char ext_to_int[52];
extern char int_to_ext_list[55];
extern BitArray ext_to_bits[64];
extern long * white_man_hash;
extern long * black_man_hash;
extern long * white_king_hash;
extern long * black_king_hash;
extern long * empty_field_hash;
extern int current_search_depth;
extern int print_moves_flag; //test
extern char extented_search_depth;
extern short int value_root_node;
extern struct piece_count_record org_piece_count;
extern int skip_early_quit;
extern double move_start_time;
extern double move_finish_time;
extern double move_duration;
extern BitArray sel_3_white, sel_4_white, sel_5_white, sel_6_white;
extern BitArray sel_3_black, sel_4_black, sel_5_black, sel_6_black;
extern int black_nr_of_moves_to_go;
extern int white_nr_of_moves_to_go;
extern int total_moves_allowed;
extern FILE * log_file;
extern char result[2048];
extern int result_length;
extern unsigned char white_freq[76][55][55];
extern unsigned char black_freq[76][55][55];
extern int global_move_nr;
extern int stop_flag;
extern int external_stop;
//extern long useless[200];
//extern long useless_from_hash[200];
//extern long depth_ext[200];
//extern long maybe_useless[200];
extern Thread Threads[THREAD_MAX];
extern int ActiveThreads;
extern pthread_mutex_t EndGameLock;
extern pthread_mutex_t MoveTimeOut;
extern pthread_mutex_t PonderLock;
//extern int global_beta;
//extern struct hash_node * prev_hash;
extern bool Idle;
extern unsigned long long white_item_count[31];
extern unsigned long long black_item_count[31];
extern float wweight[NR_OF_WEIGHTS];
extern float bweight[NR_OF_WEIGHTS];
extern struct tnode self_play_book[65];
extern int start_move_nr[65];
extern bool AllThreadsShouldExit;
extern int current_ply_nr;

