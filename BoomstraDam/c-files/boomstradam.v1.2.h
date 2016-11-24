/*
  This file is Copyright (C) 2010 Feike Boomstra.
  Distributed under the Boost Software License, Version 1.0.
  (See accompanying file LICENSE_1_0.txt or copy at
  http://www.boost.org/LICENSE_1_0.txt)
*/


#include <stdio.h>
#include <intrin.h>
#pragma intrinsic(_BitScanForward)
#pragma intrinsic(__ull_rshift)

#define IS_WHITE_ON_MOVE 0x1				/* the color of the next move in this position, WHITE = 1, BLACK = 0 */
#define INV_WHITE_ON_MOVE 0xFE
#define IS_MAX_NODE 0x2						/* the nature of the node: MAX_NODE = 1, MIN_NODE = 0 */
#define IS_EVALUATED 0x4					/* is the node allready evaluated */
#define IS_EARLY_QUIT 0x8					// too much losses
#define IS_NOT_IN_HASH 0x10					// node is not found in the hash (anymore)
#define IS_EXACT_VALUE 0x20					// evaluation value is coming from the endgame database
#define MASK_HASH       0x3FFFFF			/* the mask according to this length */
#define HASH_ARRAY_SIZE 0x400000			/* the related array size [0..HASH_ARRAY_SIZE -1] */
#define WHITE_HASH (0x5555555555555555 & MASK_HASH) // to distinguish between white on move or black on move
#define INFINITY  32767						/* used in the minmax routines */
#define NR_OF_ENDGAME_PIECES 6
//#define QUIT_EARLY -30000					// used to quit a search in case of severe material shortage
#define TEST_DEPTH 16 /* test only */
#define counters 0							// define and administer various counters
#define increment2_test 0					// test increment depth variant

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


// some macro's to deal with 64 bit operations on bits

#define count_bits_and_fill_list(bit_var, nr_of_bits,  bit_list) {unsigned long lower64;unsigned long long all64;unsigned char isNonZero;nr_of_bits=0;lower64 = (unsigned long) bit_var;while (1) {isNonZero = _BitScanForward( & bit_list[nr_of_bits], lower64);if (isNonZero){lower64 = (lower64 & (lower64 - 1));nr_of_bits++;} else break;}all64 = bit_var; all64 = __ull_rshift(all64, 16);all64 = __ull_rshift(all64, 16);lower64 = (unsigned long) all64;while (1){isNonZero = _BitScanForward(&bit_list[nr_of_bits], lower64);if (isNonZero){	lower64 = (lower64 & (lower64 - 1));bit_list[nr_of_bits] = bit_list[nr_of_bits] + 32;nr_of_bits++;} else break;}};
#define count_bits(bit_var, nr_of_bits) {BitArray temp__=bit_var; nr_of_bits=0; while (temp__){temp__ = temp__ & (temp__-1); nr_of_bits++;}}
#define add_count_bits(bit_var, nr_of_bits, amount) {BitArray temp__=bit_var; while (temp__){temp__ = temp__ & (temp__-1); nr_of_bits += amount;}}
#define all_bits_set(question, constant) ((question & constant) == constant)

// struct definitions

struct tnode {
	unsigned long long man_king;				/* bit array [0..49] man = 1 king = 0  [50..55] move start [56..63] t.b.d. */
	unsigned long long white_black;				/* bit array [0..49] white = 1 black = 0 [50..55] move stop  [56..63] status bits. */
	unsigned long long occ_empty;				/* bit array [0..49] occupied = 1 empty = 0 [50..63] t.b.d. */
	short int upperbound;						/* upperbound used in alphabeta pruning */
	short int lowerbound;						/* lowerbound used in alphabeta pruning */
	unsigned long hash_key;						/* the hash key related to the board position */
	char tree_depth;							/* to check whether the boundaries has to be re-calculated for a new depth */
	unsigned char best_move_idx;				// the best move from a previous search depth
	unsigned char status;
};
struct move_type {
	unsigned char start;							/* the start position of the piece that moves (or captures) */
	unsigned char stop;							/* the stop position of the piece that moves or captures */
	unsigned long long captured;					/* bit array of pieces that are captured in this move */
	unsigned char nr_of_captures;				/* how many pieces are currently captured */
};
struct opening_book_move_type{
	unsigned char Start;
	unsigned char Stop;
	short int Next;
} ;
struct game_list {
	unsigned char Start;
	unsigned char Stop;
};

struct opening_book_position_type {
	unsigned char nr_of_moves;
	struct opening_book_move_type Moves[9];
};



