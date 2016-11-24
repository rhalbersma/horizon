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

int move_played_is_first = 0;
int move_played_is_equal = 0;
int move_played_is_second = 0;
int move_played_is_other = 0;

short int previous_evaluation_value = 0;

char previous_move_list[512];
char previous_board[512];

FILE * log_file;

double move_start_time, move_finish_time, move_duration;

double budget_move_duration[17];
double safety_margin[17] = {0, 0.0015, 0.005, 0.015, 0.06, 0.2, 0.6, 2, 6, 20, 60, 130, 320, 500, 640, 900, 1200};
double total_time_allowed = 900;   // time in seconds
double total_moves_allowed = 75;
double white_total_time_used = 0;
double white_nr_of_moves_to_go = 75;
double black_total_time_used = 0;
double black_nr_of_moves_to_go = 75;

struct move_type played_move;

char result[256];
int result_length;
static char log_file_name[256];

void init_log_file()
{	int k,j;
	_fcloseall(); // make sure all previous opened (log) files are closed
	
	for (k=0; k< 1000; k++)
	{	errno_t errno;
		j =  sprintf_s(log_file_name, 255, "C:\\Documents and Settings\\feike\\Mijn documenten\\BoomstraDam\\project.3.1\\LogFile.%03d.txt", k);
		if ((errno = fopen_s(&log_file, log_file_name, "r")) != 0) break;  // get the first non existing file
		fclose(log_file);
	}

}
void write_levelAB(unsigned char distance_from_root, NodePnt node)
{	if (node->hash_key == 0x2A483E)
	{	fopen_s(&log_file, log_file_name, "a");
		fprintf_s(log_file, "enter AB at level: %d\n", distance_from_root);
		_fcloseall();
	}
}

void calc_move_duration_budget(NodePnt node)
{	int n;
	if (node == NULL)
	{	fopen_s(&log_file, log_file_name, "a");
		fprintf_s(log_file, "calc move duration with NULL pointer\n");
		_fcloseall();
		return;
	}
	if (node->status & IS_WHITE_ON_MOVE)
	{	if (white_nr_of_moves_to_go <= 0) white_nr_of_moves_to_go = 1;		// don't divde by zero
		for (n=2; n<17; n=n+2) budget_move_duration[n] = (total_time_allowed - white_total_time_used - safety_margin[n]) / white_nr_of_moves_to_go;
	}
	else // black is on move
	{	if (black_nr_of_moves_to_go <= 0) black_nr_of_moves_to_go = 1;		// don't divde by zero
		for (n=2; n<17; n=n+2) budget_move_duration[n] = (total_time_allowed - black_total_time_used - safety_margin[n]) / black_nr_of_moves_to_go;
	}
}
void print_64_on_log(unsigned long long ull64)  // print 64bits bitmap on X86 architecture
{	unsigned long long all64;
	unsigned long lower64;
	all64 = ull64; all64 = __ull_rshift(all64, 16);all64 = __ull_rshift(all64, 16);
	lower64 = (unsigned long) all64;
	fprintf_s(log_file, "%.8X%.8X ", lower64, ull64);
	return;
}
void print_board_on_log(struct tnode * node)
{	int i;
	if (node == NULL)
	{	fprintf_s(log_file, "Print empty node ??\n");
		return;
	};
	if (node->status & IS_WHITE_ON_MOVE) fprintf_s(log_file, "white on move\n"); else fprintf_s(log_file, "black on move\n");
	if (node->status & IS_MAX_NODE) fprintf_s(log_file, "max node"); else fprintf_s(log_file, "min node");
	fprintf_s(log_file, " %6X\n\n", node->hash_key);
	for (i=0; i < 5; i++)
	{	fprintf_s(log_file, "   %c %c %c %c %c\n", lc(node,10*i), lc(node,10*i+1), lc(node,10*i+2), lc(node,10*i+3), lc(node,10*i+4));
		fprintf_s(log_file, "  %c %c %c %c %c \n", lc(node,10*i+5), lc(node,10*i+6), lc(node,10*i+7), lc(node,10*i+8), lc(node,10*i+9)); }
	fprintf_s(log_file, "\n");
}

void print_move_on_log(WsPnt ws, int i)
{	char token;	
	if (ws->legal_moves[i].nr_of_captures == 0) token = '-'; else token = 'x';
	fprintf_s(log_file, "%2d%c%2d ", int_to_ext[ws->legal_moves[i].start], token, int_to_ext[ws->legal_moves[i].stop]);			
}

void print_moves_in_log(WsPnt ws)
{	int i;
	if (ws->nr_of_legal_moves > 0)	
		for (i=0; i<ws->nr_of_legal_moves; i++) {print_move_on_log(ws, i); fprintf_s(log_file, "\n");}
	else fprintf_s(log_file, "Empty move list\n");
}


void print_leading_spaces_on_log(int k)
{	int i;
	for (i=0; i< k; i++) fprintf_s(log_file, "                          ");
}

void Print_Value_from_Hash_on_log(NodePnt node, char level)
{	if (is_same_board_position(node, &hash[node->hash_key]))
	//	//(hash[node->hash_key].hash_key == distance_from_root))			// check if it is claimed by this node			
	//
	//	
	//	//(hash[node->hash_key].tree_depth == current_search_depth)			// check if it is claimed in this run
	{	fprintf_s(log_file, "(%6d %6d)     ", hash[node->hash_key].lowerbound, hash[node->hash_key].upperbound); /*print_64_on_log(node->hash_key) */; }
	else if (hash[node->hash_key].occ_empty == 0) // = initial preset
	{	fprintf_s(log_file, "empty hash entry    ");/*print_64_on_log(node->hash_key) ;*/ }
	else
	{	fprintf_s(log_file, "entry overwritten   ");/*print_64_on_log(node->hash_key) ; */}
}

