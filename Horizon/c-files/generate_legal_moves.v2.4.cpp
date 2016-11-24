/*
  This file is Copyright (C) 2010 Feike Boomstra.
  Distributed under the Boost Software License, Version 1.0.
  (See accompanying file LICENSE_1_0.txt or copy at
  http://www.boost.org/LICENSE_1_0.txt)
*/


#include "BoomstraDam.v2.5.h"
#include <pthread.h>
// v2.2 implements statistical move ordening, combined with historical best_move
/*
BitArray king_cap_se[54]={
	0ULL|FLD7|FLD12|FLD18|FLD23|FLD29|FLD34|FLD40,
	0ULL|FLD8|FLD13|FLD19|FLD24|FLD30,
	0ULL|FLD9|FLD14|FLD20,
	0ULL|FLD10,
	0ULL,
	0ULL|FLD11|FLD17|FLD22|FLD28|FLD33|FLD39|FLD44,
	0ULL|FLD12|FLD18|FLD23|FLD29|FLD34|FLD40,
	0ULL|FLD13|FLD19|FLD24|FLD30,
	0ULL|FLD14|FLD20,
	0ULL,
	0ULL,
	0ULL|FLD17|FLD22|FLD28|FLD33|FLD39|FLD44,
	0ULL|FLD18|FLD23|FLD29|FLD34|FLD40,
	0ULL|FLD19|FLD24|FLD30,
	0ULL|FLD20,
	0ULL,
	0ULL|FLD21|FLD27|FLD32|FLD38|FLD43,
	0ULL|FLD22|FLD28|FLD33|FLD39|FLD44,
	0ULL|FLD23|FLD29|FLD34|FLD40,
	0ULL|FLD24|FLD30,
	0ULL,
	0ULL,
	0ULL|FLD27|FLD32|FLD38|FLD43,
	0ULL|FLD28|FLD33|FLD39|FLD44,
	0ULL|FLD29|FLD34|FLD40,
	0ULL|FLD30,
	0ULL,
	0ULL|FLD31|FLD37|FLD42,
	0ULL|FLD32|FLD38|FLD43,
	0ULL|FLD33|FLD39|FLD44,
	0ULL|FLD34|FLD40,
	0ULL,
	0ULL,
	0ULL|FLD37|FLD42,
	0ULL|FLD38|FLD43,
	0ULL|FLD39|FLD44,
	0ULL|FLD40,
	0ULL,
	0ULL|FLD41,
	0ULL|FLD42,
	0ULL|FLD43,
	0ULL|FLD44,
	0ULL,
	0ULL,
	0ULL,
	0ULL,
	0ULL,
	0ULL,
	0ULL,
	0ULL,
	0ULL,
	0ULL,
	0ULL,
	0ULL
};
BitArray king_cap_sw[54]={
	0ULL,
	0ULL|FLD7|FLD11,
	0ULL|FLD8|FLD12|FLD17|FLD21,
	0ULL|FLD9|FLD13|FLD18|FLD22|FLD27|FLD31,
	0ULL|FLD10|FLD14|FLD19|FLD23|FLD28|FLD32|FLD37|FLD41,
	0ULL,
	0ULL|FLD11,
	0ULL|FLD12|FLD17|FLD21,
	0ULL|FLD13|FLD18|FLD22|FLD27|FLD31,
	0ULL|FLD14|FLD19|FLD23|FLD28|FLD32|FLD37|FLD41,
	0ULL,
	0ULL,
	0ULL|FLD17|FLD21,
	0ULL|FLD18|FLD22|FLD27|FLD31,
	0ULL|FLD19|FLD23|FLD28|FLD32|FLD37|FLD41,
	0ULL|FLD20|FLD24|FLD29|FLD33|FLD38|FLD42,
	0ULL,
	0ULL|FLD21,
	0ULL|FLD22|FLD27|FLD31,
	0ULL|FLD23|FLD28|FLD32|FLD37|FLD41,
	0ULL|FLD24|FLD29|FLD33|FLD38|FLD42,
	0ULL,
	0ULL,
	0ULL|FLD27|FLD31,
	0ULL|FLD28|FLD32|FLD37|FLD41,
	0ULL|FLD29|FLD33|FLD38|FLD42,
	0ULL|FLD30|FLD34|FLD39|FLD43,
	0ULL,
	0ULL|FLD31,
	0ULL|FLD32|FLD37|FLD41,
	0ULL|FLD33|FLD38|FLD42,
	0ULL|FLD34|FLD39|FLD43,
	0ULL,
	0ULL,
	0ULL|FLD37|FLD41,
	0ULL|FLD38|FLD42,
	0ULL|FLD39|FLD43,
	0ULL|FLD40|FLD44,
	0ULL,
	0ULL|FLD41,
	0ULL|FLD42,
	0ULL|FLD43,
	0ULL|FLD44,
	0ULL,
	0ULL,
	0ULL,
	0ULL,
	0ULL,
	0ULL,
	0ULL,
	0ULL,
	0ULL,
	0ULL,
	0ULL
};
BitArray king_cap_ne[54]={
	0ULL,
	0ULL,
	0ULL,
	0ULL,
	0ULL,
	0ULL,
	0ULL,
	0ULL,
	0ULL,
	0ULL,
	0ULL,
	0ULL|FLD7,
	0ULL|FLD8,
	0ULL|FLD9,
	0ULL|FLD10,
	0ULL,
	0ULL|FLD11|FLD7,
	0ULL|FLD12|FLD8,
	0ULL|FLD13|FLD9,
	0ULL|FLD14|FLD10,
	0ULL,
	0ULL,
	0ULL|FLD17|FLD12|FLD8,
	0ULL|FLD18|FLD13|FLD9,
	0ULL|FLD19|FLD14|FLD10,
	0ULL|FLD20,
	0ULL,
	0ULL|FLD21|FLD17|FLD12|FLD8,
	0ULL|FLD22|FLD18|FLD13|FLD9,
	0ULL|FLD23|FLD19|FLD14|FLD10,
	0ULL|FLD24|FLD20,
	0ULL,
	0ULL,
	0ULL|FLD27|FLD22|FLD18|FLD13|FLD9,
	0ULL|FLD28|FLD23|FLD19|FLD14|FLD10,
	0ULL|FLD29|FLD24|FLD20,
	0ULL|FLD30,
	0ULL,
	0ULL|FLD31|FLD27|FLD22|FLD18|FLD13|FLD9,
	0ULL|FLD32|FLD28|FLD23|FLD19|FLD14|FLD10,
	0ULL|FLD33|FLD29|FLD24|FLD20,
	0ULL|FLD34|FLD30,
	0ULL,
	0ULL,
	0ULL|FLD37|FLD32|FLD28|FLD23|FLD19|FLD14|FLD10,
	0ULL|FLD38|FLD33|FLD29|FLD24|FLD20,
	0ULL|FLD39|FLD34|FLD30,
	0ULL|FLD40,
	0ULL,
	0ULL|FLD41|FLD37|FLD32|FLD28|FLD23|FLD19|FLD14|FLD10,
	0ULL|FLD42|FLD38|FLD33|FLD29|FLD24|FLD20,
	0ULL|FLD43|FLD39|FLD34|FLD30,
	0ULL|FLD44|FLD40,
	0ULL
};
BitArray king_cap_nw[54]={
	0ULL,
	0ULL,
	0ULL,
	0ULL,
	0ULL,
	0ULL,
	0ULL,
	0ULL,
	0ULL,
	0ULL,
	0ULL,
	0ULL,
	0ULL|FLD7,
	0ULL|FLD8,
	0ULL|FLD9,
	0ULL|FLD10,
	0ULL,
	0ULL|FLD11,
	0ULL|FLD12|FLD7,
	0ULL|FLD13|FLD8,
	0ULL|FLD14|FLD9,
	0ULL,
	0ULL,
	0ULL|FLD17|FLD11,
	0ULL|FLD18|FLD12|FLD7,
	0ULL|FLD19|FLD13|FLD8,
	0ULL|FLD20|FLD14|FLD9,
	0ULL,
	0ULL|FLD21,
	0ULL|FLD22|FLD17|FLD11,
	0ULL|FLD23|FLD18|FLD12|FLD7,
	0ULL|FLD24|FLD19|FLD13|FLD8,
	0ULL,
	0ULL,
	0ULL|FLD27|FLD21,
	0ULL|FLD28|FLD22|FLD17|FLD11,
	0ULL|FLD29|FLD23|FLD18|FLD12|FLD7,
	0ULL|FLD30|FLD24|FLD19|FLD13|FLD8,
	0ULL,
	0ULL|FLD31,
	0ULL|FLD32|FLD27|FLD21,
	0ULL|FLD33|FLD28|FLD22|FLD17|FLD11,
	0ULL|FLD34|FLD29|FLD23|FLD18|FLD12|FLD7,
	0ULL,
	0ULL,
	0ULL|FLD37|FLD31,
	0ULL|FLD38|FLD32|FLD27|FLD21,
	0ULL|FLD39|FLD33|FLD28|FLD22|FLD17|FLD11,
	0ULL|FLD40|FLD34|FLD29|FLD23|FLD18|FLD12|FLD7,
	0ULL,
	0ULL|FLD41,
	0ULL|FLD42|FLD37|FLD31,
	0ULL|FLD43|FLD38|FLD32|FLD27|FLD21,
	0ULL|FLD44|FLD39|FLD33|FLD28|FLD22|FLD17|FLD11
};

*/
__inline BitArray nw(BitArray org)
{ return (org >> 6); }

