/*
  This file is Copyright (C) 2010 Feike Boomstra.
  Distributed under the Boost Software License, Version 1.0.
  (See accompanying file LICENSE_1_0.txt or copy at
  http://www.boost.org/LICENSE_1_0.txt)
*/


#include "BoomstraDam.v2.5.h"

extern int voorpost_white_se_17(BitArray, BitArray, BitArray, int);
extern int voorpost_white_sw_17(BitArray, BitArray, BitArray, int);
extern int voorpost_white_se_18(BitArray, BitArray, BitArray, int);
extern int voorpost_white_sw_18(BitArray, BitArray, BitArray, int);
extern int voorpost_white_se_19(BitArray, BitArray, BitArray, int);
extern int voorpost_white_sw_19(BitArray, BitArray, BitArray, int);
extern int voorpost_white_se_22(BitArray, BitArray, BitArray, int);
extern int voorpost_white_sw_22(BitArray, BitArray, BitArray, int);
extern int voorpost_white_se_23(BitArray, BitArray, BitArray, int);
extern int voorpost_white_sw_23(BitArray, BitArray, BitArray, int);
extern int voorpost_white_se_24(BitArray, BitArray, BitArray, int);
extern int voorpost_white_sw_24(BitArray, BitArray, BitArray, int);

extern int voorpost_black_ne_34(BitArray, BitArray, BitArray, int);
extern int voorpost_black_nw_34(BitArray, BitArray, BitArray, int);
extern int voorpost_black_ne_33(BitArray, BitArray, BitArray, int);
extern int voorpost_black_nw_33(BitArray, BitArray, BitArray, int);
extern int voorpost_black_ne_32(BitArray, BitArray, BitArray, int);
extern int voorpost_black_nw_32(BitArray, BitArray, BitArray, int);
extern int voorpost_black_ne_29(BitArray, BitArray, BitArray, int);
extern int voorpost_black_nw_29(BitArray, BitArray, BitArray, int);
extern int voorpost_black_ne_28(BitArray, BitArray, BitArray, int);
extern int voorpost_black_nw_28(BitArray, BitArray, BitArray, int);
extern int voorpost_black_ne_27(BitArray, BitArray, BitArray, int);
extern int voorpost_black_nw_27(BitArray, BitArray, BitArray, int);

