/*
  This file is Copyright (C) 2010 Feike Boomstra.
  Distributed under the Boost Software License, Version 1.0.
  (See accompanying file LICENSE_1_0.txt or copy at
  http://www.boost.org/LICENSE_1_0.txt)
*/


// upper and lower limits for distribution of men over columns of width n

char warn_2_ge[21] = {1,1,2,2,2,2,2,3,3,3,3,4,4,4,4,5,5,5,5,6,6};
char warn_3_ge[21] = {1,1,2,2,2,3,3,4,4,4,5,5,5,6,6,7,7,7,8,8,8};
char warn_4_ge[21] = {1,1,2,2,3,3,4,4,5,5,6,6,7,7,8,8,9,9,10,10,11};
char warn_5_ge[21] = {1,2,2,3,3,4,5,5,6,7,7,8,8,9,10,10,11,11,12,13,13};
char warn_2_le[21] = {-1,-1,-1,0,0,0,0,0,0,0,1,1,1,1,1,1,1,2,2,2,2};
char warn_3_le[21] = {-1,-1,0,0,0,0,0,1,1,1,1,1,2,2,2,2,3,3,3,3,4};
char warn_4_le[21] = {-1,-1,0,0,0,1,1,1,1,2,2,2,3,3,3,4,4,4,5,5,5};
char warn_5_le[21] = {-1,-1,0,0,1,1,1,2,2,2,3,3,4,4,4,5,5,6,6,6,7};
char bad_2_ge[21] = {1,2,2,2,2,2,3,3,3,4,4,4,4,5,5,5,6,6,6,6,7};
char bad_3_ge[21] = {1,2,2,2,3,3,4,4,4,5,5,6,6,6,7,7,8,8,8,9,9};
char bad_4_ge[21] = {1,2,2,3,3,4,4,5,6,6,7,7,8,8,9,9,10,10,11,11,12};
char bad_5_ge[21] = {1,2,2,3,4,5,5,6,7,7,8,9,9,10,10,11,12,12,13,14,14};
char bad_2_le[21] = {-1,-1,-1,-1,-1,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1};
char bad_3_le[21] = {-1,-1,-1,0,0,0,0,0,0,1,1,1,1,1,2,2,2,2,2,3,3};
char bad_4_le[21] = {-1,-1,-1,0,0,0,0,1,1,1,1,2,2,2,3,3,3,3,4,4,4};
char bad_5_le[21] = {-1,-1,0,0,0,0,1,1,1,2,2,2,3,3,4,4,4,5,5,5,6};
char panic_2_ge[21] = {2,2,2,2,3,3,3,3,4,4,4,5,5,5,6,6,6,7,7,7,7};
char panic_3_ge[21] = {2,2,2,3,3,4,4,5,5,5,6,6,7,7,8,8,9,9,9,10,10};
char panic_4_ge[21] = {2,2,3,3,4,4,5,6,6,7,7,8,9,9,10,10,11,11,12,12,13};
char panic_5_ge[21] = {2,2,3,4,4,5,6,7,7,8,9,9,10,11,11,12,13,13,14,15,15};
char panic_2_le[21] = {-2,-2,-1,-1,-1,-1,-1,-1,-1,0,0,0,0,0,0,0,0,0,0,0,1};
char panic_3_le[21] = {-2,-1,-1,-1,-1,-1,0,0,0,0,0,0,0,1,1,1,1,1,1,2,2};
char panic_4_le[21] = {-2,-1,-1,-1,-1,0,0,0,0,0,1,1,1,1,2,2,2,2,3,3,3};
char panic_5_le[21] = {-2,-1,-1,-1,0,0,0,0,1,1,1,2,2,2,3,3,3,4,4,4,5};

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

#define W2_1 (COL1 | COL2)
#define W2_2 (COL2 | COL3)
#define W2_3 (COL3 | COL4)
#define W2_4 (COL4 | COL5)
#define W2_5 (COL5 | COL6)
#define W2_6 (COL6 | COL7)
#define W2_7 (COL7 | COL8)
#define W2_8 (COL8 | COL9)
#define W2_9 (COL9 | COL10)

#define W3_1 (COL1 | COL2 | COL3)
#define W3_2 (COL2 | COL3 | COL4)
#define W3_3 (COL3 | COL4 | COL5)
#define W3_4 (COL4 | COL5 | COL6)
#define W3_5 (COL5 | COL6 | COL7)
#define W3_6 (COL6 | COL7 | COL8)
#define W3_7 (COL7 | COL8 | COL9)
#define W3_8 (COL8 | COL9 | COL10)

#define W4_1 (COL1 | COL2 | COL3 | COL4)
#define W4_2 (COL2 | COL3 | COL4 | COL5)
#define W4_3 (COL3 | COL4 | COL5 | COL6)
#define W4_4 (COL4 | COL5 | COL6 | COL7)
#define W4_5 (COL5 | COL6 | COL7 | COL8)
#define W4_6 (COL6 | COL7 | COL8 | COL9)
#define W4_7 (COL7 | COL8 | COL9 | COL10)

