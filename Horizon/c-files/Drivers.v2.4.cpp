/*
  This file is Copyright (C) 2010 Feike Boomstra.
  Distributed under the Boost Software License, Version 1.0.
  (See accompanying file LICENSE_1_0.txt or copy at
  http://www.boost.org/LICENSE_1_0.txt)
*/


#include "BoomstraDam.v2.5.h"
#include <malloc.h>
#include <sys/types.h>
#include <sys/times.h>
#include <sys/time.h>
#include <time.h>

#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h> /* close */
#include <memory.h>
#include <pthread.h>

// v1.2
//
#define training 1 // stop on endgame database result
#define remote158 1 // stop if search result is in endgame database
//#define livedraughts 1

extern long pos_level[200];
/*
int move_played_is_first = 0;
int move_played_is_equal = 0;
int move_played_is_second = 0;
int move_played_is_other = 0;
*/
short int previous_evaluation_value = 0;
/*
char previous_move_list[512];
char previous_board[512];

FILE * log_file;
FILE * td_file;
FILE * wweight_file;
FILE * bweight_file;
*/
double move_start_time, move_finish_time, move_duration, move_init_time;

//double budget_move_duration[17];
//double safety_margin[17] = {0, 0.0015, 0.005, 0.015, 0.06, 0.2, 0.6, 2, 6, 20, 60, 130, 320, 500, 640, 900, 1200};
int total_time_allowed = 400;   // time in seconds
int total_moves_allowed = 75;
int white_total_time_used = 0;
int white_nr_of_moves_to_go = 75;
int black_total_time_used = 0;
int black_nr_of_moves_to_go = 75;

double tot_move_duration = 0;
double last_iteration_duration = 0;

int64_t tot_nr_of_nodes_visited = 0;

//unsigned long long white_item_count[31] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
//unsigned long long black_item_count[31] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

struct timeval before;
struct timeval after;

//int global_move_nr = 1;

struct stand_type stand_stack[200];
int stand_stack_pnt = 0;

bool hash_is_corrupted = false;

struct move_type played_move;

char result[2048];
int result_length;
static char log_file_name[256];
/*
void print_item_counters(){
	printf("%12lld, %12lld, man\n", white_item_count[1], black_item_count[1]);
	printf("%12lld, %12lld, king\n", white_item_count[2], black_item_count[2]);
	printf("%12lld, %12lld, man_position\n", white_item_count[3], black_item_count[3]);
	printf("%12lld, %12lld, king_position\n", white_item_count[4], black_item_count[4]);
	printf("%12lld, %12lld, row\n", white_item_count[5], black_item_count[5]);
	printf("%12lld, %12lld, man_locked_bonus\n", white_item_count[6], black_item_count[6]);
	printf("%12lld, %12lld, avoid_4641\n", white_item_count[7], black_item_count[7]);
	printf("%12lld, %12lld, avoid_2324\n", white_item_count[8], black_item_count[8]);
	printf("%12lld, %12lld, avoid_2335\n", white_item_count[9], black_item_count[9]);
	printf("%12lld, %12lld, 2_8_13\n", white_item_count[10], black_item_count[10]);
	printf("%12lld, %12lld, 253035\n", white_item_count[11], black_item_count[11]);
	printf("%12lld, %12lld, klassiek\n", white_item_count[12], black_item_count[12]);
	printf("%12lld, %12lld, tempi\n", white_item_count[13], black_item_count[13]);
	printf("%12lld, %12lld, ketting_stelling\n", white_item_count[14], black_item_count[14]);
	printf("%12lld, %12lld, ketting_stelling_2\n", white_item_count[15], black_item_count[15]);
	printf("%12lld, %12lld, halve_hek\n", white_item_count[16], black_item_count[16]);
	printf("%12lld, %12lld, active_15\n", white_item_count[17], black_item_count[17]);
	printf("%12lld, %12lld, slechte_binding\n", white_item_count[18], black_item_count[18]);
	printf("%12lld, %12lld, free_path\n", white_item_count[19], black_item_count[19]);
	printf("%12lld, %12lld, fit\n", white_item_count[20], black_item_count[20]);
	printf("%12lld, %12lld, edge_35\n", white_item_count[21], black_item_count[21]);
	printf("%12lld, %12lld, edge_36\n", white_item_count[22], black_item_count[22]);
	printf("%12lld, %12lld, 11_16_17\n", white_item_count[23], black_item_count[23]);
	printf("%12lld, %12lld, voorpost\n", white_item_count[24], black_item_count[24]);
	printf("%12lld, %12lld, locked\n", white_item_count[25], black_item_count[25]);
	printf("%12lld, %12lld, vleugel_opsluiting\n", white_item_count[26], black_item_count[26]);
	printf("%12lld, %12lld, tandem\n", white_item_count[27], black_item_count[27]);
	printf("%12lld, %12lld, mobility\n", white_item_count[28], black_item_count[28]);
	printf("%12lld, %12lld, maze_penalty\n", white_item_count[29], black_item_count[29]);
	printf("%12lld, %12lld, canon\n", white_item_count[30], black_item_count[30]);
}
*/
void copy_hash_to_node(struct hash_node * HashNode, NodePnt node) {
	node->man_king = HashNode->man_king;
	node->occ_empty = HashNode->occ_empty;
	node->white_black = HashNode->white_black;
	node->status = HashNode->status;
	node->depth = HashNode->depth;
	//node->move_status = HashNode->move_status;

	// re-calculate the numbers
	node->nr_of_white_man  = count((node->occ_empty) & (node->white_black) & (node->man_king));
	node->nr_of_white_king = count((node->occ_empty) & (node->white_black) & ~(node->man_king));
	node->nr_of_black_man  = count((node->occ_empty) & ~(node->white_black) & (node->man_king));
	node->nr_of_black_king = count((node->occ_empty) & ~(node->white_black) & ~(node->man_king));
	node->total_nr_of_pieces = node->nr_of_white_man + node->nr_of_white_king + node->nr_of_black_man + node->nr_of_black_king;

}

void copy_to_hash(NodePnt node, short int search_depth){
	hash[node->hash_key].best_move_idx = 0;
	hash[node->hash_key].distance_from_root = current_ply_nr;
	hash[node->hash_key].man_king = node->man_king;
	hash[node->hash_key].occ_empty = node->occ_empty;
	hash[node->hash_key].white_black = node->white_black;
	hash[node->hash_key].search_depth = search_depth;
	//hash[node->hash_key].move_status = node->move_status;
	hash[node->hash_key].status = node->status;
	hash[node->hash_key].upperbound = +INFINITY;
	hash[node->hash_key].lowerbound = -INFINITY;

}

void print_board_from_hash_node(struct hash_node * HashNode) {
	tnode node;
	copy_hash_to_node(HashNode, &node);
	print_board(&node);
}

short int evaluate_from_hash(struct hash_node * HashNode, int a, int b) {
	tnode node;
	copy_hash_to_node(HashNode, &node);
	return evaluate(&node, a, b, 0);
}

// to check the status of all threads
void print_thread_state(void) {
	for (int i = 0; i < 4; i++) {
		if (Threads[i].idle) printf("[%d].idle: true\n", i); else printf("[%d].idle: false\n", i);
		if (Threads[i].stop) printf("[%d].stop: true\n", i); else printf("[%d].stop: false\n", i);
		if (Threads[i].running) printf("[%d].running: true\n", i); else printf("[%d].running: false\n", i);
		if (Threads[i].workIsWaiting) printf("[%d].workIsWaiting: true\n", i); else printf("[%d].workIsWaiting: false\n", i);
	    printf("[%d].activeSplitPoints = %d\n", i, Threads[i].activeSplitPoints);
	    printf("Threads[%d].splitPoint = %x\n", i, Threads[i].splitPoint);
	}
}



int64_t get_nr_of_nodes_visited(void) {
	int64_t result = 0ULL;
	for(int i = 0; i < ActiveThreads; i++)
		result += Threads[i].nr_of_nodes_visited;
	return result;
}
int64_t get_nr_of_nodes_evaluated(void) {
	int64_t result = 0ULL;
	for(int i = 0; i < ActiveThreads; i++)
		result += Threads[i].nr_of_nodes_evaluated;
	return result;
}
int64_t get_nr_of_hits(void) {
	int64_t result = 0ULL;
	for(int i = 0; i < ActiveThreads; i++)
		result += Threads[i].nr_of_hits;
	return result;
}
int64_t get_nr_of_overwrites(void) {
	int64_t result = 0ULL;
	for(int i = 0; i < ActiveThreads; i++)
		result += Threads[i].nr_of_overwrites;
	return result;
}
int64_t get_avg_distance_from_root(void) {
	int64_t result = 0ULL;
	for(int i = 0; i < ActiveThreads; i++)
		result += Threads[i].avg_distance_from_root;
	return result;
}
int64_t get_nr_of_best_move_only_hits(void) {
	int64_t result = 0ULL;
	for(int i = 0; i < ActiveThreads; i++)
		result += Threads[i].nr_of_best_move_only_hits;
	return result;
}
unsigned int get_max_distance_from_root(void) {
	unsigned int result = 0;
	for(int i = 0; i < ActiveThreads; i++)
		if (Threads[i].max_distance_from_root > result) result = Threads[i].max_distance_from_root;
	return result - (float) current_ply_nr;

}
float get_avg_distance(void) {
	if (get_nr_of_nodes_evaluated())
		return (((float)(get_avg_distance_from_root()))/((float)get_nr_of_nodes_evaluated())) - (float) current_ply_nr;
	else
		return 0;
}

void clear_statistic_counters(void){
	for(int i = 0; i < THREAD_MAX; i++) {
		//statistics
		Threads[i].avg_distance_from_root = 0;
		Threads[i].max_distance_from_root = 0;
		Threads[i].nr_of_best_move_only_hits = 0;
		Threads[i].nr_of_hits = 0;
		Threads[i].nr_of_nodes_evaluated = 0;
		Threads[i].nr_of_nodes_visited = 0;
		Threads[i].nr_of_overwrites = 0;
		Threads[i].nr_useless_sacrifice = 0;
		Threads[i].useless_called = 0;
		Threads[i].useless_endgame = 0;
		Threads[i].useless_equal = 0;
		Threads[i].useless_both_king = 0;
		Threads[i].useless_one_king = 0;
		Threads[i].useless_run_away = 0;

	}
}

void display_detailed_counters(void) {
	printf("visited  :"); for(int i = 0; i < THREAD_MAX; i++) { printf("\t%5d", Threads[i].nr_of_nodes_visited/1000);} printf("\t%6d\n", get_nr_of_nodes_visited());
	printf("evaluated:"); for(int i = 0; i < THREAD_MAX; i++) { printf("\t%5d", Threads[i].nr_of_nodes_evaluated/1000);} printf("\t%6d\n", get_nr_of_nodes_evaluated());
	printf("hash hits:"); for(int i = 0; i < THREAD_MAX; i++) { printf("\t%5d", Threads[i].nr_of_hits/1000);} printf("\t%6d\n", get_nr_of_hits());
	printf("hash ovwr:"); for(int i = 0; i < THREAD_MAX; i++) { printf("\t%5d", Threads[i].nr_of_overwrites/1000);} printf("\n");
	printf("best move:"); for(int i = 0; i < THREAD_MAX; i++) { printf("\t%5d", Threads[i].nr_of_best_move_only_hits/1000);} printf("\n");
	printf("u  called:"); for(int i = 0; i < THREAD_MAX; i++) { printf("\t%5d", Threads[i].useless_called/1000);} printf("\n");
	printf("u endgame:"); for(int i = 0; i < THREAD_MAX; i++) { printf("\t%5d", Threads[i].useless_endgame/1000);} printf("\n");
	printf("u   equal:"); for(int i = 0; i < THREAD_MAX; i++) { printf("\t%5d", Threads[i].useless_equal/1000);} printf("\n");
	printf("ubothking:"); for(int i = 0; i < THREAD_MAX; i++) { printf("\t%5d", Threads[i].useless_both_king/1000);} printf("\n");
	printf("uone king:"); for(int i = 0; i < THREAD_MAX; i++) { printf("\t%5d", Threads[i].useless_one_king/1000);} printf("\n");
	printf("u runaway:"); for(int i = 0; i < THREAD_MAX; i++) { printf("\t%5d", Threads[i].useless_run_away);} printf("\n");
	printf("useless  :"); for(int i = 0; i < THREAD_MAX; i++) { printf("\t%5d", Threads[i].nr_useless_sacrifice/1000);} printf("\n");
}
/*
void init_log_file()
{	int k,j;
	//printf("init_log_file\n");

	for (k=0; k< 1000; k++)
	{
		j =  sprintf(log_file_name, "/home/feike/BoomstraDam/log-files/LogFile.%03d.txt", k);
		//printf("try %s\n", log_file_name);
		if ((log_file = fopen(log_file_name, "r")) == NULL) break;  // get the first non existing file
		fclose(log_file);
	}
		printf("log file = %s\n", log_file_name);
}
*/
//char td_file_name[256];
/*
void init_td_file()
{	int k,j;
	for (k=0; k< 1000; k++)
	{
		j =  sprintf(td_file_name, "/home/feike/BoomstraDam/TDlearning/td_out.%03d.txt", k);
		//printf("try %s\n", log_file_name);
		if ((td_file = fopen(td_file_name, "r")) == NULL) break;  // get the first non existing file
		fclose(td_file);
	}
		td_file = fopen(td_file_name, "w");
		if (td_file == NULL) printf("can't create td file %s\n", td_file_name);
		else {
			printf("td_file is: %s\n", td_file_name);
			fclose(td_file);
		}
}

void init_weight_file()
{	int k,j;
	char weight_file_name[256];
	for (k=0; k< 1000; k++)
	{
		j =  sprintf(weight_file_name, "/home/feike/BoomstraDam/TDlearning/wweights.%03d.txt", k);
		//printf("try %s\n", log_file_name);
		if ((wweight_file = fopen(weight_file_name, "r")) == NULL) break;  // get the first non existing file
		fclose(wweight_file);
	}
	// but we wanted the last existing one
	j =  sprintf(weight_file_name, "/home/feike/BoomstraDam/TDlearning/wweights.%03d.txt", k-1);
	wweight_file = fopen(weight_file_name, "r");
	if (wweight_file == NULL) printf("can't read white wait file %s\n", weight_file_name);
	else printf("wweight_file is: %s\n", weight_file_name);

	for (k=0; k< 1000; k++)
	{
		j =  sprintf(weight_file_name, "/home/feike/BoomstraDam/TDlearning/bweights.%03d.txt", k);
		//printf("try %s\n", log_file_name);
		if ((bweight_file = fopen(weight_file_name, "r")) == NULL) break;  // get the first non existing file
		fclose(bweight_file);
	}
	// but we wanted the last existing one
	j =  sprintf(weight_file_name, "/home/feike/BoomstraDam/TDlearning/bweights.%03d.txt", k-1);
	bweight_file = fopen(weight_file_name, "r");
	if (bweight_file == NULL) printf("can't read black wait file %s\n", weight_file_name);
	else printf("bweight_file is: %s\n", weight_file_name);
}
*/
/*
void calc_move_duration_budget(NodePnt node)
{	int n;
	if (node == NULL)
	{	//log_file = fopen(log_file_name, "a");
		//fprintf(log_file, "calc move duration with NULL pointer\n");
		//fclose(log_file);
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
*/
double cur_avg_move_time;
int absolute_max_move_time;

