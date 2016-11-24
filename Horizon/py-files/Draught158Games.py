#
#  This file is Copyright (C) 2010 Feike Boomstra.
#  Distributed under the Boost Software License, Version 1.0.
#  (See accompanying file LICENSE_1_0.txt or copy at
#  http://www.boost.org/LICENSE_1_0.txt)
#


from pickle import *
from DraughtConstants import *
dc = DraughtConstants()
from DraughtDataStructures import *
from time import *
from Tkinter import *
import os
import fnmatch
import datetime

class Draught158Games:
  def __init__(self, main):
    self.main              = main
    self.MyStartColor      = "white"
    self.CurrentFirstMove  = 0
    self.CurrentSecondMove = 0
    self.CurrentGameNr     = 0
    self.GamesLost         = 0
    self.GamesDraw         = 0
    self.GamesWon          = 0
    self.GamesUnknown      = 0
    return

  def _next_position(self):
    if self.CurrentSecondMove == 8:
      if self.CurrentFirstMove == 8:
        if self.MyStartColor == "white":
          self.CurrentSecondMove = 0
          self.CurrentFirstMove  = 0
          self.MyStartColor      = "black"
        else:
          return False
      else:
        self.CurrentSecondMove = 0
        self.CurrentFirstMove += 1
    else:
      self.CurrentSecondMove += 1
    if self.CurrentFirstMove == 1 or self.CurrentFirstMove == 2:
      if self.CurrentSecondMove == 0:
        self.CurrentSecondMove += 1  # skip these moves
    self.CurrentGameNr += 1
    return True

  def _set_position(self):
    game = GameRecord(self.main, self.main.panel, self.main.panel, None)
    game.BeginPosition.BlackMan = range(1, 21)
    game.BeginPosition.WhiteMan = range(31, 51)
    game.BeginPosition.ColorOnMove = dc.WHITE_ON_MOVE
    game.set_begin_position()
    ml = self.main.play.mg.valid_move(game)                                     # get all valid moves
    #for move in ml: print move
    game.do_move(ml[self.CurrentFirstMove])
    ml = self.main.play.mg.valid_move(game)                                     # get all valid moves
    #for move in ml: print move
    game.do_move(ml[self.CurrentSecondMove])
    return game

  def continue_158(self):
    self.main.play.EvalReceivedProc = self.one_game_finished

    self.main.TotalGameTime = 9
    self.main.HandlingTime = 0
    self.main.TotalNumberOfMoves = 75
    self.main.TotalEngineTime = 540

    if self.MyStartColor == "white":
      self.main.panel.current_black_player.set(dc.REMOTE_PLAYER)
      self.main.panel.current_white_player.set(dc.COMPUTER)
    else:
      self.main.panel.current_white_player.set(dc.REMOTE_PLAYER)
      self.main.panel.current_black_player.set(dc.COMPUTER)
    self.main.play.startup_engine()
    self.main.remote_play_mode_clicked(False)
    self.main.Games = []
    self.main.Games.append(self._set_position())
    self.main.ActiveGame = self.main.Games[0]
    if self.MyStartColor == "white":
      self.main.ActiveGame.WhitePlayer = "Hor_" + str(self.CurrentGameNr).zfill(3)
      self.main.ActiveGame.BlackPlayer = "Truus"
    else:
      self.main.ActiveGame.WhitePlayer = "Truus"
      self.main.ActiveGame.BlackPlayer = "Hor_" + str(self.CurrentGameNr).zfill(3)
    self.main.ActiveGame.Round = str(self.CurrentGameNr).zfill(3)
    self.main.ActiveGame.refresh_display()
    result_str = str(self.GamesWon) + ' - ' + str(self.GamesDraw) + ' - ' +  str(self.GamesLost) + '- (' + str(self.GamesUnknown) + ')'
    self.main.panel.result_text.set(result_str)
    # and start playing
    self.main.panel.phase.set(dc.PLAY_MODE)
    self.main.play.EndOfGameContinue = self.one_game_finished
    self.main.play.play_mode()
    #self.one_game_finished()    # test only
    return

  def one_game_finished(self, msg):
    if self.MyStartColor == "white":
        if msg[1:4] == '0-2':
          self.GamesLost += 1
          file_name = str(self.CurrentGameNr).zfill(3) + "_lost.pdn"
        elif msg[1:4] == '1-1':
          self.GamesDraw += 1
          file_name = str(self.CurrentGameNr).zfill(3) + "_draw.pdn"
        elif msg[1:4] == '2-0':
          self.GamesWon += 1
          file_name = str(self.CurrentGameNr).zfill(3) + "__won.pdn"
        else:
          self.GamesUnknown += 1
          file_name = str(self.CurrentGameNr).zfill(3) + "__unknown.pdn"
    else:
        if msg[1:4] == '2-0':
          self.GamesLost += 1
          file_name = str(self.CurrentGameNr).zfill(3) + "_lost.pdn"
        elif msg[1:4] == '1-1':
          self.GamesDraw += 1
          file_name = str(self.CurrentGameNr).zfill(3) + "_draw.pdn"
        elif msg[1:4] == '0-2':
          self.GamesWon += 1
          file_name = str(self.CurrentGameNr).zfill(3) + "__won.pdn"
        else:
          self.GamesUnknown += 1
          file_name = str(self.CurrentGameNr).zfill(3) + "__unknown.pdn"

    result_str = str(self.GamesWon) + ' - ' + str(self.GamesDraw) + ' - ' +  str(self.GamesLost) + '- (' + str(self.GamesUnknown) + ')'
    self.main.panel.result_text.set(result_str)
    self.main.ActiveGame.Result = msg[1:4]
    # save the game
    f = open('/home/feike/Horizon/pdn-files/158/' + file_name, 'w')
    f.write(self.main.save.game_to_string(self.main.ActiveGame))
    f.close()
    print file_name, "saved"
    # update the parameters
    if self._next_position() == False:
      return
    # and save environment
    file_name = '/home/feike/Horizon/pdn-files/158/pickle'
    save_main = self.main
    self.main = None
    f = open(file_name, 'w')
    dump(self, f)
    f.close()
    self.main = save_main

    sleep(5) # this is for Truus

    self._set_position()
    self.continue_158()
    return

  def play_single_game(self):
    self.info =  SingleGameInfoPanel(self, 6)
    self.info.root.mainloop()
    return


  def Continue_Single_Game(self):
    self.SingleNumber = int(self.info.game_nr_text.get())
    self.info.root.destroy()

    self.main.play.EvalReceivedProc = None
    self.main.play.EndOfGameContinue = None

    for k in range(0, self.SingleNumber):
      self._next_position()

    if self.MyStartColor == "white":
      self.main.panel.current_black_player.set(dc.REMOTE_PLAYER)
      self.main.panel.current_white_player.set(dc.COMPUTER)
    else:
      self.main.panel.current_white_player.set(dc.REMOTE_PLAYER)
      self.main.panel.current_black_player.set(dc.COMPUTER)
    self.main.play.startup_engine()
    self.main.remote_play_mode_clicked(False)
    self.main.Games = []
    self.main.Games.append(self._set_position())
    self.main.ActiveGame = self.main.Games[0]
    if self.MyStartColor == "white":
      self.main.ActiveGame.WhitePlayer = "Horizon"
      self.main.ActiveGame.BlackPlayer = "Truus"
    else:
      self.main.ActiveGame.WhitePlayer = "Truus"
      self.main.ActiveGame.BlackPlayer = "Horizon"
    self.main.ActiveGame.Round = str(self.CurrentGameNr).zfill(3)
    self.main.ActiveGame.refresh_display()
    # and start playing
    self.main.panel.phase.set(dc.PLAY_MODE)
    self.main.play.play_mode()
    #self.one_game_finished()    # test only
    return


