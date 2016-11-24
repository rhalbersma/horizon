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
from ftplib import *
from pickle import *
from time import *


class BlindData():
  def __init__(self):
    self.actual = 1
    self.ply_nr = []
    for i in range(0, 29):
      self.ply_nr.append(0)
    self.finished = []
    self.status = "known"
    self.analyze_queue = [] # last in, first out
    return


class LiveDraughts():
  def __init__(self, main):
    self.url = 'http://www.livedraughts.com/worldrecord/games'
    self.local = '/home/feike/Horizon/LiveDraughts/blind'
    self.total_nr_of_rounds = 1
    self.total_nr_of_boards = 28
    self.round_nr = 1
    self.index = 1
    self.main = main
    self.initialized = False
    file_name = '/home/feike/Horizon/LiveDraughts/blind_data.pickle'
    try:
      f = open(file_name, 'r')
      self.blind_data = load(f)
      self.blind_data.status = "lost"
      f.close()
    except:
      self.blind_data = BlindData()
    return

  def save_blind_data(self):
    file_name = '/home/feike/Horizon/LiveDraughts/blind_data.pickle'
    f = open(file_name, 'w')
    dump(self.blind_data, f)
    f.close()
    return

  def all_finished(self):
    for i in range(1, 29):
      if not i in self.blind_data.finished:
        return False
    return True

  def _inc(self):
    self.blind_data.actual += 1
    if self.blind_data.actual == 29:
      self.blind_data.actual = 1
    return


  def _next(self):
    if self.all_finished() == True: return -1
    self._inc()
    while self.blind_data.actual in self.blind_data.finished:
      self._inc()
    return

  def print_status(self, last_ply, position, finished, illegal_move):
    print "bord_nr:", self.blind_data.actual, "ply_nr:", last_ply,
    if illegal_move == True:
      print "illegal move, next bord"
    elif finished == True:
      print "game finished, next bord"
    else:
      if last_ply >= self.blind_data.ply_nr[self.blind_data.actual]:
        if self.is_white_ply(last_ply) == True:
          print "Black on move",
          if self.nr_of_moves_possible(position) == 1:
            print "forced move"
          else:
            if last_ply > self.blind_data.ply_nr[self.blind_data.actual]:
              print "next bord"
            else:
              print
        else:
          print "White on move"
      else:
        print "incomplete LDF"
    return

  def start_polling(self):
    self.main.root.after(100, self.poll_actual_bord) #
    self.main.root.after(400, self.process_analyze_queue) #
    return


  def poll_actual_bord(self):
    """
    locate the actual board.
    """
    print "poll"
    print self.blind_data.status
    if self.blind_data.status == "known":
      bord_nr = self.blind_data.actual
      last_ply, position, finished, illegal_move = self.get_last_ply_nr(bord_nr)
      self.print_status(last_ply, position, finished, illegal_move)
      if last_ply < self.blind_data.ply_nr[self.blind_data.actual]:
        self.main.root.after(500, self.poll_actual_bord) # retry
        return
      if illegal_move == True:
        self._next()
        self.save_blind_data()
        self.upload_LDF_and_picture_99(self.blind_data.actual)
        self.main.root.after(500, self.poll_actual_bord) # wait next time
        return

      if finished == True:
        self.blind_data.finished.append(self.blind_data.actual) # we are done here
        self.upload_LDF_99(self.blind_data.actual)
        self._next()
        self.save_blind_data()
        print "sleep 5.0"
        sleep(5.0) # wait before switch to next bord to show move on previous bord
        self.upload_LDF_and_picture_99(self.blind_data.actual)
        self.main.root.after(500, self.poll_actual_bord) # wait next time
        return
      if last_ply == self.blind_data.ply_nr[self.blind_data.actual]:
        self.upload_LDF_99(self.blind_data.actual)
        self.main.root.after(500, self.poll_actual_bord) # wait next time
        return
      else:
        # last_ply > self.blind_data.ply_nr[self.blind_data.actual]
        self.blind_data.analyze_queue.append(self.blind_data.actual)
        self.blind_data.ply_nr[self.blind_data.actual] = last_ply
        if self.is_white_ply(last_ply) == True:
          # switch to next board, except in case of forced move
          if self.nr_of_moves_possible(position) == 1:
            self.upload_LDF_99(self.blind_data.actual)
            self.main.root.after(500, self.poll_actual_bord) # wait next time
            return
          else:
            self.upload_LDF_99(self.blind_data.actual)
            self._next()
            self.blind_data.analyze_queue.append(self.blind_data.actual) # to make sure actual is updated asap
            self.save_blind_data()
            print "sleep 5.0"
            sleep(5.0) # wait before switch to next bord to show move on previous bord
            self.upload_LDF_and_picture_99(self.blind_data.actual)
            self.main.root.after(500, self.poll_actual_bord) # wait next time
            return
        else:
          self.upload_LDF_99(self.blind_data.actual)
          self.main.root.after(500, self.poll_actual_bord) # wait next time
          return
    else: # we are lost !!!
    # search for the board where white is on move, that's the actual board
      while True:
        #if self.blind_data.actual == 15: self._next()
        print "check bord_nr", self.blind_data.actual,
        (last_ply, position, finished, illegal_move) = self.get_last_ply_nr(self.blind_data.actual)
        print last_ply
        if last_ply >= self.blind_data.ply_nr[self.blind_data.actual]:
          if finished == True:
            self.blind_data.finished.append(self.blind_data.actual) # we are done here
          elif illegal_move == True:
            pass # nothing to do, just skip
          else:
            self.blind_data.ply_nr[self.blind_data.actual] = last_ply
            if self.is_white_ply(last_ply) == False: # last move from black
              break
          self._next()
      # found where we are
      self.blind_data.status = "known"
      self.save_blind_data()
      self.main.root.after(500, self.poll_actual_bord) # wait next time
      return




  def nr_of_moves_possible(self, pos):
    if len(pos) == 0: return 0
    game = GameRecord(self, None, None)
    game.set_50_square_position_from_LDF(pos)
    move_list = self.main.mg.valid_move(game)
    return len(move_list)

  def is_white_ply(self, ply_nr):
    return (ply_nr % 2) == 1

  def get_last_ply_nr(self, bord_nr):
    url_file =    self.url + '/' + str(bord_nr) + '.LDF.gz'
    local_file = self.local + '/' + str(bord_nr) + '.LDF.gz'
    try:
      print 'try to open', url_file
      response = urllib2.urlopen(url_file)
      print "opened url", url_file
    except:
      print "can't open url", url_file
      return -2 , "", False, False
    partij = response.read()
    f = open(local_file, 'wb')
    f.write(partij)
    f.close()
    g = gzip.open(local_file, 'rb')
    ext_ply_nr = -1 # preset
    position = ""
    finished = False
    illegal_move = False
    line_nr = 0
    for line in g:
      if len(line) > 1:
        if line[1] == 'N':
          ext_ply_nr = int(line[74:77])
          position = line[56] + line[4:54]
          if (position[0] == 'B' and (ext_ply_nr % 2 == 0)) or (position[0] == 'W' and (ext_ply_nr & 2 == 1)): ext_ply_nr += 1
          # this is to solve an EBS error
          illegal_move = False
        elif line[1:3] == 'S,':
          if int(line[74:77]) != 0:
            illegal_move = True
        elif line[1:7] == 'Result':
          #print "found Result line[8] = ", line[8]
          finished = line[8] != '*'
    g.close()
    return ext_ply_nr, position, finished, illegal_move

  def analyse_line(self, line):
    cp = 1
    while line[cp] <> '"':
      cp += 1
    return line[1:cp], line[cp+1:-3]

  def next_polling(self):
    self.index += 1
    if self.index > self.total_nr_of_boards:
      self.index = 1
    self.start_polling()
    return

  def process_analyze_queue(self):
    # the queue contains the bord_nr's of the games still to be analyzed.
    print "analyze queue:", self.blind_data.analyze_queue
    if len(self.blind_data.analyze_queue) == 0:
      self.main.root.after(500, self.process_analyze_queue) #
      return

    ana_bord = self.blind_data.analyze_queue.pop()

    local_file =  self.local + '/' + str(ana_bord) + '.LDF.gz'
    local_pdn =   self.local + '/' + str(ana_bord) + '.pdn'
    local_chart = self.local + '/' + str(ana_bord) + '.chart.png'
    url_chart   = './' + str(ana_bord) + '.chart.png'

    g = gzip.open(local_file, 'rb')
    ext_ply_nr = 0 # preset
    # is a local pdn available?
    try:
      pdn = open(local_pdn, 'r')
      pdn.close()
      game = self.main.read.get_game_from_pdn_file(local_pdn)
      game.file_name_chart = local_chart
      game.url_chart = url_chart
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
      try:
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
      except:
        pass # we give it a try in the next round
      g.close()
      #print "LDF:", ext_ply_nr, "plies"

    except:
      # LDF read for the first time
      game = GameRecord(self.main, self.main.panel, self.main.panel)
      game.file_name_chart = local_chart
      game.url_chart = url_chart
      game.file_name_pdn = local_pdn
      self.main.Games = []
      self.main.Games.append(game)
      #game.BeginPosition.WhiteMan = range(31, 51)
      #game.BeginPosition.BlackMan = range(1, 21)
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
        elif word == 'S,':
          self.set_begin_position(game, line[4:54], line[56])
          print game.BeginPosition
          game.Moves = [] #reset the moves till now, discard them
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
            self.main.root.after(500, self.process_analyze_queue) # retry
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

  def set_begin_position(self, game, pos, color):
    game.BeginPosition.BlackMan = []
    game.BeginPosition.WhiteMan = []
    game.BeginPosition.BlackKing = []
    game.BeginPosition.WhiteKing = []
    if color == 'B':
      game.BeginPosition.ColorOnMove = dc.WHITE_ON_MOVE
    elif color == 'W':
      game.BeginPosition.ColorOnMove = dc.BLACK_ON_MOVE
    else:
      print "Color on move = ??", color
    for i in range(1, 51):
      k = i-1
      if pos[k] == 'b':
        game.BeginPosition.BlackMan.append(i)
      elif pos[k] == 'B':
        game.BeginPosition.BlackKing.append(i)
      elif pos[k] == 'w':
        game.BeginPosition.WhiteMan.append(i)
      elif pos[k] == 'W':
        game.BeginPosition.WhiteKing.append(i)
    return