void print_level_on_log(NodePnt node, char level, char depth, short int goal)
{	struct work_space_move_generator ws;
	struct tnode next_node;
	int j;	
	generate_legal_moves(node, &ws, node->status & IS_WHITE_ON_MOVE);
	if (ws.nr_of_legal_moves == 0) {fprintf_s(log_file, "No move\n"); return; }
	populate_node(&ws, &next_node, 0, node);
	print_move_on_log(&ws, 0);
	Print_Value_from_Hash_on_log(&next_node, level); 	
	if (level == depth) {fprintf_s(log_file, "\n"); }
	else {print_level_on_log(&next_node, level+1, depth, goal); } ;

	for (j=1; j<ws.nr_of_legal_moves; j++)
	{	populate_node(&ws, &next_node, j, node); 
		print_leading_spaces_on_log(level); 
		print_move_on_log(&ws, j); 
		Print_Value_from_Hash_on_log(&next_node, level); 
		if (level == depth) {fprintf_s(log_file, "\n"); }
		else { print_level_on_log(&next_node, level+1, depth, goal); }
	}
	fprintf_s(log_file, "\n"); return; 
}

void print_tree_on_log(NodePnt root, char depth)
{	struct work_space_move_generator ws;
	struct tnode next_node;
	int i;	
	short int goal=0;
	//printf("New\n");
	generate_legal_moves(root, &ws, root->status & IS_WHITE_ON_MOVE);
	if (ws.nr_of_legal_moves == 0) {fprintf_s(log_file, "No move\n"); return; }
	for (i=0; i<ws.nr_of_legal_moves; i++)
	{	populate_node(&ws, &next_node, i, root); 
		print_move_on_log(&ws, i); 
		Print_Value_from_Hash_on_log(&next_node, 1); 
		if (depth>0) print_level_on_log(&next_node, 1, depth, goal);
		else fprintf_s(log_file, "\n");
	}		
}

void print_move_list_from_hash_level_on_log(NodePnt node, short int goal, short int level)
{	struct tnode next_node;
	struct work_space_move_generator ws;
	unsigned long hash_key;
	if (level > 30) /* loop ?? */ return;

	generate_legal_moves(node, &ws, node->status & IS_WHITE_ON_MOVE);			// all legal moves collected in ws
	if (ws.nr_of_legal_moves == 0) return;		// no more moves to report
	
	populate_node(&ws, &next_node, hash[node->hash_key].best_move_idx, node);
	hash_key = next_node.hash_key;
	if (is_same_board_position(&next_node, &hash[hash_key]))
		if ((hash[hash_key].lowerbound == goal) && (hash[hash_key].upperbound == goal))
		{	// found the wanted move
			print_move_on_log(&ws, hash[node->hash_key].best_move_idx);// fprintf_s(log_file, "%X" , hash_key);
			if (node->status & IS_WHITE_ON_MOVE) fprintf_s(log_file, " ");
			else  fprintf_s(log_file, "\n");
			if (hash[hash_key].status & IS_EVALUATED) return; // last node to print
			print_move_list_from_hash_level_on_log(&next_node, goal, level+1);
			return; // done with this loop
		}
		else  // I don't know how this can happen
		{	fprintf_s(log_file, "%d %d problem??\n",hash[hash_key].lowerbound, hash[hash_key].upperbound );
		}
	else
	{	// so far, pity
		fprintf_s(log_file, "Hash %X overwritten\n", hash_key);
		//fprintf_s(log_file, "In hash:\n");
		//print_board_on_log(&hash[hash_key]);
		//fprintf_s(log_file, "Want to find: \n");
		//print_board_on_log(&next_node);
		//fprintf_s(log_file, "\n\nhash:               looking for:\n");
		//print_64_on_log(hash[hash_key].occ_empty); fprintf_s(log_file, "  "); print_64_on_log(next_node.occ_empty); fprintf_s(log_file, "\n");
		//print_64_on_log(hash[hash_key].white_black); fprintf_s(log_file, "  "); print_64_on_log(next_node.white_black); fprintf_s(log_file, "\n");
		//print_64_on_log(hash[hash_key].man_king); fprintf_s(log_file, "  "); print_64_on_log(next_node.man_king); fprintf_s(log_file, "\n");
		//print_64_on_log(hash[hash_key].status); fprintf_s(log_file, "  "); print_64_on_log(next_node.status); fprintf_s(log_file, "\n");
	}

}
void print_move_list_from_hash_on_log(NodePnt node, short int goal, short int level)
{	fprintf_s(log_file, "\n");
	if (!(node->status & IS_WHITE_ON_MOVE)) fprintf_s(log_file, "       ");
	print_move_list_from_hash_level_on_log(node, goal, level);
	fprintf_s(log_file, "\n-----------------\n");
}

