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
        self.white_player_text.focus_set()
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
        self.bind("&lt;Return>", self.main.Read_Game_Info)
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



"""user interface for single move collection """
class Read_Move_Info:
    def __init__(self, main, game):
      self.game = game
      self.main = main
      return
    def __call__(self):
      self.main.save.Read_Move_Info(self.game)
      return


class DraughtSingleMoveInfo:
    def __init__(self, main, new_game, root=None) :

        if root==None:
            root=Tk()
            #root.geometry('600x400+350+300')
            self.main = main
            self.game = new_game

        self.root = root
        self.root.title("Single Move Info")
        #self.main = main

        #self.root.option_add("*Font", "TSCu_Paranar 10")


        # frames
        self.frame_overall_frame          = Frame(self.root, relief = GROOVE, bd =4, height=300)


        # key bindings
        self.root.bind("<Escape>", self.esc_proc)
        self.root.protocol("WM_DELETE_WINDOW", self.end_proc)

        # variables
        self.best_move_text = StringVar()
        self.wrong_move_text = StringVar()
        self.alt1 = StringVar()
        self.alt2 = StringVar()
        self.alt3 = StringVar()


        self.tx1 = Label(self.frame_overall_frame, text="Best Move according to experts (or Truus)", anchor=W).grid(row=0,column=0)
        self.tx2 = Label(self.frame_overall_frame, text="Wrong Move as played earlier", anchor=W).grid(row=1,column=0)
        self.tx3 = Label(self.frame_overall_frame, text="Acceptable Alternative Move 1", anchor=W).grid(row=2,column=0)
        self.tx4 = Label(self.frame_overall_frame, text="Acceptable Alternative Move 2", anchor=W).grid(row=3,column=0)
        self.tx5 = Label(self.frame_overall_frame, text="Acceptable Alternative Move 3", anchor=W).grid(row=4,column=0)



        self.best_move_text = Entry(self.frame_overall_frame, width=40)
        self.best_move_text.grid(row=0,column=1)
        self.best_move_text.insert(END, "Enter Best Move")
        self.best_move_text.select_range(0, END)
        self.best_move_text.focus_set()

        self.wrong_move_text = Entry(self.frame_overall_frame, width=40)
        self.wrong_move_text.grid(row=1,column=1)
        self.wrong_move_text.insert(END, "Enter Wrong Move as Played")
        self.alt1 =Entry(self.frame_overall_frame, width=40)
        self.alt1.grid(row=2,column=1)
        self.alt1.insert(END, "Enter Alternative 1")
        self.alt2 =Entry(self.frame_overall_frame, width=40)
        self.alt2.grid(row=3,column=1)
        self.alt2.insert(END, "Enter Alternative 2")
        self.alt3 =Entry(self.frame_overall_frame, width=40)
        self.alt3.grid(row=4,column=1)
        self.alt3.insert(END, "Enter Alternative 3")


        #ok button
        self.ok_button = Button(self.frame_overall_frame, text="OK", command=Read_Move_Info(self.main, new_game))
        self.ok_button.grid(row=5, column=1)

        self.frame_overall_frame.grid()

        return # __init__

    def get_start_stop(self, param):
        s = param.get()
        if len(s) == 3:
          start = int(s[0])
          stop = int(s[2])
        elif len(s) == 5:
          start = int(s[0:2])
          stop = int(s[3:5])
        elif len(s) == 4:
          if s[1] == '-' or s[1] == 'x':
            start = int(s[0])
            stop = int(s[2:4])
          elif s[2] == '-' or s[2] == 'x':
            start = int(s[0:2])
            stop = int(s[3])
          else:
            start = 0
            stop = 0
        else:
          start = 0
          stop = 0
        return start, stop

    def entry_is_move(self, param):
        s = param.get()
        if len(s) == 3:
          if s[0].isdigit() == False: return False
          if s[2].isdigit() == False: return False
        elif len(s) == 5:
          if s[0].isdigit() == False: return False
          if s[1].isdigit() == False: return False
          if s[3].isdigit() == False: return False
          if s[4].isdigit() == False: return False
        elif len(s) == 4:
          if s[1] == '-' or s[1] == 'x':
            if s[0].isdigit() == False: return False
            if s[2].isdigit() == False: return False
            if s[3].isdigit() == False: return False
          elif s[2] == '-' or s[2] == 'x':
            if s[0].isdigit() == False: return False
            if s[1].isdigit() == False: return False
            if s[3].isdigit() == False: return False
          else:
            return False
        else:
          return False
        return True


    # -------------------------  event handlers --------------------------------- #

    def esc_proc(self, event):
        self.root.destroy()
        return

    def end_proc(self):                                 # called from 'exit' and 'x'
        self.root.destroy()
        return

if __name__=='__main__':
    game_panel = DraughtSingleMoveInfo("main")
    game_panel.root.mainloop()
