/*
  This file is Copyright (C) 2010 Feike Boomstra.
  Distributed under the Boost Software License, Version 1.0.
  (See accompanying file LICENSE_1_0.txt or copy at
  http://www.boost.org/LICENSE_1_0.txt)
*/


#include "BoomstraDam.v2.5.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// variables for the module

//#define WhiteBookSize 39072
//#define BlackBookSize 38084
#define WhiteBookSize 453
#define BlackBookSize 487

struct opening_book_position_type WhiteBook[WhiteBookSize];
struct opening_book_position_type BlackBook[BlackBookSize];
struct game_list GameList[32];
unsigned char nr_of_moves_in_game_list = 0;


void Initialize_WhiteBook()  // the compiler can't handle ca. 15000 static assignments, so we read them from file
{
	FILE *f;
	int entry_nr;
	int entry_count;
	int start;
	int stop;
	int next;
	int i;

	// preset
	for (i=0; i<WhiteBookSize; i++) WhiteBook[i].nr_of_moves = 0;

	srand( (unsigned)time( NULL ) );  // preset the random generator with (random) time
	f = fopen("/home/feike/Horizon/Opening Books/NewWhiteOpeningBook.txt", "r");
	if (f == NULL) { printf("Problem with opening whitebook\n"); return; };
	while (1)
	{	if (EOF == fscanf(f,"%d,%d\n", &entry_nr, &entry_count)) break;
		//printf("entry: %d, %d\n", entry_nr, entry_count);
		WhiteBook[entry_nr].nr_of_moves = entry_count;
		for (i=0; i<entry_count; i++)
		{	fscanf(f, "%d,%d,%d\n", &start, &stop, &next);
			WhiteBook[entry_nr].Moves[i].Start = (unsigned char) start;
			WhiteBook[entry_nr].Moves[i].Stop = (unsigned char) stop;
			WhiteBook[entry_nr].Moves[i].Next = (int) next;
			//printf("%d, %d, %d,%d,%d\n", entry_nr, entry_count, start, stop, next);
			//printf("%d, %d, %d,%d,%d\n", entry_nr, entry_count, WhiteBook[entry_nr].Moves[i].Start, WhiteBook[entry_nr].Moves[i].Stop, WhiteBook[entry_nr].Moves[i].Next);
		}
	}

	//printf(" Opening WhiteBook Read\n");
/*	for (int j=0; j<WhiteBookSize; j++)
	{
		printf("%d,%d\n", j, WhiteBook[j].nr_of_moves);
		for (i=0; i<WhiteBook[j].nr_of_moves; i++)
		{	printf("%d,%d,%d\n",
			WhiteBook[j].Moves[i].Start,
			WhiteBook[j].Moves[i].Stop,
			WhiteBook[j].Moves[i].Next);
			//printf("%d, %d, %d,%d,%d\n", entry_nr, entry_count, start, stop, next);
		}
	}
*/	fclose(f);

}
void Initialize_BlackBook()
{
	FILE *f;
	int entry_nr;
	int entry_count;
	int start;
	int stop;
	int next;
	int i; int j;

	// preset
	for (i=0; i<BlackBookSize; i++) BlackBook[i].nr_of_moves = 0;

	srand( (unsigned)time( NULL ) );  // preset the random generator with (random) time

	f = fopen("/home/feike/Horizon/Opening Books/NewBlackOpeningBook.txt", "r");
	if (f == NULL) { printf("Problem with opening black book\n"); return; };
	for (j=0; j<BlackBookSize; j++)
	{	if (EOF == fscanf(f,"%d,%d\n", &entry_nr, &entry_count)) break;
		BlackBook[entry_nr].nr_of_moves = entry_count;
		for (i=0; i<entry_count; i++)
		{	fscanf(f, "%d,%d,%d\n", &start, &stop, &next);
			BlackBook[entry_nr].Moves[i].Start = (unsigned char) start;
			BlackBook[entry_nr].Moves[i].Stop = (unsigned char) stop;
			BlackBook[entry_nr].Moves[i].Next = (short int) next;
			//printf("%d, %d, %d,%d,%d\n", entry_nr, entry_count, start, stop, next);
		}
	}
//	printf(" Opening BlackBook Read\n");
/*	for (int j=0; j<BlackBookSize; j++)
	{
		printf("%d,%d\n", j, BlackBook[j].nr_of_moves);
		for (i=0; i<BlackBook[j].nr_of_moves; i++)
		{	printf("%d,%d,%d\n",
			BlackBook[j].Moves[i].Start,
			BlackBook[j].Moves[i].Stop,
			BlackBook[j].Moves[i].Next);
			//printf("%d, %d, %d,%d,%d\n", entry_nr, entry_count, start, stop, next);
		}
	}
*/	fclose(f);
}