void print_move_list_from_hash_level(NodePnt node, short int goal, short int level)
{	struct tnode next_node;
	struct work_space_move_generator ws;
	unsigned long hash_key;

	if (level > 30) return; // loop?

	generate_legal_moves(node, &ws, node->status & IS_WHITE_ON_MOVE);			// all legal moves collected in ws
	if (ws.nr_of_legal_moves == 0) return;		// no more moves to report
	
	populate_node(&ws, &next_node, hash[node->hash_key].best_move_idx, node);
	hash_key = next_node.hash_key;
	if (is_same_board_position(&next_node, &hash[hash_key]))
		if ((hash[hash_key].lowerbound == goal) && (hash[hash_key].upperbound == goal))
		{	// found the wanted move
			print_move(&ws, hash[node->hash_key].best_move_idx);// printf("%X" , hash_key);
			if (node->status & IS_WHITE_ON_MOVE) printf(" ");
			else  printf("\n");
			if (hash[hash_key].status & IS_EVALUATED) return; // last node to print
			print_move_list_from_hash_level(&next_node, goal, level + 1);
			return; // done with this loop
		}
		else  // best move is not set in case of early quit
		{	printf("\n");
		}
	else
	{	// so far, pity
		//printf("Hash %X overwritten\n", hash_key);
		//printf("In hash:\n");
		//print_board(&hash[hash_key]);
		//printf("Want to find: \n");
		//print_board(&next_node);
		//printf("\n\nhash:               looking for:\n");
		//print_64(hash[hash_key].occ_empty); printf("  "); print_64(next_node.occ_empty); printf("\n");
		//print_64(hash[hash_key].white_black); printf("  "); print_64(next_node.white_black); printf("\n");
		//print_64(hash[hash_key].man_king); printf("  "); print_64(next_node.man_king); printf("\n");
		//print_64(hash[hash_key].status); printf("  "); print_64(next_node.status); printf("\n");
	}

}
void print_move_list_from_hash(NodePnt node, short int goal)
{	if (!(node->status & IS_WHITE_ON_MOVE)) printf("       ");
	print_move_list_from_hash_level(node, goal,1);
	printf("\n\n");
}

void print_leading_spaces(int k)
{	int i;
	for (i=0; i< k; i++) printf("                          ");

}

void Print_Value_from_Hash(NodePnt node, char level)
{	if (is_same_board_position(node, &hash[node->hash_key]))
	//	//(hash[node->hash_key].hash_key == distance_from_root))			// check if it is claimed by this node			
	//
	//	
	//	//(hash[node->hash_key].tree_depth == current_search_depth)			// check if it is claimed in this run
	{	printf("%6d %6d %2d %2d ", hash[node->hash_key].lowerbound, hash[node->hash_key].upperbound,  hash[node->hash_key].hash_key, hash[node->hash_key].tree_depth); }
	else printf("no hash entry       ");
}


void print_level(NodePnt node, char level, char depth)
{	struct work_space_move_generator ws;
	struct tnode next_node;
	int i;	
	generate_legal_moves(node, &ws, node->status & IS_WHITE_ON_MOVE);
	if (ws.nr_of_legal_moves == 0) {printf("No move\n"); return; }
	if (ws.nr_of_legal_moves > 0)
	{	print_move(&ws, 0);
		populate_node(&ws, &next_node, 0, node); 
		Print_Value_from_Hash(&next_node, level); 
		if (level == depth) {printf("\n"); }
		else {print_level(&next_node, level+1, depth); } ;

		for (i=1; i<ws.nr_of_legal_moves; i++)
		{	print_leading_spaces(level); 
			print_move(&ws, i); 
			populate_node(&ws, &next_node, i, node); 
			Print_Value_from_Hash(&next_node, level); 
			if (level == depth) {printf("\n"); }
			else { print_level(&next_node, level+1, depth); }
		}
	}
}

void print_tree(NodePnt root, char depth)
{	print_level(root, 0, depth);
	
}

void convert_position(const char * ps, NodePnt node)
{	int i;
	switch (ps[0])
	{	case 'W':
			node->status = IS_MAX_NODE | IS_WHITE_ON_MOVE;
			break;
		case 'Z':
			node->status = IS_MAX_NODE;
			break;
		default:
			printf("Convert position -- invalid color: %c\n", ps[0]);
			break;
	}
	node->occ_empty   = 0;
	node->white_black = 0;
	node->man_king    = 0;
	for (i=1; i<51; i++)
	{
		switch (ps[i])
		{	case 'e':			// empty field
				break;
			case 'w':			// white man
				node->occ_empty   = node->occ_empty   | ext_to_bits[i];
				node->white_black = node->white_black | ext_to_bits[i];
				node->man_king    = node->man_king    | ext_to_bits[i];
				break;
			case 'W':			// white king
				node->occ_empty   = node->occ_empty   | ext_to_bits[i];
				node->white_black = node->white_black | ext_to_bits[i];
				break;
			case 'z':			// black man
				node->occ_empty   = node->occ_empty   | ext_to_bits[i];
				node->man_king    = node->man_king    | ext_to_bits[i];
				break;
			case 'Z':			// black king
				node->occ_empty   = node->occ_empty   | ext_to_bits[i];
				break;
			default:
				printf("Convert position -- invalid field: %c\n", ps[i]);
				break;
		}
	}
	node->hash_key = calc_initial_hash_key(node);
}

void convert_move(const char * ps, struct move_type * move)
{	int i, caption;
	sscanf_s(ps, "%2d%2d%2d", &(move->start), &(move->stop), &(move->nr_of_captures));
	ps += 6;
	move->captured = 0;
	printf("Convert move -- got: %d - %d, %d , %c %c\n", move->start, move->stop, move->nr_of_captures, ps[0], ps[1]);

	for (i=0; i<move->nr_of_captures; i++)
	{	sscanf_s(ps, "%2d", &caption);
		printf("%d\n", caption);
		move->captured = move->captured | ext_to_bits[caption];
		ps += 2;
	}
	printf("Convert move -- got: %d - %d\n", move->start, move->stop);
}

void swap_colors(NodePnt old_node, NodePnt new_node)
{	int i;
	new_node->man_king = 0;
	new_node->occ_empty = 0;
	new_node->white_black = 0;
	new_node->status = old_node->status ^ IS_WHITE_ON_MOVE;

	for (i=0; i<54; i++)
	{	if (old_node->man_king & bits[i])
			new_node->man_king = new_node->man_king | bits[ext_to_int[51 -int_to_ext[i]]];
		if (old_node->occ_empty & bits[i])
			new_node->occ_empty = new_node->occ_empty | bits[ext_to_int[51 -int_to_ext[i]]];
		if (!(old_node->white_black & bits[i]))
			new_node->white_black = new_node->white_black | bits[ext_to_int[51 -int_to_ext[i]]];
	}
	new_node->white_black = new_node->white_black & new_node->occ_empty;
}
		