#-----------------------------------------------------------------------------------------------------------------------------------



  def get_file_status(self):
    board_file = self.url + self.round + str(self.index) + '.LDF.gz'
    # first question, does the board_file exists?
    try:
      response = urllib2.urlopen(board_file)
    except:
      return "not existing"
    file_status = "unknown"
    partij = response.read()
    f = open(str(self.index) + '.LDF.gz', 'wb')
    f.write(partij)
    f.close()
    g = gzip.open(str(self.index) + '.LDF.gz', 'rb')
    for line in g:
      if line[1:7] == 'Result' and line[8] != '*':
        g.close()
        return "file done"
      if line[1:3] == 'S,':
        file_status = "file initialized"
      if line[1:3] == 'N,':
        file_status = "file running"
    g.close()
    return file_status



  def search_round(self):
    # round_status can be:
    #    not initialized  - no 1.LDF.gz file found
    #    initialized      - one or more *.LDF.gz files are found, all in the start position
    #    running          - initialized + one or more files have moves
    #    done             - running + all files have result

    self.round = 'ronde' + str(self.round_nr) + '/'
    round_status = "not initialized"
    all_done = True  # to check if all files are done
    for self.index in range(1, self.total_nr_of_boards + 1):
      index_status = self.get_file_status()
      #print index_status
      if index_status == "not existing":
        break # no more files to observe
      elif index_status == "file initialized":
        round_status = "initialized"
        all_done = False
      elif index_status == "file running":
        round_status = "running"
        all_done = False
      elif index_status == "file done":
        round_status = "running"
      else:
        round_status = "unknown"
    if round_status == "running" and all_done == True:
      round_status = "done"
    return round_status

  def get_current_round(self):
    for self.round_nr in range(1, self.total_nr_of_rounds + 1):
      round_status = self.search_round()
      if round_status != "done":
        break
    print "current round nr:", self.round_nr
    self.initialized = True
    self.index = 1
    return

  def put_start_charts_in_current_round(self):
    #self.get_current_round()
    ftp = FTP('www.livedraughts.com')
    ftp.login("konin897", "me82rz")
    for self.index in range(1, self.total_nr_of_boards + 1):
      print "upload:" + str(self.index) + '.chart.png'
      ftp.storbinary('STOR ./www/EK2009/ronde' + str(self.round_nr) + '/' + str(self.index) + '.chart.png', \
                     open('/home/feike/Horizon/LiveDraughts/start.png' , 'rb'))
    ftp.quit()
    return

  def upload_LDF_99(self, bord_nr):
    done = False
    while done == False:
      connect = False
      while connect == False:
        try:
          ftp = FTP('www.livedraughts.testvws.nl.testvws.nl')
          connect = True
        except:
          print "can't connect to FTP host"
          sleep(2.0)
      login = False
      while login == False:
        try:
          ftp.login("koning59", "sj41uu")
          login = True
        except:
          print "can't login to FTP host"
          sleep(2.0)
      try:
        ftp.storbinary('STOR ./www/99.LDF.gz', \
                       open('/home/feike/Horizon/LiveDraughts/blind/' + str(bord_nr) + '.LDF.gz' , 'rb'))
        print "LDF", bord_nr, 'to 99'
        done = True
      except:
        print "can't upload to 99", '/home/feike/Horizon/LiveDraughts/blind/' + str(bord_nr) + '.LDF.gz'
        sleep(1.0)
      try:
        ftp.quit()
      except:
        pass
