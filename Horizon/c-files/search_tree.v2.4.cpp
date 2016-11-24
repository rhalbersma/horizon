/*
  This file is Copyright (C) 2010 Feike Boomstra.
  Distributed under the Boost Software License, Version 1.0.
  (See accompanying file LICENSE_1_0.txt or copy at
  http://www.boost.org/LICENSE_1_0.txt)
*/


// Move and tree generation
//
#include "BoomstraDam.v2.5.h"
#include <malloc.h>


BitArray bits[64] = {				FLD1,	FLD2,	  FLD3,		FLD4,	  FLD5,		FLD6,	  FLD7,		FLD8,	  FLD9,		FLD10, 0,
									FLD11,	FLD12,	  FLD13,	FLD14,	  FLD15,	FLD16,	  FLD17,	FLD18,	  FLD19,	FLD20, 0,
									FLD21,	FLD22,	  FLD23,	FLD24,	  FLD25,	FLD26,	  FLD27,	FLD28,	  FLD29,	FLD30, 0,
									FLD31,	FLD32,	  FLD33,	FLD34,	  FLD35,	FLD36,	  FLD37,	FLD38,	  FLD39,	FLD40, 0,
									FLD41,	FLD42,	  FLD43,	FLD44,	  FLD45,	FLD46,	  FLD47,	FLD48,	  FLD49,	FLD50};
BitArray ext_to_bits[64] = {	0,  FLD1,	  FLD2,		  FLD3,		  FLD4,		  FLD5,		  FLD6,		  FLD7,		  FLD8,		  FLD9,		  FLD10,
									FLD11,	  FLD12,	  FLD13,	  FLD14,	  FLD15,	  FLD16,	  FLD17,	  FLD18,	  FLD19,	  FLD20,
									FLD21,	  FLD22,	  FLD23,	  FLD24,	  FLD25,	  FLD26,	  FLD27,	  FLD28,	  FLD29,	  FLD30,
									FLD31,	  FLD32,	  FLD33,	  FLD34,	  FLD35,	  FLD36,	  FLD37,	  FLD38,	  FLD39,	  FLD40,
									FLD41,	  FLD42,	  FLD43,	  FLD44,	  FLD45,	  FLD46,	  FLD47,	  FLD48,	  FLD49,	  FLD50};

char int_to_ext_list[55] = {	 1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 0,
                        11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 0,
                        21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 0,
                        31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 0,
						41, 42, 43, 44, 45, 46, 47, 48, 49, 50};
char ext_to_int[52] = {10,   0,	 1,  2,  3,  4,  5,  6,  7,  8,  9,
							11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
							22, 23, 24, 25, 26, 27, 28, 29, 30, 31,
							33, 34, 35, 36, 37, 38, 39, 40, 41, 42,
							44, 45, 46, 47, 48, 49, 50,	51,	52,	53, 10  };


BitArray not_bits[64] = {~FLD1,		  ~FLD2,	  ~FLD3,	  ~FLD4,	  ~FLD5,	  ~FLD6,	  ~FLD7,	  ~FLD8,	  ~FLD9,	  ~FLD10, 0xFFFFFFFFFFFFFFFF,
						 ~FLD11,	  ~FLD12,	  ~FLD13,	  ~FLD14,	  ~FLD15,	  ~FLD16,	  ~FLD17,	  ~FLD18,	  ~FLD19,	  ~FLD20, 0xFFFFFFFFFFFFFFFF,
						 ~FLD21,	  ~FLD22,	  ~FLD23,	  ~FLD24,	  ~FLD25,	  ~FLD26,	  ~FLD27,	  ~FLD28,	  ~FLD29,	  ~FLD30, 0xFFFFFFFFFFFFFFFF,
						 ~FLD31,	  ~FLD32,	  ~FLD33,	  ~FLD34,	  ~FLD35,	  ~FLD36,	  ~FLD37,	  ~FLD38,	  ~FLD39,	  ~FLD40, 0xFFFFFFFFFFFFFFFF,
						 ~FLD41,	  ~FLD42,	  ~FLD43,	  ~FLD44,	  ~FLD45,	  ~FLD46,	  ~FLD47,	  ~FLD48,	  ~FLD49,	  ~FLD50};