bool do_next_depth(NodePnt node, double used_till_now, int idx1, int idx2, int idx3, int k) {
	double cur_remaining_time;
	int remaining_moves;

	if (node->status & IS_WHITE_ON_MOVE) {
		if (white_nr_of_moves_to_go <= 0) white_nr_of_moves_to_go = 1;		// don't divde by zero
		cur_remaining_time = total_time_allowed - white_total_time_used;
		cur_avg_move_time = (cur_remaining_time)/ white_nr_of_moves_to_go;
		remaining_moves = white_nr_of_moves_to_go - 1;
		if (remaining_moves == 0) remaining_moves = 1;
	}
	else {
		if (black_nr_of_moves_to_go <= 0) black_nr_of_moves_to_go = 1;		// don't divde by zero
		cur_remaining_time = total_time_allowed - black_total_time_used;
		cur_avg_move_time = (cur_remaining_time)/ black_nr_of_moves_to_go;
		remaining_moves = black_nr_of_moves_to_go - 1;
		if (remaining_moves == 0) remaining_moves = 1;
	}
	if (used_till_now > cur_avg_move_time/3.5)
		{ //printf("stopped used: %5.3f, cur_avg = %5.3f\n",used_till_now, cur_avg_move_time/2.5);
		  //printf("remaining = %6.2f, used = %6.2f, min_rest = %6.2f, max = %6.2f\n", cur_remaining_time, used_till_now, 0.9*cur_avg_move_time*remaining_moves, (cur_remaining_time - used_till_now - 0.9*cur_avg_move_time*remaining_moves));
		  return false; // around 4 is the average multiplication factor, 1 sec initialisation
		}
	if (remaining_moves){
		//printf("remaining = %6.2f, used = %6.2f, min_rest = %6.2f, max = %6.2f\n", cur_remaining_time, used_till_now, 0.9*cur_avg_move_time*remaining_moves, (cur_remaining_time - used_till_now - 0.9*cur_avg_move_time*remaining_moves));
		absolute_max_move_time = (int) cur_remaining_time - used_till_now - 0.95*cur_avg_move_time*remaining_moves;
		if (idx1 != -1 && idx1==idx2 && idx1==idx3 && k >= 12)
			if (2*cur_avg_move_time < absolute_max_move_time) absolute_max_move_time = 2*cur_avg_move_time;
		return true;
	}
	else {
		if (used_till_now > cur_avg_move_time/10.0) return false;
		else {
			absolute_max_move_time = cur_remaining_time - 1;
			return true;
		}
	}
}
/*
void print_64_on_log(unsigned long ull64)  // print 64bits bitmap on X86 architecture
{	unsigned long long all64;
	unsigned int lower64;
	all64 = ull64; all64 = all64 >> 16; all64 = all64 >> 16;
	lower64 = (unsigned long) all64;
	fprintf(log_file, "%.8X%.8lX ", lower64, ull64);
	return;
}
*/
/*
void print_board_on_log(struct tnode * node)
{	int i;
	if (node == NULL)
	{	fprintf(log_file, "Print empty node ??\n");
		return;
	};
	if (node->status & IS_WHITE_ON_MOVE) fprintf(log_file, "white on move\n"); else fprintf(log_file, "black on move\n");
	if (node->status & IS_MAX_NODE) fprintf(log_file, "max node"); else fprintf(log_file, "min node");
	fprintf(log_file, " %6X\n\n", node->hash_key);
	for (i=0; i < 5; i++)
	{	fprintf(log_file, "   %c %c %c %c %c\n", lc(node,10*i), lc(node,10*i+1), lc(node,10*i+2), lc(node,10*i+3), lc(node,10*i+4));
		fprintf(log_file, "  %c %c %c %c %c \n", lc(node,10*i+5), lc(node,10*i+6), lc(node,10*i+7), lc(node,10*i+8), lc(node,10*i+9)); }
	fprintf(log_file, "\n");
}
*/
void print_move(WsPnt ws, int i)
{	char token;
	if ((i < 0) || (i > ws->nr_of_legal_moves)) printf("Illegal move nr: %d\n", i);
	else if (ws->max_nr_of_captures == 0) token = '-'; else token = 'x';
	printf("%2d%c%2d ", int_to_ext(ws->legal_moves[i].start), token, int_to_ext(ws->legal_moves[i].stop));			/* +1 gives external representation */
}

void print_moves(WsPnt ws)
{	unsigned int i;
	if (ws->nr_of_legal_moves > 0)
		for (i=0; i<ws->nr_of_legal_moves; i++) {print_move(ws, i); printf("\n");}
	else printf("Empty move list\n");
}
/*
void print_move_on_log(WsPnt ws, int i)
{	char token;
	if ((i < 0) || (i > ws->nr_of_legal_moves)) fprintf(log_file,"Illegal move nr: %d\n", i);
	else if (ws->max_nr_of_captures == 0) token = '-'; else token = 'x';
	fprintf(log_file, "%2d%c%2d ", int_to_ext(ws->legal_moves[i].start), token, int_to_ext(ws->legal_moves[i].stop));
}
*/
void print_move_on_result(WsPnt ws, int i)
{	char token;

	if (ws->max_nr_of_captures == 0) token = '-'; else token = 'x';
	result_length += sprintf(result + result_length, "%2d%c%2d ", int_to_ext(ws->legal_moves[i].start), token, int_to_ext(ws->legal_moves[i].stop));
}
/*
void print_moves_in_log(WsPnt ws)
{	unsigned int i;
	if (ws->nr_of_legal_moves > 0)
		for (i=0; i<ws->nr_of_legal_moves; i++) {print_move_on_log(ws, i); fprintf(log_file, "\n");}
	else fprintf(log_file, "Empty move list\n");
}


void print_leading_spaces_on_log(int k)
{	int i;
	for (i=0; i< k; i++) fprintf(log_file, "                                ");
}

void Print_Value_from_Hash_on_log(NodePnt node, char level)
{	if (is_same_board_position(node, &hash[node->hash_key]) && (hash[node->hash_key].search_depth == current_search_depth))
	{	fprintf(log_file, "%6X(%6d %6d)[%2d] ", node->hash_key, hash[node->hash_key].lowerbound, hash[node->hash_key].upperbound, hash[node->hash_key].best_move_idx); print_64_on_log(node->hash_key) ; }
	else if (is_same_board_position(node, &hash[node->hash_key]))
	{	fprintf(log_file, "      prev  %2d %2d %x", hash[node->hash_key].distance_from_root, hash[node->hash_key].search_depth, node->hash_key);}
	else if (hash[node->hash_key].occ_empty == 0) // = initial preset
	{	fprintf(log_file, "      empty %2d %2d         ", hash[node->hash_key].distance_from_root, hash[node->hash_key].search_depth);}
	else
	{	fprintf(log_file, "      overw %2d %2d %6x  ", hash[node->hash_key].distance_from_root, hash[node->hash_key].search_depth, node->hash_key);}
}
*/

int best_move_from_hash(NodePnt node)
{	if (is_same_board_position(node, &hash[node->hash_key]) && (hash[node->hash_key].search_depth == current_search_depth))
		return hash[node->hash_key].best_move_idx;
	else
		return -1;
}