void evaluate_board_position/*_for_eval*/(const char *ps) // used to evaluate board
{
	short int best_move_value = -INFINITY;
	struct tnode root;
	struct tnode swap;

	init_hash();
	init_log_file();
	convert_position(ps, &root);
	calc_piece_count(&root, &org_piece_count);
	root.status = root.status & ~IS_MAX_NODE; // eval is always called from a min-node
	print_board(&root);
	
	best_move_value = evaluate(&root, 1, 0);    // print the evaluation
	printf("\nTotal: %d\n", best_move_value);

	swap_colors(&root, &swap);

	if (best_move_value != evaluate(&swap,0, 0))
	{	print_board(&swap);
		best_move_value = evaluate(&swap, 1, 0);    // print the evaluation
		printf("\nTotal: %d\n", best_move_value);
		printf("\n\n\n                        -- Problem in eval with swap colors --\n\n\n\n");
	}
	//printf("Swap colors is temporary removed\n");
	return;

};
NodePnt get_end_node(NodePnt node, short int level)
{	unsigned long temp = node->hash_key;
	struct tnode next_node;
	struct work_space_move_generator ws;

	if (level > 30) return NULL;
	

	if (is_same_board_position(node, &hash[temp]))
	{	if (hash[temp].status & IS_EVALUATED) /* we are at the end */ return &hash[temp];
		generate_legal_moves(node, &ws, node->status & IS_WHITE_ON_MOVE);
		populate_node(&ws, &next_node, hash[temp].best_move_idx, node);
		NodePnt end_node = get_end_node(&next_node, level + 1);
		if (end_node == NULL) return &hash[temp]; // return the last known node
		else return end_node;
	}
	else return NULL;
}
//static char move_string[16];

void sample_eval(NodePnt root, WsPnt ws, int k)
{	NodePnt end_node;
	unsigned char best_move_idx = hash[root->hash_key].best_move_idx;
	char move_op;
	short int move_nr;
	// get node to be (re)evaluated
	end_node = get_end_node(root, 0);
	if (end_node == NULL) return;
	if (ws->max_nr_of_captures > 0) move_op = '-'; else move_op = 'x';
	//sprintf_s(move_string, 16, "%2d%c%2d\0", ws->legal_moves[best_move_idx].start, move_op, ws->legal_moves[best_move_idx].stop);
	if (root->status & IS_WHITE_ON_MOVE) move_nr =(short int)(total_moves_allowed - white_nr_of_moves_to_go); else move_nr = (short int)(total_moves_allowed - black_nr_of_moves_to_go);
	evaluate(end_node, 1, 0);
}
void open_log_file()
{	fopen_s(&log_file, log_file_name, "a");
}

unsigned char move_is_early_quit(NodePnt node, short int level)
{	unsigned long temp = node->hash_key;
	struct tnode next_node;
	struct work_space_move_generator ws;

	if (level > 30)
	{	fopen_s(&log_file, log_file_name, "a");
		fprintf_s(log_file, "loop detected\n");
		print_board_on_log(node);
		_fcloseall();
		return IS_EARLY_QUIT; // we don't know, so we assume the worst
	}

	if (is_same_board_position(node, &hash[temp]))
	{	if (hash[temp].status & IS_EVALUATED) /* we are at the end */ return hash[temp].status;
		generate_legal_moves(node, &ws, node->status & IS_WHITE_ON_MOVE);
		populate_node(&ws, &next_node, hash[temp].best_move_idx, node);
		return move_is_early_quit(&next_node, level + 1);
	}
	else return IS_EARLY_QUIT; // we don't know, so we assume the worst
}

void create_final_report()
{	// int i;
	printf("Final report:\n");
	printf("fst = %d\neql = %d\nsec = %d\noth = %d\n", move_played_is_first, move_played_is_equal, move_played_is_second, move_played_is_other); 
}



void print_node_info(NodePnt node)
{	if (node->hash_key > HASH_ARRAY_SIZE)
	{	printf("hash_key problem: key = %X\n", node->hash_key);
		fopen_s(&log_file, log_file_name, "a");
		fprintf_s(log_file, "hash_key problem: key = %X\n", node->hash_key);
		_fcloseall();
	}
	else
	{	fopen_s(&log_file, log_file_name, "a");
		fprintf_s(log_file, "lowerbound    %d\n", hash[node->hash_key].lowerbound);
		fprintf_s(log_file, "upperbound    %d\n", hash[node->hash_key].upperbound);
		fprintf_s(log_file, "best move idx %d\n", hash[node->hash_key].best_move_idx);
		fprintf_s(log_file, "status        %X\n", hash[node->hash_key].status);
		_fcloseall();
	}
}


void init_game(const char *ps)
{	init_hash();
	printf("%s\n", ps);
	sscanf_s(ps, "%lf %lf", &total_moves_allowed, &total_time_allowed);
	total_time_allowed = total_time_allowed*60;
	printf("Total moves allowed = %4.0f, time = %4.0f sec\n", total_moves_allowed, total_time_allowed);

	white_total_time_used = 0;
	white_nr_of_moves_to_go = total_moves_allowed;
	black_total_time_used = 0;
	black_nr_of_moves_to_go = total_moves_allowed;	
	init_log_file();
	printf("Init game\n)");
	fopen_s(&log_file, log_file_name, "a");
}

 short int abs_(short int a)
{	if (a < 0) return (-a);
	else return a;
}