class Play_158:
  def __init__(self,  main):
    self.main = main
    return

  def play_158(self):
    file_name = '/home/feike/Horizon/pdn-files/158/pickle'
    try:
      f = open(file_name, 'r')
      play158 = load(f)
      f.close()
      play158.main = self.main
    except:
      play158 = Draught158Games(self.main)
    play158.continue_158()
    return

  def play_single_game(self):
    play158 = Draught158Games(self.main)
    play158.play_single_game()
    return

  def make_lost_docs(self):
    for file in os.listdir('/home/feike/Horizon/pdn-files/158'):
      if fnmatch.fnmatch(file, '*lost.pdn'):
        long_file_name = '/home/feike/Horizon/pdn-files/158/' + file
        self.main.read.get_pdn_file(True, long_file_name)
        export_file_name = '/home/feike/workspace/lost_' + str(datetime.date.today()) + '_' + self.main.ActiveGame.Round + '.doc'
        self.main.save.save_doc_file(export_file_name)
        # and copy the pdn file:
        f = open(long_file_name, 'r')
        g = open('/home/feike/Horizon/pdn-files/lost/lost_' + str(datetime.date.today()) + '_' + self.main.ActiveGame.Round + '.pdn', 'w')
        for line in f:
          g.write(line)
        f.close()
        g.close()
    return



class SingleGameInfoPanel:


    def __init__(self, p158, default_number) :

        self.p158=p158

        self.root = Tk()
        self.root.title("Get Game Nr")
        #self.main = main

        #self.root.option_add("*Font", "TSCu_Paranar 10")

        #variables

        # frames
        self.frame_overall_frame          = Frame(self.root, relief = GROOVE, bd =4, height=200)


        # key bindings
        self.root.bind("<Escape>", self.esc_proc)
        self.root.protocol("WM_DELETE_WINDOW", self.end_proc)


        self.tx1 = Label(self.frame_overall_frame, text="Game Nr").grid(row=0,column=0)


        self.game_nr_text = Entry(self.frame_overall_frame)
        self.game_nr_text.grid(row=0,column=1)
        self.game_nr_text.insert(END, str(default_number))

        #ok button
        self.ok_button = Button(self.frame_overall_frame, text="OK", command=self.p158.Continue_Single_Game)
        self.ok_button.grid(row=3, column=1)


#        self.left_frame.pack(side=LEFT)
#        self.right_frame.pack(side=RIGHT)
        self.frame_overall_frame.grid()

        return # __init__

    # -------------------------  event handlers --------------------------------- #

    def esc_proc(self, event):
        self.root.destroy()
        return

    def end_proc(self):                                 # called from 'exit' and 'x'
        self.root.destroy()
        return
