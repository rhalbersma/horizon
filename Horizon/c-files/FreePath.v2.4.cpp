/*
  This file is Copyright (C) 2010 Feike Boomstra.
  Distributed under the Boost Software License, Version 1.0.
  (See accompanying file LICENSE_1_0.txt or copy at
  http://www.boost.org/LICENSE_1_0.txt)
*/


#include "BoomstraDam.v2.5.h"
#include "eval.v2.4.h"

#define equal(man, F1, F2) (((man & F1)  && (man & F2)) || ((empty_field & F1) && (empty_field & F2)))  // both empty or the same
#define gate(own_man, oth_man, F1, F2) ((equal(oth_man, F1, F2)) || ((own_man & F1) && (oth_man & F2)))
#define lim_gate(own_man, oth_man, F1, F2) (((empty_field & F1) && (empty_field & F2)) || ((own_man & F1) && (oth_man & F2)))

#define MIN_L_BLACK_36 ((empty_field & FLD41) && (empty_field & FLD47) && gate(black_man, white_man, FLD37, FLD46))
#define MIN_L_BLACK_37 ((empty_field & FLD42) && (empty_field & FLD48) && gate(black_man, white_man, FLD38, FLD47) && (empty_or_black & FLD41))
#define MIN_L_BLACK_38 ((empty_field & FLD43) && (empty_field & FLD49) && gate(black_man, white_man, FLD39, FLD48) && (empty_or_black & FLD42))
#define MIN_L_BLACK_39 ((empty_field & FLD44) && (empty_field & FLD50) && gate(black_man, white_man, FLD40, FLD49) && (empty_or_black & FLD43))
#define MIN_L_BLACK_40 ((empty_field & FLD45) && (empty_field & FLD50) && (empty_or_black & FLD44))

#define MIN_R_BLACK_40 ((empty_field & FLD44) && (empty_field & FLD49) && gate(black_man, white_man, FLD39, FLD50) && (empty_or_white & FLD45))
#define MIN_R_BLACK_39 ((empty_field & FLD43) && (empty_field & FLD48) && gate(black_man, white_man, FLD38, FLD49) && (empty_or_white & FLD44))
#define MIN_R_BLACK_38 ((empty_field & FLD42) && (empty_field & FLD47) && gate(black_man, white_man, FLD37, FLD48) && (empty_or_white & FLD43))
#define MIN_R_BLACK_37 ((empty_field & FLD41) && (empty_field & FLD46) && gate(black_man, white_man, FLD36, FLD47) && (empty_or_white & FLD42))

#define MIN_L_WHITE_15 ((empty_field & FLD10) && (empty_field & FLD4) && gate(white_man, black_man, FLD14, FLD5))
#define MIN_L_WHITE_14 ((empty_field & FLD9)  && (empty_field & FLD3) && gate(white_man, black_man, FLD13, FLD4) && (empty_or_white & FLD10))
#define MIN_L_WHITE_13 ((empty_field & FLD8)  && (empty_field & FLD2) && gate(white_man, black_man, FLD12, FLD3) && (empty_or_white & FLD9))
#define MIN_L_WHITE_12 ((empty_field & FLD7)  && (empty_field & FLD1) && gate(white_man, black_man, FLD11, FLD2) && (empty_or_white & FLD8))
#define MIN_L_WHITE_11 ((empty_field & FLD6)  && (empty_field & FLD1) && (empty_or_white & FLD7))

#define MIN_R_WHITE_11 ((empty_field & FLD7)  && (empty_field & FLD2) && gate(white_man, black_man, FLD12, FLD1) && (empty_or_white & FLD6))
#define MIN_R_WHITE_12 ((empty_field & FLD8)  && (empty_field & FLD3) && gate(white_man, black_man, FLD13, FLD2) && (empty_or_white & FLD7))
#define MIN_R_WHITE_13 ((empty_field & FLD9)  && (empty_field & FLD4) && gate(white_man, black_man, FLD14, FLD3) && (empty_or_white & FLD8))
#define MIN_R_WHITE_14 ((empty_field & FLD10) && (empty_field & FLD5) && gate(white_man, black_man, FLD15, FLD4) && (empty_or_white & FLD9))

