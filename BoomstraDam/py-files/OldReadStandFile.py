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
            self.StandFileName = self.fname
            self.f.close()
            self.display_stand_line(0)
        return
        
    def display_stand_line(self, line_nr):
        Positions = open(self.StandFileName, 'r')
        cnt = 0
        for line in Positions:
            if cnt == line_nr: break
            cnt += 1
        # we are at line_nr in the file
        #print line,
        WhiteRating = int(line[5:9])
        BlackRating = int(line[10:14])
        MoveNr = int(line[15:17])
        WhiteResult = int(line[18:19])
        BlackResult = int(line[20:21])
        ColorOnMove = line[22:23]
        pos = line[22:73]
##        self.main.panel.notation.delete(1.0, END)
##        disp = 'result ' + line[18:21] + '\n'
##        disp = disp + 'tempi ' + str(self.tempi_white(pos)) + ' ' + str(self.tempi_black(pos)) + '\n'
##        disp = disp + 'distr ' + str(self.calc_spreiding_white(pos)) + ' ' + str(self.calc_spreiding_black(pos)) + '\n'
##        disp = disp + 'centr ' + str(self.centr_white(pos)) + ' ' + str(self.centr_black(pos))
##        self.main.panel.notation.insert(END, disp)
        self.display_pos(pos, WhiteRating, BlackRating, MoveNr, line[18:21])
        Positions.close()
        return
        
    def old_display_pos(self, pos):
        white_man = []
        black_man = []
        white_king = []
        black_king = []
        empty = []
        for i in range(1,51):
            if pos[i] == 'w':
                white_man.append(i)
            elif pos[i] == 'z':
                black_man.append(i)
            elif pos[i] == 'e':
                empty.append(i)
            elif pos[i] == 'W':
                white_king.append(i)
            elif pos[i] == 'Z':
                black_king.append(i)
            
        for piece in white_man:           self.main.panel.set_white_man   (piece, False)
        for piece in black_man:           self.main.panel.set_black_man   (piece, False)
        for piece in white_king:           self.main.panel.set_white_king   (piece, False)
        for piece in black_king:           self.main.panel.set_black_king   (piece, False)
        for piece in empty:                 self.main.panel.set_empty_field  (piece, False)
        return

    def display_pos(self, pos, WhiteRating, BlackRating, MoveNr, Result):
        self.main.Games = []
        game = GameRecord(self.main, self.main.panel, self.main.panel)
        self.main.Games.append(game)
        self.main.ActiveGame = self.main.Games[0]
        self.main.ActiveGame.WhitePlayer = str(WhiteRating)
        self.main.ActiveGame.BlackPlayer = str(BlackRating)
        self.main.ActiveGame.Event = ''                                     # PDN element
        self.main.ActiveGame.Date = ''                                      # PDN element
        self.main.ActiveGame.Result = Result
        self.main.ActiveGame.Round = str(MoveNr)
        
        self.main.Games[0].BeginPosition.WhiteMan = []
        self.main.Games[0].BeginPosition.BlackMan = []
        self.main.Games[0].BeginPosition.WhiteKing = []
        self.main.Games[0].BeginPosition.BlackKing = []
        self.main.Games[0].BeginPosition.Selected = []
        if pos[0] == 'W':
            self.main.Games[0].BeginPosition.ColorOnMove = dc.WHITE_ON_MOVE
            self.main.Games[0].CurrentPosition.ColorOnMove = dc.WHITE_ON_MOVE
        else:
            self.main.Games[0].BeginPosition.ColorOnMove = dc.BLACK_ON_MOVE
            self.main.Games[0].CurrentPosition.ColorOnMove = dc.BLACK_ON_MOVE
        for i in range(1,51):
            if pos[i] == 'w':
                self.main.Games[0].BeginPosition.WhiteMan.append(i)
                self.main.Games[0].CurrentPosition.WhiteMan.append(i)
            elif pos[i] == 'z':
                self.main.Games[0].BeginPosition.BlackMan.append(i)
                self.main.Games[0].CurrentPosition.BlackMan.append(i)
            elif pos[i] == 'W':
                self.main.Games[0].BeginPosition.WhiteKing.append(i)
                self.main.Games[0].CurrentPosition.WhiteKing.append(i)
            elif pos[i] == 'Z':
                self.main.Games[0].BeginPosition.BlackKing.append(i)
                self.main.Games[0].CurrentPosition.BlackKing.append(i)
        self.main.ActiveGame.refresh_display()
        self.anal_stand()
        return
    
    def anal_stand(self, display=False):
        self.main.anal.eval_mode_clicked(self.main.ActiveGame, display)
        self.main.play.startup_engine()
        self.main.play.engine.send_evaluate_position(self.main.ActiveGame)
##        self.PrintFlagSet = display
        self.main.play.ProcesFixedDepth = self.fixed_depth_ready        
##        if self.main.ActiveGame.is_color_white() == True:
##            orig_color = 'W'
##        else:
##            orig_color = 'Z'
##        self.main.play.engine.send_get_fixed_depth_move( self.main.ActiveGame, 6, 0, orig_color, display, True)
        return

    def fixed_depth_ready(self,s) :
        if self.PrintFlagSet == True: return
        game = self.main.ActiveGame
        print s
        start = int(s[1:3])
        stop = int(s[3:5])
        result = int(s[5:11])
        nr_of_caps = int(s[11:13])
        print "python nr of caps: ", nr_of_caps
        caps = []
        k = 13
        for i in range(0, nr_of_caps):
            k = 13 + 2*(i)
            cap = int(s[k:k+2])
            caps.append(cap)
        self.enter_move(game, start, stop, caps, nr_of_caps)
