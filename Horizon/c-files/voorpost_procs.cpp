/*
  This file is Copyright (C) 2010 Feike Boomstra.
  Distributed under the Boost Software License, Version 1.0.
  (See accompanying file LICENSE_1_0.txt or copy at
  http://www.boost.org/LICENSE_1_0.txt)
*/


#include "BoomstraDam.v2.5.h"
extern int calc_dist_9(BitArray, BitArray, BitArray, BitArray, BitArray, BitArray, BitArray, BitArray, BitArray, BitArray *, BitArray);
extern int calc_dist_8(BitArray, BitArray, BitArray, BitArray, BitArray, BitArray, BitArray, BitArray, BitArray *, BitArray);
extern int calc_dist_7(BitArray, BitArray, BitArray, BitArray, BitArray, BitArray, BitArray, BitArray *, BitArray);
extern int calc_dist_6(BitArray, BitArray, BitArray, BitArray, BitArray, BitArray, BitArray *, BitArray);
extern int calc_dist_5(BitArray, BitArray, BitArray, BitArray, BitArray, BitArray *, BitArray);
extern int calc_dist_4(BitArray, BitArray, BitArray, BitArray, BitArray *, BitArray);
extern int calc_dist_3(BitArray, BitArray, BitArray, BitArray *, BitArray);
extern int calc_dist_2(BitArray, BitArray, BitArray *, BitArray);
extern int calc_dist_1(BitArray, BitArray *, BitArray);
// witte voorposten se
extern int voorpost_white_se_17(BitArray, BitArray, BitArray, int);
int voorpost_white_se_17(BitArray white_man, BitArray black_man, BitArray empty_field, int inc) {
BitArray my_pieces_used, his_pieces_used;
int number_of_defences, number_of_attacks = 0;
int his_min_backing_steps, my_min_backing_steps;
int my_min_defence_steps, his_min_attack_steps;
bool backing_defence_piece, backing_attack_piece;

if ((white_man & FLD17) &&  (all_empty_or_black((FLD11|FLD6))) &&  (all_empty_or_white((FLD28|FLD33))) &&  (empty_field & FLD22)) {
//so this is what anyway has to be there to have a voorpost

//if the field sw of b is occupied by me or can be occupied by me, no attack is possible, so the question is, can the piece on sw(b) be exchanged
if (white_man & FLD16) {
if ((all_black((FLD7|FLD2|FLD0)) && (empty_field & FLD11)) &&
    !(((black_man & FLD12) && (empty_field & FLD18)) ||  ((black_man & FLD1) && (empty_field & FLD0))))
number_of_attacks = -1;  // can be exchanged,  start with one attack less
else goto check_afruil; // no attacks possible
}

//same for the defence: if ne(d) is occupied by him, we may have a problem in the defence
if (black_man & FLD23) {
if ((all_white((FLD32|FLD39|FLD41)) && (empty_field & FLD28)) ||
    (all_white((FLD28|FLD37|FLD41)) && (empty_field & FLD32)))
number_of_defences = count(((FLD28|FLD32|FLD37|FLD38|FLD41|FLD42|FLD43|FLD46|FLD47|FLD48|FLD49) & white_man)) - 1;
else number_of_defences = 0;
} else number_of_defences = count(((FLD28|FLD32|FLD37|FLD38|FLD41|FLD42|FLD43|FLD46|FLD47|FLD48|FLD49) & white_man));

//check the availability of the backing pieces
backing_defence_piece = bool ((FLD33|FLD38|FLD39|FLD42|FLD43|FLD44|FLD47|FLD48|FLD49|FLD50) & white_man);
backing_attack_piece = (bool) ((FLD1|FLD6) & black_man);
number_of_attacks += count(((FLD1|FLD2|FLD7|FLD11) & black_man));
if (!((FLD6) & black_man)) number_of_attacks -= 1;
if (!((FLD33|FLD39|FLD44|FLD50) & white_man)) number_of_defences -= 1;

//so now we know whether we have a backing piece and how many pieces for the attack/defence
if (number_of_attacks <= 0) return 180;
if (!(backing_defence_piece) && (backing_attack_piece) && (number_of_attacks > 0)) { // looks bad, but maybe an escape?
if ((white_man & FLD18) && (all_black((FLD12|FLD7))) && ( all_empty((FLD11|FLD16)))) return 0;
his_pieces_used = 0;   // calc the minimal amount of steps for having the backing piece in place as well as the attack resp.defence piece
his_min_backing_steps = calc_dist_2( (FLD6), (FLD1),  &his_pieces_used, black_man);
his_min_attack_steps = calc_dist_3( (FLD11), (FLD7), (FLD1|FLD2),  &his_pieces_used, black_man);
if ((all_empty_or_white((FLD18|FLD27)))  || (all_occupied((FLD18|FLD27)))) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_5( (FLD28), (FLD32|FLD33), (FLD37|FLD38|FLD39), (FLD41|FLD42|FLD43|FLD44), (FLD46|FLD47|FLD48|FLD49|FLD50),  &my_pieces_used, white_man) + 1;
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if (empty_or_white & FLD18) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_6( (FLD22), (FLD27|FLD28), (FLD31|FLD32|FLD33), (FLD36|FLD37|FLD38|FLD39), (FLD41|FLD42|FLD43|FLD44), (FLD46|FLD47|FLD48|FLD49|FLD50),  &my_pieces_used, white_man);
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if ((all_white((FLD21|FLD26))) && (all_empty(( FLD12|FLD8))))  return -200; // vlucht
if ((occupied & FLD18) && (all_empty(( FLD12|FLD8))))  return -200; // vlucht
if ((all_white((FLD21|FLD26))) && (black_man & FLD8) && (occupied & FLD18) && (empty_field & FLD12)) return 0; //paddestoel
if ((all_white((FLD21|FLD26))) && (black_man & FLD8) && (empty_or_white &FLD18) && (empty_field & FLD12)) return 0; //kan wegruilen
 return -700;
}

if (!(backing_attack_piece)) goto check_afruil;
// both backings are available
if (number_of_attacks > number_of_defences) {  // it looks bad, but maybe an escape?
if ((white_man & FLD18) && (all_black((FLD12|FLD7))) && ( all_empty((FLD11|FLD16)))) return 0;
his_pieces_used = 0;   // calc the minimal amount of steps for having the backing piece in place as well as the attack resp.defence piece
his_min_backing_steps = calc_dist_2( (FLD6), (FLD1),  &his_pieces_used, black_man);
his_min_attack_steps = calc_dist_3( (FLD11), (FLD7), (FLD1|FLD2),  &his_pieces_used, black_man);
if ((all_empty_or_white((FLD18|FLD27)))  || (all_occupied((FLD18|FLD27)))) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_5( (FLD28), (FLD32|FLD33), (FLD37|FLD38|FLD39), (FLD41|FLD42|FLD43|FLD44), (FLD46|FLD47|FLD48|FLD49|FLD50),  &my_pieces_used, white_man) + 1;
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if (empty_or_white & FLD18) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_6( (FLD22), (FLD27|FLD28), (FLD31|FLD32|FLD33), (FLD36|FLD37|FLD38|FLD39), (FLD41|FLD42|FLD43|FLD44), (FLD46|FLD47|FLD48|FLD49|FLD50),  &my_pieces_used, white_man);
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if ((all_white((FLD21|FLD26))) && (all_empty(( FLD12|FLD8))))  return -200; // vlucht
if ((occupied & FLD18) && (all_empty(( FLD12|FLD8))))  return -200; // vlucht
if ((all_white((FLD21|FLD26))) && (black_man & FLD8) && (occupied & FLD18) && (empty_field & FLD12)) return 0; //paddestoel
if ((all_white((FLD21|FLD26))) && (black_man & FLD8) && (empty_or_white &FLD18) && (empty_field & FLD12)) return 0; //kan wegruilen
return -700;
}

//calc the minimal amount of steps for having the backing piece in place as well as the attack resp.defence piece
his_pieces_used = 0;
his_min_backing_steps = calc_dist_2( (FLD6), (FLD1),  &his_pieces_used, black_man);
his_min_attack_steps = calc_dist_3( (FLD11), (FLD7), (FLD1|FLD2),  &his_pieces_used, black_man);
my_pieces_used = 0;
my_min_backing_steps = calc_dist_4( (FLD33), (FLD38|FLD39), (FLD42|FLD43|FLD44), (FLD47|FLD48|FLD49|FLD50),  &my_pieces_used, white_man);
my_min_defence_steps = calc_dist_5( (FLD28), (FLD32), (FLD37|FLD38), (FLD41|FLD42|FLD43), (FLD46|FLD47|FLD48|FLD49),  &my_pieces_used, white_man);

// we have calculated: his_min_backing_steps, his_min_attack_steps, my_min_defence_steps and my_min_backing steps

if ((his_min_backing_steps + his_min_attack_steps) - (my_min_backing_steps + my_min_defence_steps + inc) < -2) {
if ((white_man & FLD18) && (all_black((FLD12|FLD7))) && ( all_empty((FLD11|FLD16)))) return 0;
his_pieces_used = 0;   // calc the minimal amount of steps for having the backing piece in place as well as the attack resp.defence piece
his_min_backing_steps = calc_dist_2( (FLD6), (FLD1),  &his_pieces_used, black_man);
his_min_attack_steps = calc_dist_3( (FLD11), (FLD7), (FLD1|FLD2),  &his_pieces_used, black_man);
if ((all_empty_or_white((FLD18|FLD27)))  || (all_occupied((FLD18|FLD27)))) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_5( (FLD28), (FLD32|FLD33), (FLD37|FLD38|FLD39), (FLD41|FLD42|FLD43|FLD44), (FLD46|FLD47|FLD48|FLD49|FLD50),  &my_pieces_used, white_man) + 1;
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if (empty_or_white & FLD18) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_6( (FLD22), (FLD27|FLD28), (FLD31|FLD32|FLD33), (FLD36|FLD37|FLD38|FLD39), (FLD41|FLD42|FLD43|FLD44), (FLD46|FLD47|FLD48|FLD49|FLD50),  &my_pieces_used, white_man);
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if ((all_white((FLD21|FLD26))) && (all_empty(( FLD12|FLD8))))  return -200; // vlucht
if ((occupied & FLD18) && (all_empty(( FLD12|FLD8))))  return -200; // vlucht
if ((all_white((FLD21|FLD26))) && (black_man & FLD8) && (occupied & FLD18) && (empty_field & FLD12)) return 0; //paddestoel
if ((all_white((FLD21|FLD26))) && (black_man & FLD8) && (empty_or_white &FLD18) && (empty_field & FLD12)) return 0; //kan wegruilen
return -700;
}

// and a second attack??

if (number_of_attacks > 1) {
his_min_attack_steps = calc_dist_3( (FLD11), (FLD7), (FLD1|FLD2),  &his_pieces_used, black_man);
my_min_defence_steps = calc_dist_5( (FLD28), (FLD32), (FLD37|FLD38), (FLD41|FLD42|FLD43), (FLD46|FLD47|FLD48|FLD49),  &my_pieces_used, white_man);
if ((his_min_attack_steps) - (my_min_defence_steps + inc) < -2) { // too late for the second attempt
if ((white_man & FLD18) && (all_black((FLD12|FLD7))) && ( all_empty((FLD11|FLD16)))) return 0;
his_pieces_used = 0;   // calc the minimal amount of steps for having the backing piece in place as well as the attack resp.defence piece
his_min_backing_steps = calc_dist_2( (FLD6), (FLD1),  &his_pieces_used, black_man);
his_min_attack_steps = calc_dist_3( (FLD11), (FLD7), (FLD1|FLD2),  &his_pieces_used, black_man);
if ((all_empty_or_white((FLD18|FLD27)))  || (all_occupied((FLD18|FLD27)))) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_5( (FLD28), (FLD32|FLD33), (FLD37|FLD38|FLD39), (FLD41|FLD42|FLD43|FLD44), (FLD46|FLD47|FLD48|FLD49|FLD50),  &my_pieces_used, white_man) + 1;
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if (empty_or_white & FLD18) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_6( (FLD22), (FLD27|FLD28), (FLD31|FLD32|FLD33), (FLD36|FLD37|FLD38|FLD39), (FLD41|FLD42|FLD43|FLD44), (FLD46|FLD47|FLD48|FLD49|FLD50),  &my_pieces_used, white_man);
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if ((all_white((FLD21|FLD26))) && (all_empty(( FLD12|FLD8))))  return -200; // vlucht
if ((occupied & FLD18) && (all_empty(( FLD12|FLD8))))  return -200; // vlucht
if ((all_white((FLD21|FLD26))) && (black_man & FLD8) && (occupied & FLD18) && (empty_field & FLD12)) return 0; //paddestoel
if ((all_white((FLD21|FLD26))) && (black_man & FLD8) && (empty_or_white &FLD18) && (empty_field & FLD12)) return 0; //kan wegruilen
return -400;
}
}
check_afruil:
if (all_black((FLD3|FLD8))) return 0; // voorpost wordt afgeruild
// so we have hope to have a nice voorpost
return 180;  // no problems, so a small advantage
}
return 0;
}
extern int voorpost_white_se_18(BitArray, BitArray, BitArray, int);
int voorpost_white_se_18(BitArray white_man, BitArray black_man, BitArray empty_field, int inc) {
BitArray my_pieces_used, his_pieces_used;
int number_of_defences, number_of_attacks = 0;
int his_min_backing_steps, my_min_backing_steps;
int my_min_defence_steps, his_min_attack_steps;
bool backing_defence_piece, backing_attack_piece;

if ((white_man & FLD18) &&  (all_empty_or_black((FLD12|FLD7))) &&  (all_empty_or_white((FLD29|FLD34))) &&  (empty_field & FLD23)) {
//so this is what anyway has to be there to have a voorpost

//if the field sw of b is occupied by me or can be occupied by me, no attack is possible, so the question is, can the piece on sw(b) be exchanged
if ((white_man & FLD17) ||  ((white_man & FLD21) && ((all_empty_or_white(FLD11|FLD22)) ||  (all_occupied(FLD11|FLD22))))) {
if ((all_black((FLD8|FLD3|FLD0)) && (empty_field & FLD12)) &&
    !(((black_man & FLD13) && (empty_field & FLD19)) ||  ((black_man & FLD2) && (empty_field & FLD0))))
number_of_attacks = -1;  // can be exchanged,  start with one attack less
else goto check_afruil; // no attacks possible
}

//same for the defence: if ne(d) is occupied by him, we may have a problem in the defence
if (black_man & FLD24) {
if ((all_white((FLD33|FLD40|FLD42)) && (empty_field & FLD29)) ||
    (all_white((FLD29|FLD38|FLD42)) && (empty_field & FLD33)))
number_of_defences = count(((FLD29|FLD33|FLD38|FLD39|FLD42|FLD43|FLD44|FLD47|FLD48|FLD49|FLD50) & white_man)) - 1;
else number_of_defences = 0;
} else number_of_defences = count(((FLD29|FLD33|FLD38|FLD39|FLD42|FLD43|FLD44|FLD47|FLD48|FLD49|FLD50) & white_man));

//check the availability of the backing pieces
backing_defence_piece = bool ((FLD34|FLD39|FLD40|FLD43|FLD44|FLD45|FLD48|FLD49|FLD50) & white_man);
backing_attack_piece = (bool) ((FLD1|FLD2|FLD7) & black_man);
number_of_attacks += count(((FLD2|FLD3|FLD8|FLD12) & black_man));
if (!((FLD1|FLD7) & black_man)) number_of_attacks -= 1;
if (!((FLD34|FLD40|FLD45) & white_man)) number_of_defences -= 1;

//so now we know whether we have a backing piece and how many pieces for the attack/defence
if (number_of_attacks <= 0) return 180;
if (!(backing_defence_piece) && (backing_attack_piece) && (number_of_attacks > 0)) { // looks bad, but maybe an escape?
if ((white_man & FLD19) && (all_black((FLD13|FLD8))) && ( all_empty((FLD12|FLD17)))) return 0;
his_pieces_used = 0;   // calc the minimal amount of steps for having the backing piece in place as well as the attack resp.defence piece
his_min_backing_steps = calc_dist_2( (FLD7), (FLD1|FLD2),  &his_pieces_used, black_man);
his_min_attack_steps = calc_dist_3( (FLD12), (FLD8), (FLD2|FLD3),  &his_pieces_used, black_man);
if ((all_empty_or_white((FLD19|FLD28)))  || (all_occupied((FLD19|FLD28)))) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_5( (FLD29), (FLD33|FLD34), (FLD38|FLD39|FLD40), (FLD42|FLD43|FLD44|FLD45), (FLD47|FLD48|FLD49|FLD50),  &my_pieces_used, white_man) + 1;
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if (empty_or_white & FLD19) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_6( (FLD23), (FLD28|FLD29), (FLD32|FLD33|FLD34), (FLD37|FLD38|FLD39|FLD40), (FLD41|FLD42|FLD43|FLD44|FLD45), (FLD46|FLD47|FLD48|FLD49|FLD50),  &my_pieces_used, white_man);
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if ((all_white((FLD22|FLD27))) && (all_empty(( FLD13|FLD9))))  return -200; // vlucht
if ((occupied & FLD19) && (all_empty(( FLD13|FLD9))))  return -200; // vlucht
if ((all_white((FLD22|FLD27))) && (black_man & FLD9) && (occupied & FLD19) && (empty_field & FLD13)) return 0; //paddestoel
if ((all_white((FLD22|FLD27))) && (black_man & FLD9) && (empty_or_white &FLD19) && (empty_field & FLD13)) return 0; //kan wegruilen
 return -700;
}

if (!(backing_attack_piece)) goto check_afruil;
// both backings are available
if (number_of_attacks > number_of_defences) {  // it looks bad, but maybe an escape?
if ((white_man & FLD19) && (all_black((FLD13|FLD8))) && ( all_empty((FLD12|FLD17)))) return 0;
his_pieces_used = 0;   // calc the minimal amount of steps for having the backing piece in place as well as the attack resp.defence piece
his_min_backing_steps = calc_dist_2( (FLD7), (FLD1|FLD2),  &his_pieces_used, black_man);
his_min_attack_steps = calc_dist_3( (FLD12), (FLD8), (FLD2|FLD3),  &his_pieces_used, black_man);
if ((all_empty_or_white((FLD19|FLD28)))  || (all_occupied((FLD19|FLD28)))) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_5( (FLD29), (FLD33|FLD34), (FLD38|FLD39|FLD40), (FLD42|FLD43|FLD44|FLD45), (FLD47|FLD48|FLD49|FLD50),  &my_pieces_used, white_man) + 1;
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if (empty_or_white & FLD19) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_6( (FLD23), (FLD28|FLD29), (FLD32|FLD33|FLD34), (FLD37|FLD38|FLD39|FLD40), (FLD41|FLD42|FLD43|FLD44|FLD45), (FLD46|FLD47|FLD48|FLD49|FLD50),  &my_pieces_used, white_man);
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if ((all_white((FLD22|FLD27))) && (all_empty(( FLD13|FLD9))))  return -200; // vlucht
if ((occupied & FLD19) && (all_empty(( FLD13|FLD9))))  return -200; // vlucht
if ((all_white((FLD22|FLD27))) && (black_man & FLD9) && (occupied & FLD19) && (empty_field & FLD13)) return 0; //paddestoel
if ((all_white((FLD22|FLD27))) && (black_man & FLD9) && (empty_or_white &FLD19) && (empty_field & FLD13)) return 0; //kan wegruilen
return -700;
}

//calc the minimal amount of steps for having the backing piece in place as well as the attack resp.defence piece
his_pieces_used = 0;
his_min_backing_steps = calc_dist_2( (FLD7), (FLD1|FLD2),  &his_pieces_used, black_man);
his_min_attack_steps = calc_dist_3( (FLD12), (FLD8), (FLD2|FLD3),  &his_pieces_used, black_man);
my_pieces_used = 0;
my_min_backing_steps = calc_dist_4( (FLD34), (FLD39|FLD40), (FLD43|FLD44|FLD45), (FLD48|FLD49|FLD50),  &my_pieces_used, white_man);
my_min_defence_steps = calc_dist_5( (FLD29), (FLD33), (FLD38|FLD39), (FLD42|FLD43|FLD44), (FLD47|FLD48|FLD49|FLD50),  &my_pieces_used, white_man);

// we have calculated: his_min_backing_steps, his_min_attack_steps, my_min_defence_steps and my_min_backing steps

if ((his_min_backing_steps + his_min_attack_steps) - (my_min_backing_steps + my_min_defence_steps + inc) < -2) {
if ((white_man & FLD19) && (all_black((FLD13|FLD8))) && ( all_empty((FLD12|FLD17)))) return 0;
his_pieces_used = 0;   // calc the minimal amount of steps for having the backing piece in place as well as the attack resp.defence piece
his_min_backing_steps = calc_dist_2( (FLD7), (FLD1|FLD2),  &his_pieces_used, black_man);
his_min_attack_steps = calc_dist_3( (FLD12), (FLD8), (FLD2|FLD3),  &his_pieces_used, black_man);
if ((all_empty_or_white((FLD19|FLD28)))  || (all_occupied((FLD19|FLD28)))) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_5( (FLD29), (FLD33|FLD34), (FLD38|FLD39|FLD40), (FLD42|FLD43|FLD44|FLD45), (FLD47|FLD48|FLD49|FLD50),  &my_pieces_used, white_man) + 1;
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if (empty_or_white & FLD19) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_6( (FLD23), (FLD28|FLD29), (FLD32|FLD33|FLD34), (FLD37|FLD38|FLD39|FLD40), (FLD41|FLD42|FLD43|FLD44|FLD45), (FLD46|FLD47|FLD48|FLD49|FLD50),  &my_pieces_used, white_man);
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if ((all_white((FLD22|FLD27))) && (all_empty(( FLD13|FLD9))))  return -200; // vlucht
if ((occupied & FLD19) && (all_empty(( FLD13|FLD9))))  return -200; // vlucht
if ((all_white((FLD22|FLD27))) && (black_man & FLD9) && (occupied & FLD19) && (empty_field & FLD13)) return 0; //paddestoel
if ((all_white((FLD22|FLD27))) && (black_man & FLD9) && (empty_or_white &FLD19) && (empty_field & FLD13)) return 0; //kan wegruilen
return -700;
}

// and a second attack??

if (number_of_attacks > 1) {
his_min_attack_steps = calc_dist_3( (FLD12), (FLD8), (FLD2|FLD3),  &his_pieces_used, black_man);
my_min_defence_steps = calc_dist_5( (FLD29), (FLD33), (FLD38|FLD39), (FLD42|FLD43|FLD44), (FLD47|FLD48|FLD49|FLD50),  &my_pieces_used, white_man);
if ((his_min_attack_steps) - (my_min_defence_steps + inc) < -2) { // too late for the second attempt
if ((white_man & FLD19) && (all_black((FLD13|FLD8))) && ( all_empty((FLD12|FLD17)))) return 0;
his_pieces_used = 0;   // calc the minimal amount of steps for having the backing piece in place as well as the attack resp.defence piece
his_min_backing_steps = calc_dist_2( (FLD7), (FLD1|FLD2),  &his_pieces_used, black_man);
his_min_attack_steps = calc_dist_3( (FLD12), (FLD8), (FLD2|FLD3),  &his_pieces_used, black_man);
if ((all_empty_or_white((FLD19|FLD28)))  || (all_occupied((FLD19|FLD28)))) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_5( (FLD29), (FLD33|FLD34), (FLD38|FLD39|FLD40), (FLD42|FLD43|FLD44|FLD45), (FLD47|FLD48|FLD49|FLD50),  &my_pieces_used, white_man) + 1;
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if (empty_or_white & FLD19) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_6( (FLD23), (FLD28|FLD29), (FLD32|FLD33|FLD34), (FLD37|FLD38|FLD39|FLD40), (FLD41|FLD42|FLD43|FLD44|FLD45), (FLD46|FLD47|FLD48|FLD49|FLD50),  &my_pieces_used, white_man);
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if ((all_white((FLD22|FLD27))) && (all_empty(( FLD13|FLD9))))  return -200; // vlucht
if ((occupied & FLD19) && (all_empty(( FLD13|FLD9))))  return -200; // vlucht
if ((all_white((FLD22|FLD27))) && (black_man & FLD9) && (occupied & FLD19) && (empty_field & FLD13)) return 0; //paddestoel
if ((all_white((FLD22|FLD27))) && (black_man & FLD9) && (empty_or_white &FLD19) && (empty_field & FLD13)) return 0; //kan wegruilen
return -400;
}
}
check_afruil:
if (all_black((FLD1|FLD7))) return 0; // voorpost wordt afgeruild
if (all_black((FLD4|FLD9))) return 0; // voorpost wordt afgeruild
// so we have hope to have a nice voorpost
return 180;  // no problems, so a small advantage
}
return 0;
}
extern int voorpost_white_se_19(BitArray, BitArray, BitArray, int);
int voorpost_white_se_19(BitArray white_man, BitArray black_man, BitArray empty_field, int inc) {
BitArray my_pieces_used, his_pieces_used;
int number_of_defences, number_of_attacks = 0;
int his_min_backing_steps, my_min_backing_steps;
int my_min_defence_steps, his_min_attack_steps;
bool backing_defence_piece, backing_attack_piece;

if ((white_man & FLD19) &&  (all_empty_or_black((FLD13|FLD8))) &&  (all_empty_or_white((FLD30|FLD35))) &&  (empty_field & FLD24)) {
//so this is what anyway has to be there to have a voorpost

//if the field sw of b is occupied by me or can be occupied by me, no attack is possible, so the question is, can the piece on sw(b) be exchanged
if ((white_man & FLD18) ||  ((white_man & FLD22) && ((all_empty_or_white(FLD12|FLD23)) ||  (all_occupied(FLD12|FLD23))))) {
if ((all_black((FLD9|FLD4|FLD0)) && (empty_field & FLD13)) &&
    !(((black_man & FLD14) && (empty_field & FLD20)) ||  ((black_man & FLD3) && (empty_field & FLD0))))
number_of_attacks = -1;  // can be exchanged,  start with one attack less
else goto check_afruil; // no attacks possible
}

//same for the defence: if ne(d) is occupied by him, we may have a problem in the defence
if (black_man & FLD25) {
if ((all_white((FLD34|FLD0|FLD43)) && (empty_field & FLD30)) ||
    (all_white((FLD30|FLD39|FLD43)) && (empty_field & FLD34)))
number_of_defences = count(((FLD30|FLD34|FLD39|FLD40|FLD43|FLD44|FLD45|FLD48|FLD49|FLD50) & white_man)) - 1;
else number_of_defences = 0;
} else number_of_defences = count(((FLD30|FLD34|FLD39|FLD40|FLD43|FLD44|FLD45|FLD48|FLD49|FLD50) & white_man));

//check the availability of the backing pieces
backing_defence_piece = bool ((FLD35|FLD40|FLD44|FLD45|FLD49|FLD50) & white_man);
backing_attack_piece = (bool) ((FLD2|FLD3|FLD8) & black_man);
number_of_attacks += count(((FLD3|FLD4|FLD9|FLD13) & black_man));
if (!((FLD2|FLD8) & black_man)) number_of_attacks -= 1;
if (!((FLD35) & white_man)) number_of_defences -= 1;

//so now we know whether we have a backing piece and how many pieces for the attack/defence
if (number_of_attacks <= 0) return 180;
if (!(backing_defence_piece) && (backing_attack_piece) && (number_of_attacks > 0)) { // looks bad, but maybe an escape?
if ((white_man & FLD20) && (all_black((FLD14|FLD9))) && ( all_empty((FLD13|FLD18)))) return 0;
his_pieces_used = 0;   // calc the minimal amount of steps for having the backing piece in place as well as the attack resp.defence piece
his_min_backing_steps = calc_dist_2( (FLD8), (FLD2|FLD3),  &his_pieces_used, black_man);
his_min_attack_steps = calc_dist_3( (FLD13), (FLD9), (FLD3|FLD4),  &his_pieces_used, black_man);
if ((all_empty_or_white((FLD20|FLD29)))  || (all_occupied((FLD20|FLD29)))) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_5( (FLD30), (FLD34|FLD35), (FLD39|FLD40), (FLD43|FLD44|FLD45), (FLD48|FLD49|FLD50),  &my_pieces_used, white_man) + 1;
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if (empty_or_white & FLD20) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_6( (FLD24), (FLD29|FLD30), (FLD33|FLD34|FLD35), (FLD38|FLD39|FLD40), (FLD42|FLD43|FLD44|FLD45), (FLD47|FLD48|FLD49|FLD50),  &my_pieces_used, white_man);
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if ((all_white((FLD23|FLD28))) && (all_empty(( FLD14|FLD10))))  return -200; // vlucht
if ((occupied & FLD20) && (all_empty(( FLD14|FLD10))))  return -200; // vlucht
if ((all_white((FLD23|FLD28))) && (black_man & FLD10) && (occupied & FLD20) && (empty_field & FLD14)) return 0; //paddestoel
if ((all_white((FLD23|FLD28))) && (black_man & FLD10) && (empty_or_white &FLD20) && (empty_field & FLD14)) return 0; //kan wegruilen
 return -700;
}

if (!(backing_attack_piece)) goto check_afruil;
// both backings are available
if (number_of_attacks > number_of_defences) {  // it looks bad, but maybe an escape?
if ((white_man & FLD20) && (all_black((FLD14|FLD9))) && ( all_empty((FLD13|FLD18)))) return 0;
his_pieces_used = 0;   // calc the minimal amount of steps for having the backing piece in place as well as the attack resp.defence piece
his_min_backing_steps = calc_dist_2( (FLD8), (FLD2|FLD3),  &his_pieces_used, black_man);
his_min_attack_steps = calc_dist_3( (FLD13), (FLD9), (FLD3|FLD4),  &his_pieces_used, black_man);
if ((all_empty_or_white((FLD20|FLD29)))  || (all_occupied((FLD20|FLD29)))) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_5( (FLD30), (FLD34|FLD35), (FLD39|FLD40), (FLD43|FLD44|FLD45), (FLD48|FLD49|FLD50),  &my_pieces_used, white_man) + 1;
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if (empty_or_white & FLD20) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_6( (FLD24), (FLD29|FLD30), (FLD33|FLD34|FLD35), (FLD38|FLD39|FLD40), (FLD42|FLD43|FLD44|FLD45), (FLD47|FLD48|FLD49|FLD50),  &my_pieces_used, white_man);
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if ((all_white((FLD23|FLD28))) && (all_empty(( FLD14|FLD10))))  return -200; // vlucht
if ((occupied & FLD20) && (all_empty(( FLD14|FLD10))))  return -200; // vlucht
if ((all_white((FLD23|FLD28))) && (black_man & FLD10) && (occupied & FLD20) && (empty_field & FLD14)) return 0; //paddestoel
if ((all_white((FLD23|FLD28))) && (black_man & FLD10) && (empty_or_white &FLD20) && (empty_field & FLD14)) return 0; //kan wegruilen
return -700;
}

//calc the minimal amount of steps for having the backing piece in place as well as the attack resp.defence piece
his_pieces_used = 0;
his_min_backing_steps = calc_dist_2( (FLD8), (FLD2|FLD3),  &his_pieces_used, black_man);
his_min_attack_steps = calc_dist_3( (FLD13), (FLD9), (FLD3|FLD4),  &his_pieces_used, black_man);
my_pieces_used = 0;
my_min_backing_steps = calc_dist_4( (FLD35), (FLD40), (FLD44|FLD45), (FLD49|FLD50),  &my_pieces_used, white_man);
my_min_defence_steps = calc_dist_5( (FLD30), (FLD34), (FLD39|FLD40), (FLD43|FLD44|FLD45), (FLD48|FLD49|FLD50),  &my_pieces_used, white_man);

// we have calculated: his_min_backing_steps, his_min_attack_steps, my_min_defence_steps and my_min_backing steps

if ((his_min_backing_steps + his_min_attack_steps) - (my_min_backing_steps + my_min_defence_steps + inc) < -2) {
if ((white_man & FLD20) && (all_black((FLD14|FLD9))) && ( all_empty((FLD13|FLD18)))) return 0;
his_pieces_used = 0;   // calc the minimal amount of steps for having the backing piece in place as well as the attack resp.defence piece
his_min_backing_steps = calc_dist_2( (FLD8), (FLD2|FLD3),  &his_pieces_used, black_man);
his_min_attack_steps = calc_dist_3( (FLD13), (FLD9), (FLD3|FLD4),  &his_pieces_used, black_man);
if ((all_empty_or_white((FLD20|FLD29)))  || (all_occupied((FLD20|FLD29)))) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_5( (FLD30), (FLD34|FLD35), (FLD39|FLD40), (FLD43|FLD44|FLD45), (FLD48|FLD49|FLD50),  &my_pieces_used, white_man) + 1;
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if (empty_or_white & FLD20) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_6( (FLD24), (FLD29|FLD30), (FLD33|FLD34|FLD35), (FLD38|FLD39|FLD40), (FLD42|FLD43|FLD44|FLD45), (FLD47|FLD48|FLD49|FLD50),  &my_pieces_used, white_man);
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if ((all_white((FLD23|FLD28))) && (all_empty(( FLD14|FLD10))))  return -200; // vlucht
if ((occupied & FLD20) && (all_empty(( FLD14|FLD10))))  return -200; // vlucht
if ((all_white((FLD23|FLD28))) && (black_man & FLD10) && (occupied & FLD20) && (empty_field & FLD14)) return 0; //paddestoel
if ((all_white((FLD23|FLD28))) && (black_man & FLD10) && (empty_or_white &FLD20) && (empty_field & FLD14)) return 0; //kan wegruilen
return -700;
}

// and a second attack??

if (number_of_attacks > 1) {
his_min_attack_steps = calc_dist_3( (FLD13), (FLD9), (FLD3|FLD4),  &his_pieces_used, black_man);
my_min_defence_steps = calc_dist_5( (FLD30), (FLD34), (FLD39|FLD40), (FLD43|FLD44|FLD45), (FLD48|FLD49|FLD50),  &my_pieces_used, white_man);
if ((his_min_attack_steps) - (my_min_defence_steps + inc) < -2) { // too late for the second attempt
if ((white_man & FLD20) && (all_black((FLD14|FLD9))) && ( all_empty((FLD13|FLD18)))) return 0;
his_pieces_used = 0;   // calc the minimal amount of steps for having the backing piece in place as well as the attack resp.defence piece
his_min_backing_steps = calc_dist_2( (FLD8), (FLD2|FLD3),  &his_pieces_used, black_man);
his_min_attack_steps = calc_dist_3( (FLD13), (FLD9), (FLD3|FLD4),  &his_pieces_used, black_man);
if ((all_empty_or_white((FLD20|FLD29)))  || (all_occupied((FLD20|FLD29)))) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_5( (FLD30), (FLD34|FLD35), (FLD39|FLD40), (FLD43|FLD44|FLD45), (FLD48|FLD49|FLD50),  &my_pieces_used, white_man) + 1;
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if (empty_or_white & FLD20) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_6( (FLD24), (FLD29|FLD30), (FLD33|FLD34|FLD35), (FLD38|FLD39|FLD40), (FLD42|FLD43|FLD44|FLD45), (FLD47|FLD48|FLD49|FLD50),  &my_pieces_used, white_man);
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if ((all_white((FLD23|FLD28))) && (all_empty(( FLD14|FLD10))))  return -200; // vlucht
if ((occupied & FLD20) && (all_empty(( FLD14|FLD10))))  return -200; // vlucht
if ((all_white((FLD23|FLD28))) && (black_man & FLD10) && (occupied & FLD20) && (empty_field & FLD14)) return 0; //paddestoel
if ((all_white((FLD23|FLD28))) && (black_man & FLD10) && (empty_or_white &FLD20) && (empty_field & FLD14)) return 0; //kan wegruilen
return -400;
}
}
check_afruil:
if (all_black((FLD2|FLD8))) return 0; // voorpost wordt afgeruild
if (all_black((FLD5|FLD10))) return 0; // voorpost wordt afgeruild
// so we have hope to have a nice voorpost
return 180;  // no problems, so a small advantage
}
return 0;
}
extern int voorpost_white_se_22(BitArray, BitArray, BitArray, int);
int voorpost_white_se_22(BitArray white_man, BitArray black_man, BitArray empty_field, int inc) {
BitArray my_pieces_used, his_pieces_used;
int number_of_defences, number_of_attacks = 0;
int his_min_backing_steps, my_min_backing_steps;
int my_min_defence_steps, his_min_attack_steps;
bool backing_defence_piece, backing_attack_piece;

if ((white_man & FLD22) &&  (all_empty_or_black((FLD17|FLD11))) &&  (all_empty_or_white((FLD33|FLD39))) &&  (empty_field & FLD28)) {
//so this is what anyway has to be there to have a voorpost

//if the field sw of b is occupied by me or can be occupied by me, no attack is possible, so the question is, can the piece on sw(b) be exchanged
if ((white_man & FLD21) ||  ((white_man & FLD26) && ((all_empty_or_white(FLD16|FLD27)) ||  (all_occupied(FLD16|FLD27))))) {
if ((all_black((FLD12|FLD8|FLD3)) && (empty_field & FLD17)) &&
    !(((black_man & FLD18) && (empty_field & FLD23)) ||  ((black_man & FLD7) && (empty_field & FLD1))))
number_of_attacks = -1;  // can be exchanged,  start with one attack less
else goto check_afruil; // no attacks possible
}

//same for the defence: if ne(d) is occupied by him, we may have a problem in the defence
if (black_man & FLD29) {
if ((all_white((FLD38|FLD44|FLD47)) && (empty_field & FLD33)) ||
    (all_white((FLD33|FLD42|FLD47)) && (empty_field & FLD38)))
number_of_defences = count(((FLD33|FLD38|FLD42|FLD43|FLD47|FLD48|FLD49) & white_man)) - 1;
else number_of_defences = 0;
} else number_of_defences = count(((FLD33|FLD38|FLD42|FLD43|FLD47|FLD48|FLD49) & white_man));

//check the availability of the backing pieces
backing_defence_piece = bool ((FLD39|FLD43|FLD44|FLD48|FLD49|FLD50) & white_man);
backing_attack_piece = (bool) ((FLD1|FLD2|FLD6|FLD7|FLD11) & black_man);
number_of_attacks += count(((FLD1|FLD2|FLD3|FLD7|FLD8|FLD12|FLD17) & black_man));
if (!((FLD6|FLD11) & black_man)) number_of_attacks -= 1;
if (!((FLD39|FLD44|FLD50) & white_man)) number_of_defences -= 1;

//so now we know whether we have a backing piece and how many pieces for the attack/defence
if (number_of_attacks <= 0) return 180;
if (!(backing_defence_piece) && (backing_attack_piece) && (number_of_attacks > 0)) { // looks bad, but maybe an escape?
if ((white_man & FLD23) && (all_black((FLD18|FLD12))) && ( all_empty((FLD17|FLD21)))) return 0;
his_pieces_used = 0;   // calc the minimal amount of steps for having the backing piece in place as well as the attack resp.defence piece
his_min_backing_steps = calc_dist_3( (FLD11), (FLD6|FLD7), (FLD1|FLD2),  &his_pieces_used, black_man);
his_min_attack_steps = calc_dist_4( (FLD17), (FLD12), (FLD7|FLD8), (FLD1|FLD2|FLD3),  &his_pieces_used, black_man);
if ((all_empty_or_white((FLD23|FLD32)))  || (all_occupied((FLD23|FLD32)))) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_4( (FLD33), (FLD38|FLD39), (FLD42|FLD43|FLD44), (FLD47|FLD48|FLD49|FLD50),  &my_pieces_used, white_man) + 1;
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if (empty_or_white & FLD23) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_5( (FLD28), (FLD32|FLD33), (FLD37|FLD38|FLD39), (FLD41|FLD42|FLD43|FLD44), (FLD46|FLD47|FLD48|FLD49|FLD50),  &my_pieces_used, white_man);
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if ((all_white((FLD27|FLD31))) && (all_empty(( FLD18|FLD13))))  return -200; // vlucht
if ((occupied & FLD23) && (all_empty(( FLD18|FLD13))))  return -200; // vlucht
if ((all_white((FLD27|FLD31))) && (black_man & FLD13) && (occupied & FLD23) && (empty_field & FLD18)) return 0; //paddestoel
if ((all_white((FLD27|FLD31))) && (black_man & FLD13) && (empty_or_white &FLD23) && (empty_field & FLD18)) return 0; //kan wegruilen
 return -700;
}

if (!(backing_attack_piece)) goto check_afruil;
// both backings are available
if (number_of_attacks > number_of_defences) {  // it looks bad, but maybe an escape?
if ((white_man & FLD23) && (all_black((FLD18|FLD12))) && ( all_empty((FLD17|FLD21)))) return 0;
his_pieces_used = 0;   // calc the minimal amount of steps for having the backing piece in place as well as the attack resp.defence piece
his_min_backing_steps = calc_dist_3( (FLD11), (FLD6|FLD7), (FLD1|FLD2),  &his_pieces_used, black_man);
his_min_attack_steps = calc_dist_4( (FLD17), (FLD12), (FLD7|FLD8), (FLD1|FLD2|FLD3),  &his_pieces_used, black_man);
if ((all_empty_or_white((FLD23|FLD32)))  || (all_occupied((FLD23|FLD32)))) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_4( (FLD33), (FLD38|FLD39), (FLD42|FLD43|FLD44), (FLD47|FLD48|FLD49|FLD50),  &my_pieces_used, white_man) + 1;
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if (empty_or_white & FLD23) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_5( (FLD28), (FLD32|FLD33), (FLD37|FLD38|FLD39), (FLD41|FLD42|FLD43|FLD44), (FLD46|FLD47|FLD48|FLD49|FLD50),  &my_pieces_used, white_man);
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if ((all_white((FLD27|FLD31))) && (all_empty(( FLD18|FLD13))))  return -200; // vlucht
if ((occupied & FLD23) && (all_empty(( FLD18|FLD13))))  return -200; // vlucht
if ((all_white((FLD27|FLD31))) && (black_man & FLD13) && (occupied & FLD23) && (empty_field & FLD18)) return 0; //paddestoel
if ((all_white((FLD27|FLD31))) && (black_man & FLD13) && (empty_or_white &FLD23) && (empty_field & FLD18)) return 0; //kan wegruilen
return -700;
}

//calc the minimal amount of steps for having the backing piece in place as well as the attack resp.defence piece
his_pieces_used = 0;
his_min_backing_steps = calc_dist_3( (FLD11), (FLD6|FLD7), (FLD1|FLD2),  &his_pieces_used, black_man);
his_min_attack_steps = calc_dist_4( (FLD17), (FLD12), (FLD7|FLD8), (FLD1|FLD2|FLD3),  &his_pieces_used, black_man);
my_pieces_used = 0;
my_min_backing_steps = calc_dist_3( (FLD39), (FLD43|FLD44), (FLD48|FLD49|FLD50),  &my_pieces_used, white_man);
my_min_defence_steps = calc_dist_4( (FLD33), (FLD38), (FLD42|FLD43), (FLD47|FLD48|FLD49),  &my_pieces_used, white_man);

// we have calculated: his_min_backing_steps, his_min_attack_steps, my_min_defence_steps and my_min_backing steps

if ((his_min_backing_steps + his_min_attack_steps) - (my_min_backing_steps + my_min_defence_steps + inc) < -2) {
if ((white_man & FLD23) && (all_black((FLD18|FLD12))) && ( all_empty((FLD17|FLD21)))) return 0;
his_pieces_used = 0;   // calc the minimal amount of steps for having the backing piece in place as well as the attack resp.defence piece
his_min_backing_steps = calc_dist_3( (FLD11), (FLD6|FLD7), (FLD1|FLD2),  &his_pieces_used, black_man);
his_min_attack_steps = calc_dist_4( (FLD17), (FLD12), (FLD7|FLD8), (FLD1|FLD2|FLD3),  &his_pieces_used, black_man);
if ((all_empty_or_white((FLD23|FLD32)))  || (all_occupied((FLD23|FLD32)))) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_4( (FLD33), (FLD38|FLD39), (FLD42|FLD43|FLD44), (FLD47|FLD48|FLD49|FLD50),  &my_pieces_used, white_man) + 1;
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if (empty_or_white & FLD23) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_5( (FLD28), (FLD32|FLD33), (FLD37|FLD38|FLD39), (FLD41|FLD42|FLD43|FLD44), (FLD46|FLD47|FLD48|FLD49|FLD50),  &my_pieces_used, white_man);
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if ((all_white((FLD27|FLD31))) && (all_empty(( FLD18|FLD13))))  return -200; // vlucht
if ((occupied & FLD23) && (all_empty(( FLD18|FLD13))))  return -200; // vlucht
if ((all_white((FLD27|FLD31))) && (black_man & FLD13) && (occupied & FLD23) && (empty_field & FLD18)) return 0; //paddestoel
if ((all_white((FLD27|FLD31))) && (black_man & FLD13) && (empty_or_white &FLD23) && (empty_field & FLD18)) return 0; //kan wegruilen
return -700;
}

// and a second attack??

if (number_of_attacks > 1) {
his_min_attack_steps = calc_dist_4( (FLD17), (FLD12), (FLD7|FLD8), (FLD1|FLD2|FLD3),  &his_pieces_used, black_man);
my_min_defence_steps = calc_dist_4( (FLD33), (FLD38), (FLD42|FLD43), (FLD47|FLD48|FLD49),  &my_pieces_used, white_man);
if ((his_min_attack_steps) - (my_min_defence_steps + inc) < -2) { // too late for the second attempt
if ((white_man & FLD23) && (all_black((FLD18|FLD12))) && ( all_empty((FLD17|FLD21)))) return 0;
his_pieces_used = 0;   // calc the minimal amount of steps for having the backing piece in place as well as the attack resp.defence piece
his_min_backing_steps = calc_dist_3( (FLD11), (FLD6|FLD7), (FLD1|FLD2),  &his_pieces_used, black_man);
his_min_attack_steps = calc_dist_4( (FLD17), (FLD12), (FLD7|FLD8), (FLD1|FLD2|FLD3),  &his_pieces_used, black_man);
if ((all_empty_or_white((FLD23|FLD32)))  || (all_occupied((FLD23|FLD32)))) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_4( (FLD33), (FLD38|FLD39), (FLD42|FLD43|FLD44), (FLD47|FLD48|FLD49|FLD50),  &my_pieces_used, white_man) + 1;
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if (empty_or_white & FLD23) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_5( (FLD28), (FLD32|FLD33), (FLD37|FLD38|FLD39), (FLD41|FLD42|FLD43|FLD44), (FLD46|FLD47|FLD48|FLD49|FLD50),  &my_pieces_used, white_man);
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if ((all_white((FLD27|FLD31))) && (all_empty(( FLD18|FLD13))))  return -200; // vlucht
if ((occupied & FLD23) && (all_empty(( FLD18|FLD13))))  return -200; // vlucht
if ((all_white((FLD27|FLD31))) && (black_man & FLD13) && (occupied & FLD23) && (empty_field & FLD18)) return 0; //paddestoel
if ((all_white((FLD27|FLD31))) && (black_man & FLD13) && (empty_or_white &FLD23) && (empty_field & FLD18)) return 0; //kan wegruilen
return -400;
}
}
check_afruil:
if (all_black((FLD6|FLD11))) return 0; // voorpost wordt afgeruild
if (all_black((FLD4|FLD9|FLD13))) return 0; // voorpost wordt afgeruild
// so we have hope to have a nice voorpost
return 180;  // no problems, so a small advantage
}
return 0;
}
extern int voorpost_white_se_23(BitArray, BitArray, BitArray, int);
int voorpost_white_se_23(BitArray white_man, BitArray black_man, BitArray empty_field, int inc) {
BitArray my_pieces_used, his_pieces_used;
int number_of_defences, number_of_attacks = 0;
int his_min_backing_steps, my_min_backing_steps;
int my_min_defence_steps, his_min_attack_steps;
bool backing_defence_piece, backing_attack_piece;

if ((white_man & FLD23) &&  (all_empty_or_black((FLD18|FLD12))) &&  (all_empty_or_white((FLD34|FLD40))) &&  (empty_field & FLD29)) {
//so this is what anyway has to be there to have a voorpost

//if the field sw of b is occupied by me or can be occupied by me, no attack is possible, so the question is, can the piece on sw(b) be exchanged
if ((white_man & FLD22) ||  ((white_man & FLD27) && ((all_empty_or_white(FLD17|FLD28)) ||  (all_occupied(FLD17|FLD28))))) {
if ((all_black((FLD13|FLD9|FLD4)) && (empty_field & FLD18)) &&
    !(((black_man & FLD19) && (empty_field & FLD24)) ||  ((black_man & FLD8) && (empty_field & FLD2))))
number_of_attacks = -1;  // can be exchanged,  start with one attack less
else goto check_afruil; // no attacks possible
}

//same for the defence: if ne(d) is occupied by him, we may have a problem in the defence
if (black_man & FLD30) {
if ((all_white((FLD39|FLD45|FLD48)) && (empty_field & FLD34)) ||
    (all_white((FLD34|FLD43|FLD48)) && (empty_field & FLD39)))
number_of_defences = count(((FLD34|FLD39|FLD43|FLD44|FLD48|FLD49|FLD50) & white_man)) - 1;
else number_of_defences = 0;
} else number_of_defences = count(((FLD34|FLD39|FLD43|FLD44|FLD48|FLD49|FLD50) & white_man));

//check the availability of the backing pieces
backing_defence_piece = bool ((FLD40|FLD44|FLD45|FLD49|FLD50) & white_man);
backing_attack_piece = (bool) ((FLD1|FLD2|FLD3|FLD7|FLD8|FLD12) & black_man);
number_of_attacks += count(((FLD2|FLD3|FLD4|FLD8|FLD9|FLD13|FLD18) & black_man));
if (!((FLD1|FLD7|FLD12) & black_man)) number_of_attacks -= 1;
if (!((FLD40|FLD45) & white_man)) number_of_defences -= 1;

//so now we know whether we have a backing piece and how many pieces for the attack/defence
if (number_of_attacks <= 0) return 180;
if (!(backing_defence_piece) && (backing_attack_piece) && (number_of_attacks > 0)) { // looks bad, but maybe an escape?
if ((white_man & FLD24) && (all_black((FLD19|FLD13))) && ( all_empty((FLD18|FLD22)))) return 0;
his_pieces_used = 0;   // calc the minimal amount of steps for having the backing piece in place as well as the attack resp.defence piece
his_min_backing_steps = calc_dist_3( (FLD12), (FLD7|FLD8), (FLD1|FLD2|FLD3),  &his_pieces_used, black_man);
his_min_attack_steps = calc_dist_4( (FLD18), (FLD13), (FLD8|FLD9), (FLD2|FLD3|FLD4),  &his_pieces_used, black_man);
if ((all_empty_or_white((FLD24|FLD33)))  || (all_occupied((FLD24|FLD33)))) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_4( (FLD34), (FLD39|FLD40), (FLD43|FLD44|FLD45), (FLD48|FLD49|FLD50),  &my_pieces_used, white_man) + 1;
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if (empty_or_white & FLD24) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_5( (FLD29), (FLD33|FLD34), (FLD38|FLD39|FLD40), (FLD42|FLD43|FLD44|FLD45), (FLD47|FLD48|FLD49|FLD50),  &my_pieces_used, white_man);
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if ((all_white((FLD28|FLD32))) && (all_empty(( FLD19|FLD14))))  return -200; // vlucht
if ((occupied & FLD24) && (all_empty(( FLD19|FLD14))))  return -200; // vlucht
if ((all_white((FLD28|FLD32))) && (black_man & FLD14) && (occupied & FLD24) && (empty_field & FLD19)) return 0; //paddestoel
if ((all_white((FLD28|FLD32))) && (black_man & FLD14) && (empty_or_white &FLD24) && (empty_field & FLD19)) return 0; //kan wegruilen
 return -700;
}

if (!(backing_attack_piece)) goto check_afruil;
// both backings are available
if (number_of_attacks > number_of_defences) {  // it looks bad, but maybe an escape?
if ((white_man & FLD24) && (all_black((FLD19|FLD13))) && ( all_empty((FLD18|FLD22)))) return 0;
his_pieces_used = 0;   // calc the minimal amount of steps for having the backing piece in place as well as the attack resp.defence piece
his_min_backing_steps = calc_dist_3( (FLD12), (FLD7|FLD8), (FLD1|FLD2|FLD3),  &his_pieces_used, black_man);
his_min_attack_steps = calc_dist_4( (FLD18), (FLD13), (FLD8|FLD9), (FLD2|FLD3|FLD4),  &his_pieces_used, black_man);
if ((all_empty_or_white((FLD24|FLD33)))  || (all_occupied((FLD24|FLD33)))) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_4( (FLD34), (FLD39|FLD40), (FLD43|FLD44|FLD45), (FLD48|FLD49|FLD50),  &my_pieces_used, white_man) + 1;
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if (empty_or_white & FLD24) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_5( (FLD29), (FLD33|FLD34), (FLD38|FLD39|FLD40), (FLD42|FLD43|FLD44|FLD45), (FLD47|FLD48|FLD49|FLD50),  &my_pieces_used, white_man);
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if ((all_white((FLD28|FLD32))) && (all_empty(( FLD19|FLD14))))  return -200; // vlucht
if ((occupied & FLD24) && (all_empty(( FLD19|FLD14))))  return -200; // vlucht
if ((all_white((FLD28|FLD32))) && (black_man & FLD14) && (occupied & FLD24) && (empty_field & FLD19)) return 0; //paddestoel
if ((all_white((FLD28|FLD32))) && (black_man & FLD14) && (empty_or_white &FLD24) && (empty_field & FLD19)) return 0; //kan wegruilen
return -700;
}

//calc the minimal amount of steps for having the backing piece in place as well as the attack resp.defence piece
his_pieces_used = 0;
his_min_backing_steps = calc_dist_3( (FLD12), (FLD7|FLD8), (FLD1|FLD2|FLD3),  &his_pieces_used, black_man);
his_min_attack_steps = calc_dist_4( (FLD18), (FLD13), (FLD8|FLD9), (FLD2|FLD3|FLD4),  &his_pieces_used, black_man);
my_pieces_used = 0;
my_min_backing_steps = calc_dist_3( (FLD40), (FLD44|FLD45), (FLD49|FLD50),  &my_pieces_used, white_man);
my_min_defence_steps = calc_dist_4( (FLD34), (FLD39), (FLD43|FLD44), (FLD48|FLD49|FLD50),  &my_pieces_used, white_man);

// we have calculated: his_min_backing_steps, his_min_attack_steps, my_min_defence_steps and my_min_backing steps

if ((his_min_backing_steps + his_min_attack_steps) - (my_min_backing_steps + my_min_defence_steps + inc) < -2) {
if ((white_man & FLD24) && (all_black((FLD19|FLD13))) && ( all_empty((FLD18|FLD22)))) return 0;
his_pieces_used = 0;   // calc the minimal amount of steps for having the backing piece in place as well as the attack resp.defence piece
his_min_backing_steps = calc_dist_3( (FLD12), (FLD7|FLD8), (FLD1|FLD2|FLD3),  &his_pieces_used, black_man);
his_min_attack_steps = calc_dist_4( (FLD18), (FLD13), (FLD8|FLD9), (FLD2|FLD3|FLD4),  &his_pieces_used, black_man);
if ((all_empty_or_white((FLD24|FLD33)))  || (all_occupied((FLD24|FLD33)))) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_4( (FLD34), (FLD39|FLD40), (FLD43|FLD44|FLD45), (FLD48|FLD49|FLD50),  &my_pieces_used, white_man) + 1;
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if (empty_or_white & FLD24) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_5( (FLD29), (FLD33|FLD34), (FLD38|FLD39|FLD40), (FLD42|FLD43|FLD44|FLD45), (FLD47|FLD48|FLD49|FLD50),  &my_pieces_used, white_man);
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if ((all_white((FLD28|FLD32))) && (all_empty(( FLD19|FLD14))))  return -200; // vlucht
if ((occupied & FLD24) && (all_empty(( FLD19|FLD14))))  return -200; // vlucht
if ((all_white((FLD28|FLD32))) && (black_man & FLD14) && (occupied & FLD24) && (empty_field & FLD19)) return 0; //paddestoel
if ((all_white((FLD28|FLD32))) && (black_man & FLD14) && (empty_or_white &FLD24) && (empty_field & FLD19)) return 0; //kan wegruilen
return -700;
}

// and a second attack??

if (number_of_attacks > 1) {
his_min_attack_steps = calc_dist_4( (FLD18), (FLD13), (FLD8|FLD9), (FLD2|FLD3|FLD4),  &his_pieces_used, black_man);
my_min_defence_steps = calc_dist_4( (FLD34), (FLD39), (FLD43|FLD44), (FLD48|FLD49|FLD50),  &my_pieces_used, white_man);
if ((his_min_attack_steps) - (my_min_defence_steps + inc) < -2) { // too late for the second attempt
if ((white_man & FLD24) && (all_black((FLD19|FLD13))) && ( all_empty((FLD18|FLD22)))) return 0;
his_pieces_used = 0;   // calc the minimal amount of steps for having the backing piece in place as well as the attack resp.defence piece
his_min_backing_steps = calc_dist_3( (FLD12), (FLD7|FLD8), (FLD1|FLD2|FLD3),  &his_pieces_used, black_man);
his_min_attack_steps = calc_dist_4( (FLD18), (FLD13), (FLD8|FLD9), (FLD2|FLD3|FLD4),  &his_pieces_used, black_man);
if ((all_empty_or_white((FLD24|FLD33)))  || (all_occupied((FLD24|FLD33)))) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_4( (FLD34), (FLD39|FLD40), (FLD43|FLD44|FLD45), (FLD48|FLD49|FLD50),  &my_pieces_used, white_man) + 1;
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if (empty_or_white & FLD24) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_5( (FLD29), (FLD33|FLD34), (FLD38|FLD39|FLD40), (FLD42|FLD43|FLD44|FLD45), (FLD47|FLD48|FLD49|FLD50),  &my_pieces_used, white_man);
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if ((all_white((FLD28|FLD32))) && (all_empty(( FLD19|FLD14))))  return -200; // vlucht
if ((occupied & FLD24) && (all_empty(( FLD19|FLD14))))  return -200; // vlucht
if ((all_white((FLD28|FLD32))) && (black_man & FLD14) && (occupied & FLD24) && (empty_field & FLD19)) return 0; //paddestoel
if ((all_white((FLD28|FLD32))) && (black_man & FLD14) && (empty_or_white &FLD24) && (empty_field & FLD19)) return 0; //kan wegruilen
return -400;
}
}
check_afruil:
if (all_black((FLD1|FLD7|FLD12))) return 0; // voorpost wordt afgeruild
if (all_black((FLD5|FLD10|FLD14))) return 0; // voorpost wordt afgeruild
// so we have hope to have a nice voorpost
if (((all_black((FLD24|FLD20|FLD15)) && (all_empty((FLD30|FLD34)))) ||  (all_black((FLD25|FLD20|FLD15)) && (all_empty((FLD24|FLD30|FLD34))))) &&  (white_man & FLD35)) return -700; // een om twee mogelijk
return 180;  // no problems, so a small advantage
}
return 0;
}
extern int voorpost_white_se_24(BitArray, BitArray, BitArray, int);
int voorpost_white_se_24(BitArray white_man, BitArray black_man, BitArray empty_field, int inc) {
BitArray my_pieces_used, his_pieces_used;
int number_of_defences, number_of_attacks = 0;
int his_min_backing_steps, my_min_backing_steps;
int my_min_defence_steps, his_min_attack_steps;
bool backing_defence_piece, backing_attack_piece;

if ((white_man & FLD24) &&  (all_empty_or_black((FLD19|FLD13))) &&  (empty_or_white & FLD35) &&  (empty_field & FLD30)) {
//so this is what anyway has to be there to have a voorpost

//if the field sw of b is occupied by me or can be occupied by me, no attack is possible, so the question is, can the piece on sw(b) be exchanged
if ((white_man & FLD23) ||  ((white_man & FLD28) && ((all_empty_or_white(FLD18|FLD29)) ||  (all_occupied(FLD18|FLD29))))) {
if ((all_black((FLD14|FLD10|FLD5)) && (empty_field & FLD19)) &&
    !(((black_man & FLD20) && (empty_field & FLD25)) ||  ((black_man & FLD9) && (empty_field & FLD3))))
number_of_attacks = -1;  // can be exchanged,  start with one attack less
else goto check_afruil; // no attacks possible
}

number_of_defences = count(((FLD35|FLD40|FLD44|FLD45|FLD49|FLD50) & white_man));

//check the availability of the backing pieces
backing_defence_piece = true; // no backing defence piece (edge)
backing_attack_piece = (bool) ((FLD2|FLD3|FLD4|FLD8|FLD9|FLD13) & black_man);
number_of_attacks += count(((FLD3|FLD4|FLD5|FLD9|FLD10|FLD14|FLD19) & black_man));
if (!((FLD2|FLD8|FLD13) & black_man)) number_of_attacks -= 1;

//so now we know whether we have a backing piece and how many pieces for the attack/defence
if (number_of_attacks <= 0) return 180;
if (!(backing_attack_piece)) goto check_afruil;
// both backings are available
if (number_of_attacks > number_of_defences) {  // it looks bad, but maybe an escape?
if (all_black((FLD15|FLD20|FLD25))) return -700; // vleugelopsluiting
if ((white_man & FLD25) && (all_black((FLD20|FLD14))) && ( all_empty((FLD19|FLD23)))) return 0;
his_pieces_used = 0;   // calc the minimal amount of steps for having the backing piece in place as well as the attack resp.defence piece
his_min_backing_steps = calc_dist_3( (FLD13), (FLD8|FLD9), (FLD2|FLD3|FLD4),  &his_pieces_used, black_man);
his_min_attack_steps = calc_dist_4( (FLD19), (FLD14), (FLD9|FLD10), (FLD3|FLD4|FLD5),  &his_pieces_used, black_man);
if ((all_empty_or_white((FLD25|FLD34)))  || (all_occupied((FLD25|FLD34)))) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_4( (FLD35), (FLD40), (FLD44|FLD45), (FLD49|FLD50),  &my_pieces_used, white_man) + 1;
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if (empty_or_white & FLD25) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_5( (FLD30), (FLD34|FLD35), (FLD39|FLD40), (FLD43|FLD44|FLD45), (FLD48|FLD49|FLD50),  &my_pieces_used, white_man);
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if ((all_white((FLD29|FLD33))) && (all_empty(( FLD20|FLD15))))  return 50; // vlucht naar rand
if ((occupied & FLD25) && (all_empty(( FLD20|FLD15))))  return 50; // vlucht naar rand
if ((all_white((FLD29|FLD33))) && (black_man & FLD15) && (occupied & FLD25) && (empty_field & FLD20)) return 0; //paddestoel
if ((all_white((FLD29|FLD33))) && (black_man & FLD15) && (empty_or_white &FLD25) && (empty_field & FLD20)) return 0; //kan wegruilen
return -700;
}

//calc the minimal amount of steps for having the backing piece in place as well as the attack resp.defence piece
his_pieces_used = 0;
his_min_backing_steps = calc_dist_3( (FLD13), (FLD8|FLD9), (FLD2|FLD3|FLD4),  &his_pieces_used, black_man);
his_min_attack_steps = calc_dist_4( (FLD19), (FLD14), (FLD9|FLD10), (FLD3|FLD4|FLD5),  &his_pieces_used, black_man);
my_pieces_used = 0;
my_min_backing_steps = 0;
my_min_defence_steps = calc_dist_4( (FLD35), (FLD40), (FLD44|FLD45), (FLD49|FLD50),  &my_pieces_used, white_man);

// we have calculated: his_min_backing_steps, his_min_attack_steps, my_min_defence_steps and my_min_backing steps

if ((his_min_backing_steps + his_min_attack_steps) - (my_min_backing_steps + my_min_defence_steps + inc) < -2) {
if (all_black((FLD15|FLD20|FLD25))) return -700; // vleugelopsluiting
if ((white_man & FLD25) && (all_black((FLD20|FLD14))) && ( all_empty((FLD19|FLD23)))) return 0;
his_pieces_used = 0;   // calc the minimal amount of steps for having the backing piece in place as well as the attack resp.defence piece
his_min_backing_steps = calc_dist_3( (FLD13), (FLD8|FLD9), (FLD2|FLD3|FLD4),  &his_pieces_used, black_man);
his_min_attack_steps = calc_dist_4( (FLD19), (FLD14), (FLD9|FLD10), (FLD3|FLD4|FLD5),  &his_pieces_used, black_man);
if ((all_empty_or_white((FLD25|FLD34)))  || (all_occupied((FLD25|FLD34)))) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_4( (FLD35), (FLD40), (FLD44|FLD45), (FLD49|FLD50),  &my_pieces_used, white_man) + 1;
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if (empty_or_white & FLD25) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_5( (FLD30), (FLD34|FLD35), (FLD39|FLD40), (FLD43|FLD44|FLD45), (FLD48|FLD49|FLD50),  &my_pieces_used, white_man);
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if ((all_white((FLD29|FLD33))) && (all_empty(( FLD20|FLD15))))  return 50; // vlucht naar rand
if ((occupied & FLD25) && (all_empty(( FLD20|FLD15))))  return 50; // vlucht naar rand
if ((all_white((FLD29|FLD33))) && (black_man & FLD15) && (occupied & FLD25) && (empty_field & FLD20)) return 0; //paddestoel
if ((all_white((FLD29|FLD33))) && (black_man & FLD15) && (empty_or_white &FLD25) && (empty_field & FLD20)) return 0; //kan wegruilen
return -700;
}

// and a second attack??

if (number_of_attacks > 1) {
his_min_attack_steps = calc_dist_4( (FLD19), (FLD14), (FLD9|FLD10), (FLD3|FLD4|FLD5),  &his_pieces_used, black_man);
my_min_defence_steps = calc_dist_4( (FLD35), (FLD40), (FLD44|FLD45), (FLD49|FLD50),  &my_pieces_used, white_man);
if ((his_min_attack_steps) - (my_min_defence_steps + inc) < -2) { // too late for the second attempt
if (all_black((FLD15|FLD20|FLD25))) return -700; // vleugelopsluiting
if ((white_man & FLD25) && (all_black((FLD20|FLD14))) && ( all_empty((FLD19|FLD23)))) return 0;
his_pieces_used = 0;   // calc the minimal amount of steps for having the backing piece in place as well as the attack resp.defence piece
his_min_backing_steps = calc_dist_3( (FLD13), (FLD8|FLD9), (FLD2|FLD3|FLD4),  &his_pieces_used, black_man);
his_min_attack_steps = calc_dist_4( (FLD19), (FLD14), (FLD9|FLD10), (FLD3|FLD4|FLD5),  &his_pieces_used, black_man);
if ((all_empty_or_white((FLD25|FLD34)))  || (all_occupied((FLD25|FLD34)))) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_4( (FLD35), (FLD40), (FLD44|FLD45), (FLD49|FLD50),  &my_pieces_used, white_man) + 1;
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if (empty_or_white & FLD25) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_5( (FLD30), (FLD34|FLD35), (FLD39|FLD40), (FLD43|FLD44|FLD45), (FLD48|FLD49|FLD50),  &my_pieces_used, white_man);
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if ((all_white((FLD29|FLD33))) && (all_empty(( FLD20|FLD15))))  return 50; // vlucht naar rand
if ((occupied & FLD25) && (all_empty(( FLD20|FLD15))))  return 50; // vlucht naar rand
if ((all_white((FLD29|FLD33))) && (black_man & FLD15) && (occupied & FLD25) && (empty_field & FLD20)) return 0; //paddestoel
if ((all_white((FLD29|FLD33))) && (black_man & FLD15) && (empty_or_white &FLD25) && (empty_field & FLD20)) return 0; //kan wegruilen
return -400;
}
}
check_afruil:
if (all_black((FLD2|FLD8|FLD13))) return 0; // voorpost wordt afgeruild
// so we have hope to have a nice voorpost
return 180;  // no problems, so a small advantage
}
return 0;
}
// witte voorposten sw
extern int voorpost_white_sw_17(BitArray, BitArray, BitArray, int);
int voorpost_white_sw_17(BitArray white_man, BitArray black_man, BitArray empty_field, int inc) {
BitArray my_pieces_used, his_pieces_used;
int number_of_defences, number_of_attacks = 0;
int his_min_backing_steps, my_min_backing_steps;
int my_min_defence_steps, his_min_attack_steps;
bool backing_defence_piece, backing_attack_piece;

if ((white_man & FLD17) &&  (all_empty_or_black((FLD12|FLD8))) &&  (empty_or_white & FLD26) &&  (empty_field & FLD21)) {
//so this is what anyway has to be there to have a voorpost

//if the field sw of b is occupied by me or can be occupied by me, no attack is possible, so the question is, can the piece on sw(b) be exchanged
if ((white_man & FLD18) ||  ((white_man & FLD23) && ((all_empty_or_white(FLD13|FLD22)) ||  (all_occupied(FLD13|FLD22))))) {
if ((all_black((FLD7|FLD1|FLD0)) && (empty_field & FLD12)) &&
    !(((black_man & FLD11) && (empty_field & FLD16)) ||  ((black_man & FLD2) && (empty_field & FLD0))))
number_of_attacks = -1;  // can be exchanged,  start with one attack less
else goto check_afruil; // no attacks possible
}

number_of_defences = count(((FLD26|FLD31|FLD36|FLD37|FLD41|FLD42|FLD46|FLD47|FLD48) & white_man));

//check the availability of the backing pieces
backing_defence_piece = true; // no backing defence piece (edge)
backing_attack_piece = (bool) ((FLD2|FLD3|FLD8) & black_man);
number_of_attacks += count(((FLD1|FLD2|FLD7|FLD12) & black_man));
if (!((FLD3|FLD8) & black_man)) number_of_attacks -= 1;

//so now we know whether we have a backing piece and how many pieces for the attack/defence
if (number_of_attacks <= 0) return 180;
if (!(backing_attack_piece)) goto check_afruil;
// both backings are available
if (number_of_attacks > number_of_defences) {  // it looks bad, but maybe an escape?
if (all_black((FLD6|FLD11|FLD16))) return -700; // vleugelopsluiting
if ((white_man & FLD16) && (all_black((FLD11|FLD7))) && ( all_empty((FLD12|FLD18)))) return 0;
his_pieces_used = 0;   // calc the minimal amount of steps for having the backing piece in place as well as the attack resp.defence piece
his_min_backing_steps = calc_dist_2( (FLD8), (FLD2|FLD3),  &his_pieces_used, black_man);
his_min_attack_steps = calc_dist_3( (FLD12), (FLD7), (FLD1|FLD2),  &his_pieces_used, black_man);
if ((all_empty_or_white((FLD16|FLD27)))  || (all_occupied((FLD16|FLD27)))) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_5( (FLD26), (FLD31), (FLD36|FLD37), (FLD41|FLD42), (FLD46|FLD47|FLD48),  &my_pieces_used, white_man) + 1;
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if (empty_or_white & FLD16) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_6( (FLD21), (FLD26|FLD27), (FLD31|FLD32), (FLD36|FLD37|FLD38), (FLD41|FLD42|FLD43), (FLD46|FLD47|FLD48|FLD49),  &my_pieces_used, white_man);
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if ((all_white((FLD22|FLD28))) && (all_empty(( FLD11|FLD6))))  return 50; // vlucht naar rand
if ((occupied & FLD16) && (all_empty(( FLD11|FLD6))))  return 50; // vlucht naar rand
if ((all_white((FLD22|FLD28))) && (black_man & FLD6) && (occupied & FLD16) && (empty_field & FLD11)) return 0; //paddestoel
if ((all_white((FLD22|FLD28))) && (black_man & FLD6) && (empty_or_white &FLD16) && (empty_field & FLD11)) return 0; //kan wegruilen
return -700;
}

//calc the minimal amount of steps for having the backing piece in place as well as the attack resp.defence piece
his_pieces_used = 0;
his_min_backing_steps = calc_dist_2( (FLD8), (FLD2|FLD3),  &his_pieces_used, black_man);
his_min_attack_steps = calc_dist_3( (FLD12), (FLD7), (FLD1|FLD2),  &his_pieces_used, black_man);
my_pieces_used = 0;
my_min_backing_steps = 0;
my_min_defence_steps = calc_dist_5( (FLD26), (FLD31), (FLD36|FLD37), (FLD41|FLD42), (FLD46|FLD47|FLD48),  &my_pieces_used, white_man);

// we have calculated: his_min_backing_steps, his_min_attack_steps, my_min_defence_steps and my_min_backing steps

if ((his_min_backing_steps + his_min_attack_steps) - (my_min_backing_steps + my_min_defence_steps + inc) < -2) {
if (all_black((FLD6|FLD11|FLD16))) return -700; // vleugelopsluiting
if ((white_man & FLD16) && (all_black((FLD11|FLD7))) && ( all_empty((FLD12|FLD18)))) return 0;
his_pieces_used = 0;   // calc the minimal amount of steps for having the backing piece in place as well as the attack resp.defence piece
his_min_backing_steps = calc_dist_2( (FLD8), (FLD2|FLD3),  &his_pieces_used, black_man);
his_min_attack_steps = calc_dist_3( (FLD12), (FLD7), (FLD1|FLD2),  &his_pieces_used, black_man);
if ((all_empty_or_white((FLD16|FLD27)))  || (all_occupied((FLD16|FLD27)))) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_5( (FLD26), (FLD31), (FLD36|FLD37), (FLD41|FLD42), (FLD46|FLD47|FLD48),  &my_pieces_used, white_man) + 1;
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if (empty_or_white & FLD16) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_6( (FLD21), (FLD26|FLD27), (FLD31|FLD32), (FLD36|FLD37|FLD38), (FLD41|FLD42|FLD43), (FLD46|FLD47|FLD48|FLD49),  &my_pieces_used, white_man);
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if ((all_white((FLD22|FLD28))) && (all_empty(( FLD11|FLD6))))  return 50; // vlucht naar rand
if ((occupied & FLD16) && (all_empty(( FLD11|FLD6))))  return 50; // vlucht naar rand
if ((all_white((FLD22|FLD28))) && (black_man & FLD6) && (occupied & FLD16) && (empty_field & FLD11)) return 0; //paddestoel
if ((all_white((FLD22|FLD28))) && (black_man & FLD6) && (empty_or_white &FLD16) && (empty_field & FLD11)) return 0; //kan wegruilen
return -700;
}

// and a second attack??

if (number_of_attacks > 1) {
his_min_attack_steps = calc_dist_3( (FLD12), (FLD7), (FLD1|FLD2),  &his_pieces_used, black_man);
my_min_defence_steps = calc_dist_5( (FLD26), (FLD31), (FLD36|FLD37), (FLD41|FLD42), (FLD46|FLD47|FLD48),  &my_pieces_used, white_man);
if ((his_min_attack_steps) - (my_min_defence_steps + inc) < -2) { // too late for the second attempt
if (all_black((FLD6|FLD11|FLD16))) return -700; // vleugelopsluiting
if ((white_man & FLD16) && (all_black((FLD11|FLD7))) && ( all_empty((FLD12|FLD18)))) return 0;
his_pieces_used = 0;   // calc the minimal amount of steps for having the backing piece in place as well as the attack resp.defence piece
his_min_backing_steps = calc_dist_2( (FLD8), (FLD2|FLD3),  &his_pieces_used, black_man);
his_min_attack_steps = calc_dist_3( (FLD12), (FLD7), (FLD1|FLD2),  &his_pieces_used, black_man);
if ((all_empty_or_white((FLD16|FLD27)))  || (all_occupied((FLD16|FLD27)))) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_5( (FLD26), (FLD31), (FLD36|FLD37), (FLD41|FLD42), (FLD46|FLD47|FLD48),  &my_pieces_used, white_man) + 1;
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if (empty_or_white & FLD16) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_6( (FLD21), (FLD26|FLD27), (FLD31|FLD32), (FLD36|FLD37|FLD38), (FLD41|FLD42|FLD43), (FLD46|FLD47|FLD48|FLD49),  &my_pieces_used, white_man);
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if ((all_white((FLD22|FLD28))) && (all_empty(( FLD11|FLD6))))  return 50; // vlucht naar rand
if ((occupied & FLD16) && (all_empty(( FLD11|FLD6))))  return 50; // vlucht naar rand
if ((all_white((FLD22|FLD28))) && (black_man & FLD6) && (occupied & FLD16) && (empty_field & FLD11)) return 0; //paddestoel
if ((all_white((FLD22|FLD28))) && (black_man & FLD6) && (empty_or_white &FLD16) && (empty_field & FLD11)) return 0; //kan wegruilen
return -400;
}
}
check_afruil:
if (all_black((FLD3|FLD8))) return 0; // voorpost wordt afgeruild
// so we have hope to have a nice voorpost
return 180;  // no problems, so a small advantage
}
return 0;
}
extern int voorpost_white_sw_18(BitArray, BitArray, BitArray, int);
int voorpost_white_sw_18(BitArray white_man, BitArray black_man, BitArray empty_field, int inc) {
BitArray my_pieces_used, his_pieces_used;
int number_of_defences, number_of_attacks = 0;
int his_min_backing_steps, my_min_backing_steps;
int my_min_defence_steps, his_min_attack_steps;
bool backing_defence_piece, backing_attack_piece;

if ((white_man & FLD18) &&  (all_empty_or_black((FLD13|FLD9))) &&  (all_empty_or_white((FLD27|FLD31))) &&  (empty_field & FLD22)) {
//so this is what anyway has to be there to have a voorpost

//if the field sw of b is occupied by me or can be occupied by me, no attack is possible, so the question is, can the piece on sw(b) be exchanged
if ((white_man & FLD19) ||  ((white_man & FLD24) && ((all_empty_or_white(FLD14|FLD23)) ||  (all_occupied(FLD14|FLD23))))) {
if ((all_black((FLD8|FLD2|FLD0)) && (empty_field & FLD13)) &&
    !(((black_man & FLD12) && (empty_field & FLD17)) ||  ((black_man & FLD3) && (empty_field & FLD0))))
number_of_attacks = -1;  // can be exchanged,  start with one attack less
else goto check_afruil; // no attacks possible
}

//same for the defence: if ne(d) is occupied by him, we may have a problem in the defence
if (black_man & FLD21) {
if ((all_white((FLD32|FLD36|FLD43)) && (empty_field & FLD27)) ||
    (all_white((FLD27|FLD38|FLD43)) && (empty_field & FLD32)))
number_of_defences = count(((FLD27|FLD32|FLD37|FLD38|FLD41|FLD42|FLD43|FLD46|FLD47|FLD48|FLD49) & white_man)) - 1;
else number_of_defences = 0;
} else number_of_defences = count(((FLD27|FLD32|FLD37|FLD38|FLD41|FLD42|FLD43|FLD46|FLD47|FLD48|FLD49) & white_man));

//check the availability of the backing pieces
backing_defence_piece = bool ((FLD31|FLD36|FLD37|FLD41|FLD42|FLD46|FLD47|FLD48) & white_man);
backing_attack_piece = (bool) ((FLD3|FLD4|FLD9) & black_man);
number_of_attacks += count(((FLD2|FLD3|FLD8|FLD13) & black_man));
if (!((FLD4|FLD9) & black_man)) number_of_attacks -= 1;
if (!((FLD31|FLD36) & white_man)) number_of_defences -= 1;

//so now we know whether we have a backing piece and how many pieces for the attack/defence
if (number_of_attacks <= 0) return 180;
if (!(backing_defence_piece) && (backing_attack_piece) && (number_of_attacks > 0)) { // looks bad, but maybe an escape?
if ((white_man & FLD17) && (all_black((FLD12|FLD8))) && ( all_empty((FLD13|FLD19)))) return 0;
his_pieces_used = 0;   // calc the minimal amount of steps for having the backing piece in place as well as the attack resp.defence piece
his_min_backing_steps = calc_dist_2( (FLD9), (FLD3|FLD4),  &his_pieces_used, black_man);
his_min_attack_steps = calc_dist_3( (FLD13), (FLD8), (FLD2|FLD3),  &his_pieces_used, black_man);
if ((all_empty_or_white((FLD17|FLD28)))  || (all_occupied((FLD17|FLD28)))) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_5( (FLD27), (FLD31|FLD32), (FLD36|FLD37|FLD38), (FLD41|FLD42|FLD43), (FLD46|FLD47|FLD48|FLD49),  &my_pieces_used, white_man) + 1;
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if (empty_or_white & FLD17) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_6( (FLD22), (FLD27|FLD28), (FLD31|FLD32|FLD33), (FLD36|FLD37|FLD38|FLD39), (FLD41|FLD42|FLD43|FLD44), (FLD46|FLD47|FLD48|FLD49|FLD50),  &my_pieces_used, white_man);
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if ((all_white((FLD23|FLD29))) && (all_empty(( FLD12|FLD7))))  return -200; // vlucht
if ((occupied & FLD17) && (all_empty(( FLD12|FLD7))))  return -200; // vlucht
if ((all_white((FLD23|FLD29))) && (black_man & FLD7) && (occupied & FLD17) && (empty_field & FLD12)) return 0; //paddestoel
if ((all_white((FLD23|FLD29))) && (black_man & FLD7) && (empty_or_white &FLD17) && (empty_field & FLD12)) return 0; //kan wegruilen
 return -700;
}

if (!(backing_attack_piece)) goto check_afruil;
// both backings are available
if (number_of_attacks > number_of_defences) {  // it looks bad, but maybe an escape?
if ((white_man & FLD17) && (all_black((FLD12|FLD8))) && ( all_empty((FLD13|FLD19)))) return 0;
his_pieces_used = 0;   // calc the minimal amount of steps for having the backing piece in place as well as the attack resp.defence piece
his_min_backing_steps = calc_dist_2( (FLD9), (FLD3|FLD4),  &his_pieces_used, black_man);
his_min_attack_steps = calc_dist_3( (FLD13), (FLD8), (FLD2|FLD3),  &his_pieces_used, black_man);
if ((all_empty_or_white((FLD17|FLD28)))  || (all_occupied((FLD17|FLD28)))) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_5( (FLD27), (FLD31|FLD32), (FLD36|FLD37|FLD38), (FLD41|FLD42|FLD43), (FLD46|FLD47|FLD48|FLD49),  &my_pieces_used, white_man) + 1;
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if (empty_or_white & FLD17) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_6( (FLD22), (FLD27|FLD28), (FLD31|FLD32|FLD33), (FLD36|FLD37|FLD38|FLD39), (FLD41|FLD42|FLD43|FLD44), (FLD46|FLD47|FLD48|FLD49|FLD50),  &my_pieces_used, white_man);
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if ((all_white((FLD23|FLD29))) && (all_empty(( FLD12|FLD7))))  return -200; // vlucht
if ((occupied & FLD17) && (all_empty(( FLD12|FLD7))))  return -200; // vlucht
if ((all_white((FLD23|FLD29))) && (black_man & FLD7) && (occupied & FLD17) && (empty_field & FLD12)) return 0; //paddestoel
if ((all_white((FLD23|FLD29))) && (black_man & FLD7) && (empty_or_white &FLD17) && (empty_field & FLD12)) return 0; //kan wegruilen
return -700;
}

//calc the minimal amount of steps for having the backing piece in place as well as the attack resp.defence piece
his_pieces_used = 0;
his_min_backing_steps = calc_dist_2( (FLD9), (FLD3|FLD4),  &his_pieces_used, black_man);
his_min_attack_steps = calc_dist_3( (FLD13), (FLD8), (FLD2|FLD3),  &his_pieces_used, black_man);
my_pieces_used = 0;
my_min_backing_steps = calc_dist_4( (FLD31), (FLD36|FLD37), (FLD41|FLD42), (FLD46|FLD47|FLD48),  &my_pieces_used, white_man);
my_min_defence_steps = calc_dist_5( (FLD27), (FLD32), (FLD37|FLD38), (FLD41|FLD42|FLD43), (FLD46|FLD47|FLD48|FLD49),  &my_pieces_used, white_man);

// we have calculated: his_min_backing_steps, his_min_attack_steps, my_min_defence_steps and my_min_backing steps

if ((his_min_backing_steps + his_min_attack_steps) - (my_min_backing_steps + my_min_defence_steps + inc) < -2) {
if ((white_man & FLD17) && (all_black((FLD12|FLD8))) && ( all_empty((FLD13|FLD19)))) return 0;
his_pieces_used = 0;   // calc the minimal amount of steps for having the backing piece in place as well as the attack resp.defence piece
his_min_backing_steps = calc_dist_2( (FLD9), (FLD3|FLD4),  &his_pieces_used, black_man);
his_min_attack_steps = calc_dist_3( (FLD13), (FLD8), (FLD2|FLD3),  &his_pieces_used, black_man);
if ((all_empty_or_white((FLD17|FLD28)))  || (all_occupied((FLD17|FLD28)))) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_5( (FLD27), (FLD31|FLD32), (FLD36|FLD37|FLD38), (FLD41|FLD42|FLD43), (FLD46|FLD47|FLD48|FLD49),  &my_pieces_used, white_man) + 1;
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if (empty_or_white & FLD17) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_6( (FLD22), (FLD27|FLD28), (FLD31|FLD32|FLD33), (FLD36|FLD37|FLD38|FLD39), (FLD41|FLD42|FLD43|FLD44), (FLD46|FLD47|FLD48|FLD49|FLD50),  &my_pieces_used, white_man);
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if ((all_white((FLD23|FLD29))) && (all_empty(( FLD12|FLD7))))  return -200; // vlucht
if ((occupied & FLD17) && (all_empty(( FLD12|FLD7))))  return -200; // vlucht
if ((all_white((FLD23|FLD29))) && (black_man & FLD7) && (occupied & FLD17) && (empty_field & FLD12)) return 0; //paddestoel
if ((all_white((FLD23|FLD29))) && (black_man & FLD7) && (empty_or_white &FLD17) && (empty_field & FLD12)) return 0; //kan wegruilen
return -700;
}

// and a second attack??

if (number_of_attacks > 1) {
his_min_attack_steps = calc_dist_3( (FLD13), (FLD8), (FLD2|FLD3),  &his_pieces_used, black_man);
my_min_defence_steps = calc_dist_5( (FLD27), (FLD32), (FLD37|FLD38), (FLD41|FLD42|FLD43), (FLD46|FLD47|FLD48|FLD49),  &my_pieces_used, white_man);
if ((his_min_attack_steps) - (my_min_defence_steps + inc) < -2) { // too late for the second attempt
if ((white_man & FLD17) && (all_black((FLD12|FLD8))) && ( all_empty((FLD13|FLD19)))) return 0;
his_pieces_used = 0;   // calc the minimal amount of steps for having the backing piece in place as well as the attack resp.defence piece
his_min_backing_steps = calc_dist_2( (FLD9), (FLD3|FLD4),  &his_pieces_used, black_man);
his_min_attack_steps = calc_dist_3( (FLD13), (FLD8), (FLD2|FLD3),  &his_pieces_used, black_man);
if ((all_empty_or_white((FLD17|FLD28)))  || (all_occupied((FLD17|FLD28)))) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_5( (FLD27), (FLD31|FLD32), (FLD36|FLD37|FLD38), (FLD41|FLD42|FLD43), (FLD46|FLD47|FLD48|FLD49),  &my_pieces_used, white_man) + 1;
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if (empty_or_white & FLD17) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_6( (FLD22), (FLD27|FLD28), (FLD31|FLD32|FLD33), (FLD36|FLD37|FLD38|FLD39), (FLD41|FLD42|FLD43|FLD44), (FLD46|FLD47|FLD48|FLD49|FLD50),  &my_pieces_used, white_man);
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if ((all_white((FLD23|FLD29))) && (all_empty(( FLD12|FLD7))))  return -200; // vlucht
if ((occupied & FLD17) && (all_empty(( FLD12|FLD7))))  return -200; // vlucht
if ((all_white((FLD23|FLD29))) && (black_man & FLD7) && (occupied & FLD17) && (empty_field & FLD12)) return 0; //paddestoel
if ((all_white((FLD23|FLD29))) && (black_man & FLD7) && (empty_or_white &FLD17) && (empty_field & FLD12)) return 0; //kan wegruilen
return -400;
}
}
check_afruil:
if (all_black((FLD4|FLD9))) return 0; // voorpost wordt afgeruild
if (all_black((FLD1|FLD7))) return 0; // voorpost wordt afgeruild
// so we have hope to have a nice voorpost
if (((all_black((FLD17|FLD11|FLD6)) && (all_empty((FLD21|FLD27)))) ||  (all_black((FLD16|FLD11|FLD6)) && (all_empty((FLD17|FLD21|FLD27))))) &&  (white_man & FLD26)) return -700; // een om twee mogelijk
return 180;  // no problems, so a small advantage
}
return 0;
}
extern int voorpost_white_sw_19(BitArray, BitArray, BitArray, int);
int voorpost_white_sw_19(BitArray white_man, BitArray black_man, BitArray empty_field, int inc) {
BitArray my_pieces_used, his_pieces_used;
int number_of_defences, number_of_attacks = 0;
int his_min_backing_steps, my_min_backing_steps;
int my_min_defence_steps, his_min_attack_steps;
bool backing_defence_piece, backing_attack_piece;

if ((white_man & FLD19) &&  (all_empty_or_black((FLD14|FLD10))) &&  (all_empty_or_white((FLD28|FLD32))) &&  (empty_field & FLD23)) {
//so this is what anyway has to be there to have a voorpost

//if the field sw of b is occupied by me or can be occupied by me, no attack is possible, so the question is, can the piece on sw(b) be exchanged
if ((white_man & FLD20) ||  ((white_man & FLD25) && ((all_empty_or_white(FLD15|FLD24)) ||  (all_occupied(FLD15|FLD24))))) {
if ((all_black((FLD9|FLD3|FLD0)) && (empty_field & FLD14)) &&
    !(((black_man & FLD13) && (empty_field & FLD18)) ||  ((black_man & FLD4) && (empty_field & FLD0))))
number_of_attacks = -1;  // can be exchanged,  start with one attack less
else goto check_afruil; // no attacks possible
}

//same for the defence: if ne(d) is occupied by him, we may have a problem in the defence
if (black_man & FLD22) {
if ((all_white((FLD33|FLD37|FLD44)) && (empty_field & FLD28)) ||
    (all_white((FLD28|FLD39|FLD44)) && (empty_field & FLD33)))
number_of_defences = count(((FLD28|FLD33|FLD38|FLD39|FLD42|FLD43|FLD44|FLD47|FLD48|FLD49|FLD50) & white_man)) - 1;
else number_of_defences = 0;
} else number_of_defences = count(((FLD28|FLD33|FLD38|FLD39|FLD42|FLD43|FLD44|FLD47|FLD48|FLD49|FLD50) & white_man));

//check the availability of the backing pieces
backing_defence_piece = bool ((FLD32|FLD37|FLD38|FLD41|FLD42|FLD43|FLD46|FLD47|FLD48|FLD49) & white_man);
backing_attack_piece = (bool) ((FLD4|FLD5|FLD10) & black_man);
number_of_attacks += count(((FLD3|FLD4|FLD9|FLD14) & black_man));
if (!((FLD5|FLD10) & black_man)) number_of_attacks -= 1;
if (!((FLD32|FLD37|FLD41|FLD46) & white_man)) number_of_defences -= 1;

//so now we know whether we have a backing piece and how many pieces for the attack/defence
if (number_of_attacks <= 0) return 180;
if (!(backing_defence_piece) && (backing_attack_piece) && (number_of_attacks > 0)) { // looks bad, but maybe an escape?
if ((white_man & FLD18) && (all_black((FLD13|FLD9))) && ( all_empty((FLD14|FLD20)))) return 0;
his_pieces_used = 0;   // calc the minimal amount of steps for having the backing piece in place as well as the attack resp.defence piece
his_min_backing_steps = calc_dist_2( (FLD10), (FLD4|FLD5),  &his_pieces_used, black_man);
his_min_attack_steps = calc_dist_3( (FLD14), (FLD9), (FLD3|FLD4),  &his_pieces_used, black_man);
if ((all_empty_or_white((FLD18|FLD29)))  || (all_occupied((FLD18|FLD29)))) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_5( (FLD28), (FLD32|FLD33), (FLD37|FLD38|FLD39), (FLD41|FLD42|FLD43|FLD44), (FLD46|FLD47|FLD48|FLD49|FLD50),  &my_pieces_used, white_man) + 1;
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if (empty_or_white & FLD18) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_6( (FLD23), (FLD28|FLD29), (FLD32|FLD33|FLD34), (FLD37|FLD38|FLD39|FLD40), (FLD41|FLD42|FLD43|FLD44|FLD45), (FLD46|FLD47|FLD48|FLD49|FLD50),  &my_pieces_used, white_man);
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if ((all_white((FLD24|FLD30))) && (all_empty(( FLD13|FLD8))))  return -200; // vlucht
if ((occupied & FLD18) && (all_empty(( FLD13|FLD8))))  return -200; // vlucht
if ((all_white((FLD24|FLD30))) && (black_man & FLD8) && (occupied & FLD18) && (empty_field & FLD13)) return 0; //paddestoel
if ((all_white((FLD24|FLD30))) && (black_man & FLD8) && (empty_or_white &FLD18) && (empty_field & FLD13)) return 0; //kan wegruilen
 return -700;
}

if (!(backing_attack_piece)) goto check_afruil;
// both backings are available
if (number_of_attacks > number_of_defences) {  // it looks bad, but maybe an escape?
if ((white_man & FLD18) && (all_black((FLD13|FLD9))) && ( all_empty((FLD14|FLD20)))) return 0;
his_pieces_used = 0;   // calc the minimal amount of steps for having the backing piece in place as well as the attack resp.defence piece
his_min_backing_steps = calc_dist_2( (FLD10), (FLD4|FLD5),  &his_pieces_used, black_man);
his_min_attack_steps = calc_dist_3( (FLD14), (FLD9), (FLD3|FLD4),  &his_pieces_used, black_man);
if ((all_empty_or_white((FLD18|FLD29)))  || (all_occupied((FLD18|FLD29)))) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_5( (FLD28), (FLD32|FLD33), (FLD37|FLD38|FLD39), (FLD41|FLD42|FLD43|FLD44), (FLD46|FLD47|FLD48|FLD49|FLD50),  &my_pieces_used, white_man) + 1;
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if (empty_or_white & FLD18) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_6( (FLD23), (FLD28|FLD29), (FLD32|FLD33|FLD34), (FLD37|FLD38|FLD39|FLD40), (FLD41|FLD42|FLD43|FLD44|FLD45), (FLD46|FLD47|FLD48|FLD49|FLD50),  &my_pieces_used, white_man);
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if ((all_white((FLD24|FLD30))) && (all_empty(( FLD13|FLD8))))  return -200; // vlucht
if ((occupied & FLD18) && (all_empty(( FLD13|FLD8))))  return -200; // vlucht
if ((all_white((FLD24|FLD30))) && (black_man & FLD8) && (occupied & FLD18) && (empty_field & FLD13)) return 0; //paddestoel
if ((all_white((FLD24|FLD30))) && (black_man & FLD8) && (empty_or_white &FLD18) && (empty_field & FLD13)) return 0; //kan wegruilen
return -700;
}

//calc the minimal amount of steps for having the backing piece in place as well as the attack resp.defence piece
his_pieces_used = 0;
his_min_backing_steps = calc_dist_2( (FLD10), (FLD4|FLD5),  &his_pieces_used, black_man);
his_min_attack_steps = calc_dist_3( (FLD14), (FLD9), (FLD3|FLD4),  &his_pieces_used, black_man);
my_pieces_used = 0;
my_min_backing_steps = calc_dist_4( (FLD32), (FLD37|FLD38), (FLD41|FLD42|FLD43), (FLD46|FLD47|FLD48|FLD49),  &my_pieces_used, white_man);
my_min_defence_steps = calc_dist_5( (FLD28), (FLD33), (FLD38|FLD39), (FLD42|FLD43|FLD44), (FLD47|FLD48|FLD49|FLD50),  &my_pieces_used, white_man);

// we have calculated: his_min_backing_steps, his_min_attack_steps, my_min_defence_steps and my_min_backing steps

if ((his_min_backing_steps + his_min_attack_steps) - (my_min_backing_steps + my_min_defence_steps + inc) < -2) {
if ((white_man & FLD18) && (all_black((FLD13|FLD9))) && ( all_empty((FLD14|FLD20)))) return 0;
his_pieces_used = 0;   // calc the minimal amount of steps for having the backing piece in place as well as the attack resp.defence piece
his_min_backing_steps = calc_dist_2( (FLD10), (FLD4|FLD5),  &his_pieces_used, black_man);
his_min_attack_steps = calc_dist_3( (FLD14), (FLD9), (FLD3|FLD4),  &his_pieces_used, black_man);
if ((all_empty_or_white((FLD18|FLD29)))  || (all_occupied((FLD18|FLD29)))) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_5( (FLD28), (FLD32|FLD33), (FLD37|FLD38|FLD39), (FLD41|FLD42|FLD43|FLD44), (FLD46|FLD47|FLD48|FLD49|FLD50),  &my_pieces_used, white_man) + 1;
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if (empty_or_white & FLD18) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_6( (FLD23), (FLD28|FLD29), (FLD32|FLD33|FLD34), (FLD37|FLD38|FLD39|FLD40), (FLD41|FLD42|FLD43|FLD44|FLD45), (FLD46|FLD47|FLD48|FLD49|FLD50),  &my_pieces_used, white_man);
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if ((all_white((FLD24|FLD30))) && (all_empty(( FLD13|FLD8))))  return -200; // vlucht
if ((occupied & FLD18) && (all_empty(( FLD13|FLD8))))  return -200; // vlucht
if ((all_white((FLD24|FLD30))) && (black_man & FLD8) && (occupied & FLD18) && (empty_field & FLD13)) return 0; //paddestoel
if ((all_white((FLD24|FLD30))) && (black_man & FLD8) && (empty_or_white &FLD18) && (empty_field & FLD13)) return 0; //kan wegruilen
return -700;
}

// and a second attack??

if (number_of_attacks > 1) {
his_min_attack_steps = calc_dist_3( (FLD14), (FLD9), (FLD3|FLD4),  &his_pieces_used, black_man);
my_min_defence_steps = calc_dist_5( (FLD28), (FLD33), (FLD38|FLD39), (FLD42|FLD43|FLD44), (FLD47|FLD48|FLD49|FLD50),  &my_pieces_used, white_man);
if ((his_min_attack_steps) - (my_min_defence_steps + inc) < -2) { // too late for the second attempt
if ((white_man & FLD18) && (all_black((FLD13|FLD9))) && ( all_empty((FLD14|FLD20)))) return 0;
his_pieces_used = 0;   // calc the minimal amount of steps for having the backing piece in place as well as the attack resp.defence piece
his_min_backing_steps = calc_dist_2( (FLD10), (FLD4|FLD5),  &his_pieces_used, black_man);
his_min_attack_steps = calc_dist_3( (FLD14), (FLD9), (FLD3|FLD4),  &his_pieces_used, black_man);
if ((all_empty_or_white((FLD18|FLD29)))  || (all_occupied((FLD18|FLD29)))) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_5( (FLD28), (FLD32|FLD33), (FLD37|FLD38|FLD39), (FLD41|FLD42|FLD43|FLD44), (FLD46|FLD47|FLD48|FLD49|FLD50),  &my_pieces_used, white_man) + 1;
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if (empty_or_white & FLD18) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_6( (FLD23), (FLD28|FLD29), (FLD32|FLD33|FLD34), (FLD37|FLD38|FLD39|FLD40), (FLD41|FLD42|FLD43|FLD44|FLD45), (FLD46|FLD47|FLD48|FLD49|FLD50),  &my_pieces_used, white_man);
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if ((all_white((FLD24|FLD30))) && (all_empty(( FLD13|FLD8))))  return -200; // vlucht
if ((occupied & FLD18) && (all_empty(( FLD13|FLD8))))  return -200; // vlucht
if ((all_white((FLD24|FLD30))) && (black_man & FLD8) && (occupied & FLD18) && (empty_field & FLD13)) return 0; //paddestoel
if ((all_white((FLD24|FLD30))) && (black_man & FLD8) && (empty_or_white &FLD18) && (empty_field & FLD13)) return 0; //kan wegruilen
return -400;
}
}
check_afruil:
if (all_black((FLD5|FLD10))) return 0; // voorpost wordt afgeruild
if (all_black((FLD2|FLD8))) return 0; // voorpost wordt afgeruild
// so we have hope to have a nice voorpost
return 180;  // no problems, so a small advantage
}
return 0;
}
extern int voorpost_white_sw_22(BitArray, BitArray, BitArray, int);
int voorpost_white_sw_22(BitArray white_man, BitArray black_man, BitArray empty_field, int inc) {
BitArray my_pieces_used, his_pieces_used;
int number_of_defences, number_of_attacks = 0;
int his_min_backing_steps, my_min_backing_steps;
int my_min_defence_steps, his_min_attack_steps;
bool backing_defence_piece, backing_attack_piece;

if ((white_man & FLD22) &&  (all_empty_or_black((FLD18|FLD13))) &&  (all_empty_or_white((FLD31|FLD36))) &&  (empty_field & FLD27)) {
//so this is what anyway has to be there to have a voorpost

//if the field sw of b is occupied by me or can be occupied by me, no attack is possible, so the question is, can the piece on sw(b) be exchanged
if ((white_man & FLD23) ||  ((white_man & FLD29) && ((all_empty_or_white(FLD19|FLD28)) ||  (all_occupied(FLD19|FLD28))))) {
if ((all_black((FLD12|FLD7|FLD1)) && (empty_field & FLD18)) &&
    !(((black_man & FLD17) && (empty_field & FLD21)) ||  ((black_man & FLD8) && (empty_field & FLD3))))
number_of_attacks = -1;  // can be exchanged,  start with one attack less
else goto check_afruil; // no attacks possible
}

//same for the defence: if ne(d) is occupied by him, we may have a problem in the defence
if (black_man & FLD26) {
if ((all_white((FLD37|FLD0|FLD48)) && (empty_field & FLD31)) ||
    (all_white((FLD31|FLD42|FLD48)) && (empty_field & FLD37)))
number_of_defences = count(((FLD31|FLD37|FLD41|FLD42|FLD46|FLD47|FLD48) & white_man)) - 1;
else number_of_defences = 0;
} else number_of_defences = count(((FLD31|FLD37|FLD41|FLD42|FLD46|FLD47|FLD48) & white_man));

//check the availability of the backing pieces
backing_defence_piece = bool ((FLD36|FLD41|FLD46|FLD47) & white_man);
backing_attack_piece = (bool) ((FLD2|FLD3|FLD4|FLD8|FLD9|FLD13) & black_man);
number_of_attacks += count(((FLD1|FLD2|FLD3|FLD7|FLD8|FLD12|FLD18) & black_man));
if (!((FLD4|FLD9|FLD13) & black_man)) number_of_attacks -= 1;
if (!((FLD36) & white_man)) number_of_defences -= 1;

//so now we know whether we have a backing piece and how many pieces for the attack/defence
if (number_of_attacks <= 0) return 180;
if (!(backing_defence_piece) && (backing_attack_piece) && (number_of_attacks > 0)) { // looks bad, but maybe an escape?
if ((white_man & FLD21) && (all_black((FLD17|FLD12))) && ( all_empty((FLD18|FLD23)))) return 0;
his_pieces_used = 0;   // calc the minimal amount of steps for having the backing piece in place as well as the attack resp.defence piece
his_min_backing_steps = calc_dist_3( (FLD13), (FLD8|FLD9), (FLD2|FLD3|FLD4),  &his_pieces_used, black_man);
his_min_attack_steps = calc_dist_4( (FLD18), (FLD12), (FLD7|FLD8), (FLD1|FLD2|FLD3),  &his_pieces_used, black_man);
if ((all_empty_or_white((FLD21|FLD32)))  || (all_occupied((FLD21|FLD32)))) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_4( (FLD31), (FLD36|FLD37), (FLD41|FLD42), (FLD46|FLD47|FLD48),  &my_pieces_used, white_man) + 1;
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if (empty_or_white & FLD21) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_5( (FLD27), (FLD31|FLD32), (FLD36|FLD37|FLD38), (FLD41|FLD42|FLD43), (FLD46|FLD47|FLD48|FLD49),  &my_pieces_used, white_man);
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if ((all_white((FLD28|FLD33))) && (all_empty(( FLD17|FLD11))))  return -200; // vlucht
if ((occupied & FLD21) && (all_empty(( FLD17|FLD11))))  return -200; // vlucht
if ((all_white((FLD28|FLD33))) && (black_man & FLD11) && (occupied & FLD21) && (empty_field & FLD17)) return 0; //paddestoel
if ((all_white((FLD28|FLD33))) && (black_man & FLD11) && (empty_or_white &FLD21) && (empty_field & FLD17)) return 0; //kan wegruilen
 return -700;
}

if (!(backing_attack_piece)) goto check_afruil;
// both backings are available
if (number_of_attacks > number_of_defences) {  // it looks bad, but maybe an escape?
if ((white_man & FLD21) && (all_black((FLD17|FLD12))) && ( all_empty((FLD18|FLD23)))) return 0;
his_pieces_used = 0;   // calc the minimal amount of steps for having the backing piece in place as well as the attack resp.defence piece
his_min_backing_steps = calc_dist_3( (FLD13), (FLD8|FLD9), (FLD2|FLD3|FLD4),  &his_pieces_used, black_man);
his_min_attack_steps = calc_dist_4( (FLD18), (FLD12), (FLD7|FLD8), (FLD1|FLD2|FLD3),  &his_pieces_used, black_man);
if ((all_empty_or_white((FLD21|FLD32)))  || (all_occupied((FLD21|FLD32)))) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_4( (FLD31), (FLD36|FLD37), (FLD41|FLD42), (FLD46|FLD47|FLD48),  &my_pieces_used, white_man) + 1;
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if (empty_or_white & FLD21) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_5( (FLD27), (FLD31|FLD32), (FLD36|FLD37|FLD38), (FLD41|FLD42|FLD43), (FLD46|FLD47|FLD48|FLD49),  &my_pieces_used, white_man);
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if ((all_white((FLD28|FLD33))) && (all_empty(( FLD17|FLD11))))  return -200; // vlucht
if ((occupied & FLD21) && (all_empty(( FLD17|FLD11))))  return -200; // vlucht
if ((all_white((FLD28|FLD33))) && (black_man & FLD11) && (occupied & FLD21) && (empty_field & FLD17)) return 0; //paddestoel
if ((all_white((FLD28|FLD33))) && (black_man & FLD11) && (empty_or_white &FLD21) && (empty_field & FLD17)) return 0; //kan wegruilen
return -700;
}

//calc the minimal amount of steps for having the backing piece in place as well as the attack resp.defence piece
his_pieces_used = 0;
his_min_backing_steps = calc_dist_3( (FLD13), (FLD8|FLD9), (FLD2|FLD3|FLD4),  &his_pieces_used, black_man);
his_min_attack_steps = calc_dist_4( (FLD18), (FLD12), (FLD7|FLD8), (FLD1|FLD2|FLD3),  &his_pieces_used, black_man);
my_pieces_used = 0;
my_min_backing_steps = calc_dist_3( (FLD36), (FLD41), (FLD46|FLD47),  &my_pieces_used, white_man);
my_min_defence_steps = calc_dist_4( (FLD31), (FLD37), (FLD41|FLD42), (FLD46|FLD47|FLD48),  &my_pieces_used, white_man);

// we have calculated: his_min_backing_steps, his_min_attack_steps, my_min_defence_steps and my_min_backing steps

if ((his_min_backing_steps + his_min_attack_steps) - (my_min_backing_steps + my_min_defence_steps + inc) < -2) {
if ((white_man & FLD21) && (all_black((FLD17|FLD12))) && ( all_empty((FLD18|FLD23)))) return 0;
his_pieces_used = 0;   // calc the minimal amount of steps for having the backing piece in place as well as the attack resp.defence piece
his_min_backing_steps = calc_dist_3( (FLD13), (FLD8|FLD9), (FLD2|FLD3|FLD4),  &his_pieces_used, black_man);
his_min_attack_steps = calc_dist_4( (FLD18), (FLD12), (FLD7|FLD8), (FLD1|FLD2|FLD3),  &his_pieces_used, black_man);
if ((all_empty_or_white((FLD21|FLD32)))  || (all_occupied((FLD21|FLD32)))) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_4( (FLD31), (FLD36|FLD37), (FLD41|FLD42), (FLD46|FLD47|FLD48),  &my_pieces_used, white_man) + 1;
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if (empty_or_white & FLD21) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_5( (FLD27), (FLD31|FLD32), (FLD36|FLD37|FLD38), (FLD41|FLD42|FLD43), (FLD46|FLD47|FLD48|FLD49),  &my_pieces_used, white_man);
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if ((all_white((FLD28|FLD33))) && (all_empty(( FLD17|FLD11))))  return -200; // vlucht
if ((occupied & FLD21) && (all_empty(( FLD17|FLD11))))  return -200; // vlucht
if ((all_white((FLD28|FLD33))) && (black_man & FLD11) && (occupied & FLD21) && (empty_field & FLD17)) return 0; //paddestoel
if ((all_white((FLD28|FLD33))) && (black_man & FLD11) && (empty_or_white &FLD21) && (empty_field & FLD17)) return 0; //kan wegruilen
return -700;
}

// and a second attack??

if (number_of_attacks > 1) {
his_min_attack_steps = calc_dist_4( (FLD18), (FLD12), (FLD7|FLD8), (FLD1|FLD2|FLD3),  &his_pieces_used, black_man);
my_min_defence_steps = calc_dist_4( (FLD31), (FLD37), (FLD41|FLD42), (FLD46|FLD47|FLD48),  &my_pieces_used, white_man);
if ((his_min_attack_steps) - (my_min_defence_steps + inc) < -2) { // too late for the second attempt
if ((white_man & FLD21) && (all_black((FLD17|FLD12))) && ( all_empty((FLD18|FLD23)))) return 0;
his_pieces_used = 0;   // calc the minimal amount of steps for having the backing piece in place as well as the attack resp.defence piece
his_min_backing_steps = calc_dist_3( (FLD13), (FLD8|FLD9), (FLD2|FLD3|FLD4),  &his_pieces_used, black_man);
his_min_attack_steps = calc_dist_4( (FLD18), (FLD12), (FLD7|FLD8), (FLD1|FLD2|FLD3),  &his_pieces_used, black_man);
if ((all_empty_or_white((FLD21|FLD32)))  || (all_occupied((FLD21|FLD32)))) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_4( (FLD31), (FLD36|FLD37), (FLD41|FLD42), (FLD46|FLD47|FLD48),  &my_pieces_used, white_man) + 1;
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if (empty_or_white & FLD21) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_5( (FLD27), (FLD31|FLD32), (FLD36|FLD37|FLD38), (FLD41|FLD42|FLD43), (FLD46|FLD47|FLD48|FLD49),  &my_pieces_used, white_man);
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if ((all_white((FLD28|FLD33))) && (all_empty(( FLD17|FLD11))))  return -200; // vlucht
if ((occupied & FLD21) && (all_empty(( FLD17|FLD11))))  return -200; // vlucht
if ((all_white((FLD28|FLD33))) && (black_man & FLD11) && (occupied & FLD21) && (empty_field & FLD17)) return 0; //paddestoel
if ((all_white((FLD28|FLD33))) && (black_man & FLD11) && (empty_or_white &FLD21) && (empty_field & FLD17)) return 0; //kan wegruilen
return -400;
}
}
check_afruil:
if (all_black((FLD4|FLD9|FLD13))) return 0; // voorpost wordt afgeruild
if (all_black((FLD6|FLD11))) return 0; // voorpost wordt afgeruild
// so we have hope to have a nice voorpost
return 180;  // no problems, so a small advantage
}
return 0;
}
extern int voorpost_white_sw_23(BitArray, BitArray, BitArray, int);
int voorpost_white_sw_23(BitArray white_man, BitArray black_man, BitArray empty_field, int inc) {
BitArray my_pieces_used, his_pieces_used;
int number_of_defences, number_of_attacks = 0;
int his_min_backing_steps, my_min_backing_steps;
int my_min_defence_steps, his_min_attack_steps;
bool backing_defence_piece, backing_attack_piece;

if ((white_man & FLD23) &&  (all_empty_or_black((FLD19|FLD14))) &&  (all_empty_or_white((FLD32|FLD37))) &&  (empty_field & FLD28)) {
//so this is what anyway has to be there to have a voorpost

//if the field sw of b is occupied by me or can be occupied by me, no attack is possible, so the question is, can the piece on sw(b) be exchanged
if ((white_man & FLD24) ||  ((white_man & FLD30) && ((all_empty_or_white(FLD20|FLD29)) ||  (all_occupied(FLD20|FLD29))))) {
if ((all_black((FLD13|FLD8|FLD2)) && (empty_field & FLD19)) &&
    !(((black_man & FLD18) && (empty_field & FLD22)) ||  ((black_man & FLD9) && (empty_field & FLD4))))
number_of_attacks = -1;  // can be exchanged,  start with one attack less
else goto check_afruil; // no attacks possible
}

//same for the defence: if ne(d) is occupied by him, we may have a problem in the defence
if (black_man & FLD27) {
if ((all_white((FLD38|FLD41|FLD49)) && (empty_field & FLD32)) ||
    (all_white((FLD32|FLD43|FLD49)) && (empty_field & FLD38)))
number_of_defences = count(((FLD32|FLD38|FLD42|FLD43|FLD47|FLD48|FLD49) & white_man)) - 1;
else number_of_defences = 0;
} else number_of_defences = count(((FLD32|FLD38|FLD42|FLD43|FLD47|FLD48|FLD49) & white_man));

//check the availability of the backing pieces
backing_defence_piece = bool ((FLD37|FLD41|FLD42|FLD46|FLD47|FLD48) & white_man);
backing_attack_piece = (bool) ((FLD3|FLD4|FLD5|FLD9|FLD10|FLD14) & black_man);
number_of_attacks += count(((FLD2|FLD3|FLD4|FLD8|FLD9|FLD13|FLD19) & black_man));
if (!((FLD5|FLD10|FLD14) & black_man)) number_of_attacks -= 1;
if (!((FLD37|FLD41|FLD46) & white_man)) number_of_defences -= 1;

//so now we know whether we have a backing piece and how many pieces for the attack/defence
if (number_of_attacks <= 0) return 180;
if (!(backing_defence_piece) && (backing_attack_piece) && (number_of_attacks > 0)) { // looks bad, but maybe an escape?
if ((white_man & FLD22) && (all_black((FLD18|FLD13))) && ( all_empty((FLD19|FLD24)))) return 0;
his_pieces_used = 0;   // calc the minimal amount of steps for having the backing piece in place as well as the attack resp.defence piece
his_min_backing_steps = calc_dist_3( (FLD14), (FLD9|FLD10), (FLD3|FLD4|FLD5),  &his_pieces_used, black_man);
his_min_attack_steps = calc_dist_4( (FLD19), (FLD13), (FLD8|FLD9), (FLD2|FLD3|FLD4),  &his_pieces_used, black_man);
if ((all_empty_or_white((FLD22|FLD33)))  || (all_occupied((FLD22|FLD33)))) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_4( (FLD32), (FLD37|FLD38), (FLD41|FLD42|FLD43), (FLD46|FLD47|FLD48|FLD49),  &my_pieces_used, white_man) + 1;
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if (empty_or_white & FLD22) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_5( (FLD28), (FLD32|FLD33), (FLD37|FLD38|FLD39), (FLD41|FLD42|FLD43|FLD44), (FLD46|FLD47|FLD48|FLD49|FLD50),  &my_pieces_used, white_man);
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if ((all_white((FLD29|FLD34))) && (all_empty(( FLD18|FLD12))))  return -200; // vlucht
if ((occupied & FLD22) && (all_empty(( FLD18|FLD12))))  return -200; // vlucht
if ((all_white((FLD29|FLD34))) && (black_man & FLD12) && (occupied & FLD22) && (empty_field & FLD18)) return 0; //paddestoel
if ((all_white((FLD29|FLD34))) && (black_man & FLD12) && (empty_or_white &FLD22) && (empty_field & FLD18)) return 0; //kan wegruilen
 return -700;
}

if (!(backing_attack_piece)) goto check_afruil;
// both backings are available
if (number_of_attacks > number_of_defences) {  // it looks bad, but maybe an escape?
if ((white_man & FLD22) && (all_black((FLD18|FLD13))) && ( all_empty((FLD19|FLD24)))) return 0;
his_pieces_used = 0;   // calc the minimal amount of steps for having the backing piece in place as well as the attack resp.defence piece
his_min_backing_steps = calc_dist_3( (FLD14), (FLD9|FLD10), (FLD3|FLD4|FLD5),  &his_pieces_used, black_man);
his_min_attack_steps = calc_dist_4( (FLD19), (FLD13), (FLD8|FLD9), (FLD2|FLD3|FLD4),  &his_pieces_used, black_man);
if ((all_empty_or_white((FLD22|FLD33)))  || (all_occupied((FLD22|FLD33)))) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_4( (FLD32), (FLD37|FLD38), (FLD41|FLD42|FLD43), (FLD46|FLD47|FLD48|FLD49),  &my_pieces_used, white_man) + 1;
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if (empty_or_white & FLD22) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_5( (FLD28), (FLD32|FLD33), (FLD37|FLD38|FLD39), (FLD41|FLD42|FLD43|FLD44), (FLD46|FLD47|FLD48|FLD49|FLD50),  &my_pieces_used, white_man);
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if ((all_white((FLD29|FLD34))) && (all_empty(( FLD18|FLD12))))  return -200; // vlucht
if ((occupied & FLD22) && (all_empty(( FLD18|FLD12))))  return -200; // vlucht
if ((all_white((FLD29|FLD34))) && (black_man & FLD12) && (occupied & FLD22) && (empty_field & FLD18)) return 0; //paddestoel
if ((all_white((FLD29|FLD34))) && (black_man & FLD12) && (empty_or_white &FLD22) && (empty_field & FLD18)) return 0; //kan wegruilen
return -700;
}

//calc the minimal amount of steps for having the backing piece in place as well as the attack resp.defence piece
his_pieces_used = 0;
his_min_backing_steps = calc_dist_3( (FLD14), (FLD9|FLD10), (FLD3|FLD4|FLD5),  &his_pieces_used, black_man);
his_min_attack_steps = calc_dist_4( (FLD19), (FLD13), (FLD8|FLD9), (FLD2|FLD3|FLD4),  &his_pieces_used, black_man);
my_pieces_used = 0;
my_min_backing_steps = calc_dist_3( (FLD37), (FLD41|FLD42), (FLD46|FLD47|FLD48),  &my_pieces_used, white_man);
my_min_defence_steps = calc_dist_4( (FLD32), (FLD38), (FLD42|FLD43), (FLD47|FLD48|FLD49),  &my_pieces_used, white_man);

// we have calculated: his_min_backing_steps, his_min_attack_steps, my_min_defence_steps and my_min_backing steps

if ((his_min_backing_steps + his_min_attack_steps) - (my_min_backing_steps + my_min_defence_steps + inc) < -2) {
if ((white_man & FLD22) && (all_black((FLD18|FLD13))) && ( all_empty((FLD19|FLD24)))) return 0;
his_pieces_used = 0;   // calc the minimal amount of steps for having the backing piece in place as well as the attack resp.defence piece
his_min_backing_steps = calc_dist_3( (FLD14), (FLD9|FLD10), (FLD3|FLD4|FLD5),  &his_pieces_used, black_man);
his_min_attack_steps = calc_dist_4( (FLD19), (FLD13), (FLD8|FLD9), (FLD2|FLD3|FLD4),  &his_pieces_used, black_man);
if ((all_empty_or_white((FLD22|FLD33)))  || (all_occupied((FLD22|FLD33)))) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_4( (FLD32), (FLD37|FLD38), (FLD41|FLD42|FLD43), (FLD46|FLD47|FLD48|FLD49),  &my_pieces_used, white_man) + 1;
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if (empty_or_white & FLD22) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_5( (FLD28), (FLD32|FLD33), (FLD37|FLD38|FLD39), (FLD41|FLD42|FLD43|FLD44), (FLD46|FLD47|FLD48|FLD49|FLD50),  &my_pieces_used, white_man);
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if ((all_white((FLD29|FLD34))) && (all_empty(( FLD18|FLD12))))  return -200; // vlucht
if ((occupied & FLD22) && (all_empty(( FLD18|FLD12))))  return -200; // vlucht
if ((all_white((FLD29|FLD34))) && (black_man & FLD12) && (occupied & FLD22) && (empty_field & FLD18)) return 0; //paddestoel
if ((all_white((FLD29|FLD34))) && (black_man & FLD12) && (empty_or_white &FLD22) && (empty_field & FLD18)) return 0; //kan wegruilen
return -700;
}

// and a second attack??

if (number_of_attacks > 1) {
his_min_attack_steps = calc_dist_4( (FLD19), (FLD13), (FLD8|FLD9), (FLD2|FLD3|FLD4),  &his_pieces_used, black_man);
my_min_defence_steps = calc_dist_4( (FLD32), (FLD38), (FLD42|FLD43), (FLD47|FLD48|FLD49),  &my_pieces_used, white_man);
if ((his_min_attack_steps) - (my_min_defence_steps + inc) < -2) { // too late for the second attempt
if ((white_man & FLD22) && (all_black((FLD18|FLD13))) && ( all_empty((FLD19|FLD24)))) return 0;
his_pieces_used = 0;   // calc the minimal amount of steps for having the backing piece in place as well as the attack resp.defence piece
his_min_backing_steps = calc_dist_3( (FLD14), (FLD9|FLD10), (FLD3|FLD4|FLD5),  &his_pieces_used, black_man);
his_min_attack_steps = calc_dist_4( (FLD19), (FLD13), (FLD8|FLD9), (FLD2|FLD3|FLD4),  &his_pieces_used, black_man);
if ((all_empty_or_white((FLD22|FLD33)))  || (all_occupied((FLD22|FLD33)))) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_4( (FLD32), (FLD37|FLD38), (FLD41|FLD42|FLD43), (FLD46|FLD47|FLD48|FLD49),  &my_pieces_used, white_man) + 1;
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if (empty_or_white & FLD22) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_5( (FLD28), (FLD32|FLD33), (FLD37|FLD38|FLD39), (FLD41|FLD42|FLD43|FLD44), (FLD46|FLD47|FLD48|FLD49|FLD50),  &my_pieces_used, white_man);
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if ((all_white((FLD29|FLD34))) && (all_empty(( FLD18|FLD12))))  return -200; // vlucht
if ((occupied & FLD22) && (all_empty(( FLD18|FLD12))))  return -200; // vlucht
if ((all_white((FLD29|FLD34))) && (black_man & FLD12) && (occupied & FLD22) && (empty_field & FLD18)) return 0; //paddestoel
if ((all_white((FLD29|FLD34))) && (black_man & FLD12) && (empty_or_white &FLD22) && (empty_field & FLD18)) return 0; //kan wegruilen
return -400;
}
}
check_afruil:
if (all_black((FLD5|FLD10|FLD14))) return 0; // voorpost wordt afgeruild
if (all_black((FLD1|FLD7|FLD12))) return 0; // voorpost wordt afgeruild
// so we have hope to have a nice voorpost
return 180;  // no problems, so a small advantage
}
return 0;
}
extern int voorpost_white_sw_24(BitArray, BitArray, BitArray, int);
int voorpost_white_sw_24(BitArray white_man, BitArray black_man, BitArray empty_field, int inc) {
BitArray my_pieces_used, his_pieces_used;
int number_of_defences, number_of_attacks = 0;
int his_min_backing_steps, my_min_backing_steps;
int my_min_defence_steps, his_min_attack_steps;
bool backing_defence_piece, backing_attack_piece;

if ((white_man & FLD24) &&  (all_empty_or_black((FLD20|FLD15))) &&  (all_empty_or_white((FLD33|FLD38))) &&  (empty_field & FLD29)) {
//so this is what anyway has to be there to have a voorpost

//if the field sw of b is occupied by me or can be occupied by me, no attack is possible, so the question is, can the piece on sw(b) be exchanged
if (white_man & FLD25) {
if ((all_black((FLD14|FLD9|FLD3)) && (empty_field & FLD20)) &&
    !(((black_man & FLD19) && (empty_field & FLD23)) ||  ((black_man & FLD10) && (empty_field & FLD5))))
number_of_attacks = -1;  // can be exchanged,  start with one attack less
else goto check_afruil; // no attacks possible
}

//same for the defence: if ne(d) is occupied by him, we may have a problem in the defence
if (black_man & FLD28) {
if ((all_white((FLD39|FLD42|FLD50)) && (empty_field & FLD33)) ||
    (all_white((FLD33|FLD44|FLD50)) && (empty_field & FLD39)))
number_of_defences = count(((FLD33|FLD39|FLD43|FLD44|FLD48|FLD49|FLD50) & white_man)) - 1;
else number_of_defences = 0;
} else number_of_defences = count(((FLD33|FLD39|FLD43|FLD44|FLD48|FLD49|FLD50) & white_man));

//check the availability of the backing pieces
backing_defence_piece = bool ((FLD38|FLD42|FLD43|FLD47|FLD48|FLD49) & white_man);
backing_attack_piece = (bool) ((FLD4|FLD5|FLD10|FLD15) & black_man);
number_of_attacks += count(((FLD3|FLD4|FLD5|FLD9|FLD10|FLD14|FLD20) & black_man));
if (!((FLD15) & black_man)) number_of_attacks -= 1;
if (!((FLD38|FLD42|FLD47) & white_man)) number_of_defences -= 1;

//so now we know whether we have a backing piece and how many pieces for the attack/defence
if (number_of_attacks <= 0) return 180;
if (!(backing_defence_piece) && (backing_attack_piece) && (number_of_attacks > 0)) { // looks bad, but maybe an escape?
if ((white_man & FLD23) && (all_black((FLD19|FLD14))) && ( all_empty((FLD20|FLD25)))) return 0;
his_pieces_used = 0;   // calc the minimal amount of steps for having the backing piece in place as well as the attack resp.defence piece
his_min_backing_steps = calc_dist_3( (FLD15), (FLD10), (FLD4|FLD5),  &his_pieces_used, black_man);
his_min_attack_steps = calc_dist_4( (FLD20), (FLD14), (FLD9|FLD10), (FLD3|FLD4|FLD5),  &his_pieces_used, black_man);
if ((all_empty_or_white((FLD23|FLD34)))  || (all_occupied((FLD23|FLD34)))) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_4( (FLD33), (FLD38|FLD39), (FLD42|FLD43|FLD44), (FLD47|FLD48|FLD49|FLD50),  &my_pieces_used, white_man) + 1;
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if (empty_or_white & FLD23) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_5( (FLD29), (FLD33|FLD34), (FLD38|FLD39|FLD40), (FLD42|FLD43|FLD44|FLD45), (FLD47|FLD48|FLD49|FLD50),  &my_pieces_used, white_man);
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if ((all_white((FLD30|FLD35))) && (all_empty(( FLD19|FLD13))))  return -200; // vlucht
if ((occupied & FLD23) && (all_empty(( FLD19|FLD13))))  return -200; // vlucht
if ((all_white((FLD30|FLD35))) && (black_man & FLD13) && (occupied & FLD23) && (empty_field & FLD19)) return 0; //paddestoel
if ((all_white((FLD30|FLD35))) && (black_man & FLD13) && (empty_or_white &FLD23) && (empty_field & FLD19)) return 0; //kan wegruilen
 return -700;
}

if (!(backing_attack_piece)) goto check_afruil;
// both backings are available
if (number_of_attacks > number_of_defences) {  // it looks bad, but maybe an escape?
if ((white_man & FLD23) && (all_black((FLD19|FLD14))) && ( all_empty((FLD20|FLD25)))) return 0;
his_pieces_used = 0;   // calc the minimal amount of steps for having the backing piece in place as well as the attack resp.defence piece
his_min_backing_steps = calc_dist_3( (FLD15), (FLD10), (FLD4|FLD5),  &his_pieces_used, black_man);
his_min_attack_steps = calc_dist_4( (FLD20), (FLD14), (FLD9|FLD10), (FLD3|FLD4|FLD5),  &his_pieces_used, black_man);
if ((all_empty_or_white((FLD23|FLD34)))  || (all_occupied((FLD23|FLD34)))) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_4( (FLD33), (FLD38|FLD39), (FLD42|FLD43|FLD44), (FLD47|FLD48|FLD49|FLD50),  &my_pieces_used, white_man) + 1;
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if (empty_or_white & FLD23) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_5( (FLD29), (FLD33|FLD34), (FLD38|FLD39|FLD40), (FLD42|FLD43|FLD44|FLD45), (FLD47|FLD48|FLD49|FLD50),  &my_pieces_used, white_man);
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if ((all_white((FLD30|FLD35))) && (all_empty(( FLD19|FLD13))))  return -200; // vlucht
if ((occupied & FLD23) && (all_empty(( FLD19|FLD13))))  return -200; // vlucht
if ((all_white((FLD30|FLD35))) && (black_man & FLD13) && (occupied & FLD23) && (empty_field & FLD19)) return 0; //paddestoel
if ((all_white((FLD30|FLD35))) && (black_man & FLD13) && (empty_or_white &FLD23) && (empty_field & FLD19)) return 0; //kan wegruilen
return -700;
}

//calc the minimal amount of steps for having the backing piece in place as well as the attack resp.defence piece
his_pieces_used = 0;
his_min_backing_steps = calc_dist_3( (FLD15), (FLD10), (FLD4|FLD5),  &his_pieces_used, black_man);
his_min_attack_steps = calc_dist_4( (FLD20), (FLD14), (FLD9|FLD10), (FLD3|FLD4|FLD5),  &his_pieces_used, black_man);
my_pieces_used = 0;
my_min_backing_steps = calc_dist_3( (FLD38), (FLD42|FLD43), (FLD47|FLD48|FLD49),  &my_pieces_used, white_man);
my_min_defence_steps = calc_dist_4( (FLD33), (FLD39), (FLD43|FLD44), (FLD48|FLD49|FLD50),  &my_pieces_used, white_man);

// we have calculated: his_min_backing_steps, his_min_attack_steps, my_min_defence_steps and my_min_backing steps

if ((his_min_backing_steps + his_min_attack_steps) - (my_min_backing_steps + my_min_defence_steps + inc) < -2) {
if ((white_man & FLD23) && (all_black((FLD19|FLD14))) && ( all_empty((FLD20|FLD25)))) return 0;
his_pieces_used = 0;   // calc the minimal amount of steps for having the backing piece in place as well as the attack resp.defence piece
his_min_backing_steps = calc_dist_3( (FLD15), (FLD10), (FLD4|FLD5),  &his_pieces_used, black_man);
his_min_attack_steps = calc_dist_4( (FLD20), (FLD14), (FLD9|FLD10), (FLD3|FLD4|FLD5),  &his_pieces_used, black_man);
if ((all_empty_or_white((FLD23|FLD34)))  || (all_occupied((FLD23|FLD34)))) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_4( (FLD33), (FLD38|FLD39), (FLD42|FLD43|FLD44), (FLD47|FLD48|FLD49|FLD50),  &my_pieces_used, white_man) + 1;
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if (empty_or_white & FLD23) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_5( (FLD29), (FLD33|FLD34), (FLD38|FLD39|FLD40), (FLD42|FLD43|FLD44|FLD45), (FLD47|FLD48|FLD49|FLD50),  &my_pieces_used, white_man);
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if ((all_white((FLD30|FLD35))) && (all_empty(( FLD19|FLD13))))  return -200; // vlucht
if ((occupied & FLD23) && (all_empty(( FLD19|FLD13))))  return -200; // vlucht
if ((all_white((FLD30|FLD35))) && (black_man & FLD13) && (occupied & FLD23) && (empty_field & FLD19)) return 0; //paddestoel
if ((all_white((FLD30|FLD35))) && (black_man & FLD13) && (empty_or_white &FLD23) && (empty_field & FLD19)) return 0; //kan wegruilen
return -700;
}

// and a second attack??

if (number_of_attacks > 1) {
his_min_attack_steps = calc_dist_4( (FLD20), (FLD14), (FLD9|FLD10), (FLD3|FLD4|FLD5),  &his_pieces_used, black_man);
my_min_defence_steps = calc_dist_4( (FLD33), (FLD39), (FLD43|FLD44), (FLD48|FLD49|FLD50),  &my_pieces_used, white_man);
if ((his_min_attack_steps) - (my_min_defence_steps + inc) < -2) { // too late for the second attempt
if ((white_man & FLD23) && (all_black((FLD19|FLD14))) && ( all_empty((FLD20|FLD25)))) return 0;
his_pieces_used = 0;   // calc the minimal amount of steps for having the backing piece in place as well as the attack resp.defence piece
his_min_backing_steps = calc_dist_3( (FLD15), (FLD10), (FLD4|FLD5),  &his_pieces_used, black_man);
his_min_attack_steps = calc_dist_4( (FLD20), (FLD14), (FLD9|FLD10), (FLD3|FLD4|FLD5),  &his_pieces_used, black_man);
if ((all_empty_or_white((FLD23|FLD34)))  || (all_occupied((FLD23|FLD34)))) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_4( (FLD33), (FLD38|FLD39), (FLD42|FLD43|FLD44), (FLD47|FLD48|FLD49|FLD50),  &my_pieces_used, white_man) + 1;
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if (empty_or_white & FLD23) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_5( (FLD29), (FLD33|FLD34), (FLD38|FLD39|FLD40), (FLD42|FLD43|FLD44|FLD45), (FLD47|FLD48|FLD49|FLD50),  &my_pieces_used, white_man);
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if ((all_white((FLD30|FLD35))) && (all_empty(( FLD19|FLD13))))  return -200; // vlucht
if ((occupied & FLD23) && (all_empty(( FLD19|FLD13))))  return -200; // vlucht
if ((all_white((FLD30|FLD35))) && (black_man & FLD13) && (occupied & FLD23) && (empty_field & FLD19)) return 0; //paddestoel
if ((all_white((FLD30|FLD35))) && (black_man & FLD13) && (empty_or_white &FLD23) && (empty_field & FLD19)) return 0; //kan wegruilen
return -400;
}
}
check_afruil:
if (all_black((FLD2|FLD8|FLD13))) return 0; // voorpost wordt afgeruild
// so we have hope to have a nice voorpost
return 180;  // no problems, so a small advantage
}
return 0;
}
// zwarte voorposten ne
extern int voorpost_black_ne_27(BitArray, BitArray, BitArray, int);
int voorpost_black_ne_27(BitArray white_man, BitArray black_man, BitArray empty_field, int inc) {
BitArray my_pieces_used, his_pieces_used;
int number_of_defences, number_of_attacks = 0;
int his_min_backing_steps, my_min_backing_steps;
int my_min_defence_steps, his_min_attack_steps;
bool backing_defence_piece, backing_attack_piece;

if ((black_man & FLD27) &&  (all_empty_or_white((FLD31|FLD36))) &&  (all_empty_or_black((FLD18|FLD13))) &&  (empty_field & FLD22)) {
//so this is what anyway has to be there to have a voorpost

//if the field sw of b is occupied by me or can be occupied by me, no attack is possible, so the question is, can the piece on sw(b) be exchanged
if (black_man & FLD26) {
if ((all_white((FLD37|FLD42|FLD48)) && (empty_field & FLD31)) &&
    !(((white_man & FLD32) && (empty_field & FLD28)) ||  ((white_man & FLD41) && (empty_field & FLD46))))
number_of_attacks = -1;  // can be exchanged,  start with one attack less
else goto check_afruil; // no attacks possible
}

//same for the defence: if ne(d) is occupied by him, we may have a problem in the defence
if (white_man & FLD23) {
if ((all_black((FLD12|FLD9|FLD1)) && (empty_field & FLD18)) ||
    (all_black((FLD18|FLD7|FLD1)) && (empty_field & FLD12)))
number_of_defences = count(((FLD1|FLD2|FLD3|FLD7|FLD8|FLD12|FLD18) & black_man)) - 1;
else number_of_defences = 0;
} else number_of_defences = count(((FLD1|FLD2|FLD3|FLD7|FLD8|FLD12|FLD18) & black_man));

//check the availability of the backing pieces
backing_defence_piece = bool ((FLD2|FLD3|FLD4|FLD8|FLD9|FLD13) & black_man);
backing_attack_piece = (bool) ((FLD36|FLD41|FLD46|FLD47) & white_man);
number_of_attacks += count(((FLD31|FLD37|FLD41|FLD42|FLD46|FLD47|FLD48) & white_man));
if (!((FLD36) & white_man)) number_of_attacks -= 1;
if (!((FLD4|FLD9|FLD13) & black_man)) number_of_defences -= 1;

//so now we know whether we have a backing piece and how many pieces for the attack/defence
if (number_of_attacks <= 0) return 180;
if (!(backing_defence_piece) && (backing_attack_piece) && (number_of_attacks > 0)) { // looks bad, but maybe an escape?
if ((black_man & FLD28) && (all_white((FLD32|FLD37))) && ( all_empty((FLD31|FLD26)))) return 0;
his_pieces_used = 0;   // calc the minimal amount of steps for having the backing piece in place as well as the attack resp.defence piece
his_min_backing_steps = calc_dist_3( (FLD36), (FLD41), (FLD46|FLD47),  &his_pieces_used, white_man);
his_min_attack_steps = calc_dist_4( (FLD31), (FLD37), (FLD41|FLD42), (FLD46|FLD47|FLD48),  &his_pieces_used, white_man);
if ((all_empty_or_black((FLD28|FLD17)))  || (all_occupied((FLD28|FLD17)))) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_4( (FLD18), (FLD12|FLD13), (FLD7|FLD8|FLD9), (FLD1|FLD2|FLD3|FLD4),  &my_pieces_used, black_man) + 1;
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if (empty_or_black & FLD28) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_5( (FLD22), (FLD17|FLD18), (FLD11|FLD12|FLD13), (FLD6|FLD7|FLD8|FLD9), (FLD1|FLD2|FLD3|FLD4),  &my_pieces_used, black_man);
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if ((all_black((FLD21|FLD16))) && (all_empty(( FLD32|FLD38))))  return -200; // vlucht
if ((occupied & FLD28) && (all_empty(( FLD32|FLD38))))  return -200; // vlucht
if ((all_black((FLD21|FLD16))) && (white_man & FLD38) && (occupied & FLD28) && (empty_field & FLD32)) return 0; //paddestoel
if ((all_black((FLD21|FLD16))) && (white_man & FLD38) && (empty_or_black &FLD28) && (empty_field & FLD32)) return 0; //kan wegruilen
 return -700;
}

if (!(backing_attack_piece)) goto check_afruil;
// both backings are available
if (number_of_attacks > number_of_defences) {  // it looks bad, but maybe an escape?
if ((black_man & FLD28) && (all_white((FLD32|FLD37))) && ( all_empty((FLD31|FLD26)))) return 0;
his_pieces_used = 0;   // calc the minimal amount of steps for having the backing piece in place as well as the attack resp.defence piece
his_min_backing_steps = calc_dist_3( (FLD36), (FLD41), (FLD46|FLD47),  &his_pieces_used, white_man);
his_min_attack_steps = calc_dist_4( (FLD31), (FLD37), (FLD41|FLD42), (FLD46|FLD47|FLD48),  &his_pieces_used, white_man);
if ((all_empty_or_black((FLD28|FLD17)))  || (all_occupied((FLD28|FLD17)))) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_4( (FLD18), (FLD12|FLD13), (FLD7|FLD8|FLD9), (FLD1|FLD2|FLD3|FLD4),  &my_pieces_used, black_man) + 1;
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if (empty_or_black & FLD28) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_5( (FLD22), (FLD17|FLD18), (FLD11|FLD12|FLD13), (FLD6|FLD7|FLD8|FLD9), (FLD1|FLD2|FLD3|FLD4),  &my_pieces_used, black_man);
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if ((all_black((FLD21|FLD16))) && (all_empty(( FLD32|FLD38))))  return -200; // vlucht
if ((occupied & FLD28) && (all_empty(( FLD32|FLD38))))  return -200; // vlucht
if ((all_black((FLD21|FLD16))) && (white_man & FLD38) && (occupied & FLD28) && (empty_field & FLD32)) return 0; //paddestoel
if ((all_black((FLD21|FLD16))) && (white_man & FLD38) && (empty_or_black &FLD28) && (empty_field & FLD32)) return 0; //kan wegruilen
return -700;
}

//calc the minimal amount of steps for having the backing piece in place as well as the attack resp.defence piece
his_pieces_used = 0;
his_min_backing_steps = calc_dist_3( (FLD36), (FLD41), (FLD46|FLD47),  &his_pieces_used, white_man);
his_min_attack_steps = calc_dist_4( (FLD31), (FLD37), (FLD41|FLD42), (FLD46|FLD47|FLD48),  &his_pieces_used, white_man);
my_pieces_used = 0;
my_min_backing_steps = calc_dist_3( (FLD13), (FLD8|FLD9), (FLD2|FLD3|FLD4),  &my_pieces_used, black_man);
my_min_defence_steps = calc_dist_4( (FLD18), (FLD12), (FLD7|FLD8), (FLD1|FLD2|FLD3),  &my_pieces_used, black_man);

// we have calculated: his_min_backing_steps, his_min_attack_steps, my_min_defence_steps and my_min_backing steps

if ((his_min_backing_steps + his_min_attack_steps) - (my_min_backing_steps + my_min_defence_steps + inc) < -2) {
if ((black_man & FLD28) && (all_white((FLD32|FLD37))) && ( all_empty((FLD31|FLD26)))) return 0;
his_pieces_used = 0;   // calc the minimal amount of steps for having the backing piece in place as well as the attack resp.defence piece
his_min_backing_steps = calc_dist_3( (FLD36), (FLD41), (FLD46|FLD47),  &his_pieces_used, white_man);
his_min_attack_steps = calc_dist_4( (FLD31), (FLD37), (FLD41|FLD42), (FLD46|FLD47|FLD48),  &his_pieces_used, white_man);
if ((all_empty_or_black((FLD28|FLD17)))  || (all_occupied((FLD28|FLD17)))) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_4( (FLD18), (FLD12|FLD13), (FLD7|FLD8|FLD9), (FLD1|FLD2|FLD3|FLD4),  &my_pieces_used, black_man) + 1;
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if (empty_or_black & FLD28) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_5( (FLD22), (FLD17|FLD18), (FLD11|FLD12|FLD13), (FLD6|FLD7|FLD8|FLD9), (FLD1|FLD2|FLD3|FLD4),  &my_pieces_used, black_man);
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if ((all_black((FLD21|FLD16))) && (all_empty(( FLD32|FLD38))))  return -200; // vlucht
if ((occupied & FLD28) && (all_empty(( FLD32|FLD38))))  return -200; // vlucht
if ((all_black((FLD21|FLD16))) && (white_man & FLD38) && (occupied & FLD28) && (empty_field & FLD32)) return 0; //paddestoel
if ((all_black((FLD21|FLD16))) && (white_man & FLD38) && (empty_or_black &FLD28) && (empty_field & FLD32)) return 0; //kan wegruilen
return -700;
}

// and a second attack??

if (number_of_attacks > 1) {
his_min_attack_steps = calc_dist_4( (FLD31), (FLD37), (FLD41|FLD42), (FLD46|FLD47|FLD48),  &his_pieces_used, white_man);
my_min_defence_steps = calc_dist_4( (FLD18), (FLD12), (FLD7|FLD8), (FLD1|FLD2|FLD3),  &my_pieces_used, black_man);
if ((his_min_attack_steps) - (my_min_defence_steps + inc) < -2) { // too late for the second attempt
if ((black_man & FLD28) && (all_white((FLD32|FLD37))) && ( all_empty((FLD31|FLD26)))) return 0;
his_pieces_used = 0;   // calc the minimal amount of steps for having the backing piece in place as well as the attack resp.defence piece
his_min_backing_steps = calc_dist_3( (FLD36), (FLD41), (FLD46|FLD47),  &his_pieces_used, white_man);
his_min_attack_steps = calc_dist_4( (FLD31), (FLD37), (FLD41|FLD42), (FLD46|FLD47|FLD48),  &his_pieces_used, white_man);
if ((all_empty_or_black((FLD28|FLD17)))  || (all_occupied((FLD28|FLD17)))) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_4( (FLD18), (FLD12|FLD13), (FLD7|FLD8|FLD9), (FLD1|FLD2|FLD3|FLD4),  &my_pieces_used, black_man) + 1;
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if (empty_or_black & FLD28) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_5( (FLD22), (FLD17|FLD18), (FLD11|FLD12|FLD13), (FLD6|FLD7|FLD8|FLD9), (FLD1|FLD2|FLD3|FLD4),  &my_pieces_used, black_man);
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if ((all_black((FLD21|FLD16))) && (all_empty(( FLD32|FLD38))))  return -200; // vlucht
if ((occupied & FLD28) && (all_empty(( FLD32|FLD38))))  return -200; // vlucht
if ((all_black((FLD21|FLD16))) && (white_man & FLD38) && (occupied & FLD28) && (empty_field & FLD32)) return 0; //paddestoel
if ((all_black((FLD21|FLD16))) && (white_man & FLD38) && (empty_or_black &FLD28) && (empty_field & FLD32)) return 0; //kan wegruilen
return -400;
}
}
check_afruil:
if (all_white((FLD49|FLD43|FLD38))) return 0; // voorpost wordt afgeruild
// so we have hope to have a nice voorpost
return 180;  // no problems, so a small advantage
}
return 0;
}
extern int voorpost_black_ne_28(BitArray, BitArray, BitArray, int);
int voorpost_black_ne_28(BitArray white_man, BitArray black_man, BitArray empty_field, int inc) {
BitArray my_pieces_used, his_pieces_used;
int number_of_defences, number_of_attacks = 0;
int his_min_backing_steps, my_min_backing_steps;
int my_min_defence_steps, his_min_attack_steps;
bool backing_defence_piece, backing_attack_piece;

if ((black_man & FLD28) &&  (all_empty_or_white((FLD32|FLD37))) &&  (all_empty_or_black((FLD19|FLD14))) &&  (empty_field & FLD23)) {
//so this is what anyway has to be there to have a voorpost

//if the field sw of b is occupied by me or can be occupied by me, no attack is possible, so the question is, can the piece on sw(b) be exchanged
if ((black_man & FLD27) ||  ((black_man & FLD21) && ((all_empty_or_black(FLD31|FLD22)) ||  (all_occupied(FLD31|FLD22))))) {
if ((all_white((FLD38|FLD43|FLD49)) && (empty_field & FLD32)) &&
    !(((white_man & FLD33) && (empty_field & FLD29)) ||  ((white_man & FLD42) && (empty_field & FLD47))))
number_of_attacks = -1;  // can be exchanged,  start with one attack less
else goto check_afruil; // no attacks possible
}

//same for the defence: if ne(d) is occupied by him, we may have a problem in the defence
if (white_man & FLD24) {
if ((all_black((FLD13|FLD10|FLD2)) && (empty_field & FLD19)) ||
    (all_black((FLD19|FLD8|FLD2)) && (empty_field & FLD13)))
number_of_defences = count(((FLD2|FLD3|FLD4|FLD8|FLD9|FLD13|FLD19) & black_man)) - 1;
else number_of_defences = 0;
} else number_of_defences = count(((FLD2|FLD3|FLD4|FLD8|FLD9|FLD13|FLD19) & black_man));

//check the availability of the backing pieces
backing_defence_piece = bool ((FLD3|FLD4|FLD5|FLD9|FLD10|FLD14) & black_man);
backing_attack_piece = (bool) ((FLD37|FLD41|FLD42|FLD46|FLD47|FLD48) & white_man);
number_of_attacks += count(((FLD32|FLD38|FLD42|FLD43|FLD47|FLD48|FLD49) & white_man));
if (!((FLD37|FLD41|FLD46) & white_man)) number_of_attacks -= 1;
if (!((FLD5|FLD10|FLD14) & black_man)) number_of_defences -= 1;

//so now we know whether we have a backing piece and how many pieces for the attack/defence
if (number_of_attacks <= 0) return 180;
if (!(backing_defence_piece) && (backing_attack_piece) && (number_of_attacks > 0)) { // looks bad, but maybe an escape?
if ((black_man & FLD29) && (all_white((FLD33|FLD38))) && ( all_empty((FLD32|FLD27)))) return 0;
his_pieces_used = 0;   // calc the minimal amount of steps for having the backing piece in place as well as the attack resp.defence piece
his_min_backing_steps = calc_dist_3( (FLD37), (FLD41|FLD42), (FLD46|FLD47|FLD48),  &his_pieces_used, white_man);
his_min_attack_steps = calc_dist_4( (FLD32), (FLD38), (FLD42|FLD43), (FLD47|FLD48|FLD49),  &his_pieces_used, white_man);
if ((all_empty_or_black((FLD29|FLD18)))  || (all_occupied((FLD29|FLD18)))) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_4( (FLD19), (FLD13|FLD14), (FLD8|FLD9|FLD10), (FLD2|FLD3|FLD4|FLD5),  &my_pieces_used, black_man) + 1;
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if (empty_or_black & FLD29) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_5( (FLD23), (FLD18|FLD19), (FLD12|FLD13|FLD14), (FLD7|FLD8|FLD9|FLD10), (FLD1|FLD2|FLD3|FLD4|FLD5),  &my_pieces_used, black_man);
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if ((all_black((FLD22|FLD17))) && (all_empty(( FLD33|FLD39))))  return -200; // vlucht
if ((occupied & FLD29) && (all_empty(( FLD33|FLD39))))  return -200; // vlucht
if ((all_black((FLD22|FLD17))) && (white_man & FLD39) && (occupied & FLD29) && (empty_field & FLD33)) return 0; //paddestoel
if ((all_black((FLD22|FLD17))) && (white_man & FLD39) && (empty_or_black &FLD29) && (empty_field & FLD33)) return 0; //kan wegruilen
 return -700;
}

if (!(backing_attack_piece)) goto check_afruil;
// both backings are available
if (number_of_attacks > number_of_defences) {  // it looks bad, but maybe an escape?
if ((black_man & FLD29) && (all_white((FLD33|FLD38))) && ( all_empty((FLD32|FLD27)))) return 0;
his_pieces_used = 0;   // calc the minimal amount of steps for having the backing piece in place as well as the attack resp.defence piece
his_min_backing_steps = calc_dist_3( (FLD37), (FLD41|FLD42), (FLD46|FLD47|FLD48),  &his_pieces_used, white_man);
his_min_attack_steps = calc_dist_4( (FLD32), (FLD38), (FLD42|FLD43), (FLD47|FLD48|FLD49),  &his_pieces_used, white_man);
if ((all_empty_or_black((FLD29|FLD18)))  || (all_occupied((FLD29|FLD18)))) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_4( (FLD19), (FLD13|FLD14), (FLD8|FLD9|FLD10), (FLD2|FLD3|FLD4|FLD5),  &my_pieces_used, black_man) + 1;
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if (empty_or_black & FLD29) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_5( (FLD23), (FLD18|FLD19), (FLD12|FLD13|FLD14), (FLD7|FLD8|FLD9|FLD10), (FLD1|FLD2|FLD3|FLD4|FLD5),  &my_pieces_used, black_man);
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if ((all_black((FLD22|FLD17))) && (all_empty(( FLD33|FLD39))))  return -200; // vlucht
if ((occupied & FLD29) && (all_empty(( FLD33|FLD39))))  return -200; // vlucht
if ((all_black((FLD22|FLD17))) && (white_man & FLD39) && (occupied & FLD29) && (empty_field & FLD33)) return 0; //paddestoel
if ((all_black((FLD22|FLD17))) && (white_man & FLD39) && (empty_or_black &FLD29) && (empty_field & FLD33)) return 0; //kan wegruilen
return -700;
}

//calc the minimal amount of steps for having the backing piece in place as well as the attack resp.defence piece
his_pieces_used = 0;
his_min_backing_steps = calc_dist_3( (FLD37), (FLD41|FLD42), (FLD46|FLD47|FLD48),  &his_pieces_used, white_man);
his_min_attack_steps = calc_dist_4( (FLD32), (FLD38), (FLD42|FLD43), (FLD47|FLD48|FLD49),  &his_pieces_used, white_man);
my_pieces_used = 0;
my_min_backing_steps = calc_dist_3( (FLD14), (FLD9|FLD10), (FLD3|FLD4|FLD5),  &my_pieces_used, black_man);
my_min_defence_steps = calc_dist_4( (FLD19), (FLD13), (FLD8|FLD9), (FLD2|FLD3|FLD4),  &my_pieces_used, black_man);

// we have calculated: his_min_backing_steps, his_min_attack_steps, my_min_defence_steps and my_min_backing steps

if ((his_min_backing_steps + his_min_attack_steps) - (my_min_backing_steps + my_min_defence_steps + inc) < -2) {
if ((black_man & FLD29) && (all_white((FLD33|FLD38))) && ( all_empty((FLD32|FLD27)))) return 0;
his_pieces_used = 0;   // calc the minimal amount of steps for having the backing piece in place as well as the attack resp.defence piece
his_min_backing_steps = calc_dist_3( (FLD37), (FLD41|FLD42), (FLD46|FLD47|FLD48),  &his_pieces_used, white_man);
his_min_attack_steps = calc_dist_4( (FLD32), (FLD38), (FLD42|FLD43), (FLD47|FLD48|FLD49),  &his_pieces_used, white_man);
if ((all_empty_or_black((FLD29|FLD18)))  || (all_occupied((FLD29|FLD18)))) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_4( (FLD19), (FLD13|FLD14), (FLD8|FLD9|FLD10), (FLD2|FLD3|FLD4|FLD5),  &my_pieces_used, black_man) + 1;
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if (empty_or_black & FLD29) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_5( (FLD23), (FLD18|FLD19), (FLD12|FLD13|FLD14), (FLD7|FLD8|FLD9|FLD10), (FLD1|FLD2|FLD3|FLD4|FLD5),  &my_pieces_used, black_man);
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if ((all_black((FLD22|FLD17))) && (all_empty(( FLD33|FLD39))))  return -200; // vlucht
if ((occupied & FLD29) && (all_empty(( FLD33|FLD39))))  return -200; // vlucht
if ((all_black((FLD22|FLD17))) && (white_man & FLD39) && (occupied & FLD29) && (empty_field & FLD33)) return 0; //paddestoel
if ((all_black((FLD22|FLD17))) && (white_man & FLD39) && (empty_or_black &FLD29) && (empty_field & FLD33)) return 0; //kan wegruilen
return -700;
}

// and a second attack??

if (number_of_attacks > 1) {
his_min_attack_steps = calc_dist_4( (FLD32), (FLD38), (FLD42|FLD43), (FLD47|FLD48|FLD49),  &his_pieces_used, white_man);
my_min_defence_steps = calc_dist_4( (FLD19), (FLD13), (FLD8|FLD9), (FLD2|FLD3|FLD4),  &my_pieces_used, black_man);
if ((his_min_attack_steps) - (my_min_defence_steps + inc) < -2) { // too late for the second attempt
if ((black_man & FLD29) && (all_white((FLD33|FLD38))) && ( all_empty((FLD32|FLD27)))) return 0;
his_pieces_used = 0;   // calc the minimal amount of steps for having the backing piece in place as well as the attack resp.defence piece
his_min_backing_steps = calc_dist_3( (FLD37), (FLD41|FLD42), (FLD46|FLD47|FLD48),  &his_pieces_used, white_man);
his_min_attack_steps = calc_dist_4( (FLD32), (FLD38), (FLD42|FLD43), (FLD47|FLD48|FLD49),  &his_pieces_used, white_man);
if ((all_empty_or_black((FLD29|FLD18)))  || (all_occupied((FLD29|FLD18)))) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_4( (FLD19), (FLD13|FLD14), (FLD8|FLD9|FLD10), (FLD2|FLD3|FLD4|FLD5),  &my_pieces_used, black_man) + 1;
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if (empty_or_black & FLD29) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_5( (FLD23), (FLD18|FLD19), (FLD12|FLD13|FLD14), (FLD7|FLD8|FLD9|FLD10), (FLD1|FLD2|FLD3|FLD4|FLD5),  &my_pieces_used, black_man);
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if ((all_black((FLD22|FLD17))) && (all_empty(( FLD33|FLD39))))  return -200; // vlucht
if ((occupied & FLD29) && (all_empty(( FLD33|FLD39))))  return -200; // vlucht
if ((all_black((FLD22|FLD17))) && (white_man & FLD39) && (occupied & FLD29) && (empty_field & FLD33)) return 0; //paddestoel
if ((all_black((FLD22|FLD17))) && (white_man & FLD39) && (empty_or_black &FLD29) && (empty_field & FLD33)) return 0; //kan wegruilen
return -400;
}
}
check_afruil:
if (all_white((FLD46|FLD41|FLD37))) return 0; // voorpost wordt afgeruild
if (all_white((FLD50|FLD44|FLD39))) return 0; // voorpost wordt afgeruild
// so we have hope to have a nice voorpost
return 180;  // no problems, so a small advantage
}
return 0;
}
extern int voorpost_black_ne_29(BitArray, BitArray, BitArray, int);
int voorpost_black_ne_29(BitArray white_man, BitArray black_man, BitArray empty_field, int inc) {
BitArray my_pieces_used, his_pieces_used;
int number_of_defences, number_of_attacks = 0;
int his_min_backing_steps, my_min_backing_steps;
int my_min_defence_steps, his_min_attack_steps;
bool backing_defence_piece, backing_attack_piece;

if ((black_man & FLD29) &&  (all_empty_or_white((FLD33|FLD38))) &&  (all_empty_or_black((FLD20|FLD15))) &&  (empty_field & FLD24)) {
//so this is what anyway has to be there to have a voorpost

//if the field sw of b is occupied by me or can be occupied by me, no attack is possible, so the question is, can the piece on sw(b) be exchanged
if ((black_man & FLD28) ||  ((black_man & FLD22) && ((all_empty_or_black(FLD32|FLD23)) ||  (all_occupied(FLD32|FLD23))))) {
if ((all_white((FLD39|FLD44|FLD50)) && (empty_field & FLD33)) &&
    !(((white_man & FLD34) && (empty_field & FLD30)) ||  ((white_man & FLD43) && (empty_field & FLD48))))
number_of_attacks = -1;  // can be exchanged,  start with one attack less
else goto check_afruil; // no attacks possible
}

//same for the defence: if ne(d) is occupied by him, we may have a problem in the defence
if (white_man & FLD25) {
if ((all_black((FLD14|FLD0|FLD3)) && (empty_field & FLD20)) ||
    (all_black((FLD20|FLD9|FLD3)) && (empty_field & FLD14)))
number_of_defences = count(((FLD3|FLD4|FLD5|FLD9|FLD10|FLD14|FLD20) & black_man)) - 1;
else number_of_defences = 0;
} else number_of_defences = count(((FLD3|FLD4|FLD5|FLD9|FLD10|FLD14|FLD20) & black_man));

//check the availability of the backing pieces
backing_defence_piece = bool ((FLD4|FLD5|FLD10|FLD15) & black_man);
backing_attack_piece = (bool) ((FLD38|FLD42|FLD43|FLD47|FLD48|FLD49) & white_man);
number_of_attacks += count(((FLD33|FLD39|FLD43|FLD44|FLD48|FLD49|FLD50) & white_man));
if (!((FLD38|FLD42|FLD47) & white_man)) number_of_attacks -= 1;
if (!((FLD15) & black_man)) number_of_defences -= 1;

//so now we know whether we have a backing piece and how many pieces for the attack/defence
if (number_of_attacks <= 0) return 180;
if (!(backing_defence_piece) && (backing_attack_piece) && (number_of_attacks > 0)) { // looks bad, but maybe an escape?
if ((black_man & FLD30) && (all_white((FLD34|FLD39))) && ( all_empty((FLD33|FLD28)))) return 0;
his_pieces_used = 0;   // calc the minimal amount of steps for having the backing piece in place as well as the attack resp.defence piece
his_min_backing_steps = calc_dist_3( (FLD38), (FLD42|FLD43), (FLD47|FLD48|FLD49),  &his_pieces_used, white_man);
his_min_attack_steps = calc_dist_4( (FLD33), (FLD39), (FLD43|FLD44), (FLD48|FLD49|FLD50),  &his_pieces_used, white_man);
if ((all_empty_or_black((FLD30|FLD19)))  || (all_occupied((FLD30|FLD19)))) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_4( (FLD20), (FLD14|FLD15), (FLD9|FLD10), (FLD3|FLD4|FLD5),  &my_pieces_used, black_man) + 1;
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if (empty_or_black & FLD30) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_5( (FLD24), (FLD19|FLD20), (FLD13|FLD14|FLD15), (FLD8|FLD9|FLD10), (FLD2|FLD3|FLD4|FLD5),  &my_pieces_used, black_man);
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if ((all_black((FLD23|FLD18))) && (all_empty(( FLD34|FLD40))))  return -200; // vlucht
if ((occupied & FLD30) && (all_empty(( FLD34|FLD40))))  return -200; // vlucht
if ((all_black((FLD23|FLD18))) && (white_man & FLD40) && (occupied & FLD30) && (empty_field & FLD34)) return 0; //paddestoel
if ((all_black((FLD23|FLD18))) && (white_man & FLD40) && (empty_or_black &FLD30) && (empty_field & FLD34)) return 0; //kan wegruilen
 return -700;
}

if (!(backing_attack_piece)) goto check_afruil;
// both backings are available
if (number_of_attacks > number_of_defences) {  // it looks bad, but maybe an escape?
if ((black_man & FLD30) && (all_white((FLD34|FLD39))) && ( all_empty((FLD33|FLD28)))) return 0;
his_pieces_used = 0;   // calc the minimal amount of steps for having the backing piece in place as well as the attack resp.defence piece
his_min_backing_steps = calc_dist_3( (FLD38), (FLD42|FLD43), (FLD47|FLD48|FLD49),  &his_pieces_used, white_man);
his_min_attack_steps = calc_dist_4( (FLD33), (FLD39), (FLD43|FLD44), (FLD48|FLD49|FLD50),  &his_pieces_used, white_man);
if ((all_empty_or_black((FLD30|FLD19)))  || (all_occupied((FLD30|FLD19)))) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_4( (FLD20), (FLD14|FLD15), (FLD9|FLD10), (FLD3|FLD4|FLD5),  &my_pieces_used, black_man) + 1;
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if (empty_or_black & FLD30) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_5( (FLD24), (FLD19|FLD20), (FLD13|FLD14|FLD15), (FLD8|FLD9|FLD10), (FLD2|FLD3|FLD4|FLD5),  &my_pieces_used, black_man);
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if ((all_black((FLD23|FLD18))) && (all_empty(( FLD34|FLD40))))  return -200; // vlucht
if ((occupied & FLD30) && (all_empty(( FLD34|FLD40))))  return -200; // vlucht
if ((all_black((FLD23|FLD18))) && (white_man & FLD40) && (occupied & FLD30) && (empty_field & FLD34)) return 0; //paddestoel
if ((all_black((FLD23|FLD18))) && (white_man & FLD40) && (empty_or_black &FLD30) && (empty_field & FLD34)) return 0; //kan wegruilen
return -700;
}

//calc the minimal amount of steps for having the backing piece in place as well as the attack resp.defence piece
his_pieces_used = 0;
his_min_backing_steps = calc_dist_3( (FLD38), (FLD42|FLD43), (FLD47|FLD48|FLD49),  &his_pieces_used, white_man);
his_min_attack_steps = calc_dist_4( (FLD33), (FLD39), (FLD43|FLD44), (FLD48|FLD49|FLD50),  &his_pieces_used, white_man);
my_pieces_used = 0;
my_min_backing_steps = calc_dist_3( (FLD15), (FLD10), (FLD4|FLD5),  &my_pieces_used, black_man);
my_min_defence_steps = calc_dist_4( (FLD20), (FLD14), (FLD9|FLD10), (FLD3|FLD4|FLD5),  &my_pieces_used, black_man);

// we have calculated: his_min_backing_steps, his_min_attack_steps, my_min_defence_steps and my_min_backing steps

if ((his_min_backing_steps + his_min_attack_steps) - (my_min_backing_steps + my_min_defence_steps + inc) < -2) {
if ((black_man & FLD30) && (all_white((FLD34|FLD39))) && ( all_empty((FLD33|FLD28)))) return 0;
his_pieces_used = 0;   // calc the minimal amount of steps for having the backing piece in place as well as the attack resp.defence piece
his_min_backing_steps = calc_dist_3( (FLD38), (FLD42|FLD43), (FLD47|FLD48|FLD49),  &his_pieces_used, white_man);
his_min_attack_steps = calc_dist_4( (FLD33), (FLD39), (FLD43|FLD44), (FLD48|FLD49|FLD50),  &his_pieces_used, white_man);
if ((all_empty_or_black((FLD30|FLD19)))  || (all_occupied((FLD30|FLD19)))) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_4( (FLD20), (FLD14|FLD15), (FLD9|FLD10), (FLD3|FLD4|FLD5),  &my_pieces_used, black_man) + 1;
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if (empty_or_black & FLD30) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_5( (FLD24), (FLD19|FLD20), (FLD13|FLD14|FLD15), (FLD8|FLD9|FLD10), (FLD2|FLD3|FLD4|FLD5),  &my_pieces_used, black_man);
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if ((all_black((FLD23|FLD18))) && (all_empty(( FLD34|FLD40))))  return -200; // vlucht
if ((occupied & FLD30) && (all_empty(( FLD34|FLD40))))  return -200; // vlucht
if ((all_black((FLD23|FLD18))) && (white_man & FLD40) && (occupied & FLD30) && (empty_field & FLD34)) return 0; //paddestoel
if ((all_black((FLD23|FLD18))) && (white_man & FLD40) && (empty_or_black &FLD30) && (empty_field & FLD34)) return 0; //kan wegruilen
return -700;
}

// and a second attack??

if (number_of_attacks > 1) {
his_min_attack_steps = calc_dist_4( (FLD33), (FLD39), (FLD43|FLD44), (FLD48|FLD49|FLD50),  &his_pieces_used, white_man);
my_min_defence_steps = calc_dist_4( (FLD20), (FLD14), (FLD9|FLD10), (FLD3|FLD4|FLD5),  &my_pieces_used, black_man);
if ((his_min_attack_steps) - (my_min_defence_steps + inc) < -2) { // too late for the second attempt
if ((black_man & FLD30) && (all_white((FLD34|FLD39))) && ( all_empty((FLD33|FLD28)))) return 0;
his_pieces_used = 0;   // calc the minimal amount of steps for having the backing piece in place as well as the attack resp.defence piece
his_min_backing_steps = calc_dist_3( (FLD38), (FLD42|FLD43), (FLD47|FLD48|FLD49),  &his_pieces_used, white_man);
his_min_attack_steps = calc_dist_4( (FLD33), (FLD39), (FLD43|FLD44), (FLD48|FLD49|FLD50),  &his_pieces_used, white_man);
if ((all_empty_or_black((FLD30|FLD19)))  || (all_occupied((FLD30|FLD19)))) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_4( (FLD20), (FLD14|FLD15), (FLD9|FLD10), (FLD3|FLD4|FLD5),  &my_pieces_used, black_man) + 1;
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if (empty_or_black & FLD30) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_5( (FLD24), (FLD19|FLD20), (FLD13|FLD14|FLD15), (FLD8|FLD9|FLD10), (FLD2|FLD3|FLD4|FLD5),  &my_pieces_used, black_man);
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if ((all_black((FLD23|FLD18))) && (all_empty(( FLD34|FLD40))))  return -200; // vlucht
if ((occupied & FLD30) && (all_empty(( FLD34|FLD40))))  return -200; // vlucht
if ((all_black((FLD23|FLD18))) && (white_man & FLD40) && (occupied & FLD30) && (empty_field & FLD34)) return 0; //paddestoel
if ((all_black((FLD23|FLD18))) && (white_man & FLD40) && (empty_or_black &FLD30) && (empty_field & FLD34)) return 0; //kan wegruilen
return -400;
}
}
check_afruil:
if (all_white((FLD47|FLD42|FLD38))) return 0; // voorpost wordt afgeruild
if (all_white((FLD45|FLD40))) return 0; // voorpost wordt afgeruild
// so we have hope to have a nice voorpost
return 180;  // no problems, so a small advantage
}
return 0;
}
extern int voorpost_black_ne_32(BitArray, BitArray, BitArray, int);
int voorpost_black_ne_32(BitArray white_man, BitArray black_man, BitArray empty_field, int inc) {
BitArray my_pieces_used, his_pieces_used;
int number_of_defences, number_of_attacks = 0;
int his_min_backing_steps, my_min_backing_steps;
int my_min_defence_steps, his_min_attack_steps;
bool backing_defence_piece, backing_attack_piece;

if ((black_man & FLD32) &&  (all_empty_or_white((FLD37|FLD41))) &&  (all_empty_or_black((FLD23|FLD19))) &&  (empty_field & FLD28)) {
//so this is what anyway has to be there to have a voorpost

//if the field sw of b is occupied by me or can be occupied by me, no attack is possible, so the question is, can the piece on sw(b) be exchanged
if ((black_man & FLD31) ||  ((black_man & FLD26) && ((all_empty_or_black(FLD36|FLD27)) ||  (all_occupied(FLD36|FLD27))))) {
if ((all_white((FLD42|FLD48|FLD0)) && (empty_field & FLD37)) &&
    !(((white_man & FLD38) && (empty_field & FLD33)) ||  ((white_man & FLD47) && (empty_field & FLD0))))
number_of_attacks = -1;  // can be exchanged,  start with one attack less
else goto check_afruil; // no attacks possible
}

//same for the defence: if ne(d) is occupied by him, we may have a problem in the defence
if (white_man & FLD29) {
if ((all_black((FLD18|FLD14|FLD7)) && (empty_field & FLD23)) ||
    (all_black((FLD23|FLD12|FLD7)) && (empty_field & FLD18)))
number_of_defences = count(((FLD1|FLD2|FLD3|FLD4|FLD7|FLD8|FLD9|FLD12|FLD13|FLD18|FLD23) & black_man)) - 1;
else number_of_defences = 0;
} else number_of_defences = count(((FLD1|FLD2|FLD3|FLD4|FLD7|FLD8|FLD9|FLD12|FLD13|FLD18|FLD23) & black_man));

//check the availability of the backing pieces
backing_defence_piece = bool ((FLD2|FLD3|FLD4|FLD5|FLD8|FLD9|FLD10|FLD13|FLD14|FLD19) & black_man);
backing_attack_piece = (bool) ((FLD41|FLD46|FLD47) & white_man);
number_of_attacks += count(((FLD37|FLD42|FLD47|FLD48) & white_man));
if (!((FLD41|FLD46) & white_man)) number_of_attacks -= 1;
if (!((FLD5|FLD10|FLD14|FLD19) & black_man)) number_of_defences -= 1;

//so now we know whether we have a backing piece and how many pieces for the attack/defence
if (number_of_attacks <= 0) return 180;
if (!(backing_defence_piece) && (backing_attack_piece) && (number_of_attacks > 0)) { // looks bad, but maybe an escape?
if ((black_man & FLD33) && (all_white((FLD38|FLD42))) && ( all_empty((FLD37|FLD31)))) return 0;
his_pieces_used = 0;   // calc the minimal amount of steps for having the backing piece in place as well as the attack resp.defence piece
his_min_backing_steps = calc_dist_2( (FLD41), (FLD46|FLD47),  &his_pieces_used, white_man);
his_min_attack_steps = calc_dist_3( (FLD37), (FLD42), (FLD47|FLD48),  &his_pieces_used, white_man);
if ((all_empty_or_black((FLD33|FLD22)))  || (all_occupied((FLD33|FLD22)))) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_5( (FLD23), (FLD18|FLD19), (FLD12|FLD13|FLD14), (FLD7|FLD8|FLD9|FLD10), (FLD1|FLD2|FLD3|FLD4|FLD5),  &my_pieces_used, black_man) + 1;
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if (empty_or_black & FLD33) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_6( (FLD28), (FLD22|FLD23), (FLD17|FLD18|FLD19), (FLD11|FLD12|FLD13|FLD14), (FLD6|FLD7|FLD8|FLD9|FLD10), (FLD1|FLD2|FLD3|FLD4|FLD5),  &my_pieces_used, black_man);
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if ((all_black((FLD27|FLD21))) && (all_empty(( FLD38|FLD43))))  return -200; // vlucht
if ((occupied & FLD33) && (all_empty(( FLD38|FLD43))))  return -200; // vlucht
if ((all_black((FLD27|FLD21))) && (white_man & FLD43) && (occupied & FLD33) && (empty_field & FLD38)) return 0; //paddestoel
if ((all_black((FLD27|FLD21))) && (white_man & FLD43) && (empty_or_black &FLD33) && (empty_field & FLD38)) return 0; //kan wegruilen
 return -700;
}

if (!(backing_attack_piece)) goto check_afruil;
// both backings are available
if (number_of_attacks > number_of_defences) {  // it looks bad, but maybe an escape?
if ((black_man & FLD33) && (all_white((FLD38|FLD42))) && ( all_empty((FLD37|FLD31)))) return 0;
his_pieces_used = 0;   // calc the minimal amount of steps for having the backing piece in place as well as the attack resp.defence piece
his_min_backing_steps = calc_dist_2( (FLD41), (FLD46|FLD47),  &his_pieces_used, white_man);
his_min_attack_steps = calc_dist_3( (FLD37), (FLD42), (FLD47|FLD48),  &his_pieces_used, white_man);
if ((all_empty_or_black((FLD33|FLD22)))  || (all_occupied((FLD33|FLD22)))) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_5( (FLD23), (FLD18|FLD19), (FLD12|FLD13|FLD14), (FLD7|FLD8|FLD9|FLD10), (FLD1|FLD2|FLD3|FLD4|FLD5),  &my_pieces_used, black_man) + 1;
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if (empty_or_black & FLD33) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_6( (FLD28), (FLD22|FLD23), (FLD17|FLD18|FLD19), (FLD11|FLD12|FLD13|FLD14), (FLD6|FLD7|FLD8|FLD9|FLD10), (FLD1|FLD2|FLD3|FLD4|FLD5),  &my_pieces_used, black_man);
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if ((all_black((FLD27|FLD21))) && (all_empty(( FLD38|FLD43))))  return -200; // vlucht
if ((occupied & FLD33) && (all_empty(( FLD38|FLD43))))  return -200; // vlucht
if ((all_black((FLD27|FLD21))) && (white_man & FLD43) && (occupied & FLD33) && (empty_field & FLD38)) return 0; //paddestoel
if ((all_black((FLD27|FLD21))) && (white_man & FLD43) && (empty_or_black &FLD33) && (empty_field & FLD38)) return 0; //kan wegruilen
return -700;
}

//calc the minimal amount of steps for having the backing piece in place as well as the attack resp.defence piece
his_pieces_used = 0;
his_min_backing_steps = calc_dist_2( (FLD41), (FLD46|FLD47),  &his_pieces_used, white_man);
his_min_attack_steps = calc_dist_3( (FLD37), (FLD42), (FLD47|FLD48),  &his_pieces_used, white_man);
my_pieces_used = 0;
my_min_backing_steps = calc_dist_4( (FLD19), (FLD13|FLD14), (FLD8|FLD9|FLD10), (FLD2|FLD3|FLD4|FLD5),  &my_pieces_used, black_man);
my_min_defence_steps = calc_dist_5( (FLD23), (FLD18), (FLD12|FLD13), (FLD7|FLD8|FLD9), (FLD1|FLD2|FLD3|FLD4),  &my_pieces_used, black_man);

// we have calculated: his_min_backing_steps, his_min_attack_steps, my_min_defence_steps and my_min_backing steps

if ((his_min_backing_steps + his_min_attack_steps) - (my_min_backing_steps + my_min_defence_steps + inc) < -2) {
if ((black_man & FLD33) && (all_white((FLD38|FLD42))) && ( all_empty((FLD37|FLD31)))) return 0;
his_pieces_used = 0;   // calc the minimal amount of steps for having the backing piece in place as well as the attack resp.defence piece
his_min_backing_steps = calc_dist_2( (FLD41), (FLD46|FLD47),  &his_pieces_used, white_man);
his_min_attack_steps = calc_dist_3( (FLD37), (FLD42), (FLD47|FLD48),  &his_pieces_used, white_man);
if ((all_empty_or_black((FLD33|FLD22)))  || (all_occupied((FLD33|FLD22)))) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_5( (FLD23), (FLD18|FLD19), (FLD12|FLD13|FLD14), (FLD7|FLD8|FLD9|FLD10), (FLD1|FLD2|FLD3|FLD4|FLD5),  &my_pieces_used, black_man) + 1;
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if (empty_or_black & FLD33) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_6( (FLD28), (FLD22|FLD23), (FLD17|FLD18|FLD19), (FLD11|FLD12|FLD13|FLD14), (FLD6|FLD7|FLD8|FLD9|FLD10), (FLD1|FLD2|FLD3|FLD4|FLD5),  &my_pieces_used, black_man);
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if ((all_black((FLD27|FLD21))) && (all_empty(( FLD38|FLD43))))  return -200; // vlucht
if ((occupied & FLD33) && (all_empty(( FLD38|FLD43))))  return -200; // vlucht
if ((all_black((FLD27|FLD21))) && (white_man & FLD43) && (occupied & FLD33) && (empty_field & FLD38)) return 0; //paddestoel
if ((all_black((FLD27|FLD21))) && (white_man & FLD43) && (empty_or_black &FLD33) && (empty_field & FLD38)) return 0; //kan wegruilen
return -700;
}

// and a second attack??

if (number_of_attacks > 1) {
his_min_attack_steps = calc_dist_3( (FLD37), (FLD42), (FLD47|FLD48),  &his_pieces_used, white_man);
my_min_defence_steps = calc_dist_5( (FLD23), (FLD18), (FLD12|FLD13), (FLD7|FLD8|FLD9), (FLD1|FLD2|FLD3|FLD4),  &my_pieces_used, black_man);
if ((his_min_attack_steps) - (my_min_defence_steps + inc) < -2) { // too late for the second attempt
if ((black_man & FLD33) && (all_white((FLD38|FLD42))) && ( all_empty((FLD37|FLD31)))) return 0;
his_pieces_used = 0;   // calc the minimal amount of steps for having the backing piece in place as well as the attack resp.defence piece
his_min_backing_steps = calc_dist_2( (FLD41), (FLD46|FLD47),  &his_pieces_used, white_man);
his_min_attack_steps = calc_dist_3( (FLD37), (FLD42), (FLD47|FLD48),  &his_pieces_used, white_man);
if ((all_empty_or_black((FLD33|FLD22)))  || (all_occupied((FLD33|FLD22)))) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_5( (FLD23), (FLD18|FLD19), (FLD12|FLD13|FLD14), (FLD7|FLD8|FLD9|FLD10), (FLD1|FLD2|FLD3|FLD4|FLD5),  &my_pieces_used, black_man) + 1;
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if (empty_or_black & FLD33) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_6( (FLD28), (FLD22|FLD23), (FLD17|FLD18|FLD19), (FLD11|FLD12|FLD13|FLD14), (FLD6|FLD7|FLD8|FLD9|FLD10), (FLD1|FLD2|FLD3|FLD4|FLD5),  &my_pieces_used, black_man);
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if ((all_black((FLD27|FLD21))) && (all_empty(( FLD38|FLD43))))  return -200; // vlucht
if ((occupied & FLD33) && (all_empty(( FLD38|FLD43))))  return -200; // vlucht
if ((all_black((FLD27|FLD21))) && (white_man & FLD43) && (occupied & FLD33) && (empty_field & FLD38)) return 0; //paddestoel
if ((all_black((FLD27|FLD21))) && (white_man & FLD43) && (empty_or_black &FLD33) && (empty_field & FLD38)) return 0; //kan wegruilen
return -400;
}
}
check_afruil:
if (all_white((FLD46|FLD41))) return 0; // voorpost wordt afgeruild
if (all_white((FLD49|FLD43))) return 0; // voorpost wordt afgeruild
// so we have hope to have a nice voorpost
return 180;  // no problems, so a small advantage
}
return 0;
}
extern int voorpost_black_ne_33(BitArray, BitArray, BitArray, int);
int voorpost_black_ne_33(BitArray white_man, BitArray black_man, BitArray empty_field, int inc) {
BitArray my_pieces_used, his_pieces_used;
int number_of_defences, number_of_attacks = 0;
int his_min_backing_steps, my_min_backing_steps;
int my_min_defence_steps, his_min_attack_steps;
bool backing_defence_piece, backing_attack_piece;

if ((black_man & FLD33) &&  (all_empty_or_white((FLD38|FLD42))) &&  (all_empty_or_black((FLD24|FLD20))) &&  (empty_field & FLD29)) {
//so this is what anyway has to be there to have a voorpost

//if the field sw of b is occupied by me or can be occupied by me, no attack is possible, so the question is, can the piece on sw(b) be exchanged
if ((black_man & FLD32) ||  ((black_man & FLD27) && ((all_empty_or_black(FLD37|FLD28)) ||  (all_occupied(FLD37|FLD28))))) {
if ((all_white((FLD43|FLD49|FLD0)) && (empty_field & FLD38)) &&
    !(((white_man & FLD39) && (empty_field & FLD34)) ||  ((white_man & FLD48) && (empty_field & FLD0))))
number_of_attacks = -1;  // can be exchanged,  start with one attack less
else goto check_afruil; // no attacks possible
}

//same for the defence: if ne(d) is occupied by him, we may have a problem in the defence
if (white_man & FLD30) {
if ((all_black((FLD19|FLD15|FLD8)) && (empty_field & FLD24)) ||
    (all_black((FLD24|FLD13|FLD8)) && (empty_field & FLD19)))
number_of_defences = count(((FLD2|FLD3|FLD4|FLD5|FLD8|FLD9|FLD10|FLD13|FLD14|FLD19|FLD24) & black_man)) - 1;
else number_of_defences = 0;
} else number_of_defences = count(((FLD2|FLD3|FLD4|FLD5|FLD8|FLD9|FLD10|FLD13|FLD14|FLD19|FLD24) & black_man));

//check the availability of the backing pieces
backing_defence_piece = bool ((FLD3|FLD4|FLD5|FLD9|FLD10|FLD14|FLD15|FLD20) & black_man);
backing_attack_piece = (bool) ((FLD42|FLD47|FLD48) & white_man);
number_of_attacks += count(((FLD38|FLD43|FLD48|FLD49) & white_man));
if (!((FLD42|FLD47) & white_man)) number_of_attacks -= 1;
if (!((FLD15|FLD20) & black_man)) number_of_defences -= 1;

//so now we know whether we have a backing piece and how many pieces for the attack/defence
if (number_of_attacks <= 0) return 180;
if (!(backing_defence_piece) && (backing_attack_piece) && (number_of_attacks > 0)) { // looks bad, but maybe an escape?
if ((black_man & FLD34) && (all_white((FLD39|FLD43))) && ( all_empty((FLD38|FLD32)))) return 0;
his_pieces_used = 0;   // calc the minimal amount of steps for having the backing piece in place as well as the attack resp.defence piece
his_min_backing_steps = calc_dist_2( (FLD42), (FLD47|FLD48),  &his_pieces_used, white_man);
his_min_attack_steps = calc_dist_3( (FLD38), (FLD43), (FLD48|FLD49),  &his_pieces_used, white_man);
if ((all_empty_or_black((FLD34|FLD23)))  || (all_occupied((FLD34|FLD23)))) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_5( (FLD24), (FLD19|FLD20), (FLD13|FLD14|FLD15), (FLD8|FLD9|FLD10), (FLD2|FLD3|FLD4|FLD5),  &my_pieces_used, black_man) + 1;
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if (empty_or_black & FLD34) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_6( (FLD29), (FLD23|FLD24), (FLD18|FLD19|FLD20), (FLD12|FLD13|FLD14|FLD15), (FLD7|FLD8|FLD9|FLD10), (FLD1|FLD2|FLD3|FLD4|FLD5),  &my_pieces_used, black_man);
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if ((all_black((FLD28|FLD22))) && (all_empty(( FLD39|FLD44))))  return -200; // vlucht
if ((occupied & FLD34) && (all_empty(( FLD39|FLD44))))  return -200; // vlucht
if ((all_black((FLD28|FLD22))) && (white_man & FLD44) && (occupied & FLD34) && (empty_field & FLD39)) return 0; //paddestoel
if ((all_black((FLD28|FLD22))) && (white_man & FLD44) && (empty_or_black &FLD34) && (empty_field & FLD39)) return 0; //kan wegruilen
 return -700;
}

if (!(backing_attack_piece)) goto check_afruil;
// both backings are available
if (number_of_attacks > number_of_defences) {  // it looks bad, but maybe an escape?
if ((black_man & FLD34) && (all_white((FLD39|FLD43))) && ( all_empty((FLD38|FLD32)))) return 0;
his_pieces_used = 0;   // calc the minimal amount of steps for having the backing piece in place as well as the attack resp.defence piece
his_min_backing_steps = calc_dist_2( (FLD42), (FLD47|FLD48),  &his_pieces_used, white_man);
his_min_attack_steps = calc_dist_3( (FLD38), (FLD43), (FLD48|FLD49),  &his_pieces_used, white_man);
if ((all_empty_or_black((FLD34|FLD23)))  || (all_occupied((FLD34|FLD23)))) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_5( (FLD24), (FLD19|FLD20), (FLD13|FLD14|FLD15), (FLD8|FLD9|FLD10), (FLD2|FLD3|FLD4|FLD5),  &my_pieces_used, black_man) + 1;
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if (empty_or_black & FLD34) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_6( (FLD29), (FLD23|FLD24), (FLD18|FLD19|FLD20), (FLD12|FLD13|FLD14|FLD15), (FLD7|FLD8|FLD9|FLD10), (FLD1|FLD2|FLD3|FLD4|FLD5),  &my_pieces_used, black_man);
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if ((all_black((FLD28|FLD22))) && (all_empty(( FLD39|FLD44))))  return -200; // vlucht
if ((occupied & FLD34) && (all_empty(( FLD39|FLD44))))  return -200; // vlucht
if ((all_black((FLD28|FLD22))) && (white_man & FLD44) && (occupied & FLD34) && (empty_field & FLD39)) return 0; //paddestoel
if ((all_black((FLD28|FLD22))) && (white_man & FLD44) && (empty_or_black &FLD34) && (empty_field & FLD39)) return 0; //kan wegruilen
return -700;
}

//calc the minimal amount of steps for having the backing piece in place as well as the attack resp.defence piece
his_pieces_used = 0;
his_min_backing_steps = calc_dist_2( (FLD42), (FLD47|FLD48),  &his_pieces_used, white_man);
his_min_attack_steps = calc_dist_3( (FLD38), (FLD43), (FLD48|FLD49),  &his_pieces_used, white_man);
my_pieces_used = 0;
my_min_backing_steps = calc_dist_4( (FLD20), (FLD14|FLD15), (FLD9|FLD10), (FLD3|FLD4|FLD5),  &my_pieces_used, black_man);
my_min_defence_steps = calc_dist_5( (FLD24), (FLD19), (FLD13|FLD14), (FLD8|FLD9|FLD10), (FLD2|FLD3|FLD4|FLD5),  &my_pieces_used, black_man);

// we have calculated: his_min_backing_steps, his_min_attack_steps, my_min_defence_steps and my_min_backing steps

if ((his_min_backing_steps + his_min_attack_steps) - (my_min_backing_steps + my_min_defence_steps + inc) < -2) {
if ((black_man & FLD34) && (all_white((FLD39|FLD43))) && ( all_empty((FLD38|FLD32)))) return 0;
his_pieces_used = 0;   // calc the minimal amount of steps for having the backing piece in place as well as the attack resp.defence piece
his_min_backing_steps = calc_dist_2( (FLD42), (FLD47|FLD48),  &his_pieces_used, white_man);
his_min_attack_steps = calc_dist_3( (FLD38), (FLD43), (FLD48|FLD49),  &his_pieces_used, white_man);
if ((all_empty_or_black((FLD34|FLD23)))  || (all_occupied((FLD34|FLD23)))) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_5( (FLD24), (FLD19|FLD20), (FLD13|FLD14|FLD15), (FLD8|FLD9|FLD10), (FLD2|FLD3|FLD4|FLD5),  &my_pieces_used, black_man) + 1;
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if (empty_or_black & FLD34) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_6( (FLD29), (FLD23|FLD24), (FLD18|FLD19|FLD20), (FLD12|FLD13|FLD14|FLD15), (FLD7|FLD8|FLD9|FLD10), (FLD1|FLD2|FLD3|FLD4|FLD5),  &my_pieces_used, black_man);
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if ((all_black((FLD28|FLD22))) && (all_empty(( FLD39|FLD44))))  return -200; // vlucht
if ((occupied & FLD34) && (all_empty(( FLD39|FLD44))))  return -200; // vlucht
if ((all_black((FLD28|FLD22))) && (white_man & FLD44) && (occupied & FLD34) && (empty_field & FLD39)) return 0; //paddestoel
if ((all_black((FLD28|FLD22))) && (white_man & FLD44) && (empty_or_black &FLD34) && (empty_field & FLD39)) return 0; //kan wegruilen
return -700;
}

// and a second attack??

if (number_of_attacks > 1) {
his_min_attack_steps = calc_dist_3( (FLD38), (FLD43), (FLD48|FLD49),  &his_pieces_used, white_man);
my_min_defence_steps = calc_dist_5( (FLD24), (FLD19), (FLD13|FLD14), (FLD8|FLD9|FLD10), (FLD2|FLD3|FLD4|FLD5),  &my_pieces_used, black_man);
if ((his_min_attack_steps) - (my_min_defence_steps + inc) < -2) { // too late for the second attempt
if ((black_man & FLD34) && (all_white((FLD39|FLD43))) && ( all_empty((FLD38|FLD32)))) return 0;
his_pieces_used = 0;   // calc the minimal amount of steps for having the backing piece in place as well as the attack resp.defence piece
his_min_backing_steps = calc_dist_2( (FLD42), (FLD47|FLD48),  &his_pieces_used, white_man);
his_min_attack_steps = calc_dist_3( (FLD38), (FLD43), (FLD48|FLD49),  &his_pieces_used, white_man);
if ((all_empty_or_black((FLD34|FLD23)))  || (all_occupied((FLD34|FLD23)))) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_5( (FLD24), (FLD19|FLD20), (FLD13|FLD14|FLD15), (FLD8|FLD9|FLD10), (FLD2|FLD3|FLD4|FLD5),  &my_pieces_used, black_man) + 1;
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if (empty_or_black & FLD34) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_6( (FLD29), (FLD23|FLD24), (FLD18|FLD19|FLD20), (FLD12|FLD13|FLD14|FLD15), (FLD7|FLD8|FLD9|FLD10), (FLD1|FLD2|FLD3|FLD4|FLD5),  &my_pieces_used, black_man);
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if ((all_black((FLD28|FLD22))) && (all_empty(( FLD39|FLD44))))  return -200; // vlucht
if ((occupied & FLD34) && (all_empty(( FLD39|FLD44))))  return -200; // vlucht
if ((all_black((FLD28|FLD22))) && (white_man & FLD44) && (occupied & FLD34) && (empty_field & FLD39)) return 0; //paddestoel
if ((all_black((FLD28|FLD22))) && (white_man & FLD44) && (empty_or_black &FLD34) && (empty_field & FLD39)) return 0; //kan wegruilen
return -400;
}
}
check_afruil:
if (all_white((FLD47|FLD42))) return 0; // voorpost wordt afgeruild
if (all_white((FLD50|FLD44))) return 0; // voorpost wordt afgeruild
// so we have hope to have a nice voorpost
if (((all_white((FLD34|FLD40|FLD45)) && (all_empty((FLD30|FLD24)))) ||  (all_white((FLD35|FLD40|FLD45)) && (all_empty((FLD34|FLD30|FLD24))))) &&  (black_man & FLD25)) return -700; // een om twee mogelijk
return 180;  // no problems, so a small advantage
}
return 0;
}
extern int voorpost_black_ne_34(BitArray, BitArray, BitArray, int);
int voorpost_black_ne_34(BitArray white_man, BitArray black_man, BitArray empty_field, int inc) {
BitArray my_pieces_used, his_pieces_used;
int number_of_defences, number_of_attacks = 0;
int his_min_backing_steps, my_min_backing_steps;
int my_min_defence_steps, his_min_attack_steps;
bool backing_defence_piece, backing_attack_piece;

if ((black_man & FLD34) &&  (all_empty_or_white((FLD39|FLD43))) &&  (empty_or_black & FLD25) &&  (empty_field & FLD30)) {
//so this is what anyway has to be there to have a voorpost

//if the field sw of b is occupied by me or can be occupied by me, no attack is possible, so the question is, can the piece on sw(b) be exchanged
if ((black_man & FLD33) ||  ((black_man & FLD28) && ((all_empty_or_black(FLD38|FLD29)) ||  (all_occupied(FLD38|FLD29))))) {
if ((all_white((FLD44|FLD50|FLD0)) && (empty_field & FLD39)) &&
    !(((white_man & FLD40) && (empty_field & FLD35)) ||  ((white_man & FLD49) && (empty_field & FLD0))))
number_of_attacks = -1;  // can be exchanged,  start with one attack less
else goto check_afruil; // no attacks possible
}

number_of_defences = count(((FLD3|FLD4|FLD5|FLD9|FLD10|FLD14|FLD15|FLD20|FLD25) & black_man));

//check the availability of the backing pieces
backing_defence_piece = true; // no backing defence piece (edge)
backing_attack_piece = (bool) ((FLD43|FLD48|FLD49) & white_man);
number_of_attacks += count(((FLD39|FLD44|FLD49|FLD50) & white_man));
if (!((FLD43|FLD48) & white_man)) number_of_attacks -= 1;

//so now we know whether we have a backing piece and how many pieces for the attack/defence
if (number_of_attacks <= 0) return 180;
if (!(backing_attack_piece)) goto check_afruil;
// both backings are available
if (number_of_attacks > number_of_defences) {  // it looks bad, but maybe an escape?
if (all_white((FLD45|FLD40|FLD35))) return -700; // vleugelopsluiting
if ((black_man & FLD35) && (all_white((FLD40|FLD44))) && ( all_empty((FLD39|FLD33)))) return 0;
his_pieces_used = 0;   // calc the minimal amount of steps for having the backing piece in place as well as the attack resp.defence piece
his_min_backing_steps = calc_dist_2( (FLD43), (FLD48|FLD49),  &his_pieces_used, white_man);
his_min_attack_steps = calc_dist_3( (FLD39), (FLD44), (FLD49|FLD50),  &his_pieces_used, white_man);
if ((all_empty_or_black((FLD35|FLD24)))  || (all_occupied((FLD35|FLD24)))) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_5( (FLD25), (FLD20), (FLD14|FLD15), (FLD9|FLD10), (FLD3|FLD4|FLD5),  &my_pieces_used, black_man) + 1;
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if (empty_or_black & FLD35) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_6( (FLD30), (FLD24|FLD25), (FLD19|FLD20), (FLD13|FLD14|FLD15), (FLD8|FLD9|FLD10), (FLD2|FLD3|FLD4|FLD5),  &my_pieces_used, black_man);
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if ((all_black((FLD29|FLD23))) && (all_empty(( FLD40|FLD45))))  return 50; // vlucht naar rand
if ((occupied & FLD35) && (all_empty(( FLD40|FLD45))))  return 50; // vlucht naar rand
if ((all_black((FLD29|FLD23))) && (white_man & FLD45) && (occupied & FLD35) && (empty_field & FLD40)) return 0; //paddestoel
if ((all_black((FLD29|FLD23))) && (white_man & FLD45) && (empty_or_black &FLD35) && (empty_field & FLD40)) return 0; //kan wegruilen
return -700;
}

//calc the minimal amount of steps for having the backing piece in place as well as the attack resp.defence piece
his_pieces_used = 0;
his_min_backing_steps = calc_dist_2( (FLD43), (FLD48|FLD49),  &his_pieces_used, white_man);
his_min_attack_steps = calc_dist_3( (FLD39), (FLD44), (FLD49|FLD50),  &his_pieces_used, white_man);
my_pieces_used = 0;
my_min_backing_steps = 0;
my_min_defence_steps = calc_dist_5( (FLD25), (FLD20), (FLD14|FLD15), (FLD9|FLD10), (FLD3|FLD4|FLD5),  &my_pieces_used, black_man);

// we have calculated: his_min_backing_steps, his_min_attack_steps, my_min_defence_steps and my_min_backing steps

if ((his_min_backing_steps + his_min_attack_steps) - (my_min_backing_steps + my_min_defence_steps + inc) < -2) {
if (all_white((FLD45|FLD40|FLD35))) return -700; // vleugelopsluiting
if ((black_man & FLD35) && (all_white((FLD40|FLD44))) && ( all_empty((FLD39|FLD33)))) return 0;
his_pieces_used = 0;   // calc the minimal amount of steps for having the backing piece in place as well as the attack resp.defence piece
his_min_backing_steps = calc_dist_2( (FLD43), (FLD48|FLD49),  &his_pieces_used, white_man);
his_min_attack_steps = calc_dist_3( (FLD39), (FLD44), (FLD49|FLD50),  &his_pieces_used, white_man);
if ((all_empty_or_black((FLD35|FLD24)))  || (all_occupied((FLD35|FLD24)))) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_5( (FLD25), (FLD20), (FLD14|FLD15), (FLD9|FLD10), (FLD3|FLD4|FLD5),  &my_pieces_used, black_man) + 1;
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if (empty_or_black & FLD35) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_6( (FLD30), (FLD24|FLD25), (FLD19|FLD20), (FLD13|FLD14|FLD15), (FLD8|FLD9|FLD10), (FLD2|FLD3|FLD4|FLD5),  &my_pieces_used, black_man);
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if ((all_black((FLD29|FLD23))) && (all_empty(( FLD40|FLD45))))  return 50; // vlucht naar rand
if ((occupied & FLD35) && (all_empty(( FLD40|FLD45))))  return 50; // vlucht naar rand
if ((all_black((FLD29|FLD23))) && (white_man & FLD45) && (occupied & FLD35) && (empty_field & FLD40)) return 0; //paddestoel
if ((all_black((FLD29|FLD23))) && (white_man & FLD45) && (empty_or_black &FLD35) && (empty_field & FLD40)) return 0; //kan wegruilen
return -700;
}

// and a second attack??

if (number_of_attacks > 1) {
his_min_attack_steps = calc_dist_3( (FLD39), (FLD44), (FLD49|FLD50),  &his_pieces_used, white_man);
my_min_defence_steps = calc_dist_5( (FLD25), (FLD20), (FLD14|FLD15), (FLD9|FLD10), (FLD3|FLD4|FLD5),  &my_pieces_used, black_man);
if ((his_min_attack_steps) - (my_min_defence_steps + inc) < -2) { // too late for the second attempt
if (all_white((FLD45|FLD40|FLD35))) return -700; // vleugelopsluiting
if ((black_man & FLD35) && (all_white((FLD40|FLD44))) && ( all_empty((FLD39|FLD33)))) return 0;
his_pieces_used = 0;   // calc the minimal amount of steps for having the backing piece in place as well as the attack resp.defence piece
his_min_backing_steps = calc_dist_2( (FLD43), (FLD48|FLD49),  &his_pieces_used, white_man);
his_min_attack_steps = calc_dist_3( (FLD39), (FLD44), (FLD49|FLD50),  &his_pieces_used, white_man);
if ((all_empty_or_black((FLD35|FLD24)))  || (all_occupied((FLD35|FLD24)))) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_5( (FLD25), (FLD20), (FLD14|FLD15), (FLD9|FLD10), (FLD3|FLD4|FLD5),  &my_pieces_used, black_man) + 1;
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if (empty_or_black & FLD35) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_6( (FLD30), (FLD24|FLD25), (FLD19|FLD20), (FLD13|FLD14|FLD15), (FLD8|FLD9|FLD10), (FLD2|FLD3|FLD4|FLD5),  &my_pieces_used, black_man);
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if ((all_black((FLD29|FLD23))) && (all_empty(( FLD40|FLD45))))  return 50; // vlucht naar rand
if ((occupied & FLD35) && (all_empty(( FLD40|FLD45))))  return 50; // vlucht naar rand
if ((all_black((FLD29|FLD23))) && (white_man & FLD45) && (occupied & FLD35) && (empty_field & FLD40)) return 0; //paddestoel
if ((all_black((FLD29|FLD23))) && (white_man & FLD45) && (empty_or_black &FLD35) && (empty_field & FLD40)) return 0; //kan wegruilen
return -400;
}
}
check_afruil:
if (all_white((FLD48|FLD43))) return 0; // voorpost wordt afgeruild
// so we have hope to have a nice voorpost
return 180;  // no problems, so a small advantage
}
return 0;
}
// zwarte voorposten nw
extern int voorpost_black_nw_27(BitArray, BitArray, BitArray, int);
int voorpost_black_nw_27(BitArray white_man, BitArray black_man, BitArray empty_field, int inc) {
BitArray my_pieces_used, his_pieces_used;
int number_of_defences, number_of_attacks = 0;
int his_min_backing_steps, my_min_backing_steps;
int my_min_defence_steps, his_min_attack_steps;
bool backing_defence_piece, backing_attack_piece;

if ((black_man & FLD27) &&  (all_empty_or_white((FLD32|FLD38))) &&  (empty_or_black & FLD16) &&  (empty_field & FLD21)) {
//so this is what anyway has to be there to have a voorpost

//if the field sw of b is occupied by me or can be occupied by me, no attack is possible, so the question is, can the piece on sw(b) be exchanged
if ((black_man & FLD28) ||  ((black_man & FLD23) && ((all_empty_or_black(FLD33|FLD22)) ||  (all_occupied(FLD33|FLD22))))) {
if ((all_white((FLD37|FLD41|FLD46)) && (empty_field & FLD32)) &&
    !(((white_man & FLD31) && (empty_field & FLD26)) ||  ((white_man & FLD42) && (empty_field & FLD48))))
number_of_attacks = -1;  // can be exchanged,  start with one attack less
else goto check_afruil; // no attacks possible
}

number_of_defences = count(((FLD1|FLD2|FLD6|FLD7|FLD11|FLD16) & black_man));

//check the availability of the backing pieces
backing_defence_piece = true; // no backing defence piece (edge)
backing_attack_piece = (bool) ((FLD38|FLD42|FLD43|FLD47|FLD48|FLD49) & white_man);
number_of_attacks += count(((FLD32|FLD37|FLD41|FLD42|FLD46|FLD47|FLD48) & white_man));
if (!((FLD38|FLD43|FLD49) & white_man)) number_of_attacks -= 1;

//so now we know whether we have a backing piece and how many pieces for the attack/defence
if (number_of_attacks <= 0) return 180;
if (!(backing_attack_piece)) goto check_afruil;
// both backings are available
if (number_of_attacks > number_of_defences) {  // it looks bad, but maybe an escape?
if (all_white((FLD36|FLD31|FLD26))) return -700; // vleugelopsluiting
if ((black_man & FLD26) && (all_white((FLD31|FLD37))) && ( all_empty((FLD32|FLD28)))) return 0;
his_pieces_used = 0;   // calc the minimal amount of steps for having the backing piece in place as well as the attack resp.defence piece
his_min_backing_steps = calc_dist_3( (FLD38), (FLD42|FLD43), (FLD47|FLD48|FLD49),  &his_pieces_used, white_man);
his_min_attack_steps = calc_dist_4( (FLD32), (FLD37), (FLD41|FLD42), (FLD46|FLD47|FLD48),  &his_pieces_used, white_man);
if ((all_empty_or_black((FLD26|FLD17)))  || (all_occupied((FLD26|FLD17)))) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_4( (FLD16), (FLD11), (FLD6|FLD7), (FLD1|FLD2),  &my_pieces_used, black_man) + 1;
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if (empty_or_black & FLD26) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_5( (FLD21), (FLD16|FLD17), (FLD11|FLD12), (FLD6|FLD7|FLD8), (FLD1|FLD2|FLD3),  &my_pieces_used, black_man);
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if ((all_black((FLD22|FLD18))) && (all_empty(( FLD31|FLD36))))  return 50; // vlucht naar rand
if ((occupied & FLD26) && (all_empty(( FLD31|FLD36))))  return 50; // vlucht naar rand
if ((all_black((FLD22|FLD18))) && (white_man & FLD36) && (occupied & FLD26) && (empty_field & FLD31)) return 0; //paddestoel
if ((all_black((FLD22|FLD18))) && (white_man & FLD36) && (empty_or_black &FLD26) && (empty_field & FLD31)) return 0; //kan wegruilen
return -700;
}

//calc the minimal amount of steps for having the backing piece in place as well as the attack resp.defence piece
his_pieces_used = 0;
his_min_backing_steps = calc_dist_3( (FLD38), (FLD42|FLD43), (FLD47|FLD48|FLD49),  &his_pieces_used, white_man);
his_min_attack_steps = calc_dist_4( (FLD32), (FLD37), (FLD41|FLD42), (FLD46|FLD47|FLD48),  &his_pieces_used, white_man);
my_pieces_used = 0;
my_min_backing_steps = 0;
my_min_defence_steps = calc_dist_4( (FLD16), (FLD11), (FLD6|FLD7), (FLD1|FLD2),  &my_pieces_used, black_man);

// we have calculated: his_min_backing_steps, his_min_attack_steps, my_min_defence_steps and my_min_backing steps

if ((his_min_backing_steps + his_min_attack_steps) - (my_min_backing_steps + my_min_defence_steps + inc) < -2) {
if (all_white((FLD36|FLD31|FLD26))) return -700; // vleugelopsluiting
if ((black_man & FLD26) && (all_white((FLD31|FLD37))) && ( all_empty((FLD32|FLD28)))) return 0;
his_pieces_used = 0;   // calc the minimal amount of steps for having the backing piece in place as well as the attack resp.defence piece
his_min_backing_steps = calc_dist_3( (FLD38), (FLD42|FLD43), (FLD47|FLD48|FLD49),  &his_pieces_used, white_man);
his_min_attack_steps = calc_dist_4( (FLD32), (FLD37), (FLD41|FLD42), (FLD46|FLD47|FLD48),  &his_pieces_used, white_man);
if ((all_empty_or_black((FLD26|FLD17)))  || (all_occupied((FLD26|FLD17)))) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_4( (FLD16), (FLD11), (FLD6|FLD7), (FLD1|FLD2),  &my_pieces_used, black_man) + 1;
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if (empty_or_black & FLD26) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_5( (FLD21), (FLD16|FLD17), (FLD11|FLD12), (FLD6|FLD7|FLD8), (FLD1|FLD2|FLD3),  &my_pieces_used, black_man);
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if ((all_black((FLD22|FLD18))) && (all_empty(( FLD31|FLD36))))  return 50; // vlucht naar rand
if ((occupied & FLD26) && (all_empty(( FLD31|FLD36))))  return 50; // vlucht naar rand
if ((all_black((FLD22|FLD18))) && (white_man & FLD36) && (occupied & FLD26) && (empty_field & FLD31)) return 0; //paddestoel
if ((all_black((FLD22|FLD18))) && (white_man & FLD36) && (empty_or_black &FLD26) && (empty_field & FLD31)) return 0; //kan wegruilen
return -700;
}

// and a second attack??

if (number_of_attacks > 1) {
his_min_attack_steps = calc_dist_4( (FLD32), (FLD37), (FLD41|FLD42), (FLD46|FLD47|FLD48),  &his_pieces_used, white_man);
my_min_defence_steps = calc_dist_4( (FLD16), (FLD11), (FLD6|FLD7), (FLD1|FLD2),  &my_pieces_used, black_man);
if ((his_min_attack_steps) - (my_min_defence_steps + inc) < -2) { // too late for the second attempt
if (all_white((FLD36|FLD31|FLD26))) return -700; // vleugelopsluiting
if ((black_man & FLD26) && (all_white((FLD31|FLD37))) && ( all_empty((FLD32|FLD28)))) return 0;
his_pieces_used = 0;   // calc the minimal amount of steps for having the backing piece in place as well as the attack resp.defence piece
his_min_backing_steps = calc_dist_3( (FLD38), (FLD42|FLD43), (FLD47|FLD48|FLD49),  &his_pieces_used, white_man);
his_min_attack_steps = calc_dist_4( (FLD32), (FLD37), (FLD41|FLD42), (FLD46|FLD47|FLD48),  &his_pieces_used, white_man);
if ((all_empty_or_black((FLD26|FLD17)))  || (all_occupied((FLD26|FLD17)))) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_4( (FLD16), (FLD11), (FLD6|FLD7), (FLD1|FLD2),  &my_pieces_used, black_man) + 1;
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if (empty_or_black & FLD26) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_5( (FLD21), (FLD16|FLD17), (FLD11|FLD12), (FLD6|FLD7|FLD8), (FLD1|FLD2|FLD3),  &my_pieces_used, black_man);
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if ((all_black((FLD22|FLD18))) && (all_empty(( FLD31|FLD36))))  return 50; // vlucht naar rand
if ((occupied & FLD26) && (all_empty(( FLD31|FLD36))))  return 50; // vlucht naar rand
if ((all_black((FLD22|FLD18))) && (white_man & FLD36) && (occupied & FLD26) && (empty_field & FLD31)) return 0; //paddestoel
if ((all_black((FLD22|FLD18))) && (white_man & FLD36) && (empty_or_black &FLD26) && (empty_field & FLD31)) return 0; //kan wegruilen
return -400;
}
}
check_afruil:
if (all_white((FLD49|FLD43|FLD38))) return 0; // voorpost wordt afgeruild
// so we have hope to have a nice voorpost
return 180;  // no problems, so a small advantage
}
return 0;
}
extern int voorpost_black_nw_28(BitArray, BitArray, BitArray, int);
int voorpost_black_nw_28(BitArray white_man, BitArray black_man, BitArray empty_field, int inc) {
BitArray my_pieces_used, his_pieces_used;
int number_of_defences, number_of_attacks = 0;
int his_min_backing_steps, my_min_backing_steps;
int my_min_defence_steps, his_min_attack_steps;
bool backing_defence_piece, backing_attack_piece;

if ((black_man & FLD28) &&  (all_empty_or_white((FLD33|FLD39))) &&  (all_empty_or_black((FLD17|FLD11))) &&  (empty_field & FLD22)) {
//so this is what anyway has to be there to have a voorpost

//if the field sw of b is occupied by me or can be occupied by me, no attack is possible, so the question is, can the piece on sw(b) be exchanged
if ((black_man & FLD29) ||  ((black_man & FLD24) && ((all_empty_or_black(FLD34|FLD23)) ||  (all_occupied(FLD34|FLD23))))) {
if ((all_white((FLD38|FLD42|FLD47)) && (empty_field & FLD33)) &&
    !(((white_man & FLD32) && (empty_field & FLD27)) ||  ((white_man & FLD43) && (empty_field & FLD49))))
number_of_attacks = -1;  // can be exchanged,  start with one attack less
else goto check_afruil; // no attacks possible
}

//same for the defence: if ne(d) is occupied by him, we may have a problem in the defence
if (white_man & FLD21) {
if ((all_black((FLD12|FLD6|FLD3)) && (empty_field & FLD17)) ||
    (all_black((FLD17|FLD8|FLD3)) && (empty_field & FLD12)))
number_of_defences = count(((FLD1|FLD2|FLD3|FLD7|FLD8|FLD12|FLD17) & black_man)) - 1;
else number_of_defences = 0;
} else number_of_defences = count(((FLD1|FLD2|FLD3|FLD7|FLD8|FLD12|FLD17) & black_man));

//check the availability of the backing pieces
backing_defence_piece = bool ((FLD1|FLD2|FLD6|FLD7|FLD11) & black_man);
backing_attack_piece = (bool) ((FLD39|FLD43|FLD44|FLD48|FLD49|FLD50) & white_man);
number_of_attacks += count(((FLD33|FLD38|FLD42|FLD43|FLD47|FLD48|FLD49) & white_man));
if (!((FLD39|FLD44|FLD50) & white_man)) number_of_attacks -= 1;
if (!((FLD6|FLD11) & black_man)) number_of_defences -= 1;

//so now we know whether we have a backing piece and how many pieces for the attack/defence
if (number_of_attacks <= 0) return 180;
if (!(backing_defence_piece) && (backing_attack_piece) && (number_of_attacks > 0)) { // looks bad, but maybe an escape?
if ((black_man & FLD27) && (all_white((FLD32|FLD38))) && ( all_empty((FLD33|FLD29)))) return 0;
his_pieces_used = 0;   // calc the minimal amount of steps for having the backing piece in place as well as the attack resp.defence piece
his_min_backing_steps = calc_dist_3( (FLD39), (FLD43|FLD44), (FLD48|FLD49|FLD50),  &his_pieces_used, white_man);
his_min_attack_steps = calc_dist_4( (FLD33), (FLD38), (FLD42|FLD43), (FLD47|FLD48|FLD49),  &his_pieces_used, white_man);
if ((all_empty_or_black((FLD27|FLD18)))  || (all_occupied((FLD27|FLD18)))) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_4( (FLD17), (FLD11|FLD12), (FLD6|FLD7|FLD8), (FLD1|FLD2|FLD3),  &my_pieces_used, black_man) + 1;
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if (empty_or_black & FLD27) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_5( (FLD22), (FLD17|FLD18), (FLD11|FLD12|FLD13), (FLD6|FLD7|FLD8|FLD9), (FLD1|FLD2|FLD3|FLD4),  &my_pieces_used, black_man);
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if ((all_black((FLD23|FLD19))) && (all_empty(( FLD32|FLD37))))  return -200; // vlucht
if ((occupied & FLD27) && (all_empty(( FLD32|FLD37))))  return -200; // vlucht
if ((all_black((FLD23|FLD19))) && (white_man & FLD37) && (occupied & FLD27) && (empty_field & FLD32)) return 0; //paddestoel
if ((all_black((FLD23|FLD19))) && (white_man & FLD37) && (empty_or_black &FLD27) && (empty_field & FLD32)) return 0; //kan wegruilen
 return -700;
}

if (!(backing_attack_piece)) goto check_afruil;
// both backings are available
if (number_of_attacks > number_of_defences) {  // it looks bad, but maybe an escape?
if ((black_man & FLD27) && (all_white((FLD32|FLD38))) && ( all_empty((FLD33|FLD29)))) return 0;
his_pieces_used = 0;   // calc the minimal amount of steps for having the backing piece in place as well as the attack resp.defence piece
his_min_backing_steps = calc_dist_3( (FLD39), (FLD43|FLD44), (FLD48|FLD49|FLD50),  &his_pieces_used, white_man);
his_min_attack_steps = calc_dist_4( (FLD33), (FLD38), (FLD42|FLD43), (FLD47|FLD48|FLD49),  &his_pieces_used, white_man);
if ((all_empty_or_black((FLD27|FLD18)))  || (all_occupied((FLD27|FLD18)))) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_4( (FLD17), (FLD11|FLD12), (FLD6|FLD7|FLD8), (FLD1|FLD2|FLD3),  &my_pieces_used, black_man) + 1;
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if (empty_or_black & FLD27) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_5( (FLD22), (FLD17|FLD18), (FLD11|FLD12|FLD13), (FLD6|FLD7|FLD8|FLD9), (FLD1|FLD2|FLD3|FLD4),  &my_pieces_used, black_man);
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if ((all_black((FLD23|FLD19))) && (all_empty(( FLD32|FLD37))))  return -200; // vlucht
if ((occupied & FLD27) && (all_empty(( FLD32|FLD37))))  return -200; // vlucht
if ((all_black((FLD23|FLD19))) && (white_man & FLD37) && (occupied & FLD27) && (empty_field & FLD32)) return 0; //paddestoel
if ((all_black((FLD23|FLD19))) && (white_man & FLD37) && (empty_or_black &FLD27) && (empty_field & FLD32)) return 0; //kan wegruilen
return -700;
}

//calc the minimal amount of steps for having the backing piece in place as well as the attack resp.defence piece
his_pieces_used = 0;
his_min_backing_steps = calc_dist_3( (FLD39), (FLD43|FLD44), (FLD48|FLD49|FLD50),  &his_pieces_used, white_man);
his_min_attack_steps = calc_dist_4( (FLD33), (FLD38), (FLD42|FLD43), (FLD47|FLD48|FLD49),  &his_pieces_used, white_man);
my_pieces_used = 0;
my_min_backing_steps = calc_dist_3( (FLD11), (FLD6|FLD7), (FLD1|FLD2),  &my_pieces_used, black_man);
my_min_defence_steps = calc_dist_4( (FLD17), (FLD12), (FLD7|FLD8), (FLD1|FLD2|FLD3),  &my_pieces_used, black_man);

// we have calculated: his_min_backing_steps, his_min_attack_steps, my_min_defence_steps and my_min_backing steps

if ((his_min_backing_steps + his_min_attack_steps) - (my_min_backing_steps + my_min_defence_steps + inc) < -2) {
if ((black_man & FLD27) && (all_white((FLD32|FLD38))) && ( all_empty((FLD33|FLD29)))) return 0;
his_pieces_used = 0;   // calc the minimal amount of steps for having the backing piece in place as well as the attack resp.defence piece
his_min_backing_steps = calc_dist_3( (FLD39), (FLD43|FLD44), (FLD48|FLD49|FLD50),  &his_pieces_used, white_man);
his_min_attack_steps = calc_dist_4( (FLD33), (FLD38), (FLD42|FLD43), (FLD47|FLD48|FLD49),  &his_pieces_used, white_man);
if ((all_empty_or_black((FLD27|FLD18)))  || (all_occupied((FLD27|FLD18)))) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_4( (FLD17), (FLD11|FLD12), (FLD6|FLD7|FLD8), (FLD1|FLD2|FLD3),  &my_pieces_used, black_man) + 1;
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if (empty_or_black & FLD27) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_5( (FLD22), (FLD17|FLD18), (FLD11|FLD12|FLD13), (FLD6|FLD7|FLD8|FLD9), (FLD1|FLD2|FLD3|FLD4),  &my_pieces_used, black_man);
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if ((all_black((FLD23|FLD19))) && (all_empty(( FLD32|FLD37))))  return -200; // vlucht
if ((occupied & FLD27) && (all_empty(( FLD32|FLD37))))  return -200; // vlucht
if ((all_black((FLD23|FLD19))) && (white_man & FLD37) && (occupied & FLD27) && (empty_field & FLD32)) return 0; //paddestoel
if ((all_black((FLD23|FLD19))) && (white_man & FLD37) && (empty_or_black &FLD27) && (empty_field & FLD32)) return 0; //kan wegruilen
return -700;
}

// and a second attack??

if (number_of_attacks > 1) {
his_min_attack_steps = calc_dist_4( (FLD33), (FLD38), (FLD42|FLD43), (FLD47|FLD48|FLD49),  &his_pieces_used, white_man);
my_min_defence_steps = calc_dist_4( (FLD17), (FLD12), (FLD7|FLD8), (FLD1|FLD2|FLD3),  &my_pieces_used, black_man);
if ((his_min_attack_steps) - (my_min_defence_steps + inc) < -2) { // too late for the second attempt
if ((black_man & FLD27) && (all_white((FLD32|FLD38))) && ( all_empty((FLD33|FLD29)))) return 0;
his_pieces_used = 0;   // calc the minimal amount of steps for having the backing piece in place as well as the attack resp.defence piece
his_min_backing_steps = calc_dist_3( (FLD39), (FLD43|FLD44), (FLD48|FLD49|FLD50),  &his_pieces_used, white_man);
his_min_attack_steps = calc_dist_4( (FLD33), (FLD38), (FLD42|FLD43), (FLD47|FLD48|FLD49),  &his_pieces_used, white_man);
if ((all_empty_or_black((FLD27|FLD18)))  || (all_occupied((FLD27|FLD18)))) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_4( (FLD17), (FLD11|FLD12), (FLD6|FLD7|FLD8), (FLD1|FLD2|FLD3),  &my_pieces_used, black_man) + 1;
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if (empty_or_black & FLD27) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_5( (FLD22), (FLD17|FLD18), (FLD11|FLD12|FLD13), (FLD6|FLD7|FLD8|FLD9), (FLD1|FLD2|FLD3|FLD4),  &my_pieces_used, black_man);
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if ((all_black((FLD23|FLD19))) && (all_empty(( FLD32|FLD37))))  return -200; // vlucht
if ((occupied & FLD27) && (all_empty(( FLD32|FLD37))))  return -200; // vlucht
if ((all_black((FLD23|FLD19))) && (white_man & FLD37) && (occupied & FLD27) && (empty_field & FLD32)) return 0; //paddestoel
if ((all_black((FLD23|FLD19))) && (white_man & FLD37) && (empty_or_black &FLD27) && (empty_field & FLD32)) return 0; //kan wegruilen
return -400;
}
}
check_afruil:
if (all_white((FLD50|FLD44|FLD39))) return 0; // voorpost wordt afgeruild
if (all_white((FLD46|FLD41|FLD37))) return 0; // voorpost wordt afgeruild
// so we have hope to have a nice voorpost
if (((all_white((FLD27|FLD31|FLD36)) && (all_empty((FLD21|FLD17)))) ||  (all_white((FLD26|FLD31|FLD36)) && (all_empty((FLD27|FLD21|FLD17))))) &&  (black_man & FLD16)) return -700; // een om twee mogelijk
return 180;  // no problems, so a small advantage
}
return 0;
}
extern int voorpost_black_nw_29(BitArray, BitArray, BitArray, int);
int voorpost_black_nw_29(BitArray white_man, BitArray black_man, BitArray empty_field, int inc) {
BitArray my_pieces_used, his_pieces_used;
int number_of_defences, number_of_attacks = 0;
int his_min_backing_steps, my_min_backing_steps;
int my_min_defence_steps, his_min_attack_steps;
bool backing_defence_piece, backing_attack_piece;

if ((black_man & FLD29) &&  (all_empty_or_white((FLD34|FLD40))) &&  (all_empty_or_black((FLD18|FLD12))) &&  (empty_field & FLD23)) {
//so this is what anyway has to be there to have a voorpost

//if the field sw of b is occupied by me or can be occupied by me, no attack is possible, so the question is, can the piece on sw(b) be exchanged
if ((black_man & FLD30) ||  ((black_man & FLD25) && ((all_empty_or_black(FLD35|FLD24)) ||  (all_occupied(FLD35|FLD24))))) {
if ((all_white((FLD39|FLD43|FLD48)) && (empty_field & FLD34)) &&
    !(((white_man & FLD33) && (empty_field & FLD28)) ||  ((white_man & FLD44) && (empty_field & FLD50))))
number_of_attacks = -1;  // can be exchanged,  start with one attack less
else goto check_afruil; // no attacks possible
}

//same for the defence: if ne(d) is occupied by him, we may have a problem in the defence
if (white_man & FLD22) {
if ((all_black((FLD13|FLD7|FLD4)) && (empty_field & FLD18)) ||
    (all_black((FLD18|FLD9|FLD4)) && (empty_field & FLD13)))
number_of_defences = count(((FLD2|FLD3|FLD4|FLD8|FLD9|FLD13|FLD18) & black_man)) - 1;
else number_of_defences = 0;
} else number_of_defences = count(((FLD2|FLD3|FLD4|FLD8|FLD9|FLD13|FLD18) & black_man));

//check the availability of the backing pieces
backing_defence_piece = bool ((FLD1|FLD2|FLD3|FLD7|FLD8|FLD12) & black_man);
backing_attack_piece = (bool) ((FLD40|FLD44|FLD45|FLD49|FLD50) & white_man);
number_of_attacks += count(((FLD34|FLD39|FLD43|FLD44|FLD48|FLD49|FLD50) & white_man));
if (!((FLD40|FLD45) & white_man)) number_of_attacks -= 1;
if (!((FLD1|FLD7|FLD12) & black_man)) number_of_defences -= 1;

//so now we know whether we have a backing piece and how many pieces for the attack/defence
if (number_of_attacks <= 0) return 180;
if (!(backing_defence_piece) && (backing_attack_piece) && (number_of_attacks > 0)) { // looks bad, but maybe an escape?
if ((black_man & FLD28) && (all_white((FLD33|FLD39))) && ( all_empty((FLD34|FLD30)))) return 0;
his_pieces_used = 0;   // calc the minimal amount of steps for having the backing piece in place as well as the attack resp.defence piece
his_min_backing_steps = calc_dist_3( (FLD40), (FLD44|FLD45), (FLD49|FLD50),  &his_pieces_used, white_man);
his_min_attack_steps = calc_dist_4( (FLD34), (FLD39), (FLD43|FLD44), (FLD48|FLD49|FLD50),  &his_pieces_used, white_man);
if ((all_empty_or_black((FLD28|FLD19)))  || (all_occupied((FLD28|FLD19)))) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_4( (FLD18), (FLD12|FLD13), (FLD7|FLD8|FLD9), (FLD1|FLD2|FLD3|FLD4),  &my_pieces_used, black_man) + 1;
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if (empty_or_black & FLD28) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_5( (FLD23), (FLD18|FLD19), (FLD12|FLD13|FLD14), (FLD7|FLD8|FLD9|FLD10), (FLD1|FLD2|FLD3|FLD4|FLD5),  &my_pieces_used, black_man);
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if ((all_black((FLD24|FLD20))) && (all_empty(( FLD33|FLD38))))  return -200; // vlucht
if ((occupied & FLD28) && (all_empty(( FLD33|FLD38))))  return -200; // vlucht
if ((all_black((FLD24|FLD20))) && (white_man & FLD38) && (occupied & FLD28) && (empty_field & FLD33)) return 0; //paddestoel
if ((all_black((FLD24|FLD20))) && (white_man & FLD38) && (empty_or_black &FLD28) && (empty_field & FLD33)) return 0; //kan wegruilen
 return -700;
}

if (!(backing_attack_piece)) goto check_afruil;
// both backings are available
if (number_of_attacks > number_of_defences) {  // it looks bad, but maybe an escape?
if ((black_man & FLD28) && (all_white((FLD33|FLD39))) && ( all_empty((FLD34|FLD30)))) return 0;
his_pieces_used = 0;   // calc the minimal amount of steps for having the backing piece in place as well as the attack resp.defence piece
his_min_backing_steps = calc_dist_3( (FLD40), (FLD44|FLD45), (FLD49|FLD50),  &his_pieces_used, white_man);
his_min_attack_steps = calc_dist_4( (FLD34), (FLD39), (FLD43|FLD44), (FLD48|FLD49|FLD50),  &his_pieces_used, white_man);
if ((all_empty_or_black((FLD28|FLD19)))  || (all_occupied((FLD28|FLD19)))) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_4( (FLD18), (FLD12|FLD13), (FLD7|FLD8|FLD9), (FLD1|FLD2|FLD3|FLD4),  &my_pieces_used, black_man) + 1;
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if (empty_or_black & FLD28) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_5( (FLD23), (FLD18|FLD19), (FLD12|FLD13|FLD14), (FLD7|FLD8|FLD9|FLD10), (FLD1|FLD2|FLD3|FLD4|FLD5),  &my_pieces_used, black_man);
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if ((all_black((FLD24|FLD20))) && (all_empty(( FLD33|FLD38))))  return -200; // vlucht
if ((occupied & FLD28) && (all_empty(( FLD33|FLD38))))  return -200; // vlucht
if ((all_black((FLD24|FLD20))) && (white_man & FLD38) && (occupied & FLD28) && (empty_field & FLD33)) return 0; //paddestoel
if ((all_black((FLD24|FLD20))) && (white_man & FLD38) && (empty_or_black &FLD28) && (empty_field & FLD33)) return 0; //kan wegruilen
return -700;
}

//calc the minimal amount of steps for having the backing piece in place as well as the attack resp.defence piece
his_pieces_used = 0;
his_min_backing_steps = calc_dist_3( (FLD40), (FLD44|FLD45), (FLD49|FLD50),  &his_pieces_used, white_man);
his_min_attack_steps = calc_dist_4( (FLD34), (FLD39), (FLD43|FLD44), (FLD48|FLD49|FLD50),  &his_pieces_used, white_man);
my_pieces_used = 0;
my_min_backing_steps = calc_dist_3( (FLD12), (FLD7|FLD8), (FLD1|FLD2|FLD3),  &my_pieces_used, black_man);
my_min_defence_steps = calc_dist_4( (FLD18), (FLD13), (FLD8|FLD9), (FLD2|FLD3|FLD4),  &my_pieces_used, black_man);

// we have calculated: his_min_backing_steps, his_min_attack_steps, my_min_defence_steps and my_min_backing steps

if ((his_min_backing_steps + his_min_attack_steps) - (my_min_backing_steps + my_min_defence_steps + inc) < -2) {
if ((black_man & FLD28) && (all_white((FLD33|FLD39))) && ( all_empty((FLD34|FLD30)))) return 0;
his_pieces_used = 0;   // calc the minimal amount of steps for having the backing piece in place as well as the attack resp.defence piece
his_min_backing_steps = calc_dist_3( (FLD40), (FLD44|FLD45), (FLD49|FLD50),  &his_pieces_used, white_man);
his_min_attack_steps = calc_dist_4( (FLD34), (FLD39), (FLD43|FLD44), (FLD48|FLD49|FLD50),  &his_pieces_used, white_man);
if ((all_empty_or_black((FLD28|FLD19)))  || (all_occupied((FLD28|FLD19)))) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_4( (FLD18), (FLD12|FLD13), (FLD7|FLD8|FLD9), (FLD1|FLD2|FLD3|FLD4),  &my_pieces_used, black_man) + 1;
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if (empty_or_black & FLD28) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_5( (FLD23), (FLD18|FLD19), (FLD12|FLD13|FLD14), (FLD7|FLD8|FLD9|FLD10), (FLD1|FLD2|FLD3|FLD4|FLD5),  &my_pieces_used, black_man);
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if ((all_black((FLD24|FLD20))) && (all_empty(( FLD33|FLD38))))  return -200; // vlucht
if ((occupied & FLD28) && (all_empty(( FLD33|FLD38))))  return -200; // vlucht
if ((all_black((FLD24|FLD20))) && (white_man & FLD38) && (occupied & FLD28) && (empty_field & FLD33)) return 0; //paddestoel
if ((all_black((FLD24|FLD20))) && (white_man & FLD38) && (empty_or_black &FLD28) && (empty_field & FLD33)) return 0; //kan wegruilen
return -700;
}

// and a second attack??

if (number_of_attacks > 1) {
his_min_attack_steps = calc_dist_4( (FLD34), (FLD39), (FLD43|FLD44), (FLD48|FLD49|FLD50),  &his_pieces_used, white_man);
my_min_defence_steps = calc_dist_4( (FLD18), (FLD13), (FLD8|FLD9), (FLD2|FLD3|FLD4),  &my_pieces_used, black_man);
if ((his_min_attack_steps) - (my_min_defence_steps + inc) < -2) { // too late for the second attempt
if ((black_man & FLD28) && (all_white((FLD33|FLD39))) && ( all_empty((FLD34|FLD30)))) return 0;
his_pieces_used = 0;   // calc the minimal amount of steps for having the backing piece in place as well as the attack resp.defence piece
his_min_backing_steps = calc_dist_3( (FLD40), (FLD44|FLD45), (FLD49|FLD50),  &his_pieces_used, white_man);
his_min_attack_steps = calc_dist_4( (FLD34), (FLD39), (FLD43|FLD44), (FLD48|FLD49|FLD50),  &his_pieces_used, white_man);
if ((all_empty_or_black((FLD28|FLD19)))  || (all_occupied((FLD28|FLD19)))) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_4( (FLD18), (FLD12|FLD13), (FLD7|FLD8|FLD9), (FLD1|FLD2|FLD3|FLD4),  &my_pieces_used, black_man) + 1;
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if (empty_or_black & FLD28) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_5( (FLD23), (FLD18|FLD19), (FLD12|FLD13|FLD14), (FLD7|FLD8|FLD9|FLD10), (FLD1|FLD2|FLD3|FLD4|FLD5),  &my_pieces_used, black_man);
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if ((all_black((FLD24|FLD20))) && (all_empty(( FLD33|FLD38))))  return -200; // vlucht
if ((occupied & FLD28) && (all_empty(( FLD33|FLD38))))  return -200; // vlucht
if ((all_black((FLD24|FLD20))) && (white_man & FLD38) && (occupied & FLD28) && (empty_field & FLD33)) return 0; //paddestoel
if ((all_black((FLD24|FLD20))) && (white_man & FLD38) && (empty_or_black &FLD28) && (empty_field & FLD33)) return 0; //kan wegruilen
return -400;
}
}
check_afruil:
if (all_white((FLD45|FLD40))) return 0; // voorpost wordt afgeruild
if (all_white((FLD47|FLD42|FLD38))) return 0; // voorpost wordt afgeruild
// so we have hope to have a nice voorpost
return 180;  // no problems, so a small advantage
}
return 0;
}
extern int voorpost_black_nw_32(BitArray, BitArray, BitArray, int);
int voorpost_black_nw_32(BitArray white_man, BitArray black_man, BitArray empty_field, int inc) {
BitArray my_pieces_used, his_pieces_used;
int number_of_defences, number_of_attacks = 0;
int his_min_backing_steps, my_min_backing_steps;
int my_min_defence_steps, his_min_attack_steps;
bool backing_defence_piece, backing_attack_piece;

if ((black_man & FLD32) &&  (all_empty_or_white((FLD38|FLD43))) &&  (all_empty_or_black((FLD21|FLD16))) &&  (empty_field & FLD27)) {
//so this is what anyway has to be there to have a voorpost

//if the field sw of b is occupied by me or can be occupied by me, no attack is possible, so the question is, can the piece on sw(b) be exchanged
if ((black_man & FLD33) ||  ((black_man & FLD29) && ((all_empty_or_black(FLD39|FLD28)) ||  (all_occupied(FLD39|FLD28))))) {
if ((all_white((FLD42|FLD47|FLD0)) && (empty_field & FLD38)) &&
    !(((white_man & FLD37) && (empty_field & FLD31)) ||  ((white_man & FLD48) && (empty_field & FLD0))))
number_of_attacks = -1;  // can be exchanged,  start with one attack less
else goto check_afruil; // no attacks possible
}

//same for the defence: if ne(d) is occupied by him, we may have a problem in the defence
if (white_man & FLD26) {
if ((all_black((FLD17|FLD0|FLD8)) && (empty_field & FLD21)) ||
    (all_black((FLD21|FLD12|FLD8)) && (empty_field & FLD17)))
number_of_defences = count(((FLD1|FLD2|FLD3|FLD6|FLD7|FLD8|FLD11|FLD12|FLD17|FLD21) & black_man)) - 1;
else number_of_defences = 0;
} else number_of_defences = count(((FLD1|FLD2|FLD3|FLD6|FLD7|FLD8|FLD11|FLD12|FLD17|FLD21) & black_man));

//check the availability of the backing pieces
backing_defence_piece = bool ((FLD1|FLD2|FLD6|FLD7|FLD11|FLD16) & black_man);
backing_attack_piece = (bool) ((FLD43|FLD48|FLD49) & white_man);
number_of_attacks += count(((FLD38|FLD42|FLD47|FLD48) & white_man));
if (!((FLD43|FLD49) & white_man)) number_of_attacks -= 1;
if (!((FLD16) & black_man)) number_of_defences -= 1;

//so now we know whether we have a backing piece and how many pieces for the attack/defence
if (number_of_attacks <= 0) return 180;
if (!(backing_defence_piece) && (backing_attack_piece) && (number_of_attacks > 0)) { // looks bad, but maybe an escape?
if ((black_man & FLD31) && (all_white((FLD37|FLD42))) && ( all_empty((FLD38|FLD33)))) return 0;
his_pieces_used = 0;   // calc the minimal amount of steps for having the backing piece in place as well as the attack resp.defence piece
his_min_backing_steps = calc_dist_2( (FLD43), (FLD48|FLD49),  &his_pieces_used, white_man);
his_min_attack_steps = calc_dist_3( (FLD38), (FLD42), (FLD47|FLD48),  &his_pieces_used, white_man);
if ((all_empty_or_black((FLD31|FLD22)))  || (all_occupied((FLD31|FLD22)))) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_5( (FLD21), (FLD16|FLD17), (FLD11|FLD12), (FLD6|FLD7|FLD8), (FLD1|FLD2|FLD3),  &my_pieces_used, black_man) + 1;
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if (empty_or_black & FLD31) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_6( (FLD27), (FLD21|FLD22), (FLD16|FLD17|FLD18), (FLD11|FLD12|FLD13), (FLD6|FLD7|FLD8|FLD9), (FLD1|FLD2|FLD3|FLD4),  &my_pieces_used, black_man);
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if ((all_black((FLD28|FLD23))) && (all_empty(( FLD37|FLD41))))  return -200; // vlucht
if ((occupied & FLD31) && (all_empty(( FLD37|FLD41))))  return -200; // vlucht
if ((all_black((FLD28|FLD23))) && (white_man & FLD41) && (occupied & FLD31) && (empty_field & FLD37)) return 0; //paddestoel
if ((all_black((FLD28|FLD23))) && (white_man & FLD41) && (empty_or_black &FLD31) && (empty_field & FLD37)) return 0; //kan wegruilen
 return -700;
}

if (!(backing_attack_piece)) goto check_afruil;
// both backings are available
if (number_of_attacks > number_of_defences) {  // it looks bad, but maybe an escape?
if ((black_man & FLD31) && (all_white((FLD37|FLD42))) && ( all_empty((FLD38|FLD33)))) return 0;
his_pieces_used = 0;   // calc the minimal amount of steps for having the backing piece in place as well as the attack resp.defence piece
his_min_backing_steps = calc_dist_2( (FLD43), (FLD48|FLD49),  &his_pieces_used, white_man);
his_min_attack_steps = calc_dist_3( (FLD38), (FLD42), (FLD47|FLD48),  &his_pieces_used, white_man);
if ((all_empty_or_black((FLD31|FLD22)))  || (all_occupied((FLD31|FLD22)))) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_5( (FLD21), (FLD16|FLD17), (FLD11|FLD12), (FLD6|FLD7|FLD8), (FLD1|FLD2|FLD3),  &my_pieces_used, black_man) + 1;
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if (empty_or_black & FLD31) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_6( (FLD27), (FLD21|FLD22), (FLD16|FLD17|FLD18), (FLD11|FLD12|FLD13), (FLD6|FLD7|FLD8|FLD9), (FLD1|FLD2|FLD3|FLD4),  &my_pieces_used, black_man);
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if ((all_black((FLD28|FLD23))) && (all_empty(( FLD37|FLD41))))  return -200; // vlucht
if ((occupied & FLD31) && (all_empty(( FLD37|FLD41))))  return -200; // vlucht
if ((all_black((FLD28|FLD23))) && (white_man & FLD41) && (occupied & FLD31) && (empty_field & FLD37)) return 0; //paddestoel
if ((all_black((FLD28|FLD23))) && (white_man & FLD41) && (empty_or_black &FLD31) && (empty_field & FLD37)) return 0; //kan wegruilen
return -700;
}

//calc the minimal amount of steps for having the backing piece in place as well as the attack resp.defence piece
his_pieces_used = 0;
his_min_backing_steps = calc_dist_2( (FLD43), (FLD48|FLD49),  &his_pieces_used, white_man);
his_min_attack_steps = calc_dist_3( (FLD38), (FLD42), (FLD47|FLD48),  &his_pieces_used, white_man);
my_pieces_used = 0;
my_min_backing_steps = calc_dist_4( (FLD16), (FLD11), (FLD6|FLD7), (FLD1|FLD2),  &my_pieces_used, black_man);
my_min_defence_steps = calc_dist_5( (FLD21), (FLD17), (FLD11|FLD12), (FLD6|FLD7|FLD8), (FLD1|FLD2|FLD3),  &my_pieces_used, black_man);

// we have calculated: his_min_backing_steps, his_min_attack_steps, my_min_defence_steps and my_min_backing steps

if ((his_min_backing_steps + his_min_attack_steps) - (my_min_backing_steps + my_min_defence_steps + inc) < -2) {
if ((black_man & FLD31) && (all_white((FLD37|FLD42))) && ( all_empty((FLD38|FLD33)))) return 0;
his_pieces_used = 0;   // calc the minimal amount of steps for having the backing piece in place as well as the attack resp.defence piece
his_min_backing_steps = calc_dist_2( (FLD43), (FLD48|FLD49),  &his_pieces_used, white_man);
his_min_attack_steps = calc_dist_3( (FLD38), (FLD42), (FLD47|FLD48),  &his_pieces_used, white_man);
if ((all_empty_or_black((FLD31|FLD22)))  || (all_occupied((FLD31|FLD22)))) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_5( (FLD21), (FLD16|FLD17), (FLD11|FLD12), (FLD6|FLD7|FLD8), (FLD1|FLD2|FLD3),  &my_pieces_used, black_man) + 1;
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if (empty_or_black & FLD31) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_6( (FLD27), (FLD21|FLD22), (FLD16|FLD17|FLD18), (FLD11|FLD12|FLD13), (FLD6|FLD7|FLD8|FLD9), (FLD1|FLD2|FLD3|FLD4),  &my_pieces_used, black_man);
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if ((all_black((FLD28|FLD23))) && (all_empty(( FLD37|FLD41))))  return -200; // vlucht
if ((occupied & FLD31) && (all_empty(( FLD37|FLD41))))  return -200; // vlucht
if ((all_black((FLD28|FLD23))) && (white_man & FLD41) && (occupied & FLD31) && (empty_field & FLD37)) return 0; //paddestoel
if ((all_black((FLD28|FLD23))) && (white_man & FLD41) && (empty_or_black &FLD31) && (empty_field & FLD37)) return 0; //kan wegruilen
return -700;
}

// and a second attack??

if (number_of_attacks > 1) {
his_min_attack_steps = calc_dist_3( (FLD38), (FLD42), (FLD47|FLD48),  &his_pieces_used, white_man);
my_min_defence_steps = calc_dist_5( (FLD21), (FLD17), (FLD11|FLD12), (FLD6|FLD7|FLD8), (FLD1|FLD2|FLD3),  &my_pieces_used, black_man);
if ((his_min_attack_steps) - (my_min_defence_steps + inc) < -2) { // too late for the second attempt
if ((black_man & FLD31) && (all_white((FLD37|FLD42))) && ( all_empty((FLD38|FLD33)))) return 0;
his_pieces_used = 0;   // calc the minimal amount of steps for having the backing piece in place as well as the attack resp.defence piece
his_min_backing_steps = calc_dist_2( (FLD43), (FLD48|FLD49),  &his_pieces_used, white_man);
his_min_attack_steps = calc_dist_3( (FLD38), (FLD42), (FLD47|FLD48),  &his_pieces_used, white_man);
if ((all_empty_or_black((FLD31|FLD22)))  || (all_occupied((FLD31|FLD22)))) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_5( (FLD21), (FLD16|FLD17), (FLD11|FLD12), (FLD6|FLD7|FLD8), (FLD1|FLD2|FLD3),  &my_pieces_used, black_man) + 1;
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if (empty_or_black & FLD31) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_6( (FLD27), (FLD21|FLD22), (FLD16|FLD17|FLD18), (FLD11|FLD12|FLD13), (FLD6|FLD7|FLD8|FLD9), (FLD1|FLD2|FLD3|FLD4),  &my_pieces_used, black_man);
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if ((all_black((FLD28|FLD23))) && (all_empty(( FLD37|FLD41))))  return -200; // vlucht
if ((occupied & FLD31) && (all_empty(( FLD37|FLD41))))  return -200; // vlucht
if ((all_black((FLD28|FLD23))) && (white_man & FLD41) && (occupied & FLD31) && (empty_field & FLD37)) return 0; //paddestoel
if ((all_black((FLD28|FLD23))) && (white_man & FLD41) && (empty_or_black &FLD31) && (empty_field & FLD37)) return 0; //kan wegruilen
return -400;
}
}
check_afruil:
if (all_white((FLD49|FLD43))) return 0; // voorpost wordt afgeruild
if (all_white((FLD46|FLD41))) return 0; // voorpost wordt afgeruild
// so we have hope to have a nice voorpost
return 180;  // no problems, so a small advantage
}
return 0;
}
extern int voorpost_black_nw_33(BitArray, BitArray, BitArray, int);
int voorpost_black_nw_33(BitArray white_man, BitArray black_man, BitArray empty_field, int inc) {
BitArray my_pieces_used, his_pieces_used;
int number_of_defences, number_of_attacks = 0;
int his_min_backing_steps, my_min_backing_steps;
int my_min_defence_steps, his_min_attack_steps;
bool backing_defence_piece, backing_attack_piece;

if ((black_man & FLD33) &&  (all_empty_or_white((FLD39|FLD44))) &&  (all_empty_or_black((FLD22|FLD17))) &&  (empty_field & FLD28)) {
//so this is what anyway has to be there to have a voorpost

//if the field sw of b is occupied by me or can be occupied by me, no attack is possible, so the question is, can the piece on sw(b) be exchanged
if ((black_man & FLD34) ||  ((black_man & FLD30) && ((all_empty_or_black(FLD40|FLD29)) ||  (all_occupied(FLD40|FLD29))))) {
if ((all_white((FLD43|FLD48|FLD0)) && (empty_field & FLD39)) &&
    !(((white_man & FLD38) && (empty_field & FLD32)) ||  ((white_man & FLD49) && (empty_field & FLD0))))
number_of_attacks = -1;  // can be exchanged,  start with one attack less
else goto check_afruil; // no attacks possible
}

//same for the defence: if ne(d) is occupied by him, we may have a problem in the defence
if (white_man & FLD27) {
if ((all_black((FLD18|FLD11|FLD9)) && (empty_field & FLD22)) ||
    (all_black((FLD22|FLD13|FLD9)) && (empty_field & FLD18)))
number_of_defences = count(((FLD1|FLD2|FLD3|FLD4|FLD7|FLD8|FLD9|FLD12|FLD13|FLD18|FLD22) & black_man)) - 1;
else number_of_defences = 0;
} else number_of_defences = count(((FLD1|FLD2|FLD3|FLD4|FLD7|FLD8|FLD9|FLD12|FLD13|FLD18|FLD22) & black_man));

//check the availability of the backing pieces
backing_defence_piece = bool ((FLD1|FLD2|FLD3|FLD6|FLD7|FLD8|FLD11|FLD12|FLD17) & black_man);
backing_attack_piece = (bool) ((FLD44|FLD49|FLD50) & white_man);
number_of_attacks += count(((FLD39|FLD43|FLD48|FLD49) & white_man));
if (!((FLD44|FLD50) & white_man)) number_of_attacks -= 1;
if (!((FLD6|FLD11|FLD17) & black_man)) number_of_defences -= 1;

//so now we know whether we have a backing piece and how many pieces for the attack/defence
if (number_of_attacks <= 0) return 180;
if (!(backing_defence_piece) && (backing_attack_piece) && (number_of_attacks > 0)) { // looks bad, but maybe an escape?
if ((black_man & FLD32) && (all_white((FLD38|FLD43))) && ( all_empty((FLD39|FLD34)))) return 0;
his_pieces_used = 0;   // calc the minimal amount of steps for having the backing piece in place as well as the attack resp.defence piece
his_min_backing_steps = calc_dist_2( (FLD44), (FLD49|FLD50),  &his_pieces_used, white_man);
his_min_attack_steps = calc_dist_3( (FLD39), (FLD43), (FLD48|FLD49),  &his_pieces_used, white_man);
if ((all_empty_or_black((FLD32|FLD23)))  || (all_occupied((FLD32|FLD23)))) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_5( (FLD22), (FLD17|FLD18), (FLD11|FLD12|FLD13), (FLD6|FLD7|FLD8|FLD9), (FLD1|FLD2|FLD3|FLD4),  &my_pieces_used, black_man) + 1;
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if (empty_or_black & FLD32) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_6( (FLD28), (FLD22|FLD23), (FLD17|FLD18|FLD19), (FLD11|FLD12|FLD13|FLD14), (FLD6|FLD7|FLD8|FLD9|FLD10), (FLD1|FLD2|FLD3|FLD4|FLD5),  &my_pieces_used, black_man);
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if ((all_black((FLD29|FLD24))) && (all_empty(( FLD38|FLD42))))  return -200; // vlucht
if ((occupied & FLD32) && (all_empty(( FLD38|FLD42))))  return -200; // vlucht
if ((all_black((FLD29|FLD24))) && (white_man & FLD42) && (occupied & FLD32) && (empty_field & FLD38)) return 0; //paddestoel
if ((all_black((FLD29|FLD24))) && (white_man & FLD42) && (empty_or_black &FLD32) && (empty_field & FLD38)) return 0; //kan wegruilen
 return -700;
}

if (!(backing_attack_piece)) goto check_afruil;
// both backings are available
if (number_of_attacks > number_of_defences) {  // it looks bad, but maybe an escape?
if ((black_man & FLD32) && (all_white((FLD38|FLD43))) && ( all_empty((FLD39|FLD34)))) return 0;
his_pieces_used = 0;   // calc the minimal amount of steps for having the backing piece in place as well as the attack resp.defence piece
his_min_backing_steps = calc_dist_2( (FLD44), (FLD49|FLD50),  &his_pieces_used, white_man);
his_min_attack_steps = calc_dist_3( (FLD39), (FLD43), (FLD48|FLD49),  &his_pieces_used, white_man);
if ((all_empty_or_black((FLD32|FLD23)))  || (all_occupied((FLD32|FLD23)))) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_5( (FLD22), (FLD17|FLD18), (FLD11|FLD12|FLD13), (FLD6|FLD7|FLD8|FLD9), (FLD1|FLD2|FLD3|FLD4),  &my_pieces_used, black_man) + 1;
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if (empty_or_black & FLD32) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_6( (FLD28), (FLD22|FLD23), (FLD17|FLD18|FLD19), (FLD11|FLD12|FLD13|FLD14), (FLD6|FLD7|FLD8|FLD9|FLD10), (FLD1|FLD2|FLD3|FLD4|FLD5),  &my_pieces_used, black_man);
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if ((all_black((FLD29|FLD24))) && (all_empty(( FLD38|FLD42))))  return -200; // vlucht
if ((occupied & FLD32) && (all_empty(( FLD38|FLD42))))  return -200; // vlucht
if ((all_black((FLD29|FLD24))) && (white_man & FLD42) && (occupied & FLD32) && (empty_field & FLD38)) return 0; //paddestoel
if ((all_black((FLD29|FLD24))) && (white_man & FLD42) && (empty_or_black &FLD32) && (empty_field & FLD38)) return 0; //kan wegruilen
return -700;
}

//calc the minimal amount of steps for having the backing piece in place as well as the attack resp.defence piece
his_pieces_used = 0;
his_min_backing_steps = calc_dist_2( (FLD44), (FLD49|FLD50),  &his_pieces_used, white_man);
his_min_attack_steps = calc_dist_3( (FLD39), (FLD43), (FLD48|FLD49),  &his_pieces_used, white_man);
my_pieces_used = 0;
my_min_backing_steps = calc_dist_4( (FLD17), (FLD11|FLD12), (FLD6|FLD7|FLD8), (FLD1|FLD2|FLD3),  &my_pieces_used, black_man);
my_min_defence_steps = calc_dist_5( (FLD22), (FLD18), (FLD12|FLD13), (FLD7|FLD8|FLD9), (FLD1|FLD2|FLD3|FLD4),  &my_pieces_used, black_man);

// we have calculated: his_min_backing_steps, his_min_attack_steps, my_min_defence_steps and my_min_backing steps

if ((his_min_backing_steps + his_min_attack_steps) - (my_min_backing_steps + my_min_defence_steps + inc) < -2) {
if ((black_man & FLD32) && (all_white((FLD38|FLD43))) && ( all_empty((FLD39|FLD34)))) return 0;
his_pieces_used = 0;   // calc the minimal amount of steps for having the backing piece in place as well as the attack resp.defence piece
his_min_backing_steps = calc_dist_2( (FLD44), (FLD49|FLD50),  &his_pieces_used, white_man);
his_min_attack_steps = calc_dist_3( (FLD39), (FLD43), (FLD48|FLD49),  &his_pieces_used, white_man);
if ((all_empty_or_black((FLD32|FLD23)))  || (all_occupied((FLD32|FLD23)))) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_5( (FLD22), (FLD17|FLD18), (FLD11|FLD12|FLD13), (FLD6|FLD7|FLD8|FLD9), (FLD1|FLD2|FLD3|FLD4),  &my_pieces_used, black_man) + 1;
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if (empty_or_black & FLD32) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_6( (FLD28), (FLD22|FLD23), (FLD17|FLD18|FLD19), (FLD11|FLD12|FLD13|FLD14), (FLD6|FLD7|FLD8|FLD9|FLD10), (FLD1|FLD2|FLD3|FLD4|FLD5),  &my_pieces_used, black_man);
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if ((all_black((FLD29|FLD24))) && (all_empty(( FLD38|FLD42))))  return -200; // vlucht
if ((occupied & FLD32) && (all_empty(( FLD38|FLD42))))  return -200; // vlucht
if ((all_black((FLD29|FLD24))) && (white_man & FLD42) && (occupied & FLD32) && (empty_field & FLD38)) return 0; //paddestoel
if ((all_black((FLD29|FLD24))) && (white_man & FLD42) && (empty_or_black &FLD32) && (empty_field & FLD38)) return 0; //kan wegruilen
return -700;
}

// and a second attack??

if (number_of_attacks > 1) {
his_min_attack_steps = calc_dist_3( (FLD39), (FLD43), (FLD48|FLD49),  &his_pieces_used, white_man);
my_min_defence_steps = calc_dist_5( (FLD22), (FLD18), (FLD12|FLD13), (FLD7|FLD8|FLD9), (FLD1|FLD2|FLD3|FLD4),  &my_pieces_used, black_man);
if ((his_min_attack_steps) - (my_min_defence_steps + inc) < -2) { // too late for the second attempt
if ((black_man & FLD32) && (all_white((FLD38|FLD43))) && ( all_empty((FLD39|FLD34)))) return 0;
his_pieces_used = 0;   // calc the minimal amount of steps for having the backing piece in place as well as the attack resp.defence piece
his_min_backing_steps = calc_dist_2( (FLD44), (FLD49|FLD50),  &his_pieces_used, white_man);
his_min_attack_steps = calc_dist_3( (FLD39), (FLD43), (FLD48|FLD49),  &his_pieces_used, white_man);
if ((all_empty_or_black((FLD32|FLD23)))  || (all_occupied((FLD32|FLD23)))) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_5( (FLD22), (FLD17|FLD18), (FLD11|FLD12|FLD13), (FLD6|FLD7|FLD8|FLD9), (FLD1|FLD2|FLD3|FLD4),  &my_pieces_used, black_man) + 1;
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if (empty_or_black & FLD32) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_6( (FLD28), (FLD22|FLD23), (FLD17|FLD18|FLD19), (FLD11|FLD12|FLD13|FLD14), (FLD6|FLD7|FLD8|FLD9|FLD10), (FLD1|FLD2|FLD3|FLD4|FLD5),  &my_pieces_used, black_man);
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if ((all_black((FLD29|FLD24))) && (all_empty(( FLD38|FLD42))))  return -200; // vlucht
if ((occupied & FLD32) && (all_empty(( FLD38|FLD42))))  return -200; // vlucht
if ((all_black((FLD29|FLD24))) && (white_man & FLD42) && (occupied & FLD32) && (empty_field & FLD38)) return 0; //paddestoel
if ((all_black((FLD29|FLD24))) && (white_man & FLD42) && (empty_or_black &FLD32) && (empty_field & FLD38)) return 0; //kan wegruilen
return -400;
}
}
check_afruil:
if (all_white((FLD50|FLD44))) return 0; // voorpost wordt afgeruild
if (all_white((FLD47|FLD42))) return 0; // voorpost wordt afgeruild
// so we have hope to have a nice voorpost
return 180;  // no problems, so a small advantage
}
return 0;
}
extern int voorpost_black_nw_34(BitArray, BitArray, BitArray, int);
int voorpost_black_nw_34(BitArray white_man, BitArray black_man, BitArray empty_field, int inc) {
BitArray my_pieces_used, his_pieces_used;
int number_of_defences, number_of_attacks = 0;
int his_min_backing_steps, my_min_backing_steps;
int my_min_defence_steps, his_min_attack_steps;
bool backing_defence_piece, backing_attack_piece;

if ((black_man & FLD34) &&  (all_empty_or_white((FLD40|FLD45))) &&  (all_empty_or_black((FLD23|FLD18))) &&  (empty_field & FLD29)) {
//so this is what anyway has to be there to have a voorpost

//if the field sw of b is occupied by me or can be occupied by me, no attack is possible, so the question is, can the piece on sw(b) be exchanged
if (black_man & FLD35) {
if ((all_white((FLD44|FLD49|FLD0)) && (empty_field & FLD40)) &&
    !(((white_man & FLD39) && (empty_field & FLD33)) ||  ((white_man & FLD50) && (empty_field & FLD0))))
number_of_attacks = -1;  // can be exchanged,  start with one attack less
else goto check_afruil; // no attacks possible
}

//same for the defence: if ne(d) is occupied by him, we may have a problem in the defence
if (white_man & FLD28) {
if ((all_black((FLD19|FLD12|FLD10)) && (empty_field & FLD23)) ||
    (all_black((FLD23|FLD14|FLD10)) && (empty_field & FLD19)))
number_of_defences = count(((FLD2|FLD3|FLD4|FLD5|FLD8|FLD9|FLD10|FLD13|FLD14|FLD19|FLD23) & black_man)) - 1;
else number_of_defences = 0;
} else number_of_defences = count(((FLD2|FLD3|FLD4|FLD5|FLD8|FLD9|FLD10|FLD13|FLD14|FLD19|FLD23) & black_man));

//check the availability of the backing pieces
backing_defence_piece = bool ((FLD1|FLD2|FLD3|FLD4|FLD7|FLD8|FLD9|FLD12|FLD13|FLD18) & black_man);
backing_attack_piece = (bool) ((FLD45|FLD50) & white_man);
number_of_attacks += count(((FLD40|FLD44|FLD49|FLD50) & white_man));
if (!((FLD45) & white_man)) number_of_attacks -= 1;
if (!((FLD1|FLD7|FLD12|FLD18) & black_man)) number_of_defences -= 1;

//so now we know whether we have a backing piece and how many pieces for the attack/defence
if (number_of_attacks <= 0) return 180;
if (!(backing_defence_piece) && (backing_attack_piece) && (number_of_attacks > 0)) { // looks bad, but maybe an escape?
if ((black_man & FLD33) && (all_white((FLD39|FLD44))) && ( all_empty((FLD40|FLD35)))) return 0;
his_pieces_used = 0;   // calc the minimal amount of steps for having the backing piece in place as well as the attack resp.defence piece
his_min_backing_steps = calc_dist_2( (FLD45), (FLD50),  &his_pieces_used, white_man);
his_min_attack_steps = calc_dist_3( (FLD40), (FLD44), (FLD49|FLD50),  &his_pieces_used, white_man);
if ((all_empty_or_black((FLD33|FLD24)))  || (all_occupied((FLD33|FLD24)))) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_5( (FLD23), (FLD18|FLD19), (FLD12|FLD13|FLD14), (FLD7|FLD8|FLD9|FLD10), (FLD1|FLD2|FLD3|FLD4|FLD5),  &my_pieces_used, black_man) + 1;
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if (empty_or_black & FLD33) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_6( (FLD29), (FLD23|FLD24), (FLD18|FLD19|FLD20), (FLD12|FLD13|FLD14|FLD15), (FLD7|FLD8|FLD9|FLD10), (FLD1|FLD2|FLD3|FLD4|FLD5),  &my_pieces_used, black_man);
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if ((all_black((FLD30|FLD25))) && (all_empty(( FLD39|FLD43))))  return -200; // vlucht
if ((occupied & FLD33) && (all_empty(( FLD39|FLD43))))  return -200; // vlucht
if ((all_black((FLD30|FLD25))) && (white_man & FLD43) && (occupied & FLD33) && (empty_field & FLD39)) return 0; //paddestoel
if ((all_black((FLD30|FLD25))) && (white_man & FLD43) && (empty_or_black &FLD33) && (empty_field & FLD39)) return 0; //kan wegruilen
 return -700;
}

if (!(backing_attack_piece)) goto check_afruil;
// both backings are available
if (number_of_attacks > number_of_defences) {  // it looks bad, but maybe an escape?
if ((black_man & FLD33) && (all_white((FLD39|FLD44))) && ( all_empty((FLD40|FLD35)))) return 0;
his_pieces_used = 0;   // calc the minimal amount of steps for having the backing piece in place as well as the attack resp.defence piece
his_min_backing_steps = calc_dist_2( (FLD45), (FLD50),  &his_pieces_used, white_man);
his_min_attack_steps = calc_dist_3( (FLD40), (FLD44), (FLD49|FLD50),  &his_pieces_used, white_man);
if ((all_empty_or_black((FLD33|FLD24)))  || (all_occupied((FLD33|FLD24)))) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_5( (FLD23), (FLD18|FLD19), (FLD12|FLD13|FLD14), (FLD7|FLD8|FLD9|FLD10), (FLD1|FLD2|FLD3|FLD4|FLD5),  &my_pieces_used, black_man) + 1;
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if (empty_or_black & FLD33) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_6( (FLD29), (FLD23|FLD24), (FLD18|FLD19|FLD20), (FLD12|FLD13|FLD14|FLD15), (FLD7|FLD8|FLD9|FLD10), (FLD1|FLD2|FLD3|FLD4|FLD5),  &my_pieces_used, black_man);
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if ((all_black((FLD30|FLD25))) && (all_empty(( FLD39|FLD43))))  return -200; // vlucht
if ((occupied & FLD33) && (all_empty(( FLD39|FLD43))))  return -200; // vlucht
if ((all_black((FLD30|FLD25))) && (white_man & FLD43) && (occupied & FLD33) && (empty_field & FLD39)) return 0; //paddestoel
if ((all_black((FLD30|FLD25))) && (white_man & FLD43) && (empty_or_black &FLD33) && (empty_field & FLD39)) return 0; //kan wegruilen
return -700;
}

//calc the minimal amount of steps for having the backing piece in place as well as the attack resp.defence piece
his_pieces_used = 0;
his_min_backing_steps = calc_dist_2( (FLD45), (FLD50),  &his_pieces_used, white_man);
his_min_attack_steps = calc_dist_3( (FLD40), (FLD44), (FLD49|FLD50),  &his_pieces_used, white_man);
my_pieces_used = 0;
my_min_backing_steps = calc_dist_4( (FLD18), (FLD12|FLD13), (FLD7|FLD8|FLD9), (FLD1|FLD2|FLD3|FLD4),  &my_pieces_used, black_man);
my_min_defence_steps = calc_dist_5( (FLD23), (FLD19), (FLD13|FLD14), (FLD8|FLD9|FLD10), (FLD2|FLD3|FLD4|FLD5),  &my_pieces_used, black_man);

// we have calculated: his_min_backing_steps, his_min_attack_steps, my_min_defence_steps and my_min_backing steps

if ((his_min_backing_steps + his_min_attack_steps) - (my_min_backing_steps + my_min_defence_steps + inc) < -2) {
if ((black_man & FLD33) && (all_white((FLD39|FLD44))) && ( all_empty((FLD40|FLD35)))) return 0;
his_pieces_used = 0;   // calc the minimal amount of steps for having the backing piece in place as well as the attack resp.defence piece
his_min_backing_steps = calc_dist_2( (FLD45), (FLD50),  &his_pieces_used, white_man);
his_min_attack_steps = calc_dist_3( (FLD40), (FLD44), (FLD49|FLD50),  &his_pieces_used, white_man);
if ((all_empty_or_black((FLD33|FLD24)))  || (all_occupied((FLD33|FLD24)))) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_5( (FLD23), (FLD18|FLD19), (FLD12|FLD13|FLD14), (FLD7|FLD8|FLD9|FLD10), (FLD1|FLD2|FLD3|FLD4|FLD5),  &my_pieces_used, black_man) + 1;
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if (empty_or_black & FLD33) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_6( (FLD29), (FLD23|FLD24), (FLD18|FLD19|FLD20), (FLD12|FLD13|FLD14|FLD15), (FLD7|FLD8|FLD9|FLD10), (FLD1|FLD2|FLD3|FLD4|FLD5),  &my_pieces_used, black_man);
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if ((all_black((FLD30|FLD25))) && (all_empty(( FLD39|FLD43))))  return -200; // vlucht
if ((occupied & FLD33) && (all_empty(( FLD39|FLD43))))  return -200; // vlucht
if ((all_black((FLD30|FLD25))) && (white_man & FLD43) && (occupied & FLD33) && (empty_field & FLD39)) return 0; //paddestoel
if ((all_black((FLD30|FLD25))) && (white_man & FLD43) && (empty_or_black &FLD33) && (empty_field & FLD39)) return 0; //kan wegruilen
return -700;
}

// and a second attack??

if (number_of_attacks > 1) {
his_min_attack_steps = calc_dist_3( (FLD40), (FLD44), (FLD49|FLD50),  &his_pieces_used, white_man);
my_min_defence_steps = calc_dist_5( (FLD23), (FLD19), (FLD13|FLD14), (FLD8|FLD9|FLD10), (FLD2|FLD3|FLD4|FLD5),  &my_pieces_used, black_man);
if ((his_min_attack_steps) - (my_min_defence_steps + inc) < -2) { // too late for the second attempt
if ((black_man & FLD33) && (all_white((FLD39|FLD44))) && ( all_empty((FLD40|FLD35)))) return 0;
his_pieces_used = 0;   // calc the minimal amount of steps for having the backing piece in place as well as the attack resp.defence piece
his_min_backing_steps = calc_dist_2( (FLD45), (FLD50),  &his_pieces_used, white_man);
his_min_attack_steps = calc_dist_3( (FLD40), (FLD44), (FLD49|FLD50),  &his_pieces_used, white_man);
if ((all_empty_or_black((FLD33|FLD24)))  || (all_occupied((FLD33|FLD24)))) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_5( (FLD23), (FLD18|FLD19), (FLD12|FLD13|FLD14), (FLD7|FLD8|FLD9|FLD10), (FLD1|FLD2|FLD3|FLD4|FLD5),  &my_pieces_used, black_man) + 1;
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if (empty_or_black & FLD33) {
my_pieces_used = 0;
my_min_defence_steps = calc_dist_6( (FLD29), (FLD23|FLD24), (FLD18|FLD19|FLD20), (FLD12|FLD13|FLD14|FLD15), (FLD7|FLD8|FLD9|FLD10), (FLD1|FLD2|FLD3|FLD4|FLD5),  &my_pieces_used, black_man);
if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;
}
if ((all_black((FLD30|FLD25))) && (all_empty(( FLD39|FLD43))))  return -200; // vlucht
if ((occupied & FLD33) && (all_empty(( FLD39|FLD43))))  return -200; // vlucht
if ((all_black((FLD30|FLD25))) && (white_man & FLD43) && (occupied & FLD33) && (empty_field & FLD39)) return 0; //paddestoel
if ((all_black((FLD30|FLD25))) && (white_man & FLD43) && (empty_or_black &FLD33) && (empty_field & FLD39)) return 0; //kan wegruilen
return -400;
}
}
check_afruil:
if (all_white((FLD48|FLD43))) return 0; // voorpost wordt afgeruild
// so we have hope to have a nice voorpost
return 180;  // no problems, so a small advantage
}
return 0;
}