/*
void print_level_on_log(NodePnt node, char level, char depth, short int goal)
{	struct work_space_move_generator ws;
	struct tnode next_node;
	unsigned int j;
	generate_legal_moves(node, &ws, node->status & IS_WHITE_ON_MOVE, best_move_from_hash(node));
	if (ws.nr_of_legal_moves == 0) {fprintf(log_file, "No move\n"); return; }
	populate_node(&ws, &next_node, ws.move_order[0], node);
	print_move_on_log(&ws, ws.move_order[0]);
	Print_Value_from_Hash_on_log(&next_node, level);
	if (level == depth) {fprintf(log_file, "\n"); }
	else { print_level_on_log(&next_node, level+1, depth, goal); }
	for (j=1; j<ws.nr_of_legal_moves; j++)
	{	populate_node(&ws, &next_node, ws.move_order[j], node);
		print_leading_spaces_on_log(level);
		print_move_on_log(&ws, ws.move_order[j]);
		Print_Value_from_Hash_on_log(&next_node, level);
		if (level == depth) {fprintf(log_file, "\n"); }
		else { print_level_on_log(&next_node, level+1, depth, goal); }
	}
	fprintf(log_file, "\n"); return;
}

void print_tree_on_log(NodePnt root, char depth)
{	struct work_space_move_generator ws;
	struct tnode next_node;
	unsigned int i;
	short int goal=0;
	//printf("New\n");
	generate_legal_moves(root, &ws, root->status & IS_WHITE_ON_MOVE, -1);
	if (ws.nr_of_legal_moves == 0) {fprintf(log_file, "No move\n"); return; }
	for (i=0; i<ws.nr_of_legal_moves; i++)
	{	populate_node(&ws, &next_node, ws.move_order[i], root);
		fprintf(log_file, "*");
		print_move_on_log(&ws, i);
		Print_Value_from_Hash_on_log(&next_node, 1);
		if (depth>0) print_level_on_log(&next_node, 1, depth, goal);
		else fprintf(log_file, "\n");
	}
}

*/
/*
void print_move_list_from_hash_on_result(NodePnt node, short int goal, short int level)
{	struct tnode next_node;
	struct work_space_move_generator ws;
	unsigned int hash_key;
	if (level > 30)  return;

	generate_legal_moves(node, &ws, node->status & IS_WHITE_ON_MOVE, -1);			// all legal moves collected in ws
	if (ws.nr_of_legal_moves == 0) return;		// no more moves to report

	populate_node(&ws, &next_node, hash[node->hash_key].best_move_idx, node);
	hash_key = next_node.hash_key;
	if (is_same_board_position(&next_node, &hash[hash_key]))
		if ((hash[hash_key].lowerbound == goal) && (hash[hash_key].upperbound == goal) &&
			(hash[hash_key].search_depth == current_search_depth) &&
			(hash[hash_key].distance_from_root == level+1))
		{	// found the wanted move
			print_move_on_result(&ws, hash[node->hash_key].best_move_idx);
			//fprintf(log_file, " %lX %lX %lX" , hash[node->hash_key].occ_empty, hash[node->hash_key].white_black, hash[node->hash_key].man_king);
			result_length += sprintf(result + result_length," ");
			if (hash[hash_key].status & IS_EVALUATED) return; // last node to print
			print_move_list_from_hash_on_result(&next_node, goal, level+1);
			return; // done with this loop
		}
		else
			// I don't know how this can happen
		{	fprintf(log_file, "problem?? in  list low=%d, high=%d, k=%d, dist=%d\n",
			hash[hash_key].lowerbound, hash[hash_key].upperbound, hash[hash_key].search_depth, hash[hash_key].distance_from_root );
		}
	else
	{	// so far, pity
		fprintf(log_file, "Hash %X overwritten\n", hash_key);
		//fprintf(log_file, "In hash:\n");
		//print_board_on_log(&hash[hash_key]);
		//fprintf(log_file, "Want to find: \n");
		//print_board_on_log(&next_node);
		//fprintf(log_file, "\n\nhash:               looking for:\n");
		//print_64_on_log(hash[hash_key].occ_empty); fprintf(log_file, "  "); print_64_on_log(next_node.occ_empty); fprintf(log_file, "\n");
		//print_64_on_log(hash[hash_key].white_black); fprintf(log_file, "  "); print_64_on_log(next_node.white_black); fprintf(log_file, "\n");
		//print_64_on_log(hash[hash_key].man_king); fprintf(log_file, "  "); print_64_on_log(next_node.man_king); fprintf(log_file, "\n");
		//print_64_on_log(hash[hash_key].status); fprintf(log_file, "  "); print_64_on_log(next_node.status); fprintf(log_file, "\n");
	}

}

void print_move_list_from_hash_level_on_log(NodePnt node, short int goal, short int level)
{	struct tnode next_node;
	struct work_space_move_generator ws;
	unsigned int hash_key;
	if (level > 30)  return;

	generate_legal_moves(node, &ws, node->status & IS_WHITE_ON_MOVE, -1);			// all legal moves collected in ws
	if (ws.nr_of_legal_moves == 0) return;		// no more moves to report

	populate_node(&ws, &next_node, hash[node->hash_key].best_move_idx, node);
	hash_key = next_node.hash_key;
	if (is_same_board_position(&next_node, &hash[hash_key]))
		if ((hash[hash_key].lowerbound == goal) && (hash[hash_key].upperbound == goal) &&
			(hash[hash_key].search_depth == current_search_depth) &&
			(hash[hash_key].distance_from_root == level+1))
		{	// found the wanted move
			print_move_on_log(&ws, hash[node->hash_key].best_move_idx);
			//fprintf(log_file, " %lX %lX %lX" , hash[node->hash_key].occ_empty, hash[node->hash_key].white_black, hash[node->hash_key].man_king);
			if (node->status & IS_WHITE_ON_MOVE) fprintf(log_file, " ");
			else  fprintf(log_file, "\n");
			if (hash[hash_key].status & IS_EVALUATED) return; // last node to print
			print_move_list_from_hash_level_on_log(&next_node, goal, level+1);
			return; // done with this loop
		}
		else
			// I don't know how this can happen
		{	fprintf(log_file, "problem?? in  list low=%d, high=%d, k=%d, dist=%d\n",
			hash[hash_key].lowerbound, hash[hash_key].upperbound, hash[hash_key].search_depth, hash[hash_key].distance_from_root );
		}
	else
	{	// so far, pity
		fprintf(log_file, "Hash %X overwritten\n", hash_key);
		//fprintf(log_file, "In hash:\n");
		//print_board_on_log(&hash[hash_key]);
		//fprintf(log_file, "Want to find: \n");
		//print_board_on_log(&next_node);
		//fprintf(log_file, "\n\nhash:               looking for:\n");
		//print_64_on_log(hash[hash_key].occ_empty); fprintf(log_file, "  "); print_64_on_log(next_node.occ_empty); fprintf(log_file, "\n");
		//print_64_on_log(hash[hash_key].white_black); fprintf(log_file, "  "); print_64_on_log(next_node.white_black); fprintf(log_file, "\n");
		//print_64_on_log(hash[hash_key].man_king); fprintf(log_file, "  "); print_64_on_log(next_node.man_king); fprintf(log_file, "\n");
		//print_64_on_log(hash[hash_key].status); fprintf(log_file, "  "); print_64_on_log(next_node.status); fprintf(log_file, "\n");
	}

}
void print_move_list_from_hash_on_log(NodePnt node, short int goal, short int level)
{	fprintf(log_file, "\n");
	if (!(node->status & IS_WHITE_ON_MOVE)) fprintf(log_file, "       ");
	print_move_list_from_hash_level_on_log(node, goal, level);
	fprintf(log_file, "\n-----------------\n");
}
*/
int count_move_list_from_hash(NodePnt node, short int goal, short int level)
{	struct tnode next_node;
	struct work_space_move_generator ws;
	unsigned long hash_key;

	if (level > 30) return level; // loop?

	generate_legal_moves(node, &ws, node->status & IS_WHITE_ON_MOVE, -1);			// all legal moves collected in ws
	if (ws.nr_of_legal_moves == 0) return level;		// no more moves to report

	populate_node(&ws, &next_node, hash[node->hash_key].best_move_idx, node);
	hash_key = next_node.hash_key;
	if (is_same_board_position(&next_node, &hash[hash_key]))
		if ((hash[hash_key].lowerbound == goal) &&
		    (hash[hash_key].upperbound == goal) &&
		    (hash[hash_key].search_depth == current_search_depth))
		{	// found the wanted move
			if (hash[hash_key].status & IS_EVALUATED) return level;

			return count_move_list_from_hash(&next_node, goal, level + 1);; // done with this loop
		}
		else  // best move is not set in case of early quit
		{	return level-1;
		}
	return level;
}
void print_move_list_from_hash_level(NodePnt node, short int goal, short int level)
{	struct tnode next_node;
	struct work_space_move_generator ws;
	unsigned long hash_key;

	if (level > 30) return; // loop?

	generate_legal_moves(node, &ws, node->status & IS_WHITE_ON_MOVE, -1);			// all legal moves collected in ws
	if (ws.nr_of_legal_moves == 0) return;		// no more moves to report

	populate_node(&ws, &next_node, hash[node->hash_key].best_move_idx, node);
	hash_key = next_node.hash_key;
	if (is_same_board_position(&next_node, &hash[hash_key]))
		if ((hash[hash_key].lowerbound == goal) &&
		    (hash[hash_key].upperbound == goal) &&
		    (hash[hash_key].search_depth == current_search_depth))
		{	// found the wanted move
			print_move(&ws, hash[node->hash_key].best_move_idx);// printf("%X" , hash_key);
			if (node->status & IS_WHITE_ON_MOVE) printf(" ");
			else  printf("\n");
			if (hash[hash_key].status & IS_EVALUATED)
			{	if (hash[hash_key].status & IS_USELESS_SACRIFICE)
				{	printf("useless=%d\n", hash[hash_key].lowerbound);
					return; // last node to print
				}
				else
				{	printf("low=%d, high=%d, eval=%d\n", hash[hash_key].lowerbound, hash[hash_key].upperbound, evaluate_from_hash(&hash[hash_key],0,0));
					return; // last node to print
				}
			}
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
	//	//(hash[node->hash_key].search_depth == current_search_depth)			// check if it is claimed in this run
	{	printf("%6d %6d %2d %2d ", hash[node->hash_key].lowerbound, hash[node->hash_key].upperbound,  hash[node->hash_key].distance_from_root, hash[node->hash_key].search_depth); }
	else printf("no hash entry       ");
}


void print_level(NodePnt node, char level, char depth)
{	struct work_space_move_generator ws;
	struct tnode next_node;
	unsigned int i;
	generate_legal_moves(node, &ws, node->status & IS_WHITE_ON_MOVE, -1);
	if (ws.nr_of_legal_moves == 0) {printf("No move\n"); return; }
	if (ws.nr_of_legal_moves > 0)
	{	print_move(&ws, 0);
		for (i=1; i<ws.nr_of_legal_moves; i++)
		{	print_leading_spaces(level);
			print_move(&ws, i);
			populate_node(&ws, &next_node, ws.move_order[i], node);
			Print_Value_from_Hash(&next_node, level);
			if (level == depth) {printf("\n"); }
			else { print_level(&next_node, level+1, depth); }
		}
	}
}

void print_tree(NodePnt root, char depth)
{	print_level(root, 0, depth);

}

void store_stand(NodePnt node)
{
	if ((stand_stack_pnt < 200) && ((~node->man_king) & (node->occ_empty)))  // not full and contains kings
	{	stand_stack[stand_stack_pnt].man_king = node->man_king;
		stand_stack[stand_stack_pnt].white_black = node->white_black;
		stand_stack[stand_stack_pnt].occ_empty = node->occ_empty;
		printf("stand stored\n");
		stand_stack_pnt++;
	};
}

int check_stand(NodePnt node) // count the number of times te same stand is found
{	int counter = 0;
	for (int loc_pnt = stand_stack_pnt-1;loc_pnt >=0; loc_pnt--)
	{	if (stand_stack[stand_stack_pnt].man_king    == node->man_king) goto check_end;
		if (stand_stack[stand_stack_pnt].white_black == node->white_black) goto check_end;
		if (stand_stack[stand_stack_pnt].occ_empty == node->occ_empty) goto check_end;
		counter++;
	check_end: ;
	};
	//if (counter > 0){ printf("stand found\n"); print_board(node); }
	return counter;
}


void convert_position(const char * ps, NodePnt node, int * current_ply_nr)
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
	node->depth = 0;
	//node->move_status = MIN_NODE_DID_FREE_MOVE;

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
	node->nr_of_white_man  = (unsigned char)__builtin_popcountll( node->man_king & node->occ_empty &  node->white_black);
	node->nr_of_black_man  = (unsigned char)__builtin_popcountll( node->man_king & node->occ_empty & ~node->white_black);
	node->nr_of_white_king = (unsigned char)__builtin_popcountll(~node->man_king & node->occ_empty &  node->white_black);
	node->nr_of_black_king = (unsigned char)__builtin_popcountll(~node->man_king & node->occ_empty & ~node->white_black);
	node->total_nr_of_pieces = node->nr_of_white_man + node->nr_of_white_king + node->nr_of_black_man + node->nr_of_black_king;
	node->previous_node = NULL;
	//node->mutual_sacrifice = 0;

	// read the ply_nr:
	ps += 51;
	sscanf(ps, "%3d", current_ply_nr);
	ps += 3;

}
/*
void convert_move(const char * ps, struct move_type * move)
{	int i, caption;
	sscanf(ps, "%2d%2d%2d", &(move->start), &(move->stop), &(move->nr_of_captures));
	ps += 6;
	move->captured = 0;
	printf("Convert move -- got: %d - %d, %d , %c %c\n", move->start, move->stop, move->nr_of_captures, ps[0], ps[1]);

	for (i=0; i<move->nr_of_captures; i++)
	{	sscanf(ps, "%2d", &caption);
		printf("%d\n", caption);
		move->captured = move->captured | ext_to_bits[caption];
		ps += 2;
	}
	printf("Convert move -- got: %d - %d\n", move->start, move->stop);
}
*/
void swap_colors(NodePnt old_node, NodePnt new_node)
{	int i;
	new_node->man_king = 0;
	new_node->occ_empty = 0;
	new_node->white_black = 0;
	new_node->status = old_node->status ^ IS_WHITE_ON_MOVE;

	for (i=0; i<54; i++)
	{	if (old_node->man_king & bits[i])
			new_node->man_king = new_node->man_king | bits[(int)ext_to_int[51 -(int)int_to_ext_list[i]]];
		if (old_node->occ_empty & bits[i])
			new_node->occ_empty = new_node->occ_empty | bits[(int)ext_to_int[51 -(int)int_to_ext_list[i]]];
		if (!(old_node->white_black & bits[i]))
			new_node->white_black = new_node->white_black | bits[(int)ext_to_int[51 -(int)int_to_ext_list[i]]];
	}
	new_node->white_black = new_node->white_black & new_node->occ_empty;
	new_node->nr_of_white_man  = (unsigned char)__builtin_popcountll( new_node->man_king & new_node->occ_empty &  new_node->white_black);
	new_node->nr_of_black_man  = (unsigned char)__builtin_popcountll( new_node->man_king & new_node->occ_empty & ~new_node->white_black);
	new_node->nr_of_white_king = (unsigned char)__builtin_popcountll(~new_node->man_king & new_node->occ_empty &  new_node->white_black);
	new_node->nr_of_black_king = (unsigned char)__builtin_popcountll(~new_node->man_king & new_node->occ_empty & ~new_node->white_black);
	new_node->total_nr_of_pieces = new_node->nr_of_white_man + new_node->nr_of_white_king + new_node->nr_of_black_man + new_node->nr_of_black_king;


}


void evaluate_board_position/*_for_eval*/(char *ps) // used to evaluate board
{
	short int best_move_value = 0;
	struct tnode root;
	struct tnode swap;
	struct work_space_move_generator ws;
	int print_flag;

	//init_hash();
	convert_position(ps, &root, &current_ply_nr);
	ps += 51 + 3;
	sscanf(ps, "%1d", &print_flag);

	result[0] = 'S'; result[1]='x'; result[2] = 0; result_length = 3;

	calc_piece_count(&root, &org_piece_count);
	if (root.status & IS_WHITE_ON_MOVE)
	{	if (org_piece_count.nr_of_white_man + org_piece_count.nr_of_white_king == 0) best_move_value = -INFINITY;
		if (org_piece_count.nr_of_black_man + org_piece_count.nr_of_black_king == 0) best_move_value = +INFINITY;
	}
	else
	{	if (org_piece_count.nr_of_white_man + org_piece_count.nr_of_white_king == 0) best_move_value = +INFINITY;
		if (org_piece_count.nr_of_black_man + org_piece_count.nr_of_black_king == 0) best_move_value = -INFINITY;
	}

	root.status = root.status & ~IS_MAX_NODE;
	if (print_flag) print_board(&root);

	generate_legal_moves(&root, &ws, root.status & IS_WHITE_ON_MOVE, 0);			// all legal moves collected in ws
	if (ws.nr_of_legal_moves == 0) best_move_value = -INFINITY;

	if (best_move_value == 0)
	{
		best_move_value = evaluate(&root, print_flag, 0, 0);    // print the evaluation
		if (print_flag) printf("\nTotal: %d\n", -1*best_move_value);

		swap_colors(&root, &swap);
		calc_piece_count(&swap, &org_piece_count);
		if (best_move_value != evaluate(&swap,0, 0, 0))
		{	print_board(&swap);
			best_move_value = evaluate(&swap, print_flag, 0, 0);    // print the evaluation
			printf("\nTotal: %d\n", best_move_value);
			printf("\n\n\n                        -- Problem in eval with swap colors --\n\n\n\n");
		}
		//printf("Swap colors is temporary removed\n");
		int j =  sprintf(result,"S%6d", -best_move_value);
		j += sprintf(result + j, "\0");
		result_length = j + 1;

		// to test the move generator

/*		for (int i = 0; i<ws.nr_of_legal_moves; i++){
			print_bits(ws.legal_moves[i].start);
			print_bits(ws.legal_moves[i].stop);
			print_bits(ws.legal_moves[i].captured);
			printf("-----\n");
		}

*/
		return;
	}
	else
	{	if (print_flag) printf("no legal move, result = %d\n", best_move_value);
		sprintf(result,"S%6d\n", best_move_value);
	}
	return;
};
void evaluate_board_position_2/*_for_eval*/(char *ps) // used to evaluate board
{
	short int best_move_value = -INFINITY;
	struct tnode root; root.previous_node = NULL;

	init_hash();
	convert_position(ps, &root, &current_ply_nr);
	ps += 51 + 3;

	calc_piece_count(&root, &org_piece_count);
	root.status = root.status & ~IS_MAX_NODE; // eval is always called from a min-node
	best_move_value = evaluate(&root, 0, 1, 0);    // sprintf in result
	Idle = true;
	return;

};
void evaluate_board_position_t/*for test*/(char *ps) // used to evaluate board
{
	//short int best_move_value = -INFINITY;
	struct tnode root; root.previous_node = NULL;

	init_hash();
	convert_position(ps, &root, &current_ply_nr);
	ps += 51 + 3;

	calc_piece_count(&root, &org_piece_count);
	root.status = root.status & ~IS_MAX_NODE; // eval is always called from a min-node
	printf("not on capure: %d\n",other_side_is_not_on_capture (&root));
	Idle = true;
	return;

};

