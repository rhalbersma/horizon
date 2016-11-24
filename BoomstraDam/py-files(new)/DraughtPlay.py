#
#  This file is Copyright (C) 2010 Feike Boomstra.
#  Distributed under the Boost Software License, Version 1.0.
#  (See accompanying file LICENSE_1_0.txt or copy at
#  http://www.boost.org/LICENSE_1_0.txt)
#


import os
import subprocess
import time
import sys
import copy
from Tkinter import *
import tkMessageBox
from DraughtConstants import *
from MoveGenerator import *
from ComputerPlayer import *
from DraughtSetUp import *
from DraughtDisplayOnly import *
#import winsound
from DraughtSocketInterface import *
from DraughtEngineInterface import *
from DraughtSocketReader import *
import os
from datetime import *

dc = DraughtConstants()


class DraughtPlay:
    def __init__(self, main, panel):
        self.play_process_state=StringVar()                         # proces-state to share information between the various events
        self.main = main
        self.panel = panel
        self.root = self.panel.root
        self.mg = MoveGenerator()
        self.cp = ComputerPlayer()
        self.get_move_from_book = False
        self.EvalReceivedProc = None
        self.EvalReceivedContinueProc = None
        self.ContinuationMessage = ''
        self.ProcesFixedDepth = None
        return

    def create_variant(self, game):
        new = FloatingBoard(self.main, self.end_new_game, 0, game.ActivePanel.top)                
        new_game = GameRecord(self.main, new, self.main.panel, game)
        new_game.ActivePanel = new
        new_game.BeginPosition.WhiteMan = game.CurrentPosition.WhiteMan[:]
        new_game.BeginPosition.WhiteKing = game.CurrentPosition.WhiteKing[:]
        new_game.BeginPosition.BlackMan = game.CurrentPosition.BlackMan[:]
        new_game.BeginPosition.BlackKing = game.CurrentPosition.BlackKing[:]
        new_game.BeginPosition.ColorOnMove = game.CurrentPosition.ColorOnMove
        new_game.CurrentPosition.WhiteMan = new_game.BeginPosition.WhiteMan[:]
        new_game.CurrentPosition.WhiteKing = new_game.BeginPosition.WhiteKing[:]
        new_game.CurrentPosition.BlackMan = new_game.BeginPosition.BlackMan[:]
        new_game.CurrentPosition.BlackKing = new_game.BeginPosition.BlackKing[:]
        new_game.CurrentPosition.ColorOnMove = new_game.BeginPosition.ColorOnMove
        new_game.PreviousActiveGame = game
        new_game.PreviousHalfPlyPointer = game.HalfPlyPointer
        curr_half_ply = game.get_half_ply_pointer_record()
        curr_ply = game.get_half_ply_pointer_ply_record()
        new_game.CurrentPlyCounter = curr_ply.Ply_nr - 1
        curr_half_ply.Variant = new_game
        self.main.ActiveGameNumber += 1
        self.main.ActiveGame = new_game
        new_game.refresh_display()
        return new_game

    def startup_engine(self):
        if self.main.EngineStarted == False:
            # now the engine:
            #self.main.EngineID = os.spawnl(os.P_NOWAIT, "/home/feike/boomstradam/gnu-gcc/src/boomstradam")                    
            self.main.EngineID = os.spawnl(os.P_NOWAIT, "/home/feike/eclipse/workspace2/boomv2.4/Debug/BoomstraDam")                    
            self.engine = EngineSocket()
            if self.engine.flag == True:            
                self.engine.conn.setblocking(0) # set nonblocking
                print "Engine started"
                self.main.EngineStarted = True
                self.engine_socket_reader = SocketReader("python", self.main, self, self.engine.conn, self.dispatch_engine_messages, self.main.root)
                #self.socket.input_add(self.engine.conn, 0, self.dispatch_engine_messages)            
            else:
                print "can't start engine"
                self.main.root.quit()
        return
    
    def startup_remote(self):
        if self.main.RemoteSocketStarted == False:
            self.remote = DamExchange(True)
            self.main.RemoteSocketStarted = True
            self.remote.level1.conn.setblocking(0)
        #self.socket = SocketInterface(self.main.panel.root)
        self.remote_socket_reader = SocketReader("remote", self.main, self, self.remote.level1.conn, self.dispatch_remote_messages, self.main.root)
        return
    
    def remote_req_game(self):
        #ThinkingTime = 15
        #NumberOfPlies = 75
        if self.panel.current_white_player.get()==dc.REMOTE_PLAYER:
            ColorRemotePlayer = 'W'
        else: ColorRemotePlayer = 'Z'
        game =self.main.ActiveGame
        if game.is_standard_begin_position():
            BeginPositionID = 'A'
        else: BeginPositionID = 'B'
        if BeginPositionID == 'A':
            self.remote.send_game_req_message(ColorRemotePlayer, self.main.TotalEngineTime/60, self.main.TotalNumberOfMoves, BeginPositionID)
            return
        if  game.BeginPosition.ColorOnMove == dc.WHITE_ON_MOVE:
            ColorOnMove = 'W'
        else: ColorOnMove = 'Z'
        BeginPosition = game.get_50_sq_position()
        self.remote.send_game_req_message(ColorRemotePlayer, ThinkingTime, \
                                             NumberOfPlies, BeginPositionID, ColorOnMove, BeginPosition)        
        return
    
    def delayed_received_game_req_message(self):
        self.received_game_req_message(self.ContinuationMessage)
        return
    
    def received_game_req_message(self, msg):
        if self.EvalReceivedProc:
            self.EvalReceivedContinueProc = self.delayed_received_game_req_message
            self.ContinuationMessage = msg
            return
        else:
            self.EvalReceivedContinueProc = None
            self.ContinuationMessage = ''
            header = msg[0]
            version = msg[1:3]
            initiator = msg[3:35]
            color = msg[35]
            think = msg[36:39]
            numb = msg[39:42]
            posID = msg[42]
            if posID == 'B':
                color_on_move = msg[43]
                pos = msg[44:94]
            else:
                color_on_move = 'W'
                pos = 'zzzzzzzzzzzzzzzzzzzzeeeeeeeeeewwwwwwwwwwwwwwwwwwww'
            game = self.main.ActiveGame
            if len(game.Moves) > 0:
                game = GameRecord(self.main, self.main.panel, self.main.panel)
                self.main.PreviousActiveGame = self.main.ActiveGame
                self.main.ActiveGameNumber += 1
                self.main.ActiveGame =game
            
            game.WhitePlayer = 'dam2.2'
            game.BlackPlayer = 'BoomstraDam'
            game.Date = datetime.today().date() 
            game.Round = str(self.main.ActiveGameNumber + 1)
            
            if color_on_move == 'W':
                self.panel.current_color_on_move.set(dc.WHITE_ON_MOVE)
                game.BeginPosition.ColorOnMove = dc.WHITE_ON_MOVE
            else: 
                self.panel.current_color_on_move.set(dc.BLACK_ON_MOVE)
                game.BeginPosition.ColorOnMove = dc.BLACK_ON_MOVE
            if color ==  'Z':
                self.panel.current_white_player.set(dc.REMOTE_PLAYER)
                self.panel.current_black_player.set(dc.COMPUTER)
            else:
                self.panel.current_black_player.set(dc.REMOTE_PLAYER)
                self.panel.current_white_player.set(dc.COMPUTER)
            for i in range(1, 51):
                if pos[i-1] == 'w':
                    game.BeginPosition.WhiteMan.append(i)
                elif pos[i-1] == 'W':
                    game.BeginPosition.WhiteKing.append(i)
                elif pos[i-1] == 'z':
                    game.BeginPosition.BlackMan.append(i)
                elif pos[i-1] == 'Z':
                    game.BeginPosition.BlackKing.append(i)
            # copy begin -> current
            game.CurrentPosition.WhiteMan = game.BeginPosition.WhiteMan[:]
            game.CurrentPosition.WhiteKing = game.BeginPosition.WhiteKing[:]
            game.CurrentPosition.BlackMan = game.BeginPosition.BlackMan[:]
            game.CurrentPosition.BlackKing = game.BeginPosition.BlackKing[:]
            game.CurrentPosition.ColorOnMove = game.BeginPosition.ColorOnMove
            game.refresh_display()
            game.write_moves()
            self.panel.phase.set(dc.PLAY_MODE)
            self.startup_engine()
            self.remote.send_game_acc_message()
            print "Remote send acc message"
            self.engine.send_init_game(game, numb, think)
            self.get_move_from_book = True
            self.check_next_move()
        return 
    
    def check_next_move(self):
        # if "remote site" is on move, just wait, otherwise "next move".
        if self.panel.current_color_on_move.get()==dc.WHITE_ON_MOVE:
            if self.panel.current_white_player.get()==dc.REMOTE_PLAYER:
                pass
                print "wait for white remote player"
            else: self.next_move(self.main.ActiveGame)
        else:
            if self.panel.current_black_player.get()==dc.REMOTE_PLAYER:
                pass
                print "wait for black remote player"
            else: self.next_move(self.main.ActiveGame)                    
        return
    
    def dispatch_engine_messages(self, msg):
        #print "received:", msg[0]
        if msg[0] == 'N':
            if self.main.panel.is_play_mode():
                self.move_computed(msg)
        elif msg[0] == 'X':
            self.main.EngineStarted = False
        elif msg[0] == 'T':    
           self.main.anal.send_next_move_for_timing() 
        elif msg[0] == 'E': 
            print "End of Game (by Engine) received"
            self.engine_says_end_of_game() 
        elif msg[0] == 'F':    
            self.main.anal.final_report_ready() 
        elif msg[0] == 'M':
            self.ProcesFixedDepth(msg)
        elif msg[0] == 'S':
            print "python: Eval Received"
            self.eval_value_received(msg)
        elif msg[0] == 'L':
            print "python: Eval Parameters Received"
            self.main.anal.eval_par_received(msg)
        elif msg[0] == 'A':
            print "python: All Moves Received"
            self.main.anal.receive_all_moves(msg)
        else:
            print "Dispatch engine message -- unknown message: ", msg
        return
    
    def eval_value_received(self, msg):
        self.main.LastEval = int(msg[1:7])
        if self.EvalReceivedProc: self.EvalReceivedProc()
        if self.EvalReceivedContinueProc: self.EvalReceivedContinueProc() 
        return
        
    def engine_says_end_of_game(self):
        if self.main.RemoteSocketStarted == True:
            self.remote.send_end_of_game()
        self.main.panel.phase.set(dc.SETUP_MODE)                      # stop playing
        self.main.panel.setup_mode()
        #self.obtain_result()        
        return    

    def dispatch_remote_messages(self, msg):
        if msg[0] == 'R':
            print "Remote rec game req"
            self.received_game_req_message(msg)            
        elif msg[0] == 'A':
            # game acc, we have to decide whether further action is needed
            self.check_next_move()            
        elif msg[0] == 'M':
            # move
            self.proces_remote_move(msg)
        elif msg[0] == 'E':
            # game end
            print "Remote sent end game"
            self.main.panel.phase.set(dc.SETUP_MODE)                      # stop playing
            self.main.panel.setup_mode()
            self.obtain_result()
        elif msg[0] == 'B':
            # back req
            self.remote.send_back_acc_message()
        elif msg[0] == 'K':
            # back acc
            pass
        else:
            print "Dispatch remote message -- unknown message: ", msg
        return

    def obtain_result(self):
        # end-game from remote or engine
        self.EvalReceivedProc = self.GetResultGame
        self.main.anal.eval_mode_clicked(self.main.ActiveGame, False) # get eval, no printing
        return
        
    def  GetResultGame(self):
        self.EvalReceivedProc = None # clear link
        if self.main.ActiveGame.Result == '':
            if self.main.ActiveGame.CurrentPosition.ColorOnMove == dc.WHITE_ON_MOVE:
                game_result = '0-0'
                if self.main.LastEval <= -20000:
                    self.remote.RemoteWhiteResult += 0
                    self.remote.RemoteBlackResult  += 2
                    game_result = '0-2'
                elif self.main.LastEval <= 10 and self.main.LastEval >= -10:
                    self.remote.RemoteWhiteResult += 1
                    self.remote.RemoteBlackResult  += 1
                    game_result = '1-1'
                elif self.main.LastEval >= 20000:
                    self.remote.RemoteWhiteResult += 2
                    self.remote.RemoteBlackResult  += 0
                    game_result = '2-0'
            else:
                if self.main.LastEval >= 20000:
                    self.remote.RemoteWhiteResult += 0
                    self.remote.RemoteBlackResult  += 2
                    game_result = '0-2'
                elif self.main.LastEval <= 10 and self.main.LastEval >= -10:
                    self.remote.RemoteWhiteResult += 1
                    self.remote.RemoteBlackResult  += 1
                    game_result = '1-1'
                if self.main.LastEval <= -20000:
                    self.remote.RemoteWhiteResult += 2
                    self.remote.RemoteBlackResult  += 0
                    game_result = '2-0'
            result_str = str(self.remote.RemoteWhiteResult) + ' - ' + str(self.remote.RemoteBlackResult)
            self.panel.result_text.set(result_str)
            self.main.ActiveGame.Result = game_result
            #self.ResultCalculated = True
        return
        
    def remote_play_mode(self):
        # called to initiate special dam2.2 mode
        self.startup_remote()
        # wait for game req message
        return

    def play_mode(self):
        if self.main.ActiveGame == None:
            print "No active game"
            self.panel.phase.set(dc.SETUP_MODE)
            return
        game = self.main.ActiveGame
        # first we have to do some administration
        # is game.halfplypointer at the first free location in Moves??
        # if so, just (resume) appending the moves
        # if not, ask whether we have to overwrite the existing moves or
        # we have to create a variant.
        if game.CurrentPosition.ColorOnMove <> self.panel.current_color_on_move.get():
            game.CurrentPosition.ColorOnMove = self.panel.current_color_on_move.get()
            game.Moves = [] # changed colors, new start
            game.BeginPosition.BlackKing = game.CurrentPosition.BlackKing[:]
            game.BeginPosition.BlackMan = game.CurrentPosition.BlackMan[:]
            game.BeginPosition.ColorOnMove = game.CurrentPosition.ColorOnMove
            game.BeginPosition.WhiteKing = game.CurrentPosition.WhiteKing[:]
            game.BeginPosition.WhiteMan = game.CurrentPosition.WhiteMan[:]
            game.refresh_display()
            game.refresh_number_of_pieces_display()
            game.write_moves()        
        elif game.HalfPlyPointer == game.half_ply_length():
            if game.Moves == []:
                game.BeginPosition.ColorOnMove = self.panel.current_color_on_move.get()
        else:
            if tkMessageBox.askyesno("BoomstraDam Question", "Overwrite remaining moves") == True:
                to_be_removed = game.half_ply_length() - game.HalfPlyPointer
                print "Overwrite, to be removed = ", to_be_removed
                for k in range(0, to_be_removed):
                    game.remove_from_tail()
                game.refresh_display()
            else:
                game = self.create_variant(game)
                
        
        if self.panel.current_black_player.get()==dc.COMPUTER or \
           self.panel.current_white_player.get()==dc.COMPUTER:
            self.startup_engine()
            self.engine.send_init_game(game, self.main.TotalNumberOfMoves, self.main.TotalEngineTime/60)
        if self.panel.current_black_player.get()==dc.REMOTE_PLAYER and \
           self.panel.current_white_player.get()==dc.REMOTE_PLAYER:
            print "only one remote player !!"
            self.main.panel.setup_mode()
            return
        if self.panel.current_black_player.get()==dc.REMOTE_PLAYER or \
           self.panel.current_white_player.get()==dc.REMOTE_PLAYER:
            self.startup_remote()
            self.remote_req_game() # send req game message
            return  # and wait for acc message
            
        self.next_move(game)
        return
    
    def end_new_game(self, level):         # called when window is destroyed by user
        self.panel.setup_mode()
        self.main.ActiveGame = self.main.ActiveGame.PreviousActiveGame
        self.main.ActiveGame.refresh_display()
        self.main.ActiveGame.write_moves()
        return

    def end_proc(self):                                         # trigered from 'exit' and 'x
        if self.main.RemoteSocketStarted == True:
            self.remote.level1.conn.close()
        if self.main.EngineStarted == True:
            self.engine.send_close_engine()
            os.waitpid(self.main.EngineID, 0)
            try:
                print "python -- close socket"
                self.engine.conn.close()
            except socket.error, reason:
                code, msg = reason.args
                print msg                
            print "waiting for engine to stop"
        self.main.root.quit()
        return
    
    def stop_proc(self):                                        # triggered by esc & setup mode
        pass
        return

    def next_move(self, game, prev_move=None):
        if self.panel.current_color_on_move.get()==dc.WHITE_ON_MOVE:
            game.CurrentPosition.ColorOnMove = dc.WHITE_ON_MOVE
            if self.panel.current_white_player.get()==dc.COMPUTER:
                self.panel.current_player.set(dc.COMPUTER)
                self.computer_player(game, prev_move)
            elif self.panel.current_white_player.get()==dc.HUMAN_PLAYER:
                self.panel.current_player.set(dc.HUMAN_PLAYER)
                self.human_player(game, prev_move)
            elif self.panel.current_white_player.get()==dc.REMOTE_PLAYER:
                self.panel.current_player.set(dc.REMOTE_PLAYER)
                self.remote_player(prev_move)
            else: print "invalid current white player"
        elif self.panel.current_color_on_move.get()==dc.BLACK_ON_MOVE:
            game.CurrentPosition.ColorOnMove = dc.BLACK_ON_MOVE
            if self.panel.current_black_player.get()==dc.COMPUTER:
                self.computer_player(game, prev_move)
            elif self.panel.current_black_player.get()==dc.HUMAN_PLAYER:
                self.human_player(game, prev_move)
            elif self.panel.current_black_player.get()==dc.REMOTE_PLAYER:
                self.remote_player(prev_move)
            else: print "invalid current black player"
        else: print "invalid current color on move"            
        return # next_move

    def computer_player(self, game, move):
        # print "And now the computer"
        if len(self.main.ActiveGame.Moves) < 16 and self.get_move_from_book == True:
            self.engine.send_get_next_move_from_opening_book(game)
        else:
            self.engine.send_get_next_move(game)
        # req for move send, now wait for answer
        return
    
    
    def move_computed(self, s):
        game = self.main.ActiveGame
        if s[1] == 'x':
            if self.get_move_from_book == True:
                self.engine.send_get_next_move(game)
                self.get_move_from_book = False
                return
            else:
                if self.panel.current_color_on_move.get()==dc.WHITE_ON_MOVE:
                    self.panel.status_display.set("Black Wins!")
                else: self.panel.status_display.set("White Wins!")
                self.main.panel.setup_mode()
                return

        # print "move_computed -- len(s) = ", len(s), "s = ", s
        start = int(s[1:3])
        stop = int(s[3:5])
        result = int(s[5:11])
        nr_of_caps = int(s[11:13])
        caps = []
        k = 13
        while len(s) >= k+2:
            cap = int(s[k:k+2])
            caps.append(cap)
            k = k + 2
        
        move, valid, unique = self.get_valid_move(game, start, stop, caps, nr_of_caps)
        if valid == True:
            move.Unique = unique
            move.Comment = str(result)
            game.do_move(move, True)
            game.refresh_display()
            game.write_moves()
            self.make_sound_alert()
            self.switch_color_and_next_move(game, move)
        return
    
    def get_valid_move(self, game, start, stop, caps, nr_of_caps):
        ml = self.main.play.mg.valid_move(game)                                     # get all valid moves, maybe we have a capture.
        for move in ml:
            if nr_of_caps == 0: # moves from the book don't provide caps
                if move.Start == start and move.Stop == stop:
                    break
            else:
                if move.Start == start and move.Stop == stop and set(caps) == set(move.Cap):
                    break
        if not (move.Start == start and move.Stop == stop): 
            print "Mismatch"
            print "wanted : ", start , stop
            print "got :"
            for move in ml: print move
            return None, False, False
        else:
            save_move = move
            move_count = 0
            for move in ml:
                if move.Start == start and move.Stop == stop:
                    move_count += 1
            return save_move, True, (move_count == 1)

    def make_sound_alert(self):
        if self.panel.sound_mode == dc.SOUND_ON:
            #sound=open("/dev/dsp",'w')
            subprocess.Popen(["totem", "--play", "/home/feike/BoomstraDam/images/carhorn.au"])
            #time.sleep(1)
            #sound.close()
            pass
        elif self.panel.sound_mode == dc.SOUND_OFF:
            pass
        elif self.panel.sound_mode == dc.SOUND_TWO:
            #sound=open("/dev/dsp",'w')
            subprocess.Popen(["totem", "--play", "/home/feike/BoomstraDam/images/fred-family_feud_bell.au"])
            #time.sleep(1)
            #sound.close()
            pass
        return
    
    def switch_color_and_next_move(self, game, prev_move):
        if self.main.ActiveGame.CurrentPosition.ColorOnMove == dc.WHITE_ON_MOVE:
            self.panel.current_color_on_move.set(dc.WHITE_ON_MOVE)
            self.panel.status_display.set("White on Move!")
        else:
            self.panel.current_color_on_move.set(dc.BLACK_ON_MOVE)
            self.panel.status_display.set("Black on Move!")
        self.next_move(game, prev_move)
        return
    
    def remote_player(self, prev_move):
        if prev_move == None:
            pass    # wait for the first move to arrive
        else:
           self.remote.send_move(prev_move.Start, prev_move.Stop, prev_move.Cap) 
            
        return
    
    def proces_remote_move(self, s):
        start = int(s[5:7])
        stop = int(s[7:9])
        nr_of_caps = int(s[9:11])
        caps = []
        k = 11
        while len(s) >= k+2:
            cap = int(s[k:k+2])
            caps.append(cap)
            k = k + 2
        game =self.main.ActiveGame    
        ml = self.mg.valid_move(game)                                     # get all valid moves, maybe we have a capture.
        for move in ml:
            if nr_of_caps == 0: # moves from the book don't provide caps
                if move.Start == start and move.Stop == stop:
                    break
            else:
                if move.Start == start and move.Stop == stop and set(caps) == set(move.Cap):
                    break
        if not (move.Start == start and move.Stop == stop): 
            print "Mismatch"
            print "wanted : ", start , stop
            #print "got :"
            #for move in ml: print move;
            self.main.panel.setup_mode()
            game.ActivePanel.root.update()
            return
        # no invalid neither impossible moves          
        # print "--------------------------------", start, stop
        game.do_move(move, True)
        game.refresh_display()
        game.write_moves()
        self.make_sound_alert()
        self.switch_color_and_next_move(game, move)
        return

    def human_player(self, game, prev_move):
        # create the list of valid moves
        self.move_list = self.mg.valid_move(game)
        # self.debug_print("human")
        if self.is_game_finished():
            self.main.panel.setup_mode()
            return
        if self.one_move_and_auto_capture(game):
            game.refresh_display()
            game.write_moves()
            self.switch_color_and_next_move(game, self.move_list[0])
            return                                                  # we are done for this move
        else:
            self.field_selection_list=[]                            # prepare for receiving the human players move
        return # human player
    
    def one_move_and_auto_capture(self, game):
        if self.panel.auto_move.get()==1:                                                    # Auto Capture?
            if len(self.move_list)==1:                                          # only one move?
                game.do_move(self.move_list[0], True)                                                 # do the move
                return True
            elif len(self.move_list)>1:
                if self.multiple_moves_same_effect():                           # moves with different 'in between' landing points for kings ??
                    game.do_move(self.move_list[0], True)                                              
                    return True
            else: return False
        else: return False
    
    def is_game_finished(self):
        if len(self.move_list)==0:
            if self.panel.current_color_on_move.get()==dc.WHITE_ON_MOVE:
                self.panel.status_display.set("Black Wins!")
            else: self.panel.status_display.set("White Wins!")
            return True
        else: return False
        return

    def multiple_moves_same_effect(self):
        for i in range(1, len(self.move_list)): 
            # print self.move_list[i].start,   self.move_list[i].stop,  set(self.move_list[i].cap)                             # for all possible moves
            if not (self.move_list[0].Start == self.move_list[i].Start and \
                    self.move_list[0].Stop  == self.move_list[i].Stop  and \
                set(self.move_list[0].Cap)  == set(self.move_list[i].Cap)):
                return False                                                  # not the same
        return True                                                             # all the same start, stop, captured
    
