/*
  This file is Copyright (C) 2010 Feike Boomstra.
  Distributed under the Boost Software License, Version 1.0.
  (See accompanying file LICENSE_1_0.txt or copy at
  http://www.boost.org/LICENSE_1_0.txt)
*/




#define R3_ROW  (BIT1  | BIT2  | BIT3  | BIT4  | BIT5  | BIT6  | BIT7  | BIT8  | BIT9  | BIT10 | BIT11 | BIT12 | BIT13 | \
	            BIT31 | BIT32 | BIT33 | BIT34 | BIT35 | BIT36 | BIT37 | BIT38 | BIT39 | BIT40 | BIT41 | BIT42)
#define R4_ROW  (BIT14 | BIT15 | BIT16 | BIT17 | BIT18 | BIT19 | BIT20 | BIT21 |BIT43 | BIT44 | BIT45 | BIT46 | BIT47 | BIT48 | BIT49 | BIT50)
#define R5_ROW  (BIT22 | BIT23 | BIT24 | BIT25 | BIT26 | BIT51 | BIT52 | BIT53 | BIT54)
#define R6_ROW  (BIT27 |BIT55)

// stuff for pieces distribution

#define	COL1 (FLD6 | FLD16 | FLD26 | FLD36 | FLD46)
#define	COL2 (FLD1 | FLD11 | FLD21 | FLD31 | FLD41)
#define	COL3 (FLD7 | FLD17 | FLD27 | FLD37 | FLD47)
#define	COL4 (FLD2 | FLD12 | FLD22 | FLD32 | FLD42)
#define	COL5 (FLD8 | FLD18 | FLD28 | FLD38 | FLD48)
#define	COL6 (FLD3 | FLD13 | FLD23 | FLD33 | FLD43)
#define	COL7 (FLD9 | FLD19 | FLD29 | FLD39 | FLD49)
#define	COL8 (FLD4 | FLD14 | FLD24 | FLD34 | FLD44)
#define	COL9 (FLD10 | FLD20 | FLD30 | FLD40 | FLD50)
#define	COL10 (FLD5 | FLD15 | FLD25 | FLD35 | FLD45)

#define W2_1_5 (COL1 | COL2 | COL3 | COL4 | COL5)
#define W2_2_5 (COL6 | COL7 | COL8 | COL9 | COL10)

#define W3_1_3 (COL1 | COL2 | COL3)
#define W3_2_4 (COL4 | COL5 | COL6 | COL7)
#define W3_3_3 (COL8 | COL9 | COL10)

#define W4_1_2 (COL1 | COL2)
#define W4_2_3 (COL3 | COL4 | COL5)
#define W4_3_3 (COL6 | COL7 | COL8)
#define W4_4_2 (COL9 | COL10)

#define W5_1_2 (COL1 | COL2)
#define W5_2_2 (COL3 | COL4)
#define W5_3_2 (COL5 | COL6)
#define W5_4_2 (COL7 | COL8)
#define W5_5_2 (COL9 | COL10)

// special cases

#define TRIANGLE_11 (FLD11 | FLD16 | FLD17)
#define TRIANGLE_40 (FLD40 | FLD34 | FLD35)


// ketting stelling

#define WHITE_BLACK_CHAIN1 ( FLD12 | FLD17 | FLD18 | FLD22)
#define WHITE_BLACK_CHAIN1_2 ( FLD17 | FLD18 | FLD22)
#define WHITE_WHITE_CHAIN1 ( FLD27 | FLD31 | FLD36 | FLD28 | FLD33 | FLD39)
#define WHITE_BLACK_CHAIN2 ( FLD13 | FLD18 | FLD19 | FLD23)
#define WHITE_BLACK_CHAIN2_2 ( FLD18 | FLD19 | FLD23)
#define WHITE_WHITE_CHAIN2 ( FLD28 | FLD32 | FLD37 | FLD29 | FLD34 | FLD40)
#define WHITE_BLACK_CHAIN3 ( FLD18 | FLD22 | FLD23 | FLD28)
#define WHITE_BLACK_CHAIN3_2 ( FLD22 | FLD23 | FLD28)
#define WHITE_WHITE_CHAIN3 ( FLD32 | FLD37 | FLD41 | FLD33 | FLD39 | FLD44)
#define WHITE_BLACK_CHAIN4 ( FLD19 | FLD23 | FLD24 | FLD29)
#define WHITE_BLACK_CHAIN4_2 ( FLD23 | FLD24 | FLD29)
#define WHITE_WHITE_CHAIN4 ( FLD33 | FLD38 | FLD42 | FLD34 | FLD40 | FLD45)
#define BLACK_WHITE_CHAIN1 ( FLD29 | FLD33 | FLD34 | FLD39)
#define BLACK_WHITE_CHAIN1_2 ( FLD29 | FLD33 | FLD34)
#define BLACK_BLACK_CHAIN1 ( FLD24 | FLD20 | FLD15 | FLD23 | FLD18 | FLD12)
#define BLACK_WHITE_CHAIN2 ( FLD28 | FLD32 | FLD33 | FLD38)
#define BLACK_WHITE_CHAIN2_2 ( FLD28 | FLD32 | FLD33)
#define BLACK_BLACK_CHAIN2 ( FLD23 | FLD19 | FLD14 | FLD22 | FLD17 | FLD11)
#define BLACK_WHITE_CHAIN3 ( FLD22 | FLD27 | FLD28 | FLD32)
#define BLACK_WHITE_CHAIN3_2 ( FLD22 | FLD27 | FLD28)
#define BLACK_BLACK_CHAIN3 ( FLD18 | FLD13 | FLD9  | FLD17 | FLD11 | FLD6)
#define BLACK_WHITE_CHAIN4 ( FLD23 | FLD28 | FLD29 | FLD33)
#define BLACK_WHITE_CHAIN4_2 ( FLD23 | FLD28 | FLD29)
#define BLACK_BLACK_CHAIN4 ( FLD19 | FLD14 | FLD10 | FLD18 | FLD12 | FLD7)

// halve hek

#define WHITE_WHITE_HALVE_HEK (FLD36 | FLD31 | FLD27)
#define WHITE_BLACK_HALVE_HEK (FLD16 | FLD18 | FLD22)
#define BLACK_BLACK_HALVE_HEK (FLD15 | FLD20 | FLD24)
#define BLACK_WHITE_HALVE_HEK (FLD35 | FLD33 | FLD29)

// distributie

#define LINE1 (FLD1 | FLD2 | FLD3 | FLD4 | FLD5)
#define LINE2 (FLD6 | FLD7 | FLD8 | FLD9 | FLD10)
#define LINE3 (FLD11 | FLD12 | FLD13 | FLD14 | FLD15)
#define LINE4 (FLD16 | FLD17 | FLD18 | FLD19 | FLD20)
#define LINE5 (FLD21 | FLD22 | FLD23 | FLD24 | FLD25)
#define LINE6 (FLD26 | FLD27 | FLD28 | FLD29 | FLD30)
#define LINE7 (FLD31 | FLD32 | FLD33 | FLD34 | FLD35)
#define LINE8 (FLD36 | FLD37 | FLD38 | FLD39 | FLD40)
#define LINE9 (FLD41 | FLD42 | FLD43 | FLD44 | FLD45)
#define LINE10 (FLD46 | FLD47 | FLD48 | FLD49 | FLD50)