HashPnt get_end_node(NodePnt node, short int goal, short int level)
{	unsigned long cur_key = node->hash_key;
	unsigned long nxt_key ;
	struct tnode next_node;
	struct work_space_move_generator ws;
	int save_best_move_indx = hash[cur_key].best_move_idx;
	//BitArray save_occ_empty = hash[cur_key].occ_empty;
	HashPnt end_node;
	char current_depth = hash[cur_key].depth;
	unsigned char current_move_status;// = hash[cur_key].move_status;

	if (level > 40) return NULL;

	if (is_same_board_position(node, &hash[cur_key]) &&
			(hash[cur_key].lowerbound == goal) && (hash[cur_key].upperbound == goal) &&
			(hash[cur_key].search_depth == current_search_depth) //&&
			//(hash[cur_key].distance_from_root == level + current_ply_nr)
			) {
		//printf("hash search depth: %d, current search: %d, current ply nr %d\n", hash[cur_key].search_depth, current_search_depth,  current_ply_nr);
		//printf("hash distance: %d, current level: %d\n", hash[cur_key].distance_from_root, level);
		if (hash[cur_key].status & (IS_EVALUATED|IS_EXACT_VALUE)) { /* we are at the end */
			return &hash[cur_key];
		}
		generate_legal_moves(node, &ws, node->status & IS_WHITE_ON_MOVE, -1);
		populate_node(&ws, &next_node, hash[cur_key].best_move_idx, node);

		// test if next node is in hash
		nxt_key = next_node.hash_key;
		if (!(is_same_board_position(&next_node, &hash[nxt_key]) &&
				(hash[nxt_key].lowerbound == goal) && (hash[nxt_key].upperbound == goal) &&
				(hash[nxt_key].search_depth == current_search_depth) &&
				(hash[nxt_key].distance_from_root == current_ply_nr + level+1))) {
			// next hash node not found
			//printf("try to recovery at level = %d, depth = %d, goal = %d, node =\n", level, current_depth, goal);
			hash[nxt_key].occ_empty = 0; // free the next hash location
			hash[nxt_key].search_depth = -1;
			hash[cur_key].occ_empty = 0; // to prevent getting the result back form hash
			hash[cur_key].search_depth = -1;

			int next_depth;
			if ((ws.nr_of_legal_moves == 1)) next_depth = current_depth; else next_depth = current_depth - 1;

			int g = search(node, goal, goal+1, current_ply_nr + level, next_depth, 0, true);
			g = search(node, goal-1, goal, current_ply_nr + level, next_depth, 0, true);
			if (save_best_move_indx != hash[cur_key].best_move_idx) {
				if (g != goal) {
					//printf("Problem, goal changed: g= %d, goal = %d\n", g, goal);
					//printf("Problem: best move changed: %d, to %d\n", hash[cur_key].best_move_idx, save_best_move_indx);
				}
				else { // who cares
					// re-populate
					//printf("Best move changed: %d, to %d\n", hash[cur_key].best_move_idx, save_best_move_indx);
					populate_node(&ws, &next_node, hash[cur_key].best_move_idx, node);
					nxt_key = next_node.hash_key;
				}
			}

			// now it should be ok

			if (is_same_board_position(&next_node, &hash[nxt_key]) &&
					(hash[nxt_key].lowerbound == goal) && (hash[nxt_key].upperbound == goal) &&
					(hash[nxt_key].search_depth == current_search_depth) &&
					(hash[nxt_key].distance_from_root == current_ply_nr+level+1)) {
				// is ok now, hash entry restored
				end_node = get_end_node(&next_node, goal, level + 1);
				//if (end_node == NULL) printf("recovery chain at level = %d, depth = %d, goal = %d\n", level, hash[node->hash_key].depth, goal);
				return end_node;
			}
			else {
				//printf("recovery failed at level = %d, depth = %d, goal = %d, node:\n", level, hash[cur_key].depth, goal);
				//print_board(&next_node);
				//printf("hash = \n");
				//printf("upper = %d, lower = %d, dist = %d\n", hash[nxt_key].upperbound, hash[nxt_key].lowerbound, hash[nxt_key].distance_from_root);
				//print_board_from_hash_node(&hash[nxt_key]);
				return NULL;

			}
		}
		else // everything was ok
			end_node = get_end_node(&next_node, goal, level + 1);
			//if (end_node == NULL) printf("recovery chain at level = %d, depth = %d, goal = %d\n", level, hash[cur_key].depth, goal);
			return end_node;
	}
	else
		return NULL;
}


//static char move_string[16];
/*
void sample_eval(NodePnt root, WsPnt ws, int k)
{	NodePnt end_node;
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
*/
/*
void open_log_file()
{	log_file = fopen( log_file_name, "a");
}

FILE * open_hash_dump(int nr){
	char hash_dump_name[256];
	sprintf(hash_dump_name, "/home/feike/BoomstraDam/log-files/HashDump.%03d.dmp", nr);
	return fopen(hash_dump_name, "w");

}

unsigned char move_is_early_quit(NodePnt node, short int level)
{	unsigned long temp = node->hash_key;
	struct tnode next_node;
	struct work_space_move_generator ws;

	if (level > 30)
	{	//log_file = fopen( log_file_name, "a");
		//fprintf(log_file, "loop detected\n");
		//print_board_on_log(node);
		//fcloseall();
		return IS_USELESS_SACRIFICE; // we don't know, so we assume the worst
	}

	if (is_same_board_position(node, &hash[temp]))
	{	if (hash[temp].status & IS_EVALUATED)  return hash[temp].status;
		generate_legal_moves(node, &ws, node->status & IS_WHITE_ON_MOVE, -1);
		populate_node(&ws, &next_node, hash[temp].best_move_idx, node);
		return move_is_early_quit(&next_node, level + 1);
	}
	else return IS_USELESS_SACRIFICE; // we don't know, so we assume the worst
}
*/

void create_final_report()
{	int k,j;
	char log_file_name[100];
	FILE* perf_file;
	result[0]='F';result[1] = 0; result_length = 2;

for (k=0; k< 1000; k++)
{
	j =  sprintf(log_file_name, "/home/feike/BoomstraDam/log-files/PerformanceFile.%03d.txt", k);
	//printf("try %s\n", log_file_name);
	if ((perf_file = fopen(log_file_name, "r")) == NULL) break;  // get the first non existing file
	fclose(perf_file);
}
	printf("perf file = %s\n", log_file_name);
	perf_file = fopen(log_file_name, "w");
// int i;
	printf("Final report:\n");
	printf("total visited:\t%ld\ntotal time:\t%5.2f\neval/sec\t %4.0f\n", \
		tot_nr_of_nodes_visited, tot_move_duration, tot_nr_of_nodes_visited/(tot_move_duration*1000));
	fprintf(perf_file,"parallel with %d threads\n", ActiveThreads);

	fprintf(perf_file,"total visited:\t%ld\ntotal time:\t%5.2f\neval/sec\t %4.0f\n", \
		tot_nr_of_nodes_visited, tot_move_duration, tot_nr_of_nodes_visited/(tot_move_duration*1000));
	fclose(perf_file);
	//printf("perf file is closed\n");
}



void print_node_info(NodePnt node)
{	if (node->hash_key > HASH_ARRAY_SIZE)
	{	printf("hash_key problem: key = %X\n", node->hash_key);
		//log_file = fopen( log_file_name, "a");
		//fprintf(log_file, "hash_key problem: key = %X\n", node->hash_key);
		//fclose(log_file);
	}
	else
	{	//log_file = fopen( log_file_name, "a");
		printf("lowerbound    %d\n", hash[node->hash_key].lowerbound);
		printf("upperbound    %d\n", hash[node->hash_key].upperbound);
		printf("best move idx %d\n", hash[node->hash_key].best_move_idx);
		printf("status        %X\n", hash[node->hash_key].status);
		//fclose(log_file);
	}
}

void print_bits(BitArray bitarray)
{	int bit_list[21];
	int nr_of_bits;
	int i;
	if (bitarray)
	{
		count_bits_and_fill_list(bitarray, nr_of_bits, bit_list);
		printf("[");
		for (i=0; i< nr_of_bits - 1; i++) printf("%d, ", int_to_ext_list[bit_list[i]]);
		printf("%d]\n", int_to_ext_list[bit_list[nr_of_bits-1]]);

	}
	else printf("[]\n");
}

void set_result(char a, char b)
{	result[0] = 'E';
	result[1] = a;
	result[2] = '-';
	result[3] = b;
	result[4] = '\0';
	result_length = 5;
}

void set_sp_result(char a, char b)
{	result[0] = 'P';
	result[1] = a;
	result[2] = '-';
	result[3] = b;
	result[4] = '\0';
	result_length = 5;
}
/*
void read_weights() {
	int td_out_number;
	fscanf(wweight_file, "%d\n", &td_out_number);
	printf("wweight_file generated from td_out.%03d.txt\n", td_out_number);
	for (int i=0; i < 2*NR_OF_WEIGHTS ; i++) {
		fscanf(wweight_file, "%f\n", &wweight[i]);
		//printf("%6.3f\n", white_weight[i]);
	}
	fscanf(bweight_file, "%d\n", &td_out_number);
	printf("bweight_file generated from td_out.%03d.txt\n", td_out_number);
	for (int i=0; i < 2*NR_OF_WEIGHTS ; i++) {
		fscanf(bweight_file, "%f\n", &bweight[i]);
		//printf("%6.3f\n", white_weight[i]);
	}
}

*/
void init_game(char *ps)
{	printf("Init game: hash\n");
	init_hash();
	printf("%s\n", ps);
	sscanf(ps, "%3d %5d", &total_moves_allowed, &total_time_allowed);
	//total_time_allowed = total_time_allowed*60;
	printf("Total moves allowed = %d, time = %d seconds\n", total_moves_allowed, total_time_allowed);

	white_total_time_used = 0;
	white_nr_of_moves_to_go = total_moves_allowed;
	black_total_time_used = 0;
	black_nr_of_moves_to_go = total_moves_allowed;
//	init_log_file();
	//printf("Init game\n");
//	log_file = fopen( log_file_name, "a");
	tot_move_duration = 0;
	stand_stack_pnt = 0;
}

 short int abs_(short int a)
{	if (a < 0) return (-a);
	else return a;
}

 void get_best_move_from_hash(NodePnt root, short int best_move_value)
 {	struct work_space_move_generator ws;
 	//struct tnode next_node;
 	int best_move_index;
 	generate_legal_moves(root, &ws, root->status & IS_WHITE_ON_MOVE, -1);			// all legal moves collected in ws
 	for (best_move_index = 0; best_move_index < ws.nr_of_legal_moves; best_move_index++)
 	{	unsigned int next_hash_key;
 		struct tnode next_node;
 		populate_node(&ws, &next_node, best_move_index, root);
 		next_hash_key = next_node.hash_key;
 		//printf("up= %d, low= %d, best=%d\n",hash[next_hash_key].upperbound,hash[next_hash_key].lowerbound,best_move_value);
 		if (is_same_board_position(&next_node, &hash[next_hash_key]))
 			if ((best_move_value == hash[next_hash_key].lowerbound) &&
 			(best_move_value == hash[next_hash_key].upperbound))
 			{
				hash[root->hash_key].best_move_idx = (unsigned char) best_move_index;
 				break;
 			}
/* 			else {
 				//printf("upperbound and lowerbound don't match with best value: %d, %d, %d\n",hash[next_hash_key].upperbound,hash[next_hash_key].lowerbound,best_move_value);
 				hash[root->hash_key].best_move_idx = (unsigned char) best_move_index;
 			 	break;
 			}
*/
 	};
 }
 bool verify_best_move_in_hash(NodePnt root, short int best_move_value, unsigned char best_move_index)
 {	struct work_space_move_generator ws;
	unsigned int next_hash_key;
	struct tnode next_node;

	generate_legal_moves(root, &ws, root->status & IS_WHITE_ON_MOVE, -1);			// all legal moves collected in ws
 	populate_node(&ws, &next_node, best_move_index, root);
 	next_hash_key = next_node.hash_key;
 	return  (is_same_board_position(&next_node, &hash[next_hash_key]) &&
 			(best_move_value == hash[next_hash_key].lowerbound) &&
 			(best_move_value == hash[next_hash_key].upperbound) &&
 			(current_search_depth == hash[next_hash_key].search_depth));

 }
bool test_proc_fixed_depth(NodePnt node, WsPnt ws, unsigned char best_move_index, short int best_move_value, int depth, int nr_of_threads){
	int move_value = best_move_value;
	int prev_thread = ActiveThreads;


	ActiveThreads = nr_of_threads;  // test with this number of threads
	init_hash();					// we have to reset the environment
	for (int k = 0 ; k < HASH_ARRAY_SIZE; k++)	{hash[k].occ_empty = 0; }; // preset with empty boards
	move_start_time = gettimeofday(&before, NULL);

	for (int k=0; k<= depth; k +=2) {
		clear_statistic_counters();

		move_value = mtdf_depth_limited(node, move_value, k);

		move_finish_time = gettimeofday(&after, NULL);
		move_duration = (double)after.tv_sec - (double)before.tv_sec + ((double)after.tv_usec - (double)before.tv_usec)/1000000.0;
		if (move_duration <= 0.001) move_duration = 0.001;
		printf("   dur: %6.2f, k=%2d avg=%4.1f max=%2d s=%4d ", move_duration, k, get_avg_distance(), get_max_distance_from_root(), get_nr_of_nodes_visited()/int(move_duration*1000.0));
		print_move(ws, hash[node->hash_key].best_move_idx); printf(" (%d)\n", move_value);
	}


	ActiveThreads = prev_thread;
	Idle = true;

	// if both the index and the value are the same, it is simple
	if ((hash[node->hash_key].best_move_idx == best_move_index) && (move_value == best_move_value))
		return true;
	// if move values are different, we have a problem:
	if ((move_value != best_move_value))
		return false;
	//if the value is the same, but it is a different move, we have to investigate:
	struct tnode next_node;
	populate_node(ws, &next_node, best_move_index, node);
	int next_hash_key = next_node.hash_key;
	 if (is_same_board_position(&next_node, &hash[next_hash_key]) &&
			((best_move_value == hash[next_hash_key].lowerbound) ||
			(best_move_value == hash[next_hash_key].upperbound)) &&
			(current_search_depth == hash[next_hash_key].search_depth))
		 return true;
	 else {
		 printf("now previous move has upperbound: %d and lower %d\n", hash[next_hash_key].upperbound, hash[next_hash_key].lowerbound);
		 return false;
	 }


}