char build_game_list( const char * S)
{	int i;
	char white = 1;

	for (i=0; i<32; i++)
	{	GameList[i].Start = -1;
		GameList[i].Stop  = -1;
	}

	for (i=0; i<nr_of_moves_in_game_list; i++)
	{	GameList[i].Start = S[2*i]  - '(' ;  // in previous version -1 ???
		GameList[i].Stop  = S[2*i+1] - '(';  // here too
		white = 1 - white;
	}
	return white;
}
short int search_move(struct opening_book_position_type *Book, unsigned char start, unsigned char stop, short int current_position)
{	int i;
	short int found;
	found = -1;												// preset
	if (current_position < 0) return -1;					// previous search allready not found
	for (i=0; i<Book[current_position].nr_of_moves; i++)
	{	if ((Book[current_position].Moves[i].Start == start) && (Book[current_position].Moves[i].Stop == stop))
			found = Book[current_position].Moves[i].Next;
	}
	return found;
}
/*
void get_move_from_opening_book(char * S)
{	int i;
	short int current_position;
	char white = 1;
	int move_position;
	int j=0;
	int nr_of_char;
	int nr_of_moves;
	double random_value;

	nr_of_char = 0;
	while (1)
	{	if (S[nr_of_char] == 0) break;
		nr_of_char++;
	}
	nr_of_moves_in_game_list = nr_of_char / 2;

	white = build_game_list(S);

	//printf("GameList: %s\n", S);


	//for (i=0; i<nr_of_moves_in_game_list; i++) printf("%d, %d\n", GameList[i].Start, GameList[i].Stop);
	//printf("\n");

	current_position = 0;

	for (i=0; i<nr_of_moves_in_game_list; i++)
	{	if (white) current_position = search_move(WhiteBook, GameList[i].Start, GameList[i].Stop, current_position);
	    else current_position = search_move(BlackBook, GameList[i].Start, GameList[i].Stop, current_position);
	}

	//printf("Get move from opening book: white = %d, current pos = %d\n", white, current_position);

	// preset no move
	result[0] = 'N'; result[1]='x'; result[2] = 0; result_length = 3;

	// can we provide a move ??

	if (current_position < 0) { printf("Other party move not in book\n"); return ; }  // this move-list not found in the book

	if (white)
	{	if (WhiteBook[current_position].nr_of_moves == 0) { printf("No more moves in book\n");  return; }  // no further moves in the book
	}
	else
	{	if (BlackBook[current_position].nr_of_moves == 0) { printf("No more moves in book\n"); return; }  // no further moves in the book
	}


	random_value = (double) rand() / (((double) RAND_MAX) + 1.0);

	if (white)
	{	move_position = (int)(( random_value)* (double) WhiteBook[current_position].nr_of_moves) ;
		j =  sprintf(result,        "N%2d", WhiteBook[current_position].Moves[move_position].Start);
		j += sprintf(result + j, "%2d00000000\0", WhiteBook[current_position].Moves[move_position].Stop);
		result_length = j + 1;
		//// debug
		nr_of_moves = WhiteBook[current_position].nr_of_moves;
		//printf("Get move from white opening book: move_position = %d\n", move_position);
		//for (i=0; i<WhiteBook[current_position].nr_of_moves; i++)
		//	printf("%d, %d, %d\n", WhiteBook[current_position].Moves[i].Start+1, WhiteBook[current_position].Moves[i].Stop+1, WhiteBook[current_position].Moves[i].Next);

	}
	else
	{	move_position = (int)( random_value * (double) BlackBook[current_position].nr_of_moves);
		j =  sprintf(result,        "N%2d", BlackBook[current_position].Moves[move_position].Start);
		j += sprintf(result + j, "%2d00000000\0", BlackBook[current_position].Moves[move_position].Stop);
		result_length = j + 1;
		//// debug
		nr_of_moves = BlackBook[current_position].nr_of_moves;
		//printf("Get move from black opening book: move_position = %d\n", move_position);
		//for (i=0; i<BlackBook[current_position].nr_of_moves; i++)
		//	printf("%d, %d, %d\n", BlackBook[current_position].Moves[i].Start+1, BlackBook[current_position].Moves[i].Stop+1, BlackBook[current_position].Moves[i].Next);

	}
	if (white)
	{	--white_nr_of_moves_to_go;
		global_move_nr = (int)(total_moves_allowed - white_nr_of_moves_to_go);
	}
	else
	{	--black_nr_of_moves_to_go;
		global_move_nr = (int)(total_moves_allowed - black_nr_of_moves_to_go);
	}
	if (white) printf("%2.0f.", total_moves_allowed - white_nr_of_moves_to_go);
	else printf("%2.0f.", total_moves_allowed - black_nr_of_moves_to_go);
	printf("->%s book %d %d %d %d ", result, move_position, nr_of_moves, current_position, nr_of_moves_in_game_list);
	if (white) printf("white book\n"); else printf("black book\n");
	return;
}
*/