#define LINE01_05 (LINE1|LINE2|LINE3|LINE5|LINE5)
#define LINE01_05_R (LINE1|LINE2|LINE3|LINE4|FLD21 | FLD22 | FLD23 | FLD25)
#define LINE06_10 (LINE6|LINE7|LINE8|LINE9|LINE10)
#define LINE06_10_R (LINE10|LINE7|LINE8|LINE9|FLD26 | FLD28 | FLD29 | FLD30)

#define CENTRUM (FLD22|FLD23|FLD24|FLD27|FLD28|FLD29)

#define KLASSIEK_WHITE (FLD27 | FLD28 | FLD32)
#define KLASSIEK_BLACK (FLD23 | FLD24 | FLD19)

#define WHITE_CROWN FLD48
#define BLACK_CROWN FLD3

#define WHITE_OLYMPIC (FLD40 | FLD45 )
#define BLACK_OLYMPIC (FLD11 | FLD6)

// to calculate locked men by a king

#define L16NW (FLD16  | FLD11 | FLD7 | FLD2)
#define L26NW (FLD26 | FLD21 | FLD17 | FLD12 | FLD8 | FLD3)
#define L36NW (FLD36 | FLD31 | FLD27 | FLD22 | FLD18 | FLD13 | FLD9 | FLD4)
#define L46NW (FLD46 | FLD41 | FLD37 | FLD32 | FLD28 | FLD23 | FLD19 | FLD14 | FLD10 | FLD5)
#define L16NW_ (FLD11 | FLD7)
#define L26NW_ (FLD21 | FLD17 | FLD12 | FLD8)
#define L36NW_ (FLD31 | FLD27 | FLD22 | FLD18 | FLD13 | FLD9)
#define L46NW_ (FLD41 | FLD37 | FLD32 | FLD28 | FLD23 | FLD19 | FLD14 | FLD10)

#define L5SE L46NW
#define L15SE (FLD15 | FLD20 | FLD24 | FLD29 | FLD33 | FLD38 | FLD42 | FLD47)
#define L25SE (FLD25 | FLD30 | FLD34 | FLD39 | FLD43 | FLD48)
#define L35SE (FLD35 | FLD40 | FLD44 | FLD49)
#define L5SE_ L46NW_
#define L15SE_ (FLD20 | FLD24 | FLD29 | FLD33 | FLD38 | FLD42)
#define L25SE_ (FLD30 | FLD34 | FLD39 | FLD43)
#define L35SE_ (FLD40 | FLD44)

#define L4SW (FLD4 | FLD10 | FLD15)
#define L3SW (FLD3 | FLD9 | FLD14 | FLD20 | FLD25)
#define L2SW (FLD2 | FLD8 | FLD13 | FLD19 | FLD24 | FLD30 | FLD35)
#define L1SW (FLD1 | FLD7 | FLD12 | FLD18 | FLD23 | FLD29 | FLD34 | FLD40 | FLD45)
#define L4SW_ (FLD10)
#define L3SW_ (FLD9 | FLD14 | FLD20)
#define L2SW_ (FLD8 | FLD13 | FLD19 | FLD24 | FLD30)
#define L1SW_ (FLD7 | FLD12 | FLD18 | FLD23 | FLD29 | FLD34 | FLD40)

#define L6SW (FLD6 | FLD11 | FLD17 | FLD22 | FLD28 | FLD33 | FLD39 | FLD44 | FLD50)
#define L16SW (FLD16 | FLD21 | FLD27 | FLD32 | FLD38 | FLD43 | FLD49)
#define L26SW (FLD26 | FLD31 | FLD37 | FLD42 | FLD48)
#define L36SW (FLD36 | FLD41 | FLD47)
#define L6SW_ (FLD11 | FLD17 | FLD22 | FLD28 | FLD33 | FLD39 | FLD44)
#define L16SW_ (FLD21 | FLD27 | FLD32 | FLD38 | FLD43)
#define L26SW_ (FLD31 | FLD37 | FLD42)
#define L36SW_ (FLD41)

#define B16NW (FLD1 | FLD6)
#define B26NW (L16NW | B16NW)
#define B36NW (L26NW | B26NW)
#define B46NW (L36NW | B36NW)

#define B4SW (FLD5)
#define B3SW (L4SW | B4SW)
#define B2SW (L3SW | B3SW)
#define B1SW (L2SW | B2SW)

#define B36SW (FLD46)
#define B26SW (L36SW | B36SW)
#define B16SW (L26SW | B26SW)
#define B6SW (L16SW | B16SW)

#define B35SE (FLD45 | FLD50)
#define B25SE (L35SE | B35SE)
#define B15SE (L25SE | B25SE)
#define B5SE (L15SE | B15SE)

// special cases diagonal

#define DIAGLOW (FLD46 | FLD41 | FLD37 | FLD32 | FLD28 | FLD23 | FLD19 )
#define DIAGHIGH (FLD32 | FLD28 | FLD23 | FLD19 | FLD14 | FLD10 | FLD5)

#define WM_01_25 (FLD6 | FLD7 | FLD8 | FLD9 | FLD10 | FLD11 | FLD12 | FLD13 | FLD14 | FLD15 | \
                  FLD16 | FLD17 | FLD18 | FLD19 | FLD20 | FLD21 | FLD22 | FLD23 | FLD24 | FLD25)
#define BM_50_26 (FLD45 | FLD44 | FLD43 | FLD42 | FLD41 | FLD40 | FLD39 | FLD38 | FLD37 | FLD36 | \
                  FLD35 | FLD34 | FLD33 | FLD32 | FLD31 | FLD30 | FLD29 | FLD28 | FLD27 | FLD26)





// voorpost definitions

#define BV1_W12 FLD1
#define BV2_W12 FLD3
#define BV1_W13 FLD2
#define BV2_W13 FLD4
#define BV1_W14 FLD3
#define BV2_W14 FLD5
#define BV1_W17 FLD6
#define BV2_W17 FLD8
#define BV1_W18 FLD7
#define BV2_W18 FLD9
#define BV1_W19 FLD8
#define BV2_W19 FLD10
#define BV1_W22 FLD11
#define BV2_W22 FLD13
#define BV1_W23 FLD12
#define BV2_W23 FLD14
#define BV1_W24 FLD13
#define BV2_W24 FLD15

#define BV1_W2_12 FLD2
#define BV2_W2_12 FLD2
#define BV1_W2_13 FLD3
#define BV2_W2_13 FLD3
#define BV1_W2_14 FLD4
#define BV2_W2_14 FLD4
#define BV1_W2_17 (FLD1 | FLD2 | FLD7)
#define BV2_W2_17 (FLD1 | FLD2 | FLD7)
#define BV1_W2_18 (FLD2 | FLD3 | FLD8)
#define BV2_W2_18 (FLD2 | FLD3 | FLD8)
#define BV1_W2_19 (FLD3 | FLD4 | FLD9)
#define BV2_W2_19 (FLD3 | FLD4 | FLD9)
#define BV1_W2_22 (FLD12 | FLD7 | FLD8 | FLD1 | FLD2 | FLD3)
#define BV2_W2_22 (FLD12 | FLD7 | FLD8 | FLD1 | FLD2 | FLD3)
#define BV1_W2_23 (FLD13 | FLD8 | FLD9 | FLD2 | FLD3 | FLD4)
#define BV2_W2_23 (FLD13 | FLD8 | FLD9 | FLD2 | FLD3 | FLD4)
#define BV1_W2_24 (FLD14 | FLD9 | FLD10 | FLD3 | FLD4 | FLD5)
#define BV2_W2_24 (FLD14 | FLD9 | FLD10 | FLD3 | FLD4 | FLD5)

