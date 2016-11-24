#
#  This file is Copyright (C) 2010 Feike Boomstra.
#  Distributed under the Boost Software License, Version 1.0.
#  (See accompanying file LICENSE_1_0.txt or copy at
#  http://www.boost.org/LICENSE_1_0.txt)
#


from math import *
from DraughtConstants import *
dc =DraughtConstants()         # get the global constants

class TDlearning():
  def __init__(self,  main):
    self.VW = []  # the weights for white
    self.VB = []  # the weights for black
    self.XW = []  # the feature values for white
    self.XB = []  # the feature values for black
    self.white_win = 0
    self.black_win = 0
    self.remise = 0
    self.rho = 0.00026
    self.td_out_version = 0
    self.wweight_version = 0
    self.bweight_version = 0
    self.main = main
    return
    
  
  def read_weight_file(self):
    for i in range(0, 1000):
      file_name = '/home/feike/BoomstraDam/TDlearning/wweights.' + str(i).zfill(3) +'.txt'
      try:
        weight_file = open(file_name,  'r')
      except:
        break
    file_name = '/home/feike/BoomstraDam/TDlearning/wweights.' + str(i-1).zfill(3) +'.txt' # last valid
    self.wweight_version = i-1
    self.VW =[]      #reset the weight structures
    self.VW.append([])
    weight_file = open(file_name,  'r') 
    count = 0
    while True:
      line = weight_file.readline()
      if line ==''  or line == '\n': break
      if count == 0:
        print "read weight file: ",  str(self.wweight_version),  'last updated from td_out: ', line        
      else:
        self.VW[-1].append( float(line) )
      count +=1
    weight_file.close()