__inline BitArray nw_nw(BitArray org)
{ return (org >> 12); }

__inline BitArray ne(BitArray org)
{ return (org >> 5); }

__inline BitArray ne_ne(BitArray org)
{ return (org >> 10); }

__inline BitArray sw(BitArray org)
{ return (org << 5); }

__inline BitArray sw_sw(BitArray org)
{ return (org << 10); }

__inline BitArray se(BitArray org)
{ return (org << 6);}

__inline BitArray se_se(BitArray org)
{ return (org << 12);}

/*int compare_function(WsPnt ws, int y, int x)
{ int a = ws->legal_moves[x].value;
  int b = ws->legal_moves[y].value;
  if (a > b)
    return 1;
  else if (a < b)
    return -1;
  else
    return 0;
}
*/
void store_move(WsPnt ws, BitArray mystart, BitArray mystop)
{	//if (int_to_ext[mystop] == 0) printf("mystop=%d\n", mystop);

	ws->legal_moves[ws->nr_of_legal_moves].start=mystart;
	ws->legal_moves[ws->nr_of_legal_moves].stop=mystop;
	ws->legal_moves[ws->nr_of_legal_moves].captured=0;
	ws->nr_of_legal_moves++;
}

void move_king(WsPnt ws, BitArray own_king) {
	BitArray test_location;
	// nw ??
	test_location = nw(own_king);
	while (test_location & ws->empty_field) {
		store_move(ws, own_king, test_location);
		test_location = nw(test_location);
	}
	// ne ??
	test_location = ne(own_king);
	while (test_location & ws->empty_field) {
		store_move(ws, own_king, test_location);
		test_location = ne(test_location);
	}
	// sw ??
	test_location = sw(own_king);
	while (test_location & ws->empty_field) {
		store_move(ws, own_king, test_location);
		test_location = sw(test_location);
	}
	// se ??
	test_location = se(own_king);
	while (test_location & ws->empty_field) {
		store_move(ws, own_king, test_location);
		test_location = se(test_location);
	}
	return;
}


