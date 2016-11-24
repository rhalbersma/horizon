#
#  This file is Copyright (C) 2010 Feike Boomstra.
#  Distributed under the Boost Software License, Version 1.0.
#  (See accompanying file LICENSE_1_0.txt or copy at
#  http://www.boost.org/LICENSE_1_0.txt)
#


import sys
sys.path = sys.path + ['/home/feike/Horizon/py-files']
import gzip
import os
#import shutil
#from ftplib import FTP
#from random import *
from Tkinter import *
#from time import *

from DraughtConstants import *
from MoveGenerator import *
from DraughtDataStructures import *
from DraughtReadPDN import *

from string import *
#from DraughtDataStructures import *

class DraughtName:
  def __init__(self, name):
    self.name = name
    self.freq = 0
    return
  def __gt__(self, a):
    return self.freq > a.freq

class DraughtMakeDatabase:
    def __init__(self):
        self.root = Tk()
        self.read = ReadPDN(self, self.root)
        self.mg = MoveGenerator()
        self.rating_name_list = []
        self.rating_value_list = []
        self.NoRating = []
        return

    def insert_name(self, name):
      found = False
      for a in range(0, len(self.NoRating)):
        if self.NoRating[a].name == name:
          self.NoRating[a].freq += 1
          found = True
      if found == False:
        temp = DraughtName(name)
        self.NoRating.append(temp)
      return

    def get_rating_info(self):
        RatingFile = '/home/feike/workspace/collected/fmjd_rating_04_2009.txt'
        rating = open(RatingFile, 'r')
        while 1:
            try:
                rating_line = rating.readline()
            except:
                break
            try:
                rating_name = rating_line[0:rating_line.index('\t')]
            except:
                break
            rating_value = int(rating_line[rating_line.index('\t')+1:-1])
            self.rating_name_list.append(rating_name)
            self.rating_value_list.append(rating_value)
        rating.close()
        # got the rating info per player
        for i in range(0, 10):
          print self.rating_name_list[i], self.rating_value_list[i]
        print self.rating_name_list[-1], self.rating_value_list[-1]
        return

    def Make_alle_partijen_met_rating_gt_2100(self):
        self.get_rating_info()
        game_cnt = 0
        InputFile = '/home/feike/dammen/alle_partijen.doc'
        OutputFile = '/home/feike/dammen/alle_partijen_met_rating_gt_2100.doc'
        # file format:
        # line1: [0:25] FirstName, [27:53] SecName, [53] FirstResult, [55] SecondResult
        # line2..n [0] == ' '
        # line_last: empty
        file = open(InputFile, 'r')
        ofile = open(OutputFile, 'w')
        #while 1:
        eof = False
        for i in range(0,400000):
          game_body = ''
          while True:
            try:
                line = file.readline()
                if len(line) == 2:
                  break
                if line[0] == ' ':
                  game_body += line[0:-2]
                else:
                  if len(line) > 55:
                    if line[0:25].find(',') == -1:
                      WhiteName = line[0:line[0:25].index(' ')]
                    else:
                      WhiteName = line[0:line[0:25].index(',')]
                    if line[27:53].find(',') == -1:
                      BlackName = line[27:line[27:53].index(' ')]
                    else:
                      BlackName = line[27:27+line[27:53].index(',')]
                    WhiteResult = line[53]
                    BlackResult = line[55]
                  else:
                    #print "no result found"
                    WhiteResult = '?'
            except:
                print "readline exception"
                eof = True
                break #has to be end of file
          if eof == True: break
          if WhiteResult in ['0', '1', '2'] and BlackResult in ['0', '1', '2'] :
            #print WhiteName,  BlackName, WhiteResult, '-', BlackResult,  game_body
            FirstNameFound = True
            try:
                WhiteRating = self.rating_value_list[self.rating_name_list.index(WhiteName.lower())]
                #print WhiteName, "found" ,

            except:
                #print WhiteName, "not found" ,

                #self.insert_name(WhiteName)
                FirstNameFound = False

            SecNameFound = True
            try:
                BlackRating = self.rating_value_list[self.rating_name_list.index(BlackName.lower())]
                #print BlackName, "found"
            except:
                #self.insert_name(BlackName)
                #print BlackName, "not found"
                SecNameFound = 0

            if FirstNameFound and SecNameFound:
                #print WhiteName, '-', BlackName, '(', i, ')'
                s = str(WhiteName).ljust(12) + str(BlackName).ljust(12) + WhiteResult + BlackResult + str(WhiteRating).zfill(4) + str(BlackRating).zfill(4) + ' ' + game_body
                ofile.write(s + '\n')
                game_cnt += 1
                if game_cnt % 1000 == 0: print game_cnt
        file.close()
        ofile.close()

        print "nr of games",  game_cnt
        return

    def Make_alle_quiescence_standen(self):
        file_name = '/home/feike/dammen/alle_partijen_met_rating_gt_2100.doc'
        file = open(file_name, 'r')
        OutPartijFile = '/home/feike/dammen/alle_partij_info.doc'
        OutPosFile = '/home/feike/dammen/alle_quiescence_standen.doc'
        PartijFile = open(OutPartijFile, 'w')
        PosFile = open(OutPosFile, 'w')

        game_cnt = 0
        #for i in range(0, 10):
            #line = file.readline()
        for line in file:
            #line:
            # [0:12] WhiteName [12:24] BlackName [24] WhiteResult [25] BlackResult [26:30] WhiteRating, [30:34] BlackRating, [35:-1] game_body
            self.read.s = line[35:-1] + ' * \n\0'
            game = GameRecord(self, None, None)
            self.read.lastchrpnt = 0;
            cp = self.read.GetGame(0, game)
            if cp == 0:
                print "Error in PDN, last char tried = ", self.read.lastchrpnt
                print self.read.s[self.read.lastchrpnt-70:self.read.lastchrpnt]