#define BV1_W3_12 (FLD18 | FLD23 )
#define BV2_W3_12 (FLD17 | FLD21 )
#define BV1_W3_13 (FLD19 | FLD24 )
#define BV2_W3_13 (FLD18 | FLD22 )
#define BV1_W3_14 (FLD20 | FLD25 )
#define BV2_W3_14 (FLD19 | FLD23 )
#define BV1_W3_17 (FLD22 | FLD28 )
#define BV2_W3_17 (FLD21 | FLD26 )
#define BV1_W3_18 (FLD23 | FLD29 )
#define BV2_W3_18 (FLD22 | FLD27 )
#define BV1_W3_19 (FLD24 | FLD30 )
#define BV2_W3_19 (FLD23 | FLD28 )
#define BV1_W3_22 (FLD28 | FLD33 )
#define BV2_W3_22 (FLD27 | FLD31 )
#define BV1_W3_23 (FLD29 | FLD34 )
#define BV2_W3_23 (FLD28 | FLD32 )
#define BV1_W3_24 (FLD30 | FLD35 )
#define BV2_W3_24 (FLD29 | FLD33 )

//if ((black_man & FLD39) && (white_man & WV1_W39) && (white_man & WV1_W2_39) && (all_bits_set(empty_field, WV1_W3_39))) black_cnt++;
//if ((black_man & FLD39) && (white_man & WV2_W39) && (white_man & WV2_W2_39) && (all_bits_set(empty_field, WV2_W3_39))) black_cnt++;

#define WV1_W39 FLD50
#define WV2_W39 FLD48
#define WV1_W38 FLD49
#define WV2_W38 FLD47
#define WV1_W37 FLD48
#define WV2_W37 FLD46
#define WV1_W34 FLD45
#define WV2_W34 FLD43
#define WV1_W33 FLD44
#define WV2_W33 FLD42
#define WV1_W32 FLD43
#define WV2_W32 FLD41
#define WV1_W29 FLD40
#define WV2_W29 FLD38
#define WV1_W28 FLD39
#define WV2_W28 FLD37
#define WV1_W27 FLD38
#define WV2_W27 FLD36

#define WV1_W2_39 FLD49
#define WV2_W2_39 FLD49
#define WV1_W2_38 FLD48
#define WV2_W2_38 FLD48
#define WV1_W2_37 FLD47
#define WV2_W2_37 FLD47
#define WV1_W2_34 (FLD50 | FLD49 | FLD44)
#define WV2_W2_34 (FLD50 | FLD49 | FLD44)
#define WV1_W2_33 (FLD49 | FLD48 | FLD43)
#define WV2_W2_33 (FLD49 | FLD48 | FLD43)
#define WV1_W2_32 (FLD48 | FLD47 | FLD42)
#define WV2_W2_32 (FLD48 | FLD47 | FLD42)
#define WV1_W2_29 (FLD39 | FLD43 | FLD44 | FLD48 | FLD49 | FLD50)
#define WV2_W2_29 (FLD39 | FLD43 | FLD44 | FLD48 | FLD49 | FLD50)
#define WV1_W2_28 (FLD38 | FLD42 | FLD43 | FLD47 | FLD48 | FLD49)
#define WV2_W2_28 (FLD38 | FLD42 | FLD43 | FLD47 | FLD48 | FLD49)
#define WV1_W2_27 (FLD37 | FLD41 | FLD42 | FLD46 | FLD47 | FLD48)
#define WV2_W2_27 (FLD37 | FLD41 | FLD42 | FLD46 | FLD47 | FLD48)

#define WV1_W3_39 (FLD33 | FLD28 )
#define WV2_W3_39 (FLD34 | FLD30 )
#define WV1_W3_38 (FLD32 | FLD27 )
#define WV2_W3_38 (FLD33 | FLD29 )
#define WV1_W3_37 (FLD31 | FLD26 )
#define WV2_W3_37 (FLD32 | FLD28 )
#define WV1_W3_34 (FLD29 | FLD23 )
#define WV2_W3_34 (FLD30 | FLD25 )
#define WV1_W3_33 (FLD28 | FLD22 )
#define WV2_W3_33 (FLD29 | FLD24 )
#define WV1_W3_32 (FLD27 | FLD21 )
#define WV2_W3_32 (FLD28 | FLD23 )
#define WV1_W3_29 (FLD23 | FLD18 )
#define WV2_W3_29 (FLD24 | FLD20 )
#define WV1_W3_28 (FLD22 | FLD17 )
#define WV2_W3_28 (FLD23 | FLD19 )
#define WV1_W3_27 (FLD21 | FLD16 )
#define WV2_W3_27 (FLD22 | FLD18 )

// very simple locked pieces

#define BL_W12_L (FLD1 | FLD11)
#define BL_W12_R (FLD3 | FLD13)
#define BL_W13_L (FLD2 | FLD12)
#define BL_W13_R (FLD4 | FLD14)
#define BL_W14_L (FLD3 | FLD13)
#define BL_W14_R (FLD5 | FLD15)
#define BL_W17_L (FLD6 | FLD16)
#define BL_W17_R (FLD8 | FLD18)
#define BL_W18_L (FLD7 | FLD17)
#define BL_W18_R (FLD9 | FLD19)
#define BL_W19_L (FLD8 | FLD18)
#define BL_W19_R (FLD10 | FLD20)

#define WL_W39_L (FLD50 | FLD40)
#define WL_W39_R (FLD48 | FLD38)
#define WL_W38_L (FLD49 | FLD39)
#define WL_W38_R (FLD47 | FLD37)
#define WL_W37_L (FLD48 | FLD38)
#define WL_W37_R (FLD46 | FLD36)
#define WL_W34_L (FLD45 | FLD35)
#define WL_W34_R (FLD43 | FLD33)
#define WL_W33_L (FLD44 | FLD34)
#define WL_W33_R (FLD42 | FLD32)
#define WL_W32_L (FLD43 | FLD33)
#define WL_W32_R (FLD41 | FLD31)

#define BL_W3_12 (FLD2 | FLD7 | FLD8)
#define BL_W3_13 (FLD3 | FLD8 | FLD9)
#define BL_W3_14 (FLD4 | FLD9 | FLD10)
#define BL_W3_17 (FLD1 | FLD2 | FLD3 | FLD7 | FLD11 | FLD12)
#define BL_W3_18 (FLD1 | FLD2 | FLD3 | FLD4 | FLD8 | FLD12 | FLD13)
#define BL_W3_19 (FLD2 | FLD3 | FLD4 | FLD5 | FLD9 | FLD13 | FLD14)

#define WL_W3_39 (FLD49 | FLD44 | FLD43)
#define WL_W3_38 (FLD48 | FLD43 | FLD42)
#define WL_W3_37 (FLD47 | FLD42 | FLD41)
#define WL_W3_34 (FLD50 | FLD49 | FLD48 | FLD44 | FLD40 | FLD39)
#define WL_W3_33 (FLD50 | FLD49 | FLD48 | FLD47 | FLD43 | FLD39 | FLD38)
#define WL_W3_32 (FLD49 | FLD48 | FLD47 | FLD46 | FLD42 | FLD38 | FLD37)



// vleugelopsluitingen

