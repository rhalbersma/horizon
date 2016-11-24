/*
  This file is Copyright (C) 2010 Feike Boomstra.
  Distributed under the Boost Software License, Version 1.0.
  (See accompanying file LICENSE_1_0.txt or copy at
  http://www.boost.org/LICENSE_1_0.txt)
*/


#include "BoomstraDam.v2.5.h"


// patterns for white rows

#define D1_W (FLD16 | FLD11 | FLD7)
#define D4_W (FLD17 | FLD12 | FLD8)
#define D9_W (FLD18 | FLD13 | FLD9)
#define D16_W (FLD19 | FLD14 | FLD10)
#define D2_W (FLD26 | FLD21 | FLD17)
#define D7_W (FLD27 | FLD22 | FLD18)
#define D14_W (FLD28 | FLD23 | FLD19)
#define D21_W (FLD29 | FLD24 | FLD20)
#define D5_W (FLD36 | FLD31 | FLD27)
#define D12_W (FLD37 | FLD32 | FLD28)
#define D19_W (FLD38 | FLD33 | FLD29)
#define D25_W (FLD39 | FLD34 | FLD30)
#define D10_W (FLD46 | FLD41 | FLD37)
#define D17_W (FLD47 | FLD42 | FLD38)
#define D23_W (FLD48 | FLD43 | FLD39)
#define D27_W (FLD49 | FLD44 | FLD40)
#define D3_W (FLD21 | FLD17 | FLD12)
#define D8_W (FLD22 | FLD18 | FLD13)
#define D15_W (FLD23 | FLD19 | FLD14)
#define D22_W (FLD24 | FLD20 | FLD15)
#define D6_W (FLD31 | FLD27 | FLD22)
#define D13_W (FLD32 | FLD28 | FLD23)
#define D20_W (FLD33 | FLD29 | FLD24)
#define D26_W (FLD34 | FLD30 | FLD25)
#define D11_W (FLD41 | FLD37 | FLD32)
#define D18_W (FLD42 | FLD38 | FLD33)
#define D24_W (FLD43 | FLD39 | FLD34)
#define D28_W (FLD44 | FLD40 | FLD35)
#define D41_W (FLD6 | FLD11 | FLD17)
#define D35_W (FLD7 | FLD12 | FLD18)
#define D31_W (FLD8 | FLD13 | FLD19)
#define D29_W (FLD9 | FLD14 | FLD20)
#define D48_W (FLD16 | FLD21 | FLD27)
#define D43_W (FLD17 | FLD22 | FLD28)
#define D37_W (FLD18 | FLD23 | FLD29)
#define D33_W (FLD19 | FLD24 | FLD30)
#define D53_W (FLD26 | FLD31 | FLD37)
#define D50_W (FLD27 | FLD32 | FLD38)
#define D45_W (FLD28 | FLD33 | FLD39)
#define D39_W (FLD29 | FLD34 | FLD40)
#define D56_W (FLD36 | FLD41 | FLD47)
#define D55_W (FLD37 | FLD42 | FLD48)
#define D52_W (FLD38 | FLD43 | FLD49)
#define D47_W (FLD39 | FLD44 | FLD50)
#define D42_W (FLD11 | FLD17 | FLD22)
#define D36_W (FLD12 | FLD18 | FLD23)
#define D32_W (FLD13 | FLD19 | FLD24)
#define D30_W (FLD14 | FLD20 | FLD25)
#define D49_W (FLD21 | FLD27 | FLD32)
#define D44_W (FLD22 | FLD28 | FLD33)
#define D38_W (FLD23 | FLD29 | FLD34)
#define D34_W (FLD24 | FLD30 | FLD35)
#define D54_W (FLD31 | FLD37 | FLD42)
#define D51_W (FLD32 | FLD38 | FLD43)
#define D46_W (FLD33 | FLD39 | FLD44)
#define D40_W (FLD34 | FLD40 | FLD45)
#define V1_W (FLD26 | FLD21 | FLD17 | FLD12)
#define V5_W (FLD27 | FLD22 | FLD18 | FLD13)
#define V11_W (FLD28 | FLD23 | FLD19 | FLD14)
#define V17_W (FLD29 | FLD24 | FLD20 | FLD15)
#define V3_W (FLD36 | FLD31 | FLD27 | FLD22)
#define V9_W (FLD37 | FLD32 | FLD28 | FLD23)
#define V15_W (FLD38 | FLD33 | FLD29 | FLD24)
#define V20_W (FLD39 | FLD34 | FLD30 | FLD25)
#define V7_W (FLD46 | FLD41 | FLD37 | FLD32)
#define V13_W (FLD47 | FLD42 | FLD38 | FLD33)
#define V18_W (FLD48 | FLD43 | FLD39 | FLD34)
#define V21_W (FLD49 | FLD44 | FLD40 | FLD35)
#define V2_W (FLD21 | FLD17 | FLD12 | FLD8)
#define V6_W (FLD22 | FLD18 | FLD13 | FLD9)
#define V12_W (FLD23 | FLD19 | FLD14 | FLD10)
#define V4_W (FLD31 | FLD27 | FLD22 | FLD18)
#define V10_W (FLD32 | FLD28 | FLD23 | FLD19)
#define V16_W (FLD33 | FLD29 | FLD24 | FLD20)
#define V8_W (FLD41 | FLD37 | FLD32 | FLD28)
#define V14_W (FLD42 | FLD38 | FLD33 | FLD29)
#define V19_W (FLD43 | FLD39 | FLD34 | FLD30)
#define V31_W (FLD6 | FLD11 | FLD17 | FLD22)
#define V26_W (FLD7 | FLD12 | FLD18 | FLD23)
#define V23_W (FLD8 | FLD13 | FLD19 | FLD24)
#define V22_W (FLD9 | FLD14 | FLD20 | FLD25)
#define V37_W (FLD16 | FLD21 | FLD27 | FLD32)
#define V33_W (FLD17 | FLD22 | FLD28 | FLD33)
#define V28_W (FLD18 | FLD23 | FLD29 | FLD34)
#define V25_W (FLD19 | FLD24 | FLD30 | FLD35)
#define V41_W (FLD26 | FLD31 | FLD37 | FLD42)
#define V39_W (FLD27 | FLD32 | FLD38 | FLD43)
#define V35_W (FLD28 | FLD33 | FLD39 | FLD44)
#define V30_W (FLD29 | FLD34 | FLD40 | FLD45)
#define V32_W (FLD11 | FLD17 | FLD22 | FLD28)
#define V27_W (FLD12 | FLD18 | FLD23 | FLD29)
#define V24_W (FLD13 | FLD19 | FLD24 | FLD30)
#define V38_W (FLD21 | FLD27 | FLD32 | FLD38)
#define V34_W (FLD22 | FLD28 | FLD33 | FLD39)
#define V29_W (FLD23 | FLD29 | FLD34 | FLD40)
#define V42_W (FLD31 | FLD37 | FLD42 | FLD48)
#define V40_W (FLD32 | FLD38 | FLD43 | FLD49)
#define V36_W (FLD33 | FLD39 | FLD44 | FLD50)
#define F1_W (FLD26 | FLD21 | FLD17 | FLD12 | FLD8)
#define F4_W (FLD27 | FLD22 | FLD18 | FLD13 | FLD9)
#define F9_W (FLD28 | FLD23 | FLD19 | FLD14 | FLD10)
#define F2_W (FLD36 | FLD31 | FLD27 | FLD22 | FLD18)
#define F7_W (FLD37 | FLD32 | FLD28 | FLD23 | FLD19)
#define F12_W (FLD38 | FLD33 | FLD29 | FLD24 | FLD20)
#define F5_W (FLD46 | FLD41 | FLD37 | FLD32 | FLD28)
#define F10_W (FLD47 | FLD42 | FLD38 | FLD33 | FLD29)
#define F14_W (FLD48 | FLD43 | FLD39 | FLD34 | FLD30)
#define F3_W (FLD31 | FLD27 | FLD22 | FLD18 | FLD13)
#define F8_W (FLD32 | FLD28 | FLD23 | FLD19 | FLD14)
#define F13_W (FLD33 | FLD29 | FLD24 | FLD20 | FLD15)
#define F6_W (FLD41 | FLD37 | FLD32 | FLD28 | FLD23)
#define F11_W (FLD42 | FLD38 | FLD33 | FLD29 | FLD24)
#define F15_W (FLD43 | FLD39 | FLD34 | FLD30 | FLD25)
#define F22_W (FLD6 | FLD11 | FLD17 | FLD22 | FLD28)
#define F18_W (FLD7 | FLD12 | FLD18 | FLD23 | FLD29)
#define F16_W (FLD8 | FLD13 | FLD19 | FLD24 | FLD30)
#define F27_W (FLD16 | FLD21 | FLD27 | FLD32 | FLD38)
#define F24_W (FLD17 | FLD22 | FLD28 | FLD33 | FLD39)
#define F20_W (FLD18 | FLD23 | FLD29 | FLD34 | FLD40)
#define F30_W (FLD26 | FLD31 | FLD37 | FLD42 | FLD48)
#define F29_W (FLD27 | FLD32 | FLD38 | FLD43 | FLD49)
#define F26_W (FLD28 | FLD33 | FLD39 | FLD44 | FLD50)
#define F23_W (FLD11 | FLD17 | FLD22 | FLD28 | FLD33)
#define F19_W (FLD12 | FLD18 | FLD23 | FLD29 | FLD34)
#define F17_W (FLD13 | FLD19 | FLD24 | FLD30 | FLD35)
#define F28_W (FLD21 | FLD27 | FLD32 | FLD38 | FLD43)
#define F25_W (FLD22 | FLD28 | FLD33 | FLD39 | FLD44)
#define F21_W (FLD23 | FLD29 | FLD34 | FLD40 | FLD45)
#define Z2_W (FLD31 | FLD27 | FLD22 | FLD18 | FLD13 | FLD9)
#define Z6_W (FLD32 | FLD28 | FLD23 | FLD19 | FLD14 | FLD10)
#define Z4_W (FLD41 | FLD37 | FLD32 | FLD28 | FLD23 | FLD19)
#define Z8_W (FLD42 | FLD38 | FLD33 | FLD29 | FLD24 | FLD20)
#define Z1_W (FLD36 | FLD31 | FLD27 | FLD22 | FLD18 | FLD13)
#define Z5_W (FLD37 | FLD32 | FLD28 | FLD23 | FLD19 | FLD14)
#define Z9_W (FLD38 | FLD33 | FLD29 | FLD24 | FLD20 | FLD15)
#define Z3_W (FLD46 | FLD41 | FLD37 | FLD32 | FLD28 | FLD23)
#define Z7_W (FLD47 | FLD42 | FLD38 | FLD33 | FLD29 | FLD24)
#define Z10_W (FLD48 | FLD43 | FLD39 | FLD34 | FLD30 | FLD25)
#define Z15_W (FLD6 | FLD11 | FLD17 | FLD22 | FLD28 | FLD33)
#define Z12_W (FLD7 | FLD12 | FLD18 | FLD23 | FLD29 | FLD34)
#define Z11_W (FLD8 | FLD13 | FLD19 | FLD24 | FLD30 | FLD35)
#define Z19_W (FLD16 | FLD21 | FLD27 | FLD32 | FLD38 | FLD43)
#define Z17_W (FLD17 | FLD22 | FLD28 | FLD33 | FLD39 | FLD44)
#define Z14_W (FLD18 | FLD23 | FLD29 | FLD34 | FLD40 | FLD45)
#define Z16_W (FLD11 | FLD17 | FLD22 | FLD28 | FLD33 | FLD39)
#define Z13_W (FLD12 | FLD18 | FLD23 | FLD29 | FLD34 | FLD40)
#define Z20_W (FLD21 | FLD27 | FLD32 | FLD38 | FLD43 | FLD49)
#define Z18_W (FLD22 | FLD28 | FLD33 | FLD39 | FLD44 | FLD50)