#define MAX_L_BLACK_36 ((all_bits_set(empty_field,(FLD41 | FLD47 | FLD42)) && lim_gate(black_man, white_man, FLD37, FLD46)))
#define MAX_L_BLACK_37 ((all_bits_set(empty_field,(FLD42 | FLD48 | FLD43)) && lim_gate(black_man, white_man, FLD38, FLD47)))
#define MAX_L_BLACK_38 ((all_bits_set(empty_field,(FLD43 | FLD49 | FLD44)) && lim_gate(black_man, white_man, FLD39, FLD48)))
#define MAX_L_BLACK_39 ((all_bits_set(empty_field,(FLD44 | FLD50 | FLD45)) && lim_gate(black_man, white_man, FLD40, FLD49)))
#define MAX_L_BLACK_40 ((all_bits_set(empty_field,(FLD45 | FLD50))))

#define MAX_R_BLACK_40 ((all_bits_set(empty_field,(FLD44 | FLD49 | FLD43)) && lim_gate(black_man, white_man, FLD39, FLD50)))
#define MAX_R_BLACK_39 ((all_bits_set(empty_field,(FLD43 | FLD48 | FLD42)) && lim_gate(black_man, white_man, FLD38, FLD49)))
#define MAX_R_BLACK_38 ((all_bits_set(empty_field,(FLD42 | FLD47 | FLD41)) && lim_gate(black_man, white_man, FLD37, FLD48)))
#define MAX_R_BLACK_37 ((all_bits_set(empty_field,(FLD41 | FLD46 )) && lim_gate(black_man, white_man, FLD36, FLD47)))

#define MAX_L_WHITE_15 ((all_empty((FLD10|FLD4|FLD9))) && lim_gate(white_man, black_man, FLD14, FLD5))
#define MAX_L_WHITE_14 ((all_empty((FLD9 |FLD3|FLD8))) && lim_gate(white_man, black_man, FLD13, FLD4))
#define MAX_L_WHITE_13 ((all_empty((FLD8 |FLD2|FLD7))) && lim_gate(white_man, black_man, FLD12, FLD3))
#define MAX_L_WHITE_12 ((all_empty((FLD7 |FLD1|FLD6))) && lim_gate(white_man, black_man, FLD11, FLD2))
#define MAX_L_WHITE_11 ((all_empty((FLD6 |FLD1))))

#define MAX_R_WHITE_11 ((all_empty((FLD7 |FLD2|FLD8))) && lim_gate(white_man, black_man, FLD12, FLD1))
#define MAX_R_WHITE_12 ((all_empty((FLD8 |FLD3|FLD9))) && lim_gate(white_man, black_man, FLD13, FLD2))
#define MAX_R_WHITE_13 ((all_empty((FLD9 |FLD4|FLD10))) && lim_gate(white_man, black_man, FLD14, FLD3))
#define MAX_R_WHITE_14 ((all_empty((FLD10|FLD5))) && lim_gate(white_man, black_man, FLD15, FLD4))

/*
#define MAX_L_BLACK_36 all_bits_set(empty_field, ( FLD37 |  FLD41 |  FLD42 |  FLD46 |  FLD47))
#define MAX_L_BLACK_37 all_bits_set(empty_field, ( FLD38 |  FLD42 |  FLD43 |  FLD47 |  FLD48))
#define MAX_L_BLACK_38 all_bits_set(empty_field, ( FLD39 |  FLD43 |  FLD44 |  FLD48 |  FLD49))
#define MAX_L_BLACK_39 all_bits_set(empty_field, ( FLD40 |  FLD44 |  FLD45 |  FLD49 |  FLD50))
#define MAX_L_BLACK_40 all_bits_set(empty_field, ( FLD45 |  FLD50 ))

#define MAX_R_BLACK_40 all_bits_set(empty_field, ( FLD39 |  FLD43 |  FLD44 |  FLD49 |  FLD50))
#define MAX_R_BLACK_39 all_bits_set(empty_field, ( FLD38 |  FLD42 |  FLD43 |  FLD48 |  FLD49))
#define MAX_R_BLACK_38 all_bits_set(empty_field, ( FLD37 |  FLD41 |  FLD42 |  FLD47 |  FLD48))
#define MAX_R_BLACK_37 all_bits_set(empty_field, ( FLD36 |  FLD41 |           FLD46 |  FLD47))
*/
#define MIN_BLACK_31  ((MAX_L_BLACK_36 &&                                                              (all_empty((FLD41 | FLD36))))  || \
					  ((MAX_L_BLACK_37 || MAX_R_BLACK_37) && gate(black_man, white_man, FLD32,FLD41) && (all_empty((FLD42 | FLD37)))))
