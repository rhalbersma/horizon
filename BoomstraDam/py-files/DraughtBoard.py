#
#  This file is Copyright (C) 2010 Feike Boomstra.
#  Distributed under the Boost Software License, Version 1.0.
#  (See accompanying file LICENSE_1_0.txt or copy at
#  http://www.boost.org/LICENSE_1_0.txt)
#


from Tkinter import *
import tkFont
from DraughtDataStructures import *
from DraughtConstants import *
dc = DraughtConstants()

class DraughtBoardView:
    HEIGHT=23               # height of a square on the board
    WIDTH=23                # width of a square on the board

    def __init__(self, board_frame):

        self.board_position = DraughtBoard()
        # service varaibles
        self.current_color_on_move=IntVar() # current color state
        self.invert_board = IntVar()        # turn the board 180 degrees
        self.white_numbers_display = StringVar() # to display the number of men and king
        self.black_numbers_display =StringVar()

        self.entire_board_frame = board_frame
        self.service_frame = Frame(self.entire_board_frame, height=10*self.HEIGHT, width=self.WIDTH,      bd=4, relief=RAISED)
        self.board_frame   = Frame(self.entire_board_frame, height=10*self.HEIGHT, width=10*self.WIDTH, bd=4, relief=RAISED)
        self.service_frame.grid(row=0,column=0)
        self.board_frame.grid(row=0,column=1)
        self.entire_board_frame.rowconfigure(0, weight=100)
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
##            self.c[i].bind("<Button-1>", self.left_button_pressed)
##            self.c[i].bind("<Button-3>", self.right_button_pressed)

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
            
        #now the service part:
        self.wm = Radiobutton(self.service_frame, text="", variable=self.current_color_on_move, value=dc.WHITE_ON_MOVE)
        self.bm = Radiobutton(self.service_frame, text="", variable=self.current_color_on_move, value=dc.BLACK_ON_MOVE)
        self.black_numbers = Label(self.service_frame, textvariable = self.black_numbers_display)
        self.white_numbers = Label(self.service_frame, textvariable = self.white_numbers_display)
        self.inv = Checkbutton(self.service_frame, text="Inv", variable=self.invert_board, command = self.flip)
        
        
        self.wm.grid(row=3,column=0)
        self.white_numbers.grid(row=2,column=0)
        self.black_numbers.grid(row=1,column=0)
        self.bm.grid(row=0,column=0)
        self.inv.grid(row=4,column=0)

        self.service_frame.rowconfigure(0,weight = 2, pad=33)
        self.service_frame.rowconfigure(1,weight = 3, pad=33)
        self.service_frame.rowconfigure(2,weight = 3, pad=34)
        self.service_frame.rowconfigure(3,weight = 1, pad=34)
        self.service_frame.rowconfigure(4,weight = 1)
        
        self.wm.config(state=DISABLED)
        self.bm.config(state=DISABLED)
        
        # end of definition of draught board
        
        return
        
        
    def flip(self):
        self.display(self.board_position)
        return
        
        
    def display(self, position):
        # to remember the pieces when we have to flip
        self.board_position.WhiteMan = position.WhiteMan[:]
        self.board_position.WhiteKing = position.WhiteKing[:]
        self.board_position.BlackMan = position.BlackMan[:]
        self.board_position.BlackKing = position.BlackKing[:]
        self.board_position.ColorOnMove = position.ColorOnMove
    
        temp = position.WhiteMan + position.WhiteKing + position.BlackKing + position.BlackMan
        Empty = range(1,51)
        for item in temp:
            Empty.remove(item)
        for i in position.WhiteMan:
            self.set_white_man(i, False)
        for i in position.BlackMan:
            self.set_black_man(i, False)
        for i in position.WhiteKing:
            self.set_white_king(i, False)
        for i in position.BlackKing:
            self.set_black_king(i, False)
        for i in Empty:
            self.set_empty_field(i,False)
        # and the service part
        if self.invert_board.get() == 0:
            if position.ColorOnMove == dc.WHITE_ON_MOVE:
                self.wm.select()
            else:
                self.bm.select()
            self.white_numbers_display.set(str(len(position.WhiteMan)) + '(' + str(len(position.WhiteKing)) +')')
            self.black_numbers_display.set(str(len(position.BlackMan)) + '(' + str(len(position.BlackKing)) +')')
        else:
            if position.ColorOnMove == dc.WHITE_ON_MOVE:
                self.bm.select()
            else:
                self.wm.select()
            self.black_numbers_display.set(str(len(position.WhiteMan)) + '(' + str(len(position.WhiteKing)) +')')
            self.white_numbers_display.set(str(len(position.BlackMan)) + '(' + str(len(position.BlackKing)) +')')
        return
        
    def set_white_man(self, i, selected=False):
        if self.invert_board.get() == 1:
            j = 51 - i
        else:
            j = i        
        if not selected:
            self.c[j].itemconfigure(self.image_item[j], image=self.ws)
        else:
            self.c[j].itemconfigure(self.image_item[j], image=self.fws)
        return
        
    def set_white_king(self, i, selected=False):
        if self.invert_board.get() == 1:
            j = 51 - i
        else:
            j = i        
        if not selected:
            self.c[j].itemconfigure(self.image_item[i], image=self.wd)
        else:
            self.c[j].itemconfigure(self.image_item[i], image=self.fwd)
        return
        
    def set_black_man(self, i, selected=False):
        if self.invert_board.get() == 1:
            j = 51 - i
        else:
            j = i        
        if not selected:
            self.c[j].itemconfigure(self.image_item[j], image=self.zs)
        else:
            self.c[j].itemconfigure(self.image_item[j], image=self.fzs)
        return
        
    def set_black_king(self, i, selected=False):
        if self.invert_board.get() == 1:
            j = 51 - i
        else:
            j = i        
        if not selected:
            self.c[j].itemconfigure(self.image_item[j], image=self.zd)
        else:
            self.c[j].itemconfigure(self.image_item[j], image=self.fzd)
        return
        
    def set_empty_field(self, i, selected=False):
        if self.invert_board.get() == 1:
            j = 51 - i
        else:
            j = i        
        if not selected:
            self.c[j].itemconfigure(self.image_item[j], image=self.dv)
        else:
            self.c[j].itemconfigure(self.image_item[j], image=self.fdv)
        return
 
