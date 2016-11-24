/*
  This file is Copyright (C) 2010 Feike Boomstra.
  Distributed under the Boost Software License, Version 1.0.
  (See accompanying file LICENSE_1_0.txt or copy at
  http://www.boost.org/LICENSE_1_0.txt)
*/


#include "BoomstraDam.v2.5.h"
#include "eval.v2.4.h"

/* We try to calculate the compactness of the position             *
 * We do this by counting the number of connections for each piece *
 * A connection is as there is a neighbour of the same color       *
 */

BitArray conn_mask[55] = {
		w2(6,7),		w2(7,8),			w2(8,9),			w2(9,10),			0,
		w2( 1,11),		w4( 1,11, 2,12),	w4( 2,12, 3,13),	w4( 3,13, 4,14),	w4( 4,14, 5,15),	0,
		w4( 6,16, 7,17),w4( 7,17, 8,18),	w4( 8,18, 9,19),	w4( 9,19,10,20),	w2(10,20),
		w2(11,21),		w4(11,21,12,22),	w4(12,22,13,23),	w4(13,23,14,24),	w4(14,24,15,25),	0,
		w4(16,26,17,27),w4(17,27,18,28),	w4(18,28,19,29),	w4(19,29,20,30),	w2(20,30),
		w2(21,31),		w4(21,31,22,32),	w4(22,32,23,33),	w4(23,33,24,34),	w4(24,34,25,35),	0,
		w4(26,36,27,37),w4(27,37,28,38),	w4(28,38,29,39),	w4(29,39,30,40),	w2(30,40),
		w2(31,41),		w4(31,41,32,42),	w4(32,42,33,43),	w4(33,43,34,44),	w4(34,44,35,45),	0,
		w4(36,46,37,47),w4(37,47,38,48),	w4(38,48,39,49),	w4(39,49,40,50),	w2(40,50),
		0,				w2(41,42),			w2(42,43),			w2(43,44),			w2(44,45),			0
};

BitArray white_conn_mask[55] = {
							0,					0,						0,						0,						0,
					0,					w2(11,12),			w2(12,13),				w2(13,14),				w2(14,15)		,		0,
							w2(16,17),			w2(17,18),				w2(18,19),				w2(19,20),				0,
					0,					w2(21,22),			w2(22,23),				w2(23,24),				w2(24,25)		,		0,
							w2(26,27),			w2(27,28),				w2(28,29),				w2(29,30),				0,
					0,					w2(31,32),			w2(32,33),				w2(33,34),				w2(34,35)		,		0,
							w2(36,37),			w2(37,38),				w2(38,39),				w2(39,40),				0,
					0,					w2(41,42),			w2(42,43),				w2(43,44),				w2(44,45)		,		0,
							w2(46,47),			w2(47,48),				w2(48,49),				w2(49,50),				0,
					0,					0,					0,						0,						0,						0
};

BitArray black_conn_mask[55] = {
							0,					0,						0,						0,						0,
					0,					w2( 1, 2),			w2( 2, 3),				w2( 3, 4),				w2( 4, 5)		,		0,
							w2( 6, 7),			w2( 7, 8),				w2( 8, 9),				w2( 9,20),				0,
					0,					w2(11,12),			w2(12,13),				w2(13,14),				w2(14,15)		,		0,
							w2(16,17),			w2(17,18),				w2(18,19),				w2(19,20),				0,
					0,					w2(21,22),			w2(22,23),				w2(23,24),				w2(24,25)		,		0,
							w2(26,27),			w2(27,28),				w2(28,29),				w2(29,30),				0,
					0,					w2(31,32),			w2(32,33),				w2(33,34),				w2(34,35)		,		0,
							w2(36,37),			w2(37,38),				w2(38,39),				w2(39,40),				0,
					0,					0,					0,						0,						0,						0
};

const char compact_weight[13][12] = {
		{0,0,0,0,0,0,0,1,6,19,42,72},
		{0,0,0,0,0,1,4,16,40,78,100,100},
		{0,0,0,0,1,5,15,39,71,100,84,51},
		{0,0,0,0,3,14,32,60,77,100,39,14},
		{0,0,0,2,10,33,62,95,100,100,37,14},
		{0,0,2,9,30,65,88,100,100,100,9,2},
		{0,1,8,25,57,96,100,100,100,100,2,0},
		{0,4,19,50,84,100,100,100,100,100,0,0},
		{1,13,45,81,100,100,100,100,100,100,0,0},
		{7,35,77,100,100,100,100,100,100,100,0,0},
		{19,66,100,100,100,100,100,100,100,100,0,0},
		{51,89,100,100,100,100,100,100,100,100,0,0},
		{81,100,100,100,100,100,100,100,100,100,0,0},
};

int fetch_compact_weight(unsigned char nr_of_man, int count){
	if (nr_of_man > 18 || nr_of_man < 7) return 100;
	if (count > 12) return 100;
	return compact_weight[count][18 - nr_of_man];
}

int white_connections(BitArray white_man, unsigned char nr_of_man){
BitArray piece_probe;
BitArray connection_probe;
int white_connection_counter = 0;

			BitArray front_piece = white_man;
			int front_index = __builtin_ctzll(front_piece);
			printf("voorste witte stuk is: %d\n", int_to_ext_list[front_index]);
			print_bits(front_piece);

			for (int i = int_to_ext_list[front_index]; i < 51; i++){
				piece_probe = bits[ext_to_int[i]];
				if (white_man & piece_probe) {
					connection_probe = white_conn_mask[ext_to_int[i]];
					print_bits(piece_probe);
					print_bits(connection_probe);
					//printf("count: %d\n", count_white(connection_probe));
					if (connection_probe) if (all_white(connection_probe)){
						white_connection_counter++; //printf("white: i = %d\n",i);
						}

				}
			}
			printf("white compact = %d weight = %d\n\n",white_connection_counter, fetch_compact_weight(nr_of_man, white_connection_counter));

			return white_connection_counter;
}

int black_connections(BitArray black_man, unsigned char nr_of_man){
BitArray piece_probe;
BitArray connection_probe;
int black_connection_counter = 0;

			BitArray front_piece = black_man;
			int front_index = 63 - __builtin_clzll(front_piece);
			//printf("voorste zwarte stuk is: %d\n", int_to_ext_list[front_index]);
			//print_bits(front_piece);

			for (int i = int_to_ext_list[front_index]; i >= 0; i--){
				piece_probe = bits[ext_to_int[i]];
				if (black_man & piece_probe) {
					connection_probe = black_conn_mask[ext_to_int[i]];
					//print_bits(piece_probe);
					//print_bits(connection_probe);
					//printf("count: %d\n", count_black(connection_probe));
					if (connection_probe) if (all_black(connection_probe)) { black_connection_counter++; printf("black: i = %d\n",i); }
				}
			}
			printf("black compact = %d weight = %d\n\n",black_connection_counter, fetch_compact_weight(nr_of_man, black_connection_counter));

			return black_connection_counter;
}
