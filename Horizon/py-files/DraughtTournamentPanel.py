#
#  This file is Copyright (C) 2010 Feike Boomstra.
#  Distributed under the Boost Software License, Version 1.0.
#  (See accompanying file LICENSE_1_0.txt or copy at
#  http://www.boost.org/LICENSE_1_0.txt)
#


"""User Interface for International Draught Board"""

from Tkinter import *
import tkFont
from DraughtConstants import *
dc =DraughtConstants()         # get the global constants

class DraughtPanel:

    HEIGHT=72               # height of a square on the board
    WIDTH =72               # width of a square on the board

    def __init__(self, main, root=None) :

        if root==None:
            root=Tk()
            #root.geometry('600x800+350+70')

        self.root = root
        self.top  = root
        self.root.title("Horizon v1.0,       what's beyond the horizon....")
        self.main = main

        #self.root.option_add("*Font", "TSCu_Paranar 10")

        # variables
        self.current_white_player=IntVar()  # (computer, human player, remote player)
        self.current_black_player=IntVar()  # (computer, human player, remote player)
        self.current_player=IntVar()        # (computer, human player, remote player) , redundant to simplifly coding
        self.current_color_on_move=IntVar() # current color state
        self.auto_move = IntVar()           # auto move and capture for human player
        self.invert_board = IntVar()        # turn the board 180 degrees
        self.status_display = StringVar()   # to show move and lost/win status
        self.phase=IntVar()
        self.notation_display = StringVar()   # to display the notation of the game
        self.white_numbers_display = StringVar() # to display the number of men and king
        self.black_numbers_display =StringVar()
        self.sound_mode = dc.SOUND_ON      # reflects the sound mode
        self.WhitePlayer = StringVar()  #the info for the game
        self.BlackPlayer = StringVar()
        self.Result = StringVar()
        self.Date = StringVar()
        self.Round = StringVar()
        self.Event = StringVar()
        self.result_text = StringVar()
        self.time_text = StringVar()
        self.myFont = tkFont.Font(family="bitstream vera sans", weight="bold", size=9)
        # frames