void test_move_generator(const char *ps)
{	struct tnode root;
	//struct tnode next_node;
	int best_value=0;
	//struct work_space_move_generator ws;
	//int k;
	init_game("100100");
	convert_position(ps, &root);
	printf("Converted\n");
	nr_of_nodes_visited = 0;
	nr_of_evaluations = 0;
	nr_of_hits = 0;
	nr_of_overwrites = 0;

	best_value = mtdf_depth_limited(&root, 0, 2);
	fprintf_s(log_file, "Mtdf 2 --------------------------------------------------------\n");
	print_tree_on_log(&root, 0);
	best_value = mtdf_depth_limited(&root, best_value, 4);
	fprintf_s(log_file, "Mtdf 4 --------------------------------------------------------\n");
	print_tree_on_log(&root, 0);
	best_value = mtdf_depth_limited(&root, best_value, 6);
	fprintf_s(log_file, "Mtdf 6 --------------------------------------------------------\n");
	print_tree_on_log(&root, 0);
	best_value = mtdf_depth_limited(&root, best_value, 8);
	fprintf_s(log_file, "Mtdf 8 --------------------------------------------------------\n");
	print_tree_on_log(&root, 0);
	best_value = mtdf_depth_limited(&root, best_value, 10);
	fprintf_s(log_file, "Mtdf 10 --------------------------------------------------------\n");
	print_tree_on_log(&root, 0);
	best_value = mtdf_depth_limited(&root, best_value, 12);
	fprintf_s(log_file, "Mtdf 12 --------------------------------------------------------\n");
	print_tree_on_log(&root, 0);

	//print_board(&root);
	//generate_legal_moves(&root, &ws, root.status & IS_WHITE_ON_MOVE);
	//for (k=0; k<ws.nr_of_legal_moves; k++)
	//{	populate_node(&ws, &next_node, k, &root);
	//	print_board(&next_node);
	//}
	fclose(log_file);
}
 
 char * compare_calc_move_with_played_move(const char *ps, int analyse_only)
/***************************************************************
 *	This procedure is called to test the various parameters    *
 *	If it is called with a valid move, then this move is       *
 *	compared against the moves calculated by the engine        *
 *	otherwise it just displays the value of the various moves  *
 ***************************************************************/
{	int i, k;
	short int best_move_value = -INFINITY;
	short int sec_best_move_value = -INFINITY;
	short int third_best_move_value = -INFINITY;
	short int move_value_list[150];
	short int move_value;
	struct tnode root;
	struct tnode next_node;
	struct work_space_move_generator ws;
	char played_move_index = -1;
	char best_move_index = -1;
	char sec_best_move_index = -1;
	char third_best_move_index = -1;
	int current_max_depth;
	int analyse_depth;
	
//	init_log_file();
	nr_of_evaluations = 0;
	result[0] = 'N'; result[1]='x'; result[2] = 0; result_length = 3;
	skip_early_quit = 0;														// test on early quit conditions
	init_hash();
	convert_position(ps, &root);
	ps += 51;
	sscanf_s(ps, "%2d", &analyse_depth);
	ps += 2;
	if (analyse_only)
	{	printf("Analyse only\n");
		played_move.start = 0;
		played_move.stop = 0;
	} else convert_move(ps, &played_move);
	init_log_file();
	calc_piece_count(&root, &org_piece_count);
	generate_legal_moves(&root, &ws, root.status & IS_WHITE_ON_MOVE);			// all legal moves collected in ws
	if (ws.nr_of_legal_moves > 1)				// no time on lost game and forced move
	{	for (i=0; i < ws.nr_of_legal_moves; i++)
		{	print_move(&ws, i); printf("( ");
			populate_node(&ws, &next_node, i, &root);
			move_value = 0;
			nr_of_nodes_visited = 0;
			nr_of_evaluations = 0;
			nr_of_hits = 0;
			nr_of_overwrites = 0;
			current_max_depth = analyse_depth;
			int start_value; if (ws.max_nr_of_captures) start_value = 2; else start_value = 1;
			for (k=start_value; k <= current_max_depth; k=k+2)
			{	// collecting
				move_value = mtdf_depth_limited(&next_node, move_value, k);
				//if ((move_value == +INFINITY-1 ) || (move_value == -INFINITY+1 )) break;				
			};
			printf("%d )\n", move_value);

			print_move_list_from_hash(&next_node, move_value);
			open_log_file();
			print_move_on_log(&ws, i); fprintf_s(log_file,"( %d )\n", move_value); 
			print_move_list_from_hash_on_log(&next_node, move_value, 0);
			NodePnt end_node = get_end_node(&next_node, 0);
			print_board_on_log(end_node);
			evaluate(end_node,1,1);

			_fcloseall();


			//print_tree_on_log(&next_node, 4);
			move_value_list[i] = move_value;
			if (move_value >= best_move_value) { sec_best_move_value = best_move_value; sec_best_move_index = best_move_index; best_move_value= move_value; best_move_index = i; }
			else if (move_value > sec_best_move_value) {sec_best_move_value = move_value; sec_best_move_index = i;}
			if ((ws.legal_moves[i].start    == played_move.start)  &&
				(ws.legal_moves[i].stop     == played_move.stop)   &&
				(ws.legal_moves[i].captured == played_move.captured))
				played_move_index = i;
		};

// We have collected the values for each legal move in this position and we know the move played in the sample game
// There are at least two legal moves in this situation

		if (played_move_index >= 0)
		{	if ((move_value_list[played_move_index] == move_value_list[best_move_index]) &&
				(move_value_list[played_move_index] != move_value_list[sec_best_move_index])) move_played_is_first++;
			if ((move_value_list[played_move_index] == move_value_list[best_move_index]) &&
				(move_value_list[played_move_index] == move_value_list[sec_best_move_index])) move_played_is_equal++;
			if ((move_value_list[played_move_index] != move_value_list[best_move_index]) &&
				(move_value_list[played_move_index] == move_value_list[sec_best_move_index])) move_played_is_second++;
			if ((move_value_list[played_move_index] != move_value_list[best_move_index]) &&
				(move_value_list[played_move_index] != move_value_list[sec_best_move_index])) move_played_is_other++;
		
			print_board(&root);

			printf("\nmove (p)    : %2d%c%2d value = %d\n", int_to_ext[ws.legal_moves[played_move_index].start], ' ', int_to_ext[ws.legal_moves[played_move_index].stop], move_value_list[played_move_index]);
			printf("    move (1): %2d%c%2d value = %d\n", int_to_ext[ws.legal_moves[best_move_index].start], ' ', int_to_ext[ws.legal_moves[best_move_index].stop], move_value_list[best_move_index]);
			printf("    move (2): %2d%c%2d value = %d\n", int_to_ext[ws.legal_moves[sec_best_move_index].start], ' ', int_to_ext[ws.legal_moves[sec_best_move_index].stop], move_value_list[sec_best_move_index]);
		}
		else // this is for the test mode: analyse board position
		{
			
			for (i=0; i<ws.nr_of_legal_moves; i++)
			{	printf("move    : %2d%c%2d value = %d\n", int_to_ext[ws.legal_moves[i].start], ' ', int_to_ext[ws.legal_moves[i].stop], move_value_list[i]);
			}

			print_board(&root);
			printf("best move: %2d%c%2d value = %d\n", int_to_ext[ws.legal_moves[best_move_index].start], ' ', int_to_ext[ws.legal_moves[best_move_index].stop], move_value_list[best_move_index]);
		}

	}  // if ws.nr_of_legal_moves > 0
	return result;

};