#define MIN_BLACK_32 (((MAX_L_BLACK_37 || MAX_R_BLACK_37) && gate(black_man, white_man, FLD31,FLD42) && (all_empty((FLD41 | FLD37))))  || \
					  ((MAX_L_BLACK_38 || MAX_R_BLACK_38) && gate(black_man, white_man, FLD33,FLD42) && (all_empty((FLD43 | FLD38)))))
#define MIN_BLACK_33 (((MAX_L_BLACK_38 || MAX_R_BLACK_38) && gate(black_man, white_man, FLD32,FLD43) && (all_empty((FLD42 | FLD38))))  || \
					  ((MAX_L_BLACK_39 || MAX_R_BLACK_39) && gate(black_man, white_man, FLD34,FLD43) && (all_empty((FLD44 | FLD39)))))
#define MIN_BLACK_34 (((MAX_L_BLACK_39 || MAX_R_BLACK_39) && gate(black_man, white_man, FLD33,FLD44) && (all_empty((FLD43 | FLD39))))  || \
					  ((MAX_L_BLACK_40 || MAX_R_BLACK_40) && gate(black_man, white_man, FLD35,FLD44) && (all_empty((FLD45 | FLD40)))))
#define MIN_BLACK_35  ((MAX_L_BLACK_40 || MAX_R_BLACK_40) && gate(black_man, white_man, FLD34,FLD45) && (all_empty((FLD44 | FLD40))))

#define MIN_WHITE_20   ((MAX_L_WHITE_15 &&                                                              (all_empty((FLD10 | FLD15))))  || \
					   ((MAX_L_WHITE_14 || MAX_R_WHITE_14) && gate(white_man, black_man, FLD19,FLD10) && (all_empty((FLD9 | FLD14)))))
#define MIN_WHITE_19  (((MAX_L_WHITE_14 || MAX_R_WHITE_14) && gate(white_man, black_man, FLD20, FLD9) && (all_empty((FLD10 | FLD14))))  || \
					   ((MAX_L_WHITE_13 || MAX_R_WHITE_13) && gate(white_man, black_man, FLD18, FLD9) && (all_empty((FLD8 | FLD13)))))
#define MIN_WHITE_18  (((MAX_L_WHITE_13 || MAX_R_WHITE_13) && gate(white_man, black_man, FLD19, FLD8) && (all_empty((FLD9 | FLD13))))  || \
					   ((MAX_L_WHITE_12 || MAX_R_WHITE_12) && gate(white_man, black_man, FLD17, FLD8) && (all_empty((FLD7 | FLD12)))))
#define MIN_WHITE_17  (((MAX_L_WHITE_12 || MAX_R_WHITE_12) && gate(white_man, black_man, FLD18, FLD7) && (all_empty((FLD8 | FLD12))))  || \
					   ((MAX_L_WHITE_11 || MAX_R_WHITE_11) && gate(white_man, black_man, FLD16, FLD7) && (all_empty((FLD6 | FLD11)))))
#define MIN_WHITE_16   ((MAX_L_WHITE_11 || MAX_R_WHITE_11) && gate(white_man, black_man, FLD17, FLD6) && (all_empty((FLD7 | FLD11))))


#define MAX_BLACK_31  ((MAX_L_BLACK_36 &&                                                                  (all_empty((FLD41 |FLD46 |FLD47 | FLD36 | FLD42 | FLD48))))  || \
					  ((MAX_L_BLACK_37 || MAX_R_BLACK_37) && lim_gate(black_man, white_man, FLD32,FLD41) && (all_empty((FLD42 |FLD47 |FLD48 | FLD37 | FLD43)))))
#define MAX_BLACK_32 ((((MAX_L_BLACK_37 || MAX_R_BLACK_37) && lim_gate(black_man, white_man, FLD31,FLD42) && (all_empty((FLD41 |FLD46 |FLD47 | FLD37))))  || \
					  ((MAX_L_BLACK_38 || MAX_R_BLACK_38) && lim_gate(black_man, white_man, FLD33,FLD42) && (all_empty((FLD43 |FLD48 |FLD49 | FLD38 | FLD44))))) && !((all_white((FLD33|FLD36))) && (empty_field & FLD31) && !((all_white((FLD31|FLD39))) && (empty_field & FLD33))))
