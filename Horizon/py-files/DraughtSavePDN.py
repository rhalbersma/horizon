#
#  This file is Copyright (C) 2010 Feike Boomstra.
#  Distributed under the Boost Software License, Version 1.0.
#  (See accompanying file LICENSE_1_0.txt or copy at
#  http://www.boost.org/LICENSE_1_0.txt)
#


from DraughtDataStructures import *
import FileDialog
from Tkinter import *
import os
import datetime
import copy
import symbol
from DraughtConstants import *
from DraughtGamePanel import *

dc = DraughtConstants()

class DraughtSavePDN:

    def __init__(self, main, root = None):
        if root==None:
            root=Tk()
        self.root          = root
        self.main          = main
        self.s             = ''                # the string with the input to analyse
        self.cp            = 0                 # current position pointer in this string
        self.file_name = ''
        return

    def file_save_game_clicked_3(self):

        self.entire_file = self.game_to_string(self.main.ActiveGame)
        print self.entire_file
        return


    def file_save_game_clicked(self, file_name = None):
      if file_name == None:
        self.fdlg = FileDialog.SaveFileDialog(self.root, title="Save A PDN File")
        if self.file_name == "":
            self.file_name = "test.pdn"
        self.file_name = self.fdlg.go(dir_or_file='/home/feike/Horizon/pdn-files', \
                                      default = self.file_name, key = None)
        self.file_open_flag = True
        try:
            self.f=open(self.file_name, 'w')
        except:
            print "File not created"
      else:
        try:
            self.f=open(file_name, 'w')
        except:
            print "File not created"

      self.entire_file = ''
      for game in self.main.Games:
          self.entire_file = self.entire_file + self.game_to_string(game)
      print self.entire_file

      try:
          self.f.write(self.entire_file)
      except:
          print "File not written"
      try:
          self.f.close()
      except:
          print "File not closed"
      return

    def game_to_string(self, game):
        s = ''
        if game.WhitePlayer <> '': s = s + '[White "'+ str(game.WhitePlayer) +'"]\n'
        if game.BlackPlayer <> '': s = s + '[Black "'+ str(game.BlackPlayer) +'"]\n'
        if game.Result <> '':      s = s + '[Result "'+ str(game.Result) +'"]\n'
        if game.Event <> '':       s = s + '[Event "'+ str(game.Event) +'"]\n'
        if game.Date <> '':        s = s + '[Date "'+ str(game.Date) +'"]\n'
        if game.Round <> '':       s = s + '[Round "'+ str(game.Round) +'"]\n'
        if game.GameType <> '':    s = s + '[GameType "'+ str(self.Round) +'"]\n'

        # test if beginposition == defaultposition

        if set(game.BeginPosition.BlackMan) == set(range(1,21)) and \
           set(game.BeginPosition.WhiteMan) == set(range(31,51)):
            pass
        else:
            s = s + '[FEN "'
            if game.BeginPosition.ColorOnMove == dc.WHITE_ON_MOVE:
                s = s + "W:W"
            else: s = s + "B:W"
            if game.BeginPosition.WhiteMan <> []:
                for sq in game.BeginPosition.WhiteMan:
                    s = s + str(sq) + ','
            if game.BeginPosition.WhiteKing <> []:
                for sq in game.BeginPosition.WhiteKing:
                    s = s + 'K' + str(sq) + ','
            s = s[0: len(s)-1]      # remove the last comma
            s = s + ":\nB"
            if game.BeginPosition.BlackMan <> []:
                for sq in game.BeginPosition.BlackMan:
                    s = s + str(sq) + ','
            if game.BeginPosition.BlackKing <> []:
                for sq in game.BeginPosition.BlackKing:
                    s = s + 'K' + str(sq) + ','
            s = s[0: len(s)-1]      # remove the last comma
            s = s + '"]\n'
        # This was the header, now the moves

        s = s + self.get_moves_pdn_string(game) + ' *'


        return s

    def get_moves_pdn_string(self, game, format = True):
        k = 0
        s = ''

        for j in range(0,len(game.Moves)):
            if game.Moves[j].Ply_nr != '':
              s = s + repr(game.Moves[j].Ply_nr).zfill(2) + "."
            if game.Moves[j].WhiteMove:
                if (len(game.Moves[j].WhiteMove.Step) == 2) or (game.Moves[j].WhiteMove.Unique == True):
                    s = s + repr(game.Moves[j].WhiteMove.Start).zfill(2)
                    s = s + game.Moves[j].WhiteMove.op
                    s = s + repr(game.Moves[j].WhiteMove.Stop).zfill(2) + " "
                else:
                    for step in game.Moves[j].WhiteMove.Step:
                        s = s + repr(step).zfill(2) + 'x'
                    s = s[0:-1]
                    s = s + ' ' # replace the last 'x'
                if game.Moves[j].WhiteMove.Variant <> '':
                   s = s + "(" + self.get_moves_pdn_string(game.Moves[j].WhiteMove.Variant, False) + ") "
                if game.Moves[j].WhiteMove.Comment <> '':
                    s = s +  self.check_pv_comment_to_string(game.Moves[j].WhiteMove, game.Moves[j].WhiteMove.Comment, True)

            if game.Moves[j].BlackMove :
                if (len(game.Moves[j].BlackMove.Step) == 2) or (game.Moves[j].BlackMove.Unique == True):
                    s = s + repr(game.Moves[j].BlackMove.Start).zfill(2)
                    s = s + game.Moves[j].BlackMove.op
                    s = s + repr(game.Moves[j].BlackMove.Stop).zfill(2) + " "
                else:
                    for step in game.Moves[j].BlackMove.Step:
                        s = s + repr(step).zfill(2) + 'x'
                    s = s[0:-1]
                    s = s + ' ' # replace the last 'x'
                if game.Moves[j].BlackMove.Variant <> '':
                   s = s + "(" + self.get_moves_pdn_string(game.Moves[j].BlackMove.Variant, False) + ") "
                if game.Moves[j].BlackMove.Comment <> '':
                    s = s +  self.check_pv_comment_to_string(game.Moves[j].BlackMove, game.Moves[j].BlackMove.Comment, False)
                if format == True: s = s + '\n'

        return s

    def check_pv_comment_to_string(self, move, comment, white):
      # during the game the evaluated value (and the pv) may be stored in the comment field
      # Horizon on move: nnn
      # Truus on move: "val " n.nn "pv " list of moves

      if comment[0:5] == '%eval' : return '{' + comment + '} '

      s = ''
      # first distinguish between Horzion and Truus:
      if comment[0:3] == "val":
        # it is truus, but is the pv a valid pv?
        new_game = GameRecord(self.main, '', self.main.panel, self.main.ActiveGame)
        self.main.read.s = comment[13:] + '\0'
        self.main.read.cp = 0
        if white==True:
          moves_end = self.main.read.GetWhiteStartMoves(0, new_game)
          if moves_end > 0: same_move = (move.Start == new_game.Moves[0].WhiteMove.Start) and (move.Stop == new_game.Moves[0].WhiteMove.Stop)
          else: same_move = False
        else:
          moves_end = self.main.read.GetBlackStartMoves(0, new_game)
          if moves_end > 0: same_move = (move.Start == new_game.Moves[0].BlackMove.Start) and (move.Stop == new_game.Moves[0].BlackMove.Stop)
          else: same_move = False
          print  move.Start, new_game.Moves[0].BlackMove.Start,  move.Stop, new_game.Moves[0].BlackMove.Stop
        if same_move == True: # moves
          s = s + '{%eval "truus" ' + comment[4:8] + '} ' + '(' + comment[13:13+moves_end] + ')'
        else:
          pass
      elif comment.isdigit() == True or (comment[0] == '-' and comment[1:].isdigit() == True):
        s = s + '{%eval "Horizon" ' + comment + '} '
      else:
        s = s + '{' + comment + '} '
      return s

    def doc_op(self,  ply):
      if ply.op == '-':
        return '-'
      else:
        return '*'


    def save_doc_file(self, doc_file_name = None):  # this format can be read by TRUUS
        if doc_file_name == None:
          self.fdlg = FileDialog.SaveFileDialog(self.root, title="Save A DOC File")
          if self.file_name == "":
              self.file_name = "truus2009.doc"
          #self.file_name = self.fdlg.go(dir_or_file='/home/feike/BoomstraDam/doc files', \
          self.file_name = self.fdlg.go(dir_or_file='/home/feike/workspace', \
                                        default = self.file_name, key = None)
        else:
          self.file_name = doc_file_name
        self.file_open_flag = True
        try:
            self.f=open(self.file_name, 'w')
        except:
            print "File not created"
            return
        s = ''
        for game in self.main.Games:
          if game.is_standard_begin_position():
            s = s + '\015\012'
            wp = game.WhitePlayer
            if wp == '': wp = "white player"
            bp = game.BlackPlayer
            if bp == '': bp = "black player"
            s = s + str(wp).ljust(24) + '  -  ' + str(bp).ljust(24) + '       00.00 / 00.00' + '\015\012'
            for move in game.Moves:
              s = s + str(move.Ply_nr).zfill(2) + '.' + \
              str(move.WhiteMove.Start).zfill(2) + self.doc_op(move.WhiteMove) + str(move.WhiteMove.Stop).zfill(2)
              if move.BlackMove:
                s = s + ' ' + str(move.BlackMove.Start).zfill(2) + self.doc_op(move.BlackMove) + str(move.BlackMove.Stop).zfill(2)
              s = s + '\015\012'
            s = s + '\015\012' + '\015\012'
            self.f.write(s)
        self.f.close()

        return

    def make_unique_file_name(self):
      base = str(datetime.date.today())
