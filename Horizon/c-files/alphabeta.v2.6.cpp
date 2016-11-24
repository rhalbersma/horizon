/*
  This file contains thread code from Glaurung, a UCI chess playing engine,
  Copyright (C) 2004-2008 Tord Romstad.

  The mtdf search code is Copyright (C) 2010 Feike Boomstra.

  This file is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This file is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#include "BoomstraDam.v2.5.h"
#include <malloc.h>
#include <time.h>
#include <pthread.h>

//V2.6 major change in stopping "weggevertje"
// this version implements depth reduction

// This is the function as given by Aske van der Plaat:
//function AlphaBetaWithMemory(n : node_type; alpha , beta , d : integer) : integer;
//
//      if retrieve(n) == OK then /* Transposition table lookup */
//            if n.lowerbound >= beta then return n.lowerbound;
//            if n.upperbound <= alpha then return n.upperbound;
//            alpha := max(alpha, n.lowerbound);
//            beta := min(beta, n.upperbound);
//      if d == 0 then g := evaluate(n); /* leaf node */
//      else if n == MAXNODE then
//            g := -INFINITY; a := alpha; /* save original alpha value */
//            c := firstchild(n);
//            while (g < beta) and (c != NOCHILD) do
//                  g := max(g, AlphaBetaWithMemory(c, a, beta, d - 1));
//                  a := max(a, g);
//                  c := nextbrother(c);
//      else /* n is a MINNODE */
//            g := +INFINITY; b := beta; /* save original beta value */
//            c := firstchild(n);
//            while (g > alpha) and (c != NOCHILD) do
//                  g := min(g, AlphaBetaWithMemory(c, alpha, b, d - 1));
//                  b := min(b, g);
//                  c := nextbrother(c);
//      /* Traditional transposition table storing of bounds */
//     /* Fail low result implies an upper bound */
//      if g <= alpha then n.upperbound := g; store n.upperbound;
//      /* Found an accurate minimax value - will not occur if called with zero window */
//      if g >  alpha and g < beta then
//            n.lowerbound := g; n.upperbound := g; store n.lowerbound, n.upperbound;
//      /* Fail high result implies a lower bound */
//      if g >= beta then n.lowerbound := g; store n.lowerbound;
//      return g;

void sp_search(WsPnt, int);

bool start_looking = false;
struct tnode orig_root;
// global variables
extern long pos_level[200];
long pos_level[200];
int current_search_depth = 0;								// global variable to distinguish various runs
int current_ply_nr = -1;
float last_time_used = 0.0;									// to calculate, whether to start a next calculation
int last_best_move_index = 0;								// to use for immediate return if no time for next run is available
int last_best_move_value = 0;								//                             ""
int skip_early_quit;
int stop_flag = 0;
int external_stop;

// various administration for parallel operations
int ActiveThreads = NUM_OF_THREADS;
// MP related variables
int MinimumSplitDepth = 4;
int MaxThreadsPerSplitPoint = 2;
Thread Threads[THREAD_MAX];
pthread_mutex_t MPLock;
pthread_mutex_t MoveTimeOut;
pthread_mutex_t PonderLock;
bool AllThreadsShouldExit = false;
const int MaxActiveSplitPoints = 4;
WsPnt SplitPointStack[THREAD_MAX][MaxActiveSplitPoints];
bool Idle = true;
bool AbortSearch = false;

// locks for the hash table
#define LOCK_ARRAY_SIZE  0x100
#define LOCK_MASK  LOCK_ARRAY_SIZE - 1
pthread_mutex_t hash_lock[LOCK_ARRAY_SIZE];
pthread_mutex_t WaitLock;  // general wait for waiting threads
pthread_cond_t WaitCond;   // wakeup condition
pthread_mutex_t IOLock;    // don't know yet
pthread_mutex_t EndGameLock;    // endgame database not yet re-entrent.

int print_once = 1;

int check_nodes(NodePnt node1, NodePnt node2)
{
	if (node1->hash_key    != node2->hash_key)    return 0;
	if (node1->occ_empty   != node2->occ_empty)   return 0;
	if (node1->man_king    != node2->man_king)    return 0;
	if (node1->white_black != node2->white_black) return 0;
	return 1;
}
int is_cycle(NodePnt node)
{	NodePnt compare_node = node;
	if (!(node->status & IS_CYCLE_POSSIBLE)) return 0;


	while (1)
	{	compare_node = compare_node->previous_node; if (compare_node == NULL) return 0;
		if (!(compare_node->status & IS_CYCLE_POSSIBLE)) return 0; //-1
		compare_node = compare_node->previous_node; if (compare_node == NULL) return 0;
		if (!(compare_node->status & IS_CYCLE_POSSIBLE)) return 0; //-2
		compare_node = compare_node->previous_node; if (compare_node == NULL) return 0;
		if (!(compare_node->status & IS_CYCLE_POSSIBLE)) return 0; //-3
		compare_node = compare_node->previous_node; if (compare_node == NULL) return 0;
		if (!(compare_node->status & IS_CYCLE_POSSIBLE)) return 0; //-4
		if (check_nodes(node, compare_node)) return 1;
	}
}

void write_stand_line(NodePnt node, FILE * f) {
	char pos[54];
	if (node->status & IS_WHITE_ON_MOVE) pos[0] = 'W'; else pos[0] = 'B';
	for (int i = 0; i < 51; i++) {
		     if (ext_to_bits[i] &  node->man_king &  node->occ_empty &  node->white_black) pos[i] = 'w';
		else if (ext_to_bits[i] & ~node->man_king &  node->occ_empty &  node->white_black) pos[i] = 'W';
		else if (ext_to_bits[i] &  node->man_king &  node->occ_empty & ~node->white_black) pos[i] = 'z';
		else if (ext_to_bits[i] & ~node->man_king &  node->occ_empty & ~node->white_black) pos[i] = 'Z';
		else if (ext_to_bits[i] &                   ~node->occ_empty                     ) pos[i] = 'e';
	}
	pos[51] = '\n';
	pos[52] = '\0';
	fprintf(f, "%s", pos);

}

