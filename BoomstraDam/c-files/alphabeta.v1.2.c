/*
  This file is Copyright (C) 2010 Feike Boomstra.
  Distributed under the Boost Software License, Version 1.0.
  (See accompanying file LICENSE_1_0.txt or copy at
  http://www.boost.org/LICENSE_1_0.txt)
*/


#include "BoomstraDam.v1.2.h"
#include <malloc.h>
#include <time.h>
// v1.2
//

char current_search_depth = 0;								// global variable to distinguish various runs
long nr_of_nodes_visited;
long nr_of_hits;
long nr_of_overwrites;
//long nr_of_shortage_quits[50];
//long claimed;
char skip_early_quit;



char lc(struct tnode * node, int idx)
{	idx++;
	if (~(node->occ_empty) & ext_to_bits[idx])
		return '.';
	else
	{	if (node->white_black & ext_to_bits[idx])
		{	if (node->man_king & ext_to_bits[idx])
				return 'o';
			else
				return 'O';
		} 
		else
		{	if (node->man_king & ext_to_bits[idx])
				return 'x';
			else
				return 'X';		
		};
	}
}
void print_board(struct tnode * node)
{	int i;
	if (node == NULL)
	{	printf("Print empty node ??\n");
		return;
	};
	if (node->status & IS_WHITE_ON_MOVE) printf("white on move\n"); else printf("black on move\n");
	if (node->status & IS_MAX_NODE) printf("max node"); else printf("min node");
	printf(" %6X\n\n", node->hash_key);
	for (i=0; i < 5; i++)
	{	printf("   %c %c %c %c %c\n", lc(node,10*i), lc(node,10*i+1), lc(node,10*i+2), lc(node,10*i+3), lc(node,10*i+4));
		printf("  %c %c %c %c %c \n", lc(node,10*i+5), lc(node,10*i+6), lc(node,10*i+7), lc(node,10*i+8), lc(node,10*i+9)); }
	printf("\n");
}
void print_board_in_string(struct tnode * node, char * result)
{	int i;
	int j = 0;

	if (node == NULL)
	{	j +=  sprintf_s(result + j, 511 - j, "Print empty node ??\n");
		return;
	};
	if (node->status & IS_WHITE_ON_MOVE) j +=  sprintf_s(result + j, 511 - j, "white on move\n"); else j +=  sprintf_s(result + j, 511 - j, "black on move\n");
	if (node->status & IS_MAX_NODE) j +=  sprintf_s(result + j, 511 - j, "max node\n\n"); else j +=  sprintf_s(result + j, 511 - j, "min node\n\n");
	for (i=0; i < 5; i++)
	{	j +=  sprintf_s(result + j, 511 - j, "   .%c.%c.%c.%c.%c\n", lc(node,10*i), lc(node,10*i+1), lc(node,10*i+2), lc(node,10*i+3), lc(node,10*i+4));
		j +=  sprintf_s(result + j, 511 - j, "   %c.%c.%c.%c.%c.\n", lc(node,10*i+5), lc(node,10*i+6), lc(node,10*i+7), lc(node,10*i+8), lc(node,10*i+9)); }
	j +=  sprintf_s(result + j, 511 - j, "\n");
}
int is_same_board_position(NodePnt node1, NodePnt node2) /* check if the board position is the same */
{	if ((node1 == NULL) || (node2 == NULL)) { printf("Null pointer in same board position\n"); return 0;}
	if ((node1->occ_empty) != (node2->occ_empty)) return 0; /* check occ_empty fields */
	if (((node1->status) & (IS_MAX_NODE | IS_WHITE_ON_MOVE)) != ((node2->status)& (IS_MAX_NODE | IS_WHITE_ON_MOVE))) return 0; /* check color, min/max */
	if ((node1->white_black) != (node2->white_black)) return 0; /* check white black fields */
	if ((node1->man_king) != (node2->man_king)) return 0; /* check man_king fields */
	/* printf("found %d == %d\n", node1 - mem, node2 - mem);*/
	return 1; /* return OK */

}
 void initial_store_common(NodePnt node, short int g, char best_move, unsigned char distance_from_root, unsigned char status_info)
{	hash[node->hash_key].white_black = node->white_black;
	hash[node->hash_key].man_king = node->man_king;
	hash[node->hash_key].occ_empty = node->occ_empty;
	hash[node->hash_key].hash_key = distance_from_root;  // mis-use hash_key
	hash[node->hash_key].tree_depth = current_search_depth;
	hash[node->hash_key].best_move_idx = best_move; 
}
 void update_store_both(NodePnt node, short int g, char best_move, unsigned char distance_from_root, unsigned char status_info)
{	hash[node->hash_key].lowerbound = g;
	hash[node->hash_key].upperbound = g;
	hash[node->hash_key].status = (node->status | status_info);				
	hash[node->hash_key].best_move_idx = best_move; 
}
 void update_store_upper(NodePnt node, short int g, char best_move, unsigned char distance_from_root, unsigned char status_info)
{	hash[node->hash_key].upperbound = g;
	hash[node->hash_key].status = (node->status | status_info);				
	hash[node->hash_key].best_move_idx = best_move; 
}
 void update_store_lower(NodePnt node, short int g, char best_move, unsigned char distance_from_root, unsigned char status_info)
{	hash[node->hash_key].lowerbound = g;
	hash[node->hash_key].status = (node->status | status_info);				
	hash[node->hash_key].best_move_idx = best_move; 
}
 //void test_store(char * ps, short int g, NodePnt node, unsigned char distance_from_root, unsigned char status_info)
 //{	if (node->hash_key == 0x331E76) 
	//{	fprintf_s(log_file, "%s: val=%d dis=%d status=%X\n", ps, g, distance_from_root, status_info);
	//	print_board_on_log(node);
	//}