// white locked pieces
#define OW1_1 (FLD26 | FLD27 | FLD31 | FLD32 | FLD33)
#define OW1_2 (FLD21 | FLD26 | FLD27 | FLD31 | FLD33)
#define OW1_3 (FLD21 | FLD26 | FLD27 | FLD28)
#define OW1_4 (FLD26 |  FLD16 )
#define OW1_5 (FLD16 | FLD22)
#define OW1_6 (FLD36 | FLD37 | FLD41 | FLD42 | FLD43)
#define OW1_7 (FLD31 | FLD36 | FLD37 | FLD41 | FLD43)
#define OW1_8 (FLD31 | FLD36 | FLD37 | FLD38)
#define OW1_9 (FLD36 | FLD26 )
#define OW1_10 (FLD26 | FLD32)
#define OW1_11 (FLD41 | FLD46 | FLD47 | FLD48)
#define OW1_12 (FLD46 | FLD41 | FLD36 | FLD48)
#define OW1_13 (FLD36 | FLD42)
#define OW1_14 (FLD24 | FLD25 | FLD28 | FLD29 | FLD30)
#define OW1_15 (FLD20 | FLD24 | FLD25 | FLD28 | FLD30)
#define OW1_16 (FLD20 | FLD24 | FLD25 | FLD23)
#define OW1_17 (FLD15 | FLD25)
#define OW1_18 (FLD15 | FLD19)
#define OW1_19 (FLD34 | FLD35 | FLD38 | FLD39 | FLD40)
#define OW1_20 (FLD30 | FLD34 | FLD35 | FLD38 | FLD40)
#define OW1_21 (FLD30 | FLD34 | FLD35 | FLD33)
#define OW1_22 (FLD25 | FLD35 )
#define OW1_23 (FLD25 | FLD29)
#define OW1_24 (FLD44 | FLD48 | FLD49 | FLD50)
#define OW1_25 (FLD40 | FLD44 | FLD45 | FLD48 | FLD50)
#define OW1_26 (FLD40 | FLD44 | FLD45 | FLD43)
#define OW1_27 (FLD35 | FLD45 )
#define OW1_28 (FLD35 | FLD39)
#define OW1_29 (FLD45 | FLD49)
#define OW2_1 (FLD22 | FLD18)
#define OW2_2 (FLD22 | FLD18)
#define OW2_3 (FLD17 | FLD12)
#define OW2_4 (FLD17 | FLD12)
#define OW2_5 (FLD11 | FLD7)
#define OW2_6 (FLD32 | FLD28)
#define OW2_7 (FLD32 | FLD28)
#define OW2_8 (FLD27 | FLD22)
#define OW2_9 (FLD27 | FLD22)
#define OW2_10 (FLD21 | FLD17)
#define OW2_11 (FLD37 | FLD32)
#define OW2_12 (FLD37 | FLD32)
#define OW2_13 (FLD31 | FLD27)
#define OW2_14 (FLD19 | FLD13)
#define OW2_15 (FLD19 | FLD13)
#define OW2_16 (FLD14 | FLD9)
#define OW2_17 (FLD14 | FLD9)
#define OW2_18 (FLD10 | FLD4)
#define OW2_19 (FLD29 | FLD23)
#define OW2_20 (FLD29 | FLD23)
#define OW2_21 (FLD24 | FLD19)
#define OW2_22 (FLD24 | FLD19)
#define OW2_23 (FLD20 | FLD14)
#define OW2_24 (FLD39 | FLD33)
#define OW2_25 (FLD39 | FLD33)
#define OW2_26 (FLD34 | FLD29)
#define OW2_27 (FLD34 | FLD29)
#define OW2_28 (FLD30 | FLD24)
#define OW2_29 (FLD40 | FLD34)
#define OW3_1 (FLD1 | FLD6 | FLD7 | FLD11 | FLD12 | FLD16 | FLD17 | FLD21)
#define OW3_2 (FLD1 | FLD6 | FLD7 | FLD11 | FLD12 | FLD16 | FLD17)
#define OW3_3 (FLD1 | FLD6 | FLD7 | FLD11 | FLD16)
#define OW3_4 (FLD1 | FLD6 | FLD7 | FLD11)
#define OW3_5 (FLD1 | FLD6)
#define OW3_6 (FLD6 | FLD11 | FLD16 | FLD17 | FLD21 | FLD22 | FLD26 | FLD27 | FLD31)
#define OW3_7 (FLD6 | FLD11 | FLD16 | FLD17 | FLD21 | FLD22 | FLD26 | FLD27)
#define OW3_8 (FLD6 | FLD11 | FLD16 | FLD17 | FLD21 | FLD26)
#define OW3_9 (FLD6 | FLD11 | FLD16 | FLD17 | FLD21)
#define OW3_10 (FLD6 | FLD11 | FLD16)
#define OW3_11 (FLD16 | FLD21 | FLD26 | FLD27 | FLD31)
#define OW3_12 (FLD16 | FLD21 | FLD26 | FLD27 | FLD31)
#define OW3_13 (FLD16 | FLD21 | FLD26)
#define OW3_14 (FLD4 | FLD5 | FLD9 | FLD10 | FLD14 | FLD15)
#define OW3_15 (FLD4 | FLD5 | FLD9 | FLD10 | FLD14 | FLD15)
#define OW3_16 (FLD4 | FLD5 | FLD10)
#define OW3_17 (FLD4 | FLD5 | FLD10)
#define OW3_18 (FLD5)
#define OW3_19 (FLD5 | FLD10 | FLD14 | FLD15 | FLD19 | FLD20 | FLD24 | FLD25)
#define OW3_20 (FLD5 | FLD10 | FLD14 | FLD15 | FLD19 | FLD20 | FLD24 | FLD25)
#define OW3_21 (FLD5 | FLD10 | FLD14 | FLD15 | FLD20)
#define OW3_22 (FLD5 | FLD10 | FLD14 | FLD15 | FLD20)
#define OW3_23 (FLD5 | FLD10 | FLD15)
#define OW3_24 (FLD15 | FLD20 | FLD24 | FLD25 | FLD29 | FLD30 | FLD34 | FLD35 | FLD40 | FLD45)
#define OW3_25 (FLD15 | FLD20 | FLD24 | FLD25 | FLD29 | FLD30 | FLD34 | FLD35)
#define OW3_26 (FLD15 | FLD20 | FLD24 | FLD25 | FLD30)
#define OW3_27 (FLD15 | FLD20 | FLD24 | FLD25 | FLD30)
#define OW3_28 (FLD15 | FLD20 | FLD25)
#define OW3_29 (FLD25 | FLD30 | FLD35)
#define OW4_1 ( FLD28 |FLD16 |FLD21 |FLD36 |FLD46 |FLD41 |FLD33 |FLD47 |FLD42 |FLD38 )
#define OB4_1 ( FLD23 |FLD35 |FLD30 |FLD15 |FLD5 |FLD10 |FLD18 |FLD4 |FLD9 |FLD13 )
#define OW4_2 ( FLD28 |FLD16 |FLD36 |FLD46 |FLD41 |FLD33 |FLD47 |FLD42 |FLD38 )
#define OB4_2 ( FLD23 |FLD35 |FLD15 |FLD5 |FLD10 |FLD18 |FLD4 |FLD9 |FLD13 )
#define OW4_3 ( FLD22 |FLD16 |FLD36 |FLD31 |FLD46 |FLD41 |FLD37 |FLD32 )
#define OB4_3 ( FLD29 |FLD35 |FLD15 |FLD20 |FLD5 |FLD10 |FLD14 |FLD19 )
#define OW4_4 (   FLD21 |FLD36 |FLD31 |FLD27 |FLD22 |FLD46 |FLD41 |FLD37 |FLD32 )
#define OW4_4_2 ( FLD21 |FLD36 |FLD31 |FLD27 |FLD22 )
#define OB4_4 ( FLD30 |FLD15 |FLD20 |FLD24 |FLD29 |FLD5 |FLD10 |FLD14 |FLD19 )
#define OB4_4_2 ( FLD30 |FLD15 |FLD20 |FLD24 |FLD29 )
#define OW4_5 ( FLD26 |FLD21 |FLD17 |FLD36 |FLD31 |FLD27 )
#define OB4_5 ( FLD25 |FLD30 |FLD34 |FLD15 |FLD20 |FLD24 )
#define OW4_6 ( FLD26 |FLD31 |FLD38 |FLD46 |FLD47 |FLD48 )
#define OB4_6 ( FLD25 |FLD20 |FLD13 |FLD5 |FLD4 |FLD3 )
#define OW4_7 ( FLD26 |FLD38 |FLD42 |FLD46 |FLD47 |FLD48 )
#define OB4_7 ( FLD25 |FLD13 |FLD9 |FLD5 |FLD4 |FLD3 )
#define OW4_8 ( FLD26 |FLD32 |FLD46 |FLD41 |FLD47 |FLD42 )
#define OB4_8 ( FLD25 |FLD19 |FLD5 |FLD10 |FLD4 |FLD9 )
#define OW4_9 (   FLD31 |FLD32 |FLD46 |FLD41 |FLD37 |FLD47 |FLD42 )
#define OW4_9_2 ( FLD31 |FLD32 |FLD46 |FLD41 |FLD37 )
#define OB4_9 (   FLD20 | FLD19 |FLD5 |FLD10 |FLD14 |FLD4 |FLD9 )
#define OB4_9_2 ( FLD20 | FLD19 |FLD5 |FLD10 |FLD14 )
#define OW4_10 ( FLD36 |FLD31 |FLD27 |FLD46 |FLD41 |FLD37 )
#define OB4_10 ( FLD15 |FLD20 |FLD24 |FLD5 |FLD10 |FLD14 )
#define OW4_11 ( FLD36 |FLD42 )
#define OB4_11 ( FLD15 |FLD9 )
#define OW4_12 ( FLD47 |FLD42 )
#define OB4_12 ( FLD4 |FLD9 )
#define OW4_13 ( FLD46 |FLD41 |FLD37 |FLD47 )
#define OB4_13 ( FLD5 |FLD10 |FLD14 |FLD4 )
#define OW4_14 ( FLD23 |FLD15 |FLD20 |FLD35 |FLD34 |FLD40 |FLD45 |FLD33 |FLD39 |FLD44 |FLD50 )
#define OB4_14 ( FLD28 |FLD36 |FLD31 |FLD16 |FLD17 |FLD11 |FLD6 |FLD18 |FLD12 |FLD7 |FLD1 )
#define OW4_15 ( FLD23 |FLD15 |FLD29 |FLD35 |FLD34 |FLD40 |FLD45 |FLD33 |FLD39 |FLD44 |FLD50 )
#define OB4_15 ( FLD28 |FLD36 |FLD22 |FLD16 |FLD17 |FLD11 |FLD6 |FLD18 |FLD12 |FLD7 |FLD1 )
#define OW4_16 ( FLD15 |FLD19 |FLD30 |FLD35 |FLD29 |FLD34 |FLD40 |FLD45 )
#define OB4_16 ( FLD36 |FLD32 |FLD21 |FLD16 |FLD22 |FLD17 |FLD11 |FLD6 )
#define OW4_17 (   FLD19 | FLD20 | FLD24 |FLD30 |FLD35 |FLD29 |FLD34 |FLD40 |FLD45 )
#define OW4_17_2 ( FLD19 | FLD20 | FLD24 |FLD30 |FLD35 )
#define OB4_17 (   FLD31 | FLD27 |FLD32 |FLD21 |FLD16 |FLD22 |FLD17 |FLD11 |FLD6 )
#define OB4_17_2 ( FLD31 | FLD27 |FLD32 |FLD21 |FLD16 )
#define OW4_18 ( FLD14 |FLD20 |FLD25 |FLD24 |FLD30 |FLD35 )
#define OB4_18 ( FLD37 |FLD31 |FLD26 |FLD27 |FLD21 |FLD16 )
#define OW4_19 ( FLD25 |FLD30 |FLD33 |FLD45 |FLD44 |FLD50 |FLD43 |FLD49 )
#define OB4_19 ( FLD26 |FLD21 |FLD18 |FLD6 |FLD7 |FLD1 |FLD8 |FLD2 )
#define OW4_20 ( FLD25 |FLD39 |FLD33 |FLD45 |FLD44 |FLD50 |FLD43 |FLD49 )
#define OB4_20 ( FLD26 |FLD12 |FLD18 |FLD6 |FLD7 |FLD1 |FLD8 |FLD2 )
#define OW4_21 ( FLD25 |FLD29 |FLD40 |FLD45 |FLD39 |FLD44 |FLD50 )
#define OB4_21 ( FLD26 |FLD22 |FLD11 |FLD6 |FLD12 |FLD7 |FLD1 )
#define OW4_22 (   FLD30 | FLD34 |FLD29 |FLD40 |FLD45 |FLD39 |FLD44 |FLD50 )
#define OW4_22_2 ( FLD30 | FLD34 |FLD40 |FLD45)
#define OB4_22 (   FLD21 |FLD17 |FLD22 |FLD11 |FLD6 |FLD12 |FLD7 |FLD1 )
#define OB4_22_2 ( BLD30 | BLD34 |BLD40 |BLD45 )
#define OW4_23 ( FLD24 |FLD30 |FLD35 |FLD34 |FLD40 |FLD45 )
#define OB4_23 ( FLD27 |FLD21 |FLD16 |FLD17 |FLD11 |FLD6 )
#define OW4_24 ( FLD35 |FLD40 |FLD43 |FLD45 )
#define OB4_24 ( FLD16 |FLD11 |FLD8 |FLD6 )
#define OW4_25 ( FLD35 |FLD49 |FLD43 |FLD45 )
#define OB4_25 ( FLD16 |FLD2 |FLD8 |FLD6 )
#define OW4_26 ( FLD35 |FLD39 |FLD49 |FLD50 )
#define OB4_26 ( FLD16 |FLD12 |FLD2 |FLD1 )
#define OW4_27 (   FLD40 | FLD44 |FLD39 |FLD49 |FLD50 )
#define OW4_27_2 ( FLD40 | FLD44 |FLD39 |FLD50 )
#define OB4_27 (   FLD11 |FLD7 |FLD12 |FLD2 |FLD1 )
#define OB4_27_2 ( FLD11 |FLD7 |FLD12 |FLD1 )
#define OW4_28 ( FLD34 |FLD40 |FLD45 |FLD44 |FLD50 )
#define OB4_28 ( FLD17 |FLD11 |FLD6 |FLD7 |FLD1 )
#define OW4_29 ( FLD34 |FLD50 )
#define OB4_29 ( FLD17 |FLD1 )

