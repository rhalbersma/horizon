/*
  This file is Copyright (C) 2010 Feike Boomstra.
  Distributed under the Boost Software License, Version 1.0.
  (See accompanying file LICENSE_1_0.txt or copy at
  http://www.boost.org/LICENSE_1_0.txt)
*/


#include "BoomstraDam.v1.2.h"

#define equal(man, F1, F2) (((man & F1)  && (man & F2)) || ((man & F1) == (man & F2)))  // both bits zero or both bits one

#define MIN_L_BLACK_36 ((empty_field & FLD41) && (empty_field & FLD47) && equal(white_man, FLD37, FLD46))
#define MIN_L_BLACK_37 ((empty_field & FLD42) && (empty_field & FLD48) && equal(white_man, FLD38, FLD47))
#define MIN_L_BLACK_38 ((empty_field & FLD43) && (empty_field & FLD49) && equal(white_man, FLD39, FLD48))
#define MIN_L_BLACK_39 ((empty_field & FLD44) && (empty_field & FLD50) && equal(white_man, FLD40, FLD49))
#define MIN_L_BLACK_40 ((empty_field & FLD45) && (empty_field & FLD50))

#define MIN_R_BLACK_40 ((empty_field & FLD44) && (empty_field & FLD49) && equal(white_man, FLD39, FLD50))
#define MIN_R_BLACK_39 ((empty_field & FLD43) && (empty_field & FLD48) && equal(white_man, FLD38, FLD49))
#define MIN_R_BLACK_38 ((empty_field & FLD42) && (empty_field & FLD47) && equal(white_man, FLD37, FLD48))
#define MIN_R_BLACK_37 ((empty_field & FLD41) && (empty_field & FLD46) && equal(white_man, FLD36, FLD47))


#define MAX_L_BLACK_36 all_bits_set(empty_field, ( FLD37 |  FLD41 |  FLD42 |  FLD46 |  FLD47))
#define MAX_L_BLACK_37 all_bits_set(empty_field, ( FLD38 |  FLD42 |  FLD43 |  FLD47 |  FLD48))
#define MAX_L_BLACK_38 all_bits_set(empty_field, ( FLD39 |  FLD43 |  FLD44 |  FLD48 |  FLD49))
#define MAX_L_BLACK_39 all_bits_set(empty_field, ( FLD40 |  FLD44 |  FLD45 |  FLD49 |  FLD50))
#define MAX_L_BLACK_40 all_bits_set(empty_field, ( FLD45 |  FLD50 ))

#define MAX_R_BLACK_40 all_bits_set(empty_field, ( FLD39 |  FLD43 |  FLD44 |  FLD49 |  FLD50))
#define MAX_R_BLACK_39 all_bits_set(empty_field, ( FLD38 |  FLD42 |  FLD43 |  FLD48 |  FLD49))
#define MAX_R_BLACK_38 all_bits_set(empty_field, ( FLD37 |  FLD41 |  FLD42 |  FLD47 |  FLD48))
#define MAX_R_BLACK_37 all_bits_set(empty_field, ( FLD36 |  FLD41 |           FLD46 |  FLD47))

#define MIN_BLACK_31  ((MAX_L_BLACK_36 &&                                           (empty_field & FLD36))  | \
					  ((MAX_L_BLACK_37 | MAX_R_BLACK_37) && !(white_man & FLD32) && (empty_field & FLD37)))
#define MIN_BLACK_32 (((MAX_L_BLACK_37 | MAX_R_BLACK_37) && !(white_man & FLD31) && (empty_field & FLD37))  | \
					  ((MAX_L_BLACK_38 | MAX_R_BLACK_38) && !(white_man & FLD33) && (empty_field & FLD38)))
#define MIN_BLACK_33 (((MAX_L_BLACK_38 | MAX_R_BLACK_38) && !(white_man & FLD32) && (empty_field & FLD38))  | \
					  ((MAX_L_BLACK_39 | MAX_R_BLACK_39) && !(white_man & FLD34) && (empty_field & FLD39)))
#define MIN_BLACK_34 (((MAX_L_BLACK_39 | MAX_R_BLACK_39) && !(white_man & FLD33) && (empty_field & FLD39))  | \
					  ((MAX_L_BLACK_40 | MAX_R_BLACK_40) && !(white_man & FLD35) && (empty_field & FLD40)))
