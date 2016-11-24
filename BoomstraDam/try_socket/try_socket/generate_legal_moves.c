/*
  This file is Copyright (C) 2010 Feike Boomstra.
  Distributed under the Boost Software License, Version 1.0.
  (See accompanying file LICENSE_1_0.txt or copy at
  http://www.boost.org/LICENSE_1_0.txt)
*/


#include "BoomstraDam.v1.2.h"

__inline BitArray nw(BitArray org)
{ return (org >> 6); }

__inline BitArray ne(BitArray org)
{ return (org >> 5); }

__inline BitArray sw(BitArray org)
{ return (org << 5); }

__inline BitArray se(BitArray org)
{ return (org << 6);}


void store_move(WsPnt ws, char mystart, char mystop)
{	ws->legal_moves[ws->nr_of_legal_moves].start=mystart; 
	ws->legal_moves[ws->nr_of_legal_moves].stop=mystop;
	ws->legal_moves[ws->nr_of_legal_moves].captured=0;
	ws->legal_moves[ws->nr_of_legal_moves].nr_of_captures=0; 
	ws->nr_of_legal_moves++;
}

void move_king(WsPnt ws, char own_king_index)
{	BitArray own_king_bit = bits[own_king_index];
	BitArray probe;
	char probe_idx;

	// move in nw direction??
	probe = nw(own_king_bit);
	probe_idx = own_king_index + nw_dir;
	while (1)
	{	if (!(probe & ws->empty_field)) break;
		store_move(ws, own_king_index, probe_idx);
		probe = nw(probe);
		probe_idx += nw_dir;
	}
	// move in ne direction??
	probe = ne(own_king_bit);
	probe_idx = own_king_index + ne_dir;
	while (1)
	{	if (!(probe & ws->empty_field)) break;
		store_move(ws, own_king_index, probe_idx);
		probe = ne(probe);
		probe_idx += ne_dir;
	}
	// move in sw direction??
	probe = sw(own_king_bit);
	probe_idx = own_king_index + sw_dir;
	while (1)
	{	if (!(probe & ws->empty_field)) break;
		store_move(ws, own_king_index, probe_idx);
		probe = sw(probe);
		probe_idx += sw_dir;
	}
	// move in se direction??
	probe = se(own_king_bit);
	probe_idx = own_king_index + se_dir;
	while (1)
	{	if (!(probe & ws->empty_field)) break;
		store_move(ws, own_king_index, probe_idx);
		probe = se(probe);
		probe_idx += se_dir;
	}

}