#        self.frame_overall_frame          = Frame(self.root, relief = GROOVE, bd =4)
        self.row2_frame             = Frame(self.root)
        self.info_frame             = Frame(self.row2_frame, bd=3, relief=GROOVE)
        self.time_display_frame     = Frame(self.row2_frame, bd=3, relief=GROOVE)
        self.buttonbar              = Frame(self.root)#, bd=3, relief=GROOVE)
        self.frame_remaining_layout = Frame(self.root )
        self.frame_notation_frame   = Frame(self.root)
        self.frame_right_hand       = Frame(self.frame_remaining_layout)
        self.frame_left_hand        = Frame(self.frame_remaining_layout)
        self.board_frame            = Frame(self.frame_remaining_layout, height=10*self.HEIGHT, width=10*self.WIDTH, bd=4, relief=RAISED, bg="lightgray")
        self.current_player_frame   = Frame(self.frame_right_hand)


        # key bindings
        self.root.bind("<Escape>", self.esc_proc)
        self.root.protocol("WM_DELETE_WINDOW", self.end_proc)
        self.root.bind("<space>", self.move_now)
        self.root.bind("<Left>", self.main.left_arrow_clicked)
        self.root.bind("<Right>", self.main.right_arrow_clicked)
        self.root.bind("<Up>", self.main.up_arrow_clicked)
        self.root.bind("<Down>", self.main.down_arrow_clicked)
        self.root.bind("<Control-Left>", self.main.control_left_arrow_clicked)
        self.root.bind("<Control-Right>", self.main.control_right_arrow_clicked)
        self.root.bind("<Control-Up>", self.main.control_up_arrow_clicked)
        self.root.bind("<Control-Down>", self.main.control_down_arrow_clicked)
        self.root.bind("<Next>", self.main.page_down_clicked)
        self.root.bind("<Prior>", self.main.page_up_clicked)
        #self.root.bind("n",self.main.stand.next_line)
        #self.root.bind("p",self.main.stand.prev_line)


        # define the draught board itself
        self.empty_field          = PhotoImage(file="/home/feike/Horizon/images/ds72.gif")
        self.empty_field_selected = PhotoImage(file="/home/feike/Horizon/images/fds72.gif")
        self.empty_field_proposed = PhotoImage(file="/home/feike/Horizon/images/bds72.gif")
        self.white_man            = PhotoImage(file="/home/feike/Horizon/images/ws72.gif")
        self.white_man_selected   = PhotoImage(file="/home/feike/Horizon/images/fws72.gif")
        self.white_man_proposed   = PhotoImage(file="/home/feike/Horizon/images/bws72.gif")
        self.white_king           = PhotoImage(file="/home/feike/Horizon/images/wd72.gif")
        self.white_king_selected  = PhotoImage(file="/home/feike/Horizon/images/fwd72.gif")
        self.white_king_proposed  = PhotoImage(file="/home/feike/Horizon/images/bwd72.gif")
        self.black_man            = PhotoImage(file="/home/feike/Horizon/images/zs72.gif")
        self.black_man_selected   = PhotoImage(file="/home/feike/Horizon/images/fzs72.gif")
        self.black_man_proposed   = PhotoImage(file="/home/feike/Horizon/images/bzs72.gif")
        self.black_king           = PhotoImage(file="/home/feike/Horizon/images/zd72.gif")
        self.black_king_selected  = PhotoImage(file="/home/feike/Horizon/images/fzd72.gif")
        self.black_king_proposed  = PhotoImage(file="/home/feike/Horizon/images/bzd72.gif")
        self.yellow_lit           = PhotoImage(file="/home/feike/Horizon/images/yellow lit.gif")
        self.grey_lit             = PhotoImage(file="/home/feike/Horizon/images/grey lit.gif")

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
            self.image_item.append(self.c[i].create_image(0, 0, image=self.empty_field, anchor=NW, tags=str(i)))
        # end of definition of draught board

        # The other parts of the application interface: toolbar and buttons

        self.menubar = Menu(self.root)

        # create a pulldown menu "File", and add it to the menu bar
        self.filemenu = Menu(self.menubar, tearoff=0)
        self.filemenu.add_command(label="New Game", command=self.main.new_game_clicked)
        self.filemenu.add_command(label="Open Game", command=self.main.file_open_game_clicked)
        self.filemenu.add_command(label="Save Game", command=self.main.file_save_game_clicked)
        #self.filemenu.add_command(label="Save Doc",  command=self.main.file_save_doc_clicked)
        #self.filemenu.add_command(label="Make Records", command=self.main.make_game_records.MakeOpening)
        #self.filemenu.add_command(label="Open Stands", command=self.main.stand.get_stand_file)
        self.filemenu.add_separator()
        #self.filemenu.add_command(label="Stop Engine", command=self.stop_engine)
        self.filemenu.add_command(label="Exit", command=self.end_proc)
        self.menubar.add_cascade(label="File", menu=self.filemenu)

##        # create a pulldown menu "Remote", and add it to the menu bar
##        self.remotemenu = Menu(self.menubar, tearoff=0)
##        self.remotemenu.add_command(label="Make Connection", command=self.hello)
##        self.remotemenu.add_command(label="Wait for Connection", command=self.remote_play_mode)
##        self.remotemenu.add_command(label="all 158", command=self.main.play_158)
##        self.remotemenu.add_command(label="single 158", command=self.main.play_single_game)
##        self.menubar.add_cascade(label="Remote", menu=self.remotemenu)

        # create a pulldown menu "Info", and add it to the menu bar
        self.info_menu = Menu(self.menubar, tearoff=0)
        self.info_menu.add_command(label="Game Info", command=self.main.Get_Game_Info)
        self.info_menu.add_command(label="Time Info", command=self.main.Get_Time_Info)
        self.info_menu.add_command(label="Position reconstruction",  command = self.main.Reconstruct_Position )
        self.menubar.add_cascade(label="Info", menu=self.info_menu)