extern int calc_dist_1(BitArray, BitArray *, BitArray);
int calc_dist_1(BitArray bc0, BitArray * used_bits, BitArray man) {
BitArray temp;
	if (temp = (bc0 & ~(*used_bits) & man)) {*used_bits |= temp ^ (temp & (temp - 1)); return 0;}
	return 100;

}
extern int calc_dist_2(BitArray, BitArray, BitArray *, BitArray);
int calc_dist_2(BitArray bc0, BitArray bc1, BitArray * used_bits, BitArray man) {
	BitArray temp;
	if (temp = (bc0 & ~(*used_bits) & man)) {*used_bits |= temp ^ (temp & (temp - 1)); return 0;}
	if (temp = (bc1 & ~(*used_bits) & man)) {*used_bits |= temp ^ (temp & (temp - 1)); return 1;}
	return 100;

}
extern int calc_dist_3(BitArray, BitArray, BitArray, BitArray *, BitArray);
int calc_dist_3(BitArray bc0, BitArray bc1, BitArray bc2, BitArray * used_bits, BitArray man) {
	BitArray temp;
	if (temp = (bc0 & ~(*used_bits) & man)) {*used_bits |= temp ^ (temp & (temp - 1)); return 0;}
	if (temp = (bc1 & ~(*used_bits) & man)) {*used_bits |= temp ^ (temp & (temp - 1)); return 1;}
	if (temp = (bc2 & ~(*used_bits) & man)) {*used_bits |= temp ^ (temp & (temp - 1)); return 2;}
	return 100;

}
extern int calc_dist_4(BitArray, BitArray, BitArray, BitArray, BitArray *, BitArray);
int calc_dist_4(BitArray bc0, BitArray bc1, BitArray bc2, BitArray bc3, BitArray * used_bits, BitArray man) {
	BitArray temp;
	if (temp = (bc0 & ~(*used_bits) & man)) {*used_bits |= temp ^ (temp & (temp - 1)); return 0;}
	if (temp = (bc1 & ~(*used_bits) & man)) {*used_bits |= temp ^ (temp & (temp - 1)); return 1;}
	if (temp = (bc2 & ~(*used_bits) & man)) {*used_bits |= temp ^ (temp & (temp - 1)); return 2;}
	if (temp = (bc3 & ~(*used_bits) & man)) {*used_bits |= temp ^ (temp & (temp - 1)); return 3;}
	return 100;

}
extern int calc_dist_5(BitArray, BitArray, BitArray, BitArray, BitArray, BitArray *, BitArray);
int calc_dist_5(BitArray bc0, BitArray bc1, BitArray bc2, BitArray bc3, BitArray bc4, BitArray * used_bits, BitArray man) {
	BitArray temp;
	if (temp = (bc0 & ~(*used_bits) & man)) {*used_bits |= temp ^ (temp & (temp - 1)); return 0;}
	if (temp = (bc1 & ~(*used_bits) & man)) {*used_bits |= temp ^ (temp & (temp - 1)); return 1;}
	if (temp = (bc2 & ~(*used_bits) & man)) {*used_bits |= temp ^ (temp & (temp - 1)); return 2;}
	if (temp = (bc3 & ~(*used_bits) & man)) {*used_bits |= temp ^ (temp & (temp - 1)); return 3;}
	if (temp = (bc4 & ~(*used_bits) & man)) {*used_bits |= temp ^ (temp & (temp - 1)); return 4;}
	return 100;

}
extern int calc_dist_6(BitArray, BitArray, BitArray, BitArray, BitArray, BitArray, BitArray *, BitArray);
int calc_dist_6(BitArray bc0, BitArray bc1, BitArray bc2, BitArray bc3, BitArray bc4, BitArray bc5, BitArray * used_bits, BitArray man) {
	BitArray temp;
	if (temp = (bc0 & ~(*used_bits) & man)) {*used_bits |= temp ^ (temp & (temp - 1)); return 0;}
	if (temp = (bc1 & ~(*used_bits) & man)) {*used_bits |= temp ^ (temp & (temp - 1)); return 1;}
	if (temp = (bc2 & ~(*used_bits) & man)) {*used_bits |= temp ^ (temp & (temp - 1)); return 2;}
	if (temp = (bc3 & ~(*used_bits) & man)) {*used_bits |= temp ^ (temp & (temp - 1)); return 3;}
	if (temp = (bc4 & ~(*used_bits) & man)) {*used_bits |= temp ^ (temp & (temp - 1)); return 4;}
	if (temp = (bc5 & ~(*used_bits) & man)) {*used_bits |= temp ^ (temp & (temp - 1)); return 5;}
	return 100;

}
extern int calc_dist_7(BitArray, BitArray, BitArray, BitArray, BitArray, BitArray, BitArray, BitArray *, BitArray);
int calc_dist_7(BitArray bc0, BitArray bc1, BitArray bc2, BitArray bc3, BitArray bc4, BitArray bc5, BitArray bc6, BitArray * used_bits, BitArray man) {
	BitArray temp;
	if (temp = (bc0 & ~(*used_bits) & man)) {*used_bits |= temp ^ (temp & (temp - 1)); return 0;}
	if (temp = (bc1 & ~(*used_bits) & man)) {*used_bits |= temp ^ (temp & (temp - 1)); return 1;}
	if (temp = (bc2 & ~(*used_bits) & man)) {*used_bits |= temp ^ (temp & (temp - 1)); return 2;}
	if (temp = (bc3 & ~(*used_bits) & man)) {*used_bits |= temp ^ (temp & (temp - 1)); return 3;}
	if (temp = (bc4 & ~(*used_bits) & man)) {*used_bits |= temp ^ (temp & (temp - 1)); return 4;}
	if (temp = (bc5 & ~(*used_bits) & man)) {*used_bits |= temp ^ (temp & (temp - 1)); return 5;}
	if (temp = (bc6 & ~(*used_bits) & man)) {*used_bits |= temp ^ (temp & (temp - 1)); return 6;}
	return 100;

}
extern int calc_dist_8(BitArray, BitArray, BitArray, BitArray, BitArray, BitArray, BitArray, BitArray, BitArray *, BitArray);
int calc_dist_8(BitArray bc0, BitArray bc1, BitArray bc2, BitArray bc3, BitArray bc4, BitArray bc5, BitArray bc6, BitArray bc7, BitArray * used_bits, BitArray man) {
	BitArray temp;
	if (temp = (bc0 & ~(*used_bits) & man)) {*used_bits |= temp ^ (temp & (temp - 1)); return 0;}
	if (temp = (bc1 & ~(*used_bits) & man)) {*used_bits |= temp ^ (temp & (temp - 1)); return 1;}
	if (temp = (bc2 & ~(*used_bits) & man)) {*used_bits |= temp ^ (temp & (temp - 1)); return 2;}
	if (temp = (bc3 & ~(*used_bits) & man)) {*used_bits |= temp ^ (temp & (temp - 1)); return 3;}
	if (temp = (bc4 & ~(*used_bits) & man)) {*used_bits |= temp ^ (temp & (temp - 1)); return 4;}
	if (temp = (bc5 & ~(*used_bits) & man)) {*used_bits |= temp ^ (temp & (temp - 1)); return 5;}
	if (temp = (bc6 & ~(*used_bits) & man)) {*used_bits |= temp ^ (temp & (temp - 1)); return 6;}
	if (temp = (bc7 & ~(*used_bits) & man)) {*used_bits |= temp ^ (temp & (temp - 1)); return 7;}
	return 100;

}
extern int calc_dist_9(BitArray, BitArray, BitArray, BitArray, BitArray, BitArray, BitArray, BitArray, BitArray, BitArray *, BitArray);
int calc_dist_9(BitArray bc0, BitArray bc1, BitArray bc2, BitArray bc3, BitArray bc4, BitArray bc5, BitArray bc6, BitArray bc7, BitArray bc8, BitArray * used_bits, BitArray man) {
	BitArray temp;
	if (temp = (bc0 & ~(*used_bits) & man)) {*used_bits |= temp ^ (temp & (temp - 1)); return 0;}
	if (temp = (bc1 & ~(*used_bits) & man)) {*used_bits |= temp ^ (temp & (temp - 1)); return 1;}
	if (temp = (bc2 & ~(*used_bits) & man)) {*used_bits |= temp ^ (temp & (temp - 1)); return 2;}
	if (temp = (bc3 & ~(*used_bits) & man)) {*used_bits |= temp ^ (temp & (temp - 1)); return 3;}
	if (temp = (bc4 & ~(*used_bits) & man)) {*used_bits |= temp ^ (temp & (temp - 1)); return 4;}
	if (temp = (bc5 & ~(*used_bits) & man)) {*used_bits |= temp ^ (temp & (temp - 1)); return 5;}
	if (temp = (bc6 & ~(*used_bits) & man)) {*used_bits |= temp ^ (temp & (temp - 1)); return 6;}
	if (temp = (bc7 & ~(*used_bits) & man)) {*used_bits |= temp ^ (temp & (temp - 1)); return 7;}
	if (temp = (bc8 & ~(*used_bits) & man)) {*used_bits |= temp ^ (temp & (temp - 1)); return 8;}
	return 100;
}