#define W5_1 (COL1 | COL2 | COL3 | COL4 | COL5)
#define W5_2 (COL2 | COL3 | COL4 | COL5 | COL6)
#define W5_3 (COL3 | COL4 | COL5 | COL6 | COL7)
#define W5_4 (COL4 | COL5 | COL6 | COL7 | COL8)
#define W5_5 (COL5 | COL6 | COL7 | COL8 | COL9)
#define W5_6 (COL6 | COL7 | COL8 | COL9 | COL10)

BitArray w2[9] = {W2_1, W2_2, W2_3, W2_4, W2_5, W2_6, W2_7, W2_8, W2_9};
BitArray w3[8] = {W3_1, W3_2, W3_3, W3_4, W3_5, W3_6, W3_7, W3_8};
BitArray w4[7] = {W4_1, W4_2, W4_3, W4_4, W4_5, W4_6, W4_7};
BitArray w5[6] = {W5_1, W5_2, W5_3, W5_4, W5_5, W5_6};





{	int temp_5_max = -1, temp_4_max = -1, temp_3_max = -1, temp_2_max = -1;
			int temp_5_min = 20, temp_4_min = 20, temp_3_min = 20, temp_2_min = 20;

			for (i=0; i<6; i++)
			{	int temp;
				count_bits(white_man & w5[i], temp);
				if (temp < temp_5_min) temp_5_min = temp;
				else if (temp > temp_5_max) temp_5_max = temp;
			}
			if (temp_5_max >= panic_5_ge[nr_of_white_man]) { white_fit_score = panic_5; goto end_white_distribution;}
			if (temp_5_min <= panic_5_le[nr_of_white_man]) { white_fit_score = panic_5; goto end_white_distribution;}
			for (i=0; i<7; i++)
			{	int temp;
				count_bits(white_man & w4[i], temp);
				if (temp < temp_4_min) temp_4_min = temp;
				else if (temp > temp_4_max) temp_4_max = temp;
			}
			if (temp_4_max >= panic_4_ge[nr_of_white_man]) { white_fit_score = panic_4; goto end_white_distribution;}
			if (temp_4_min <= panic_4_le[nr_of_white_man]) { white_fit_score = panic_4; goto end_white_distribution;}
			for (i=0; i<8; i++)
			{	int temp;
				count_bits(white_man & w3[i], temp);
				if (temp < temp_3_min) temp_3_min = temp;
				else if (temp > temp_3_max) temp_3_max = temp;
			}
			if (temp_3_max >= panic_3_ge[nr_of_white_man]) { white_fit_score = panic_3; goto end_white_distribution;}
			if (temp_3_min <= panic_3_le[nr_of_white_man]) { white_fit_score = panic_3; goto end_white_distribution;}
			for (i=0; i<9; i++)
			{	int temp;
				count_bits(white_man & w2[i], temp);
				if (temp < temp_2_min) temp_2_min = temp;
				else if (temp > temp_2_max) temp_2_max = temp;
			}
			if (temp_2_max >= panic_2_ge[nr_of_white_man]) { white_fit_score = panic_2; goto end_white_distribution;}
			if (temp_2_min <= panic_2_le[nr_of_white_man]) { white_fit_score = panic_2; goto end_white_distribution;}
			if (temp_5_max >= bad_5_ge[nr_of_white_man]) { white_fit_score = bad_5; goto end_white_distribution;}
			if (temp_5_min <= bad_5_le[nr_of_white_man]) { white_fit_score = bad_5; goto end_white_distribution;}
			if (temp_4_max >= bad_4_ge[nr_of_white_man]) { white_fit_score = bad_4; goto end_white_distribution;}
			if (temp_4_min <= bad_4_le[nr_of_white_man]) { white_fit_score = bad_4; goto end_white_distribution;}
			if (temp_3_max >= bad_3_ge[nr_of_white_man]) { white_fit_score = bad_3; goto end_white_distribution;}
			if (temp_3_min <= bad_3_le[nr_of_white_man]) { white_fit_score = bad_3; goto end_white_distribution;}
			if (temp_2_max >= bad_2_ge[nr_of_white_man]) { white_fit_score = bad_2; goto end_white_distribution;}
			if (temp_2_min <= bad_2_le[nr_of_white_man]) { white_fit_score = bad_2; goto end_white_distribution;}
			if (temp_5_max >= warn_5_ge[nr_of_white_man]) { white_fit_score = warn_5; goto end_white_distribution;}
			if (temp_5_min <= warn_5_le[nr_of_white_man]) { white_fit_score = warn_5; goto end_white_distribution;}
			if (temp_4_max >= warn_4_ge[nr_of_white_man]) { white_fit_score = warn_4; goto end_white_distribution;}
			if (temp_4_min <= warn_4_le[nr_of_white_man]) { white_fit_score = warn_4; goto end_white_distribution;}
			if (temp_3_max >= warn_3_ge[nr_of_white_man]) { white_fit_score = warn_3; goto end_white_distribution;}
			if (temp_3_min <= warn_3_le[nr_of_white_man]) { white_fit_score = warn_3; goto end_white_distribution;}
			if (temp_2_max >= warn_2_ge[nr_of_white_man]) { white_fit_score = warn_2; goto end_white_distribution;}
			if (temp_2_min <= warn_2_le[nr_of_white_man]) { white_fit_score = warn_2; goto end_white_distribution;}
			white_fit_score = 0;
		}