// black locked pieces
#define OB1_1 (FLD25 | FLD24 | FLD20 | FLD19 | FLD18)
#define OB1_2 (FLD30 | FLD25 | FLD24 | FLD20 | FLD18)
#define OB1_3 (FLD30 | FLD25 | FLD24 | FLD23)
#define OB1_4 (FLD25 | FLD35 )
#define OB1_5 (FLD35 | FLD29)
#define OB1_6 (FLD15 | FLD14 | FLD10 | FLD9 | FLD8)
#define OB1_7 (FLD20 | FLD15 | FLD14 | FLD10 | FLD8)
#define OB1_8 (FLD20 | FLD15 | FLD14 | FLD13)
#define OB1_9 (FLD15 | FLD25 )
#define OB1_10 (FLD25 | FLD19)
#define OB1_11 (FLD10 | FLD5 | FLD4 | FLD3)
#define OB1_12 (FLD5 | FLD10 | FLD15 | FLD3)
#define OB1_13 (FLD15 | FLD9)
#define OB1_14 (FLD27 | FLD26 | FLD23 | FLD22 | FLD21)
#define OB1_15 (FLD31 | FLD27 | FLD26 | FLD23 | FLD21)
#define OB1_16 (FLD31 | FLD27 | FLD26 | FLD28)
#define OB1_17 (FLD36 | FLD26)
#define OB1_18 (FLD36 | FLD32)
#define OB1_19 (FLD17 | FLD16 | FLD13 | FLD12 | FLD11)
#define OB1_20 (FLD21 | FLD17 | FLD16 | FLD13 | FLD11)
#define OB1_21 (FLD21 | FLD17 | FLD16 | FLD18)
#define OB1_22 (FLD26 | FLD16 )
#define OB1_23 (FLD26 | FLD22)
#define OB1_24 (FLD7 | FLD3 | FLD2 | FLD1)
#define OB1_25 (FLD11 | FLD7 | FLD6 | FLD3 | FLD1)
#define OB1_26 (FLD11 | FLD7 | FLD6 | FLD8)
#define OB1_27 (FLD16 |  FLD6)
#define OB1_28 (FLD16 | FLD12)
#define OB1_29 (FLD6 | FLD2)
#define OB2_1 (FLD29 | FLD33)
#define OB2_2 (FLD29 | FLD33)
#define OB2_3 (FLD34 | FLD39)
#define OB2_4 (FLD34 | FLD39)
#define OB2_5 (FLD40 | FLD44)
#define OB2_6 (FLD19 | FLD23)
#define OB2_7 (FLD19 | FLD23)
#define OB2_8 (FLD24 | FLD29)
#define OB2_9 (FLD24 | FLD29)
#define OB2_10 (FLD30 | FLD34)
#define OB2_11 (FLD14 | FLD19)
#define OB2_12 (FLD14 | FLD19)
#define OB2_13 (FLD20 | FLD24)
#define OB2_14 (FLD32 | FLD38)
#define OB2_15 (FLD32 | FLD38)
#define OB2_16 (FLD37 | FLD42)
#define OB2_17 (FLD37 | FLD42)
#define OB2_18 (FLD41 | FLD47)
#define OB2_19 (FLD22 | FLD28)
#define OB2_20 (FLD22 | FLD28)
#define OB2_21 (FLD27 | FLD32)
#define OB2_22 (FLD27 | FLD32)
#define OB2_23 (FLD31 | FLD37)
#define OB2_24 (FLD12 | FLD18)
#define OB2_25 (FLD12 | FLD18)
#define OB2_26 (FLD17 | FLD22)
#define OB2_27 (FLD17 | FLD22)
#define OB2_28 (FLD21 | FLD27)
#define OB2_29 (FLD11 | FLD17)
#define OB3_1 (FLD50 | FLD45 | FLD44 | FLD40 | FLD39 | FLD35 | FLD34 | FLD30)
#define OB3_2 (FLD50 | FLD45 | FLD44 | FLD40 | FLD39 | FLD35 | FLD34)
#define OB3_3 (FLD50 | FLD45 | FLD44 | FLD40 | FLD35)
#define OB3_4 (FLD50 | FLD45 | FLD44 | FLD40)
#define OB3_5 (FLD50 | FLD45)
#define OB3_6 (FLD45 | FLD40 | FLD35 | FLD34 | FLD30 | FLD29 | FLD25 | FLD24 | FLD20)
#define OB3_7 (FLD45 | FLD40 | FLD35 | FLD34 | FLD30 | FLD29 | FLD25 | FLD24)
#define OB3_8 (FLD45 | FLD40 | FLD35 | FLD34 | FLD30 | FLD25)
#define OB3_9 (FLD45 | FLD40 | FLD35 | FLD34 | FLD30)
#define OB3_10 (FLD45 | FLD40 | FLD35)
#define OB3_11 (FLD35 | FLD30 | FLD25 | FLD24 | FLD20)
#define OB3_12 (FLD35 | FLD30 | FLD25 | FLD24 | FLD20)
#define OB3_13 (FLD35 | FLD30 | FLD25)
#define OB3_14 (FLD47 | FLD46 | FLD42 | FLD41 | FLD37 | FLD36)
#define OB3_15 (FLD47 | FLD46 | FLD42 | FLD41 | FLD37 | FLD36)
#define OB3_16 (FLD47 | FLD46 | FLD41)
#define OB3_17 (FLD47 | FLD46 | FLD41)
#define OB3_18 (FLD46)
#define OB3_19 (FLD46 | FLD41 | FLD37 | FLD36 | FLD32 | FLD31 | FLD27 | FLD26)
#define OB3_20 (FLD46 | FLD41 | FLD37 | FLD36 | FLD32 | FLD31 | FLD27 | FLD26)
#define OB3_21 (FLD46 | FLD41 | FLD37 | FLD36 | FLD31)
#define OB3_22 (FLD46 | FLD41 | FLD37 | FLD36 | FLD31)
#define OB3_23 (FLD46 | FLD41 | FLD36)
#define OB3_24 (FLD36 | FLD31 | FLD27 | FLD26 | FLD22 | FLD21 | FLD17 | FLD16 | FLD11 | FLD6)
#define OB3_25 (FLD36 | FLD31 | FLD27 | FLD26 | FLD22 | FLD21 | FLD17 | FLD16)
#define OB3_26 (FLD36 | FLD31 | FLD27 | FLD26 | FLD21)
#define OB3_27 (FLD36 | FLD31 | FLD27 | FLD26 | FLD21)
#define OB3_28 (FLD36 | FLD31 | FLD26)
#define OB3_29 (FLD26 | FLD21 | FLD16)


