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


#define AVOID_4641_WHITE (FLD31 | FLD37 | FLD41 )
#define AVOID_4641_BLACK (FLD20 | FLD14 | FLD10 )
#define AVOID2_4641_WHITE (FLD31 | FLD36 | FLD37 | FLD41 )
#define AVOID2_4641_BLACK (FLD20 | FLD15 | FLD14 | FLD10 )

// ketting stelling

#define WHITE_BLACK_CHAIN1 ( FLD12 | FLD17 | FLD18 | FLD22)
#define WHITE_WHITE_CHAIN1 ( FLD27 | FLD31 | FLD28 | FLD33)
#define WHITE_BLACK_CHAIN2 ( FLD13 | FLD18 | FLD19 | FLD23)
#define WHITE_WHITE_CHAIN2 ( FLD28 | FLD32 | FLD29 | FLD34)
#define WHITE_BLACK_CHAIN3 ( FLD18 | FLD22 | FLD23 | FLD28)
#define WHITE_WHITE_CHAIN3 ( FLD32 | FLD37 | FLD33 | FLD39)
#define WHITE_BLACK_CHAIN4 ( FLD19 | FLD23 | FLD24 | FLD29)
#define WHITE_WHITE_CHAIN4 ( FLD33 | FLD38 | FLD34 | FLD40)
#define BLACK_WHITE_CHAIN1 ( FLD29 | FLD33 | FLD34 | FLD39)
#define BLACK_BLACK_CHAIN1 ( FLD24 | FLD20 | FLD23 | FLD18)
#define BLACK_WHITE_CHAIN2 ( FLD28 | FLD32 | FLD33 | FLD38)
#define BLACK_BLACK_CHAIN2 ( FLD23 | FLD19 | FLD22 | FLD17)
#define BLACK_WHITE_CHAIN3 ( FLD22 | FLD27 | FLD28 | FLD32)
#define BLACK_BLACK_CHAIN3 ( FLD18 | FLD13 | FLD17 | FLD11)
#define BLACK_WHITE_CHAIN4 ( FLD23 | FLD28 | FLD29 | FLD33)
#define BLACK_BLACK_CHAIN4 ( FLD19 | FLD14 | FLD18 | FLD12)

// halve hek

#define WHITE_WHITE_HALVE_HEK (FLD36 | FLD31 | FLD27)
#define WHITE_BLACK_HALVE_HEK (FLD12 | FLD16 | FLD17 | FLD18 | FLD22)
#define BLACK_BLACK_HALVE_HEK (FLD15 | FLD20 | FLD24)
#define BLACK_WHITE_HALVE_HEK (FLD39 | FLD35 | FLD34 | FLD33 | FLD29)

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

#define KLASSIEK_WHITE (FLD27 | FLD28 | FLD32)
#define KLASSIEK_BLACK (FLD23 | FLD24 | FLD19)

#define WHITE_CROWN FLD48
#define BLACK_CROWN FLD3

#define WHITE_OLYMPIC (FLD40 | FLD45)
#define BLACK_OLYMPIC (FLD11 | FLD6)

// to calculate locked men by a king

#define L16NW (FLD16  | FLD11 | FLD7 | FLD2)
#define L26NW (FLD26 | FLD21 | FLD17 | FLD12 | FLD8 | FLD3)
#define L36NW (FLD36 | FLD31 | FLD27 | FLD22 | FLD18 | FLD13 | FLD9 | FLD4)
#define L46NW (FLD46 | FLD41 | FLD37 | FLD32 | FLD28 | FLD23 | FLD19 | FLD14 | FLD10 | FLD5)

#define L5SE L46NW
#define L15SE (FLD15 | FLD20 | FLD24 | FLD29 | FLD33 | FLD38 | FLD42 | FLD47)
#define L25SE (FLD25 | FLD30 | FLD34 | FLD39 | FLD43 | FLD48)
#define L35SE (FLD35 | FLD40 | FLD44 | FLD49)

#define L4SW (FLD4 | FLD10 | FLD15)
#define L3SW (FLD3 | FLD9 | FLD14 | FLD20 | FLD25)
#define L2SW (FLD2 | FLD8 | FLD13 | FLD19 | FLD24 | FLD30 | FLD35)
#define L1SW (FLD1 | FLD7 | FLD12 | FLD18 | FLD23 | FLD29 | FLD34 | FLD40 | FLD45)

