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
        self.Game_1 = GameRecord(self.main,  None, None)
        self.Game_2 = GameRecord(self.main,  None, None)
        self.Game_3 = GameRecord(self.main,  None, None)
        game = self.main.ActiveGame
        self.main.play.startup_engine()
        game.CurrentPosition.ColorOnMove = self.main.panel.current_color_on_move.get()
        self.analpan = DraughtAnalyzePanel(self)
        self.analpan.top.title("Analyze (depth = " + str(wanted_depth).zfill(2) + ' )' )        
        self.CurrentMoveDepth = wanted_depth
        self.SearchDepth = self.CurrentMoveDepth
        self.analpan.board3.display(game.CurrentPosition)
        self.main.play.ProcesFixedDepth = self.proces_fixed_depth_move

        self.main.play.engine.send_analyse_position(game, wanted_depth)
        # obtained moves in "receive all moves"
        return
    
    def receive_all_moves(self, msg):
        #receive all moves (with move-value) in the given configuration
        #print "python: ", msg
        cnt = (len(msg) - 1)/14
        #print cnt, len(msg)
        self.all_moves = []
        for j in range(0, cnt):
            k=j*14+1
            next_move = HalfPlyRecord()
            next_move.Start = int(msg[k:k+2])
            next_move.op = msg[k+2:k+3]
            next_move.Stop = int(msg[k+3:k+5])
            next_move.MoveValue = int( msg[k+7:k+13])
            #print next_move
            self.all_moves.append(next_move)
            
        self.all_moves.sort(reverse=True)
        # moves are sorted in decreasing order by MoveValue
        self.analpan.all_moves.delete(first=0, last=self.analpan.all_moves.size()-1)
        for j in range(0, cnt):
            self.analpan.all_moves.insert(END, str(self.all_moves[j].Start).zfill(2) + self.all_moves[j].op + str(self.all_moves[j].Stop).zfill(2) +\
            ' (' + str(self.all_moves[j].MoveValue).rjust(6) + ')')
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
        self.ActiveEvalGame = self.Game_1
        self.Show_continue()
        return
    
    def Show_continue(self):
        selection = self.analpan.all_moves.curselection()
        if selection==(): 
            self.analpan.main_variant.delete(1.0, END)
            self.analpan.main_variant.insert(END, "please select a move first")
            return
        self.CurrentMoveDepth = self.SearchDepth
        self.MaxColor = self.analpan.board3.board_position.ColorOnMove
        self.ActiveEvalGame.clear()
        self.ActiveEvalGame.set_begin_position_ext(self.analpan.board3.board_position)
        move = self.all_moves[int(selection[0])]
        self.ActiveEvalGame.do_move(move)
        self.ActiveBoard.display(self.ActiveEvalGame.CurrentPosition)
        self.analpan.main_variant.delete(1.0, END)
        if self.MaxColor == dc.WHITE_ON_MOVE:
            self.analpan.main_variant.insert(END, \
                    str(move.Start).zfill(2) + move.op + str(move.Stop).zfill(2) + '(' + str(move.MoveValue).rjust(6) + ') ')
        else:
            self.analpan.main_variant.insert(END, '              ' + \
                    str(move.Start).zfill(2) + move.op + str(move.Stop).zfill(2) + '(' + str(move.MoveValue).rjust(6) + ')\n')
        if self.all_moves[int(selection[0])].op == '-':
            self.CurrentMoveDepth = self.CurrentMoveDepth - 1
        value = self.all_moves[int(selection[0])].MoveValue
        if self.CurrentMoveDepth > 0 :
            self.main.play.engine.send_get_fixed_depth_move(self.ActiveEvalGame , self.CurrentMoveDepth, \
                                                                                          value, self.analpan.get_orig_color(), False,True)
        return
        
        
    def proces_fixed_depth_move(self, s):
        game = self.ActiveEvalGame
        if s[1] == 'x': return
        start = int(s[1:3])
        stop = int(s[3:5])
        value = int(s[5:11])
        print "python(1): ", start, stop, value
        valid, move = game.start_stop_to_move_record(start, stop)
        if valid == False: return
        print "python(2)", move.Start, move.Stop
        #we have to decide whether we should stop, is different for MAX side and MIN side
        if self.MaxColor == self.ActiveBoard.board_position.ColorOnMove:
            # MAX node
            if self.CurrentMoveDepth < 0 and move.op == '-': 
                #we have collected the main variant, now get the evaluation
                self.main.play.engine.send_evaluate_position_2(game, False)            
                return # the end of the row
        else:
            if self.CurrentMoveDepth <= 0 and move.op == '-': 
                #we have collected the main variant, now get the evaluation
                self.main.play.engine.send_evaluate_position_2(game, False)            
                return # the end of the row
            
        game.do_move(move,True)
        game.write_moves()
#        self.analpan.main_variant.insert(END, str(move.Start).zfill(2) + move.op + str(move.Stop).zfill(2) + '(' + str(value).rjust(6) + ')')
#        if self.ActiveBoard.board_position.ColorOnMove == dc.WHITE_ON_MOVE:
#            self.analpan.main_variant.insert(END, ' ')
#        else:
#            self.analpan.main_variant.insert(END, '\n')
#        self.ActiveBoard.display(self.ActiveEvalGame.CurrentPosition)
#        if move.op == '-':
#            self.CurrentMoveDepth = self.CurrentMoveDepth - 1
#        self.main.play.engine.send_get_fixed_depth_move(self.ActiveEvalGame , self.CurrentMoveDepth, \
#                                                                                          value, self.analpan.get_orig_color(), False)
            
        return

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
        game.HalfPlyPointer = 14  # was 80
        self.send_next_move_for_timing()
        return
    
    def send_next_move_for_timing(self):
        game = self.main.ActiveGame
        game.HalfPlyPointer += 6
        if game.HalfPlyPointer <= 21: #game.get_half_ply_length(): 
            game.build_till_pointer()
            game.refresh_display()
            #print "python -- start next move"
            self.main.play.engine.send_analyse_timing(game)
        else:
            #print "python -- start final report"
            self.main.play.engine.send_final_report()
        return
    
    def final_report_ready(self):
        #print "python -- final report ready"
        self.main.panel.setup_mode()
        self.main.play.end_proc()
        return    
    
    def eval_mode_clicked(self, game, display=True):
        self.main.play.startup_engine()
        game.CurrentPosition.ColorOnMove = self.main.panel.current_color_on_move.get()
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