#-----------------------------------------------------------------------------------
#       now the asynchronise part
#-----------------------------------------------------------------------------------

    def move_proposal(self, idx, game):                                   # entry for clicking a field in play-mode
        # print "move proposal", self.panel.current_color_on_move.get()
        self.field_selection_list.append(idx)
        # print self.field_selection_list
        count=0
        # for move in self.move_list: print move
        # self.debug_print("move proposal")
        for i in range(0,len(self.move_list)):
            if self.list_in_list(self.field_selection_list, self.move_list[i].Step):       # the list structure: [start, end, captured, [start, .. , .. , .. , stop]]
                count=count+1
                cur_pos=i
        # print "count=", count
        if count == 1:
            game.do_move(self.move_list[cur_pos])                   # exactly one possible move, so execute
            game.refresh_display()
            game.write_moves()
            self.switch_color_and_next_move(game, self.move_list[cur_pos])
            return
        elif count == 0:
            self.de_select_all_fields(game)                                         # invalid field
            self.field_selection_list[:]=[]                                     # clear all selected fields
            return
        elif self.multiple_moves_same_effect():                                 # moves with different 'in between' landing points for kings ??
            game.do_move(self.move_list[cur_pos])                   # exactly one possible move, so execute
            game.refresh_display()
            game.write_moves()
            self.switch_color_and_next_move(game, self.move_list[cur_pos])
            return
        else:
            self.toggle_selection(idx, game)
        return

    def list_in_list(self, list1, list2):
        for i in range(0,len(list1)):
            if list1[i] in list2:
                pass
            else: return False
        return True
    
    def de_select_all_fields(self, game):
        for piece in self.field_selection_list:
            if piece in game.CurrentPosition.WhiteMan: game.ActivePanel.set_white_man(piece, False)
            if piece in game.CurrentPosition.BlackMan: game.ActivePanel.set_black_man(piece, False)
            if piece in game.CurrentPosition.WhiteKing: game.ActivePanel.set_white_king(piece, False)
            if piece in game.CurrentPosition.BlackKing: game.ActivePanel.set_black_king(piece, False)
            if piece in game.CurrentPosition.get_empty_fields(): game.ActivePanel.set_empty_field(piece, False)
        self.field_selection_list = []
        return
    
    def toggle_selection(self, idx, game):        
        if self.field_selection_list.count(idx) == 2:              # two times == no times
            self.field_selection_list.remove(idx)
            self.field_selection_list.remove(idx)            
            if idx in game.CurrentPosition.WhiteMan: game.ActivePanel.set_white_man(idx, False)
            if idx in game.CurrentPosition.BlackMan: game.ActivePanel.set_black_man(idx, False)
            if idx in game.CurrentPosition.WhiteKing: game.ActivePanel.set_white_king(idx, False)
            if idx in game.CurrentPosition.BlackKing: game.ActivePanel.set_black_king(idx, False)
            if idx in game.CurrentPosition.get_empty_fields(): game.ActivePanel.set_empty_field(idx, False)        
        else:
            if idx in game.CurrentPosition.WhiteMan: game.ActivePanel.set_white_man(idx, True)
            if idx in game.CurrentPosition.BlackMan: game.ActivePanel.set_black_man(idx, True)
            if idx in game.CurrentPosition.WhiteKing: game.ActivePanel.set_white_king(idx, True)
            if idx in game.CurrentPosition.BlackKing: game.ActivePanel.set_black_king(idx, True)
            if idx in game.CurrentPosition.get_empty_fields(): game.ActivePanel.set_empty_field(idx, True)        
        return


    