#define L6SW (FLD6 | FLD11 | FLD17 | FLD22 | FLD28 | FLD33 | FLD39 | FLD44 | FLD50)
#define L16SW (FLD16 | FLD21 | FLD27 | FLD32 | FLD38 | FLD43 | FLD49)
#define L26SW (FLD26 | FLD31 | FLD37 | FLD42 | FLD48)
#define L36SW (FLD36 | FLD41 | FLD47)

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

#define WM_01_20 (FLD7 | FLD8 | FLD9 | FLD10 | FLD11 | FLD12 | FLD13 | FLD14 | FLD16 | FLD17 | FLD18 | FLD19 | FLD20)
#define BM_50_31 (FLD44 | FLD43 | FLD42 | FLD41 | FLD40 | FLD39 | FLD38 | FLD37 | FLD35 | FLD34 | FLD33 | FLD32 | FLD31)





// voorpost definitions

#define BV1_W12 FLD1
#define BV2_W12 FLD2
#define BV1_W13 FLD2
#define BV2_W13 FLD3
#define BV1_W14 FLD3
#define BV2_W14 FLD4
#define BV1_W17 FLD6
#define BV2_W17 FLD8
#define BV1_W18 FLD7
#define BV2_W18 FLD9
#define BV1_W19 FLD8
#define BV2_W19 FLD10

#define BV1_W2_12 FLD2
#define BV2_W2_12 FLD3
#define BV1_W2_13 FLD3
#define BV2_W2_13 FLD4
#define BV1_W2_14 FLD4
#define BV2_W2_14 FLD5
#define BV1_W2_17 (FLD1 | FLD2 | FLD7)
#define BV2_W2_17 (FLD1 | FLD2 | FLD7)
#define BV1_W2_18 (FLD2 | FLD3 | FLD8)
#define BV2_W2_18 (FLD2 | FLD3 | FLD8)
#define BV1_W2_19 (FLD3 | FLD4 | FLD9)
#define BV2_W2_19 (FLD3 | FLD4 | FLD9)

#define BV1_W3_12 (FLD18 | FLD23)
#define BV2_W3_12 (FLD17 | FLD21)
#define BV1_W3_13 (FLD19 | FLD24)
#define BV2_W3_13 (FLD18 | FLD22)
#define BV1_W3_14 (FLD20 | FLD25)
#define BV2_W3_14 (FLD19 | FLD23)
#define BV1_W3_17 (FLD22 | FLD28)
#define BV2_W3_17 (FLD21 | FLD26)
#define BV1_W3_18 (FLD23 | FLD29)
#define BV2_W3_18 (FLD22 | FLD27)
#define BV1_W3_19 (FLD24 | FLD30)
#define BV2_W3_19 (FLD23 | FLD28)

#define WV1_W39 FLD50
#define WV2_W39 FLD49
#define WV1_W38 FLD49
#define WV2_W38 FLD48
#define WV1_W37 FLD48
#define WV2_W37 FLD47
#define WV1_W34 FLD45
#define WV2_W34 FLD43
#define WV1_W33 FLD44
#define WV2_W33 FLD42
#define WV1_W32 FLD43
#define WV2_W32 FLD41

#define WV1_W2_39 FLD49
#define WV2_W2_39 FLD48
#define WV1_W2_38 FLD48
#define WV2_W2_38 FLD47
#define WV1_W2_37 FLD47
#define WV2_W2_37 FLD46
#define WV1_W2_34 (FLD50 | FLD49 | FLD44)
#define WV2_W2_34 (FLD50 | FLD49 | FLD44)
#define WV1_W2_33 (FLD49 | FLD48 | FLD43)
#define WV2_W2_33 (FLD49 | FLD48 | FLD43)
#define WV1_W2_32 (FLD48 | FLD47 | FLD42)
#define WV2_W2_32 (FLD48 | FLD47 | FLD42)

#define WV1_W3_39 (FLD33 | FLD28)
#define WV2_W3_39 (FLD34 | FLD30)
#define WV1_W3_38 (FLD32 | FLD27)
#define WV2_W3_38 (FLD33 | FLD29)
#define WV1_W3_37 (FLD31 | FLD26)
#define WV2_W3_37 (FLD32 | FLD28)
#define WV1_W3_34 (FLD29 | FLD23)
#define WV2_W3_34 (FLD30 | FLD25)
#define WV1_W3_33 (FLD28 | FLD22)
#define WV2_W3_33 (FLD29 | FLD24)
#define WV1_W3_32 (FLD27 | FLD21)
#define WV2_W3_32 (FLD28 | FLD23)

// very simple locked pieces