# and now for black
    for i in range(0, 1000):
      file_name = '/home/feike/BoomstraDam/TDlearning/bweights.' + str(i).zfill(3) +'.txt'
      try:
        weight_file = open(file_name,  'r')
      except:
        break
    file_name = '/home/feike/BoomstraDam/TDlearning/bweights.' + str(i-1).zfill(3) +'.txt' # last valid
    self.bweight_version = i-1
    self.VB =[]      #reset the weight structures
    self.VB.append([])
    weight_file = open(file_name,  'r') 
    count = 0
    while True:
      line = weight_file.readline()
      if line ==''  or line == '\n': break
      if count == 0:
        print "read weight file: ",  str(self.bweight_version),  'last updated from td_out: ', line        
      else:
        self.VB[-1].append( float(line) )
      count +=1
    weight_file.close()
    #print self.VB
    return
    
  def write_new_weight_file(self):
    file_name = '/home/feike/BoomstraDam/TDlearning/wweights.' + str(self.wweight_version + 1).zfill(3) +'.txt' # last valid
    weight_file = open(file_name,  'w')
    weight_file.write(str(self.td_out_version).zfill(3) + '\n')
    for k in range(0, len(self.VW[0])):
      weight_file.write( ('%13.11f' % self.VW[-1][k]) + '\n')
    weight_file.close()
    file_name = '/home/feike/BoomstraDam/TDlearning/bweights.' + str(self.bweight_version + 1).zfill(3) +'.txt' # last valid
    weight_file = open(file_name,  'w')
    weight_file.write(str(self.td_out_version).zfill(3) + '\n')
    for k in range(0, len(self.VB[-1])):
      weight_file.write( ('%13.11f' % self.VB[-1][k]) + '\n')
    weight_file.close()
    return
    
    

  def read_td_out_file(self):
    for i in range(0, 1000):
      file_name = '/home/feike/BoomstraDam/TDlearning/td_out.' + str(i).zfill(3) +'.txt'
      try:
        weight_file = open(file_name,  'r')
      except:
        break
    file_name = '/home/feike/BoomstraDam/TDlearning/td_out.' + str(i-1).zfill(3) +'.txt' # last valid
    self.td_out_version = i - 1
    td_file = open(file_name,  'r')
    print "read: " ,  file_name
    count = 0
    x_index = 0
    self.XW = []       # reset
    self.XB = []       # reset
    while True:
      line = td_file.readline()
      if line ==''  or line == '\n': break
      if line[0:5] == 'white':
        X = self.XW
        mult = 1
      elif line[0:5] == 'black':
        X = self.XB
        mult = -1
      else:
        print "problem with td file"
      X.append([])  # create new column
      for k in range(0, 64):
        line = td_file.readline()
        X[-1].append( mult*int(line) )

      # move is read, do some administration
      if X[-1][32] <> 0:
        X[-1][0] = X[-1][32] #endgame database result always in index 0
        X[-1][32] = 0
        if X[-1][0] < -25000 or X[-1][0] > 25000: # database win or loss
          for z in range(1, len(X[-1])):
            X[-1][z] = 0
    td_file.close()
    return

  def P(self, t, tt, X, V):
    if X[tt][0] == 0:   #database result in index 0
      p = 0
      for k in range(1, len(V[0])):
        p += V[t][k] * X[tt][k]
      return tanh(self.rho*p)
    else:
      return tanh(self.rho*X[tt][0])
  
  def next_vi(self,  vi,  alpha,  next_y,  gamma,  Ptplus1,  Pt,  xi):    
    return vi + alpha*(next_y + gamma*Ptplus1 -Pt) * xi
    
  def part_der(self, i, t,  X, V):
    return (self.rho*X[t][i] / ((cosh(self.rho*self.P(t, t, X, V)))**2))
    
  def next_yt(self,  i,  X, V):
      return self.P(0, i+1,  X, V)

  def proces_new_td_out(self):
    alpha = 0.1
    gamma = 0.9
    self.read_weight_file()
    self.read_td_out_file()
    #for [X, V,  color] in [[self.XW, self.VW, 'white'], [self.XB, self.VB, 'black']]:
    for [X, V,  color] in [[self.XB, self.VB, 'black']]:
      for k in range(0, len(X)-1):
        V.append([])
        for n in range(0, len(X[0])):
          V[-1].append(0)
          if n <> 0 and n<> 1:
            #if n == 2: print "% 5.3f" % V[k][n], X[k][n], "% 5.3f" % self.next_yt(k, X, V), "% 5.3f" % self.P(k, k+1, X, V), "% 5.3f" % self.P(k, k, X, V), "% 5.3f" % self.part_der(n, k, X, V)
            V[k+1][n] = self.next_vi(V[k][n], alpha, self.next_yt(k, X, V), gamma, self.P(k, k+1, X, V), self.P(k, k, X, V), self.part_der(n, k, X, V))
          else:
            #print k,  n,  V[k][n]
            V[k+1][n] = V[k][n]
    self.write_new_weight_file()
    self.obtain_result()
    self.continue_learning()
    return
  
  def we_are_up_to_date(self):
    # get last weight file
    for i in range(0, 1000):
      file_name = '/home/feike/BoomstraDam/TDlearning/wweights.' + str(i).zfill(3) +'.txt'
      try:
        weight_file = open(file_name,  'r')
      except:
        break
    file_name = '/home/feike/BoomstraDam/TDlearning/wweights.' + str(i-1).zfill(3) +'.txt' # last valid
    # and get the number of the last processed td_out file
    weight_file = open(file_name,  'r') 
    td_version = int(weight_file.readline())
    # now the last version of td_out:
    for i in range(0, 1000):
      file_name = '/home/feike/BoomstraDam/TDlearning/td_out.' + str(i).zfill(3) +'.txt'
      try:
        weight_file = open(file_name,  'r')
      except:
        break
    return (td_version >= i-1)
    

  def start_learning(self):
    if self.we_are_up_to_date():
      self.continue_learning()
    else:
      self.proces_new_td_out()
    return
  
  def continue_learning(self):
    # initiate self play
    self.main.board_begin_position_clicked()
    self.main.panel.current_white_player.set(dc.COMPUTER)
    self.main.panel.current_black_player.set(dc.COMPUTER)
    self.main.play.ContinuePlaying = self.proces_new_td_out
    
    self.main.TotalGameTime = 4
    self.main.HandlingTime = 0
    self.main.TotalNumberOfMoves = 75        
    self.main.TotalEngineTime = 60*self.main.TotalGameTime - self.main.TotalNumberOfMoves*self.main.HandlingTime
    self.main.panel.phase.set(dc.PLAY_MODE)
    self.main.panel.play_mode()
    return
    
  def obtain_result(self):
      # end-game from engine
      self.main.play.EvalReceivedProc = self.GetResultGame
      self.main.anal.eval_mode_clicked(self.main.ActiveGame, False) # get eval, no printing
      return
      
  def GetResultGame(self):
      self.main.play.EvalReceivedProc = None # clear link
      if self.main.ActiveGame.CurrentPosition.ColorOnMove == dc.WHITE_ON_MOVE:
          if self.main.LastEval <= -20000:
              self.black_win += 1
          elif self.main.LastEval <= 10 and self.main.LastEval >= -10:
              self.remise += 1
          elif self.main.LastEval >= 20000:
              self.white_win += 1
      else:
          if self.main.LastEval >= 20000:
              self.black_win += 1
          elif self.main.LastEval <= 10 and self.main.LastEval >= -10:
              self.remise += 1
          elif self.main.LastEval <= -20000:
              self.white_win += 1
      result_str = 'w(' + str(self.white_win) + ') b(' + str(self.black_win) + ') r(' + str(self.remise) +')'
      self.main.panel.result_text.set(result_str)
      return




if __name__=='__main__':        
  main = 2
  td = TDlearning(main)
  td.proces_new_td_out()
  print td.VW[-1]
