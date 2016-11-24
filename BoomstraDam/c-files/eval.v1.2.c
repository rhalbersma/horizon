/*
  This file is Copyright (C) 2010 Feike Boomstra.
  Distributed under the Boost Software License, Version 1.0.
  (See accompanying file LICENSE_1_0.txt or copy at
  http://www.boost.org/LICENSE_1_0.txt)
*/


#include "BoomstraDam.v1.2.h"
#include "eval.h"

short int white_man_dik_pos_value[55] = {
0, 0, 0, 0, 0, 500, 1000, 1000, 1000, 1000, 0, 100, 200, 200, 200, 100, 70, 100, 100, 100, 90, 0, 50, 200, 200, 200, 70, 30, 150, 200, 150, 50, 0, 50, 100, 100, 100, 40, 40, 100, 100, 100, 50, 0, 50, 100, 100, 50, 30, -100, 50, 200, 50, 10, 0
};
short int black_man_dik_pos_value[55] = {
10, 50, 200, 50, -100, 30, 50, 100, 100, 50, 0, 50, 100, 100, 100, 40, 40, 100, 100, 100, 50, 0, 50, 150, 200, 150, 30, 70, 200, 200, 200, 50, 0, 90, 100, 100, 100, 70, 100, 200, 200, 200, 100, 0, 1000, 1000, 1000, 1000, 500, 0, 0, 0, 0, 0, 0
};
short int white_man_dun_pos_value[55] = {
0, 0, 0, 0, 0, 500, 1000, 1000, 1000, 1000, 0, 250, 500, 500, 500, 250, -50, 300, 300, 300, 0, 0, 0, 200, 200, 200, 0, 0, 140, 160, 140, 50, 0, 50, 110, 110, 110, 0, 30, 70, 70, 70, 35, 0, 20, 30, 30, 20, 10, -100, 0, 0, 0, 0, 0
};
short int black_man_dun_pos_value[55] = {
0, 0, 0, 0, -100, 10, 20, 30, 30, 20, 0, 35, 70, 70, 70, 30, 0, 110, 110, 110, 50, 0, 50, 140, 160, 140, 0, 0, 200, 200, 200, 0, 0, 0, 300, 300, 300, -50, 250, 500, 500, 500, 250, 0, 1000, 1000, 1000, 1000, 500, 0, 0, 0, 0, 0, 0, 
};

short int white_man_position_in_case_of_kings[55] = {
	0, 0, 0, 0, 0, 500, 1000, 1000, 1000, 1000, 0, 280, 0, 0, 140, 400, 200, 70, 0, 0, 280, 0, 140, 0, 0, 70, 200, 100, 35, 0, 0, 140, 0, 70, 0, 0, 35, 100, 50, 18, 0, 0, 70, 0, 35, 0, 0, 18, 50, 25, 9, 0, 0, 35, 0};

short int black_man_position_in_case_of_kings[55] = {
	35, 0, 0, 9, 25, 50, 18, 0, 0, 35, 0, 70, 0, 0, 18, 50, 100, 35, 0, 0, 70, 0, 140, 0, 0, 35, 100, 200, 70, 0, 0, 140, 0, 280, 0, 0, 70, 200, 400, 140, 0, 0, 280, 0, 1000, 1000, 1000, 1000, 500, 0, 0, 0, 0, 0, 0};


short int king_pos_value[55] = {	40,		10,		10,		10,		200,	40,		20,		0,		0,		50,		0,
									20,		20,		0,		50,		10,		10,		20,		20,		50,		0,		0,
									0,		20,		50,		0,		10,		10,		0,		50,		20,		0,		0,
									0,		50,		20,		20,		10,		10,		50,		0,		20,		20,		0,
									50,		0,		0,		20,		40,		200,	10,		10,		10,		40};

BitArray black_row_masks[52] = {
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
BitArray include_bits[52] = {
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
BitArray exclude_bits[52] = {
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
BitArray white_row_masks[52] = {
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

unsigned char c2[21][21] = {
{0,0,0,0,0,10,11,12,13,13,14,15,15,16,17,17,18,18,19,19,20},
{0,0,0,0,0,0,0,0,0,0,7,8,9,10,11,12,12,13,14,14,15},
{0,0,25,18,13,10,0,0,0,0,0,0,0,0,0,6,7,8,8,9,10},
{0,0,0,31,25,20,16,14,11,9,7,0,0,0,0,0,0,0,0,0,5},
{0,0,0,0,36,30,26,22,19,16,14,12,10,9,7,6,0,0,0,0,0},
{0,0,0,0,0,40,35,30,27,24,21,19,17,15,13,12,10,9,7,6,5},
{0,0,0,0,0,0,44,39,35,31,28,26,23,21,19,17,16,14,13,11,10},
{0,0,0,0,0,0,0,47,43,39,35,32,30,27,25,23,21,20,18,16,15},
{0,0,0,0,0,0,0,0,51,46,42,39,36,33,31,29,27,25,23,22,20},
{0,0,0,0,0,0,0,0,0,54,49,46,43,40,37,35,32,30,28,27,25},
{0,0,0,0,0,0,0,0,0,0,57,53,49,46,43,40,38,36,34,32,30},
{0,0,0,0,0,0,0,0,0,0,0,59,56,52,49,46,44,41,39,37,35},
{0,0,0,0,0,0,0,0,0,0,0,0,62,58,55,52,49,47,44,42,40},
{0,0,0,0,0,0,0,0,0,0,0,0,0,64,61,58,55,52,50,47,45},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,67,64,60,57,55,52,50},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,69,66,63,60,57,55},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,72,68,65,63,60},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,74,71,68,65},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,76,73,70},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,78,75},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,80}
};
unsigned char c3[21][21] = {
{0,0,0,0,11,12,13,14,15,16,17,18,19,20,20,21,22,23,23,24,24},
{0,0,0,0,0,0,0,8,9,10,12,13,14,15,16,16,17,18,19,20,20},
{0,0,18,12,0,0,0,0,0,0,6,7,8,10,11,12,13,14,15,15,16},
{0,0,0,22,16,12,9,0,0,0,0,0,0,0,6,7,8,9,10,11,12},
{0,0,0,0,26,20,16,13,10,8,6,0,0,0,0,0,0,5,6,7,8},
{0,0,0,0,0,29,24,20,17,14,12,9,7,6,0,0,0,0,0,0,4},
{0,0,0,0,0,0,31,27,23,20,17,15,13,11,9,7,5,0,0,0,0},
{0,0,0,0,0,0,0,34,30,26,23,20,18,16,14,12,10,8,7,5,4},
{0,0,0,0,0,0,0,0,36,32,29,26,23,21,19,16,15,13,11,10,8},
{0,0,0,0,0,0,0,0,0,38,35,31,28,26,23,21,19,17,15,14,12},
{0,0,0,0,0,0,0,0,0,0,40,37,34,31,28,26,24,22,20,18,16},
{0,0,0,0,0,0,0,0,0,0,0,42,39,36,33,31,28,26,24,22,20},
{0,0,0,0,0,0,0,0,0,0,0,0,44,41,38,35,33,31,28,26,24},
{0,0,0,0,0,0,0,0,0,0,0,0,0,46,43,40,37,35,33,31,29},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,48,45,42,39,37,35,33},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,49,47,44,41,39,37},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,51,48,46,43,41},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,53,50,47,45},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,54,52,49},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,56,53},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,57}
};
unsigned char c4[21][21] = {
{0,0,0,11,13,14,15,17,18,19,20,21,22,23,24,24,25,26,27,28,28},
{0,0,0,0,0,7,9,11,12,14,15,16,17,18,19,20,21,22,23,24,25},
{0,0,13,0,0,0,0,0,7,8,10,11,13,14,15,16,17,18,19,20,21},
{0,0,0,16,11,7,0,0,0,0,5,7,8,10,11,12,13,15,16,17,18},
{0,0,0,0,19,14,10,7,0,0,0,0,0,5,7,8,9,11,12,13,14},
{0,0,0,0,0,21,17,13,10,7,5,0,0,0,0,4,6,7,8,9,11},
{0,0,0,0,0,0,23,19,16,13,10,8,5,0,0,0,0,0,4,6,7},
{0,0,0,0,0,0,0,25,21,18,15,12,10,8,6,4,0,0,0,0,4},
{0,0,0,0,0,0,0,0,27,23,20,17,15,12,10,8,6,5,0,0,0},
{0,0,0,0,0,0,0,0,0,28,25,22,19,17,14,12,10,8,7,5,4},
{0,0,0,0,0,0,0,0,0,0,30,27,24,21,19,16,14,12,10,9,7},
{0,0,0,0,0,0,0,0,0,0,0,31,28,25,23,20,18,16,14,12,11},
{0,0,0,0,0,0,0,0,0,0,0,0,33,30,27,24,22,20,18,16,14},
{0,0,0,0,0,0,0,0,0,0,0,0,0,34,31,29,26,24,22,20,18},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,35,33,30,28,25,23,21},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,37,34,31,29,27,25},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,38,35,33,30,28},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,39,37,34,32},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,40,38,35},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,41,39},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,42}
};
unsigned char c5[21][21] = {
{0,0,10,12,14,16,17,19,20,21,22,23,24,25,26,27,28,29,30,31,32},
{0,0,0,0,7,9,12,13,15,16,18,19,20,22,23,24,25,26,27,28,28},
{0,0,10,0,0,0,6,8,10,12,13,15,16,18,19,20,21,22,23,24,25},
{0,0,0,12,7,0,0,0,5,7,9,11,12,14,15,16,18,19,20,21,22},
{0,0,0,0,14,9,6,0,0,0,4,6,8,10,11,13,14,15,17,18,19},
{0,0,0,0,0,16,12,8,5,0,0,0,4,6,8,9,11,12,13,15,16},
{0,0,0,0,0,0,17,13,10,7,4,0,0,0,4,5,7,9,10,11,13},
{0,0,0,0,0,0,0,19,15,12,9,6,4,0,0,0,4,5,7,8,9},
{0,0,0,0,0,0,0,0,20,16,13,11,8,6,4,0,0,0,3,5,6},
{0,0,0,0,0,0,0,0,0,21,18,15,12,10,8,5,4,0,0,0,3},
{0,0,0,0,0,0,0,0,0,0,22,19,16,14,11,9,7,5,3,0,0},
{0,0,0,0,0,0,0,0,0,0,0,23,20,18,15,13,11,9,7,5,3},
{0,0,0,0,0,0,0,0,0,0,0,0,24,22,19,16,14,12,10,8,6},
{0,0,0,0,0,0,0,0,0,0,0,0,0,25,23,20,18,15,13,11,9},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,26,24,21,19,17,15,13},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,27,25,22,20,18,16},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,28,26,23,21,19},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,29,27,24,22},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,30,28,25},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,31,28},
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,32},
};






int more_4_pieces_bonus = 500;
// men on a row
int row_3_value[21] = {165,165,165,165,165,165,165,165,165,165,164,162,159,155,150,144,137,129,120,110,99};
int row_4_value[21] = {182,182,182,182,182,182,182,182,182,182,180,178,175,171,165,158,1151,142,132,121,109};
int row_5_value[21] = {187,187,187,187,187,187,187,187,187,187,186,184,180,176,170,163,155,146,136,125,112};
int row_6_value[21] = {189,189,189,189,189,189,189,189,189,189,188,186,182,178,172,165,157,148,138,126,114};

