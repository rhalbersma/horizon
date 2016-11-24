/*
  This file is Copyright (C) 2010 Feike Boomstra.
  Distributed under the Boost Software License, Version 1.0.
  (See accompanying file LICENSE_1_0.txt or copy at
  http://www.boost.org/LICENSE_1_0.txt)
*/


#include "BoomstraDam.v2.5.h"


int increase_depth_white_on_move(NodePnt node)
{	BitArray white_man = (node->occ_empty) & (node->white_black) & (node->man_king);
	BitArray black_man = (node->occ_empty) & ~(node->white_black) & (node->man_king);
	BitArray empty_field = ~(node->occ_empty) & BITS50;
	BitArray any_piece = (node->occ_empty) & BITS50;

	if ((all_white((FLD26|FLD28|FLD18))) && (all_black((FLD17| FLD8))) && (all_empty((FLD7|FLD12|FLD21|FLD22)))) return 1;
	if ((all_white((FLD27|FLD29|FLD19))) && (all_black((FLD18| FLD9))) && (all_empty((FLD8|FLD13|FLD22|FLD23)))) return 1;
	if ((all_white((FLD28|FLD30|FLD20))) && (all_black((FLD19|FLD10))) && (all_empty((FLD9|FLD14|FLD23|FLD24)))) return 1;

	if ((all_white((FLD31|FLD33|FLD23))) && (all_black((FLD22|FLD13))) && (all_empty((FLD12|FLD18|FLD27|FLD28)))) return 1;
	if ((all_white((FLD32|FLD34|FLD24))) && (all_black((FLD23|FLD14))) && (all_empty((FLD13|FLD19|FLD28|FLD29)))) return 1;
	if ((all_white((FLD33|FLD35|FLD25))) && (all_black((FLD24|FLD15))) && (all_empty((FLD14|FLD20|FLD29|FLD30)))) return 1;

	if ((all_white((FLD36|FLD38|FLD28))) && (all_black((FLD27|FLD18))) && (all_empty((FLD17|FLD22|FLD31|FLD32)))) return 1;
	if ((all_white((FLD37|FLD39|FLD29))) && (all_black((FLD28|FLD19))) && (all_empty((FLD18|FLD23|FLD32|FLD33)))) return 1;
	if ((all_white((FLD38|FLD40|FLD30))) && (all_black((FLD29|FLD20))) && (all_empty((FLD19|FLD24|FLD33|FLD34)))) return 1;

	if ((all_white((FLD26|FLD28|FLD16))) && (all_black((FLD17| FLD6))) && (all_empty((FLD7|FLD11|FLD21|FLD22)))) return 1;
	if ((all_white((FLD27|FLD29|FLD17))) && (all_black((FLD18| FLD7))) && (all_empty((FLD8|FLD12|FLD22|FLD23)))) return 1;
	if ((all_white((FLD28|FLD30|FLD18))) && (all_black((FLD19| FLD8))) && (all_empty((FLD9|FLD13|FLD23|FLD24)))) return 1;

	if ((all_white((FLD31|FLD33|FLD21))) && (all_black((FLD22|FLD11))) && (all_empty((FLD12|FLD17|FLD27|FLD28)))) return 1;
	if ((all_white((FLD32|FLD34|FLD22))) && (all_black((FLD23|FLD12))) && (all_empty((FLD13|FLD18|FLD28|FLD29)))) return 1;
	if ((all_white((FLD33|FLD35|FLD23))) && (all_black((FLD24|FLD13))) && (all_empty((FLD14|FLD19|FLD29|FLD30)))) return 1;

	if ((all_white((FLD36|FLD38|FLD26))) && (all_black((FLD27|FLD16))) && (all_empty((FLD17|FLD21|FLD31|FLD32)))) return 1;
	if ((all_white((FLD37|FLD39|FLD27))) && (all_black((FLD28|FLD17))) && (all_empty((FLD18|FLD22|FLD32|FLD33)))) return 1;
	if ((all_white((FLD38|FLD40|FLD28))) && (all_black((FLD29|FLD18))) && (all_empty((FLD19|FLD23|FLD33|FLD34)))) return 1;

	//een om twee paddestoel wit
	if (all_white((FLD16|FLD11)) && (empty_field & FLD7) && (black_man & FLD2)) {
		if ((any_piece & FLD1) && (black_man & FLD12) && (empty_field & FLD18)) return 1;
	}
	if (all_white((FLD21|FLD17|FLD12)) && (empty_field & FLD8) && (black_man & FLD3)) {
		if ((any_piece & FLD2) && (black_man & FLD13) && (empty_field & FLD19)) return 1;
	}
	if (all_white((FLD23|FLD18|FLD12)) && (empty_field & FLD7) && (black_man & FLD1)) {
		if ((any_piece & FLD2) && (black_man & FLD11) && (empty_field & FLD16)) return 1;
	}
	if (all_white((FLD22|FLD18|FLD13)) && (empty_field & FLD9) && (black_man & FLD4)) {
		if ((any_piece & FLD3) && (black_man & FLD14) && (empty_field & FLD20)) return 1;
	}
	if (all_white((FLD24|FLD19|FLD13)) && (empty_field & FLD8) && (black_man & FLD2)) {
		if ((any_piece & FLD3) && (black_man & FLD12) && (empty_field & FLD17)) return 1;
	}
	if (all_white((FLD25|FLD20|FLD14)) && (empty_field & FLD9) && (black_man & FLD3)) {
		if ((any_piece & FLD4) && (black_man & FLD13) && (empty_field & FLD18)) return 1;
	}
	if (all_white((FLD26|FLD21|FLD17)) && (empty_field & FLD12) && (black_man & FLD8)) {
		if (((any_piece & FLD7) && (black_man & FLD18) && (empty_field & FLD23)) ||
		   ((any_piece & FLD18) && (black_man & FLD7) && (empty_field & FLD1))) return 1;
	}
	if (all_white((FLD28|FLD22|FLD17)) && (empty_field & FLD11) && (black_man & FLD6)) {
		if ((any_piece & FLD16) && (black_man & FLD7) && (empty_field & FLD2)) return 1;
	}
	if (all_white((FLD27|FLD22|FLD18)) && (empty_field & FLD13) && (black_man & FLD9)) {
		if (((any_piece & FLD8) && (black_man & FLD19) && (empty_field & FLD24)) ||
		   ((any_piece & FLD19) && (black_man & FLD8) && (empty_field & FLD2))) return 1;
	}
	if (all_white((FLD29|FLD23|FLD18)) && (empty_field & FLD12) && (black_man & FLD7)) {
		if (((any_piece & FLD8) && (black_man & FLD17) && (empty_field & FLD21)) ||
		   ((any_piece & FLD17) && (black_man & FLD8) && (empty_field & FLD3))) return 1;
	}
	if (all_white((FLD28|FLD23|FLD19)) && (empty_field & FLD14) && (black_man & FLD10)) {
		if (((any_piece & FLD9) && (black_man & FLD20) && (empty_field & FLD25)) ||
		   ((any_piece & FLD20) && (black_man & FLD9) && (empty_field & FLD3))) return 1;
	}
	if (all_white((FLD30|FLD24|FLD19)) && (empty_field & FLD13) && (black_man & FLD8)) {
		if (((any_piece & FLD9) && (black_man & FLD18) && (empty_field & FLD22)) ||
		   ((any_piece & FLD18) && (black_man & FLD9) && (empty_field & FLD4))) return 1;
	}
	if (all_white((FLD25|FLD20)) && (empty_field & FLD14) && (black_man & FLD9)) {
		if (((any_piece & FLD10) && (black_man & FLD19) && (empty_field & FLD23)) ||
		   ((any_piece & FLD19) && (black_man & FLD10) && (empty_field & FLD5))) return 1;
	}
	if (all_white((FLD26|FLD21)) && (empty_field & FLD17) && (black_man & FLD12)) {
		if (((any_piece & FLD11) && (black_man & FLD22) && (empty_field & FLD28)) ||
		   ((any_piece & FLD22) && (black_man & FLD11) && (empty_field & FLD6))) return 1;
	}
	if (all_white((FLD31|FLD27|FLD22)) && (empty_field & FLD18) && (black_man & FLD13)) {
		if (((any_piece & FLD12) && (black_man & FLD23) && (empty_field & FLD29)) ||
		   ((any_piece & FLD23) && (black_man & FLD12) && (empty_field & FLD7))) return 1;
	}
	if (all_white((FLD33|FLD28|FLD22)) && (empty_field & FLD17) && (black_man & FLD11)) {
		if (((any_piece & FLD12) && (black_man & FLD21) && (empty_field & FLD26)) ||
		   ((any_piece & FLD21) && (black_man & FLD12) && (empty_field & FLD8))) return 1;
	}
	if (all_white((FLD32|FLD28|FLD23)) && (empty_field & FLD19) && (black_man & FLD14)) {
		if (((any_piece & FLD13) && (black_man & FLD24) && (empty_field & FLD30)) ||
		   ((any_piece & FLD24) && (black_man & FLD13) && (empty_field & FLD8))) return 1;
	}
	if (all_white((FLD34|FLD29|FLD23)) && (empty_field & FLD18) && (black_man & FLD12)) {
		if (((any_piece & FLD13) && (black_man & FLD22) && (empty_field & FLD27)) ||
		   ((any_piece & FLD22) && (black_man & FLD13) && (empty_field & FLD9))) return 1;
	}
	if (all_white((FLD33|FLD29|FLD24)) && (empty_field & FLD20) && (black_man & FLD15)) {
		if ((any_piece & FLD25) && (black_man & FLD14) && (empty_field & FLD9)) return 1;
	}
	if (all_white((FLD35|FLD30|FLD24)) && (empty_field & FLD19) && (black_man & FLD13)) {
		if (((any_piece & FLD14) && (black_man & FLD23) && (empty_field & FLD28)) ||
		   ((any_piece & FLD23) && (black_man & FLD14) && (empty_field & FLD10))) return 1;
	}
	if (all_white((FLD36|FLD31|FLD27)) && (empty_field & FLD22) && (black_man & FLD18)) {
		if (((any_piece & FLD17) && (black_man & FLD28) && (empty_field & FLD33)) ||
		   ((any_piece & FLD28) && (black_man & FLD17) && (empty_field & FLD11))) return 1;
	}
	if (all_white((FLD38|FLD32|FLD27)) && (empty_field & FLD21) && (black_man & FLD16)) {
		if ((any_piece & FLD26) && (black_man & FLD17) && (empty_field & FLD12)) return 1;
	}
	if (all_white((FLD37|FLD32|FLD28)) && (empty_field & FLD23) && (black_man & FLD19)) {
		if (((any_piece & FLD18) && (black_man & FLD29) && (empty_field & FLD34)) ||
		   ((any_piece & FLD29) && (black_man & FLD18) && (empty_field & FLD12))) return 1;
	}
	if (all_white((FLD39|FLD33|FLD28)) && (empty_field & FLD22) && (black_man & FLD17)) {
		if (((any_piece & FLD18) && (black_man & FLD27) && (empty_field & FLD31)) ||
		   ((any_piece & FLD27) && (black_man & FLD18) && (empty_field & FLD13))) return 1;
	}
	if (all_white((FLD38|FLD33|FLD29)) && (empty_field & FLD24) && (black_man & FLD20)) {
		if (((any_piece & FLD19) && (black_man & FLD30) && (empty_field & FLD35)) ||
		   ((any_piece & FLD30) && (black_man & FLD19) && (empty_field & FLD13))) return 1;
	}
	if (all_white((FLD40|FLD34|FLD29)) && (empty_field & FLD23) && (black_man & FLD18)) {
		if (((any_piece & FLD19) && (black_man & FLD28) && (empty_field & FLD32)) ||
		   ((any_piece & FLD28) && (black_man & FLD19) && (empty_field & FLD14))) return 1;
	}
	if (all_white((FLD35|FLD30)) && (empty_field & FLD24) && (black_man & FLD19)) {
		if (((any_piece & FLD20) && (black_man & FLD29) && (empty_field & FLD33)) ||
		   ((any_piece & FLD29) && (black_man & FLD20) && (empty_field & FLD15))) return 1;
	}
	if (all_white((FLD36|FLD31)) && (empty_field & FLD27) && (black_man & FLD22)) {
		if (((any_piece & FLD21) && (black_man & FLD32) && (empty_field & FLD38)) ||
		   ((any_piece & FLD32) && (black_man & FLD21) && (empty_field & FLD16))) return 1;
	}
	if (all_white((FLD41|FLD37|FLD32)) && (empty_field & FLD28) && (black_man & FLD23)) {
		if (((any_piece & FLD22) && (black_man & FLD33) && (empty_field & FLD39)) ||
		   ((any_piece & FLD33) && (black_man & FLD22) && (empty_field & FLD17))) return 1;
	}
	if (all_white((FLD43|FLD38|FLD32)) && (empty_field & FLD27) && (black_man & FLD21)) {
		if (((any_piece & FLD22) && (black_man & FLD31) && (empty_field & FLD36)) ||
		   ((any_piece & FLD31) && (black_man & FLD22) && (empty_field & FLD18))) return 1;
	}
	if (all_white((FLD42|FLD38|FLD33)) && (empty_field & FLD29) && (black_man & FLD24)) {
		if (((any_piece & FLD23) && (black_man & FLD34) && (empty_field & FLD40)) ||
		   ((any_piece & FLD34) && (black_man & FLD23) && (empty_field & FLD18))) return 1;
	}
	if (all_white((FLD44|FLD39|FLD33)) && (empty_field & FLD28) && (black_man & FLD22)) {
		if (((any_piece & FLD23) && (black_man & FLD32) && (empty_field & FLD37)) ||
		   ((any_piece & FLD32) && (black_man & FLD23) && (empty_field & FLD19))) return 1;
	}
	if (all_white((FLD43|FLD39|FLD34)) && (empty_field & FLD30) && (black_man & FLD25)) {
		if ((any_piece & FLD35) && (black_man & FLD24) && (empty_field & FLD19)) return 1;
	}
	if (all_white((FLD45|FLD40|FLD34)) && (empty_field & FLD29) && (black_man & FLD23)) {
		if (((any_piece & FLD24) && (black_man & FLD33) && (empty_field & FLD38)) ||
		   ((any_piece & FLD33) && (black_man & FLD24) && (empty_field & FLD20))) return 1;
	}
	if (all_white((FLD46|FLD41|FLD37)) && (empty_field & FLD32) && (black_man & FLD28)) {
		if (((any_piece & FLD27) && (black_man & FLD38) && (empty_field & FLD43)) ||
		   ((any_piece & FLD38) && (black_man & FLD27) && (empty_field & FLD21))) return 1;
	}
	if (all_white((FLD48|FLD42|FLD37)) && (empty_field & FLD31) && (black_man & FLD26)) {
		if ((any_piece & FLD36) && (black_man & FLD27) && (empty_field & FLD22)) return 1;
	}
	if (all_white((FLD47|FLD42|FLD38)) && (empty_field & FLD33) && (black_man & FLD29)) {
		if (((any_piece & FLD28) && (black_man & FLD39) && (empty_field & FLD44)) ||
		   ((any_piece & FLD39) && (black_man & FLD28) && (empty_field & FLD22))) return 1;
	}
	if (all_white((FLD49|FLD43|FLD38)) && (empty_field & FLD32) && (black_man & FLD27)) {
		if (((any_piece & FLD28) && (black_man & FLD37) && (empty_field & FLD41)) ||
		   ((any_piece & FLD37) && (black_man & FLD28) && (empty_field & FLD23))) return 1;
	}
	if (all_white((FLD48|FLD43|FLD39)) && (empty_field & FLD34) && (black_man & FLD30)) {
		if (((any_piece & FLD29) && (black_man & FLD40) && (empty_field & FLD45)) ||
		   ((any_piece & FLD40) && (black_man & FLD29) && (empty_field & FLD23))) return 1;
	}
	if (all_white((FLD50|FLD44|FLD39)) && (empty_field & FLD33) && (black_man & FLD28)) {
		if (((any_piece & FLD29) && (black_man & FLD38) && (empty_field & FLD42)) ||
		   ((any_piece & FLD38) && (black_man & FLD29) && (empty_field & FLD24))) return 1;
	}
	if (all_white((FLD45|FLD40)) && (empty_field & FLD34) && (black_man & FLD29)) {
		if (((any_piece & FLD30) && (black_man & FLD39) && (empty_field & FLD43)) ||
		   ((any_piece & FLD39) && (black_man & FLD30) && (empty_field & FLD25))) return 1;
	}

	// white pootje


	if (all_white(FLD23|FLD18|FLD12)  && all_empty(FLD8|FLD7) && (black_man & FLD3))
		if ((all_occupied(FLD13|FLD2))  || (all_empty_or_white(FLD13|FLD2))) {
			if ((black_man & FLD11) && (empty_field & FLD16)) return 1;
	}
	if (all_white(FLD23|FLD18|FLD13)  && all_empty(FLD8|FLD7|FLD12) && (black_man & FLD3))
		if (empty_or_white&FLD2) {
			if ((black_man & FLD11) && (empty_field & FLD16)) return 1;
	}
	if (all_white(FLD24|FLD19|FLD13)  && all_empty(FLD9|FLD8) && (black_man & FLD4))
		if ((all_occupied(FLD14|FLD3))  || (all_empty_or_white(FLD14|FLD3))) {
			if ((black_man & FLD12) && (empty_field & FLD17)) return 1;
	}
	if (all_white(FLD24|FLD19|FLD14)  && all_empty(FLD9|FLD8|FLD13) && (black_man & FLD4))
		if (empty_or_white&FLD3) {
			if ((black_man & FLD12) && (empty_field & FLD17)) return 1;
	}
	if (all_white(FLD25|FLD20|FLD14)  && all_empty(FLD10|FLD9) && (black_man & FLD5))
		if ((all_occupied(FLD15|FLD4))  || (all_empty_or_white(FLD15|FLD4))) {
			if ((black_man & FLD13) && (empty_field & FLD18)) return 1;
	}
	if (all_white(FLD25|FLD20|FLD15)  && all_empty(FLD10|FLD9|FLD14) && (black_man & FLD5))
		if (empty_or_white&FLD4) {
			if ((black_man & FLD13) && (empty_field & FLD18)) return 1;
	}
	if (all_white(FLD28|FLD22|FLD17)  && all_empty(FLD12|FLD11) && (black_man & FLD8))
		if ((all_occupied(FLD18|FLD7))  || (all_empty_or_white(FLD18|FLD7))) {
			if ((black_man & FLD7) && (empty_field & FLD2)) return 1;
	}
	if (all_white(FLD28|FLD22|FLD18)  && all_empty(FLD12|FLD11|FLD17) && (black_man & FLD8))
		if (empty_or_white&FLD7) {
			if ((black_man & FLD7) && (empty_field & FLD2)) return 1;
	}
	if (all_white(FLD29|FLD23|FLD18)  && all_empty(FLD13|FLD12) && (black_man & FLD9))
		if ((all_occupied(FLD19|FLD8))  || (all_empty_or_white(FLD19|FLD8))) {
			if ((black_man & FLD7) && (empty_field & FLD1)) return 1;
			if ((black_man & FLD17) && (empty_field & FLD21)) return 1;
			if ((black_man & FLD8) && (empty_field & FLD3)) return 1;
	}
	if (all_white(FLD29|FLD23|FLD19)  && all_empty(FLD13|FLD12|FLD18) && (black_man & FLD9))
		if (empty_or_white&FLD8) {
			if ((black_man & FLD7) && (empty_field & FLD1)) return 1;
			if ((black_man & FLD17) && (empty_field & FLD21)) return 1;
			if ((black_man & FLD8) && (empty_field & FLD3)) return 1;
	}
	if (all_white(FLD30|FLD24|FLD19)  && all_empty(FLD14|FLD13) && (black_man & FLD10))
		if ((all_occupied(FLD20|FLD9))  || (all_empty_or_white(FLD20|FLD9))) {
			if ((black_man & FLD8) && (empty_field & FLD2)) return 1;
			if ((black_man & FLD18) && (empty_field & FLD22)) return 1;
			if ((black_man & FLD9) && (empty_field & FLD4)) return 1;
	}
	if (all_white(FLD30|FLD24|FLD20)  && all_empty(FLD14|FLD13|FLD19) && (black_man & FLD10))
		if (empty_or_white&FLD9) {
			if ((black_man & FLD8) && (empty_field & FLD2)) return 1;
			if ((black_man & FLD18) && (empty_field & FLD22)) return 1;
			if ((black_man & FLD9) && (empty_field & FLD4)) return 1;
	}
	if (all_white(FLD32|FLD27|FLD21)  && all_empty(FLD17|FLD16) && (black_man & FLD12))
		if ((all_occupied(FLD22|FLD11))  || (all_empty_or_white(FLD22|FLD11))) {
			if ((black_man & FLD11) && (empty_field & FLD7)) return 1;
	}
	if (all_white(FLD32|FLD27|FLD22)  && all_empty(FLD17|FLD16|FLD21) && (black_man & FLD12))
		if (empty_or_white&FLD11) {
			if ((black_man & FLD11) && (empty_field & FLD7)) return 1;
	}
	if (all_white(FLD33|FLD28|FLD22)  && all_empty(FLD18|FLD17) && (black_man & FLD13))
		if ((all_occupied(FLD23|FLD12))  || (all_empty_or_white(FLD23|FLD12))) {
			if ((black_man & FLD11) && (empty_field & FLD6)) return 1;
			if ((black_man & FLD21) && (empty_field & FLD26)) return 1;
			if ((black_man & FLD12) && (empty_field & FLD8)) return 1;
	}
	if (all_white(FLD33|FLD28|FLD23)  && all_empty(FLD18|FLD17|FLD22) && (black_man & FLD13))
		if (empty_or_white&FLD12) {
			if ((black_man & FLD11) && (empty_field & FLD6)) return 1;
			if ((black_man & FLD21) && (empty_field & FLD26)) return 1;
			if ((black_man & FLD12) && (empty_field & FLD8)) return 1;
	}
	if (all_white(FLD34|FLD29|FLD23)  && all_empty(FLD19|FLD18) && (black_man & FLD14))
		if ((all_occupied(FLD24|FLD13))  || (all_empty_or_white(FLD24|FLD13))) {
			if ((black_man & FLD12) && (empty_field & FLD7)) return 1;
			if ((black_man & FLD22) && (empty_field & FLD27)) return 1;
			if ((black_man & FLD13) && (empty_field & FLD9)) return 1;
	}
	if (all_white(FLD34|FLD29|FLD24)  && all_empty(FLD19|FLD18|FLD23) && (black_man & FLD14))
		if (empty_or_white&FLD13) {
			if ((black_man & FLD12) && (empty_field & FLD7)) return 1;
			if ((black_man & FLD22) && (empty_field & FLD27)) return 1;
			if ((black_man & FLD13) && (empty_field & FLD9)) return 1;
	}
	if (all_white(FLD35|FLD30|FLD24)  && all_empty(FLD20|FLD19) && (black_man & FLD15))
		if ((all_occupied(FLD25|FLD14))  || (all_empty_or_white(FLD25|FLD14))) {
			if ((black_man & FLD13) && (empty_field & FLD8)) return 1;
			if ((black_man & FLD23) && (empty_field & FLD28)) return 1;
			if ((black_man & FLD14) && (empty_field & FLD10)) return 1;
	}
	if (all_white(FLD35|FLD30|FLD25)  && all_empty(FLD20|FLD19|FLD24) && (black_man & FLD15))
		if (empty_or_white&FLD14) {
			if ((black_man & FLD13) && (empty_field & FLD8)) return 1;
			if ((black_man & FLD23) && (empty_field & FLD28)) return 1;
			if ((black_man & FLD14) && (empty_field & FLD10)) return 1;
	}
	if (all_white(FLD38|FLD32|FLD27)  && all_empty(FLD22|FLD21) && (black_man & FLD18))
		if ((all_occupied(FLD28|FLD17))  || (all_empty_or_white(FLD28|FLD17))) {
			if ((black_man & FLD17) && (empty_field & FLD12)) return 1;
	}
	if (all_white(FLD38|FLD32|FLD28)  && all_empty(FLD22|FLD21|FLD27) && (black_man & FLD18))
		if (empty_or_white&FLD17) {
			if ((black_man & FLD17) && (empty_field & FLD12)) return 1;
	}
	if (all_white(FLD39|FLD33|FLD28)  && all_empty(FLD23|FLD22) && (black_man & FLD19))
		if ((all_occupied(FLD29|FLD18))  || (all_empty_or_white(FLD29|FLD18))) {
			if ((black_man & FLD17) && (empty_field & FLD11)) return 1;
			if ((black_man & FLD27) && (empty_field & FLD31)) return 1;
			if ((black_man & FLD18) && (empty_field & FLD13)) return 1;
	}
	if (all_white(FLD39|FLD33|FLD29)  && all_empty(FLD23|FLD22|FLD28) && (black_man & FLD19))
		if (empty_or_white&FLD18) {
			if ((black_man & FLD17) && (empty_field & FLD11)) return 1;
			if ((black_man & FLD27) && (empty_field & FLD31)) return 1;
			if ((black_man & FLD18) && (empty_field & FLD13)) return 1;
	}
	if (all_white(FLD40|FLD34|FLD29)  && all_empty(FLD24|FLD23) && (black_man & FLD20))
		if ((all_occupied(FLD30|FLD19))  || (all_empty_or_white(FLD30|FLD19))) {
			if ((black_man & FLD18) && (empty_field & FLD12)) return 1;
			if ((black_man & FLD28) && (empty_field & FLD32)) return 1;
			if ((black_man & FLD19) && (empty_field & FLD14)) return 1;
	}
	if (all_white(FLD40|FLD34|FLD30)  && all_empty(FLD24|FLD23|FLD29) && (black_man & FLD20))
		if (empty_or_white&FLD19) {
			if ((black_man & FLD18) && (empty_field & FLD12)) return 1;
			if ((black_man & FLD28) && (empty_field & FLD32)) return 1;
			if ((black_man & FLD19) && (empty_field & FLD14)) return 1;
	}
	if (all_white(FLD21|FLD17|FLD12)  && all_empty(FLD7|FLD8) && (black_man & FLD1))
		if ((all_occupied(FLD11|FLD2))  || (all_empty_or_white(FLD11|FLD2))) {
			if ((black_man & FLD13) && (empty_field & FLD19)) return 1;
	}
	if (all_white(FLD21|FLD17|FLD11)  && all_empty(FLD7|FLD8|FLD12) && (black_man & FLD1))
		if (empty_or_white&FLD2) {
			if ((black_man & FLD13) && (empty_field & FLD19)) return 1;
	}
	if (all_white(FLD22|FLD18|FLD13)  && all_empty(FLD8|FLD9) && (black_man & FLD2))
		if ((all_occupied(FLD12|FLD3))  || (all_empty_or_white(FLD12|FLD3))) {
			if ((black_man & FLD14) && (empty_field & FLD20)) return 1;
	}
	if (all_white(FLD22|FLD18|FLD12)  && all_empty(FLD8|FLD9|FLD13) && (black_man & FLD2))
		if (empty_or_white&FLD3) {
			if ((black_man & FLD14) && (empty_field & FLD20)) return 1;
	}
	if (all_white(FLD26|FLD21|FLD17)  && all_empty(FLD11|FLD12) && (black_man & FLD6))
		if ((all_occupied(FLD16|FLD7))  || (all_empty_or_white(FLD16|FLD7))) {
			if ((black_man & FLD8) && (empty_field & FLD3)) return 1;
			if ((black_man & FLD18) && (empty_field & FLD23)) return 1;
			if ((black_man & FLD7) && (empty_field & FLD1)) return 1;
	}
	if (all_white(FLD26|FLD21|FLD16)  && all_empty(FLD11|FLD12|FLD17) && (black_man & FLD6))
		if (empty_or_white&FLD7) {
			if ((black_man & FLD8) && (empty_field & FLD3)) return 1;
			if ((black_man & FLD18) && (empty_field & FLD23)) return 1;
			if ((black_man & FLD7) && (empty_field & FLD1)) return 1;
	}
	if (all_white(FLD27|FLD22|FLD18)  && all_empty(FLD12|FLD13) && (black_man & FLD7))
		if ((all_occupied(FLD17|FLD8))  || (all_empty_or_white(FLD17|FLD8))) {
			if ((black_man & FLD9) && (empty_field & FLD4)) return 1;
			if ((black_man & FLD19) && (empty_field & FLD24)) return 1;
			if ((black_man & FLD8) && (empty_field & FLD2)) return 1;
	}
	if (all_white(FLD27|FLD22|FLD17)  && all_empty(FLD12|FLD13|FLD18) && (black_man & FLD7))
		if (empty_or_white&FLD8) {
			if ((black_man & FLD9) && (empty_field & FLD4)) return 1;
			if ((black_man & FLD19) && (empty_field & FLD24)) return 1;
			if ((black_man & FLD8) && (empty_field & FLD2)) return 1;
	}
	if (all_white(FLD28|FLD23|FLD19)  && all_empty(FLD13|FLD14) && (black_man & FLD8))
		if ((all_occupied(FLD18|FLD9))  || (all_empty_or_white(FLD18|FLD9))) {
			if ((black_man & FLD10) && (empty_field & FLD5)) return 1;
			if ((black_man & FLD20) && (empty_field & FLD25)) return 1;
			if ((black_man & FLD9) && (empty_field & FLD3)) return 1;
	}
	if (all_white(FLD28|FLD23|FLD18)  && all_empty(FLD13|FLD14|FLD19) && (black_man & FLD8))
		if (empty_or_white&FLD9) {
			if ((black_man & FLD10) && (empty_field & FLD5)) return 1;
			if ((black_man & FLD20) && (empty_field & FLD25)) return 1;
			if ((black_man & FLD9) && (empty_field & FLD3)) return 1;
	}
	if (all_white(FLD29|FLD24|FLD20)  && all_empty(FLD14|FLD15) && (black_man & FLD9))
		if ((all_occupied(FLD19|FLD10))  || (all_empty_or_white(FLD19|FLD10))) {
			if ((black_man & FLD10) && (empty_field & FLD4)) return 1;
	}
	if (all_white(FLD29|FLD24|FLD19)  && all_empty(FLD14|FLD15|FLD20) && (black_man & FLD9))
		if (empty_or_white&FLD10) {
			if ((black_man & FLD10) && (empty_field & FLD4)) return 1;
	}
	if (all_white(FLD31|FLD27|FLD22)  && all_empty(FLD17|FLD18) && (black_man & FLD11))
		if ((all_occupied(FLD21|FLD12))  || (all_empty_or_white(FLD21|FLD12))) {
			if ((black_man & FLD13) && (empty_field & FLD9)) return 1;
			if ((black_man & FLD23) && (empty_field & FLD29)) return 1;
			if ((black_man & FLD12) && (empty_field & FLD7)) return 1;
	}
	if (all_white(FLD31|FLD27|FLD21)  && all_empty(FLD17|FLD18|FLD22) && (black_man & FLD11))
		if (empty_or_white&FLD12) {
			if ((black_man & FLD13) && (empty_field & FLD9)) return 1;
			if ((black_man & FLD23) && (empty_field & FLD29)) return 1;
			if ((black_man & FLD12) && (empty_field & FLD7)) return 1;
	}
	if (all_white(FLD32|FLD28|FLD23)  && all_empty(FLD18|FLD19) && (black_man & FLD12))
		if ((all_occupied(FLD22|FLD13))  || (all_empty_or_white(FLD22|FLD13))) {
			if ((black_man & FLD14) && (empty_field & FLD10)) return 1;
			if ((black_man & FLD24) && (empty_field & FLD30)) return 1;
			if ((black_man & FLD13) && (empty_field & FLD8)) return 1;
	}
	if (all_white(FLD32|FLD28|FLD22)  && all_empty(FLD18|FLD19|FLD23) && (black_man & FLD12))
		if (empty_or_white&FLD13) {
			if ((black_man & FLD14) && (empty_field & FLD10)) return 1;
			if ((black_man & FLD24) && (empty_field & FLD30)) return 1;
			if ((black_man & FLD13) && (empty_field & FLD8)) return 1;
	}
	if (all_white(FLD33|FLD29|FLD24)  && all_empty(FLD19|FLD20) && (black_man & FLD13))
		if ((all_occupied(FLD23|FLD14))  || (all_empty_or_white(FLD23|FLD14))) {
			if ((black_man & FLD14) && (empty_field & FLD9)) return 1;
	}
	if (all_white(FLD33|FLD29|FLD23)  && all_empty(FLD19|FLD20|FLD24) && (black_man & FLD13))
		if (empty_or_white&FLD14) {
			if ((black_man & FLD14) && (empty_field & FLD9)) return 1;
	}
	if (all_white(FLD36|FLD31|FLD27)  && all_empty(FLD21|FLD22) && (black_man & FLD16))
		if ((all_occupied(FLD26|FLD17))  || (all_empty_or_white(FLD26|FLD17))) {
			if ((black_man & FLD18) && (empty_field & FLD13)) return 1;
			if ((black_man & FLD28) && (empty_field & FLD33)) return 1;
			if ((black_man & FLD17) && (empty_field & FLD11)) return 1;
	}
	if (all_white(FLD36|FLD31|FLD26)  && all_empty(FLD21|FLD22|FLD27) && (black_man & FLD16))
		if (empty_or_white&FLD17) {
			if ((black_man & FLD18) && (empty_field & FLD13)) return 1;
			if ((black_man & FLD28) && (empty_field & FLD33)) return 1;
			if ((black_man & FLD17) && (empty_field & FLD11)) return 1;
	}
	if (all_white(FLD37|FLD32|FLD28)  && all_empty(FLD22|FLD23) && (black_man & FLD17))
		if ((all_occupied(FLD27|FLD18))  || (all_empty_or_white(FLD27|FLD18))) {
			if ((black_man & FLD19) && (empty_field & FLD14)) return 1;
			if ((black_man & FLD29) && (empty_field & FLD34)) return 1;
			if ((black_man & FLD18) && (empty_field & FLD12)) return 1;
	}
	if (all_white(FLD37|FLD32|FLD27)  && all_empty(FLD22|FLD23|FLD28) && (black_man & FLD17))
		if (empty_or_white&FLD18) {
			if ((black_man & FLD19) && (empty_field & FLD14)) return 1;
			if ((black_man & FLD29) && (empty_field & FLD34)) return 1;
			if ((black_man & FLD18) && (empty_field & FLD12)) return 1;
	}
	if (all_white(FLD38|FLD33|FLD29)  && all_empty(FLD23|FLD24) && (black_man & FLD18))
		if ((all_occupied(FLD28|FLD19))  || (all_empty_or_white(FLD28|FLD19))) {
			if ((black_man & FLD20) && (empty_field & FLD15)) return 1;
			if ((black_man & FLD30) && (empty_field & FLD35)) return 1;
			if ((black_man & FLD19) && (empty_field & FLD13)) return 1;
	}
	if (all_white(FLD38|FLD33|FLD28)  && all_empty(FLD23|FLD24|FLD29) && (black_man & FLD18))
		if (empty_or_white&FLD19) {
			if ((black_man & FLD20) && (empty_field & FLD15)) return 1;
			if ((black_man & FLD30) && (empty_field & FLD35)) return 1;
			if ((black_man & FLD19) && (empty_field & FLD13)) return 1;
	}
	if (all_white(FLD39|FLD34|FLD30)  && all_empty(FLD24|FLD25) && (black_man & FLD19))
		if ((all_occupied(FLD29|FLD20))  || (all_empty_or_white(FLD29|FLD20))) {
			if ((black_man & FLD20) && (empty_field & FLD14)) return 1;
	}
	if (all_white(FLD39|FLD34|FLD29)  && all_empty(FLD24|FLD25|FLD30) && (black_man & FLD19))
		if (empty_or_white&FLD20) {
			if ((black_man & FLD20) && (empty_field & FLD14)) return 1;
	}

	return 0;
}


