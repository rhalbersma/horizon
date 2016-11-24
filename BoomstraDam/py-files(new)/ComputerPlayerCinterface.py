#
#  This file is Copyright (C) 2010 Feike Boomstra.
#  Distributed under the Boost Software License, Version 1.0.
#  (See accompanying file LICENSE_1_0.txt or copy at
#  http://www.boost.org/LICENSE_1_0.txt)
#


from MoveGenerator import *
from BitVector import *

class ComputerPlayerCinterface:
    def __init__(self, root, call_back, WHITE_ON_MOVE):
        self.root=root
        self.call_back=call_back
        self.movegenerator=MoveGenerator(WHITE_ON_MOVE)
        self.cur_cap_path=BitVector(size=64)
        self.move_list=[]
        return
    
    def forground_c_best_move(self, color, time, white_man, white_knight, black_man, black_knight):
        # print "computer plays with ", color.get()
        # self.root.after(500)
#     def valid_move(self, color, current_cap_path, own_color_man, own_color_knight, other_color_man, other_color_knight):

        self.move_list=self.movegenerator.valid_move(color, self.cur_cap_path, white_man, white_knight, black_man, black_knight)
        if len(self.move_list) > 1:
            self.move_list=self.move_list[0]
        return self.move_list
        while True:
            if self.call_back()==False:
                # print "Foreground aborted"
                return
            else:
                # print "Foreground Continue"
                self.root.after(500)
                time=time-1
                if time==0:            
                    return
        # end while
    
    def background_py_best_move(self, color):
        while True:
            # print "background processing"
            self.root.after(400)
            if self.call_back()==False:
                # print "Background aborted"
                break
        #end while
        return
    