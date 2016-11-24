#
#  This file is Copyright (C) 2010 Feike Boomstra.
#  Distributed under the Boost Software License, Version 1.0.
#  (See accompanying file LICENSE_1_0.txt or copy at
#  http://www.boost.org/LICENSE_1_0.txt)
#


import pickle
import sys
sys.path.append('/home/feike/Horizon/py-files')
from MoveGenerator import *
from DraughtDataStructures import *
from DraughtPrintBoard import *

class EnhanceOpening:
  def __init__(self):
    self.dist = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
    self.mg = MoveGenerator()
    self.board_print = DraughtPrintBoard(self)
    return

  def enhance(self):
    file = open('/home/feike/Horizon/Opening Books/PosDict.pickle', 'r')
    self.PosDict = pickle.load(file)
    file.close()
    self.kill = []
    print len(self.PosDict)
    for key in self.PosDict.keys():
      if len(self.PosDict[key]) == 1:
        res, ml = self.is_not_a_single_move(key)
        if res == True: # ???
          for moves in ml:
            print moves
          print self.PosDict[key]
          return

#      if self.is_not_a_single_move(key) == True:
#        self.dist[self.get_index(self.get_max(self.PosDict[key]))] += 1
#    print self.dist
#      if self.get_max(self.PosDict[key]) < 4 or len(self.PosDict[key]) == 1:
#        self.kill.append(key)
#    for key in self.kill:
#      del(self.PosDict[key])
#    print len(self.PosDict)
#    file = open('/home/feike/Horizon/Opening Books/ReducedPosDict.pickle', 'w')
#    pickle.dump(self.PosDict, file)
#    file.close()
    return

  def get_max(self, dic):
    max = 0
    sum = 0
    for key in dic:
      if dic[key] > max:
        max = dic[key]
      sum += dic[key]
    return max, sum

  def get_index(self, num):
    if num <= 10:
      return num
    elif num <=25:
      return 11
    elif num <= 50:
      return 12
    elif num <= 100:
      return 13
    elif num <= 250:
      return 14
    elif num <= 500:
      return 15
    else:
      return 16

  def is_not_a_single_move(self, pos):
    game = GameRecord(self, None, None)
    game.set_50_square_position(pos)
    ml = self.mg.valid_move(game)
    return len(ml) > 1, ml

  def inspect(self):
    file = open('/home/feike/Horizon/Opening Books/ReducedPosDict.pickle', 'r')
    self.PosDict = pickle.load(file)
    file.close()
    all = []
    count_move = 0
    for key in self.PosDict.keys():
      ml = []
      mt = []
      max_val, tot_sum = self.get_max(self.PosDict[key])
      for mkey in self.PosDict[key]:
        if self.allow(self.PosDict[key][mkey], max_val, tot_sum) == True and max_val >= 7:
	  count_move += 1
	  print self.PosDict[key][mkey], max_val,tot_sum
          #ml.append(self.PosDict[key][mkey])
          ml.append(mkey)
        mt.append(self.PosDict[key][mkey])
      if ml != []:
        all.append((key, ml))
    all_dict = dict(all)
    print "pos = ", len(all_dict.keys()), "moves = ", count_move 
    file = open('/home/feike/Horizon/Opening Books/OpeningBook_test.pickle', 'w')
    pickle.dump(all_dict, file)
    file.close()
    return

  def allow(self, value, max_val, tot_sum):
    if max_val > 10000:
      return value > int(0.1*float(max_val)+0.5)
    elif max_val > 5000:
      return value > int(0.2*float(max_val)+0.5)
    elif max_val > 2500:
      return value > int(0.3*float(max_val)+0.5)
    elif max_val > 1000:
      return value > int(0.4*float(max_val)+0.5)
    elif max_val >500:
      return value > int(0.45*float(max_val)+0.5)
    elif max_val >250:
      return value > int(0.50*float(max_val)+0.5)
    elif max_val >100:
      return value > int(0.55*float(max_val)+0.5)
    elif max_val >50:
      return value > int(0.60*float(max_val)+0.5)
    elif max_val >25:
      return value > int(0.65*float(max_val)+0.5)
    else:
      return value > int(0.7*float(max_val)+0.5)

  def test_speed(self):
    file = open('/home/feike/Horizon/Opening Books/OpeningBook.pickle', 'r')
    self.all_dict = pickle.load(file)
    file.close()
    print "start"
    print self.all_dict['Wzzzzzzzzzzzzzzzzzzzzeeeeeeeeeewwwwwwwwwwwwwwwwwwww']
    return





if __name__=='__main__':
  enh = EnhanceOpening()
  enh.inspect()
#  enh.enhance()
#  print sys.path
#  enh.test_speed()