int increase_depth_black_on_move(NodePnt node)
{	BitArray white_man = (node->occ_empty) & (node->white_black) & (node->man_king);
	BitArray black_man = (node->occ_empty) & ~(node->white_black) & (node->man_king);
	BitArray empty_field = ~(node->occ_empty) & BITS50;
	BitArray any_piece = (node->occ_empty) & BITS50;


	if ((all_black((BLD26|BLD28|BLD18))) && (all_white((BLD17| BLD8))) && (all_empty((BLD7|BLD12|BLD21|BLD22)))) return 1;
	if ((all_black((BLD27|BLD29|BLD19))) && (all_white((BLD18| BLD9))) && (all_empty((BLD8|BLD13|BLD22|BLD23)))) return 1;
	if ((all_black((BLD28|BLD30|BLD20))) && (all_white((BLD19|BLD10))) && (all_empty((BLD9|BLD14|BLD23|BLD24)))) return 1;

	if ((all_black((BLD31|BLD33|BLD23))) && (all_white((BLD22|BLD13))) && (all_empty((BLD12|BLD18|BLD27|BLD28)))) return 1;
	if ((all_black((BLD32|BLD34|BLD24))) && (all_white((BLD23|BLD14))) && (all_empty((BLD13|BLD19|BLD28|BLD29)))) return 1;
	if ((all_black((BLD33|BLD35|BLD25))) && (all_white((BLD24|BLD15))) && (all_empty((BLD14|BLD20|BLD29|BLD30)))) return 1;

	if ((all_black((BLD36|BLD38|BLD28))) && (all_white((BLD27|BLD18))) && (all_empty((BLD17|BLD22|BLD31|BLD32)))) return 1;
	if ((all_black((BLD37|BLD39|BLD29))) && (all_white((BLD28|BLD19))) && (all_empty((BLD18|BLD23|BLD32|BLD33)))) return 1;
	if ((all_black((BLD38|BLD40|BLD30))) && (all_white((BLD29|BLD20))) && (all_empty((BLD19|BLD24|BLD33|BLD34)))) return 1;

	if ((all_black((BLD26|BLD28|BLD16))) && (all_white((BLD17| BLD6))) && (all_empty((BLD7|BLD11|BLD21|BLD22)))) return 1;
	if ((all_black((BLD27|BLD29|BLD17))) && (all_white((BLD18| BLD7))) && (all_empty((BLD8|BLD12|BLD22|BLD23)))) return 1;
	if ((all_black((BLD28|BLD30|BLD18))) && (all_white((BLD19| BLD8))) && (all_empty((BLD9|BLD13|BLD23|BLD24)))) return 1;

	if ((all_black((BLD31|BLD33|BLD21))) && (all_white((BLD22|BLD11))) && (all_empty((BLD12|BLD17|BLD27|BLD28)))) return 1;
	if ((all_black((BLD32|BLD34|BLD22))) && (all_white((BLD23|BLD12))) && (all_empty((BLD13|BLD18|BLD28|BLD29)))) return 1;
	if ((all_black((BLD33|BLD35|BLD23))) && (all_white((BLD24|BLD13))) && (all_empty((BLD14|BLD19|BLD29|BLD30)))) return 1;

	if ((all_black((BLD36|BLD38|BLD26))) && (all_white((BLD27|BLD16))) && (all_empty((BLD17|BLD21|BLD31|BLD32)))) return 1;
	if ((all_black((BLD37|BLD39|BLD27))) && (all_white((BLD28|BLD17))) && (all_empty((BLD18|BLD22|BLD32|BLD33)))) return 1;
	if ((all_black((BLD38|BLD40|BLD28))) && (all_white((BLD29|BLD18))) && (all_empty((BLD19|BLD23|BLD33|BLD34)))) return 1;

	//een om twee paddestoel zwart
	if (all_black((FLD6|FLD11)) && (empty_field & FLD17) && (white_man & FLD22)) {
		if (((any_piece & FLD21) && (white_man & FLD12) && (empty_field & FLD8)) ||
		   ((any_piece & FLD12) && (white_man & FLD21) && (empty_field & FLD26))) return 1;
	}
	if (all_black((FLD1|FLD7|FLD12)) && (empty_field & FLD18) && (white_man & FLD23)) {
		if (((any_piece & FLD22) && (white_man & FLD13) && (empty_field & FLD9)) ||
		   ((any_piece & FLD13) && (white_man & FLD22) && (empty_field & FLD27))) return 1;
	}
	if (all_black((FLD3|FLD8|FLD12)) && (empty_field & FLD17) && (white_man & FLD21)) {
		if (((any_piece & FLD22) && (white_man & FLD11) && (empty_field & FLD6)) ||
		   ((any_piece & FLD11) && (white_man & FLD22) && (empty_field & FLD28))) return 1;
	}
	if (all_black((FLD2|FLD8|FLD13)) && (empty_field & FLD19) && (white_man & FLD24)) {
		if (((any_piece & FLD23) && (white_man & FLD14) && (empty_field & FLD10)) ||
		   ((any_piece & FLD14) && (white_man & FLD23) && (empty_field & FLD28))) return 1;
	}
	if (all_black((FLD4|FLD9|FLD13)) && (empty_field & FLD18) && (white_man & FLD22)) {
		if (((any_piece & FLD23) && (white_man & FLD12) && (empty_field & FLD7)) ||
		   ((any_piece & FLD12) && (white_man & FLD23) && (empty_field & FLD29))) return 1;
	}
	if (all_black((FLD3|FLD9|FLD14)) && (empty_field & FLD20) && (white_man & FLD25)) {
		if ((any_piece & FLD15) && (white_man & FLD24) && (empty_field & FLD29)) return 1;
	}
	if (all_black((FLD5|FLD10|FLD14)) && (empty_field & FLD19) && (white_man & FLD23)) {
		if (((any_piece & FLD24) && (white_man & FLD13) && (empty_field & FLD8)) ||
		   ((any_piece & FLD13) && (white_man & FLD24) && (empty_field & FLD30))) return 1;
	}
	if (all_black((FLD6|FLD11|FLD17)) && (empty_field & FLD22) && (white_man & FLD28)) {
		if (((any_piece & FLD27) && (white_man & FLD18) && (empty_field & FLD13)) ||
		   ((any_piece & FLD18) && (white_man & FLD27) && (empty_field & FLD31))) return 1;
	}
	if (all_black((FLD8|FLD12|FLD17)) && (empty_field & FLD21) && (white_man & FLD26)) {
		if ((any_piece & FLD16) && (white_man & FLD27) && (empty_field & FLD32)) return 1;
	}
	if (all_black((FLD7|FLD12|FLD18)) && (empty_field & FLD23) && (white_man & FLD29)) {
		if (((any_piece & FLD28) && (white_man & FLD19) && (empty_field & FLD14)) ||
		   ((any_piece & FLD19) && (white_man & FLD28) && (empty_field & FLD32))) return 1;
	}
	if (all_black((FLD9|FLD13|FLD18)) && (empty_field & FLD22) && (white_man & FLD27)) {
		if (((any_piece & FLD28) && (white_man & FLD17) && (empty_field & FLD11)) ||
		   ((any_piece & FLD17) && (white_man & FLD28) && (empty_field & FLD33))) return 1;
	}
	if (all_black((FLD8|FLD13|FLD19)) && (empty_field & FLD24) && (white_man & FLD30)) {
		if (((any_piece & FLD29) && (white_man & FLD20) && (empty_field & FLD15)) ||
		   ((any_piece & FLD20) && (white_man & FLD29) && (empty_field & FLD33))) return 1;
	}
	if (all_black((FLD10|FLD14|FLD19)) && (empty_field & FLD23) && (white_man & FLD28)) {
		if (((any_piece & FLD29) && (white_man & FLD18) && (empty_field & FLD12)) ||
		   ((any_piece & FLD18) && (white_man & FLD29) && (empty_field & FLD34))) return 1;
	}
	if (all_black((FLD15|FLD20)) && (empty_field & FLD24) && (white_man & FLD29)) {
		if (((any_piece & FLD30) && (white_man & FLD19) && (empty_field & FLD13)) ||
		   ((any_piece & FLD19) && (white_man & FLD30) && (empty_field & FLD35))) return 1;
	}
	if (all_black((FLD16|FLD21)) && (empty_field & FLD27) && (white_man & FLD32)) {
		if (((any_piece & FLD31) && (white_man & FLD22) && (empty_field & FLD18)) ||
		   ((any_piece & FLD22) && (white_man & FLD31) && (empty_field & FLD36))) return 1;
	}
	if (all_black((FLD11|FLD17|FLD22)) && (empty_field & FLD28) && (white_man & FLD33)) {
		if (((any_piece & FLD32) && (white_man & FLD23) && (empty_field & FLD19)) ||
		   ((any_piece & FLD23) && (white_man & FLD32) && (empty_field & FLD37))) return 1;
	}
	if (all_black((FLD13|FLD18|FLD22)) && (empty_field & FLD27) && (white_man & FLD31)) {
		if (((any_piece & FLD32) && (white_man & FLD21) && (empty_field & FLD16)) ||
		   ((any_piece & FLD21) && (white_man & FLD32) && (empty_field & FLD38))) return 1;
	}
	if (all_black((FLD12|FLD18|FLD23)) && (empty_field & FLD29) && (white_man & FLD34)) {
		if (((any_piece & FLD33) && (white_man & FLD24) && (empty_field & FLD20)) ||
		   ((any_piece & FLD24) && (white_man & FLD33) && (empty_field & FLD38))) return 1;
	}
	if (all_black((FLD14|FLD19|FLD23)) && (empty_field & FLD28) && (white_man & FLD32)) {
		if (((any_piece & FLD33) && (white_man & FLD22) && (empty_field & FLD17)) ||
		   ((any_piece & FLD22) && (white_man & FLD33) && (empty_field & FLD39))) return 1;
	}
	if (all_black((FLD13|FLD19|FLD24)) && (empty_field & FLD30) && (white_man & FLD35)) {
		if ((any_piece & FLD25) && (white_man & FLD34) && (empty_field & FLD39)) return 1;
	}
	if (all_black((FLD15|FLD20|FLD24)) && (empty_field & FLD29) && (white_man & FLD33)) {
		if (((any_piece & FLD34) && (white_man & FLD23) && (empty_field & FLD18)) ||
		   ((any_piece & FLD23) && (white_man & FLD34) && (empty_field & FLD40))) return 1;
	}
	if (all_black((FLD16|FLD21|FLD27)) && (empty_field & FLD32) && (white_man & FLD38)) {
		if (((any_piece & FLD37) && (white_man & FLD28) && (empty_field & FLD23)) ||
		   ((any_piece & FLD28) && (white_man & FLD37) && (empty_field & FLD41))) return 1;
	}
	if (all_black((FLD18|FLD22|FLD27)) && (empty_field & FLD31) && (white_man & FLD36)) {
		if ((any_piece & FLD26) && (white_man & FLD37) && (empty_field & FLD42)) return 1;
	}
	if (all_black((FLD17|FLD22|FLD28)) && (empty_field & FLD33) && (white_man & FLD39)) {
		if (((any_piece & FLD38) && (white_man & FLD29) && (empty_field & FLD24)) ||
		   ((any_piece & FLD29) && (white_man & FLD38) && (empty_field & FLD42))) return 1;
	}
	if (all_black((FLD19|FLD23|FLD28)) && (empty_field & FLD32) && (white_man & FLD37)) {
		if (((any_piece & FLD38) && (white_man & FLD27) && (empty_field & FLD21)) ||
		   ((any_piece & FLD27) && (white_man & FLD38) && (empty_field & FLD43))) return 1;
	}
	if (all_black((FLD18|FLD23|FLD29)) && (empty_field & FLD34) && (white_man & FLD40)) {
		if (((any_piece & FLD39) && (white_man & FLD30) && (empty_field & FLD25)) ||
		   ((any_piece & FLD30) && (white_man & FLD39) && (empty_field & FLD43))) return 1;
	}
	if (all_black((FLD20|FLD24|FLD29)) && (empty_field & FLD33) && (white_man & FLD38)) {
		if (((any_piece & FLD39) && (white_man & FLD28) && (empty_field & FLD22)) ||
		   ((any_piece & FLD28) && (white_man & FLD39) && (empty_field & FLD44))) return 1;
	}
	if (all_black((FLD25|FLD30)) && (empty_field & FLD34) && (white_man & FLD39)) {
		if (((any_piece & FLD40) && (white_man & FLD29) && (empty_field & FLD23)) ||
		   ((any_piece & FLD29) && (white_man & FLD40) && (empty_field & FLD45))) return 1;
	}
	if (all_black((FLD26|FLD31)) && (empty_field & FLD37) && (white_man & FLD42)) {
		if (((any_piece & FLD41) && (white_man & FLD32) && (empty_field & FLD28)) ||
		   ((any_piece & FLD32) && (white_man & FLD41) && (empty_field & FLD46))) return 1;
	}
	if (all_black((FLD21|FLD27|FLD32)) && (empty_field & FLD38) && (white_man & FLD43)) {
		if (((any_piece & FLD42) && (white_man & FLD33) && (empty_field & FLD29)) ||
		   ((any_piece & FLD33) && (white_man & FLD42) && (empty_field & FLD47))) return 1;
	}
	if (all_black((FLD23|FLD28|FLD32)) && (empty_field & FLD37) && (white_man & FLD41)) {
		if (((any_piece & FLD42) && (white_man & FLD31) && (empty_field & FLD26)) ||
		   ((any_piece & FLD31) && (white_man & FLD42) && (empty_field & FLD48))) return 1;
	}
	if (all_black((FLD22|FLD28|FLD33)) && (empty_field & FLD39) && (white_man & FLD44)) {
		if (((any_piece & FLD43) && (white_man & FLD34) && (empty_field & FLD30)) ||
		   ((any_piece & FLD34) && (white_man & FLD43) && (empty_field & FLD48))) return 1;
	}
	if (all_black((FLD24|FLD29|FLD33)) && (empty_field & FLD38) && (white_man & FLD42)) {
		if (((any_piece & FLD43) && (white_man & FLD32) && (empty_field & FLD27)) ||
		   ((any_piece & FLD32) && (white_man & FLD43) && (empty_field & FLD49))) return 1;
	}
	if (all_black((FLD23|FLD29|FLD34)) && (empty_field & FLD40) && (white_man & FLD45)) {
		if ((any_piece & FLD35) && (white_man & FLD44) && (empty_field & FLD49)) return 1;
	}
	if (all_black((FLD25|FLD30|FLD34)) && (empty_field & FLD39) && (white_man & FLD43)) {
		if (((any_piece & FLD44) && (white_man & FLD33) && (empty_field & FLD28)) ||
		   ((any_piece & FLD33) && (white_man & FLD44) && (empty_field & FLD50))) return 1;
	}
	if (all_black((FLD26|FLD31|FLD37)) && (empty_field & FLD42) && (white_man & FLD48)) {
		if ((any_piece & FLD47) && (white_man & FLD38) && (empty_field & FLD33)) return 1;
	}
	if (all_black((FLD27|FLD32|FLD38)) && (empty_field & FLD43) && (white_man & FLD49)) {
		if ((any_piece & FLD48) && (white_man & FLD39) && (empty_field & FLD34)) return 1;
	}
	if (all_black((FLD29|FLD33|FLD38)) && (empty_field & FLD42) && (white_man & FLD47)) {
		if ((any_piece & FLD48) && (white_man & FLD37) && (empty_field & FLD31)) return 1;
	}
	if (all_black((FLD28|FLD33|FLD39)) && (empty_field & FLD44) && (white_man & FLD50)) {
		if ((any_piece & FLD49) && (white_man & FLD40) && (empty_field & FLD35)) return 1;
	}
	if (all_black((FLD30|FLD34|FLD39)) && (empty_field & FLD43) && (white_man & FLD48)) {
		if ((any_piece & FLD49) && (white_man & FLD38) && (empty_field & FLD32)) return 1;
	}
	if (all_black((FLD35|FLD40)) && (empty_field & FLD44) && (white_man & FLD49)) {
		if ((any_piece & FLD50) && (white_man & FLD39) && (empty_field & FLD33)) return 1;
	}
	// zwart pootje


	if (all_black(FLD11|FLD17|FLD22)  && all_empty(FLD27|FLD28) && (white_man & FLD31))
		if ((all_occupied(FLD21|FLD32))  || (all_empty_or_black(FLD21|FLD32))) {
			if ((white_man & FLD33) && (empty_field & FLD39)) return 1;
			if ((white_man & FLD23) && (empty_field & FLD19)) return 1;
			if ((white_man & FLD32) && (empty_field & FLD37)) return 1;
	}
	if (all_black(FLD11|FLD17|FLD21)  && all_empty(FLD27|FLD28|FLD22) && (white_man & FLD31))
		if (empty_or_black&FLD32) {
			if ((white_man & FLD33) && (empty_field & FLD39)) return 1;
			if ((white_man & FLD23) && (empty_field & FLD19)) return 1;
			if ((white_man & FLD32) && (empty_field & FLD37)) return 1;
	}
	if (all_black(FLD12|FLD18|FLD23)  && all_empty(FLD28|FLD29) && (white_man & FLD32))
		if ((all_occupied(FLD22|FLD33))  || (all_empty_or_black(FLD22|FLD33))) {
			if ((white_man & FLD34) && (empty_field & FLD40)) return 1;
			if ((white_man & FLD24) && (empty_field & FLD20)) return 1;
			if ((white_man & FLD33) && (empty_field & FLD38)) return 1;
	}
	if (all_black(FLD12|FLD18|FLD22)  && all_empty(FLD28|FLD29|FLD23) && (white_man & FLD32))
		if (empty_or_black&FLD33) {
			if ((white_man & FLD34) && (empty_field & FLD40)) return 1;
			if ((white_man & FLD24) && (empty_field & FLD20)) return 1;
			if ((white_man & FLD33) && (empty_field & FLD38)) return 1;
	}
	if (all_black(FLD13|FLD19|FLD24)  && all_empty(FLD29|FLD30) && (white_man & FLD33))
		if ((all_occupied(FLD23|FLD34))  || (all_empty_or_black(FLD23|FLD34))) {
			if ((white_man & FLD34) && (empty_field & FLD39)) return 1;
	}
	if (all_black(FLD13|FLD19|FLD23)  && all_empty(FLD29|FLD30|FLD24) && (white_man & FLD33))
		if (empty_or_black&FLD34) {
			if ((white_man & FLD34) && (empty_field & FLD39)) return 1;
	}
	if (all_black(FLD16|FLD21|FLD27)  && all_empty(FLD31|FLD32) && (white_man & FLD36))
		if ((all_occupied(FLD26|FLD37))  || (all_empty_or_black(FLD26|FLD37))) {
			if ((white_man & FLD38) && (empty_field & FLD43)) return 1;
			if ((white_man & FLD28) && (empty_field & FLD23)) return 1;
			if ((white_man & FLD37) && (empty_field & FLD41)) return 1;
	}
	if (all_black(FLD16|FLD21|FLD26)  && all_empty(FLD31|FLD32|FLD27) && (white_man & FLD36))
		if (empty_or_black&FLD37) {
			if ((white_man & FLD38) && (empty_field & FLD43)) return 1;
			if ((white_man & FLD28) && (empty_field & FLD23)) return 1;
			if ((white_man & FLD37) && (empty_field & FLD41)) return 1;
	}
	if (all_black(FLD17|FLD22|FLD28)  && all_empty(FLD32|FLD33) && (white_man & FLD37))
		if ((all_occupied(FLD27|FLD38))  || (all_empty_or_black(FLD27|FLD38))) {
			if ((white_man & FLD39) && (empty_field & FLD44)) return 1;
			if ((white_man & FLD29) && (empty_field & FLD24)) return 1;
			if ((white_man & FLD38) && (empty_field & FLD42)) return 1;
	}
	if (all_black(FLD17|FLD22|FLD27)  && all_empty(FLD32|FLD33|FLD28) && (white_man & FLD37))
		if (empty_or_black&FLD38) {
			if ((white_man & FLD39) && (empty_field & FLD44)) return 1;
			if ((white_man & FLD29) && (empty_field & FLD24)) return 1;
			if ((white_man & FLD38) && (empty_field & FLD42)) return 1;
	}
	if (all_black(FLD18|FLD23|FLD29)  && all_empty(FLD33|FLD34) && (white_man & FLD38))
		if ((all_occupied(FLD28|FLD39))  || (all_empty_or_black(FLD28|FLD39))) {
			if ((white_man & FLD40) && (empty_field & FLD45)) return 1;
			if ((white_man & FLD30) && (empty_field & FLD25)) return 1;
			if ((white_man & FLD39) && (empty_field & FLD43)) return 1;
	}
	if (all_black(FLD18|FLD23|FLD28)  && all_empty(FLD33|FLD34|FLD29) && (white_man & FLD38))
		if (empty_or_black&FLD39) {
			if ((white_man & FLD40) && (empty_field & FLD45)) return 1;
			if ((white_man & FLD30) && (empty_field & FLD25)) return 1;
			if ((white_man & FLD39) && (empty_field & FLD43)) return 1;
	}
	if (all_black(FLD19|FLD24|FLD30)  && all_empty(FLD34|FLD35) && (white_man & FLD39))
		if ((all_occupied(FLD29|FLD40))  || (all_empty_or_black(FLD29|FLD40))) {
			if ((white_man & FLD40) && (empty_field & FLD44)) return 1;
	}
	if (all_black(FLD19|FLD24|FLD29)  && all_empty(FLD34|FLD35|FLD30) && (white_man & FLD39))
		if (empty_or_black&FLD40) {
			if ((white_man & FLD40) && (empty_field & FLD44)) return 1;
	}
	if (all_black(FLD21|FLD27|FLD32)  && all_empty(FLD37|FLD38) && (white_man & FLD41))
		if ((all_occupied(FLD31|FLD42))  || (all_empty_or_black(FLD31|FLD42))) {
			if ((white_man & FLD43) && (empty_field & FLD49)) return 1;
			if ((white_man & FLD33) && (empty_field & FLD29)) return 1;
			if ((white_man & FLD42) && (empty_field & FLD47)) return 1;
	}
	if (all_black(FLD21|FLD27|FLD31)  && all_empty(FLD37|FLD38|FLD32) && (white_man & FLD41))
		if (empty_or_black&FLD42) {
			if ((white_man & FLD43) && (empty_field & FLD49)) return 1;
			if ((white_man & FLD33) && (empty_field & FLD29)) return 1;
			if ((white_man & FLD42) && (empty_field & FLD47)) return 1;
	}
	if (all_black(FLD22|FLD28|FLD33)  && all_empty(FLD38|FLD39) && (white_man & FLD42))
		if ((all_occupied(FLD32|FLD43))  || (all_empty_or_black(FLD32|FLD43))) {
			if ((white_man & FLD44) && (empty_field & FLD50)) return 1;
			if ((white_man & FLD34) && (empty_field & FLD30)) return 1;
			if ((white_man & FLD43) && (empty_field & FLD48)) return 1;
	}
	if (all_black(FLD22|FLD28|FLD32)  && all_empty(FLD38|FLD39|FLD33) && (white_man & FLD42))
		if (empty_or_black&FLD43) {
			if ((white_man & FLD44) && (empty_field & FLD50)) return 1;
			if ((white_man & FLD34) && (empty_field & FLD30)) return 1;
			if ((white_man & FLD43) && (empty_field & FLD48)) return 1;
	}
	if (all_black(FLD23|FLD29|FLD34)  && all_empty(FLD39|FLD40) && (white_man & FLD43))
		if ((all_occupied(FLD33|FLD44))  || (all_empty_or_black(FLD33|FLD44))) {
			if ((white_man & FLD44) && (empty_field & FLD49)) return 1;
	}
	if (all_black(FLD23|FLD29|FLD33)  && all_empty(FLD39|FLD40|FLD34) && (white_man & FLD43))
		if (empty_or_black&FLD44) {
			if ((white_man & FLD44) && (empty_field & FLD49)) return 1;
	}
	if (all_black(FLD26|FLD31|FLD37)  && all_empty(FLD41|FLD42) && (white_man & FLD46))
		if ((all_occupied(FLD36|FLD47))  || (all_empty_or_black(FLD36|FLD47))) {
			if ((white_man & FLD38) && (empty_field & FLD33)) return 1;
	}
	if (all_black(FLD26|FLD31|FLD36)  && all_empty(FLD41|FLD42|FLD37) && (white_man & FLD46))
		if (empty_or_black&FLD47) {
			if ((white_man & FLD38) && (empty_field & FLD33)) return 1;
	}
	if (all_black(FLD27|FLD32|FLD38)  && all_empty(FLD42|FLD43) && (white_man & FLD47))
		if ((all_occupied(FLD37|FLD48))  || (all_empty_or_black(FLD37|FLD48))) {
			if ((white_man & FLD39) && (empty_field & FLD34)) return 1;
	}
	if (all_black(FLD27|FLD32|FLD37)  && all_empty(FLD42|FLD43|FLD38) && (white_man & FLD47))
		if (empty_or_black&FLD48) {
			if ((white_man & FLD39) && (empty_field & FLD34)) return 1;
	}
	if (all_black(FLD28|FLD33|FLD39)  && all_empty(FLD43|FLD44) && (white_man & FLD48))
		if ((all_occupied(FLD38|FLD49))  || (all_empty_or_black(FLD38|FLD49))) {
			if ((white_man & FLD40) && (empty_field & FLD35)) return 1;
	}
	if (all_black(FLD28|FLD33|FLD38)  && all_empty(FLD43|FLD44|FLD39) && (white_man & FLD48))
		if (empty_or_black&FLD49) {
			if ((white_man & FLD40) && (empty_field & FLD35)) return 1;
	}
	if (all_black(FLD12|FLD17|FLD21)  && all_empty(FLD27|FLD26) && (white_man & FLD32))
		if ((all_occupied(FLD22|FLD31))  || (all_empty_or_black(FLD22|FLD31))) {
			if ((white_man & FLD31) && (empty_field & FLD37)) return 1;
	}
	if (all_black(FLD12|FLD17|FLD22)  && all_empty(FLD27|FLD26|FLD21) && (white_man & FLD32))
		if (empty_or_black&FLD31) {
			if ((white_man & FLD31) && (empty_field & FLD37)) return 1;
	}
	if (all_black(FLD13|FLD18|FLD22)  && all_empty(FLD28|FLD27) && (white_man & FLD33))
		if ((all_occupied(FLD23|FLD32))  || (all_empty_or_black(FLD23|FLD32))) {
			if ((white_man & FLD31) && (empty_field & FLD36)) return 1;
			if ((white_man & FLD21) && (empty_field & FLD16)) return 1;
			if ((white_man & FLD32) && (empty_field & FLD38)) return 1;
	}
	if (all_black(FLD13|FLD18|FLD23)  && all_empty(FLD28|FLD27|FLD22) && (white_man & FLD33))
		if (empty_or_black&FLD32) {
			if ((white_man & FLD31) && (empty_field & FLD36)) return 1;
			if ((white_man & FLD21) && (empty_field & FLD16)) return 1;
			if ((white_man & FLD32) && (empty_field & FLD38)) return 1;
	}
	if (all_black(FLD14|FLD19|FLD23)  && all_empty(FLD29|FLD28) && (white_man & FLD34))
		if ((all_occupied(FLD24|FLD33))  || (all_empty_or_black(FLD24|FLD33))) {
			if ((white_man & FLD32) && (empty_field & FLD37)) return 1;
			if ((white_man & FLD22) && (empty_field & FLD17)) return 1;
			if ((white_man & FLD33) && (empty_field & FLD39)) return 1;
	}
	if (all_black(FLD14|FLD19|FLD24)  && all_empty(FLD29|FLD28|FLD23) && (white_man & FLD34))
		if (empty_or_black&FLD33) {
			if ((white_man & FLD32) && (empty_field & FLD37)) return 1;
			if ((white_man & FLD22) && (empty_field & FLD17)) return 1;
			if ((white_man & FLD33) && (empty_field & FLD39)) return 1;
	}
	if (all_black(FLD15|FLD20|FLD24)  && all_empty(FLD30|FLD29) && (white_man & FLD35))
		if ((all_occupied(FLD25|FLD34))  || (all_empty_or_black(FLD25|FLD34))) {
			if ((white_man & FLD33) && (empty_field & FLD38)) return 1;
			if ((white_man & FLD23) && (empty_field & FLD18)) return 1;
			if ((white_man & FLD34) && (empty_field & FLD40)) return 1;
	}
	if (all_black(FLD15|FLD20|FLD25)  && all_empty(FLD30|FLD29|FLD24) && (white_man & FLD35))
		if (empty_or_black&FLD34) {
			if ((white_man & FLD33) && (empty_field & FLD38)) return 1;
			if ((white_man & FLD23) && (empty_field & FLD18)) return 1;
			if ((white_man & FLD34) && (empty_field & FLD40)) return 1;
	}
	if (all_black(FLD18|FLD22|FLD27)  && all_empty(FLD32|FLD31) && (white_man & FLD38))
		if ((all_occupied(FLD28|FLD37))  || (all_empty_or_black(FLD28|FLD37))) {
			if ((white_man & FLD37) && (empty_field & FLD42)) return 1;
	}
	if (all_black(FLD18|FLD22|FLD28)  && all_empty(FLD32|FLD31|FLD27) && (white_man & FLD38))
		if (empty_or_black&FLD37) {
			if ((white_man & FLD37) && (empty_field & FLD42)) return 1;
	}
	if (all_black(FLD19|FLD23|FLD28)  && all_empty(FLD33|FLD32) && (white_man & FLD39))
		if ((all_occupied(FLD29|FLD38))  || (all_empty_or_black(FLD29|FLD38))) {
			if ((white_man & FLD37) && (empty_field & FLD41)) return 1;
			if ((white_man & FLD27) && (empty_field & FLD21)) return 1;
			if ((white_man & FLD38) && (empty_field & FLD43)) return 1;
	}
	if (all_black(FLD19|FLD23|FLD29)  && all_empty(FLD33|FLD32|FLD28) && (white_man & FLD39))
		if (empty_or_black&FLD38) {
			if ((white_man & FLD37) && (empty_field & FLD41)) return 1;
			if ((white_man & FLD27) && (empty_field & FLD21)) return 1;
			if ((white_man & FLD38) && (empty_field & FLD43)) return 1;
	}
	if (all_black(FLD20|FLD24|FLD29)  && all_empty(FLD34|FLD33) && (white_man & FLD40))
		if ((all_occupied(FLD30|FLD39))  || (all_empty_or_black(FLD30|FLD39))) {
			if ((white_man & FLD38) && (empty_field & FLD42)) return 1;
			if ((white_man & FLD28) && (empty_field & FLD22)) return 1;
			if ((white_man & FLD39) && (empty_field & FLD44)) return 1;
	}
	if (all_black(FLD20|FLD24|FLD30)  && all_empty(FLD34|FLD33|FLD29) && (white_man & FLD40))
		if (empty_or_black&FLD39) {
			if ((white_man & FLD38) && (empty_field & FLD42)) return 1;
			if ((white_man & FLD28) && (empty_field & FLD22)) return 1;
			if ((white_man & FLD39) && (empty_field & FLD44)) return 1;
	}
	if (all_black(FLD22|FLD27|FLD31)  && all_empty(FLD37|FLD36) && (white_man & FLD42))
		if ((all_occupied(FLD32|FLD41))  || (all_empty_or_black(FLD32|FLD41))) {
			if ((white_man & FLD41) && (empty_field & FLD47)) return 1;
	}
	if (all_black(FLD22|FLD27|FLD32)  && all_empty(FLD37|FLD36|FLD31) && (white_man & FLD42))
		if (empty_or_black&FLD41) {
			if ((white_man & FLD41) && (empty_field & FLD47)) return 1;
	}
	if (all_black(FLD23|FLD28|FLD32)  && all_empty(FLD38|FLD37) && (white_man & FLD43))
		if ((all_occupied(FLD33|FLD42))  || (all_empty_or_black(FLD33|FLD42))) {
			if ((white_man & FLD41) && (empty_field & FLD46)) return 1;
			if ((white_man & FLD31) && (empty_field & FLD26)) return 1;
			if ((white_man & FLD42) && (empty_field & FLD48)) return 1;
	}
	if (all_black(FLD23|FLD28|FLD33)  && all_empty(FLD38|FLD37|FLD32) && (white_man & FLD43))
		if (empty_or_black&FLD42) {
			if ((white_man & FLD41) && (empty_field & FLD46)) return 1;
			if ((white_man & FLD31) && (empty_field & FLD26)) return 1;
			if ((white_man & FLD42) && (empty_field & FLD48)) return 1;
	}
	if (all_black(FLD24|FLD29|FLD33)  && all_empty(FLD39|FLD38) && (white_man & FLD44))
		if ((all_occupied(FLD34|FLD43))  || (all_empty_or_black(FLD34|FLD43))) {
			if ((white_man & FLD42) && (empty_field & FLD47)) return 1;
			if ((white_man & FLD32) && (empty_field & FLD27)) return 1;
			if ((white_man & FLD43) && (empty_field & FLD49)) return 1;
	}
	if (all_black(FLD24|FLD29|FLD34)  && all_empty(FLD39|FLD38|FLD33) && (white_man & FLD44))
		if (empty_or_black&FLD43) {
			if ((white_man & FLD42) && (empty_field & FLD47)) return 1;
			if ((white_man & FLD32) && (empty_field & FLD27)) return 1;
			if ((white_man & FLD43) && (empty_field & FLD49)) return 1;
	}
	if (all_black(FLD25|FLD30|FLD34)  && all_empty(FLD40|FLD39) && (white_man & FLD45))
		if ((all_occupied(FLD35|FLD44))  || (all_empty_or_black(FLD35|FLD44))) {
			if ((white_man & FLD43) && (empty_field & FLD48)) return 1;
			if ((white_man & FLD33) && (empty_field & FLD28)) return 1;
			if ((white_man & FLD44) && (empty_field & FLD50)) return 1;
	}
	if (all_black(FLD25|FLD30|FLD35)  && all_empty(FLD40|FLD39|FLD34) && (white_man & FLD45))
		if (empty_or_black&FLD44) {
			if ((white_man & FLD43) && (empty_field & FLD48)) return 1;
			if ((white_man & FLD33) && (empty_field & FLD28)) return 1;
			if ((white_man & FLD44) && (empty_field & FLD50)) return 1;
	}
	if (all_black(FLD29|FLD33|FLD38)  && all_empty(FLD43|FLD42) && (white_man & FLD49))
		if ((all_occupied(FLD39|FLD48))  || (all_empty_or_black(FLD39|FLD48))) {
			if ((white_man & FLD37) && (empty_field & FLD31)) return 1;
	}
	if (all_black(FLD29|FLD33|FLD39)  && all_empty(FLD43|FLD42|FLD38) && (white_man & FLD49))
		if (empty_or_black&FLD48) {
			if ((white_man & FLD37) && (empty_field & FLD31)) return 1;
	}
	if (all_black(FLD30|FLD34|FLD39)  && all_empty(FLD44|FLD43) && (white_man & FLD50))
		if ((all_occupied(FLD40|FLD49))  || (all_empty_or_black(FLD40|FLD49))) {
			if ((white_man & FLD38) && (empty_field & FLD32)) return 1;
	}
	if (all_black(FLD30|FLD34|FLD40)  && all_empty(FLD44|FLD43|FLD39) && (white_man & FLD50))
		if (empty_or_black&FLD49) {
			if ((white_man & FLD38) && (empty_field & FLD32)) return 1;
	}
	return 0;
}