void cap_king_next(WsPnt ws, BitArray own_king, int nr_of_captures, BitArray cur_captured, int last_direction) {
	BitArray test_location;
	BitArray last_captured;
	if (nr_of_captures < 3) {
	// first try nw
		if (last_direction != se_dir) {
			test_location = nw(own_king); //skip over empty space
			while (test_location & ws->empty_field)
				test_location = nw(test_location); //skip over empty space
			if (!(test_location & ws->other_piece)) goto N1; // capture not possible
			last_captured = test_location;
			test_location = nw(test_location); 		//location behind the possible capture
			if (!(test_location & ws->empty_field)) goto N1; // at least one landing point
			while (test_location & ws->empty_field) {
				cap_king_next(ws, test_location, nr_of_captures + 1, cur_captured | last_captured, nw_dir);
				test_location = nw(test_location);
			}
		}
	N1:
	// try ne
		if (last_direction != sw_dir) {
			test_location = ne(own_king); //skip over empty space
			while (test_location & ws->empty_field)
				test_location = ne(test_location); //skip over empty space
			if (!(test_location & ws->other_piece)) goto N2; // capture not possible
			last_captured = test_location;
			test_location = ne(test_location); 		//location behind the possible capture
			if (!(test_location & ws->empty_field)) goto N2; // at least one landing point
			while (test_location & ws->empty_field) {
				cap_king_next(ws, test_location, nr_of_captures + 1, cur_captured | last_captured, ne_dir);
				test_location = ne(test_location);
			}
		}
	N2:
	//  try sw
		if (last_direction != ne_dir) {
			test_location = sw(own_king); //skip over empty space
			while (test_location & ws->empty_field)
				test_location = sw(test_location); //skip over empty space
			if (!(test_location & ws->other_piece)) goto N3; // capture not possible
			last_captured = test_location;
			test_location = sw(test_location); 		//location behind the possible capture
			if (!(test_location & ws->empty_field)) goto N3; // at least one landing point
			while (test_location & ws->empty_field) {
				cap_king_next(ws, test_location, nr_of_captures + 1, cur_captured | last_captured, sw_dir);
				test_location = sw(test_location);
			}
		}
	N3:

	//  try se
		if (last_direction != nw_dir) {
			test_location = se(own_king); //skip over empty space
			while (test_location & ws->empty_field)
				test_location = se(test_location); //skip over empty space
			if (!(test_location & ws->other_piece)) goto N4; // capture not possible
			last_captured = test_location;
			test_location = se(test_location); 		//location behind the possible capture
			if (!(test_location & ws->empty_field)) goto N4; // at least one landing point
			while (test_location & ws->empty_field) {
				cap_king_next(ws, test_location, nr_of_captures + 1, cur_captured | last_captured, se_dir);
				test_location = se(test_location);
			}
		}
	}
	else { // more than 4 pieces captured
		// first try nw
			if (last_direction != se_dir) {
				test_location = nw(own_king); //skip over empty space
				while (test_location & ws->empty_field)
					test_location = nw(test_location); //skip over empty space
				if (!(test_location & ws->other_piece)) goto N11; // capture not possible
				if ((test_location & cur_captured)) goto N11; // twice same capture not allowed
				last_captured = test_location;
				test_location = nw(test_location); 		//location behind the possible capture
				if (!(test_location & ws->empty_field)) goto N11; // at least one landing point
				while (test_location & ws->empty_field) {
					cap_king_next(ws, test_location, nr_of_captures + 1, cur_captured | last_captured, nw_dir);
					test_location = nw(test_location);
				}
			}
		N11:
		// try ne
			if (last_direction != sw_dir) {
				test_location = ne(own_king); //skip over empty space
				while (test_location & ws->empty_field)
					test_location = ne(test_location); //skip over empty space
				if (!(test_location & ws->other_piece)) goto N12; // capture not possible
				if ((test_location & cur_captured)) goto N12; // twice same capture not allowed
				last_captured = test_location;
				test_location = ne(test_location); 		//location behind the possible capture
				if (!(test_location & ws->empty_field)) goto N12; // at least one landing point
				while (test_location & ws->empty_field) {
					cap_king_next(ws, test_location, nr_of_captures + 1, cur_captured | last_captured, ne_dir);
					test_location = ne(test_location);
				}
			}
		N12:
		//  try sw
			if (last_direction != ne_dir) {
				test_location = sw(own_king); //skip over empty space
				while (test_location & ws->empty_field)
					test_location = sw(test_location); //skip over empty space
				if (!(test_location & ws->other_piece)) goto N13; // capture not possible
				if ((test_location & cur_captured)) goto N13; // twice same capture not allowed
				last_captured = test_location;
				test_location = sw(test_location); 		//location behind the possible capture
				if (!(test_location & ws->empty_field)) goto N13; // at least one landing point
				while (test_location & ws->empty_field) {
					cap_king_next(ws, test_location, nr_of_captures + 1, cur_captured | last_captured, sw_dir);
					test_location = sw(test_location);
				}
			}
		N13:

		//  try se
			if (last_direction != nw_dir) {
				test_location = se(own_king); //skip over empty space
				while (test_location & ws->empty_field)
					test_location = se(test_location); //skip over empty space
				if (!(test_location & ws->other_piece)) goto N4; // capture not possible
				if ((test_location & cur_captured)) goto N4; // twice same capture not allowed
				last_captured = test_location;
				test_location = se(test_location); 		//location behind the possible capture
				if (!(test_location & ws->empty_field)) goto N4; // at least one landing point
				while (test_location & ws->empty_field) {
					cap_king_next(ws, test_location, nr_of_captures + 1, cur_captured | last_captured, se_dir);
					test_location = se(test_location);
				}
			}
		}


N4:
{	if (nr_of_captures >= ws->max_nr_of_captures)												/* at least we should save this move */
	{	int not_found = 1;
		int i;
		if (nr_of_captures > ws->max_nr_of_captures)											/* reset the nr_of_legal_moves */
		{	ws->max_nr_of_captures = nr_of_captures ;											/* update the current maximum */
			ws->nr_of_legal_moves = 0;
		};																						/* and start over again */
		ws->legal_moves[ws->nr_of_legal_moves].stop = own_king;									/* from here there were no more captures */
		ws->legal_moves[ws->nr_of_legal_moves].start = ws->current_start_position;
		//ws->legal_moves[ws->nr_of_legal_moves].nr_of_captures = ws->max_nr_of_captures ;		/* for later use  */
		ws->legal_moves[ws->nr_of_legal_moves].captured = cur_captured;							/* to be able to calculate the new board position */
		if (ws->max_nr_of_captures > 3)	// in case 4 or more pieces are captured multiple pathes are possible
		{	for (i=0; i < ws->nr_of_legal_moves; i++)												/* check whether we have a "new" capture */
			{	if ((ws->legal_moves[ws->nr_of_legal_moves].captured == ws->legal_moves[i].captured) && \
					(ws->legal_moves[ws->nr_of_legal_moves].start == ws->legal_moves[i].start) && \
					(ws->legal_moves[ws->nr_of_legal_moves].stop == ws->legal_moves[i].stop))
				{	not_found = 0; break; }
			}
			if (not_found) ws->nr_of_legal_moves++;													/* and save */
		}
		else ws->nr_of_legal_moves++;													/* and save */
	};
}



	return; // we are done

}
bool is_cap_king(BitArray other_piece, BitArray empty_field, BitArray own_king) {
	BitArray test_location;
// first try nw
	test_location = nw(own_king); //skip over empty space
	while (test_location & empty_field)
		test_location = nw(test_location); //skip over empty space
	if (!(test_location & other_piece)) goto N1; // capture not possible
	test_location = nw(test_location); 		//location behind the possible capture
	if ((test_location & empty_field)) return true;
N1:
// try ne
	test_location = ne(own_king); //skip over empty space
	while (test_location & empty_field)
		test_location = ne(test_location); //skip over empty space
	if (!(test_location & other_piece)) goto N2; // capture not possible
	test_location = ne(test_location); 		//location behind the possible capture
	if ((test_location & empty_field)) return true;
N2:
//  try sw
	test_location = sw(own_king); //skip over empty space
	while (test_location & empty_field)
		test_location = sw(test_location); //skip over empty space
	if (!(test_location & other_piece)) goto N3; // capture not possible
	test_location = sw(test_location); 		//location behind the possible capture
	if ((test_location & empty_field)) return true;
N3:

//  try se
	test_location = se(own_king); //skip over empty space
	while (test_location & empty_field)
		test_location = se(test_location); //skip over empty space
	if (!(test_location & other_piece)) goto N4; // capture not possible
	test_location = se(test_location); 		//location behind the possible capture
	if ((test_location & empty_field)) return true;
N4: return false; // we are done no king captures

}



void cap_king(WsPnt ws, BitArray own_king, int nr_of_captures, BitArray cur_captured) {
	BitArray test_location;
	BitArray last_captured;

// first try nw
	test_location = nw(own_king); //skip over empty space
	while (test_location & ws->empty_field)
		test_location = nw(test_location); //skip over empty space
	if (!(test_location & ws->other_piece)) goto N1; // capture not possible
	last_captured = test_location;
	test_location = nw(test_location); 		//location behind the possible capture
	if (!(test_location & ws->empty_field)) goto N1; // at least one landing point
	while (test_location & ws->empty_field) {
		cap_king_next(ws, test_location, nr_of_captures + 1, cur_captured | last_captured, nw_dir);
		test_location = nw(test_location);
	}
N1:
// try ne
	test_location = ne(own_king); //skip over empty space
	while (test_location & ws->empty_field)
		test_location = ne(test_location); //skip over empty space
	if (!(test_location & ws->other_piece)) goto N2; // capture not possible
	last_captured = test_location;
	test_location = ne(test_location); 		//location behind the possible capture
	if (!(test_location & ws->empty_field)) goto N2; // at least one landing point
	while (test_location & ws->empty_field) {
		cap_king_next(ws, test_location, nr_of_captures + 1, cur_captured | last_captured, ne_dir);
		test_location = ne(test_location);
	}
N2:
//  try sw
	test_location = sw(own_king); //skip over empty space
	while (test_location & ws->empty_field)
		test_location = sw(test_location); //skip over empty space
	if (!(test_location & ws->other_piece)) goto N3; // capture not possible
	last_captured = test_location;
	test_location = sw(test_location); 		//location behind the possible capture
	if (!(test_location & ws->empty_field)) goto N3; // at least one landing point
	while (test_location & ws->empty_field) {
		cap_king_next(ws, test_location, nr_of_captures + 1, cur_captured | last_captured, sw_dir);
		test_location = sw(test_location);

	}
N3:

//  try se
	test_location = se(own_king); //skip over empty space
	while (test_location & ws->empty_field)
		test_location = se(test_location); //skip over empty space
	if (!(test_location & ws->other_piece)) goto N4; // capture not possible
	last_captured = test_location;
	test_location = se(test_location); 		//location behind the possible capture
	if (!(test_location & ws->empty_field)) goto N4; // at least one landing point
	while (test_location & ws->empty_field) {
		cap_king_next(ws, test_location, nr_of_captures + 1, cur_captured | last_captured, se_dir);
		test_location = se(test_location);

	}
N4: return; // we are done

}