#define MIN_BLACK_35  ((MAX_L_BLACK_40 | MAX_R_BLACK_40) && !(white_man & FLD34) && (empty_field & FLD40))

#define MIN_L_WHITE_15 ((empty_field & FLD10) && (empty_field & FLD4) && equal(black_man, FLD14, FLD5))
#define MIN_L_WHITE_14 ((empty_field & FLD9)  && (empty_field & FLD3) && equal(black_man, FLD13, FLD4))
#define MIN_L_WHITE_13 ((empty_field & FLD8)  && (empty_field & FLD2) && equal(black_man, FLD12, FLD3))
#define MIN_L_WHITE_12 ((empty_field & FLD7)  && (empty_field & FLD1) && equal(black_man, FLD11, FLD2))
#define MIN_L_WHITE_11 ((empty_field & FLD6)  && (empty_field & FLD1))                        

#define MIN_R_WHITE_11 ((empty_field & FLD7)  && (empty_field & FLD2) && equal(black_man, FLD12, FLD1))
#define MIN_R_WHITE_12 ((empty_field & FLD8)  && (empty_field & FLD3) && equal(black_man, FLD13, FLD2))
#define MIN_R_WHITE_13 ((empty_field & FLD9)  && (empty_field & FLD4) && equal(black_man, FLD14, FLD3))
#define MIN_R_WHITE_14 ((empty_field & FLD10) && (empty_field & FLD5) && equal(black_man, FLD15, FLD4)) 


#define MAX_L_WHITE_15 all_bits_set(empty_field, ( FLD14 |  FLD10 |  FLD9 |  FLD5 |  FLD4)) 
#define MAX_L_WHITE_14 all_bits_set(empty_field, ( FLD13 |  FLD9  |  FLD8 |  FLD4 |  FLD3))
#define MAX_L_WHITE_13 all_bits_set(empty_field, ( FLD12 |  FLD8  |  FLD7 |  FLD3 |  FLD2))
#define MAX_L_WHITE_12 all_bits_set(empty_field, ( FLD11 |  FLD7  |  FLD6 |  FLD2 |  FLD1))
#define MAX_L_WHITE_11 all_bits_set(empty_field, (          FLD6  |          FLD1        ))

#define MAX_R_WHITE_11 all_bits_set(empty_field, ( FLD12 |  FLD8  |  FLD7  |  FLD2 |  FLD1))
#define MAX_R_WHITE_12 all_bits_set(empty_field, ( FLD13 |  FLD9  |  FLD8  |  FLD3 |  FLD2))
#define MAX_R_WHITE_13 all_bits_set(empty_field, ( FLD14 |  FLD10 |  FLD9  |  FLD4 |  FLD3))
#define MAX_R_WHITE_14 all_bits_set(empty_field, ( FLD15 |           FLD10 |  FLD5 |  FLD4))

#define MIN_WHITE_20   ((MAX_L_WHITE_15 &&                                           (empty_field & FLD15))  | \
					   ((MAX_L_WHITE_14 | MAX_R_WHITE_14) && !(black_man & FLD19) && (empty_field & FLD14))) 
#define MIN_WHITE_19  (((MAX_L_WHITE_14 | MAX_R_WHITE_14) && !(black_man & FLD20) && (empty_field & FLD14))  | \
					   ((MAX_L_WHITE_13 | MAX_R_WHITE_13) && !(black_man & FLD18) && (empty_field & FLD13)))
#define MIN_WHITE_18  (((MAX_L_WHITE_13 | MAX_R_WHITE_13) && !(black_man & FLD19) && (empty_field & FLD13))  | \
					   ((MAX_L_WHITE_12 | MAX_R_WHITE_12) && !(black_man & FLD17) && (empty_field & FLD12)))
#define MIN_WHITE_17  (((MAX_L_WHITE_12 | MAX_R_WHITE_12) && !(black_man & FLD18) && (empty_field & FLD12))  | \
					   ((MAX_L_WHITE_11 | MAX_R_WHITE_11) && !(black_man & FLD16) && (empty_field & FLD11)))
