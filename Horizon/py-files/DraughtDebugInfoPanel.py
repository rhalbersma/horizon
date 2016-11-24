#
#  This file is Copyright (C) 2010 Feike Boomstra.
#  Distributed under the Boost Software License, Version 1.0.
#  (See accompanying file LICENSE_1_0.txt or copy at
#  http://www.boost.org/LICENSE_1_0.txt)
#


"""User Interface for obtaining walk hash information"""

from Tkinter import *
from DraughtConstants import *
dc =DraughtConstants()         # get the global constants

class DraughtDebugInfoPanel:

    
    def __init__(self, walk, default_depth,  default_threads,  hash_init) :

        self.walk=walk

        self.root = Tk()
        self.root.title("Walk Through Hash Info")
        #self.main = main
        
        #self.root.option_add("*Font", "TSCu_Paranar 10")
        
        #variables
        self.Init_Hash = IntVar()
        
        # frames
        self.frame_overall_frame          = Frame(self.root, relief = GROOVE, bd =4, height=300)
        
        
        # key bindings
        self.root.bind("<Escape>", self.esc_proc)
        self.root.protocol("WM_DELETE_WINDOW", self.end_proc)
        
        
        self.tx1 = Label(self.frame_overall_frame, text="Search Depth").grid(row=0,column=0)
        self.tx2 = Label(self.frame_overall_frame, text="Number of Threads").grid(row=1,column=0)
        self.chk = Checkbutton(self.frame_overall_frame, text="Init Hash",  variable=self.Init_Hash).grid(row=3,column=0)

        #preset
        self.Init_Hash.set(1)
        
        self.search_depth_text = Entry(self.frame_overall_frame)
        self.search_depth_text.grid(row=0,column=1)
        self.search_depth_text.insert(END, str(default_depth))
        self.search_depth_text.select_range(0,END)
        self.num_threads_text = Entry(self.frame_overall_frame,)
        self.num_threads_text.grid(row=1,column=1)
        self.num_threads_text.insert(END, str(default_threads))
        
        #ok button
        self.ok_button = Button(self.frame_overall_frame, text="OK", command=self.walk.Read_Walk_Info)
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

if __name__=='__main__':        
    game_panel = DraughtGamePanel()
    game_panel.root.mainloop()