void cap_king(WsPnt ws, char own_king_index, char nr_of_captures, BitArray cur_captured)
{	BitArray own_king_bit = bits[own_king_index];
	BitArray probe;
	char loc_cap_idx;
	char probe_idx;
	// cap in nw direction??
	//fprintf_s(log_file,"King cap in nw\n");
	probe = nw(own_king_bit);
	probe_idx = own_king_index + nw_dir;
	while (1)
	{	if (!(probe & ws->empty_field)) break;
		probe = nw(probe);
		probe_idx += nw_dir;
	}
	// on first not empty field
	if (!(probe & ws->other_piece)) goto N1; // can't capture
	if (cur_captured & probe) goto N1; // is allready captured
	loc_cap_idx = probe_idx; // save loc captured piece
	probe = nw(probe);
	probe_idx += nw_dir; // on next square
	if (!(probe & ws->empty_field)) goto N1; // must be at least one place to land
	while (1)
	{	cur_captured = cur_captured | bits[loc_cap_idx];
		cap_king(ws, probe_idx, nr_of_captures + 1, cur_captured);
		cur_captured = cur_captured & not_bits[loc_cap_idx];
		probe = nw(probe);
		probe_idx += nw_dir; // on next square
		if (!(probe & ws->empty_field)) break; // done here
	}
N1:	//fprintf_s(log_file,"King cap in ne\n");

	// cap in ne direction??
	probe = ne(own_king_bit);
	probe_idx = own_king_index + ne_dir;
	while (1)
	{	if (!(probe & ws->empty_field)) break;
		probe = ne(probe);
		probe_idx += ne_dir;
	}
	// on first not empty field
	if (!(probe & ws->other_piece)) goto N2; // can't capture
	if (cur_captured & probe) goto N2; // is allready captured
	loc_cap_idx = probe_idx; // save loc captured piece
	probe = ne(probe);
	probe_idx += ne_dir; // on next square
	if (!(probe & ws->empty_field)) goto N2; // must be at least one place to land
	while (1)
	{	cur_captured = cur_captured | bits[loc_cap_idx];
		cap_king(ws, probe_idx, nr_of_captures + 1, cur_captured);
		cur_captured = cur_captured & not_bits[loc_cap_idx];
		probe = ne(probe);
		probe_idx += ne_dir; // on next square
		if (!(probe & ws->empty_field)) break; // done here
	}
N2:	//fprintf_s(log_file,"King cap in sw\n");

	// cap in sw direction??
	probe = sw(own_king_bit);
	probe_idx = own_king_index + sw_dir;
	while (1)
	{	if (!(probe & ws->empty_field)) break;
		probe = sw(probe);
		probe_idx += sw_dir;
	}
	// on first not empty field
	if (!(probe & ws->other_piece)) goto N3; // can't capture
	if (cur_captured & probe) goto N3; // is allready captured
	loc_cap_idx = probe_idx; // save loc captured piece
	probe = sw(probe);
	probe_idx += sw_dir; // on next square
	if (!(probe & ws->empty_field)) goto N3; // must be at least one place to land
	while (1)
	{	cur_captured = cur_captured | bits[loc_cap_idx];
		cap_king(ws, probe_idx, nr_of_captures + 1, cur_captured);
		cur_captured = cur_captured & not_bits[loc_cap_idx];
		probe = sw(probe);
		probe_idx += sw_dir; // on next square
		if (!(probe & ws->empty_field)) break; // done here
	}
N3:	//fprintf_s(log_file,"King cap in se\n");

	// cap in se direction??
	probe = se(own_king_bit);
	probe_idx = own_king_index + se_dir;
	while (1)
	{	if (!(probe & ws->empty_field)) break;
		probe = se(probe);
		probe_idx += se_dir;
	}
	// on first not empty field
	if (!(probe & ws->other_piece)) goto N4; // can't capture
	if (cur_captured & probe) goto N4; // is allready captured
	loc_cap_idx = probe_idx; // save loc captured piece
	probe = se(probe);
	probe_idx += se_dir; // on next square
	if (!(probe & ws->empty_field)) goto N4; // must be at least one place to land
	while (1)
	{	cur_captured = cur_captured | bits[loc_cap_idx];
		cap_king(ws, probe_idx, nr_of_captures + 1, cur_captured);
		cur_captured = cur_captured & not_bits[loc_cap_idx];
		probe = se(probe);
		probe_idx += se_dir; // on next square
		if (!(probe & ws->empty_field)) break; // done here
	}
N4:	//fprintf_s(log_file,"King cap all done\n");
// all done

	if (nr_of_captures > 0)
	{	if (nr_of_captures >= ws->max_nr_of_captures)												/* at least we should save this move */
		{	int not_found = 1;
			int i;
			if (nr_of_captures > ws->max_nr_of_captures)											/* reset the nr_of_legal_moves */
			{	ws->max_nr_of_captures = nr_of_captures ;											/* update the current maximum */
				ws->nr_of_legal_moves = 0;	
			};																						/* and start over again */
			ws->legal_moves[ws->nr_of_legal_moves].stop = own_king_index;							/* from here there were no more captures */
			ws->legal_moves[ws->nr_of_legal_moves].start = ws->current_start_position;
			ws->legal_moves[ws->nr_of_legal_moves].nr_of_captures = ws->max_nr_of_captures ;		/* for later use  */
			ws->legal_moves[ws->nr_of_legal_moves].captured = cur_captured;							/* to be able to calculate the new board position */
			for (i=0; i < ws->nr_of_legal_moves; i++)												/* check whether we have a "new" capture */
			{	if ((ws->legal_moves[ws->nr_of_legal_moves].captured == ws->legal_moves[i].captured) && \
					(ws->legal_moves[ws->nr_of_legal_moves].start == ws->legal_moves[i].start) && \
					(ws->legal_moves[ws->nr_of_legal_moves].stop == ws->legal_moves[i].stop))
				{	not_found = 0; break; }
			}
			if (not_found) ws->nr_of_legal_moves++;													/* and save */
		};
	}
	return;
}

