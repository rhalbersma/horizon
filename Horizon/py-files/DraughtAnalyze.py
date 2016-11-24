#
#  This file is Copyright (C) 2010 Feike Boomstra.
#  Distributed under the Boost Software License, Version 1.0.
#  (See accompanying file LICENSE_1_0.txt or copy at
#  http://www.boost.org/LICENSE_1_0.txt)
#


from DraughtConstants import *
from ComputerPlayer import *
from DraughtAnalyzePanel import *
from DraughtDataStructures import *
dc = DraughtConstants()
from LiveDraughtsWK import *

class DraughtAnalyze:
    def __init__(self, main):
        self.cp = ComputerPlayer()
        self.main = main
        self.analpan = None
        self.CurrentMoveDepth = 0
        self.ActiveBoard = None
        self.ActiveEvalPar = None
        self.ActiveEvalGame = None
        self.SearchDepth = 0

        return

    def anal_pos_mode_clicked(self, wanted_depth):
        game = self.main.ActiveGame
        self.main.play.startup_engine()
        game.CurrentPosition.ColorOnMove = self.main.panel.current_color_on_move.get()
        self.analpan = DraughtAnalyzePanel(self)
        self.analpan.top.title("Analyze (depth = " + str(wanted_depth).zfill(2) + ' )' )
        self.Game_1 = GameRecord(self.main,  self.analpan.board1, self.analpan.board1)
        self.Game_2 = GameRecord(self.main,  self.analpan.board2, self.analpan.board2)
        self.Game_3 = GameRecord(self.main,  self.analpan.board3, self.analpan.board3)
        self.CurrentMoveDepth = wanted_depth
        self.SearchDepth = self.CurrentMoveDepth
        self.analpan.board3.display(game.CurrentPosition)
        #self.main.play.ProcesFixedDepth = self.proces_fixed_depth_move
        self.all_moves = self.main.mg.valid_move(game)
        self.main.play.engine.send_analyse_position(game, wanted_depth)
        # obtained moves in "receive all moves"
        return

    def receive_all_moves(self, msg):
        #receive all moves (with move-value) in the given configuration
        #print "python: ", msg
        cnt = (len(msg) - 1)/(14 + 51)
        #print cnt, len(msg)
        #self.all_moves = []
        for j in range(0, cnt):
            k=j*65+1
            Start = int(msg[k:k+2])
            Stop = int(msg[k+3:k+5])
            MoveValue = int( msg[k+7:k+13])
            stand = msg[k+14:k+65]
            #print msg[k+14:k+65]
            self.InsertValue(Start,  Stop,  MoveValue, stand)
        self.all_moves.sort(reverse=True)
        # moves are sorted in decreasing order by MoveValue
        self.analpan.all_moves.delete(first=0, last=self.analpan.all_moves.size()-1)
        for j in range(0, cnt):
            self.analpan.all_moves.insert(END, str(self.all_moves[j].Start).zfill(2) + self.all_moves[j].op + str(self.all_moves[j].Stop).zfill(2) +\
            ' (' + str(self.all_moves[j].MoveValue).rjust(6) + ')')
        return

    def InsertValue(self,  start,  stop,  value, stand):
        for move in self.all_moves:
            if move.Start == start and move.Stop == stop:
                move.MoveValue = value
                move.stand = stand
        return

    def show1_pressed(self):
        self.ActiveBoard = self.analpan.board1
        self.ActiveEvalPar = self.analpan.eval1_par
        self.ActiveEvalGame = self.Game_1
        self.Show_continue()
        return

    def show2_pressed(self):
        self.ActiveBoard = self.analpan.board2
        self.ActiveEvalPar = self.analpan.eval2_par
        self.ActiveEvalGame = self.Game_2
        self.Show_continue()
        return

    def Show_continue(self):
        #print "Show_continue"
        selection = self.analpan.all_moves.curselection()
        if selection==():
            self.analpan.main_variant.delete(1.0, END)
            self.analpan.main_variant.insert(END, "please select a move first")
            return
        stand = self.all_moves[int(selection[0])].stand
        #print "stand is: ", stand
        self.ActiveEvalGame.set_50_square_position(stand)
        self.ActiveEvalGame.refresh_display()
        self.main.play.engine.send_evaluate_position_2(self.ActiveEvalGame, False)
        return