char dikke_stand[41] = {
0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	1,	1,	1,	2,	3,	4,	6,	8,	12,	17,	25,	35,	50,	65,	75,	83,	88,	92,	94,	96,	97,	98,	99,	99,	99,	100, 100, 100, 100, 100, 100};
char dunne_stand[41] = {
100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 99, 99, 99, 98, 97, 96, 94, 92, 88, 83, 75, 65, 50, 35, 25, 17, 12, 8, 6, 4, 3, 2, 1, 1, 1, 0, 0, 0, 0, 0, 0};


int king_value_without_other_king[21] = {2500, 2500, 2500, 2600, 2700, 2800, 2900, 3000, 3100, 3200, 3200, 3200, 3200, 3000, 2800, 2600, 2400, 2200, 2000, 1800, 1600} ;									
int king_value_with_other_king[21] =    {3200, 3200, 3200, 3200, 3200, 3200, 3200, 3200, 3200, 3200, 3200, 3200, 3200, 3000, 2800, 2600, 2400, 2200, 2000, 1800, 1600} ;

int freedom_correction[5][5] = {
	{   0,  200,  400, 600, 800},
	{-200,    0,  100, 200, 400},
	{-400, -100,    0,  50, 125},
	{-600, -200,   50,   0,  25},
	{-800, -400, -125, -25,   0}
};

int slechte_binding_score[3][15] = {
{-400,	-300,	-200,	-100,	   0,	  50,	 100,	 130,	 140,	150,	160,	170,	180,	190,	200},
{-500,	-400,	-300,	-200,	-100,	   0,	  40,	  80,	 100,   110,	120,	130,	140,	150,	160},
{-600,	-500,	-400,	-300,	-200,	-100,	   0,	  40,	  80,	 100,   110,	120,	130,	140,	150}
};

