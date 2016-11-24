/*
  This file is Copyright (C) 2010 Feike Boomstra.
  Distributed under the Boost Software License, Version 1.0.
  (See accompanying file LICENSE_1_0.txt or copy at
  http://www.boost.org/LICENSE_1_0.txt)
*/


#include "BoomstraDam.v2.5.h"
#include "eval.v2.4.h"
#include <math.h>
#include <pthread.h>
#include <time.h>

#define GRANULARITY 20
//#define log_items 1
#define normal 1
//#define CEM 1
const short int king_pos_value[55] = {	40,		10,		10,		10,		200,	40,		20,		0,		0,		50,		0,
									20,		20,		0,		50,		10,		10,		20,		20,		50,		0,		0,
									0,		20,		50,		0,		10,		10,		0,		50,		20,		0,		0,
									0,		50,		20,		20,		10,		10,		50,		0,		20,		20,		0,
									50,		0,		0,		20,		40,		200,	10,		10,		10,		40};

const BitArray black_centrum_masks[52] = {
(FLD6 | FLD11 | FLD17),
(FLD16 | FLD21 | FLD27),
(FLD26 | FLD31 | FLD37),
(FLD11 | FLD17 | FLD22),
(FLD21 | FLD27 | FLD32),
(FLD7 | FLD12 | FLD18),
(FLD17 | FLD22 | FLD28),
(FLD27 | FLD32 | FLD38),
(FLD12 | FLD18 | FLD23),
(FLD22 | FLD28 | FLD33),
(FLD8 | FLD13 | FLD19),
(FLD18 | FLD23 | FLD29),
(FLD28 | FLD33 | FLD39),
(FLD6 | FLD11 | FLD17 | FLD22),
(FLD16 | FLD21 | FLD27 | FLD32),
(FLD11 | FLD17 | FLD22 | FLD28),
(FLD21 | FLD27 | FLD32 | FLD38),
(FLD7 | FLD12 | FLD18 | FLD23),
(FLD17 | FLD22 | FLD28 | FLD33),
(FLD12 | FLD18 | FLD23 | FLD29),
(FLD22 | FLD28 | FLD33 | FLD39),
(FLD6 | FLD11 | FLD17 | FLD22 | FLD28),
(FLD16 | FLD21 | FLD27 | FLD32 | FLD38),
(FLD11 | FLD17 | FLD22 | FLD28 | FLD33),
(FLD7 | FLD12 | FLD18 | FLD23 | FLD29),
(FLD17 | FLD22 | FLD28 | FLD33 | FLD39),
(FLD6 | FLD11 | FLD17 | FLD22 | FLD28 | FLD33),
(FLD10 | FLD14 | FLD19),
(FLD20 | FLD24 | FLD29),
(FLD30 | FLD34 | FLD39),
(FLD15 | FLD20 | FLD24),
(FLD25 | FLD30 | FLD34),
(FLD14 | FLD19 | FLD23),
(FLD24 | FLD29 | FLD33),
(FLD9 | FLD13 | FLD18),
(FLD19 | FLD23 | FLD28),
(FLD29 | FLD33 | FLD38),
(FLD13 | FLD18 | FLD22),
(FLD23 | FLD28 | FLD32),
(FLD10 | FLD14 | FLD19 | FLD23),
(FLD20 | FLD24 | FLD29 | FLD33),
(FLD15 | FLD20 | FLD24 | FLD29),
(FLD25 | FLD30 | FLD34 | FLD39),
(FLD14 | FLD19 | FLD23 | FLD28),
(FLD24 | FLD29 | FLD33 | FLD38),
(FLD9 | FLD13 | FLD18 | FLD22),
(FLD19 | FLD23 | FLD28 | FLD32),
(FLD10 | FLD14 | FLD19 | FLD23 | FLD28),
(FLD20 | FLD24 | FLD29 | FLD33 | FLD38),
(FLD15 | FLD20 | FLD24 | FLD29 | FLD33),
(FLD14 | FLD19 | FLD23 | FLD28 | FLD32),
(FLD10 | FLD14 | FLD19 | FLD23 | FLD28 | FLD32)
};
const BitArray include_bits[52] = {
BIT1,
BIT2,
BIT3,
BIT4,
BIT5,
BIT6,
BIT7,
BIT8,
BIT9,
BIT10,
BIT11,
BIT12,
BIT13,
BIT14,
BIT15,
BIT16,
BIT17,
BIT18,
BIT19,
BIT20,
BIT21,
BIT22,
BIT23,
BIT24,
BIT25,
BIT26,
BIT27,
BIT31,
BIT32,
BIT33,
BIT34,
BIT35,
BIT36,
BIT37,
BIT38,
BIT39,
BIT40,
BIT41,
BIT42,
BIT43,
BIT44,
BIT45,
BIT46,
BIT47,
BIT48,
BIT49,
BIT50,
BIT51,
BIT52,
BIT53,
BIT54,
BIT55
};
const BitArray exclude_bits[52] = {
0,
0,
0,
0,
0,
0,
0,
0,
0,
0,
0,
0,
0,
(BIT1 | BIT4),
(BIT2 | BIT5),
(BIT4 | BIT7),
(BIT5 | BIT8),
(BIT6 | BIT9),
(BIT7 | BIT10),
(BIT9 | BIT12),
(BIT10 | BIT13),
(BIT1 | BIT4 | BIT7 | BIT14 | BIT16),
(BIT2 | BIT5 | BIT8 | BIT15 | BIT17),
(BIT4 | BIT7 | BIT10 | BIT16 | BIT19),
(BIT6 | BIT9 | BIT12 | BIT18 | BIT20),
(BIT7 | BIT10 | BIT13 | BIT19 | BIT21),
(BIT1 | BIT4 | BIT7 | BIT10 | BIT14 | BIT16 | BIT19 | BIT22 | BIT24),
0,
0,
0,
0,
0,
0,
0,
0,
0,
0,
0,
0,
(BIT31 | BIT36),
(BIT32 | BIT37),
(BIT34 | BIT32),
(BIT35 | BIT33),
(BIT36 | BIT39),
(BIT37 | BIT40),
(BIT38 | BIT41),
(BIT39 | BIT42),
(BIT31 | BIT36 | BIT43 | BIT39 | BIT47),
(BIT32 | BIT37 | BIT44 | BIT40 | BIT48),
(BIT34 | BIT32 | BIT45 | BIT37 | BIT44),
(BIT36 | BIT39 | BIT47 | BIT42 | BIT50),
(BIT31 | BIT36 | BIT39 | BIT42 | BIT43 | BIT47 | BIT50 | BIT51 | BIT54)
};
const BitArray white_centrum_masks[52] = {
(FLD45 | FLD40 | FLD34),
(FLD35 | FLD30 | FLD24),
(FLD25 | FLD20 | FLD14),
(FLD40 | FLD34 | FLD29),
(FLD30 | FLD24 | FLD19),
(FLD44 | FLD39 | FLD33),
(FLD34 | FLD29 | FLD23),
(FLD24 | FLD19 | FLD13),
(FLD39 | FLD33 | FLD28),
(FLD29 | FLD23 | FLD18),
(FLD43 | FLD38 | FLD32),
(FLD33 | FLD28 | FLD22),
(FLD23 | FLD18 | FLD12),
(FLD45 | FLD40 | FLD34 | FLD29),
(FLD35 | FLD30 | FLD24 | FLD19),
(FLD40 | FLD34 | FLD29 | FLD23),
(FLD30 | FLD24 | FLD19 | FLD13),
(FLD44 | FLD39 | FLD33 | FLD28),
(FLD34 | FLD29 | FLD23 | FLD18),
(FLD39 | FLD33 | FLD28 | FLD22),
(FLD29 | FLD23 | FLD18 | FLD12),
(FLD45 | FLD40 | FLD34 | FLD29 | FLD23),
(FLD35 | FLD30 | FLD24 | FLD19 | FLD13),
(FLD40 | FLD34 | FLD29 | FLD23 | FLD18),
(FLD44 | FLD39 | FLD33 | FLD28 | FLD22),
(FLD34 | FLD29 | FLD23 | FLD18 | FLD12),
(FLD45 | FLD40 | FLD34 | FLD29 | FLD23 | FLD18),
(FLD41 | FLD37 | FLD32),
(FLD31 | FLD27 | FLD22),
(FLD21 | FLD17 | FLD12),
(FLD36 | FLD31 | FLD27),
(FLD26 | FLD21 | FLD17),
(FLD37 | FLD32 | FLD28),
(FLD27 | FLD22 | FLD18),
(FLD42 | FLD38 | FLD33),
(FLD32 | FLD28 | FLD23),
(FLD22 | FLD18 | FLD13),
(FLD38 | FLD33 | FLD29),
(FLD28 | FLD23 | FLD19),
(FLD41 | FLD37 | FLD32 | FLD28),
(FLD31 | FLD27 | FLD22 | FLD18),
(FLD36 | FLD31 | FLD27 | FLD22),
(FLD26 | FLD21 | FLD17 | FLD12),
(FLD37 | FLD32 | FLD28 | FLD23),
(FLD27 | FLD22 | FLD18 | FLD13),
(FLD42 | FLD38 | FLD33 | FLD29),
(FLD32 | FLD28 | FLD23 | FLD19),
(FLD41 | FLD37 | FLD32 | FLD28 | FLD23),
(FLD31 | FLD27 | FLD22 | FLD18 | FLD13),
(FLD36 | FLD31 | FLD27 | FLD22 | FLD18),
(FLD37 | FLD32 | FLD28 | FLD23 | FLD19),
(FLD41 | FLD37 | FLD32 | FLD28 | FLD23 | FLD19)
};


const char distribution[19][21] =
{
		{0, 0, 0, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19},
		{0, 0, 0, 0, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18},
		{-1, 0, 0, 0, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18},
		{-1, 0, 0, 0, 0, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17},
		{-2, -1, 0, 0, 0, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17},
		{-2, -1, 0, 0, 0, 0, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16},
		{-3, -2, -1, 0, 0, 0, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16},
		{-3, -2, -1, 0, 0, 0, 0, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15},
		{-4, -3, -2, -1, 0, 0, 0, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15},
		{-4, -3, -2, -1, 0, 0, 0, 0, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14},
		{-5, -4, -3, -2, -1, 0, 0, 0, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14},
		{-5, -4, -3, -2, -1, 0, 0, 0, 0, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13},
		{-6, -5, -4, -3, -2, -1, 0, 0, 0, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13},
		{-6, -5, -4, -3, -2, -1, 0, 0, 0, 0, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12},
		{-7, -6, -5, -4, -3, -2, -1, 0, 0, 0, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12},
		{-7, -6, -5, -4, -3, -2, -1, 0, 0, 0, 0, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11},
		{-8, -7, -6, -5, -4, -3, -2, -1, 0, 0, 0, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11},
		{-8, -7, -6, -5, -4, -3, -2, -1, 0, 0, 0, 0, 2, 3, 4, 5, 6, 7, 8, 9, 10},
		{-9, -8, -7, -6, -5, -4, -3, -2, -1, 0, 0, 0, 2, 3, 4, 5, 6, 7, 8, 9, 10}

};


const char dikke_stand[41] = {
 0,	 0,   0,   0,	0,   0,   0,   0,   0,   1,
 1,  2,	  3,   4,	6,   8,  12,  17,  25,  35,
50,	65,	 75,  83,  88,  92,  94,  96,  97,  98,
99,	99,	100, 100, 100, 100, 100, 100, 100, 100, 100};
const char dunne_stand[41] = {
100, 100, 100, 100, 100, 100, 100, 100, 100, 99, 99, 98, 97, 96, 94, 92, 88, 83, 75, 65, 50, 35, 25, 17, 12, 8, 6, 4, 3, 2, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0};

const char slow_down[41] = {
0,0,0,1,1,2,3,4,6,9,13,18,23,28,33,38,43,48,53,58,63,68,73,78,83,88,93,96,97,98,99,99,100,100,100,100,100,100,100,100,100
};
//                                             0     1     2     3     4     5     6     7     8     9     10    11    12    13    14    15    16    17    18    19    20
//const int king_value_without_other_king[21] = {2700, 2800, 2900, 3000, 3100, 3100, 3200, 3200, 3200, 2800, 2400, 2000, 2000, 2000, 2000, 2000, 2000, 1900, 1800, 1700, 1600} ;
//const int king_value_with_other_king[21] =    {3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 3000, 2800, 2400, 2000, 2000, 2000, 2000, 2000, 2000, 1900, 1800, 1700, 1600} ;
const int king_value_without_other_king[21] = {2700, 2800, 2900, 3000, 3100, 3100, 3200, 3200, 3200, 3100, 3100, 3100, 3100, 3100, 3100, 3100, 3100, 2500, 1800, 1700, 1600} ;
const int king_value_with_other_king[21] =    {3000, 3000, 3000, 3000, 3100, 3100, 3200, 3200, 3200, 3100, 3100, 3100, 3100, 3100, 3100, 3100, 3100, 2500, 1800, 1700, 1600} ;
const int mobility_value[3][9] = {
		{ 1000, 800, 600, 400, 300, 200, 100, 50, 25},
		{ 1200, 960, 720, 480, 360, 240, 120, 60, 30},
		{ 1300,1040, 780, 520, 390, 250, 130, 70, 32}
};
//      0  1    2    3    4    5    6    7    8    9    10     11     12     13     14       15     16     17     18     19     20
const int mobility_sensivity[41] = {
		0, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0, 0,0, 10,10, 20,20, 50,50, 80,80, 100,100, 80,80, 60,60, 40,40, 20,20, 10,10, 0,0
};
//      0   1      2      3      4      5      6      7      8      9      10     11     12     13     14       15     16     17     18     19     20
const int down_to_50[41] = {
		50, 50,50, 50,50, 50,50, 50,50, 55,55, 60,60, 70,70, 80,80, 90,90, 100,100, 100,100, 100,100, 100,100, 100,100, 100,100, 100,100, 100,100, 100,100, 100,100, 100,100
};
const int slechte_binding[3][15] = {
{-400,	-380,	-360,	-340,	-320,	-300,	-280,	-260,   -240,  -200,   -160,   -120,    -80,    -40,      0},
{-500,	-475,	-450,	-425,	-400,	-375,	-350,	-325,	-300,  -250,   -200,   -150,   -100,	-50,      0},
{-600,	-570,	-540,	-510,	-480,	-450,	-420,	-390,	-360,  -300,   -240,   -180,   -120,	-60,	  0}
};

const int hek[8] = {50, 400, 500, 575, 625, 650, 650, 650};
const int lock[8] = {0, 500, 800, 500, 400, 300, 200, 100}; //in case many are locked, catching the king is easier
const int vleugel[9] = {0, 700, 800, 850, 875, 875, 875, 875, 875};
const int free_path_in_case_of_king[7] = {1500, 1450, 1200, 700, 200, 0, 0};
const int maze_penalty[9] = {0, 0, -20, -50, -100, -200, -400, -600, -700};
int nr_of_evaluations;
struct piece_count_record org_piece_count;

float wweight[NR_OF_WEIGHTS];
float bweight[NR_OF_WEIGHTS];

int wweight_item(int item, int indx, bool white) {
			 if (white) return (int) (wweight[indx]* (float) item);
			 else return (int) (bweight[indx]* (float) item);
}

int bweight_item(int item, int indx, bool white) {
	 if (white) return (int) (wweight[indx]* (float) item);
	 else return (int) (bweight[indx]* (float) item);
}

int row_nr_even_black(BitArray var) {
	int dis = 0;
	if (var & ROW10) dis = 10;
	else if (var & ROW8) dis = 8;
	else if (var & ROW6) dis = 6;
	else if (var & ROW4) dis = 4;
	else if (var & ROW2) dis = 2;
	return dis;
}

int row_nr_odd_black(BitArray var) {
	int dis = -1;
	if (var & ROW9) dis = 9;
	else if (var & ROW7) dis = 7;
	else if (var & ROW5) dis = 5;
	else if (var & ROW3) dis = 3;
	else if (var & ROW1) dis = 1;
	return dis;
}

int row_nr_even_white(BitArray var) {
	int dis = 12;
	if (var & ROW2) dis = 2;
	else if (var & ROW4) dis = 4;
	else if (var & ROW6) dis = 6;
	else if (var & ROW8) dis = 8;
	else if (var & ROW10) dis = 10;
	return dis;
}

int row_nr_odd_white(BitArray var) {
	int dis = 11;
	if (var & ROW3) dis = 3;
	else if (var & ROW5) dis = 5;
	else if (var & ROW7) dis = 7;
	else if (var & ROW9) dis = 9;
	return dis;
}

int calc_black_white_distance(BitArray white_man, BitArray black_man) {
	int dist;
	dist  = row_nr_even_white(COL1 & white_man) - row_nr_even_black(COL1 & black_man);
	dist += row_nr_odd_white (COL2 & white_man) - row_nr_odd_black (COL2 & black_man);
	dist += row_nr_even_white(COL3 & white_man) - row_nr_even_black(COL3 & black_man);
	dist += row_nr_odd_white (COL4 & white_man) - row_nr_odd_black (COL4 & black_man);
	dist += row_nr_even_white(COL5 & white_man) - row_nr_even_black(COL5 & black_man);
	dist += row_nr_odd_white (COL6 & white_man) - row_nr_odd_black (COL6 & black_man);
	dist += row_nr_even_white(COL7 & white_man) - row_nr_even_black(COL7 & black_man);
	dist += row_nr_odd_white (COL8 & white_man) - row_nr_odd_black (COL8 & black_man);
	dist += row_nr_even_white(COL9 & white_man) - row_nr_even_black(COL9 & black_man);
	dist += row_nr_odd_white (COL10 & white_man) - row_nr_odd_black (COL10 & black_man);
	return dist;
}

int white_distance (BitArray var, NodePnt node)
{	int dis = 0;
	if (var & ROW2) dis = 1;
	else if (var & ROW3) dis = 2;
	else if (var & ROW4) dis = 3;
	else if (var & ROW5) dis = 4;
	else if (var & ROW6) dis = 5;
	else if (var & ROW7) dis = 6;
	if (dis>0) if (node->status & IS_WHITE_ON_MOVE) dis=dis-1;
	return free_path_in_case_of_king[dis];
}

int black_distance (BitArray var, NodePnt node)
{	int dis = 0;
	if (var & ROW9) dis = 1;
	else if (var & ROW8) dis = 2;
	else if (var & ROW7) dis = 3;
	else if (var & ROW6) dis = 4;
	else if (var & ROW5) dis = 5;
	else if (var & ROW4) dis = 6;
	if (dis>0) if (!(node->status & IS_WHITE_ON_MOVE)) dis=dis-1;
	return free_path_in_case_of_king[dis];
}

int white_distance_calc (BitArray var, NodePnt node)
{	int dis = 0;
	if (var & ROW2) dis = 1;
	else if (var & ROW3) dis = 2;
	else if (var & ROW4) dis = 3;
	else if (var & ROW5) dis = 4;
	else if (var & ROW6) dis = 5;
	else if (var & ROW7) dis = 6;
	if (dis>0) if (node->status & IS_WHITE_ON_MOVE) dis=dis-1;
	return dis;
}

int black_distance_calc (BitArray var, NodePnt node)
{	int dis = 0;
	if (var & ROW9) dis = 1;
	else if (var & ROW8) dis = 2;
	else if (var & ROW7) dis = 3;
	else if (var & ROW6) dis = 4;
	else if (var & ROW5) dis = 5;
	else if (var & ROW4) dis = 6;
	if (dis>0) if (!(node->status & IS_WHITE_ON_MOVE)) dis=dis-1;
	return dis;
}

 int less_two(BitArray white_man, const unsigned long long mask)
{	int counter;
	count_bits((white_man & mask), counter);
	if (counter < 2) return 1;
	else return 0;
}


 char min_(char a, char b)
{	if (a<b) return a;
	else return b;
}

 char max_(char a, char b)
{	if (a>b) return a;
	else return b;
}

