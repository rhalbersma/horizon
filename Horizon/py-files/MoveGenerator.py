#
#  This file is Copyright (C) 2010 Feike Boomstra.
#  Distributed under the Boost Software License, Version 1.0.
#  (See accompanying file LICENSE_1_0.txt or copy at
#  http://www.boost.org/LICENSE_1_0.txt)
#


from DraughtDataStructures import *
from DraughtConstants import *

class MoveGenerator:
    """ Move Generator for international draughts.

    """
    def __init__(self):
        """ Defines the layout of the international draught board

        For each board position (=field) the possible successors are
        given. So different lists for white men (going north) and black
        man (going south) moves. One list for king moves and captures.
        """

        self.field_list = [0]
        for i in range(51):
            self.field_list.append([])
        self.field_list[1] = [[7, 12, 18, 23, 29, 34, 40, 45]]
        self.field_list[2] = [[8, 13, 19, 24, 30, 35], [7, 11, 16]]
        self.field_list[3] = [[9, 14, 20, 25], [8, 12, 17, 21, 26]]
        self.field_list[4] = [[10, 15], [9, 13, 18, 22, 27, 31, 36]]
        self.field_list[5] = [[10, 14, 19, 23, 28, 32, 37, 41, 46]]
        self.field_list[6] = [[11, 17, 22, 28, 33, 39, 44, 50]]
        self.field_list[7] = [[12, 18, 23, 29, 34, 40, 45], [11, 16]]
        self.field_list[8] = [[13, 19, 24, 30, 35], [12, 17, 21, 26]]
        self.field_list[9] = [[14, 20, 25], [13, 18, 22, 27, 31, 36]]
        self.field_list[10] = [[14, 19, 23, 28, 32, 37, 41, 46]]
        self.field_list[11] = [[7, 2], [17, 22, 28, 33, 39, 44, 50]]
        self.field_list[12] = [[7, 1], [8, 3], [18, 23, 29, 34, 40, 45], [17, 21, 26]]
        self.field_list[13] = [[8, 2], [9, 4], [19, 24, 30, 35], [18, 22, 27, 31, 36]]
        self.field_list[14] = [[9, 3], [10, 5], [20, 25], [19, 23, 28, 32, 37, 41, 46]]
        self.field_list[15] = [[10, 4], [20, 24, 29, 33, 38, 42, 47]]
        self.field_list[16] = [[11, 7, 2], [21, 27, 32, 38, 43, 49]]
        self.field_list[17] = [[11, 6], [12, 8, 3], [22, 28, 33, 39, 44, 50], [21, 26]]
        self.field_list[18] = [[12, 7, 1], [13, 9, 4], [23, 29, 34, 40, 45], [22, 27, 31, 36]]
        self.field_list[19] = [[13, 8, 2], [14, 10, 5], [24, 30, 35], [23, 28, 32, 37, 41, 46]]
        self.field_list[20] = [[14, 9, 3], [24, 29, 33, 38, 42, 47]]
        self.field_list[21] = [[17, 12, 8, 3], [27, 32, 38, 43, 49]]
        self.field_list[22] = [[17, 11, 6], [18, 13, 9, 4], [28, 33, 39, 44, 50], [27, 31, 36]]
        self.field_list[23] = [[18, 12, 7, 1], [19, 14, 10, 5], [29, 34, 40, 45], [28, 32, 37, 41, 46]]
        self.field_list[24] = [[19, 13, 8, 2], [20, 15], [30, 35], [29, 33, 38, 42, 47]]
        self.field_list[25] = [[20, 14, 9, 3], [30, 34, 39, 43, 48]]
        self.field_list[26] = [[21, 17, 12, 8, 3], [31, 37, 42, 48]]
        self.field_list[27] = [[21, 16], [22, 18, 13, 9, 4], [32, 38, 43, 49], [31, 36]]
        self.field_list[28] = [[22, 17, 11, 6], [23, 19, 14, 10, 5], [33, 39, 44, 50], [32, 37, 41, 46]]
        self.field_list[29] = [[23, 18, 12, 7, 1], [24, 20, 15], [34, 40, 45], [33, 38, 42, 47]]
        self.field_list[30] = [[24, 19, 13, 8, 2], [34, 39, 43, 48]]
        self.field_list[31] = [[27, 22, 18, 13, 9, 4], [37, 42, 48]]
        self.field_list[32] = [[27, 21, 16], [28, 23, 19, 14, 10, 5], [38, 43, 49], [37, 41, 46]]
        self.field_list[33] = [[28, 22, 17, 11, 6], [29, 24, 20, 15], [39, 44, 50], [38, 42, 47]]
        self.field_list[34] = [[29, 23, 18, 12, 7, 1], [30, 25], [40, 45], [39, 43, 48]]
        self.field_list[35] = [[30, 24, 19, 13, 8, 2], [40, 44, 49]]
        self.field_list[36] = [[31, 27, 22, 18, 13, 9, 4], [41, 47]]
        self.field_list[37] = [[31, 26], [32, 28, 23, 19, 14, 10, 5], [42, 48], [41, 46]]
        self.field_list[38] = [[32, 27, 21, 16], [33, 29, 24, 20, 15], [43, 49], [42, 47]]
        self.field_list[39] = [[33, 28, 22, 17, 11, 6], [34, 30, 25], [44, 50], [43, 48]]
        self.field_list[40] = [[34, 29, 23, 18, 12, 7, 1], [44, 49]]
        self.field_list[41] = [[37, 32, 28, 23, 19, 14, 10, 5]]
        self.field_list[42] = [[37, 31, 26], [38, 33, 29, 24, 20, 15]]
        self.field_list[43] = [[38, 32, 27, 21, 16], [39, 34, 30, 25]]
        self.field_list[44] = [[39, 33, 28, 22, 17, 11, 6], [40, 35]]
        self.field_list[45] = [[40, 34, 29, 23, 18, 12, 7, 1]]
        self.field_list[46] = [[41, 37, 32, 28, 23, 19, 14, 10, 5]]
        self.field_list[47] = [[41, 36], [42, 38, 33, 29, 24, 20, 15]]
        self.field_list[48] = [[42, 37, 31, 26], [43, 39, 34, 30, 25]]
        self.field_list[49] = [[43, 38, 32, 27, 21, 16], [44, 40, 35]]
        self.field_list[50] = [[44, 39, 33, 28, 22, 17, 11, 6]]

        self.white_move_list = [0]
        for i in range(51):
            self.white_move_list.append([])
        self.white_move_list[6] = [1]
        self.white_move_list[7] = [1,2]
        self.white_move_list[8] = [2,3]
        self.white_move_list[9] = [3,4]
        self.white_move_list[10] = [4,5]
        self.white_move_list[11] = [6,7]
        self.white_move_list[12] = [7,8]
        self.white_move_list[13] = [8,9]
        self.white_move_list[14] = [9,10]
        self.white_move_list[15] = [10]
        self.white_move_list[16] = [11]
        self.white_move_list[17] = [11,12]
        self.white_move_list[18] = [12,13]
        self.white_move_list[19] = [13,14]
        self.white_move_list[20] = [14,15]
        self.white_move_list[21] = [16,17]
        self.white_move_list[22] = [17,18]
        self.white_move_list[23] = [18,19]
        self.white_move_list[24] = [19,20]
        self.white_move_list[25] = [20]
        self.white_move_list[26] = [21]
        self.white_move_list[27] = [21,22]
        self.white_move_list[28] = [22,23]
        self.white_move_list[29] = [23,24]
        self.white_move_list[30] = [24,25]
        self.white_move_list[31] = [26,27]
        self.white_move_list[32] = [27,28]
        self.white_move_list[33] = [28,29]
        self.white_move_list[34] = [29,30]
        self.white_move_list[35] = [30]
        self.white_move_list[36] = [31]
        self.white_move_list[37] = [31,32]
        self.white_move_list[38] = [32,33]
        self.white_move_list[39] = [33,34]
        self.white_move_list[40] = [34,35]
        self.white_move_list[41] = [36,37]
        self.white_move_list[42] = [37,38]
        self.white_move_list[43] = [38,39]
        self.white_move_list[44] = [39,40]
        self.white_move_list[45] = [40]
        self.white_move_list[46] = [41]
        self.white_move_list[47] = [41,42]
        self.white_move_list[48] = [42,43]
        self.white_move_list[49] = [43,44]
        self.white_move_list[50] = [44,45]

        self.black_move_list = [0]
        for i in range(51):
            self.black_move_list.append([])
        self.black_move_list[1] = [7,6]
        self.black_move_list[2] = [8,7]
        self.black_move_list[3] = [9,8]
        self.black_move_list[4] = [10,9]
        self.black_move_list[5] = [10]
        self.black_move_list[6] = [11]
        self.black_move_list[7] = [12,11]
        self.black_move_list[8] = [13,12]
        self.black_move_list[9] = [14,13]
        self.black_move_list[10] = [15,14]
        self.black_move_list[11] = [17,16]
        self.black_move_list[12] = [18,17]
        self.black_move_list[13] = [19,18]
        self.black_move_list[14] = [20,19]
        self.black_move_list[15] = [20]
        self.black_move_list[16] = [21]
        self.black_move_list[17] = [22,21]
        self.black_move_list[18] = [23,22]
        self.black_move_list[19] = [24,23]
        self.black_move_list[20] = [25,24]
        self.black_move_list[21] = [27,26]
        self.black_move_list[22] = [28,27]
        self.black_move_list[23] = [29,28]
        self.black_move_list[24] = [30,29]
        self.black_move_list[25] = [30]
        self.black_move_list[26] = [31]
        self.black_move_list[27] = [32,31]
        self.black_move_list[28] = [33,32]
        self.black_move_list[29] = [34,33]
        self.black_move_list[30] = [35,34]
        self.black_move_list[31] = [37,36]
        self.black_move_list[32] = [38,37]
        self.black_move_list[33] = [39,38]
        self.black_move_list[34] = [40,39]
        self.black_move_list[35] = [40]
        self.black_move_list[36] = [41]
        self.black_move_list[37] = [42,41]
        self.black_move_list[38] = [43,42]
        self.black_move_list[39] = [44,43]
        self.black_move_list[40] = [45,44]
        self.black_move_list[41] = [47,46]
        self.black_move_list[42] = [48,47]
        self.black_move_list[43] = [49,48]
        self.black_move_list[44] = [50,49]
        self.black_move_list[45] = [50]

        self.king_move_list = [0]
        for i in range(51):
            self.king_move_list.append([])
        self.king_move_list[1] = [[7, 12, 18, 23, 29, 34, 40, 45], [6]]
        self.king_move_list[2] = [[8, 13, 19, 24, 30, 35], [7, 11, 16]]
        self.king_move_list[3] = [[9, 14, 20, 25], [8, 12, 17, 21, 26]]
        self.king_move_list[4] = [[10, 15], [9, 13, 18, 22, 27, 31, 36]]
        self.king_move_list[5] = [[10, 14, 19, 23, 28, 32, 37, 41, 46]]
        self.king_move_list[6] = [[1], [11, 17, 22, 28, 33, 39, 44, 50]]
        self.king_move_list[7] = [[1], [2], [12, 18, 23, 29, 34, 40, 45], [11, 16]]
        self.king_move_list[8] = [[2], [3], [13, 19, 24, 30, 35], [12, 17, 21, 26]]
        self.king_move_list[9] = [[3], [4], [14, 20, 25], [13, 18, 22, 27, 31, 36]]
        self.king_move_list[10] = [[4], [5], [15], [14, 19, 23, 28, 32, 37, 41, 46]]
        self.king_move_list[11] = [[6], [7, 2], [17, 22, 28, 33, 39, 44, 50], [16]]
        self.king_move_list[12] = [[7, 1], [8, 3], [18, 23, 29, 34, 40, 45], [17, 21, 26]]
        self.king_move_list[13] = [[8, 2], [9, 4], [19, 24, 30, 35], [18, 22, 27, 31, 36]]
        self.king_move_list[14] = [[9, 3], [10, 5], [20, 25], [19, 23, 28, 32, 37, 41, 46]]
        self.king_move_list[15] = [[10, 4], [20, 24, 29, 33, 38, 42, 47]]
        self.king_move_list[16] = [[11, 7, 2], [21, 27, 32, 38, 43, 49]]
        self.king_move_list[17] = [[11, 6], [12, 8, 3], [22, 28, 33, 39, 44, 50], [21, 26]]
        self.king_move_list[18] = [[12, 7, 1], [13, 9, 4], [23, 29, 34, 40, 45], [22, 27, 31, 36]]
        self.king_move_list[19] = [[13, 8, 2], [14, 10, 5], [24, 30, 35], [23, 28, 32, 37, 41, 46]]
        self.king_move_list[20] = [[14, 9, 3], [15], [25], [24, 29, 33, 38, 42, 47]]
        self.king_move_list[21] = [[16], [17, 12, 8, 3], [27, 32, 38, 43, 49], [26]]
        self.king_move_list[22] = [[17, 11, 6], [18, 13, 9, 4], [28, 33, 39, 44, 50], [27, 31, 36]]
        self.king_move_list[23] = [[18, 12, 7, 1], [19, 14, 10, 5], [29, 34, 40, 45], [28, 32, 37, 41, 46]]
        self.king_move_list[24] = [[19, 13, 8, 2], [20, 15], [30, 35], [29, 33, 38, 42, 47]]
        self.king_move_list[25] = [[20, 14, 9, 3], [30, 34, 39, 43, 48]]
        self.king_move_list[26] = [[21, 17, 12, 8, 3], [31, 37, 42, 48]]
        self.king_move_list[27] = [[21, 16], [22, 18, 13, 9, 4], [32, 38, 43, 49], [31, 36]]
        self.king_move_list[28] = [[22, 17, 11, 6], [23, 19, 14, 10, 5], [33, 39, 44, 50], [32, 37, 41, 46]]
        self.king_move_list[29] = [[23, 18, 12, 7, 1], [24, 20, 15], [34, 40, 45], [33, 38, 42, 47]]
        self.king_move_list[30] = [[24, 19, 13, 8, 2], [25], [35], [34, 39, 43, 48]]
        self.king_move_list[31] = [[26], [27, 22, 18, 13, 9, 4], [37, 42, 48], [36]]
        self.king_move_list[32] = [[27, 21, 16], [28, 23, 19, 14, 10, 5], [38, 43, 49], [37, 41, 46]]
        self.king_move_list[33] = [[28, 22, 17, 11, 6], [29, 24, 20, 15], [39, 44, 50], [38, 42, 47]]
        self.king_move_list[34] = [[29, 23, 18, 12, 7, 1], [30, 25], [40, 45], [39, 43, 48]]
        self.king_move_list[35] = [[30, 24, 19, 13, 8, 2], [40, 44, 49]]
        self.king_move_list[36] = [[31, 27, 22, 18, 13, 9, 4], [41, 47]]
        self.king_move_list[37] = [[31, 26], [32, 28, 23, 19, 14, 10, 5], [42, 48], [41, 46]]
        self.king_move_list[38] = [[32, 27, 21, 16], [33, 29, 24, 20, 15], [43, 49], [42, 47]]
        self.king_move_list[39] = [[33, 28, 22, 17, 11, 6], [34, 30, 25], [44, 50], [43, 48]]
        self.king_move_list[40] = [[34, 29, 23, 18, 12, 7, 1], [35], [45], [44, 49]]
        self.king_move_list[41] = [[36], [37, 32, 28, 23, 19, 14, 10, 5], [47], [46]]
        self.king_move_list[42] = [[37, 31, 26], [38, 33, 29, 24, 20, 15], [48], [47]]
        self.king_move_list[43] = [[38, 32, 27, 21, 16], [39, 34, 30, 25], [49], [48]]
        self.king_move_list[44] = [[39, 33, 28, 22, 17, 11, 6], [40, 35], [50], [49]]
        self.king_move_list[45] = [[40, 34, 29, 23, 18, 12, 7, 1], [50]]
        self.king_move_list[46] = [[41, 37, 32, 28, 23, 19, 14, 10, 5]]
        self.king_move_list[47] = [[41, 36], [42, 38, 33, 29, 24, 20, 15]]
        self.king_move_list[48] = [[42, 37, 31, 26], [43, 39, 34, 30, 25]]
        self.king_move_list[49] = [[43, 38, 32, 27, 21, 16], [44, 40, 35]]
        self.king_move_list[50] = [[44, 39, 33, 28, 22, 17, 11, 6], [45]]

        return #__init__


    def valid_move(self, game):
        """ Generate all valid moves from position 'game.CurrentPosition'.

        Returns a list of legal moves, first try the captures,
        if no captures possible then move.
        During capture collection, the move_list is maintained.
        All captures with nr_of_captures == max_nr_of_captures are
        kept in the list.
        One word about the step-list. This list is maintained to be able
        to recognize whether a certain field uniquely identify a move.
        """


        self.max_nr_of_captures = 0
        self.current_captured =[]
        self.move_list = []
        self.step_list = []
        self.current_empty_fields = game.CurrentPosition.get_empty_fields()


        for i in game.get_own_man():                                           # first captures with men
            self.start_point = i
            self.step_list.append(i)
            self.current_empty_fields.append(i)
            self.capture_with_man(i, 0, game)
            self.current_empty_fields.remove(i)
            self.step_list.remove(i)

        for i in game.get_own_king():                                          # then look for captures with kings
            self.start_point = i
            self.step_list.append(i)
            self.current_empty_fields.append(i)
            self.capture_with_king(i, 0, game)
            self.current_empty_fields.remove(i)
            self.step_list.remove(i)

        #if no pieces are captured, we try a move
        if self.max_nr_of_captures == 0:
            for i in game.get_own_man():
                self.start_point = i
                self.step_list.append(i)
                if game.is_color_white():
                    self._white_move(i)
                else:
                    self._black_move(i)
                self.step_list.remove(i)
            # and king moves ...
            for i in game.get_own_king():
                self.start_point = i
                self.step_list.append(i)
                self._king_move(i)
                self.step_list.remove(i)

        return  self.move_list #valid_move


    def _king_move(self,own_color_king):
        for direc in self.king_move_list[own_color_king]:                  # go over all directions[own_color_king]
            for cur_pos in direc:                                              # go over all fields of this direction[i]
                if cur_pos in self.current_empty_fields:                       # this is what we want
                    self.step_list.append(cur_pos)                             # put the stop-point on the step list
                    self._push_move(own_color_king, cur_pos)                          # and save this move
                    self.step_list.remove(cur_pos)                             # put the stop-point on the step list
                else:
                    break                                                      # no more direct-empty fields in this direction
        return


    def _white_move(self, own_man):
        for cur_pos in self.white_move_list[own_man]:
            if cur_pos in self.current_empty_fields:                           # this is what we want
                self.step_list.append(cur_pos)                                 # put the stop-point on the step list
                self._push_move(own_man, cur_pos)                              # and save this move
                self.step_list.remove(cur_pos)                                 # put the stop-point on the step list
        return

    def _black_move(self, own_man):
        for cur_pos in self.black_move_list[own_man]:
            if cur_pos in self.current_empty_fields:                           # this is what we want
                self.step_list.append(cur_pos)                             # put the stop-point on the step list
                self._push_move(own_man, cur_pos)                                 # and save this move
                self.step_list.remove(cur_pos)                             # put the stop-point on the step list
        return

    def capture_with_king(self, own_color_king, depth, game):
        for direc in self.field_list[own_color_king]:
            loc_status = "leading empty"                                       #final state machine: leading empty, piece seen, trailing empty, false
            for cur_pos in direc:                                              #go over all fields of this direction
                if cur_pos in self.current_empty_fields:                       #events: self.current_empty_fields, other_color_piece, else_field
                    loc_event="empty field"
                elif game.is_other_color_piece(cur_pos):
                    if not cur_pos in self.current_captured:
                        loc_event="other color piece"
                    else:
                        loc_event="else field"
                else:
                    loc_event="else field"                                     #event calculated

                #calculate the new state of the finale state machine
                if loc_status=="leading empty":
                    if loc_event=="empty field":
                        loc_status="leading empty"
                    elif loc_event=="other color piece":
                        loc_status="piece seen"
                        piece_seen=cur_pos
                    else:
                        loc_status="false"
                elif loc_status=="piece seen":
                    if loc_event=="empty field":
                        loc_status="trailing empty"
                    elif loc_event=="other color piece":
                        loc_status="false"
                    else:
                        loc_status="false"
                elif loc_status=="trailing empty":
                    if loc_event=="empty field":
                        loc_status="trailing empty"
                    elif loc_event=="other color piece":
                        loc_status="false"
                    else:
                        loc_status="false"
                #print "status: ", loc_status, "event: ", loc_event, "depth: ", depth, "pos: ", cur_pos
                # act according to loc_status
                if loc_status=="leading empty":
                    pass    #just continue
                elif loc_status=="piece seen":
                    pass    #just continue
                elif loc_status=="trailing empty":      #use this field as startfield for a next leg in the capture path
                    depth = depth + 1
                    self.current_captured.append(piece_seen)
                    self.step_list.append(cur_pos)
                    self.capture_with_king(cur_pos, depth, game)
                    self.step_list.remove(cur_pos)
                    self.current_captured.remove(piece_seen)
                    depth = depth - 1
                else:
                    break                               #skip the remaining of the fields in this direction to examin

        self._proces_depth(own_color_king, depth)
        return


    def capture_with_man(self, own_color_man, depth, game ):
        for direc in self.field_list[own_color_man]:
            cur_piece_pos = direc[0]
            cur_new_pos = direc[1]
            if game.is_other_color_piece(cur_piece_pos):
                if not cur_piece_pos in self.current_captured:
                    if cur_new_pos in self.current_empty_fields:
                        depth = depth + 1
                        self.step_list.append(cur_new_pos)
                        self.current_captured.append(cur_piece_pos)
                        self.capture_with_man(cur_new_pos, depth, game)
                        self.current_captured.remove(cur_piece_pos)
                        self.step_list.remove(cur_new_pos)
                        depth = depth - 1
        self._proces_depth(own_color_man, depth)
        return

    def _proces_depth(self, end_point, depth):
        if depth > 0:
            if depth > self.max_nr_of_captures:
                self.max_nr_of_captures = depth
                self.move_list = []
                self._push_move(self.start_point, end_point)
            elif depth==self.max_nr_of_captures:
                self._push_move(self.start_point, end_point)
        return

    def _push_move(self, start, stop ):
        if self._not_duplicated_move(start, stop):
            move = HalfPlyRecord()
            move.Cap = self.current_captured[:]
            move.Start = start
            move.Stop = stop
            move.Step = self.step_list[:]
            self.move_list.append(move)
        return

    def _not_duplicated_move(self, start, stop):
        for move in self.move_list:
            if move.Start == start and move.Stop == stop and set(move.Cap) == set(self.current_captured):
                # found identical move, merge step-lists
                move.Step = list(set(move.Step + self.step_list))
                return False
        return True

if __name__== '__main__':
    mv = MoveGenerator()
    dc = DraughtConstants()
    game = GameRecord(None)
    game.CurrentPosition.WhiteKing = [46]
    game.CurrentPosition.BlackMan = [41, 10, 44]
    game.CurrentPosition.ColorOnMove = dc.WHITE_ON_MOVE
    print game
    moves = mv.valid_move(game)
    for i in moves:
        print i