void cap_man(WsPnt ws, BitArray own_man_bit, int nr_of_captures, BitArray cur_captured, int last_direction)
{	// check all directions
	BitArray temp;
//	int nr_of_bits;
//	unsigned long bit_list[20];

	if (nr_of_captures >= 4) {
		// capture in nw direction??
		if (last_direction != se_dir) {
			if (!(temp = (own_man_bit & se(ws->other_piece)))) goto N1;
			if (!(temp = (temp & se_se(ws->empty_field)))) goto N1;
			if (cur_captured & nw(temp)) goto N1;
			cap_man(ws, nw_nw(own_man_bit), nr_of_captures + 1, cur_captured | nw(own_man_bit), nw_dir);						/* capture with the man */
		}
	N1:	// capture in ne direction??
		if (last_direction != sw_dir) {
			if (!(temp = (own_man_bit & sw(ws->other_piece)))) goto N2;
			if (!(temp = (temp & sw_sw(ws->empty_field)))) goto N2;
			if (cur_captured & ne(temp)) goto N2;
			cap_man(ws, ne_ne(own_man_bit), nr_of_captures + 1, cur_captured | ne(own_man_bit), ne_dir);						/* capture with the man */
		}
	N2:	// capture in sw direction??
		if (last_direction != ne_dir) {
			if (!(temp = (own_man_bit & ne(ws->other_piece)))) goto N3;
			if (!(temp = (temp & ne_ne(ws->empty_field)))) goto N3;
			if (cur_captured & sw(temp)) goto N3;
			cap_man(ws, sw_sw(own_man_bit), nr_of_captures + 1, cur_captured | sw(own_man_bit), sw_dir);						/* capture with the man */
		}
	N3:	// capture in se direction??
		if (last_direction != nw_dir) {
			if (!(temp = (own_man_bit & nw(ws->other_piece)))) goto N4;
			if (!(temp = (temp & nw_nw(ws->empty_field)))) goto N4;
			if (cur_captured & se(temp)) goto N4;
			cap_man(ws, se_se(own_man_bit), nr_of_captures + 1, cur_captured | se(own_man_bit), se_dir);						/* capture with the man */
		}
	}
	else { // nr of captures < 4, so no double capture possible
		// capture in nw direction??
		if (last_direction != se_dir) {
			if (!(temp = (own_man_bit & se(ws->other_piece)))) goto N11;
			if (!(temp = (temp & se_se(ws->empty_field)))) goto N11;
			cap_man(ws, nw_nw(own_man_bit), nr_of_captures + 1, cur_captured | nw(own_man_bit), nw_dir);						/* capture with the man */
		}
		N11:	// capture in ne direction??
		if (last_direction != sw_dir) {
			if (!(temp = (own_man_bit & sw(ws->other_piece)))) goto N12;
			if (!(temp = (temp & sw_sw(ws->empty_field)))) goto N12;
			cap_man(ws, ne_ne(own_man_bit), nr_of_captures + 1, cur_captured | ne(own_man_bit), ne_dir);						/* capture with the man */
		}
		N12:	// capture in sw direction??
		if (last_direction != ne_dir) {
			if (!(temp = (own_man_bit & ne(ws->other_piece)))) goto N13;
			if (!(temp = (temp & ne_ne(ws->empty_field)))) goto N13;
			cap_man(ws, sw_sw(own_man_bit), nr_of_captures + 1, cur_captured | sw(own_man_bit), sw_dir);						/* capture with the man */
		}
		N13:	// capture in se direction??
		if (last_direction != nw_dir) {
			if (!(temp = (own_man_bit & nw(ws->other_piece)))) goto N4;
			if (!(temp = (temp & nw_nw(ws->empty_field)))) goto N4;
			cap_man(ws, se_se(own_man_bit), nr_of_captures + 1, cur_captured | se(own_man_bit), se_dir);						/* capture with the man */
		}
	}
	N4: // all done

	{	if (nr_of_captures >= ws->max_nr_of_captures)												/* at least we should save this move */
		{	int not_found = 1;
			int i;
			if (nr_of_captures > ws->max_nr_of_captures)											/* reset the nr_of_legal_moves */
			{	ws->max_nr_of_captures = nr_of_captures ;											/* update the current maximum */
				ws->nr_of_legal_moves = 0;
			};																						/* and start over again */
			ws->legal_moves[ws->nr_of_legal_moves].stop = own_man_bit;							/* from here there were no more captures */
			ws->legal_moves[ws->nr_of_legal_moves].start = ws->current_start_position;
			//ws->legal_moves[ws->nr_of_legal_moves].nr_of_captures = ws->max_nr_of_captures ;		/* for later use  */
			ws->legal_moves[ws->nr_of_legal_moves].captured = cur_captured;							/* to be able to calculate the new board position */
			if (ws->max_nr_of_captures > 3)	// in case 4 or more pieces are captured multiple pathes are possible
			{	for (i=0; i < ws->nr_of_legal_moves; i++)												/* check whether we have a "new" capture */
				{	if ((ws->legal_moves[ws->nr_of_legal_moves].captured == ws->legal_moves[i].captured) && \
						(ws->legal_moves[ws->nr_of_legal_moves].start == ws->legal_moves[i].start) && \
						(ws->legal_moves[ws->nr_of_legal_moves].stop == ws->legal_moves[i].stop))
					{	not_found = 0; break; }
				}
				if (not_found) ws->nr_of_legal_moves++;													/* and save */
			}
			else ws->nr_of_legal_moves++;													/* and save */
		};
	}
	return;


}
int more_or_equal_pieces(NodePnt node)
{
	if (node->status & IS_WHITE_ON_MOVE)
		return (node->nr_of_white_man-org_piece_count.nr_of_white_man) + 3*(node->nr_of_white_king-org_piece_count.nr_of_white_king) >=
		(node->nr_of_black_man-org_piece_count.nr_of_black_man) + 3*(node->nr_of_black_king-org_piece_count.nr_of_black_king);
	else  // black on move
		return (node->nr_of_white_man-org_piece_count.nr_of_white_man) + 3*(node->nr_of_white_king-org_piece_count.nr_of_white_king) <=
		(node->nr_of_black_man-org_piece_count.nr_of_black_man) + 3*(node->nr_of_black_king-org_piece_count.nr_of_black_king);
}

