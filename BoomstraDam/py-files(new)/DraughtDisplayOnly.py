#
#  This file is Copyright (C) 2010 Feike Boomstra.
#  Distributed under the Boost Software License, Version 1.0.
#  (See accompanying file LICENSE_1_0.txt or copy at
#  http://www.boost.org/LICENSE_1_0.txt)
#


from Tkinter import *
from DraughtConstants import *

dc = DraughtConstants()

class FloatingBoard:

    HEIGHT=23               # height of a square on the board
    WIDTH=23                # width of a square on the board
    
    
    def __init__(self, main, del_window, level, master ):
        
        self.main = main
        self.local_end_proc = del_window      # to handle delete window actions
        self.level = level                    # to know where it came from
        
        # frames
        
        self.top                          = Toplevel()
        self.top.transient(master)
        self.board_frame                  = Frame(self.top, height=10*self.HEIGHT, width=10*self.WIDTH, bd=4, relief=RAISED)
        
        # binding
        self.top.protocol("WM_DELETE_WINDOW", self.end_proc)

        
        # define the draught board itself
        self.dv=PhotoImage(file="/home/feike/BoomstraDam/images/ds.gif")
        self.ws=PhotoImage(file="/home/feike/BoomstraDam/images/ws.gif")
        self.wd=PhotoImage(file="/home/feike/BoomstraDam/images/wd.gif")
        self.zs=PhotoImage(file="/home/feike/BoomstraDam/images/zs.gif")
        self.zd=PhotoImage(file="/home/feike/BoomstraDam/images/zd.gif")
        self.fdv=PhotoImage(file="/home/feike/BoomstraDam/images/fds.gif")
        self.fws=PhotoImage(file="/home/feike/BoomstraDam/images/fws.gif")
        self.fwd=PhotoImage(file="/home/feike/BoomstraDam/images/fwd.gif")
        self.fzs=PhotoImage(file="/home/feike/BoomstraDam/images/fzs.gif")
        self.fzd=PhotoImage(file="/home/feike/BoomstraDam/images/fzd.gif")

        self.board_frame.columnconfigure(0,minsize=0, pad=0)
        self.board_frame.columnconfigure(1,minsize=0, pad=0)
        self.board_frame.columnconfigure(2,minsize=0, pad=0)
        self.board_frame.columnconfigure(3,minsize=0, pad=0)
        self.board_frame.columnconfigure(4,minsize=0, pad=0)
        self.board_frame.columnconfigure(5,minsize=0, pad=0)
        self.board_frame.columnconfigure(6,minsize=0, pad=0)
        self.board_frame.columnconfigure(7,minsize=0, pad=0)
        self.board_frame.columnconfigure(8,minsize=0, pad=0)
        self.board_frame.columnconfigure(9,minsize=0, pad=0)

        self.board_frame.rowconfigure(0, minsize=0, pad=0)
        self.board_frame.rowconfigure(1, minsize=0, pad=0)
        self.board_frame.rowconfigure(2, minsize=0, pad=0)
        self.board_frame.rowconfigure(3, minsize=0, pad=0)
        self.board_frame.rowconfigure(4, minsize=0, pad=0)
        self.board_frame.rowconfigure(5, minsize=0, pad=0)
        self.board_frame.rowconfigure(6, minsize=0, pad=0)
        self.board_frame.rowconfigure(7, minsize=0, pad=0)
        self.board_frame.rowconfigure(8, minsize=0, pad=0)
        self.board_frame.rowconfigure(9, minsize=0, pad=0)


        self.c=[]
        for i in range(51):
            self.c.append(Canvas(self.board_frame, height=self.HEIGHT, width=self.WIDTH, bd=0, selectborderwidth=0, highlightthickness=0))
            self.c[i].bind("<Button-1>", self.left_button_pressed)
            self.c[i].bind("<Button-3>", self.right_button_pressed)

        self.c[1].grid(row=0, column=1)
        self.c[2].grid(row=0, column=3)
        self.c[3].grid(row=0, column=5)
        self.c[4].grid(row=0, column=7)
        self.c[5].grid(row=0, column=9)
        self.c[6].grid(row=1, column=0)
        self.c[7].grid(row=1, column=2)
        self.c[8].grid(row=1, column=4)
        self.c[9].grid(row=1, column=6)
        self.c[10].grid(row=1, column=8)
        self.c[11].grid(row=2, column=1)
        self.c[12].grid(row=2, column=3)
        self.c[13].grid(row=2, column=5)
        self.c[14].grid(row=2, column=7)
        self.c[15].grid(row=2, column=9)
        self.c[16].grid(row=3, column=0)
        self.c[17].grid(row=3, column=2)
        self.c[18].grid(row=3, column=4)
        self.c[19].grid(row=3, column=6)
        self.c[20].grid(row=3, column=8)
        self.c[21].grid(row=4, column=1)
        self.c[22].grid(row=4, column=3)
        self.c[23].grid(row=4, column=5)
        self.c[24].grid(row=4, column=7)
        self.c[25].grid(row=4, column=9)
        self.c[26].grid(row=5, column=0)
        self.c[27].grid(row=5, column=2)
        self.c[28].grid(row=5, column=4)
        self.c[29].grid(row=5, column=6)
        self.c[30].grid(row=5, column=8)
        self.c[31].grid(row=6, column=1)
        self.c[32].grid(row=6, column=3)
        self.c[33].grid(row=6, column=5)
        self.c[34].grid(row=6, column=7)
        self.c[35].grid(row=6, column=9)
        self.c[36].grid(row=7, column=0)
        self.c[37].grid(row=7, column=2)
        self.c[38].grid(row=7, column=4)
        self.c[39].grid(row=7, column=6)
        self.c[40].grid(row=7, column=8)
        self.c[41].grid(row=8, column=1)
        self.c[42].grid(row=8, column=3)
        self.c[43].grid(row=8, column=5)
        self.c[44].grid(row=8, column=7)
        self.c[45].grid(row=8, column=9)
        self.c[46].grid(row=9, column=0)
        self.c[47].grid(row=9, column=2)
        self.c[48].grid(row=9, column=4)
        self.c[49].grid(row=9, column=6)
        self.c[50].grid(row=9, column=8)

        self.image_item=[]
        for i in range(51):
            self.image_item.append(self.c[i].create_image(0, 0, image=self.dv, anchor=NW, tags=str(i)))
        # end of definition of draught board

        self.board_frame.pack()

        return  # __init__
    
    def left_button_pressed(self, event):
        self.main.left_mousebutton_clicked(self.get_field_nr(event))
        return
    
    def right_button_pressed(self, event):
        self.main.right_mousebutton_clicked(self.get_field_nr(event))
        return
    
    
    def get_field_nr(self, event):
        if self.main.panel.invert_board.get() == 0:
            for i in range(1, 51):
                if self.c[i]==event.widget:
                    return i
        else:
            for i in range(1, 51):
                if self.c[i]==event.widget:
                    return 51 - i
        return 0 
    
    def end_proc(self):
        self.local_end_proc(self.level)
        self.top.destroy()
        return   

    def set_white_man(self, i, selected):
        if self.main.panel.invert_board.get() == 1:
            j = 51 - i
        else:
            j = i        
        if not selected:
            self.c[j].itemconfigure(self.image_item[j], image=self.ws)
        else:
            self.c[j].itemconfigure(self.image_item[j], image=self.fws)
        return
        
    def set_white_king(self, i, selected):
        if self.main.panel.invert_board.get() == 1:
            j = 51 - i
        else:
            j = i        
        if not selected:
            self.c[j].itemconfigure(self.image_item[i], image=self.wd)
        else:
            self.c[j].itemconfigure(self.image_item[i], image=self.fwd)
        return
        
    def set_black_man(self, i, selected):
        if self.main.panel.invert_board.get() == 1:
            j = 51 - i
        else:
            j = i        
        if not selected:
            self.c[j].itemconfigure(self.image_item[j], image=self.zs)
        else:
            self.c[j].itemconfigure(self.image_item[j], image=self.fzs)
        return
        
    def set_black_king(self, i, selected):
        if self.main.panel.invert_board.get() == 1:
            j = 51 - i
        else:
            j = i        
        if not selected:
            self.c[j].itemconfigure(self.image_item[j], image=self.zd)
        else:
            self.c[j].itemconfigure(self.image_item[j], image=self.fzd)
        return
        
    def set_empty_field(self, i, selected):
        if self.main.panel.invert_board.get() == 1:
            j = 51 - i
        else:
            j = i        
        if not selected:
            self.c[j].itemconfigure(self.image_item[j], image=self.dv)
        else:
            self.c[j].itemconfigure(self.image_item[j], image=self.fdv)
        return
    
    def geometry(self, geometry=None):
        return self.top.geometry(geometry)

    