/*
  This file is Copyright (C) 2010 Feike Boomstra.
  Distributed under the Boost Software License, Version 1.0.
  (See accompanying file LICENSE_1_0.txt or copy at
  http://www.boost.org/LICENSE_1_0.txt)
*/


// Move and tree generation
//
#include "BoomstraDam.v1.2.h"
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

char int_to_ext[55] = {	 1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 0,
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


void print_move(WsPnt ws, int i)
{	char token;	
	if (ws->legal_moves[i].nr_of_captures == 0) token = '-'; else token = 'x';
	printf("%2d%c%2d ", int_to_ext[ws->legal_moves[i].start], token, int_to_ext[ws->legal_moves[i].stop]);			/* +1 gives external representation */	
}

void print_moves(WsPnt ws)
{	int i;
	if (ws->nr_of_legal_moves > 0)	
		for (i=0; i<ws->nr_of_legal_moves; i++) {print_move(ws, i); printf("\n");}
	else printf("Empty move list\n");
}



void populate_node(WsPnt ws, NodePnt node, int idx, NodePnt prev_node)				// In ws is the start board position (in expanded mode)
																	// node will contain the new board position (in compact mode)
																	// idx is the move number, the possible moves are in ws->moves[..]
{	int i;
//	long temp_hash;

	//if (calc_initial_hash_key(prev_node) != prev_node->hash_key) {printf("       org problem %X, %X\n", calc_initial_hash_key(prev_node), prev_node->hash_key); }
	//else printf("ok\n");

	/* start */
	/* attention:  the color on move in this node is the color to be used for the next move, not the current one */
	/* so here it is inverted */

	node->hash_key = prev_node->hash_key ^ WHITE_HASH;	/* start with hash key from previous position toggled with the color */
	node->man_king = prev_node->man_king;
	node->white_black = prev_node->white_black;
	node->occ_empty = prev_node->occ_empty;

	node->status = (~(prev_node->status)) & (IS_MAX_NODE | IS_WHITE_ON_MOVE);					// Invert the status bits

	node->occ_empty = node->occ_empty & not_bits[ws->legal_moves[idx].start];					/* remove the start piece */
	node->occ_empty = node->occ_empty | bits[ws->legal_moves[idx].stop];						/* add the end piece */
	node->occ_empty = node->occ_empty & ~(ws->legal_moves[idx].captured) & BITS50;				// remove the captured pieces

	if (node->status & IS_WHITE_ON_MOVE)												/* Here it means that the move was with black!!*/
	// move was with black !!
	{	node->white_black = node->white_black & not_bits[ws->legal_moves[idx].stop];				// set color black
		if (ws->own_man & bits[ws->legal_moves[idx].start])										/* ply with a man ? */
		{	node->hash_key = node->hash_key ^ black_man_hash[ws->legal_moves[idx].start];		/* remove his hash_key */
			//printf("Remove black man %d\n",ws->legal_moves[idx].start); 
			if (bits[ws->legal_moves[idx].stop] & FIELDS_46_50)									// if at bottom row
			{	node->man_king = node->man_king & not_bits[ws->legal_moves[idx].stop];		/* promotion, add a king */
				node->hash_key = node->hash_key ^ black_king_hash[ws->legal_moves[idx].stop];	/* and add the hash_key */			
				//printf("Add black king %d\n",ws->legal_moves[idx].stop); 
			}
			else 
			{	node->man_king = node->man_king | bits[ws->legal_moves[idx].stop];			// set to man
				node->hash_key = node->hash_key ^ black_man_hash[ws->legal_moves[idx].stop];	/* and add the hash_key */			
				//printf("Add black man %d\n",ws->legal_moves[idx].stop); 
			}
		} 
		else																					// ply with a king
		{	node->man_king = node->man_king & not_bits[ws->legal_moves[idx].stop];				// add on stop
			node->hash_key = node->hash_key ^ black_king_hash[ws->legal_moves[idx].start];		/* and the hash_key */			
			//printf("Remove black king %d\n",ws->legal_moves[idx].start); 
			node->hash_key = node->hash_key ^ black_king_hash[ws->legal_moves[idx].stop];		/* and the hash_key */			
			//printf("Add black king %d\n",ws->legal_moves[idx].stop); 
		};
		if (ws->legal_moves[idx].nr_of_captures > 0)
		{	for (i=0; i<50; i++)
			{	if (ws->legal_moves[idx].captured & bits[i])									/* this piece has to be removed */
				{	if ( node->man_king & bits[i])											/* is it a man? */
					{	node->hash_key = node->hash_key ^ white_man_hash[i];					/* remove the hash */
						//printf("Remove captured white man %d\n",i); 

					}
					else																	/* it is a king */
					{	node->hash_key = node->hash_key ^ white_king_hash[i];				/* and the hash */
						//printf("Remove captured white king %d\n",i); 
					}
				}
			}
		};
	}
	else																						/* white is own here */
	{	node->white_black = node->white_black | bits[ws->legal_moves[idx].stop];				// set color white
		if (ws->own_man & bits[ws->legal_moves[idx].start])										/* ply with a man ? */
		{	node->hash_key = node->hash_key ^ white_man_hash[ws->legal_moves[idx].start];		/* remove his hash_key */
			//printf("Remove white man %d\n",ws->legal_moves[idx].start); 
			if (bits[ws->legal_moves[idx].stop] & FIELDS_01_05)									// if at bottom row
			{	node->man_king = node->man_king & not_bits[ws->legal_moves[idx].stop];		/* promotion, add a king */
				node->hash_key = node->hash_key ^ white_king_hash[ws->legal_moves[idx].stop];	/* and add the hash_key */			
				//printf("Add white king %d\n",ws->legal_moves[idx].stop); 
			}
			else 
			{	node->man_king = node->man_king | bits[ws->legal_moves[idx].stop];			// set to man
				node->hash_key = node->hash_key ^ white_man_hash[ws->legal_moves[idx].stop];	/* and add the hash_key */			
				//printf("Add white man %d\n",ws->legal_moves[idx].stop); 
			}
		} 
		else																					// ply with a king
		{	node->man_king = node->man_king & not_bits[ws->legal_moves[idx].stop];				// add on stop
			node->hash_key = node->hash_key ^ white_king_hash[ws->legal_moves[idx].stop];		/* and the hash_key */			
			//printf("Add white king %d\n",ws->legal_moves[idx].stop); 
			node->man_king = node->man_king & not_bits[ws->legal_moves[idx].start];				// remove from start
			node->hash_key = node->hash_key ^ white_king_hash[ws->legal_moves[idx].start];		/* and the hash_key */			
			//printf("Remove white king %d\n",ws->legal_moves[idx].start); 
		};
		if (ws->legal_moves[idx].nr_of_captures > 0)
		{	for (i=0; i<50; i++)
			{	if (ws->legal_moves[idx].captured & bits[i])									/* this piece has to be removed */
				{	if ( node->man_king & bits[i])											/* is it a man? */
					{	node->hash_key = node->hash_key ^ black_man_hash[i];					/* remove the hash */
						//printf("Remove captured black man %d\n",i); 
					}
					else																	/* it is a king */
					{	node->hash_key = node->hash_key ^ black_king_hash[i];				/* and the hash */
						//printf("Remove captured black king %d\n",i); 
					}
				}
			}
		};
	}
	node->man_king = node->man_king & node->occ_empty;		// otherwise problem in is_same_board
	node->white_black = node->white_black & node->occ_empty;

	//if (calc_initial_hash_key(prev_node) != prev_node->hash_key) {printf("org problem %X %X\n", calc_initial_hash_key(prev_node), prev_node->hash_key); }
	//BitArray temp_hash;
	//temp_hash = calc_initial_hash_key(node);
	//if (temp_hash != node->hash_key) printf("problem after populate %X %X\n", temp_hash ,node->hash_key);
	return;
}
void print_64(unsigned long long ull64)  // print 64bits bitmap on X86 architecture
{	unsigned long long all64;
	unsigned long lower64;
	all64 = ull64; all64 = __ull_rshift(all64, 16);all64 = __ull_rshift(all64, 16);
	lower64 = (unsigned long) all64;
	printf("%.8X%.8X ", lower64, ull64);
	return;
}

void print_fields(BitArray fields)
{	unsigned long to_print_list[21];
	int to_print_nr;
	int i;
	count_bits_and_fill_list(fields, to_print_nr, to_print_list);
	printf("(");
	for (i=0; i<to_print_nr; i++) printf("%d, ", int_to_ext[to_print_list[i]]);
	printf(")\n");
}


