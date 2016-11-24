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
    
    
    def get_stand_file(self):
        self.s             = ''                # the string with the input to analyse
        self.cp            = 0                 # current position pointer in this string
        self.fdlg = FileDialog.LoadFileDialog(self.root, title="Choose A Stand File")
        self.fname = self.fdlg.go(pattern="*.stand", dir_or_file='/home/feike/BoomstraDam/stand files')
        self.file_open_flag = True
        try:
            self.f = open(self.fname, 'r')
        except:
            self.file_open_flag = False        
        if self.file_open_flag == True:
            # reset the Games environment
            self.main.Games = []
            game = GameRecord(self.main, self.main.panel, self.main.panel)
            self.main.ActiveGameNumber = 0
            self.main.ActiveTag = ''
            # now enter a new game for each line in the stand file
            for line in self.f:
                WhiteRating = int(line[5:9])
                BlackRating = int(line[10:14])
                MoveNr = int(line[15:17])
                WhiteResult = line[18:19]
                BlackResult = line[20:21]
                Result = WhiteResult + '-' + BlackResult
                pos = line[22:73]
                self.enter_pos(game,  pos, WhiteRating, BlackRating, MoveNr, Result)
                # is entered, make room for a new position
                game = GameRecord(self.main, self.main.panel, self.main.panel)
            self.main.ActiveGameNumber = 0
            self.main.ActiveGame = self.main.Games[0]
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
    