typedef struct tnode * NodePnt;
typedef struct work_space_move_generator * WsPnt;
typedef unsigned long long BitArray;

struct work_space_move_generator {
	unsigned char max_nr_of_captures;				/* to keep hold of the rule: you have to capture as many pieces as possible */
	unsigned char current_start_position;			/* to keep the start position during capturing with a king */
	unsigned char nr_of_legal_moves;					/* to keep track of the total number of legal moves */
	unsigned char current_move;						// to keep track of the current move
	struct move_type legal_moves[200];		// to store the legal moves
	BitArray own_man;						/* bit arrays, the names are self explaining */
	BitArray own_king;
	BitArray other_man;
	BitArray other_king;
	BitArray other_piece;
	BitArray empty_field;
	long hash_key;
};

struct piece_count_record {
	char nr_of_white_man;
	char nr_of_black_man;
	char nr_of_white_king;
	char nr_of_black_king;
};

// global procedures

extern char * compare_calc_move_with_played_move(const char *, int);
extern void call_init_tree();
extern void init_hash();
extern void create_final_report();
extern void generate_legal_moves(NodePnt, WsPnt, char);
extern unsigned char generate_limited_moves(NodePnt, WsPnt , char, BitArray, char, char);
extern void populate_node(WsPnt, NodePnt, int, NodePnt);
extern short int evaluate(NodePnt, char, char);
extern char white_is_locking_pieces(NodePnt, char, BitArray, BitArray);
extern char black_is_locking_pieces(NodePnt, char, BitArray, BitArray);
extern long calc_initial_hash_key(NodePnt);
extern void test_evaluator();
extern void get_best_move(const char *);
extern void test_move_generator(const char *);
extern void print_move(WsPnt , int );
extern int severe_shortage_min_mode(NodePnt);
extern int severe_shortage_max_mode(NodePnt);
extern void print_moves(WsPnt);
extern void print_fields(BitArray);
extern void calc_piece_count(NodePnt, struct piece_count_record *);
extern void Initialize_WhiteBook();
extern void Initialize_BlackBook();
extern void get_move_from_opening_book(const char * );
extern void evaluate_board_position(const char *);
extern char * analyse_timing(const char *);
extern void print_board(struct tnode *);
extern void print_board_in_string(struct tnode *, char * );
extern void print_64(unsigned long long );
extern int is_same_board_position(NodePnt, NodePnt);
extern short int mtdf_depth_limited(NodePnt, int, char);
extern short int mtdf_time_report(NodePnt, int, char, double);
extern void calc_patterns(BitArray, BitArray, int, int);
extern void init_game(const char *);
extern char lc(struct tnode * , int);
extern int nr_of_black_max_free_path(BitArray, BitArray, BitArray);
extern int nr_of_black_min_free_path(BitArray, BitArray, BitArray);
extern int nr_of_white_max_free_path(BitArray, BitArray, BitArray);
extern int nr_of_white_min_free_path(BitArray, BitArray, BitArray);
extern short int evaluate_with_capture(NodePnt, int);
extern void print_board_on_log(struct tnode *);
extern void write_levelAB(unsigned char, NodePnt);
extern void open_log_file();
extern void init_log_file();
extern int freedom(char, BitArray, BitArray, BitArray);
extern int evaluate_endgame_position(char, int, unsigned long *, int, unsigned long *, int, unsigned long *, int, unsigned long *);
extern int alt_evaluate_endgame_position(char, int, unsigned long *, int, unsigned long *, int, unsigned long *, int, unsigned long *);


// global variables

extern NodePnt hash;
extern BitArray bits[64];
extern BitArray not_bits[64];
extern char int_to_ext[55];
extern char ext_to_int[52];
extern BitArray ext_to_bits[64];
extern long * white_man_hash;
extern long * black_man_hash;
extern long * white_king_hash;
extern long * black_king_hash;
extern long * empty_field_hash;
extern char current_search_depth;
extern int print_moves_flag; //test
extern char extented_search_depth;
extern long nr_of_evaluations;
extern short int value_root_node;
extern struct piece_count_record org_piece_count;
extern char skip_early_quit;
extern long nr_of_nodes_visited;
extern long nr_of_hits;
extern long nr_of_overwrites;
extern double move_start_time;
extern double move_finish_time;
extern double move_duration;
extern BitArray sel_3_white, sel_4_white, sel_5_white, sel_6_white;
extern BitArray sel_3_black, sel_4_black, sel_5_black, sel_6_black;
extern double black_nr_of_moves_to_go;
extern double white_nr_of_moves_to_go;
extern double total_moves_allowed;
extern FILE * log_file;
extern char result[256];
extern int result_length;