char * get_all_moves_with_value(char *ps)
/***************************************************************
*	This procedure is called to obtain all moves in a given    *
*   position together with the move values of that move        *
***************************************************************/
{	unsigned int i; int k;
	short int move_value;
	struct tnode root; root.previous_node = NULL;
	struct tnode next_node;
	struct work_space_move_generator ws;
	int analyse_depth;
	char move_op;

	result[0] = 'A'; result[1]='x'; result[2] = 0; result_length = 3;
	convert_position(ps, &root, &current_ply_nr);
	ps += 51 + 3;
	current_ply_nr += 1;
	sscanf(ps, "%2d", &analyse_depth);
	ps += 2;


	printf("start analyze, depth = %d\n", analyse_depth);
	skip_early_quit = 0;														// test on early quit conditions
	init_hash();
	printf("hash initialized\n");
	generate_legal_moves(&root, &ws, root.status & IS_WHITE_ON_MOVE, -1);			// all legal moves collected in ws
	if (ws.nr_of_legal_moves == 0)
	{ 	printf("No moves\n");
		result_length = sprintf(result, "Ano legal move\0 ");
		printf("%s\n", result);
		return result;
	}
	else if (ws.nr_of_legal_moves == 1)
	{ 	if (ws.max_nr_of_captures == 0) move_op = '-'; else move_op = 'x';
		printf("Forced move\n");
		result_length = sprintf(result, "A%2d%c%2d (     0)\0 ", int_to_ext(ws.legal_moves[0].start), move_op, int_to_ext(ws.legal_moves[0].stop));
		return result;
	}
	else
	{	result_length = sprintf(result, "A"); // prepare return string
		for (i=0; i < ws.nr_of_legal_moves; i++)
		//i = ws.nr_of_legal_moves - 1;  //test only
		{	//printf("populate move nr %d\n", i);
			populate_node(&ws, &next_node, i, &root);
			copy_to_hash(&next_node, -1);
			calc_piece_count(&next_node, &org_piece_count);
			move_value = 0;
restart:
			int start_value; if (ws.max_nr_of_captures) start_value = 2; else start_value = 1;
			for (k=start_value; k <= analyse_depth; k=k+2)
			{	// collecting
				printf("move %d, depth %d\n", i, k);
				move_value = mtdf_depth_limited(&next_node, move_value, k);
				if (external_stop) break;
				if (move_value == 0) break;
			};
/*			if (move_is_early_quit(&next_node, 0) & IS_USELESS_SACRIFICE)
			{	if ((move_value - (move_value/1000)*1000 == 0) && (skip_early_quit == 0))
				{	skip_early_quit = 1;
					printf("Restart due to early quit\n");
					goto restart; } 		// test on early quit conditions
			}
*/			if (ws.max_nr_of_captures == 0) move_op = '-'; else move_op = 'x';
			printf("%2d%c%2d (%6d)\n", int_to_ext(ws.legal_moves[ws.move_order[i]].start), move_op, int_to_ext(ws.legal_moves[ws.move_order[i]].stop), move_value);
			result_length += sprintf(result + result_length, "%2d%c%2d (%6d)", int_to_ext(ws.legal_moves[ws.move_order[i]].start), move_op, int_to_ext(ws.legal_moves[ws.move_order[i]].stop), move_value);

			HashPnt EndPoint = get_end_node(&next_node, move_value, 0);
			if (EndPoint != NULL){
				BitArray white_man  = EndPoint->occ_empty &   EndPoint->man_king  &   EndPoint->white_black;
				BitArray white_king = EndPoint->occ_empty & ~(EndPoint->man_king) &   EndPoint->white_black;
				BitArray black_man  = EndPoint->occ_empty &   EndPoint->man_king  & ~(EndPoint->white_black);
				BitArray black_king = EndPoint->occ_empty & ~(EndPoint->man_king) & ~(EndPoint->white_black);

				if (EndPoint->status & IS_WHITE_ON_MOVE)      result_length += sprintf(result + result_length, "W");
				else                                          result_length += sprintf(result + result_length, "Z");
				for (int i=1; i<51; i++){
					if      (white_man  & bits[ext_to_int[i]]) result_length += sprintf(result + result_length, "w");
					else if (white_king & bits[ext_to_int[i]]) result_length += sprintf(result + result_length, "W");
					else if (black_man  & bits[ext_to_int[i]]) result_length += sprintf(result + result_length, "z");
					else if (black_king & bits[ext_to_int[i]]) result_length += sprintf(result + result_length, "Z");
					else                                      result_length += sprintf(result + result_length, "e");
				}
				print_move_list_from_hash(&next_node, move_value);
			}
			else { // no valid end_point obtained
				result_length += sprintf(result + result_length, "W");
				for (int i=0; i<50; i++){
                    result_length += sprintf(result + result_length, "e");
				}
			}
			skip_early_quit = 0;  // reset

		}
	}

	printf("get_all_moves: visited = %d, evaluated = %d, avg_depth = %3.2f, max_depth = %d\n", get_nr_of_nodes_visited(), get_nr_of_nodes_evaluated(), get_avg_distance(), get_max_distance_from_root());
	for (int level = 0; level < 200; level++) {
		if (pos_level[level])
		{
			printf("%d\t%d\n", level, pos_level[level]);
		}
	}
	result_length += sprintf(result + result_length,"\0 ");
	//printf("driver: %s\n", result);
	Idle = true;
	return result;

};
/*
char * get_all_moves_with_value(char *ps, int analyse_only)
*/
/***************************************************************
*	This procedure is called to test the various parameters    *
*	If it is called with a valid move, then this move is       *
*	compared against the moves calculated by the engine        *
*	otherwise it just displays the value of the various moves  *
***************************************************************/
/*{	unsigned int i; int k;
	short int best_move_value = -INFINITY;
	short int sec_best_move_value = -INFINITY;
	short int move_value_list[150];
	short int move_value;
	struct tnode root;
	struct tnode next_node;
	struct work_space_move_generator ws;
	int played_move_index = -1;
	int best_move_index = -1;
	int sec_best_move_index = -1;
	int current_max_depth;
	int analyse_depth;

//	init_log_file();
	get_nr_of_nodes_evaluated() = 0;
	result[0] = 'N'; result[1]='x'; result[2] = 0; result_length = 3;
	skip_early_quit = 0;														// test on early quit conditions
	init_hash();
	convert_position(ps, &root);
	ps += 51;
	sscanf(ps, "%2d", &analyse_depth);
	ps += 2;
	if (analyse_only)
	{	printf("Analyse only\n");
		played_move.start = 0;
		played_move.stop = 0;
	} else convert_move(ps, &played_move);
	printf("Analyze with depth = %d\n", analyse_depth);
	init_log_file();
	open_log_file();
	fprintf(log_file, "Analyze with depth = %d\n", analyse_depth);
	fprintf(log_file, "for board position:\n");
	print_board_on_log(&root);
	fclose(log_file);
	calc_piece_count(&root, &org_piece_count);
	generate_legal_moves(&root, &ws, root.status & IS_WHITE_ON_MOVE, -1);			// all legal moves collected in ws
	if (ws.nr_of_legal_moves <= 1) printf("legal moves: %d\n", ws.nr_of_legal_moves);
	if (ws.nr_of_legal_moves > 1)				// no time on lost game and forced move
	{	for (i=0; i < ws.nr_of_legal_moves; i++)
		//i = ws.nr_of_legal_moves - 1;  //test only
		{
			print_move(&ws, i); printf("( ");
			populate_node(&ws, &next_node, i, &root);
			move_value = 0;
			get_nr_of_nodes_visited() = 0;
			get_nr_of_nodes_evaluated() = 0;
			get_nr_of_hits() = 0;
			get_nr_of_overwrites() = 0;
			current_max_depth = analyse_depth;
			int start_value; if (ws.max_nr_of_captures) start_value = 2; else start_value = 1;
			for (k=start_value; k <= current_max_depth; k=k+2)
			{	// collecting
				move_value = mtdf_depth_limited(&next_node, move_value, k);
				if (external_stop) return;
				//if ((move_value == +INFINITY-1 ) || (move_value == -INFINITY+1 )) break;
			};
			printf("%d )\n", move_value);
			get_best_move_from_hash(&next_node, move_value);
			print_move_list_from_hash(&next_node, move_value);
			open_log_file();
			print_move_on_log(&ws, i); fprintf(log_file,"( %d )\n", move_value);
			print_move_list_from_hash_on_log(&next_node, move_value, 0);
			NodePnt end_node = get_end_node(&next_node, move_value, 0, 1);
			print_board_on_log(end_node);
			short int eval_value = evaluate(end_node,1,1);
			print_tree_on_log(&next_node, 4);

			fclose(log_file);


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
		{	if ((move_value_list[played_move_index] == move_value_list[(int)best_move_index]) &&
				(move_value_list[played_move_index] != move_value_list[(int)sec_best_move_index])) move_played_is_first++;
			if ((move_value_list[played_move_index] == move_value_list[(int)best_move_index]) &&
				(move_value_list[played_move_index] == move_value_list[(int)sec_best_move_index])) move_played_is_equal++;
			if ((move_value_list[played_move_index] != move_value_list[(int)best_move_index]) &&
				(move_value_list[played_move_index] == move_value_list[(int)sec_best_move_index])) move_played_is_second++;
			if ((move_value_list[played_move_index] != move_value_list[(int)best_move_index]) &&
				(move_value_list[played_move_index] != move_value_list[(int)sec_best_move_index])) move_played_is_other++;

			//print_board(&root);

			printf("\nmove (p)    : %2d%c%2d value = %d\n", int_to_ext(ws.legal_moves[played_move_index].start), ' ', int_to_ext(ws.legal_moves[played_move_index].stop), move_value_list[played_move_index]);
			printf("    move (1): %2d%c%2d value = %d\n", int_to_ext(ws.legal_moves[best_move_index].start), ' ', int_to_ext(ws.legal_moves[best_move_index].stop), move_value_list[best_move_index]);
			printf("    move (2): %2d%c%2d value = %d\n", int_to_ext(ws.legal_moves[sec_best_move_index].start), ' ', int_to_ext(ws.legal_moves[sec_best_move_index].stop), move_value_list[sec_best_move_index]);
		}
		else // this is for the test mode: analyse board position
		{

			for (i=0; i<ws.nr_of_legal_moves; i++)
			{	printf("move    : %2d%c%2d value = %d\n", int_to_ext(ws.legal_moves[i].start), ' ', int_to_ext(ws.legal_moves[i].stop), move_value_list[i]);
			}

			print_board(&root);
			printf("best move: %2d%c%2d value = %d\n", int_to_ext(ws.legal_moves[best_move_index].start), ' ', int_to_ext(ws.legal_moves[best_move_index].stop), move_value_list[best_move_index]);
		}

	}  // if ws.nr_of_legal_moves > 0

	return result;

};
*/

bool is_in_hash( WsPnt ws, int best_move_index, int best_move_value, NodePnt root) {
	struct tnode next_node;
	unsigned int next_hash_key;
	if (stop_flag) return true;
	populate_node(ws, &next_node, best_move_index, root);
	next_hash_key = next_node.hash_key;
	return (is_same_board_position(&next_node, &hash[next_hash_key]) &&
			(best_move_value == hash[next_hash_key].lowerbound) &&
			(best_move_value == hash[next_hash_key].upperbound));
}
int find_from_hash( WsPnt ws, int best_move_value, NodePnt root) {
	struct tnode next_node;
	unsigned int next_hash_key;
	for (int i = 0; i < ws->nr_of_legal_moves; i++) {
		populate_node(ws, &next_node, i, root);
		next_hash_key = next_node.hash_key;
		if (is_same_board_position(&next_node, &hash[next_hash_key]) &&
				(best_move_value == hash[next_hash_key].lowerbound) &&
				(best_move_value == hash[next_hash_key].upperbound)) return i;
	}
	// if we are here, we really have a problem
	printf("problem, best move not found in hash-table\n");
	for (int i = 0; i < ws->nr_of_legal_moves; i++) {
		populate_node(ws, &next_node, i, root);
		next_hash_key = next_node.hash_key;
		if (is_same_board_position(&next_node, &hash[next_hash_key])) {
			printf("low = %d, high = %d\n", hash[next_hash_key].lowerbound, hash[next_hash_key].upperbound);
		}
	}
	print_board_from_hash_node(&hash[next_hash_key]);
	printf("search_depth = %d, distance = %d, current_ply = %d, current_search_depth = %d\n", hash[next_hash_key].search_depth, hash[next_hash_key].distance_from_root, current_ply_nr, current_search_depth);
	printf("low = %d, high = %d\n", hash[next_hash_key].lowerbound, hash[next_hash_key].upperbound);
	pause();
	return -1;
}

void * wait_max_move_time(void* arg) {
	sleep(absolute_max_move_time);
	lock_grab(&MoveTimeOut);
		stop_flag = 1; // time is really over, stop
		printf("search aborted\n");
		//pthread_exit((void *) &absolute_max_move_time);
	lock_release(&MoveTimeOut);
	return NULL;
}
/*
bool found_early_quit(NodePnt node, int best_move_value) {
	if (best_move_value - (best_move_value/1000)*1000 == 0)
		if (move_is_early_quit(node, 0) & IS_USELESS_SACRIFICE)
		{
			if (skip_early_quit == 0)
			{	skip_early_quit = 1;
				return true;
			}
		}
	return false;
}
*/
struct tnode ponder_root;
struct work_space_move_generator ponder_ws;

bool IsPondering = false;
pthread_t Pondering_id;

void * pondering(void * arg){
	int k = 1;
	short int best_move_value = 0;

	init_hash();
	if (stop_flag) {
		pthread_exit((void *) &Pondering_id);
		return (void *) NULL;
	}
	if (is_same_board_position(&ponder_root, &hash[ponder_root.hash_key])) {
		//printf("ponder_root in hash with search_depth = %d ", hash[ponder_root.hash_key].search_depth - current_ply_nr);
		//printf("upper-bound: %d ", hash[ponder_root.hash_key].upperbound);
		//printf("lower-bound: %d\n", hash[ponder_root.hash_key].lowerbound);
		print_move(&ponder_ws, hash[ponder_root.hash_key].best_move_idx);
		if (hash[ponder_root.hash_key].upperbound == hash[ponder_root.hash_key].lowerbound) {
			printf("last_depth = %d, current_ply_nr = %d\n", hash[ponder_root.hash_key].search_depth, current_ply_nr);
			k = hash[ponder_root.hash_key].search_depth - current_ply_nr + 2;
			if (k <=0) k = 1;
		}
		else {
		k = hash[ponder_root.hash_key].search_depth - current_ply_nr;
			if (k <=0) k = 1;
		}
		if (k % 2 == 0) k -= 1;
		if (hash[ponder_root.hash_key].upperbound != +INFINITY && hash[ponder_root.hash_key].upperbound != -INFINITY) best_move_value = hash[ponder_root.hash_key].upperbound;
		else best_move_value = hash[ponder_root.hash_key].lowerbound;
		printf("start pondering with k=%d, val=%d\n",k,best_move_value);
	}
	else {
		printf("ponder_root not found, start at k=1\n");
//		print_board(&ponder_root);
//		printf("but got:\n");
//		print_board_from_hash_node(&hash[ponder_root.hash_key]);
		copy_to_hash(&ponder_root, -1);
		k = 1;
	}

	while (true) {
		// run forever
		best_move_value = mtdf_depth_limited(&ponder_root, best_move_value, k);
		// debug
		if (best_move_value == INFINITY) print_board(&ponder_root);
		if (stop_flag) break;
		k = k + 2;
		printf("pondering level: %d:", k);
		print_move(&ponder_ws, hash[ponder_root.hash_key].best_move_idx); printf(" (%d)\n", best_move_value);
		if (k>30) break;
	}
	//printf("pondering -- found stop_flag is set\n");
	pthread_exit((void *) &Pondering_id);
	return (void *) NULL;
}

