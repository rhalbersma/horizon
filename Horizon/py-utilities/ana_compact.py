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

class compactness():
  def __init__(self):
   self.WhiteMan = []
   self.BlackMan = []
   self.WhiteKing = []
   self.BlackKing = []
   self.c = [set([]), \
             set([6,7]),		set([7,8]),			set([8,9]),			set([9,10]), set([]), \
		           set([ 1,11]),		set([ 1,11, 2,12]),	set([ 2,12, 3,13]),	set([ 3,13, 4,14]),	set([ 4,14, 5,15]),\
		           set([ 6,16, 7,17]),set([ 7,17, 8,18]),	set([ 8,18, 9,19]),	set([ 9,19,10,20]),	set([10,20]),\
		           set([11,21]),		set([11,21,12,22]),	set([12,22,13,23]),	set([13,23,14,24]),	set([14,24,15,25]),\
		           set([16,26,17,27]),set([17,27,18,28]),	set([18,28,19,29]),	set([19,29,20,30]),	set([20,30]),\
		           set([21,31]),		set([21,31,22,32]),	set([22,32,23,33]),	set([23,33,24,34]),	set([24,34,25,35]),\
		           set([26,36,27,37]),set([27,37,28,38]),	set([28,38,29,39]),	set([29,39,30,40]),	set([30,40]),\
		           set([31,41]),		set([31,41,32,42]),	set([32,42,33,43]),	set([33,43,34,44]),	set([34,44,35,45]),\
		           set([36,46,37,47]),set([37,47,38,48]),	set([38,48,39,49]),	set([39,49,40,50]),	set([40,50]),\
		           set([]),				set([41,42]),			set([42,43]),			set([43,44]),			set([44,45])
           ]
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

  def count_compact(self, stand, ww):
    self.set_50_square_position_from_LDF(stand)
    win_cnt = 0
    los_cnt = 0
    if ww == True:
      for i in range(1, 51):
        if i in self.WhiteMan:
          if len(self.c[i] & set(self.WhiteMan)) >= 2 :win_cnt += 1
      for i in range(1, 51):
        if i in self.BlackMan:
          if len(self.c[i] & set(self.BlackMan)) >= 2 :los_cnt += 1
    else:
      for i in range(1, 51):
        if i in self.WhiteMan:
          if len(self.c[i] & set(self.WhiteMan)) >= 2 :los_cnt += 1
      for i in range(1, 51):
        if i in self.BlackMan:
          if len(self.c[i] & set(self.BlackMan)) >= 2 :win_cnt += 1

    self.hist_2_0[win_cnt] += 1
#    self.hist_2_1[win_cnt[3]+win_cnt[4]+win_cnt[5]+win_cnt[6]] += 1
#    self.hist_2_2[win_cnt[7]+win_cnt[8]+win_cnt[9]] += 1
#
    self.hist_0_0[los_cnt] += 1
#    self.hist_0_1[los_cnt[3]+los_cnt[4]+los_cnt[5]+los_cnt[6]] += 1
#    self.hist_0_2[los_cnt[7]+los_cnt[8]+los_cnt[9]] += 1

#    self.hist_2_0[win_cnt[0]+win_cnt[1]+win_cnt[2]+win_cnt[3]+win_cnt[4]] += 1
#    #self.hist_2_1[win_cnt[3]+win_cnt[4]+win_cnt[5]+win_cnt[6]] += 1
#    self.hist_2_2[win_cnt[7]+win_cnt[8]+win_cnt[9]+win_cnt[5]+win_cnt[6]] += 1
#
#    self.hist_0_0[los_cnt[0]+los_cnt[1]+los_cnt[2]+los_cnt[3]+los_cnt[4]] += 1
#    #self.hist_0_1[los_cnt[3]+los_cnt[4]+los_cnt[5]+los_cnt[6]] += 1
#    self.hist_0_2[los_cnt[7]+los_cnt[8]+los_cnt[9]+los_cnt[5]+los_cnt[6]] += 1
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
        self.count_compact(stand, ww)
    for i in range(0, 19):
      print i, '\t', self.hist_0_0[i], '\t', self.hist_2_0[i]
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

      main = compactness()
      main.read_infile(str(target).zfill(2))
