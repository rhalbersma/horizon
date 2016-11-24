#
#  This file is Copyright (C) 2010 Feike Boomstra.
#  Distributed under the Boost Software License, Version 1.0.
#  (See accompanying file LICENSE_1_0.txt or copy at
#  http://www.boost.org/LICENSE_1_0.txt)
#


from Tkinter import *
from DraughtConstants import *
from DraughtPosition import *
from DraughtPlay import *

dc = DraughtConstants()


class ComputerPlayer:
    """ This class implements the draught playing computer,
        The real processor/memory bound parts will be written
        in C (or maybe even assembler). This class also the
        C interface.
        There are two parts in the playing:
            thinking in your own time (compute_foreground_best_move(...)
            thinking in the opponents time (compute_background_best_move(...)
        Both processes have to call back on a regular base, whether they can continue
    """
    def __init__(self):
##        self.play_process_state=play_process_state
##        generate_move.set_call_back(self.call_back) # initialize the call back entry in C
        return #  __init__(self
    
    def compute_foreground_best_move(self, game, played_move, time):
        # self.timer_id = self.root.after(1000*time, self.timer_routine)
        self.best_move = generate_move.foreground_c_best_move(self.build_interface_string(game, played_move))
        # self.root.after_cancel(self.timer_id)
        return self.best_move # compute_foreground_best_move(...)

    def analyze_position(self, game, played_move, time):
        # self.timer_id = self.root.after(1000*time, self.timer_routine)
        self.best_move = generate_move.analyse_position(self.build_interface_string(game, played_move))
        # self.root.after_cancel(self.timer_id)
        return self.best_move
        
    def evaluate_position(self, game, played_move, time):
        # self.timer_id = self.root.after(1000*time, self.timer_routine)
        self.best_move = generate_move.evaluate_position(self.build_interface_string(game, played_move))
        # self.root.after_cancel(self.timer_id)
        return self.best_move
        
    def analyze_timing(self, game, played_move, time):
        # self.timer_id = self.root.after(1000*time, self.timer_routine)
        self.best_move = generate_move.analyse_timing(self.build_interface_string(game, played_move))
        # self.root.after_cancel(self.timer_id)
        return self.best_move
        

    def build_interface_string(self, game, played_move):
        if game.is_color_white():
            s = chr(ord('(')+1)
        else:
            s = chr(ord('(')) 
        s = s + played_move
        for i in game.CurrentPosition.WhiteMan:
            s = s + chr(ord('(')+i)
        s = s + '('
        for i in game.CurrentPosition.WhiteKnight:
            s = s + chr(ord('(')+i)
        s = s + '('
        for i in game.CurrentPosition.BlackMan:
            s = s + chr(ord('(')+i)
        s = s + '('
        for i in game.CurrentPosition.BlackKnight:
            s = s + chr(ord('(')+i)
        s = s + '('
        # print "send = ", s
        return s

##    def call_back(self, nr):
##        self.root.update()
##        # print "call back: ", self.play_process_state.get()
##        if self.play_process_state.get()=="stop":            
##            return 0   # return False
##        elif self.play_process_state.get()=="exit":
##            return 0
##        elif self.play_process_state.get()=="move":
##            self.play_process_state.set("play")
##            return 0
##        elif self.play_process_state.get()=="play":
##            return 1   # return True
##        else:
##            print "?? call back: ", self.play_process_state.get()
##            return 0
##        
##    def compute_background_best_move(self, color):
##        generate_move.background_c_best_move()
##        return
##
##    def timer_routine(self):
##        # self.play_process_state.set("move")
##        return
    
    def c_init_tree(self):
        generate_move.init_c_best_move()
        return
    
    