int material_to_depth_reduction(NodePnt);
/*
void print_stack(NodePnt node)
{	NodePnt prev_node;
	FILE * f;
	lock_grab(&IOLock);
	if (print_once)
	{
		f = fopen("/home/feike/Horizon/Stand/Stack_Dump.stand", "w");
		printf("----------------------- node stack\n");
		if (node->status & IS_WHITE_ON_MOVE)
			printf("white on move, %d (%d) %d (%d) ", node->nr_of_white_man, node->nr_of_white_king, node->nr_of_black_man, node->nr_of_black_king);
		else
			printf("black on move, %d (%d) %d (%d) ", node->nr_of_white_man, node->nr_of_white_king, node->nr_of_black_man, node->nr_of_black_king);
		printf(" depth_reduction: %d\n", material_to_depth_reduction( node));
		if (node->mutual_sacrifice == MAX_SACRIFICED) printf("MAX_SACRIFICED\n");
		else if (node->mutual_sacrifice == MIN_SACRIFICED) printf("MIN_SACRIFICED\n");
		else if (node->mutual_sacrifice == BOTH_SACRIFICED) printf("BOTH_SACRIFICED\n");
		//		print_move_status(node->move_status);
		print_board(node);
		write_stand_line(node, f);
		prev_node = node->previous_node;
		while (prev_node)
		{
			if (prev_node->status & IS_WHITE_ON_MOVE)
				printf("white on move, %d (%d) %d (%d) ", prev_node->nr_of_white_man, prev_node->nr_of_white_king, prev_node->nr_of_black_man, prev_node->nr_of_black_king);
			else
				printf("black on move, %d (%d) %d (%d) ", prev_node->nr_of_white_man, prev_node->nr_of_white_king, prev_node->nr_of_black_man, prev_node->nr_of_black_king);
			printf(" depth_reduction: %d\n", material_to_depth_reduction( prev_node));
			if (prev_node->mutual_sacrifice == MAX_SACRIFICED) printf("MAX_SACRIFICED\n");
			else if (prev_node->mutual_sacrifice == MIN_SACRIFICED) printf("MIN_SACRIFICED\n");
			else if (prev_node->mutual_sacrifice == BOTH_SACRIFICED) printf("BOTH_SACRIFICED\n");

//			print_move_status(prev_node->move_status);
			print_board(prev_node);
			write_stand_line(prev_node, f);
			prev_node = prev_node->previous_node;
		}
	}
	print_once = 0;
	lock_release(&IOLock);
	fclose(f);
	return;
}
*/
// wake_sleeping_threads() wakes up all sleeping threads when it is time
// to start a new search from the root.

void wake_sleeping_threads() {
  if(ActiveThreads > 1) {
    for(int i = 1; i < ActiveThreads; i++) {
      Threads[i].idle = true;
      Threads[i].workIsWaiting = false;
    }
    pthread_mutex_lock(&WaitLock);
    pthread_cond_broadcast(&WaitCond);
    pthread_mutex_unlock(&WaitLock);
  }
}



// thread_should_stop() checks whether the thread with a given threadID has
  // been asked to stop, directly or indirectly.  This can happen if a beta
  // cutoff has occured in thre thread's currently active split point, or in
  // some ancestor of the current split point.

  bool thread_should_stop(int threadID) {
    assert(threadID >= 0 && threadID < ActiveThreads);

    WsPnt sp;

    if(Threads[threadID].stop)
      return true;
    if(ActiveThreads <= 2)
      return false;
    for(sp = Threads[threadID].splitPoint; sp != NULL; sp = sp->parent)
      if(sp->finished) {
        Threads[threadID].stop = true;
        //printf("thread should stop\n");
        return true;
      }
    return false;
  }


  // thread_is_available() checks whether the thread with threadID "slave" is
  // available to help the thread with threadID "master" at a split point.  An
  // obvious requirement is that "slave" must be idle.  With more than two
  // threads, this is not by itself sufficient:  If "slave" is the master of
  // some active split point, it is only available as a slave to the other
  // threads which are busy searching the split point at the top of "slave"'s
  // split point stack (the "helpful master concept" in YBWC terminology).

  bool thread_is_available(int slave, int master) {
    assert(slave >= 0 && slave < ActiveThreads);
    assert(master >= 0 && master < ActiveThreads);
    assert(ActiveThreads > 1);

    if(!Threads[slave].idle || slave == master)
      return false;

    if(Threads[slave].activeSplitPoints == 0)
      // No active split points means that the thread is available as a slave
      // for any other thread.
      return true;

    if(ActiveThreads == 2)
      return true;

/*    if (Threads[slave].activeSplitPoints > 0 && Threads[slave].splitPoint == NULL) {
    	assert(Threads[slave].splitPoint != NULL);
    }
    //assert(SplitPointStack[slave][Threads[slave].activeSplitPoints-1] != NULL);
    // debug
    if (SplitPointStack[slave][Threads[slave].activeSplitPoints-1] == NULL){ // should be impossible
    	printf("test thread is available, slave = %d, master = %d",slave,master);
    	for (int i = 0; i < ActiveThreads; i++) {
    		printf("Threads[%d]: activeSplitPoints = %d, stop = %d, running = %d, idle = %d, workIsWaiting = %d",
    				i, Threads[i].activeSplitPoints, Threads[i].stop, Threads[i].running, Threads[i].idle, Threads[i].workIsWaiting);
    		if (Threads[i].splitPoint == NULL){
    			printf("\tsplitPoint == 0");
    		}
    		else
    			if (Threads[i].splitPoint != NULL){
    				printf("\tsplitPoint.activeSplitPoints = %d", Threads[i].splitPoint->ActiveSplitPoints);
					for (int j=0; j<ActiveThreads; j++){
						printf("\tsplitPoint.slave[%d] = %d", j, Threads[i].splitPoint->slaves[j]);
					}
    			}
    			else
    				printf("Threads[%d].splitPoint == NULL");
    	}
    	printf("----------SplitPointStack---------------");
    	for (int i = 0; i < ActiveThreads; i++) {
			for (int j=0; j<MaxActiveSplitPoints; j++){
				printf("SplitPointStack[%d][%d] = %X", i, j, SplitPointStack[i][j]);
			}

    	}
    }
  */  // Apply the "helpful master" concept if possible.
    if(SplitPointStack[slave][Threads[slave].activeSplitPoints-1]->slaves[master])
    	return true;

    return false;
  }


  // idle_thread_exists() tries to find an idle thread which is available as
  // a slave for the thread with threadID "master".

  bool idle_thread_exists(int master) {
    //assert(master >= 0 && master < ActiveThreads);
    //assert(ActiveThreads > 1);

    for(int i = 0; i < ActiveThreads; i++)
      if(thread_is_available(i, master))
        return true;
    return false;
  }
// idle_loop() is where the threads are parked when they have no work to do.
// The parameter "waitSp", if non-NULL, is a pointer to an active SplitPoint
// object for which the current thread is the master.

  void idle_loop(int threadID, WsPnt waitSp) {
  	assert(threadID >= 0 && threadID < THREAD_MAX);

  	Threads[threadID].running = true;
	//printf("enter idle, ID=%d, WorkIsWaiting = %d\n",threadID,Threads[threadID].workIsWaiting);

  	while(true)
  	{
  		if(AllThreadsShouldExit && threadID != 0) break;
  		//if(stop_flag && threadID != 0) break;

  		// If we are not thinking, wait for a condition to be signaled instead
  		// of wasting CPU time polling for work:
  		while(threadID != 0 && (Idle || threadID >= ActiveThreads)) {
  			pthread_mutex_lock(&WaitLock);
  			if(Idle || threadID >= ActiveThreads)
  				pthread_cond_wait(&WaitCond, &WaitLock);
  			pthread_mutex_unlock(&WaitLock);
  		}

  		// If this thread has been assigned work, launch a search:
  		//if (threadID == 0) printf("check waiting, ID=%d, WorkIsWaiting = %d\n",threadID,Threads[threadID].workIsWaiting);
  		if(Threads[threadID].workIsWaiting) {
  			Threads[threadID].workIsWaiting = false;
  			//printf("enter sp_search, ID=%d\n",threadID);
  			sp_search(Threads[threadID].splitPoint, threadID);
  			//printf("exit sp_search, ID=%d\n",threadID);
  			Threads[threadID].idle = true;
  		}

      // If this thread is the master of a split point and all threads have
      // finished their work at this split point, return from the idle loop:
      if(waitSp != NULL && waitSp->cpus <= 0)
        return;
    }

    Threads[threadID].running = false;
  }