# -------------------------------------------- chart to different server ---------------------------------
    done = False
    while done == False:
      connect = False
      while connect == False:
        try:
          ftp = FTP('www.livedraughts.testvws.nl.testvws.nl')
          connect = True
        except:
          print "can't connect to FTP host"
          sleep(2.0)
      login = False
      while login == False:
        try:
          ftp.login("koning59", "sj41uu")
          login = True
        except:
          print "can't login to FTP host"
          sleep(2.0)
      try:
        ftp.storbinary('STOR ./www/99.chart.png', \
                       open('/home/feike/Horizon/LiveDraughts/blind/' + str(bord_nr) + '.chart.png' , 'rb'))
        print "chart", bord_nr, 'to 99'
        done = True
      except:
        print "can't upload to 99", '/home/feike/Horizon/LiveDraughts/blind/' + str(bord_nr) + '.chart.png'
        sleep(1.0)
      try:
        ftp.quit()
      except:
        pass
    return

  def upload_LDF_and_picture_99(self, bord_nr):
    done = False
    while done == False:
      connect = False
      while connect == False:
        try:
          ftp = FTP('www.livedraughts.testvws.nl.testvws.nl')
          connect = True
        except:
          print "can't connect to FTP host"
          sleep(2.0)
      login = False
      while login == False:
        try:
          ftp.login("koning59", "sj41uu")
          login = True
        except:
          print "can't login to FTP host"
          sleep(2.0)
      try:
        ftp.storbinary('STOR ./www/99.LDF.gz', \
                       open('/home/feike/Horizon/LiveDraughts/blind/' + str(bord_nr) + '.LDF.gz' , 'rb'))
        print "LDF", bord_nr, 'to 99'
        done = True
      except:
        print "can't upload to 99", '/home/feike/Horizon/LiveDraughts/blind/' + str(bord_nr) + '.LDF.gz'
        sleep(1.0)
      try:
        ftp.quit()
      except:
        pass