void stop_pondering(){
	lock_grab(&PonderLock);
		if (IsPondering) {
			stop_flag = 1;
			//printf("stop_flag is set\n");
			pthread_join( Pondering_id, NULL);
			//pthread_cancel(Pondering_id);
			//printf("ponder thread is stopped\n");
			Idle = true;
			IsPondering = false;
			stop_flag = 0; // reset stop_flag for next action
		}
	lock_release(&PonderLock);
}

void ponder(char *ps) {
	/**************************************************************
	 * start analyzing the position ad infinitum
	 * alpha-beta will run in a thread, the main
	 * program will return to the socket-wait-loop
	 *
	 * the pondering can be stopped by setting the stopflag to 1
	 * wait till the thread is finished
	 */
	int is_max;
	lock_grab(&PonderLock);
		if (IsPondering) {
			lock_release(&PonderLock);
			return; //don't restart pondering
		}
		IsPondering = true;
		skip_early_quit = 0;
		stop_flag = 0;
		convert_position(ps, &ponder_root, &current_ply_nr);
		ponder_root.previous_node = NULL;
		ps += 54;
		sscanf(ps, "%1d", &is_max);
		if (!(is_max)) ponder_root.status &= ~IS_MAX_NODE;
		generate_legal_moves(&ponder_root, &ponder_ws, ponder_root.status & IS_WHITE_ON_MOVE, -1);
		pthread_create( &Pondering_id, NULL, pondering, NULL );
	lock_release(&PonderLock);
	return;

}

int draw_in_endgame_database = 0;

void get_best_move(char *ps)  // called from play-mode
/******************************************************************
 *	This procedure is called in case the engine has to generate   *
 *	just the best move, no alternatives are evaluated.            *
 *  It increases the search depth untill time is up               *
 ******************************************************************/
{	int i, j;
	unsigned char k;			// current depth
	short int best_move_value;
	struct tnode root; root.previous_node = NULL;
	//struct tnode next_node;
	struct work_space_move_generator ws;
	//int test = 0;
	char best_move_op = '-';
	struct move_type * best_move_pnt;
	int piece_count;
	unsigned long capture_list[21];
	int nr_of_captures;
	int best_move_k6 = -1;
	int best_move_k8 = -1;
	pthread_t panic_time_out;
	int prev_best_move_value;
	int prev_best_move_index;
	int best_move_index;
	int last_index1 = -1;
	int last_index2 = -1;
	int last_index3 = -1;
	int temp_ply_nr;
	int remaining_engine_time_available;

//	fcloseall(); //log_file only open during writing, due to program crashes analyses

	move_start_time = gettimeofday(&before, NULL);

	result[0] = 'E'; result[1]='x'; result[2] = 0; result_length = 3;
	//skip_early_quit = 0;
	convert_position(ps, &root, &temp_ply_nr); ps += 54;
	sscanf(ps, "%5d", &remaining_engine_time_available); //ps += 5;

	//printf("ply number: %d\n",temp_ply_nr);
	//printf("remaining engine time available: %d\n", remaining_engine_time_available);



	//print_board(&root);

	if (root.status & IS_WHITE_ON_MOVE)
	{
		white_nr_of_moves_to_go = total_moves_allowed -(temp_ply_nr/2 + 1);
		//printf("ply_nr = %d, to_go = %2.0f\n",temp_ply_nr, white_nr_of_moves_to_go);
		if (white_nr_of_moves_to_go <= 0) {set_result('0','0'); return;};
		if (remaining_engine_time_available != -1) //update for actual handling time usage
		{	white_total_time_used = total_time_allowed - remaining_engine_time_available;
			//printf("white used: %d\n", white_total_time_used);
		}
	}
	else
	{
		black_nr_of_moves_to_go = total_moves_allowed -(temp_ply_nr/2 + 1);
		if (black_nr_of_moves_to_go <= 0) {set_result('0','0'); return;};
		if (remaining_engine_time_available != -1) //update for actual handling time usage
		{	black_total_time_used = total_time_allowed - remaining_engine_time_available;
			//printf("black used: %d\n", black_total_time_used);
		}
	}


	calc_piece_count(&root, &org_piece_count);
	piece_count = org_piece_count.nr_of_black_king + org_piece_count.nr_of_black_man + org_piece_count.nr_of_white_king + org_piece_count.nr_of_white_man;

#ifdef training        // stop if root-position is in database
	if (piece_count <= NR_OF_ENDGAME_PIECES)
	{	// check endgame database if not capture
		generate_legal_moves(&root, &ws, root.status & IS_WHITE_ON_MOVE, -1);
		if (ws.max_nr_of_captures == 0)
		{
			short int eval_value = evaluate(&root, 0, 0, 0);
			if (eval_value != 0) // found in end_game database
			{	if (root.status & IS_WHITE_ON_MOVE)
				{	if (eval_value > 8000) set_result('2', '0');
					else if (eval_value < -8000) set_result('0', '2');
					else set_result('1', '1');
				}
				else
				{	if (eval_value < -8000) set_result('2', '0');
					else if (eval_value > 8000) set_result('0', '2');
					else set_result('1', '1');
				}
				Idle = true;
				return;
			}
		}
	}
#endif

	generate_legal_moves(&root, &ws, root.status & IS_WHITE_ON_MOVE, -1);
	if (ws.nr_of_legal_moves == 0)
	{	printf("No legal moves anymore ...\n");
#ifdef livedraughts
		if (root.status & IS_WHITE_ON_MOVE) sprintf(result, "N0000%6d00",-INFINITY);
		else sprintf(result, "N0000%6d00",INFINITY);
#else
		if (root.status & IS_WHITE_ON_MOVE) set_result('0', '2'); else set_result('2', '0');
#endif
		Idle = true;
		return;
	}
	if (ws.nr_of_legal_moves == 1)									     // don't spend time on the onvermijdelijke
	{	j =  sprintf(result,     "N%2d", int_to_ext(ws.legal_moves[0].start));
		j += sprintf(result + j, "%2d", int_to_ext(ws.legal_moves[0].stop));
		j += sprintf(result + j, "000000");
		count_bits_and_fill_list(ws.legal_moves[0].captured, nr_of_captures, capture_list);
		j += sprintf(result + j, "%2d", nr_of_captures);
		for (i=0; i < nr_of_captures; i++) j += sprintf(result + j, "%2d", int_to_ext_list[capture_list[i]]);
		j += sprintf(result + j, "\0");
		result_length = j + 1;
#ifndef livedraughts
		previous_evaluation_value = - previous_evaluation_value;
		if (root.status & IS_WHITE_ON_MOVE)
		{	white_total_time_used += 1.0;
			--white_nr_of_moves_to_go;
		}
		else
		{	black_total_time_used += 1.0;
			--black_nr_of_moves_to_go;
		}
#endif
		if (ws.max_nr_of_captures == 0) best_move_op = '-'; else best_move_op = 'x' ;
		if (temp_ply_nr % 2 == 1) printf("%2d. ... ", temp_ply_nr/2 + 1);
		else                      printf("%2d. ", temp_ply_nr/2 + 1);
		printf(" %d%c%d forced move\n", int_to_ext(ws.legal_moves[0].start), best_move_op, int_to_ext(ws.legal_moves[0].stop));
		printf("%s", result);
		Idle = true;
		return;
	};
	//stop_pondering();
	current_ply_nr = temp_ply_nr;
	init_hash();
	// just for test purposes!!!!
	//for (int k = 0 ; k < HASH_ARRAY_SIZE; k++)	{hash[k].occ_empty = 0; }; // preset with empty boards

	best_move_value = 1;  // start the guess with 1
	prev_best_move_value = 1;
	//print_thread_state();
	if (is_same_board_position(&root, &hash[root.hash_key])) {
		//printf("root in hash with search_depth = %d ", hash[root.hash_key].search_depth - current_ply_nr);
		//printf("upper-bound: %d ", hash[root.hash_key].upperbound);
		//printf("lower-bound: %d\n", hash[root.hash_key].lowerbound);
		print_move(&ws, hash[root.hash_key].best_move_idx);
		if (hash[root.hash_key].upperbound == hash[root.hash_key].lowerbound) {
			k = hash[root.hash_key].search_depth - current_ply_nr + 2;
			if (k <=0) k = 2;
			if (k % 2 == 1) k = k - 1;
		}
		else {
			k = hash[root.hash_key].search_depth - current_ply_nr;
			if (k <=0) k = 2;
			if (k % 2 == 1) k = k - 1;
		}
		if (hash[root.hash_key].upperbound == +INFINITY && hash[root.hash_key].lowerbound == -INFINITY) best_move_value = 0;
		if (hash[root.hash_key].upperbound != +INFINITY && hash[root.hash_key].upperbound != -INFINITY) best_move_value = hash[root.hash_key].upperbound;
		else best_move_value = hash[root.hash_key].lowerbound;

		prev_best_move_value = best_move_value;
		best_move_index = hash[root.hash_key].best_move_idx;
		prev_best_move_index = best_move_index;
	}
	else {
		//printf("root not found\n");
		copy_to_hash(&root, -1);
		k = 2;
	}
	if (hash_is_corrupted) { parallel_init_hash_depth(); k=2; hash_is_corrupted = false; printf("reset hash: ");}
	printf(" start searching with k=%d, val=%d\n",k, best_move_value);



	do_next_depth(&root, 0, -1, -1, -1,0);
	//calc_move_duration_budget(&root);
	//k = 2;		// only even nr of search depth used.
	printf("  [%3d] ", absolute_max_move_time);
	while (1)
	{	// collecting
		// restart here in case of early quit result
//restart:
		clear_statistic_counters();
		stop_flag = 0;
	    pthread_create(&panic_time_out, NULL, wait_max_move_time, NULL);
		best_move_value = mtdf_depth_limited(&root, best_move_value, k);
		lock_grab(&MoveTimeOut);
			// time out??
			if (stop_flag) {
				best_move_value = prev_best_move_value;
				best_move_index = prev_best_move_index;
				hash_is_corrupted = true;
			}
			else {
				prev_best_move_value = best_move_value;
				best_move_index = hash[root.hash_key].best_move_idx;
				prev_best_move_index = best_move_index;
				last_index1 = last_index2; last_index2 = last_index3; last_index3 = best_move_index;
				pthread_cancel(panic_time_out);  //
			}
		lock_release(&MoveTimeOut);
		move_finish_time = gettimeofday(&after, NULL);
		move_duration = (double)after.tv_sec - (double)before.tv_sec + ((double)after.tv_usec - (double)before.tv_usec)/1000000.0;
		if (move_duration <= 0.0001) move_duration = 0.0001;
		if (!stop_flag){
//			log_file = fopen( log_file_name, "a");
//			fprintf(log_file, "           duration: %6.2f, budget: %6.2f, k: %d ", move_duration, cur_avg_move_time, k);
//			print_move_on_log(&ws, hash[root.hash_key].best_move_idx);
//			fprintf(log_file, " (%d)\n", best_move_value);
//			fclose(log_file);
			printf("dur: %6.2f, budg: %6.2f, k=%2d avg=%4.1f max=%2d s=%4d ", move_duration, cur_avg_move_time, k, get_avg_distance(), get_max_distance_from_root(), (int)(((double)get_nr_of_nodes_visited())/(move_duration*1000.0)));
			print_move(&ws, hash[root.hash_key].best_move_idx); printf(" (%d)\n", best_move_value);

/*			if ((best_move_value % 1000 == 0) && !(skip_early_quit)) {
				printf("restart due to useless sacrifice\n");
				parallel_init_hash_depth();
				printf("search depth set to -1\n");
				skip_early_quit = 1;
				if (root.status & IS_WHITE_ON_MOVE) white_total_time_used += move_duration; else black_total_time_used += move_duration;
				move_start_time = gettimeofday(&before, NULL);
				do_next_depth(&root, 0, -1, -1, -1,0);
				lock_grab(&MoveTimeOut);
					// cancel panic
					if (!(stop_flag)) pthread_cancel(panic_time_out);
				lock_release(&MoveTimeOut);
				k = 2;
				printf("  [%3d] ", absolute_max_move_time);
				goto restart;
			}
*/

			if (ws.nr_of_legal_moves == 2)
			{	if (k==8) best_move_k6 = hash[root.hash_key].best_move_idx;
				if (k==10) best_move_k8 = hash[root.hash_key].best_move_idx;
				if (k==12) if ((best_move_k6 == hash[root.hash_key].best_move_idx) && (best_move_k8 == hash[root.hash_key].best_move_idx)) break;

			}
			if (not do_next_depth(&root, move_duration, last_index1, last_index2, last_index3, k)) break;
			k = k + 2;
			if (k > 30) break;
			printf("  [%3d] ", absolute_max_move_time);
		}
		else
			break;

	}; // end while

	// hash[root->hash_key].best_move_idx has the correct best move.
/*	 if (!(is_in_hash( &ws, best_move_index, best_move_value, &root)))
		 best_move_index = find_from_hash(&ws, best_move_value, &root);
	 if (best_move_index < 0) {
		 k = 2;
		 goto restart; // best move not found in hash, I don't know but try again??
	 }
*/
	best_move_pnt = & ws.legal_moves[best_move_index];
	if (ws.max_nr_of_captures) best_move_op = 'x'; else best_move_op = '-';

#ifdef remote158
	{	if (root.status & IS_WHITE_ON_MOVE)
		{	if (best_move_value >  8000) { set_result('2', '0'); Idle = true; return; } // we are done
			if (best_move_value < -8000) { set_result('0', '2'); Idle = true; return; }
		}
		else
		{	if (best_move_value < -8000) { set_result('2', '0'); Idle = true; return; } // we are done
			if (best_move_value >  8000) { set_result('0', '2'); Idle = true; return; }
		}
	if ((best_move_value >= 2 && best_move_value <= 19) || (best_move_value <= -2 && best_move_value >= -19)) draw_in_endgame_database++;
	else draw_in_endgame_database = 0;
	if (draw_in_endgame_database >= 3) { set_result('1', '1'); Idle = true; return; }
	}
#endif

#ifndef livedraughts

	if (root.status & IS_WHITE_ON_MOVE)
	{	white_total_time_used += move_duration;
		--white_nr_of_moves_to_go;
		//global_move_nr = (int)(total_moves_allowed - white_nr_of_moves_to_go);
		if (white_nr_of_moves_to_go < 0) {
			Idle = true;
			return;
			}
	}
	else
	{	black_total_time_used += move_duration;
		--black_nr_of_moves_to_go;
		//global_move_nr = (int)(total_moves_allowed - black_nr_of_moves_to_go);
		if (black_nr_of_moves_to_go < 0) {
			Idle = true;
			return;
			}
	}
#endif

	j =  sprintf(result,     "N%2d", int_to_ext(best_move_pnt->start));
	j += sprintf(result + j, "%2d",  int_to_ext(best_move_pnt->stop));
	j += sprintf(result + j, "%6d",  best_move_value);
	count_bits_and_fill_list(best_move_pnt->captured, nr_of_captures, capture_list)
	j += sprintf(result + j, "%2d", nr_of_captures);
	for (i=0; i < nr_of_captures; i++) j += sprintf(result + j, "%2d", int_to_ext_list[capture_list[i]]);
	j += sprintf(result + j, "\0");
	result_length = j + 1;

	if (current_ply_nr % 2 == 1) printf("%2d. ... ", current_ply_nr/2 + 1);
	else                         printf("%2d. ", current_ply_nr/2 + 1);
	printf(" %d%c%d (%d) k=%2d avg=%4.1f max=%2d\n", int_to_ext(best_move_pnt->start), best_move_op, int_to_ext(best_move_pnt->stop), best_move_value, k, float(get_avg_distance_from_root())/float(get_nr_of_nodes_evaluated()), get_max_distance_from_root());
/*	if (root.status & IS_WHITE_ON_MOVE)
		printf("%4.0f sec norm = %4.0f\n",white_total_time_used, ((total_moves_allowed - white_nr_of_moves_to_go)/total_moves_allowed)*total_time_allowed);
	else
		printf("%4.0f sec norm = %4.0f\n",black_total_time_used, ((total_moves_allowed - black_nr_of_moves_to_go)/total_moves_allowed)*total_time_allowed);
*///	fcloseall();
	//print_item_counters();
	Idle = true;
	return;
};
/*
char * analyse_timing(char *ps) // used to test duration expansion
{	int k;




	struct tnode root; root.previous_node = NULL;



	short int move_value;
	struct work_space_move_generator ws;
	int piece_count;
	printf("engine -- enter analyse timing");
	result[0] = 'T'; result[1]='x'; result[2] = 0; result_length = 3;
	init_hash();
	for (long node = 0 ; node < HASH_ARRAY_SIZE; node++)	{hash[node].occ_empty = 0; }; // preset with empty boards
	convert_position(ps, &root, &current_ply_nr);
	calc_piece_count(&root, &org_piece_count);
	piece_count = org_piece_count.nr_of_black_king + org_piece_count.nr_of_black_man + org_piece_count.nr_of_white_king + org_piece_count.nr_of_white_man;
	generate_legal_moves(&root, &ws, root.status & IS_WHITE_ON_MOVE, -1);
	if (ws.nr_of_legal_moves <2 ) { return result; } // no action, forced move or no more moves

	move_value = 0;
	k=2;
	move_start_time = gettimeofday(&before, NULL);
	move_duration = 0;
	skip_early_quit = 0;
	fcloseall();
	for (k=2; k<12; k=k+2)
	//k=2;
	{	printf("enter mtdf with k = %d\n", k);
		move_value = mtdf_depth_limited(&root, move_value, k);

		move_finish_time = gettimeofday(&after, NULL);
		move_duration = (double)after.tv_sec - (double)before.tv_sec + ((double)after.tv_usec - (double)before.tv_usec)/1000000.0;
		tot_move_duration += move_duration;
		tot_nr_of_nodes_visited += get_nr_of_nodes_visited();
		//printf("tot nr visited: %ld\n", tot_nr_of_nodes_visited);

		printf("%d\t%5.3g\t%d\t%d\t%d\t%d\t", k, move_duration,	get_nr_of_nodes_visited(), get_nr_of_nodes_evaluated(), get_nr_of_hits(), piece_count);
		log_file = fopen( log_file_name, "a");
		fprintf(log_file, "%d\t%5.3f\t%d\t%d\t%d\t%d\t", k, move_duration,	get_nr_of_nodes_visited(), get_nr_of_nodes_evaluated(), get_nr_of_hits(), piece_count);
		fclose(log_file);
		//if ((move_value == +INFINITY-1 ) || (move_value == -INFINITY+1 )) break;
		//if ((k == 8) &&  (move_duration > 2.0)) break;


	}
	printf("exit for k = %d\n", k);
	log_file = fopen( log_file_name, "a");
	fprintf(log_file, "\n");
	fcloseall();
	Idle = true;

	return result;
}
*/
int get_move_info(NodePnt node, WsPnt ws, int k, int j, struct hash_node * hash) {
	struct tnode next_node;

	j += sprintf(result + j, "%2d", int_to_ext(ws->legal_moves[k].start));
	j += sprintf(result + j, "%2d", int_to_ext(ws->legal_moves[k].stop));

	populate_node(ws, &next_node, k, node);

	if (is_same_board_position(&next_node, &hash[next_node.hash_key]) &&
			(hash[next_node.hash_key].search_depth == current_search_depth)){
		j += sprintf(result + j, "1");
		j += sprintf(result + j, "%6d", hash[next_node.hash_key].lowerbound);
		j += sprintf(result + j, "%6d", hash[next_node.hash_key].upperbound);
		j += sprintf(result + j, "%7x", next_node.hash_key);

	}
	else {
		j += sprintf(result + j, "1");
		j += sprintf(result + j, "    --");
		j += sprintf(result + j, "    --");
		j += sprintf(result + j, "%7x", next_node.hash_key);
	}
	return j;


}