// init_thread() is the function which is called when a new thread is
// launched.  It simply calls the idle_loop() function with the supplied
// threadID.

// init_threads and init_hash_locks is called once

void *init_thread(void *threadID) {
  idle_loop(*(int *)threadID, NULL);
  return NULL;
}


// init_split_point_stack() is called during program initialization, and
// initializes all split point objects.

void init_split_point_stack() {
  for(int i = 0; i < THREAD_MAX; i++)
    for(int j = 0; j < MaxActiveSplitPoints; j++) {
      SplitPointStack[i][j] = NULL;
//      lock_init(&(SplitPointStack[i][j].lock), NULL);
    }
}
// init_threads and init_hash_locks is called once
/// init_threads() is called during startup.  It launches all helper threads,
/// the global locks and condition
/// objects.


void init_threads() {
  volatile int i;
  //pthread_t pthread[1];

  for(i = 0; i < THREAD_MAX; i++) {
    // parallel operation
    Threads[i].activeSplitPoints = 0;
    Threads[i].splitPoint = NULL;
    Threads[i].idle = true;
    Threads[i].running = false;
    Threads[i].stop = false;
    Threads[i].workIsWaiting = false;
  }

  // Initialize global locks:
  lock_init(&MPLock, NULL);
  lock_init(&IOLock, NULL);
  lock_init(&EndGameLock, NULL);
  lock_init(&MoveTimeOut, NULL);
  lock_init(&PonderLock, NULL);

  pthread_mutex_init(&WaitLock, NULL);
  pthread_cond_init(&WaitCond, NULL);

  // All threads except the main thread should be initialized to idle state:
  for(i = 1; i < THREAD_MAX; i++) {
    Threads[i].stop = false;
    Threads[i].workIsWaiting = false;
    Threads[i].idle = true;
    Threads[i].running = false;
  }

  // Launch the helper threads:
  for(i = 1; i < THREAD_MAX; i++) {
    //pthread_create(pthread, NULL, init_thread, (void*)(&i));
	  pthread_create(&Threads[i].os_thread_id, NULL, init_thread, (void*)(&i));
    // Wait until the thread has finished launching:
    while(!Threads[i].running);
  }
}


/// stop_threads() is called when the program exits.  It makes all the
/// helper threads exit cleanly.



/// nodes_searched() returns the total number of nodes searched so far in
/// the current search.