// black rows

#define D1_B (FLD35 | FLD40 | FLD44)
#define D4_B (FLD34 | FLD39 | FLD43)
#define D9_B (FLD33 | FLD38 | FLD42)
#define D16_B (FLD32 | FLD37 | FLD41)
#define D2_B (FLD25 | FLD30 | FLD34)
#define D7_B (FLD24 | FLD29 | FLD33)
#define D14_B (FLD23 | FLD28 | FLD32)
#define D21_B (FLD22 | FLD27 | FLD31)
#define D5_B (FLD15 | FLD20 | FLD24)
#define D12_B (FLD14 | FLD19 | FLD23)
#define D19_B (FLD13 | FLD18 | FLD22)
#define D25_B (FLD12 | FLD17 | FLD21)
#define D10_B (FLD5 | FLD10 | FLD14)
#define D17_B (FLD4 | FLD9 | FLD13)
#define D23_B (FLD3 | FLD8 | FLD12)
#define D27_B (FLD2 | FLD7 | FLD11)
#define D3_B (FLD30 | FLD34 | FLD39)
#define D8_B (FLD29 | FLD33 | FLD38)
#define D15_B (FLD28 | FLD32 | FLD37)
#define D22_B (FLD27 | FLD31 | FLD36)
#define D6_B (FLD20 | FLD24 | FLD29)
#define D13_B (FLD19 | FLD23 | FLD28)
#define D20_B (FLD18 | FLD22 | FLD27)
#define D26_B (FLD17 | FLD21 | FLD26)
#define D11_B (FLD10 | FLD14 | FLD19)
#define D18_B (FLD9 | FLD13 | FLD18)
#define D24_B (FLD8 | FLD12 | FLD17)
#define D28_B (FLD7 | FLD11 | FLD16)
#define D41_B (FLD45 | FLD40 | FLD34)
#define D35_B (FLD44 | FLD39 | FLD33)
#define D31_B (FLD43 | FLD38 | FLD32)
#define D29_B (FLD42 | FLD37 | FLD31)
#define D48_B (FLD35 | FLD30 | FLD24)
#define D43_B (FLD34 | FLD29 | FLD23)
#define D37_B (FLD33 | FLD28 | FLD22)
#define D33_B (FLD32 | FLD27 | FLD21)
#define D53_B (FLD25 | FLD20 | FLD14)
#define D50_B (FLD24 | FLD19 | FLD13)
#define D45_B (FLD23 | FLD18 | FLD12)
#define D39_B (FLD22 | FLD17 | FLD11)
#define D56_B (FLD15 | FLD10 | FLD4)
#define D55_B (FLD14 | FLD9 | FLD3)
#define D52_B (FLD13 | FLD8 | FLD2)
#define D47_B (FLD12 | FLD7 | FLD1)
#define D42_B (FLD40 | FLD34 | FLD29)
#define D36_B (FLD39 | FLD33 | FLD28)
#define D32_B (FLD38 | FLD32 | FLD27)
#define D30_B (FLD37 | FLD31 | FLD26)
#define D49_B (FLD30 | FLD24 | FLD19)
#define D44_B (FLD29 | FLD23 | FLD18)
#define D38_B (FLD28 | FLD22 | FLD17)
#define D34_B (FLD27 | FLD21 | FLD16)
#define D54_B (FLD20 | FLD14 | FLD9)
#define D51_B (FLD19 | FLD13 | FLD8)
#define D46_B (FLD18 | FLD12 | FLD7)
#define D40_B (FLD17 | FLD11 | FLD6)
#define V1_B (FLD25 | FLD30 | FLD34 | FLD39)
#define V5_B (FLD24 | FLD29 | FLD33 | FLD38)
#define V11_B (FLD23 | FLD28 | FLD32 | FLD37)
#define V17_B (FLD22 | FLD27 | FLD31 | FLD36)
#define V3_B (FLD15 | FLD20 | FLD24 | FLD29)
#define V9_B (FLD14 | FLD19 | FLD23 | FLD28)
#define V15_B (FLD13 | FLD18 | FLD22 | FLD27)
#define V20_B (FLD12 | FLD17 | FLD21 | FLD26)
#define V7_B (FLD5 | FLD10 | FLD14 | FLD19)
#define V13_B (FLD4 | FLD9 | FLD13 | FLD18)
#define V18_B (FLD3 | FLD8 | FLD12 | FLD17)
#define V21_B (FLD2 | FLD7 | FLD11 | FLD16)
#define V2_B (FLD30 | FLD34 | FLD39 | FLD43)
#define V6_B (FLD29 | FLD33 | FLD38 | FLD42)
#define V12_B (FLD28 | FLD32 | FLD37 | FLD41)
#define V4_B (FLD20 | FLD24 | FLD29 | FLD33)
#define V10_B (FLD19 | FLD23 | FLD28 | FLD32)
#define V16_B (FLD18 | FLD22 | FLD27 | FLD31)
#define V8_B (FLD10 | FLD14 | FLD19 | FLD23)
#define V14_B (FLD9 | FLD13 | FLD18 | FLD22)
#define V19_B (FLD8 | FLD12 | FLD17 | FLD21)
#define V31_B (FLD45 | FLD40 | FLD34 | FLD29)
#define V26_B (FLD44 | FLD39 | FLD33 | FLD28)
#define V23_B (FLD43 | FLD38 | FLD32 | FLD27)
#define V22_B (FLD42 | FLD37 | FLD31 | FLD26)
#define V37_B (FLD35 | FLD30 | FLD24 | FLD19)
#define V33_B (FLD34 | FLD29 | FLD23 | FLD18)
#define V28_B (FLD33 | FLD28 | FLD22 | FLD17)
#define V25_B (FLD32 | FLD27 | FLD21 | FLD16)
#define V41_B (FLD25 | FLD20 | FLD14 | FLD9)
#define V39_B (FLD24 | FLD19 | FLD13 | FLD8)
#define V35_B (FLD23 | FLD18 | FLD12 | FLD7)
#define V30_B (FLD22 | FLD17 | FLD11 | FLD6)
#define V32_B (FLD40 | FLD34 | FLD29 | FLD23)
#define V27_B (FLD39 | FLD33 | FLD28 | FLD22)
#define V24_B (FLD38 | FLD32 | FLD27 | FLD21)
#define V38_B (FLD30 | FLD24 | FLD19 | FLD13)
#define V34_B (FLD29 | FLD23 | FLD18 | FLD12)
#define V29_B (FLD28 | FLD22 | FLD17 | FLD11)
#define V42_B (FLD20 | FLD14 | FLD9 | FLD3)
#define V40_B (FLD19 | FLD13 | FLD8 | FLD2)
#define V36_B (FLD18 | FLD12 | FLD7 | FLD1)
#define F1_B (FLD25 | FLD30 | FLD34 | FLD39 | FLD43)
#define F4_B (FLD24 | FLD29 | FLD33 | FLD38 | FLD42)
#define F9_B (FLD23 | FLD28 | FLD32 | FLD37 | FLD41)
#define F2_B (FLD15 | FLD20 | FLD24 | FLD29 | FLD33)
#define F7_B (FLD14 | FLD19 | FLD23 | FLD28 | FLD32)
#define F12_B (FLD13 | FLD18 | FLD22 | FLD27 | FLD31)
#define F5_B (FLD5 | FLD10 | FLD14 | FLD19 | FLD23)
#define F10_B (FLD4 | FLD9 | FLD13 | FLD18 | FLD22)
#define F14_B (FLD3 | FLD8 | FLD12 | FLD17 | FLD21)
#define F3_B (FLD20 | FLD24 | FLD29 | FLD33 | FLD38)
#define F8_B (FLD19 | FLD23 | FLD28 | FLD32 | FLD37)
#define F13_B (FLD18 | FLD22 | FLD27 | FLD31 | FLD36)
#define F6_B (FLD10 | FLD14 | FLD19 | FLD23 | FLD28)
#define F11_B (FLD9 | FLD13 | FLD18 | FLD22 | FLD27)
#define F15_B (FLD8 | FLD12 | FLD17 | FLD21 | FLD26)
#define F22_B (FLD45 | FLD40 | FLD34 | FLD29 | FLD23)
#define F18_B (FLD44 | FLD39 | FLD33 | FLD28 | FLD22)
#define F16_B (FLD43 | FLD38 | FLD32 | FLD27 | FLD21)
#define F27_B (FLD35 | FLD30 | FLD24 | FLD19 | FLD13)
#define F24_B (FLD34 | FLD29 | FLD23 | FLD18 | FLD12)
#define F20_B (FLD33 | FLD28 | FLD22 | FLD17 | FLD11)
#define F30_B (FLD25 | FLD20 | FLD14 | FLD9 | FLD3)
#define F29_B (FLD24 | FLD19 | FLD13 | FLD8 | FLD2)
#define F26_B (FLD23 | FLD18 | FLD12 | FLD7 | FLD1)
#define F23_B (FLD40 | FLD34 | FLD29 | FLD23 | FLD18)
#define F19_B (FLD39 | FLD33 | FLD28 | FLD22 | FLD17)
#define F17_B (FLD38 | FLD32 | FLD27 | FLD21 | FLD16)
#define F28_B (FLD30 | FLD24 | FLD19 | FLD13 | FLD8)
#define F25_B (FLD29 | FLD23 | FLD18 | FLD12 | FLD7)
#define F21_B (FLD28 | FLD22 | FLD17 | FLD11 | FLD6)
#define Z2_B (FLD20 | FLD24 | FLD29 | FLD33 | FLD38 | FLD42)
#define Z6_B (FLD19 | FLD23 | FLD28 | FLD32 | FLD37 | FLD41)
#define Z4_B (FLD10 | FLD14 | FLD19 | FLD23 | FLD28 | FLD32)
#define Z8_B (FLD9 | FLD13 | FLD18 | FLD22 | FLD27 | FLD31)
#define Z1_B (FLD15 | FLD20 | FLD24 | FLD29 | FLD33 | FLD38)
#define Z5_B (FLD14 | FLD19 | FLD23 | FLD28 | FLD32 | FLD37)
#define Z9_B (FLD13 | FLD18 | FLD22 | FLD27 | FLD31 | FLD36)
#define Z3_B (FLD5 | FLD10 | FLD14 | FLD19 | FLD23 | FLD28)
#define Z7_B (FLD4 | FLD9 | FLD13 | FLD18 | FLD22 | FLD27)
#define Z10_B (FLD3 | FLD8 | FLD12 | FLD17 | FLD21 | FLD26)
#define Z15_B (FLD45 | FLD40 | FLD34 | FLD29 | FLD23 | FLD18)
#define Z12_B (FLD44 | FLD39 | FLD33 | FLD28 | FLD22 | FLD17)
#define Z11_B (FLD43 | FLD38 | FLD32 | FLD27 | FLD21 | FLD16)
#define Z19_B (FLD35 | FLD30 | FLD24 | FLD19 | FLD13 | FLD8)
#define Z17_B (FLD34 | FLD29 | FLD23 | FLD18 | FLD12 | FLD7)
#define Z14_B (FLD33 | FLD28 | FLD22 | FLD17 | FLD11 | FLD6)
#define Z16_B (FLD40 | FLD34 | FLD29 | FLD23 | FLD18 | FLD12)
#define Z13_B (FLD39 | FLD33 | FLD28 | FLD22 | FLD17 | FLD11)
#define Z20_B (FLD30 | FLD24 | FLD19 | FLD13 | FLD8 | FLD2)
#define Z18_B (FLD29 | FLD23 | FLD18 | FLD12 | FLD7 | FLD1)