#define BL_W12 (FLD1 | FLD3)
#define BL_W13 (FLD2 | FLD4)
#define BL_W14 (FLD3 | FLD5)
#define BL_W17 (FLD6 | FLD8)
#define BL_W18 (FLD7 | FLD9)
#define BL_W19 (FLD8 | FLD10)

#define WL_W39 (FLD50 | FLD48)
#define WL_W38 (FLD49 | FLD47)
#define WL_W37 (FLD48 | FLD46)
#define WL_W34 (FLD45 | FLD43)
#define WL_W33 (FLD44 | FLD42)
#define WL_W32 (FLD43 | FLD41)

#define BL_W3_12 (FLD2 | FLD7 | FLD8)
#define BL_W3_13 (FLD3 | FLD8 | FLD9)
#define BL_W3_14 (FLD4 | FLD9 | FLD10)
#define BL_W3_17 (FLD1 | FLD2 | FLD7)
#define BL_W3_18 (FLD2 | FLD3 | FLD8)
#define BL_W3_19 (FLD3 | FLD4 | FLD9)

#define WL_W3_39 (FLD49 | FLD44 | FLD43)
#define WL_W3_38 (FLD48 | FLD43 | FLD42)
#define WL_W3_37 (FLD47 | FLD42 | FLD41)
#define WL_W3_34 (FLD50 | FLD49 | FLD44)
#define WL_W3_33 (FLD49 | FLD48 | FLD43)
#define WL_W3_32 (FLD48 | FLD47 | FLD42)

// vleugelopsluitingen

// white locked pieces
#define OW1_1 (FLD26 | FLD27 | FLD31 | FLD32 | FLD33)
#define OW1_2 (FLD21 | FLD26 | FLD27 | FLD31 | FLD33)
#define OW1_3 (FLD21 | FLD26 | FLD27 | FLD28)
#define OW1_4 (FLD26 | FLD21 | FLD16 | FLD28)
#define OW1_5 (FLD16 | FLD22)
#define OW1_6 (FLD36 | FLD37 | FLD41 | FLD42 | FLD43)
#define OW1_7 (FLD31 | FLD36 | FLD37 | FLD41 | FLD43)
#define OW1_8 (FLD31 | FLD36 | FLD37 | FLD38)
#define OW1_9 (FLD36 | FLD31 | FLD26 | FLD38)
#define OW1_10 (FLD26 | FLD32)
#define OW1_11 (FLD41 | FLD46 | FLD47 | FLD48)
#define OW1_12 (FLD46 | FLD41 | FLD36 | FLD48)
#define OW1_13 (FLD36 | FLD42)
#define OW1_14 (FLD24 | FLD25 | FLD28 | FLD29 | FLD30)
#define OW1_15 (FLD20 | FLD24 | FLD25 | FLD28 | FLD30)
#define OW1_16 (FLD20 | FLD24 | FLD25 | FLD23)
#define OW1_17 (FLD15 | FLD20 | FLD25 | FLD23)
#define OW1_18 (FLD15 | FLD19)
#define OW1_19 (FLD34 | FLD35 | FLD38 | FLD39 | FLD40)
#define OW1_20 (FLD30 | FLD34 | FLD35 | FLD38 | FLD40)
#define OW1_21 (FLD30 | FLD34 | FLD35 | FLD33)
#define OW1_22 (FLD25 | FLD30 | FLD35 | FLD33)
#define OW1_23 (FLD25 | FLD29)
#define OW1_24 (FLD44 | FLD48 | FLD49 | FLD50)
#define OW1_25 (FLD40 | FLD44 | FLD45 | FLD48 | FLD50)
#define OW1_26 (FLD40 | FLD44 | FLD45 | FLD43)
#define OW1_27 (FLD35 | FLD40 | FLD45 | FLD43)
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
#define OW3_24 (FLD15 | FLD20 | FLD24 | FLD25 | FLD29 | FLD30 | FLD34 | FLD35 | FLD45)
#define OW3_25 (FLD15 | FLD20 | FLD24 | FLD25 | FLD29 | FLD30 | FLD34 | FLD35)
#define OW3_26 (FLD15 | FLD20 | FLD24 | FLD25 | FLD30)
#define OW3_27 (FLD15 | FLD20 | FLD24 | FLD25 | FLD30)
#define OW3_28 (FLD15 | FLD20 | FLD25)
#define OW3_29 (FLD25 | FLD30 | FLD35)
#define OW4_1 (FLD16 | FLD36 | FLD21)
#define OW4_2 (FLD16 | FLD36)
#define OW4_3 
#define OW4_4 
#define OW4_5 
#define OW4_6 (FLD26 | FLD46 | FLD31)
#define OW4_7 (FLD26 | FLD46)
#define OW4_8 
#define OW4_9 
#define OW4_10 
#define OW4_11 
#define OW4_12 
#define OW4_13 
#define OW4_14 
#define OW4_15 (FLD15)
#define OW4_16 
#define OW4_17 
#define OW4_18 
#define OW4_19 
#define OW4_20 (FLD25)
#define OW4_21 
#define OW4_22 
#define OW4_23 
#define OW4_24 (FLD45)
#define OW4_25 (FLD35)
#define OW4_26 
#define OW4_27 
#define OW4_28 
#define OW4_29 