void init_hash_locks()
{
	init_threads();
	for (int i=0; i < LOCK_ARRAY_SIZE; i++)
		hash_lock_init(&hash_lock[i], NULL);
	//printf("hash_locks initialised\n");
    return;
}

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
	if (node->status & IS_WHITE_ON_MOVE) printf("white "); else printf("black ");
	if (node->status & IS_MAX_NODE) printf("max "); else printf("min ");
	if (node->status & IS_EVALUATED) printf("eval ");
	if (node->status & IS_USELESS_SACRIFICE) printf("useless "); else printf("        ");
	if (node->status & IS_EXACT_VALUE) printf("exact ");
	if (node->status & IS_CYCLE_POSSIBLE) printf("cycle_p ");
	if (node->status & IS_JUST_PROMOTED) printf("prom ");
	if (node->status & HAD_ONCE_A_WHITE_KING) printf("white-king ");
	if (node->status & HAD_ONCE_A_BLACK_KING) printf("black-king ");
	if (node->status & IS_QUIESCENCE) printf("Quiescence ");
	if (node->status & HAD_TO_CAPTURE) printf("Capture ");
	if (node->status & IS_EXTENDED) printf("ext ");
	if (node->status & HAS_EXTENSION_CONDITION) printf("ext_cond\n");
	printf("depth = %d\n", node->depth);
	for (i=0; i < 5; i++)
	{	printf("    %c   %c   %c   %c   %c\n", lc(node,10*i), lc(node,10*i+1), lc(node,10*i+2), lc(node,10*i+3), lc(node,10*i+4));
		printf("  %c   %c   %c   %c   %c\n", lc(node,10*i+5), lc(node,10*i+6), lc(node,10*i+7), lc(node,10*i+8), lc(node,10*i+9)); }
	printf("\n");
}
void print_board_in_string(struct tnode * node, char * result)
{	int i;
	int j = 0;

	if (node == NULL)
	{	j +=  sprintf(result + j, "Print empty node ??\n");
		return;
	};
	if (node->status & IS_WHITE_ON_MOVE) j +=  sprintf(result + j, "white on move\n"); else j +=  sprintf(result + j, "black on move\n");
	if (node->status & IS_MAX_NODE) j +=  sprintf(result + j, "max node\n\n"); else j +=  sprintf(result + j, "min node\n\n");
	for (i=0; i < 5; i++)
	{	j +=  sprintf(result + j, "   .%c.%c.%c.%c.%c\n", lc(node,10*i), lc(node,10*i+1), lc(node,10*i+2), lc(node,10*i+3), lc(node,10*i+4));
		j +=  sprintf(result + j, "   %c.%c.%c.%c.%c.\n", lc(node,10*i+5), lc(node,10*i+6), lc(node,10*i+7), lc(node,10*i+8), lc(node,10*i+9)); }
	j +=  sprintf(result + j, "\n");
}
int is_same_board_position( NodePnt node1, struct hash_node * node2) /* check if the board position is the same */
{
//	if ((node1 == NULL) || (node2 == NULL))
//	{ 	printf("Null pointer in same board position\n");
//		return 0;
//	};
	if ((node1->occ_empty) != (node2->occ_empty))
	{
		return 0; /* check occ_empty fields */
	};
	if ((node1->white_black) != (node2->white_black))
	{
		return 0; /* check white black fields */
	};
	if (((node1->status) & (IS_MAX_NODE | IS_WHITE_ON_MOVE)) != ((node2->status)& (IS_MAX_NODE | IS_WHITE_ON_MOVE)))
	{
		return 0; /* check color, min/max */
	};
	if ((node1->man_king) != (node2->man_king))
	{
		return 0; /* check man_king fields */
	};
	/* printf("found %d == %d\n", node1 - mem, node2 - mem);*/
	return 1; /* return OK */

}

 void initial_store_common(NodePnt node, short int g, char best_move, unsigned char distance_from_root, unsigned char status_info)
{	hash[node->hash_key].white_black = node->white_black;
	hash[node->hash_key].man_king = node->man_king;
	hash[node->hash_key].occ_empty = node->occ_empty;
	hash[node->hash_key].distance_from_root = distance_from_root;
	hash[node->hash_key].search_depth = current_search_depth;
	//hash[node->hash_key].best_move_idx = best_move;
	//hash[node->hash_key].move_status = node->move_status;
	//hash[node->hash_key].depth = node->depth;
}
 void update_store_both(NodePnt node, short int g, char best_move, unsigned char distance_from_root, unsigned char status_info)
{	hash[node->hash_key].lowerbound = g;
	hash[node->hash_key].upperbound = g;
	hash[node->hash_key].status = (node->status | status_info);
	hash[node->hash_key].best_move_idx = best_move;
	hash[node->hash_key].depth = node->depth;
}
 void update_store_upper(NodePnt node, short int g, char best_move, unsigned char distance_from_root, unsigned char status_info)
{	hash[node->hash_key].upperbound = g;
	hash[node->hash_key].status = (node->status | status_info);
	hash[node->hash_key].best_move_idx = best_move;
	hash[node->hash_key].depth = node->depth;
}
 void update_store_lower(NodePnt node, short int g, char best_move, unsigned char distance_from_root, unsigned char status_info)
{	hash[node->hash_key].lowerbound = g;
	hash[node->hash_key].status = (node->status | status_info);
	hash[node->hash_key].best_move_idx = best_move;
	hash[node->hash_key].depth = node->depth;
}
 void initial_store_both(NodePnt node, short int g, char best_move, unsigned char distance_from_root, unsigned char status_info)
 {	initial_store_common(node, g, best_move, distance_from_root, status_info);
	update_store_both(node, g, best_move, distance_from_root, status_info);
}
 void initial_store_upper(NodePnt node, short int g, char best_move, unsigned char distance_from_root, unsigned char status_info)
{	initial_store_common(node, g, best_move, distance_from_root, status_info);
	hash[node->hash_key].lowerbound = -INFINITY;
	update_store_upper(node, g, best_move, distance_from_root, status_info);
}
 void initial_store_lower(NodePnt node, short int g, char best_move, unsigned char distance_from_root, unsigned char status_info)
{	initial_store_common(node, g, best_move, distance_from_root, status_info);
	hash[node->hash_key].upperbound = +INFINITY;
	update_store_lower(node, g, best_move, distance_from_root, status_info);
}

 // stores both upper- and lowerbound in case:
 // 1) there is no stop condition
 // 2) the hash_entry is empty, if not:
 // 3) the hash_entry was not used in this run with current_search_depth, if not
 // 4) this node is closer to the root
 // otherwise not stored.

 void store_both_bounds(int ThreadID, NodePnt node, short int g, char best_move, unsigned char distance_from_root, unsigned char status_info)
{
	if ( !AbortSearch && !thread_should_stop(ThreadID)) {
		hash_lock_grab(&hash_lock[node->hash_key & LOCK_MASK]);
			if (hash[node->hash_key].occ_empty == 0) {
				initial_store_both(node, g, best_move, distance_from_root, status_info);
				hash_lock_release(&hash_lock[node->hash_key & LOCK_MASK]);
				return; }
			// hash entry is not empty
			if (hash[node->hash_key].search_depth != current_search_depth || hash[node->hash_key].search_depth - current_ply_nr < 0){
					initial_store_both(node, g, best_move, distance_from_root, status_info);
					hash_lock_release(&hash_lock[node->hash_key & LOCK_MASK]);
					return; }
			// search_depth == current_search_depth
			if (hash[node->hash_key].distance_from_root > distance_from_root) {
					initial_store_both(node, g, best_move, distance_from_root, status_info);
					hash_lock_release(&hash_lock[node->hash_key & LOCK_MASK]);
					return; }
			// overwrite in case new node is closer to root
			if ((is_same_board_position(node, &hash[node->hash_key])) && (hash[node->hash_key].distance_from_root >= distance_from_root)) {
					//Threads[ThreadID].nr_of_overwrites++;
					update_store_both(node, g, best_move, distance_from_root, status_info);
					hash_lock_release(&hash_lock[node->hash_key & LOCK_MASK]);
					return; }
/*			// overwrite in case of update of the root
				if ((is_same_board_position(node, &hash[node->hash_key])) && (distance_from_root == current_ply_nr))
				{
					//if (distance_from_root == current_ply_nr) printf("store upper, updated\n");
					//Threads[ThreadID].nr_of_overwrites++;
					update_store_both(node, g, best_move, distance_from_root, status_info);
					hash_lock_release(&hash_lock[node->hash_key & LOCK_MASK]);
					return;
				}
			// level 0 has priority
			if (distance_from_root == current_ply_nr) {
					initial_store_both(node, g, best_move, distance_from_root, status_info);
					hash_lock_release(&hash_lock[node->hash_key & LOCK_MASK]);
					return;
			}
*/		hash_lock_release(&hash_lock[node->hash_key & LOCK_MASK]);
	}
	return;
}

 void store_leaf_node(int ThreadID, NodePnt node, short int g, char best_move, unsigned char distance_from_root, unsigned char status_info)
 {
		store_both_bounds(ThreadID, node, g, best_move, distance_from_root, status_info);
		Threads[ThreadID].avg_distance_from_root += distance_from_root;
		if (Threads[ThreadID].max_distance_from_root < distance_from_root)
			Threads[ThreadID].max_distance_from_root = distance_from_root;
		return;
 }

void store_upperbound(int ThreadID, NodePnt node, short int g, char best_move, unsigned char distance_from_root, unsigned char status_info)
{
	if ( !AbortSearch && !thread_should_stop(ThreadID)) {
		//if (distance_from_root == current_ply_nr)
			//printf("\nstore upperbound with: \nnode_search_depth = %d, current_search_depth = %d,\nnode_distance = %d, current_distacne = %d\n",
			//	hash[node->hash_key].search_depth, current_search_depth, hash[node->hash_key].distance_from_root, distance_from_root);
		hash_lock_grab(&hash_lock[node->hash_key & LOCK_MASK]);
			if (hash[node->hash_key].occ_empty == 0)
			{
				//if (distance_from_root == current_ply_nr) printf("store upper, init (empty record)\n");
				initial_store_upper(node, g, best_move, distance_from_root, status_info);
				hash_lock_release(&hash_lock[node->hash_key & LOCK_MASK]);
				return;
			}
		// hash entry is not empty
			if (hash[node->hash_key].search_depth != current_search_depth || hash[node->hash_key].search_depth - current_ply_nr < 0)
			{
				//if (distance_from_root == current_ply_nr) printf("store upper, init (higher search depth)\n");
				initial_store_upper(node, g, best_move, distance_from_root, status_info);
				hash_lock_release(&hash_lock[node->hash_key & LOCK_MASK]);
				return;
			}
		// search_depth == current_search_depth
			if (hash[node->hash_key].distance_from_root > distance_from_root)
			{
				//if (distance_from_root == current_ply_nr) printf("store upper, init (closer to root)\n");
				initial_store_upper(node, g, best_move, distance_from_root, status_info);
				hash_lock_release(&hash_lock[node->hash_key & LOCK_MASK]);
				return;
			}
		// overwrite in case of update or the new node is closer to root
			if ((is_same_board_position(node, &hash[node->hash_key])) && (hash[node->hash_key].distance_from_root >= distance_from_root))
			{
				//if (distance_from_root == current_ply_nr) printf("store upper, updated\n");
				//Threads[ThreadID].nr_of_overwrites++;
				update_store_upper(node, g, best_move, distance_from_root, status_info);
				hash_lock_release(&hash_lock[node->hash_key & LOCK_MASK]);
				return;
			}
    	hash_lock_release(&hash_lock[node->hash_key & LOCK_MASK]);
	}
    return;
}