int create_position_string(NodePnt node, int p) {
	BitArray white_man  = ( node->man_king & node->occ_empty &  node->white_black);
	BitArray black_man  = ( node->man_king & node->occ_empty & ~node->white_black);
	BitArray white_king = (~node->man_king & node->occ_empty &  node->white_black);
	BitArray black_king = (~node->man_king & node->occ_empty & ~node->white_black);



	for (int k=1; k<51; k++){
		int z = ext_to_int[k];
		if (bits[z] & white_man) result[p+k] = 'w';
		else if (bits[z] & white_king) result[p+k] = 'W';
		else if (bits[z] & black_man) result[p+k] = 'z';
		else if (bits[z] & black_king) result[p+k] = 'Z';
		else result[p+k] = 'e';
	}
	return p+51;
}


int get_hash_info(unsigned int hash_key, struct hash_node * hash, int p){
	int k;
	struct work_space_move_generator ws;
	struct tnode node;

	BitArray white_man  = ( hash[hash_key].man_king & hash[hash_key].occ_empty &  hash[hash_key].white_black);
	BitArray black_man  = ( hash[hash_key].man_king & hash[hash_key].occ_empty & ~hash[hash_key].white_black);
	BitArray white_king = (~hash[hash_key].man_king & hash[hash_key].occ_empty &  hash[hash_key].white_black);
	BitArray black_king = (~hash[hash_key].man_king & hash[hash_key].occ_empty & ~hash[hash_key].white_black);



	for (k=1; k<51; k++){
		int z = ext_to_int[k];
		if (bits[z] & white_man) result[p+k] = 'w';
		else if (bits[z] & white_king) result[p+k] = 'W';
		else if (bits[z] & black_man) result[p+k] = 'z';
		else if (bits[z] & black_king) result[p+k] = 'Z';
		else result[p+k] = 'e';
	}
	if (hash[hash_key].status & IS_MAX_NODE)             result[p+51] = '1'; else result[p+51] = '0';
	if (hash[hash_key].status & IS_WHITE_ON_MOVE)        result[p+52] = '1'; else result[p+52] = '0';
	if (hash[hash_key].status & IS_EVALUATED)            result[p+53] = '1'; else result[p+53] = '0';
	if (hash[hash_key].status & IS_USELESS_SACRIFICE)    result[p+54] = '1'; else result[p+54] = '0';
	if (hash[hash_key].status & IS_EXACT_VALUE)          result[p+55] = '1'; else result[p+55] = '0';
	if (hash[hash_key].status & IS_CYCLE_POSSIBLE)       result[p+56] = '1'; else result[p+56] = '0';
	if (hash[hash_key].status & IS_JUST_PROMOTED)        result[p+57] = '1'; else result[p+57] = '0';
	if (hash[hash_key].status & IS_EXTENDED)             result[p+58] = '1'; else result[p+58] = '0';
	if (hash[hash_key].status & HAS_EXTENSION_CONDITION) result[p+59] = '1'; else result[p+59] = '0';

	int j = p + 60;
	j += sprintf(result + j, "%3d", hash[hash_key].search_depth);
	j += sprintf(result + j, "%3d", hash[hash_key].distance_from_root);
	j += sprintf(result + j, "%6d", hash[hash_key].lowerbound);
	j += sprintf(result + j, "%6d", hash[hash_key].upperbound);

	copy_hash_to_node(&hash[hash_key], &node);
	node.hash_key = calc_initial_hash_key(&node);
	generate_legal_moves(&node, &ws, hash[hash_key].status & IS_WHITE_ON_MOVE, -1);

	j += sprintf(result + j, "%2d", ws.nr_of_legal_moves);
	if (ws.nr_of_legal_moves > 40) {
		printf("Problem? nr moves > 40\n");
	}
	else {
		for (k=0; k< ws.nr_of_legal_moves; k++) {
			j = get_move_info(&node, &ws, k, j, hash);
		}
	}

	// some remaining info (defined later, therefor here
	j += sprintf(result + j, "%2d", hash[hash_key].best_move_idx);
	if ((hash[hash_key].status & IS_QUIESCENCE))  j += sprintf(result + j, "1"); else j += sprintf(result + j, "0");
	if ((hash[hash_key].status & HAD_TO_CAPTURE)) j += sprintf(result + j, "1"); else j += sprintf(result + j, "0");
	//j += sprintf(result + j, "%2d", hash[hash_key].move_status);
	j += sprintf(result + j, "%3d", hash[hash_key].depth);

	//printf("depth = %3d\n", hash[hash_key].depth);

	//printf("%d\n", hash[hash_key].move_status);

	//printf("%s", result);
	return j;

}
/***************************************************
 * called during walking the hash in case          *
 * the hash is overwritten or not written          *
 * at all. So we get the last known position       *
 * and the move_nr to make and find out            *
 * if there are child positions that are in hash   *
 ***************************************************/

void find_what_is_in_hash(char *ps) {
	struct tnode node, next_node;
	int distance_from_root, best_move_index, max_node, next_best_move_index, best_move_value;
	struct work_space_move_generator ws;

	convert_position(ps, &node, &current_ply_nr);
	sscanf(ps, "%6d", &best_move_value);
	ps += 6;
	sscanf(ps, "%2d", &best_move_index);
	ps += 2;
	sscanf(ps, "%2d", &distance_from_root);
	ps += 2;
	sscanf(ps, "%1d", &max_node);

	if (max_node) node.status |= IS_MAX_NODE;
	generate_legal_moves(&node, &ws, node.status & IS_WHITE_ON_MOVE, -1);
	populate_node(&ws, &next_node, best_move_index, &node); // So in next_node is the that wasn't in the hash
	generate_legal_moves(&next_node, &ws, next_node.status & IS_WHITE_ON_MOVE, -1);
	next_best_move_index = -1;
	for (int i = 0; i <ws.nr_of_legal_moves; i++) {
		if (is_same_board_position(&next_node, &hash[next_node.hash_key])){
			if (hash[next_node.hash_key].lowerbound == best_move_value &&
				hash[next_node.hash_key].upperbound == best_move_value) {
				next_best_move_index = i; // found the next node in the pv chain
				goto found;
			}

		}
	}
	for (int i = 0; i <ws.nr_of_legal_moves; i++) {
		if (is_same_board_position(&next_node, &hash[next_node.hash_key])){
			if (hash[next_node.hash_key].lowerbound == best_move_value ||
				hash[next_node.hash_key].upperbound == best_move_value) {
				next_best_move_index = i; // found a matchin next node
				goto found;
			}

		}
	}
found:

	return;

}

void find_hash_position(char *ps){
	struct tnode root;
	int j;
	convert_position(ps, &root, &current_ply_nr);
	printf("hash = %x\n", root.hash_key);
	if (is_same_board_position(&root, &hash[root.hash_key])){
		result[0] = 'H';
		j = get_hash_info(root.hash_key, hash, 0);
		printf("j=%d\n", j);
		j = get_hash_info(root.hash_key, hash, j-1);
		printf("j=%d\n", j);
		j += sprintf(result + j, "\0");
		printf("j=%d\n", j);
	}
	else {
		result[0] = 'H'; result[1]='x'; result[2] = 0;
	}
	return;
}
void find_hash_hashkey(char *ps){
	unsigned int hash_key;
	int j;
	sscanf(ps, "%7x", &hash_key);
	//printf("%s , %7x", ps, hash_key);
	result[0] = 'H';
	j = get_hash_info(hash_key, hash, 0);
	j = get_hash_info(hash_key, hash, j-1);
	j += sprintf(result + j, "\0");
	//printf("result = %s\n", result);
	return;
}

void follow_up_position(char *ps) {
/***************************************************************
 * Called to follow up a intermediate position from the hash   *
 * the depth, distance_from_root, status and move_status are   *
 * taken from the hash. From that position the PV from there   *
 * is generated.                                               *
 * We use the value of search_depth as found from the          *
 * previous setup_hash() run                                   *
 * The purpose of the feature is debug-only.                   *
 **************************************************************/
	struct work_space_move_generator ws;
	struct tnode root; root.previous_node = NULL;
	struct tnode next_node; next_node.previous_node = &root;
	int save_current_ply_nr = current_ply_nr;
	int move_nr;
	int initial_guess;

	convert_position(ps, &root, &current_ply_nr);
	move_nr = current_ply_nr; current_ply_nr = save_current_ply_nr;

	copy_hash_to_node(&hash[root.hash_key], &root);
	generate_legal_moves(&root, &ws, root.status & IS_WHITE_ON_MOVE, -1);
	populate_node(&ws, &next_node, move_nr, &root);
	next_node.depth = hash[next_node.hash_key].depth;
	//next_node.move_status = hash[next_node.hash_key].move_status;
	next_node.status = hash[next_node.hash_key].status;

	if (hash[next_node.hash_key].lowerbound == +INFINITY || hash[next_node.hash_key].lowerbound == -INFINITY)
		initial_guess = hash[next_node.hash_key].upperbound;
	else
		initial_guess = hash[next_node.hash_key].lowerbound;

	Idle = false;
	wake_sleeping_threads();


	print_board(&next_node);
	printf("guess=%d, dist=%d, depth=%d\n", initial_guess, hash[next_node.hash_key].distance_from_root, hash[next_node.hash_key].depth);
	mtdf_continue(&next_node, initial_guess, hash[next_node.hash_key].distance_from_root, hash[next_node.hash_key].depth);
	printf("done\n");

	Idle = true;

	calc_piece_count(&root, &org_piece_count);




}


