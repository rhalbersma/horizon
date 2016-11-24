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


    def file_save_game_clicked(self):
        self.fdlg = FileDialog.SaveFileDialog(self.root, title="Save A PDN File")
        if self.file_name == "":
            self.file_name = "test.pdn"            
        self.file_name = self.fdlg.go(dir_or_file='/home/feike/BoomstraDam/pdn files', \
                                      default = self.file_name, key = None) 
        self.file_open_flag = True
        try:
            self.f=open(self.file_name, 'w')
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
        
        s = s + self.get_moves_pdn_string(game)
        
        if game.Result <> '':
            s = s + game.Result
        else:
            s = s + '0-0'        
            
        return s
    
    def get_moves_pdn_string(self, game, format = True):
        k = 0
        s = ''
        
        for j in range(0,len(game.Moves)):
            s = s + repr(game.Moves[j].Ply_nr).zfill(2) + "."
            if game.Moves[j].WhiteMove <> '':
                if (len(game.Moves[j].WhiteMove.Step) == 2) or (game.Moves[j].WhiteMove.Unique == True):
                    s = s + repr(game.Moves[j].WhiteMove.Start).zfill(2)
                    s = s + game.Moves[j].WhiteMove.op
                    s = s + repr(game.Moves[j].WhiteMove.Stop).zfill(2) + " "
                else:
                    for step in game.Moves[j].WhiteMove.Step:
                        s = s + repr(step).zfill(2) + 'x'
                    s = s[0:-1]
                    s = s + ' ' # replace the last 'x'
                #if game.Moves[j].WhiteMove.Variant <> '':
                #   s = s + "(" + game.Moves[j].WhiteMove.Variant.get_moves_pdn_string(False) + ") "
                if game.Moves[j].WhiteMove.Comment <> '':
                    s = s + '{' + game.Moves[j].WhiteMove.Comment + '} '
            if game.Moves[j].BlackMove <> '':
                if (len(game.Moves[j].BlackMove.Step) == 2) or (game.Moves[j].BlackMove.Unique == True):
                    s = s + repr(game.Moves[j].BlackMove.Start).zfill(2)
                    s = s + game.Moves[j].BlackMove.op
                    s = s + repr(game.Moves[j].BlackMove.Stop).zfill(2) + " "
                else:
                    for step in game.Moves[j].BlackMove.Step:
                        s = s + repr(step).zfill(2) + 'x'
                    s = s[0:-1]
                    s = s + ' ' # replace the last 'x'
                #if game.Moves[j].BlackMove.Variant <> '':
                #    s = s + "(" + game.Moves[j].BlackMove.Variant.get_moves_pdn_string(False) + ") "
                if game.Moves[j].BlackMove.Comment <> '':
                    s = s + '{' + game.Moves[j].BlackMove.Comment + '} '
        
        if format == False: return s
        
        # insert a \n before 80th character
        
        t = ''

        while k+80 < len(s):
            n = 2
            while not game.is_ply_nr(s, k + 80 - n):
                n = n + 1
            t = t + s[k:k+80-n] + "\n"
            k = k + 80 - n
            
        t = t + s[k:len(s)] + "\n"

        return t
    
    