// kanon

#define CANON_W1_WHITE (FLD46 |FLD36 |FLD41 | FLD37 )
#define CANON_W1_BLACK  FLD26
#define CANON_W1_EMPTY (FLD31 |FLD32)
#define CANON_W1_EMPTY_OR_WHITE_EMPTY FLD42
#define CANON_W1_EMPTY_OR_WHITE_WHITE (FLD42 | FLD48)

#define CANON_W2_WHITE (FLD36 |FLD26 |FLD31 | FLD27 )
#define CANON_W2_BLACK  FLD16
#define CANON_W2_EMPTY (FLD21 |FLD22)
#define CANON_W2_EMPTY_OR_WHITE_EMPTY FLD32
#define CANON_W2_EMPTY_OR_WHITE_WHITE (FLD32 | FLD38)

#define CANON_W3_WHITE (FLD26 |FLD16 |FLD21 | FLD17 )
#define CANON_W3_BLACK  FLD6
#define CANON_W3_EMPTY (FLD11 |FLD12)
#define CANON_W3_EMPTY_OR_WHITE_EMPTY FLD22
#define CANON_W3_EMPTY_OR_WHITE_WHITE (FLD22 | FLD28)

#define CANON_W4_WHITE (FLD34 |FLD35 | FLD40 | FLD45 )
#define CANON_W4_BLACK  FLD25
#define CANON_W4_EMPTY (FLD30 |FLD29)
#define CANON_W4_EMPTY_OR_WHITE_EMPTY FLD39
#define CANON_W4_EMPTY_OR_WHITE_WHITE (FLD39 | FLD43)