int calc_black_voorpost(BitArray white_man, BitArray black_man, BitArray empty_field, int white_on_move) {
int inc;
int black_voorpost_value = 0;

if ((white_on_move)) inc = 0; else inc = -1;

if  (!(black_man & (FLD37|FLD38|FLD39|FLD40|FLD41|FLD42|FLD43|FLD44))) {
	if (black_man & (FLD32|FLD33|FLD34)) {
			black_voorpost_value += imin(voorpost_black_ne_32(white_man, black_man, empty_field, inc), voorpost_black_nw_32(white_man, black_man, empty_field, inc));
			black_voorpost_value += imin(voorpost_black_ne_33(white_man, black_man, empty_field, inc), voorpost_black_nw_33(white_man, black_man, empty_field, inc));
			black_voorpost_value += imin(voorpost_black_ne_34(white_man, black_man, empty_field, inc), voorpost_black_nw_34(white_man, black_man, empty_field, inc));
	}
	else if (black_man & (FLD27|FLD28|FLD29)) {
		black_voorpost_value += imin(voorpost_black_ne_27(white_man, black_man, empty_field, inc), voorpost_black_nw_27(white_man, black_man, empty_field, inc));
		black_voorpost_value += imin(voorpost_black_ne_28(white_man, black_man, empty_field, inc), voorpost_black_nw_28(white_man, black_man, empty_field, inc));
		black_voorpost_value += imin(voorpost_black_ne_29(white_man, black_man, empty_field, inc), voorpost_black_nw_29(white_man, black_man, empty_field, inc));
	}

}
return black_voorpost_value;

}

int calc_white_voorpost(BitArray white_man, BitArray black_man, BitArray empty_field, int white_on_move) {
int inc;
int white_voorpost_value = 0;

if (white_on_move) inc = -1; else inc = 0;

if (!(white_man & (BLD37|BLD38|BLD39|BLD40|BLD41|BLD42|BLD43|BLD44))) {
	if (white_man & (BLD32|BLD33|BLD34)) {
		white_voorpost_value += imin(voorpost_white_sw_17(white_man, black_man, empty_field, inc), voorpost_white_se_17(white_man, black_man, empty_field, inc));
		white_voorpost_value += imin(voorpost_white_sw_18(white_man, black_man, empty_field, inc), voorpost_white_se_18(white_man, black_man, empty_field, inc));
		white_voorpost_value += imin(voorpost_white_sw_19(white_man, black_man, empty_field, inc), voorpost_white_se_19(white_man, black_man, empty_field, inc));
	} else if (white_man & (BLD27|BLD28|BLD29)) {
		white_voorpost_value += imin(voorpost_white_sw_22(white_man, black_man, empty_field, inc), voorpost_white_se_22(white_man, black_man, empty_field, inc));
		white_voorpost_value += imin(voorpost_white_sw_23(white_man, black_man, empty_field, inc), voorpost_white_se_23(white_man, black_man, empty_field, inc));
		white_voorpost_value += imin(voorpost_white_sw_24(white_man, black_man, empty_field, inc), voorpost_white_se_24(white_man, black_man, empty_field, inc));
	}
}
return white_voorpost_value;
}