void cap_man(WsPnt ws, char own_man_index, char nr_of_captures, BitArray cur_captured)
{	// check all directions
	BitArray own_man_bit = bits[own_man_index];
	BitArray temp;
//	int nr_of_bits;
//	unsigned long bit_list[20];

	// capture in nw direction??
	if (!(temp = (own_man_bit & se(ws->other_piece)))) goto N1;
	if (!(temp = (temp & se(se(ws->empty_field))))) goto N1;
	if (cur_captured & nw(temp)) goto N1;
	cur_captured = cur_captured | bits[own_man_index + nw_dir];
	cap_man(ws, own_man_index + nw_dir + nw_dir, nr_of_captures + 1, cur_captured);			/* capture with the man */
	cur_captured = cur_captured & not_bits[own_man_index + nw_dir];

N1:	// capture in ne direction??
	if (!(temp = (own_man_bit & sw(ws->other_piece)))) goto N2;
	if (!(temp = (temp & sw(sw(ws->empty_field))))) goto N2;
	if (cur_captured & ne(temp)) goto N2;
	cur_captured = cur_captured | bits[own_man_index + ne_dir];
	cap_man(ws, own_man_index + ne_dir + ne_dir, nr_of_captures + 1, cur_captured);			/* capture with the man */
	cur_captured = cur_captured & not_bits[own_man_index + ne_dir];

N2:	// capture in sw direction??
	if (!(temp = (own_man_bit & ne(ws->other_piece)))) goto N3;
	if (!(temp = (temp & ne(ne(ws->empty_field))))) goto N3;
	if (cur_captured & sw(temp)) goto N3;
	cur_captured = cur_captured | bits[own_man_index + sw_dir];
	cap_man(ws, own_man_index + sw_dir + sw_dir, nr_of_captures + 1, cur_captured);			/* capture with the man */
	cur_captured = cur_captured & not_bits[own_man_index + sw_dir];

N3:	// capture in se direction??
	if (!(temp = (own_man_bit & nw(ws->other_piece)))) goto N4;
	if (!(temp = (temp & nw(nw(ws->empty_field))))) goto N4;
	if (cur_captured & se(temp)) goto N4;
	cur_captured = cur_captured | bits[own_man_index + se_dir];
	cap_man(ws, own_man_index + se_dir + se_dir, nr_of_captures + 1, cur_captured);			/* capture with the man */
	cur_captured = cur_captured & not_bits[own_man_index + se_dir];

N4: // all done

	if (nr_of_captures > 0)
	{	if (nr_of_captures >= ws->max_nr_of_captures)												/* at least we should save this move */
		{	int not_found = 1;
			int i;
			if (nr_of_captures > ws->max_nr_of_captures)											/* reset the nr_of_legal_moves */
			{	ws->max_nr_of_captures = nr_of_captures ;											/* update the current maximum */
				ws->nr_of_legal_moves = 0;	
			};																						/* and start over again */
			ws->legal_moves[ws->nr_of_legal_moves].stop = own_man_index;							/* from here there were no more captures */
			ws->legal_moves[ws->nr_of_legal_moves].start = ws->current_start_position;
			ws->legal_moves[ws->nr_of_legal_moves].nr_of_captures = ws->max_nr_of_captures ;		/* for later use  */
			ws->legal_moves[ws->nr_of_legal_moves].captured = cur_captured;							/* to be able to calculate the new board position */
			for (i=0; i < ws->nr_of_legal_moves; i++)												/* check whether we have a "new" capture */
			{	if ((ws->legal_moves[ws->nr_of_legal_moves].captured == ws->legal_moves[i].captured) && \
					(ws->legal_moves[ws->nr_of_legal_moves].start == ws->legal_moves[i].start) && \
					(ws->legal_moves[ws->nr_of_legal_moves].stop == ws->legal_moves[i].stop))
				{	not_found = 0; break; }
			}
			if (not_found) ws->nr_of_legal_moves++;													/* and save */
		};
	}
	return;


}