// bitmaps for various rows

#define BM3_D1 (BIT1)
#define BM3_D4 (BIT4)
#define BM3_D9 (BIT9)
#define BM3_D16 (BIT16)
#define BM3_D2 (BIT2)
#define BM3_D7 (BIT7)
#define BM3_D14 (BIT14)
#define BM3_D21 (BIT21)
#define BM3_D5 (BIT5)
#define BM3_D12 (BIT12)
#define BM3_D19 (BIT19)
#define BM3_D25 (BIT25)
#define BM3_D10 (BIT10)
#define BM3_D17 (BIT17)
#define BM3_D23 (BIT23)
#define BM3_D27 (BIT27)
#define BM3_D3 (BIT3)
#define BM3_D8 (BIT8)
#define BM3_D15 (BIT15)
#define BM3_D22 (BIT22)
#define BM3_D6 (BIT6)
#define BM3_D13 (BIT13)
#define BM3_D20 (BIT20)
#define BM3_D26 (BIT26)
#define BM3_D11 (BIT11)
#define BM3_D18 (BIT18)
#define BM3_D24 (BIT24)
#define BM3_D28 (BIT28)
#define BM3_D41 (BIT41)
#define BM3_D35 (BIT35)
#define BM3_D31 (BIT31)
#define BM3_D29 (BIT29)
#define BM3_D48 (BIT48)
#define BM3_D43 (BIT43)
#define BM3_D37 (BIT37)
#define BM3_D33 (BIT33)
#define BM3_D53 (BIT53)
#define BM3_D50 (BIT50)
#define BM3_D45 (BIT45)
#define BM3_D39 (BIT39)
#define BM3_D56 (BIT56)
#define BM3_D55 (BIT55)
#define BM3_D52 (BIT52)
#define BM3_D47 (BIT47)
#define BM3_D42 (BIT42)
#define BM3_D36 (BIT36)
#define BM3_D32 (BIT32)
#define BM3_D30 (BIT30)
#define BM3_D49 (BIT49)
#define BM3_D44 (BIT44)
#define BM3_D38 (BIT38)
#define BM3_D34 (BIT34)
#define BM3_D54 (BIT54)
#define BM3_D51 (BIT51)
#define BM3_D46 (BIT46)
#define BM3_D40 (BIT40)
#define BM4_V1 (BIT1)
#define BM3_V1 (BIT2 |  BIT3)
#define BM4_V5 (BIT5)
#define BM3_V5 (BIT7 |  BIT8)
#define BM4_V11 (BIT11)
#define BM3_V11 (BIT14 |  BIT15)
#define BM4_V17 (BIT17)
#define BM3_V17 (BIT21 |  BIT22)
#define BM4_V3 (BIT3)
#define BM3_V3 (BIT5 |  BIT6)
#define BM4_V9 (BIT9)
#define BM3_V9 (BIT12 |  BIT13)
#define BM4_V15 (BIT15)
#define BM3_V15 (BIT19 |  BIT20)
#define BM4_V20 (BIT20)
#define BM3_V20 (BIT25 |  BIT26)
#define BM4_V7 (BIT7)
#define BM3_V7 (BIT10 |  BIT11)
#define BM4_V13 (BIT13)
#define BM3_V13 (BIT17 |  BIT18)
#define BM4_V18 (BIT18)
#define BM3_V18 (BIT23 |  BIT24)
#define BM4_V21 (BIT21)
#define BM3_V21 (BIT27 |  BIT28)
#define BM4_V2 (BIT2)
#define BM3_V2 (BIT3 |  BIT4)
#define BM4_V6 (BIT6)
#define BM3_V6 (BIT8 |  BIT9)
#define BM4_V12 (BIT12)
#define BM3_V12 (BIT15 |  BIT16)
#define BM4_V4 (BIT4)
#define BM3_V4 (BIT6 |  BIT7)
#define BM4_V10 (BIT10)
#define BM3_V10 (BIT13 |  BIT14)
#define BM4_V16 (BIT16)
#define BM3_V16 (BIT20 |  BIT21)
#define BM4_V8 (BIT8)
#define BM3_V8 (BIT11 |  BIT12)
#define BM4_V14 (BIT14)
#define BM3_V14 (BIT18 |  BIT19)
#define BM4_V19 (BIT19)
#define BM3_V19 (BIT24 |  BIT25)
#define BM4_V31 (BIT31)
#define BM3_V31 (BIT41 |  BIT42)
#define BM4_V26 (BIT26)
#define BM3_V26 (BIT35 |  BIT36)
#define BM4_V23 (BIT23)
#define BM3_V23 (BIT31 |  BIT32)
#define BM4_V22 (BIT22)
#define BM3_V22 (BIT29 |  BIT30)
#define BM4_V37 (BIT37)
#define BM3_V37 (BIT48 |  BIT49)
#define BM4_V33 (BIT33)
#define BM3_V33 (BIT43 |  BIT44)
#define BM4_V28 (BIT28)
#define BM3_V28 (BIT37 |  BIT38)
#define BM4_V25 (BIT25)
#define BM3_V25 (BIT33 |  BIT34)
#define BM4_V41 (BIT41)
#define BM3_V41 (BIT53 |  BIT54)
#define BM4_V39 (BIT39)
#define BM3_V39 (BIT50 |  BIT51)
#define BM4_V35 (BIT35)
#define BM3_V35 (BIT45 |  BIT46)
#define BM4_V30 (BIT30)
#define BM3_V30 (BIT39 |  BIT40)
#define BM4_V32 (BIT32)
#define BM3_V32 (BIT42 |  BIT43)
#define BM4_V27 (BIT27)
#define BM3_V27 (BIT36 |  BIT37)
#define BM4_V24 (BIT24)
#define BM3_V24 (BIT32 |  BIT33)
#define BM4_V38 (BIT38)
#define BM3_V38 (BIT49 |  BIT50)
#define BM4_V34 (BIT34)
#define BM3_V34 (BIT44 |  BIT45)
#define BM4_V29 (BIT29)
#define BM3_V29 (BIT38 |  BIT39)
#define BM4_V42 (BIT42)
#define BM3_V42 (BIT54 |  BIT55)
#define BM4_V40 (BIT40)
#define BM3_V40 (BIT51 |  BIT52)
#define BM4_V36 (BIT36)
#define BM3_V36 (BIT46 |  BIT47)
#define BM5_F1 (BIT1)
#define BM4_F1 (BIT1 |  BIT2)
#define BM3_F1 (BIT2 |  BIT3 |  BIT4)
#define BM5_F4 (BIT4)
#define BM4_F4 (BIT5 |  BIT6)
#define BM3_F4 (BIT7 |  BIT8 |  BIT9)
#define BM5_F9 (BIT9)
#define BM4_F9 (BIT11 |  BIT12)
#define BM3_F9 (BIT14 |  BIT15 |  BIT16)
#define BM5_F2 (BIT2)
#define BM4_F2 (BIT3 |  BIT4)
#define BM3_F2 (BIT5 |  BIT6 |  BIT7)
#define BM5_F7 (BIT7)
#define BM4_F7 (BIT9 |  BIT10)
#define BM3_F7 (BIT12 |  BIT13 |  BIT14)
#define BM5_F12 (BIT12)
#define BM4_F12 (BIT15 |  BIT16)
#define BM3_F12 (BIT19 |  BIT20 |  BIT21)
#define BM5_F5 (BIT5)
#define BM4_F5 (BIT7 |  BIT8)
#define BM3_F5 (BIT10 |  BIT11 |  BIT12)
#define BM5_F10 (BIT10)
#define BM4_F10 (BIT13 |  BIT14)
#define BM3_F10 (BIT17 |  BIT18 |  BIT19)
#define BM5_F14 (BIT14)
#define BM4_F14 (BIT18 |  BIT19)
#define BM3_F14 (BIT23 |  BIT24 |  BIT25)
#define BM5_F3 (BIT3)
#define BM4_F3 (BIT4 |  BIT5)
#define BM3_F3 (BIT6 |  BIT7 |  BIT8)
#define BM5_F8 (BIT8)
#define BM4_F8 (BIT10 |  BIT11)
#define BM3_F8 (BIT13 |  BIT14 |  BIT15)
#define BM5_F13 (BIT13)
#define BM4_F13 (BIT16 |  BIT17)
#define BM3_F13 (BIT20 |  BIT21 |  BIT22)
#define BM5_F6 (BIT6)
#define BM4_F6 (BIT8 |  BIT9)
#define BM3_F6 (BIT11 |  BIT12 |  BIT13)
#define BM5_F11 (BIT11)
#define BM4_F11 (BIT14 |  BIT15)
#define BM3_F11 (BIT18 |  BIT19 |  BIT20)
#define BM5_F15 (BIT15)
#define BM4_F15 (BIT19 |  BIT20)
#define BM3_F15 (BIT24 |  BIT25 |  BIT26)
#define BM5_F22 (BIT22)
#define BM4_F22 (BIT31 |  BIT32)
#define BM3_F22 (BIT41 |  BIT42 |  BIT43)
#define BM5_F18 (BIT18)
#define BM4_F18 (BIT26 |  BIT27)
#define BM3_F18 (BIT35 |  BIT36 |  BIT37)
#define BM5_F16 (BIT16)
#define BM4_F16 (BIT23 |  BIT24)
#define BM3_F16 (BIT31 |  BIT32 |  BIT33)
#define BM5_F27 (BIT27)
#define BM4_F27 (BIT37 |  BIT38)
#define BM3_F27 (BIT48 |  BIT49 |  BIT50)
#define BM5_F24 (BIT24)
#define BM4_F24 (BIT33 |  BIT34)
#define BM3_F24 (BIT43 |  BIT44 |  BIT45)
#define BM5_F20 (BIT20)
#define BM4_F20 (BIT28 |  BIT29)
#define BM3_F20 (BIT37 |  BIT38 |  BIT39)
#define BM5_F30 (BIT30)
#define BM4_F30 (BIT41 |  BIT42)
#define BM3_F30 (BIT53 |  BIT54 |  BIT55)
#define BM5_F29 (BIT29)
#define BM4_F29 (BIT39 |  BIT40)
#define BM3_F29 (BIT50 |  BIT51 |  BIT52)
#define BM5_F26 (BIT26)
#define BM4_F26 (BIT35 |  BIT36)
#define BM3_F26 (BIT45 |  BIT46 |  BIT47)
#define BM5_F23 (BIT23)
#define BM4_F23 (BIT32 |  BIT33)
#define BM3_F23 (BIT42 |  BIT43 |  BIT44)
#define BM5_F19 (BIT19)
#define BM4_F19 (BIT27 |  BIT28)
#define BM3_F19 (BIT36 |  BIT37 |  BIT38)
#define BM5_F17 (BIT17)
#define BM4_F17 (BIT24 |  BIT25)
#define BM3_F17 (BIT32 |  BIT33 |  BIT34)
#define BM5_F28 (BIT28)
#define BM4_F28 (BIT38 |  BIT39)
#define BM3_F28 (BIT49 |  BIT50 |  BIT51)
#define BM5_F25 (BIT25)
#define BM4_F25 (BIT34 |  BIT35)
#define BM3_F25 (BIT44 |  BIT45 |  BIT46)
#define BM5_F21 (BIT21)
#define BM4_F21 (BIT29 |  BIT30)
#define BM3_F21 (BIT38 |  BIT39 |  BIT40)
#define BM6_Z2 (BIT2)
#define BM5_Z2 (BIT3 |  BIT4)
#define BM4_Z2 (BIT4 |  BIT5 |  BIT6)
#define BM3_Z2 (BIT6 |  BIT7 |  BIT8 |  BIT9)
#define BM6_Z6 (BIT6)
#define BM5_Z6 (BIT8 |  BIT9)
#define BM4_Z6 (BIT10 |  BIT11 |  BIT12)
#define BM3_Z6 (BIT13 |  BIT14 |  BIT15 |  BIT16)
#define BM6_Z4 (BIT4)
#define BM5_Z4 (BIT6 |  BIT7)
#define BM4_Z4 (BIT8 |  BIT9 |  BIT10)
#define BM3_Z4 (BIT11 |  BIT12 |  BIT13 |  BIT14)
#define BM6_Z8 (BIT8)
#define BM5_Z8 (BIT11 |  BIT12)
#define BM4_Z8 (BIT14 |  BIT15 |  BIT16)
#define BM3_Z8 (BIT18 |  BIT19 |  BIT20 |  BIT21)
#define BM6_Z1 (BIT1)
#define BM5_Z1 (BIT2 |  BIT3)
#define BM4_Z1 (BIT3 |  BIT4 |  BIT5)
#define BM3_Z1 (BIT5 |  BIT6 |  BIT7 |  BIT8)
#define BM6_Z5 (BIT5)
#define BM5_Z5 (BIT7 |  BIT8)
#define BM4_Z5 (BIT9 |  BIT10 |  BIT11)
#define BM3_Z5 (BIT12 |  BIT13 |  BIT14 |  BIT15)
#define BM6_Z9 (BIT9)
#define BM5_Z9 (BIT12 |  BIT13)
#define BM4_Z9 (BIT15 |  BIT16 |  BIT17)
#define BM3_Z9 (BIT19 |  BIT20 |  BIT21 |  BIT22)
#define BM6_Z3 (BIT3)
#define BM5_Z3 (BIT5 |  BIT6)
#define BM4_Z3 (BIT7 |  BIT8 |  BIT9)
#define BM3_Z3 (BIT10 |  BIT11 |  BIT12 |  BIT13)
#define BM6_Z7 (BIT7)
#define BM5_Z7 (BIT10 |  BIT11)
#define BM4_Z7 (BIT13 |  BIT14 |  BIT15)
#define BM3_Z7 (BIT17 |  BIT18 |  BIT19 |  BIT20)
#define BM6_Z10 (BIT10)
#define BM5_Z10 (BIT14 |  BIT15)
#define BM4_Z10 (BIT18 |  BIT19 |  BIT20)
#define BM3_Z10 (BIT23 |  BIT24 |  BIT25 |  BIT26)
#define BM6_Z15 (BIT15)
#define BM5_Z15 (BIT22 |  BIT23)
#define BM4_Z15 (BIT31 |  BIT32 |  BIT33)
#define BM3_Z15 (BIT41 |  BIT42 |  BIT43 |  BIT44)
#define BM6_Z12 (BIT12)
#define BM5_Z12 (BIT18 |  BIT19)
#define BM4_Z12 (BIT26 |  BIT27 |  BIT28)
#define BM3_Z12 (BIT35 |  BIT36 |  BIT37 |  BIT38)
#define BM6_Z11 (BIT11)
#define BM5_Z11 (BIT16 |  BIT17)
#define BM4_Z11 (BIT23 |  BIT24 |  BIT25)
#define BM3_Z11 (BIT31 |  BIT32 |  BIT33 |  BIT34)
#define BM6_Z19 (BIT19)
#define BM5_Z19 (BIT27 |  BIT28)
#define BM4_Z19 (BIT37 |  BIT38 |  BIT39)
#define BM3_Z19 (BIT48 |  BIT49 |  BIT50 |  BIT51)
#define BM6_Z17 (BIT17)
#define BM5_Z17 (BIT24 |  BIT25)
#define BM4_Z17 (BIT33 |  BIT34 |  BIT35)
#define BM3_Z17 (BIT43 |  BIT44 |  BIT45 |  BIT46)
#define BM6_Z14 (BIT14)
#define BM5_Z14 (BIT20 |  BIT21)
#define BM4_Z14 (BIT28 |  BIT29 |  BIT30)
#define BM3_Z14 (BIT37 |  BIT38 |  BIT39 |  BIT40)
#define BM6_Z16 (BIT16)
#define BM5_Z16 (BIT23 |  BIT24)
#define BM4_Z16 (BIT32 |  BIT33 |  BIT34)
#define BM3_Z16 (BIT42 |  BIT43 |  BIT44 |  BIT45)
#define BM6_Z13 (BIT13)
#define BM5_Z13 (BIT19 |  BIT20)
#define BM4_Z13 (BIT27 |  BIT28 |  BIT29)
#define BM3_Z13 (BIT36 |  BIT37 |  BIT38 |  BIT39)
#define BM6_Z20 (BIT20)
#define BM5_Z20 (BIT28 |  BIT29)
#define BM4_Z20 (BIT38 |  BIT39 |  BIT40)
#define BM3_Z20 (BIT49 |  BIT50 |  BIT51 |  BIT52)
#define BM6_Z18 (BIT18)
#define BM5_Z18 (BIT25 |  BIT26)
#define BM4_Z18 (BIT34 |  BIT35 |  BIT36)
#define BM3_Z18 (BIT44 |  BIT45 |  BIT46 |  BIT47)

