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

class hek():
  def __init__(self):
   self.WhiteMan = []
   self.BlackMan = []
   self.WhiteKing = []
   self.BlackKing = []
   self.c = []
   self.c.append(set((46, 41, 37, 32, 28, 23, 19, 14, 10, 5)))
   self.c.append(set((47, 42, 38, 33, 29, 24, 20, 15)))
   self.c.append(set((48, 43, 39, 34, 30, 25)))
   self.c.append(set((49, 44, 40, 35, 50, 45)))
   self.hist_2 = []
   self.hist_1 = []
   self.hist_0 = []
   for i in range(0, 51):
     self.hist_2.append(0)
     self.hist_1.append(0)
     self.hist_0.append(0)
   return

  def set_50_square_position(self, stand):
    self.WhiteMan = []
    self.BlackMan = []
    self.WhiteKing = []
    self.BlackKing = []
    self.Empty = []
    if stand[0] == "W": self.ColorOnMove = dc.WHITE_ON_MOVE
    else: self.ColorOnMove = dc.BLACK_ON_MOVE
    for i in range(1,51):
      if stand[i] == "w":
        self.WhiteMan .append(i)
      elif stand[i] == "W":
        self.WhiteKing.append(i)
      elif stand[i] == "z":
        self.BlackMan .append(i)
      elif stand[i] == "Z":
        self.BlackKing.append(i)
      elif stand[i] == "e":
        self.Empty.append(i)
    return

  def count_white(self):
    c0 = len(set(self.WhiteMan) & self.c[0])
    c1 = len(set(self.WhiteMan) & self.c[1])
    c2 = len(set(self.WhiteMan) & self.c[2])
    c3 = len(set(self.WhiteMan) & self.c[3])
    return c0*4 + c1*3 + c2*2 + c3


  def read_infile(self):
    infile_name = '/home/feike/dammen/alle_quiescence_standen.doc'
    infile = open(infile_name, 'r')
    self.previous = -1
    #for i in range(0, 1000):
      #line = infile.readline()
    for line in infile:
      if line[55] == 'N'and line[47:49] == line[51:53]:
        stand = line[56:107]
        self.set_50_square_position(stand)
        GameNr = int(line[0:6])
        if (set([36, 31, 26, 27]) & set(self.WhiteMan) == set([36, 31, 26, 27])) and (set([16, 22, 18]) & set(self.BlackMan) ==  set([16, 22, 18])) and ((21) in self.Empty) and (self.previous != GameNr) and (len(set([46, 41, 37, 32]) & set(self.WhiteMan)) + 4 < len(set([1, 7, 12, 6, 11, 17]) & set(self.BlackMan)) + 3):
#          WhiteRating = int(line[33: 37])
#          BlackRating = int(line[37: 41])
          WhiteResult = line[31: 32]
#          BlackResult = line[32: 33]
#          MoveNr = int(line[42: 45])
#          print str(GameNr).zfill(4) + ' ' + str(WhiteRating).zfill(4) + ' ' + str(BlackRating).zfill(4),
#          print str(MoveNr).zfill(2) + ' ' + str(WhiteResult).zfill(1) + ' ' + str(BlackResult).zfill(1),
#          print stand

          if WhiteResult == "2":
            self.hist_2[self.count_white()] += 1
            self.hist_2[50] += 1
          elif WhiteResult == "1":
            self.hist_1[self.count_white()] += 1
            self.hist_1[50] += 1
          else:
            self.hist_0[self.count_white()] += 1
            self.hist_0[50] += 1


          self.previous = GameNr
    for i in range(5, 50):
      print float(self.hist_0[i])/float(self.hist_0[50]), '\t', float(self.hist_1[i])/float(self.hist_1[50]), '\t', float(self.hist_2[i])/float(self.hist_2[50])
    infile.close()
    return

if __name__=='__main__':
    main = hek()
    main.read_infile()
