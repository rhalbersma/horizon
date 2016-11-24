#
#  This file is Copyright (C) 2010 Feike Boomstra.
#  Distributed under the Boost Software License, Version 1.0.
#  (See accompanying file LICENSE_1_0.txt or copy at
#  http://www.boost.org/LICENSE_1_0.txt)
#


from DraughtConstants import *
dc = DraughtConstants()
from ParsePDNfile import *

class DraughtPosition:
    """ To store info about the draught position
    
    WhiteMan    a list of fieldnrs of the white men
    BlackMan    a list of fieldnrs of the black men
    WhiteKnight a list of fieldnrs of the white knight
    BlackKnight a list of fieldnrs of the black knight
    ColorOnMove a constant, defined in DraughtConstants (WHITE_ON_MOVE, BLACK_ON_MOVE)
    """
    
    def __init__(self, panel):
        self.panel = panel
        self.WhiteMan = []
        self.BlackMan = []
        self.WhiteKnight = []
        self.BlackKnight = []
        self.Selected = []
        self.ColorOnMove = 0
        self.BoardIsModified = bool(True)       # unknown status of board
        self.Games = []
        self.game_pnt = -1
        self.current_half_ply_counter = 0
        return
    
    def __str__(self):
        return str("WhiteMan....: ") + '\t' + str(self.WhiteMan) + '\n' + \
               "BlackMan....: " + str(self.BlackMan) + '\n' + \
               "WhiteKnight.: " + str(self.WhiteKnight) + '\n' + \
               "BlackKnight.: " + str(self.BlackKnight) + '\n' + \
               "ColorOnMove: " + dc.DCtoString(self.ColorOnMove) + '\n'
    
    def print_bord(color, idx):
        print "trying to move with ", color, " with ", idx
        for i in range(0, 5):
            print "...", loc_to_char(color, 1+i*10), "...", loc_to_char(color, 2+i*10), "...", loc_to_char(color, 3+i*10), "...", loc_to_char(color, 4+i*10), "...", loc_to_char(color, 5+i*10)
            print loc_to_char(color, 6+i*10), "...", loc_to_char(color,7+i*10), "...", loc_to_char(color, 8+i*10), "...", loc_to_char(color, 9+i*10), "...", loc_to_char(color, 10+i*10), "..."
        return
    
    def loc_to_char(color, idx):
        if idx in self.WhiteMan:             char = 'o'
        elif idx in self.WhiteKnight:        char = 'O'
        elif id in self.BlackMan:            char = 'x'
        elif idx in self.BlackKnight:        char = 'X'
        else: char = '::'
        return char
    
    def is_color_white(self):
        return self.ColorOnMove == dc.WHITE_ON_MOVE
    
    def is_empty_field(self, idx):
        return not idx in self.WhiteMan + self.WhiteKnight + self.BlackKnight + self.BlackMan

    def toggle_color(self):
        if self.ColorOnMove == dc.WHITE_ON_MOVE:
            self.ColorOnMove = dc.BLACK_ON_MOVE
        else:
            self.ColorOnMove = dc.WHITE_ON_MOVE
        return

    def store_move(self, move):
        if self.BoardIsModified == True:
            pass
            # print "don't store, board is modified"
        elif self.current_half_ply_counter <> self.stored_half_ply_length(self.Games[self.game_pnt]):
            pass
            # print "don't store, not at the end", self.current_half_ply_counter , self.stored_half_ply_length(), int(self.current_half_ply_counter/2)
        else:
            # print "store game = ", self.game_pnt, "move = ", self.current_half_ply_counter
            if self.ColorOnMove == dc.WHITE_ON_MOVE:
                # prepare a record to append, in this case we get a copy in the list
                m = MoveRecord()
                m.White_start = move.start
                m.White_stop = move.stop
                if len(move.cap) == 0:
                    m.White_op = '-'
                else:
                    m.White_op = 'x'
                m.Ply_nr = len(self.Games[self.game_pnt].Moves)+1
                
                self.Games[self.game_pnt].Moves.append(m)
            else:
                self.Games[self.game_pnt].Moves[int(self.current_half_ply_counter/2)].Black_start = move.start
                self.Games[self.game_pnt].Moves[int(self.current_half_ply_counter/2)].Black_stop = move.stop
                if len(move.cap) == 0:
                    self.Games[self.game_pnt].Moves[int(self.current_half_ply_counter/2)].Black_op = '-'
                else:
                    self.Games[self.game_pnt].Moves[int(self.current_half_ply_counter/2)].Black_op = 'x'                
            self.current_half_ply_counter = self.current_half_ply_counter + 1
            self.panel.notation_display.set(self.build_game_pdn_string(self.game_pnt, False))
        return
    
    def stored_half_ply_length(self, game):   # return the first free location in a game
        i = len(game.Moves)
        if i == 0:
            k = 0
        elif game.Moves[(i-1)].Black_start == '':
            k = 2*(i - 1) + 1
        else:
            k = 2*i
        return k
    
    def build_pdn_string(self, long):
        s = ''
        for i in range(0,len(self.Games)):
            if long == True:
                s = s + "[White \"a\"][Black \"b\"] "
            s = s + self.build_game_pdn_string(i, long)
            if long == True:
                s = s + "0-0\n"
        return s
  
    def build_game_pdn_string(self, i, long):
        k = 1
        s = ''
        if len(self.Games) == 0:
            return s
        elif i >= 0 and i <= len(self.Games):
            for j in range(0,len(self.Games[i].Moves)):
                s = s + repr(self.Games[i].Moves[j].Ply_nr).rjust(2) + "."
                s = s + repr(self.Games[i].Moves[j].White_start).rjust(2)
                s = s + self.Games[i].Moves[j].White_op
                s = s + repr(self.Games[i].Moves[j].White_stop).rjust(2) + " "
                if self.Games[i].Moves[j].Black_start <> '':
                    s = s + repr(self.Games[i].Moves[j].Black_start).rjust(2)
                    s = s + self.Games[i].Moves[j].Black_op
                    s = s + repr(self.Games[i].Moves[j].Black_stop).rjust(2) + " "
                    k = k + 1
                if k == 6:
                    s = s + '\n'
                    k = 1
            s = s + '\n'        
        return s
