#
#  This file is Copyright (C) 2010 Feike Boomstra.
#  Distributed under the Boost Software License, Version 1.0.
#  (See accompanying file LICENSE_1_0.txt or copy at
#  http://www.boost.org/LICENSE_1_0.txt)
#


from Tkinter import *
from DraughtRePlay import *
from DraughtConstants import *
dc = DraughtConstants()

class DraughtBoard:
    def __init__(self):
        self.WhiteMan = []
        self.BlackMan = []
        self.WhiteKing = []
        self.BlackKing = []
        self.Selected = []
        self.ColorOnMove = dc.WHITE_ON_MOVE
        return

    def __str__(self):
        return str("WhiteMan: ").rjust(15) + str(self.WhiteMan) + '\n' + \
               "BlackMan: " + str(self.BlackMan) + '\n' + \
               "WhiteKing: " + str(self.WhiteKing) + '\n' + \
               "BlackKing: " + str(self.BlackKing) + '\n' + \
               "ColorOnMove: " + dc.DCtoString(self.ColorOnMove) + '\n'

    def get_empty_fields(self):
        temp = self.WhiteMan + self.WhiteKing + self.BlackKing + self.BlackMan
        all = range(1,51)
        for item in temp:
            if item in all:
                all.remove(item)
            else:
                print "try to remove:" , item
        return all

    def is_empty_field(self, nr):
        if nr in self.WhiteMan:  return False
        if nr in self.BlackMan:  return False
        if nr in self.WhiteKing: return False
        if nr in self.BlackKing: return False
        return True

#-----------------------------------------------------------------------------------------------#

