#
#  This file is Copyright (C) 2010 Feike Boomstra.
#  Distributed under the Boost Software License, Version 1.0.
#  (See accompanying file LICENSE_1_0.txt or copy at
#  http://www.boost.org/LICENSE_1_0.txt)
#


import urllib2
from zlib import *
import gzip
from DraughtDataStructures import *
from DraughtSetUp import *
import os
import shutil

class LiveMenuProc():
  def __init__(self, panel, main):
    self.main  = main
    self.live = panel.live
    #print "LiveMenu Instance created"
    return

  def __call__(self):
    #print "LiveMenu Instance called"
    self.live_clicked(self.live.index)
    return

  def analyse_line(self, line):
    cp = 1
    while line[cp] <> '"':
      cp += 1
    return line[1:cp], line[cp+1:-3]

  def live_clicked(self, index):
    while True:
      if self.live_clicked_proc(index) == True:
        break
    return

  def live_clicked_proc(self, index):
    url_file =    self.live.url   + self.live.round + '/' + str(index) + '.LDF.gz'
    local_file =  self.live.local + self.live.round + '/' + str(index) + '.LDF.gz'
    local_pdn =   self.live.local + self.live.round + '/' + str(index) + '.pdn'
    local_chart = self.live.local + self.live.round + '/' + str(index) + '.png'
    try:
      print "open url", url_file

      response = urllib2.urlopen(url_file)
    except:
      print "can't open url", url_file
      return False
    partij = response.read()
    f = open(local_file, 'wb')
    f.write(partij)
    f.close()
    g = gzip.open(local_file, 'rb')
    ext_ply_nr = 0 # preset
    # is a local pdn available?
    try:
      pdn = open(local_pdn, 'r')
      pdn.close()
      game = self.main.read.get_game_from_pdn_file(local_pdn)
      game.file_name_chart = local_chart
      game.file_name_pdn = local_pdn
      # add the new collected moves (if any)
      self.main.ActiveGame = game
      self.main.Games[0] = game
      game.HalfPlyPointer = game.half_ply_length()
      self.main.ActiveGameNumber = 0
      self.main.panel.current_color_on_move.set(self.main.ActiveGame.BeginPosition.ColorOnMove)
      game.refresh_display()
      game.write_moves()
      self.main.re_play.click(self.main.re_play.compose_tag([0, game.half_ply_length()]))
      #print "pdn is loaded"
      #print "last move is",
      #print game.Moves[-1]
      for line in g:
        if line[1] == 'N':
          ext_ply_nr = int(line[74:77])
          #print ext_ply_nr, game.half_ply_length()
          if ext_ply_nr > game.half_ply_length():
            start = int(line[78:80])
            stop = int(line[81:83])
            op = line[80]
            color = line[56]
            print "adding: ", ext_ply_nr, color, start, op, stop
            if color == 'B': # it was the white move, now black is on move
              move = PlyRecord()
              move.Ply_nr = (ext_ply_nr - 1)/2 + 1
              move.WhiteMove = HalfPlyRecord()
              move.WhiteMove.Start = start
              move.WhiteMove.op = op
              move.WhiteMove.Stop = stop
              move.WhiteMove.Step.append(start)
              move.WhiteMove.Step.append(stop)
              game.Moves.append(move)
            else:
              # we have to append only a black move.BlackMove
              move = game.Moves[-1]
              move.BlackMove = HalfPlyRecord()
              move.BlackMove.Start = start
              move.BlackMove.op = op
              move.BlackMove.Stop = stop
              move.BlackMove.Step.append(start)
              move.BlackMove.Step.append(stop)