void generate_legal_moves(NodePnt node, WsPnt ws, char color_white)
{	BitArray temp;
	unsigned long bit_list[21];
	unsigned long king_list[21];
	int nr_of_bits;
	int nr_of_kings;
	BitArray cur_captured;
	unsigned char iidx;
	unsigned char idx;
	//fprintf_s(log_file,"Generate legal moves\n");
	ws->empty_field = ~(node->occ_empty) & BITS50;
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
	//printf("\n");
	//print_64(FLD1 | FLD2| FLD3| FLD4| FLD5| FLD6| FLD7| FLD8| FLD9| FLD10| FLD11 | FLD12| FLD13| FLD14| FLD15| FLD16| FLD17| FLD18| FLD19| FLD20); printf("\n");
	//print_64(BITS50); printf("\n");
	//print_64(sw(ws->other_piece)); printf("\n");
	//print_64(sw(sw(ws->empty_field)) & ws->own_man);printf("\n");
	//printf("\n");
	/* initialize (global) variables */

	ws->nr_of_legal_moves = 0;
	ws->max_nr_of_captures = 0;

	// first check the possible captures with a man
	
	if (!(temp = (ws->own_man & se(ws->other_piece)))) goto N1;
	if (!(temp = (temp & se(se(ws->empty_field))))) goto N1;
	// in temp the start bits for a capture in nw direction
	count_bits_and_fill_list(temp, nr_of_bits, bit_list);
	for (iidx=0; iidx < nr_of_bits; iidx++)
	{	idx = (unsigned char) bit_list[iidx];
		ws->empty_field = ws->empty_field | bits[idx];	/* set it temporary free */
		ws->current_start_position = idx ;			/* to keep the start position */
		cur_captured = bits[idx + nw_dir];
		cap_man(ws, idx + nw_dir + nw_dir, 1, cur_captured);			/* capture with the man */
		ws->empty_field = ws->empty_field & not_bits[idx];	
	};
N1:	//fprintf_s(log_file,"Legal moves N1\n");
	if (!(temp = (ws->own_man & sw(ws->other_piece)))) goto N2;
	if (!(temp = (temp & sw(sw(ws->empty_field))))) goto N2;
	// in temp the start bits for a capture in ne direction
	count_bits_and_fill_list(temp, nr_of_bits, bit_list);
	for (iidx=0; iidx < nr_of_bits; iidx++)
	{	idx = (unsigned char) bit_list[iidx];
		ws->empty_field = ws->empty_field | bits[idx];	/* set it temporary free */
		ws->current_start_position = idx ;			/* to keep the start position */
		cur_captured = bits[idx + ne_dir];
		cap_man(ws, idx + ne_dir + ne_dir, 1, cur_captured);			/* capture with the man */
		ws->empty_field = ws->empty_field & not_bits[idx];	
	};
N2:	//fprintf_s(log_file,"Legal moves N2\n");
	if (!(temp = (ws->own_man & ne(ws->other_piece)))) goto N3;
	if (!(temp = (temp & ne(ne(ws->empty_field))))) goto N3;
	// in temp the start bits for a capture in sw direction
	count_bits_and_fill_list(temp, nr_of_bits, bit_list);
	for (iidx=0; iidx < nr_of_bits; iidx++)
	{	idx = (unsigned char) bit_list[iidx];
		ws->empty_field = ws->empty_field | bits[idx];	/* set it temporary free */
		ws->current_start_position = idx ;			/* to keep the start position */
		cur_captured = bits[idx + sw_dir];
		cap_man(ws, idx + sw_dir + sw_dir, 1, cur_captured);			/* capture with the man */
		ws->empty_field = ws->empty_field & not_bits[idx];	
	};
N3:	//fprintf_s(log_file,"Legal moves N3\n");
	if (!(temp = (ws->own_man & nw(ws->other_piece)))) goto N4;
	if (!(temp = (temp & nw(nw(ws->empty_field))))) goto N4;
	// in temp the start bits for a capture in se direction
	count_bits_and_fill_list(temp, nr_of_bits, bit_list);
	for (iidx=0; iidx < nr_of_bits; iidx++)
	{	idx = (unsigned char) bit_list[iidx];
		ws->empty_field = ws->empty_field | bits[idx];	/* set it temporary free */
		ws->current_start_position = idx ;			/* to keep the start position */
		cur_captured = bits[idx + se_dir];
		cap_man(ws, idx + se_dir + se_dir, 1, cur_captured);			/* capture with the man */
		ws->empty_field = ws->empty_field & not_bits[idx];	
	};
N4: //fprintf_s(log_file,"Legal moves N4\n");
	// all possible captures with man are done
	// we will look now for captures with a king
	if (ws->own_king)
	{	count_bits_and_fill_list(ws->own_king, nr_of_kings, king_list);
		for (iidx=0; iidx < nr_of_kings; iidx++)
		{	idx = (unsigned char) king_list[iidx];
			ws->empty_field = ws->empty_field | bits[idx];	/* set it temporary free */
			ws->current_start_position = idx ;			/* to keep the start position */
			cur_captured = 0;
			cap_king(ws, idx, 0, cur_captured);			/* capture with the king */
			ws->empty_field = ws->empty_field & not_bits[idx];	
		};
	}
	//fprintf_s(log_file,"Legal moves N5\n");
	
	if (ws->max_nr_of_captures > 0) return; // done

	// no captures, try moves:

	if (color_white)
	{	temp = se(ws->empty_field) & ws->own_man;
		if (temp)
		{	count_bits_and_fill_list(temp, nr_of_bits, bit_list);
			for (iidx=0; iidx < nr_of_bits; iidx++)
			{	idx = (unsigned char) bit_list[iidx];
				store_move(ws, idx, idx + nw_dir);
			}
		}

		temp = sw(ws->empty_field) & ws->own_man;
		if (temp)
		{	count_bits_and_fill_list(temp, nr_of_bits, bit_list);
			for (iidx=0; iidx < nr_of_bits; iidx++)
			{	idx = (unsigned char) bit_list[iidx];
				store_move(ws, idx, idx + ne_dir);
			}
		}
	}
	else //color == black
	{	temp = ne(ws->empty_field) & ws->own_man;
		if (temp)
		{	count_bits_and_fill_list(temp, nr_of_bits, bit_list);
			for (iidx=0; iidx < nr_of_bits; iidx++)
			{	idx = (unsigned char) bit_list[iidx];
				store_move(ws, idx, idx + sw_dir);
			}
		}
		temp = nw(ws->empty_field) & ws->own_man;
		if (temp)
		{	count_bits_and_fill_list(temp, nr_of_bits, bit_list);
			for (iidx=0; iidx < nr_of_bits; iidx++)
			{	idx = (unsigned char) bit_list[iidx];
				store_move(ws, idx, idx + se_dir);
			}
		}
	}
	// and finally the moves with a king
	if (ws->own_king)
	{
		for (iidx=0; iidx < nr_of_kings; iidx++)
		{	idx = (unsigned char) king_list[iidx];
			move_king(ws, idx);
		}
	}
	return;
}