class GameRecord:

    def __init__(self, main, active_panel, support_panel, prev_game=''):

        self.main = main
        self.ActivePanel = active_panel                     # to distinguish a variant from the main game
        self.panel = support_panel                          # here are all the buttons etc.

        self.WhitePlayer = ''                               # PDN element
        self.BlackPlayer = ''                               # PDN element
        self.Event = ''                                     # PDN element
        self.Date = ''                                      # PDN element
        self.Result = ''                                    # PDN element
        self.Round = ''                                     # PDN element
        self.Comment = ''                                   # PDN element
        self.GameType = ''                                  # PDN element
        self.Moves = []                                     # keeps the moves == ply == 2 x half ply
        self.HalfPlyPointer = 0                             # points at the first free half ply in play-mode
                                                            # points at the half ply to be done in re-play mode
        self.CurrentPlyCounter = 0                          # defines the ply nr of the last move
        self.BeginPosition = DraughtBoard()                 # the beginposition of the game, set in setup mode
        self.CurrentPosition = DraughtBoard()               # currentposition is used in play and re-play
        self.PreviousActiveGame = prev_game                 # used in a variant to know the parents
        self.PreviousHalfPlyPointer = 0                     # used in a variant to know the parents
        self.file_name_chart = None
        self.file_name_pdn = None
        return

    def __str__(self):
        s = ''
        if self.WhitePlayer <> '': s = s + '[White "'+ str(self.WhitePlayer) +'"]\n'
        if self.BlackPlayer <> '': s = s + '[Black "'+ str(self.BlackPlayer) +'"]\n'
        if self.Result <> '':      s = s + '[Result "'+ str(self.Result) +'"]\n'
        if self.Event <> '':       s = s + '[Event "'+ str(self.Event) +'"]\n'
        if self.Date <> '':        s = s + '[Date "'+ str(self.Date) +'"]\n'
        if self.Round <> '':       s = s + '[Round "'+ str(self.Round) +'"]\n'
        if self.GameType <> '':    s = s + '[GameType "'+ str(self.GameType) +'"]\n'
        s = s + 'Begin:\n' + str(self.BeginPosition)
        for move in self.Moves:
            s = s + str(move)
        s = s + 'Current:\n' + str(self.CurrentPosition)
        return s

    def line_nr(self, s):
        # s: n.k, nn.k, n.kk, nn.kkk
        return int(s[:s.index('.')])


    def clear(self):            # clear entire game
        self.WhitePlayer = ''                               # PDN element
        self.BlackPlayer = ''                               # PDN element
        self.Event = ''                                     # PDN element
        self.Date = ''                                      # PDN element
        self.Result = ''                                    # PDN element
        self.Round = ''                                     # PDN element
        self.Comment = ''                                   # PDN element
        self.GameType = ''                                  # PDN element
        self.Moves = []                                     # keeps the moves == ply == 2 x half ply
        self.HalfPlyPointer = 0                             # points at the first free half ply in play-mode
                                                            # points at the half ply to be done in re-play mode
        self.CurrentPlyCounter = 0                          # defines the ply nr of the last move
        self.BeginPosition = DraughtBoard()                 # the beginposition of the game, set in setup mode
        self.CurrentPosition = DraughtBoard()               # currentposition is used in play and re-play
        self.PreviousHalfPlyPointer = 0                     # used in a variant to know the parents
        return

    def set_begin_position_ext(self, position):
        self.BeginPosition.WhiteMan = position.WhiteMan[:]
        self.BeginPosition.WhiteKing = position.WhiteKing[:]
        self.BeginPosition.BlackMan = position.BlackMan[:]
        self.BeginPosition.BlackKing = position.BlackKing[:]
        self.BeginPosition.ColorOnMove = position.ColorOnMove
        self.CurrentPosition.WhiteMan = position.WhiteMan[:]
        self.CurrentPosition.WhiteKing = position.WhiteKing[:]
        self.CurrentPosition.BlackMan = position.BlackMan[:]
        self.CurrentPosition.BlackKing = position.BlackKing[:]
        self.CurrentPosition.ColorOnMove = position.ColorOnMove
        return



    def half_ply_length(self):
        k = 2 * len(self.Moves)
        if k == 0: return 0
        if not self.Moves[0].WhiteMove : k = k - 1
        if not self.Moves[ - 1].BlackMove : k = k - 1
        return k

    def refresh_display(self):
        for piece in self.CurrentPosition.WhiteMan:           self.ActivePanel.set_white_man   (piece, piece in self.CurrentPosition.Selected)
        for piece in self.CurrentPosition.BlackMan:           self.ActivePanel.set_black_man   (piece, piece in self.CurrentPosition.Selected)
        for piece in self.CurrentPosition.WhiteKing:          self.ActivePanel.set_white_king (piece, piece in self.CurrentPosition.Selected)
        for piece in self.CurrentPosition.BlackKing:          self.ActivePanel.set_black_king (piece, piece in self.CurrentPosition.Selected)
        for piece in self.CurrentPosition.get_empty_fields(): self.ActivePanel.set_empty_field(piece, piece in self.CurrentPosition.Selected)
        if self.main.panel.is_play_mode():
            self.main.Games[self.main.ActiveGameNumber].write_moves()
        self.refresh_number_of_pieces_display()
        if self.CurrentPosition.ColorOnMove == dc.WHITE_ON_MOVE:
            self.panel.wm.select()
        else:
            self.panel.bm.select()
        self.CurrentPosition.Selected = []
        #if self.HalfPlyPointer == 0:
        self.write_moves()
        self.refresh_number_of_pieces_display()
        self.refresh_game_info()
        return

    def refresh_game_info(self):
      if self.panel.WhitePlayer:
        if self.panel.WhitePlayer:
            if self.WhitePlayer <> '': self.panel.WhitePlayer.set(str(self.WhitePlayer))
        if self.panel.BlackPlayer:
            if self.BlackPlayer <> '': self.panel.BlackPlayer.set(str(self.BlackPlayer))
        if self.panel.Result:
            if self.Result <> '':
              self.panel.Result.set(str(self.Result))
        if self.panel.Event:
            if self.Event <> '': self.panel.Event.set(str(self.Event))
        if self.panel.Round:
            if self.Round <> '': self.panel.Round.set(str(self.Round))
        if self.panel.Date:
            if self.Date <> '': self.panel.Date.set(str(self.Date))
      return


    def refresh_number_of_pieces_display(self):
 #       if self.main.panel.is_play_mode():
        if self.panel.invert_board:
            if self.panel.invert_board.get() == 1:
                self.panel.black_numbers_display.set(str(len(self.CurrentPosition.BlackMan)).rjust(2) + \
                " (" + str(len(self.CurrentPosition.BlackKing)) + ")")
                self.panel.white_numbers_display.set(str(len(self.CurrentPosition.WhiteMan)).rjust(2) +\
                " (" + str(len(self.CurrentPosition.WhiteKing)) + ")")
            else:
                self.panel.white_numbers_display.set(str(len(self.CurrentPosition.BlackMan)).rjust(2) + \
                " (" + str(len(self.CurrentPosition.BlackKing)) + ")")
                self.panel.black_numbers_display.set(str(len(self.CurrentPosition.WhiteMan)).rjust(2) + \
                " (" + str(len(self.CurrentPosition.WhiteKing)) + ")")
            return

    def get_own_man(self):
        if self.CurrentPosition.ColorOnMove == dc.WHITE_ON_MOVE:
            return self.CurrentPosition.WhiteMan
        else: return self.CurrentPosition.BlackMan

    def get_own_king(self):
        if self.CurrentPosition.ColorOnMove == dc.WHITE_ON_MOVE:
            return self.CurrentPosition.WhiteKing
        else: return self.CurrentPosition.BlackKing

    def is_other_color_piece(self, idx):
        if self.CurrentPosition.ColorOnMove == dc.WHITE_ON_MOVE:
            temp = self.CurrentPosition.BlackMan + self.CurrentPosition.BlackKing
        else: temp = self.CurrentPosition.WhiteMan + self.CurrentPosition.WhiteKing
        return idx in temp

    def start_stop_to_move_record(self, start, stop):
        ml = self.main.mg.valid_move(self)
        if ml == []: return False, None
        for move in ml:
            if move.Start == start and move.Stop == stop:
                if move.Cap == []:
                    move.op = '-'
                else:
                    move.op = 'x'
                return True, move
        return False, None

    def do_move(self, move, append = True):
        for piece in move.Cap:
            if piece in self.CurrentPosition.WhiteMan: self.CurrentPosition.WhiteMan.remove(piece)
            if piece in self.CurrentPosition.WhiteKing: self.CurrentPosition.WhiteKing.remove(piece)
            if piece in self.CurrentPosition.BlackMan: self.CurrentPosition.BlackMan.remove(piece)
            if piece in self.CurrentPosition.BlackKing: self.CurrentPosition.BlackKing.remove(piece)
            self.CurrentPosition.Selected.append(piece)
        if move.Start in self.CurrentPosition.WhiteMan:                                         # started the move with a white man?
            self.CurrentPosition.WhiteMan.remove(move.Start)
            if move.Stop in [1,2,3,4,5]:                                        # ended it at the upper row ?
                self.CurrentPosition.WhiteKing.append(move.Stop)                              # promotion
            else:
                self.CurrentPosition.WhiteMan.append(move.Stop)
        elif move.Start in self.CurrentPosition.BlackMan:                                       # similair questions
            self.CurrentPosition.BlackMan.remove(move.Start)
            if move.Stop in [46, 47, 48, 49, 50]:
                self.CurrentPosition.BlackKing.append(move.Stop)                              # promotion
            else:
                self.CurrentPosition.BlackMan.append(move.Stop)
            self.CurrentPosition.Selected.append(move.Stop)
        elif move.Start in self.CurrentPosition.WhiteKing:
            self.CurrentPosition.WhiteKing.remove(move.Start)
            self.CurrentPosition.WhiteKing.append(move.Stop)
        elif move.Start in self.CurrentPosition.BlackKing:
            self.CurrentPosition.BlackKing.remove(move.Start)
            self.CurrentPosition.BlackKing.append(move.Stop)
        else:
            print "error"
        self.CurrentPosition.Selected.append(move.Stop)
        self.CurrentPosition.Selected.append(move.Start)
        if self.CurrentPosition.ColorOnMove == dc.WHITE_ON_MOVE:
              self.CurrentPosition.ColorOnMove = dc.BLACK_ON_MOVE
        else: self.CurrentPosition.ColorOnMove = dc.WHITE_ON_MOVE
        if append == True: self.add_to_tail(move)
        return

    def is_valid_move(self, half_ply):
        if not half_ply : return None
        start = half_ply.Start
        stop  = half_ply.Stop
        if start == '' or stop == '': return None
        ml = self.main.mg.valid_move(self)
        if ml == []:
            print "is valid move - Empty move list"
            return None
        for move in ml:
            if move.Start == start and move.Stop == stop:
                if len(half_ply.Step) == 2:
                    break
                else:
                    if set(half_ply.Step) == set(move.Step):
                        break
        if not (move.Start == start and move.Stop == stop):
            print "is valid move - Mismatch"
            print "is valid move - wanted : ", start , stop
            print "is valid move - got :"
            for move in ml: print move;
            return None
        # no invalid neither impossible moves
        return move

    def build_till_pointer(self):


        self.CurrentPosition.WhiteMan = self.BeginPosition.WhiteMan[:]
        self.CurrentPosition.WhiteKing = self.BeginPosition.WhiteKing[:]
        self.CurrentPosition.BlackMan = self.BeginPosition.BlackMan[:]
        self.CurrentPosition.BlackKing = self.BeginPosition.BlackKing[:]
        self.CurrentPosition.ColorOnMove = self.BeginPosition.ColorOnMove

        loc_pnt = 0
        if self.HalfPlyPointer > 0:
            for k in range(0, len(self.Moves)):
                if self.Moves[k].WhiteMove:
                    valid_move = self.is_valid_move(self.Moves[k].WhiteMove)
                    if valid_move <> None:
                        self.do_move(valid_move, False)
                        loc_pnt = loc_pnt + 1
                        if loc_pnt == self.HalfPlyPointer: break
                    else: # move is incorrect
                      print "found illegal move"
                      return False
                if self.Moves[k].BlackMove:
                    valid_move = self.is_valid_move(self.Moves[k].BlackMove)
                    if valid_move <> None:
                        self.do_move(valid_move, False)
                        loc_pnt = loc_pnt + 1
                        if loc_pnt == self.HalfPlyPointer: break
                    else: # move is incorrect
                      print "found illegal move"
                      return False

        self.CurrentPosition.Selected = []

        return True

    def set_begin_position(self):
        self.CurrentPosition.WhiteMan = self.BeginPosition.WhiteMan[:]
        self.CurrentPosition.WhiteKing = self.BeginPosition.WhiteKing[:]
        self.CurrentPosition.BlackMan = self.BeginPosition.BlackMan[:]
        self.CurrentPosition.BlackKing = self.BeginPosition.BlackKing[:]
        self.CurrentPosition.ColorOnMove = self.BeginPosition.ColorOnMove
        self.HalfPlyPointer = 0;
        return

    def get_next_board_layout(self):
        next_move = self.get_half_ply_pointer_record()
        valid_move = self.is_valid_move(next_move)
        if valid_move:
            pass
        else:
            return None
        self.do_move(valid_move, False)
        self.HalfPlyPointer += 1
        # new position in CurrentPosition, capture?
        ml = self.main.mg.valid_move(self)
        if ml == []:return None
        if ml[0].Cap != []:
          #print "capture position"
          return None # capture
        # switch color
        if self.CurrentPosition.ColorOnMove == dc.WHITE_ON_MOVE:
              self.CurrentPosition.ColorOnMove = dc.BLACK_ON_MOVE
        else: self.CurrentPosition.ColorOnMove = dc.WHITE_ON_MOVE
        ml = self.main.mg.valid_move(self)
        # switch color back
        if self.CurrentPosition.ColorOnMove == dc.WHITE_ON_MOVE:
              self.CurrentPosition.ColorOnMove = dc.BLACK_ON_MOVE
        else: self.CurrentPosition.ColorOnMove = dc.WHITE_ON_MOVE
        if ml == []:return None
        if ml[0].Cap != []:
          #print "other side capture position"
          return None # capture

        if self.is_color_white() == True: color = "W"
        else: color = "B"
        return color + self.get_50_sq_position()

    def is_color_white(self):
        return self.CurrentPosition.ColorOnMove == dc.WHITE_ON_MOVE


    def get_move_string(self):
        s = ""
        for j in range(0, len(self.Moves)):
            if self.Moves[j].WhiteMove:
                start_value = self.Moves[j].WhiteMove.Start
                stop_value  = self.Moves[j].WhiteMove.Stop
                s = s + chr(start_value + ord('('))
                s = s + chr(stop_value  + ord('('))
            if self.Moves[j].BlackMove:
                start_value = self.Moves[j].BlackMove.Start
                stop_value  = self.Moves[j].BlackMove.Stop
                s = s + chr(start_value + ord('('))
                s = s + chr(stop_value  + ord('('))
        return s

    def write_moves(self):
      if self.panel.notation:
        self.init_moves_display()
        self.put_moves_display(True)
        self.panel.notation.config(state=DISABLED)
      return

    def init_moves_display(self):
        self.main.LastCharCount = 0                     # to be able to insert a \n before the 80th char
        #self.main.LeadingTag = str(self.main.Games.index(self)).zfill(3) + '_' # to construct the leveled tag e.g. 000_018_013,
                                                        # the 13th half_ply of the variant
                                                        # starting at the 18th half_ply of game nr 000
        self.panel.notation.config(state=NORMAL)
        self.panel.notation.delete(1.0, END)
        return

    def put_moves_display(self, format = True):

        self.HalfPlyDisplayPointer = 0

        for j in range(0,len(self.Moves)):
            if self.main.LastCharCount >= 65:
                self.panel.notation.insert(END, '\n')
                self.main.LastCharCount = 0
            if self.Moves[j].Ply_nr <> '':
                self.panel.notation.insert(END, repr(self.Moves[j].Ply_nr).rjust(2) + ".")
            self.main.LastCharCount = self.main.LastCharCount + 3
            if self.Moves[j].WhiteMove:
                if len(self.Moves[j].WhiteMove.Step) < 6:
                    s = repr(self.Moves[j].WhiteMove.Start).rjust(2)
                    s = s + self.Moves[j].WhiteMove.op
                    s = s + repr(self.Moves[j].WhiteMove.Stop).rjust(2)
                else:
                    s = ''
                    for step in self.Moves[j].WhiteMove.Step:
                        s = s + repr(step).rjust(2) + 'x'
                    s = s[0: len(s)-1] # remove the last 'x'
                if self.Moves[j].WhiteMove.Comment <> '':
                    s = s + '{' + self.Moves[j].WhiteMove.Comment  + '} '
                    self.main.LastCharCount = self.main.LastCharCount + len(self.Moves[j].WhiteMove.Comment) + 3




                # we have the white move in s
                tag = self.main.LeadingTag + str(self.HalfPlyDisplayPointer).zfill(3)
                self.panel.notation.insert(END, s, tag)
                self.panel.notation.tag_bind(tag, "<Button-1>", ClickMoveTag(self.main.re_play, tag))
                self.panel.notation.insert(END, ' ')
                self.main.LastCharCount = self.main.LastCharCount + len(s) + 1

                if self.Moves[j].WhiteMove.Variant <> '':
                    self.panel.notation.insert(END, '(')
                    self.main.LastCharCount = self.main.LastCharCount + 1
                    self.main.LeadingTag = self.main.LeadingTag + str(self.HalfPlyDisplayPointer).zfill(3) + "_"
                    self.Moves[j].WhiteMove.Variant.put_moves_display(False)
                    self.panel.notation.insert(END, ') ')
                    self.main.LastCharCount = self.main.LastCharCount + 2
                    self.main.LeadingTag = self.main.LeadingTag[0:len(self.main.LeadingTag)-4]

                self.HalfPlyDisplayPointer = self.HalfPlyDisplayPointer + 1


            if self.Moves[j].BlackMove:
                if len(self.Moves[j].BlackMove.Step) < 6:
                    s = repr(self.Moves[j].BlackMove.Start).rjust(2)
                    s = s + self.Moves[j].BlackMove.op
                    s = s + repr(self.Moves[j].BlackMove.Stop).rjust(2) + " "
                else:
                    s = ''
                    for step in self.Moves[j].BlackMove.Step:
                        s = s + repr(step).rjust(2) + 'x'
                    s = s[0: len(s)-1] # remove the last 'x'
                if self.Moves[j].BlackMove.Comment <> '':
                    s = s + '{' + self.Moves[j].BlackMove.Comment  + '} '
                    self.main.LastCharCount = self.main.LastCharCount + len(self.Moves[j].BlackMove.Comment) + 3

                # we have the black move in s
                tag = self.main.LeadingTag + str(self.HalfPlyDisplayPointer).zfill(3)
                self.panel.notation.insert(END, s, tag)
                self.panel.notation.tag_bind(tag, "<Button-1>", ClickMoveTag(self.main.re_play, tag))
                self.panel.notation.insert(END, ' ')
                self.main.LastCharCount = self.main.LastCharCount + len(s) + 1

                if self.Moves[j].BlackMove.Variant <> '':
                    self.panel.notation.insert(END, '(')
                    self.main.LastCharCount = self.main.LastCharCount + 1
                    self.main.LeadingTag = self.main.LeadingTag + str(self.HalfPlyDisplayPointer).zfill(3) + "_"
                    self.Moves[j].BlackMove.Variant.put_moves_display(False)
                    self.panel.notation.insert(END, ') ')
                    self.main.LastCharCount = self.main.LastCharCount + 2
                    self.main.LeadingTag = self.main.LeadingTag[0:len(self.main.LeadingTag)-4]

                self.HalfPlyDisplayPointer = self.HalfPlyDisplayPointer + 1

        tag = self.main.LeadingTag + str(self.HalfPlyDisplayPointer).zfill(3)
        self.panel.notation.insert(END, '#', tag)
        self.panel.notation.tag_bind(tag, "<Button-1>", ClickMoveTag(self.main.re_play, tag))
        #self.main.panel.notation.yview_scroll(self.line_nr(self.main.panel.notation.tag_ranges(tag)[0]), UNITS)
        return

    def is_ply_nr(self, s, n):
        return (s[n] == ' ' or ('0' <= s[n] and s[n] <= '9')) and ('0' <= s[n+1] and s[n+1] <= '9') and (s[n+2] == '.') or \
            ( ('0' <= s[n] and s[n] <= '9') and  ('0' <= s[n+1] and s[n+1] <= '9') and  ('0' <= s[n+2] and s[n+2] <= '9') and (s[n+3] == '.'))

    def add_to_tail(self, half_ply):
        if half_ply.Cap == []: half_ply.op = '-'
        else: half_ply.op = 'x'
        if self.CurrentPosition.ColorOnMove == dc.BLACK_ON_MOVE: #do_move has already toggled colors
            move = PlyRecord()
            move.WhiteMove = half_ply
            self.CurrentPlyCounter = self.CurrentPlyCounter + 1
            move.Ply_nr = self.CurrentPlyCounter
            self.Moves.append(move)

        elif len(self.Moves) > 0:
            self.Moves[len(self.Moves) - 1].BlackMove = half_ply
        else:
            move = PlyRecord()
            move.BlackMove = half_ply
            self.CurrentPlyCounter = self.CurrentPlyCounter + 1
            move.Ply_nr = self.CurrentPlyCounter
            self.Moves.append(move)

        self.HalfPlyPointer = self.HalfPlyPointer + 1
        return

    def remove_from_tail(self):
        if len(self.Moves) > 0:
            if self.Moves[len(self.Moves) - 1].BlackMove:
                self.Moves[len(self.Moves) - 1].BlackMove = None
            else:
                self.Moves.pop() # remove the last entry

        self.HalfPlyPointer = self.half_ply_length()
        return

    def get_half_ply_pointer_record(self):
        if len(self.Moves) > 0:
            calc_pnt = self.HalfPlyPointer
            if not self.Moves[0].WhiteMove: calc_pnt = calc_pnt + 1
            calc_mpnt = int(calc_pnt / 2)
            diff = calc_pnt - 2*calc_mpnt
            if calc_mpnt >= len(self.Moves): return None
            if diff == 1: return self.Moves[calc_mpnt].BlackMove
            else: return self.Moves[calc_mpnt].WhiteMove
        else:
            return None

    def get_previous_half_ply_pointer_record(self):
        if len(self.Moves) > 0:
            calc_pnt = self.HalfPlyPointer - 1
            if not self.Moves[0].WhiteMove: calc_pnt = calc_pnt + 1
            calc_mpnt = int(calc_pnt / 2)
            diff = calc_pnt - 2*calc_mpnt
            if calc_mpnt >= len(self.Moves): return None
            if diff == 1: return self.Moves[calc_mpnt].BlackMove
            else: return self.Moves[calc_mpnt].WhiteMove
        else:
            return None

    def get_half_ply_length(self):
        if len(self.Moves) > 0:
            calc_pnt = 2*len(self.Moves)
            if self.Moves[0].WhiteMove: calc_pnt -=  1
            if self.Moves[-1].BlackMove: calc_pnt -= 1
            return calc_pnt
        else: return 0

    def get_half_ply_pointer_ply_record(self):
        calc_pnt = self.HalfPlyPointer
        if self.Moves[0].WhiteMove: calc_pnt = calc_pnt + 1
        calc_mpnt = int(calc_pnt / 2)
        return self.Moves[calc_mpnt]

    def get_50_sq_position(self, include_color=False):
        if include_color == True:
          if self.CurrentPosition.ColorOnMove == dc.WHITE_ON_MOVE:
            s = "W"
          else: s = "B"
        else:
          s = ''
        for i in range(1,51):
            if i in self.CurrentPosition.WhiteMan:
                s = s + 'w'
            elif i in self.CurrentPosition.WhiteKing:
                s = s + 'W'
            elif i in self.CurrentPosition.BlackMan:
                s = s + 'z'
            elif i in self.CurrentPosition.BlackKing:
                s = s + 'Z'
            else: s = s + 'e'
        return s

    def set_50_square_position(self, stand):
      self.CurrentPosition.WhiteMan  = []
      self.CurrentPosition.WhiteKing = []
      self.CurrentPosition.BlackMan  = []
      self.CurrentPosition.BlackKing = []
      if stand[0] == "W": self.CurrentPosition.ColorOnMove = dc.WHITE_ON_MOVE
      else: self.CurrentPosition.ColorOnMove = dc.BLACK_ON_MOVE
      for i in range(1,51):
        if stand[i] == "w":
          self.CurrentPosition.WhiteMan .append(i)
        elif stand[i] == "W":
          self.CurrentPosition.WhiteKing.append(i)
        elif stand[i] == "z":
          self.CurrentPosition.BlackMan .append(i)
        elif stand[i] == "Z":
          self.CurrentPosition.BlackKing.append(i)
      return

    def set_50_square_position_from_LDF(self, stand):
      self.CurrentPosition.WhiteMan  = []
      self.CurrentPosition.WhiteKing = []
      self.CurrentPosition.BlackMan  = []
      self.CurrentPosition.BlackKing = []
      if stand[0] == "W": self.CurrentPosition.ColorOnMove = dc.WHITE_ON_MOVE
      else: self.CurrentPosition.ColorOnMove = dc.BLACK_ON_MOVE
      for i in range(1,51):
        if stand[i] == "w":
          self.CurrentPosition.WhiteMan .append(i)
        elif stand[i] == "W":
          self.CurrentPosition.WhiteKing.append(i)
        elif stand[i] == "b":
          self.CurrentPosition.BlackMan .append(i)
        elif stand[i] == "B":
          self.CurrentPosition.BlackKing.append(i)
      return

    def is_standard_begin_position(self):
        return set(self.BeginPosition.BlackMan) == set(range(1,21)) and \
               set(self.BeginPosition.WhiteMan) == set(range(31,51)) and \
               self.BeginPosition.BlackKing == [] and \
               self.BeginPosition.WhiteKing == [] and \
               self.BeginPosition.ColorOnMove == dc.WHITE_ON_MOVE


    def is_standard_current_position(self):
        return set(self.CurrentPosition.BlackMan) == set(range(1,21)) and \
               set(self.CurrentPosition.WhiteMan) == set(range(31,51)) and \
               self.CurrentPosition.BlackKing == [] and \
               self.CurrentPosition.WhiteKing == [] and \
               self.CurrentPosition.ColorOnMove == dc.WHITE_ON_MOVE