##        # create a pulldown menu "Test", and add it to the menu bar
##        self.test_menu = Menu(self.menubar, tearoff=0)
##        self.test_menu.add_command(label="Eval", command=self.main.eval_mode_clicked)
##        self.test_menu.add_command(label="(Re)load Engine", command=self.main.reload_engine)
##        self.test_menu.add_command(label="Anal Time", command=self.main.anal_time_mode_clicked)
##        self.test_menu.add_command(label="Anal Pos d=02", command=self.main.anal_pos_mode_clicked_02)
##        self.test_menu.add_command(label="Anal Pos d=04", command=self.main.anal_pos_mode_clicked_04)
##        self.test_menu.add_command(label="Anal Pos d=06", command=self.main.anal_pos_mode_clicked_06)
##        self.test_menu.add_command(label="Anal Pos d=08", command=self.main.anal_pos_mode_clicked_08)
##        self.test_menu.add_command(label="Anal Pos d=10", command=self.main.anal_pos_mode_clicked_10)
##        self.test_menu.add_command(label="Anal Pos d=12", command=self.main.anal_pos_mode_clicked_12)
##        self.test_menu.add_command(label="Anal Pos d=14", command=self.main.anal_pos_mode_clicked_14)
##        self.menubar.add_cascade(label="Test", menu=self.test_menu)

##        # create a pulldown menu "WalkHash", and add it to the menu bar
##        self.walkhash_menu = Menu(self.menubar, tearoff=0)
##        self.walkhash_menu.add_command(label="Set Depth", command=self.main.set_depth)
##        self.walkhash_menu.add_command(label="Set Threads", command=self.main.set_threads)
##        self.walkhash_menu.add_command(label="Get Best Move", command=self.main.setup_hash)
##        self.walkhash_menu.add_command(label="Start Pondering", command=self.main.setup_ponder)
##        self.walkhash_menu.add_command(label="Show PV", command=self.main.walk_hash_proc)
##        self.menubar.add_cascade(label="Analyze", menu=self.walkhash_menu)

##        # create more pulldown menus
##        self.viewmenu = Menu(self.menubar, tearoff=0)
##        self.viewmenu.add_command(label="> Sound -- On", command=self.set_sound_level_on)
##        self.viewmenu.add_command(label="  Sound -- TWO", command=self.set_sound_level_TWO)
##        self.viewmenu.add_command(label="  Sound -- Off", command=self.set_sound_level_off)
##        self.menubar.add_cascade(label="View", menu=self.viewmenu)

##        # live draught utilities
##        self.livemenu = Menu(self.menubar, tearoff=0)
##        self.livemenu.add_command(label="Show Current Round", command=self.main.get_current_round)
##        self.livemenu.add_command(label="Upload Start Charts", command=self.main.put_start_charts_in_current_round)
##        self.livemenu.add_command(label="Start Polling", command=self.main.start_polling)
##        self.livemenu.add_command(label="Analyze Game", command=self.main.analyze_game)
##        self.livemenu.add_command(label="Create Chart", command=self.main.create_chart)
##        self.menubar.add_cascade(label="Live Games", menu=self.livemenu)