##                    self.main.read.debug = '*'
##                    game = GameRecord(self.main, self.main.panel, self.main.panel)
##                    self.main.read.Games.append(game)
##                    cp = self.main.read.GetGame(0, game)
##                    self.main.read.debug=None
##                    break
            else:
              pass
              # we have got a valid pdn
              # first we check whether all moves are legal

              last_ply = game.get_half_ply_length()
              game.half_ply_pointer = last_ply
              if game.build_till_pointer() == True:
                #print last_ply
                game_cnt += 1
                #PartijFile.write(str(game_cnt).zfill(6) + ' ' + line[0:34] + '\n')
                game.set_begin_position()
                for i in range(0, game.get_half_ply_length()):
                  pos = game.get_next_board_layout()
                  if pos <> None: #valid move
                    PosFile.write(str(game_cnt).zfill(6) + ' ' + line[0:34] + ' ' + str(game.HalfPlyPointer).zfill(3) + self.count_items(pos) + pos + '\n')
                  else:
                    pass
                    #print game.HalfPlyPointer, "not quienscence"
        file.close()
        PartijFile.close()
        PosFile.close()
        print "done"
        return

    def count_items(self, pos):
      tot = 0
      WhiteMan = 0
      WhiteKing = 0
      BlackMan = 0
      BlackKing = 0
      AnyKing = "N"
      for ch in pos[1:]:
        if ch == 'W':
          WhiteKing += 1
          tot += 1
          AnyKing = "Y"
        elif ch == "Z":
          BlackKing += 1
          tot += 1
          AnyKing = "Y"
        elif ch == "w":
          WhiteMan += 1
          tot += 1
        elif ch == "z":
          BlackMan += 1
          tot += 1
        else:
          pass
      return str(tot).zfill(2)+str(WhiteMan).zfill(2)+str(WhiteKing).zfill(2)+str(BlackMan).zfill(2)+str(BlackKing).zfill(2)+AnyKing

if __name__=='__main__':
    main = DraughtMakeDatabase()
    #main.Make_alle_partijen_met_rating_gt_2100()
    main.Make_alle_quiescence_standen()

