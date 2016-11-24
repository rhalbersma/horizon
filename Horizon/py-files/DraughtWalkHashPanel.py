#
#  This file is Copyright (C) 2010 Feike Boomstra.
#  Distributed under the Boost Software License, Version 1.0.
#  (See accompanying file LICENSE_1_0.txt or copy at
#  http://www.boost.org/LICENSE_1_0.txt)
#


"""User Interface for walking though the hash"""

from Tkinter import *
import tkFont
from DraughtBoard import *
from DraughtDataStructures import *
from DraughtConstants import *
dc =DraughtConstants()         # get the global constants

class DraughtWalkHashPanel:

    HEIGHT=23               # height of a square on the board
    WIDTH=23                # width of a square on the board

    def __init__(self, walk) :
        self.walk = walk
        self.top = Toplevel()
        self.top.geometry('400x1100')
        self.top.title("Hash View")
        #self.top.option_add("*Font", "TSCu_Paranar 10")
        self.top.option_add("*Font", "Helvetica 10")
        self.myBoldFont = tkFont.Font(family="helvetica", weight="bold", size=10)
        self.myFont = tkFont.Font(family="helvetica", size=10)

        # key bindings
        self.top.bind("<Escape>", self.esc_proc)
        self.top.protocol("WM_DELETE_WINDOW", self.end_proc)

        # the string variables for the onfo bar
        self.LowerBound = StringVar()
        self.UpperBound = StringVar()
        self.Node = StringVar()
        self.Color = StringVar()
        self.IsEvaluated = StringVar()
        self.Useless = StringVar()
        self.ExactValue = StringVar()
        self.CyclePossible = StringVar()
        self.JustPromoted = StringVar()
        self.IsExtended = StringVar()
        self.HasExtCondition = StringVar()
        self.SearchDepth = StringVar()
        self.DistanceFromRoot = StringVar()
        self.BestMoveIndex = StringVar()
        self.IsQuiescence = StringVar()
        self.HasToCapture = StringVar()
        self.MoveStatus = StringVar()
        self.Depth = StringVar()

        # frames
        self.overall_frame          = Frame(self.top, relief = GROOVE, bd =4)
        self.info_frame              = Frame(self.overall_frame, relief=RIDGE, bd=3)
        self.board3_frame         = Frame(self.overall_frame)
        self.eval3_am_frame      = Frame(self.overall_frame, relief=RIDGE, bd=3)
        self.eval3_bt_frame       = Frame(self.overall_frame, relief=RIDGE, bd=3)

        #the info frame

        self.ilbl = Label(self.info_frame, text="LowerBound").grid(row=0,column=0, sticky=E)
        self.ilb = Label(self.info_frame, textvariable=self.LowerBound, width=6, anchor=W)
        self.ilb.config(font=self.myFont)
        self.ilb.grid(row=0,column=1, sticky=E)
        self.iubl = Label(self.info_frame, text="Uppder Bound").grid(row=1,column=0, sticky=E)
        self.iub = Label(self.info_frame, textvariable=self.UpperBound, width=6, anchor=W)
        self.iub.config(font=self.myFont)
        self.iub.grid(row=1,column=1, sticky=E)
        self.irsl = Label(self.info_frame, text="Node").grid(row=2,column=0, sticky=E)
        self.irs = Label(self.info_frame, textvariable=self.Node, width=6, anchor=W)
        self.irs.config(font=self.myFont)
        self.irs.grid(row=2,column=1, sticky=E)
        self.ievl = Label(self.info_frame, text="Color").grid(row=3,column=0, sticky=E)
        self.iev = Label(self.info_frame, textvariable=self.Color, width=6, anchor=W)
        self.iev.config(font=self.myFont)
        self.iev.grid(row=3,column=1, sticky=E)
        self.iewl = Label(self.info_frame, text="Search Depth").grid(row=4,column=0, sticky=E)
        self.iew = Label(self.info_frame, textvariable=self.SearchDepth, width=6, anchor=W)
        self.iew.config(font=self.myFont)
        self.iew.grid(row=4,column=1, sticky=E)
        self.iezl = Label(self.info_frame, text="Distance").grid(row=5,column=0, sticky=E)
        self.iez = Label(self.info_frame, textvariable=self.DistanceFromRoot, width=6, anchor=W)
        self.iez.config(font=self.myFont)
        self.iez.grid(row=5,column=1, sticky=E)
        self.iexl = Label(self.info_frame, text="Best Move").grid(row=6,column=0, sticky=E)
        self.iex = Label(self.info_frame, textvariable=self.BestMoveIndex, width=6, anchor=W)
        self.iex.config(font=self.myFont)
        self.iex.grid(row=6,column=1, sticky=E)
        self.ieyl = Label(self.info_frame, text="Depth").grid(row=7,column=0, sticky=E)
        self.iey = Label(self.info_frame, textvariable=self.Depth, width=6, anchor=W)
        self.iey.config(font=self.myFont)
        self.iey.grid(row=7,column=1, sticky=E)
        self.idt = Label(self.info_frame, textvariable=self.IsEvaluated, width=20, anchor=W)
        self.idt.config(font=self.myFont)
        self.idt.grid(row=0,column=3, sticky=E)
        self.irn = Label(self.info_frame, textvariable=self.Useless, width=20, anchor=W)
        self.irn.config(font=self.myFont)
        self.irn.grid(row=1,column=3, sticky=E)
        self.irm = Label(self.info_frame, textvariable=self.ExactValue, width=20, anchor=W)
        self.irm.config(font=self.myFont)
        self.irm.grid(row=2,column=3, sticky=E)
        self.irp = Label(self.info_frame, textvariable=self.CyclePossible, width=20, anchor=W)
        self.irp.config(font=self.myFont)
        self.irp.grid(row=3,column=3, sticky=E)
        self.irq = Label(self.info_frame, textvariable=self.JustPromoted, width=20, anchor=W)
        self.irq.config(font=self.myFont)
        self.irq.grid(row=4,column=3, sticky=E)
        self.irr = Label(self.info_frame, textvariable=self.IsExtended, width=20, anchor=W)
        self.irr.config(font=self.myFont)
        self.irr.grid(row=5,column=3, sticky=E)
        self.irs = Label(self.info_frame, textvariable=self.HasExtCondition, width=20, anchor=W)
        self.irs.config(font=self.myFont)
        self.irs.grid(row=6,column=3, sticky=E)
        self.irt = Label(self.info_frame, textvariable=self.IsQuiescence, width=20, anchor=W)
        self.irt.config(font=self.myFont)
        self.irt.grid(row=7,column=3, sticky=E)
        self.iru = Label(self.info_frame, textvariable=self.HasToCapture, width=20, anchor=W)
        self.iru.config(font=self.myFont)
        self.iru.grid(row=8,column=3, sticky=E)
        self.irw = Label(self.info_frame, textvariable=self.MoveStatus, width=20, anchor=W)
        self.irw.config(font=self.myFont)
        self.irw.grid(row=9,column=3, sticky=E)


        self.board3                    = DraughtBoardView(self.board3_frame)



        self.scrollbar                  = Scrollbar(self.eval3_am_frame, orient=VERTICAL)
        self.all_moves                = Listbox(self.eval3_am_frame, selectmode=SINGLE, yscrollcommand=self.scrollbar.set, height = 20, width=34,\
                                                             font = ("Courier",12))
        self.scrollbar.config(command=self.all_moves.yview)
        self.scrollbar.pack(side=RIGHT, fill=Y)
        self.all_moves.insert(END, "please wait, collecting moves")
        self.all_moves.pack(side=LEFT, fill=BOTH, expand=1)

        self.show1 = Button(self.eval3_bt_frame, text="Home", command=self.walk.show_home_node_pressed)
        self.show2 = Button(self.eval3_bt_frame, text="Prev", command=self.walk.show_prev_node_pressed)
        self.show3 = Button(self.eval3_bt_frame, text="Next", command=self.walk.show_next_node_pressed)
        self.show5 = Button(self.eval3_bt_frame, text="Follow", command=self.walk.show_follow_pressed)
        self.show4 = Button(self.eval3_bt_frame, text="Eval", command=self.walk.show_eval_node_pressed)

        self.show1.grid(row=0, column=0)
        self.show2.grid(row=0, column=1)
        self.show3.grid(row=0, column=2)
        self.show5.grid(row=0, column=3)
        self.show4.grid(row=0, column=4)

        self.info_frame.grid(row=0, column=0,  sticky=W)
        self.board3_frame.grid(row=1,column=0)
        self.eval3_am_frame.grid(row=2,column=0)
        self.eval3_bt_frame.grid(row=3, column=0)
        self.overall_frame.grid()
        return # __init__


    # -------------------------  event handlers --------------------------------- #

    def esc_proc(self, event):
        self.top.destroy()
        return

    def end_proc(self):                                 # called from 'exit' and 'x'
        self.top.destroy()
        return