void setup_hash(char *ps) {
/***************************************************************
 * Called to setup the hash for debug investigation            *
 * with the walk through procedure                             *
 * parameters: the 50sq position                               *
 * the search depth
 * the active number of threads                                            *
 *                                                             *
 * output: none to GUI                                         *
 *       : progress on stdout                                  *
 **************************************************************/
	struct work_space_move_generator ws;
	struct tnode root; root.previous_node = NULL;
	int search_depth, nr_of_threads;
	int prev_thread = ActiveThreads;
	int move_value = 0;
	int init_hash_q;
	int k;
	//FILE * hash_dump;

	convert_position(ps, &root, &current_ply_nr);
	calc_piece_count(&root, &org_piece_count);
	//init_game();
	ps += 51 +3;
	sscanf(ps, "%2d", &search_depth);
	printf("|%1c%1c|", ps[0], ps[1]);
	ps += 2;
	sscanf(ps, "%2d", &nr_of_threads);
	printf("|%1c%1c|", ps[0], ps[1]);
	ps += 2;
	sscanf(ps, "%1d", &init_hash_q);
	printf("|%1c%1c|", ps[0], ps[1]);
	ps += 1;
	print_board(&root);
	printf("depth = %d, threads = %d, init hash = %d\n", search_depth, nr_of_threads, init_hash_q);

#ifdef td_learning
	// td_learning
	init_weight_file();
	read_weights();
#endif

	generate_legal_moves(&root, &ws, root.status & IS_WHITE_ON_MOVE, -1); // to be able to print

	skip_early_quit = 0;														// test on early quit conditions
	//ActiveThreads = nr_of_threads;  // test with this number of threads
	//wake_sleeping_threads();		// just to be sure all involved threads are active
	move_start_time = gettimeofday(&before, NULL);
	init_hash();					// we have to reset the environment
	if (is_same_board_position(&root, &hash[root.hash_key])) {
		printf("root in hash with search_depth = %d\n", hash[root.hash_key].search_depth - current_ply_nr);
		printf("upper-bound: %d\n", hash[root.hash_key].upperbound);
		printf("lower-bound: %d\n", hash[root.hash_key].lowerbound);
	}
	else printf("root not found\n");

restart:
	for (k = 0 ; k < HASH_ARRAY_SIZE; k++)	{hash[k].search_depth = -1;}; // reset depth to -1 for each new move
	printf("search depth set to -1\n");
//	if (init_hash_q)
//		for (int k = 0 ; k < HASH_ARRAY_SIZE; k++)	{hash[k].occ_empty = 0; }; // preset with empty boards

	// just to investigate:
	for (k=2; k<= search_depth; k +=2) {
		clear_statistic_counters();

		move_value = mtdf_depth_limited(&root, move_value, k);

		move_finish_time = gettimeofday(&after, NULL);
		move_duration = (double)after.tv_sec - (double)before.tv_sec + ((double)after.tv_usec - (double)before.tv_usec)/1000000.0;
		if (move_duration <= 0.001) move_duration = 0.001;
		printf("   dur: %7.3f, k=%2d avg=%4.1f max=%2d s=%4d ", move_duration, k, get_avg_distance(), get_max_distance_from_root(), get_nr_of_nodes_visited()/int(move_duration*1000.0));
		get_best_move_from_hash(&root, move_value);
		print_move(&ws, hash[root.hash_key].best_move_idx);
		printf(" kN: %4d (%d) ", get_nr_of_nodes_visited()/1000, move_value);
		printf(" kN: %4d\n", get_nr_of_hits()/1000);
		if (move_duration > 6.0) break;
	}
	HashPnt end_node = get_end_node(&root, move_value, 0);


//	init_hash();
//	mtdf_run_once(&root, move_value, 100, k);

#ifdef td_learning
	HashPnt end_node = get_end_node(&root, move_value, 0);
	if (end_node != NULL) {
		print_board_from_hash_node(end_node);
		printf("total = %d\n",evaluate_from_hash(end_node, 1, 0));
	}
#endif
	//display_detailed_counters();
	//ActiveThreads = prev_thread;
	//wake_sleeping_threads();		// just to be sure all involved threads are active
	//AllThreadsShouldExit = true;
	Idle = true;
	return;
}



char * get_best_move_fixed_depth(char *ps)
/***************************************************************
*	This procedure is called to obtain the single best move    *
*	with a fixed search depth.                                 *
*	if returns the value of the best move, and displays        *
*	the main variant and the evaluation of the end position    *
***************************************************************/
{	unsigned int i; int k;
	short int move_value;
	struct tnode root; root.previous_node = NULL;
	struct work_space_move_generator ws;
	int current_max_depth;
	int analyse_depth;
	unsigned long capture_list[21];
	int nr_of_captures = 0;
	char best_move_op = '-';
	struct move_type * best_move_pnt;
	int print_flag;
	int target_move_value;
	int first_move;
	char orig_color;

//	init_log_file();
	result[0] = 'M'; result[1]='x'; result[2] = 0; result_length = 3;
	skip_early_quit = 0;														// test on early quit conditions
	convert_position(ps, &root, &current_ply_nr);
	ps += 51 + 3;
	sscanf(ps, "%2d", &analyse_depth);
	//printf("|%1c%1c|", ps[0], ps[1]);
	ps += 2;
	sscanf(ps, "%1d", &print_flag);
	//printf("|%1c|", ps[0]);
	ps += 1;
	sscanf(ps, "%6d", &target_move_value);
	//printf("|%1c%1c%1c%1c%1c%1c%1c|\n", ps[0], ps[1], ps[2], ps[3], ps[4], ps[5]);
	//printf("0=%d, 1=%d, 2=%d, 3=%d, 4=%d, 5=%d", ps[0], ps[1], ps[2], ps[3], ps[4], ps[5]);
	ps += 6;
	sscanf(ps, "%1d", &first_move);
	//printf("|%1c|", ps[0]);
	ps += 1;
	sscanf(ps, "%1c", &orig_color);
	//printf("|%1c|\n", ps[0]);
	ps += 1;

	printf("depth = %d, print = %d, val = %d, fm = %d, orig = %c\n", analyse_depth, print_flag, target_move_value, first_move, orig_color);

	if (((root.status & IS_WHITE_ON_MOVE) && (orig_color == 'W')) || (!(root.status & IS_WHITE_ON_MOVE) && (orig_color == 'Z')))
		root.status = root.status | IS_MAX_NODE;
	else
		root.status = root.status & (~IS_MAX_NODE);

	calc_piece_count(&root, &org_piece_count);
	current_max_depth = analyse_depth;
	skip_early_quit = 0;

	{	init_hash();
		move_value = target_move_value;
		for (k=1; k <= current_max_depth; k=k+2)
		{	move_value = mtdf_depth_limited(&root, move_value, k);
		}
	}


	//if (print_flag) printf("(%d )\n", move_value);
	get_best_move_from_hash(&root, move_value);
	print_move_list_from_hash(&root, move_value);
	HashPnt end_node = get_end_node(&root, move_value, 0);
	if (end_node != NULL) {
		print_board_from_hash_node(end_node);
		evaluate_from_hash(end_node,print_flag,0);
	}
	generate_legal_moves(&root, &ws, root.status & IS_WHITE_ON_MOVE, -1);

	int best_move_index = hash[root.hash_key].best_move_idx;
	if (best_move_index >= ws.nr_of_legal_moves)
	{	printf("Best move not found in hash!!\n");
		best_move_index = 0;
	};

	best_move_pnt = & ws.legal_moves[best_move_index];
	if (ws.max_nr_of_captures) best_move_op = 'x'; else best_move_op = '-';
	printf("return move: %2d", int_to_ext(best_move_pnt->start));
	printf("%1c", best_move_op);
	printf("%2d",  int_to_ext(best_move_pnt->stop));
	printf("(%6d)\n",  move_value);

	result_length =  sprintf(result,     "M%2d", int_to_ext(best_move_pnt->start));
	result_length += sprintf(result + result_length, "%2d",  int_to_ext(best_move_pnt->stop));
	result_length += sprintf(result + result_length, "%6d",  move_value);
	count_bits_and_fill_list(best_move_pnt->captured, nr_of_captures, capture_list)
	printf("nr of cap: %d \n", nr_of_captures);
	result_length += sprintf(result + result_length, "%2d", nr_of_captures);
	for (i=0; i < nr_of_captures; i++) result_length += sprintf(result + result_length, "%2d", int_to_ext_list[capture_list[i]]);
	result_length += sprintf(result + result_length, "%2d",count_move_list_from_hash(&root, move_value, 0));
	//print_move_list_from_hash_on_result(&root, move_value, 0);
	result_length += sprintf(result + result_length, "\0");
	result_length = result_length + 1;

	Idle = true;

	return result;

};

int do_self_play_move(NodePnt root, NodePnt next_node, int prev_best_move_value) {
	struct work_space_move_generator ws;
	int best_move_value = prev_best_move_value;

	//print_board(root);
	// first the administration:
	// moves to go?
	generate_legal_moves(root, &ws, root->status & IS_WHITE_ON_MOVE, -1);

	if (ws.nr_of_legal_moves == 0) {
		if (root->status & IS_WHITE_ON_MOVE) set_sp_result('0', '2');
		else                                 set_sp_result('2', '0');
		return 0;
	}

	if (((root->total_nr_of_pieces <= NR_OF_ENDGAME_PIECES) && (ws.max_nr_of_captures == 0)) ||
		((root->status & IS_WHITE_ON_MOVE) && (white_nr_of_moves_to_go <= 0) && (ws.max_nr_of_captures == 0)) ||
		(!(root->status & IS_WHITE_ON_MOVE) && (black_nr_of_moves_to_go <= 0 && (ws.max_nr_of_captures == 0))))
		{	short int eval_value = evaluate(root, 0, 0, 0);
			if (eval_value != 0) // found in end_game database
			{	if (root->status & IS_WHITE_ON_MOVE)
				{	if (eval_value > 20000) set_sp_result('2', '0');
					else if (eval_value < -20000) set_sp_result('0', '2');
					else set_sp_result('1', '1');
				}
				else
				{	if (eval_value < -20000) set_sp_result('2', '0');
					else if (eval_value > 20000) set_sp_result('0', '2');
					else set_sp_result('1', '1');
				}
				return 0;
			}
		}
	if (ws.nr_of_legal_moves == 1) {
		// just do the move
		populate_node(&ws, next_node, 0, root);
		next_node->status |= IS_MAX_NODE; //populate toggles max node with min node
		if (root->status & IS_WHITE_ON_MOVE)
			white_nr_of_moves_to_go--;
		else
			black_nr_of_moves_to_go--;
		return 1;
	}
	// no special cases anymore
	move_start_time = gettimeofday(&before, NULL);

	calc_piece_count(root, &org_piece_count);
	init_hash();
	for (int k=2; k<24; k += 2) {
		best_move_value = mtdf_depth_limited(root, best_move_value, k);
		move_finish_time = gettimeofday(&after, NULL);
		move_duration = (double)after.tv_sec - (double)before.tv_sec + ((double)after.tv_usec - (double)before.tv_usec)/1000000.0;
		if (move_duration > 1.0) break;
	}
	// search done, take the best move from hash
	populate_node(&ws, next_node, hash[root->hash_key].best_move_idx, root);
	next_node->status |= IS_MAX_NODE; //populate toggles max node with min node
	print_board(next_node);
	if (root->status & IS_WHITE_ON_MOVE)
		white_nr_of_moves_to_go--;
	else
		black_nr_of_moves_to_go--;
	return best_move_value;

}
/*
void self_play(char *ps) {
/************************************************
 * the goal of this driver is to execute one self-play game as quick as possible
 * no reports on the progress are reported, just the final outcome of the game
 * a filename for the white parameters and a filename for the black parameters is given
 * for the time being we will play with a fixed depth of 10 plies.
 * first read the filenames, open the file and read the parameters
 * then play a game (using the openingbooks)
 * send back the result.
 * filename: nn_mm.weight where nn is the id of the sample and mm is the round
 */
/*	char white_file[2];
	char black_file[2];
	char series[2];
	char weight_file_name[256];
	int j;
	NodePnt root;
	struct tnode node_one;
	struct tnode node_two;
	int move_value = 0;

	sscanf(ps, "%2s %2s %2s", &series, &white_file, &black_file);
	printf("%5s %5s\n", white_file, black_file);
	j =  sprintf(weight_file_name, "/home/feike/BoomstraDam/CEM/series_%2s/%2s.weight", series, white_file);
	if ((wweight_file = fopen(weight_file_name, "r")) == NULL) {
		printf("can't open %s",weight_file_name);
	}
	else {
		for (int i=0; i < NR_OF_WEIGHTS ; i++) {
			fscanf(wweight_file, "%f\n", &wweight[i]);
			//printf("%6.3f\n", white_weight[i]);
		}
		fclose(wweight_file);
	}
	j =  sprintf(weight_file_name, "/home/feike/BoomstraDam/CEM/series_%2s/%2s.weight", series, black_file);
	if ((wweight_file = fopen(weight_file_name, "r")) == NULL) {
		printf("can't open %s",weight_file_name);
	}
	else {
		for (int i=0; i < NR_OF_WEIGHTS ; i++) {
			fscanf(wweight_file, "%f\n", &bweight[i]);
			//printf("%6.3f\n", white_weight[i]);
		}
		fclose(wweight_file);
	}
	// parameters are read, we can start the match
	init_hash();
	root = get_random_begin_postion();
	memcpy(&node_one, root, sizeof(tnode));

	while (true) {
		move_value = do_self_play_move(&node_one, &node_two, move_value);
		if (move_value == 0) { break;}
		move_value = do_self_play_move(&node_two, &node_one, move_value);
		if (move_value == 0) { break;}

	}
	Idle = true;
	return;
}
*/