long nr_of_evaluations;
struct piece_count_record org_piece_count;




 int less_two(BitArray white_man, const unsigned long long mask)
{	int counter;
	count_bits((white_man & mask), counter)
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


short int evaluate(NodePnt node, char print_flag, char log_flag )			
/***************************************************
 * this function is only called from a min-node    *
 * so it is always the other party that is on move *
 * it is only called in a quiet state ..  no       *
 * captures are possible.                          *
 ***************************************************/
{	register int i;
//	register BitArray t;
	BitArray white_man = 0;
	BitArray black_man = 0;
	BitArray white_king = 0;
	BitArray black_king = 0;
	BitArray empty_field = 0;
	char nr_of_white_man;
	char nr_of_black_man ;
	char nr_of_white_king;
	char nr_of_black_king;
	int white_man_position_score = 0;
	int black_man_position_score = 0;
	int white_king_position_score = 0;
	int black_king_position_score = 0;
	unsigned long white_man_idx[21];
	unsigned long white_king_idx[21];
	unsigned long black_man_idx[21];
	unsigned long black_king_idx[21];
	int white_row_3_count = 0;
	int black_row_3_count = 0;
	int white_row_4_count = 0;
	int black_row_4_count = 0;
	int white_row_5_count = 0;
	int black_row_5_count = 0;
	int white_row_6_count = 0;
	int black_row_6_count = 0;
	int white_row_score = 0;
	int black_row_score = 0;
	int white_more_4_pieces_bonus = 0;
	int black_more_4_pieces_bonus = 0;
	char nr_of_white_man_locked = 0;
	char nr_of_black_man_locked = 0;
	BitArray white_man_locked = 0;
	BitArray black_man_locked = 0;
	int white_man_locked_bonus = 0;
	int black_man_locked_bonus = 0;
	int white_avoid_4641_bonus = 0;
	int black_avoid_4641_bonus = 0;
	int white_avoid_2324_bonus = 0;
	int black_avoid_2324_bonus = 0;
	int white_avoid_2335_bonus = 0;
	int black_avoid_2335_bonus = 0;
	int white_ketting_stelling = 0;
	int black_ketting_stelling = 0;
	int is_klassiek = 0;
	int white_halve_hek = 0;
	int white_halve_hek_ = 0;
	int black_halve_hek = 0;
	int black_halve_hek_ = 0;
	int white_tempi = 0;
	int black_tempi = 0;
//	int white_free_moves = 0;
//	int black_free_moves = 0;
	int white_klassiek_score = 0;
	int black_klassiek_score = 0;
	int white_tempi_score = 0;
	int black_tempi_score = 0;
	int white_slechte_binding = 0;
	int black_slechte_binding = 0;
	int nr_of_white_free_path = 0;
	int nr_of_black_free_path = 0;
	int white_fit_score=0;
	int black_fit_score=0;
	int white_36_47_bonus = 0;
	int black_36_47_bonus = 0;
	int white_11_16_17_bonus = 0;
	int black_11_16_17_bonus = 0;
	int white_voorpost_score = 0;
	int black_voorpost_score = 0;
	int white_locked_bonus = 0;
	int black_locked_bonus = 0;
	int white_vleugel_opsluiting = 0;
	int black_vleugel_opsluiting = 0;
	int white_king_value;
	int black_king_value;
	int nr_of_pieces;
	int white_tandem_bonus = 0;
	int black_tandem_bonus = 0;
	int white_freedom;
	int black_freedom;
	int black_score;
	int white_score;

	short int score;

	if (node == NULL) { printf("NULL node in eval\n"); return 0; } ;
	white_man = (node->occ_empty) & (node->white_black) & (node->man_king);
	white_king = (node->occ_empty) & (node->white_black) & ~(node->man_king);
	black_man = (node->occ_empty) & ~(node->white_black) & (node->man_king);
	black_king = (node->occ_empty) & ~(node->white_black) & ~(node->man_king);
	empty_field = ~(node->occ_empty) & BITS50;
	
	nr_of_evaluations++;
	count_bits_and_fill_list(white_man, nr_of_white_man, white_man_idx);
	count_bits_and_fill_list(black_man, nr_of_black_man, black_man_idx);
	if (white_king)
	{	count_bits_and_fill_list(white_king, nr_of_white_king, white_king_idx);}
	else nr_of_white_king = 0;
	if (black_king)
	{	count_bits_and_fill_list(black_king, nr_of_black_king, black_king_idx);}
	else nr_of_black_king = 0;

	nr_of_pieces = nr_of_white_man + nr_of_black_man + nr_of_white_king + nr_of_black_king;
	
	if (nr_of_pieces <= NR_OF_ENDGAME_PIECES) 
	{	int endgame_score = evaluate_endgame_position(  node->status & IS_WHITE_ON_MOVE, 
													nr_of_white_man, &white_man_idx[0], 
													nr_of_white_king, &white_king_idx[0], 
													nr_of_black_man, &black_man_idx[0], 
													nr_of_black_king, &black_king_idx[0]);
		if (endgame_score != 0)
		{	if (print_flag) printf("from endgame database: score = %d\n", endgame_score);
			if (log_flag)	
			{	_fcloseall(); open_log_file();
				fprintf_s(log_file,"from endgame database: score = %d\n", endgame_score);
				_fcloseall();
			};
			node->status = node->status | IS_EXACT_VALUE;
			//if (node->status & IS_WHITE_ON_MOVE)
				return -endgame_score;
			//else return endgame_score;
		}
	}

	
	// calculate king value:

	if (nr_of_black_king) white_king_value = king_value_with_other_king[nr_of_white_man];
	else white_king_value = king_value_without_other_king[nr_of_white_man];
	if (nr_of_white_king) black_king_value = king_value_with_other_king[nr_of_black_man];
	else black_king_value = king_value_without_other_king[nr_of_black_man];

	/* In general you can't win with less that 4 pieces */

	if (nr_of_white_man + nr_of_white_king >= 4) white_more_4_pieces_bonus = more_4_pieces_bonus;
	if (nr_of_black_man + nr_of_black_king >= 4) black_more_4_pieces_bonus = more_4_pieces_bonus;

	/****************************************************************************************************************************************
	 *  now we calculate the position value of the individual pieces                                                                        *
	 *  for each party, we have two cases                                                                                                 *
	 *	the other party has no kings:				  evaluate with value_of_man_position and patterns                                      *
	 *	the other party has one or more kings:	      evaluate with value_of_man_position_in_case_of_kings and value_of_king_position       *
	 ****************************************************************************************************************************************/
	
	if (nr_of_black_king == 0)
	{	int idx; for (i=0; i<nr_of_white_man; i++)
		{	idx = white_man_idx[i];
			//if (log_flag) printf("white man : %d at internal (%d), external (%d)\n",white_man_pos_value[idx], idx, int_to_ext[idx]); 
			white_man_position_score += (dikke_stand[nr_of_pieces]*white_man_dik_pos_value[idx] + dunne_stand[nr_of_pieces]*white_man_dun_pos_value[idx])/100;
		}
	}
	else
	{	int idx; for (i=0; i<nr_of_white_man; i++)
		{	idx = white_man_idx[i];
			white_man_position_score += white_man_position_in_case_of_kings[idx];
		}
		for (i=0; i<nr_of_black_king; i++)
		{	idx = black_king_idx[i];
			black_king_position_score += king_pos_value[idx];
		}	
	}

	if (nr_of_white_king == 0)
	{	int idx; for (i=0; i<nr_of_black_man; i++)
		{	idx = black_man_idx[i];
			//if (log_flag) printf("black man : %d at internal (%d), external (%d)\n",black_man_pos_value[idx], idx, int_to_ext[idx]); 
			black_man_position_score += (dikke_stand[nr_of_pieces]*black_man_dik_pos_value[idx] + dunne_stand[nr_of_pieces]*black_man_dun_pos_value[idx])/100;
		}
	}
	else
	{	int idx; for (i=0; i<nr_of_black_man; i++)
		{	idx = black_man_idx[i];
			black_man_position_score += black_man_position_in_case_of_kings[idx];
		}
		for (i=0; i<nr_of_white_king; i++)
		{	idx = white_king_idx[i];
			white_king_position_score += king_pos_value[idx];
		}
	}
	

	// count the number of locked men. A locked man is a man that can't reach the damlijn.
	// Lxx is a line definition Bxx are the fields that are blocked by that line

	if ((nr_of_white_king > 0) && (nr_of_black_king == 0))
	{	if ((white_king & L16NW) && ((white_man & L16NW) == 0)) { black_man_locked = black_man_locked | (black_man & B16NW); }
		if ((white_king & L26NW) && ((white_man & L26NW) == 0)) { black_man_locked = black_man_locked | (black_man & B26NW); }
		if ((white_king & L36NW) && ((white_man & L36NW) == 0)) { black_man_locked = black_man_locked | (black_man & B36NW); }
		if ((white_king & L4SW)  && ((white_man & L4SW)  == 0)) { black_man_locked = black_man_locked | (black_man & B4SW); }
		if ((white_king & L3SW)  && ((white_man & L3SW)  == 0)) { black_man_locked = black_man_locked | (black_man & B3SW); }
		if ((white_king & L2SW)  && ((white_man & L2SW)  == 0)) { black_man_locked = black_man_locked | (black_man & B2SW); }
		if ((white_king & L1SW)  && ((white_man & L1SW)  == 0)) { black_man_locked = black_man_locked | (black_man & B1SW); }
	}
	if (nr_of_white_king > 0)
	{	if (((white_king & DIAGLOW) && ((white_man & DIAGLOW) == 0)) || ((white_king & DIAGHIGH) && ((white_man & DIAGHIGH) == 0))) 
			{ black_man_locked = black_man_locked | (black_man & B46NW); }
		count_bits(black_man_locked, nr_of_black_man_locked)
	}  // all locked black man


	if ((nr_of_black_king > 0) && (nr_of_white_king == 0))
	{	if ((black_king & L6SW)  && ((black_man & L6SW)  == 0)) { white_man_locked = white_man_locked | (white_man & B6SW); }
		if ((black_king & L16SW) && ((black_man & L16SW) == 0)) { white_man_locked = white_man_locked | (white_man & B6SW); }
		if ((black_king & L26SW) && ((black_man & L26SW) == 0)) { white_man_locked = white_man_locked | (white_man & B6SW); }
		if ((black_king & L36SW) && ((black_man & L36SW) == 0)) { white_man_locked = white_man_locked | (white_man & B6SW); }
		if ((black_king & L15SE) && ((black_man & L15SE) == 0)) { white_man_locked = white_man_locked | (white_man & B15SE); }
		if ((black_king & L25SE) && ((black_man & L25SE) == 0)) { white_man_locked = white_man_locked | (white_man & B25SE); }
		if ((black_king & L35SE) && ((black_man & L35SE) == 0)) { white_man_locked = white_man_locked | (white_man & B35SE); }
	}
	if (nr_of_black_king > 0)
	{	if (((black_king & DIAGLOW) && ((black_man & DIAGLOW) == 0)) || ((black_king & DIAGHIGH) && ((black_man & DIAGHIGH) == 0))) 
			{ white_man_locked = white_man_locked | (white_man & B5SE); }
		count_bits(white_man_locked, nr_of_white_man_locked)
	} // all locked white man

	white_man_locked_bonus = (-250) * (min_(4, nr_of_white_man_locked));
	black_man_locked_bonus = (-250) * (min_(4, nr_of_black_man_locked));
	
	// calcutate patterns (3row/4row/5row/and 6row)




	if (nr_of_black_king == 0)				// count white pattern
	{	BitArray incl_all = 0, excl_all = 0;
		for (i=0; i<52; i++)
		{	if (all_bits_set(white_man, white_row_masks[i])) 
			{	incl_all = incl_all | include_bits[i]; 
				excl_all = excl_all | exclude_bits[i];
			}
		}
		count_bits(((incl_all & R3_ROW) & (~excl_all)), white_row_3_count)
		count_bits(((incl_all & R4_ROW) & (~excl_all)), white_row_4_count)
		count_bits(((incl_all & R5_ROW) & (~excl_all)), white_row_5_count)
		count_bits(((incl_all & R6_ROW) & (~excl_all)), white_row_6_count)
	}

	if (nr_of_white_king == 0)				// calculate black pattern
	{	BitArray incl_all = 0, excl_all = 0;
		for (i=0; i<52; i++)
		{	if (all_bits_set(black_man, black_row_masks[i])) 
			{	incl_all = incl_all | include_bits[i]; 
				excl_all = excl_all | exclude_bits[i];
			}
		}
		count_bits(((incl_all & R3_ROW) & (~excl_all)), black_row_3_count)
		count_bits(((incl_all & R4_ROW) & (~excl_all)), black_row_4_count)
		count_bits(((incl_all & R5_ROW) & (~excl_all)), black_row_5_count)
		count_bits(((incl_all & R6_ROW) & (~excl_all)), black_row_6_count)
	}
	white_row_score =
		white_row_3_count * row_3_value[nr_of_white_man] +
		white_row_4_count * row_4_value[nr_of_white_man] +
		white_row_5_count * row_5_value[nr_of_white_man] +
		white_row_6_count * row_6_value[nr_of_white_man];

	black_row_score =
		black_row_3_count * row_3_value[nr_of_black_man] +
		black_row_4_count * row_4_value[nr_of_black_man] +
		black_row_5_count * row_5_value[nr_of_black_man] +
		black_row_6_count * row_6_value[nr_of_black_man];
	
	//{	int tot_row_score;
	//	tot_row_score = white_row_score + black_row_score;
	//	if (tot_row_score)
	//	{	white_row_score = imin((white_row_score*900/tot_row_score), white_row_score);
	//		black_row_score = imin((black_row_score*900/tot_row_score), black_row_score);
	//	}
	//}


	if (nr_of_white_king + nr_of_black_king == 0)
	{
		// some special cases

		if (((white_man & AVOID_4641_WHITE) == AVOID_4641_WHITE) && (empty_field & FLD46) && (black_man & FLD26)) white_avoid_4641_bonus = - 1500;
		if (((black_man & AVOID_4641_BLACK) == AVOID_4641_BLACK) && (empty_field &  FLD5) && (white_man & FLD25)) black_avoid_4641_bonus = - 1500;
		if (((white_man & AVOID_4641_WHITE) == AVOID_4641_WHITE) && (empty_field & FLD46) && (empty_field & FLD26) && (black_man & FLD21)) white_avoid_4641_bonus = - 1200;
		if (((black_man & AVOID_4641_BLACK) == AVOID_4641_BLACK) && (empty_field &  FLD5) && (empty_field & FLD25) && (white_man & FLD30)) black_avoid_4641_bonus = - 1200;
		if (all_bits_set(white_man, AVOID2_4641_WHITE) && all_bits_set(empty_field, (FLD46 | FLD47)) && (black_man & FLD26)) white_avoid_4641_bonus += -500;
		if (all_bits_set(white_man, AVOID2_4641_WHITE) && all_bits_set(empty_field, (FLD46 | FLD47 | FLD26)) && (black_man & FLD25)) white_avoid_4641_bonus += -500;
		if (all_bits_set(black_man, AVOID2_4641_BLACK) && all_bits_set(empty_field, (FLD4  | FLD5 )) && (white_man & FLD26)) black_avoid_4641_bonus += -500;
		if (all_bits_set(black_man, AVOID2_4641_BLACK) && all_bits_set(empty_field, (FLD4  | FLD5  | FLD25)) && (white_man & FLD30)) black_avoid_4641_bonus += -500;
		if ((white_man & FLD35) && (white_man & FLD23)) white_avoid_2335_bonus = -300*(imax(nr_of_white_man - 10, 0)/nr_of_white_man);
		if ((black_man & FLD16) && (black_man & FLD28)) black_avoid_2335_bonus = -300*(imax(nr_of_black_man - 10, 0)/nr_of_black_man);
		if ((black_man & FLD23) && (white_man & FLD24)) white_avoid_2324_bonus = -400*(imax(nr_of_white_man - 10, 0)/nr_of_white_man);
		if ((white_man & FLD28) && (black_man & FLD27)) black_avoid_2324_bonus = -400*(imax(nr_of_black_man - 10, 0)/nr_of_black_man);
		if ((black_man & FLD36) && (empty_field & FLD47)) white_36_47_bonus = -800;
		if ((white_man & FLD15) && (empty_field & FLD4)) black_36_47_bonus = -800;
		if ((black_man & FLD35) && (empty_field & FLD44)) white_36_47_bonus = -800;
		if ((white_man & FLD16) && (empty_field & FLD7)) black_36_47_bonus = -800;
		if ((white_man & FLD26) && (empty_field & FLD6)  && (all_bits_set(black_man, TRIANGLE_11))) black_11_16_17_bonus = -400;
		if ((black_man & FLD25) && (empty_field & FLD45) && (all_bits_set(white_man, TRIANGLE_40))) white_11_16_17_bonus = -400;

		// centrum spel
		if (all_bits_set(white_man, (FLD22 | FLD23))) 
			if (white_man & FLD28) white_tandem_bonus += (400*dikke_stand[nr_of_pieces])/100; else white_tandem_bonus += (300*dikke_stand[nr_of_pieces])/100; 
		if (all_bits_set(white_man, (FLD23 | FLD24)))
			if (white_man & FLD29) white_tandem_bonus += (400*dikke_stand[nr_of_pieces])/100; else white_tandem_bonus += (300*dikke_stand[nr_of_pieces])/100;  
		if (all_bits_set(black_man, (FLD29 | FLD28))) 
			if (black_man & FLD23) black_tandem_bonus += (400*dikke_stand[nr_of_pieces])/100; else black_tandem_bonus += (300*dikke_stand[nr_of_pieces])/100;  
		if (all_bits_set(black_man, (FLD28 | FLD27)))
			if (black_man & FLD22) black_tandem_bonus += (400*dikke_stand[nr_of_pieces])/100; else black_tandem_bonus += (300*dikke_stand[nr_of_pieces])/100; 

		// ketting stelling ?
		if (all_bits_set(white_man, WHITE_WHITE_CHAIN1) && all_bits_set(black_man, WHITE_BLACK_CHAIN1)) white_ketting_stelling++;
		if (all_bits_set(white_man, WHITE_WHITE_CHAIN2) && all_bits_set(black_man, WHITE_BLACK_CHAIN2)) white_ketting_stelling++;
		if (all_bits_set(white_man, WHITE_WHITE_CHAIN3) && all_bits_set(black_man, WHITE_BLACK_CHAIN3)) white_ketting_stelling++;
		if (all_bits_set(white_man, WHITE_WHITE_CHAIN4) && all_bits_set(black_man, WHITE_BLACK_CHAIN4)) white_ketting_stelling++;

		if (all_bits_set(black_man, BLACK_BLACK_CHAIN1) && all_bits_set(white_man, BLACK_WHITE_CHAIN1)) black_ketting_stelling++;
		if (all_bits_set(black_man, BLACK_BLACK_CHAIN2) && all_bits_set(white_man, BLACK_WHITE_CHAIN2)) black_ketting_stelling++;
		if (all_bits_set(black_man, BLACK_BLACK_CHAIN3) && all_bits_set(white_man, BLACK_WHITE_CHAIN3)) black_ketting_stelling++;
		if (all_bits_set(black_man, BLACK_BLACK_CHAIN4) && all_bits_set(white_man, BLACK_WHITE_CHAIN4)) black_ketting_stelling++;

		// halve hek stelling ?

		if (all_bits_set(white_man, WHITE_WHITE_HALVE_HEK) && all_bits_set(black_man, WHITE_BLACK_HALVE_HEK) && all_bits_set(empty_field, (FLD26 | FLD21)))
		{	char cnt; count_bits(white_man & DIAGONAL, cnt); if (cnt < 2) white_halve_hek++; else if (cnt <3) white_halve_hek_++;}
		if (all_bits_set(black_man, BLACK_BLACK_HALVE_HEK) && all_bits_set(white_man, BLACK_WHITE_HALVE_HEK) && all_bits_set(empty_field, (FLD25 | FLD30)))
		{	char cnt; count_bits(black_man & DIAGONAL, cnt); if (cnt < 2) black_halve_hek++; else if (cnt <3) black_halve_hek_++;}
	
		// klassiek ?

		if (all_bits_set(white_man, KLASSIEK_WHITE) && all_bits_set(black_man, KLASSIEK_BLACK)) is_klassiek = 1;
		else is_klassiek = 0;

		// calc tempi

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
		
		if (is_klassiek)
		{	white_klassiek_score = (black_tempi - white_tempi) * 25;
			black_klassiek_score = - white_klassiek_score;
			white_tempi = 0;	// no further contribution
			black_tempi = 0;
			if (all_bits_set(white_man, WHITE_CROWN)) white_klassiek_score += 200;
			if (all_bits_set(white_man, WHITE_OLYMPIC)) white_klassiek_score += 150;
			if (all_bits_set(black_man, BLACK_CROWN)) black_klassiek_score += 200;
			if (all_bits_set(black_man, BLACK_OLYMPIC)) black_klassiek_score += 150;
		}

		// calc tempi_score

		white_tempi_score = white_tempi * 25;
		black_tempi_score = black_tempi * 25;

		// slechte bindingen

		{	int white_slechte_binding_found = 0;
			int black_slechte_binding_found = 0;
			int white_slechte_binding_rows = 0;
			int black_slechte_binding_rows = 0;

			if (all_bits_set(white_man, (FLD27 | FLD29)) && (empty_field & FLD28) && (!(white_man & WM_01_20)))
			{	white_slechte_binding_found = 1;
				if (all_bits_set(black_man, (FLD7 | FLD12 | FLD18))) white_slechte_binding_rows++;
				if (all_bits_set(black_man, (FLD9 | FLD13 | FLD18))) white_slechte_binding_rows++;
			}
			else if (all_bits_set(white_man, (FLD26 | FLD28)) && (empty_field & FLD27) && (!(white_man & WM_01_20)))
			{	white_slechte_binding_found = 1;
				if (all_bits_set(black_man, (FLD6 | FLD11 | FLD17)))
				{	white_slechte_binding_rows++;
					if (all_bits_set(black_man, (FLD8 | FLD12 | FLD17))) white_slechte_binding_rows++;
				}
			}
			if (all_bits_set(black_man, (FLD24 | FLD22)) && (empty_field & FLD23) && (!(black_man & BM_50_31)))
			{	black_slechte_binding_found = 1;
				if (all_bits_set(white_man, (FLD44 | FLD39 | FLD33))) black_slechte_binding_rows++;
				if (all_bits_set(white_man, (FLD42 | FLD38 | FLD33))) black_slechte_binding_rows++;
			}
			else if (all_bits_set(black_man, (FLD25 | FLD23)) && (empty_field & FLD24) && (!(black_man & BM_50_31)))
			{	black_slechte_binding_found = 1;
				if (all_bits_set(white_man, (FLD45 | FLD40 | FLD34))) 
				{	black_slechte_binding_rows++;
					if (all_bits_set(white_man, (FLD43 | FLD39 | FLD34))) black_slechte_binding_rows++;
				}
			}

			if (white_slechte_binding_found)
			{	int tempi_diff = white_tempi -black_tempi;
				tempi_diff = imin(tempi_diff, 10); tempi_diff = imax(tempi_diff, -4); tempi_diff += 4; // tempi_diff now limited 0..14
				white_slechte_binding = slechte_binding_score[white_slechte_binding_rows][tempi_diff]; //printf("tempi diff = %d, white_slechte_binding_rows = %d\n", tempi_diff, white_slechte_binding_rows);
			}
			if (black_slechte_binding_found)
			{	int tempi_diff = black_tempi -white_tempi;
				tempi_diff = imin(tempi_diff, 10); tempi_diff = imax(tempi_diff, -4); tempi_diff += 4; // tempi_diff now limited 0..14
				black_slechte_binding = slechte_binding_score[black_slechte_binding_rows][tempi_diff];
			}
		}

		// vrije doorgang naar dam

		if (node->status & IS_WHITE_ON_MOVE)  // remember: alway called in min-mode
		{	nr_of_white_free_path = nr_of_white_min_free_path(white_man, black_man, empty_field); 
			nr_of_black_free_path = nr_of_black_max_free_path(white_man, black_man, empty_field); 			
		}
		else
		{	nr_of_black_free_path = nr_of_black_min_free_path(white_man, black_man, empty_field); 
			nr_of_white_free_path = nr_of_white_max_free_path(white_man, black_man, empty_field);			
		}

		// distribution of white men
		{	int t2_1_5, t2_2_5, t3_1_3, t3_2_4, t3_3_3, t4_1_2, t4_2_3, t4_3_3, t4_4_2, t5_1_2, t5_2_2, t5_3_2, t5_4_2, t5_5_2;
			int tc2, tc3, tc4, tc5;
			
			count_bits(white_man & W2_1_5, t2_1_5);
			count_bits(white_man & W2_2_5, t2_2_5);
			count_bits(white_man & W3_1_3, t3_1_3);
			count_bits(white_man & W3_2_4, t3_2_4);
			count_bits(white_man & W3_3_3, t3_3_3);
			count_bits(white_man & W4_1_2, t4_1_2);
			count_bits(white_man & W4_2_3, t4_2_3);
			count_bits(white_man & W4_3_3, t4_3_3);
			count_bits(white_man & W4_4_2, t4_4_2);
			count_bits(white_man & W5_1_2, t5_1_2);
			count_bits(white_man & W5_2_2, t5_2_2);
			count_bits(white_man & W5_3_2, t5_3_2);
			count_bits(white_man & W5_4_2, t5_4_2);
			count_bits(white_man & W5_5_2, t5_5_2);
			tc2 = c5[t2_1_5][nr_of_white_man] + c5[t2_2_5][nr_of_white_man];
			tc3 = c3[t3_1_3][nr_of_white_man] + c4[t3_2_4][nr_of_white_man] + c3[t3_3_3][nr_of_white_man];
			tc4 = c2[t4_1_2][nr_of_white_man] + c3[t4_2_3][nr_of_white_man] + c3[t4_3_3][nr_of_white_man] + c2[t4_4_2][nr_of_white_man];
			tc5 = c2[t5_1_2][nr_of_white_man] + c2[t5_2_2][nr_of_white_man] + c2[t5_3_2][nr_of_white_man] + c2[t5_4_2][nr_of_white_man] + c2[t5_5_2][nr_of_white_man];
			white_fit_score = -(tc2*10 + tc3*6 + tc4*3 + tc5*2);

			//printf("white:\n");
			//printf("tc2 = %d, (%d %d)\n", tc2, t2_1_5, t2_2_5);
			//printf("tc3 = %d, (%d %d %d)\n", tc3, t3_1_3, t3_2_4, t3_3_3);
			//printf("tc4 = %d, (%d %d %d %d)\n", tc4, t4_1_2, t4_2_3, t4_3_3, t4_4_2);
			//printf("tc5 = %d, (%d %d %d %d %d)\n", tc5, t5_1_2, t5_2_2, t5_3_2, t5_4_2, t5_5_2);
			//printf("total %d %d\n", tc2*10 + tc3*6 +tc4*3 +tc5*2, white_fit_score);
			//printf("\n");
		}


		// distribution of black men
		{	int t2_1_5, t2_2_5, t3_1_3, t3_2_4, t3_3_3, t4_1_2, t4_2_3, t4_3_3, t4_4_2, t5_1_2, t5_2_2, t5_3_2, t5_4_2, t5_5_2;
			int tc2, tc3, tc4, tc5;
			
			count_bits(black_man & W2_1_5, t2_1_5);
			count_bits(black_man & W2_2_5, t2_2_5);
			count_bits(black_man & W3_1_3, t3_1_3);
			count_bits(black_man & W3_2_4, t3_2_4);
			count_bits(black_man & W3_3_3, t3_3_3);
			count_bits(black_man & W4_1_2, t4_1_2);
			count_bits(black_man & W4_2_3, t4_2_3);
			count_bits(black_man & W4_3_3, t4_3_3);
			count_bits(black_man & W4_4_2, t4_4_2);
			count_bits(black_man & W5_1_2, t5_1_2);
			count_bits(black_man & W5_2_2, t5_2_2);
			count_bits(black_man & W5_3_2, t5_3_2);
			count_bits(black_man & W5_4_2, t5_4_2);
			count_bits(black_man & W5_5_2, t5_5_2);
			tc2 = c5[t2_1_5][nr_of_black_man] + c5[t2_2_5][nr_of_black_man];
			tc3 = c3[t3_1_3][nr_of_black_man] + c4[t3_2_4][nr_of_black_man] + c3[t3_3_3][nr_of_black_man];
			tc4 = c2[t4_1_2][nr_of_black_man] + c3[t4_2_3][nr_of_black_man] + c3[t4_3_3][nr_of_black_man] + c2[t4_4_2][nr_of_black_man];
			tc5 = c2[t5_1_2][nr_of_black_man] + c2[t5_2_2][nr_of_black_man] + c2[t5_3_2][nr_of_black_man] + c2[t5_4_2][nr_of_black_man] + c2[t5_5_2][nr_of_black_man];
			black_fit_score = -(tc2*10 + tc3*6 + tc4*3 + tc5*2);
			//printf("black:\n");
			//printf("tc2 = %d, (%d %d)\n", tc2, t2_1_5, t2_2_5);
			//printf("tc3 = %d, (%d %d %d)\n", tc3, t3_1_3, t3_2_4, t3_3_3);
			//printf("tc4 = %d, (%d %d %d %d)\n", tc4, t4_1_2, t4_2_3, t4_3_3, t4_4_2);
			//printf("tc5 = %d, (%d %d %d %d %d)\n", tc5, t5_1_2, t5_2_2, t5_3_2, t5_4_2, t5_5_2);
			//printf("total %d %d\n", tc2*10 + tc3*6 +tc4*3 +tc5*2, black_fit_score);
			//printf("\n");

		}
		
		// voorpost calculations
		{	int black_cnt = 0, white_cnt = 0;

			if ((black_man & FLD39) && (white_man & WV1_W39) && (white_man & WV1_W2_39) && (all_bits_set(empty_field, WV1_W3_39))) black_cnt++;
			if ((black_man & FLD39) && (white_man & WV2_W39) && (white_man & WV2_W2_39) && (all_bits_set(empty_field, WV2_W3_39))) black_cnt++;
			if ((black_man & FLD38) && (white_man & WV1_W38) && (white_man & WV1_W2_38) && (all_bits_set(empty_field, WV1_W3_38))) black_cnt++;
			if ((black_man & FLD38) && (white_man & WV2_W38) && (white_man & WV2_W2_38) && (all_bits_set(empty_field, WV2_W3_38))) black_cnt++;
			if ((black_man & FLD37) && (white_man & WV1_W37) && (white_man & WV1_W2_37) && (all_bits_set(empty_field, WV1_W3_37))) black_cnt++;
			if ((black_man & FLD37) && (white_man & WV2_W37) && (white_man & WV2_W2_37) && (all_bits_set(empty_field, WV2_W3_37))) black_cnt++;
			if ((black_man & FLD34) && (white_man & WV1_W34) && (white_man & WV1_W2_34) && (all_bits_set(empty_field, WV1_W3_34))) black_cnt++;
			if ((black_man & FLD34) && (white_man & WV2_W34) && (white_man & WV2_W2_34) && (all_bits_set(empty_field, WV2_W3_34))) black_cnt++;
			if ((black_man & FLD33) && (white_man & WV1_W33) && (white_man & WV1_W2_33) && (all_bits_set(empty_field, WV1_W3_33))) black_cnt++;
			if ((black_man & FLD33) && (white_man & WV2_W33) && (white_man & WV2_W2_33) && (all_bits_set(empty_field, WV2_W3_33))) black_cnt++;
			if ((black_man & FLD32) && (white_man & WV1_W32) && (white_man & WV1_W2_32) && (all_bits_set(empty_field, WV1_W3_32))) black_cnt++;
			if ((black_man & FLD32) && (white_man & WV2_W32) && (white_man & WV2_W2_32) && (all_bits_set(empty_field, WV2_W3_32))) black_cnt++;

			if ((white_man & FLD12) && (black_man & BV1_W12) && (black_man & BV1_W2_12) && (all_bits_set(empty_field, BV1_W3_12))) white_cnt++;
			if ((white_man & FLD12) && (black_man & BV2_W12) && (black_man & BV2_W2_12) && (all_bits_set(empty_field, BV2_W3_12))) white_cnt++;
			if ((white_man & FLD13) && (black_man & BV1_W13) && (black_man & BV1_W2_13) && (all_bits_set(empty_field, BV1_W3_13))) white_cnt++;
			if ((white_man & FLD13) && (black_man & BV2_W13) && (black_man & BV2_W2_13) && (all_bits_set(empty_field, BV2_W3_13))) white_cnt++;
			if ((white_man & FLD14) && (black_man & BV1_W14) && (black_man & BV1_W2_14) && (all_bits_set(empty_field, BV1_W3_14))) white_cnt++;
			if ((white_man & FLD14) && (black_man & BV2_W14) && (black_man & BV2_W2_14) && (all_bits_set(empty_field, BV2_W3_14))) white_cnt++;
			if ((white_man & FLD17) && (black_man & BV1_W17) && (black_man & BV1_W2_17) && (all_bits_set(empty_field, BV1_W3_17))) white_cnt++;
			if ((white_man & FLD17) && (black_man & BV2_W17) && (black_man & BV2_W2_17) && (all_bits_set(empty_field, BV2_W3_17))) white_cnt++;
			if ((white_man & FLD18) && (black_man & BV1_W18) && (black_man & BV1_W2_18) && (all_bits_set(empty_field, BV1_W3_18))) white_cnt++;
			if ((white_man & FLD18) && (black_man & BV2_W18) && (black_man & BV2_W2_18) && (all_bits_set(empty_field, BV2_W3_18))) white_cnt++;
			if ((white_man & FLD19) && (black_man & BV1_W19) && (black_man & BV1_W2_19) && (all_bits_set(empty_field, BV1_W3_19))) white_cnt++;
			if ((white_man & FLD19) && (black_man & BV2_W19) && (black_man & BV2_W2_19) && (all_bits_set(empty_field, BV2_W3_19))) white_cnt++;

			white_voorpost_score = (-600 * white_cnt * nr_of_white_man) / 20;
			black_voorpost_score = (-600 * black_cnt * nr_of_black_man) / 20;
		}

		// very simple 1 piece locks 2 pieces situations

		{	int black_cnt = 0, white_cnt = 0;
			if ((white_man & FLD12) && (all_bits_set(black_man, BL_W12)) && (all_bits_set(empty_field, BL_W3_12))) white_cnt++;
			if ((white_man & FLD13) && (all_bits_set(black_man, BL_W13)) && (all_bits_set(empty_field, BL_W3_13))) white_cnt++;
			if ((white_man & FLD14) && (all_bits_set(black_man, BL_W14)) && (all_bits_set(empty_field, BL_W3_14))) white_cnt++;
			if ((white_man & FLD17) && (all_bits_set(black_man, BL_W17)) && (all_bits_set(empty_field, BL_W3_17))) white_cnt++;
			if ((white_man & FLD18) && (all_bits_set(black_man, BL_W18)) && (all_bits_set(empty_field, BL_W3_18))) white_cnt++;
			if ((white_man & FLD19) && (all_bits_set(black_man, BL_W19)) && (all_bits_set(empty_field, BL_W3_19))) white_cnt++;

			if ((black_man & FLD39) && (all_bits_set(white_man, WL_W39)) && (all_bits_set(empty_field, WL_W3_39))) black_cnt++;
			if ((black_man & FLD38) && (all_bits_set(white_man, WL_W38)) && (all_bits_set(empty_field, WL_W3_38))) black_cnt++;
			if ((black_man & FLD37) && (all_bits_set(white_man, WL_W37)) && (all_bits_set(empty_field, WL_W3_37))) black_cnt++;
			if ((black_man & FLD34) && (all_bits_set(white_man, WL_W34)) && (all_bits_set(empty_field, WL_W3_34))) black_cnt++;
			if ((black_man & FLD33) && (all_bits_set(white_man, WL_W33)) && (all_bits_set(empty_field, WL_W3_33))) black_cnt++;
			if ((black_man & FLD32) && (all_bits_set(white_man, WL_W32)) && (all_bits_set(empty_field, WL_W3_32))) black_cnt++;

			white_locked_bonus = 500 * white_cnt;
			black_locked_bonus = 500 * black_cnt;
		}
		
		// vleugel opsluiting
		{	int white_cnt = 0; BitArray empty_or_white = empty_field | white_man; 
			int black_cnt = 0; BitArray empty_or_black = empty_field | black_man;
			int white_amount = 0, black_amount = 0, white_warning = 0, black_warning = 0;

			if (all_bits_set(white_man, OW1_1) && (empty_or_white & (FLD28)) && (all_bits_set(black_man, OW2_1))){int bc, wc; count_bits((black_man & OW3_1), bc); count_bits((white_man & OW4_1), wc);  if (bc-wc-3>0) {white_warning++; white_cnt = white_is_locking_pieces(node, bc - wc - 3, (OW2_1 | OW3_1), OW1_1); if (white_cnt){ white_amount += bc-wc-3; goto N1;} } }
			if (all_bits_set(white_man, OW1_2) && (empty_or_white & (FLD28)) && (all_bits_set(black_man, OW2_2))){int bc, wc; count_bits((black_man & OW3_2), bc); count_bits((white_man & OW4_2), wc);  if (bc-wc-3>0) {white_warning++; white_cnt = white_is_locking_pieces(node, bc - wc - 3, (OW2_2 | OW3_2), OW1_2); if (white_cnt){ white_amount += bc-wc-3; goto N1;} } }
			if (all_bits_set(white_man, OW1_3) && (empty_or_white & (FLD22)) && (all_bits_set(black_man, OW2_3))){int bc, wc; count_bits((black_man & OW3_3), bc); wc=0;  if (bc-wc-2>0) {white_warning++; white_cnt = white_is_locking_pieces(node, bc - wc - 2, (OW2_3 | OW3_3), OW1_3); if (white_cnt){ white_amount += bc-wc-2; goto N1;} } }
			if (all_bits_set(white_man, OW1_4) && (empty_or_white & (FLD22)) && (all_bits_set(black_man, OW2_4))){int bc, wc; count_bits((black_man & OW3_4), bc); wc=0;  if (bc-wc-2>0) {white_warning++; white_cnt = white_is_locking_pieces(node, bc - wc - 2, (OW2_4 | OW3_4), OW1_4); if (white_cnt){ white_amount += bc-wc-2; goto N1;} } }
			if (all_bits_set(white_man, OW1_5) && (empty_or_white & (FLD17)) && (all_bits_set(black_man, OW2_5))){int bc, wc; count_bits((black_man & OW3_5), bc); wc=0;  if (bc-wc-0>0) {white_warning++; white_cnt = white_is_locking_pieces(node, bc - wc - 0, (OW2_5 | OW3_5), OW1_5); if (white_cnt){ white_amount += bc-wc-0; goto N1;} } }
N1:			if (all_bits_set(white_man, OW1_6) && (empty_or_white & (FLD38)) && (all_bits_set(black_man, OW2_6))){int bc, wc; count_bits((black_man & OW3_6), bc); count_bits((white_man & OW4_6), wc);  if (bc-wc-3>0) {white_warning++; white_cnt = white_is_locking_pieces(node, bc - wc - 3, (OW2_6 | OW3_6), OW1_6); if (white_cnt){ white_amount += bc-wc-3; goto N2;} } }
			if (all_bits_set(white_man, OW1_7) && (empty_or_white & (FLD38)) && (all_bits_set(black_man, OW2_7))){int bc, wc; count_bits((black_man & OW3_7), bc); count_bits((white_man & OW4_7), wc);  if (bc-wc-3>0) {white_warning++; white_cnt = white_is_locking_pieces(node, bc - wc - 3, (OW2_7 | OW3_7), OW1_7); if (white_cnt){ white_amount += bc-wc-3; goto N2;} } }
			if (all_bits_set(white_man, OW1_8) && (empty_or_white & (FLD32)) && (all_bits_set(black_man, OW2_8))){int bc, wc; count_bits((black_man & OW3_8), bc); wc=0;  if (bc-wc-2>0) {white_warning++; white_cnt = white_is_locking_pieces(node, bc - wc - 2, (OW2_8 | OW3_8), OW1_8); if (white_cnt){ white_amount += bc-wc-2; goto N2;} } }
			if (all_bits_set(white_man, OW1_9) && (empty_or_white & (FLD32)) && (all_bits_set(black_man, OW2_9))){int bc, wc; count_bits((black_man & OW3_9), bc); wc=0;  if (bc-wc-2>0) {white_warning++; white_cnt = white_is_locking_pieces(node, bc - wc - 2, (OW2_9 | OW3_9), OW1_9); if (white_cnt){ white_amount += bc-wc-2; goto N2;} } }
			if (all_bits_set(white_man, OW1_10) && (empty_or_white & (FLD27)) && (all_bits_set(black_man, OW2_10))){int bc, wc; count_bits((black_man & OW3_10), bc); wc=0;  if (bc-wc-0>0) {white_warning++; white_cnt = white_is_locking_pieces(node, bc - wc - 0, (OW2_10 | OW3_10), OW1_10); if (white_cnt){ white_amount += bc-wc-0; goto N2;} } }
			if (all_bits_set(white_man, OW1_11) && (empty_or_white & (FLD42)) && (all_bits_set(black_man, OW2_11))){int bc, wc; count_bits((black_man & OW3_11), bc); wc=0;  if (bc-wc-2>0) {white_warning++; white_cnt = white_is_locking_pieces(node, bc - wc - 2, (OW2_11 | OW3_11), OW1_11); if (white_cnt){ white_amount += bc-wc-2; goto N2;} } }
			if (all_bits_set(white_man, OW1_12) && (empty_or_white & (FLD42)) && (all_bits_set(black_man, OW2_12))){int bc, wc; count_bits((black_man & OW3_12), bc); wc=0;  if (bc-wc-2>0) {white_warning++; white_cnt = white_is_locking_pieces(node, bc - wc - 2, (OW2_12 | OW3_12), OW1_12); if (white_cnt){ white_amount += bc-wc-2; goto N2;} } }
			if (all_bits_set(white_man, OW1_13) && (empty_or_white & (FLD37)) && (all_bits_set(black_man, OW2_13))){int bc, wc; count_bits((black_man & OW3_13), bc); wc=0;  if (bc-wc-0>0) {white_warning++; white_cnt = white_is_locking_pieces(node, bc - wc - 0, (OW2_13 | OW3_13), OW1_13); if (white_cnt){ white_amount += bc-wc-0; goto N2;} } }
N2:			if (all_bits_set(white_man, OW1_14) && (empty_or_white & (FLD23)) && (all_bits_set(black_man, OW2_14))){int bc, wc; count_bits((black_man & OW3_14), bc); wc=0;  if (bc-wc-3>0) {white_warning++; white_cnt = white_is_locking_pieces(node, bc - wc - 3, (OW2_14 | OW3_14), OW1_14); if (white_cnt){ white_amount += bc-wc-3; goto N3;} } }
			if (all_bits_set(white_man, OW1_15) && (empty_or_white & (FLD23)) && (all_bits_set(black_man, OW2_15))){int bc, wc; count_bits((black_man & OW3_15), bc); count_bits((white_man & OW4_15), wc);  if (bc-wc-3>0) {white_warning++; white_cnt = white_is_locking_pieces(node, bc - wc - 3, (OW2_15 | OW3_15), OW1_15); if (white_cnt){ white_amount += bc-wc-3; goto N3;} } }
			if (all_bits_set(white_man, OW1_16) && (empty_or_white & (FLD19)) && (all_bits_set(black_man, OW2_16))){int bc, wc; count_bits((black_man & OW3_16), bc); wc=0;  if (bc-wc-2>0) {white_warning++; white_cnt = white_is_locking_pieces(node, bc - wc - 2, (OW2_16 | OW3_16), OW1_16); if (white_cnt){ white_amount += bc-wc-2; goto N3;} } }
			if (all_bits_set(white_man, OW1_17) && (empty_or_white & (FLD19)) && (all_bits_set(black_man, OW2_17))){int bc, wc; count_bits((black_man & OW3_17), bc); wc=0;  if (bc-wc-2>0) {white_warning++; white_cnt = white_is_locking_pieces(node, bc - wc - 2, (OW2_17 | OW3_17), OW1_17); if (white_cnt){ white_amount += bc-wc-2; goto N3;} } }
			if (all_bits_set(white_man, OW1_18) && (empty_or_white & (FLD14)) && (all_bits_set(black_man, OW2_18))){int bc, wc; count_bits((black_man & OW3_18), bc); wc=0;  if (bc-wc-0>0) {white_warning++; white_cnt = white_is_locking_pieces(node, bc - wc - 0, (OW2_18 | OW3_18), OW1_18); if (white_cnt){ white_amount += bc-wc-0; goto N3;} } }
N3:			if (all_bits_set(white_man, OW1_19) && (empty_or_white & (FLD33)) && (all_bits_set(black_man, OW2_19))){int bc, wc; count_bits((black_man & OW3_19), bc); wc=0;  if (bc-wc-3>0) {white_warning++; white_cnt = white_is_locking_pieces(node, bc - wc - 3, (OW2_19 | OW3_19), OW1_19); if (white_cnt){ white_amount += bc-wc-3; goto N4;} } }
			if (all_bits_set(white_man, OW1_20) && (empty_or_white & (FLD33)) && (all_bits_set(black_man, OW2_20))){int bc, wc; count_bits((black_man & OW3_20), bc); count_bits((white_man & OW4_20), wc);  if (bc-wc-3>0) {white_warning++; white_cnt = white_is_locking_pieces(node, bc - wc - 3, (OW2_20 | OW3_20), OW1_20); if (white_cnt){ white_amount += bc-wc-3; goto N4;} } }
			if (all_bits_set(white_man, OW1_21) && (empty_or_white & (FLD29)) && (all_bits_set(black_man, OW2_21))){int bc, wc; count_bits((black_man & OW3_21), bc); wc=0;  if (bc-wc-2>0) {white_warning++; white_cnt = white_is_locking_pieces(node, bc - wc - 2, (OW2_21 | OW3_21), OW1_21); if (white_cnt){ white_amount += bc-wc-2; goto N4;} } }
			if (all_bits_set(white_man, OW1_22) && (empty_or_white & (FLD29)) && (all_bits_set(black_man, OW2_22))){int bc, wc; count_bits((black_man & OW3_22), bc); wc=0;  if (bc-wc-2>0) {white_warning++; white_cnt = white_is_locking_pieces(node, bc - wc - 2, (OW2_22 | OW3_22), OW1_22); if (white_cnt){ white_amount += bc-wc-2; goto N4;} } }
			if (all_bits_set(white_man, OW1_23) && (empty_or_white & (FLD24)) && (all_bits_set(black_man, OW2_23))){int bc, wc; count_bits((black_man & OW3_23), bc); wc=0;  if (bc-wc-0>0) {white_warning++; white_cnt = white_is_locking_pieces(node, bc - wc - 0, (OW2_23 | OW3_23), OW1_23); if (white_cnt){ white_amount += bc-wc-0; goto N4;} } }
N4:			if (all_bits_set(white_man, OW1_24) && (empty_or_white & (FLD43)) && (all_bits_set(black_man, OW2_24))){int bc, wc; count_bits((black_man & OW3_24), bc); count_bits((white_man & OW4_24), wc);  if (bc-wc-2>0) {white_warning++; white_cnt = white_is_locking_pieces(node, bc - wc - 2, (OW2_24 | OW3_24), OW1_24); if (white_cnt){ white_amount += bc-wc-2; goto N5;} } }
			if (all_bits_set(white_man, OW1_25) && (empty_or_white & (FLD43)) && (all_bits_set(black_man, OW2_25))){int bc, wc; count_bits((black_man & OW3_25), bc); count_bits((white_man & OW4_25), wc);  if (bc-wc-3>0) {white_warning++; white_cnt = white_is_locking_pieces(node, bc - wc - 3, (OW2_25 | OW3_25), OW1_25); if (white_cnt){ white_amount += bc-wc-3; goto N5;} } }
			if (all_bits_set(white_man, OW1_26) && (empty_or_white & (FLD39)) && (all_bits_set(black_man, OW2_26))){int bc, wc; count_bits((black_man & OW3_26), bc); wc=0;  if (bc-wc-2>0) {white_warning++; white_cnt = white_is_locking_pieces(node, bc - wc - 2, (OW2_26 | OW3_26), OW1_26); if (white_cnt){ white_amount += bc-wc-2; goto N5;} } }
			if (all_bits_set(white_man, OW1_27) && (empty_or_white & (FLD39)) && (all_bits_set(black_man, OW2_27))){int bc, wc; count_bits((black_man & OW3_27), bc); wc=0;  if (bc-wc-2>0) {white_warning++; white_cnt = white_is_locking_pieces(node, bc - wc - 2, (OW2_27 | OW3_27), OW1_27); if (white_cnt){ white_amount += bc-wc-2; goto N5;} } }
			if (all_bits_set(white_man, OW1_28) && (empty_or_white & (FLD34)) && (all_bits_set(black_man, OW2_28))){int bc, wc; count_bits((black_man & OW3_28), bc); wc=0;  if (bc-wc-0>0) {white_warning++; white_cnt = white_is_locking_pieces(node, bc - wc - 0, (OW2_28 | OW3_28), OW1_28); if (white_cnt){ white_amount += bc-wc-0; goto N5;} } }
N5:			if (all_bits_set(white_man, OW1_29) && (empty_or_white & (FLD44)) && (all_bits_set(black_man, OW2_29))){int bc, wc; count_bits((black_man & OW3_29), bc); wc=0;  if (bc-wc-0>0) {white_warning++; white_cnt = white_is_locking_pieces(node, bc - wc - 0, (OW2_29 | OW3_29), OW1_29); if (white_cnt){ white_amount += bc-wc-0; goto N6;} } }
N6:			//printf("white_cnt = %d\n", white_cnt);
			if (all_bits_set(black_man, OB1_1) && (empty_or_black & (FLD23)) && (all_bits_set(white_man, OB2_1))){int bc, wc; count_bits((white_man & OB3_1), wc); count_bits((black_man & OB4_1), bc); if ( wc-bc-3) {black_warning++; black_cnt = black_is_locking_pieces(node, wc -bc - 3, (OB2_1 | OB3_1), OB1_1);if (black_cnt){ black_amount += wc-bc-3; goto N11;} } }
			if (all_bits_set(black_man, OB1_2) && (empty_or_black & (FLD23)) && (all_bits_set(white_man, OB2_2))){int bc, wc; count_bits((white_man & OB3_2), wc); count_bits((black_man & OB4_2), bc); if ( wc-bc-3) {black_warning++; black_cnt = black_is_locking_pieces(node, wc -bc - 3, (OB2_2 | OB3_2), OB1_2);if (black_cnt){ black_amount += wc-bc-3; goto N11;} } }
			if (all_bits_set(black_man, OB1_3) && (empty_or_black & (FLD29)) && (all_bits_set(white_man, OB2_3))){int bc, wc; count_bits((white_man & OB3_3), wc); bc=0; if ( wc-bc-2) {black_warning++; black_cnt = black_is_locking_pieces(node, wc -bc - 2, (OB2_3 | OB3_3), OB1_3);if (black_cnt){ black_amount += wc-bc-2; goto N11;} } }
			if (all_bits_set(black_man, OB1_4) && (empty_or_black & (FLD29)) && (all_bits_set(white_man, OB2_4))){int bc, wc; count_bits((white_man & OB3_4), wc); bc=0; if ( wc-bc-2) {black_warning++; black_cnt = black_is_locking_pieces(node, wc -bc - 2, (OB2_4 | OB3_4), OB1_4);if (black_cnt){ black_amount += wc-bc-2; goto N11;} } }
			if (all_bits_set(black_man, OB1_5) && (empty_or_black & (FLD34)) && (all_bits_set(white_man, OB2_5))){int bc, wc; count_bits((white_man & OB3_5), wc); bc=0; if ( wc-bc-0) {black_warning++; black_cnt = black_is_locking_pieces(node, wc -bc - 0, (OB2_5 | OB3_5), OB1_5);if (black_cnt){ black_amount += wc-bc-0; goto N11;} } }
N11:		if (all_bits_set(black_man, OB1_6) && (empty_or_black & (FLD13)) && (all_bits_set(white_man, OB2_6))){int bc, wc; count_bits((white_man & OB3_6), wc); count_bits((black_man & OB4_6), bc); if ( wc-bc-3) {black_warning++; black_cnt = black_is_locking_pieces(node, wc -bc - 3, (OB2_6 | OB3_6), OB1_6);if (black_cnt){ black_amount += wc-bc-3; goto N12;} } }
			if (all_bits_set(black_man, OB1_7) && (empty_or_black & (FLD13)) && (all_bits_set(white_man, OB2_7))){int bc, wc; count_bits((white_man & OB3_7), wc); count_bits((black_man & OB4_7), bc); if ( wc-bc-3) {black_warning++; black_cnt = black_is_locking_pieces(node, wc -bc - 3, (OB2_7 | OB3_7), OB1_7);if (black_cnt){ black_amount += wc-bc-3; goto N12;} } }
			if (all_bits_set(black_man, OB1_8) && (empty_or_black & (FLD19)) && (all_bits_set(white_man, OB2_8))){int bc, wc; count_bits((white_man & OB3_8), wc); bc=0; if ( wc-bc-2) {black_warning++; black_cnt = black_is_locking_pieces(node, wc -bc - 2, (OB2_8 | OB3_8), OB1_8);if (black_cnt){ black_amount += wc-bc-2; goto N12;} } }
			if (all_bits_set(black_man, OB1_9) && (empty_or_black & (FLD19)) && (all_bits_set(white_man, OB2_9))){int bc, wc; count_bits((white_man & OB3_9), wc); bc=0; if ( wc-bc-2) {black_warning++; black_cnt = black_is_locking_pieces(node, wc -bc - 2, (OB2_9 | OB3_9), OB1_9);if (black_cnt){ black_amount += wc-bc-2; goto N12;} } }
			if (all_bits_set(black_man, OB1_10) && (empty_or_black & (FLD24)) && (all_bits_set(white_man, OB2_10))){int bc, wc; count_bits((white_man & OB3_10), wc); bc=0; if ( wc-bc-0) {black_warning++; black_cnt = black_is_locking_pieces(node, wc -bc - 0, (OB2_10 | OB3_10), OB1_10);if (black_cnt){ black_amount += wc-bc-0; goto N12;} } }
			if (all_bits_set(black_man, OB1_11) && (empty_or_black & (FLD9)) && (all_bits_set(white_man, OB2_11))){int bc, wc; count_bits((white_man & OB3_11), wc); bc=0; if ( wc-bc-2) {black_warning++; black_cnt = black_is_locking_pieces(node, wc -bc - 2, (OB2_11 | OB3_11), OB1_11);if (black_cnt){ black_amount += wc-bc-2; goto N12;} } }
			if (all_bits_set(black_man, OB1_12) && (empty_or_black & (FLD9)) && (all_bits_set(white_man, OB2_12))){int bc, wc; count_bits((white_man & OB3_12), wc); bc=0; if ( wc-bc-2) {black_warning++; black_cnt = black_is_locking_pieces(node, wc -bc - 2, (OB2_12 | OB3_12), OB1_12);if (black_cnt){ black_amount += wc-bc-2; goto N12;} } }
			if (all_bits_set(black_man, OB1_13) && (empty_or_black & (FLD14)) && (all_bits_set(white_man, OB2_13))){int bc, wc; count_bits((white_man & OB3_13), wc); bc=0; if ( wc-bc-0) {black_warning++; black_cnt = black_is_locking_pieces(node, wc -bc - 0, (OB2_13 | OB3_13), OB1_13);if (black_cnt){ black_amount += wc-bc-0; goto N12;} } }
N12:		if (all_bits_set(black_man, OB1_14) && (empty_or_black & (FLD28)) && (all_bits_set(white_man, OB2_14))){int bc, wc; count_bits((white_man & OB3_14), wc); bc=0; if ( wc-bc-3) {black_warning++; black_cnt = black_is_locking_pieces(node, wc -bc - 3, (OB2_14 | OB3_14), OB1_14);if (black_cnt){ black_amount += wc-bc-3; goto N13;} } }
			if (all_bits_set(black_man, OB1_15) && (empty_or_black & (FLD28)) && (all_bits_set(white_man, OB2_15))){int bc, wc; count_bits((white_man & OB3_15), wc); count_bits((black_man & OB4_15), bc); if ( wc-bc-3) {black_warning++; black_cnt = black_is_locking_pieces(node, wc -bc - 3, (OB2_15 | OB3_15), OB1_15);if (black_cnt){ black_amount += wc-bc-3; goto N13;} } }
			if (all_bits_set(black_man, OB1_16) && (empty_or_black & (FLD32)) && (all_bits_set(white_man, OB2_16))){int bc, wc; count_bits((white_man & OB3_16), wc); bc=0; if ( wc-bc-2) {black_warning++; black_cnt = black_is_locking_pieces(node, wc -bc - 2, (OB2_16 | OB3_16), OB1_16);if (black_cnt){ black_amount += wc-bc-2; goto N13;} } }
			if (all_bits_set(black_man, OB1_17) && (empty_or_black & (FLD32)) && (all_bits_set(white_man, OB2_17))){int bc, wc; count_bits((white_man & OB3_17), wc); bc=0; if ( wc-bc-2) {black_warning++; black_cnt = black_is_locking_pieces(node, wc -bc - 2, (OB2_17 | OB3_17), OB1_17);if (black_cnt){ black_amount += wc-bc-2; goto N13;} } }
			if (all_bits_set(black_man, OB1_18) && (empty_or_black & (FLD37)) && (all_bits_set(white_man, OB2_18))){int bc, wc; count_bits((white_man & OB3_18), wc); bc=0; if ( wc-bc-0) {black_warning++; black_cnt = black_is_locking_pieces(node, wc -bc - 0, (OB2_18 | OB3_18), OB1_18);if (black_cnt){ black_amount += wc-bc-0; goto N13;} } }
N13:		if (all_bits_set(black_man, OB1_19) && (empty_or_black & (FLD18)) && (all_bits_set(white_man, OB2_19))){int bc, wc; count_bits((white_man & OB3_19), wc); bc=0; if ( wc-bc-3) {black_warning++; black_cnt = black_is_locking_pieces(node, wc -bc - 3, (OB2_19 | OB3_19), OB1_19);if (black_cnt){ black_amount += wc-bc-3; goto N14;} } }
			if (all_bits_set(black_man, OB1_20) && (empty_or_black & (FLD18)) && (all_bits_set(white_man, OB2_20))){int bc, wc; count_bits((white_man & OB3_20), wc); count_bits((black_man & OB4_20), bc); if ( wc-bc-3) {black_warning++; black_cnt = black_is_locking_pieces(node, wc -bc - 3, (OB2_20 | OB3_20), OB1_20);if (black_cnt){ black_amount += wc-bc-3; goto N14;} } }
			if (all_bits_set(black_man, OB1_21) && (empty_or_black & (FLD22)) && (all_bits_set(white_man, OB2_21))){int bc, wc; count_bits((white_man & OB3_21), wc); bc=0; if ( wc-bc-2) {black_warning++; black_cnt = black_is_locking_pieces(node, wc -bc - 2, (OB2_21 | OB3_21), OB1_21);if (black_cnt){ black_amount += wc-bc-2; goto N14;} } }
			if (all_bits_set(black_man, OB1_22) && (empty_or_black & (FLD22)) && (all_bits_set(white_man, OB2_22))){int bc, wc; count_bits((white_man & OB3_22), wc); bc=0; if ( wc-bc-2) {black_warning++; black_cnt = black_is_locking_pieces(node, wc -bc - 2, (OB2_22 | OB3_22), OB1_22);if (black_cnt){ black_amount += wc-bc-2; goto N14;} } }
			if (all_bits_set(black_man, OB1_23) && (empty_or_black & (FLD27)) && (all_bits_set(white_man, OB2_23))){int bc, wc; count_bits((white_man & OB3_23), wc); bc=0; if ( wc-bc-0) {black_warning++; black_cnt = black_is_locking_pieces(node, wc -bc - 0, (OB2_23 | OB3_23), OB1_23);if (black_cnt){ black_amount += wc-bc-0; goto N14;} } }
N14:		if (all_bits_set(black_man, OB1_24) && (empty_or_black & (FLD8)) && (all_bits_set(white_man, OB2_24))){int bc, wc; count_bits((white_man & OB3_24), wc); count_bits((black_man & OB4_24), bc); if ( wc-bc-2) {black_warning++; black_cnt = black_is_locking_pieces(node, wc -bc - 2, (OB2_24 | OB3_24), OB1_24);if (black_cnt){ black_amount += wc-bc-2; goto N15;} } }
			if (all_bits_set(black_man, OB1_25) && (empty_or_black & (FLD8)) && (all_bits_set(white_man, OB2_25))){int bc, wc; count_bits((white_man & OB3_25), wc); count_bits((black_man & OB4_25), bc); if ( wc-bc-3) {black_warning++; black_cnt = black_is_locking_pieces(node, wc -bc - 3, (OB2_25 | OB3_25), OB1_25);if (black_cnt){ black_amount += wc-bc-3; goto N15;} } }
			if (all_bits_set(black_man, OB1_26) && (empty_or_black & (FLD12)) && (all_bits_set(white_man, OB2_26))){int bc, wc; count_bits((white_man & OB3_26), wc); bc=0; if ( wc-bc-2) {black_warning++; black_cnt = black_is_locking_pieces(node, wc -bc - 2, (OB2_26 | OB3_26), OB1_26);if (black_cnt){ black_amount += wc-bc-2; goto N15;} } }
			if (all_bits_set(black_man, OB1_27) && (empty_or_black & (FLD12)) && (all_bits_set(white_man, OB2_27))){int bc, wc; count_bits((white_man & OB3_27), wc); bc=0; if ( wc-bc-2) {black_warning++; black_cnt = black_is_locking_pieces(node, wc -bc - 2, (OB2_27 | OB3_27), OB1_27);if (black_cnt){ black_amount += wc-bc-2; goto N15;} } }
			if (all_bits_set(black_man, OB1_28) && (empty_or_black & (FLD17)) && (all_bits_set(white_man, OB2_28))){int bc, wc; count_bits((white_man & OB3_28), wc); bc=0; if ( wc-bc-0) {black_warning++; black_cnt = black_is_locking_pieces(node, wc -bc - 0, (OB2_28 | OB3_28), OB1_28);if (black_cnt){ black_amount += wc-bc-0; goto N15;} } }
N15:		if (all_bits_set(black_man, OB1_29) && (empty_or_black & (FLD7)) && (all_bits_set(white_man, OB2_29))){int bc, wc; count_bits((white_man & OB3_29), wc); bc=0; if ( wc-bc-0) {black_warning++; black_cnt = black_is_locking_pieces(node, wc -bc - 0, (OB2_29 | OB3_29), OB1_29);if (black_cnt){ black_amount += wc-bc-0; goto N16;} } }
N16:		//printf("black_cnt = %d\n", black_cnt);
			
			white_vleugel_opsluiting = white_amount * 700 + white_warning*100;
			black_vleugel_opsluiting = black_amount * 700 + black_warning*100;

		}

	// freedom:  the number of direct reachable fields with no immediate capture

		white_freedom = freedom(1, empty_field, white_man, black_man);
		black_freedom = freedom(0, empty_field, black_man, white_man);
		int white_temp = imin(white_freedom, 4);
		int black_temp = imin(black_freedom, 4);
		int white_correction = freedom_correction[black_temp][white_temp];
		white_freedom = white_freedom*20 + white_correction; // correct if there are only a few free moves
		black_freedom = black_freedom*20 - white_correction; // the matrix is symetric


	} // total nr of kings == 0

	white_score = 
		nr_of_white_man * 1000 + 
		nr_of_white_king * white_king_value + 
		white_man_position_score + 
		white_king_position_score + 
		white_more_4_pieces_bonus +
		white_row_score +
		white_man_locked_bonus +
		white_avoid_4641_bonus +
		white_avoid_2324_bonus +
		white_avoid_2335_bonus +
		white_klassiek_score +
		white_tempi_score +
		white_ketting_stelling * 800 +
		white_halve_hek * 400 +
		white_slechte_binding +
		nr_of_white_free_path * 800 +
		white_fit_score +
		white_36_47_bonus +
		white_11_16_17_bonus +
		white_voorpost_score +
		white_locked_bonus +
		white_vleugel_opsluiting +
		white_tandem_bonus +
		white_freedom;
		;

	black_score = 
		nr_of_black_man * 1000 + 
		nr_of_black_king * black_king_value + 
		black_man_position_score + 
		black_king_position_score + 
		black_more_4_pieces_bonus +
		black_row_score +
		black_man_locked_bonus +
		black_avoid_4641_bonus +
		black_avoid_2324_bonus +
		black_avoid_2335_bonus +
		black_klassiek_score +
		black_tempi_score +
		black_ketting_stelling * 800 +
		black_halve_hek * 400 +
		black_slechte_binding +
		nr_of_black_free_path * 800 +
		black_fit_score +
		black_36_47_bonus +
		black_11_16_17_bonus +
		black_voorpost_score +
		black_locked_bonus +
		black_vleugel_opsluiting +
		black_tandem_bonus +
		black_freedom;
		;

	if (print_flag)
	{	printf("     \tman\tking\tlocked\tman pos\tkng pos\t4_bonus\trow_scr\n");
		printf("White\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n"  , nr_of_white_man*1000, nr_of_white_king*white_king_value, nr_of_white_man_locked, white_man_position_score, white_king_position_score, white_more_4_pieces_bonus, white_row_score);
		printf("Black\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n\n", nr_of_black_man*1000, nr_of_black_king*black_king_value, nr_of_black_man_locked, black_man_position_score, black_king_position_score, black_more_4_pieces_bonus, black_row_score);

		printf("     \trow_3\trow_4\trow_5\trow_6\tlckbns\t4641\t2324\t2335\n");
		printf("White\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n"  , white_row_3_count, white_row_4_count, white_row_5_count, white_row_6_count, white_man_locked_bonus, white_avoid_4641_bonus, white_avoid_2324_bonus, white_avoid_2335_bonus);
		printf("Black\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n\n", black_row_3_count, black_row_4_count, black_row_5_count, black_row_6_count, black_man_locked_bonus, black_avoid_4641_bonus, black_avoid_2324_bonus, black_avoid_2335_bonus);

		printf("     \tketting\th hek\th hek-\ttempi\tklssiek\tbindng\tvr dg\tfit\n");
		printf("White\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n"  , white_ketting_stelling, white_halve_hek, white_halve_hek_, white_tempi_score, white_klassiek_score, white_slechte_binding, nr_of_white_free_path, white_fit_score);
		printf("Black\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n"  , black_ketting_stelling, black_halve_hek, black_halve_hek_, black_tempi_score, black_klassiek_score, black_slechte_binding, nr_of_black_free_path, black_fit_score);

		printf("     \t36_47\ttri_11\tvoorp\tlock\tvleug\tcentrum\tvrijh\tscore\n");
		printf("White\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n"  , white_36_47_bonus, white_11_16_17_bonus, white_voorpost_score, white_locked_bonus, white_vleugel_opsluiting, white_tandem_bonus, white_freedom, white_score);
		printf("Black\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n"  , black_36_47_bonus, black_11_16_17_bonus, black_voorpost_score, black_locked_bonus, black_vleugel_opsluiting, black_tandem_bonus, black_freedom, black_score);

		printf("\n\n");
	}
	if (log_flag)
		// and also on the log file
	{	_fcloseall(); open_log_file();
		fprintf_s(log_file,"     \tman\tking\tlocked\tman pos\tkng pos\t4_bonus\trow_scr\n");
		fprintf_s(log_file,"White\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n"  , nr_of_white_man*1000, nr_of_white_king*white_king_value, nr_of_white_man_locked, white_man_position_score, white_king_position_score, white_more_4_pieces_bonus, white_row_score);
		fprintf_s(log_file,"Black\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n\n", nr_of_black_man*1000, nr_of_black_king*black_king_value, nr_of_black_man_locked, black_man_position_score, black_king_position_score, black_more_4_pieces_bonus, black_row_score);

		fprintf_s(log_file,"     \trow_3\trow_4\trow_5\trow_6\tlckbns\t4641\t2324\t2335\n");
		fprintf_s(log_file,"White\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n"  , white_row_3_count, white_row_4_count, white_row_5_count, white_row_6_count, white_man_locked_bonus, white_avoid_4641_bonus, white_avoid_2324_bonus, white_avoid_2335_bonus);
		fprintf_s(log_file,"Black\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n\n", black_row_3_count, black_row_4_count, black_row_5_count, black_row_6_count, black_man_locked_bonus, black_avoid_4641_bonus, black_avoid_2324_bonus, black_avoid_2335_bonus);

		fprintf_s(log_file,"     \tketting\th hek\th hek-\ttempi\tklssiek\tbindng\tvr dg\tfit\n");
		fprintf_s(log_file,"White\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n"  , white_ketting_stelling, white_halve_hek, white_halve_hek_, white_tempi_score, white_klassiek_score, white_slechte_binding, nr_of_white_free_path, white_fit_score);
		fprintf_s(log_file,"Black\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n"  , black_ketting_stelling, black_halve_hek, black_halve_hek_, black_tempi_score, black_klassiek_score, black_slechte_binding, nr_of_black_free_path, black_fit_score);

		fprintf_s(log_file,"     \t36_47\ttri_11\tvoorp\tlock\tvleug\tcentrum\tvrijh\tscore\n");
		fprintf_s(log_file,"White\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n"  , white_36_47_bonus, white_11_16_17_bonus, white_voorpost_score, white_locked_bonus, white_vleugel_opsluiting, white_tandem_bonus, white_freedom, white_score);
		fprintf_s(log_file,"Black\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n"  , black_36_47_bonus, black_11_16_17_bonus, black_voorpost_score, black_locked_bonus, black_vleugel_opsluiting, black_tandem_bonus, black_freedom, black_score);
		fprintf_s(log_file, "\n\n");
		_fcloseall();
	}

	if (node->status & IS_WHITE_ON_MOVE)		// remember min-node, this is the opponents color
		score =	(short int) ( black_score - white_score) ;
	else
		score =	(short int) ( white_score - black_score) ;

	if (score == 0) score = 1;  // score == 0 means ignore, no stable position

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
}

int severe_shortage_min_mode(NodePnt node)	// quick check to see whether there is material shortage
											// this function is only called from a min-node
{	struct piece_count_record pc;	
	
	//if (node->status & IS_MAX_NODE) {printf("Early quit in max node ???\n") ; return 0; }
	
	if (node->occ_empty & node->man_king & node->white_black & NEARLY_WHITE_KING) return 0; // nearly promotion don't cut off
	if (node->occ_empty & node->man_king & ~(node->white_black) & NEARLY_BLACK_KING) return 0; // nearly promotion don't cut off
	
	calc_piece_count(node, &pc);

	// remember it's the opponents turn:
	if ((pc.nr_of_white_king > 0) || (pc.nr_of_black_king > 0)) return 0;			// too complicated to cut off
	if ((org_piece_count.nr_of_white_king > 0) || (org_piece_count.nr_of_black_king > 0)) return 0; // also too complicated
	
	if ((node->status & IS_WHITE_ON_MOVE)	)
	{	if (((org_piece_count.nr_of_black_man  - org_piece_count.nr_of_white_man) <= (pc.nr_of_black_man  - pc.nr_of_white_man)) ) return 0; 
		else {/*printf("Wit aanzet\n"); print_board(node);*/ return ((pc.nr_of_black_man  - pc.nr_of_white_man)*1000) ;}
	}
	else
	{	if ((org_piece_count.nr_of_white_man - org_piece_count.nr_of_black_man) <= (pc.nr_of_white_man - pc.nr_of_black_man)) return 0; 
		else {/*printf("Zwart aanzet\n"); print_board(node);*/ return ((pc.nr_of_white_man - pc.nr_of_black_man)*1000) ;}		
	}

}

int severe_shortage_max_mode(NodePnt node)	// quick check to see whether there is material shortage
											// this function is only called from a max-node
{	struct piece_count_record pc;	
	
	//if (!(node->status & IS_MAX_NODE)) {printf("Early quit in min node ???\n") ; return 0; }
	
	if (node->occ_empty & node->man_king & node->white_black & NEARLY_WHITE_KING) return 0; // nearly promotion don't cut off
	if (node->occ_empty & node->man_king & ~(node->white_black) & NEARLY_BLACK_KING) return 0; // nearly promotion don't cut off
	
	calc_piece_count(node, &pc);

	// remember it's the opponents turn:
	if ((pc.nr_of_white_king > 0) || (pc.nr_of_black_king > 0)) return 0;			// too complicated to cut off
	if ((org_piece_count.nr_of_white_king > 0) || (org_piece_count.nr_of_black_king > 0)) return 0; // also too complicated
	
	if (!(node->status & IS_WHITE_ON_MOVE)	)
	{	if (((org_piece_count.nr_of_black_man  - org_piece_count.nr_of_white_man) <= (pc.nr_of_black_man  - pc.nr_of_white_man)) ) return 0; 
		else {/*printf("Zwart aanzet\n"); print_board(node);*/ return ((pc.nr_of_black_man  - pc.nr_of_white_man)*1000) ;}
	}
	else
	{	if ((org_piece_count.nr_of_white_man - org_piece_count.nr_of_black_man) <= (pc.nr_of_white_man - pc.nr_of_black_man)) return 0; 
		else {/*printf("Wit aanzet\n"); print_board(node);*/ return ((pc.nr_of_white_man - pc.nr_of_black_man)*1000) ;}		
	}
}

short int evaluate_with_capture(NodePnt node, int PrintFlag)  // called in min-mode
// very simple evaluator in case of a capture
// just count the pieces

{	struct piece_count_record pc;	
	calc_piece_count(node, &pc);
	if (node->status & IS_WHITE_ON_MOVE) return ((pc.nr_of_black_man - pc.nr_of_white_man)*1000 + (pc.nr_of_black_king - pc.nr_of_white_king)*3000);
	else                                 return ((pc.nr_of_white_man - pc.nr_of_black_man)*1000 + (pc.nr_of_white_king - pc.nr_of_black_king)*3000);

	
	
}