##        self.helpmenu = Menu(self.menubar, tearoff=0)
##        self.helpmenu.add_command(label="About", command=self.hello)
##        self.menubar.add_cascade(label="Help", menu=self.helpmenu)
        self.root.config(menu=self.menubar, bd=3, relief=GROOVE)

        #the info bar

        self.iwpl = Label(self.info_frame, text="White Player").grid(row=0,column=0, sticky=W)
        self.iwp = Label(self.info_frame, textvariable=self.WhitePlayer, width=18, anchor=W)
        self.iwp.config(font=self.myFont)
        self.iwp.grid(row=0,column=1, sticky=W)
        self.ibpl = Label(self.info_frame, text="Black Player").grid(row=0,column=2, sticky=W)
        self.ibp = Label(self.info_frame, textvariable=self.BlackPlayer, width=18, anchor=W)
        self.ibp.config(font=self.myFont)
        self.ibp.grid(row=0,column=3, sticky=W)
        self.irsl = Label(self.info_frame, text="Result").grid(row=0,column=4, sticky=W)
        self.irs = Label(self.info_frame, textvariable=self.Result, width=5, anchor=W)
        self.irs.config(font=self.myFont)
        self.irs.grid(row=0,column=5, sticky=W)
        self.ievl = Label(self.info_frame, text="Event").grid(row=1,column=0, sticky=W)
        self.iev = Label(self.info_frame, textvariable=self.Event, width=18, anchor=W)
        self.iev.config(font=self.myFont)
        self.iev.grid(row=1,column=1, sticky=W)
        self.idtl = Label(self.info_frame, text="Date").grid(row=1,column=2, sticky=W)
        self.idt = Label(self.info_frame, textvariable=self.Date, width=18, anchor=W)
        self.idt.config(font=self.myFont)
        self.idt.grid(row=1,column=3, sticky=W)
        self.irnl = Label(self.info_frame, text="Round").grid(row=1,column=4, sticky=W)
        self.irn = Label(self.info_frame, textvariable=self.Round, width=5, anchor=W)
        self.irn.config(font=self.myFont)
        self.irn.grid(row=1,column=5, sticky=W)
        #self.info_frame.grid(row=0, column=0,  sticky=W)


        #time display
        self.time_display = Label(self.time_display_frame, textvariable = self.time_text, font=("lucidatypewriter", "25", "bold"), width = 6)
        self.time_display.grid(row=0, column=0)
        self.time_text.set("20:00")

        #clock is running lamp
        self.lit_display = Canvas(self.time_display_frame, height = 35, width=35)
        self.lit_item = self.lit_display.create_image(4, 4, image = self.grey_lit, anchor=NW)
        self.lit_display.grid(row=0, column=1)


        # the button bar
        self.bs = Button(self.buttonbar, text="setup", width=6, command=self.main.board_begin_position_clicked)
        self.bs.grid(sticky=W, padx=2, pady=8, row=0, column=0)
        self.bc = Button(self.buttonbar, text="clear", width=6, command=self.main.board_clear_clicked)
        self.bc.grid(sticky=W, padx=2, pady=2, row=0, column=1)
        self.rs = Radiobutton(self.buttonbar, text="pieces set-up", variable=self.phase, value=dc.SETUP_MODE, command=self.setup_mode)
        self.rs.grid(sticky=W, padx=2, pady=2, row=0, column=2)
        self.rp = Radiobutton(self.buttonbar, text="play", variable=self.phase, value=dc.PLAY_MODE, command=self.play_mode)
        self.rp.grid(sticky=W, padx=2, pady=2, row=0, column=3)
        self.rs.select()
##        self.bt1 = Button(self.buttonbar, text="clear print", width=6, command=self.main.clear_prints_clicked)
##        self.bt1.grid(sticky=W, padx=2, pady=8, row=0, column=4)
##        self.bt2 = Button(self.buttonbar, text="add print", width=6, command=self.main.add_print_clicked)
##        self.bt2.grid(sticky=W, padx=2, pady=8, row=0, column=5)
##        self.bt3 = Button(self.buttonbar, text="make print", width=6, command=self.main.print_collection_clicked)
##        self.bt3.grid(sticky=W, padx=2, pady=8, row=0, column=6)

        self.tx = Label(self.frame_right_hand, textvariable=self.status_display).grid(row=0,pady=8, padx=6)
        self.status_display.set("Setup Mode!")


        self.wc = Radiobutton(self.current_player_frame, text="", variable=self.current_white_player, value=dc.COMPUTER)
        self.wh = Radiobutton(self.current_player_frame, text="", variable=self.current_white_player, value=dc.HUMAN_PLAYER)
##        self.wr = Radiobutton(self.current_player_frame, text="", variable=self.current_white_player, value=dc.REMOTE_PLAYER)
        self.wm = Radiobutton(self.current_player_frame, text="", variable=self.current_color_on_move, value=dc.WHITE_ON_MOVE  \
                                            ,command=self.main.ChangeColor)

        self.wc.grid(sticky=E, padx=6, pady=4, row=0, column=0)
        self.wh.grid(sticky=E, padx=6, pady=4, row=1, column=0)
##        self.wr.grid(sticky=E, padx=6, pady=4, row=2, column=0)
        self.wm.grid(sticky=E, padx=6, pady=4, row=2, column=0)

        self.bc = Radiobutton(self.current_player_frame, text="computer", variable=self.current_black_player, value=dc.COMPUTER)
        self.bh = Radiobutton(self.current_player_frame, text="human player", variable=self.current_black_player, value=dc.HUMAN_PLAYER)
##        self.br = Radiobutton(self.current_player_frame, text="remote player", variable=self.current_black_player, value=dc.REMOTE_PLAYER)
        self.bm = Radiobutton(self.current_player_frame, text="on move", variable=self.current_color_on_move, value=dc.BLACK_ON_MOVE \
                                            ,command=self.main.ChangeColor)

        self.bc.grid(sticky=W, padx=2, pady=4, row=0, column=1)
        self.bh.grid(sticky=W, padx=2, pady=4, row=1, column=1)