void store_lowerbound(int ThreadID, NodePnt node, short int g, char best_move, unsigned char distance_from_root, unsigned char status_info)
{
	if ( !AbortSearch && !thread_should_stop(ThreadID)) {
		//if (distance_from_root == current_ply_nr)
			//printf("\nstore lowerbound with: \nnode_search_depth = %d, current_search_depth = %d, \nnode_distance = %d, current_distacne = %d\n",
				//hash[node->hash_key].search_depth, current_search_depth, hash[node->hash_key].distance_from_root, distance_from_root);
		hash_lock_grab(&hash_lock[node->hash_key & LOCK_MASK]);
		if (hash[node->hash_key].occ_empty == 0) {
				//if (distance_from_root == current_ply_nr) printf("store lower, init (empty record)\n");
				initial_store_lower(node, g, best_move, distance_from_root, status_info);
				hash_lock_release(&hash_lock[node->hash_key & LOCK_MASK]);
				return; }
		// hash entry is not empty
		if (hash[node->hash_key].search_depth != current_search_depth || hash[node->hash_key].search_depth - current_ply_nr < 0){
			//if (distance_from_root == current_ply_nr) printf("store lower, init (search depth)\n");
				initial_store_lower(node, g, best_move, distance_from_root, status_info);
				hash_lock_release(&hash_lock[node->hash_key & LOCK_MASK]);
				return; }
		// search_depth == current_search_depth
		if (hash[node->hash_key].distance_from_root > distance_from_root) {
			//if (distance_from_root == current_ply_nr) printf("store lower, init (distance)\n");
				initial_store_lower(node, g, best_move, distance_from_root, status_info);
				hash_lock_release(&hash_lock[node->hash_key & LOCK_MASK]);
				return; }
		// overwrite in case new node is closer to root
		if ((is_same_board_position(node, &hash[node->hash_key])) && (hash[node->hash_key].distance_from_root >= distance_from_root)) {
				//if (distance_from_root == current_ply_nr) printf("store lower, updated\n");
				//Threads[ThreadID].nr_of_overwrites++;
				update_store_lower(node, g, best_move, distance_from_root, status_info);
				hash_lock_release(&hash_lock[node->hash_key & LOCK_MASK]);
				return; }
		hash_lock_release(&hash_lock[node->hash_key & LOCK_MASK]);
	}
	return;
}

void print_move_info(WsPnt ws, int move_nr, int dist, NodePnt node, int enter, short int z)
{	for (int i=0; i<dist; i++)	printf(" ");
	if (node->status & IS_WHITE_ON_MOVE)
		printf("white on move tries: ");
	else
		printf("black on move tries: ");
	print_move(ws, move_nr);
	if (!enter)printf("(exit %d)",z);
	printf("\n");
}
/*
int check_node(NodePnt node, unsigned int distance_from_root)
{	if ((node->hash_key == 0x30cc14) && (current_search_depth == 10) && (distance_from_root == 3))
		return 1;
	else
		return 0;
}
*/
// split() does the actual work of distributing the work at a node between
// several threads at PV nodes.  If it does not succeed in splitting the
// node (because no idle threads are available, or because we have no unused
// split point objects), the function immediately returns false.  If
// splitting is possible, a SplitPoint object is initialized with all the
// data that must be copied to the helper threads (the current position and
// search stack, alpha, beta, the search depth, etc.), and we tell our
// helper threads that they have been assigned work.  This will cause them
// to instantly leave their idle loops and call sp_search_pv().  When all
// threads have returned from sp_search_pv (or, equivalently, when
// splitPoint->cpus becomes 0), split() returns true.

bool split(const NodePnt node, WsPnt ws, int alpha, int beta, int depth, int distance_from_root, int master, int next_move) {
  assert(master >= 0 && master < ActiveThreads);
  assert(ActiveThreads > 1);
  assert(ws != NULL);
  //printf("enter split\n");
  int i;

  lock_grab(&MPLock);

  // If no other thread is available to help us, or if we have too many
  // active split points, don't split:
  if(!idle_thread_exists(master) ||
     Threads[master].activeSplitPoints >= MaxActiveSplitPoints) {
    lock_release(&MPLock);
    //printf("leaving split with false\n");
    return false;
  }

  //printf("--split: thread=%d, depth = %d\n", master, depth);

  // Assign to the next available split point object from the split point stack
  // the current working set

  SplitPointStack[master][Threads[master].activeSplitPoints] = ws;
  Threads[master].activeSplitPoints++;

  // Initialize the split point object:
  lock_init(&(ws->ws_lock), NULL);
  ws->first_free_move = next_move;
  ws->parent = Threads[master].splitPoint;
  ws->finished = false;
  ws->depth = depth;
  ws->alpha = alpha;
  ws->beta = beta;
  ws->distance_from_root = distance_from_root;
  ws->master_thread = master;
  ws->cpus = 1;
  ws->org_node = node;
  for(i = 0; i < ActiveThreads; i++)
    ws->slaves[i] = 0;
  Threads[master].splitPoint = ws;

  // Make copies of the current position and search stack for each thread:
  for(i = 0; i < ActiveThreads && ws->cpus < MaxThreadsPerSplitPoint; i++)
    if(thread_is_available(i, master)) {
    	Threads[i].splitPoint = ws;
    	ws->slaves[i] = 1;
    	ws->cpus++;
    }

  // Tell the threads that they have work to do.  This will make them leave
  // their idle loop.
  for(i = 0; i < ActiveThreads; i++)
    if(i == master || ws->slaves[i]) {
      Threads[i].workIsWaiting = true;
      Threads[i].idle = false;
      Threads[i].stop = false;
    }

  lock_release(&MPLock);
  //printf("leaving split for idle\n");

  // Everything is set up.  The master thread enters the idle loop, from
  // which it will instantly launch a search, because its workIsWaiting
  // slot is 'true'.  We send the split point as a second parameter to the
  // idle loop, which means that the main thread will return from the idle
  // loop when all threads have finished their work at this split point
  // (i.e. when // splitPoint->cpus == 0).

  idle_loop(master, ws);

  // We have returned from the idle loop, which means that all threads are
  // finished.  Update alpha, beta and bestvalue, and return:
  lock_grab(&MPLock);
	  Threads[master].stop = false;
	  Threads[master].idle = false;
	  Threads[master].activeSplitPoints--;
	  Threads[master].splitPoint = ws->parent;
  lock_release(&MPLock);

  return true;
}