int imin(int a, int b)
{	if (a<b) return a;
	else return b;
}
int imax(int a, int b)
{	if (a>b) return a;
	else return b;
}

int calc_centre_value(BitArray man)
{	int col1_count = __builtin_popcountll(man & (COL1 | COL10));
	int col2_count = __builtin_popcountll(man & (COL2 | COL9));
	int col3_count = __builtin_popcountll(man & (COL3 | COL8));
	int col4_count = __builtin_popcountll(man & (COL4 | COL7));
	int col5_count = __builtin_popcountll(man & (COL5 | COL6));
	int total = col1_count + col2_count + col3_count + col4_count + col5_count;
	//printf("c5=%d, c4=%d, c3=%d, c2=%d, total=%d\n", 8*col5_count, 7*col4_count, 5*col3_count, 4*col2_count, 8*col5_count+ 7*col4_count+ 5*col3_count+ 4*col2_count);
	if (total > 0)return ((1*col2_count + 3*col3_count + 6*col4_count + 7*col5_count)*100)/20;
	else          return 0;
}

int man_position_with_kings(BitArray man) {
	int col1_count = __builtin_popcountll(man & (COL1 | COL10));
	int col2_count = __builtin_popcountll(man & (COL2 | COL9));
	int col3_count = __builtin_popcountll(man & (COL3 | COL8));
	int col4_count = __builtin_popcountll(man & (COL4 | COL7));
	int col5_count = __builtin_popcountll(man & (COL5 | COL6));
	int total = col1_count + col2_count + col3_count + col4_count + col5_count;
	if (total > 0)return ((1*col4_count + 3*col3_count + 6*col2_count + 7*col1_count)*100)/20;
	else          return 0;
}

int king_position(BitArray kings) {
BitArray temp;
int result = 0;
	if (kings == 0) return 0;
	while(kings) {
		temp = kings ^ (kings & (kings - 1)); kings = (kings & (kings - 1));
		if (temp & (FLD46|FLD5)) result += 100;
		else if (temp & (FLD41|FLD37|FLD32|FLD28|FLD23|FLD19|FLD14|FLD10)) result += 80;
		else if (temp & (FLD1|FLD6|FLD45|FLD50)) result += 40;
		else if (temp & (FLD7|FLD11|FLD12|FLD17|FLD18|FLD22|FLD29|FLD33|FLD34|FLD39|FLD40|FLD44)) result += 20;
	}
	return result;
}



int ok(BitArray oth_man, BitArray own_man, BitArray empty, BitArray fld1, BitArray fld11, BitArray fld21)
{	return	(((oth_man & fld1) && (empty & fld11)) ||
			 ((oth_man & fld11) && (empty & fld1)) ||
			 ((oth_man & fld1) && (oth_man & fld11) && (empty & fld21)));

}
int ok_bits_set(BitArray empty, BitArray man, BitArray fld1, BitArray fld2)
{	if (fld1 & man){ if (fld2 & man) { return 1;} else return 0; }
	if (fld1 & empty) { if (fld2 & empty){ return 1;}
	if (fld2 & man) { return 1; }}
	return 0;
}
int white_check_counts_and_locks(NodePnt node, BitArray white_man, BitArray black_man, int *white_warning, int nr, BitArray ow1_1, BitArray ow2_1, BitArray ow3_1, BitArray ow4_1)
{	int bc, wc;
	count_bits((black_man & ow3_1), bc);
	count_bits((white_man & ow4_1), wc);
	//printf("wit: wc=%d, bc=%d nr=%d\n", wc, bc, nr);
	if ((bc-wc-nr) > 0) {
		//*white_warning++;
		return white_is_locking_pieces(node, bc - wc - nr, (ow2_1), ow1_1);
	}
	else {
		if  (((bc-wc-nr) == 0) && (white_is_locking_pieces(node, bc - wc - nr, (ow2_1), ow1_1))) (*white_warning)++;
		return 0;
	}
}

int black_check_counts_and_locks(NodePnt node, BitArray black_man, BitArray white_man, int *black_warning, int nr, BitArray ow1_1, BitArray ow2_1, BitArray ow3_1, BitArray ow4_1)
{	int bc, wc;
	count_bits((white_man & ow3_1), wc);
	count_bits((black_man & ow4_1), bc);
	if ( wc-bc-nr > 0) {
		//*black_warning++;
		return black_is_locking_pieces(node, wc -bc - nr, (ow2_1 ), ow1_1);
	}
	else {
		if (( wc-bc-nr == 0) && (black_is_locking_pieces(node, wc -bc - nr, (ow2_1 ), ow1_1))) (*black_warning)++;
		return 0;
	}

}

__inline int tot_saldo(NodePnt node) {
	return (3*node->nr_of_white_king + 3*node->nr_of_black_king + node->nr_of_white_man + node->nr_of_black_man) -
			(3*org_piece_count.nr_of_black_king + 3*org_piece_count.nr_of_white_king + org_piece_count.nr_of_white_man + org_piece_count.nr_of_black_man);
}

__inline BitArray nw(BitArray org)
{ return (org >> 6) & BITS50; }

__inline BitArray nw_nw(BitArray org)
{ return (org >> 12) & BITS50; }

__inline BitArray ne(BitArray org)
{ return (org >> 5) & BITS50; }

__inline BitArray ne_ne(BitArray org)
{ return (org >> 10) & BITS50; }

__inline BitArray sw(BitArray org)
{ return (org << 5) & BITS50; }

__inline BitArray sw_sw(BitArray org)
{ return (org << 10) & BITS50; }

__inline BitArray se(BitArray org)
{ return (org << 6) & BITS50;}

__inline BitArray se_se(BitArray org)
{ return (org << 12) & BITS50;}

__inline BitArray east(BitArray org)
{ return (org << 1) & BITS50;}

__inline BitArray west(BitArray org)
{ return (org >> 1) & BITS50;}

__inline BitArray north(BitArray org)
{ return (org >> 11) & BITS50;}

__inline BitArray south(BitArray org)
{ return (org << 11) & BITS50;}

int color_or_edge(BitArray var, BitArray man)
{
	if (var)
	{
		if (man & var) return 1;
		else return 0;
	}
	else return 1;
}

#define is_white_or_edge(var) (color_or_edge((var), white_man))
#define is_white(var) (white_man & (var))
#define is_black_or_edge(var) (color_or_edge((var), black_man))
#define is_black(var) (black_man & (var))

/*
int white_right_wing_lock(BitArray org, BitArray white_man, BitArray black_man, BitArray empty) // runtime test to find wing-locks
// org contains the SE bottom of the black man
{	BitArray nw_org = nw(org);
	if (!(black_man & org)) return 0;
	if (!(black_man & nw_org)) return 0;
	if (black_man & ne(org))	// is it a square?
	{	//yes
		if (se_se(org))
		{	if (!(white_man & se_se(org))) if !(white_man & se(org)) return 0;
		}
		if (east(org))
		{	if (white_man & east(org))
			{
				printf("config ");
			}
			else
			{
				if ()
			}
				return 0;
		}
	}
	else
	{	// no it is not a square
		if (!(white_man & se(org))) return 0;
		if (se_se(org)) if (!(white_man & se_se(org))) return 0;
	}
	return 1;
}
*/

int dist1(int num, BitArray a0, BitArray a1)
{	if (count(a0) >= num) return 0;
	else if (count(a0)+count(a1) >= num) return 1*num;
	else printf("dist1 problem: num=%d, count=%d\n", num, count(a0)+count(a1));
	return -1;
}
int dist2(int num, BitArray a0, BitArray a1, BitArray a2)
{	if (count(a0) >= num) return 0;
	else if (count(a0)+count(a1) >= num) return 1*num;
	else if (count(a0)+count(a1) + count(a2) >= num) return 1*count(a1) + 2*(num - count(a1));
	else printf("dist2 problem: num=%d, count=%d\n", num, count(a0)+count(a1)+ count(a2));
	return -1;
}
int dist3(int num, BitArray a0, BitArray a1, BitArray a2, BitArray a3)
{	if (count(a0) >= num) return 0;
	else if (count(a0)+count(a1) >= num) return 1*num;
	else if (count(a0)+count(a1) + count(a2) >= num) return 1*count(a1) + 2*(num - count(a1));
	else if (count(a0)+count(a1) + count(a2)+ count(a3) >= num) return 1*count(a1) + 2*count(a2) + 3*(num - count(a1) - count(a2));
	else printf("dist3 problem: num=%d, count=%d\n", num, count(a0)+count(a1)+ count(a2)+ count(a3));
	return -1;
}
int dist4(int num, BitArray a0, BitArray a1, BitArray a2, BitArray a3, BitArray a4)
{	if (count(a0) >= num) return 0;
	else if (count(a0)+count(a1) >= num) return 1*num;
	else if (count(a0)+count(a1) + count(a2) >= num) return 1*count(a1) + 2*(num - count(a1));
	else if (count(a0)+count(a1) + count(a2)+ count(a3) >= num) return 1*count(a1) + 2*count(a2) + 3*(num - count(a1) - count(a2));
	else if (count(a0)+count(a1) + count(a2)+ count(a3)+ count(a4) >= num) return 1*count(a1) + 2*count(a2) + 3*count(a3)+ 4*(num - count(a1) - count(a2) - count(a4));
	else printf("dist4 problem: num=%d, count=%d\n", num, count(a0)+count(a1)+ count(a2)+ count(a3)+ count(a4));
	return -1;
}

int re_scale(int wp, int row) // two voorpost scores on the same piece
{	if (row==5)
	{	if (wp==200) return 100;
		else if (wp==-300) return -400;
		else if (wp==-800) return -500;
		else return wp;
	} else
	{	if (wp==200) return 120;
		else if (wp==-300) return -400;
		else if (wp==-800) return -500;
		else return wp;
	}

}

