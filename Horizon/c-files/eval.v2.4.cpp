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



const char dikke_stand[41] = {
 0,	 0,   0,   0,	0,   0,   0,   0,   0,   1,
 1,  2,	  3,   4,	6,   8,  12,  17,  25,  35,
50,	65,	 75,  83,  88,  92,  94,  96,  97,  98,
99,	99,	100, 100, 100, 100, 100, 100, 100, 100, 100};

const char dunne_stand[41] = {
//0       1         2         3         4        5       6       7       8       9       10      11      12    13
100, 100, 100, 100, 100, 100, 100, 100, 100, 99, 99, 98, 97, 96, 94, 92, 88, 83, 75, 65, 50, 35, 25, 17, 12, 8, 6, 4, 3, 2, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0};

const char slow_down[41] = {
0,0,0,1,1,2,3,4,6,9,13,18,23,28,33,38,43,48,53,58,63,68,73,78,83,88,93,96,97,98,99,99,100,100,100,100,100,100,100,100,100
};

const char midden_stand[41] = {
//0   1   2   3     4     5     6     7     8     9      10      11      12    13    14    15    16    17    18  19  20
0,0,0,0,0,0,0,0,10,10,15,15,30,30,50,50,70,70,90,90,100,100,100,100,100,100,90,90,70,70,50,50,30,30,15,15,10,10,0,0,0
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

const char c3_0[11][12] = {
		{0,0,0,0,0,0,0,0,0,1,2,5},
		{0,0,0,0,2,4,8,14,23,30,41,57},
		{0,1,6,15,33,47,63,87,100,100,100,100},
		{6,17,41,65,93,100,100,100,88,64,48,37},
		{45,80,100,100,100,76,58,45,30,17,10,7},
		{100,100,79,58,43,27,17,10,5,2,1,0},
		{50,38,24,13,8,4,2,1,0,0,0,0},
		{3,2,2,1,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0}
};
const char c3_1[11][12] = {
		{0,0,0,0,0,0,0,0,0,0,1,2},
		{0,0,0,0,0,0,0,0,1,3,8,19},
		{0,0,0,0,0,0,1,3,8,18,35,60},
		{0,0,0,0,1,3,9,19,34,57,85,100},
		{0,0,1,3,9,18,35,56,81,100,100,67},
		{0,4,13,21,41,57,81,100,100,81,46,18},
		{9,37,51,67,92,100,100,84,54,26,8,1},
		{58,93,100,100,100,76,53,31,12,3,0,0},
		{100,100,80,60,45,25,12,4,1,0,0,0},
		{36,32,23,14,8,3,1,0,0,0,0,0},
		{5,3,2,1,0,0,0,0,0,0,0,0}
};
const char c3_2[11][12] = {
		{0,0,0,0,0,0,0,0,0,1,2,5},
		{0,0,0,0,1,2,5,10,17,25,36,51},
		{0,0,2,7,17,34,48,64,87,100,100,100},
		{2,9,26,49,71,100,100,100,100,82,61,46},
		{29,46,85,100,100,97,72,54,41,27,15,10},
		{100,100,100,75,54,39,24,14,9,4,2,1},
		{64,35,23,12,7,5,2,1,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0}
};
const char c2_0[11][12] = {
		{0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,1,6},
		{0,0,0,0,0,0,0,0,2,6,14,34},
		{0,0,0,0,0,1,3,8,16,33,56,98},
		{0,0,0,0,3,10,19,38,57,90,100,100},
		{0,0,1,9,20,40,64,91,100,100,62,35},
		{0,3,17,40,60,96,100,100,67,39,14,5},
		{5,36,63,90,100,100,73,45,19,6,1,0},
		{80,100,100,100,72,49,23,8,2,0,0,0},
		{100,91,64,44,20,9,2,0,0,0,0,0},
		{46,31,14,6,1,0,0,0,0,0,0,0}
};
const char c2_1[11][12] = {
		{0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,1,5},
		{0,0,0,0,0,0,0,0,2,6,14,35},
		{0,0,0,0,0,0,2,8,18,39,61,100},
		{0,0,0,0,1,9,23,46,66,100,100,98},
		{0,0,0,6,20,50,73,100,100,91,56,32},
		{0,2,14,44,72,100,100,91,57,33,13,5},
		{4,31,63,100,100,97,64,38,16,6,1,0},
		{46,91,100,90,60,40,19,8,2,0,0,0},
		{100,100,63,40,21,10,3,1,0,0,0,0},
		{80,36,17,9,3,1,0,0,0,0,0,0}
};

int fetch_from_table(const char table[11][12], int total_number_of_man, int number_of_man_in_column) {
	if (total_number_of_man > 18 || total_number_of_man < 7) return 100;
	if (number_of_man_in_column > 10) return 0;
	int idx1 = 18 - total_number_of_man;
	return (int)table[number_of_man_in_column][idx1];
}


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
int iabs(int a){
	if (a>= 0) return a;
	else       return -a;
}

int calc_centre_value(BitArray man)
{	int col1_count = __builtin_popcountll(man & (COL1 | COL10));
	int col2_count = __builtin_popcountll(man & (COL2 | COL9));
	int col3_count = __builtin_popcountll(man & (COL3 | COL8));
	int col4_count = __builtin_popcountll(man & (COL4 | COL7));
	int col5_count = __builtin_popcountll(man & (COL5 | COL6));
	int total = col1_count + col2_count + col3_count + col4_count + col5_count;
	//printf("c5=%d, c4=%d, c3=%d, c2=%d, total=%d\n", 8*col5_count, 7*col4_count, 5*col3_count, 4*col2_count, 8*col5_count+ 7*col4_count+ 5*col3_count+ 4*col2_count);
//	if (total > 0)return ((0*col2_count + 2*col3_count + 6*col4_count + 8*col5_count)*5);
	if (total > 0)return ((1*col2_count + 2*col3_count + 5*col4_count + 6*col5_count)*5);
	else          return 0;
//	return 0;
}

int calc_white_klassiek(BitArray white_man, int white_tempi, int black_tempi){
	int                                                                    white_klassiek = (black_tempi - white_tempi) * 20;
		if (all_white(WHITE_CROWN))                                        white_klassiek += 100;
		if (all_white(WHITE_OLYMPIC)) 									   white_klassiek += 75;
		else if (count_white(w4(45,40,50,44)) >= 2) 					   white_klassiek += 75;
		else if (count_white(w4(45,40,50,49)) >= 2)                        white_klassiek += 75;
		return white_klassiek;
}

int calc_black_klassiek(BitArray black_man, int white_tempi, int black_tempi){
	int                                                                    black_klassiek = (white_tempi - black_tempi) * 20;
		if (all_black(BLACK_CROWN))                                        black_klassiek += 100;
		if (all_black(BLACK_OLYMPIC)) 									   black_klassiek += 75;
		else if (count_black(b4(45,40,50,44)) >= 2) 					   black_klassiek += 75;
		else if (count_black(b4(45,40,50,49)) >= 2)                        black_klassiek += 75;
		return black_klassiek;
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
	int white_corner_35 = 0;
	int black_corner_35 = 0;
	int white_corner_36 = 0;
	int black_corner_36 = 0;
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
	int white_edge = 0;
	int black_edge = 0;
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


// *+* end game database ---------------------------------------------------------------------------------------------------------

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


		//lock_grab(&EndGameLock);
		endgame = evaluate_endgame_position(  node->status & IS_WHITE_ON_MOVE,
													nr_of_white_man, &white_man_idx[0],
													nr_of_white_king, &white_king_idx[0],
													nr_of_black_man, &black_man_idx[0],
													nr_of_black_king, &black_king_idx[0]);
		//lock_release(&EndGameLock);
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

	// *+* is klassiek ?-----------------------------------------------------------------------------------------------------------

	if (all_bits_set(white_man, KLASSIEK_WHITE) && all_bits_set(black_man, KLASSIEK_BLACK) && all_empty((FLD22|FLD29))) is_klassiek = 1;
	else is_klassiek = 0;



	// *+* man position ------------------------------------------------------------------------------------------------------------

	if (node->nr_of_black_king == 0) {
		if (!(is_klassiek)){
			white_man_position = calc_centre_value(white_man);
			//printf("white org1=%d\n", white_man_position);
			if (white_man & FLD46)                                         white_edge += -15;
			//if (white_man & FLD36) white_edge += -20;
			if (white_man & FLD26)                                         white_edge += -15;
			if (white_man & FLD16)                                         white_edge += -25;
			if (white_man & FLD35)                                         white_edge += -15;
			//printf("white org 46,26,16,35 =%d\n", white_edge);
			if (white_man & FLD25)                                         white_edge += -10;
			//printf("white org 25 =%d\n", white_edge);
			if (white_man & FLD48)                                         white_edge += (35*slow_down[node->total_nr_of_pieces])/100;
			//printf("white org 48 =%d\n", white_edge);
			if (white_man & FLD47)                                         white_edge += (30*slow_down[node->total_nr_of_pieces])/100;
			//printf("white org 47 =%d\n", white_edge);
			//white_edge = (white_edge*man_distr_slope[node->total_nr_of_pieces])/100;
			//printf("white org6=%d\n", white_edge);
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
			if (black_man & FLD5) black_edge += -15;
			//if (black_man & FLD15) black_edge += -20;
			if (black_man & FLD25) black_edge += -15;
			if (black_man & FLD35) black_edge += -25;
			if (black_man & FLD16)
			{	if (black_man & FLD27) black_edge += -25;
				else black_edge += -15;
			}
			if (black_man & FLD26) black_edge += -10;
			if (black_man & FLD3) black_edge += (35*slow_down[node->total_nr_of_pieces])/100;
			if (black_man & FLD4) black_edge += (30*slow_down[node->total_nr_of_pieces]) /100;
			//black_edge = (black_edge*man_distr_slope[node->total_nr_of_pieces])/100;
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
		if (white_free_path_pieces){
			if (node->nr_of_black_king)
			{	// special checks if other king
				//printf("black king\n");
				white_free_path = white_distance(white_free_path_pieces, node); // only the closest to the kingline counts
			}
			else
			{	//printf("no black king\n");
				white_free_path = 1500;
			}
			if ((white_free_path_pieces & LINE2) && (node->status & IS_WHITE_ON_MOVE)) {
				white_free_path = white_king_value - 1000;
				black_man_locked_bonus = 0; // nearly a king
			}
		}

		black_free_path_pieces = (black_free_path_pieces & ~black_man_locked);
		if (black_free_path_pieces){
			if  (node->nr_of_white_king)
			{
				//printf("white king\n");
				black_free_path += black_distance(black_free_path_pieces, node);
			}
			else
			{	//printf("No white king\n");
				black_free_path = 1500;
			}
			if ((black_free_path_pieces & LINE9) && !(node->status & IS_WHITE_ON_MOVE)){
				black_free_path = black_king_value - 1000;
				white_man_locked_bonus = 0; // nearly a king
			}
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


	if (node->nr_of_white_king + node->nr_of_black_king == 0)
	{
		// *+* some special cases (active_15, avoid_4641, avoid_2325, avoid_2324, corner_35, corner_36, 11_16_17, 2_8_13) -------------------

		if (((white_man & FLD15) && (empty_field & FLD24)) && ((empty_or_white & FLD19) || (white_man & FLD30)))
			white_active_15 = (80*dikke_stand[node->total_nr_of_pieces])/100;
		if (((black_man & BLD15) && (empty_field & BLD24)) && ((empty_or_black & BLD19) || (black_man & BLD30)))
				black_active_15 = (80*dikke_stand[node->total_nr_of_pieces])/100;

		if ((empty_field & FLD46) && (black_man & FLD26 ) && (all_white(w2(41,37))) && (count(white_man & w3(36,31,27)) > 1))
			if (white_man & FLD32)
				if (empty_field & w1(36)) white_avoid_4641 = -50;
				else white_avoid_4641 = -400;
			else if (all_white(w2(36,31)) && (black_man & (w1(27))) && all_empty(w2(32,38))) white_avoid_4641 = -700;
				 else white_avoid_4641 = -200;

		if ((empty_field & FLD5) && (white_man & FLD25) && (all_black(b2(41,37))) && (count(black_man & b3(36,31,27)) > 1))
			if (black_man & BLD32)
				if (empty_field & b1(36)) black_avoid_4641 = -50;
				else black_avoid_4641 = -400;
			else if (all_black(b2(36,31)) && (white_man & (b1(27))) && all_empty(b2(32,38))) black_avoid_4641 = -700;
				 else black_avoid_4641 = -200;

		// white corner_36
		if ((black_man & w1(36) || ((black_man & w1(31)) && (empty_field & w1(36))))) {
			if ((all_empty((FLD47 | FLD37 | FLD32))) && (white_man & (FLD46)))                       white_corner_36 -= 200*slow_down[node->total_nr_of_pieces]/100;
			if ((all_empty((FLD47 | FLD46 | FLD41))) && (white_man & (FLD37))) {
			if (all_empty_or_black(w4(31,32,26,27)))                                                 white_corner_36 -= 400;
			else                                                                                     white_corner_36 -= 201*slow_down[node->total_nr_of_pieces]/100;
			}
		}
		if (black_man & w1(26) && all_white(w2(36,27)) && all_empty(w7(31,32,37,41,42,46,47))) 	     white_corner_36 -= 402;
		if ((all_black((FLD16|FLD26|FLD22|FLD18))) &&  (all_white((FLD27|FLD31|FLD32|FLD36|FLD37)))) white_corner_36 -= 300;
		if (all_empty(w4(47,46,41,36)))
			if (any_white(w3(48,42,37)))                       				                         white_corner_36 -= 150*slow_down[node->total_nr_of_pieces]/100;
			else                                                                                     white_corner_36 -= 400*slow_down[node->total_nr_of_pieces]/100;

		// black corner_36
		if ((white_man & b1(36) || ((white_man & b1(31)) && (empty_field & b1(36))))) {
			if ((all_empty((FLD4  | FLD14 | FLD19))) && (black_man & (FLD5 )))                       black_corner_36 -= 200*slow_down[node->total_nr_of_pieces]/100;
			if ((all_empty((FLD4  | FLD5  | FLD10))) && (black_man & (FLD14))) {
			if (all_empty_or_white(b4(31,32,26,27)))                                                 black_corner_36 -= 400;
			else                                                                                     black_corner_36 -= 201*slow_down[node->total_nr_of_pieces]/100;
			}
		}
		if (white_man & b1(26) && all_black(b2(36,27)) && all_empty(b7(31,32,37,41,42,46,47)))          black_corner_36 -= 402;
		if ((all_white((BLD16|BLD26|BLD22|BLD18))) &&  (all_black((BLD27|BLD31|BLD32|BLD36|BLD37)))) black_corner_36 -= 300;
		if (all_empty(b4(47,46,41,36)))
			if (any_black(b3(48,42,37)))                       				                         black_corner_36 -= 150*slow_down[node->total_nr_of_pieces]/100;
			else                                                                                     black_corner_36 -= 400*slow_down[node->total_nr_of_pieces]/100;

		// white corner_35
		if ((black_man & FLD35) && (white_man & FLD34) && all_bits_set(empty_field,((FLD40|FLD44|FLD45|FLD49|FLD50))))
		{	if ((black_or_empty & FLD25) && (black_or_empty & FLD24) && (black_man &(FLD25|FLD24)) && (empty_field & FLD30)) white_corner_35 = -400;
			else                                                                                                             white_corner_35 = -100;
		}
		if (all_empty(w6(35,40,44,45,49,50)))                                                                                white_corner_35 -= 150*slow_down[node->total_nr_of_pieces]/100;
		else if (count_white(w6(35,40,44,45,49,50)) == 1)                                                                    white_corner_35 -= 50*slow_down[node->total_nr_of_pieces]/100;
		if (all_black(w4(16,21,26,27)) && all_white(w2(36,37)) && all_empty(w5(31,41,42,46,47)))                             white_corner_35 -= 1500;
		if ((black_man & FLD35) && (white_man & FLD45) && all_bits_set(empty_field,((FLD39|FLD40|FLD44|FLD34|FLD49|FLD50)))) white_corner_35 -= 50;
		if (!(white_man & (FLD34|FLD35|FLD39|FLD40|FLD43|FLD44|FLD45|FLD48|FLD49|FLD50)))                                    white_corner_35 -= 4 * slow_down[node->total_nr_of_pieces];
		if ((all_black((FLD6|FLD11|FLD16))) && (black_man & (FLD17|FLD21|FLD26)) &&
				(all_white((FLD27|FLD31|FLD32))) && all_empty((FLD36|FLD41|FLD46)))                                          white_corner_35 -= 2 * slow_down[node->total_nr_of_pieces];
		if ((all_black((FLD15|FLD25|FLD19|FLD13))) && (all_white((FLD24|FLD30|FLD29|FLD35|FLD34))))                          white_corner_35 -= 300;

		// black corner_35
		if ((white_man & FLD16) && (black_man & FLD17) && all_bits_set(empty_field, ((FLD11|FLD6|FLD7|FLD1|FLD2))))
		{	if ((white_or_empty & BLD25) && (white_or_empty & BLD24) && (white_man & (BLD25|BLD24)) && (empty_field & BLD30)) black_corner_35 = -400;
			else                                                                                                              black_corner_35 = -100;
		}
		if (all_empty(b6(35,40,44,45,49,50)))                                                                                 black_corner_35 -= 150*slow_down[node->total_nr_of_pieces]/100;
		else if (count_black(b6(35,40,44,45,49,50)) == 1)                                                                     black_corner_35 -= 50*slow_down[node->total_nr_of_pieces]/100;
		if (all_white(b4(16,21,26,27)) && all_black(b2(36,37)) && all_empty(b5(31,41,42,46,47)))                              black_corner_35 -= 1500;
		if ((white_man & FLD16) && (black_man & FLD6) &&  all_bits_set(empty_field,((FLD12|FLD11|FLD7 |FLD17|FLD2 |FLD1 ))))  black_corner_35 = -50;

		if (!(black_man & (BLD34|BLD35|BLD39|BLD40|BLD43|BLD44|BLD45|BLD48|BLD49|BLD50))) 									  black_corner_35 = -4 * slow_down[node->total_nr_of_pieces];
		if ((all_white((BLD6|BLD11|BLD16))) && (white_man & (BLD17|BLD21|BLD26)) &&
				(all_black((BLD27|BLD31|BLD32))) && all_empty((BLD36|BLD41|BLD46)))                                           black_corner_35 = -2 * slow_down[node->total_nr_of_pieces];
		if ((all_white((BLD15|BLD25|BLD19|BLD13))) && (all_black((BLD24|BLD30|BLD29|BLD35|BLD34))))                           black_corner_35 -= 300;

		if ((white_man & FLD26) && (all_empty((FLD1|FLD6|FLD21)))  && (all_bits_set(black_man, (TRIANGLE_11)))) black_11_16_17 = -400;
		if ((black_man & FLD25) && (all_empty((FLD50|FLD45|FLD30))) && (all_bits_set(white_man, (TRIANGLE_40)))) white_11_16_17 = -400;

		//if ((black_man & BLD24) && (!(all_white((BLD2|BLD8|BLD13))))) white_2_8_13 = -1*dikke_stand[node->total_nr_of_pieces];
		//if ((white_man & FLD24) && (!(all_black((FLD2|FLD8|FLD13))))) black_2_8_13 = -1*dikke_stand[node->total_nr_of_pieces];


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
				if (cnt == 0) white_halve_hek = 200;
				else white_halve_hek = 100;
			} else if (cnt >2)
				if (cnt==3) white_halve_hek = -50;
				else white_halve_hek = -100;
		}

		if (all_bits_set(black_man, BLACK_BLACK_HALVE_HEK) && all_bits_set(white_man, BLACK_WHITE_HALVE_HEK) && all_bits_set(empty_field, (FLD25 | FLD30)))
		{	char cnt; count_bits(black_man & DIAGONAL, cnt);
			if (cnt < 2) {
				if (cnt == 0) black_halve_hek = 200;
				else black_halve_hek = 100;
			} else if (cnt >2)
				if (cnt==3) black_halve_hek = -50;
				else black_halve_hek = -100;

		}

		// *+* (gesloten hek) stelling ? --------------------------------------------------------------------------

		if (all_white(w4(36,31,26,27)) && all_black(w3(22,16,18)) && (empty_field & FLD21) && (count_white(w4(46,41,37,32)) <= 3) &&
				(count_white(w4(46,41,37,32)) < count_black(w6(1,6,7,11,12,17))) && (((empty_field & (w1(17))) && (white_man & (w1(33)))) || (black_man & (w1(17)))))
			if (white_man & FLD32) white_halve_hek += 1*dunne_stand[imax(0,node->total_nr_of_pieces - 7)]+50; // gesloten
			else white_halve_hek += 3*dunne_stand[imax(0,node->total_nr_of_pieces - 14)]+50;

		if (all_black(b4(36,31,26,27)) && all_white(b3(22,16,18)) && (empty_field & BLD21) && (count_black(b4(46,41,37,32)) <= 3) &&
				(count_black(b4(46,41,37,32)) < count_white(b6(1,6,7,11,12,17))) && (((empty_field & (b1(17))) && (black_man & (b1(33)))) || (white_man & (b1(17)))))
			if (black_man & BLD32) black_halve_hek += 1*dunne_stand[imax(0,node->total_nr_of_pieces - 7)]+50; // gesloten
			else black_halve_hek += 3*dunne_stand[imax(0,node->total_nr_of_pieces - 14)]+50;

//		printf("%d %d %d %d %d %d %d",all_black(b4(36,31,26,27)), all_white(b3(22,16,18)), (empty_field & BLD21), count_black(b4(46,41,37,32)),
//				(count_black(b4(46,41,37,32)) < count_white(b6(1,6,7,11,12,17))), ((empty_field & b1(17)) && (black_man & b1(33))), (white_man & b1(17)));
		// *+* calc tempi --------------------------------------------------------------------------------------------

		// In case of an hekstelling, the other pieces must be as far away from the hek as possible:

		if (all_white(w3(36,31,27)) && all_black(w3(16,22,18)) && (empty_field & w1(21))) {
			int white_distance = 0;
			add_count_bits(white_man & w10(46,41,37,32,28,23,19,14,10,5),white_distance,-2);
			add_count_bits(white_man & w8(47,42,38,33,29,24,20,15),white_distance,-1);
			//add_count_bits(white_man & w6(48,43,39,34,30,25),white_distance,0);
			add_count_bits(white_man & w6(49,44,40,35,50,45),white_distance,1);
			if (white_halve_hek > 0) white_halve_hek += white_distance*20;
		}

		if (all_black(b3(36,31,27)) && all_white(b3(16,22,18)) && (empty_field & b1(21))) {
			int black_distance = 0;
			add_count_bits(black_man & b10(46,41,37,32,28,23,19,14,10,5),black_distance,-2);
			add_count_bits(black_man & b8(47,42,38,33,29,24,20,15),black_distance,-1);
			//add_count_bits(black_man & b6(48,43,39,34,30,25),black_distance,0);
			add_count_bits(black_man & b6(49,44,40,35,50,45),black_distance,1);
			if (black_halve_hek > 0) black_halve_hek += black_distance*20;
		}


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

		// *+* formaties langs de kant die meestal slecht zijn --------------------------------------------------------------
		// white


		if      (all_white(w5(36,31,26,27,22)))                            white_253035 -= 100;
		else if (all_white(w4(36,31,27,22)) && !(all_black(w3(16,21,26)))) white_253035 -= 75;
		else if (all_white(w4(36,31,26,27)) &&   all_empty(w2(16,21)))     white_253035 -= 75;
		else if (all_white(w3(36,31,27))    &&   all_empty(w2(16,21)))     white_253035 -= 50;
		else if (all_white(w3(36,26,27))	&&	 (empty_field & w1(31)))   white_253035 -= 100;

		if      (all_white(w5(26,21,17,16,12)))                            white_253035 -= 100;
		else if (all_white(w4(26,21,17,12)) && !(all_black(w3( 6,11,16)))) white_253035 -= 75;
		else if (all_white(w4(26,21,17,16)) &&   all_empty(w2( 6,11)))     white_253035 -= 75;
		else if (all_white(w3(26,21,17))    &&   all_empty(w2( 6,11)))     white_253035 -= 50;
		else if (all_white(w3(26,16,17))	&&	 (empty_field & w1(21)))   white_253035 -= 100;

		if      (all_white(w3(16,11, 7)))                                  white_253035 -= 50;

		if      (all_white(w5(35,30,24,19,25)))                            white_253035 -= 100;
		else if (all_white(w4(35,30,24,19)) && !(all_black(w3(15,20,25)))) white_253035 -= 75;
		else if (all_white(w4(35,30,24,25)) &&   all_empty(w2(15,20)))     white_253035 -= 75;
		else if (all_white(w3(35,30,24))    &&   all_empty(w2(15,20)))     white_253035 -= 50;
		else if (all_white(w3(35,25,24))	&&	 (empty_field & w1(30)))   white_253035 -= 100;

		if      (all_white(w5(25,20,14, 9,15)))                            white_253035 -= 100;
		else if (all_white(w4(25,20,14, 9)) && !(all_black(w3( 5,10,15)))) white_253035 -= 75;
		else if (all_white(w4(25,20,14,15)) &&   all_empty(w2( 5,10)))     white_253035 -= 75;
		else if (all_white(w3(25,20,14))    &&   all_empty(w2( 5,10)))     white_253035 -= 50;
		else if (all_white(w3(25,15,14))	&&	 (empty_field & w1(20)))   white_253035 -= 100;


		// black


		if      (all_black(b5(36,31,26,27,22)))                            black_253035 -= 100;
		else if (all_black(b4(36,31,27,22)) && !(all_white(b3(16,21,26)))) black_253035 -= 75;
		else if (all_black(b4(36,31,26,27)) &&   all_empty(b2(16,21)))     black_253035 -= 75;
		else if (all_black(b3(36,31,27))    &&   all_empty(b2(16,21)))     black_253035 -= 50;
		else if (all_black(b3(36,26,27))	&&	 (empty_field & w1(31)))   black_253035 -= 100;

		if      (all_black(b5(26,21,17,16,12)))                            black_253035 -= 100;
		else if (all_black(b4(26,21,17,12)) && !(all_white(b3( 6,11,16)))) black_253035 -= 75;
		else if (all_black(b4(26,21,17,16)) &&   all_empty(b2( 6,11)))     black_253035 -= 75;
		else if (all_black(b3(26,21,17))    &&   all_empty(b2( 6,11)))     black_253035 -= 50;
		else if (all_black(b3(26,16,17))	&&	 (empty_field & w1(21)))   black_253035 -= 100;

		if      (all_black(b3(16,11, 7)))                                  black_253035 -= 50;

		if      (all_black(b5(35,30,24,19,25)))                            black_253035 -= 100;
		else if (all_black(b4(35,30,24,19)) && !(all_white(b3(15,20,25)))) black_253035 -= 75;
		else if (all_black(b4(35,30,24,25)) &&   all_empty(b2(15,20)))     black_253035 -= 75;
		else if (all_black(b3(35,30,24))    &&   all_empty(b2(15,20)))     black_253035 -= 50;
		else if (all_black(b3(35,25,24))	&&	 (empty_field & w1(30)))   black_253035 -= 100;

		if      (all_black(b5(25,20,14, 9,15)))                            black_253035 -= 100;
		else if (all_black(b4(25,20,14, 9)) && !(all_white(b3( 5,10,15)))) black_253035 -= 75;
		else if (all_black(b4(25,20,14,15)) &&   all_empty(b2( 5,10)))     black_253035 -= 75;
		else if (all_black(b3(25,20,14))    &&   all_empty(b2( 5,10)))     black_253035 -= 50;
		else if (all_black(b3(25,15,14))	&&	 (empty_field & w1(20)))   black_253035 -= 100;





		// *+* slechte bindingen  ------------------------------------------------------------------------------------



		// bindingen v2
		// roozenburg white
		if ((all_white((FLD27|FLD29|FLD24))) && (all_black((FLD15|FLD18|FLD23))) &&
			(!(white_man & LINE01_05_R)) && (white_tempi  < black_tempi) && (empty_field & FLD28))
				white_slechte_binding = 50*(white_tempi - black_tempi)*dikke_stand[node->total_nr_of_pieces]/100;
		// anders
		else if ((all_white((FLD27|FLD29)) ) && !(white_man & LINE01_05) && ((white_tempi - 4) < black_tempi)) {
			if ((empty_field & FLD28) /*&& (count(empty_field & w4(21,22,23,24))<3)*/)
				white_slechte_binding = 50*(white_tempi - 4 - black_tempi)*slow_down[node->total_nr_of_pieces]/100;
			else if (white_man & FLD28)
				white_slechte_binding = 20*(white_tempi - 4 - black_tempi)*slow_down[node->total_nr_of_pieces]/100;
		}
/*		else if (all_white((FLD26|FLD28)) && !(white_man & LINE01_05) && ((white_tempi - 4) < black_tempi)) {
			if (empty_field & FLD27)
				white_slechte_binding = 20*(white_tempi - 4 - black_tempi)*dikke_stand[node->total_nr_of_pieces]/100;
		}
*/		//roozenburg ??
		if ((all_black((BLD27|BLD29|BLD24))) && (all_white((BLD15|BLD18|BLD23))) &&
			!(black_man & LINE06_10_R) && (black_tempi  < white_tempi) && (empty_field & BLD28))
				black_slechte_binding = 50*(black_tempi - white_tempi)*dikke_stand[node->total_nr_of_pieces]/100;
		// anders
		else if ((all_black((BLD27|BLD29)) ) && !(black_man & LINE06_10) && ((black_tempi - 4) < white_tempi)) {
			if ((empty_field & BLD28) /*&& (count(empty_field & b4(21,22,23,24))<3)*/)
				black_slechte_binding = 50*(black_tempi - 4 - white_tempi)*slow_down[node->total_nr_of_pieces]/100;
			else if (black_man & BLD28)
				black_slechte_binding = 20*(black_tempi - 4 - white_tempi)*slow_down[node->total_nr_of_pieces]/100;
		}
/*		else if (all_black((BLD26|BLD28)) && !(black_man & LINE06_10) && ((black_tempi - 4) < white_tempi)) {
			if (empty_field & BLD27)
				black_slechte_binding = 20*(black_tempi - 4 - white_tempi)*dikke_stand[node->total_nr_of_pieces]/100;
		}
*/

		// *+* distribution of men -------------------------------------------------------------------------------------

		{	int t2_1_5, t2_2_5;
			int tc2;



			if (node->nr_of_white_man >= 4){
				count_bits(white_man & W2_1_5, t2_1_5);
				count_bits(white_man & W2_2_5, t2_2_5);

				tc2 = iabs(t2_1_5 - t2_2_5);
				switch (tc2) {
				case 0:
					break;
				case 1:
					break;
				case 2:
					white_fit = -50;
					break;
				case 3:
					white_fit = -75;
					break;
				case 4:
					white_fit = -200;
					break;
				case 5:
					white_fit = -300;
					break;
				default:
					white_fit = -500;
				}
			}







		// distribution of black men

			if (node->nr_of_black_man >= 4){
				count_bits(black_man & W2_1_5, t2_1_5);
				count_bits(black_man & W2_2_5, t2_2_5);

				tc2 = iabs(t2_1_5 - t2_2_5);
				switch (tc2) {
				case 0:
					break;
				case 1:
					break;
				case 2:
					black_fit = -50;
					break;
				case 3:
					black_fit = -75;
					break;
				case 4:
					black_fit = -200;
					break;
				case 5:
					black_fit = -300;
					break;
				default:
					black_fit = -500;
				}
			}
		}

		{	int t2_1_5, t2_2_5;
			int t3_1_3, t3_2_4, t3_3_3;
			int white_fit_t = 0;
			int black_fit_t = 0;

//			count_bits(white_man & W2_1_5, t2_1_5);
//			count_bits(white_man & W2_2_5, t2_2_5);
			count_bits(white_man & W3_1_3, t3_1_3);
			count_bits(white_man & W3_2_4, t3_2_4);
			count_bits(white_man & W3_3_3, t3_3_3);

//			white_fit_t += fetch_from_table(c2_0, node->nr_of_white_man, t2_1_5)*5;
//			white_fit_t += fetch_from_table(c2_1, node->nr_of_white_man, t2_2_5)*5;
			white_fit_t += fetch_from_table(c3_0, node->nr_of_white_man, t3_1_3)*3;
			white_fit_t += fetch_from_table(c3_1, node->nr_of_white_man, t3_2_4)*4;
			white_fit_t += fetch_from_table(c3_2, node->nr_of_white_man, t3_3_3)*3;
			white_fit_t = white_fit_t/10;
			white_fit = white_fit_t + white_fit;

//			count_bits(black_man & W2_1_5, t2_2_5);
//			count_bits(black_man & W2_2_5, t2_1_5);
			count_bits(black_man & W3_1_3, t3_3_3);
			count_bits(black_man & W3_2_4, t3_2_4);
			count_bits(black_man & W3_3_3, t3_1_3);

//			black_fit_t += fetch_from_table(c2_0, node->nr_of_black_man, t2_1_5)*5;
			//printf("black_fit_t = %d %d\n",t2_1_5 ,black_fit_t);
//			black_fit_t += fetch_from_table(c2_1, node->nr_of_black_man, t2_2_5)*5;
			//printf("black_fit_t = %d %d\n",t2_2_5 ,black_fit_t);
			black_fit_t += fetch_from_table(c3_0, node->nr_of_black_man, t3_1_3)*3;
			//printf("black_fit_t = %d %d\n",t3_1_3 ,black_fit_t);
			black_fit_t += fetch_from_table(c3_1, node->nr_of_black_man, t3_2_4)*4;
			//printf("black_fit_t = %d %d\n",t3_2_4 ,black_fit_t);
			black_fit_t += fetch_from_table(c3_2, node->nr_of_black_man, t3_3_3)*3;
			//printf("black_fit_t = %d %d\n",t3_3_3 ,black_fit_t);
			black_fit_t = black_fit_t/10;
			black_fit = black_fit_t + black_fit;
		}
			// *+* centrum controle, first simple try -------------------------------------------------------------------------------

			// *+* centrum spel -------------------------------------------------------------------------------------------


			if ((white_man & FLD35) && (white_man & FLD23)) white_avoid_2335 = -50*dikke_stand[node->total_nr_of_pieces]/100;
			if ((black_man & FLD16) && (black_man & FLD28)) black_avoid_2335 = -50*dikke_stand[node->total_nr_of_pieces]/100;

//			if ((black_man & FLD23) && (white_man & FLD24)) white_avoid_2324 = -65*dikke_stand[node->total_nr_of_pieces]/100;
//			if ((white_man & FLD28) && (black_man & FLD27)) black_avoid_2324 = -65*dikke_stand[node->total_nr_of_pieces]/100;

			white_centrum = 30*(count((CENTRUM & white_man)) + count(FLD24 & white_man));
			if ((FLD24 & white_man) && (FLD15 & empty_field))                                                  white_centrum +=  20;
			if (all_white(w1(24)) && all_black(w2(23,25)))                                                     white_centrum -= 120;
			if (all_white(w3(27,28,24)))                                                                       white_centrum += 200;
			if (all_white(w3(22,23,28)))                                                                       white_centrum += 120;
			else if(all_white(w2(22,23)))                                                                      white_centrum +=  80;
			else if (all_bits_set(white_man, ((FLD23 | FLD24)))){
				if (white_man & FLD29)                                                                         white_centrum += 150;
				else                                                                                           white_centrum += 100;
			}
			else if((all_white((FLD30|FLD27))) && (all_empty((FLD28|FLD29))))                                  white_centrum -=  50;

			white_centrum = (white_centrum*midden_stand[node->total_nr_of_pieces])/100;
			// --------------------------------------------------------------------------------------------------------------------
			black_centrum = 30*(count((CENTRUM & black_man)) + count(FLD27 & black_man));
			if ((BLD24 & black_man) && (BLD15 & empty_field))                                                  black_centrum +=  20;
			if (all_black(b1(24)) && all_white(b2(23,25)))                                                     black_centrum -= 120;
			if (all_black(b3(27,28,24)))                                                                       black_centrum += 200;
			if (all_bits_set(black_man, ((FLD29 | FLD28)))) {
				if (black_man & FLD23)                                                                         black_centrum += 120;
				else                                                                                           black_centrum +=  80;
			}
			else if (all_bits_set(black_man, ((FLD28 | FLD27)))) {
				if (black_man & FLD22)                                                                         black_centrum += 150;
				else                                                                                           black_centrum += 100;
			}
			else if((all_black((BLD30|BLD27))) && (all_empty((BLD28|BLD29))))                                  black_centrum -=  50;


			black_centrum = (black_centrum*midden_stand[node->total_nr_of_pieces])/100;

			//printf("white %d, black %d centre(2)\n",white_centrum, black_centrum);

		// *+* voorpost calculations -------------------------------------------------------------------------------------------
			white_voorpost = calc_white_voorpost(white_man, black_man, empty_field, node->status & IS_WHITE_ON_MOVE);
			//if (white_voorpost == 180) white_voorpost = (180*dikke_stand[node->total_nr_of_pieces])/100;
			white_voorpost = (white_voorpost*dikke_stand[node->total_nr_of_pieces])/100;

			black_voorpost = calc_black_voorpost(white_man, black_man, empty_field, node->status & IS_WHITE_ON_MOVE);
			//if (black_voorpost == 180) black_voorpost = (180*dikke_stand[node->total_nr_of_pieces])/100;
			black_voorpost = (black_voorpost*dikke_stand[node->total_nr_of_pieces])/100;
		// *+* very simple 1 piece locks 2 pieces situations -------------------------------------------------------------------

			// *+* begin *+* one_keeps_two ------------------------------------------------------------------------------------
// ----  white part -----
// against the damlijn

if (((white_man & w1(12)) && (black_man & w2(1,11)) && (black_man & w2(3,13)) && all_empty(w7(2,7,8,6,16,9,19))) ||
    ((white_man & w1(13)) && (black_man & w2(2,12)) && (black_man & w2(4,14)) && all_empty(w7(3,8,9,7,17,10,20))) ||
    ((white_man & w1(14)) && (black_man & w2(3,13)) && (black_man & w1(5)) && all_empty(w5(4,9,10,8,18)))        ||
    ((white_man & w1(14)) && (black_man & w2(3,13)) && (black_man & w1(15)) && all_empty(w6(4,9,10,8,18,25))))              white_locked += 700;

else if
   (((white_man & w1(17)) && (black_man & w2(6,16)) && (black_man & w2(8,18)) && all_empty(w6(11,12,7,1,2,3))) ||
    ((white_man & w1(18)) && (black_man & w2(7,17)) && (black_man & w2(9,19)) && all_empty(w7(12,13,8,1,2,3,4))) ||
    ((white_man & w1(19)) && (black_man & w2(8,18)) && (black_man & w2(10,20)) && all_empty(w7(13,14,9,2,3,4,5))))          white_locked += 500;


else if
   (((white_man & w1(22)) && (black_man & w2(11,21)) && (black_man & w2(13,23)) && all_empty(w8(17,18,12,7,8,1,2,3))) ||
    ((white_man & w1(23)) && (black_man & w2(12,22)) && (black_man & w2(14,24)) && all_empty(w8(18,19,13,8,9,2,3,4))) ||
    ((white_man & w1(24)) && (black_man & w2(13,23)) && (black_man & w2(15,25)) && all_empty(w8(19,20,14,9,10,3,4,5))))      white_locked += 200;


//against the left edge


// if (((white_man & w1(14)) && all_black(w2(5,15)) && all_empty(w4(20,25,4,10))) is not proffitable, either there is a breakthrough, or a black_man at 3
if ((white_man & w1(24)) && all_black(w2(15,25)) && all_empty(w2(30,35)) && all_empty_or_white(w6(14,9,3,10,4,5)) && !((black_man & w2(13,23)))) white_locked += 200;
// if (((white_man & w1(34)) && all_black(w2(25,35)) && all_empty(w16(40,45,30,24,19,13,8,2,20,14,9,3,15,10,4,5))) not good, due to breakthrough risks


// against the right edge



//if (((white_man & w1(17)) && all_black(w2(6,16)) && all_empty(w6(21,26,11,7,2,1))) is not proffitable, either there is a breakthrough, or a black_man at 8
if   ((white_man & w1(27)) && all_black(w2(16,26)) && all_empty(w2(31,36)) && all_empty_or_white(w10(21,17,12,8,3,11,7,2,6,1)) && !((black_man & w2(14,24)))) white_locked += 200;
// with (white_man & w1(37)) there is nearly a breakthrough


// ----  black part -----
// against the damlijn

if (((black_man & b1(12)) && (white_man & b2(1,11)) && (white_man & b2(3,13)) && all_empty(b7(2,7,8,6,16,9,19))) ||
    ((black_man & b1(13)) && (white_man & b2(2,12)) && (white_man & b2(4,14)) && all_empty(b7(3,8,9,7,17,10,20))) ||
    ((black_man & b1(14)) && (white_man & b2(3,13)) && (white_man & b1(5)) && all_empty(b5(4,9,10,8,18)))        ||
    ((black_man & b1(14)) && (white_man & b2(3,13)) && (white_man & b1(15)) && all_empty(b6(4,9,10,8,18,25))))              white_locked += 700;


else if
   (((black_man & w1(34)) && (white_man & w2(45,35)) && (white_man & w2(43,33)) && all_empty(w6(40,39,44,50,49,48))) ||
    ((black_man & w1(33)) && (white_man & w2(44,34)) && (white_man & w2(42,32)) && all_empty(w7(39,38,43,50,49,48,47))) ||
    ((black_man & w1(32)) && (white_man & w2(43,33)) && (white_man & w2(41,31)) && all_empty(w7(38,37,42,49,48,47,46))))          black_locked += 500;


else if
   (((black_man & w1(29)) && (white_man & w2(40,30)) && (white_man & w2(38,28)) && all_empty(w8(34,33,39,44,43,50,49,48))) ||
    ((black_man & w1(28)) && (white_man & w2(39,29)) && (white_man & w2(37,27)) && all_empty(w8(33,32,38,43,42,49,48,47))) ||
    ((black_man & w1(27)) && (white_man & w2(38,28)) && (white_man & w2(36,26)) && all_empty(w8(32,31,37,42,41,48,47,46))))      black_locked += 200;


//against the left edge


// if (((black_man & w1(37)) && all_white(w2(46,36)) && all_empty(w4(31,26,47,41))) is not proffitable, either there is a breakthrough, or a white_man at 3
if ((black_man & w1(27)) && all_white(w2(36,26)) && all_empty(w2(21,16)) && all_empty_or_black(w6(37,42,48,41,47,46)) && !((white_man & w2(38,28)))) black_locked += 200;
// if (((black_man & w1(17)) && all_white(w2(26,16)) && all_empty(w16(11,6,21,27,32,38,43,49,31,37,42,48,36,41,47,46))) not good, due to breakthrough risks


// against the right edge



//if (((black_man & w1(34)) && all_white(w2(45,35)) && all_empty(w6(30,25,40,44,49,50))) is not proffitable, either there is a breakthrough, or a white_man at 8
if   ((black_man & w1(24)) && all_white(w2(35,25)) && all_empty(w2(20,15)) && all_empty_or_black(w10(30,34,39,43,48,40,44,49,45,50)) && !((white_man & w2(37,27)))) black_locked += 200;
// with (black_man & w1(14)) there is nearly a breakthrough


		// *+* end *+* one_keeps_two ------------------------------------------------------------------------------------

		// *+* begin *+* two_keeps_three ------------------------------------------------------------------------------------
// ----  white part -----


if (white_free_path == 0) {
	     if (all_white(w2(19,14)) && all_black(w3(5,10,15)) && all_empty(w5(20,25,24,30,35))) white_locked += 4*dunne_stand[node->total_nr_of_pieces];
	else if (all_white(w2(29,24)) && all_black(w3(15,20,25)) && all_empty(w5(30,35,34,40,45))) white_locked += 4*dunne_stand[node->total_nr_of_pieces];
	else if (all_white(w2(39,34)) && all_black(w3(25,30,35)) && all_empty(w4(40,45,44,50))) white_locked += 4*dunne_stand[node->total_nr_of_pieces];
}

if (all_white(w2(24,20)) && all_black(w3(5,10,15)) && !((black_man & w6(4,3,9,2,8,13))) && all_empty(w5(25,30,35,14,19))) white_locked += 700;
else if
   (all_white(w2(34,30)) && all_black(w3(15,20,25)) && !((black_man & w15(5,4,10,3,9,14,2,8,13,19,1,7,13,18,23))) && all_empty(w5(35,40,45,24,29))) white_locked  += 700;
// all_white(w2(44,40)) is not a realistic advantage, attack on 44 may lead to a breakthrough.



if (all_white(w2(21,27)) && all_black(w3(6,11,16)) && !((black_man & w10(1,2,7,3,8,12,4,9,13,18))) && all_empty(w5(26,31,36,17,22))) white_locked += 700;
// all_white(w2(31,37)) is not realistic
// all_white(w2(41,47)) is something you don't want


// ----  black part -----


if (black_free_path == 0) {
	     if (all_black(w2(32,37)) && all_white(w3(46,41,36)) && all_empty(w5(31,26,27,21,16))) black_locked += 4*dunne_stand[node->total_nr_of_pieces];
	else if (all_black(w2(22,27)) && all_white(w3(36,31,26)) && all_empty(w5(21,16,17,11,6))) black_locked += 4*dunne_stand[node->total_nr_of_pieces];
	else if (all_black(w2(12,17)) && all_white(w3(26,21,16)) && all_empty(w4(11,6,7,1))) black_locked += 4*dunne_stand[node->total_nr_of_pieces];
}

if (all_black(w2(27,31)) && all_white(w3(46,41,36)) && !((white_man & w6(47,48,42,49,43,38))) && all_empty(w5(26,21,16,37,32))) black_locked += 700;
else if
   (all_black(w2(17,21)) && all_white(w3(36,31,26)) && !((white_man & w15(46,47,41,48,42,37,49,43,38,32,50,44,38,33,28))) && all_empty(w5(16,11,6,27,22))) black_locked  += 700;
// all_black(w2(7,11)) is not a realistic advantage, attack on 44 may lead to a breakthrough.



if (all_black(w2(30,24)) && all_white(w3(45,40,35)) && !((white_man & w10(50,49,44,48,43,39,47,42,38,33))) && all_empty(w5(25,20,15,34,29))) black_locked += 700;
// all_black(w2(20,14)) is not realistic
// all_black(w2(10,4)) is something you don't want


			// *+* end *+* two_keeps_three ------------------------------------------------------------------------------------


			// *+* just triangles along the edge

			if (all_black((FLD26|FLD36|FLD31)) && (all_empty((FLD46|FLD41|FLD37)))) black_253035 -= 50;
			if (all_black((FLD16|FLD26|FLD21)) && (all_empty((FLD36|FLD31|FLD27)))) black_253035 -= 50;
			if (all_black(w5(26,21,16,17,11)) && all_white(w3(27,28,32)) && all_empty(w4(1,6,22,31))) black_253035 -= 400;

			if (all_black((FLD25|FLD30|FLD35)) && (all_empty((FLD45|FLD40|FLD34)))) black_253035 -= 50;
			else if ((all_white((FLD25|FLD30|FLD35))) && (white_tempi > black_tempi) && (black_man & FLD15))
				        white_253035 += imin(10, white_tempi - black_tempi)*(10*slow_down[node->total_nr_of_pieces])/100;

			if (all_black((FLD15|FLD20|FLD25)) && (all_empty((FLD35|FLD30|FLD24)))) black_253035 -= 50;


			if (all_white((BLD26|BLD36|BLD31)) && (all_empty((BLD46|BLD41|BLD37)))) black_253035 -= 50;
			if (all_white((BLD16|BLD26|BLD21)) && (all_empty((BLD36|BLD31|BLD27)))) white_253035 -= 50;
			if (all_white(b5(26,21,16,17,11)) && all_black(b3(27,28,32)) && all_empty(b4(1,6,22,31))) white_253035 -= 400;

			if (all_white((BLD25|BLD30|BLD35)) && (all_empty((BLD45|BLD40|BLD34)))) white_253035 -= 50;
			else if ((all_black((BLD25|BLD30|BLD35))) && (black_tempi > white_tempi) && (white_man & BLD15))
				black_253035 += imin(10, black_tempi - white_tempi)*(10*slow_down[node->total_nr_of_pieces])/100;

			if (all_white((BLD15|BLD20|BLD25)) && (all_empty((BLD35|BLD30|BLD24)))) white_253035 -= 50;


			// *+* vleugel opsluitingen --------------------------------------------------------------------------------------

			//white

			if (all_white(w3(36,41,46)) && all_black(w4(27,31,32,37))){
				 if (all_empty(w12(26,21,17,12,8,3,16,11,7,2,6,1))) white_vleugel_opsluiting += 700;
				 else if (count_black(w3(26,21,16)) - count_white(w1(47)) >= 0) white_vleugel_opsluiting += 4*dunne_stand[node->total_nr_of_pieces];
			}
			else if (all_white(w3(26,31,36)) && all_black(w4(17,21,22,27))){
					 if (all_empty(w6(16,11,6,7,1,2)))                  white_vleugel_opsluiting += 700;
					 else if (count_black(w3(16,11,6)) - count_white(w3(46,41,37)) >= 0) white_vleugel_opsluiting += 4*dunne_stand[node->total_nr_of_pieces];
			}
			else if (all_white(w3(16,21,26)) && all_black(w4( 7,11,12,17))){
					 if (all_empty(w2(6,1)))                            white_vleugel_opsluiting += 200;
					 else if (count_black(w2(6,1)) - count_white(w3(36,31,27)) >= 0) white_vleugel_opsluiting += 4*dunne_stand[node->total_nr_of_pieces];
			}

			if (all_white(w3(35,40,45)) && all_black(w4(24,29,30,34))){
				 if (all_empty(w9(25,20,14,9,3,15,10,4,5)))         white_vleugel_opsluiting += 700;
				 else if (count_black(w3(25,20,15)) - count_white(w2(50,44)) >= 0) white_vleugel_opsluiting += 4*dunne_stand[node->total_nr_of_pieces];
			}
			else if (all_white(w3(25,30,35)) && all_black(w4(14,19,20,24))){
					 if (all_empty(w4(15,10,4,5)))                      white_vleugel_opsluiting += 500;
					 else if (count_black(w3(15,10,5)) - count_white(w3(45,40,34)) >= 0)  white_vleugel_opsluiting += 4*dunne_stand[node->total_nr_of_pieces];
			}
			else if (all_white(w3(15,20,25)) && all_black(w4( 4, 9,10,14))){
					 if ((empty_field & FLD5))                          white_vleugel_opsluiting += 100;
					 else if (count_black(w1(5)) - count_black(w3(35,30,24)) >= 0) white_vleugel_opsluiting += 4*dunne_stand[node->total_nr_of_pieces];
			}

			if ((all_white((FLD37|FLD36|FLD46|FLD41))) && (empty_field & FLD42) && (all_black((FLD27|FLD28|FLD32))) &&
					((count((black_man & (FLD6|FLD11|FLD16|FLD17|FLD21|FLD22|FLD26))) - count((white_man & (FLD31|FLD47)))) > 1))
				white_vleugel_opsluiting += 4*dunne_stand[node->total_nr_of_pieces];

			if ((all_white((FLD45|FLD40|FLD34|FLD35))) && (empty_field & FLD39) && (all_black((FLD23|FLD24|FLD29))) &&
					((count((black_man & (FLD5|FLD10|FLD14|FLD15|FLD19|FLD20|FLD25))) - count((white_man & (FLD30|FLD44|FLD50)))) > 1))
				white_vleugel_opsluiting += 4*dunne_stand[node->total_nr_of_pieces];

			if ((all_white((FLD26|FLD27|FLD32))) && all_black((FLD21|FLD16|FLD17|FLD11))) {
					if (all_empty((FLD6|FLD1))) white_vleugel_opsluiting += 700;
					else { white_vleugel_opsluiting += 300;
						//if (count((white_man & (FLD27|FLD31|FLD36|FLD37|FLD41|FLD46))) - count(black_man & FLD6) <= 2)
						//	white_vleugel_opsluiting += 4*dunne_stand[node->total_nr_of_pieces];
					}
			}


//black
			if (all_black(b3(36,41,46)) && all_white(b4(27,31,32,37))){
				 if (all_empty(b12(26,21,17,12,8,3,16,11,7,2,6,1))) black_vleugel_opsluiting += 700;
				 else if (count_white(b3(26,21,16)) - count_black(b1(47)) >= 0) black_vleugel_opsluiting += 4*dunne_stand[node->total_nr_of_pieces];
			}
			else if (all_black(b3(26,31,36)) && all_white(b4(17,21,22,27))){
					 if (all_empty(b6(16,11,6,7,1,2)))                  black_vleugel_opsluiting += 700;
		    	     else if (count_white(b3(16,11,6)) - count_black(b3(46,41,37)) >= 0) black_vleugel_opsluiting += 4*dunne_stand[node->total_nr_of_pieces];
			}
			else if (all_black(b3(16,21,26)) && all_white(b4( 7,11,12,17))){
				     if (all_empty(b2(6,1)))                            black_vleugel_opsluiting += 200;
				     else if (count_white(b2(6,1)) - count_black(b3(36,31,27)) >= 0) black_vleugel_opsluiting += 4*dunne_stand[node->total_nr_of_pieces];
			}
			if (all_black(b3(35,40,45)) && all_white(b4(24,29,30,34))){
				 if (all_empty(b9(25,20,14,9,3,15,10,4,5)))         black_vleugel_opsluiting += 700;
				 else if (count_white(b3(25,20,15)) - count_black(b2(50,44)) >= 0) black_vleugel_opsluiting += 4*dunne_stand[node->total_nr_of_pieces];
			}
			else if (all_black(b3(25,30,35)) && all_white(b4(14,19,20,24))){
				     if (all_empty(b4(15,10,4,5)))                      black_vleugel_opsluiting += 500;
				     else if (count_white(b3(15,10,5)) - count_black(b3(45,40,34)) >= 0)  black_vleugel_opsluiting += 4*dunne_stand[node->total_nr_of_pieces];
			}
			else if (all_black(b3(15,20,25)) && all_white(b4( 4, 9,10,14))){
				     if ((empty_field & BLD5))                          black_vleugel_opsluiting += 100;
				     else if (count_white(b1(5)) - count_white(b3(35,30,24)) >= 0) black_vleugel_opsluiting += 4*dunne_stand[node->total_nr_of_pieces];
			}

			if ((all_black((BLD37|BLD36|BLD46|BLD41))) && (empty_field & BLD42) && (all_white((BLD27|BLD28|BLD32))) &&
					((count((white_man & (BLD6|BLD11|BLD16|BLD17|BLD21|BLD22|BLD26))) - count((black_man & (BLD31|BLD47)))) > 1))
				black_vleugel_opsluiting += 4*dunne_stand[node->total_nr_of_pieces];

			if ((all_black((BLD45|BLD40|BLD34|BLD35))) && (empty_field & BLD39) && (all_white((BLD23|BLD24|BLD29))) &&
					((count((white_man & (BLD5|BLD10|BLD14|BLD15|BLD19|BLD20|BLD25))) - count((black_man & (BLD30|BLD44|BLD50)))) > 1))
				black_vleugel_opsluiting += 4*dunne_stand[node->total_nr_of_pieces];

			if ((all_black((BLD26|BLD27|BLD32))) && all_white((BLD21|BLD16|BLD17|BLD11))) {
					if (all_empty((BLD6|BLD1))) black_vleugel_opsluiting += 700;
					else { black_vleugel_opsluiting += 300;
						//if (count((black_man & (BLD27|BLD31|BLD36|BLD37|BLD41|BLD46))) - count(white_man & BLD6) <= 2)
						//	black_vleugel_opsluiting += 4*dunne_stand[node->total_nr_of_pieces];
					}
			}
			// twee om twee, maar wit kan kiezen of hij weg wil
/*			if (black_vleugel_opsluiting == 0){
				if ((all_black(w2(35,29))) && all_white(w2(40,44)) && all_empty(w3(30,34,39))) white_253035 -= 210;
				else if ((all_black(w2(25,19))) && all_white(w2(30,34)) && all_empty(w3(20,24,29))) white_253035 -= 210;
				else if ((all_black(w2(15, 9))) && all_white(w2(20,24)) && all_empty(w3(10,14,19))) white_253035 -= 210;
				if ((all_black(w2(26,22))) && all_white(w2(31,37)) && all_empty(w3(21,27,32))) white_253035 -= 210;
				else if ((all_black(w2(16,12))) && all_white(w2(21,27)) && all_empty(w3(11,17,12))) white_253035 -= 210;
			}
			if (white_vleugel_opsluiting == 0){
				if ((all_white(b2(35,29))) && all_black(b2(40,44)) && all_empty(b3(30,34,39))) black_253035 -= 210;
				else if ((all_white(b2(25,19))) && all_black(b2(30,34)) && all_empty(b3(20,24,29))) black_253035 -= 210;
				else if ((all_white(b2(15, 9))) && all_black(b2(20,24)) && all_empty(b3(10,14,19))) black_253035 -= 210;
				if ((all_white(b2(26,22))) && all_black(b2(31,37)) && all_empty(b3(21,27,32))) black_253035 -= 210;
				else if ((all_white(b2(16,12))) && all_black(b2(21,27)) && all_empty(b3(11,17,12))) black_253035 -= 210;
			}
*/			if (black_vleugel_opsluiting == 0){
				if ((all_black(w2(35,29))) && all_white(w2(40,44)))      white_253035 -= 210;
				else if ((all_black(w2(25,19))) && all_white(w2(30,34))) white_253035 -= 210;
				else if ((all_black(w2(15, 9))) && all_white(w2(20,24))) white_253035 -= 210;
				if ((all_black(w2(26,22))) && all_white(w2(31,37)))      white_253035 -= 210;
				else if ((all_black(w2(16,12))) && all_white(w2(21,27))) white_253035 -= 210;
			}
			if (white_vleugel_opsluiting == 0){
				if ((all_white(b2(35,29))) && all_black(b2(40,44)))      black_253035 -= 210;
				else if ((all_white(b2(25,19))) && all_black(b2(30,34))) black_253035 -= 210;
				else if ((all_white(b2(15, 9))) && all_black(b2(20,24))) black_253035 -= 210;
				if ((all_white(b2(26,22))) && all_black(b2(31,37)))      black_253035 -= 210;
				else if ((all_white(b2(16,12))) && all_black(b2(21,27))) black_253035 -= 210;
			}
		//} // end vleugelopsluitingen

	// *+* mobility:  the number of direct reachable fields with no immediate capture ------------------------------------------------

		int white_temp = freedom(1, empty_field, white_man, black_man);
		int black_temp = freedom(0, empty_field, black_man, white_man);

		//printf("mob %d, %d\n", white_temp, black_temp);

		if (node->status & IS_WHITE_ON_MOVE) { if (white_temp) white_temp--;}
		else { if (black_temp) black_temp--;}

		//printf("mob %d, %d\n", white_temp, black_temp);

		int distance_correction = calc_black_white_distance(white_man, black_man);
		if (distance_correction < 30) distance_correction = 30;

		//printf("dist %d\n", distance_correction);

		distance_correction = ((distance_correction - 26)*100)/4;
		//printf("dist %d\n", distance_correction);

		if (white_temp == black_temp) {
			white_mobility = 0;
			black_mobility = 0;
		}
		else
			if (white_temp > black_temp){
				black_mobility = 0;
				white_mobility = mobility_value[imin(white_temp-black_temp, 2)][imin(black_temp, 8)];
				//printf("white mob %d\n", white_mobility);
				if (is_klassiek)
					white_mobility = (white_mobility)/distance_correction;
				else
					white_mobility = (mobility_sensivity[node->total_nr_of_pieces]*white_mobility)/distance_correction;
				//printf("white mob %d, corr %d\n", white_mobility, mobility_sensivity[node->total_nr_of_pieces]);
			}
			else {
				white_mobility = 0;
				black_mobility = mobility_value[imin(black_temp-white_temp, 2)][imin(white_temp, 8)];
				//printf("black mob %d\n", black_mobility);
				if (is_klassiek)
					black_mobility = (black_mobility)/distance_correction;
				else
					black_mobility = (mobility_sensivity[node->total_nr_of_pieces]*black_mobility)/distance_correction;
				//printf("black mob %d, corr %d\n", black_mobility, mobility_sensivity[node->total_nr_of_pieces]);
			}


		// *+* klassiek calculations -----------------------------------------------------------------------------------

		if (is_klassiek)
		{	white_klassiek = calc_white_klassiek(white_man, white_tempi, black_tempi);
			black_klassiek = calc_black_klassiek(black_man, white_tempi, black_tempi);
			//printf("white black %d %d\n", white_klassiek, black_klassiek);
			white_tempi_bonus = 0;	// no further contribution
			black_tempi_bonus = 0;
			white_centrum = 0;
			black_centrum = 0;
			white_fit = 0;
			black_fit = 0;
		}
	}

		// nearly klassiek

		/* if white is on move and able to make klassiek in the next move and klassiek is in favor of white, then apply the klassiek score
		 *
		 */

		if ((node->status & IS_WHITE_ON_MOVE) && all_black(w3(19,23,24)) &&
				(((all_white(w3(27,32,33)) && (empty_field & w1(28)))) || (all_white(w3(31,32,33)) && (empty_field & w1(27))))) {
			int white_klassiek_t = calc_white_klassiek(white_man, white_tempi, black_tempi);
			int black_klassiek_t = calc_black_klassiek(black_man, white_tempi, black_tempi);
			//printf("nearly: white %d, black %d",white_klassiek_t,black_klassiek_t);
			if ((white_klassiek_t - black_klassiek_t) >= white_tempi_bonus + white_centrum + white_fit - (black_tempi_bonus + black_centrum + black_fit)){
				white_klassiek = white_klassiek_t;
				black_klassiek = black_klassiek_t;
				white_tempi_bonus = 0;	// no further contribution
				black_tempi_bonus = 0;
				white_centrum = 0;
				black_centrum = 0;
				white_fit = 0;
				black_fit = 0;

			}
		}
		else if (!(node->status & IS_WHITE_ON_MOVE) && all_white(w3(27,28,32)) &&
				(((all_black(w3(18,19,24)) && (empty_field & w1(23)))) || (all_black(w3(23,19,20)) && (empty_field & w1(24))))) {
			int white_klassiek_t = calc_white_klassiek(white_man, white_tempi, black_tempi);
			int black_klassiek_t = calc_black_klassiek(black_man, white_tempi, black_tempi);
			//printf("nearly: white %d, black %d",white_klassiek_t,black_klassiek_t);
			if ((black_klassiek_t - white_klassiek_t) >= 100){
				white_klassiek = white_klassiek_t;
				black_klassiek = black_klassiek_t;
				white_tempi_bonus = 0;	// no further contribution
				black_tempi_bonus = 0;
				white_centrum = 0;
				black_centrum = 0;
				white_fit = 0;
				black_fit = 0;
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

	if (white_halve_hek || black_halve_hek){
		white_fit /= 2;
		black_fit /= 2;
		white_man_position /= 3;
		black_man_position /= 3;
		white_tempi_bonus = (white_tempi_bonus*3)/2;
		black_tempi_bonus = (black_tempi_bonus*3)/2;
	}

// define contribution
#define man_position_contribution 100/100
#define king_position_contribution 100/100
#define centrum_contribution 100/100
#define binding_contribution 100/100
#define fit_contribution 100/100
#define tempi_contribution 100/100
#define mobility_contribution 100/100
#define edge_contribution 150/100

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
		wcount_item(white_corner_35, 21) +
		wcount_item(white_corner_36, 22) +
		wcount_item(white_11_16_17, 23) +
		wcount_item(white_voorpost, 24) +
		wcount_item(white_locked, 25) +
		wcount_item(white_vleugel_opsluiting, 26) +
		wcount_item(white_tandem, 27) +
		wcount_item(white_mobility * mobility_contribution, 28) +
		wcount_item(white_edge * edge_contribution, 29) +
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
		bcount_item(black_corner_35, 21) +
		bcount_item(black_corner_36, 22) +
		bcount_item(black_11_16_17, 23) +
		bcount_item(black_voorpost, 24) +
		bcount_item(black_locked, 25) +
		bcount_item(black_vleugel_opsluiting, 26) +
		bcount_item(black_tandem, 27) +
		bcount_item(black_mobility * mobility_contribution, 28) +
		bcount_item(black_edge * edge_contribution, 29) +
		bcount_item(black_canon, 30)
		;

	if (print_flag)
	{	printf("     \tman\tking\tlocked\tman pos\tkng pos\tcentre\n");
		printf("White\t%d\t%d\t%d\t%d\t%d\t%d\n"  , node->nr_of_white_man*1000, node->nr_of_white_king*white_king_value, nr_of_white_man_locked, white_man_position*man_position_contribution, white_king_position*king_position_contribution, white_centrum*centrum_contribution);
		printf("Black\t%d\t%d\t%d\t%d\t%d\t%d\n\n", node->nr_of_black_man*1000, node->nr_of_black_king*black_king_value, nr_of_black_man_locked, black_man_position*man_position_contribution, black_king_position*king_position_contribution, black_centrum*centrum_contribution);

		printf("     \tedge\t253035\t2813\tcanon\tlckbns\t4641\t2324\t2335\n");
		printf("White\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n"  , white_edge*edge_contribution, white_253035, white_2_8_13, white_canon, white_man_locked_bonus, white_avoid_4641, white_avoid_2324, white_avoid_2335);
		printf("Black\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n\n", black_edge*edge_contribution, black_253035, black_2_8_13, black_canon, black_man_locked_bonus, black_avoid_4641, black_avoid_2324, black_avoid_2335);

		printf("     \tketting\th hek\tact_15\ttempi\tklssiek\tbindng\tvr dg\tfit\n");
		printf("White\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n"  , white_ketting_stelling * 400 + white_ketting_stelling_2, white_halve_hek, white_active_15, white_tempi_bonus*tempi_contribution, white_klassiek, white_slechte_binding*binding_contribution, white_free_path, white_fit*fit_contribution);
		printf("Black\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n"  , black_ketting_stelling * 400 + black_ketting_stelling_2, black_halve_hek, black_active_15, black_tempi_bonus*tempi_contribution, black_klassiek, black_slechte_binding*binding_contribution, black_free_path, black_fit*fit_contribution);

		printf("     \ttri_11\tvoorp\tlock\tvleug\tcentrum\tmoblty\tscore\n");
		printf("White\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n"  , white_11_16_17, white_voorpost, white_locked, white_vleugel_opsluiting, white_tandem, white_mobility*mobility_contribution, white);
		printf("Black\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n"  , black_11_16_17, black_voorpost, black_locked, black_vleugel_opsluiting, black_tandem, black_mobility*mobility_contribution, black);

		printf("     \tp 35\tp 36\n");
		printf("White\t%d\t%d\n"  , white_corner_35, white_corner_36);
		printf("Black\t%d\t%d\n"  , black_corner_35, black_corner_36);

		printf("\n\n");
	}
	// decrease score if there are kings or nearly kings at both sides
	if (((node->nr_of_white_king > 0) || (white_free_path > 0)) && ((node->nr_of_black_king > 0) || (black_free_path > 0))) {
		white = white/4; black = black/4;
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
		j += sprintf(result+j, "%5d",white_edge* edge_contribution);
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
		j += sprintf(result+j, "%5d",white_corner_35);
		j += sprintf(result+j, "%5d",white_corner_36);
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
		j += sprintf(result+j, "%5d",black_edge* edge_contribution);
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
		j += sprintf(result+j, "%5d",black_corner_35);
		j += sprintf(result+j, "%5d",black_corner_36);
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