##        self.br.grid(sticky=W, padx=2, pady=4, row=2, column=1)
        self.bm.grid(sticky=W, padx=2, pady=4, row=2, column=1)

        self.wh.select()
        self.bc.select()
        self.wm.select()

        self.current_player_frame.grid(row=1)

##        self.acb = Checkbutton(self.frame_right_hand, text="Auto Move", variable=self.auto_move).grid(sticky=W,row=2, pady=4, padx=6)
##        self.auto_move.set(0)                                                          # set it off
        self.inv = Checkbutton(self.frame_right_hand, text="Invert Board", variable=self.invert_board).grid(sticky=W,row=3,pady=4, padx=6)
        self.invert_board.set(0)
##        self.res = Label(self.frame_right_hand, textvariable=self.result_text).grid(row=5,pady=4, padx=6)
##        self.result_text.set("No games")

        self.scroll = Scrollbar (self.frame_notation_frame, orient=VERTICAL )
        self.notation = Text(self.frame_notation_frame, height = 18, width = 120)

        self.notation.grid(row=0, column = 0)
        self.scroll.grid(row=0, column = 1,  sticky =N+S)
        self.notation.insert(END, "Welcome")
        self.notation.config(font = ("Courier", 9))
        self.notation.config(yscrollcommand=self.scroll.set)
        self.scroll.config(command = self.notation.yview)


        self.frame_left_hand.grid(column=0, row=0, sticky=N+S)
        self.board_frame.grid(column=1, row=0)
        self.frame_right_hand.grid(column=2, row=0, sticky=N)

        self.black_numbers = Label(self.frame_left_hand, textvariable = self.black_numbers_display)
        self.white_numbers = Label(self.frame_left_hand, textvariable = self.white_numbers_display)
        self.white_numbers.grid(row=0,column=0, sticky=N+S)
        self.black_numbers.grid(row=1,column=0, sticky=N+S)
        self.white_numbers_display.set( "0 (0)")
        self.black_numbers_display.set( "0 (0)")

        self.row2_frame.grid(row=0, column=0)
        self.info_frame.grid(row=0, column=0)
        self.time_display_frame.grid(row=0, column=1)
        self.buttonbar.grid(row=1, column=0, sticky=W)
        self.frame_remaining_layout.grid(row=2, column=0, sticky=W+E)
        self.frame_notation_frame.grid(row=3, column=0, sticky=N+E+S+W)
        #self.frame_overall_frame.grid(sticky=N+E+S+W)

        return # __init__

    # -------------------------  event handlers --------------------------------- #
    # here we check whether the event is active at this moment -------------------#
    # if so, the call-back function is called, otherwise just ignore -------------#
    # also the enable - disable of the buttons etc. is done here -----------------#


    def enable_setup_switches(self):
        self.wc.config(state=ACTIVE)
        self.wh.config(state=ACTIVE)
##        self.wr.config(state=ACTIVE)
        self.bc.config(state=ACTIVE)
        self.bh.config(state=ACTIVE)
##        self.br.config(state=ACTIVE)
        self.wm.config(state=ACTIVE)
        self.bm.config(state=ACTIVE)
        return

    def disable_setup_switches(self):
        self.wc.config(state=DISABLED)
        self.wh.config(state=DISABLED)
##        self.wr.config(state=DISABLED)
        self.bc.config(state=DISABLED)
        self.bh.config(state=DISABLED)