int other_side_is_not_on_capture (NodePnt);
int other_side_was_earlier_on_capture(NodePnt);

void set_capture(WsPnt ws, NodePnt node){
	ws->capture = (ws->max_nr_of_captures > 0);
	//ws->other_side_has_to_capture = !(other_side_is_not_on_capture(node));
	if (ws->capture) node->status = node->status | HAD_TO_CAPTURE;
}

void set_quiscence(WsPnt ws, NodePnt node){
	//ws->org_node = node;
	//ws->parent = NULL;
	bool other_side_ok;
	if (other_side_is_not_on_capture(node)) other_side_ok = true;
	//else if ((other_side_was_earlier_on_capture(node))) other_side_ok = true; //??
	else other_side_ok = false;

	if (more_or_equal_pieces(node))
	{	if (node->total_nr_of_pieces > NR_OF_ENDGAME_PIECES){
			ws->quiescence = (other_side_ok && !(ws->capture));
		}
		else
			ws->quiescence = (!(ws->capture));
	}
	else ws->quiescence = !(ws->capture); // if this side is behind, don't extend the search
/*

	if (node->total_nr_of_pieces > NR_OF_ENDGAME_PIECES){
		ws->quiescence = (other_side_is_not_on_capture(node) && (ws->nr_of_legal_moves >= 3) && !(ws->capture));
	}
	else
		ws->quiescence = (!(ws->capture));

*/
	if (ws->quiescence) node->status = node->status | IS_QUIESCENCE;
	return;
}


void generate_legal_moves(NodePnt node, WsPnt ws, int color_white, int best_move)
{	BitArray temp;
//	unsigned long bit_list[21];
//	unsigned long king_list[21];
//	int nr_of_bits;
//	int nr_of_kings;
	BitArray orig_empty, moving_piece;
//	unsigned char iidx;
	int idx;
	orig_empty = ~node->occ_empty & BITS50;
	if (color_white)
	{
		ws->own_man = node->occ_empty & node->white_black & node->man_king;
		ws->own_king = node->occ_empty & node->white_black & ~(node->man_king);
		ws->other_piece = node->occ_empty & ~(node->white_black);
	}
	else
	{	ws->own_man = node->occ_empty & ~(node->white_black) & node->man_king;
		ws->own_king = node->occ_empty & ~(node->white_black) & ~(node->man_king);
		ws->other_piece = node->occ_empty & (node->white_black);
	};

	/* initialize (global) variables */

	ws->nr_of_legal_moves = 0;
	ws->max_nr_of_captures = 0;

	// first check the possibility of captures with a man

	if ((nw(ws->other_piece) | ne(ws->other_piece) | sw(ws->other_piece) | se(ws->other_piece)) & ws->own_man == 0) goto N4;

	// now check the possible captures with a man

	if (!(temp = (ws->own_man & se(ws->other_piece)))) goto N1;
	if (!(temp = (temp & se_se(orig_empty)))) goto N1;
		// in temp the start bits for a capture in nw direction
	while (temp)
	{
		moving_piece = temp ^ (temp & (temp - 1)); temp = (temp & (temp - 1));
		ws->empty_field = orig_empty | moving_piece;	/* set it temporary free */
		ws->current_start_position = moving_piece ;			/* to keep the start position */
		cap_man(ws, nw_nw(moving_piece), 1, nw(moving_piece), nw_dir);			/* capture with the man */
	}

N1:	//printf("Legal moves N1\n");
	if (!(temp = (ws->own_man & sw(ws->other_piece)))) goto N2;
	if (!(temp = (temp & sw_sw(orig_empty)))) goto N2;
	// in temp the start bits for a capture in ne direction
	while (temp)
	{
		moving_piece = temp ^ (temp & (temp - 1)); temp = (temp & (temp - 1));
		ws->empty_field = orig_empty | moving_piece;	/* set it temporary free */
		ws->current_start_position = moving_piece ;			/* to keep the start position */
		cap_man(ws, ne_ne(moving_piece), 1, ne(moving_piece), ne_dir);			/* capture with the man */
	};
N2:	//printf("Legal moves N2\n");
	if (!(temp = (ws->own_man & ne(ws->other_piece)))) goto N3;
	if (!(temp = (temp & ne_ne(orig_empty)))) goto N3;
	// in temp the start bits for a capture in sw direction
	while (temp)
	{
		moving_piece = temp ^ (temp & (temp - 1)); temp = (temp & (temp - 1));
		ws->empty_field = orig_empty | moving_piece;	/* set it temporary free */
		ws->current_start_position = moving_piece ;			/* to keep the start position */
		cap_man(ws, sw_sw(moving_piece), 1, sw(moving_piece), sw_dir);			/* capture with the man */
	};
N3:	//printf("Legal moves N3\n");
	if (!(temp = (ws->own_man & nw(ws->other_piece)))) goto N4;
	if (!(temp = (temp & nw_nw(orig_empty)))) goto N4;
	// in temp the start bits for a capture in se direction
	while (temp)
	{
		moving_piece = temp ^ (temp & (temp - 1)); temp = (temp & (temp - 1));
		ws->empty_field = orig_empty | moving_piece;	/* set it temporary free */
		ws->current_start_position = moving_piece ;			/* to keep the start position */
		cap_man(ws, se_se(moving_piece), 1, se(moving_piece), se_dir);			/* capture with the man */
	};
N4: 	//printf("Legal moves N4\n");
	// all possible captures with man are done
	// we will look now for captures with a king
	temp = ws->own_king;
	while (temp)
	{
		moving_piece = temp ^ (temp & (temp - 1)); temp = (temp & (temp - 1));
		ws->empty_field = orig_empty | moving_piece;	/* set it temporary free */
		ws->current_start_position = moving_piece ;			/* to keep the start position */
		cap_king(ws, moving_piece, 0, 0);			/* capture with the king */
	};
	//fprintf_s(log_file,"Legal moves N5\n");

	if (ws->max_nr_of_captures == 0)
	{

	// no captures, try moves:
		ws->empty_field = orig_empty;
		if (color_white)
		{	temp = se(orig_empty) & ws->own_man;
			while (temp)
			{
				moving_piece = temp ^ (temp & (temp - 1));
				temp = (temp & (temp - 1));
				store_move(ws, moving_piece, nw(moving_piece));
			}

			temp = sw(orig_empty) & ws->own_man;
			while (temp)
			{
				moving_piece = temp ^ (temp & (temp - 1)); temp = (temp & (temp - 1));
				store_move(ws, moving_piece, ne(moving_piece));
			}
		}
		else //color == black
		{	temp = ne(orig_empty) & ws->own_man;
			while (temp)
			{
				moving_piece = temp ^ (temp & (temp - 1)); temp = (temp & (temp - 1));
				store_move(ws, moving_piece, sw(moving_piece));
			}
			temp = nw(orig_empty) & ws->own_man;
			while (temp)
			{
				moving_piece = temp ^ (temp & (temp - 1)); temp = (temp & (temp - 1));
				store_move(ws, moving_piece, se(moving_piece));
			}
		}
		// and finally the moves with a king
		temp = ws->own_king;
		while (temp)
		{
			moving_piece = temp ^ (temp & (temp - 1)); temp = (temp & (temp - 1));
			move_king(ws, moving_piece);
		}
	}
	if (ws->nr_of_legal_moves > 1)
	{
		//printf("max cap = %d, moves = %d\n", ws->max_nr_of_captures, ws->nr_of_legal_moves);
		for (idx=0; idx < ws->nr_of_legal_moves; idx++)
		{	ws->move_order[idx] = idx;
/*			if (color_white) ws->legal_moves[iidx].value = white_freq[global_move_nr][ws->legal_moves[iidx].start][ws->legal_moves[iidx].stop];
			else ws->legal_moves[iidx].value = black_freq[global_move_nr][ws->legal_moves[iidx].start][ws->legal_moves[iidx].stop];*/
		}
		if ((best_move > 0) && (best_move < ws->nr_of_legal_moves))
		{	int temp = ws->move_order[best_move];
			ws->move_order[best_move] = ws->move_order[0];
			ws->move_order[0] = temp;
		}
		//printf("Now we start Qsort with %d moves\n", ws->nr_of_legal_moves);
		//Qsort(ws, ws->move_order, &compare_function, sort_start, ws->nr_of_legal_moves - 1);
	}
	else ws->move_order[0] = 0;


	return;
}