##    def proces_fixed_depth_move(self, s):
##        game = self.ActiveEvalGame
##        if s[1] == 'x': return
##        start = int(s[1:3])
##        stop = int(s[3:5])
##        value = int(s[5:11])
##        print "python(1): ", start, stop, value
##        valid, move = game.start_stop_to_move_record(start, stop)
##        if valid == False: return
##        print "python(2)", move.Start, move.Stop
##        #we have to decide whether we should stop, is different for MAX side and MIN side
##        if self.MaxColor == self.ActiveBoard.board_position.ColorOnMove:
##            # MAX node
##            if self.CurrentMoveDepth <= 0 and move.op == '-':
##                #we have collected the main variant, now get the evaluation
##                self.main.play.engine.send_evaluate_position_2(game, False)
##                return # the end of the row
##        else:
##            if self.CurrentMoveDepth <= 0 and move.op == '-':
##                #we have collected the main variant, now get the evaluation
##                self.main.play.engine.send_evaluate_position_2(game, False)
##                return # the end of the row
##
##        game.do_move(move,True)
##        self.analpan.main_variant.insert(END, str(move.Start).zfill(2) + move.op + str(move.Stop).zfill(2) + '(' + str(value).rjust(6) + ')')
##        if self.ActiveBoard.board_position.ColorOnMove == dc.WHITE_ON_MOVE:
##            self.analpan.main_variant.insert(END, ' ')
##        else:
##            self.analpan.main_variant.insert(END, '\n')
##        self.ActiveBoard.display(self.ActiveEvalGame.CurrentPosition)
##        if move.op == '-':
##            self.CurrentMoveDepth = self.CurrentMoveDepth - 1
##        self.main.play.engine.send_get_fixed_depth_move(self.ActiveEvalGame , self.CurrentMoveDepth, \
##                                                                                          value, self.analpan.get_orig_color(), False)
##
##        return

    def eval_par_received(self, msg):
        eval = self.ActiveEvalPar.display(msg)
        return



    def anal_pos_mode_clicked_old(self, wanted_depth):
        game = self.main.ActiveGame
        self.main.play.startup_engine()
        game.CurrentPosition.ColorOnMove = self.main.panel.current_color_on_move.get()
        move = None #game.is_valid_move(game.get_half_ply_pointer_record())
        if move == None:
            self.main.play.engine.send_analyse_position(game, wanted_depth)
        else:
            self.main.play.engine.send_analyse_move(game, move, wanted_depth)
        return

    def anal_time_mode_clicked(self):
        self.main.read.get_pdn_file(True) #open the time test pdn file
        self.main.play.startup_engine()
        game = self.main.ActiveGame
        self.main.play.engine.send_init_game(game)
        self.NextHalfPly = 56
        self.ShowTimingOfThisPostion(14)
        return

    def ShowTimingOfThisPostion(self,  half_ply):
        game = self.main.ActiveGame
        game.HalfPlyPointer = half_ply
        game.build_till_pointer()
        game.refresh_display()
        self.main.play.engine.send_analyse_timing(game)
        return

    def send_next_move_for_timing(self):
        self.main.play.engine.send_final_report()
        if self.NextHalfPly == 56:
            self.NextHalfPly = 76
        elif self.NextHalfPly == 76:
            self.NextHalfPly = 96
        else:
            return
        self.ShowTimingOfThisPostion(self.NextHalfPly)
        return

    def final_report_ready(self):
        #print "python -- final report ready"
        self.main.play.engine.send_final_report()
        return

    def eval_mode_clicked(self, game, display=True):
        self.main.play.startup_engine()
        game.CurrentPosition.ColorOnMove = self.main.panel.current_color_on_move.get()
        #self.main.play.engine.send_get_fixed_depth_move(game , 10, 0,  'W', False,True)
        self.main.play.engine.send_evaluate_position(game, display)
        self.main.panel.setup_mode()
        return

    def init_board(self):
        self.setup.setup_board()
        self.bord.ColorOnMove = dc.WHITE_ON_MOVE
        self.cp.c_init_tree()
        return

    def get_next_move(self, game, pnt):
        if self.bord.is_color_white():
            start = game.Moves[pnt].White_start
            stop = game.Moves[pnt].White_stop
            token = game.Moves[pnt].White_op
        else:
            start = game.Moves[pnt].Black_start
            stop = game.Moves[pnt].Black_stop
            token = game.Moves[pnt].Black_op
        ply = game.Moves[pnt].Ply_nr
        if start == '' or stop == '': return "empty start of stop"
        ml = self.mg.valid_move(self.bord)                                     # get all valid moves, maybe we have a capture.
        if ml == []: return "empty list"
        for move in ml:
            if move.start == start and move.stop == stop:
                break
        if not (move.start == start and move.stop == stop):
            return "mismatch"
        # no invalid neither impossible moves
        # print  start, stop, chr(ord('(')+start) + chr(ord('(')+stop)
        return chr(ord('(')+start) + chr(ord('(')+stop)

    def get_Incremental_board_position(self, game, ply):                  # for testing only, must be called incremental
        ok = self.setup.exec_one_half_ply(game, ply)                          # white move
        if not ok: return
        ok = self.setup.exec_one_half_ply(game, ply)                          # black move
        if not ok: return
        return

    def analyze_game(self):    # called from LiveDraughts
      game = self.main.ActiveGame
      game.HalfPlyPointer = 0
      while True:
        move = game.get_half_ply_pointer_record()
        if not move:
          if game.file_name_chart and game.url_chart:
            #self.main.root.after(500, self.main.live.process_analyze_queue) #blind
            self.main.root.after(500, self.main.live.next_polling) #tournament
          else:
            self.main.chart.create_chart("chart.png")
          return # we are done
        #print move.Start, move.Stop
        if move.Comment == '':
          break
        game.HalfPlyPointer += 1
      # first not analysed move
      while True:
        if game.HalfPlyPointer >= 6: break # get calculated best move value
        move.Comment = '0: None '
        game.HalfPlyPointer += 1
        move = game.get_half_ply_pointer_record()
        if not move:
          if game.file_name_pdn:
            self.main.save.file_save_game_clicked(game.file_name_pdn)
          if game.file_name_chart:
            self.main.chart.create_chart(game.file_name_chart)
          else:
            self.main.chart.create_chart("chart.png")
          if game.file_name_chart and game.url_chart:
            self.main.play.beam_up_chart(game.file_name_chart, game.url_chart)
            #self.main.root.after(500, self.main.live.process_analyze_queue) #blind
            self.main.root.after(500, self.main.live.next_polling) #tournament
          return # we are done

      #self.main.play.beam_up_chart('/home/feike/Horizon/LiveDraughts/wait.png')
      game.HalfPlyPointer += 1 # has to point to the next move
      print "start analysing with half ply =", game.HalfPlyPointer
      if game.build_till_pointer()== False:
        print "build till pointer is False"
        if game.file_name_pdn:
          self.main.save.file_save_game_clicked(game.file_name_pdn)
        if game.file_name_chart:
          self.main.chart.create_chart_with_illegal_move(game.file_name_chart)
        #self.main.root.after(500, self.main.live.process_analyze_queue) #blind
        self.main.root.after(500, self.main.live.next_polling) #tournament
        return
      game.refresh_display()
      self.main.play.startup_engine()
      self.main.play.engine.send_get_next_move(game)
      return