##        self.br.config(state=DISABLED)
        self.wm.config(state=DISABLED)
        self.bm.config(state=DISABLED)
        return

    def esc_proc(self, event):
        self.phase.set(dc.SETUP_MODE)                      # stop playing
        return

    def end_proc(self):                                 # called from 'exit' and 'x'
        if self.phase.get()==dc.PLAY_MODE:
            self.main.exit_clicked()
        else:
            #self.main.play.engine.send_close_engine()
            self.root.quit()
        return

    def stop_engine(self):
        self.main.play.engine.send_close_engine()
        return


    def play_mode(self):
        self.disable_setup_switches()
        self.main.play_mode_clicked()
        return

    def remote_play_mode(self):
        self.disable_setup_switches()
        self.main.remote_play_mode_clicked()
        return

    def setup_mode(self):
        self.phase.set(dc.SETUP_MODE)
        self.enable_setup_switches()
        self.main.setup_mode_clicked()
        return

    def move_now(self, event):
        #if self.phase.get()==dc.PLAY_MODE or self.phase.get()==dc.ANALYZE_MODE:
        self.main.spacebar_clicked()
        return

    def left_button_pressed(self, event):
        self.main.left_mousebutton_clicked(self.get_field_nr(event))
        return

    def right_button_pressed(self, event):
        self.main.right_mousebutton_clicked(self.get_field_nr(event))
        return


    def get_field_nr(self, event):
        if self.invert_board.get() == 0:
            for i in range(1, 51):
                if self.c[i]==event.widget:
                    return i
        else:
            for i in range(1, 51):
                if self.c[i]==event.widget:
                    return 51 - i
        return 0

    def set_sound_level_on(self):
        self.sound_mode = dc.SOUND_ON
        self.viewmenu.entryconfig(1, label='  Sound -- TWO')
        self.viewmenu.entryconfig(2, label='  Sound -- Off')
        self.viewmenu.entryconfig(0, label='> Sound -- On')
        return
    def set_sound_level_TWO(self):
        self.sound_mode = dc.SOUND_TWO
        self.viewmenu.entryconfig(1, label='> Sound -- TWO')
        self.viewmenu.entryconfig(2, label='  Sound -- Off')
        self.viewmenu.entryconfig(0, label='  Sound -- On')
        return
    def set_sound_level_off(self):
        self.sound_mode = dc.SOUND_OFF
        self.viewmenu.entryconfig(1, label='  Sound -- TWO')
        self.viewmenu.entryconfig(2, label='> Sound -- Off')
        self.viewmenu.entryconfig(0, label='  Sound -- On')
        return

    def hello(self):
        self.disable_setup_switches()
        self.main.remote_play_mode_clicked(False)
        return

    def set_white_man(self, i, selected):
        if self.invert_board.get() == 1:
            j = 51 - i
        else:
            j = i
        if not selected:
            self.c[j].itemconfigure(self.image_item[j], image=self.white_man)
        else:
            self.c[j].itemconfigure(self.image_item[j], image=self.white_man_selected)
        return

    def set_white_king(self, i, selected):
        if self.invert_board.get() == 1:
            j = 51 - i
        else:
            j = i
        if not selected:
            self.c[j].itemconfigure(self.image_item[i], image=self.white_king)
        else:
            self.c[j].itemconfigure(self.image_item[i], image=self.white_king_selected)
        return

    def set_black_man(self, i, selected):
        if self.invert_board.get() == 1:
            j = 51 - i
        else:
            j = i
        if not selected:
            self.c[j].itemconfigure(self.image_item[j], image=self.black_man)
        else:
            self.c[j].itemconfigure(self.image_item[j], image=self.black_man_selected)
        return

    def set_black_king(self, i, selected):
        if self.invert_board.get() == 1:
            j = 51 - i
        else:
            j = i
        if not selected:
            self.c[j].itemconfigure(self.image_item[j], image=self.black_king)
        else:
            self.c[j].itemconfigure(self.image_item[j], image=self.black_king_selected)
        return

    def set_empty_field(self, i, selected):
        if self.invert_board.get() == 1:
            j = 51 - i
        else:
            j = i
        if not selected:
            self.c[j].itemconfigure(self.image_item[j], image=self.empty_field)
        else:
            self.c[j].itemconfigure(self.image_item[j], image=self.empty_field_selected)
        return

    def set_running(self):
      self.lit_display.itemconfigure(self.lit_item, image=self.yellow_lit)
      return

    def stop_running(self):
      self.lit_display.itemconfigure(self.lit_item, image=self.grey_lit)
      return


    def is_setup_mode(self):
        return self.phase.get() == dc.SETUP_MODE

    def is_play_mode(self):
        return self.phase.get() == dc.PLAY_MODE

    def get_analyse_depth(self):
        items = self.depth_list_box.curselection()
        try:
            items = map(int, items)
        except ValueError: pass
        return (2* items[0]) +2

    def color_background(self):
      self.board_frame.config(bg = "lightblue")
      return

    def clear_background(self):
      self.board_frame.config(bg="lightgray")
      return

