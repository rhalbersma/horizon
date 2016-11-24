#
#  This file is Copyright (C) 2010 Feike Boomstra.
#  Distributed under the Boost Software License, Version 1.0.
#  (See accompanying file LICENSE_1_0.txt or copy at
#  http://www.boost.org/LICENSE_1_0.txt)
#


"""User Interface for obtaining timing game information"""

from Tkinter import *
from DraughtConstants import *
dc =DraughtConstants()         # get the global constants

class DraughtTimeSetPanel:


    def __init__(self, main, duration="20", handling="10", number_moves="75", root=None) :

        if root==None:
            root=Tk()
            root.geometry('300x400+350+300')
            self.main = main

        self.root = root
        self.root.title("Time Info")
        #self.main = main

        #self.root.option_add("*Font", "TSCu_Paranar 10")


        # frames
        self.frame_overall_frame          = Frame(self.root, relief = GROOVE, bd =4, height=300)


        # key bindings
        self.root.bind("<Escape>", self.esc_proc)
        self.root.protocol("WM_DELETE_WINDOW", self.end_proc)


        self.tx1 = Label(self.frame_overall_frame, text="Game Duration (in minutes)").grid(row=0,column=0)
        self.tx2 = Label(self.frame_overall_frame, text="Handling per move (in sec)").grid(row=1,column=0)
        self.tx3 = Label(self.frame_overall_frame, text="Number of Moves").grid(row=2,column=0)



        self.main_time = Entry(self.frame_overall_frame)
        self.main_time.grid(row=0,column=1)
        self.main_time.insert(END, duration)
        self.handling_time = Entry(self.frame_overall_frame,)
        self.handling_time.grid(row=1,column=1)
        self.handling_time.insert(END, handling)
        self.number_of_moves =Entry(self.frame_overall_frame)
        self.number_of_moves.grid(row=2,column=1)
        self.number_of_moves.insert(END, number_moves)

        #ok button
        self.ok_button = Button(self.frame_overall_frame, text="OK", command=self.main.Read_Time_Info)
        self.ok_button.grid(row=5, column=1)


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

class DraughtReconstructionPanel:
    def __init__(self, main, root=None, duration="20:00", number_moves="57"):
        if root==None:
            root=Tk()
        self.root = root
        self.top  = root
        self.root.title("Horizon v1.0,  recontruct board position")
        self.main = main

        # frames
        self.frame_overall_frame          = Frame(self.root, relief = GROOVE, bd =4, height=300)

        # key bindings
        self.root.bind("<Escape>", self.esc_proc)
        self.root.protocol("WM_DELETE_WINDOW", self.end_proc)

        self.tx1 = Label(self.frame_overall_frame, text="Remaining Game Duration (in minutes:seconds)").grid(row=0,column=0)
        #self.tx2 = Label(self.frame_overall_frame, text="Handling per move (in sec)").grid(row=1,column=0)
        self.tx3 = Label(self.frame_overall_frame, text="Remaining Number of Moves").grid(row=2,column=0)



        self.main_time = Entry(self.frame_overall_frame)
        self.main_time.grid(row=0,column=1)
        self.main_time.insert(END, duration)
#        self.handling_time = Entry(self.frame_overall_frame,)
#        self.handling_time.grid(row=1,column=1)
#        self.handling_time.insert(END, handling)
        self.number_of_moves =Entry(self.frame_overall_frame)
        self.number_of_moves.grid(row=2,column=1)
        self.number_of_moves.insert(END, number_moves)

        #ok button
        self.ok_button = Button(self.frame_overall_frame, text="OK", command=self.main.Read_Recontruct_Panel)
        self.ok_button.grid(row=5, column=1)


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


if __name__=='__main__':
    game_panel = DraughtTimeSetPanel()
    game_panel.root.mainloop()
