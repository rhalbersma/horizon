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

class LiveMenuProc():
  def __init__(self, main, index):
    self.main  = main
    self.index = index
    self.live = LiveDraughts()
    return

  def __call__(self):
    self.live_clicked(self.index)
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
    url_file = self.live.url + str(index) + '.LDF.gz'
    try:
      response = urllib2.urlopen(url_file)
    except:
      print "can't open url", url_file
      return
    partij = response.read()
    f = open(str(index) + '.LDF.gz', 'wb')
    f.write(partij)
    f.close()
    g = gzip.open(str(index) + '.LDF.gz', 'rb')
    game = GameRecord(self.main, self.main.panel, self.main.panel)
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

    self.main.ActiveGame = self.main.Games[0]
    self.main.ActiveGameNumber = 0
    self.main.panel.current_color_on_move.set(self.main.ActiveGame.BeginPosition.ColorOnMove)
    self.main.re_play.click(self.main.re_play.compose_tag([0, game.half_ply_length()]))
    game.refresh_display()
    game.write_moves()
    self.main.walk_hash.setup_hash()
    return True



#-----------------------------------------------------------------------------------------------------------------------------------

class LiveDraughts():
  def __init__(self):
    self.url = 'http://www.livedraughts.com/WK2009/ronde1/'
    return

  def get_players(self):
    result = []
    for i in range(1, 8):
      url_file = self.url + str(i) + '.LDF.gz'
      try:
        response = urllib2.urlopen(self.url + str(i) + '.LDF.gz')
      except:
        break
      partij = response.read()
      f = open(str(i) + '.LDF.gz', 'wb')
      f.write(partij)
      f.close()
      g = gzip.open(str(i) + '.LDF.gz', 'rb')
      line = g.readline()
      line = g.readline()
      line = g.readline()
      line = g.readline()
      white = line[7:-3]
      line = g.readline()
      black = line[7:-3]
      g.close()
      result.append(white + ' - ' + black)
    return result