#define CANON_W5_WHITE (FLD24 |FLD25 | FLD30 | FLD35 )
#define CANON_W5_BLACK  FLD15
#define CANON_W5_EMPTY (FLD20 |FLD19)
#define CANON_W5_EMPTY_OR_WHITE_EMPTY FLD29
#define CANON_W5_EMPTY_OR_WHITE_WHITE (FLD29 | FLD33)


#define CANON_B1_BLACK (FLD5 |FLD15 |FLD10 | FLD14 )
#define CANON_B1_WHITE  FLD25
#define CANON_B1_EMPTY (FLD20 |FLD19)
#define CANON_B1_EMPTY_OR_BLACK_EMPTY FLD9
#define CANON_B1_EMPTY_OR_BLACK_BLACK (FLD9 | FLD3)

#define CANON_B2_BLACK (FLD15 |FLD25 |FLD20 | FLD24 )
#define CANON_B2_WHITE  FLD35
#define CANON_B2_EMPTY (FLD30 |FLD29)
#define CANON_B2_EMPTY_OR_BLACK_EMPTY FLD19
#define CANON_B2_EMPTY_OR_BLACK_BLACK (FLD19 | FLD13)

#define CANON_B3_BLACK (FLD25 |FLD35 |FLD30 | FLD34 )
#define CANON_B3_WHITE  FLD45
#define CANON_B3_EMPTY (FLD40 |FLD39)
#define CANON_B3_EMPTY_OR_BLACK_EMPTY FLD29
#define CANON_B3_EMPTY_OR_BLACK_BLACK (FLD29 | FLD23)

#define CANON_B4_BLACK (FLD17 |FLD16 | FLD11 | FLD6 )
#define CANON_B4_WHITE  FLD26
#define CANON_B4_EMPTY (FLD21 |FLD22)
#define CANON_B4_EMPTY_OR_BLACK_EMPTY FLD12
#define CANON_B4_EMPTY_OR_BLACK_BLACK (FLD12 | FLD8)

#define CANON_B5_BLACK (FLD27 |FLD26 | FLD21 | FLD16 )
#define CANON_B5_WHITE  FLD36
#define CANON_B5_EMPTY (FLD31 |FLD32)
#define CANON_B5_EMPTY_OR_BLACK_EMPTY FLD22
#define CANON_B5_EMPTY_OR_BLACK_BLACK (FLD22 | FLD18)

#define white_field(num) (FLD##num)
#define black_field(num) (BLD##num)

#define black_jump(start, over, stop) ((empty_field & white_field(start)) && (black_man & white_field(over)) && (empty_field & white_field(stop)))
#define white_jump(start, over, stop) ((empty_field & black_field(start)) && (white_man & black_field(over)) && (empty_field & black_field(stop)))

#define w1(v1) white_field(v1)
#define w2(v1,v2) (w1(v2)|w1(v1))
#define w3(v1,v2,v3) (w1(v3)|w2(v1,v2))
#define w4(v1,v2,v3,v4) (w1(v4)|w3(v1,v2,v3))
#define w5(v1,v2,v3,v4,v5) (w1(v5)|w4(v1,v2,v3,v4))
#define w6(v1,v2,v3,v4,v5,v6) (w1(v6)|w5(v1,v2,v3,v4,v5))
#define w7(v1,v2,v3,v4,v5,v6,v7) (w1(v7)|w6(v1,v2,v3,v4,v5,v6))
#define w8(v1,v2,v3,v4,v5,v6,v7,v8) (w1(v8)|w7(v1,v2,v3,v4,v5,v6,v7))
#define w9(v1,v2,v3,v4,v5,v6,v7,v8,v9) (w1(v9)|w8(v1,v2,v3,v4,v5,v6,v7,v8))
#define w10(v1,v2,v3,v4,v5,v6,v7,v8,v9,v10) (w1(v10)|w9(v1,v2,v3,v4,v5,v6,v7,v8,v9))
#define w11(v1,v2,v3,v4,v5,v6,v7,v8,v9,v10,v11) (w1(v11)|w10(v1,v2,v3,v4,v5,v6,v7,v8,v9,v10))
#define w12(v1,v2,v3,v4,v5,v6,v7,v8,v9,v10,v11,v12) (w1(v12)|w11(v1,v2,v3,v4,v5,v6,v7,v8,v9,v10,v11))
#define w13(v1,v2,v3,v4,v5,v6,v7,v8,v9,v10,v11,v12,v13) (w1(v13)|w12(v1,v2,v3,v4,v5,v6,v7,v8,v9,v10,v11,v12))
#define w14(v1,v2,v3,v4,v5,v6,v7,v8,v9,v10,v11,v12,v13,v14) (w1(v14)|w13(v1,v2,v3,v4,v5,v6,v7,v8,v9,v10,v11,v12,v13))
#define w15(v1,v2,v3,v4,v5,v6,v7,v8,v9,v10,v11,v12,v13,v14,v15) (w1(v15)|w14(v1,v2,v3,v4,v5,v6,v7,v8,v9,v10,v11,v12,v13,v14))

#define b1(v1) black_field(v1)
#define b2(v1,v2) (b1(v2)|b1(v1))
#define b3(v1,v2,v3) (b1(v3)|b2(v1,v2))
#define b4(v1,v2,v3,v4) (b1(v4)|b3(v1,v2,v3))
#define b5(v1,v2,v3,v4,v5) (b1(v5)|b4(v1,v2,v3,v4))
#define b6(v1,v2,v3,v4,v5,v6) (b1(v6)|b5(v1,v2,v3,v4,v5))
#define b7(v1,v2,v3,v4,v5,v6,v7) (b1(v7)|b6(v1,v2,v3,v4,v5,v6))
#define b8(v1,v2,v3,v4,v5,v6,v7,v8) (b1(v8)|b7(v1,v2,v3,v4,v5,v6,v7))
#define b9(v1,v2,v3,v4,v5,v6,v7,v8,v9) (b1(v9)|b8(v1,v2,v3,v4,v5,v6,v7,v8))
#define b10(v1,v2,v3,v4,v5,v6,v7,v8,v9,v10) (b1(v10)|b9(v1,v2,v3,v4,v5,v6,v7,v8,v9))
#define b11(v1,v2,v3,v4,v5,v6,v7,v8,v9,v10,v11) (b1(v11)|b10(v1,v2,v3,v4,v5,v6,v7,v8,v9,v10))
#define b12(v1,v2,v3,v4,v5,v6,v7,v8,v9,v10,v11,v12) (b1(v12)|b11(v1,v2,v3,v4,v5,v6,v7,v8,v9,v10,v11))
#define b13(v1,v2,v3,v4,v5,v6,v7,v8,v9,v10,v11,v12,v13) (b1(v13)|b12(v1,v2,v3,v4,v5,v6,v7,v8,v9,v10,v11,v12))
#define b14(v1,v2,v3,v4,v5,v6,v7,v8,v9,v10,v11,v12,v13,v14) (b1(v14)|b13(v1,v2,v3,v4,v5,v6,v7,v8,v9,v10,v11,v12,v13))
#define b15(v1,v2,v3,v4,v5,v6,v7,v8,v9,v10,v11,v12,v13,v14,v15) (b1(v15)|b14(v1,v2,v3,v4,v5,v6,v7,v8,v9,v10,v11,v12,v13,v14))