void get_best_move(const char *ps)  // called from play-mode
/******************************************************************
 *	This procedure is called in case the engine has to generate   *
 *	just the best move, no alternatives are evaluated.            *
 *  It increases the search depth untill time is up               *
 ******************************************************************/
{	int i, j;
	unsigned char k;			// current depth
	short int best_move_value;
	struct tnode root;
	struct work_space_move_generator ws;
	int test = 0;
	char best_move_op = '-';
	struct move_type * best_move_pnt;
	int piece_count;
	unsigned long capture_list[21];
	char nr_of_captures;
		
	_fcloseall(); //log_file only open during writing, due to program crashes analyses
	result[0] = 'E'; result[1]='x'; result[2] = 0; result_length = 3;
	skip_early_quit = 0;														// 0 = test on early quit conditions
	convert_position(ps, &root);
	//print_board(&root);
	if (root.status & IS_WHITE_ON_MOVE)
	{	 if (white_nr_of_moves_to_go <= 0) return;
	}
	else
	{	if (black_nr_of_moves_to_go <= 0) return;
	}

	calc_piece_count(&root, &org_piece_count);
	piece_count = org_piece_count.nr_of_black_king + org_piece_count.nr_of_black_man + org_piece_count.nr_of_white_king + org_piece_count.nr_of_white_man;
	generate_legal_moves(&root, &ws, root.status & IS_WHITE_ON_MOVE);
	if (ws.nr_of_legal_moves == 0) {printf("No legal moves anymore ...\n"); fclose(log_file); return; }   // return empty string to stop process - lost
	if (ws.nr_of_legal_moves == 1)												// don't spend time on the onvermijdelijke
	{	j =  sprintf_s(result,     255,     "N%2d", int_to_ext[ws.legal_moves[0].start]);
		j += sprintf_s(result + j, 255 - j, "%2d", int_to_ext[ws.legal_moves[0].stop]);
		count_bits_and_fill_list(ws.legal_moves[0].captured, nr_of_captures, capture_list)
		j += sprintf_s(result + j, 255 - j, "%2d", nr_of_captures);
		for (i=0; i < nr_of_captures; i++) j += sprintf_s(result + j, 255 - j, "%2d", int_to_ext[capture_list[i]]);
		j += sprintf_s(result + j, 255 - j, "\0");
		result_length = j + 1;

		previous_evaluation_value = - previous_evaluation_value;
		if (root.status & IS_WHITE_ON_MOVE)
		{	white_total_time_used += 1.0;
			--white_nr_of_moves_to_go;
		}
		else
		{	black_total_time_used += 1.0;
			--black_nr_of_moves_to_go;
		}
		if (ws.legal_moves[0].nr_of_captures == 0) best_move_op = '-'; else best_move_op = 'x' ;
		
		if (root.status & IS_WHITE_ON_MOVE) printf("%2.0f.", total_moves_allowed - white_nr_of_moves_to_go);
		else printf("%2.0f.", total_moves_allowed - black_nr_of_moves_to_go);
		printf(" %d%c%d forced move\n", int_to_ext[ws.legal_moves[0].start], best_move_op, int_to_ext[ws.legal_moves[0].stop]);

		fopen_s(&log_file, log_file_name, "a");
		if (root.status & IS_WHITE_ON_MOVE) fprintf_s(log_file, "%2.0f.", total_moves_allowed - white_nr_of_moves_to_go);
		else fprintf_s(log_file, "%2.0f.", total_moves_allowed - black_nr_of_moves_to_go);
		fprintf_s(log_file, " %d%c%d forced move\n", int_to_ext[ws.legal_moves[0].start], best_move_op, int_to_ext[ws.legal_moves[0].stop]);
		_fcloseall();
		printf("%s", result);
		return;
	};

	init_hash();
	best_move_value = 0;  // start the guess with 0
	nr_of_nodes_visited = 0;
	nr_of_evaluations = 0;
	nr_of_hits = 0;
	nr_of_overwrites = 0;

restart:

	move_start_time = clock();
	calc_move_duration_budget(&root);
	k = 2;		// only even nr of search depth used. Eval always in min-node.
	while (1)
	{	// collecting
		best_move_value = mtdf_depth_limited(&root, best_move_value, k);
		move_finish_time = clock();
		//sample_eval(&root, &ws, k);
		move_duration = (double)(move_finish_time - move_start_time) / CLOCKS_PER_SEC;
		fopen_s(&log_file, log_file_name, "a");
		fprintf_s(log_file, "           duration: %6.2f, budget: %6.2f, k: %d", move_duration, budget_move_duration[k],k ); print_move_on_log(&ws, hash[root.hash_key].best_move_idx); fprintf_s(log_file, " (%d)\n", best_move_value);
		_fcloseall();
		printf("           duration: %6.2f, budget: %6.2f, k: %d ", move_duration, budget_move_duration[k],k ); print_move(&ws, hash[root.hash_key].best_move_idx); printf(" (%d)\n", best_move_value);
		//if ((best_move_value == +INFINITY-1 ) || (best_move_value == -INFINITY+1 )) { break; }		
		//if (k == 12) break;
		if (k >= 8) { if (move_duration < 0.2 * budget_move_duration[k+2]) k = k +2; else break; }
		else { if (move_duration < 0.40 * budget_move_duration[k+2]) k = k +2; else break; }

	}; // end while


	if (move_is_early_quit(&root, 0) & IS_EARLY_QUIT)
	{	if (best_move_value - (best_move_value/1000)*1000 == 0)
			if (skip_early_quit == 0)
			{	skip_early_quit = 1; 
				fopen_s(&log_file, log_file_name, "a");
				fprintf_s(log_file, "Restart due to early quit\n"); 
				_fcloseall();
				printf("Restart due to early quit\n"); 
				if (root.status & IS_WHITE_ON_MOVE) white_total_time_used += move_duration;
				else black_total_time_used += move_duration;
				goto restart; } 		// test on early quit conditions
	}
	if (!(is_same_board_position(&root, &hash[root.hash_key]))) 
	{		fopen_s(&log_file, log_file_name, "a");
			fprintf_s(log_file, "Root not in hash ?? \n");
			_fcloseall();
			best_move_pnt = &ws.legal_moves[0]; printf("Root not in hash!\n"); 
	}
	else
	{	// collect best move info from hash
		//print_node_info(&root);
		best_move_pnt = & ws.legal_moves[hash[root.hash_key].best_move_idx];
		if (best_move_pnt->nr_of_captures) best_move_op = 'x'; else best_move_op = '-';
		{	unsigned long next_hash_key;
			struct tnode next_node;
			populate_node(&ws, &next_node, hash[root.hash_key].best_move_idx, &root);
			next_hash_key = next_node.hash_key;
			if (is_same_board_position(&next_node, &hash[root.hash_key]))
			{	if (best_move_value != hash[next_hash_key].lowerbound) { printf("No match lowerbound with best move value\n"); }
				if (best_move_value != hash[next_hash_key].upperbound) { printf("No match upperbound with best move value\n"); }
			}
		}
	}
	
	if (root.status & IS_WHITE_ON_MOVE)
	{	white_total_time_used += move_duration;
		--white_nr_of_moves_to_go;
		if (white_nr_of_moves_to_go < 0) {fclose(log_file); return; }
	}
	else
	{	black_total_time_used += move_duration;
		--black_nr_of_moves_to_go;
		if (black_nr_of_moves_to_go < 0) {fclose(log_file); return; }
	}
	
	if (test)
	{	if ((previous_evaluation_value != 0) && (abs_(best_move_value + previous_evaluation_value) >= 1000))  // this is ment in computer - computer tests so prev+current = 0
		{	// test only: if there is a big difference, display info and stop moving
			if (root.status & IS_WHITE_ON_MOVE)
			{	printf("%2.0f. %d%c%d (%d) (%d) %4.0f sec\n", (total_moves_allowed - white_nr_of_moves_to_go), int_to_ext[best_move_pnt->start], best_move_op, int_to_ext[best_move_pnt->stop], best_move_value, k, white_total_time_used);
				fprintf_s(log_file, "%2.0f. %d%c%d (%d) (%d) %4.0f sec\n", (total_moves_allowed - white_nr_of_moves_to_go), int_to_ext[best_move_pnt->start], best_move_op, int_to_ext[best_move_pnt->stop], best_move_value, k, white_total_time_used);
				print_move_list_from_hash_on_log(&root, best_move_value, 0);
				//print_tree_on_log(&root, 3); fprintf_s(log_file, "------------------------##--------------------\n\n");
			}
			else
			{	printf("%2.0f. %d%c%d (%d) (%d) %4.0f sec\n", (total_moves_allowed - black_nr_of_moves_to_go), int_to_ext[best_move_pnt->start], best_move_op, int_to_ext[best_move_pnt->stop], best_move_value, k, black_total_time_used);
				fprintf_s(log_file, "%2.0f. %d%c%d (%d) (%d) %4.0f sec\n", (total_moves_allowed - black_nr_of_moves_to_go), int_to_ext[best_move_pnt->start], best_move_op, int_to_ext[best_move_pnt->stop], best_move_value, k, black_total_time_used);
				print_move_list_from_hash_on_log(&root, best_move_value, 0);
				//print_tree_on_log(&root, 3); fprintf_s(log_file, "------------------------##--------------------\n\n");
			}
			printf("Stopped due to big jump in eval\n");
			fclose(log_file);
			return;   // return empty string to stop process
		}
		else
		{	if (best_move_value == 0) previous_evaluation_value = 1; else previous_evaluation_value = best_move_value;
		}
	}
	//fopen_s(&log_file, log_file_name, "a");
	//fprintf_s(log_file, "Going to create move in result\n");
	//_fcloseall();
	
	j =  sprintf_s(result,     255,     "N%2d", int_to_ext[best_move_pnt->start]);
	j += sprintf_s(result + j, 255 - j, "%2d",  int_to_ext[best_move_pnt->stop]);
	count_bits_and_fill_list(best_move_pnt->captured, nr_of_captures, capture_list)
	j += sprintf_s(result + j, 255 - j, "%2d", nr_of_captures);
	for (i=0; i < nr_of_captures; i++) j += sprintf_s(result + j, 255 - j, "%2d", int_to_ext[capture_list[i]]);
	j += sprintf_s(result + j, 255 - j, "\0");
	result_length = j + 1;

	fopen_s(&log_file, log_file_name, "a");
	if (root.status & IS_WHITE_ON_MOVE)
	{	printf("%2.0f. %d%c%d (%d) (%d) %4.0f sec norm = %4.0f\n", (total_moves_allowed - white_nr_of_moves_to_go), int_to_ext[best_move_pnt->start], best_move_op, int_to_ext[best_move_pnt->stop], best_move_value, k, white_total_time_used, ((total_moves_allowed - white_nr_of_moves_to_go)/total_moves_allowed)*total_time_allowed);
		fprintf_s(log_file, "%2.0f. %d%c%d (%d) (%d) %4.0f sec norm = %4.0f\n", (total_moves_allowed - white_nr_of_moves_to_go), int_to_ext[best_move_pnt->start], best_move_op, int_to_ext[best_move_pnt->stop], best_move_value, k, white_total_time_used, ((total_moves_allowed - white_nr_of_moves_to_go)/total_moves_allowed)*total_time_allowed);
		print_move_list_from_hash_on_log(&root, best_move_value, 0);
		print_tree_on_log(&root, 2); fprintf_s(log_file, "------------------------##--------------------\n\n");
	}
	else
	{	printf("%2.0f. %d%c%d (%d) (%d) %4.0f sec norm = %4.0f\n", (total_moves_allowed - black_nr_of_moves_to_go), int_to_ext[best_move_pnt->start], best_move_op, int_to_ext[best_move_pnt->stop], best_move_value, k, black_total_time_used, ((total_moves_allowed - black_nr_of_moves_to_go)/total_moves_allowed)*total_time_allowed);
		fprintf_s(log_file, "%2.0f. %d%c%d (%d) (%d) %4.0f sec norm = %4.0f\n", (total_moves_allowed - black_nr_of_moves_to_go), int_to_ext[best_move_pnt->start], best_move_op, int_to_ext[best_move_pnt->stop], best_move_value, k, black_total_time_used, ((total_moves_allowed - black_nr_of_moves_to_go)/total_moves_allowed)*total_time_allowed);
		print_move_list_from_hash_on_log(&root, best_move_value, 0);
		print_tree_on_log(&root, 2); fprintf_s(log_file, "------------------------##--------------------\n\n");
	}
	_fcloseall();
	return;
};
char * analyse_timing(const char *ps) // used to test duration expansion
{	int k;
	short int best_move_value = -INFINITY;
	short int sec_best_move_value = -INFINITY;
	short int third_best_move_value = -INFINITY;
	short int move = 0 ;
	struct tnode root;
	char played_move_index = -1;
	char best_move_index = -1;
	char sec_best_move_index = -1;
	char third_best_move_index = -1;
	short int move_value;
	struct work_space_move_generator ws;
	char piece_count;

	result[0] = 'T'; result[1]='x'; result[2] = 0; result_length = 3;
	init_hash();
	convert_position(ps, &root);
	calc_piece_count(&root, &org_piece_count);
	piece_count = org_piece_count.nr_of_black_king + org_piece_count.nr_of_black_man + org_piece_count.nr_of_white_king + org_piece_count.nr_of_white_man;
	generate_legal_moves(&root, &ws, root.status & IS_WHITE_ON_MOVE);
	if (ws.nr_of_legal_moves <2 ) { return result; } // no action, forced move or no more moves

	move_value = 0;
	nr_of_nodes_visited = 0;
	nr_of_evaluations = 0;
	nr_of_hits = 0;
	nr_of_overwrites = 0;
	k=2;
	move_start_time = clock();
	move_duration = 0;
	_fcloseall();
	for (k=2; k<12; k=k+2)
	{	printf("enter mtdf with k = %d\n", k);
		move_value = mtdf_depth_limited(&root, move_value, k);

		move_finish_time = clock();
		move_duration = (double)(move_finish_time - move_start_time) / CLOCKS_PER_SEC;

		printf("%d\t%5.3f\t%d\t%d\t%d\t%d\t", k, move_duration,	nr_of_nodes_visited, nr_of_evaluations, nr_of_hits, piece_count);
		fopen_s(&log_file, log_file_name, "a");
		fprintf_s(log_file, "%d\t%5.3f\t%d\t%d\t%d\t%d\t", k, move_duration,	nr_of_nodes_visited, nr_of_evaluations, nr_of_hits, piece_count);
		_fcloseall();
		//if ((move_value == +INFINITY-1 ) || (move_value == -INFINITY+1 )) break;				
		if ((k == 8) &&  (move_duration > 2.0)) break;


	}
	printf("exit for k = %d\n", k);
	fopen_s(&log_file, log_file_name, "a");
	fprintf_s(log_file, "\n");
	_fcloseall();
	
	return result;
}