short int evaluate(NodePnt node, char print_flag, char log_flag, int ThreadID )
/***************************************************
 * it is only called in a quiet state ..  no       *
 * captures are possible from the color on move.   *
 ***************************************************/
{
	//int more_4_pieces = 500;
//	register BitArray t;
	BitArray white_man = 0;
	BitArray black_man = 0;
	BitArray white_king = 0;
	BitArray black_king = 0;
	BitArray empty_field = 0;
	int nr_of_white_man;
	int nr_of_black_man ;
	int nr_of_white_king;
	int nr_of_black_king;
	int white_man_position = 0;
	int black_man_position = 0;
	int white_king_position = 0;
	int black_king_position = 0;
	unsigned long white_man_idx[21];
	unsigned long white_king_idx[21];
	unsigned long black_man_idx[21];
	unsigned long black_king_idx[21];
	int white_centrum = 0;
	int black_centrum = 0;
	//int white_more_4_pieces = 0;
	//int black_more_4_pieces = 0;
	int nr_of_white_man_locked = 0;
	int nr_of_black_man_locked = 0;
	BitArray white_man_locked = 0;
	BitArray black_man_locked = 0;
	int white_man_locked_bonus = 0;
	int black_man_locked_bonus = 0;
	int white_avoid_4641 = 0;
	int black_avoid_4641 = 0;
	int white_avoid_2324 = 0;
	int black_avoid_2324 = 0;
	int white_avoid_2335 = 0;
	int black_avoid_2335 = 0;
	int white_ketting_stelling = 0;
	int black_ketting_stelling = 0;
	int white_ketting_stelling_2 = 0;
	int black_ketting_stelling_2 = 0;
	int is_klassiek = 0;
	int white_halve_hek = 0;
	int black_halve_hek = 0;
	int white_active_15 = 0;
	int black_active_15 = 0;
	int white_tempi = 0;
	int black_tempi = 0;
//	int white_free_moves = 0;
//	int black_free_moves = 0;
	int white_klassiek = 0;
	int black_klassiek = 0;
	int white_tempi_bonus = 0;
	int black_tempi_bonus = 0;
	int white_slechte_binding = 0;
	int black_slechte_binding = 0;
	int white_free_path = 0;
	int black_free_path = 0;
	int white_fit=0;
	int black_fit=0;
	int white_edge_35 = 0;
	int black_edge_35 = 0;
	int white_edge_36 = 0;
	int black_edge_36 = 0;
	int white_2_8_13 = 0;
	int black_2_8_13 = 0;
	int white_11_16_17 = 0;
	int black_11_16_17 = 0;
	int white_253035 = 0;
	int black_253035 = 0;
	int white_voorpost = 0;
	int black_voorpost = 0;
	int white_locked = 0;
	int black_locked = 0;
	int white_vleugel_opsluiting = 0;
	int black_vleugel_opsluiting = 0;
	int white_king_value;
	int black_king_value;
	//int nr_of_pieces;
	int white_tandem = 0;
	int black_tandem = 0;
	int white_mobility = 0;
	int black_mobility = 0;
	int white_canon = 0;
	int black_canon = 0;
	int white_maze_penalty = 0;
	int black_maze_penalty = 0;
	int black;
	int white;
	int white_endgame = 0;
	int black_endgame = 0;
	BitArray white_free_path_pieces;
	BitArray black_free_path_pieces;


	short int score;

	if (node == NULL) { printf("NULL node in eval\n"); return 0; } ;

	white_man = (node->occ_empty) & (node->white_black) & (node->man_king);
	white_king = (node->occ_empty) & (node->white_black) & ~(node->man_king);
	black_man = (node->occ_empty) & ~(node->white_black) & (node->man_king);
	black_king = (node->occ_empty) & ~(node->white_black) & ~(node->man_king);
	empty_field = ~(node->occ_empty) & BITS50;

	Threads[ThreadID].nr_of_nodes_evaluated++;


// *+* eng game database ---------------------------------------------------------------------------------------------------------

	if (node->total_nr_of_pieces <= NR_OF_ENDGAME_PIECES)
	{ 	int endgame;
		count_bits_and_fill_list(white_man, nr_of_white_man, white_man_idx);
		count_bits_and_fill_list(black_man, nr_of_black_man, black_man_idx);
		if (white_king)
		{	count_bits_and_fill_list(white_king, nr_of_white_king, white_king_idx);}
		else nr_of_white_king = 0;
		if (black_king)
		{	count_bits_and_fill_list(black_king, nr_of_black_king, black_king_idx);}
		else nr_of_black_king = 0;


		lock_grab(&EndGameLock);
		endgame = evaluate_endgame_position(  node->status & IS_WHITE_ON_MOVE,
													nr_of_white_man, &white_man_idx[0],
													nr_of_white_king, &white_king_idx[0],
													nr_of_black_man, &black_man_idx[0],
													nr_of_black_king, &black_king_idx[0]);
		lock_release(&EndGameLock);
		if (endgame != 0)
		{	if (print_flag) printf("from endgame database: score = %d\n", -endgame);
/*			if (log_flag)
			{	fcloseall(); open_log_file();
				fprintf(log_file,"from endgame database: score = %d\n", -endgame);
				fcloseall();
			};
*/			node->status = node->status | IS_EXACT_VALUE;
			if (node->status & IS_MAX_NODE)
				return endgame;
			else
				return -endgame;
		}
	}

	// *+* calculate king value: ----------------------------------------------------------------------------------------------------

	if (node->nr_of_black_king) white_king_value = king_value_with_other_king[node->nr_of_black_man];
	else white_king_value = king_value_without_other_king[node->nr_of_black_man];
	if (node->nr_of_white_king) black_king_value = king_value_with_other_king[node->nr_of_white_man];
	else black_king_value = king_value_without_other_king[node->nr_of_white_man];

	/* In general you can't win with less that 4 pieces */

	//if (node->nr_of_white_man + node->nr_of_white_king >= 4) white_more_4_pieces = more_4_pieces;
	//if (node->nr_of_black_man + node->nr_of_black_king >= 4) black_more_4_pieces = more_4_pieces;

	// *+* is klassiek ?-----------------------------------------------------------------------------------------------------------

	if (all_bits_set(white_man, KLASSIEK_WHITE) && all_bits_set(black_man, KLASSIEK_BLACK) && all_empty((FLD22|FLD29))) is_klassiek = 1;
	else is_klassiek = 0;


/********************************************************************************
 * replaced position value with kind of centre algoritm
 *
 * ******************************************************************************/

	// *+* man position ------------------------------------------------------------------------------------------------------------

	if (node->nr_of_black_king == 0) {
		if (!(is_klassiek)){
			white_man_position = calc_centre_value(white_man);
			//printf("white org1=%d\n", white_man_position);
			if (white_man & FLD46) white_man_position += -50;
			//if (white_man & FLD36) white_man_position += -20;
			if (white_man & FLD26) white_man_position += -60;
			if (white_man & FLD16) white_man_position += -80;
			if (white_man & FLD35)
			{	if (white_man & FLD24) white_man_position += -60;
				else white_man_position += -30;
			}
			//printf("white org2=%d\n", white_man_position);
			if (white_man & FLD25) white_man_position += 50;
			//printf("white org3=%d\n", white_man_position);
			if (white_man & FLD48) white_man_position += (60*slow_down[node->total_nr_of_pieces])/100;
			//printf("white org4=%d\n", white_man_position);
			if (white_man & FLD47) white_man_position += (55*slow_down[node->total_nr_of_pieces])/100;
			//printf("white org5=%d, %d\n", white_man_position, (55*slow_down[node->total_nr_of_pieces])/100);
			//white_man_position = (white_man_position*man_distr_slope[node->total_nr_of_pieces])/100;
			//printf("white org6=%d\n", white_man_position);
		}
	}
	else {
		black_king_position = king_position(black_king);
		white_man_position = man_position_with_kings(white_man);
		//white_man_position = (white_man_position*man_distr_slope[node->total_nr_of_pieces])/100;
	}

	if (node->nr_of_white_king == 0) {
		if (!(is_klassiek)){
			black_man_position = calc_centre_value(black_man);
			if (black_man & FLD5) black_man_position += -50;
			//if (black_man & FLD15) black_man_position += -20;
			if (black_man & FLD25) black_man_position += -60;
			if (black_man & FLD35) black_man_position += -80;
			if (black_man & FLD16)
			{	if (black_man & FLD27) black_man_position += -60;
				else black_man_position += -30;
			}
			if (black_man & FLD26) black_man_position += 50;
			if (black_man & FLD3) black_man_position += (60*slow_down[node->total_nr_of_pieces])/100;
			if (black_man & FLD4) black_man_position += (55*slow_down[node->total_nr_of_pieces]) /100;
			//black_man_position = (black_man_position*man_distr_slope[node->total_nr_of_pieces])/100;
		}
	}
	else {
		white_king_position = king_position(white_king);
		black_man_position = man_position_with_kings(black_man);
		//black_man_position = (black_man_position*man_distr_slope[node->total_nr_of_pieces])/100;
	}



	// *+* count the number of locked men --------------------------------------------------------------------------------------------------

	//A locked man is a man that can't reach the damlijn.
	// Lxx is a line definition Bxx are the fields that are blocked by that line

	if ((node->nr_of_white_king > 0) && (node->nr_of_black_king == 0))
	{	if ((white_king & L16NW) && ((white_man & L16NW_) == 0) && ((black_man & L16NW)==0)) { black_man_locked = black_man_locked | (black_man & B16NW); }
		if ((white_king & L26NW) && ((white_man & L26NW_) == 0) && ((black_man & L26NW)==0)) { black_man_locked = black_man_locked | (black_man & B26NW); }
		if ((white_king & L36NW) && ((white_man & L36NW_) == 0) && ((black_man & L36NW)==0)) { black_man_locked = black_man_locked | (black_man & B36NW); }
		if ((white_king & L4SW)  && ((white_man & L4SW_)  == 0) && ((black_man & L4SW)==0)) { black_man_locked = black_man_locked | (black_man & B4SW); }
		if ((white_king & L3SW)  && ((white_man & L3SW_)  == 0) && ((black_man & L3SW)==0)) { black_man_locked = black_man_locked | (black_man & B3SW); }
		if ((white_king & L2SW)  && ((white_man & L2SW_)  == 0) && ((black_man & L2SW)==0)) { black_man_locked = black_man_locked | (black_man & B2SW); }
		if ((white_king & L1SW)  && ((white_man & L1SW_)  == 0) && ((black_man & L1SW)==0)) { black_man_locked = black_man_locked | (black_man & B1SW); }
		if (((white_king & DIAGLOW) && ((white_man & DIAGLOW) == 0)) || ((white_king & DIAGHIGH) && ((white_man & DIAGHIGH) == 0))) { black_man_locked = black_man_locked | (black_man & B46NW); }
		count_bits(black_man_locked, nr_of_black_man_locked)
	}
	  // all locked black man


	if ((node->nr_of_black_king > 0) && (node->nr_of_white_king == 0))
	{	if ((black_king & L6SW)  && ((black_man & L6SW_)  == 0) && ((white_man & L6SW)==0)) { white_man_locked = white_man_locked | (white_man & B6SW); }
		if ((black_king & L16SW) && ((black_man & L16SW_) == 0) && ((white_man & L16SW)==0)) { white_man_locked = white_man_locked | (white_man & B16SW); }
		if ((black_king & L26SW) && ((black_man & L26SW_) == 0) && ((white_man & L26SW)==0)) { white_man_locked = white_man_locked | (white_man & B26SW); }
		if ((black_king & L36SW) && ((black_man & L36SW_) == 0) && ((white_man & L36SW)==0)) { white_man_locked = white_man_locked | (white_man & B36SW); }
		if ((black_king & L15SE) && ((black_man & L15SE_) == 0) && ((white_man & L15SE)==0)) { white_man_locked = white_man_locked | (white_man & B15SE); }
		if ((black_king & L25SE) && ((black_man & L25SE_) == 0) && ((white_man & L25SE)==0)) { white_man_locked = white_man_locked | (white_man & B25SE); }
		if ((black_king & L35SE) && ((black_man & L35SE_) == 0) && ((white_man & L35SE)==0)) { white_man_locked = white_man_locked | (white_man & B35SE); }
		if (((black_king & DIAGLOW) && ((black_man & DIAGLOW) == 0)) || ((black_king & DIAGHIGH) && ((black_man & DIAGHIGH) == 0)))	{ white_man_locked = white_man_locked | (white_man & B5SE); }
		count_bits(white_man_locked, nr_of_white_man_locked)
	}
	 // all locked white man

	if (node->nr_of_white_man) black_man_locked_bonus = (nr_of_white_man_locked*lock[min_(7,nr_of_white_man_locked)])/node->nr_of_white_man;
	if (node->nr_of_black_man) white_man_locked_bonus = (nr_of_black_man_locked*lock[min_(7,nr_of_black_man_locked)])/node->nr_of_black_man;

	// *+* vrije doorgang naar dam -------------------------------------------------------------------------------
	{

		if (node->status & IS_WHITE_ON_MOVE)
		{	white_free_path_pieces = nr_of_white_on_move_free_path(white_man, black_man, empty_field);
			black_free_path_pieces = nr_of_black_not_on_move_free_path(white_man, black_man, empty_field);
		}
		else
		{	black_free_path_pieces = nr_of_black_on_move_free_path(white_man, black_man, empty_field);
			white_free_path_pieces = nr_of_white_not_on_move_free_path(white_man, black_man, empty_field);
		}
		//print_bits(white_free_path_pieces);
		white_free_path_pieces = (white_free_path_pieces & ~white_man_locked); //remove locked pieces
		//print_bits(white_free_path_pieces);
		if (white_free_path_pieces)
			if (node->nr_of_black_king)
			{	// special checks if other king
				//printf("black king\n");
				white_free_path = white_distance(white_free_path_pieces, node); // only the closest to the kingline counts
			}
			else
			{	//printf("no black king\n");
				white_free_path = 1500;
			}

		black_free_path_pieces = (black_free_path_pieces & ~black_man_locked);
		if (black_free_path_pieces)
			if  (node->nr_of_white_king)
			{
				//printf("white king\n");
				black_free_path += black_distance(black_free_path_pieces, node);
			}
			else
			{	//printf("No white king\n");
				black_free_path = 1500;
			}
		// if both parties has no king and both have a breakthrough, than adjust the
		// value of the breakthrough with the distance to the kingsrow.

		if (white_free_path == 1500 && black_free_path == 1500)
		{	int white_distance = white_distance_calc(white_free_path_pieces, node);
			int black_distance = black_distance_calc(black_free_path_pieces, node);
			if (white_distance < black_distance) // small is beautiful
				black_free_path = free_path_in_case_of_king[black_distance - white_distance];
			if (black_distance < white_distance) // small is beautiful
				white_free_path = free_path_in_case_of_king[white_distance - black_distance];

		}

		if (count(white_free_path_pieces)>1) white_free_path += 80;
		if (count(black_free_path_pieces)>1) black_free_path += 80;

	}
	// *+* maze patterns -------------------------------------------------------------------------------------------

	if (node->total_nr_of_pieces > 20)
	{	//gatenkaas
		int black_maze6 = 0, black_maze1 = 0, black_maze7 = 0, black_maze2 = 0;
		int max_black_maze;
		if (empty_field & (FLD6|FLD8|FLD10)){
			if (black_jump( 6,11,17 )) black_maze6++;
			if (black_jump(17,21,26 )) black_maze6++;
			if (black_jump(17,22,28 )) black_maze6++;
			if (black_jump(17,12,8 )) black_maze6++;
			if (black_jump( 8,13,19 )) black_maze6++;
			if (black_jump(19,23,28 )) black_maze6++;
			if (black_jump(19,24,30 )) black_maze6++;
			if (black_jump(19,14,10 )) black_maze6++;
		}
		if (empty_field & (FLD1|FLD3|FLD5)){
			if (black_jump( 1, 7,12 )) black_maze1++;
			if (black_jump(12,17,21 )) black_maze1++;
			if (black_jump(12,18,23 )) black_maze1++;
			if (black_jump(12, 8, 3 )) black_maze1++;
			if (black_jump(23,19,14 )) black_maze1++;
			if (black_jump(14, 9, 3 )) black_maze1++;
			if (black_jump(14,10, 5 )) black_maze1++;
			if (black_jump(14,20,25 )) black_maze1++;
		}
		if (empty_field & (FLD7|FLD9)){
			if (black_jump(16,11, 7 )) black_maze7++;
			if (black_jump(16,21,27 )) black_maze7++;
			if (black_jump( 7,12,18 )) black_maze7++;
			if (black_jump(27,22,18 )) black_maze7++;
			if (black_jump(18,13, 9 )) black_maze7++;
			if (black_jump(18,23,29 )) black_maze7++;
			if (black_jump(29,24,20 )) black_maze7++;
			if (black_jump( 9,14,20 )) black_maze7++;
		}
		if (empty_field & (FLD2|FLD4)){
			if (black_jump(11, 7, 2 )) black_maze2++;
			if (black_jump(11,17,22 )) black_maze2++;
			if (black_jump(22,18,13 )) black_maze2++;
			if (black_jump( 2, 8,13 )) black_maze2++;
			if (black_jump(13,19,24 )) black_maze2++;
			if (black_jump(13, 9, 4 )) black_maze2++;
			if (black_jump(24,20,15 )) black_maze2++;
			if (black_jump( 4,10,15 )) black_maze2++;
		}
		//printf("blackmaze_6 = %d,_1 = %d, _7 = %d, _2 = %d\n", black_maze6, black_maze1, black_maze7, black_maze2);
		max_black_maze = imax(black_maze6, imax(black_maze1,imax(black_maze7, black_maze2)));
		black_maze_penalty = (maze_penalty[max_black_maze]*dikke_stand[imax(0,node->total_nr_of_pieces-4)])/100;
		//printf("max = %d, penalty = %d\n", max_black_maze, black_maze_penalty);
		//printf("max=%d tot=%d\n\n", max_black_maze, tot_black_maze);

		int white_maze6 = 0, white_maze1 = 0, white_maze7 = 0, white_maze2 = 0;
		int max_white_maze;
		if (empty_field & (BLD6|BLD8|BLD10)){
			if (white_jump( 6,11,17 )) white_maze6++;
			if (white_jump(17,21,26 )) white_maze6++;
			if (white_jump(17,22,28 )) white_maze6++;
			if (white_jump(17,12,8 )) white_maze6++;
			if (white_jump( 8,13,19 )) white_maze6++;
			if (white_jump(19,23,28 )) white_maze6++;
			if (white_jump(19,24,30 )) white_maze6++;
			if (white_jump(19,14,10 )) white_maze6++;
		}
		if (empty_field & (BLD1|BLD3|BLD5)){
			if (white_jump( 1, 7,12 )) white_maze1++;
			if (white_jump(12,17,21 )) white_maze1++;
			if (white_jump(12,18,23 )) white_maze1++;
			if (white_jump(12, 8, 3 )) white_maze1++;
			if (white_jump(23,19,14 )) white_maze1++;
			if (white_jump(14, 9, 3 )) white_maze1++;
			if (white_jump(14,10, 5 )) white_maze1++;
			if (white_jump(14,20,25 )) white_maze1++;
		}
		if (empty_field & (BLD7|BLD9)){
			if (white_jump(16,11, 7 )) white_maze7++;
			if (white_jump(16,21,27 )) white_maze7++;
			if (white_jump( 7,12,18 )) white_maze7++;
			if (white_jump(27,22,18 )) white_maze7++;
			if (white_jump(18,13, 9 )) white_maze7++;
			if (white_jump(18,23,29 )) white_maze7++;
			if (white_jump(29,24,20 )) white_maze7++;
			if (white_jump( 9,14,20 )) white_maze7++;
		}
		if (empty_field & (BLD2|BLD4)){
			if (white_jump(11, 7, 2 )) white_maze2++;
			if (white_jump(11,17,22 )) white_maze2++;
			if (white_jump(22,18,13 )) white_maze2++;
			if (white_jump( 2, 8,13 )) white_maze2++;
			if (white_jump(13,19,24 )) white_maze2++;
			if (white_jump(13, 9, 4 )) white_maze2++;
			if (white_jump(24,20,15 )) white_maze2++;
			if (white_jump( 4,10,15 )) white_maze2++;
		}
		max_white_maze = imax(white_maze6, imax(white_maze1,imax(white_maze7, white_maze2)));
		white_maze_penalty = (maze_penalty[max_white_maze]*dikke_stand[imax(0, node->total_nr_of_pieces-4)])/100;
		//printf("max=%d tot=%d\n\n", max_white_maze, tot_white_maze);
	}


	// white pootje vervallen


	if (node->nr_of_white_king + node->nr_of_black_king == 0)
	{
		// *+* some special cases (active_15, avoid_4641, avoid_2325, avoid_2324, edge_35, edge_36, 11_16_17, 2_8_13) -------------------

		if (((white_man & FLD15) && (empty_field & FLD24)) && ((empty_or_white & FLD19) || (white_man & FLD30)))
			white_active_15 = (80*dikke_stand[node->total_nr_of_pieces])/100;
		if (((black_man & BLD15) && (empty_field & BLD24)) && ((empty_or_black & BLD19) || (black_man & BLD30)))
				black_active_15 = (80*dikke_stand[node->total_nr_of_pieces])/100;

		if ((empty_field & FLD46) && (black_man & FLD26 ) && (all_white(w2(41,37))) && (count(white_man & w3(36,31,27)) > 1))
			if (white_man & FLD32) white_avoid_4641 = -400;
			else white_avoid_4641 = -200;
/*		{	if      (all_bits_set(white_man,((FLD41 | FLD31 | FLD36 | FLD37)))) white_avoid_4641 = -700;
			else if (all_bits_set(white_man,(FLD41 | FLD31 | FLD37))) white_avoid_4641 = -50;
			//else if (all_bits_set(white_man,((FLD47 | FLD31 | FLD36 | FLD37)))) white_avoid_4641 = (-500*dunne_stand[node->total_nr_of_pieces])/100;
			//else if (all_white((FLD31|FLD36|FLD37))) white_avoid_4641 = -50;
			//else if (all_white((FLD41|FLD36|FLD37))) white_avoid_4641 = -50;
		};
*/		if ((empty_field & FLD5) && (white_man & FLD25) && (all_black(b2(41,37))) && (count(black_man & b3(36,31,27)) > 1))
			if (black_man & BLD32) black_avoid_4641 = -400;
			else black_avoid_4641 = -200;
/*		{	if      (all_bits_set(black_man,((BLD41 | BLD31 | BLD36 | BLD37)))) black_avoid_4641 = -700;
			else if (all_bits_set(black_man,(BLD41 | BLD31 | BLD37))) black_avoid_4641 = -50;
			//else if (all_bits_set(black_man,((BLD47 | BLD31 | BLD36 | BLD37)))) black_avoid_4641 = (-500*dunne_stand[node->total_nr_of_pieces])/100;
			//else if ((all_black((BLD31|BLD36|BLD37)))) black_avoid_4641 = -50;
			//else if ((all_black((BLD41|BLD36|BLD37)))) black_avoid_4641 = -50;
		};
*/

		if ((white_man & FLD35) && (white_man & FLD23)) white_avoid_2335 = -50*dikke_stand[node->total_nr_of_pieces]/100;
		if ((black_man & FLD16) && (black_man & FLD28)) black_avoid_2335 = -50*dikke_stand[node->total_nr_of_pieces]/100;

		if ((black_man & FLD23) && (white_man & FLD24)) white_avoid_2324 = -65*dikke_stand[node->total_nr_of_pieces]/100;
		if ((white_man & FLD28) && (black_man & FLD27)) black_avoid_2324 = -65*dikke_stand[node->total_nr_of_pieces]/100;

//		if ((black_man & FLD36)	&& (empty_field & FLD47) && (white_man & (FLD37|FLD41|FLD46))) white_36_47 = -600;
//		if ((white_man & FLD15) && (empty_field & FLD4) && (black_man & (FLD14|FLD10|FLD5))) black_36_47 = -600;

		if ((black_man & FLD36)	&& (all_empty((FLD47 | FLD37  | FLD32))) && (white_man & (FLD46)))   white_edge_36 = -75;
		if ((white_man & FLD15) && (all_empty((FLD4  | FLD14  | FLD19))) && (black_man & (FLD5 )))   black_edge_36 = -75;

		if ((black_man & FLD36)	&& (all_empty((FLD47 | FLD46 | FLD41))) && (white_man & (FLD37)))
		{	if ((black_or_empty & FLD26) && (black_or_empty & FLD27) && (black_man & (FLD26|FLD27)) && (empty_field & FLD31)) white_edge_36 = -400;
			else                                                                                                              white_edge_36 = -slow_down[node->total_nr_of_pieces];
		}
		if ((white_man & FLD15) && (all_empty((FLD4  | FLD5  | FLD10))) && (black_man & (FLD14)))
		{	if ((white_or_empty & BLD26) && (white_or_empty & BLD27) && (white_man & (BLD26|BLD27)) && (empty_field & FLD20)) black_edge_36 = -400;
			else                                                                                                              black_edge_36 = -slow_down[node->total_nr_of_pieces];
		}
		if ((all_black((FLD16|FLD26|FLD22|FLD18))) &&  (all_white((FLD27|FLD31|FLD32|FLD36|FLD37)))) white_edge_36 -= 300;
		if ((all_white((BLD16|BLD26|BLD22|BLD18))) &&  (all_black((BLD27|BLD31|BLD32|BLD36|BLD37)))) black_edge_36 -= 300;

		if ((black_man & FLD35) && (white_man & FLD34) && all_bits_set(empty_field,((FLD40|FLD44|FLD45|FLD49|FLD50))))
		{	if ((black_or_empty & FLD25) && (black_or_empty & FLD24) && (black_man &(FLD25|FLD24)) && (empty_field & FLD30)) white_edge_35 = -400;
			else                                                                                                             white_edge_35 = -100;
		}
		if ((white_man & FLD16) && (black_man & FLD17) && all_bits_set(empty_field, ((FLD11|FLD6|FLD7|FLD1|FLD2))))
		{	if ((white_or_empty & BLD25) && (white_or_empty & BLD24) && (white_man & (BLD25|BLD24)) && (empty_field & BLD30)) black_edge_35 = -400;
			else                                                                                                              black_edge_35 = -100;
		}

		if (all_empty(w4(40,44,45,50)))
			if (empty_field & FLD49) white_edge_35 -= slow_down[node->total_nr_of_pieces];
			else white_edge_35 -= 50*slow_down[node->total_nr_of_pieces]/100;

		if (all_empty(b4(40,44,45,50)))
			if (empty_field & BLD49) black_edge_35 -= slow_down[node->total_nr_of_pieces];
			else black_edge_35 -= 50*slow_down[node->total_nr_of_pieces]/100;

		if (all_black(w4(16,21,26,27)) && all_white(w2(36,37)) && all_empty(w5(31,41,42,46,47))) white_edge_35 -= 1500;
		if (all_white(b4(16,21,26,27)) && all_black(b2(36,37)) && all_empty(b5(31,41,42,46,47))) black_edge_35 -= 1500;

		if ((black_man & FLD35) && (white_man & FLD45) && all_bits_set(empty_field,((FLD40|FLD44|FLD34|FLD49|FLD50)))) white_edge_35 = -50;
		if ((white_man & FLD16) && (black_man & FLD6) &&  all_bits_set(empty_field,((FLD11|FLD7 |FLD17|FLD2 |FLD1 )))) black_edge_35 = -50;

		if (!(white_man & (FLD34|FLD35|FLD39|FLD40|FLD43|FLD44|FLD45|FLD48|FLD49|FLD50))) white_edge_35 = -4 * slow_down[node->total_nr_of_pieces];
		if (!(black_man & (BLD34|BLD35|BLD39|BLD40|BLD43|BLD44|BLD45|BLD48|BLD49|BLD50))) black_edge_35 = -4 * slow_down[node->total_nr_of_pieces];

		if ((all_black((FLD6|FLD11|FLD16))) && (black_man & (FLD17|FLD21|FLD26)) &&  (all_white((FLD27|FLD31|FLD32))) && all_empty((FLD36|FLD41|FLD46))) white_edge_35 = -2 * slow_down[node->total_nr_of_pieces];
		if ((all_white((BLD6|BLD11|BLD16))) && (white_man & (BLD17|BLD21|BLD26)) &&  (all_black((BLD27|BLD31|BLD32))) && all_empty((BLD36|BLD41|BLD46))) black_edge_35 = -2 * slow_down[node->total_nr_of_pieces];


		if ((all_black((FLD15|FLD25|FLD19|FLD13))) && (all_white((FLD24|FLD30|FLD29|FLD35|FLD34)))) white_edge_35 -= 300;
		if ((all_white((BLD15|BLD25|BLD19|BLD13))) && (all_black((BLD24|BLD30|BLD29|BLD35|BLD34)))) black_edge_35 -= 300;

		if (all_empty((FLD4|FLD5|FLD10|FLD14|FLD15)))
			if (black_man & (FLD3|FLD9))
			     black_edge_36 -=  50*slow_down[node->total_nr_of_pieces]/100;
			else black_edge_36 -= 150*slow_down[node->total_nr_of_pieces]/100;
		if (all_empty((BLD4|BLD5|BLD10|BLD14|BLD15)))
			if (white_man & (BLD3|BLD9))
				 white_edge_36 -=  50*slow_down[node->total_nr_of_pieces]/100;
			else white_edge_36 -= 150*slow_down[node->total_nr_of_pieces]/100;

		if ((white_man & FLD26) && (all_empty((FLD1|FLD6|FLD21)))  && (all_bits_set(black_man, (TRIANGLE_11)))) black_11_16_17 = -400;
		if ((black_man & FLD25) && (all_empty((FLD50|FLD45|FLD30))) && (all_bits_set(white_man, (TRIANGLE_40)))) white_11_16_17 = -400;

		if ((black_man & BLD24) && (!(all_white((BLD2|BLD8|BLD13))))) white_2_8_13 = -1*dikke_stand[node->total_nr_of_pieces];
		if ((white_man & FLD24) && (!(all_black((FLD2|FLD8|FLD13))))) black_2_8_13 = -1*dikke_stand[node->total_nr_of_pieces];

		// *+* centrum spel -------------------------------------------------------------------------------------------
		if (all_bits_set(white_man, ((FLD22 | FLD23)))){
			if (white_man & FLD28) white_tandem += (120*dikke_stand[node->total_nr_of_pieces])/100; else white_tandem += (80*dikke_stand[node->total_nr_of_pieces])/100;
		}
		else if (all_bits_set(white_man, ((FLD23 | FLD24)))){
			if (white_man & FLD29) white_tandem += (120*dikke_stand[node->total_nr_of_pieces])/100; else white_tandem += (80*dikke_stand[node->total_nr_of_pieces])/100;
		}
		else if((all_white((FLD30|FLD27))) && (all_empty((FLD28|FLD29)))) white_tandem -= (50*dikke_stand[node->total_nr_of_pieces])/100;

		if (all_bits_set(black_man, ((FLD29 | FLD28)))) {
			if (black_man & FLD23) black_tandem += (120*dikke_stand[node->total_nr_of_pieces])/100; else black_tandem += (80*dikke_stand[node->total_nr_of_pieces])/100;
		}
		else if (all_bits_set(black_man, ((FLD28 | FLD27)))) {
			if (black_man & FLD22) black_tandem += (120*dikke_stand[node->total_nr_of_pieces])/100; else black_tandem += (80*dikke_stand[node->total_nr_of_pieces])/100;
		}
		else if((all_black((BLD30|BLD27))) && (all_empty((BLD28|BLD29)))) black_tandem -= (50*dikke_stand[node->total_nr_of_pieces])/100;

		// *+* ketting stelling ----------------------------------------------------------------------------------------------

		if ((all_white((FLD27|FLD28|FLD22))) && (all_black((FLD11|FLD17|FLD13|FLD18)))) black_ketting_stelling++;
		if ((all_white((FLD28|FLD29|FLD23))) && (all_black((FLD12|FLD18|FLD14|FLD19)))) black_ketting_stelling++;
		if ((all_white((FLD29|FLD30|FLD24))) && (all_black((FLD13|FLD19|FLD15|FLD20)))) black_ketting_stelling++;

		if ((all_white((FLD31|FLD32|FLD27))) && (all_black((FLD16|FLD21|FLD18|FLD22)))) black_ketting_stelling++;
		if ((all_white((FLD32|FLD33|FLD28))) && (all_black((FLD17|FLD22|FLD19|FLD23)))) black_ketting_stelling++;
		if ((all_white((FLD33|FLD34|FLD29))) && (all_black((FLD18|FLD23|FLD20|FLD24)))) black_ketting_stelling++;

		//black
		if ((all_black((BLD27|BLD28|BLD22))) && (all_white((BLD11|BLD17|BLD13|BLD18)))) white_ketting_stelling++;
		if ((all_black((BLD28|BLD29|BLD23))) && (all_white((BLD12|BLD18|BLD14|BLD19)))) white_ketting_stelling++;
		if ((all_black((BLD29|BLD30|BLD24))) && (all_white((BLD13|BLD19|BLD15|BLD20)))) white_ketting_stelling++;

		if ((all_black((BLD31|BLD32|BLD27))) && (all_white((BLD16|BLD21|BLD18|BLD22)))) white_ketting_stelling++;
		if ((all_black((BLD32|BLD33|BLD28))) && (all_white((BLD17|BLD22|BLD19|BLD23)))) white_ketting_stelling++;
		if ((all_black((BLD33|BLD34|BLD29))) && (all_white((BLD18|BLD23|BLD20|BLD24)))) white_ketting_stelling++;




		// *+* (halve hek) stelling ? ------------------------------------------------------------------------------------------

		if ((all_white(WHITE_WHITE_HALVE_HEK)) && (all_black(WHITE_BLACK_HALVE_HEK)) && all_bits_set(empty_field, (FLD26 | FLD21)))
		{	char cnt; count_bits(white_man & DIAGONAL, cnt);
			if (cnt < 2) {
				if (cnt == 0) white_halve_hek = 400;
				else white_halve_hek = 200;
			} else if (cnt >2)
				if (cnt==3) white_halve_hek = -50;
				else white_halve_hek = -100;
		}

		if (all_bits_set(black_man, BLACK_BLACK_HALVE_HEK) && all_bits_set(white_man, BLACK_WHITE_HALVE_HEK) && all_bits_set(empty_field, (FLD25 | FLD30)))
		{	char cnt; count_bits(black_man & DIAGONAL, cnt);
			if (cnt < 2) {
				if (cnt == 0) black_halve_hek = 400;
				else black_halve_hek = 200;
			} else if (cnt >2)
				if (cnt==3) black_halve_hek = -50;
				else black_halve_hek = -100;

		}

		// *+* gesloten hek stelling ? --------------------------------------------------------------------------

		if (all_white(w4(36,31,26,27)) && all_black(w4(22,16,17,18)) && (empty_field & FLD21) && (count_white(w4(46,41,37,32)) <= 3) &&
				(count_white(w4(46,41,37,32)) < count_black(w5(1,6,7,11,12)))) white_halve_hek += 300;

		if (all_black(b4(36,31,26,27)) && all_white(b4(22,16,17,18)) && (empty_field & BLD21) && (count_black(b4(46,41,37,32)) <= 3) &&
				(count_black(b4(46,41,37,32)) < count_white(b5(1,6,7,11,12)))) black_halve_hek += 300;

		// *+* calc tempi --------------------------------------------------------------------------------------------

		add_count_bits((white_man & LINE10), white_tempi, 1)
		add_count_bits((white_man & LINE9), white_tempi, 2)
		add_count_bits((white_man & LINE8), white_tempi, 3)
		add_count_bits((white_man & LINE7), white_tempi, 4)
		add_count_bits((white_man & LINE6), white_tempi, 5)
		add_count_bits((white_man & LINE5), white_tempi, 6)
		add_count_bits((white_man & LINE4), white_tempi, 7)
		add_count_bits((white_man & LINE3), white_tempi, 8)
		add_count_bits((white_man & LINE2), white_tempi, 9)
		add_count_bits((black_man & LINE1), black_tempi, 1)
		add_count_bits((black_man & LINE2), black_tempi, 2)
		add_count_bits((black_man & LINE3), black_tempi, 3)
		add_count_bits((black_man & LINE4), black_tempi, 4)
		add_count_bits((black_man & LINE5), black_tempi, 5)
		add_count_bits((black_man & LINE6), black_tempi, 6)
		add_count_bits((black_man & LINE7), black_tempi, 7)
		add_count_bits((black_man & LINE8), black_tempi, 8)
		add_count_bits((black_man & LINE9), black_tempi, 9)

		// calc tempi

		//if (node->nr_of_white_man > 0) white_tempi_bonus = (white_tempi * 200)*(slow_down[node->total_nr_of_pieces] +10) / (110*node->nr_of_white_man);
		//if (node->nr_of_black_man > 0) black_tempi_bonus = (black_tempi * 200)*(slow_down[node->total_nr_of_pieces] +10) / (110*node->nr_of_black_man);

		white_tempi_bonus = (white_tempi * 10)*(down_to_50[node->total_nr_of_pieces]) / 100;
		black_tempi_bonus = (black_tempi * 10)*(down_to_50[node->total_nr_of_pieces]) / 100;
//		printf("tempi %d, %d, %d\n",white_tempi, black_tempi, down_to_50[node->total_nr_of_pieces]);
/*
		// forward compactness

		int forward_white[9], forward_black[9];

		forward_white[0] = count(white_man & LINE10);
		forward_white[1] = count(white_man & LINE9);
		forward_white[2] = count(white_man & LINE8);
		forward_white[3] = count(white_man & LINE7);
		forward_white[4] = count(white_man & LINE6);
		forward_white[5] = count(white_man & LINE5);
		forward_white[6] = count(white_man & LINE4);
		forward_white[7] = count(white_man & LINE3);
		forward_white[8] = count(white_man & LINE2);

		forward_black[0] = count(black_man & LINE1);
		forward_black[1] = count(black_man & LINE2);
		forward_black[2] = count(black_man & LINE3);
		forward_black[3] = count(black_man & LINE4);
		forward_black[4] = count(black_man & LINE5);
		forward_black[5] = count(black_man & LINE6);
		forward_black[6] = count(black_man & LINE7);
		forward_black[7] = count(black_man & LINE8);
		forward_black[8] = count(black_man & LINE9);

		float forward_white_average = 0.0, forward_black_average = 0.0;

		for (int i = 0; i<9; i++){
			forward_white_average += (float) (forward_white[i])*((float) i + 0.5);
			forward_black_average += (float) (forward_black[i])*((float) i + 0.5);
		}
		forward_white_average /= node->nr_of_white_man;
		forward_black_average /= node->nr_of_black_man;

		float forward_white_median,  forward_black_median;
		float forward_halfway_white, forward_halfway_black;
		float forward_count_white = 0, forward_count_black = 0;

		forward_halfway_white = ((float) node->nr_of_white_man)/2;
		forward_halfway_black = ((float) node->nr_of_black_man)/2;

		int z;

		for (z=0; z<9; z++) {
			if (forward_count_white + (float) forward_white[z] >= forward_halfway_white) break;
			else forward_count_white += (float) forward_white[z];
		}
		forward_white_median = (float) z + (forward_halfway_white - forward_count_white)/(float) forward_white[z];

		for (z=0; z<9; z++) {
			if (forward_count_black + (float) forward_black[z] >= forward_halfway_black) break;
			else forward_count_black += (float) forward_black[z];
		}
		forward_black_median = (float) z + (forward_halfway_black - forward_count_black)/(float) forward_black[z];

		//printf("white average = %5.3f, median = %5.3f, q = %5.3f, temp= %d\n", forward_white_average, forward_white_median, forward_white_median/forward_white_average, white_tempi_bonus);
		//printf("black average = %5.3f, median = %5.3f, q = %5.3f, temp= %d\n", forward_black_average, forward_black_median, forward_black_median/forward_black_average, black_tempi_bonus);

		white_tempi_bonus = (int) (((float) white_tempi_bonus) * forward_white_median / forward_white_average);
		black_tempi_bonus = (int) (((float) black_tempi_bonus) * forward_black_median / forward_black_average);
*/
		// *+* formaties langs de kant die meestal slecht zijn --------------------------------------------------------------
		// white


		if      (all_white(w5(36,31,26,27,22)))                            white_253035 -= 100;
		else if (all_white(w4(36,31,27,22)) && !(all_black(w3(16,21,26)))) white_253035 -= 75;
		else if (all_white(w4(36,31,26,27)) &&   all_empty(w2(16,21)))     white_253035 -= 75;
		else if (all_white(w3(36,31,27))    &&   all_empty(w2(16,21)))     white_253035 -= 50;

		if      (all_white(w5(26,21,17,16,12)))                            white_253035 -= 100;
		else if (all_white(w4(26,21,17,12)) && !(all_black(w3( 6,11,16)))) white_253035 -= 75;
		else if (all_white(w4(26,21,17,16)) &&   all_empty(w2( 6,11)))     white_253035 -= 75;
		else if (all_white(w3(26,21,17))    &&   all_empty(w2( 6,11)))     white_253035 -= 50;

		if      (all_white(w3(16,11, 7)))                                  white_253035 -= 50;

		if      (all_white(w5(35,30,24,19,25)))                            white_253035 -= 100;
		else if (all_white(w4(35,30,24,19)) && !(all_black(w3(15,20,25)))) white_253035 -= 75;
		else if (all_white(w4(35,30,24,25)) &&   all_empty(w2(15,20)))     white_253035 -= 75;
		else if (all_white(w3(35,30,24))    &&   all_empty(w2(15,20)))     white_253035 -= 50;

		if      (all_white(w5(25,20,14, 9,15)))                            white_253035 -= 100;
		else if (all_white(w4(25,20,14, 9)) && !(all_black(w3( 5,10,15)))) white_253035 -= 75;
		else if (all_white(w4(25,20,14,15)) &&   all_empty(w2( 5,10)))     white_253035 -= 75;
		else if (all_white(w3(25,20,14))    &&   all_empty(w2( 5,10)))     white_253035 -= 50;


		// black


		if      (all_black(b5(36,31,26,27,22)))                            black_253035 -= 100;
		else if (all_black(b4(36,31,27,22)) && !(all_white(b3(16,21,26)))) black_253035 -= 75;
		else if (all_black(b4(36,31,26,27)) &&   all_empty(b2(16,21)))     black_253035 -= 75;
		else if (all_black(b3(36,31,27))    &&   all_empty(b2(16,21)))     black_253035 -= 50;

		if      (all_black(b5(26,21,17,16,12)))                            black_253035 -= 100;
		else if (all_black(b4(26,21,17,12)) && !(all_white(b3( 6,11,16)))) black_253035 -= 75;
		else if (all_black(b4(26,21,17,16)) &&   all_empty(b2( 6,11)))     black_253035 -= 75;
		else if (all_black(b3(26,21,17))    &&   all_empty(b2( 6,11)))     black_253035 -= 50;

		if      (all_black(b3(16,11, 7)))                                  black_253035 -= 50;

		if      (all_black(b5(35,30,24,19,25)))                            black_253035 -= 100;
		else if (all_black(b4(35,30,24,19)) && !(all_white(b3(15,20,25)))) black_253035 -= 75;
		else if (all_black(b4(35,30,24,25)) &&   all_empty(b2(15,20)))     black_253035 -= 75;
		else if (all_black(b3(35,30,24))    &&   all_empty(b2(15,20)))     black_253035 -= 50;

		if      (all_black(b5(25,20,14, 9,15)))                            black_253035 -= 100;
		else if (all_black(b4(25,20,14, 9)) && !(all_white(b3( 5,10,15)))) black_253035 -= 75;
		else if (all_black(b4(25,20,14,15)) &&   all_empty(b2( 5,10)))     black_253035 -= 75;
		else if (all_black(b3(25,20,14))    &&   all_empty(b2( 5,10)))     black_253035 -= 50;





		// *+* slechte bindingen  ------------------------------------------------------------------------------------



		// bindingen v2
		// roozenburg white
		if ((all_white((FLD27|FLD29|FLD24))) && (all_black((FLD15|FLD18|FLD23))) &&
			(!(white_man & LINE01_05_R)) && (white_tempi  < black_tempi) && (empty_field & FLD28))
				white_slechte_binding = 20*(white_tempi - black_tempi)*dikke_stand[node->total_nr_of_pieces]/100;
		// anders
		else if ((all_white((FLD27|FLD29)) ) && !(white_man & LINE01_05) && ((white_tempi - 4) < black_tempi)) {
			if (empty_field & FLD28)
				white_slechte_binding = 20*(white_tempi - 4 - black_tempi)*dikke_stand[node->total_nr_of_pieces]/100;
			else if (white_man & FLD28)
				white_slechte_binding = 10*(white_tempi - 4 - black_tempi)*dikke_stand[node->total_nr_of_pieces]/100;
		}
/*		else if (all_white((FLD26|FLD28)) && !(white_man & LINE01_05) && ((white_tempi - 4) < black_tempi)) {
			if (empty_field & FLD27)
				white_slechte_binding = 20*(white_tempi - 4 - black_tempi)*dikke_stand[node->total_nr_of_pieces]/100;
		}
*/		//roozenburg ??
		if ((all_black((BLD27|BLD29|BLD24))) && (all_white((BLD15|BLD18|BLD23))) &&
			!(black_man & LINE06_10_R) && (black_tempi  < white_tempi) && (empty_field & BLD28))
				black_slechte_binding = 20*(black_tempi - white_tempi)*dikke_stand[node->total_nr_of_pieces]/100;
		// anders
		else if ((all_black((BLD27|BLD29)) ) && !(black_man & LINE06_10) && ((black_tempi - 4) < white_tempi)) {
			if (empty_field & BLD28)
				black_slechte_binding = 20*(black_tempi - 4 - white_tempi)*dikke_stand[node->total_nr_of_pieces]/100;
			else if (black_man & BLD28)
				black_slechte_binding = 10*(black_tempi - 4 - white_tempi)*dikke_stand[node->total_nr_of_pieces]/100;
		}
/*		else if (all_black((BLD26|BLD28)) && !(black_man & LINE06_10) && ((black_tempi - 4) < white_tempi)) {
			if (empty_field & BLD27)
				black_slechte_binding = 20*(black_tempi - 4 - white_tempi)*dikke_stand[node->total_nr_of_pieces]/100;
		}
*/

		// *+* distribution of men -------------------------------------------------------------------------------------

		{	int t2_1_5, t2_2_5, t3_1_3, t3_2_4, t3_3_3, t5_1_2, t5_2_2, t5_3_2, t5_4_2, t5_5_2;
			int tc2, tc3, tc5;
			int i2, i3, i4, i5;

			i2 =  8*(node->nr_of_white_man - 1)/20 - 1;
			i3 = 12*(node->nr_of_white_man - 1)/20 - 1;
			i4 = 16*(node->nr_of_white_man - 1)/20 - 1;
			i5 = 20*(node->nr_of_white_man - 1)/20 - 1;

			//printf("i2=%d, i3=%d, i4=%d, i5=%d\n",i2,i3,i4,i5);

			if (node->nr_of_white_man >= 4){
				count_bits(white_man & W2_1_5, t2_1_5);
				count_bits(white_man & W2_2_5, t2_2_5);
				count_bits(white_man & W3_1_3, t3_1_3);
				count_bits(white_man & W3_2_4, t3_2_4);
				count_bits(white_man & W3_3_3, t3_3_3);
				count_bits(white_man & W5_1_2, t5_1_2);
				count_bits(white_man & W5_2_2, t5_2_2);
				count_bits(white_man & W5_3_2, t5_3_2);
				count_bits(white_man & W5_4_2, t5_4_2);
				count_bits(white_man & W5_5_2, t5_5_2);

				tc2 = 5*(
					distribution[i5][t2_1_5] +
					distribution[i5][t2_2_5]);
				tc3 = 0;
					if (distribution[i3][t3_1_3] < 0) tc3 += -2 * distribution[i3][t3_1_3]; else tc3 += 5 * distribution[i3][t3_1_3];
					if (distribution[i4][t3_2_4] < 0) tc3 += -5 * distribution[i4][t3_2_4]; else tc3 += 1 * distribution[i4][t3_2_4];
					if (distribution[i3][t3_3_3] < 0) tc3 += -2 * distribution[i3][t3_3_3]; else tc3 += 5 * distribution[i3][t3_3_3];
				tc5 = 0;
				if (distribution[i2][t5_1_2] < 0) tc5 += -2 * distribution[i2][t5_1_2]; else tc5 += 5 * distribution[i2][t5_1_2];
				if (distribution[i2][t5_2_2] < 0) tc5 += -3 * distribution[i2][t5_2_2]; else tc5 += 3 * distribution[i2][t5_2_2];
				if (distribution[i2][t5_3_2] < 0) tc5 += -5 * distribution[i2][t5_3_2]; else tc5 += 2 * distribution[i2][t5_3_2];
				if (distribution[i2][t5_4_2] < 0) tc5 += -3 * distribution[i2][t5_4_2]; else tc5 += 3 * distribution[i2][t5_4_2];
				if (distribution[i2][t5_5_2] < 0) tc5 += -2 * distribution[i2][t5_5_2]; else tc5 += 5 * distribution[i2][t5_5_2];



				white_fit = -(tc2*60 + tc3*15 + tc5*7)/5;//*dikke_stand[node->total_nr_of_pieces]/100;

/*				printf("white:\n");
				printf("tc2 = %d, (%d %d)[%d %d]\n", tc2, t2_1_5, t2_2_5,distribution[i5][t2_1_5],distribution[i5][t2_2_5]);
				printf("tc3 = %d, (%d %d %d)[%d %d %d]\n", tc3, t3_1_3, t3_2_4, t3_3_3,distribution[i3][t3_1_3],distribution[i4][t3_2_4],distribution[i3][t3_3_3]);
				printf("tc5 = %d, (%d %d %d %d %d)[%d %d %d %d %d]\n", tc5, t5_1_2, t5_2_2, t5_3_2, t5_4_2, t5_5_2, distribution[i2][t5_1_2], distribution[i2][t5_2_2], distribution[i2][t5_3_2], distribution[i2][t5_4_2], distribution[i2][t5_5_2]);
				printf("total %d\n", tc2*60 + tc3*20  +tc5*10);
				printf("\n");
*/
				}



		// distribution of black men

			if (node->nr_of_black_man >= 4){
				count_bits(black_man & W2_1_5, t2_1_5);
				count_bits(black_man & W2_2_5, t2_2_5);
				count_bits(black_man & W3_1_3, t3_1_3);
				count_bits(black_man & W3_2_4, t3_2_4);
				count_bits(black_man & W3_3_3, t3_3_3);
				count_bits(black_man & W5_1_2, t5_1_2);
				count_bits(black_man & W5_2_2, t5_2_2);
				count_bits(black_man & W5_3_2, t5_3_2);
				count_bits(black_man & W5_4_2, t5_4_2);
				count_bits(black_man & W5_5_2, t5_5_2);

				i2 =  8*(node->nr_of_black_man - 1)/20 - 1;
				i3 = 12*(node->nr_of_black_man - 1)/20 - 1;
				i4 = 16*(node->nr_of_black_man - 1)/20 - 1;
				i5 = 20*(node->nr_of_black_man - 1)/20 - 1;

				tc2 = 5*(
					distribution[i5][t2_1_5] +
					distribution[i5][t2_2_5]);
				tc3 = 0;
					if (distribution[i3][t3_1_3] < 0) tc3 += -2 * distribution[i3][t3_1_3]; else tc3 += 5 * distribution[i3][t3_1_3];
					if (distribution[i4][t3_2_4] < 0) tc3 += -5 * distribution[i4][t3_2_4]; else tc3 += 1 * distribution[i4][t3_2_4];
					if (distribution[i3][t3_3_3] < 0) tc3 += -2 * distribution[i3][t3_3_3]; else tc3 += 5 * distribution[i3][t3_3_3];
				tc5 = 0;
				if (distribution[i2][t5_1_2] < 0) tc5 += -2 * distribution[i2][t5_1_2]; else tc5 += 5 * distribution[i2][t5_1_2];
				if (distribution[i2][t5_2_2] < 0) tc5 += -3 * distribution[i2][t5_2_2]; else tc5 += 3 * distribution[i2][t5_2_2];
				if (distribution[i2][t5_3_2] < 0) tc5 += -5 * distribution[i2][t5_3_2]; else tc5 += 2 * distribution[i2][t5_3_2];
				if (distribution[i2][t5_4_2] < 0) tc5 += -3 * distribution[i2][t5_4_2]; else tc5 += 3 * distribution[i2][t5_4_2];
				if (distribution[i2][t5_5_2] < 0) tc5 += -2 * distribution[i2][t5_5_2]; else tc5 += 5 * distribution[i2][t5_5_2];



				black_fit = -(tc2*60 + tc3*15 + tc5*7)/5;//*slow_down[node->total_nr_of_pieces]/100;

/*				printf("black:\n");
				printf("tc2 = %d, (%d %d)[%d %d]\n", tc2, t2_1_5, t2_2_5,distribution[i5][t2_1_5],distribution[i5][t2_2_5]);
				printf("tc3 = %d, (%d %d %d)[%d %d %d]\n", tc3, t3_1_3, t3_2_4, t3_3_3,distribution[i3][t3_1_3],distribution[i4][t3_2_4],distribution[i3][t3_3_3]);
				printf("tc5 = %d, (%d %d %d %d %d)[%d %d %d %d %d]\n", tc5, t5_1_2, t5_2_2, t5_3_2, t5_4_2, t5_5_2, distribution[i2][t5_1_2], distribution[i2][t5_2_2], distribution[i2][t5_3_2], distribution[i2][t5_4_2], distribution[i2][t5_5_2]);
				printf("total %d\n", tc2*60 + tc3*20  +tc5*10);
				printf("\n");
*/
			}
		}
		// *+* centrum controle, first simple try -------------------------------------------------------------------------------

			white_centrum = 30*count((CENTRUM & white_man));
			black_centrum = 30*count((CENTRUM & black_man));


		// *+* voorpost calculations -------------------------------------------------------------------------------------------
//			white_voorpost = slow_down[node->total_nr_of_pieces]*calc_white_voorpost(white_man, black_man, empty_field, node->status & IS_WHITE_ON_MOVE)/100;
//			black_voorpost = slow_down[node->total_nr_of_pieces]*calc_black_voorpost(white_man, black_man, empty_field, node->status & IS_WHITE_ON_MOVE)/100;
			white_voorpost = calc_white_voorpost(white_man, black_man, empty_field, node->status & IS_WHITE_ON_MOVE);
			black_voorpost = calc_black_voorpost(white_man, black_man, empty_field, node->status & IS_WHITE_ON_MOVE);

		// *+* very simple 1 piece locks 2 pieces situations -------------------------------------------------------------------

			// *+* begin *+* one_keeps_two ------------------------------------------------------------------------------------
// ----  white part -----
// against the damlijn
/*
       x   .   x   .   .       .   x   .   x   .       .   .   x   .   x
     .   .   .   .   .       .   .   .   .   .       .   .   .   .   .
       x   o   x   .   .       .   x   o   x   .       .   .   x   o   x
     .   .   .   .   .       .   .   .   .   .       .   .   .   .   .
       .   .   .   .   .       .   .   .   .   .       .   .   .   .   .
     .   .   .   .   .       .   .   .   .   .       .   .   .   .   .
       .   .   .   .   .       .   .   .   .   .       .   .   .   .   .
     .   .   .   .   .       .   .   .   .   .       .   .   .   .   .
       .   .   .   .   .       .   .   .   .   .       .   .   .   .   .
     .   .   .   .   .       .   .   .   .   .       .   .   .   .   .
*/

if (((white_man & w1(12)) && (black_man & w2(1,11)) && (black_man & w2(3,13)) && all_empty(w3(2,7,8))) ||
    ((white_man & w1(13)) && (black_man & w2(2,12)) && (black_man & w2(4,14)) && all_empty(w3(3,8,9))) ||
    ((white_man & w1(14)) && (black_man & w2(3,13)) && (black_man & w2(5,15)) && all_empty(w3(4,9,10))))                    white_locked += 700;

/*
       .   .   .   .   .       .   .   .   .   .       .   .   .   .   .
     x   .   x   .   .       .   x   .   x   .       .   .   x   .   x
       .   .   .   .   .       .   .   .   .   .       .   .   .   .   .
     x   o   x   .   .       .   x   o   x   .       .   .   x   o   x
       .   .   .   .   .       .   .   .   .   .       .   .   .   .   .
     .   .   .   .   .       .   .   .   .   .       .   .   .   .   .
       .   .   .   .   .       .   .   .   .   .       .   .   .   .   .
     .   .   .   .   .       .   .   .   .   .       .   .   .   .   .
       .   .   .   .   .       .   .   .   .   .       .   .   .   .   .
     .   .   .   .   .       .   .   .   .   .       .   .   .   .   .
*/

else if
   (((white_man & w1(17)) && (black_man & w2(6,16)) && (black_man & w2(8,18)) && all_empty(w6(11,12,7,1,2,3))) ||
    ((white_man & w1(18)) && (black_man & w2(7,17)) && (black_man & w2(9,19)) && all_empty(w7(12,13,8,1,2,3,4))) ||
    ((white_man & w1(19)) && (black_man & w2(8,18)) && (black_man & w2(10,20)) && all_empty(w7(13,14,9,2,3,4,5))))          white_locked += 700;
/*
       .   .   .   .   .       .   .   .   .   .       .   .   .   .   .
     .   .   .   .   .       .   .   .   .   .       .   .   .   .   .
       x   .   x   .   .       .   x   .   x   .       .   .   x   .   x
     .   .   .   .   .       .   .   .   .   .       .   .   .   .   .
       x   o   x   .   .       .   x   o   x   .       .   .   x   o   x
     .   .   .   .   .       .   .   .   .   .       .   .   .   .   .
       .   .   .   .   .       .   .   .   .   .       .   .   .   .   .
     .   .   .   .   .       .   .   .   .   .       .   .   .   .   .
       .   .   .   .   .       .   .   .   .   .       .   .   .   .   .
     .   .   .   .   .       .   .   .   .   .       .   .   .   .   .
*/


else if
   (((white_man & w1(22)) && (black_man & w2(11,21)) && (black_man & w2(13,23)) && all_empty(w8(17,18,12,7,8,1,2,3))) ||
    ((white_man & w1(23)) && (black_man & w2(12,22)) && (black_man & w2(14,24)) && all_empty(w8(18,19,13,8,9,2,3,4))) ||
    ((white_man & w1(24)) && (black_man & w2(13,23)) && (black_man & w2(15,25)) && all_empty(w8(19,20,14,9,10,3,4,5))))      white_locked += 700;


//against the left edge

/*
       .   .   .   .   x       .   .   .   .   .       .   .   .   .   .
     .   .   .   .   .       .   .   .   .   .       .   .   .   .   .
       .   .   .   o   x       .   .   .   .   x       .   .   .   .   .
     .   .   .   .   .       .   .   .   .   .       .   .   .   .   .
       .   .   .   .   .       .   .   .   o   x       .   .   .   .   x
     .   .   .   .   .       .   .   .   .   .       .   .   .   .   .
       .   .   .   .   .       .   .   .   .   .       .   .   .   o   x
     .   .   .   .   .       .   .   .   .   .       .   .   .   .   .
       .   .   .   .   .       .   .   .   .   .       .   .   .   .   .
     .   .   .   .   .       .   .   .   .   .       .   .   .   .   .
*/

// if (((white_man & w1(14)) && all_black(w2(5,15)) && all_empty(w4(20,25,4,10))) is not proffitable, either there is a breakthrough, or a black_man at 3
if ((white_man & w1(24)) && all_black(w2(15,25)) && all_empty(w2(30,35)) && all_empty_or_white(w6(14,9,3,10,4,5))) white_locked += 400;
// if (((white_man & w1(34)) && all_black(w2(25,35)) && all_empty(w16(40,45,30,24,19,13,8,2,20,14,9,3,15,10,4,5))) not good, due to breakthrough risks


// against the right edge
/*
       .   .   .   .   .       .   .   .   .   .       .   .   .   .   .
     x   .   .   .   .       .   .   .   .   .       .   .   .   .   .
       .   .   .   .   .       .   .   .   .   .       .   .   .   .   .
     x   o   .   .   .       x   .   .   .   .       .   .   .   .   .
       .   .   .   .   .       .   .   .   .   .       .   .   .   .   .
     .   .   .   .   .       x   o   .   .   .       x   .   .   .   .
       .   .   .   .   .       .   .   .   .   .       .   .   .   .   .
     .   .   .   .   .       .   .   .   .   .       x   o   .   .   .
       .   .   .   .   .       .   .   .   .   .       .   .   .   .   .
     .   .   .   .   .       .   .   .   .   .       .   .   .   .   .
*/



//if (((white_man & w1(17)) && all_black(w2(6,16)) && all_empty(w6(21,26,11,7,2,1))) is not proffitable, either there is a breakthrough, or a black_man at 8
if   ((white_man & w1(27)) && all_black(w2(16,26)) && all_empty(w2(31,36)) && all_empty_or_white(w10(21,17,12,8,3,11,7,2,6,1))) white_locked += 400;
// with (white_man & w1(37)) there is nearly a breakthrough


// ----  black part -----
// against the damlijn
/*
       x   .   x   .   .       .   x   .   x   .       .   .   x   .   x
     .   .   .   .   .       .   .   .   .   .       .   .   .   .   .
       x   o   x   .   .       .   x   o   x   .       .   .   x   o   x
     .   .   .   .   .       .   .   .   .   .       .   .   .   .   .
       .   .   .   .   .       .   .   .   .   .       .   .   .   .   .
     .   .   .   .   .       .   .   .   .   .       .   .   .   .   .
       .   .   .   .   .       .   .   .   .   .       .   .   .   .   .
     .   .   .   .   .       .   .   .   .   .       .   .   .   .   .
       .   .   .   .   .       .   .   .   .   .       .   .   .   .   .
     .   .   .   .   .       .   .   .   .   .       .   .   .   .   .
*/

if (((black_man & w1(39)) && (white_man & w2(50,40)) && (white_man & w2(48,38)) && all_empty(w3(49,44,43))) ||
    ((black_man & w1(38)) && (white_man & w2(49,39)) && (white_man & w2(47,37)) && all_empty(w3(48,43,42))) ||
    ((black_man & w1(37)) && (white_man & w2(48,38)) && (white_man & w2(46,36)) && all_empty(w3(47,42,41))))                    black_locked += 700;

/*
       .   .   .   .   .       .   .   .   .   .       .   .   .   .   .
     x   .   x   .   .       .   x   .   x   .       .   .   x   .   x
       .   .   .   .   .       .   .   .   .   .       .   .   .   .   .
     x   o   x   .   .       .   x   o   x   .       .   .   x   o   x
       .   .   .   .   .       .   .   .   .   .       .   .   .   .   .
     .   .   .   .   .       .   .   .   .   .       .   .   .   .   .
       .   .   .   .   .       .   .   .   .   .       .   .   .   .   .
     .   .   .   .   .       .   .   .   .   .       .   .   .   .   .
       .   .   .   .   .       .   .   .   .   .       .   .   .   .   .
     .   .   .   .   .       .   .   .   .   .       .   .   .   .   .
*/

else if
   (((black_man & w1(34)) && (white_man & w2(45,35)) && (white_man & w2(43,33)) && all_empty(w6(40,39,44,50,49,48))) ||
    ((black_man & w1(33)) && (white_man & w2(44,34)) && (white_man & w2(42,32)) && all_empty(w7(39,38,43,50,49,48,47))) ||
    ((black_man & w1(32)) && (white_man & w2(43,33)) && (white_man & w2(41,31)) && all_empty(w7(38,37,42,49,48,47,46))))          black_locked += 700;
/*
       .   .   .   .   .       .   .   .   .   .       .   .   .   .   .
     .   .   .   .   .       .   .   .   .   .       .   .   .   .   .
       x   .   x   .   .       .   x   .   x   .       .   .   x   .   x
     .   .   .   .   .       .   .   .   .   .       .   .   .   .   .
       x   o   x   .   .       .   x   o   x   .       .   .   x   o   x
     .   .   .   .   .       .   .   .   .   .       .   .   .   .   .
       .   .   .   .   .       .   .   .   .   .       .   .   .   .   .
     .   .   .   .   .       .   .   .   .   .       .   .   .   .   .
       .   .   .   .   .       .   .   .   .   .       .   .   .   .   .
     .   .   .   .   .       .   .   .   .   .       .   .   .   .   .
*/


else if
   (((black_man & w1(29)) && (white_man & w2(40,30)) && (white_man & w2(38,28)) && all_empty(w8(34,33,39,44,43,50,49,48))) ||
    ((black_man & w1(28)) && (white_man & w2(39,29)) && (white_man & w2(37,27)) && all_empty(w8(33,32,38,43,42,49,48,47))) ||
    ((black_man & w1(27)) && (white_man & w2(38,28)) && (white_man & w2(36,26)) && all_empty(w8(32,31,37,42,41,48,47,46))))      black_locked += 700;


//against the left edge

/*
       .   .   .   .   x       .   .   .   .   .       .   .   .   .   .
     .   .   .   .   .       .   .   .   .   .       .   .   .   .   .
       .   .   .   o   x       .   .   .   .   x       .   .   .   .   .
     .   .   .   .   .       .   .   .   .   .       .   .   .   .   .
       .   .   .   .   .       .   .   .   o   x       .   .   .   .   x
     .   .   .   .   .       .   .   .   .   .       .   .   .   .   .
       .   .   .   .   .       .   .   .   .   .       .   .   .   o   x
     .   .   .   .   .       .   .   .   .   .       .   .   .   .   .
       .   .   .   .   .       .   .   .   .   .       .   .   .   .   .
     .   .   .   .   .       .   .   .   .   .       .   .   .   .   .
*/

// if (((black_man & w1(37)) && all_white(w2(46,36)) && all_empty(w4(31,26,47,41))) is not proffitable, either there is a breakthrough, or a white_man at 3
if ((black_man & w1(27)) && all_white(w2(36,26)) && all_empty(w2(21,16)) && all_empty_or_black(w6(37,42,48,41,47,46))) black_locked += 400;
// if (((black_man & w1(17)) && all_white(w2(26,16)) && all_empty(w16(11,6,21,27,32,38,43,49,31,37,42,48,36,41,47,46))) not good, due to breakthrough risks


// against the right edge
/*
       .   .   .   .   .       .   .   .   .   .       .   .   .   .   .
     x   .   .   .   .       .   .   .   .   .       .   .   .   .   .
       .   .   .   .   .       .   .   .   .   .       .   .   .   .   .
     x   o   .   .   .       x   .   .   .   .       .   .   .   .   .
       .   .   .   .   .       .   .   .   .   .       .   .   .   .   .
     .   .   .   .   .       x   o   .   .   .       x   .   .   .   .
       .   .   .   .   .       .   .   .   .   .       .   .   .   .   .
     .   .   .   .   .       .   .   .   .   .       x   o   .   .   .
       .   .   .   .   .       .   .   .   .   .       .   .   .   .   .
     .   .   .   .   .       .   .   .   .   .       .   .   .   .   .
*/



//if (((black_man & w1(34)) && all_white(w2(45,35)) && all_empty(w6(30,25,40,44,49,50))) is not proffitable, either there is a breakthrough, or a white_man at 8
if   ((black_man & w1(24)) && all_white(w2(35,25)) && all_empty(w2(20,15)) && all_empty_or_black(w10(30,34,39,43,48,40,44,49,45,50))) black_locked += 400;
// with (black_man & w1(14)) there is nearly a breakthrough


		// *+* end *+* one_keeps_two ------------------------------------------------------------------------------------

		// *+* begin *+* two_keeps_three ------------------------------------------------------------------------------------


		// simple 2 pieces locks 3 pieces situations
		{
			if ((all_white((FLD41|FLD47))) && all_black((FLD26|FLD31|FLD36)) && (all_empty((FLD27|FLD28|FLD32|FLD37|FLD46            )))) white_locked += 300;
			if ((all_white((FLD31|FLD37))) && all_black((FLD16|FLD21|FLD26)) && (all_empty((FLD17|FLD18|FLD22|FLD27|FLD36|FLD41|FLD46)))) white_locked += 300;
			if ((all_white((FLD21|FLD27))) && all_black(( FLD6|FLD11|FLD16)) && (all_empty((FLD7 |FLD8 |FLD12|FLD17|FLD26|FLD31|FLD36)))) white_locked += 300;
			if ((all_white((FLD24|FLD20))) && all_black(( FLD5|FLD10|FLD15)) && (all_empty((FLD3 |FLD4 |FLD9 |FLD14|FLD25|FLD30|FLD35)))) white_locked += 300;
			if ((all_white((FLD34|FLD30))) && all_black((FLD15|FLD20|FLD25)) && (all_empty((FLD13|FLD14|FLD19|FLD24|FLD35|FLD40|FLD45)))) white_locked += 300;
			if ((all_white((FLD44|FLD40))) && all_black((FLD25|FLD30|FLD35)) && (all_empty((FLD23|FLD24|FLD29|FLD34|FLD45|FLD50      )))) white_locked += 300;

			if ((all_black((BLD41|BLD47))) && all_white((BLD26|BLD31|BLD36)) && (all_empty((BLD27|BLD28|BLD32|BLD37|BLD46            )))) black_locked += 300;
			if ((all_black((BLD31|BLD37))) && all_white((BLD16|BLD21|BLD26)) && (all_empty((BLD17|BLD18|BLD22|BLD27|BLD36|BLD41|BLD46)))) black_locked += 300;
			if ((all_black((BLD21|BLD27))) && all_white(( BLD6|BLD11|BLD16)) && (all_empty((BLD7 |BLD8 |BLD12|BLD17|BLD26|BLD31|BLD36)))) black_locked += 300;
			if ((all_black((BLD24|BLD20))) && all_white(( BLD5|BLD10|BLD15)) && (all_empty((BLD3 |BLD4 |BLD9 |BLD14|BLD25|BLD30|BLD35)))) black_locked += 300;
			if ((all_black((BLD34|BLD30))) && all_white((BLD15|BLD20|BLD25)) && (all_empty((BLD13|BLD14|BLD19|BLD24|BLD35|BLD40|BLD45)))) black_locked += 300;
			if ((all_black((BLD44|BLD40))) && all_white((BLD25|BLD30|BLD35)) && (all_empty((BLD23|BLD24|BLD29|BLD34|BLD45|BLD50      )))) black_locked += 300;

			// *+* end *+* two_keeps_three ------------------------------------------------------------------------------------


			// *+* triangles along the edge

			if (all_black((FLD26|FLD36|FLD31)) && (all_empty((FLD46|FLD41|FLD37)))) black_253035 -= 50;
			if (all_black((FLD16|FLD26|FLD21)) && (all_empty((FLD36|FLD31|FLD27)))) black_253035 -= 50;

			if (all_black((FLD25|FLD30|FLD35)) && (all_empty((FLD45|FLD40|FLD34)))) black_253035 -= 50;
			else if ((all_white((FLD25|FLD30|FLD35))) && (white_tempi > black_tempi) && (black_man & FLD15))
				        white_253035 += imin(10, white_tempi - black_tempi)*(10*slow_down[node->total_nr_of_pieces])/100;

			if (all_black((FLD15|FLD20|FLD25)) && (all_empty((FLD35|FLD30|FLD24)))) black_253035 -= 50;


			if (all_white((BLD26|BLD36|BLD31)) && (all_empty((BLD46|BLD41|BLD37)))) black_253035 -= 50;
			if (all_white((BLD16|BLD26|BLD21)) && (all_empty((BLD36|BLD31|BLD27)))) white_253035 -= 50;

			if (all_white((BLD25|BLD30|BLD35)) && (all_empty((BLD45|BLD40|BLD34)))) white_253035 -= 50;
			else if ((all_black((BLD25|BLD30|BLD35))) && (black_tempi > white_tempi) && (white_man & BLD15))
				black_253035 += imin(10, black_tempi - white_tempi)*(10*slow_down[node->total_nr_of_pieces])/100;

			if (all_white((BLD15|BLD20|BLD25)) && (all_empty((BLD35|BLD30|BLD24)))) white_253035 -= 50;



/*
#define wcalc(nr, ow1, ow2, ow3, ow4,lab) white_cnt = white_check_counts_and_locks(node, white_man, black_man, &white_warning, nr, ow1, ow2, ow3, ow4); if (white_cnt) goto lab;

		// vleugel opsluiting
		{	int white_cnt = 0;
			int black_cnt = 0;
			int white_warning = 0, black_warning = 0;
			int tot_white = 0, tot_black = 0;

			if (all_bits_set(white_man, OW1_1) && (empty_or_white & (FLD28)) && (all_bits_set(black_man, OW2_1))) { wcalc(3, OW1_1, OW2_1, OW3_1, OW4_1, N1);}
			if (all_bits_set(white_man, OW1_2) && (empty_or_white & (FLD28)) && (all_bits_set(black_man, OW2_2))) { wcalc(3, OW1_2, OW2_2, OW3_2, OW4_2, N1);}
			if (all_bits_set(white_man, OW1_3) && (empty_or_white & (FLD22)) && (all_bits_set(black_man, OW2_3))) { wcalc(2, OW1_3, OW2_3, OW3_3, OW4_3, N1);}
			if (all_bits_set(white_man, OW1_4) && (empty_or_white & (FLD21)) && (all_bits_set(black_man, OW2_4)))
				if ((white_man & FLD28) && (empty_or_white & (FLD22))) { wcalc(1, OW1_4, OW2_4, OW3_4, OW4_4,   N1);}
				else if (all_bits_set(empty_field, (FLD22 | FLD28)))   { wcalc(0, OW1_4, OW2_4, OW3_4, OW4_4_2, N1);}
			if (all_bits_set(white_man, OW1_5) && (empty_or_white & (FLD17)) && (all_bits_set(black_man, OW2_5))) { wcalc(0, OW1_5, OW2_5, OW3_5, OW4_5, N1);}
N1:			//if (white_cnt > 0) printf("N1\n");
			tot_white += white_cnt; white_cnt = 0;
			if (all_bits_set(white_man, OW1_6) && (empty_or_white & (FLD38)) && (all_bits_set(black_man, OW2_6))) { wcalc(3, OW1_6, OW2_6, OW3_6, OW4_6, N2);}
			if (all_bits_set(white_man, OW1_7) && (empty_or_white & (FLD38)) && (all_bits_set(black_man, OW2_7))) { wcalc(3, OW1_7, OW2_7, OW3_7, OW4_7, N2);}
			if (all_bits_set(white_man, OW1_8) && (empty_or_white & (FLD32)) && (all_bits_set(black_man, OW2_8))) { wcalc(2, OW1_8, OW2_8, OW3_8, OW4_8, N2);}
			if (all_bits_set(white_man, OW1_9) && (empty_or_white & (FLD31)) && (all_bits_set(black_man, OW2_9)))
				if ((white_man & FLD38) && (empty_or_white & (FLD32))) { wcalc(1, OW1_9, OW2_9, OW3_9, OW4_9,   N2);}
				else if (all_bits_set(empty_field, (FLD32 | FLD38)))   { wcalc(0, OW1_9, OW2_9, OW3_9, OW4_9_2, N2);}
			if (all_bits_set(white_man, OW1_10) && (empty_or_white & (FLD27)) && (all_bits_set(black_man, OW2_10))) { wcalc(0, OW1_10, OW2_10, OW3_10, OW4_10, N2);}
			if (all_bits_set(white_man, OW1_11) && (empty_or_white & (FLD42)) && (all_bits_set(black_man, OW2_11))) { wcalc(2, OW1_11, OW2_11, OW3_11, OW4_11, N2);}
			if (all_bits_set(white_man, OW1_12) && (empty_or_white & (FLD42)) && (all_bits_set(black_man, OW2_12))) { wcalc(2, OW1_12, OW2_12, OW3_12, OW4_12, N2);}
			if (all_bits_set(white_man, OW1_13) && (empty_or_white & (FLD37)) && (all_bits_set(black_man, OW2_13))) { wcalc(0, OW1_13, OW2_13, OW3_13, OW4_13, N2);}
N2:			//if (white_cnt > 0) printf("N2\n");
			tot_white += white_cnt; white_cnt = 0;
			if (all_bits_set(white_man, OW1_14) && (empty_or_white & (FLD23)) && (all_bits_set(black_man, OW2_14))) { wcalc(3, OW1_14, OW2_14, OW3_14, OW4_14, N3);}
			if (all_bits_set(white_man, OW1_15) && (empty_or_white & (FLD23)) && (all_bits_set(black_man, OW2_15))) { wcalc(3, OW1_15, OW2_15, OW3_15, OW4_15, N3);}
			if (all_bits_set(white_man, OW1_16) && (empty_or_white & (FLD19)) && (all_bits_set(black_man, OW2_16))) { wcalc(2, OW1_16, OW2_16, OW3_16, OW4_16, N3);}
			if (all_bits_set(white_man, OW1_17) && (empty_or_white & (FLD20)) && (all_bits_set(black_man, OW2_17)))
				if ((white_man & FLD23) && (empty_or_white & (FLD19))) { wcalc(1, OW1_17, OW2_17, OW3_17, OW4_17,   N3);}
				else if (all_bits_set(empty_field, (FLD19 | FLD23))) { wcalc(0, OW1_17, OW2_17, OW3_17, OW4_17_2, N3);}
			if (all_bits_set(white_man, OW1_18) && (empty_or_white & (FLD14)) && (all_bits_set(black_man, OW2_18))) { wcalc(0, OW1_18, OW2_18, OW3_18, OW4_18, N3);}
N3:			//if (white_cnt > 0) printf("N3\n");
			tot_white += white_cnt; white_cnt = 0;
			if (all_bits_set(white_man, OW1_19) && (empty_or_white & (FLD33)) && (all_bits_set(black_man, OW2_19))) { wcalc(3, OW1_19, OW2_19, OW3_19, OW4_19, N4);}
			if (all_bits_set(white_man, OW1_20) && (empty_or_white & (FLD33)) && (all_bits_set(black_man, OW2_20))) { wcalc(3, OW1_20, OW2_20, OW3_20, OW4_20, N4);}
			if (all_bits_set(white_man, OW1_21) && (empty_or_white & (FLD29)) && (all_bits_set(black_man, OW2_21))) { wcalc(2, OW1_21, OW2_21, OW3_21, OW4_21, N4);}
			if (all_bits_set(white_man, OW1_22) && (empty_or_white & (FLD30)) && (all_bits_set(black_man, OW2_22)))
				if ((white_man & FLD33) && (empty_or_white & (FLD29))) { wcalc(1, OW1_22, OW2_22, OW3_22, OW4_22,   N4);}
				else if ((white_man & FLD30))  { wcalc(0, OW1_22, OW2_22, OW3_22, OW4_22_2, N4);}
			if (all_bits_set(white_man, OW1_23) && (empty_or_white & (FLD24)) && (all_bits_set(black_man, OW2_23))) { wcalc(0, OW1_23, OW2_23, OW3_23, OW4_23, N4);}
N4:			//if (white_cnt > 0) printf("N4\n");
			tot_white += white_cnt; white_cnt = 0;
			if (all_bits_set(white_man, OW1_24) && (empty_or_white & (FLD43)) && (all_bits_set(black_man, OW2_24))) { wcalc(2, OW1_24, OW2_24, OW3_24, OW4_24, N5);}
			if (all_bits_set(white_man, OW1_25) && (empty_or_white & (FLD43)) && (all_bits_set(black_man, OW2_25))) { wcalc(3, OW1_25, OW2_25, OW3_25, OW4_25, N5);}
			if (all_bits_set(white_man, OW1_26) && (empty_or_white & (FLD39)) && (all_bits_set(black_man, OW2_26))) { wcalc(2, OW1_26, OW2_26, OW3_26, OW4_26, N5);}
			if (all_bits_set(white_man, OW1_27) && (empty_or_white & (FLD40)) && (all_bits_set(black_man, OW2_27)))
				if ((white_man & FLD43) && (empty_or_white & (FLD39))) { wcalc(1, OW1_27, OW2_27, OW3_27, OW4_27,   N5);}
				else if (all_bits_set(empty_field, (FLD39 | FLD43)))   { wcalc(0, OW1_27, OW2_27, OW3_27, OW4_27_2, N5);}
			if (all_bits_set(white_man, OW1_28) && (empty_or_white & (FLD34)) && (all_bits_set(black_man, OW2_28))) { wcalc(0, OW1_28, OW2_28, OW3_28, OW4_28, N5);}
N5:			//if (white_cnt > 0) printf("N5\n");
			tot_white += white_cnt; white_cnt = 0;
			if (all_bits_set(white_man, OW1_29) && (empty_or_white & (FLD44)) && (all_bits_set(black_man, OW2_29))) { wcalc(0, OW1_29, OW2_29, OW3_29, OW4_29, N6);}
N6:			//printf("white_cnt = %d\n", white_cnt);
			tot_white += white_cnt;
//black:
#define bcalc(nr, ob1, ob2, ob3, ob4,lab) black_cnt = black_check_counts_and_locks(node, black_man, white_man, &black_warning, nr, ob1, ob2, ob3, ob4); if (black_cnt) goto lab;

			if (all_bits_set(black_man, OB1_1) && (empty_or_black & (FLD23)) && (all_bits_set(white_man, OB2_1))) { bcalc(3, OB1_1, OB2_1, OB3_1, OB4_1, N11);}
			if (all_bits_set(black_man, OB1_2) && (empty_or_black & (FLD23)) && (all_bits_set(white_man, OB2_2))) { bcalc(3, OB1_2, OB2_2, OB3_2, OB4_2, N11);}
			if (all_bits_set(black_man, OB1_3) && (empty_or_black & (FLD29)) && (all_bits_set(white_man, OB2_3))) { bcalc(2, OB1_3, OB2_3, OB3_3, OB4_3, N11);}
			if (all_bits_set(black_man, OB1_4) && (empty_or_black & (FLD30)) && (all_bits_set(white_man, OB2_4)))
				if ((black_man & FLD23) && (empty_or_black & (FLD29))) { bcalc(1, OB1_4, OB2_4, OB3_4, OB4_4,   N11);}
				else if (all_bits_set(empty_field, (FLD23 | FLD29)))   { bcalc(0, OB1_4, OB2_4, OB3_4, OB4_4_2, N11);}
			if (all_bits_set(black_man, OB1_5) && (empty_or_black & (FLD34)) && (all_bits_set(white_man, OB2_5))) { bcalc(0, OB1_5, OB2_5, OB3_5, OB4_5, N11);}
N11:		//if (black_cnt > 0) printf("N11\n");
			tot_black += black_cnt; black_cnt = 0;
			if (all_bits_set(black_man, OB1_6) && (empty_or_black & (FLD13)) && (all_bits_set(white_man, OB2_6))) { bcalc(0, OB1_6, OB2_6, OB3_6, OB4_6, N12);}
			if (all_bits_set(black_man, OB1_7) && (empty_or_black & (FLD13)) && (all_bits_set(white_man, OB2_7))) { bcalc(0, OB1_7, OB2_7, OB3_7, OB4_7, N12);}
			if (all_bits_set(black_man, OB1_8) && (empty_or_black & (FLD19)) && (all_bits_set(white_man, OB2_8))) { bcalc(2, OB1_8, OB2_8, OB3_8, OB4_8, N12);}
			if (all_bits_set(black_man, OB1_9) && (empty_or_black & (FLD20)) && (all_bits_set(white_man, OB2_9)))
				if ((black_man & FLD13) && (empty_or_black & (FLD19))) { bcalc(1, OB1_9, OB2_9, OB3_9, OB4_9,   N12);}
				else if (all_bits_set(empty_field, (FLD13 | FLD19)))   { bcalc(0, OB1_9, OB2_9, OB3_9, OB4_9_2, N12);}
			if (all_bits_set(black_man, OB1_10) && (empty_or_black & (FLD24)) && (all_bits_set(white_man, OB2_10))) { bcalc(0, OB1_10, OB2_10, OB3_10, OB4_10, N12);}
			if (all_bits_set(black_man, OB1_11) && (empty_or_black & (FLD9)) && (all_bits_set(white_man, OB2_11))) { bcalc(0, OB1_11, OB2_11, OB3_11, OB4_11, N12);}
			if (all_bits_set(black_man, OB1_12) && (empty_or_black & (FLD9)) && (all_bits_set(white_man, OB2_12))) { bcalc(2, OB1_12, OB2_12, OB3_12, OB4_12, N12);}
			if (all_bits_set(black_man, OB1_13) && (empty_or_black & (FLD14)) && (all_bits_set(white_man, OB2_13))) { bcalc(0, OB1_13, OB2_13, OB3_13, OB4_13, N12);}
N12:		//if (black_cnt > 0) printf("N12\n");
			tot_black += black_cnt; black_cnt = 0;
			if (all_bits_set(black_man, OB1_14) && (empty_or_black & (FLD28)) && (all_bits_set(white_man, OB2_14))) { bcalc(3, OB1_14, OB2_14, OB3_14, OB4_14, N13);}
			if (all_bits_set(black_man, OB1_15) && (empty_or_black & (FLD28)) && (all_bits_set(white_man, OB2_15))) { bcalc(3, OB1_15, OB2_15, OB3_15, OB4_15, N13);}
			if (all_bits_set(black_man, OB1_16) && (empty_or_black & (FLD32)) && (all_bits_set(white_man, OB2_16))) { bcalc(2, OB1_16, OB2_16, OB3_16, OB4_16, N13);}
			if (all_bits_set(black_man, OB1_17) && (empty_or_black & (FLD31)) && (all_bits_set(white_man, OB2_17)))
				if ((black_man & FLD28) && (empty_or_black & (FLD32))) { bcalc(1, OB1_17, OB2_17, OB3_17, OB4_17,   N13);}
				else if ((all_bits_set(empty_field, (FLD28 | FLD32))) && (empty_field & FLD21)) { bcalc(1, OB1_17, OB2_17, OB3_17, OB4_17_2, N13);}
			if (all_bits_set(black_man, OB1_18) && (empty_or_black & (FLD37)) && (all_bits_set(white_man, OB2_18))) { bcalc(0, OB1_18, OB2_18, OB3_18, OB4_18, N13);}
N13:		//if (black_cnt > 0) printf("N13\n");
			tot_black += black_cnt; black_cnt = 0;
			if (all_bits_set(black_man, OB1_19) && (empty_or_black & (FLD18)) && (all_bits_set(white_man, OB2_19))) { bcalc(3, OB1_19, OB2_19, OB3_19, OB4_19, N14);}
			if (all_bits_set(black_man, OB1_20) && (empty_or_black & (FLD18)) && (all_bits_set(white_man, OB2_20))) { bcalc(3, OB1_20, OB2_20, OB3_20, OB4_20, N14);}
			if (all_bits_set(black_man, OB1_21) && (empty_or_black & (FLD22)) && (all_bits_set(white_man, OB2_21))) { bcalc(2, OB1_21, OB2_21, OB3_21, OB4_21, N14);}
			if (all_bits_set(black_man, OB1_22) && (empty_or_black & (FLD21)) && (all_bits_set(white_man, OB2_22)))
				if ((black_man & FLD18) && (empty_or_black & (FLD22))) { bcalc(1, OB1_22, OB2_22, OB3_22, OB4_22,   N14);}
				else if (black_man & BLD30)   { bcalc(0, OB1_22, OB2_22, OB3_22, OB4_22_2, N14);}
			if (all_bits_set(black_man, OB1_23) && (empty_or_black & (FLD27)) && (all_bits_set(white_man, OB2_23))) { bcalc(0, OB1_23, OB2_23, OB3_23, OB4_23, N14);}
N14:		//if (black_cnt > 0) printf("N14\n");
			tot_black += black_cnt; black_cnt = 0;
			if (all_bits_set(black_man, OB1_24) && (empty_or_black & (FLD8)) && (all_bits_set(white_man, OB2_24))) { bcalc(2, OB1_24, OB2_24, OB3_24, OB4_24, N15);}
			if (all_bits_set(black_man, OB1_25) && (empty_or_black & (FLD8)) && (all_bits_set(white_man, OB2_25))) { bcalc(3, OB1_25, OB2_25, OB3_25, OB4_25, N15);}
			if (all_bits_set(black_man, OB1_26) && (empty_or_black & (FLD12)) && (all_bits_set(white_man, OB2_26))) { bcalc(2, OB1_26, OB2_26, OB3_26, OB4_26, N15);}
			if (all_bits_set(black_man, OB1_27) && (empty_or_black & (FLD11)) && (all_bits_set(white_man, OB2_27)))
				if ((black_man & FLD8) && (empty_or_black & (FLD12))) { bcalc(1, OB1_27, OB2_27, OB3_27, OB4_27,   N15);}
				else if (all_bits_set(empty_field, (FLD8 | FLD12)))   { bcalc(0, OB1_27, OB2_27, OB3_27, OB4_27_2, N15);}
			if (all_bits_set(black_man, OB1_28) && (empty_or_black & (FLD17)) && (all_bits_set(white_man, OB2_28))) { bcalc(0, OB1_28, OB2_28, OB3_28, OB4_28, N15);}
N15:		//if (black_cnt > 0) printf("N15\n");
			tot_black += black_cnt; black_cnt = 0;
			if (all_bits_set(black_man, OB1_29) && (empty_or_black & (FLD7)) && (all_bits_set(white_man, OB2_29))) { bcalc(0, OB1_29, OB2_29, OB3_29, OB4_29, N16);}
N16:		//printf("black_cnt = %d\n", black_cnt);
			tot_black += black_cnt;

			white_vleugel_opsluiting = vleugel[tot_white] + white_warning*80;
			black_vleugel_opsluiting = vleugel[tot_black] + black_warning*80;
*/
			// *+* vleugel opsluitingen --------------------------------------------------------------------------------------
/*			if (white_vleugel_opsluiting < 700)
			{	if ((all_white((FLD16|FLD26|FLD22|FLD28)) && (all_black((FLD7|FLD11|FLD12|FLD17)) && (empty_or_white & FLD21))))
				{	//printf("case 1\n");
					if (empty_field & FLD21) white_vleugel_opsluiting += 100;
					else white_vleugel_opsluiting +=50;
				}
				if ((all_white((FLD16|FLD26|FLD21|FLD28|FLD33)) && (all_black((FLD7|FLD11|FLD12|FLD17|FLD22))))) white_vleugel_opsluiting += 100;

				if ((all_white((FLD26|FLD36|FLD32|FLD38)) && (all_black((FLD17|FLD21|FLD22|FLD27)) && (empty_or_white & FLD31))))
				{	//printf("case 1b\n");
					if (empty_field & FLD31) white_vleugel_opsluiting += 100;
					else white_vleugel_opsluiting +=50;
				}
				if ((all_white((FLD25|FLD35|FLD29|FLD33)) && (all_black((FLD14|FLD19|FLD20|FLD24)) && (empty_or_white & FLD30))))
				{	//printf("case 3\n");
					if (empty_field & FLD30) white_vleugel_opsluiting += 100;
					else white_vleugel_opsluiting +=50;
				}
				if ((all_white((FLD25|FLD30|FLD35|FLD33|FLD38)) && (all_black((FLD14|FLD19|FLD20|FLD24|FLD29))))) white_vleugel_opsluiting += 100;

				if ((all_white((FLD16|FLD26|FLD21))) && (!((empty_field & FLD22) && (white_man & (FLD27|FLD31|FLD36))))
						&& ((all_black(( FLD7|FLD11|FLD12|FLD17)))) && (!(all_white(FLD22|FLD28))))
					if  (all_empty((FLD1|FLD6)))  white_vleugel_opsluiting += 500;
//					else white_vleugel_opsluiting += 50;
				if ((all_white((FLD26|FLD36|FLD31))) && (!((empty_field & FLD32) && (white_man & (FLD37|FLD41|FLD46))))
						&& ((all_black((FLD17|FLD21|FLD22|FLD27)))) && (!(all_white(FLD32|FLD38))))
					if  (all_empty((FLD1|FLD6|FLD2|FLD7|FLD11|FLD16)))  white_vleugel_opsluiting += 500;
//					else white_vleugel_opsluiting += 50;

				if ((all_white((FLD35|FLD30|FLD25))) && (!((empty_field & FLD33) && (white_man & (FLD34|FLD40|FLD45))))
						&& ((all_black((FLD24|FLD19|FLD20|FLD14)))) && (!(all_white(FLD39|FLD43))))
					if  (all_empty((FLD4|FLD10|FLD15|FLD5)))  white_vleugel_opsluiting += 500;
//					else white_vleugel_opsluiting += 50;
				if ((all_white((FLD25|FLD20|FLD15))) && (!((empty_field & FLD23) && (white_man & (FLD24|FLD30|FLD35))))
						&& ((all_black((FLD14| FLD9|FLD10 |FLD4)))) && (!(all_white(FLD29|FLD33))))
					if  (empty_field & FLD5)  white_vleugel_opsluiting += 500;
//					else white_vleugel_opsluiting += 50;
 *
*/

				     if (all_white(w3(36,41,46)) && all_black(w4(27,31,32,37)) && all_empty(w12(26,21,17,12,8,3,16,11,7,2,6,1))) white_vleugel_opsluiting += 700;
				else if (all_white(w3(26,31,36)) && all_black(w4(17,21,22,27)) && all_empty(w6(16,11,6,7,1,2)))                  white_vleugel_opsluiting += 700;
				else if (all_white(w3(16,21,26)) && all_black(w4( 7,11,12,17)) && all_empty(w2(6,1)))                            white_vleugel_opsluiting += 700;

				     if (all_white(w3(35,40,45)) && all_black(w4(24,29,30,34)) && all_empty(w9(25,20,14,9,3,15,10,4,5)))         white_vleugel_opsluiting += 700;
				else if (all_white(w3(25,30,35)) && all_black(w4(14,19,20,24)) && all_empty(w4(15,10,4,5)))                      white_vleugel_opsluiting += 700;
				else if (all_white(w3(15,20,25)) && all_black(w4( 4, 9,10,14)) && (empty_field & FLD5))                          white_vleugel_opsluiting += 700;

				if ((all_white((FLD37|FLD36|FLD46|FLD41))) && (empty_field & FLD42) && (all_black((FLD27|FLD28|FLD32))) &&
						((count((black_man & (FLD6|FLD11|FLD16|FLD17|FLD21|FLD22|FLD26))) - count((white_man & (FLD31|FLD47)))) > 1))
					white_vleugel_opsluiting += 200;

				if ((all_white((FLD45|FLD40|FLD34|FLD35))) && (empty_field & FLD39) && (all_black((FLD23|FLD24|FLD29))) &&
						((count((black_man & (FLD5|FLD10|FLD14|FLD15|FLD19|FLD20|FLD25))) - count((white_man & (FLD30|FLD44|FLD50)))) > 1))
					white_vleugel_opsluiting += 200;

				if ((all_white((FLD26|FLD32))) && all_black((FLD21|FLD16|FLD17|FLD11)))
						if (all_empty((FLD6|FLD1))) white_vleugel_opsluiting = 700;
//						else {
//							if (count((white_man & (FLD27|FLD31|FLD36|FLD37|FLD41|FLD46))) - imin(1,count((black_man & (FLD6|FLD1)))) <= 2) white_vleugel_opsluiting = 150;
//						}



//black
/*			if (black_vleugel_opsluiting < 700)
			{	if ((all_black((BLD16|BLD26|BLD22|BLD28)) && (all_white((BLD7|BLD11|BLD12|BLD17)) && (empty_or_black & BLD21))))
				{	//printf("case 2\n");
					if (empty_field & BLD21) black_vleugel_opsluiting += 100;
					else black_vleugel_opsluiting +=50;
				}
				if ((all_black((BLD16|BLD26|BLD21|BLD28|BLD33)) && (all_white((BLD7|BLD11|BLD12|BLD17|BLD22))))) black_vleugel_opsluiting += 100;
				if ((all_black((BLD26|BLD36|BLD32|BLD38)) && (all_white((BLD17|BLD21|BLD22|BLD27)) && (empty_or_black & BLD31))))
				{	//printf("case 2b\n");
					if (empty_field & BLD31) black_vleugel_opsluiting += 100;
					else black_vleugel_opsluiting +=50;
				}
				if ((all_black((BLD25|BLD35|BLD29|BLD33)) && (all_white((BLD14|BLD19|BLD20|BLD24)) && (empty_or_black & BLD30))))
				{	//printf("case 4\n");
					if (empty_field & BLD30) black_vleugel_opsluiting += 100;
					else black_vleugel_opsluiting +=50;
				}
				if ((all_black((BLD25|BLD35|BLD35|BLD33|BLD38)) && (all_white((BLD14|BLD19|BLD20|BLD24|BLD29))))) black_vleugel_opsluiting += 100;


			//if (black_vleugel_opsluiting == 0) {
				if ((all_black((BLD16|BLD26|BLD21))) && (!((empty_field & BLD22) && (black_man & (BLD27|BLD31|BLD36)))) &&
						((all_white(( BLD7|BLD11|BLD12|BLD17)))) && (!(all_black(BLD22|BLD28))))
					if  (all_empty((BLD1|BLD6)))  black_vleugel_opsluiting += 500;
//					else black_vleugel_opsluiting += 50;
				if ((all_black((BLD26|BLD36|BLD31))) && (!((empty_field & BLD32) && (black_man & (BLD37|BLD41|BLD46)))) &&
						((all_white((BLD17|BLD21|BLD22|BLD27)))) && (!(all_black(BLD32|BLD38))))
					if  (all_empty((BLD1|BLD6|BLD2|BLD7|BLD11|BLD16)))  black_vleugel_opsluiting += 500;
//					else black_vleugel_opsluiting += 50;

				if ((all_black((BLD35|BLD30|BLD25))) && (!((empty_field & BLD33) && (black_man & (BLD34|BLD40|BLD45)))) &&
						((all_white((BLD24|BLD19|BLD20|BLD14)))) && (!(all_black(BLD39|BLD43))))
					if  (all_empty((BLD4|BLD10|BLD15|BLD5)))  black_vleugel_opsluiting += 500;
//					else black_vleugel_opsluiting += 50;
				if ((all_black((BLD25|BLD20|BLD15))) && (!((empty_field & BLD23) && (black_man & (BLD24|BLD30|BLD35)))) &&
						((all_white((BLD14| BLD9|BLD10 |BLD4)))))
					if  (empty_field & BLD5)  black_vleugel_opsluiting += 500;
//					else black_vleugel_opsluiting += 50;
*/
					 if (all_black(b3(36,41,46)) && all_white(b4(27,31,32,37)) && all_empty(b12(26,21,17,12,8,3,16,11,7,2,6,1))) black_vleugel_opsluiting += 700;
				else if (all_black(b3(26,31,36)) && all_white(b4(17,21,22,27)) && all_empty(b6(16,11,6,7,1,2)))                  black_vleugel_opsluiting += 700;
				else if (all_black(b3(16,21,26)) && all_white(b4( 7,11,12,17)) && all_empty(b2(6,1)))                            black_vleugel_opsluiting += 700;

					 if (all_black(b3(35,40,45)) && all_white(b4(24,29,30,34)) && all_empty(b9(25,20,14,9,3,15,10,4,5)))         black_vleugel_opsluiting += 700;
				else if (all_black(b3(25,30,35)) && all_white(b4(14,19,20,24)) && all_empty(b4(15,10,4,5)))                      black_vleugel_opsluiting += 700;
				else if (all_black(b3(15,20,25)) && all_white(b4( 4, 9,10,14)) && (empty_field & BLD5))                          black_vleugel_opsluiting += 700;


			     if ((all_black((BLD37|BLD36|BLD46|BLD41))) && (empty_field & BLD42) && (all_white((BLD27|BLD28|BLD32))) &&
						((count((white_man & (BLD6|BLD11|BLD16|BLD17|BLD21|BLD22|BLD26))) - count((black_man & (BLD31|BLD47)))) > 1))
					black_vleugel_opsluiting += 200;

				if ((all_black((BLD45|BLD40|BLD34|BLD35))) && (empty_field & BLD39) && (all_white((BLD23|BLD24|BLD29))) &&
						((count((white_man & (BLD5|BLD10|BLD14|BLD15|BLD19|BLD20|BLD25))) - count((black_man & (BLD30|BLD44|BLD50)))) > 1))
					black_vleugel_opsluiting += 200;

				if ((all_black((BLD26|BLD32))) && all_white((BLD21|BLD16|BLD17|BLD11)))
						if (all_empty((BLD6|BLD1))) black_vleugel_opsluiting = 700;
//						else {
//							if (count((black_man & (BLD27|BLD31|BLD36|BLD37|BLD41|BLD46))) - imin(1,count((white_man & (BLD6|BLD1)))) <= 2) black_vleugel_opsluiting = 150;
//						}

//			}
			if (white_vleugel_opsluiting == 0){
					if ((white_man & BLD35) && all_black((BLD40|BLD44)) && /*(all_empty((BLD45|BLD50))) &&*/
					    !((empty_or_black & BLD29) && (empty_field & BLD34) && (all_empty_or_black((BLD30|BLD39)) || (all_occupied((BLD30|BLD39)))))
					   ) black_253035 -= 80;

					if ((white_man & BLD25) && all_black((BLD30|BLD34)) && /*(all_empty((BLD35|BLD40|BLD45))) &&*/
					    !((empty_or_black & BLD19) && (empty_field & BLD24) && (all_empty_or_black((BLD20|BLD29)) || (all_occupied((BLD20|BLD29)))))
					   ) black_253035 -= 80;

					if ((white_man & BLD15) && all_black((BLD20|BLD24)) && /*(all_empty((BLD25|BLD30|BLD35))) &&*/
					    !((empty_or_black & BLD9) && (empty_field & BLD14) && (all_empty_or_black((BLD10|BLD19)) || (all_occupied((BLD10|BLD19)))))
					   ) black_253035 -= 80;

					if ((white_man & BLD26) && all_black((BLD31|BLD37)) && /*(all_empty((BLD36|BLD41|BLD46))) &&*/
			          !((empty_or_black & BLD22) &&(empty_field & BLD27) && (all_empty_or_black((BLD11|BLD32)) || (all_occupied((BLD21|BLD32)))))
					   ) black_253035 -= 80;

					if ((white_man & BLD16) && all_black((BLD21|BLD27)) && /*(all_empty((BLD26|BLD31|BLD36))) &&*/
					    !((empty_or_black & BLD12) && (empty_field & BLD17) && (all_empty_or_black((BLD11|BLD22)) || (all_occupied((BLD11|BLD22)))))
					   ) black_253035 -= 80;

					if ((white_man & BLD6) && all_black((BLD11|BLD17)) && /*(all_empty((BLD16|BLD21|BLD26))) &&*/
					    !((empty_or_black & BLD2) && (empty_field & BLD7) && (all_empty_or_black((BLD1|BLD12)) || (all_occupied((BLD1|BLD12)))))
					   ) black_253035 -= 80;
					}

			if (black_vleugel_opsluiting == 0){
				if ((black_man & FLD35) && all_white((FLD40|FLD44)) && /*(all_empty((FLD45|FLD50))) &&*/
				    !((empty_or_white & FLD29) && (empty_field & FLD34) && (all_empty_or_white((FLD30|FLD39)) || (all_occupied((FLD30|FLD39)))))
				   ) white_253035 -= 80;

				if ((black_man & FLD25) && all_white((FLD30|FLD34)) && /*(all_empty((FLD35|FLD40|FLD45))) &&*/
				    !((empty_or_white & FLD19) && (empty_field & FLD24) && (all_empty_or_white((FLD20|FLD29)) || (all_occupied((FLD20|FLD29)))))
				   ) white_253035 -= 80;

				if ((black_man & FLD15) && all_white((FLD20|FLD24)) && /*(all_empty((FLD25|FLD30|FLD35))) &&*/
				    !((empty_or_white & FLD9) && (empty_field & FLD14) && (all_empty_or_white((FLD10|FLD19)) || (all_occupied((FLD10|FLD19)))))
				   ) white_253035 -= 80;

				if ((black_man & FLD26) && all_white((FLD31|FLD37)) && /*(all_empty((FLD36|FLD41|FLD46))) &&*/
		          !((empty_or_white & FLD22) &&(empty_field & FLD27) && (all_empty_or_white((FLD11|FLD32)) || (all_occupied((FLD21|FLD32)))))
				   ) white_253035 -= 80;

				if ((black_man & FLD16) && all_white((FLD21|FLD27)) && /*(all_empty((FLD26|FLD31|FLD36))) &&*/
				    !((empty_or_white & FLD12) && (empty_field & FLD17) && (all_empty_or_white((FLD11|FLD22)) || (all_occupied((FLD11|FLD22)))))
				   ) white_253035 -= 80;

				if ((black_man & FLD6) && all_white((FLD11|FLD17)) && /*(all_empty((FLD16|FLD21|FLD26))) &&*/
				    !((empty_or_white & FLD2) && (empty_field & FLD7) && (all_empty_or_white((FLD1|FLD12)) || (all_occupied((FLD1|FLD12)))))
				   ) white_253035 -= 80;
				}
		//} // end vleugelopsluitingen

	// *+* mobility:  the number of direct reachable fields with no immediate capture ------------------------------------------------

		int white_temp = freedom(1, empty_field, white_man, black_man);
		int black_temp = freedom(0, empty_field, black_man, white_man);

		//printf("mob %d, %d\n", white_temp, black_temp);

		if (node->status & IS_WHITE_ON_MOVE) { if (white_temp) white_temp--;}
		else { if (black_temp) black_temp--;}

//		printf("mob %d, %d\n", white_temp, black_temp);

		int distance_correction = calc_black_white_distance(white_man, black_man);
		if (distance_correction < 30) distance_correction = 30;

//		printf("dist %d\n", distance_correction);

		distance_correction = ((distance_correction - 26)*100)/4;
//		printf("dist %d\n", distance_correction);

		if (white_temp == black_temp) {
			white_mobility = 0;
			black_mobility = 0;
		}
		else
			if (white_temp > black_temp){
				black_mobility = 0;
				white_mobility = mobility_value[imin(white_temp-black_temp, 2)][imin(black_temp, 8)];
//				printf("white mob %d\n", white_mobility);
				if (is_klassiek)
					white_mobility = (white_mobility)/distance_correction;
				else
					white_mobility = (mobility_sensivity[node->total_nr_of_pieces]*white_mobility)/distance_correction;
//				printf("white mob %d, corr %d\n", white_mobility, mobility_sensivity[node->total_nr_of_pieces]);
			}
			else {
				white_mobility = 0;
				black_mobility = mobility_value[imin(black_temp-white_temp, 2)][imin(white_temp, 8)];
//				printf("black mob %d\n", black_mobility);
				if (is_klassiek)
					black_mobility = (black_mobility)/distance_correction;
				else
					black_mobility = (mobility_sensivity[node->total_nr_of_pieces]*black_mobility)/distance_correction;
//				printf("black mob %d, corr %d\n", black_mobility, mobility_sensivity[node->total_nr_of_pieces]);
			}


// canon

/*		if (all_bits_set(white_man, CANON_W1_WHITE) && (black_man & CANON_W1_BLACK) && all_bits_set(empty_field, CANON_W1_EMPTY) &&
			((empty_field & CANON_W1_EMPTY_OR_WHITE_EMPTY) || (all_bits_set(white_man, CANON_W1_EMPTY_OR_WHITE_WHITE)))) white_canon +=40;
		if (all_bits_set(white_man, CANON_W2_WHITE) && (black_man & CANON_W2_BLACK) && all_bits_set(empty_field, CANON_W2_EMPTY) &&
			((empty_field & CANON_W2_EMPTY_OR_WHITE_EMPTY) || (all_bits_set(white_man, CANON_W2_EMPTY_OR_WHITE_WHITE)))) white_canon +=100;
		if (all_bits_set(white_man, CANON_W3_WHITE) && (black_man & CANON_W3_BLACK) && all_bits_set(empty_field, CANON_W3_EMPTY) &&
			((empty_field & CANON_W3_EMPTY_OR_WHITE_EMPTY) || (all_bits_set(white_man, CANON_W3_EMPTY_OR_WHITE_WHITE)))) white_canon +=40;
		if (all_bits_set(white_man, CANON_W4_WHITE) && (black_man & CANON_W4_BLACK) && all_bits_set(empty_field, CANON_W4_EMPTY) &&
			((empty_field & CANON_W4_EMPTY_OR_WHITE_EMPTY) || (all_bits_set(white_man, CANON_W4_EMPTY_OR_WHITE_WHITE)))) white_canon +=100;
		if (all_bits_set(white_man, CANON_W5_WHITE) && (black_man & CANON_W5_BLACK) && all_bits_set(empty_field, CANON_W5_EMPTY) &&
			((empty_field & CANON_W5_EMPTY_OR_WHITE_EMPTY) || (all_bits_set(white_man, CANON_W5_EMPTY_OR_WHITE_WHITE)))) white_canon +=100;

		if (all_bits_set(black_man, CANON_B1_BLACK) && (white_man & CANON_B1_WHITE) && all_bits_set(empty_field, CANON_B1_EMPTY) &&
			((empty_field & CANON_B1_EMPTY_OR_BLACK_EMPTY) || (all_bits_set(black_man, CANON_B1_EMPTY_OR_BLACK_BLACK)))) black_canon +=40;
		if (all_bits_set(black_man, CANON_B2_BLACK) && (white_man & CANON_B2_WHITE) && all_bits_set(empty_field, CANON_B2_EMPTY) &&
			((empty_field & CANON_B2_EMPTY_OR_BLACK_EMPTY) || (all_bits_set(black_man, CANON_B2_EMPTY_OR_BLACK_BLACK)))) black_canon +=100;
		if (all_bits_set(black_man, CANON_B3_BLACK) && (white_man & CANON_B3_WHITE) && all_bits_set(empty_field, CANON_B3_EMPTY) &&
			((empty_field & CANON_B3_EMPTY_OR_BLACK_EMPTY) || (all_bits_set(black_man, CANON_B3_EMPTY_OR_BLACK_BLACK)))) black_canon +=40;
		if (all_bits_set(black_man, CANON_B4_BLACK) && (white_man & CANON_B4_WHITE) && all_bits_set(empty_field, CANON_B4_EMPTY) &&
			((empty_field & CANON_B4_EMPTY_OR_BLACK_EMPTY) || (all_bits_set(black_man, CANON_B4_EMPTY_OR_BLACK_BLACK)))) black_canon +=100;
		if (all_bits_set(black_man, CANON_B5_BLACK) && (white_man & CANON_B5_WHITE) && all_bits_set(empty_field, CANON_B5_EMPTY) &&
			((empty_field & CANON_B5_EMPTY_OR_BLACK_EMPTY) || (all_bits_set(black_man, CANON_B5_EMPTY_OR_BLACK_BLACK)))) black_canon +=100;

*/

		// *+* klassiek calculations -----------------------------------------------------------------------------------

		if (is_klassiek)
		{	white_klassiek = (black_tempi - white_tempi) * 15;
			black_klassiek = - white_klassiek;
			//printf("white black %d %d\n", white_klassiek, black_klassiek);
			white_tempi_bonus = 0;	// no further contribution
			black_tempi_bonus = 0;
			white_centrum = 0;
			black_centrum = 0;
			white_fit = 0;
			black_fit = 0;
			if (all_bits_set(white_man, WHITE_CROWN)) white_klassiek += 200;
			//printf("white black %d %d\n", white_klassiek, black_klassiek);
			if (all_bits_set(white_man, WHITE_OLYMPIC)) white_klassiek += 150;
			else if (count((FLD45|FLD40|FLD50|FLD44)) >= 2) white_klassiek += 150;
			else if (count((FLD45|FLD40|FLD50|FLD49)) >= 2) white_klassiek += 150;
			//printf("white black %d %d\n", white_klassiek, black_klassiek);
			if (all_bits_set(black_man, BLACK_CROWN)) black_klassiek += 200;
			//printf("white black %d %d\n", white_klassiek, black_klassiek);
			if (all_bits_set(black_man, BLACK_OLYMPIC)) black_klassiek += 150;
			else if (count((BLD45|BLD40|BLD50|BLD44)) >= 2) black_klassiek += 150;
			else if (count((BLD45|BLD40|BLD50|BLD49)) >= 2) black_klassiek += 150;
			//printf("white black %d %d\n", white_klassiek, black_klassiek);
		}
	}

		if ((white_free_path) || (black_free_path)) {
			white_mobility = 0;
			black_mobility = 0;
			if (black_free_path) {
				white_man_position = man_position_with_kings(white_man);
				white_man_position = (white_man_position*slow_down[node->total_nr_of_pieces])/100;
			}
			if (white_free_path) {
				black_man_position = man_position_with_kings(black_man);
				black_man_position = (black_man_position*slow_down[node->total_nr_of_pieces])/100;
			}

		}

	if (white_tempi_bonus > black_tempi_bonus)
	{	if (black_tempi_bonus) white_mobility = (int) ((float) white_mobility)*(((float) white_tempi_bonus)/ ((float) black_tempi_bonus)); }
	else
	{	if (white_tempi_bonus) black_mobility = (int) ((float) black_mobility)*(((float) black_tempi_bonus)/ ((float) white_tempi_bonus)); }

// define contribution
#define man_position_contribution 110/100
#define king_position_contribution 100/100
#define centrum_contribution 25/100
#define binding_contribution 100/100
#define fit_contribution 80/100
#define tempi_contribution 100/100
#define mobility_contribution 50/100
#define maze_contribution 20/100

#ifdef log_items
#define wcount_item(item, indx) wcnt_item(item, indx)
#define bcount_item(item, indx) bcnt_item(item, indx)
#endif
#ifdef CEM
	int color_white;
	if (node->status & IS_MAX_NODE)
	{
		if (node->status & IS_WHITE_ON_MOVE){
			color_white = true;
		}
		else {
			color_white = false;
		}
	}
	else // min-node
	{
		if (node->status & IS_WHITE_ON_MOVE){
			color_white = false;
		}
		else{
			color_white = true;
		}
	}

#define wcount_item(item, indx) wweight_item(item, indx, color_white)
#define bcount_item(item, indx) bweight_item(item, indx, color_white)
#endif
#ifdef normal
#define wcount_item(item, indx) item
#define bcount_item(item, indx) item
#endif
//end_eval:

	white =
		wcount_item(white_endgame, 0) +
		wcount_item(node->nr_of_white_man * 1000, 1) +
		wcount_item(node->nr_of_white_king * white_king_value, 2) +
		wcount_item(white_man_position * man_position_contribution, 3) +
		wcount_item(white_king_position * king_position_contribution, 4) +
		wcount_item(white_centrum * centrum_contribution, 5) +
		wcount_item(white_man_locked_bonus, 6) +
		wcount_item(white_avoid_4641, 7) +
		wcount_item(white_avoid_2324, 8) +
		wcount_item(white_avoid_2335, 9) +
		wcount_item(white_2_8_13, 10) +
		wcount_item(white_253035, 11) +
		wcount_item(white_klassiek, 12) +
		wcount_item(white_tempi_bonus * tempi_contribution, 13) +
		wcount_item(white_ketting_stelling * 400, 14) +
		wcount_item(white_ketting_stelling_2, 15) +
		wcount_item(white_halve_hek, 16) +
		wcount_item(white_active_15, 17) +
		wcount_item(white_slechte_binding * binding_contribution, 18) +
		wcount_item(white_free_path, 19) +
		wcount_item(white_fit * fit_contribution, 20) +
		wcount_item(white_edge_35, 21) +
		wcount_item(white_edge_36, 22) +
		wcount_item(white_11_16_17, 23) +
		wcount_item(white_voorpost, 24) +
		wcount_item(white_locked, 25) +
		wcount_item(white_vleugel_opsluiting, 26) +
		wcount_item(white_tandem, 27) +
		wcount_item(white_mobility * mobility_contribution, 28) +
		wcount_item(white_maze_penalty * maze_contribution, 29) +
		wcount_item(white_canon, 30)
		;

	black =
		bcount_item(black_endgame, 0) +
		bcount_item(node->nr_of_black_man * 1000, 1) +
		bcount_item(node->nr_of_black_king * black_king_value, 2) +
		bcount_item(black_man_position * man_position_contribution, 3) +
		bcount_item(black_king_position * king_position_contribution, 4) +
		bcount_item(black_centrum * centrum_contribution, 5) +
		bcount_item(black_man_locked_bonus, 6) +
		bcount_item(black_avoid_4641, 7) +
		bcount_item(black_avoid_2324, 8) +
		bcount_item(black_avoid_2335, 9) +
		bcount_item(black_2_8_13, 10) +
		bcount_item(black_253035, 11) +
		bcount_item(black_klassiek, 12) +
		bcount_item(black_tempi_bonus * tempi_contribution, 13) +
		bcount_item(black_ketting_stelling * 400, 14) +
		bcount_item(black_ketting_stelling_2, 15) +
		bcount_item(black_halve_hek, 16) +
		bcount_item(black_active_15, 17) +
		bcount_item(black_slechte_binding * binding_contribution, 18) +
		bcount_item(black_free_path, 19) +
		bcount_item(black_fit * fit_contribution, 20) +
		bcount_item(black_edge_35, 21) +
		bcount_item(black_edge_36, 22) +
		bcount_item(black_11_16_17, 23) +
		bcount_item(black_voorpost, 24) +
		bcount_item(black_locked, 25) +
		bcount_item(black_vleugel_opsluiting, 26) +
		bcount_item(black_tandem, 27) +
		bcount_item(black_mobility * mobility_contribution, 28) +
		bcount_item(black_maze_penalty * maze_contribution, 29) +
		bcount_item(black_canon, 30)
		;

	if (print_flag)
	{	printf("     \tman\tking\tlocked\tman pos\tkng pos\trow_scr\n");
		printf("White\t%d\t%d\t%d\t%d\t%d\t%d\n"  , node->nr_of_white_man*1000, node->nr_of_white_king*white_king_value, nr_of_white_man_locked, white_man_position*man_position_contribution, white_king_position*king_position_contribution, white_centrum*centrum_contribution);
		printf("Black\t%d\t%d\t%d\t%d\t%d\t%d\n\n", node->nr_of_black_man*1000, node->nr_of_black_king*black_king_value, nr_of_black_man_locked, black_man_position*man_position_contribution, black_king_position*king_position_contribution, black_centrum*centrum_contribution);

		printf("     \tmaze\t253035\t2813\tcanon\tlckbns\t4641\t2324\t2335\n");
		printf("White\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n"  , white_maze_penalty*maze_contribution, white_253035, white_2_8_13, white_canon, white_man_locked_bonus, white_avoid_4641, white_avoid_2324, white_avoid_2335);
		printf("Black\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n\n", black_maze_penalty*maze_contribution, black_253035, black_2_8_13, black_canon, black_man_locked_bonus, black_avoid_4641, black_avoid_2324, black_avoid_2335);

		printf("     \tketting\th hek\tact_15\ttempi\tklssiek\tbindng\tvr dg\tfit\n");
		printf("White\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n"  , white_ketting_stelling * 400 + white_ketting_stelling_2, white_halve_hek, white_active_15, white_tempi_bonus*tempi_contribution, white_klassiek, white_slechte_binding*binding_contribution, white_free_path, white_fit*fit_contribution);
		printf("Black\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n"  , black_ketting_stelling * 400 + black_ketting_stelling_2, black_halve_hek, black_active_15, black_tempi_bonus*tempi_contribution, black_klassiek, black_slechte_binding*binding_contribution, black_free_path, black_fit*fit_contribution);

		printf("     \ttri_11\tvoorp\tlock\tvleug\tcentrum\tmoblty\tscore\n");
		printf("White\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n"  , white_11_16_17, white_voorpost, white_locked, white_vleugel_opsluiting, white_tandem, white_mobility*mobility_contribution, white);
		printf("Black\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n"  , black_11_16_17, black_voorpost, black_locked, black_vleugel_opsluiting, black_tandem, black_mobility*mobility_contribution, black);

		printf("     \tp 35\tp 36\n");
		printf("White\t%d\t%d\n"  , white_edge_35, white_edge_36);
		printf("Black\t%d\t%d\n"  , black_edge_35, black_edge_36);

		printf("\n\n");
	}
	// decrease score if there are kings or nearly kings at both sides
	if (((node->nr_of_white_king > 0) || (white_free_path > 0)) && ((node->nr_of_black_king > 0) || (black_free_path > 0))) {
		white = white/2; black = black/2;
	}
	if (node->status & IS_MAX_NODE)
	{
		if (node->status & IS_WHITE_ON_MOVE){
			if (white - black >= 0)
				score =	(short int) (( white - black + GRANULARITY/2)/GRANULARITY)*GRANULARITY ;
			else
				score =	(short int) (( white - black - GRANULARITY/2)/GRANULARITY)*GRANULARITY ;
		}
		else {
			if (black - white >= 0)
				score =	(short int) (( black - white + GRANULARITY/2)/GRANULARITY)*GRANULARITY ;
			else
				score =	(short int) (( black - white - GRANULARITY/2)/GRANULARITY)*GRANULARITY ;
		}
	}
	else // min-node
	{
		if (node->status & IS_WHITE_ON_MOVE){		// remember min-node, this is the opponents color
			if (black - white >= 0)
				score =	(short int) (( black - white + GRANULARITY/2)/GRANULARITY)*GRANULARITY ;
			else
				score =	(short int) (( black - white - GRANULARITY/2)/GRANULARITY)*GRANULARITY ;
		}
		else{
			if (white - black >= 0)
				score =	(short int) (( white - black + GRANULARITY/2)/GRANULARITY)*GRANULARITY ;
			else
				score =	(short int) (( white - black - GRANULARITY/2)/GRANULARITY)*GRANULARITY ;
		}
	}
	if (score == 0) score = 1;  // score == 0 means ignore, no stable position
	if (log_flag)
		// GUI asks for the parameters
	{	int j = sprintf(result, "L");
		j += sprintf(result+j, "%5d",node->nr_of_white_man * 1000);
		j += sprintf(result+j, "%5d",node->nr_of_white_king * white_king_value);
		j += sprintf(result+j, "%5d",white_man_position * man_position_contribution);
		j += sprintf(result+j, "%5d",white_king_position * king_position_contribution);
		j += sprintf(result+j, "%5d",white_tempi_bonus * tempi_contribution);
		j += sprintf(result+j, "%5d",white_fit * fit_contribution );
		j += sprintf(result+j, "%5d",white_centrum * centrum_contribution);
		j += sprintf(result+j, "%5d",white_mobility * mobility_contribution);
		j += sprintf(result+j, "%5d",white_maze_penalty* maze_contribution);
		j += sprintf(result+j, "%5d",white_man_locked_bonus);
		j += sprintf(result+j, "%5d",white_avoid_4641);
		j += sprintf(result+j, "%5d",white_avoid_2324);
		j += sprintf(result+j, "%5d",white_avoid_2335);
		j += sprintf(result+j, "%5d",white_2_8_13);
		j += sprintf(result+j, "%5d",white_253035);
		j += sprintf(result+j, "%5d",white_klassiek);
		j += sprintf(result+j, "%5d",white_ketting_stelling * 400 + white_ketting_stelling_2);
		j += sprintf(result+j, "%5d",white_halve_hek);
		j += sprintf(result+j, "%5d",white_active_15);
		j += sprintf(result+j, "%5d",white_slechte_binding * binding_contribution);
		j += sprintf(result+j, "%5d",white_free_path);
		j += sprintf(result+j, "%5d",white_edge_35);
		j += sprintf(result+j, "%5d",white_edge_36);
		j += sprintf(result+j, "%5d",white_11_16_17);
		j += sprintf(result+j, "%5d",white_voorpost);
		j += sprintf(result+j, "%5d",white_locked);
		j += sprintf(result+j, "%5d",white_vleugel_opsluiting);
		j += sprintf(result+j, "%5d",white_tandem);
		j += sprintf(result+j, "%5d",white_canon);
		j += sprintf(result+j, "%5d",white_endgame);

		j += sprintf(result+j, "%5d",node->nr_of_black_man * 1000);
		j += sprintf(result+j, "%5d",node->nr_of_black_king * black_king_value);
		j += sprintf(result+j, "%5d",black_man_position * man_position_contribution);
		j += sprintf(result+j, "%5d",black_king_position * king_position_contribution);
		j += sprintf(result+j, "%5d",black_tempi_bonus * tempi_contribution );
		j += sprintf(result+j, "%5d",black_fit * fit_contribution );
		j += sprintf(result+j, "%5d",black_centrum * centrum_contribution);
		j += sprintf(result+j, "%5d",black_mobility * mobility_contribution);
		j += sprintf(result+j, "%5d",black_maze_penalty* maze_contribution);
		j += sprintf(result+j, "%5d",black_man_locked_bonus);
		j += sprintf(result+j, "%5d",black_avoid_4641);
		j += sprintf(result+j, "%5d",black_avoid_2324);
		j += sprintf(result+j, "%5d",black_avoid_2335);
		j += sprintf(result+j, "%5d",black_2_8_13);
		j += sprintf(result+j, "%5d",black_253035);
		j += sprintf(result+j, "%5d",black_klassiek);
		j += sprintf(result+j, "%5d",black_ketting_stelling * 400 + black_ketting_stelling_2);
		j += sprintf(result+j, "%5d",black_halve_hek);
		j += sprintf(result+j, "%5d",black_active_15);
		j += sprintf(result+j, "%5d",black_slechte_binding * binding_contribution);
		j += sprintf(result+j, "%5d",black_free_path);
		j += sprintf(result+j, "%5d",black_edge_35);
		j += sprintf(result+j, "%5d",black_edge_36);
		j += sprintf(result+j, "%5d",black_11_16_17);
		j += sprintf(result+j, "%5d",black_voorpost);
		j += sprintf(result+j, "%5d",black_locked);
		j += sprintf(result+j, "%5d",black_vleugel_opsluiting);
		j += sprintf(result+j, "%5d",black_tandem);
		j += sprintf(result+j, "%5d",black_canon);
		j += sprintf(result+j, "%5d",black_endgame);
		j += sprintf(result+j, "%6d",score);
	}
	return score;
}

void calc_piece_count(NodePnt node, struct piece_count_record *pc)
{	BitArray white_man =  (node->occ_empty) &  (node->white_black) &  (node->man_king);
	BitArray white_king = (node->occ_empty) &  (node->white_black) & ~(node->man_king);
	BitArray black_man =  (node->occ_empty) & ~(node->white_black) &  (node->man_king);
	BitArray black_king = (node->occ_empty) & ~(node->white_black) & ~(node->man_king);

	count_bits(white_man,  pc->nr_of_white_man)
	count_bits(white_king, pc->nr_of_white_king)
	count_bits(black_man,  pc->nr_of_black_man)
	count_bits(black_king, pc->nr_of_black_king)
	pc->total_nr_of_pieces = pc->nr_of_white_man + pc->nr_of_white_king + pc->nr_of_black_man + pc->nr_of_black_king;
	return;
}
/*
if (node->status & IS_MAX_NODE)
	{
		if (node->status & IS_WHITE_ON_MOVE)
			score =	(short int) (( white - black)/GRANULARITY)*GRANULARITY ;
		else
			score =	(short int) (( black - white)/GRANULARITY)*GRANULARITY ;
	}
	else // min-node
	{
		if (node->status & IS_WHITE_ON_MOVE)		// remember min-node, this is the opponents color
			score =	(short int) (( black - white)/GRANULARITY)*GRANULARITY ;
		else
			score =	(short int) (( white - black)/GRANULARITY)*GRANULARITY ;
	}

*/