#      for k in range(0, 1000):
#        if os.path.exists('/home/feike/Horizon/pdn-files/test-files/' + base + '.' + str(k).zfill(3) + '.pdn') == False:
#          break
#      return base + '.' + str(k).zfill(3) + '.pdn'
      return '/home/feike/Horizon/pdn-files/test-files/' + base + '.pdn'

    def copy_current_to_begin_position(self, old_game, new_game):
        new_game.BeginPosition.WhiteMan = old_game.CurrentPosition.WhiteMan[:]
        new_game.BeginPosition.WhiteKing = old_game.CurrentPosition.WhiteKing[:]
        new_game.BeginPosition.BlackMan = old_game.CurrentPosition.BlackMan[:]
        new_game.BeginPosition.BlackKing = old_game.CurrentPosition.BlackKing[:]
        new_game.BeginPosition.ColorOnMove = old_game.CurrentPosition.ColorOnMove
        new_game.HalfPlyPointer = 0;
        return



    def save_single_move_position(self, single_move_file_name = None):  # to build a test-set
        self.file_name = self.make_unique_file_name()
        self.file_open_flag = True
        try:
            self.f=open(self.file_name, 'a')
        except:
            print "File not created"
            return
        s = ''
        new_game = GameRecord(self.main, self.main.panel, self.main.panel, self.main.ActiveGame)
        self.copy_current_to_begin_position(self.main.ActiveGame, new_game);
        new_game.set_begin_position()
        self.game_panel = DraughtSingleMoveInfo(self.main, new_game)
        self.game_panel.root.mainloop()

        return


    def Read_Move_Info(self, game):
        new_move = PlyRecord()
        if self.game_panel.entry_is_move(self.game_panel.best_move_text) == True:
          start, stop = self.game_panel.get_start_stop(self.game_panel.best_move_text)
          ok, new_ply = game.start_stop_to_move_record(start, stop)
        else:
          ok = False
        if ok == True:

          # now the remaining fields:
          if self.game_panel.entry_is_move(self.game_panel.wrong_move_text) == True:
            start, stop = self.game_panel.get_start_stop(self.game_panel.wrong_move_text)
            ok, wrong_move = game.start_stop_to_move_record(start, stop)
            if ok == True:
              new_ply.Comment = "%wrong " + repr(wrong_move.Start).rjust(2) + wrong_move.op + repr(wrong_move.Stop).rjust(2)
          if self.game_panel.entry_is_move(self.game_panel.alt1) == True:
            start, stop = self.game_panel.get_start_stop(self.game_panel.alt1)
            ok, alt1 = game.start_stop_to_move_record(start, stop)
            if ok == True:
              new_ply.Comment += " %alt1 " + repr(alt1.Start).rjust(2) + alt1.op + repr(alt1.Stop).rjust(2)
          if self.game_panel.entry_is_move(self.game_panel.alt2) == True:
            start, stop = self.game_panel.get_start_stop(self.game_panel.alt2)
            ok, alt2 = game.start_stop_to_move_record(start, stop)
            if ok == True:
              new_ply.Comment += " %alt2 " + repr(alt2.Start).rjust(2) + alt2.op + repr(alt2.Stop).rjust(2)
          if self.game_panel.entry_is_move(self.game_panel.alt3) == True:
            start, stop = self.game_panel.get_start_stop(self.game_panel.alt3)
            ok, alt3 = game.start_stop_to_move_record(start, stop)
            if ok == True:
              new_ply.Comment += " %alt3 " + repr(alt3.Start).rjust(2) + alt3.op + repr(alt3.Stop).rjust(2)

          game.Moves.append(new_move)
          if game.BeginPosition.ColorOnMove == dc.WHITE_ON_MOVE:
            game.Moves[-1].WhiteMove = new_ply
          else:
            game.Moves[-1].BlackMove = new_ply
          self.entire_file = self.game_to_string(game)
          print self.entire_file
          self.f.write(self.entire_file)


        self.f.close()
        self.game_panel.root.destroy()

        return
