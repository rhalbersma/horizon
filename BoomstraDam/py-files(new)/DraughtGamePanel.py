#
#  This file is Copyright (C) 2010 Feike Boomstra.
#  Distributed under the Boost Software License, Version 1.0.
#  (See accompanying file LICENSE_1_0.txt or copy at
#  http://www.boost.org/LICENSE_1_0.txt)
#


"""User Interface for obtaining game information"""

from Tkinter import *
from DraughtConstants import *
dc =DraughtConstants()         # get the global constants

class DraughtGamePanel:

    
    def __init__(self, main, root=None) :

        if root==None:
            root=Tk()
            root.geometry('300x400+350+300')
            self.main = main

        self.root = root
        self.root.title("Game Info")
        #self.main = main
        
        #self.root.option_add("*Font", "TSCu_Paranar 10")
        
        
        # frames
        self.frame_overall_frame          = Frame(self.root, relief = GROOVE, bd =4, height=300)
        
        
        # key bindings
        self.root.bind("<Escape>", self.esc_proc)
        self.root.protocol("WM_DELETE_WINDOW", self.end_proc)
        
        
        self.tx1 = Label(self.frame_overall_frame, text="White Player").grid(row=0,column=0)
        self.tx2 = Label(self.frame_overall_frame, text="Black Player").grid(row=1,column=0)
        self.tx3 = Label(self.frame_overall_frame, text="Tournament").grid(row=2,column=0)
        self.tx4 = Label(self.frame_overall_frame, text="Round").grid(row=3,column=0)
        self.tx5 = Label(self.frame_overall_frame, text="Result").grid(row=4,column=0)
        
        

        self.white_player_text = Entry(self.frame_overall_frame)
        self.white_player_text.grid(row=0,column=1)
        self.white_player_text.insert(END, "Enter White Player")
        self.white_player_text.select_range(0,END)
        self.black_player_text = Entry(self.frame_overall_frame,)
        self.black_player_text.grid(row=1,column=1)
        self.black_player_text.insert(END, "Enter Black Player")
        self.tournament_text =Entry(self.frame_overall_frame)
        self.tournament_text.grid(row=2,column=1)
        self.tournament_text.insert(END, "Enter Tournament")
        self.round_num_text =Entry(self.frame_overall_frame)
        self.round_num_text.grid(row=3,column=1)
        self.round_num_text.insert(END, "Enter Round Number")
        
        # results list box
        self.result_list_box = Listbox(self.frame_overall_frame, selectmode=SINGLE)
        self.result_list_box.insert(END, "2-0")
        self.result_list_box.insert(END, "0-2")
        self.result_list_box.insert(END, "1-1")
        self.result_list_box.insert(END, "0-0")
        self.result_list_box.grid(row=4,column=1)
        
        #ok button
        self.ok_button = Button(self.frame_overall_frame, text="OK", command=self.main.Read_Game_Info)
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
    game_panel = DraughtGamePanel()
    game_panel.root.mainloop()