/* ---------------------- end of global variable declarations  --------------------------------------------------- */
//

char int_to_ext(BitArray pos) {
	int idx = __builtin_ctzll(pos);
	return int_to_ext_list[idx];
}

#define index(x) __builtin_ctzll(x)

void populate_node(WsPnt ws, NodePnt node, int idx, NodePnt prev_node)				// In ws is the start board position (in expanded mode)
																	// node will contain the new board position (in compact mode)
																	// idx is the move number, the possible moves are in ws->moves[..]
{	int i;
	BitArray temp;
	int king_move = 1;

	/* start */
	/* attention:  the color on move in this node is the color to be used for the next move, not the current one */
	/* so here it is inverted */
	struct move_type * actual_move = & ws->legal_moves[idx];

	node->hash_key = prev_node->hash_key ^ WHITE_HASH;	/* start with hash key from previous position toggled with the color */
	node->man_king = prev_node->man_king;
	node->white_black = prev_node->white_black;
	node->occ_empty = prev_node->occ_empty;
	node->previous_node = prev_node;

	node->status = (~(prev_node->status)) & (IS_MAX_NODE | IS_WHITE_ON_MOVE);					// Invert the status bits, rest is cleared
	node->occ_empty = node->occ_empty & ~actual_move->start;					/* remove the start piece */
	node->occ_empty = node->occ_empty | actual_move->stop;						/* add the end piece */
	node->occ_empty = node->occ_empty & ~(actual_move->captured) & BITS50;				// remove the captured pieces

	if (node->status & IS_WHITE_ON_MOVE)												/* Here it means that the move was with black!!*/
	// move was with black !!
	{	node->white_black = node->white_black & ~actual_move->stop;						// set color black
		if (ws->own_man & actual_move->start)										/* ply with a man ? */
		{	node->hash_key = node->hash_key ^ black_man_hash[index(actual_move->start)];		/* remove his hash_key */
			king_move = 0;                                                              // move with a man (cycle detection)
			//printf("Remove black man %d\n",actual_move->start);
			if (actual_move->stop & FIELDS_46_50) {									// if at bottom row
				node->man_king = node->man_king & ~actual_move->stop;		/* promotion, add a king */
				node->hash_key = node->hash_key ^ black_king_hash[index(actual_move->stop)];	/* and add the hash_key */
				//printf("Add black king %d\n",actual_move->stop);
			}
			else
			{	node->man_king = node->man_king | actual_move->stop;			// set to man
				node->hash_key = node->hash_key ^ black_man_hash[index(actual_move->stop)];	/* and add the hash_key */
				//printf("Add black man %d\n",actual_move->stop);
			}
		}
		else																					// ply with a king
		{	node->man_king = node->man_king & ~actual_move->stop;				// add on stop
			node->hash_key = node->hash_key ^ black_king_hash[index(actual_move->start)];		/* and the hash_key */
			//printf("Remove black king %d\n",actual_move->start);
			node->hash_key = node->hash_key ^ black_king_hash[index(actual_move->stop)];		/* and the hash_key */
			//printf("Add black king %d\n",actual_move->stop);
		};
		temp = actual_move->captured;
		while (1)
		{ 	if (temp == 0) break;
			i =  __builtin_ctzll(temp); temp = (temp & (temp - 1));
			{	if ( node->man_king & bits[i])											/* is it a man? */
				{	node->hash_key = node->hash_key ^ white_man_hash[i];					/* remove the hash */
					//printf("Remove captured white man %d\n",i);
				}
				else																	/* it is a king */
				{	node->hash_key = node->hash_key ^ white_king_hash[i];				/* and the hash */
					//printf("Remove captured white king %d\n",i);
				}
			}
		};
	}
	else																						/* white is own here */
	{	node->white_black = node->white_black | actual_move->stop;				// set color white
		if (ws->own_man & actual_move->start)										/* ply with a man ? */
		{	node->hash_key = node->hash_key ^ white_man_hash[index(actual_move->start)];		/* remove his hash_key */
			//printf("Remove white man %d\n",actual_move->start);
			king_move = 0;                                                              // move with a man (cycle detection)
			if (actual_move->stop & FIELDS_01_05) {									// if at bottom row
				node->man_king = node->man_king & ~actual_move->stop;		/* promotion, add a king */
				node->hash_key = node->hash_key ^ white_king_hash[index(actual_move->stop)];	/* and add the hash_key */
				//printf("Add white king %d\n",actual_move->stop);
			}
			else
			{	node->man_king = node->man_king | actual_move->stop;			// set to man
				node->hash_key = node->hash_key ^ white_man_hash[index(actual_move->stop)];	/* and add the hash_key */
				//printf("Add white man %d\n",actual_move->stop);
			}
		}
		else																					// ply with a king
		{	node->man_king = node->man_king & ~actual_move->stop;				// add on stop
			node->hash_key = node->hash_key ^ white_king_hash[index(actual_move->stop)];		/* and the hash_key */
			//printf("Add white king %d\n",actual_move->stop);
			//node->man_king = node->man_king & not_bits[actual_move->start];				// remove from start
			node->hash_key = node->hash_key ^ white_king_hash[index(actual_move->start)];		/* and the hash_key */
			//printf("Remove white king %d\n",actual_move->start);
		};

		temp = actual_move->captured;
		while (1)
		{ 	if (temp == 0) break;
			i =  __builtin_ctzll(temp); temp = (temp & (temp - 1));
			{	if ( node->man_king & bits[i])											/* is it a man? */
				{	node->hash_key = node->hash_key ^ black_man_hash[i];					/* remove the hash */
					//printf("Remove captured black man %d\n",i);
				}
				else																	/* it is a king */
				{	node->hash_key = node->hash_key ^ black_king_hash[i];				/* and the hash */
					//printf("Remove captured black king %d\n",i);
				}
			}
		};
	}
	node->man_king = node->man_king & node->occ_empty;		// otherwise problem in is_same_board
	node->white_black = node->white_black & node->occ_empty;

	// some administration for cycle_detection

	if ((king_move) && !(actual_move->captured) &&
		(~node->man_king & node->occ_empty & node->white_black) && (~node->man_king & node->occ_empty & ~node->white_black))
		node->status = node->status | IS_CYCLE_POSSIBLE;

	node->nr_of_white_man  = (unsigned char)__builtin_popcountll( node->man_king & node->occ_empty &  node->white_black);
	node->nr_of_black_man  = (unsigned char)__builtin_popcountll( node->man_king & node->occ_empty & ~node->white_black);
	node->nr_of_white_king = (unsigned char)__builtin_popcountll(~node->man_king & node->occ_empty &  node->white_black);
	node->nr_of_black_king = (unsigned char)__builtin_popcountll(~node->man_king & node->occ_empty & ~node->white_black);
	node->total_nr_of_pieces = node->nr_of_white_man + node->nr_of_white_king + node->nr_of_black_man + node->nr_of_black_king;

	return;
}
void print_64(unsigned long ull64)
{	printf("%.16lX ", ull64);
	return;
}

void print_fields(BitArray fields)
{	unsigned long to_print_list[21];
	int to_print_nr;
	int i;
	count_bits_and_fill_list(fields, to_print_nr, to_print_list);
	printf("(");
	for (i=0; i<to_print_nr; i++) printf("%d, ", int_to_ext(to_print_list[i]));
	printf(")\n");
}