unsigned char generate_limited_moves(NodePnt node, WsPnt ws, char color_white, BitArray mask, char include_moves, char include_captures)
{
	BitArray temp, moving_piece;
	BitArray orig_empty = ~(node->occ_empty) & BITS50;
	if (color_white)
	{
		ws->own_man = node->occ_empty & node->white_black & node->man_king & mask;
		ws->own_king = node->occ_empty & node->white_black & ~(node->man_king);
		ws->other_piece = node->occ_empty & ~(node->white_black);
	}
	else
	{	ws->own_man = node->occ_empty & ~(node->white_black) & node->man_king & mask;
		ws->own_king = node->occ_empty & ~(node->white_black) & ~(node->man_king);
		ws->other_piece = node->occ_empty & (node->white_black);
	};
	/* initialize (global) variables */

	ws->nr_of_legal_moves = 0;
	ws->max_nr_of_captures = 0;

	// first check the possible captures with a man
	if (include_captures)
	{
		// now check the possible captures with a man

		if (!(temp = (ws->own_man & se(ws->other_piece)))) goto N1;
		if (!(temp = (temp & se_se(orig_empty)))) goto N1;
			// in temp the start bits for a capture in nw direction
		while (temp)
		{
			moving_piece = temp ^ (temp & (temp - 1)); temp = (temp & (temp - 1));
			ws->empty_field = orig_empty | moving_piece;	/* set it temporary free */
			ws->current_start_position = moving_piece ;			/* to keep the start position */
			cap_man(ws, nw_nw(moving_piece), 1, nw(moving_piece), nw_dir);			/* capture with the man */
		}

	N1:	//printf("Legal moves N1\n");
		if (!(temp = (ws->own_man & sw(ws->other_piece)))) goto N2;
		if (!(temp = (temp & sw_sw(orig_empty)))) goto N2;
		// in temp the start bits for a capture in ne direction
		while (temp)
		{
			moving_piece = temp ^ (temp & (temp - 1)); temp = (temp & (temp - 1));
			ws->empty_field = orig_empty | moving_piece;	/* set it temporary free */
			ws->current_start_position = moving_piece ;			/* to keep the start position */
			cap_man(ws, ne_ne(moving_piece), 1, ne(moving_piece), ne_dir);			/* capture with the man */
		};
	N2:	//printf("Legal moves N2\n");
		if (!(temp = (ws->own_man & ne(ws->other_piece)))) goto N3;
		if (!(temp = (temp & ne_ne(orig_empty)))) goto N3;
		// in temp the start bits for a capture in sw direction
		while (temp)
		{
			moving_piece = temp ^ (temp & (temp - 1)); temp = (temp & (temp - 1));
			ws->empty_field = orig_empty | moving_piece;	/* set it temporary free */
			ws->current_start_position = moving_piece ;			/* to keep the start position */
			cap_man(ws, sw_sw(moving_piece), 1, sw(moving_piece), sw_dir);			/* capture with the man */
		};
	N3:	//printf("Legal moves N3\n");
		if (!(temp = (ws->own_man & nw(ws->other_piece)))) goto N4;
		if (!(temp = (temp & nw_nw(orig_empty)))) goto N4;
		// in temp the start bits for a capture in se direction
		while (temp)
		{
			moving_piece = temp ^ (temp & (temp - 1)); temp = (temp & (temp - 1));
			ws->empty_field = orig_empty | moving_piece;	/* set it temporary free */
			ws->current_start_position = moving_piece ;			/* to keep the start position */
			cap_man(ws, se_se(moving_piece), 1, se(moving_piece), se_dir);			/* capture with the man */
		};
	N4: 	//printf("Legal moves N4\n");
		// all possible captures with man are done
		// we will look now for captures with a king
		temp = ws->own_king;
		while (temp)
		{
			moving_piece = temp ^ (temp & (temp - 1)); temp = (temp & (temp - 1));
			ws->empty_field = orig_empty | moving_piece;	/* set it temporary free */
			ws->current_start_position = moving_piece ;			/* to keep the start position */
			cap_king(ws, moving_piece, 0, 0);			/* capture with the king */
		};
		if (ws->max_nr_of_captures > 0)
		{	if (color_white)
				ws->own_man = node->occ_empty & node->white_black & node->man_king;
			else
				ws->own_man = node->occ_empty & ~(node->white_black) & node->man_king;

			return ws->max_nr_of_captures; // done
		}
	}


	if (include_moves)
	{
		if (color_white)
		{	temp = se(orig_empty) & ws->own_man;
			while (temp)
			{
				moving_piece = temp ^ (temp & (temp - 1)); temp = (temp & (temp - 1));
				store_move(ws, moving_piece, nw(moving_piece));
			}

			temp = sw(orig_empty) & ws->own_man;
			while (temp)
			{
				moving_piece = temp ^ (temp & (temp - 1)); temp = (temp & (temp - 1));
				store_move(ws, moving_piece, ne(moving_piece));
			}
		}
		else //color == black
		{	temp = ne(orig_empty) & ws->own_man;
			while (temp)
			{
				moving_piece = temp ^ (temp & (temp - 1)); temp = (temp & (temp - 1));
				store_move(ws, moving_piece, sw(moving_piece));
			}
			temp = nw(orig_empty) & ws->own_man;
			while (temp)
			{
				moving_piece = temp ^ (temp & (temp - 1)); temp = (temp & (temp - 1));
				store_move(ws, moving_piece, se(moving_piece));
			}
		}
		// and finally the moves with a king
		temp = ws->own_king;
		while (temp)
		{
			moving_piece = temp ^ (temp & (temp - 1)); temp = (temp & (temp - 1));
			move_king(ws, moving_piece);
		}
	} // include moves
	// restore own_man for populate
	if (color_white)
		ws->own_man = node->occ_empty & node->white_black & node->man_king;
	else
		ws->own_man = node->occ_empty & ~(node->white_black) & node->man_king;

	return 0;
}
//void print_val_l(char g, WsPnt ws, int best_move, char display_level)
//{	int i;
//	for (i=0; i<(display_level+1); i = i+1) printf("   ");
//	if (best_move == -1) printf("eval: ");
//	else print_move(ws, best_move);
//	printf("val = %d\n", g);
//}
//
//
//void print_move_l(NodePnt node, WsPnt ws, int idx, unsigned char display_level)
//{	int i;
//	for (i=0; i<(display_level); i = i+1) printf("      ");
//	print_move(ws, idx);
//	printf("\n");
//}


