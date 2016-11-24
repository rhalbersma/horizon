#
#  This file is Copyright (C) 2010 Feike Boomstra.
#  Distributed under the Boost Software License, Version 1.0.
#  (See accompanying file LICENSE_1_0.txt or copy at
#  http://www.boost.org/LICENSE_1_0.txt)
#


"""User Interface for Analysing stand"""

from Tkinter import *
import tkFont
from DraughtBoard import *
from DraughtDataStructures import *
from DraughtEvalDisplay import *
from DraughtConstants import *
dc =DraughtConstants()         # get the global constants

class DraughtAnalyzePanel:

    HEIGHT=23               # height of a square on the board
    WIDTH=23                # width of a square on the board
    
    def __init__(self, anal) :
        self.anal = anal
        self.top = Toplevel()
        self.top.geometry('1600x1100')
        self.top.title("Analyze")        
        #self.top.option_add("*Font", "TSCu_Paranar 10")
        self.top.option_add("*Font", "Helvetica 10")
        self.myBoldFont = tkFont.Font(family="helvetica", weight="bold", size=10)
        self.myFont = tkFont.Font(family="helvetica", size=10)
        
        # key bindings
        self.top.bind("<Escape>", self.esc_proc)
        self.top.protocol("WM_DELETE_WINDOW", self.end_proc)

        # frames
        self.overall_frame          = Frame(self.top, relief = GROOVE, bd =4)        
        self.board1_frame         = Frame(self.overall_frame)
        self.board2_frame         = Frame(self.overall_frame)
        self.board3_frame         = Frame(self.overall_frame)
        self.eval1_frame            = Frame(self.overall_frame, relief=RIDGE, bd=3)
        self.eval2_frame            = Frame(self.overall_frame, relief=RIDGE, bd=3)
        self.eval3_mv_frame      = Frame(self.overall_frame, relief=RIDGE, bd=3)
        self.eval3_am_frame      = Frame(self.overall_frame, relief=RIDGE, bd=3)
        self.eval3_bt_frame       = Frame(self.overall_frame, relief=RIDGE, bd=3)
        
        self.board1                    = DraughtBoardView(self.board1_frame)
        self.board2                    = DraughtBoardView(self.board2_frame)
        self.board3                    = DraughtBoardView(self.board3_frame)
        
        self.eval1_par                = DraughtEval(self.eval1_frame)
        self.eval2_par                = DraughtEval(self.eval2_frame)
        
        self.main_variant           = Text(self.eval3_mv_frame, font=("Courier",10), height=20, width=43)
        self.main_variant.grid()
        self.main_variant.insert(END,"waiting for selection\n")
        self.notation = self.main_variant
        
        self.scrollbar                  = Scrollbar(self.eval3_am_frame, orient=VERTICAL)        
        self.all_moves                = Listbox(self.eval3_am_frame, selectmode=SINGLE, yscrollcommand=self.scrollbar.set, height = 20, width=14,\
                                                             font = ("Courier",12))
        self.scrollbar.config(command=self.all_moves.yview)
        self.scrollbar.pack(side=RIGHT, fill=Y)
        self.all_moves.insert(END, "please wait, collecting moves")
        self.all_moves.pack(side=LEFT, fill=BOTH, expand=1)
    
        self.show1 = Button(self.eval3_bt_frame, text="Show1", command=self.anal.show1_pressed)
        self.show2 = Button(self.eval3_bt_frame, text="Show2", command=self.anal.show2_pressed)
        self.show1.grid(row=0, column=0)
        self.show2.grid(row=0, column=1)
        
        self.board1_frame.grid(row=0,column=0)        
        self.board2_frame.grid(row=0,column=1)
        self.board3_frame.grid(row=0,column=2)
        
        self.eval1_frame.grid(row=1,column=0, rowspan=2,sticky=E)
        self.eval2_frame.grid(row=1,column=1, rowspan=2,sticky=E)        
        self.eval3_mv_frame.grid(row=1,column=2)
        self.eval3_am_frame.grid(row=2,column=2)
        self.eval3_bt_frame.grid(row=3, column=2)
        self.overall_frame.grid()
        return # __init__
    
    def get_orig_color(self):
        if self.board3.board_position.ColorOnMove == dc.WHITE_ON_MOVE:
            return 'W'
        else:
            return 'Z'
    
    # -------------------------  event handlers --------------------------------- #

    def esc_proc(self, event):
        self.top.destroy()
        return
    
    def end_proc(self):                                 # called from 'exit' and 'x'       
        self.top.destroy()
        return

        
if __name__=='__main__':        
    position = DraughtBoard()
    position.WhiteMan = range(31,51)
    position.BlackMan = range(1,17)
    s = 'E'
    for i in range(0,58):
        s = s + str(i).rjust(5)
    panel =  DraughtAnalyzePanel()
    top.mainloop()          