#-----------------------------------------------------------------------------------------------------#

class HalfPlyRecord:
    """ To store information about a move (= 1/2 ply)

    Info stored:    start   the begin position of the move or capture
                    stop    the end position of the move or capture
                    cap     a list with all captures (field nrs)
                    step    a list of all steps in between in case of a capture
    """

    def __init__(self):
        self.Start = ''
        self.Stop = ''
        self.op = ''
        self.Cap = []
        self.Step = []
        self.Unique = True
        self.Comment = ''
        self.Variant = ''  # to store variances, starting here
        self.MoveValue = 0
        return

    def __str__(self):
        s = repr(self.Start).rjust(2) + self.op + repr(self.Stop).rjust(2) + ' ' + str(self.Step) + ' (' + str(self.MoveValue).rjust(6) +')'
        if self.Comment <> '': s = s + ' {' + self.Comment + '}'
        if self.Variant <> '':
            s = s + '('
            s = s + str(self.Variant)
            s = s + ' )'
        return s

    def __lt__(self, other):
        return self.MoveValue < other.MoveValue

    def __le__(self, other):
        return self.MoveValue <= other.MoveValue

    def __eq__(self, other):
        return self.MoveValue == other.MoveValue

    def __ge__(self, other):
        return self.MoveValue >= other.MoveValue

    def __gt__(self, other):
        return self.MoveValue > other.MoveValue


    def get_damexchange_move_string(self):
        s = repr(self.Start).zfill(2) + repr(self.Stop).zfill(2)
        cnt = len(self.Cap)
        s = s + repr(cnt).zfill(2)
        for i in range(0, cnt):
            s = s + str(self.Cap[i]).zfill(2)
        return s



class PlyRecord:
    def __init__(self):
        self.Ply_nr = ''
        self.WhiteMove = None
        self.BlackMove = None
        return

    def __str__(self):
        if self.Ply_nr <> '': s = repr(self.Ply_nr).rjust(2) + '. '
        else: s = ''
        if self.WhiteMove : s = s + str(self.WhiteMove) + ' '
        if self.BlackMove : s = s + str(self.BlackMove)
        return s + '\n'


