#
#  This file is Copyright (C) 2010 Feike Boomstra.
#  Distributed under the Boost Software License, Version 1.0.
#  (See accompanying file LICENSE_1_0.txt or copy at
#  http://www.boost.org/LICENSE_1_0.txt)
#


from DraughtConstants import *
from DraughtDataStructures import *
dc = DraughtConstants()


class DraughtSetUp:
    """ Routines to serve the setup phase.

    """
    def __init__(self, main, panel):
        self.main = main
        self.panel = panel
        self.LastLeftPiece = "undecided"
        self.LastRightPiece = "undecided"
        return
    def next_piece(self, piece):
      if piece == "white man":
        return "white king"
      elif piece == "white king":
        return "empty (white)"
      elif piece == "empty (white)":
        return "black man"
      elif piece == "black man":
        return "black king"
      elif piece == "black king":
        return "empty (black)"
      elif piece == "empty (black)":
        return "white man"
      else:
        print "incorrect piece found"
        return "white man"

    def get_piece_on_field(self, game, i):
      if i in game.CurrentPosition.WhiteMan:
        return "white man"
      elif i in game.CurrentPosition.BlackMan:
        return "black man"
      elif i in game.CurrentPosition.WhiteKing:
        return "white king"
      elif i in game.CurrentPosition.BlackKing:
        return "black king"
      else:
        return "empty"

    def piece_is_equal_to_field(self, piece, field):
      if field == "empty" and (piece == "empty (white)" or piece == "empty (black)"):
        return True
      else:
        return piece == field

    def set_begin_position_on_board(self, game):
        # make all lists empty
        game.CurrentPosition.WhiteMan = range(31, 51)
        game.CurrentPosition.WhiteKing = []
        game.CurrentPosition.BlackMan = range(1, 21)
        game.CurrentPosition.BlackKing = []
        game.CurrentPosition.selected = []
        game.BeginPosition.WhiteMan = range(31, 51)
        game.BeginPosition.WhiteKing = []
        game.BeginPosition.BlackMan = range(1, 21)
        game.BeginPosition.BlackKing = []

        self.panel.current_color_on_move.set(dc.WHITE_ON_MOVE)
        game.CurrentPosition.ColorOnMove = dc.WHITE_ON_MOVE  # and local
        game.BeginPosition.ColorOnMove = dc.WHITE_ON_MOVE  # and local

        # preset the pieces
        for i in range(1,21):
            game.ActivePanel.set_black_man(i, bool(False))
        for i in range(21, 31):
            game.ActivePanel.set_empty_field(i, bool(False))
        for i in range(31, 51):
            game.ActivePanel.set_white_man(i, bool(False))
        if self.panel.invert_board.get == 1:
            self.panel.black_numbers_display.set(str(len(game.CurrentPosition.BlackMan)).rjust(2) + " (" + str(len(game.CurrentPosition.BlackKing)) + ")")
            self.panel.white_numbers_display.set(str(len(game.CurrentPosition.WhiteMan)).rjust(2) + " (" + str(len(game.CurrentPosition.WhiteKing)) + ")")
        else:
            self.panel.white_numbers_display.set(str(len(game.CurrentPosition.BlackMan)).rjust(2) + " (" + str(len(game.CurrentPosition.BlackKing)) + ")")
            self.panel.black_numbers_display.set(str(len(game.CurrentPosition.WhiteMan)).rjust(2) + " (" + str(len(game.CurrentPosition.WhiteKing)) + ")")

        self.clear_moves(game)
        self.main.play.get_move_from_book = True
        self.LastLeftPiece = "undecided"
        self.LastRightPiece = "undecided"
        return

    def setup_board(self):    # called when setup is clicked

        if len(self.main.ActiveGame.Moves) > 0:
            game = GameRecord(self.main, self.panel, self.panel)
            self.main.Games.append(game)
            self.main.ActiveGame = game
            self.main.ActiveGameNumber += 1

        self.set_begin_position_on_board(self.main.ActiveGame)

        # new game record to store moves when playing

        return

    def clear_moves(self, game):
        game.Moves = []
        game.HalfPlyPointer = 0
        self.panel.notation_display.set("")
        game.refresh_display()
        return

    def clear_board(self):
        game = self.main.ActiveGame
        game.CurrentPosition.WhiteMan = []
        game.CurrentPosition.WhiteKing = []
        game.CurrentPosition.BlackMan = []
        game.CurrentPosition.BlackKing = []
        game.CurrentPosition.selected = []
        self.panel.current_color_on_move.set(dc.WHITE_ON_MOVE)
        game.CurrentPosition.ColorOnMove = dc.WHITE_ON_MOVE  # and local
        game.BeginPosition.WhiteMan = []
        game.BeginPosition.WhiteKing = []
        game.BeginPosition.BlackMan = []
        game.BeginPosition.BlackKing = []
        game.BeginPosition.ColorOnMove = dc.WHITE_ON_MOVE  # and local
        game.WhitePlayer = ''
        game.BlackPlayer = ''
        game.Result = ''
        game.Event = ''
        game.Date = ''
        game.Round = ''
        for i in range(1,51):
            self.panel.set_empty_field(i, bool(False))
        self.clear_moves(game)
        self.LastLeftPiece = "undecided"
        self.LastRightPiece = "undecided"
        return

    def set_white_piece(self, i, game):
        if game.CurrentPosition.is_empty_field(i):
            if i in [1, 2, 3, 4, 5]:
                self.panel.set_white_king(i, bool(False))
                game.CurrentPosition.WhiteKing.append(i)
                game.BeginPosition.WhiteKing.append(i)
            else:
                self.panel.set_white_man(i, bool(False))
                game.CurrentPosition.WhiteMan.append(i)
                game.BeginPosition.WhiteMan.append(i)
        elif i in game.CurrentPosition.WhiteMan:
            game.CurrentPosition.WhiteMan.remove(i)
            game.BeginPosition.WhiteMan.remove(i)
            game.CurrentPosition.WhiteKing.append(i)
            game.BeginPosition.WhiteKing.append(i)
            self.panel.set_white_king(i, bool(False))
        else:
            if i in game.CurrentPosition.WhiteKing: game.CurrentPosition.WhiteKing.remove(i)
            if i in game.BeginPosition.WhiteKing:   game.BeginPosition.WhiteKing.remove(i)
            if i in game.CurrentPosition.BlackKing: game.CurrentPosition.BlackKing.remove(i)
            if i in game.BeginPosition.BlackKing:   game.BeginPosition.BlackKing.remove(i)
            if i in game.CurrentPosition.BlackMan:  game.CurrentPosition.BlackMan.remove(i)
            if i in game.BeginPosition.BlackMan:    game.BeginPosition.BlackMan.remove(i)
            self.panel.set_empty_field(i, bool(False))
        # print self.bord
        self.clear_moves(game)
        game.refresh_number_of_pieces_display()
        return

    def set_black_piece(self, i, game):
        if game.CurrentPosition.is_empty_field(i):
            if i in [46, 47, 48, 49, 50]:
                self.panel.set_black_king(i, bool(False))
                game.CurrentPosition.BlackKing.append(i)
                game.BeginPosition.BlackKing.append(i)
            else:
                self.panel.set_black_man(i, bool(False))
                game.CurrentPosition.BlackMan.append(i)
                game.BeginPosition.BlackMan.append(i)
        elif i in game.CurrentPosition.BlackMan:
            game.CurrentPosition.BlackMan.remove(i)
            game.BeginPosition.BlackMan.remove(i)
            game.CurrentPosition.BlackKing.append(i)
            game.BeginPosition.BlackKing.append(i)
            self.panel.set_black_king(i, bool(False))
        else:
            if i in game.CurrentPosition.BlackKing: game.CurrentPosition.BlackKing.remove(i)
            if i in game.BeginPosition.BlackKing: game.BeginPosition.BlackKing.remove(i)
            if i in game.CurrentPosition.WhiteKing: game.CurrentPosition.WhiteKing.remove(i)
            if i in game.BeginPosition.WhiteKing: game.BeginPosition.WhiteKing.remove(i)
            if i in game.CurrentPosition.WhiteMan: game.CurrentPosition.WhiteMan.remove(i)
            if i in game.BeginPosition.WhiteMan: game.BeginPosition.WhiteMan.remove(i)
            self.panel.set_empty_field(i, bool(False))
        self.clear_moves(game)
        game.refresh_number_of_pieces_display()
        return



    def build_interface_string(self, bord):
        self.bord.ColorOnMove = self.panel.current_color_on_move.get()
        if bord.is_color_white():
            s = chr(ord('(')+1)
        else:
            s = chr(ord('('))
        s = s + '(('
        for i in bord.WhiteMan:
            s = s + chr(ord('(')+i)
        s = s + '('
        for i in bord.WhiteKing:
            s = s + chr(ord('(')+i)
        s = s + '('
        for i in bord.BlackMan:
            s = s + chr(ord('(')+i)
        s = s + '('
        for i in bord.BlackKing:
            s = s + chr(ord('(')+i)
        s = s + '('
        s = s + '\n'
        # print "send = ", s
        return s

    def build_board_from_string(self, s):
        self.clear_board()
        if s[0] == chr(ord('(')+1):
            self.panel.current_color_on_move.set(dc.WHITE_ON_MOVE)
        else:
            self.panel.current_color_on_move.set(dc.BLACK_ON_MOVE            )
        i = 3                       # skip the move - part
        while s[i] <> '(':
            self.panel.set_white_man(ord(s[i]) - ord('('), bool(False))
            self.bord.WhiteMan.append(ord(s[i]) - ord('('))
            i = i + 1
        i = i + 1
        while s[i] <> '(':
            self.panel.set_white_king(ord(s[i]) - ord('('), bool(False))
            self.bord.WhiteKing.append(ord(s[i]) - ord('('))
            i = i + 1
        i = i + 1
        while s[i] <> '(':
            self.panel.set_black_man(ord(s[i]) - ord('('), bool(False))
            self.bord.BlackMan.append(ord(s[i]) - ord('('))
            i = i + 1
        i = i + 1
        while s[i] <> '(':
            self.panel.set_black_king(ord(s[i]) - ord('('), bool(False))
            self.bord.BlackKing.append(ord(s[i]) - ord('('))
            i = i + 1
        return

    def file_open_clicked(self):
        self.fdlg = FileDialog.LoadFileDialog(self.root, title="Choose A BRD File")
        self.fname = self.fdlg.go(pattern="*.brd", dir_or_file=os.curdir + '\\brd files') # opt args: dir_or_file=os.curdir, pattern="*", default="", key=None)
        print self.fname
        self.chr_pnt=0
        self.entire_file=None
        self.file_open_flag = True
        try:
            self.f=open(self.fname, 'rb')
            self.entire_file=self.f.read()
        except:
            self.file_open_flag = False
        if self.file_open_flag == True:
            self.build_board_from_string(self.entire_file)
        return



    def file_open_game_clicked(self):
        ok = self.parse.get_pdn_file(False)
        try:
            self.parse.f.close()
        except:
            print "Can't close pdn file"
        if ok == True:
            self.set_begin_position_on_board()
            self.bord.game_pnt = 0      # set to the first game
            self.bord.current_half_ply_counter = 0 # and first move
            self.bord.BoardIsModified = False
            self.play.refresh_board()
        return

    def file_save_clicked(self):
        self.fdlg = FileDialog.SaveFileDialog(self.root, title="Save A BRD File")
        if self.file_name == "":
            self.file_name = "test.pdn"
        self.file_name = self.fdlg.go(dir_or_file=os.curdir + '\\brd files', default = self.file_name, key = None) # opt args: dir_or_file=os.curdir, pattern="*", default="", key=None)
        self.file_open_flag = True
        try:
            self.f=open(self.file_name, 'w')
        except:
            print "File not created"
        self.entire_file = self.build_interface_string(self.bord)
        # print self.entire_file
        try:
            self.f.write(self.entire_file)
        except:
            print "File not written"
        try:
            self.f.close()
        except:
            print "File not closed"
        return

    def set_left_mouse_piece(self, field_nr, game):
      self.LastLeftPiece = self.set_piece(self.LastLeftPiece, field_nr, game)
      return

    def set_right_mouse_piece(self, field_nr, game):
      self.LastRightPiece = self.set_piece(self.LastRightPiece, field_nr, game)
      return

    def set_piece(self, piece, field_nr, game):
      field = self.get_piece_on_field(game, field_nr)
      # special case for the first time
      if piece == "undecided" and field == "empty":
        if field_nr in range(1, 26):
          piece = "black man"
        else:
          piece = "white man"
      elif piece == "undecided" and field == "white_man":
        piece = 'empty (black)'
      elif piece == "undecided" and field == "black_man":
        piece = 'empty (white)'
      elif piece == "undecided":
        piece = self.next_piece(field)

      if self.piece_is_equal_to_field(piece, field) == True:
        piece = self.next_piece(piece)
      if field <> "empty" or piece == "empty (white)" or piece == "empty (black)":
        if field_nr in game.CurrentPosition.WhiteKing: game.CurrentPosition.WhiteKing.remove(field_nr)
        if field_nr in game.BeginPosition.WhiteKing:   game.BeginPosition.WhiteKing.remove(field_nr)
        if field_nr in game.CurrentPosition.BlackKing: game.CurrentPosition.BlackKing.remove(field_nr)
        if field_nr in game.BeginPosition.BlackKing:   game.BeginPosition.BlackKing.remove(field_nr)
        if field_nr in game.CurrentPosition.WhiteMan:  game.CurrentPosition.WhiteMan.remove(field_nr)
        if field_nr in game.BeginPosition.WhiteMan:    game.BeginPosition.WhiteMan.remove(field_nr)
        if field_nr in game.CurrentPosition.BlackMan:  game.CurrentPosition.BlackMan.remove(field_nr)
        if field_nr in game.BeginPosition.BlackMan:    game.BeginPosition.BlackMan.remove(field_nr)
      # now add the piece
      if field_nr in [1, 2, 3, 4, 5] and piece == "white man":
        piece = self.next_piece(piece)
        self.panel.set_white_king(field_nr, bool(False))
        game.CurrentPosition.WhiteKing.append(field_nr)
        game.BeginPosition.WhiteKing.append(field_nr)
      elif field_nr in [46, 47, 48, 49, 50] and piece == "black man":
        piece = self.next_piece(piece)
        self.panel.set_black_king(field_nr, bool(False))
        game.CurrentPosition.BlackKing.append(field_nr)
        game.BeginPosition.BlackKing.append(field_nr)
      elif piece == "white man":
        self.panel.set_white_man(field_nr, bool(False))
        game.CurrentPosition.WhiteMan.append(field_nr)
        game.BeginPosition.WhiteMan.append(field_nr)
      elif piece == "black man":
        self.panel.set_black_man(field_nr, bool(False))
        game.CurrentPosition.BlackMan.append(field_nr)
        game.BeginPosition.BlackMan.append(field_nr)
      elif piece == "white king":
        self.panel.set_white_king(field_nr, bool(False))
        game.CurrentPosition.WhiteKing.append(field_nr)
        game.BeginPosition.WhiteKing.append(field_nr)
      elif piece == "black king":
        self.panel.set_black_king(field_nr, bool(False))
        game.CurrentPosition.BlackKing.append(field_nr)
        game.BeginPosition.BlackKing.append(field_nr)
      else:
        self.panel.set_empty_field(field_nr, bool(False))
      self.clear_moves(game)
      game.refresh_number_of_pieces_display()
      return piece