##        nr_of_moves = int(s[k:k+2])
##        k = k + 2;
##        for i in range(1, nr_of_moves+1):
##            k=k+7
##            start = int(s[k:k+2])
##            stop = int(s[k+3:k+5])
##            self.enter_move(game, start, stop, [], 0)
##        game.refresh_display()
##        self.main.re_play.click('000_000')
            
        print "was ", self.main.LastEval, "after = " , result, " diff = ", self.main.LastEval - result
        if abs(self.main.LastEval - result) > 500:
            self.anal_stand(True) # display the analysis
        return
    
    def enter_move(self, game, start, stop, caps, nr_of_caps):
        move, valid, unique = self.main.play.get_valid_move( game, start, stop, caps, nr_of_caps)
        if valid == True:
            move.Unique = unique
            game.do_move(move, True)
            game.write_moves()
            if self.main.ActiveGame.CurrentPosition.ColorOnMove == dc.WHITE_ON_MOVE:
                self.main.panel.current_color_on_move.set(dc.WHITE_ON_MOVE)
                self.main.panel.status_display.set("White on Move!")
            else:
                self.main.panel.current_color_on_move.set(dc.BLACK_ON_MOVE)
                self.main.panel.status_display.set("Black on Move!")
        return

    def next_line(self, widget):
        self.cp += 1
        self.display_stand_line(self.cp)
        return
    
    def previous_line(self, widget):
        self.cp -= 1
        if self.cp < 0: self.cp = 0
        self.display_stand_line(self.cp)
        return

    def distance_centre(self, z):
        if z in [3,8,13,18,23,28,33,38,43,48]: return 0
        if z in [2,12,22,32,42,9,19,29,39,49]: return 1
        if z in [7,17,27,37,47,4,14,24,34,44]: return 2
        if z in [1,11,21,31,41,10,20,30,40,50]: return 3
        if z in [6,16,26,36,46,5,15,25,35,45]: return 4
    
    def centr_white(self, pos):
        centre = 0
        for i in range(1,51):
            if pos[i]=='w':
                ds = self.distance_centre(i)
                if ds == 0:
                    centre +=4
                elif ds == 1:
                    centre +=2
                elif ds == 2:
                    centre +=1
            
        return centre

    def centr_black(self, pos):
        centre= 0
        for i in range(1,51):
            if pos[i]=='z':
                ds = self.distance_centre(i)
                if ds == 0:
                    centre +=4
                elif ds == 1:
                    centre +=2
                elif ds == 2:
                    centre +=1
        return centre

    def column(self, z):
        if z in [6,16,26,36,46]: return 1
        if z in [1,11,21,31,41]: return 2
        if z in [7,17,27,37,47]: return 3
        if z in [2,12,22,32,42]: return 4
        if z in [8,18,28,38,48]: return 5
        if z in [3,13,23,33,43]: return 6
        if z in [9,19,29,39,49]: return 7
        if z in [4,14,24,34,44]: return 8
        if z in [10,20,30,40,50]: return 9
        if z in [5,15,25,35,45]: return 10
        
    def spreiding(self, col,w,nr_pieces):
##        avg = 0
##        for z in range(1,12-w):
##            for r in range(z,z+w):
##                avg += 10*col[r]
##        avg = avg/(11-w)    
        avg = nr_pieces*w
        spr = 0
        for z in range(1,12-w):
            cur = 0;
            for r in range(z,z+w):                
                cur += 10*col[r]
            if abs(cur-avg) <= 19: cur = avg
            print cur, ' ',
            spr += pow((cur - avg),2)
        print spr, ' ',
        spr = int(-10*pow(spr/((11-w)*(10-w)), 0.5))
        print spr
        return spr
    def CountPieces(self, line, kind):
        pc = 0
        for i in range(1,51):
            if line[i] == kind: pc+=1
        return pc
        
    def calc_spreiding_white(self, pos):
        col = [0,0,0,0,0,0,0,0,0,0,0,0,0]    
        for i in range (1,51):
            if pos[i]=='w':
                col[self.column(i)] +=1
        return self.spreiding(col, 4, self.CountPieces(pos,'w'))

    def calc_spreiding_black(self, pos):
        col = [0,0,0,0,0,0,0,0,0,0,0,0,0]    
        for i in range (1,51):
            if pos[i]=='z':
                col[self.column(i)] +=1
        return self.spreiding(col, 4, self.CountPieces(pos,'z'))

    def tempo_black(self, x):
        if x in range(1,6): return 1
        if x in range(6,11): return 2
        if x in range(11,16): return 3
        if x in range(16,21): return 4
        if x in range(21, 26): return 5
        if x in range(26, 31): return 6
        if x in range(31, 36): return 7
        if x in range(36, 41): return 8
        if x in range(41, 46): return 9
        return 10

    def tempo_white(self, x):
        if x in range(1,6): return 10
        if x in range(6,11): return 9
        if x in range(11,16): return 8
        if x in range(16,21): return 7
        if x in range(21, 26): return 6
        if x in range(26, 31): return 5
        if x in range(31, 36): return 4
        if x in range(36, 41): return 3
        if x in range(41, 46): return 2
        return 1
        
    def tempi_white(self, pos):
        temp = 0
        for i in range(1,51):
            if pos[i] == 'w':
                temp = temp + self.tempo_white(i)
        return temp

    def tempi_black(self, pos):
        temp = 0
        for i in range(1,51):
            if pos[i] == 'z':
                temp = temp + self.tempo_black(i)
        return temp