short int Node_in_Hash(NodePnt node, short int alpha, short int beta, int distance_from_root, WsPnt ws, int ThreadID)
{	// to be done: set best_value if node is in hash but does not cause a cutoff

	assert(ws != NULL);
	hash_lock_grab(&hash_lock[node->hash_key & LOCK_MASK]);
	if (is_same_board_position(node, &hash[node->hash_key]) && (hash[node->hash_key].distance_from_root == distance_from_root)) // check if it is claimed by this node
	{
		if (hash[node->hash_key].search_depth == current_search_depth)			// check if it is claimed in this run
		{	Threads[ThreadID].nr_of_hits++;
			if (hash[node->hash_key].lowerbound >=  beta)
			{
				short int low = hash[node->hash_key].lowerbound;
				hash_lock_release(&hash_lock[node->hash_key & LOCK_MASK]);
				//if (show_details) printf("-    return low (%d) from hash\n", low);
				return low;
			};
			if (hash[node->hash_key].upperbound <= alpha)
			{	short int high =  hash[node->hash_key].upperbound;
				hash_lock_release(&hash_lock[node->hash_key & LOCK_MASK]);
				//if (show_details) printf("-    return high (%d) from hash\n", high);
				return high;
			}
		} // if (depth ==  current_search_depth)
		else // equal node from a previous search with lesser depth
		{
/*			if (hash[node->hash_key].status & IS_EXACT_VALUE)
			{	short int low = hash[node->hash_key].lowerbound;
				hash_lock_release(&hash_lock[node->hash_key & LOCK_MASK]);
				return low;
			}
*/			if ((hash[node->hash_key].status & IS_USELESS_SACRIFICE) && !(skip_early_quit))
			{	short int low = hash[node->hash_key].lowerbound;
				hash_lock_release(&hash_lock[node->hash_key & LOCK_MASK]);
				//Threads[ThreadID].useless_from_hash[distance_from_root]++;
				return low;
			}
		}
		 ws->best_move = hash[node->hash_key].best_move_idx;	// save best move
		//Threads[ThreadID].nr_of_best_move_only_hits++;
	}
	else // no hash entry found for this node.
		ws->best_move = 0;
	hash_lock_release(&hash_lock[node->hash_key & LOCK_MASK]);
	return 0;
}




int get_next_move(WsPnt ws)
{	if (ws->first_free_move == -1) return -1;
	int result;
	lock_grab(&ws->ws_lock);
		result = ws->first_free_move;
		if (result >= ws->nr_of_legal_moves){
			ws->first_free_move = -1;
			result = -1;
		}
		else
			ws->first_free_move++;
	lock_release(&ws->ws_lock);
	return result;
}

bool update_set_stop(NodePnt node, WsPnt ws, short int z, int alpha, int beta, int next_move, int threadID)
{	if(thread_should_stop(threadID)) return true; // now handled in store_...

	lock_grab(&ws->ws_lock);
	if (node->status & IS_MAX_NODE)
	{	if (z > ws->best_value)
		{	ws->best_value = z;
			ws->best_move = ws->move_order[next_move];
		}
		if (ws->best_value >= beta) {
	        for(int i = 0; i < ActiveThreads; i++)
	        	if(i != threadID && (i == ws->master_thread || ws->slaves[i]))
	        		Threads[i].stop = true;
	        ws->finished = true;
			lock_release(&ws->ws_lock);
			return true;
		}
	}
	else // min_mode
	{	if (z < ws->best_value)
		{	ws->best_value = z;
			ws->best_move = ws->move_order[next_move];
		}
		if (ws->best_value <= alpha) {
	        for(int i = 0; i < ActiveThreads; i++)
	        	if(i != threadID && (i == ws->master_thread || ws->slaves[i]))
	        		Threads[i].stop = true;
	        ws->finished = true;
			lock_release(&ws->ws_lock);
			return true;
		}
	}
	lock_release(&ws->ws_lock);
	return false;
}

bool update(NodePnt node, WsPnt ws, short int z, int alpha, int beta, int next_move, int threadID)
{	// when running single
//	if(thread_should_stop(threadID)) return true;
//	lock_grab(&ws->ws_lock);
	if (node->status & IS_MAX_NODE)
	{	if (z > ws->best_value)
		{	ws->best_value = z;
			ws->best_move = ws->move_order[next_move];
		}
		if (ws->best_value >= beta) {
//			lock_release(&ws->ws_lock);
			return true;
		}
	}
	else // min_mode
	{	if (z < ws->best_value)
		{	ws->best_value = z;
			ws->best_move = ws->move_order[next_move];
		}
		if (ws->best_value <= alpha) {
//			lock_release(&ws->ws_lock);
			return true;
		}
	}
//	lock_release(&ws->ws_lock);
	return false;
}

int max(int a, int b) {
	if (a > b) return a;
	else return b;
}

// the mean search routine. Is called from mtdf.

bool balance(NodePnt node) {
	return ((node->nr_of_white_man -  node->nr_of_black_man) + 3*(node->nr_of_white_king - node->nr_of_black_king) == 0);
}

short int material_saldo(NodePnt node) {
	if (node->status & IS_MAX_NODE) {
		if (node->status & IS_WHITE_ON_MOVE) {
			return ((node->nr_of_white_man -  node->nr_of_black_man) + 3*(node->nr_of_white_king - node->nr_of_black_king))*1000;
		}
		else { // black on move
			return ((node->nr_of_black_man -  node->nr_of_white_man) + 3*(node->nr_of_black_king - node->nr_of_white_king))*1000;
		}
	}
	else { // min node
		if (node->status & IS_WHITE_ON_MOVE) {
			return ((node->nr_of_black_man -  node->nr_of_white_man) + 3*(node->nr_of_black_king - node->nr_of_white_king))*1000;
		}
		else { // black on move
			return ((node->nr_of_white_man -  node->nr_of_black_man) + 3*(node->nr_of_white_king - node->nr_of_black_king))*1000;
		}

	}
}
/*
int get_new_move_status(NodePnt node, WsPnt ws, unsigned char move_status) {
	if (node->status & IS_MAX_NODE){
		switch (move_status) {
		case MIN_NODE_DID_FREE_MOVE:
			if (ws->capture) 					   return MAX_CAPTURED;
			else                                   return MAX_NODE_DID_FREE_MOVE;
			break;

		case MIN_CAPTURED:
			if (ws->capture)                       return RE_CAPTURED;
			else                                   { node->mutual_sacrifice |= MIN_SACRIFICED;
												     return MAX_NODE_DID_FREE_MOVE; }
			break;


		case RE_CAPTURED:
			if (ws->capture) 					   return RE_CAPTURED;
			else                                   return MAX_NODE_DID_FREE_MOVE;
			break;
		}
	}
	else { // min-node
		switch (move_status) {
		case MAX_NODE_DID_FREE_MOVE:
			if (ws->capture)  					   return MIN_CAPTURED;
			else                                   return MIN_NODE_DID_FREE_MOVE;
			break;

		case MAX_CAPTURED:
			if (ws->capture)                       return RE_CAPTURED;
			else                                   { node->mutual_sacrifice |= MAX_SACRIFICED;
												     return MIN_NODE_DID_FREE_MOVE; }
			break;

		case RE_CAPTURED:
			if (ws->capture) 					   return RE_CAPTURED;
			else                                   return MIN_NODE_DID_FREE_MOVE;
			break;
		}
	}
}
*/
int material_to_depth_reduction(NodePnt node) {
	// used for the depth reduction.
	// the color on move must have the highest possible score, so depth_reduction should be big if score is low
	int result;
//	if (node->status & IS_MAX_NODE) {
		if (node->status & IS_WHITE_ON_MOVE) {
			result = (((node->nr_of_black_man -  node->nr_of_white_man) + (node->nr_of_black_king - node->nr_of_white_king)));
		}
		else { // black on move
			result = (((node->nr_of_white_man -  node->nr_of_black_man) + (node->nr_of_white_king - node->nr_of_black_king)));
		}
/*	}
	else { // min node
		if (node->status & IS_WHITE_ON_MOVE) {
			result = ((node->nr_of_white_man -  node->nr_of_black_man) + 3*(node->nr_of_white_king - node->nr_of_black_king));
		}
		else { // black on move
			result = ((node->nr_of_black_man -  node->nr_of_white_man) + 3*(node->nr_of_black_king - node->nr_of_white_king));
		}

	}
*/
	if (result < 0) result = -result;
	return result;
}