// black locked pieces
#define OB1_1 (FLD25 | FLD24 | FLD20 | FLD19 | FLD18)
#define OB1_2 (FLD30 | FLD25 | FLD24 | FLD20 | FLD18)
#define OB1_3 (FLD30 | FLD25 | FLD24 | FLD23)
#define OB1_4 (FLD25 | FLD30 | FLD35 | FLD23)
#define OB1_5 (FLD35 | FLD29)
#define OB1_6 (FLD15 | FLD14 | FLD10 | FLD9 | FLD8)
#define OB1_7 (FLD20 | FLD15 | FLD14 | FLD10 | FLD8)
#define OB1_8 (FLD20 | FLD15 | FLD14 | FLD13)
#define OB1_9 (FLD15 | FLD20 | FLD25 | FLD13)
#define OB1_10 (FLD25 | FLD19)
#define OB1_11 (FLD10 | FLD5 | FLD4 | FLD3)
#define OB1_12 (FLD5 | FLD10 | FLD15 | FLD3)
#define OB1_13 (FLD15 | FLD9)
#define OB1_14 (FLD27 | FLD26 | FLD23 | FLD22 | FLD21)
#define OB1_15 (FLD31 | FLD27 | FLD26 | FLD23 | FLD21)
#define OB1_16 (FLD31 | FLD27 | FLD26 | FLD28)
#define OB1_17 (FLD36 | FLD31 | FLD26 | FLD28)
#define OB1_18 (FLD36 | FLD32)
#define OB1_19 (FLD17 | FLD16 | FLD13 | FLD12 | FLD11)
#define OB1_20 (FLD21 | FLD17 | FLD16 | FLD13 | FLD11)
#define OB1_21 (FLD21 | FLD17 | FLD16 | FLD18)
#define OB1_22 (FLD26 | FLD21 | FLD16 | FLD18)
#define OB1_23 (FLD26 | FLD22)
#define OB1_24 (FLD7 | FLD3 | FLD2 | FLD1)
#define OB1_25 (FLD11 | FLD7 | FLD6 | FLD3 | FLD1)
#define OB1_26 (FLD11 | FLD7 | FLD6 | FLD8)
#define OB1_27 (FLD16 | FLD11 | FLD6 | FLD8)
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
#define OB3_24 (FLD36 | FLD31 | FLD27 | FLD26 | FLD22 | FLD21 | FLD17 | FLD16 | FLD6)
#define OB3_25 (FLD36 | FLD31 | FLD27 | FLD26 | FLD22 | FLD21 | FLD17 | FLD16)
#define OB3_26 (FLD36 | FLD31 | FLD27 | FLD26 | FLD21)
#define OB3_27 (FLD36 | FLD31 | FLD27 | FLD26 | FLD21)
#define OB3_28 (FLD36 | FLD31 | FLD26)
#define OB3_29 (FLD26 | FLD21 | FLD16)
#define OB4_1 (FLD35 | FLD15 | FLD30)
#define OB4_2 (FLD35 | FLD15)
#define OB4_3 
#define OB4_4 
#define OB4_5 
#define OB4_6 (FLD25 | FLD5 | FLD20)
#define OB4_7 (FLD25 | FLD5)
#define OB4_8 
#define OB4_9 
#define OB4_10 
#define OB4_11 
#define OB4_12 
#define OB4_13 
#define OB4_14 
#define OB4_15 (FLD36)
#define OB4_16 
#define OB4_17 
#define OB4_18 
#define OB4_19 
#define OB4_20 (FLD26)
#define OB4_21 
#define OB4_22 
#define OB4_23 
#define OB4_24 (FLD6)
#define OB4_25 (FLD16)
#define OB4_26 
#define OB4_27 
#define OB4_28 
#define OB4_29 