#define MAX_BLACK_33 ((((MAX_L_BLACK_38 || MAX_R_BLACK_38) && lim_gate(black_man, white_man, FLD32,FLD43) && (all_empty((FLD42 |FLD47 |FLD48 | FLD38 | FLD41))))  || \
					  ((MAX_L_BLACK_39 || MAX_R_BLACK_39) && lim_gate(black_man, white_man, FLD34,FLD43) && (all_empty((FLD44 |FLD49 |FLD50 | FLD39 | FLD45))))) && !((all_white((FLD34|FLD37))) && (empty_field & FLD32) && !((all_white((FLD32|FLD40))) && (empty_field & FLD34))))
#define MAX_BLACK_34 ((((MAX_L_BLACK_39 || MAX_R_BLACK_39) && lim_gate(black_man, white_man, FLD33,FLD44) && (all_empty((FLD43 |FLD48 |FLD49 | FLD39 | FLD42))))  || \
					  ((MAX_L_BLACK_40 || MAX_R_BLACK_40) && lim_gate(black_man, white_man, FLD35,FLD44) && (all_empty((FLD45 |FLD50 | FLD40))))) && !((all_white((FLD35|FLD38))) && (empty_field & FLD33)))
#define MAX_BLACK_35  ((MAX_L_BLACK_40 || MAX_R_BLACK_40) && lim_gate(black_man, white_man, FLD34,FLD45) && (all_empty((FLD44 |FLD49 |FLD50 | FLD40 | FLD43))) && !((empty_field & FLD34) && (white_man & FLD39)))


#define MAX_WHITE_20   ((MAX_L_WHITE_15 &&                                                                  (all_empty((FLD10 |FLD5  |FLD4 | FLD15 | FLD9 | FLD3))))  || \
					   ((MAX_L_WHITE_14 || MAX_R_WHITE_14) && lim_gate(white_man, black_man, FLD19,FLD10) && (all_empty((FLD9  |FLD4  |FLD3 | FLD14 | FLD8)))))
#define MAX_WHITE_19  ((((MAX_L_WHITE_14 || MAX_R_WHITE_14) && lim_gate(white_man, black_man, FLD20, FLD9) && (all_empty((FLD10 |FLD5  |FLD4 | FLD14))))  || \
					   ((MAX_L_WHITE_13 || MAX_R_WHITE_13) && lim_gate(white_man, black_man, FLD18, FLD9) && (all_empty((FLD8  |FLD3  |FLD2 | FLD13 | FLD7))))) && !((all_black((FLD18|FLD15))) && (empty_field & FLD20) && !((all_black((FLD20|FLD12))) && (empty_field & FLD18))))
#define MAX_WHITE_18  ((((MAX_L_WHITE_13 || MAX_R_WHITE_13) && lim_gate(white_man, black_man, FLD19, FLD8) && (all_empty((FLD9  |FLD4  |FLD3 | FLD13 | FLD10))))  || \
					   ((MAX_L_WHITE_12 || MAX_R_WHITE_12) && lim_gate(white_man, black_man, FLD17, FLD8) && (all_empty((FLD7  |FLD2  |FLD1 | FLD12 | FLD6))))) && !((all_black((FLD17|FLD14))) && (empty_field & FLD19) && !((all_black((FLD19|FLD11))) && (empty_field & FLD17))))
#define MAX_WHITE_17  ((((MAX_L_WHITE_12 || MAX_R_WHITE_12) && lim_gate(white_man, black_man, FLD18, FLD7) && (all_empty((FLD8  |FLD3  |FLD4 | FLD12 | FLD9))))  || \
					   ((MAX_L_WHITE_11 || MAX_R_WHITE_11) && lim_gate(white_man, black_man, FLD16, FLD7) && (all_empty((FLD6  |FLD1 | FLD11))))) && !((all_black((FLD16|FLD13))) && (empty_field & FLD18)))
#define MAX_WHITE_16   ((MAX_L_WHITE_11 || MAX_R_WHITE_11) && lim_gate(white_man, black_man, FLD17, FLD6) && (all_empty((FLD7  |FLD2  |FLD1 | FLD11 | FLD8))) && !((empty_field & FLD17) && (black_man & FLD12)))


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
#define BR35_A all_bits_set(empty_field, (FLD40 | FLD44 | FLD45 | FLD50 | FLD34 | FLD35 | FLD39 | FLD49))

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
#define WR35_A all_bits_set(empty_field, (FLD11 | FLD7  | FLD6  | FLD1  | FLD17 | FLD16 | FLD12 | FLD2))

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

