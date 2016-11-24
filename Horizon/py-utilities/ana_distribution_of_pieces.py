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

class distribution_of_pieces():
  def __init__(self):
   self.WhiteMan = []
   self.BlackMan = []
   self.WhiteKing = []
   self.BlackKing = []
   self.c = []
   self.c.append(set((6, 16, 26, 36, 46)))
   self.c.append(set((1, 11, 21, 31, 41)))
   self.c.append(set((7, 17, 27, 37, 47)))
   self.c.append(set((2, 12, 22, 32, 42)))
   self.c.append(set((8, 18, 28, 38, 48)))
   self.c.append(set((3, 13, 23, 33, 43)))
   self.c.append(set((9, 19, 29, 39, 49)))
   self.c.append(set((4, 14, 24, 34, 44)))
   self.c.append(set((10, 20, 30, 40, 50)))
   self.c.append(set((5, 15, 25, 35, 45)))
   self.hist_2_0 = []
   self.hist_2_1 = []
   self.hist_2_2 = []
   self.hist_0_0 = []
   self.hist_0_1 = []
   self.hist_0_2 = []
   for i in range(0, 51):
     self.hist_2_0.append(0)
     self.hist_2_1.append(0)
     self.hist_2_2.append(0)
     self.hist_0_0.append(0)
     self.hist_0_1.append(0)
     self.hist_0_2.append(0)

   return

  def set_50_square_position_from_LDF(self, stand):
    self.WhiteMan = []
    self.BlackMan = []
    self.WhiteKing = []
    self.BlackKing = []
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
    return

  def count_collumns(self, stand, ww):
    self.set_50_square_position_from_LDF(stand)
    win_cnt = []
    los_cnt = []
    if ww == True:
      win_cnt.append(len(set(self.WhiteMan) & self.c[0]))
      win_cnt.append(len(set(self.WhiteMan) & self.c[1]))
      win_cnt.append(len(set(self.WhiteMan) & self.c[2]))
      win_cnt.append(len(set(self.WhiteMan) & self.c[3]))
      win_cnt.append(len(set(self.WhiteMan) & self.c[4]))
      win_cnt.append(len(set(self.WhiteMan) & self.c[5]))
      win_cnt.append(len(set(self.WhiteMan) & self.c[6]))
      win_cnt.append(len(set(self.WhiteMan) & self.c[7]))
      win_cnt.append(len(set(self.WhiteMan) & self.c[8]))
      win_cnt.append(len(set(self.WhiteMan) & self.c[9]))

      los_cnt.append(len(set(self.BlackMan) & self.c[9]))
      los_cnt.append(len(set(self.BlackMan) & self.c[8]))
      los_cnt.append(len(set(self.BlackMan) & self.c[7]))
      los_cnt.append(len(set(self.BlackMan) & self.c[6]))
      los_cnt.append(len(set(self.BlackMan) & self.c[5]))
      los_cnt.append(len(set(self.BlackMan) & self.c[4]))
      los_cnt.append(len(set(self.BlackMan) & self.c[3]))
      los_cnt.append(len(set(self.BlackMan) & self.c[2]))
      los_cnt.append(len(set(self.BlackMan) & self.c[1]))
      los_cnt.append(len(set(self.BlackMan) & self.c[0]))
    else:
      los_cnt.append(len(set(self.WhiteMan) & self.c[0]))
      los_cnt.append(len(set(self.WhiteMan) & self.c[1]))
      los_cnt.append(len(set(self.WhiteMan) & self.c[2]))
      los_cnt.append(len(set(self.WhiteMan) & self.c[3]))
      los_cnt.append(len(set(self.WhiteMan) & self.c[4]))
      los_cnt.append(len(set(self.WhiteMan) & self.c[5]))
      los_cnt.append(len(set(self.WhiteMan) & self.c[6]))
      los_cnt.append(len(set(self.WhiteMan) & self.c[7]))
      los_cnt.append(len(set(self.WhiteMan) & self.c[8]))
      los_cnt.append(len(set(self.WhiteMan) & self.c[9]))

      win_cnt.append(len(set(self.BlackMan) & self.c[9]))
      win_cnt.append(len(set(self.BlackMan) & self.c[8]))
      win_cnt.append(len(set(self.BlackMan) & self.c[7]))
      win_cnt.append(len(set(self.BlackMan) & self.c[6]))
      win_cnt.append(len(set(self.BlackMan) & self.c[5]))
      win_cnt.append(len(set(self.BlackMan) & self.c[4]))
      win_cnt.append(len(set(self.BlackMan) & self.c[3]))
      win_cnt.append(len(set(self.BlackMan) & self.c[2]))
      win_cnt.append(len(set(self.BlackMan) & self.c[1]))
      win_cnt.append(len(set(self.BlackMan) & self.c[0]))

#    self.hist_2_0[win_cnt[0]+win_cnt[1]+win_cnt[2]] += 1
#    self.hist_2_1[win_cnt[3]+win_cnt[4]+win_cnt[5]+win_cnt[6]] += 1
#    self.hist_2_2[win_cnt[7]+win_cnt[8]+win_cnt[9]] += 1
#
#    self.hist_0_0[los_cnt[0]+los_cnt[1]+los_cnt[2]] += 1
#    self.hist_0_1[los_cnt[3]+los_cnt[4]+los_cnt[5]+los_cnt[6]] += 1
#    self.hist_0_2[los_cnt[7]+los_cnt[8]+los_cnt[9]] += 1

    self.hist_2_0[win_cnt[0]+win_cnt[1]+win_cnt[2]+win_cnt[3]+win_cnt[4]] += 1
    #self.hist_2_1[win_cnt[3]+win_cnt[4]+win_cnt[5]+win_cnt[6]] += 1
    self.hist_2_2[win_cnt[7]+win_cnt[8]+win_cnt[9]+win_cnt[5]+win_cnt[6]] += 1

    self.hist_0_0[los_cnt[0]+los_cnt[1]+los_cnt[2]+los_cnt[3]+los_cnt[4]] += 1
    #self.hist_0_1[los_cnt[3]+los_cnt[4]+los_cnt[5]+los_cnt[6]] += 1
    self.hist_0_2[los_cnt[7]+los_cnt[8]+los_cnt[9]+los_cnt[5]+los_cnt[6]] += 1
    return


  def read_infile(self, target):
    infile_name = '/home/feike/dammen/alle_quiescence_standen.doc'
    infile = open(infile_name, 'r')
    #for i in range(0, 10):
    #  line = infile.readline()
    for line in infile:
      if line[47:49] == target:
        stand = line[56:107]
        if line[31] == '2': ww = True
        else: ww = False
        self.count_collumns(stand, ww)
    for i in range(0, 16):
      print i, '\t', self.hist_0_0[i], '\t', self.hist_0_2[i], '\t',
      print          self.hist_2_0[i], '\t', self.hist_2_2[i]
#    for i in range(0, 16):
#      print i, '\t', self.hist_0_0[i], '\t', self.hist_0_1[i], '\t', self.hist_0_2[i], '\t',
#      print          self.hist_2_0[i], '\t', self.hist_2_1[i], '\t', self.hist_2_2[i]
    infile.close()
    return

if __name__=='__main__':
    for target in range(18, 6, -1):

      print
      print target
      print

      main = distribution_of_pieces()
      main.read_infile(str(target).zfill(2))