short int material_balance_or_one(NodePnt node){
	short int temp = material_saldo(node);
	if (temp == 0) temp = 1;
	return temp;
}

bool prev_material_balance(NodePnt node) {
	// in the previous move a man is just promoted
	// we only want to extend the search if the king is not too expensive

		if (node->status & IS_WHITE_ON_MOVE)
			return (node->nr_of_white_man-org_piece_count.nr_of_white_man) + 3*(node->nr_of_white_king-org_piece_count.nr_of_white_king) <=
			(node->nr_of_black_man-org_piece_count.nr_of_black_man) + 3*(node->nr_of_black_king-org_piece_count.nr_of_black_king);
		else  // black on move
			return (node->nr_of_white_man-org_piece_count.nr_of_white_man) + 3*(node->nr_of_white_king-org_piece_count.nr_of_white_king) >=
			(node->nr_of_black_man-org_piece_count.nr_of_black_man) + 3*(node->nr_of_black_king-org_piece_count.nr_of_black_king);
}

short int material_saldo_or_one(NodePnt node) {
	int temp = material_saldo(node);
	if (temp) return temp; else return 1;
}

short int search(NodePnt node, short int alpha, short int beta, short int distance_from_root, short int depth, int ThreadID, bool pv)
{	short int g;
	short int z;
	struct tnode next_node;
	struct work_space_move_generator ws;
	int next_move;
	int max_node = (node->status & IS_MAX_NODE);
	//int promotion_extension;
	int first_free_move;

	if (stop_flag) {
		//printf("search -- found stop_flag set on entry, ThreadID = %d, distance = %d\n", ThreadID, distance_from_root);
		return 0;
	}
/*	if (thread_should_stop(ThreadID))
	{
		//printf("--------------------------------------------stopped\n");
		return 0;
	}
*/
	//printf("enter at level 0: alpha = %d, beta = %d\n", alpha, beta);
	Threads[ThreadID].nr_of_nodes_visited++;
	//pos_level[distance_from_root]++;


//	if (distance_from_root-current_ply_nr > 30) { print_stack(node); assert(distance_from_root > 30);}
/*	if ((depth <=0) && (distance_from_root - current_ply_nr > 3*(current_search_depth - current_ply_nr)) && balance(node)) {
		//printf("too deep ? (1)\n");
		return material_saldo_or_one(node); // quit if too deep
	}
	if ((distance_from_root - current_ply_nr > 4*(current_search_depth - current_ply_nr)) && balance(node)) {
		//printf("too deep ? (2)\n");
		return material_saldo_or_one(node); // quit if too deep
	}
*/

	// check whether node in hash, update best move according found or not
	if ((g = Node_in_Hash(node, alpha, beta, distance_from_root, &ws, ThreadID))) {
		//printf("\ncut off at level 0: alpha = %d, beta = %d, best_value = %d ", alpha, beta, g);
		return g; // cutoff found in hash
	}
	// check if node is part of a cycle
	if (is_cycle(node))
	{	store_leaf_node(ThreadID, node, 1, 0, distance_from_root, IS_EVALUATED);
		//printf("cycle found, dept = %d\n", distance_from_root);
		return 1;  // return a draw
	}

	generate_legal_moves(node, &ws, node->status & IS_WHITE_ON_MOVE, ws.best_move);			// all legal moves collected in ws
	if (ws.nr_of_legal_moves == 0)				     // we are done in this branch
	{
		if (max_node)
			g = -INFINITY + 1;						// that's a pity, no more moves
		else
			g = +INFINITY -1;						// that's fine, opponent has no more moves
		store_leaf_node(ThreadID, node, g, 0, distance_from_root, IS_EVALUATED);
		return g;
	}

/*	if ((depth <= 0) && (distance_from_root - current_ply_nr > (5*(current_search_depth - current_ply_nr))/2) && !ws.capture && material_saldo_or_one(node) != 1) {
		//printf("too deep ? (3) dist = %d, search_depth = %d, material = %d\n", distance_from_root - current_ply_nr, current_search_depth - current_ply_nr, material_saldo_or_one(node));
		return material_saldo_or_one(node); // quit if too deep
	}
*/

	set_capture(&ws, node);
	if (depth <= material_to_depth_reduction(node)) {
		set_quiscence(&ws, node);
		//printf("depth = %d, depth_reduction = %d, quiescence = %d\n",depth, material_to_depth_reduction(node), ws.quiescence);
		if (ws.quiescence) {
			g = evaluate(node, 0,0, ThreadID);
			store_leaf_node(ThreadID, node, g, 0, distance_from_root, IS_EVALUATED);
			return g;
		}
	}

//	cur_move_status = get_new_move_status(node, &ws, move_status);
//	next_node.move_status = cur_move_status;

/*	if (node->mutual_sacrifice == BOTH_SACRIFICED) {
		g = evaluate(node, 0,0, ThreadID);
		store_leaf_node(ThreadID, node, g, 0, distance_from_root, IS_EVALUATED);
		return g;
	}
*/
	if (max_node) ws.best_value = -INFINITY; else ws.best_value = +INFINITY;

	// the main loop over all possible moves in this position (position in node)
	for (next_move = 0; next_move < ws.nr_of_legal_moves && !thread_should_stop(ThreadID); next_move++)
	{
		//promotion_extension = 0;
		// best move (if any) is first
		populate_node(&ws, &next_node, ws.move_order[next_move], node);
/*		if ((next_node.status & IS_JUST_PROMOTED) && (prev_material_balance(&next_node))){
			if (depth < PROMOTION_EXTENSION) {
				promotion_extension = PROMOTION_EXTENSION;
				//printf("prom ext: depth = %d, depth_reduction = %d, quiescence = %d\n",depth, material_to_depth_reduction(node),ws.quiescence);
			}
		}
*/		int next_depth;
		//if ((ws.capture) && (node->previous_node->status & HAD_TO_CAPTURE)) next_depth = depth; else next_depth = depth - 1;
		if (ws.nr_of_legal_moves == 1) next_depth = depth; else next_depth = depth - 1;
		//next_node.depth = max(next_depth, promotion_extension);
		next_node.depth = next_depth;
		if ((next_move == 0) && (pv)){ //first move of principal variation
			z = search(&next_node, alpha, beta, distance_from_root + 1, next_depth, ThreadID, true);
			//if (stop_flag){	printf("search -- found stop_flag set on exit, ThreadID = %d, distance = %d\n", ThreadID, distance_from_root); return 0; }
			if (z != 0) {if (update(node, &ws, z, alpha, beta, next_move, ThreadID)) break;}
		}
		else {
			// Split? if Split then all remaining moves are done in parallel
			if(ActiveThreads > 1 && depth >= MinimumSplitDepth
				 && ws.nr_of_legal_moves  - next_move > 1
				 //&& idle_thread_exists(ThreadID)
				 && !AbortSearch && !thread_should_stop(ThreadID)
				 && split(node, &ws, alpha, beta, depth, distance_from_root, ThreadID, next_move))
				break;
			// no split possible, just continue
			z = search(&next_node, alpha, beta, distance_from_root + 1, next_depth, ThreadID, false);
			//if (stop_flag){	printf("search -- found stop_flag set on exit, ThreadID = %d, distance = %d\n", ThreadID, distance_from_root); return 0; }
			if (z != 0) if (update(node, &ws, z, alpha, beta, next_move,ThreadID)) {
		//		for(int nn=79; nn<distance_from_root;nn++)printf("-"); printf("cut off with "); print_move(&ws, next_move); printf(" (%d)\n", ws.best_value);

				break;
			}
			//else
				//for(int nn=79; nn<distance_from_root;nn++)printf("-"); printf("             "); print_move(&ws, next_move); printf(" (%d)\n", ws.best_value);

		}
	}

	if (ws.best_value >= beta)  store_lowerbound(ThreadID, node, ws.best_value, ws.best_move, distance_from_root, 0);
	if (ws.best_value <= alpha)	store_upperbound(ThreadID, node, ws.best_value, ws.best_move, distance_from_root, 0);		// fail low implies an upper bound
//	if (distance_from_root == current_ply_nr) {
//		printf("\nlevel 0: alpha = %d, beta = %d, best_value = %d ", alpha, beta, ws.best_value);
//		if (ws.best_value >= beta) printf("stored lower bound\n");
//		if (ws.best_value <= alpha) printf("stored upper bound\n");
//	}
	//printf("\nlevel 0: alpha = %d, beta = %d, best_value = %d ", alpha, beta, ws.best_value);
	return ws.best_value;
}