end_white_distribution:
		
		// distribution of black man
		{	int temp_5_max = -1, temp_4_max = -1, temp_3_max = -1, temp_2_max = -1;
			int temp_5_min = 20, temp_4_min = 20, temp_3_min = 20, temp_2_min = 20;

			for (i=0; i<6; i++)
			{	int temp;
				count_bits(black_man & w5[i], temp);
				if (temp < temp_5_min) temp_5_min = temp;
				else if (temp > temp_5_max) temp_5_max = temp;
			}
			if (temp_5_max >= panic_5_ge[nr_of_black_man]) { black_fit_score = panic_5; goto end_black_distribution;}
			if (temp_5_min <= panic_5_le[nr_of_black_man]) { black_fit_score = panic_5; goto end_black_distribution;}
			for (i=0; i<7; i++)
			{	int temp;
				count_bits(black_man & w4[i], temp);
				if (temp < temp_4_min) temp_4_min = temp;
				else if (temp > temp_4_max) temp_4_max = temp;
			}
			if (temp_4_max >= panic_4_ge[nr_of_black_man]) { black_fit_score = panic_4; goto end_black_distribution;}
			if (temp_4_min <= panic_4_le[nr_of_black_man]) { black_fit_score = panic_4; goto end_black_distribution;}
			for (i=0; i<8; i++)
			{	int temp;
				count_bits(black_man & w3[i], temp);
				if (temp < temp_3_min) temp_3_min = temp;
				else if (temp > temp_3_max) temp_3_max = temp;
			}
			if (temp_3_max >= panic_3_ge[nr_of_black_man]) { black_fit_score = panic_3; goto end_black_distribution;}
			if (temp_3_min <= panic_3_le[nr_of_black_man]) { black_fit_score = panic_3; goto end_black_distribution;}
			for (i=0; i<9; i++)
			{	int temp;
				count_bits(black_man & w2[i], temp);
				if (temp < temp_2_min) temp_2_min = temp;
				else if (temp > temp_2_max) temp_2_max = temp;
			}
			if (temp_2_max >= panic_2_ge[nr_of_black_man]) { black_fit_score = panic_2; goto end_black_distribution;}
			if (temp_2_min <= panic_2_le[nr_of_black_man]) { black_fit_score = panic_2; goto end_black_distribution;}
			if (temp_5_max >= bad_5_ge[nr_of_black_man]) { black_fit_score = bad_5; goto end_black_distribution;}
			if (temp_5_min <= bad_5_le[nr_of_black_man]) { black_fit_score = bad_5; goto end_black_distribution;}
			if (temp_4_max >= bad_4_ge[nr_of_black_man]) { black_fit_score = bad_4; goto end_black_distribution;}
			if (temp_4_min <= bad_4_le[nr_of_black_man]) { black_fit_score = bad_4; goto end_black_distribution;}
			if (temp_3_max >= bad_3_ge[nr_of_black_man]) { black_fit_score = bad_3; goto end_black_distribution;}
			if (temp_3_min <= bad_3_le[nr_of_black_man]) { black_fit_score = bad_3; goto end_black_distribution;}
			if (temp_2_max >= bad_2_ge[nr_of_black_man]) { black_fit_score = bad_2; goto end_black_distribution;}
			if (temp_2_min <= bad_2_le[nr_of_black_man]) { black_fit_score = bad_2; goto end_black_distribution;}
			if (temp_5_max >= warn_5_ge[nr_of_black_man]) { black_fit_score = warn_5; goto end_black_distribution;}
			if (temp_5_min <= warn_5_le[nr_of_black_man]) { black_fit_score = warn_5; goto end_black_distribution;}
			if (temp_4_max >= warn_4_ge[nr_of_black_man]) { black_fit_score = warn_4; goto end_black_distribution;}
			if (temp_4_min <= warn_4_le[nr_of_black_man]) { black_fit_score = warn_4; goto end_black_distribution;}
			if (temp_3_max >= warn_3_ge[nr_of_black_man]) { black_fit_score = warn_3; goto end_black_distribution;}
			if (temp_3_min <= warn_3_le[nr_of_black_man]) { black_fit_score = warn_3; goto end_black_distribution;}
			if (temp_2_max >= warn_2_ge[nr_of_black_man]) { black_fit_score = warn_2; goto end_black_distribution;}
			if (temp_2_min <= warn_2_le[nr_of_black_man]) { black_fit_score = warn_2; goto end_black_distribution;}
			black_fit_score = 0;
		}
end_black_distribution: ;