# -------------------------------------------- chart and jpg to different server ---------------------------------
    done = False
    while done == False:
      connect = False
      while connect == False:
        try:
          ftp = FTP('www.livedraughts.testvws.nl.testvws.nl')
          connect = True
        except:
          print "can't connect to FTP host"
          sleep(2.0)
      login = False
      while login == False:
        try:
          ftp.login("koning59", "sj41uu")
          login = True
        except:
          print "can't login to FTP host"
          sleep(2.0)
      try:
        ftp.storbinary('STOR ./www/99.jpg', \
                       open('/home/feike/Horizon/LiveDraughts/images/' + str(bord_nr) + '.jpg' , 'rb'))
        print "photo nr", bord_nr, 'to 99'
        try:
          ftp.storbinary('STOR ./www/99.chart.png', \
                         open('/home/feike/Horizon/LiveDraughts/blind/' + str(bord_nr) + '.chart.png' , 'rb'))
          print "chart", bord_nr, 'to 99'
          done = True
        except:
          print "can't upload to 99", '/home/feike/Horizon/LiveDraughts/blind/' + str(bord_nr) + '.chart.png'
          sleep(1.0)
      except:
        print "can't upload to 99", '/home/feike/Horizon/LiveDraughts/images/' + str(bord_nr) + '.jpg'
        sleep(1.0)
      try:
        ftp.quit()
      except:
        pass

    return