#define MIN_WHITE_16   ((MAX_L_WHITE_11 | MAX_R_WHITE_11) && !(black_man & FLD17) && (empty_field & FLD11))

#define BR31_A all_bits_set(empty_field, (FLD36 | FLD41 | FLD37 | FLD42 | FLD46 | FLD47 | FLD31))
#define BR32_A all_bits_set(empty_field, (FLD38 | FLD42 | FLD43 | FLD47 | FLD48 | FLD49 | FLD33 | FLD39 | FLD37 | FLD41 | FLD46 | FLD32))
#define BR32_B all_bits_set(empty_field, (FLD38 | FLD43 | FLD42 | FLD47 | FLD48 | FLD49 | FLD33 | FLD39 | FLD44 | FLD50 | FLD32 | FLD37))
#define BR32_C all_bits_set(empty_field, (FLD37 | FLD41 | FLD42 | FLD46 | FLD47 | FLD48 | FLD36 | FLD31 | FLD32 | FLD38))
#define BR33_A all_bits_set(empty_field, (FLD39 | FLD43 | FLD44 | FLD48 | FLD49 | FLD50 | FLD34 | FLD40 | FLD38 | FLD42 | FLD47 | FLD33))
#define BR33_B all_bits_set(empty_field, (FLD39 | FLD44 | FLD43 | FLD48 | FLD49 | FLD50 | FLD34 | FLD40 | FLD45 | FLD33 | FLD38))
#define BR33_C all_bits_set(empty_field, (FLD38 | FLD43 | FLD42 | FLD47 | FLD48 | FLD49 | FLD37 | FLD32 | FLD39 | FLD44 | FLD50 | FLD33))
#define BR33_D all_bits_set(empty_field, (FLD38 | FLD42 | FLD43 | FLD47 | FLD48 | FLD49 | FLD37 | FLD32 | FLD46 | FLD41 | FLD37 | FLD33 | FLD39))
#define BR34_A all_bits_set(empty_field, (FLD40 | FLD45 | FLD44 | FLD49 | FLD50 | FLD35 | FLD34 | FLD39))
#define BR34_B all_bits_set(empty_field, (FLD39 | FLD44 | FLD43 | FLD48 | FLD49 | FLD50 | FLD38 | FLD33 | FLD40 | FLD45 | FLD34))
#define BR34_C all_bits_set(empty_field, (FLD39 | FLD43 | FLD44 | FLD48 | FLD49 | FLD50 | FLD38 | FLD33 | FLD47 | FLD42 | FLD38 | FLD34 | FLD40))
#define BR35_A all_bits_set(empty_field, (FLD40 | FLD44 | FLD45 | FLD50 | FLD34 | FLD35))

#define WR31_A all_bits_set(empty_field, (FLD15 | FLD10 | FLD14 | FLD9  | FLD5  | FLD4  | FLD20))
#define WR32_A all_bits_set(empty_field, (FLD13 | FLD9  | FLD8  | FLD4  | FLD3  | FLD2  | FLD18 | FLD12 | FLD14 | FLD10 | FLD5  | FLD19))
#define WR32_B all_bits_set(empty_field, (FLD13 | FLD8  | FLD9  | FLD4  | FLD3  | FLD2  | FLD18 | FLD12 | FLD7  | FLD1  | FLD19 | FLD14))
#define WR32_C all_bits_set(empty_field, (FLD14 | FLD10 | FLD9  | FLD5  | FLD4  | FLD3  | FLD15 | FLD20 | FLD19 | FLD13))
#define WR33_A all_bits_set(empty_field, (FLD12 | FLD8  | FLD7  | FLD3  | FLD2  | FLD1  | FLD17 | FLD11 | FLD13 | FLD9  | FLD4  | FLD18))
#define WR33_B all_bits_set(empty_field, (FLD12 | FLD7  | FLD8  | FLD3  | FLD2  | FLD1  | FLD17 | FLD11 | FLD6  | FLD18 | FLD13))
#define WR33_C all_bits_set(empty_field, (FLD13 | FLD8  | FLD9  | FLD4  | FLD3  | FLD2  | FLD14 | FLD19 | FLD12 | FLD7  | FLD1  | FLD18 ))
#define WR33_D all_bits_set(empty_field, (FLD13 | FLD9  | FLD8  | FLD4  | FLD3  | FLD2  | FLD14 | FLD19 | FLD5  | FLD10 | FLD14 | FLD18 | FLD12))
#define WR34_A all_bits_set(empty_field, (FLD11 | FLD6  | FLD7  | FLD2  | FLD1  | FLD16 | FLD17 | FLD12))
#define WR34_B all_bits_set(empty_field, (FLD12 | FLD7  | FLD8  | FLD3  | FLD2  | FLD1  | FLD13 | FLD18 | FLD11 | FLD6  | FLD17))
#define WR34_C all_bits_set(empty_field, (FLD12 | FLD8  | FLD7  | FLD3  | FLD2  | FLD1  | FLD13 | FLD18 | FLD4  | FLD9  | FLD13 | FLD17 | FLD11))
#define WR35_A all_bits_set(empty_field, (FLD11 | FLD7  | FLD6  | FLD1  | FLD17 | FLD16))

