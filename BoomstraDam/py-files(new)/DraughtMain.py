#
#  This file is Copyright (C) 2010 Feike Boomstra.
#  Distributed under the Boost Software License, Version 1.0.
#  (See accompanying file LICENSE_1_0.txt or copy at
#  http://www.boost.org/LICENSE_1_0.txt)
#


from DraughtConstants import *
from DraughtPanel import *
from DraughtReadPDN import *
from DraughtSavePDN import *
from DraughtSetUp import *
from DraughtPlay import *
from DraughtAnalyze import *
from DraughtDisplayOnly import *
from DraughtRePlay import *
from MoveGenerator import *
from DraughtDataStructures import *
from DraughtMakeGameRecords import *
from DraughtReadStandFile import *
from DraughtGamePanel import *
from DraughtTimeSetPanel import *
from datetime import *
#import winsound

dc = DraughtConstants()

class DraughtMain:
    def __init__(self):
        self.Games = []
        self.root=Tk()
        self.root.geometry('600x600+350+70')
        self.make_game_records = DraughtMakeGameRecords(self)
        self.stand = ReadStand(self, self.root)
        self.panel = DraughtPanel(self, self.root)
        self.read = ReadPDN(self, self.root)
        self.play = DraughtPlay(self, self.panel)
        self.setup = DraughtSetUp(self, self.panel)
        self.anal = DraughtAnalyze(self)
        self.re_play = DraughtRePlay(self)
        self.mg = MoveGenerator()
        self.save = DraughtSavePDN(self, self.root)
        game = GameRecord(self, self.panel, self.panel)
        self.ActiveGame = game
        self.ActiveGameNumber = 0
        self.ActiveTag = ''
        self.CurrentDisplayGame = []
        self.NewPanelGeometry = ['', '238x238+958+70', '238x238+958+100', '238x238+958+130', '238x238+958+160', '238x238+958+190', '238x238+958+220']
        self.EngineStarted = False
        self.RemoteSocketStarted = False
        self.SocketReaderStarted = False
        self.TotalGameTime = 25
        self.HandlingTime = 8
        self.TotalNumberOfMoves = 75
        self.TotalEngineTime = 60*self.TotalGameTime - self.TotalNumberOfMoves*self.HandlingTime
        self.LastEval = 0

        return
    
    def MainScheduler(self):
        self.panel.root.mainloop()              
        return
    
    def exit_clicked(self):
        print "Exit clicked"
        self.play.end_proc()
        return
    
    def setup_mode_clicked(self):
        self.play.stop_proc()
        return
    
    def play_mode_clicked(self):
        self.play.play_mode()
        return

    def remote_play_mode_clicked(self):
        self.play.remote_play_mode()
        return
    
    def anal_pos_mode_clicked_02(self):
        self.anal.anal_pos_mode_clicked(2)
        return
    
    def anal_pos_mode_clicked_04(self):
        self.anal.anal_pos_mode_clicked(4)
        return
    
    def anal_pos_mode_clicked_06(self):
        self.anal.anal_pos_mode_clicked(6)
        return
    
    def anal_pos_mode_clicked_08(self):
        self.anal.anal_pos_mode_clicked(8)
        return
    
    def anal_pos_mode_clicked_10(self):
        self.anal.anal_pos_mode_clicked(10)
        return
    
    def anal_pos_mode_clicked_12(self):
        self.anal.anal_pos_mode_clicked(12)
        return
    
    def anal_pos_mode_clicked_14(self):
        self.anal.anal_pos_mode_clicked(14)
        return
    
    def anal_time_mode_clicked(self):
        self.anal.anal_time_mode_clicked()
        return
    
    def eval_mode_clicked(self):
        self.anal.eval_mode_clicked(self.ActiveGame)
        return
    
    def spacebar_clicked(self):
        self.play.startup_engine()
        self.play.engine.send_move_now()
        return
    
    def left_mousebutton_clicked(self, idx):
        if self.panel.is_play_mode():
            if self.panel.current_color_on_move.get()==dc.WHITE_ON_MOVE:
                if self.panel.current_white_player.get()==dc.HUMAN_PLAYER:
                    self.play.move_proposal(idx, self.ActiveGame)
                else: self.beep()
            else:
                if self.panel.current_black_player.get()==dc.HUMAN_PLAYER:
                    self.play.move_proposal(idx, self.ActiveGame)
                else: self.beep()                                           
        else:
            self.panel.setup_mode()
            self.setup.set_white_piece(idx, self.ActiveGame)
        return
    
    def ChangeColor(self):
        if self.panel.current_color_on_move.get()==dc.WHITE_ON_MOVE:
            self.panel.bm.select()
            self.ActiveGame.CurrentPosition.ColorOnMove = dc.BLACK_ON_MOVE
        else:
            self.panel.wm.select()
            self.ActiveGame.CurrentPosition.ColorOnMove = dc.WHITE_ON_MOVE
        
        self.setup.clear_moves(self.ActiveGame)
        return
    
    def right_mousebutton_clicked(self, idx):
        if self.panel.is_play_mode():
            if self.panel.current_color_on_move.get()==dc.WHITE_ON_MOVE:
                if self.panel.current_white_player.get()==dc.HUMAN_PLAYER:
                    self.play.move_proposal(idx, self.ActiveGame)
                else: self.beep()
            else:
                if self.panel.current_black_player.get()==dc.HUMAN_PLAYER:
                    self.play.move_proposal(idx, self.ActiveGame)
                else: self.beep()                                           
        else:
            self.panel.setup_mode()
            self.setup.set_black_piece(idx, self.ActiveGame)
        return
    
    def right_arrow_clicked(self, event):
        if self.panel.is_play_mode():
            self.beep()
        else:
            self.panel.setup_mode()
            self.re_play.right_arrow_clicked()
        return
    
    def control_right_arrow_clicked(self, event):
        if self.panel.is_play_mode():
            self.beep()
        else:
            self.panel.setup_mode()
            self.re_play.control_right_arrow_clicked()
        return
    
    def left_arrow_clicked(self, event):
        if self.panel.is_play_mode():
            self.beep()
        else:
            self.panel.setup_mode()
            self.re_play.left_arrow_clicked()
        return
    
    def control_left_arrow_clicked(self, event):
        if self.panel.is_play_mode():
            self.beep()
        else:
            self.panel.setup_mode()
            self.re_play.control_left_arrow_clicked()
        return
    
    def up_arrow_clicked(self, event):
        if self.panel.is_play_mode():
            self.beep()
        else:
            self.panel.setup_mode()
            self.re_play.up_arrow_clicked()
        return
    
    def control_up_arrow_clicked(self, event):
        if self.panel.is_play_mode():
            self.beep()
        else:
            self.panel.setup_mode()
            self.re_play.control_up_arrow_clicked()
        return
    
    def down_arrow_clicked(self, event):
        if self.panel.is_play_mode():
            self.beep()
        else:
            self.panel.setup_mode()
            self.re_play.down_arrow_clicked()
        return
    
    def control_down_arrow_clicked(self, event):
        if self.panel.is_play_mode():
            self.beep()
        else:
            self.panel.setup_mode()
            self.re_play.control_down_arrow_clicked()
        return
    
    def page_down_clicked(self,event):
        if self.panel.is_play_mode():
            self.beep()
        else:
            self.panel.setup_mode()
            self.re_play.page_down_clicked()
        return
        
    def page_up_clicked(self, event):
        if self.panel.is_play_mode():
            self.beep()
        else:
            self.panel.setup_mode()
            self.re_play.page_up_clicked()
        return
    
    def board_begin_position_clicked(self):
        if self.panel.is_play_mode():
            self.beep()
        else:
            self.panel.setup_mode()
            self.setup.setup_board()    
        return
    
    def board_clear_clicked(self):
        if self.panel.is_play_mode():
            self.beep()
        else:
            self.panel.setup_mode()
            self.setup.clear_board()
        return
    
    def file_open_game_clicked(self):
        if self.panel.is_setup_mode():
            self.read.get_pdn_file(False)
        else:
            self.beep()
        return
    
    def file_save_game_clicked(self):
        if self.panel.is_setup_mode():
            self.save.file_save_game_clicked()
        else:
            self.beep()
        return

    def set_begin_position_on_board(self):
        self.setup.set_begin_position_on_board()
        return

    def setup_board(self):
        self.setup.setup_board()
        return
    
    def new_game_clicked():
        self.Games = []
        game = GameRecord()
        self.Games.append(game)
        self.ActiveGame = game
        self.ActiveGameNumber = 0
        self.ActiveGame.ActivePanel = self.panel
        return
    
    def Get_Game_Info(self):
        self.game_panel = DraughtGamePanel(self)        
        return
        
    def Read_Game_Info(self):
        self.ActiveGame.WhitePlayer = self.game_panel.white_player_text.get()
        self.ActiveGame.BlackPlayer = self.game_panel.black_player_text.get()
        self.ActiveGame.Event = self.game_panel.tournament_text.get()
        self.ActiveGame.Date = datetime.today().date()                                      # PDN element
        self.ActiveGame.Round = self.game_panel.round_num_text.get()
        items = self.game_panel.result_list_box.curselection()
        if items:
            self.ActiveGame.Result =  str(self.game_panel.result_list_box.get(items[0]))
        self.ActiveGame.refresh_display()
        self.game_panel.root.destroy()
        return
        
    def Get_Time_Info(self):
        self.time_panel = DraughtTimeSetPanel(self, self.TotalGameTime, self.HandlingTime, self.TotalNumberOfMoves )
        return
    
    def Read_Time_Info(self):
        try:
            self.TotalGameTime = int(self.time_panel.main_time.get())
        except:
            self.TotalGameTime = 0
        try:
            self.HandlingTime = int(self.time_panel.handling_time.get())
        except:
            self.HandlingTime = 0
        try:
            self.TotalNumberOfMoves = int(self.time_panel.number_of_moves.get())
        except:
            self.TotalNumberOfMoves = 0
        
        self.TotalEngineTime = 60*self.TotalGameTime - self.TotalNumberOfMoves*self.HandlingTime
        if self.TotalEngineTime <= 0:
            print "problem with time, too much handling??" ,self.TotalEngineTime
        self.time_panel.root.destroy()
        return
    
    def beep(self):
        print "\a"
        return

    def program(self):
        main = DraughtMain()
        main.MainScheduler()     
        return
       

if __name__=='__main__':        
    main = DraughtMain()
    main.MainScheduler()
