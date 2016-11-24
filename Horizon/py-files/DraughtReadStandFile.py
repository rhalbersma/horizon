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
import copy
import symbol
from DraughtConstants import *

dc = DraughtConstants()

    # local variables

class ReadStand:
    def __init__(self, main, root = None):
        if root==None:
            root=Tk()
        self.root          = root
        self.main          = main
        self.s             = ''                # the string with the input to analyse ans display
        self.cp            = 0                 # current position pointer
        self.debug = None
        self.PrintFlagSet = False
        return


#    def get_stand_file_prev(self):
#        self.s             = ''                # the string with the input to analyse
#        self.cp            = 0                 # current position pointer in this string
#        self.fdlg = FileDialog.LoadFileDialog(self.root, title="Choose A Stand File")
#        self.fname = self.fdlg.go(pattern="*.stand", dir_or_file='/home/feike/BoomstraDam/stand files')
#        self.file_open_flag = True
#        try:
#            self.f = open(self.fname, 'r')
#        except:
#            self.file_open_flag = False
#        if self.file_open_flag == True:
#            # reset the Games environment
#            self.main.Games = []
#            game = GameRecord(self.main, self.main.panel, self.main.panel)
#            self.main.Games.append(game)
#            self.main.ActiveGameNumber = 0
#            self.main.ActiveTag = ''
#            # now enter a new game for each line in the stand file
#            for line in self.f:
#                print "enter new line"
#                WhiteRating = int(line[5:9])
#                BlackRating = int(line[10:14])
#                MoveNr = int(line[15:17])
#                WhiteResult = line[18:19]
#                BlackResult = line[20:21]
#                Result = WhiteResult + '-' + BlackResult
#                pos = line[22:73]
#                self.enter_pos(game,  pos, WhiteRating, BlackRating, MoveNr, Result)
#                # is entered, make room for a new position
#                game = GameRecord(self.main, self.main.panel, self.main.panel)
#                self.main.Games.append(game)
#                self.main.ActiveGameNumber += 1
#            self.main.ActiveGameNumber = 0
#            self.main.ActiveGame = self.main.Games[0]
#            self.main.ActiveGame.refresh_display()
#        return
#
#
#    def enter_pos_prev(self, game, pos, WhiteRating, BlackRating, MoveNr, Result):
#        game.WhitePlayer = str(WhiteRating)
#        game.BlackPlayer = str(BlackRating)
#        game.Event = ''                                     # PDN element
#        game.Date = ''                                      # PDN element
#        game.Result = Result
#        game.Round = str(MoveNr)
#
#        game.BeginPosition.WhiteMan = []
#        game.BeginPosition.BlackMan = []
#        game.BeginPosition.WhiteKing = []
#        game.BeginPosition.BlackKing = []
#        game.BeginPosition.Selected = []
#        if pos[0] == 'W':
#            game.BeginPosition.ColorOnMove = dc.WHITE_ON_MOVE
#            game.CurrentPosition.ColorOnMove = dc.WHITE_ON_MOVE
#        else:
#            game.BeginPosition.ColorOnMove = dc.BLACK_ON_MOVE
#            game.CurrentPosition.ColorOnMove = dc.BLACK_ON_MOVE
#        for i in range(1,51):
#            if pos[i] == 'w':
#                game.BeginPosition.WhiteMan.append(i)
#                game.CurrentPosition.WhiteMan.append(i)
#            elif pos[i] == 'z':
#                game.BeginPosition.BlackMan.append(i)
#                game.CurrentPosition.BlackMan.append(i)
#            elif pos[i] == 'W':
#                game.BeginPosition.WhiteKing.append(i)
#                game.CurrentPosition.WhiteKing.append(i)
#            elif pos[i] == 'Z':
#                game.BeginPosition.BlackKing.append(i)
#                game.CurrentPosition.BlackKing.append(i)
#        return

    def get_stand_file_test(self):
        self.main.Games = []
        self.all_cap()
        self.main.ActiveGameNumber = 0
        self.main.ActiveGame = self.main.Games[0]
        self.main.ActiveTag = ''
        self.main.ActiveGame.refresh_display()
        return

    def insert_config(self,  white_man):
            game = GameRecord(self.main, self.main.panel, self.main.panel)
            game.BeginPosition.WhiteMan =white_man
            game.CurrentPosition.WhiteMan = white_man
            self.main.Games.append(game)
            return

    def get_stand_file(self):
        self.s             = ''                # the string with the input to analyse
        self.cp            = 0                 # current position pointer in this string
        self.fdlg = FileDialog.LoadFileDialog(self.root, title="Choose A Stand File")
        self.fname = self.fdlg.go(pattern="*.stand", dir_or_file='/home/feike/Horizon/Stand')
        self.file_open_flag = True
        try:
            self.f = open(self.fname, 'r')
        except:
            self.file_open_flag = False
        if self.file_open_flag == True:
            # reset the Games environment
            self.main.Games = []
            game = GameRecord(self.main, self.main.panel, self.main.panel)
            self.main.Games.append(game)
            self.main.ActiveGameNumber = 0
            self.main.ActiveGame = self.main.Games[0]
            self.main.ActiveTag = ''
            # now enter a new game for each line in the stand file
            MoveNr = 0
            for line in self.f:
                if len(line) > 60:
                    WhiteRating = int(line[5:9])
                    BlackRating = int(line[10:14])
                    MoveNr = int(line[15:17])
                    WhiteResult = line[18:19]
                    BlackResult = line[20:21]
                    Result = WhiteResult + '-' + BlackResult
                    pos = line[22:73]
                    self.enter_pos(game,  pos, WhiteRating, BlackRating, MoveNr, Result)
                else:
                    pos = line[0:52]
                    if len(pos) >= 51:
                        MoveNr += 1
                        if  MoveNr % 1000 == 0: print MoveNr
                        self.enter_pos(game,  pos, 0, 0, MoveNr, '0-0')
                # is entered, make room for a new position
                game = GameRecord(self.main, self.main.panel, self.main.panel)
                self.main.Games.append(game)
                self.main.ActiveGameNumber += 1
            self.main.ActiveGameNumber = 0
            self.main.ActiveGame = self.main.Games[0]
            self.main.ActiveGame.refresh_display()
        return
    def next_line(self,  event):
      if self.main.ActiveGameNumber < len(self.main.Games):
        self.main.ActiveGameNumber += 1
        self.main.ActiveGame = self.main.Games[self.main.ActiveGameNumber]
        self.main.ActiveGame.refresh_display()
      return

    def prev_line(self,  event):
      if self.main.ActiveGameNumber > 0:
        self.main.ActiveGameNumber -= 1
        self.main.ActiveGame = self.main.Games[self.main.ActiveGameNumber]
        self.main.ActiveGame.refresh_display()
      return

    def enter_pos(self, game, pos, WhiteRating, BlackRating, MoveNr, Result):
        game.WhitePlayer = str(WhiteRating)
        game.BlackPlayer = str(BlackRating)
        game.Event = ''                                     # PDN element
        game.Date = ''                                      # PDN element
        game.Result = Result
        game.Round = str(MoveNr)

        game.BeginPosition.WhiteMan = []
        game.BeginPosition.BlackMan = []
        game.BeginPosition.WhiteKing = []
        game.BeginPosition.BlackKing = []
        game.BeginPosition.Selected = []

        game.CurrentPosition.WhiteMan = []
        game.CurrentPosition.BlackMan = []
        game.CurrentPosition.WhiteKing = []
        game.CurrentPosition.BlackKing = []
        game.CurrentPosition.Selected = []

        if pos[0] == 'W':
            game.BeginPosition.ColorOnMove = dc.WHITE_ON_MOVE
            game.CurrentPosition.ColorOnMove = dc.WHITE_ON_MOVE
        else:
            game.BeginPosition.ColorOnMove = dc.BLACK_ON_MOVE
            game.CurrentPosition.ColorOnMove = dc.BLACK_ON_MOVE
        for i in range(1,51):
            if pos[i] == 'w':
                game.BeginPosition.WhiteMan.append(i)
                game.CurrentPosition.WhiteMan.append(i)
            elif pos[i] == 'z':
                game.BeginPosition.BlackMan.append(i)
                game.CurrentPosition.BlackMan.append(i)
            elif pos[i] == 'W':
                game.BeginPosition.WhiteKing.append(i)
                game.CurrentPosition.WhiteKing.append(i)
            elif pos[i] == 'Z':
                game.BeginPosition.BlackKing.append(i)
                game.CurrentPosition.BlackKing.append(i)

        return
    def all_cap(self):
        white_man = [7,12,18,23,29,34,40]
        self.insert_config(white_man)
        white_man = [8,13,19,24,30]
        self.insert_config(white_man)
        white_man = [9,14,20]
        self.insert_config(white_man)
        white_man = [10]
        self.insert_config(white_man)
        white_man = [11,17,22,28,33,39,44]
        self.insert_config(white_man)
        white_man = [12,18,23,29,34,40]
        self.insert_config(white_man)
        white_man = [13,19,24,30]
        self.insert_config(white_man)
        white_man = [14,20]
        self.insert_config(white_man)
        white_man = [17,22,28,33,39,44]
        self.insert_config(white_man)
        white_man = [18,23,29,34,40]
        self.insert_config(white_man)
        white_man = [19,24,30]
        self.insert_config(white_man)
        white_man = [20]
        self.insert_config(white_man)
        white_man = [21,27,32,38,43]
        self.insert_config(white_man)
        white_man = [22,28,33,39,44]
        self.insert_config(white_man)
        white_man = [23,29,34,40]
        self.insert_config(white_man)
        white_man = [24,30]
        self.insert_config(white_man)
        white_man = [27,32,38,43]
        self.insert_config(white_man)
        white_man = [28,33,39,44]
        self.insert_config(white_man)
        white_man = [29,34,40]
        self.insert_config(white_man)
        white_man = [30]
        self.insert_config(white_man)
        white_man = [31,37,42]
        self.insert_config(white_man)
        white_man = [32,38,43]
        self.insert_config(white_man)
        white_man = [33,39,44]
        self.insert_config(white_man)
        white_man = [34,40]
        self.insert_config(white_man)
        white_man = [37,42]
        self.insert_config(white_man)
        white_man = [38,43]
        self.insert_config(white_man)
        white_man = [39,44]
        self.insert_config(white_man)
        white_man = [40]
        self.insert_config(white_man)
        white_man = [41]
        self.insert_config(white_man)
        white_man = [42]
        self.insert_config(white_man)
        white_man = [43]
        self.insert_config(white_man)
        white_man = [44]
        self.insert_config(white_man)
        white_man = [7,11]
        self.insert_config(white_man)
        white_man = [8,12,17,21]
        self.insert_config(white_man)
        white_man = [9,13,18,22,27,31]
        self.insert_config(white_man)
        white_man = [10,14,19,23,28,32,37,41]
        self.insert_config(white_man)
        white_man = [11]
        self.insert_config(white_man)
        white_man = [12,17,21]
        self.insert_config(white_man)
        white_man = [13,18,22,27,31]
        self.insert_config(white_man)
        white_man = [14,19,23,28,32,37,41]
        self.insert_config(white_man)
        white_man = [17,21]
        self.insert_config(white_man)
        white_man = [18,22,27,31]
        self.insert_config(white_man)
        white_man = [19,23,28,32,37,41]
        self.insert_config(white_man)
        white_man = [20,24,29,33,38,42]
        self.insert_config(white_man)
        white_man = [21]
        self.insert_config(white_man)
        white_man = [22,27,31]
        self.insert_config(white_man)
        white_man = [23,28,32,37,41]
        self.insert_config(white_man)
        white_man = [24,29,33,38,42]
        self.insert_config(white_man)
        white_man = [27,31]
        self.insert_config(white_man)
        white_man = [28,32,37,41]
        self.insert_config(white_man)
        white_man = [29,33,38,42]
        self.insert_config(white_man)
        white_man = [30,34,39,43]
        self.insert_config(white_man)
        white_man = [31]
        self.insert_config(white_man)
        white_man = [32,37,41]
        self.insert_config(white_man)
        white_man = [33,38,42]
        self.insert_config(white_man)
        white_man = [34,39,43]
        self.insert_config(white_man)
        white_man = [37,41]
        self.insert_config(white_man)
        white_man = [38,42]
        self.insert_config(white_man)
        white_man = [39,43]
        self.insert_config(white_man)
        white_man = [40,44]
        self.insert_config(white_man)
        white_man = [41]
        self.insert_config(white_man)
        white_man = [42]
        self.insert_config(white_man)
        white_man = [43]
        self.insert_config(white_man)
        white_man = [44]
        self.insert_config(white_man)
        white_man = [7]
        self.insert_config(white_man)
        white_man = [8]
        self.insert_config(white_man)
        white_man = [9]
        self.insert_config(white_man)
        white_man = [10]
        self.insert_config(white_man)
        white_man = [11,7]
        self.insert_config(white_man)
        white_man = [12,8]
        self.insert_config(white_man)
        white_man = [13,9]
        self.insert_config(white_man)
        white_man = [14,10]
        self.insert_config(white_man)
        white_man = [17,12,8]
        self.insert_config(white_man)
        white_man = [18,13,9]
        self.insert_config(white_man)
        white_man = [19,14,10]
        self.insert_config(white_man)
        white_man = [20]
        self.insert_config(white_man)
        white_man = [21,17,12,8]
        self.insert_config(white_man)
        white_man = [22,18,13,9]
        self.insert_config(white_man)
        white_man = [23,19,14,10]
        self.insert_config(white_man)
        white_man = [24,20]
        self.insert_config(white_man)
        white_man = [27,22,18,13,9]
        self.insert_config(white_man)
        white_man = [28,23,19,14,10]
        self.insert_config(white_man)
        white_man = [29,24,20]
        self.insert_config(white_man)
        white_man = [30]
        self.insert_config(white_man)
        white_man = [31,27,22,18,13,9]
        self.insert_config(white_man)
        white_man = [32,28,23,19,14,10]
        self.insert_config(white_man)
        white_man = [33,29,24,20]
        self.insert_config(white_man)
        white_man = [34,30]
        self.insert_config(white_man)
        white_man = [37,32,28,23,19,14,10]
        self.insert_config(white_man)
        white_man = [38,33,29,24,20]
        self.insert_config(white_man)
        white_man = [39,34,30]
        self.insert_config(white_man)
        white_man = [40]
        self.insert_config(white_man)
        white_man = [41,37,32,28,23,19,14,10]
        self.insert_config(white_man)
        white_man = [42,38,33,29,24,20]
        self.insert_config(white_man)
        white_man = [43,39,34,30]
        self.insert_config(white_man)
        white_man = [44,40]
        self.insert_config(white_man)
        white_man = [7]
        self.insert_config(white_man)
        white_man = [8]
        self.insert_config(white_man)
        white_man = [9]
        self.insert_config(white_man)
        white_man = [10]
        self.insert_config(white_man)
        white_man = [11]
        self.insert_config(white_man)
        white_man = [12,7]
        self.insert_config(white_man)
        white_man = [13,8]
        self.insert_config(white_man)
        white_man = [14,9]
        self.insert_config(white_man)
        white_man = [17,11]
        self.insert_config(white_man)
        white_man = [18,12,7]
        self.insert_config(white_man)
        white_man = [19,13,8]
        self.insert_config(white_man)
        white_man = [20,14,9]
        self.insert_config(white_man)
        white_man = [21]
        self.insert_config(white_man)
        white_man = [22,17,11]
        self.insert_config(white_man)
        white_man = [23,18,12,7]
        self.insert_config(white_man)
        white_man = [24,19,13,8]
        self.insert_config(white_man)
        white_man = [27,21]
        self.insert_config(white_man)
        white_man = [28,22,17,11]
        self.insert_config(white_man)
        white_man = [29,23,18,12,7]
        self.insert_config(white_man)
        white_man = [30,24,19,13,8]
        self.insert_config(white_man)
        white_man = [31]
        self.insert_config(white_man)
        white_man = [32,27,21]
        self.insert_config(white_man)
        white_man = [33,28,22,17,11]
        self.insert_config(white_man)
        white_man = [34,29,23,18,12,7]
        self.insert_config(white_man)
        white_man = [37,31]
        self.insert_config(white_man)
        white_man = [38,32,27,21]
        self.insert_config(white_man)
        white_man = [39,33,28,22,17,11]
        self.insert_config(white_man)
        white_man = [40,34,29,23,18,12,7]
        self.insert_config(white_man)
        white_man = [41]
        self.insert_config(white_man)
        white_man = [42,37,31]
        self.insert_config(white_man)
        white_man = [43,38,32,27,21]
        self.insert_config(white_man)
        white_man = [44,39,33,28,22,17,11]
        self.insert_config(white_man)
        return