#define TB31 (BR31_A)
#define TB32 (BR32_A || BR32_B || BR32_C)
#define TB33 (BR33_A || BR33_B || BR33_C || BR33_D)
#define TB34 (BR34_A || BR34_B || BR34_C)
#define TB35 (BR35_A)

#define TW20 (WR31_A)
#define TW19 (WR32_A || WR32_B || WR32_C)
#define TW18 (WR33_A || WR33_B || WR33_C || WR33_D)
#define TW17 (WR34_A || WR34_B || WR34_C )
#define TW16 (WR35_A)

#define BR26_A ((empty_field & FLD27) && TB31)
#define BR27_A ((empty_field & FLD26) && TB31)
#define BR27_B ((empty_field & FLD28) && TB32)
#define BR28_A ((empty_field & FLD27) && TB32)
#define BR28_B ((empty_field & FLD29) && TB33)
#define BR29_A ((empty_field & FLD28) && TB33)
#define BR29_B ((empty_field & FLD30) && TB34)
#define BR30_A ((empty_field & FLD29) && TB34)
#define BR30_B (TB35)

#define WR25_A ((empty_field & FLD24) && TW20)
#define WR24_A ((empty_field & FLD25) && TW20)
#define WR24_B ((empty_field & FLD23) && TW19)
#define WR23_A ((empty_field & FLD24) && TW19)
#define WR23_B ((empty_field & FLD22) && TW18)
#define WR22_A ((empty_field & FLD23) && TW18)
#define WR22_B ((empty_field & FLD21) && TW17)
#define WR21_A ((empty_field & FLD22) && TW17)
#define WR21_B (TW16)

#define TB26 (BR26_A)
#define TB27 (BR27_A || BR27_B)
#define TB28 (BR28_A || BR28_B)
#define TB29 (BR29_A || BR29_B)
#define TB30 (BR30_A || BR30_B)