void sp_search(WsPnt ws, int ThreadID)
{	assert(ws != NULL);

/*
   sp_search():
   while (get_next_move !=-1) {
	populate
	search(pv = false)
	update
}
 */
	int next_move;
	tnode next_node;
	//int promotion_extension;
	int z;


	while (((next_move = get_next_move(ws)) != -1) && !thread_should_stop(ThreadID))// && !stop_flag)
	{
		//printf("entered sp_search, ID=%d, next_move=%d\n",ThreadID, next_move);
		populate_node(ws, &next_node, ws->move_order[next_move], ws->org_node);
/*		if ((next_node.status & IS_JUST_PROMOTED) && (ws->depth < PROMOTION_EXTENSION) && (prev_material_balance(&next_node)))
			promotion_extension = PROMOTION_EXTENSION;
		else promotion_extension = 0;
*/
		//next_node.move_status = ws->cur_move_status; //debug only
//		next_node.depth = max(ws->depth - 1, promotion_extension);
		next_node.depth = ws->depth - 1;
		z = search(&next_node, ws->alpha, ws->beta, ws->distance_from_root + 1, ws->depth - 1, ThreadID, false);
		if (z != 0) {if (update_set_stop(ws->org_node, ws, z, ws->alpha, ws->beta, next_move, ThreadID)) break;}
	}
	// we are done in the splitpoint


	lock_grab(&(ws->ws_lock));

		// If this is the master thread and we have been asked to stop because of
		// a cutoff higher up in the tree, stop all slave threads:
		if(ws->master_thread == ThreadID && thread_should_stop(ThreadID))
			for(int i = 0; i < ActiveThreads; i++)
				if(ws->slaves[i])
					Threads[i].stop = true;
		// anyway, we are stopped
		ws->cpus--;
		ws->slaves[ThreadID] = 0;

	lock_release(&(ws->ws_lock));
	return;
}

short int mtdf_depth_limited(NodePnt root, int initial_guess, char depth)		// algorithm as invented by Aske Plaat
{	short int g ;
	short int beta;
	short int upperbound = +INFINITY;
	short int lowerbound = -INFINITY;

	g = initial_guess;
	current_search_depth = current_ply_nr + depth;					// set global variable
	while (1)
	{
		if (g == lowerbound) beta = g + 1; else beta = g;
		//if (depth==16)printf("Mtdf calls with beta = %d, l = %d, u = %d, depth = %d\n", beta, lowerbound, upperbound, depth);
		g = search(root, beta-1, beta, current_ply_nr, depth, 0, true);
		if (stop_flag) {
//			printf("mtdf -- found stop_flag set\n");
			return 0;
		}
		else {
			if (g==0) printf("g = 0!!, no stop_flag\n");
		}
		if (g < beta) upperbound = g;
		else lowerbound = g;
		//if (depth==16)printf("returns with     val = %d, l = %d, u = %d\n", g, lowerbound, upperbound);
		if (lowerbound >= upperbound){return g;};
	}
}


short int mtdf_continue(NodePnt root, int initial_guess, int distance_from_root, char depth)
{	short int g ;
	short int beta;
	short int upperbound = +INFINITY;
	short int lowerbound = -INFINITY;

	g = initial_guess;
	current_search_depth = current_ply_nr + depth;					// set global variable
	while (1)
	{
		if (g == lowerbound) beta = g + 1; else beta = g;
		//printf("Mtdf calls with beta = %d, l = %d, u = %d, depth = %d\n", beta, lowerbound, upperbound, depth);
		g = search(root, beta-1, beta, distance_from_root, depth, 0, true);
		if (stop_flag) {
			printf("mtdf -- found stop_flag set\n");
			return 0;
		}
		else {
			if (g==0) printf("g = 0!!, no stop_flag\n");
		}
		//printf("val=%d, depth=%d\n", g, depth);
		if (g < beta) upperbound = g;
		else lowerbound = g;
		if (lowerbound >= upperbound){return g;};
	}
}


/*
 * once the best move value is obtained, we like to know what other moves scored well
 * so now we want to see which moves have scores in the window

*/
short int mtdf_run_once(NodePnt root, int best_value, int window, char depth){

	current_search_depth = depth;					// set global variable
	//printf("Mtdf calls with beta = %d, l = %d, u = %d, depth = %d\n", beta, lowerbound, upperbound, depth);
	search(root, best_value - window, best_value, 0, depth, 0, true);
	return best_value;
}