int saldo(NodePnt node, int color_is_white)
{

	//printf("color=%d, white=%d, black=%d\n", color_is_white, node->nr_of_white_man, node->nr_of_black_man);
	if (color_is_white)
		return (node->nr_of_white_man - org_piece_count.nr_of_white_man) - (node->nr_of_black_man - org_piece_count.nr_of_black_man)
		+ 3*(node->nr_of_white_king - node->nr_of_black_king); // no king in the beginning, maybe now??
	else
		return (node->nr_of_black_man - org_piece_count.nr_of_black_man) - (node->nr_of_white_man - org_piece_count.nr_of_white_man)
		+ 3*(node->nr_of_black_king - node->nr_of_white_king); // no king in the beginning, maybe now??
}

int locking_color_on_move_continue(NodePnt, BitArray, BitArray, int, int);

int locked_color_on_move_continue(NodePnt node, BitArray min_mask, BitArray max_mask, int level, int color_is_white)
{	struct work_space_move_generator ws;
	struct tnode next_node;
	unsigned int i; int z;
	int g = 40;


	generate_limited_moves(node, &ws, node->status & IS_WHITE_ON_MOVE, BITS50, 0, 1);   // check captures only

	if (ws.max_nr_of_captures > 0)
	{	for (i=0; i<ws.nr_of_legal_moves; i++)
		{	populate_node(&ws, &next_node, i, node);
			//printf("locked_color_on_move: %d  ", level); print_move(&ws, i ); printf("\n");
			z = locking_color_on_move_continue(&next_node, min_mask, max_mask, level, color_is_white);
			if (z < g) g = z;
		}
	}
	else
	{	// there are no captures, so we limit the moves to "mask"
		/*if (level <= 0)*/ { char res = saldo(node, color_is_white); /*print_val_l(res, &ws, best_move, display_level);*/ return res; } ;
		generate_limited_moves(node, &ws, node->status & IS_WHITE_ON_MOVE, min_mask, 1, 0);
		if (ws.nr_of_legal_moves == 0) return saldo(node, color_is_white);
		for (i=0; i<ws.nr_of_legal_moves; i++)
		{	populate_node(&ws, &next_node, i, node);
			//printf("locked_color_on_move: %d  ", level); print_move(&ws, i ); printf("\n");
			z = locking_color_on_move_continue(&next_node, min_mask, max_mask, level - 1, color_is_white);
			if (z < g) g = z;
		}
	}
	return g;
}


int locking_color_on_move_continue(NodePnt node, BitArray other_mask, BitArray own_mask, int level, int color_is_white)
{	struct work_space_move_generator ws;
	struct tnode next_node;
	unsigned int i; int z;
	int g = -40;

	generate_limited_moves(node, &ws, node->status & IS_WHITE_ON_MOVE, BITS50, 0, 1);  // is there a capture

	if (ws.max_nr_of_captures > 0)
	{	for (i=0; i<ws.nr_of_legal_moves; i++)
		{	populate_node(&ws, &next_node, i, node);
			//printf("locking_color_on_move: %d  ", level); print_move(&ws, i ); printf("\n");
			z = locked_color_on_move_continue(&next_node, other_mask, own_mask, level, color_is_white);
			if (z > g) g = z;
		}
	}
	else
	{	if (level <= 0) return saldo(node, color_is_white);
		generate_limited_moves(node, &ws, node->status & IS_WHITE_ON_MOVE, ~own_mask & BITS50, 1, 0);
		if (ws.nr_of_legal_moves == 0)return saldo(node, color_is_white);
		for (i=0; i<ws.nr_of_legal_moves; i++)
		{	populate_node(&ws, &next_node, i, node);
			//printf("locking_color_on_move: %d  ", level); print_move(&ws, i ); printf("\n");
			z = locked_color_on_move_continue(&next_node, other_mask, own_mask, level - 1, color_is_white);
			if (z > g) g = z;
		}
	}
	return g;
}

int locked_color_on_move(NodePnt node, BitArray other_mask, BitArray own_mask, int color_is_white)
{	struct work_space_move_generator ws;
	struct tnode next_node;
	unsigned int i; int z;
	int g = 40;
	//print_bits(other_mask);
	//print_bits(own_mask);
	// the node has the opposite color to the color we are testing for.
	// the function saldo returns the difference in number of pieces compared to the
	// starting position, seen through the eyes of the other color

	generate_limited_moves(node, &ws, node->status & IS_WHITE_ON_MOVE, other_mask, 1, 1);
	//print_moves(&ws);
	if (ws.nr_of_legal_moves == 0)
	{	return 1; // no break out
	}
	for (i=0; i<ws.nr_of_legal_moves; i++)
	{	populate_node(&ws, &next_node, i, node);
		//printf("locked_color_on_move: 2 "); print_move(&ws, i ); printf("\n");
		z = locking_color_on_move_continue(&next_node, other_mask, own_mask, 1, color_is_white);
		if (z < g) g = z;
	}
	return g;
}

int white_is_locking_pieces(NodePnt node, int wc, BitArray other_mask, BitArray own_mask)
{	struct work_space_move_generator ws;
	struct tnode next_node;
	unsigned int i;
	int g = -40, z;
	if (wc < 0) return 0;
	if (node->status & IS_WHITE_ON_MOVE)
	{	//printf("wit locking, wit on move\n");
		// white is on move, but we want to know if black can't break out, so:
		generate_limited_moves(node, &ws, node->status & IS_WHITE_ON_MOVE, (~own_mask & BITS50), 1, 1);
		if (ws.nr_of_legal_moves < 3) // nr of white moves is too little, we try them all
		{	//printf("wit locking, own moves=%d\n", ws.nr_of_legal_moves);
				for (i=0; i<ws.nr_of_legal_moves; i++)
			{	populate_node(&ws, &next_node, i, node);
				//print_move_l(node, &ws, i, 0);
				z = locked_color_on_move(&next_node, other_mask, own_mask, 1);
				if (z > g) g = z;
			}
			if (g > 0) return 1; else return 0;
			//printf("wit locking, on move, moved first, return=%d\n", g);
			//if (g > 0) return wc; else return 0;
		}
		else
		{	// we assume that there is at least one white move that does not break the "lock"
			next_node.occ_empty = node->occ_empty;
			next_node.man_king = node->man_king;
			next_node.white_black = node->white_black;
			next_node.status = ~node->status & (IS_WHITE_ON_MOVE | IS_MAX_NODE); // invert these bits
			g = locked_color_on_move(&next_node, other_mask, own_mask, 1);
			if (g > 0) return 1; else return 0;
			//printf("wit locking, on move, dummy move, return=%d\n", g);

			//if (g > 0) return wc; else return 0;
		}
	}
	else
	{	g = locked_color_on_move(node, other_mask, own_mask, 1);
		//printf("wit locking, not on move, return=%d\n", g);
		if (g > 0) return 1; else return 0;
		//if (g > 0) return wc; else return 0;
	}
}