#define TW25 (WR25_A)
#define TW24 (WR24_A || WR24_B)
#define TW23 (WR23_A || WR23_B)
#define TW22 (WR22_A || WR22_B)
#define TW21 (WR21_A || WR21_B)

 int nr_of_white_min_free_path(BitArray white_man, BitArray black_man, BitArray empty_field)
{	int cnt = 0;
	if ((white_man & FLD15) &&  MIN_L_WHITE_15) cnt++;	
	if ((white_man & FLD14) && (MIN_L_WHITE_14 || MIN_R_WHITE_14)) cnt++;	
	if ((white_man & FLD13) && (MIN_L_WHITE_13 || MIN_R_WHITE_13)) cnt++;	
	if ((white_man & FLD12) && (MIN_L_WHITE_12 || MIN_R_WHITE_12)) cnt++;	
	if ((white_man & FLD11) && (MIN_L_WHITE_11 || MIN_R_WHITE_11)) cnt++;	
	if ((white_man & FLD20) &&  MIN_WHITE_20) cnt++;	
	if ((white_man & FLD19) &&  MIN_WHITE_19) cnt++;	
	if ((white_man & FLD18) &&  MIN_WHITE_18) cnt++;	
	if ((white_man & FLD17) &&  MIN_WHITE_17) cnt++;	
	if ((white_man & FLD16) &&  MIN_WHITE_16) cnt++;	
	if ((white_man & FLD25) &&  TW25) cnt++;	
	if ((white_man & FLD24) &&  TW24) cnt++;	
	if ((white_man & FLD23) &&  TW23) cnt++;	
	if ((white_man & FLD22) &&  TW22) cnt++;	
	if ((white_man & FLD21) &&  TW21) cnt++;	

	return cnt;	
} 

 int nr_of_white_max_free_path(BitArray white_man, BitArray black_man, BitArray empty_field)
{	int cnt = 0;
	if ((white_man & FLD15) &&  MAX_L_WHITE_15) cnt++;	
	if ((white_man & FLD14) && (MAX_L_WHITE_14 || MAX_R_WHITE_14)) cnt++;	
	if ((white_man & FLD13) && (MAX_L_WHITE_13 || MAX_R_WHITE_13)) cnt++;	
	if ((white_man & FLD12) && (MAX_L_WHITE_12 || MAX_R_WHITE_12)) cnt++;	
	if ((white_man & FLD11) && (MAX_L_WHITE_11 || MAX_R_WHITE_11)) cnt++;
	if ((white_man & FLD20) && (TW20)) cnt++;
	if ((white_man & FLD19) && (TW19)) cnt++;
	if ((white_man & FLD18) && (TW18)) cnt++;
	if ((white_man & FLD17) && (TW17)) cnt++;
	if ((white_man & FLD16) && (TW16)) cnt++;
	return cnt;	
}

 int nr_of_black_min_free_path(BitArray white_man, BitArray black_man, BitArray empty_field)
{	int cnt = 0;
	if ((black_man & FLD36) &&  MIN_L_BLACK_36) cnt++;	
	if ((black_man & FLD37) && (MIN_L_BLACK_37 || MIN_R_BLACK_37)) cnt++;	
	if ((black_man & FLD38) && (MIN_L_BLACK_38 || MIN_R_BLACK_38)) cnt++;	
	if ((black_man & FLD39) && (MIN_L_BLACK_39 || MIN_R_BLACK_39)) cnt++;	
	if ((black_man & FLD40) && (MIN_L_BLACK_40 || MIN_R_BLACK_40)) cnt++;	
	if ((black_man & FLD31) &&  MIN_BLACK_31) cnt++;	
	if ((black_man & FLD32) &&  MIN_BLACK_32) cnt++;	
	if ((black_man & FLD33) &&  MIN_BLACK_33) cnt++;	
	if ((black_man & FLD34) &&  MIN_BLACK_34) cnt++;	
	if ((black_man & FLD35) &&  MIN_BLACK_35) cnt++;	
	if ((black_man & FLD26) &&  TB26) cnt++;	
	if ((black_man & FLD27) &&  TB27) cnt++;	
	if ((black_man & FLD28) &&  TB28) cnt++;	
	if ((black_man & FLD29) &&  TB29) cnt++;	
	if ((black_man & FLD30) &&  TB30) cnt++;	
	return cnt;	
} 

 int nr_of_black_max_free_path(BitArray white_man, BitArray black_man, BitArray empty_field)
{	int cnt = 0;
	if ((black_man & FLD40) && (MAX_L_BLACK_40 || MAX_R_BLACK_40)) cnt++;	
	if ((black_man & FLD39) && (MAX_L_BLACK_39 || MAX_R_BLACK_39)) cnt++;	
	if ((black_man & FLD38) && (MAX_L_BLACK_38 || MAX_R_BLACK_38)) cnt++;	
	if ((black_man & FLD37) && (MAX_L_BLACK_37 || MAX_R_BLACK_37)) cnt++;	
	if ((black_man & FLD36) && (MAX_L_BLACK_36                  )) cnt++;	
	if ((black_man & FLD31) && (TB31)) cnt++;
	if ((black_man & FLD32) && (TB32)) cnt++;
	if ((black_man & FLD33) && (TB33)) cnt++;
	if ((black_man & FLD34) && (TB34)) cnt++;
	if ((black_man & FLD35) && (TB35)) cnt++;

	return cnt;	
} 