// }
 void initial_store_both(NodePnt node, short int g, char best_move, unsigned char distance_from_root, unsigned char status_info)
 {	//test_store("initial store both", g, node, distance_from_root, status_info);
	initial_store_common(node, g, best_move, distance_from_root, status_info);
	update_store_both(node, g, best_move, distance_from_root, status_info);
}
 void initial_store_upper(NodePnt node, short int g, char best_move, unsigned char distance_from_root, unsigned char status_info)
{	//test_store("initial store upper", g, node, distance_from_root, status_info);
	initial_store_common(node, g, best_move, distance_from_root, status_info);
	hash[node->hash_key].lowerbound = -INFINITY;
	update_store_upper(node, g, best_move, distance_from_root, status_info);
}
 void initial_store_lower(NodePnt node, short int g, char best_move, unsigned char distance_from_root, unsigned char status_info)
{	//test_store("initial store lower", g, node, distance_from_root, status_info);
	initial_store_common(node, g, best_move, distance_from_root, status_info);
	hash[node->hash_key].upperbound = +INFINITY;
	update_store_lower(node, g, best_move, distance_from_root, status_info);
}

 void store_both_bounds(NodePnt node, short int g, char best_move, unsigned char distance_from_root, unsigned char status_info)
{	if (hash[node->hash_key].occ_empty == 0) { initial_store_both(node, g, best_move, distance_from_root, status_info); return; }
	// hash entry is not empty
	if (hash[node->hash_key].tree_depth != current_search_depth){ initial_store_both(node, g, best_move, distance_from_root, status_info); return; }
	// tree_depth == current_search_depth
	if (hash[node->hash_key].hash_key > distance_from_root) { initial_store_both(node, g, best_move, distance_from_root, status_info); return; }
	// overwrite in case new node is closer to root
	if ((is_same_board_position(node, &hash[node->hash_key])) && (hash[node->hash_key].hash_key >= distance_from_root)) { /*test_store("update store both", g, node, distance_from_root, status_info);*/ update_store_both(node, g, best_move, distance_from_root, status_info); return; }
}
 void store_upperbound(NodePnt node, short int g, char best_move, unsigned char distance_from_root, unsigned char status_info)
{	if (hash[node->hash_key].occ_empty == 0) { initial_store_upper(node, g, best_move, distance_from_root, status_info); return; }
	// hash entry is not empty
	if (hash[node->hash_key].tree_depth != current_search_depth){ initial_store_upper(node, g, best_move, distance_from_root, status_info); return; }
	// tree_depth == current_search_depth
	if (hash[node->hash_key].hash_key > distance_from_root) { initial_store_upper(node, g, best_move, distance_from_root, status_info); return; }
	// overwrite in case new node is closer to root
	if ((is_same_board_position(node, &hash[node->hash_key])) && (hash[node->hash_key].hash_key >= distance_from_root)) {/* test_store("update store upper", g, node, distance_from_root, status_info);*/ update_store_upper(node, g, best_move, distance_from_root, status_info); return; }
}
 void store_lowerbound(NodePnt node, short int g, char best_move, unsigned char distance_from_root, unsigned char status_info)
{	if (hash[node->hash_key].occ_empty == 0) { initial_store_lower(node, g, best_move, distance_from_root, status_info); return; }
	// hash entry is not empty
	if (hash[node->hash_key].tree_depth != current_search_depth){ initial_store_lower(node, g, best_move, distance_from_root, status_info); return; }
	// tree_depth == current_search_depth
	if (hash[node->hash_key].hash_key > distance_from_root) { initial_store_lower(node, g, best_move, distance_from_root, status_info); return; }
	// overwrite in case new node is closer to root
	if ((is_same_board_position(node, &hash[node->hash_key])) && (hash[node->hash_key].hash_key >= distance_from_root)) {/* test_store("update store lower", g, node, distance_from_root, status_info);*/ update_store_lower(node, g, best_move, distance_from_root, status_info); return; }
}





 short int early_quit_in_min_mode(NodePnt node, char depth, char nr_of_legal_moves) // should be called in min-node
{	short int piece_diff;
	if (skip_early_quit) return 0;
	if (depth > 2) return 0;
	if ((nr_of_legal_moves >= 3) && (piece_diff = severe_shortage_min_mode(node))) return evaluate(node, 0, 0);	
	return 0;
}
 short int early_quit_in_max_mode(NodePnt node, char depth, char nr_of_legal_moves) // should be called in max-node
{	short int piece_diff;
	if (skip_early_quit) return 0;
	if (depth > 2) return 0;
	if ((nr_of_legal_moves >= 3) && (piece_diff = severe_shortage_max_mode(node))) return piece_diff;
	return 0;
}


 short int min(short int a, short int b)
{	if (a < b)	return a;
	return b;
}
 short int max(short int a, short int b)
{	if (a > b)	return a;
	return b;

}
short int AlphaBetaWithMemory(NodePnt node, short int alpha, short int beta, unsigned char distance_from_root, char depth, unsigned char move_pattern)
{	short int a, b;
	short int g;
	short int z;
	struct tnode next_node;
	struct work_space_move_generator ws;
	char best_move;
	char new_best_move = 0;
	char depth_increment;
	unsigned char new_move_pattern;
	int i;
	char all_moves_ignored = 1; // to check whether all moves were ignored

	//write_levelAB(distance_from_root, node);
	nr_of_nodes_visited++;

	if (node->hash_key > MASK_HASH) {printf("hash key too big\n"); return 0; }
	if (is_same_board_position(node, &hash[node->hash_key])&& (hash[node->hash_key].hash_key == distance_from_root))					// check if it is claimed by this node			
	{	if (hash[node->hash_key].status & IS_EXACT_VALUE) return hash[node->hash_key].lowerbound; // 
		if (hash[node->hash_key].tree_depth == current_search_depth)			// check if it is claimed in this run
		{	nr_of_hits++;
			if (hash[node->hash_key].lowerbound >=  beta) return hash[node->hash_key].lowerbound; 
			if (hash[node->hash_key].upperbound <= alpha) return hash[node->hash_key].upperbound; 
			alpha = max(alpha, hash[node->hash_key].lowerbound);
			beta = min(beta, hash[node->hash_key].upperbound);
		};
		generate_legal_moves(node, &ws, node->status & IS_WHITE_ON_MOVE);			// all legal moves collected in ws
		if (ws.nr_of_legal_moves == 0)				// we are done in this branch
		{	if (node->status & IS_MAX_NODE)
				g = -INFINITY+1;						// that's a pity, no more moves 
			else g = +INFINITY-1;
			store_both_bounds(node, g, 0, distance_from_root, IS_EVALUATED);
			return g;
		};
		if ((depth <= 0) && (!(node->status & IS_MAX_NODE)) /* && (move_pattern & 0x1A) != 0x1A)*/)
		{	if (ws.max_nr_of_captures == 0)
			{	g = evaluate(node,0,0);
				store_both_bounds(node, g, 0, distance_from_root, IS_EVALUATED);		    
			}
			else g = 0; // ignore }			
			return g;
		}
		if (node->status & IS_MAX_NODE)		// MAX node
		{	if (ws.max_nr_of_captures == 0)
			{	if (g = early_quit_in_max_mode(node, depth, ws.nr_of_legal_moves))
				{	store_both_bounds(node, g, 0, distance_from_root, (IS_EARLY_QUIT | IS_EVALUATED));
					return g;
				}
			}
		g = -INFINITY; a = alpha;
		if (ws.max_nr_of_captures > 0) { depth_increment = 0; new_move_pattern = (move_pattern >> 1) | 0x10; }// in case of capture, dig deeper in tree
		else {depth_increment = 1; new_move_pattern = (move_pattern >> 1);}
			best_move = hash[node->hash_key].best_move_idx;	// save best move with the call, hash entry can be changed after recursion
			//if ((best_move < 0 ) || (best_move >= ws.nr_of_legal_moves)) {printf("Illegal best move %d\n", best_move); best_move = 0; };
			new_best_move = best_move;				// and preset the "new" best move
			populate_node(&ws, &next_node, best_move, node);
			z = AlphaBetaWithMemory(&next_node, a, beta, distance_from_root + 1, depth -depth_increment, new_move_pattern);
			if (z != 0 )
			{	g = max(g, z);
				a = max(a, g);
				all_moves_ignored = 0;
			}
			g = max(g, z);
			a = max(a, g);
			if (g < beta)
			{	for (i=0; i < ws.nr_of_legal_moves; i++)
					if (i != best_move)	
					{	populate_node(&ws, &next_node, i, node);
						z = AlphaBetaWithMemory(&next_node, a, beta, distance_from_root + 1, depth -depth_increment, new_move_pattern);
						if (z != 0)
						{	if (z > g) new_best_move = i;
							g = max(g, z);
							a = max(a, g);
							all_moves_ignored = 0;
							if (g >= beta) break;
						}
					};
			}
		}
		else											// MIN node
		{	if (ws.max_nr_of_captures == 0)
			{	if (g = early_quit_in_min_mode(node, depth, ws.nr_of_legal_moves))
				{	store_both_bounds(node, g, 0, distance_from_root, (IS_EARLY_QUIT | IS_EVALUATED));
					return g;
				}
			}
			g = +INFINITY; b = beta;
			if (ws.max_nr_of_captures > 0) { depth_increment = 0; new_move_pattern = (move_pattern >> 1) | 0x10; }// in case of capture, dig deeper in tree
			else {depth_increment = 1; new_move_pattern = (move_pattern >> 1);}
			best_move = hash[node->hash_key].best_move_idx;	// save best move with the call, hash entry can be changed after recursion
			if ((best_move < 0 ) || (best_move >= ws.nr_of_legal_moves)) {printf("Illegal best move %d\n", best_move); best_move = 0; };
			new_best_move = best_move;				// and preset the "new" best move
			populate_node(&ws, &next_node, best_move, node );  // first the best known move
			z = AlphaBetaWithMemory(&next_node, alpha, b, distance_from_root + 1, depth -depth_increment,new_move_pattern);
			if (z != 0 )
			{	g = min(g, z);
				b = min(b, g);
				all_moves_ignored = 0;
			}
			if (g > alpha)
			{	for (i=0; i < ws.nr_of_legal_moves; i++)
					if (i != best_move)
					{	populate_node(&ws, &next_node, i, node);
						z = AlphaBetaWithMemory(&next_node, alpha, b, distance_from_root + 1, depth -depth_increment,new_move_pattern);
						if (z != 0)
						{	if (z < g) new_best_move = i;
							g = min(g, z);
							b = min(b, g);
							all_moves_ignored = 0;
							if (g <= alpha) break;
						}
					};
			}
		}
	}
	else											// no lookup in hash entry for this node
	{	generate_legal_moves(node, &ws, node->status & IS_WHITE_ON_MOVE);			// all legal moves collected in ws
		if (ws.nr_of_legal_moves == 0)				// we are done in this branch
		{	if (node->status & IS_MAX_NODE)
				g = -INFINITY+1;				// that's a pity, no more moves 
			else g = +INFINITY-1;			// yes!!
			store_both_bounds(node, g, 0, distance_from_root, IS_EVALUATED);
			return g;
		};
		if ((depth <= 0) && (!(node->status & IS_MAX_NODE)) /* && (move_pattern & 0x1A) != 0x1A)*/)
		{	if (ws.max_nr_of_captures == 0)
			{	g = evaluate(node,0, 0);
				store_both_bounds(node, g, 0, distance_from_root, IS_EVALUATED);		    
			}
			else g = 0; // ignore }			
			return g;
		}
		if (node->status & IS_MAX_NODE)		// MAX node
		{	if (ws.max_nr_of_captures == 0)
			{	if (g = early_quit_in_max_mode(node, depth, ws.nr_of_legal_moves))
				{	store_both_bounds(node, g, 0, distance_from_root, (IS_EARLY_QUIT | IS_EVALUATED));
					return g;
				}
			}
			g = -INFINITY; a = alpha;
			if (ws.max_nr_of_captures > 0) { depth_increment = 0; new_move_pattern = (move_pattern >> 1) | 0x10; }// in case of capture, dig deeper in tree
			else {depth_increment = 1; new_move_pattern = (move_pattern >> 1);}
			for (i=0; i < ws.nr_of_legal_moves; i++)
			{	populate_node(&ws, &next_node, i, node);
				z = AlphaBetaWithMemory(&next_node, a, beta, distance_from_root + 1, depth - depth_increment,new_move_pattern);
				if (z != 0)
				{	if (z > g) new_best_move = i;
					g = max(g, z);
					a = max(a, g);
					all_moves_ignored = 0;
					if (g >= beta) break;
				}
			};
		}
		else											// MIN node
		{	if (ws.max_nr_of_captures == 0)
			{	if (g = early_quit_in_min_mode(node, depth, ws.nr_of_legal_moves))
				{	store_both_bounds(node, g, 0, distance_from_root, (IS_EARLY_QUIT | IS_EVALUATED));
					return g;
				}
			}
			g = +INFINITY; b = beta;
			if (ws.max_nr_of_captures > 0) { depth_increment = 0; new_move_pattern = (move_pattern >> 1) | 0x10; }// in case of capture, dig deeper in tree
			else {depth_increment = 1; new_move_pattern = (move_pattern >> 1);}
			for (i=0; i < ws.nr_of_legal_moves; i++)
			{	populate_node(&ws, &next_node, i, node);
				z = AlphaBetaWithMemory(&next_node, alpha, b, distance_from_root + 1, depth - depth_increment,new_move_pattern);
				if (z != 0)
				{	if (z < g) new_best_move = i;
					g = min(g, z);
					b = min(b, g);
					all_moves_ignored = 0;
					if (g <= alpha) break;
				}
			};
		}
	}
	if (all_moves_ignored) {if (distance_from_root == 0)
							{	printf("All moves ignored\n"); 
								print_moves(&ws);
								print_board(node);
							}
	return 0;}
	if (g <= alpha)	store_upperbound(node, g, new_best_move, distance_from_root, 0);		// fail low implies an upper bound
	if (g >= beta) store_lowerbound(node, g, new_best_move, distance_from_root, 0);
	return g;
}


short int mtdf_depth_limited(NodePnt root, int initial_guess, char depth)		// algorithm as invented by Aske Plaat
{	short int g ;
	short int beta;
	short int upperbound = +INFINITY;
	short int lowerbound = -INFINITY;

	g = initial_guess;
	current_search_depth = depth;					// set global variable

	while (1)
	{	if (g == lowerbound) beta = g + 1; else beta = g;
		//printf("Mtdf calls with beta = %d, l = %d, u = %d, depth = %d\n", beta, lowerbound, upperbound, depth);
		g = AlphaBetaWithMemory(root, beta-1, beta, 0, depth, 0);
		//printf("val=%d, depth=%d\n", g, depth);		
		if (g < beta) upperbound = g;
		else lowerbound = g;
		if (lowerbound >= upperbound){return g;};
	}
}