#define TB26 (all_empty(b9(24,20,14,15,9,10,3,4,5)))
#define TB27 (BR27_A || BR27_B)
#define TB28 (BR28_A || BR28_B)
#define TB29 (BR29_A || BR29_B)
#define TB30 (BR30_A || BR30_B)

#define TW25 (all_empty(w9(24,20,14,15,9,10,3,4,5)))
#define TW24 (WR24_A || WR24_B)
#define TW23 (WR23_A || WR23_B)
#define TW22 (WR22_A || WR22_B)
#define TW21 (WR21_A || WR21_B)


 BitArray nr_of_white_on_move_free_path(BitArray white_man, BitArray black_man, BitArray empty_field)
{	BitArray cnt = 0;
	if ((white_man & FLD6) &&  (FLD1 & empty_field)) cnt = (cnt | FLD6);
	if ((white_man & FLD7) &&  ((FLD1|FLD2) & empty_field)) cnt = (cnt | FLD7);
	if ((white_man & FLD8) &&  ((FLD2|FLD3) & empty_field)) cnt = (cnt | FLD8);
	if ((white_man & FLD9) &&  ((FLD3|FLD4) & empty_field)) cnt = (cnt | FLD9);
	if ((white_man & FLD10) &&  ((FLD4|FLD5) & empty_field)) cnt = (cnt | FLD10);
	if ((white_man & FLD15) &&  MIN_L_WHITE_15) cnt = (cnt | FLD15);
	if ((white_man & FLD14) && (MIN_L_WHITE_14 || MIN_R_WHITE_14)) cnt = (cnt | FLD14);
	if ((white_man & FLD13) && (MIN_L_WHITE_13 || MIN_R_WHITE_13)) cnt = (cnt | FLD13);
	if ((white_man & FLD12) && (MIN_L_WHITE_12 || MIN_R_WHITE_12)) cnt = (cnt | FLD12);
	if ((white_man & FLD11) && (MIN_L_WHITE_11 || MIN_R_WHITE_11)) cnt = (cnt | FLD11);
	if ((white_man & FLD20) &&  MIN_WHITE_20) cnt = (cnt | FLD20);
	if ((white_man & FLD19) &&  MIN_WHITE_19) cnt = (cnt | FLD19);
	if ((white_man & FLD18) &&  MIN_WHITE_18) cnt = (cnt | FLD18);
	if ((white_man & FLD17) &&  MIN_WHITE_17) cnt = (cnt | FLD17);
	if ((white_man & FLD16) &&  MIN_WHITE_16) cnt = (cnt | FLD16);
	if ((white_man & FLD25) &&  TW25) cnt = (cnt | FLD25);
	if ((white_man & FLD24) &&  TW24) cnt = (cnt | FLD24);
	if ((white_man & FLD23) &&  TW23) cnt = (cnt | FLD23);
	if ((white_man & FLD22) &&  TW22) cnt = (cnt | FLD22);
	if ((white_man & FLD21) &&  TW21) cnt = (cnt | FLD21);
	if ((white_man & FLD26) && all_bits_set(empty_field, (FLD27|FLD22|FLD21|FLD16|FLD17|FLD11|FLD12|FLD6|FLD7|FLD8|FLD1|FLD2|FLD3))) cnt = (cnt | FLD26);
	if ((all_white((FLD35|FLD30))) && (all_black((FLD45|FLD29))) && (all_empty((FLD34|FLD39|FLD40|FLD43|FLD44|FLD45|FLD49|FLD50)))) cnt = (cnt | FLD35);
	if ((white_man & w1(30)) && all_empty(w12(3,4,5,9,10,13,14,15,19,20,24,25))) cnt |= w1(30);
	if (all_white(w2(16,17)) && all_empty(w3(11,6,1))) cnt |= w1(17);
	return cnt;
}

 BitArray nr_of_black_on_move_free_path(BitArray white_man, BitArray black_man, BitArray empty_field)
{	BitArray cnt = 0;
	if ((black_man & FLD45) &&  ((FLD50) & empty_field)) cnt = (cnt | FLD45);
	if ((black_man & FLD44) &&  ((FLD49|FLD50) & empty_field)) cnt = (cnt | FLD44);
	if ((black_man & FLD43) &&  ((FLD48|FLD49) & empty_field)) cnt = (cnt | FLD43);
	if ((black_man & FLD42) &&  ((FLD47|FLD48) & empty_field)) cnt = (cnt | FLD42);
	if ((black_man & FLD41) &&  ((FLD46|FLD47) & empty_field)) cnt = (cnt | FLD41);
	if ((black_man & FLD36) &&  MIN_L_BLACK_36) cnt = (cnt | FLD36);
	if ((black_man & FLD37) && (MIN_L_BLACK_37 || MIN_R_BLACK_37)) cnt = (cnt | FLD37);
	if ((black_man & FLD38) && (MIN_L_BLACK_38 || MIN_R_BLACK_38)) cnt = (cnt | FLD38);
	if ((black_man & FLD39) && (MIN_L_BLACK_39 || MIN_R_BLACK_39)) cnt = (cnt | FLD39);
	if ((black_man & FLD40) && (MIN_L_BLACK_40 || MIN_R_BLACK_40)) cnt = (cnt | FLD40);
	if ((black_man & FLD31) &&  MIN_BLACK_31) cnt = (cnt | FLD31);
	if ((black_man & FLD32) &&  MIN_BLACK_32) cnt = (cnt | FLD32);
	if ((black_man & FLD33) &&  MIN_BLACK_33) cnt = (cnt | FLD33);
	if ((black_man & FLD34) &&  MIN_BLACK_34) cnt = (cnt | FLD34);
	if ((black_man & FLD35) &&  MIN_BLACK_35) cnt = (cnt | FLD35);
	if ((black_man & FLD26) &&  TB26) cnt = (cnt | FLD26);
	if ((black_man & FLD27) &&  TB27) cnt = (cnt | FLD27);
	if ((black_man & FLD28) &&  TB28) cnt = (cnt | FLD28);
	if ((black_man & FLD29) &&  TB29) cnt = (cnt | FLD29);
	if ((black_man & FLD30) &&  TB30) cnt = (cnt | FLD30);
	if ((black_man & FLD25) && all_bits_set(empty_field, (FLD24|FLD29|FLD30|FLD35|FLD34|FLD40|FLD39|FLD45|FLD44|FLD43|FLD50|FLD49|FLD48))) cnt = (cnt | FLD25);
	if ((all_black((BLD35|BLD30))) && (all_white((BLD45|BLD29))) && (all_empty((BLD34|BLD39|BLD40|BLD43|BLD44|BLD45|BLD49|BLD50)))) cnt = (cnt | BLD35);
	if ((black_man & b1(30)) && all_empty(b12(3,4,5,9,10,13,14,15,19,20,24,25))) cnt |= b1(30);
	if (all_black(b2(16,17)) && all_empty(b3(11,6,1))) cnt |= b1(17);
	return cnt;
}

 BitArray nr_of_white_not_on_move_free_path(BitArray white_man, BitArray black_man, BitArray empty_field)
{	BitArray cnt = 0;
	if ((white_man & FLD6) &&  (FLD1 & empty_field)) cnt = (cnt | FLD6);
	if ((white_man & FLD7) &&  ((FLD1|FLD2) & empty_field)) cnt = (cnt | FLD7);
	if ((white_man & FLD8) &&  ((FLD2|FLD3) & empty_field)) cnt = (cnt | FLD8);
	if ((white_man & FLD9) &&  ((FLD3|FLD4) & empty_field)) cnt = (cnt | FLD9);
	if ((white_man & FLD10) &&  ((FLD4|FLD5) & empty_field)) cnt = (cnt | FLD10);
	if ((white_man & FLD15) &&  MAX_L_WHITE_15) cnt = (cnt | FLD15);
	if ((white_man & FLD14) && (MAX_L_WHITE_14 || MAX_R_WHITE_14)) cnt = (cnt | FLD14);
	if ((white_man & FLD13) && (MAX_L_WHITE_13 || MAX_R_WHITE_13)) cnt = (cnt | FLD13);
	if ((white_man & FLD12) && (MAX_L_WHITE_12 || MAX_R_WHITE_12)) cnt = (cnt | FLD12);
	if ((white_man & FLD11) && (MAX_L_WHITE_11 || MAX_R_WHITE_11)) cnt = (cnt | FLD11);
	if ((white_man & FLD20) && (MAX_WHITE_20)) cnt = (cnt | FLD20);
	if ((white_man & FLD19) && (MAX_WHITE_19)) cnt = (cnt | FLD19);
	if ((white_man & FLD18) && (MAX_WHITE_18)) cnt = (cnt | FLD18);
	if ((white_man & FLD17) && (MAX_WHITE_17)) cnt = (cnt | FLD17);
	if ((white_man & FLD16) && (MAX_WHITE_16)) cnt = (cnt | FLD16);
	if ((white_man & w1(21)) && (all_empty(w9(1,2,6,7,11,12,16,17,22))) && (!((all_black(w3(32,33,39))) || (all_black(w3(32,38,43))))))cnt = (cnt | w1(21));
	if ((white_man & w1(22)) && (all_empty(w12(1,2,3,6,7,8,11,12,16,17,18,21))) && (!((all_black(w3(4,9,13))) || (all_black(w3(24,19,13))))))cnt = (cnt | w1(22));
	if ((white_man & w1(22)) && (all_empty(w15(1,2,3,4,7,8, 9,10,12,13,14,17,18,19,23))) && (!((all_black(w2(6,11))) || (all_black(w2(16,11))))))cnt = (cnt | w1(22));
	if ((white_man & w1(22)) && (all_empty(w14(1,2,3,7,8, 9,10,12,13,14,17,18,19,23))) && (white_man & w1(15)) && (black_man & w1(4)) && (!((all_black(w2(6,11))) || (all_black(w2(16,11))))))cnt = (cnt | w1(22));
	if ((white_man & w1(23)) && (all_empty(w14(2,3,4,5,8,9,10,13,14,15,18,19,20,24))) && (!((all_black(w3(1,7,12))) || (all_black(w3(21,17,12))))))cnt = (cnt | w1(23));
	if ((white_man & w1(23)) && (all_empty(w12(2,3,5,8,9,10,13,14,18,19,20,24))) && (white_man & w1(15)) && (black_man & w1(4)) && (!((all_black(w3(1,7,12))) || (all_black(w3(21,17,12))))))cnt = (cnt | w1(23));
	if ((white_man & w1(23)) && (all_empty(w15(1,2,3,4,6,7, 8, 9,11,12,13,17,18,19,22))) && (!((all_black(w3(5,10,14))) || (all_black(w3(25,20,15))))))cnt = (cnt | w1(23));
	if ((white_man & w1(24)) && (all_empty(w10(25,19,29,14,15,9,10,3,4,5))) && (!((all_black(w3(2,8,13))) || (all_black(w3(22,18,13)))))) cnt = (cnt | w1(24));
	if ((white_man & FLD25) && all_bits_set(empty_field, (FLD24|FLD20|FLD14|FLD15|FLD9|FLD10|FLD3|FLD4|FLD5))) cnt = (cnt | FLD25);
	if ((white_man & FLD21) && all_bits_set(empty_field, (FLD22|FLD16|FLD17|FLD11|FLD12|FLD6|FLD7|FLD8|FLD1|FLD2|FLD3))) cnt = (cnt | FLD21);
	if ((white_man & FLD26) && all_bits_set(empty_field, (FLD27|FLD22|FLD21|FLD18|FLD16|FLD17|FLD11|FLD12|FLD6|FLD7|FLD8|FLD1|FLD2|FLD3))) cnt = (cnt | FLD26);
	if ((all_white((FLD35|FLD30))) && (all_black((FLD45|FLD29))) && (all_empty((FLD34|FLD39|FLD40|FLD43|FLD44|FLD45|FLD49|FLD50)))) cnt = (cnt | FLD35);
	if ((white_man & w1(30)) && all_empty(w14(3,4,5,8,9,10,13,14,15,19,20,24,25,29))) cnt |= w1(30);
	if (all_white(w2(16,17)) && all_empty(w4(11,7,6,1))) cnt |= w1(17);
	return cnt;
}

 BitArray nr_of_black_not_on_move_free_path(BitArray white_man, BitArray black_man, BitArray empty_field)
{	BitArray cnt = 0;
	if ((black_man & FLD45) &&  ((FLD50) & empty_field)) cnt = (cnt | FLD45);
	if ((black_man & FLD44) &&  ((FLD49|FLD50) & empty_field)) cnt = (cnt | FLD44);
	if ((black_man & FLD43) &&  ((FLD48|FLD49) & empty_field)) cnt = (cnt | FLD43);
	if ((black_man & FLD42) &&  ((FLD47|FLD48) & empty_field)) cnt = (cnt | FLD42);
	if ((black_man & FLD41) &&  ((FLD46|FLD47) & empty_field)) cnt = (cnt | FLD41);
	if ((black_man & FLD40) && (MAX_L_BLACK_40 || MAX_R_BLACK_40)) cnt = (cnt | FLD40);
	if ((black_man & FLD39) && (MAX_L_BLACK_39 || MAX_R_BLACK_39)) cnt = (cnt | FLD39);
	if ((black_man & FLD38) && (MAX_L_BLACK_38 || MAX_R_BLACK_38)) cnt = (cnt | FLD38);
	if ((black_man & FLD37) && (MAX_L_BLACK_37 || MAX_R_BLACK_37)) cnt = (cnt | FLD37);
	if ((black_man & FLD36) && (MAX_L_BLACK_36                  )) cnt = (cnt | FLD36);
	if ((black_man & FLD31) && (MAX_BLACK_31))cnt = (cnt | FLD31);
	if ((black_man & FLD32) && (MAX_BLACK_32))cnt = (cnt | FLD32);
	if ((black_man & FLD33) && (MAX_BLACK_33))cnt = (cnt | FLD33);
	if ((black_man & FLD34) && (MAX_BLACK_34)) cnt = (cnt | FLD34);
	if ((black_man & FLD35) && (MAX_BLACK_35)) cnt = (cnt | FLD35);
	if ((black_man & b1(21)) && (all_empty(b9(1,2,6,7,11,12,16,17,22))) && (!((all_white(b3(32,33,39))) || (all_white(b3(32,38,43))))))cnt = (cnt | b1(21));
	if ((black_man & b1(22)) && (all_empty(b12(1,2,3,6,7,8,11,12,16,17,18,21))) && (!((all_white(b3(4,9,13))) || (all_white(b3(24,19,13))))))cnt = (cnt | b1(22));
	if ((black_man & b1(22)) && (all_empty(b15(1,2,3,4,7,8, 9,10,12,13,14,17,18,19,23))) && (!((all_white(b2(6,11))) || (all_white(b2(16,11))))))cnt = (cnt | b1(22));
	if ((black_man & b1(22)) && (all_empty(b14(1,2,3,7,8, 9,10,12,13,14,17,18,19,23))) && (black_man & b1(15)) && (white_man & b1(4)) && (!((all_white(b2(6,11))) || (all_white(b2(16,11))))))cnt = (cnt | b1(22));
	if ((black_man & b1(23)) && (all_empty(b14(2,3,4,5,8,9,10,13,14,15,18,19,20,24))) && (!((all_white(b3(1,7,12))) || (all_white(b3(21,17,12))))))cnt = (cnt | b1(23));
	if ((black_man & b1(23)) && (all_empty(b12(2,3,5,8,9,10,13,14,18,19,20,24))) && (black_man & b1(15)) && (white_man & b1(4)) && (!((all_white(b3(1,7,12))) || (all_white(b3(21,17,12))))))cnt = (cnt | b1(23));
	if ((black_man & b1(23)) && (all_empty(b15(1,2,3,4,6,7, 8, 9,11,12,13,17,18,19,22))) && (!((all_white(b3(5,10,14))) || (all_white(b3(25,20,15))))))cnt = (cnt | b1(23));
	if ((black_man & b1(24)) && (all_empty(b10(25,19,29,14,15,9,10,3,4,5))) && (!((all_white(b3(2,8,13))) || (all_white(b3(22,18,13)))))) cnt = (cnt | b1(24));
	if ((black_man & FLD26) && all_bits_set(empty_field, (FLD27|FLD31|FLD37|FLD36|FLD42|FLD41|FLD48|FLD47|FLD46))) cnt = (cnt | FLD26);
	if ((black_man & FLD30) && all_bits_set(empty_field, (FLD29|FLD35|FLD34|FLD40|FLD39|FLD45|FLD44|FLD43|FLD50|FLD49|FLD48))) cnt = (cnt | FLD30);
	if ((black_man & FLD25) && all_bits_set(empty_field, (FLD24|FLD29|FLD30|FLD33|FLD35|FLD34|FLD40|FLD39|FLD45|FLD44|FLD43|FLD50|FLD49|FLD48))) cnt = (cnt | FLD25);
	if ((all_black((BLD35|BLD30))) && (all_white((BLD45|BLD29))) && (all_empty((BLD34|BLD39|BLD40|BLD43|BLD44|BLD45|BLD49|BLD50)))) cnt = (cnt | BLD35);
	if ((black_man & b1(30)) && all_empty(b14(3,4,5,8,9,10,13,14,15,19,20,24,25,29))) cnt |= b1(30);
	if (all_black(b2(16,17)) && all_empty(b4(11,7,6,1))) cnt |= b1(17);

	return cnt;
}