#        elif line[1:7] == 'Result' and line[8] != '*':
#          # end of game in n.LDF.gz
#          # micro match, switch to other LDF
#          os.remove(local_file)
#          os.remove(local_pdn)
#          os.remove(local_chart)
#          if self.live.index == 1:
#            self.live.index = 2
#          else:
#            self.live.index = 1
#            self.main.root.after(1000, LiveMenuProc(self.main.panel, self.main))

      g.close()
      #print "LDF:", ext_ply_nr, "plies"

    except:
      # LDF read for the first time
      game = GameRecord(self.main, self.main.panel, self.main.panel)
      game.file_name_chart = local_chart
      game.file_name_pdn = local_pdn
      self.main.Games = []
      self.main.Games.append(game)
      game.BeginPosition.WhiteMan = range(31, 51)
      game.BeginPosition.BlackMan = range(1, 21)
      game.HalfPlyPointer = 0
      #self.main.setup.set_begin_position_on_board(game)
      ply_nr = 0
      white = True
      for line in g:
        word, kw_text = self.analyse_line(line)
        if   word == 'Event':  game.Event       = kw_text
        elif word == 'Date':   game.Date        = kw_text
        elif word == 'Black':  game.BlackPlayer = kw_text
        elif word == 'White':  game.WhitePlayer = kw_text
        elif word == 'Site':   game. Site       = kw_text
        elif word == 'Result': game.Result      = kw_text
        elif word == 'Round':  game.Round       = kw_text
        elif word == 'GameType': game.GameType  = kw_text
        elif word == 'SetUp':  game.SetUp       = kw_text
        elif word == 'N,': #collect the moves
          try:
            start = int(line[78:80])
            stop = int(line[81:83])
            op = line[80:81]
          except:
            #got incomplete last line
            g.close()
            print "incomplete?"
            print line
            print line[78:80], line[80:81], line[81:83]
            return False
          if white == True:
            move = PlyRecord()
            ply_nr += 1
            move.Ply_nr = ply_nr
            move.WhiteMove = HalfPlyRecord()
            move.WhiteMove.Start = start
            move.WhiteMove.op = op
            move.WhiteMove.Stop = stop
            move.WhiteMove.Step.append(start)
            move.WhiteMove.Step.append(stop)
            game.Moves.append(move)
            white = not white
          else:
            move.BlackMove = HalfPlyRecord()
            move.BlackMove.Start = start
            move.BlackMove.op = op
            move.BlackMove.Stop = stop
            move.BlackMove.Step.append(start)
            move.BlackMove.Step.append(stop)
            white = not white
      # read the first time
      #self.main.chart.create_chart(game.file_name_chart)
      #self.main.play.beam_up_chart(game.file_name_chart)



    self.main.ActiveGame = self.main.Games[0]
    self.main.ActiveGameNumber = 0
    self.main.panel.current_color_on_move.set(self.main.ActiveGame.BeginPosition.ColorOnMove)
    self.main.re_play.click(self.main.re_play.compose_tag([0, game.half_ply_length()]))
    game.refresh_display()
    game.write_moves()

    # get move values
    self.main.anal.analyze_game()
    return True



#-----------------------------------------------------------------------------------------------------------------------------------

class LiveDraughts():
  def __init__(self):
    self.url = 'http://www.livedraughts.com/WK2009/'
    self.local = '/home/feike/Horizon/LiveDraughts/'
    self.round_nr = 1
    self.round = 'ronde1'
    self.index = 1
    self.current_board = 1
    return

  def get_players(self):
    result = []
    white = ''
    black = ''
    try:
      os.remove(self.local + '1.pdn')
    except:
      pass
    try:
      os.remove(self.local + '2.pdn')
    except:
      pass
    for self.round_nr in range(2, 3):
      result_found = False
      #self.round = 'Barrage' + str(self.round_nr)
      self.round = '' # micro match
      #print "try Barrage", self.round_nr
      for i in range(self.current_board, self.current_board+1):
        self.index = i
        #url_file = self.url + self.round + '/' + str(i) + '.LDF.gz'
        url_file = self.url + str(i) + '.LDF.gz' # micro match
        try:
          response = urllib2.urlopen(url_file)
        except:
          return result
        partij = response.read()
        f = open(str(i) + '.LDF.gz', 'wb')
        f.write(partij)
        f.close()
        g = gzip.open(str(i) + '.LDF.gz', 'rb')
        for line in g:
          if line[1:6] == 'White': white = line[7:-3]
          elif line[1:6] == 'Black': black = line[7:-3]
          #elif line[1:7] == 'Result' and line[8] != '*': result_found = True
          if line[1:3] == 'S,':
            break
        g.close()
        if result_found == False:
          result.append(self.round + ': ' + white + ' - ' + black)
        else:
          #pass
          break
      if result_found == False: break
    return result


