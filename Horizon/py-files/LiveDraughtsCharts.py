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

class LiveDraughts():
  def __init__(self, main):
    self.url = 'http://www.livedraughts.com/NKvrouwen/'
    self.local = '/home/feike/Horizon/LiveDraughts/'
    self.total_nr_of_rounds = 9
    self.total_nr_of_boards = 5
    self.round_nr = 1
    self.index = 1
    self.main = main
    return

  def get_current_round_nr(self):
    pickle_file = self.local + 'vrouwen.pickle'
    try:
      f = open(pickle_file, 'r')
      self.save_data = load(f)
      f.close()
      self.round_nr = self.save_data.cur_round
      self.index = self.save_data.cur_index
    except:
      self.save_data = SaveRoundInfo(self.round_nr, self.index)
    return

  def save_current_round_nr(self):
    pickle_file = self.local + 'vrouwen.pickle'
    self.save_data.cur_round = self.round_nr
    self.save_data.cur_index = self.index
    f = open(pickle_file, 'w')
    dump(self.save_data, f)
    f.close()
    return

  def start_polling(self):
    self.get_current_round_nr()
    self.info = DraughtGetCurrentRoundPanel(self, str(self.round_nr))
    self.info.root.mainloop()
    return

  def read_current_round_nr(self):
    self.round_nr = int(self.info.cur_round.get())
    self.info.root.destroy()
    self.save_current_round_nr()
    self.continue_polling()
    return

  def analyse_line(self, line):
    cp = 1
    while line[cp] <> '"':
      cp += 1
    return line[1:cp], line[cp+1:-3]

  def next_polling(self):
    self.index += 1
    if self.index > self.total_nr_of_boards:
      self.index = 1
    self.continue_polling()
    return

  def continue_polling(self):
    url_file =    self.url   + 'ronde' + str(self.round_nr) + '/' + str(self.index) + '.LDF.gz'
    local_file =  self.local + 'ronde' + str(self.round_nr) + '/' + str(self.index) + '.LDF.gz'
    local_pdn =   self.local + 'ronde' + str(self.round_nr) + '/' + str(self.index) + '.pdn'
    local_chart = self.local + 'ronde' + str(self.round_nr) + '/' + str(self.index) + '.chart.png'
    url_chart   = './www/NKvrouwen/charts' + '/' + str(self.index) + '.chart.png'
    try:
      response = urllib2.urlopen(url_file)
      print "opened url", url_file
    except:
      print "can't open url", url_file
      self.main.root.after(500, self.next_polling) # first try next
      return
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


  def put_start_charts_in_current_round(self):
    self.get_current_round_nr()
    ftp = FTP('www.livedraughts.com')
    ftp.login("konin897", "me82rz")
    for self.index in range(1, self.total_nr_of_boards + 1):
      print "upload:" + str(self.index) + '.chart.png'
      ftp.storbinary('STOR ./www/NKvrouwen/charts/' + str(self.index) + '.chart.png', \
                     open('/home/feike/Horizon/LiveDraughts/start.png' , 'rb'))
    ftp.quit()
    return

#--------------------------------------------------------------------------------------------------------#

"""User Interface for obtaining round information"""

from Tkinter import *
from DraughtConstants import *
dc =DraughtConstants()         # get the global constants

class DraughtGetCurrentRoundPanel:


    def __init__(self, main, round, root=None) :

        if root==None:
            root=Tk()
            root.geometry('300x400+350+300')
            self.main = main

        self.root = root
        self.root.title("Current Round Info")
        self.main = main

        # frames
        self.frame_overall_frame          = Frame(self.root, relief = GROOVE, bd =4, height=300)

        # key bindings
        self.root.bind("<Escape>", self.esc_proc)
        self.root.protocol("WM_DELETE_WINDOW", self.end_proc)

        # label
        self.tx1 = Label(self.frame_overall_frame, text="Current Round Number").grid(row=0,column=0)

        # entry
        self.cur_round = Entry(self.frame_overall_frame)
        self.cur_round.grid(row=0,column=1)
        self.cur_round.insert(END, round)
        self.cur_round.select_range(0, END)

        #ok button
        self.ok_button = Button(self.frame_overall_frame, text="OK", command=self.main.read_current_round_nr)
        self.ok_button.grid(row=5, column=1)
        self.frame_overall_frame.grid()

        return # __init__

    # -------------------------  event handlers --------------------------------- #

    def esc_proc(self, event):
        self.root.destroy()
        return

    def end_proc(self):                                 # called from 'exit' and 'x'
        self.root.destroy()
        return

class SaveRoundInfo():
  def __init__(self, round, index):
    self.current_round = round
    self.cur_index = index
    return