#ifdef generate_code_for_white
#define all_white_1(v1) (white_man & white_field(v1))
#define all_white_2(v1,v2) (all_white((w2(v1,v2))))
#define all_white_3(v1,v2,v3) (all_white((w3(v1,v2,v3))))
#define all_white_4(v1,v2,v3,v4) (all_white((w4(v1,v2,v3,v4))))
#define all_white_5(v1,v2,v3,v4,v5) (all_white((w5(v1,v2,v3,v4,v5))))
#define all_white_6(v1,v2,v3,v4,v5,v6) (all_white((w6(v1,v2,v3,v4,v5,v6))))
#define all_white_7(v1,v2,v3,v4,v5,v6,v7) (all_white((w7(v1,v2,v3,v4,v5,v6,v7))))
#define all_white_8(v1,v2,v3,v4,v5,v6,v7,v8) (all_white((w8(v1,v2,v3,v4,v5,v6,v7,v8))))
#define all_white_9(v1,v2,v3,v4,v5,v6,v7,v8,v9) (all_white((w9(v1,v2,v3,v4,v5,v6,v7,v8,v9))))
#define all_white_10(v1,v2,v3,v4,v5,v6,v7,v8,v9,v10) (all_white((w10(v1,v2,v3,v4,v5,v6,v7,v8,v9,v10))))
#else
#define all_white_1(v1) (black_man & black_field(v1))
#define all_white_2(v1,v2) (all_black((b2(v1,v2))))
#define all_white_3(v1,v2,v3) (all_black((b3(v1,v2,v3))))
#define all_white_4(v1,v2,v3,v4) (all_black((b4(v1,v2,v3,v4))))
#define all_white_5(v1,v2,v3,v4,v5) (all_black((b5(v1,v2,v3,v4,v5))))
#define all_white_6(v1,v2,v3,v4,v5,v6) (all_black((b6(v1,v2,v3,v4,v5,v6))))
#define all_white_7(v1,v2,v3,v4,v5,v6,v7) (all_black((b7(v1,v2,v3,v4,v5,v6,v7))))
#define all_white_8(v1,v2,v3,v4,v5,v6,v7,v8) (all_black((b8(v1,v2,v3,v4,v5,v6,v7,v8))))
#define all_white_9(v1,v2,v3,v4,v5,v6,v7,v8,v9) (all_black((b9(v1,v2,v3,v4,v5,v6,v7,v8,v9))))
#define all_white_10(v1,v2,v3,v4,v5,v6,v7,v8,v9,v10) (all_black((b10(v1,v2,v3,v4,v5,v6,v7,v8,v9,v10))))
#endif

#ifdef generate_code_for_white
#define all_black_1(v1) (black_man & white_field(v1))
#define all_black_2(v1,v2) (all_black((w2(v1,v2))))
#define all_black_3(v1,v2,v3) (all_black((w3(v1,v2,v3))))
#define all_black_4(v1,v2,v3,v4) (all_black((w4(v1,v2,v3,v4))))
#define all_black_5(v1,v2,v3,v4,v5) (all_black((w5(v1,v2,v3,v4,v5))))
#define all_black_6(v1,v2,v3,v4,v5,v6) (all_black((w6(v1,v2,v3,v4,v5,v6))))
#define all_black_7(v1,v2,v3,v4,v5,v6,v7) (all_black((w7(v1,v2,v3,v4,v5,v6,v7))))
#define all_black_8(v1,v2,v3,v4,v5,v6,v7,v8) (all_black((w8(v1,v2,v3,v4,v5,v6,v7,v8))))
#define all_black_9(v1,v2,v3,v4,v5,v6,v7,v8,v9) (all_black((w9(v1,v2,v3,v4,v5,v6,v7,v8,v9))))
#define all_black_10(v1,v2,v3,v4,v5,v6,v7,v8,v9,v10) (all_black((w10(v1,v2,v3,v4,v5,v6,v7,v8,v9,v10))))
#else
#define all_black_1(v1) (white_man & black_field(v1))
#define all_black_2(v1,v2) (all_white((b2(v1,v2))))
#define all_black_3(v1,v2,v3) (all_white((b3(v1,v2,v3))))
#define all_black_4(v1,v2,v3,v4) (all_white((b4(v1,v2,v3,v4))))
#define all_black_5(v1,v2,v3,v4,v5) (all_white((b5(v1,v2,v3,v4,v5))))
#define all_black_6(v1,v2,v3,v4,v5,v6) (all_white((b6(v1,v2,v3,v4,v5,v6))))
#define all_black_7(v1,v2,v3,v4,v5,v6,v7) (all_white((b7(v1,v2,v3,v4,v5,v6,v7))))
#define all_black_8(v1,v2,v3,v4,v5,v6,v7,v8) (all_white((b8(v1,v2,v3,v4,v5,v6,v7,v8))))
#define all_black_9(v1,v2,v3,v4,v5,v6,v7,v8,v9) (all_white((b9(v1,v2,v3,v4,v5,v6,v7,v8,v9))))
#define all_black_10(v1,v2,v3,v4,v5,v6,v7,v8,v9,v10) (all_white((b10(v1,v2,v3,v4,v5,v6,v7,v8,v9,v10))))
#endif

#ifdef generate_code_for_white
#define all_empty_1(v1) (empty_field & white_field(v1))
#define all_empty_2(v1,v2) (all_empty((w2(v1,v2))))
#define all_empty_3(v1,v2,v3) (all_empty((w3(v1,v2,v3))))
#define all_empty_4(v1,v2,v3,v4) (all_empty((w4(v1,v2,v3,v4))))
#define all_empty_5(v1,v2,v3,v4,v5) (all_empty((w5(v1,v2,v3,v4,v5))))
#define all_empty_6(v1,v2,v3,v4,v5,v6) (all_empty((w6(v1,v2,v3,v4,v5,v6))))
#define all_empty_7(v1,v2,v3,v4,v5,v6,v7) (all_empty((w7(v1,v2,v3,v4,v5,v6,v7))))
#define all_empty_8(v1,v2,v3,v4,v5,v6,v7,v8) (all_empty((w8(v1,v2,v3,v4,v5,v6,v7,v8))))
#define all_empty_9(v1,v2,v3,v4,v5,v6,v7,v8,v9) (all_empty((w9(v1,v2,v3,v4,v5,v6,v7,v8,v9))))
#define all_empty_10(v1,v2,v3,v4,v5,v6,v7,v8,v9,v10) (all_empty((w10(v1,v2,v3,v4,v5,v6,v7,v8,v9,v10))))
#else
#define all_empty_1(v1) (empty_field & white_field(v1))
#define all_empty_2(v1,v2) (all_empty((b2(v1,v2))))
#define all_empty_3(v1,v2,v3) (all_empty((b3(v1,v2,v3))))
#define all_empty_4(v1,v2,v3,v4) (all_empty((b4(v1,v2,v3,v4))))
#define all_empty_5(v1,v2,v3,v4,v5) (all_empty((b5(v1,v2,v3,v4,v5))))
#define all_empty_6(v1,v2,v3,v4,v5,v6) (all_empty((b6(v1,v2,v3,v4,v5,v6))))
#define all_empty_7(v1,v2,v3,v4,v5,v6,v7) (all_empty((b7(v1,v2,v3,v4,v5,v6,v7))))
#define all_empty_8(v1,v2,v3,v4,v5,v6,v7,v8) (all_empty((b8(v1,v2,v3,v4,v5,v6,v7,v8))))
#define all_empty_9(v1,v2,v3,v4,v5,v6,v7,v8,v9) (all_empty((b9(v1,v2,v3,v4,v5,v6,v7,v8,v9))))
#define all_empty_10(v1,v2,v3,v4,v5,v6,v7,v8,v9,v10) (all_empty((b10(v1,v2,v3,v4,v5,v6,v7,v8,v9,v10))))
#endif