int black_is_locking_pieces(NodePnt node, int wc, BitArray other_mask, BitArray own_mask)
{	struct work_space_move_generator ws;
	struct tnode next_node;
	unsigned int i;
	int g = -40, z;
	if (wc < 0) return 0;
	if (!(node->status & IS_WHITE_ON_MOVE))
	{	//printf("zwart locking, zwart on move\n");
		// we are on move, but we want to know if white can't break out, so:
		generate_limited_moves(node, &ws, node->status & IS_WHITE_ON_MOVE, (~own_mask & BITS50), 1, 1);
		if (ws.nr_of_legal_moves < 3) // nr of white moves is too little, we try them all
		{	//printf("zwart locking, own moves=%d\n", ws.nr_of_legal_moves);
			for (i=0; i<ws.nr_of_legal_moves; i++)
			{	populate_node(&ws, &next_node, i, node);
				z = locked_color_on_move(&next_node, other_mask, own_mask, 0);
				if (z > g) g = z;
			}
			//printf("zwart locking, on move, moved first, return=%d\n", g);
			if (g > 0) return 1; else return 0;
			//if (g > 0) return wc; else return 0;
		}
		else
		{	// we assume that there is at least one white move that does not break the "lock"
			next_node.occ_empty = node->occ_empty;
			next_node.man_king = node->man_king;
			next_node.white_black = node->white_black;
			next_node.status = ~node->status & (IS_WHITE_ON_MOVE | IS_MAX_NODE); // invert these bits
			g = locked_color_on_move(&next_node, other_mask, own_mask, 0);
			//printf("zwart locking, on move, dummy move, return=%d\n", g);
			if (g > 0) return 1; else return 0;
			//if (g > 0) return wc; else return 0;
		}
	}
	else
	{	g = locked_color_on_move(node, other_mask, own_mask, 0);
		if (g > 0) return 1; else return 0;
		//printf("zwart locking, not on move, return=%d\n", g);
		//if (g > 0) return wc; else return 0;
	}
}

int freedom(char color_white, BitArray empty_field, BitArray own_man, BitArray other_man)
{	BitArray temp, temp2, cor_own_man;
//	unsigned long bit_list[21];
//	int nr_of_bits;
//	unsigned char iidx;
//	unsigned char idx;
	int free_field = 0;
	//print_fields(own_man);
// prevent capture to the just emptied location
	cor_own_man = own_man & ~((nw_nw(other_man) & nw(own_man)));
	cor_own_man = cor_own_man & ~((ne_ne(other_man) & ne(own_man)));
	cor_own_man = cor_own_man & ~((sw_sw(other_man) & sw(own_man)));
	cor_own_man = cor_own_man & ~((se_se(other_man) & se(own_man)));
	//print_fields(cor_own_man);

	if (color_white)
	{	temp = empty_field & nw(cor_own_man);
		temp = temp & ~(se(other_man));
		temp = temp & ~(sw(other_man) & ne(empty_field));
		temp = temp & ~(ne(other_man) & sw(empty_field));

		if (temp)
		{	free_field = free_field + __builtin_popcountll(temp);
			if ((temp & FLD44) && (other_man & FLD45)) free_field--;
		}

		temp2 = empty_field & ne(cor_own_man);
		temp2 = temp2 & ~(sw(other_man));
		temp2 = temp2 & ~(se(other_man) & nw(empty_field));
		temp2 = temp2 & ~(nw(other_man) & se(empty_field));

		if (temp2)
		{	free_field = free_field + __builtin_popcountll(temp2);
		if ((temp2 & FLD42) && (own_man & FLD47) && (other_man & FLD36) && (empty_field & FLD46) && (empty_field & FLD41)) free_field--;
		if ((temp2 & FLD32) && (own_man & FLD37) && (other_man & FLD36) && (empty_field & FLD46) && (empty_field & FLD41) && (empty_field & FLD47)) free_field--;
		}

		//printf("wit vrije zetten: %d\n", free_field);
		//print_fields(temp2);
	}
	else //color == black
	{	temp2 = empty_field & sw(cor_own_man);
		temp2 = temp2 & ~(ne(other_man));
		temp2 = temp2 & ~(se(other_man) & nw(empty_field));
		temp2 = temp2 & ~(nw(other_man) & se(empty_field));

		if (temp2)
		{	free_field = free_field + __builtin_popcountll(temp2);
		if ((temp2 & BLD42) && (own_man & BLD47) && (other_man & BLD36) && (empty_field & BLD46) && (empty_field & BLD41)) free_field--;
		if ((temp2 & BLD32) && (own_man & BLD37) && (other_man & BLD36) && (empty_field & BLD46) && (empty_field & BLD41) && (empty_field & BLD47)) free_field--;
		}

		temp = empty_field & se(cor_own_man);
		temp = temp & ~(nw(other_man));
		temp = temp & ~(sw(other_man) & ne(empty_field));
		temp = temp & ~(ne(other_man) & sw(empty_field));

		if (temp)
		{	free_field = free_field + __builtin_popcountll(temp);
			if ((temp & BLD44) && (other_man & BLD45)) free_field--;
		}

		//printf("zwart vrije zetten: %d\n", free_field);
		//print_fields(temp2);
	}
	return free_field;
}

int other_side_was_earlier_on_capture(NodePnt node){
	NodePnt prev_node = node->previous_node;
	if (prev_node == NULL) return 0;
	prev_node = prev_node->previous_node;
	if (prev_node == NULL) return 0;
	return !(other_side_is_not_on_capture(prev_node));
}

int other_side_is_not_on_capture (NodePnt node)
{	BitArray temp, empty_field, own_man, own_king, other_piece, moving_piece;

	/* initialize variables */
	empty_field = ~(node->occ_empty) & BITS50;

	if (!(node->status & IS_WHITE_ON_MOVE)) //other side
	{	own_man     = node->occ_empty &  (node->white_black) &   node->man_king;
		own_king    = node->occ_empty &  (node->white_black) & ~(node->man_king);
		other_piece = node->occ_empty & ~(node->white_black);
	}
	else
	{	own_man     = node->occ_empty & ~(node->white_black) &   node->man_king;
		own_king    = node->occ_empty & ~(node->white_black) & ~(node->man_king);
		other_piece = node->occ_empty &  (node->white_black);
	}



	// first check the possible captures with a man

	if ((own_man & se(other_piece) & se_se(empty_field))) return 0;
	if ((own_man & sw(other_piece) & sw_sw(empty_field))) return 0;
	if ((own_man & ne(other_piece) & ne_ne(empty_field))) return 0;
	if ((own_man & nw(other_piece) & nw_nw(empty_field))) return 0;

	// all possible captures with man are done

	// we will look now for captures with a king
	temp = own_king;

	while(temp) {
		moving_piece = temp ^ (temp & (temp - 1)); temp = (temp & (temp - 1));
		if (is_cap_king(other_piece, empty_field, moving_piece)) return 0;
	}
	return 1;
}