unsigned char generate_limited_moves(NodePnt node, WsPnt ws, char color_white, BitArray mask, char include_moves, char include_captures)
{		
	BitArray temp;
	unsigned long bit_list[21];
	unsigned long king_list[21];
	int nr_of_bits;
	int nr_of_kings = 0;
	BitArray cur_captured;
	unsigned char iidx;
	unsigned char idx;
	//fprintf_s(log_file,"Generate legal moves\n");
	ws->empty_field = ~(node->occ_empty) & BITS50;
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
	{	if (!(temp = (ws->own_man & se(ws->other_piece)))) goto N1;
		if (!(temp = (temp & se(se(ws->empty_field))))) goto N1;
		// in temp the start bits for a capture in nw direction
		count_bits_and_fill_list(temp, nr_of_bits, bit_list);
		for (iidx=0; iidx < nr_of_bits; iidx++)
		{	idx = (unsigned char) bit_list[iidx];
			ws->empty_field = ws->empty_field | bits[idx];	/* set it temporary free */
			ws->current_start_position = idx ;			/* to keep the start position */
			cur_captured = bits[idx + nw_dir];
			cap_man(ws, idx + nw_dir + nw_dir, 1, cur_captured);			/* capture with the man */
			ws->empty_field = ws->empty_field & not_bits[idx];	
		};
	N1:	//fprintf_s(log_file,"Legal moves N1\n");
		if (!(temp = (ws->own_man & sw(ws->other_piece)))) goto N2;
		if (!(temp = (temp & sw(sw(ws->empty_field))))) goto N2;
		// in temp the start bits for a capture in ne direction
		count_bits_and_fill_list(temp, nr_of_bits, bit_list);
		for (iidx=0; iidx < nr_of_bits; iidx++)
		{	idx = (unsigned char) bit_list[iidx];
			ws->empty_field = ws->empty_field | bits[idx];	/* set it temporary free */
			ws->current_start_position = idx ;			/* to keep the start position */
			cur_captured = bits[idx + ne_dir];
			cap_man(ws, idx + ne_dir + ne_dir, 1, cur_captured);			/* capture with the man */
			ws->empty_field = ws->empty_field & not_bits[idx];	
		};
	N2:	//fprintf_s(log_file,"Legal moves N2\n");
		if (!(temp = (ws->own_man & ne(ws->other_piece)))) goto N3;
		if (!(temp = (temp & ne(ne(ws->empty_field))))) goto N3;
		// in temp the start bits for a capture in sw direction
		count_bits_and_fill_list(temp, nr_of_bits, bit_list);
		for (iidx=0; iidx < nr_of_bits; iidx++)
		{	idx = (unsigned char) bit_list[iidx];
			ws->empty_field = ws->empty_field | bits[idx];	/* set it temporary free */
			ws->current_start_position = idx ;			/* to keep the start position */
			cur_captured = bits[idx + sw_dir];
			cap_man(ws, idx + sw_dir + sw_dir, 1, cur_captured);			/* capture with the man */
			ws->empty_field = ws->empty_field & not_bits[idx];	
		};
	N3:	//fprintf_s(log_file,"Legal moves N3\n");
		if (!(temp = (ws->own_man & nw(ws->other_piece)))) goto N4;
		if (!(temp = (temp & nw(nw(ws->empty_field))))) goto N4;
		// in temp the start bits for a capture in se direction
		count_bits_and_fill_list(temp, nr_of_bits, bit_list);
		for (iidx=0; iidx < nr_of_bits; iidx++)
		{	idx = (unsigned char) bit_list[iidx];
			ws->empty_field = ws->empty_field | bits[idx];	/* set it temporary free */
			ws->current_start_position = idx ;			/* to keep the start position */
			cur_captured = bits[idx + se_dir];
			cap_man(ws, idx + se_dir + se_dir, 1, cur_captured);			/* capture with the man */
			ws->empty_field = ws->empty_field & not_bits[idx];	
		}
	
N4:	
		if (ws->own_king)
		{	count_bits_and_fill_list(ws->own_king, nr_of_kings, king_list);
			for (iidx=0; iidx < nr_of_kings; iidx++)
			{	idx = (unsigned char) king_list[iidx];
				ws->empty_field = ws->empty_field | bits[idx];	/* set it temporary free */
				ws->current_start_position = idx ;			/* to keep the start position */
				cur_captured = 0;
				cap_king(ws, idx, 0, cur_captured);			/* capture with the king */
				ws->empty_field = ws->empty_field & not_bits[idx];	
			};
		}
	}
	if (ws->max_nr_of_captures > 0) 
	{	if (color_white)
			ws->own_man = node->occ_empty & node->white_black & node->man_king;
		else
			ws->own_man = node->occ_empty & ~(node->white_black) & node->man_king;
	
		return ws->max_nr_of_captures; // done
	}

	// no captures, try moves:

	if (include_moves)
	{	if (color_white)
		{	temp = se(ws->empty_field) & ws->own_man;
			if (temp)
			{	count_bits_and_fill_list(temp, nr_of_bits, bit_list);
				for (iidx=0; iidx < nr_of_bits; iidx++)
				{	idx = (unsigned char) bit_list[iidx];
					store_move(ws, idx, idx + nw_dir);
				}
			}

			temp = sw(ws->empty_field) & ws->own_man;
			if (temp)
			{	count_bits_and_fill_list(temp, nr_of_bits, bit_list);
				for (iidx=0; iidx < nr_of_bits; iidx++)
				{	idx = (unsigned char) bit_list[iidx];
					store_move(ws, idx, idx + ne_dir);
				}
			}
		}
		else //color == black
		{	temp = ne(ws->empty_field) & ws->own_man;
			if (temp)
			{	count_bits_and_fill_list(temp, nr_of_bits, bit_list);
				for (iidx=0; iidx < nr_of_bits; iidx++)
				{	idx = (unsigned char) bit_list[iidx];
					store_move(ws, idx, idx + sw_dir);
				}
			}
			temp = nw(ws->empty_field) & ws->own_man;
			if (temp)
			{	count_bits_and_fill_list(temp, nr_of_bits, bit_list);
				for (iidx=0; iidx < nr_of_bits; iidx++)
				{	idx = (unsigned char) bit_list[iidx];
					store_move(ws, idx, idx + se_dir);
				}
			}
		}
	
		// and finally the moves with a king
		if (ws->own_king)
		{
			for (iidx=0; iidx < nr_of_kings; iidx++)
			{	idx = (unsigned char) king_list[iidx];
				move_king(ws, idx);
			}
		}
	} // include moves
	// complete own_man for populate
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
		

char saldo(NodePnt node, char color_is_white)
{	struct piece_count_record cur_piece_count;
	calc_piece_count(node, &cur_piece_count);
	if (color_is_white)
		return (cur_piece_count.nr_of_white_man - org_piece_count.nr_of_white_man) - (cur_piece_count.nr_of_black_man - org_piece_count.nr_of_black_man)
		+ 3*(cur_piece_count.nr_of_white_king - cur_piece_count.nr_of_black_king); // no king in the beginning, maybe now??
	else
		return (cur_piece_count.nr_of_black_man - org_piece_count.nr_of_black_man) - (cur_piece_count.nr_of_white_man - org_piece_count.nr_of_white_man)
		+ 3*(cur_piece_count.nr_of_black_king - cur_piece_count.nr_of_white_king); // no king in the beginning, maybe now??
}

char locking_color_on_move_continue(NodePnt, BitArray, BitArray, char, char);

char locked_color_on_move_continue(NodePnt node, BitArray min_mask, BitArray max_mask, char level, char color_is_white)
{	struct work_space_move_generator ws;
	struct tnode next_node;
	int i, z;
	int g = 40;


	generate_limited_moves(node, &ws, node->status & IS_WHITE_ON_MOVE, BITS50, 0, 1);   // check captures only

	if (ws.max_nr_of_captures > 0)
	{	for (i=0; i<ws.nr_of_legal_moves; i++)
		{	populate_node(&ws, &next_node, i, node);
			z = locking_color_on_move_continue(&next_node, min_mask, max_mask, level, color_is_white);
			if (z < g) g = z;
		}
	}
	else
	{	// there are no captures, so we limit the moves to "mask"
		if (level <= 0) { char res = saldo(node, color_is_white); /*print_val_l(res, &ws, best_move, display_level);*/ return res; } ;
		generate_limited_moves(node, &ws, node->status & IS_WHITE_ON_MOVE, min_mask, 1, 0);
		if (ws.nr_of_legal_moves == 0) return saldo(node, color_is_white); 
		for (i=0; i<ws.nr_of_legal_moves; i++)
		{	populate_node(&ws, &next_node, i, node);
			z = locking_color_on_move_continue(&next_node, min_mask, max_mask, level - 1, color_is_white);
			if (z < g) g = z;
		}
	}
	return g;
}


char locking_color_on_move_continue(NodePnt node, BitArray other_mask, BitArray own_mask, char level, char color_is_white)
{	struct work_space_move_generator ws;
	struct tnode next_node;
	int i, z;
	int g = -40;
	
	generate_limited_moves(node, &ws, node->status & IS_WHITE_ON_MOVE, BITS50, 0, 1);  // is there a capture

	if (ws.max_nr_of_captures > 0)
	{	for (i=0; i<ws.nr_of_legal_moves; i++)
		{	populate_node(&ws, &next_node, i, node);
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
			z = locked_color_on_move_continue(&next_node, other_mask, own_mask, level - 1, color_is_white);
			if (z > g) g = z;
		}
	}
	return g;
}

char locked_color_on_move(NodePnt node, BitArray other_mask, BitArray own_mask, char color_is_white)
{	struct work_space_move_generator ws;
	struct tnode next_node;
	int i, z;
	int g = 40;
	
	// the node has the opposite color to the color we are testing for.
	// the function saldo returns the difference in number of pieces compared to the
	// starting position, seen through the eyes of the other color

	generate_limited_moves(node, &ws, node->status & IS_WHITE_ON_MOVE, other_mask, 1, 1);
	if (ws.nr_of_legal_moves == 0) 
	{	return 1; // no break out
	}
	for (i=0; i<ws.nr_of_legal_moves; i++)
	{	populate_node(&ws, &next_node, i, node);
		z = locking_color_on_move_continue(&next_node, other_mask, own_mask, 1, color_is_white);
		if (z < g) g = z;
	}
	return g;
}

char white_is_locking_pieces(NodePnt node, char wc, BitArray other_mask, BitArray own_mask)
{	struct work_space_move_generator ws;
	struct tnode next_node;
	int i, z;
	int g = -40;
	if (node->status & IS_WHITE_ON_MOVE)
	{	// white is on move, but we want to know if black can't break out, so:
		generate_limited_moves(node, &ws, node->status & IS_WHITE_ON_MOVE, (~own_mask & BITS50), 1, 1);
		if (ws.nr_of_legal_moves < 3) // nr of white moves is too little, we try them all
		{	for (i=0; i<ws.nr_of_legal_moves; i++)
			{	populate_node(&ws, &next_node, i, node);
				//print_move_l(node, &ws, i, 0);
				z = locked_color_on_move(&next_node, other_mask, own_mask, 1);
				if (z > g) g = z;
			}
			if (g > 0) return wc; else return 0;
		}
		else
		{	// we assume that there is at least one white move that does not break the "lock"
			next_node.occ_empty = node->occ_empty;
			next_node.man_king = node->man_king;
			next_node.white_black = node->white_black;
			next_node.status = ~node->status & (IS_WHITE_ON_MOVE | IS_MAX_NODE); // invert these bits
			g = locked_color_on_move(&next_node, other_mask, own_mask, 1);
			if (g > 0) return wc; else return 0;
		}
	}
	else
	{	g = locked_color_on_move(node, other_mask, own_mask, 1);
		if (g > 0) return wc; else return 0;
	}
}

char black_is_locking_pieces(NodePnt node, char wc, BitArray other_mask, BitArray own_mask)
{	struct work_space_move_generator ws;
	struct tnode next_node;
	int i, z;
	int g = -40;
	if (wc <=0) return 0;
	if (!(node->status & IS_WHITE_ON_MOVE))
	{	// we are on move, but we want to know if black can't break out, so:
		generate_limited_moves(node, &ws, node->status & IS_WHITE_ON_MOVE, (~own_mask & BITS50), 1, 1);
		if (ws.nr_of_legal_moves < 3) // nr of white moves is too little, we try them all
		{	for (i=0; i<ws.nr_of_legal_moves; i++)
			{	populate_node(&ws, &next_node, i, node);
				z = locked_color_on_move(&next_node, other_mask, own_mask, 0);
				if (z > g) g = z;
			}
			if (g > 0) return wc; else return 0;
		}
		else
		{	// we assume that there is at least one white move that does not break the "lock"
			next_node.occ_empty = node->occ_empty;
			next_node.man_king = node->man_king;
			next_node.white_black = node->white_black;
			next_node.status = ~node->status & (IS_WHITE_ON_MOVE | IS_MAX_NODE); // invert these bits
			g = locked_color_on_move(&next_node, other_mask, own_mask, 0);
			if (g > 0) return wc; else return 0;
		}
	}
	else
	{	g = locked_color_on_move(node, other_mask, own_mask, 0);
		if (g > 0) return wc; else return 0;
	}
}
int freedom(char color_white, BitArray empty_field, BitArray own_man, BitArray other_man)
{	BitArray temp;
	unsigned long bit_list[21];
	int nr_of_bits;
	unsigned char iidx;
	unsigned char idx;
	int free_field = 0;

	if (color_white)
	{	temp = empty_field & nw(own_man);
		if (temp)
		{	count_bits_and_fill_list(temp, nr_of_bits, bit_list);
			for (iidx=0; iidx < nr_of_bits; iidx++)
			{	idx = (unsigned char) bit_list[iidx];
				temp = bits[idx];
				if (!(nw(temp) & other_man) && !(((ne(temp) & other_man) && (sw(temp) & empty_field)) || ((sw(temp) & other_man) && (ne(temp) & empty_field))))
					free_field++;
			}
		}
	
		temp = empty_field & ne(own_man);
		if (temp)
		{	count_bits_and_fill_list(temp, nr_of_bits, bit_list);
			for (iidx=0; iidx < nr_of_bits; iidx++)
			{	idx = (unsigned char) bit_list[iidx];
				temp = bits[idx];
				if (!(ne(temp) & other_man) && !(((nw(temp) & other_man) && (se(temp) & empty_field)) || ((se(temp) & other_man) && (nw(temp) & empty_field))))
					free_field++;
			}
		}
	}
	else //color == black
	{	temp = empty_field & sw(own_man);
		if (temp)
		{	count_bits_and_fill_list(temp, nr_of_bits, bit_list);
			for (iidx=0; iidx < nr_of_bits; iidx++)
			{	idx = (unsigned char) bit_list[iidx];
				temp = bits[idx];
				if (!(sw(temp) & other_man) && !(((nw(temp) & other_man) && (se(temp) & empty_field)) || ((se(temp) & other_man) && (nw(temp) & empty_field))))
					free_field++;
			}
		}
		temp = empty_field & se(own_man);
		if (temp)
		{	count_bits_and_fill_list(temp, nr_of_bits, bit_list);
			for (iidx=0; iidx < nr_of_bits; iidx++)
			{	idx = (unsigned char) bit_list[iidx];
				temp = bits[idx];
				if (!(se(temp) & other_man) && !(((ne(temp) & other_man) && (sw(temp) & empty_field)) || ((sw(temp) & other_man) && (ne(temp) & empty_field))))
					free_field++;
			}
		}
	}
	return free_field;
}