BitArray sel_3_white, sel_4_white, sel_5_white, sel_6_white;
BitArray sel_3_black, sel_4_black, sel_5_black, sel_6_black;
BitArray rem_3_white, rem_4_white, rem_5_white, rem_6_white;
BitArray rem_3_black, rem_4_black, rem_5_black, rem_6_black;

 void calc_patterns(BitArray white_man, BitArray black_man, int nr_of_white_king, int nr_of_black_king)
{
	if (nr_of_white_king == 0)
	{	sel_3_black = 0; sel_4_black = 0; sel_5_black = 0; sel_6_black = 0;
		rem_3_black = 0; rem_4_black = 0; rem_5_black = 0; rem_6_black = 0;

		if ((black_man & D1_B) == D1_B) {sel_3_black = sel_3_black | BM3_D1; }
		if ((black_man & D10_B) == D10_B) {sel_3_black = sel_3_black | BM3_D10; }
		if ((black_man & D11_B) == D11_B) {sel_3_black = sel_3_black | BM3_D11; }
		if ((black_man & D12_B) == D12_B) {sel_3_black = sel_3_black | BM3_D12; }
		if ((black_man & D13_B) == D13_B) {sel_3_black = sel_3_black | BM3_D13; }
		if ((black_man & D14_B) == D14_B) {sel_3_black = sel_3_black | BM3_D14; }
		if ((black_man & D15_B) == D15_B) {sel_3_black = sel_3_black | BM3_D15; }
		if ((black_man & D16_B) == D16_B) {sel_3_black = sel_3_black | BM3_D16; }
		if ((black_man & D17_B) == D17_B) {sel_3_black = sel_3_black | BM3_D17; }
		if ((black_man & D18_B) == D18_B) {sel_3_black = sel_3_black | BM3_D18; }
		if ((black_man & D19_B) == D19_B) {sel_3_black = sel_3_black | BM3_D19; }
		if ((black_man & D2_B) == D2_B) {sel_3_black = sel_3_black | BM3_D2; }
		if ((black_man & D20_B) == D20_B) {sel_3_black = sel_3_black | BM3_D20; }
		if ((black_man & D21_B) == D21_B) {sel_3_black = sel_3_black | BM3_D21; }
		if ((black_man & D22_B) == D22_B) {sel_3_black = sel_3_black | BM3_D22; }
		if ((black_man & D23_B) == D23_B) {sel_3_black = sel_3_black | BM3_D23; }
		if ((black_man & D24_B) == D24_B) {sel_3_black = sel_3_black | BM3_D24; }
		if ((black_man & D25_B) == D25_B) {sel_3_black = sel_3_black | BM3_D25; }
		if ((black_man & D26_B) == D26_B) {sel_3_black = sel_3_black | BM3_D26; }
		if ((black_man & D27_B) == D27_B) {sel_3_black = sel_3_black | BM3_D27; }
		if ((black_man & D28_B) == D28_B) {sel_3_black = sel_3_black | BM3_D28; }
		if ((black_man & D29_B) == D29_B) {sel_3_black = sel_3_black | BM3_D29; }
		if ((black_man & D3_B) == D3_B) {sel_3_black = sel_3_black | BM3_D3; }
		if ((black_man & D30_B) == D30_B) {sel_3_black = sel_3_black | BM3_D30; }
		if ((black_man & D31_B) == D31_B) {sel_3_black = sel_3_black | BM3_D31; }
		if ((black_man & D32_B) == D32_B) {sel_3_black = sel_3_black | BM3_D32; }
		if ((black_man & D33_B) == D33_B) {sel_3_black = sel_3_black | BM3_D33; }
		if ((black_man & D34_B) == D34_B) {sel_3_black = sel_3_black | BM3_D34; }
		if ((black_man & D35_B) == D35_B) {sel_3_black = sel_3_black | BM3_D35; }
		if ((black_man & D36_B) == D36_B) {sel_3_black = sel_3_black | BM3_D36; }
		if ((black_man & D37_B) == D37_B) {sel_3_black = sel_3_black | BM3_D37; }
		if ((black_man & D38_B) == D38_B) {sel_3_black = sel_3_black | BM3_D38; }
		if ((black_man & D39_B) == D39_B) {sel_3_black = sel_3_black | BM3_D39; }
		if ((black_man & D4_B) == D4_B) {sel_3_black = sel_3_black | BM3_D4; }
		if ((black_man & D40_B) == D40_B) {sel_3_black = sel_3_black | BM3_D40; }
		if ((black_man & D41_B) == D41_B) {sel_3_black = sel_3_black | BM3_D41; }
		if ((black_man & D42_B) == D42_B) {sel_3_black = sel_3_black | BM3_D42; }
		if ((black_man & D43_B) == D43_B) {sel_3_black = sel_3_black | BM3_D43; }
		if ((black_man & D44_B) == D44_B) {sel_3_black = sel_3_black | BM3_D44; }
		if ((black_man & D45_B) == D45_B) {sel_3_black = sel_3_black | BM3_D45; }
		if ((black_man & D46_B) == D46_B) {sel_3_black = sel_3_black | BM3_D46; }
		if ((black_man & D47_B) == D47_B) {sel_3_black = sel_3_black | BM3_D47; }
		if ((black_man & D48_B) == D48_B) {sel_3_black = sel_3_black | BM3_D48; }
		if ((black_man & D49_B) == D49_B) {sel_3_black = sel_3_black | BM3_D49; }
		if ((black_man & D5_B) == D5_B) {sel_3_black = sel_3_black | BM3_D5; }
		if ((black_man & D50_B) == D50_B) {sel_3_black = sel_3_black | BM3_D50; }
		if ((black_man & D51_B) == D51_B) {sel_3_black = sel_3_black | BM3_D51; }
		if ((black_man & D52_B) == D52_B) {sel_3_black = sel_3_black | BM3_D52; }
		if ((black_man & D53_B) == D53_B) {sel_3_black = sel_3_black | BM3_D53; }
		if ((black_man & D54_B) == D54_B) {sel_3_black = sel_3_black | BM3_D54; }
		if ((black_man & D55_B) == D55_B) {sel_3_black = sel_3_black | BM3_D55; }
//		if ((black_man & D56_B) == D56_B) {sel_3_black = sel_3_black | BM3_D56; }
		if ((black_man & D6_B) == D6_B) {sel_3_black = sel_3_black | BM3_D6; }
		if ((black_man & D7_B) == D7_B) {sel_3_black = sel_3_black | BM3_D7; }
		if ((black_man & D8_B) == D8_B) {sel_3_black = sel_3_black | BM3_D8; }
		if ((black_man & D9_B) == D9_B) {sel_3_black = sel_3_black | BM3_D9; }
		if ((black_man & F1_B) == F1_B) {sel_5_black = sel_5_black | BM5_F1; rem_4_black = rem_4_black | BM4_F1; rem_3_black = rem_3_black | BM3_F1; }
		if ((black_man & F10_B) == F10_B) {sel_5_black = sel_5_black | BM5_F10; rem_4_black = rem_4_black | BM4_F10; rem_3_black = rem_3_black | BM3_F10; }
		if ((black_man & F11_B) == F11_B) {sel_5_black = sel_5_black | BM5_F11; rem_4_black = rem_4_black | BM4_F11; rem_3_black = rem_3_black | BM3_F11; }
		if ((black_man & F12_B) == F12_B) {sel_5_black = sel_5_black | BM5_F12; rem_4_black = rem_4_black | BM4_F12; rem_3_black = rem_3_black | BM3_F12; }
		if ((black_man & F13_B) == F13_B) {sel_5_black = sel_5_black | BM5_F13; rem_4_black = rem_4_black | BM4_F13; rem_3_black = rem_3_black | BM3_F13; }
		if ((black_man & F14_B) == F14_B) {sel_5_black = sel_5_black | BM5_F14; rem_4_black = rem_4_black | BM4_F14; rem_3_black = rem_3_black | BM3_F14; }
		if ((black_man & F15_B) == F15_B) {sel_5_black = sel_5_black | BM5_F15; rem_4_black = rem_4_black | BM4_F15; rem_3_black = rem_3_black | BM3_F15; }
		if ((black_man & F16_B) == F16_B) {sel_5_black = sel_5_black | BM5_F16; rem_4_black = rem_4_black | BM4_F16; rem_3_black = rem_3_black | BM3_F16; }
		if ((black_man & F17_B) == F17_B) {sel_5_black = sel_5_black | BM5_F17; rem_4_black = rem_4_black | BM4_F17; rem_3_black = rem_3_black | BM3_F17; }
		if ((black_man & F18_B) == F18_B) {sel_5_black = sel_5_black | BM5_F18; rem_4_black = rem_4_black | BM4_F18; rem_3_black = rem_3_black | BM3_F18; }
		if ((black_man & F19_B) == F19_B) {sel_5_black = sel_5_black | BM5_F19; rem_4_black = rem_4_black | BM4_F19; rem_3_black = rem_3_black | BM3_F19; }
		if ((black_man & F2_B) == F2_B) {sel_5_black = sel_5_black | BM5_F2; rem_4_black = rem_4_black | BM4_F2; rem_3_black = rem_3_black | BM3_F2; }
		if ((black_man & F20_B) == F20_B) {sel_5_black = sel_5_black | BM5_F20; rem_4_black = rem_4_black | BM4_F20; rem_3_black = rem_3_black | BM3_F20; }
		if ((black_man & F21_B) == F21_B) {sel_5_black = sel_5_black | BM5_F21; rem_4_black = rem_4_black | BM4_F21; rem_3_black = rem_3_black | BM3_F21; }
		if ((black_man & F22_B) == F22_B) {sel_5_black = sel_5_black | BM5_F22; rem_4_black = rem_4_black | BM4_F22; rem_3_black = rem_3_black | BM3_F22; }
		if ((black_man & F23_B) == F23_B) {sel_5_black = sel_5_black | BM5_F23; rem_4_black = rem_4_black | BM4_F23; rem_3_black = rem_3_black | BM3_F23; }
		if ((black_man & F24_B) == F24_B) {sel_5_black = sel_5_black | BM5_F24; rem_4_black = rem_4_black | BM4_F24; rem_3_black = rem_3_black | BM3_F24; }
		if ((black_man & F25_B) == F25_B) {sel_5_black = sel_5_black | BM5_F25; rem_4_black = rem_4_black | BM4_F25; rem_3_black = rem_3_black | BM3_F25; }
		if ((black_man & F26_B) == F26_B) {sel_5_black = sel_5_black | BM5_F26; rem_4_black = rem_4_black | BM4_F26; rem_3_black = rem_3_black | BM3_F26; }
		if ((black_man & F27_B) == F27_B) {sel_5_black = sel_5_black | BM5_F27; rem_4_black = rem_4_black | BM4_F27; rem_3_black = rem_3_black | BM3_F27; }
		if ((black_man & F28_B) == F28_B) {sel_5_black = sel_5_black | BM5_F28; rem_4_black = rem_4_black | BM4_F28; rem_3_black = rem_3_black | BM3_F28; }
		if ((black_man & F29_B) == F29_B) {sel_5_black = sel_5_black | BM5_F29; rem_4_black = rem_4_black | BM4_F29; rem_3_black = rem_3_black | BM3_F29; }
		if ((black_man & F3_B) == F3_B) {sel_5_black = sel_5_black | BM5_F3; rem_4_black = rem_4_black | BM4_F3; rem_3_black = rem_3_black | BM3_F3; }
		if ((black_man & F30_B) == F30_B) {/*sel_5_black = sel_5_black | BM5_F30;*/ rem_4_black = rem_4_black | BM4_F30; rem_3_black = rem_3_black | BM3_F30; }
		if ((black_man & F4_B) == F4_B) {sel_5_black = sel_5_black | BM5_F4; rem_4_black = rem_4_black | BM4_F4; rem_3_black = rem_3_black | BM3_F4; }
		if ((black_man & F5_B) == F5_B) {sel_5_black = sel_5_black | BM5_F5; rem_4_black = rem_4_black | BM4_F5; rem_3_black = rem_3_black | BM3_F5; }
		if ((black_man & F6_B) == F6_B) {sel_5_black = sel_5_black | BM5_F6; rem_4_black = rem_4_black | BM4_F6; rem_3_black = rem_3_black | BM3_F6; }
		if ((black_man & F7_B) == F7_B) {sel_5_black = sel_5_black | BM5_F7; rem_4_black = rem_4_black | BM4_F7; rem_3_black = rem_3_black | BM3_F7; }
		if ((black_man & F8_B) == F8_B) {sel_5_black = sel_5_black | BM5_F8; rem_4_black = rem_4_black | BM4_F8; rem_3_black = rem_3_black | BM3_F8; }
		if ((black_man & F9_B) == F9_B) {sel_5_black = sel_5_black | BM5_F9; rem_4_black = rem_4_black | BM4_F9; rem_3_black = rem_3_black | BM3_F9; }
		if ((black_man & V1_B) == V1_B) {sel_4_black = sel_4_black | BM4_V1; rem_3_black = rem_3_black | BM3_V1; }
		if ((black_man & V10_B) == V10_B) {sel_4_black = sel_4_black | BM4_V10; rem_3_black = rem_3_black | BM3_V10; }
		if ((black_man & V11_B) == V11_B) {sel_4_black = sel_4_black | BM4_V11; rem_3_black = rem_3_black | BM3_V11; }
		if ((black_man & V12_B) == V12_B) {sel_4_black = sel_4_black | BM4_V12; rem_3_black = rem_3_black | BM3_V12; }
		if ((black_man & V13_B) == V13_B) {sel_4_black = sel_4_black | BM4_V13; rem_3_black = rem_3_black | BM3_V13; }
		if ((black_man & V14_B) == V14_B) {sel_4_black = sel_4_black | BM4_V14; rem_3_black = rem_3_black | BM3_V14; }
		if ((black_man & V15_B) == V15_B) {sel_4_black = sel_4_black | BM4_V15; rem_3_black = rem_3_black | BM3_V15; }
		if ((black_man & V16_B) == V16_B) {sel_4_black = sel_4_black | BM4_V16; rem_3_black = rem_3_black | BM3_V16; }
		if ((black_man & V17_B) == V17_B) {sel_4_black = sel_4_black | BM4_V17; rem_3_black = rem_3_black | BM3_V17; }
		if ((black_man & V18_B) == V18_B) {sel_4_black = sel_4_black | BM4_V18; rem_3_black = rem_3_black | BM3_V18; }
		if ((black_man & V19_B) == V19_B) {sel_4_black = sel_4_black | BM4_V19; rem_3_black = rem_3_black | BM3_V19; }
		if ((black_man & V2_B) == V2_B) {sel_4_black = sel_4_black | BM4_V2; rem_3_black = rem_3_black | BM3_V2; }
		if ((black_man & V20_B) == V20_B) {sel_4_black = sel_4_black | BM4_V20; rem_3_black = rem_3_black | BM3_V20; }
		if ((black_man & V21_B) == V21_B) {/*sel_4_black = sel_4_black | BM4_V21;*/ rem_3_black = rem_3_black | BM3_V21; }
		if ((black_man & V22_B) == V22_B) {sel_4_black = sel_4_black | BM4_V22; rem_3_black = rem_3_black | BM3_V22; }
		if ((black_man & V23_B) == V23_B) {sel_4_black = sel_4_black | BM4_V23; rem_3_black = rem_3_black | BM3_V23; }
		if ((black_man & V24_B) == V24_B) {sel_4_black = sel_4_black | BM4_V24; rem_3_black = rem_3_black | BM3_V24; }
		if ((black_man & V25_B) == V25_B) {sel_4_black = sel_4_black | BM4_V25; rem_3_black = rem_3_black | BM3_V25; }
		if ((black_man & V26_B) == V26_B) {sel_4_black = sel_4_black | BM4_V26; rem_3_black = rem_3_black | BM3_V26; }
		if ((black_man & V27_B) == V27_B) {sel_4_black = sel_4_black | BM4_V27; rem_3_black = rem_3_black | BM3_V27; }
		if ((black_man & V28_B) == V28_B) {sel_4_black = sel_4_black | BM4_V28; rem_3_black = rem_3_black | BM3_V28; }
		if ((black_man & V29_B) == V29_B) {sel_4_black = sel_4_black | BM4_V29; rem_3_black = rem_3_black | BM3_V29; }
		if ((black_man & V3_B) == V3_B) {sel_4_black = sel_4_black | BM4_V3; rem_3_black = rem_3_black | BM3_V3; }
		if ((black_man & V30_B) == V30_B) {sel_4_black = sel_4_black | BM4_V30; rem_3_black = rem_3_black | BM3_V30; }
		if ((black_man & V31_B) == V31_B) {sel_4_black = sel_4_black | BM4_V31; rem_3_black = rem_3_black | BM3_V31; }
		if ((black_man & V32_B) == V32_B) {sel_4_black = sel_4_black | BM4_V32; rem_3_black = rem_3_black | BM3_V32; }
		if ((black_man & V33_B) == V33_B) {sel_4_black = sel_4_black | BM4_V33; rem_3_black = rem_3_black | BM3_V33; }
		if ((black_man & V34_B) == V34_B) {sel_4_black = sel_4_black | BM4_V34; rem_3_black = rem_3_black | BM3_V34; }
		if ((black_man & V35_B) == V35_B) {sel_4_black = sel_4_black | BM4_V35; rem_3_black = rem_3_black | BM3_V35; }
		if ((black_man & V36_B) == V36_B) {sel_4_black = sel_4_black | BM4_V36; rem_3_black = rem_3_black | BM3_V36; }
		if ((black_man & V37_B) == V37_B) {sel_4_black = sel_4_black | BM4_V37; rem_3_black = rem_3_black | BM3_V37; }
		if ((black_man & V38_B) == V38_B) {sel_4_black = sel_4_black | BM4_V38; rem_3_black = rem_3_black | BM3_V38; }
		if ((black_man & V39_B) == V39_B) {sel_4_black = sel_4_black | BM4_V39; rem_3_black = rem_3_black | BM3_V39; }
		if ((black_man & V4_B) == V4_B) {sel_4_black = sel_4_black | BM4_V4; rem_3_black = rem_3_black | BM3_V4; }
		if ((black_man & V40_B) == V40_B) {sel_4_black = sel_4_black | BM4_V40; rem_3_black = rem_3_black | BM3_V40; }
		if ((black_man & V41_B) == V41_B) {sel_4_black = sel_4_black | BM4_V41; rem_3_black = rem_3_black | BM3_V41; }
		if ((black_man & V42_B) == V42_B) {sel_4_black = sel_4_black | BM4_V42; rem_3_black = rem_3_black | BM3_V42; }
		if ((black_man & V5_B) == V5_B) {sel_4_black = sel_4_black | BM4_V5; rem_3_black = rem_3_black | BM3_V5; }
		if ((black_man & V6_B) == V6_B) {sel_4_black = sel_4_black | BM4_V6; rem_3_black = rem_3_black | BM3_V6; }
		if ((black_man & V7_B) == V7_B) {sel_4_black = sel_4_black | BM4_V7; rem_3_black = rem_3_black | BM3_V7; }
		if ((black_man & V8_B) == V8_B) {sel_4_black = sel_4_black | BM4_V8; rem_3_black = rem_3_black | BM3_V8; }
		if ((black_man & V9_B) == V9_B) {sel_4_black = sel_4_black | BM4_V9; rem_3_black = rem_3_black | BM3_V9; }
		if ((black_man & Z1_B) == Z1_B) {sel_6_black = sel_6_black | BM6_Z1; rem_5_black = rem_5_black | BM5_Z1; rem_4_black = rem_4_black | BM4_Z1; rem_3_black = rem_3_black | BM3_Z1; }
		if ((black_man & Z10_B) == Z10_B) {/*sel_6_black = sel_6_black | BM6_Z10;*/ rem_5_black = rem_5_black | BM5_Z10; rem_4_black = rem_4_black | BM4_Z10; rem_3_black = rem_3_black | BM3_Z10; }
		if ((black_man & Z11_B) == Z11_B) {sel_6_black = sel_6_black | BM6_Z11; rem_5_black = rem_5_black | BM5_Z11; rem_4_black = rem_4_black | BM4_Z11; rem_3_black = rem_3_black | BM3_Z11; }
		if ((black_man & Z12_B) == Z12_B) {sel_6_black = sel_6_black | BM6_Z12; rem_5_black = rem_5_black | BM5_Z12; rem_4_black = rem_4_black | BM4_Z12; rem_3_black = rem_3_black | BM3_Z12; }
		if ((black_man & Z13_B) == Z13_B) {sel_6_black = sel_6_black | BM6_Z13; rem_5_black = rem_5_black | BM5_Z13; rem_4_black = rem_4_black | BM4_Z13; rem_3_black = rem_3_black | BM3_Z13; }
		if ((black_man & Z14_B) == Z14_B) {sel_6_black = sel_6_black | BM6_Z14; rem_5_black = rem_5_black | BM5_Z14; rem_4_black = rem_4_black | BM4_Z14; rem_3_black = rem_3_black | BM3_Z14; }
		if ((black_man & Z15_B) == Z15_B) {sel_6_black = sel_6_black | BM6_Z15; rem_5_black = rem_5_black | BM5_Z15; rem_4_black = rem_4_black | BM4_Z15; rem_3_black = rem_3_black | BM3_Z15; }
		if ((black_man & Z16_B) == Z16_B) {sel_6_black = sel_6_black | BM6_Z16; rem_5_black = rem_5_black | BM5_Z16; rem_4_black = rem_4_black | BM4_Z16; rem_3_black = rem_3_black | BM3_Z16; }
		if ((black_man & Z17_B) == Z17_B) {sel_6_black = sel_6_black | BM6_Z17; rem_5_black = rem_5_black | BM5_Z17; rem_4_black = rem_4_black | BM4_Z17; rem_3_black = rem_3_black | BM3_Z17; }
		if ((black_man & Z18_B) == Z18_B) {sel_6_black = sel_6_black | BM6_Z18; rem_5_black = rem_5_black | BM5_Z18; rem_4_black = rem_4_black | BM4_Z18; rem_3_black = rem_3_black | BM3_Z18; }
		if ((black_man & Z19_B) == Z19_B) {sel_6_black = sel_6_black | BM6_Z19; rem_5_black = rem_5_black | BM5_Z19; rem_4_black = rem_4_black | BM4_Z19; rem_3_black = rem_3_black | BM3_Z19; }
		if ((black_man & Z2_B) == Z2_B) {sel_6_black = sel_6_black | BM6_Z2; rem_5_black = rem_5_black | BM5_Z2; rem_4_black = rem_4_black | BM4_Z2; rem_3_black = rem_3_black | BM3_Z2; }
		if ((black_man & Z20_B) == Z20_B) {sel_6_black = sel_6_black | BM6_Z20; rem_5_black = rem_5_black | BM5_Z20; rem_4_black = rem_4_black | BM4_Z20; rem_3_black = rem_3_black | BM3_Z20; }
		if ((black_man & Z3_B) == Z3_B) {sel_6_black = sel_6_black | BM6_Z3; rem_5_black = rem_5_black | BM5_Z3; rem_4_black = rem_4_black | BM4_Z3; rem_3_black = rem_3_black | BM3_Z3; }
		if ((black_man & Z4_B) == Z4_B) {sel_6_black = sel_6_black | BM6_Z4; rem_5_black = rem_5_black | BM5_Z4; rem_4_black = rem_4_black | BM4_Z4; rem_3_black = rem_3_black | BM3_Z4; }
		if ((black_man & Z5_B) == Z5_B) {sel_6_black = sel_6_black | BM6_Z5; rem_5_black = rem_5_black | BM5_Z5; rem_4_black = rem_4_black | BM4_Z5; rem_3_black = rem_3_black | BM3_Z5; }
		if ((black_man & Z6_B) == Z6_B) {sel_6_black = sel_6_black | BM6_Z6; rem_5_black = rem_5_black | BM5_Z6; rem_4_black = rem_4_black | BM4_Z6; rem_3_black = rem_3_black | BM3_Z6; }
		if ((black_man & Z7_B) == Z7_B) {sel_6_black = sel_6_black | BM6_Z7; rem_5_black = rem_5_black | BM5_Z7; rem_4_black = rem_4_black | BM4_Z7; rem_3_black = rem_3_black | BM3_Z7; }
		if ((black_man & Z8_B) == Z8_B) {sel_6_black = sel_6_black | BM6_Z8; rem_5_black = rem_5_black | BM5_Z8; rem_4_black = rem_4_black | BM4_Z8; rem_3_black = rem_3_black | BM3_Z8; }
		if ((black_man & Z9_B) == Z9_B) {sel_6_black = sel_6_black | BM6_Z9; rem_5_black = rem_5_black | BM5_Z9; rem_4_black = rem_4_black | BM4_Z9; rem_3_black = rem_3_black | BM3_Z9; }

		sel_5_black = sel_5_black & ~rem_5_black;
		sel_4_black = sel_4_black & ~rem_4_black;
		sel_3_black = sel_3_black & ~rem_3_black;

	}
	if (nr_of_black_king == 0)
	{	sel_3_white = 0; sel_4_white = 0; sel_5_white = 0; sel_6_white = 0;
		rem_3_white = 0; rem_4_white = 0; rem_5_white = 0; rem_6_white = 0;

		if ((white_man & D1_W) == D1_W) {sel_3_white = sel_3_white | BM3_D1; }
		if ((white_man & D10_W) == D10_W) {sel_3_white = sel_3_white | BM3_D10; }
		if ((white_man & D11_W) == D11_W) {sel_3_white = sel_3_white | BM3_D11; }
		if ((white_man & D12_W) == D12_W) {sel_3_white = sel_3_white | BM3_D12; }
		if ((white_man & D13_W) == D13_W) {sel_3_white = sel_3_white | BM3_D13; }
		if ((white_man & D14_W) == D14_W) {sel_3_white = sel_3_white | BM3_D14; }
		if ((white_man & D15_W) == D15_W) {sel_3_white = sel_3_white | BM3_D15; }
		if ((white_man & D16_W) == D16_W) {sel_3_white = sel_3_white | BM3_D16; }
		if ((white_man & D17_W) == D17_W) {sel_3_white = sel_3_white | BM3_D17; }
		if ((white_man & D18_W) == D18_W) {sel_3_white = sel_3_white | BM3_D18; }
		if ((white_man & D19_W) == D19_W) {sel_3_white = sel_3_white | BM3_D19; }
		if ((white_man & D2_W) == D2_W) {sel_3_white = sel_3_white | BM3_D2; }
		if ((white_man & D20_W) == D20_W) {sel_3_white = sel_3_white | BM3_D20; }
		if ((white_man & D21_W) == D21_W) {sel_3_white = sel_3_white | BM3_D21; }
		if ((white_man & D22_W) == D22_W) {sel_3_white = sel_3_white | BM3_D22; }
		if ((white_man & D23_W) == D23_W) {sel_3_white = sel_3_white | BM3_D23; }
		if ((white_man & D24_W) == D24_W) {sel_3_white = sel_3_white | BM3_D24; }
		if ((white_man & D25_W) == D25_W) {sel_3_white = sel_3_white | BM3_D25; }
		if ((white_man & D26_W) == D26_W) {sel_3_white = sel_3_white | BM3_D26; }
		if ((white_man & D27_W) == D27_W) {sel_3_white = sel_3_white | BM3_D27; }
		if ((white_man & D28_W) == D28_W) {sel_3_white = sel_3_white | BM3_D28; }
		if ((white_man & D29_W) == D29_W) {sel_3_white = sel_3_white | BM3_D29; }
		if ((white_man & D3_W) == D3_W) {sel_3_white = sel_3_white | BM3_D3; }
		if ((white_man & D30_W) == D30_W) {sel_3_white = sel_3_white | BM3_D30; }
		if ((white_man & D31_W) == D31_W) {sel_3_white = sel_3_white | BM3_D31; }
		if ((white_man & D32_W) == D32_W) {sel_3_white = sel_3_white | BM3_D32; }
		if ((white_man & D33_W) == D33_W) {sel_3_white = sel_3_white | BM3_D33; }
		if ((white_man & D34_W) == D34_W) {sel_3_white = sel_3_white | BM3_D34; }
		if ((white_man & D35_W) == D35_W) {sel_3_white = sel_3_white | BM3_D35; }
		if ((white_man & D36_W) == D36_W) {sel_3_white = sel_3_white | BM3_D36; }
		if ((white_man & D37_W) == D37_W) {sel_3_white = sel_3_white | BM3_D37; }
		if ((white_man & D38_W) == D38_W) {sel_3_white = sel_3_white | BM3_D38; }
		if ((white_man & D39_W) == D39_W) {sel_3_white = sel_3_white | BM3_D39; }
		if ((white_man & D4_W) == D4_W) {sel_3_white = sel_3_white | BM3_D4; }
		if ((white_man & D40_W) == D40_W) {sel_3_white = sel_3_white | BM3_D40; }
		if ((white_man & D41_W) == D41_W) {sel_3_white = sel_3_white | BM3_D41; }
		if ((white_man & D42_W) == D42_W) {sel_3_white = sel_3_white | BM3_D42; }
		if ((white_man & D43_W) == D43_W) {sel_3_white = sel_3_white | BM3_D43; }
		if ((white_man & D44_W) == D44_W) {sel_3_white = sel_3_white | BM3_D44; }
		if ((white_man & D45_W) == D45_W) {sel_3_white = sel_3_white | BM3_D45; }
		if ((white_man & D46_W) == D46_W) {sel_3_white = sel_3_white | BM3_D46; }
		if ((white_man & D47_W) == D47_W) {sel_3_white = sel_3_white | BM3_D47; }
		if ((white_man & D48_W) == D48_W) {sel_3_white = sel_3_white | BM3_D48; }
		if ((white_man & D49_W) == D49_W) {sel_3_white = sel_3_white | BM3_D49; }
		if ((white_man & D5_W) == D5_W) {sel_3_white = sel_3_white | BM3_D5; }
		if ((white_man & D50_W) == D50_W) {sel_3_white = sel_3_white | BM3_D50; }
		if ((white_man & D51_W) == D51_W) {sel_3_white = sel_3_white | BM3_D51; }
		if ((white_man & D52_W) == D52_W) {sel_3_white = sel_3_white | BM3_D52; }
		if ((white_man & D53_W) == D53_W) {sel_3_white = sel_3_white | BM3_D53; }
		if ((white_man & D54_W) == D54_W) {sel_3_white = sel_3_white | BM3_D54; }
		if ((white_man & D55_W) == D55_W) {sel_3_white = sel_3_white | BM3_D55; }
//		if ((white_man & D56_W) == D56_W) {sel_3_white = sel_3_white | BM3_D56; }
		if ((white_man & D6_W) == D6_W) {sel_3_white = sel_3_white | BM3_D6; }
		if ((white_man & D7_W) == D7_W) {sel_3_white = sel_3_white | BM3_D7; }
		if ((white_man & D8_W) == D8_W) {sel_3_white = sel_3_white | BM3_D8; }
		if ((white_man & D9_W) == D9_W) {sel_3_white = sel_3_white | BM3_D9; }
		if ((white_man & F1_W) == F1_W) {sel_5_white = sel_5_white | BM5_F1; rem_4_white = rem_4_white | BM4_F1; rem_3_white = rem_3_white | BM3_F1; }
		if ((white_man & F10_W) == F10_W) {sel_5_white = sel_5_white | BM5_F10; rem_4_white = rem_4_white | BM4_F10; rem_3_white = rem_3_white | BM3_F10; }
		if ((white_man & F11_W) == F11_W) {sel_5_white = sel_5_white | BM5_F11; rem_4_white = rem_4_white | BM4_F11; rem_3_white = rem_3_white | BM3_F11; }
		if ((white_man & F12_W) == F12_W) {sel_5_white = sel_5_white | BM5_F12; rem_4_white = rem_4_white | BM4_F12; rem_3_white = rem_3_white | BM3_F12; }
		if ((white_man & F13_W) == F13_W) {sel_5_white = sel_5_white | BM5_F13; rem_4_white = rem_4_white | BM4_F13; rem_3_white = rem_3_white | BM3_F13; }
		if ((white_man & F14_W) == F14_W) {sel_5_white = sel_5_white | BM5_F14; rem_4_white = rem_4_white | BM4_F14; rem_3_white = rem_3_white | BM3_F14; }
		if ((white_man & F15_W) == F15_W) {sel_5_white = sel_5_white | BM5_F15; rem_4_white = rem_4_white | BM4_F15; rem_3_white = rem_3_white | BM3_F15; }
		if ((white_man & F16_W) == F16_W) {sel_5_white = sel_5_white | BM5_F16; rem_4_white = rem_4_white | BM4_F16; rem_3_white = rem_3_white | BM3_F16; }
		if ((white_man & F17_W) == F17_W) {sel_5_white = sel_5_white | BM5_F17; rem_4_white = rem_4_white | BM4_F17; rem_3_white = rem_3_white | BM3_F17; }
		if ((white_man & F18_W) == F18_W) {sel_5_white = sel_5_white | BM5_F18; rem_4_white = rem_4_white | BM4_F18; rem_3_white = rem_3_white | BM3_F18; }
		if ((white_man & F19_W) == F19_W) {sel_5_white = sel_5_white | BM5_F19; rem_4_white = rem_4_white | BM4_F19; rem_3_white = rem_3_white | BM3_F19; }
		if ((white_man & F2_W) == F2_W) {sel_5_white = sel_5_white | BM5_F2; rem_4_white = rem_4_white | BM4_F2; rem_3_white = rem_3_white | BM3_F2; }
		if ((white_man & F20_W) == F20_W) {sel_5_white = sel_5_white | BM5_F20; rem_4_white = rem_4_white | BM4_F20; rem_3_white = rem_3_white | BM3_F20; }
		if ((white_man & F21_W) == F21_W) {sel_5_white = sel_5_white | BM5_F21; rem_4_white = rem_4_white | BM4_F21; rem_3_white = rem_3_white | BM3_F21; }
		if ((white_man & F22_W) == F22_W) {sel_5_white = sel_5_white | BM5_F22; rem_4_white = rem_4_white | BM4_F22; rem_3_white = rem_3_white | BM3_F22; }
		if ((white_man & F23_W) == F23_W) {sel_5_white = sel_5_white | BM5_F23; rem_4_white = rem_4_white | BM4_F23; rem_3_white = rem_3_white | BM3_F23; }
		if ((white_man & F24_W) == F24_W) {sel_5_white = sel_5_white | BM5_F24; rem_4_white = rem_4_white | BM4_F24; rem_3_white = rem_3_white | BM3_F24; }
		if ((white_man & F25_W) == F25_W) {sel_5_white = sel_5_white | BM5_F25; rem_4_white = rem_4_white | BM4_F25; rem_3_white = rem_3_white | BM3_F25; }
		if ((white_man & F26_W) == F26_W) {sel_5_white = sel_5_white | BM5_F26; rem_4_white = rem_4_white | BM4_F26; rem_3_white = rem_3_white | BM3_F26; }
		if ((white_man & F27_W) == F27_W) {sel_5_white = sel_5_white | BM5_F27; rem_4_white = rem_4_white | BM4_F27; rem_3_white = rem_3_white | BM3_F27; }
		if ((white_man & F28_W) == F28_W) {sel_5_white = sel_5_white | BM5_F28; rem_4_white = rem_4_white | BM4_F28; rem_3_white = rem_3_white | BM3_F28; }
		if ((white_man & F29_W) == F29_W) {sel_5_white = sel_5_white | BM5_F29; rem_4_white = rem_4_white | BM4_F29; rem_3_white = rem_3_white | BM3_F29; }
		if ((white_man & F3_W) == F3_W) {sel_5_white = sel_5_white | BM5_F3; rem_4_white = rem_4_white | BM4_F3; rem_3_white = rem_3_white | BM3_F3; }
		if ((white_man & F30_W) == F30_W) {/*sel_5_white = sel_5_white | BM5_F30;*/ rem_4_white = rem_4_white | BM4_F30; rem_3_white = rem_3_white | BM3_F30; }
		if ((white_man & F4_W) == F4_W) {sel_5_white = sel_5_white | BM5_F4; rem_4_white = rem_4_white | BM4_F4; rem_3_white = rem_3_white | BM3_F4; }
		if ((white_man & F5_W) == F5_W) {sel_5_white = sel_5_white | BM5_F5; rem_4_white = rem_4_white | BM4_F5; rem_3_white = rem_3_white | BM3_F5; }
		if ((white_man & F6_W) == F6_W) {sel_5_white = sel_5_white | BM5_F6; rem_4_white = rem_4_white | BM4_F6; rem_3_white = rem_3_white | BM3_F6; }
		if ((white_man & F7_W) == F7_W) {sel_5_white = sel_5_white | BM5_F7; rem_4_white = rem_4_white | BM4_F7; rem_3_white = rem_3_white | BM3_F7; }
		if ((white_man & F8_W) == F8_W) {sel_5_white = sel_5_white | BM5_F8; rem_4_white = rem_4_white | BM4_F8; rem_3_white = rem_3_white | BM3_F8; }
		if ((white_man & F9_W) == F9_W) {sel_5_white = sel_5_white | BM5_F9; rem_4_white = rem_4_white | BM4_F9; rem_3_white = rem_3_white | BM3_F9; }
		if ((white_man & V1_W) == V1_W) {sel_4_white = sel_4_white | BM4_V1; rem_3_white = rem_3_white | BM3_V1; }
		if ((white_man & V10_W) == V10_W) {sel_4_white = sel_4_white | BM4_V10; rem_3_white = rem_3_white | BM3_V10; }
		if ((white_man & V11_W) == V11_W) {sel_4_white = sel_4_white | BM4_V11; rem_3_white = rem_3_white | BM3_V11; }
		if ((white_man & V12_W) == V12_W) {sel_4_white = sel_4_white | BM4_V12; rem_3_white = rem_3_white | BM3_V12; }
		if ((white_man & V13_W) == V13_W) {sel_4_white = sel_4_white | BM4_V13; rem_3_white = rem_3_white | BM3_V13; }
		if ((white_man & V14_W) == V14_W) {sel_4_white = sel_4_white | BM4_V14; rem_3_white = rem_3_white | BM3_V14; }
		if ((white_man & V15_W) == V15_W) {sel_4_white = sel_4_white | BM4_V15; rem_3_white = rem_3_white | BM3_V15; }
		if ((white_man & V16_W) == V16_W) {sel_4_white = sel_4_white | BM4_V16; rem_3_white = rem_3_white | BM3_V16; }
		if ((white_man & V17_W) == V17_W) {sel_4_white = sel_4_white | BM4_V17; rem_3_white = rem_3_white | BM3_V17; }
		if ((white_man & V18_W) == V18_W) {sel_4_white = sel_4_white | BM4_V18; rem_3_white = rem_3_white | BM3_V18; }
		if ((white_man & V19_W) == V19_W) {sel_4_white = sel_4_white | BM4_V19; rem_3_white = rem_3_white | BM3_V19; }
		if ((white_man & V2_W) == V2_W) {sel_4_white = sel_4_white | BM4_V2; rem_3_white = rem_3_white | BM3_V2; }
		if ((white_man & V20_W) == V20_W) {sel_4_white = sel_4_white | BM4_V20; rem_3_white = rem_3_white | BM3_V20; }
		if ((white_man & V21_W) == V21_W) {/*sel_4_white = sel_4_white | BM4_V21;*/ rem_3_white = rem_3_white | BM3_V21; }
		if ((white_man & V22_W) == V22_W) {sel_4_white = sel_4_white | BM4_V22; rem_3_white = rem_3_white | BM3_V22; }
		if ((white_man & V23_W) == V23_W) {sel_4_white = sel_4_white | BM4_V23; rem_3_white = rem_3_white | BM3_V23; }
		if ((white_man & V24_W) == V24_W) {sel_4_white = sel_4_white | BM4_V24; rem_3_white = rem_3_white | BM3_V24; }
		if ((white_man & V25_W) == V25_W) {sel_4_white = sel_4_white | BM4_V25; rem_3_white = rem_3_white | BM3_V25; }
		if ((white_man & V26_W) == V26_W) {sel_4_white = sel_4_white | BM4_V26; rem_3_white = rem_3_white | BM3_V26; }
		if ((white_man & V27_W) == V27_W) {sel_4_white = sel_4_white | BM4_V27; rem_3_white = rem_3_white | BM3_V27; }
		if ((white_man & V28_W) == V28_W) {sel_4_white = sel_4_white | BM4_V28; rem_3_white = rem_3_white | BM3_V28; }
		if ((white_man & V29_W) == V29_W) {sel_4_white = sel_4_white | BM4_V29; rem_3_white = rem_3_white | BM3_V29; }
		if ((white_man & V3_W) == V3_W) {sel_4_white = sel_4_white | BM4_V3; rem_3_white = rem_3_white | BM3_V3; }
		if ((white_man & V30_W) == V30_W) {sel_4_white = sel_4_white | BM4_V30; rem_3_white = rem_3_white | BM3_V30; }
		if ((white_man & V31_W) == V31_W) {sel_4_white = sel_4_white | BM4_V31; rem_3_white = rem_3_white | BM3_V31; }
		if ((white_man & V32_W) == V32_W) {sel_4_white = sel_4_white | BM4_V32; rem_3_white = rem_3_white | BM3_V32; }
		if ((white_man & V33_W) == V33_W) {sel_4_white = sel_4_white | BM4_V33; rem_3_white = rem_3_white | BM3_V33; }
		if ((white_man & V34_W) == V34_W) {sel_4_white = sel_4_white | BM4_V34; rem_3_white = rem_3_white | BM3_V34; }
		if ((white_man & V35_W) == V35_W) {sel_4_white = sel_4_white | BM4_V35; rem_3_white = rem_3_white | BM3_V35; }
		if ((white_man & V36_W) == V36_W) {sel_4_white = sel_4_white | BM4_V36; rem_3_white = rem_3_white | BM3_V36; }
		if ((white_man & V37_W) == V37_W) {sel_4_white = sel_4_white | BM4_V37; rem_3_white = rem_3_white | BM3_V37; }
		if ((white_man & V38_W) == V38_W) {sel_4_white = sel_4_white | BM4_V38; rem_3_white = rem_3_white | BM3_V38; }
		if ((white_man & V39_W) == V39_W) {sel_4_white = sel_4_white | BM4_V39; rem_3_white = rem_3_white | BM3_V39; }
		if ((white_man & V4_W) == V4_W) {sel_4_white = sel_4_white | BM4_V4; rem_3_white = rem_3_white | BM3_V4; }
		if ((white_man & V40_W) == V40_W) {sel_4_white = sel_4_white | BM4_V40; rem_3_white = rem_3_white | BM3_V40; }
		if ((white_man & V41_W) == V41_W) {sel_4_white = sel_4_white | BM4_V41; rem_3_white = rem_3_white | BM3_V41; }
		if ((white_man & V42_W) == V42_W) {sel_4_white = sel_4_white | BM4_V42; rem_3_white = rem_3_white | BM3_V42; }
		if ((white_man & V5_W) == V5_W) {sel_4_white = sel_4_white | BM4_V5; rem_3_white = rem_3_white | BM3_V5; }
		if ((white_man & V6_W) == V6_W) {sel_4_white = sel_4_white | BM4_V6; rem_3_white = rem_3_white | BM3_V6; }
		if ((white_man & V7_W) == V7_W) {sel_4_white = sel_4_white | BM4_V7; rem_3_white = rem_3_white | BM3_V7; }
		if ((white_man & V8_W) == V8_W) {sel_4_white = sel_4_white | BM4_V8; rem_3_white = rem_3_white | BM3_V8; }
		if ((white_man & V9_W) == V9_W) {sel_4_white = sel_4_white | BM4_V9; rem_3_white = rem_3_white | BM3_V9; }
		if ((white_man & Z1_W) == Z1_W) {sel_6_white = sel_6_white | BM6_Z1; rem_5_white = rem_5_white | BM5_Z1; rem_4_white = rem_4_white | BM4_Z1; rem_3_white = rem_3_white | BM3_Z1; }
		if ((white_man & Z10_W) == Z10_W) {/*sel_6_white = sel_6_white | BM6_Z10;*/ rem_5_white = rem_5_white | BM5_Z10; rem_4_white = rem_4_white | BM4_Z10; rem_3_white = rem_3_white | BM3_Z10; }
		if ((white_man & Z11_W) == Z11_W) {sel_6_white = sel_6_white | BM6_Z11; rem_5_white = rem_5_white | BM5_Z11; rem_4_white = rem_4_white | BM4_Z11; rem_3_white = rem_3_white | BM3_Z11; }
		if ((white_man & Z12_W) == Z12_W) {sel_6_white = sel_6_white | BM6_Z12; rem_5_white = rem_5_white | BM5_Z12; rem_4_white = rem_4_white | BM4_Z12; rem_3_white = rem_3_white | BM3_Z12; }
		if ((white_man & Z13_W) == Z13_W) {sel_6_white = sel_6_white | BM6_Z13; rem_5_white = rem_5_white | BM5_Z13; rem_4_white = rem_4_white | BM4_Z13; rem_3_white = rem_3_white | BM3_Z13; }
		if ((white_man & Z14_W) == Z14_W) {sel_6_white = sel_6_white | BM6_Z14; rem_5_white = rem_5_white | BM5_Z14; rem_4_white = rem_4_white | BM4_Z14; rem_3_white = rem_3_white | BM3_Z14; }
		if ((white_man & Z15_W) == Z15_W) {sel_6_white = sel_6_white | BM6_Z15; rem_5_white = rem_5_white | BM5_Z15; rem_4_white = rem_4_white | BM4_Z15; rem_3_white = rem_3_white | BM3_Z15; }
		if ((white_man & Z16_W) == Z16_W) {sel_6_white = sel_6_white | BM6_Z16; rem_5_white = rem_5_white | BM5_Z16; rem_4_white = rem_4_white | BM4_Z16; rem_3_white = rem_3_white | BM3_Z16; }
		if ((white_man & Z17_W) == Z17_W) {sel_6_white = sel_6_white | BM6_Z17; rem_5_white = rem_5_white | BM5_Z17; rem_4_white = rem_4_white | BM4_Z17; rem_3_white = rem_3_white | BM3_Z17; }
		if ((white_man & Z18_W) == Z18_W) {sel_6_white = sel_6_white | BM6_Z18; rem_5_white = rem_5_white | BM5_Z18; rem_4_white = rem_4_white | BM4_Z18; rem_3_white = rem_3_white | BM3_Z18; }
		if ((white_man & Z19_W) == Z19_W) {sel_6_white = sel_6_white | BM6_Z19; rem_5_white = rem_5_white | BM5_Z19; rem_4_white = rem_4_white | BM4_Z19; rem_3_white = rem_3_white | BM3_Z19; }
		if ((white_man & Z2_W) == Z2_W) {sel_6_white = sel_6_white | BM6_Z2; rem_5_white = rem_5_white | BM5_Z2; rem_4_white = rem_4_white | BM4_Z2; rem_3_white = rem_3_white | BM3_Z2; }
		if ((white_man & Z20_W) == Z20_W) {sel_6_white = sel_6_white | BM6_Z20; rem_5_white = rem_5_white | BM5_Z20; rem_4_white = rem_4_white | BM4_Z20; rem_3_white = rem_3_white | BM3_Z20; }
		if ((white_man & Z3_W) == Z3_W) {sel_6_white = sel_6_white | BM6_Z3; rem_5_white = rem_5_white | BM5_Z3; rem_4_white = rem_4_white | BM4_Z3; rem_3_white = rem_3_white | BM3_Z3; }
		if ((white_man & Z4_W) == Z4_W) {sel_6_white = sel_6_white | BM6_Z4; rem_5_white = rem_5_white | BM5_Z4; rem_4_white = rem_4_white | BM4_Z4; rem_3_white = rem_3_white | BM3_Z4; }
		if ((white_man & Z5_W) == Z5_W) {sel_6_white = sel_6_white | BM6_Z5; rem_5_white = rem_5_white | BM5_Z5; rem_4_white = rem_4_white | BM4_Z5; rem_3_white = rem_3_white | BM3_Z5; }
		if ((white_man & Z6_W) == Z6_W) {sel_6_white = sel_6_white | BM6_Z6; rem_5_white = rem_5_white | BM5_Z6; rem_4_white = rem_4_white | BM4_Z6; rem_3_white = rem_3_white | BM3_Z6; }
		if ((white_man & Z7_W) == Z7_W) {sel_6_white = sel_6_white | BM6_Z7; rem_5_white = rem_5_white | BM5_Z7; rem_4_white = rem_4_white | BM4_Z7; rem_3_white = rem_3_white | BM3_Z7; }
		if ((white_man & Z8_W) == Z8_W) {sel_6_white = sel_6_white | BM6_Z8; rem_5_white = rem_5_white | BM5_Z8; rem_4_white = rem_4_white | BM4_Z8; rem_3_white = rem_3_white | BM3_Z8; }
		if ((white_man & Z9_W) == Z9_W) {sel_6_white = sel_6_white | BM6_Z9; rem_5_white = rem_5_white | BM5_Z9; rem_4_white = rem_4_white | BM4_Z9; rem_3_white = rem_3_white | BM3_Z9; }
		sel_5_white = sel_5_white & ~rem_5_white;
		sel_4_white = sel_4_white & ~rem_4_white;
		sel_3_white = sel_3_white & ~rem_3_white;

	}

}
