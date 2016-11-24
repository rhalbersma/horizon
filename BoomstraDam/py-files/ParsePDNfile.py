#
#  This file is Copyright (C) 2010 Feike Boomstra.
#  Distributed under the Boost Software License, Version 1.0.
#  (See accompanying file LICENSE_1_0.txt or copy at
#  http://www.boost.org/LICENSE_1_0.txt)
#


import FileDialog
from Tkinter import *
import os
import copy
import symbol

    
class ParsePDNfile:
    def __init__(self, bord, root=None):
        if root==None:
            root=Tk()
            root.geometry('600x410+350+70')

        self.root=root
        self.bord = bord        
        self.bord.game_pnt = -1        # initialize
        self.file_open_flag = False
        return

    def get_pdn_file(self, test):
        if test:
            ok = self.quick_open_file()
        else:
            ok = self.open_pdn_file()
        if ok == True:
            ok = self.final_state()
        return ok
    
    def quick_open_file(self):
        
        #self.fdlg = FileDialog.LoadFileDialog(self.root, title="Choose A PDN File")
        #self.fname = self.fdlg.go(pattern="*.pdn", dir_or_file=os.curdir) # opt args: dir_or_file=os.curdir, pattern="*", default="", key=None)
        self.fname = os.curdir + '\\pdn files\\thijssen.pdn'#                   DUTCH96H.PDN'
        #print self.fname
        self.chr_pnt=0
        self.entire_file=None
        self.file_open_flag = True
        try:
            self.f=open(self.fname, 'rb')
            self.entire_file=self.f.read()
        except:
            self.file_open_flag = False
        return self.file_open_flag

    def open_pdn_file(self):
        
        self.fdlg = FileDialog.LoadFileDialog(self.root, title="Choose A PDN File")
        self.fname = self.fdlg.go(pattern="*.pdn", dir_or_file=os.curdir + '\\pdn files') # opt args: dir_or_file=os.curdir, pattern="*", default="", key=None)
        print self.fname
        self.chr_pnt=0
        self.entire_file=None
        self.file_open_flag = True
        try:
            self.f=open(self.fname, 'rb')
            self.entire_file=self.f.read()
        except:
            self.file_open_flag = False
        return self.file_open_flag
    
    def get_next_char(self):
        if self.chr_pnt < len(self.entire_file):
            c = self.entire_file[self.chr_pnt]
            if c == '\n':
                c = ' '
            elif c == '\0':
                c = ' '
            elif c == '':
                c = ' '
            elif ord(c) == 0x0A:
                c = ' '
            elif ord(c) == 0x0D:
                c = ' '
            self.chr_pnt = self.chr_pnt + 1
        else:
            c = None
        return c



    # tokens:   digit = [0,1,2,3,4,5,6,7,8,9]
    #           point = '.'
    #           alpha = [A ..Z, a..z]
    #           opensquare = '['
    #           closesquare = ']'
    #           quote = '"'
    #           dash = '-'
    #           slash = '/'
    #           times = 'x'
    #           seperator = ' '
    #           ignore = '\n' , '\0'
    #           digits = digit | digit digit
    #           ply_nr = digits point
    #           keyword = 'White', 'Black', 'Event', 'Date', 'Result', 'GameType'
    #           result = '1-0', '0-1' '1/2-1/2' '0-0'
    #           ply_op = dash | times
    #           ply = digits ply_op digits
    #entities:  keyword_chunk : opensquare keyword quote value quote closesquote
    #           ply_chunk : ply_nr ply ply
    #           last_ply_chunk     : ply_nr (result | ply result  ply ply result)
    #game:      keyword_chunk [keyword_chunk]
    #           ply_chunk [ply_chunk]
    #           last_ply_chunk
    
    # I will try to implement it with a single pass final state machine, let's see.
    def process_keyword_chunk(self, keyword, keyword_value):
        if keyword == 'White':
            self.bord.Games[self.bord.game_pnt].White = keyword_value[:]
        elif keyword == 'Black':
            self.bord.Games[self.bord.game_pnt].Black = keyword_value[:]
        elif keyword == 'Event':
            self.bord.Games[self.bord.game_pnt].Event = keyword_value[:]
        elif keyword == 'Date':
            self.bord.Games[self.bord.game_pnt].Date = keyword_value[:]
        elif keyword == 'Result':
            self.bord.Games[self.bord.game_pnt].Result = keyword_value[:]
        elif keyword == 'Round':
            self.bord.Games[self.bord.game_pnt].Round = keyword_value[:]
        elif keyword == 'Site':
            self.bord.Games[self.bord.game_pnt].Site = keyword_value[:]
        else:
            if keyword == 'GameType':
                if keyword_value == '20':
                    return True
                else: return False
            else: 
                print "found illegal keyword:", keyword
                return False
        return True
    
    def proces_white_move_comment(self, comment):
        self.bord.Games[self.bord.game_pnt].Moves[-1].White_comment = comment
        return
    
    def proces_black_move_comment(self,comment):
        self.bord.Games[self.bord.game_pnt].Moves[-1].Black_comment = comment
        return
    
    def create_new_game(self):
        self.bord.game_pnt = self.bord.game_pnt + 1
        g = GameRecord()
        self.bord.Games.append(g)
        self.move_pnt = -1
        return
    
    def is_result(self, start_move, ply_op, stop_move):
        # is it a result: 1-0, 0-1, 0-0 or a move
        # if a move, process it
        if start_move == '0' and ply_op == '-' and stop_move == '1': return True
        if start_move == '0' and ply_op == '-' and stop_move == '0': return True
        if start_move == '1' and ply_op == '-' and stop_move == '0': return True
        if start_move == '2' and ply_op == '-' and stop_move == '0': return True
        if start_move == '1' and ply_op == '-' and stop_move == '1': return True
        if start_move == '0' and ply_op == '-' and stop_move == '2': return True
        return False

    def is_black_result(self, start_move, ply_op, stop_move):
        # is it a result: 1-0, 0-1, 0-0 or a move
        # if a move, process it
        if start_move == '0' and ply_op == '-' and stop_move == '1': return True
        if start_move == '0' and ply_op == '-' and stop_move == '0': return True
        if start_move == '1' and ply_op == '-' and stop_move == '0': return True
        # black move
        self.bord.Games[self.bord.game_pnt].Moves[-1].Black_start = int(start_move)
        self.bord.Games[self.bord.game_pnt].Moves[-1].Black_stop = int(stop_move)
        self.bord.Games[self.bord.game_pnt].Moves[-1].Black_op = ply_op
        return False

    def is_white_result(self, start_move, ply_op, stop_move, ply_nr):
        # is it a result: 1-0, 0-1, 0-0 or a move
        # if a move, process it
        if start_move == '0' and ply_op == '-' and stop_move == '1': return True
        if start_move == '0' and ply_op == '-' and stop_move == '0': return True
        if start_move == '1' and ply_op == '-' and stop_move == '0': return True
        # it is a white move
        m = MoveRecord()
        m.White_start = int(start_move)
        m.White_stop = int(stop_move)
        m.White_op = ply_op
        m.Ply_nr = int(ply_nr)
        self.bord.Games[self.bord.game_pnt].Moves.append(m)
        self.move_pnt = self.move_pnt + 1
        return False
    
    def set_status_false(self, status):
        self.previous_status.append(status)
        return 'false'
    
    def set_status(self, status, string):
        self.previous_status.append(status)
        return string
            
    def final_state(self):
        # local variables
        if not self.file_open_flag: 
            print "flag!"
            return []
        self.previous_status = []
        status = 'idle'
        keyword =''
        keyword_value = ''
        start_white_move = ''
        stop_white_move = ''
        white_ply_op = ''
        black_ply_op = ''
        start_black_move = ''
        stop_black_move = ''
        white_move_comment = ''
        black_move_comment = ''

        self.bord.Games = []        # reset Games/Moves lists
        self.bord.game_pnt = -1
        self.bord.current_half_ply_counter = 1
        
        while True:        
            c = self.get_next_char()
            if c == None: break
            if status == 'idle':
                if c == '[':
                    status = self.set_status(status,  'keyword')
                    self.create_new_game()
                    keyword = ""
                elif c == ' ' :
                    pass
                elif c in ['0','1','2','3','4','5','6','7','8','9']:
                    status = self.set_status(status,  'second ply digit')
                    ply_nr = c
                else:
                    status = self.set_status_false(status)
            elif status == 'idle-2':
                if c == '[':
                    status = self.set_status(status,  'keyword')
                    keyword = ""
                elif c == ' ' :
                    pass
                elif c in ['0','1','2','3','4','5','6','7','8','9']:
                    status = self.set_status(status,  'second ply digit')
                    ply_nr = c
                else:
                    status = self.set_status_false(status)
#
# from idle we can go to
# to keyword or
# second ply digit
#                    
            elif status == 'keyword':
                if c == ' ':
                    pass
                elif c == '"':
                    status = self.set_status(status,  'get_keyword_value')
                    keyword_value = ''
                else:
                    keyword = keyword + c
            elif status == 'get_keyword_value':
                if c == '"':
                    status = self.set_status(status,  'wait for close bracket')
                else:
                    keyword_value = keyword_value + c
            elif status == 'wait for close bracket':
                if c == ' ':
                    pass
                elif c == "]":
                    status = self.set_status(status,  'idle-2')
                    self.process_keyword_chunk(keyword, keyword_value)
                else:
                    status = self.set_status_false(status)
#
# here now the ply nr
#                    
            elif status == 'second ply digit':
                if c in ['0','1','2','3','4','5','6','7','8','9']:
                    status = self.set_status(status,  'ply point')
                    ply_nr = ply_nr +c
                elif c == '.':
                    status = self.set_status(status,  'start white first digit')
                else:
                    status = self.set_status_false(status)
            elif status == 'second ply digit':
                if c == '.':
                    status = self.set_status(status,  'start white first digit')
                else:
                    status = self.set_status_false(status)
#
# the white move
#
            elif status == 'start white first digit':
                if c == ' ':
                    pass
                elif c in ['0','1','2','3','4','5','6','7','8','9']:
                    status = self.set_status(status,  'start white second digit')
                    start_white_move = c
                elif c == '-':
                    status = self.set_status(status,  'stop white first digit')
                    white_ply_op = c
                elif c == 'x':
                    status = self.set_status(status,  'stop white first digit')
                    white_ply_op = c
                elif c == '/':
                    if start_white_move != '1':
                        status = self.set_status_false(status)
                    else:
                        status = self.set_status(status,  'result 1/')
                else:
                    status = self.set_status_false(status)
            elif status == 'start white second digit':
                if c in ['0','1','2','3','4','5','6','7','8','9']:
                    status = self.set_status(status, 'white ply op')
                    start_white_move = start_white_move + c
                elif c == '-':
                    status = self.set_status(status, 'stop white first digit')
                    white_ply_op = c
                elif c == 'x':
                    status = self.set_status(status, 'stop white first digit')
                    white_ply_op = c
                else:
                    status = self.set_status_false(status)
            elif status == 'white ply op':                
                if c == ' ':
                    pass
                elif c == '-':
                    status = self.set_status(status, 'stop white first digit')
                    white_ply_op = c
                elif c == 'x':
                    status = self.set_status(status, 'stop white first digit')
                    white_ply_op = c
                else:
                    status = self.set_status_false(status)
            elif status == 'stop white first digit':
                if c == ' ':
                    pass
                elif c in ['0','1','2','3','4','5','6','7','8','9']:
                    status = self.set_status(status, 'stop white second digit')
                    stop_white_move = c
                else:
                    status = self.set_status_false(status)
            elif status == 'stop white second digit':
                if c in ['0','1','2','3','4','5','6','7','8','9']:
                    stop_white_move = stop_white_move + c
                    if self.is_white_result(start_white_move, white_ply_op, stop_white_move, ply_nr):
                        status = self.set_status(status, 'ready')
                    else:
                        status = self.set_status(status, 'start black first digit')
                if c == ' ':
                    if self.is_white_result(start_white_move, white_ply_op, stop_white_move, ply_nr):
                        status = self.set_status(status, 'ready')
                    else:
                        status = self.set_status(status, 'start black first digit')
                    
                    
#
# the black move
#
            elif status == 'start black first digit':
                if c == ' ':
                    pass
                elif c in ['0','1','2','3','4','5','6','7','8','9']:
                    status = self.set_status(status, 'start black second digit')
                    start_black_move = c
                elif c == '-':
                    status = self.set_status(status, 'stop black first digit')
                    black_ply_op = c
                elif c == 'x':
                    status = self.set_status(status, 'stop black first digit')
                    black_ply_op = c
                elif c == '{':
                    status = self.set_status(status, 'white move comment')
                    white_move_comment = ''
                else:
                    status = self.set_status_false(status)
            elif status == 'start black second digit':
                if c in ['0','1','2','3','4','5','6','7','8','9']:
                    status = self.set_status(status, 'black ply op')
                    start_black_move = start_black_move + c
                elif c == '-':
                    status = self.set_status(status, 'stop black first digit')
                    black_ply_op = c
                elif c == 'x':
                    status = self.set_status(status, 'stop black first digit')
                    black_ply_op = c
                elif c == '/':
                    if start_black_move != '1':
                        status = self.set_status_false(status)
                    else:
                        status = self.set_status(status, 'result 1/')
                else:
                    status = self.set_status_false(status)
            elif status == 'black ply op':                
                if c == ' ':
                    pass
                elif c == '-':
                    status = self.set_status(status, 'stop black first digit')
                    black_ply_op = c
                elif c == 'x':
                    status = self.set_status(status, 'stop black first digit')
                    black_ply_op = c
                else:
                    status = self.set_status_false(status)
            elif status == 'stop black first digit':
                if c == ' ':
                    pass
                elif c in ['0','1','2','3','4','5','6','7','8','9']:
                    status = self.set_status(status, 'stop black second digit')
                    stop_black_move = c
                else:
                    status = self.set_status_false(status)
            elif status == 'stop black second digit':
                if c in ['0','1','2','3','4','5','6','7','8','9']:
                    stop_black_move = stop_black_move + c
                    if self.is_black_result(start_black_move, black_ply_op, stop_black_move):
                        status = self.set_status(status, 'ready')
                    else: 
                        status = self.set_status(status, 'ply nr or result')
                elif c == ' ':
                    if self.is_black_result(start_black_move, black_ply_op, stop_black_move):
                        status = self.set_status(status, 'ready')
                    else: 
                        status = self.set_status(status, 'ply nr or result')
                else:
                    status = self.set_status_false(status)
#
# we have the black move
#                    
            elif status == 'ply nr or result':
                if c == ' ':
                    pass
                elif c in ['0','1','2','3','4','5','6','7','8','9']:
                    status = self.set_status(status, 'point or dash')
                    ply_nr = c
                elif c == '{':
                    status = self.set_status(status, 'black move comment')
                    black_move_comment = ''
                else:
                    status = self.set_status_false(status)
            elif status == 'point or dash':                
                if c == '.':
                    status = self.set_status(status, 'start white first digit')
                elif c == '-':
                    status = self.set_status(status, 'second result digit')
                    black_ply_op = c
                elif c == '/':
                    if ply_nr != '1':
                        status = self.set_status_false(status)
                    else:
                        status = self.set_status(status,  'result 1/')
                elif c in ['0','1','2','3','4','5','6','7','8','9']:
                    ply_nr = ply_nr + c
                    status = self.set_status(status, 'point')
                else:
                    status = self.set_status_false(status)
            elif status == 'point':
                if c == '.':
                    status = self.set_status(status, 'start white first digit')
                else:
                    status = self.set_status_false(status)
            elif status == 'second result digit':
                if c in ['0','1','2','3','4','5','6','7','8','9']:
                    start_black_move = c
                    if self.is_result(ply_nr, black_ply_op, start_black_move):
                        status = self.set_status(status, 'ready')
                    else:
                        status = self.set_status_false(status)                
                else:
                    status = self.set_status_false(status)
            elif status == 'result 1/':
                if c == '2':
                    status = self.set_status(status, 'result 1/2')
                else:
                    status = self.set_status_false(status)
            elif status == 'result 1/2':
                if c == '-':
                    status = self.set_status(status, 'result 1/2-')
                else:
                    status = self.set_status_false(status)
            elif status == 'result 1/2-':
                if c == '1':
                    status = self.set_status(status, 'result 1/2-1')
                else:
                    status = self.set_status_false(status)
            elif status == 'result 1/2-1':
                if c == '/':
                    status = self.set_status(status, 'result 1/2-1/')
                else:
                    status = self.set_status_false(status)
            elif status == 'result 1/2-1/':
                if c == '2':
                    status = self.set_status(status, 'ready')
                else:
                    status = self.set_status_false(status)
            elif status == 'white move comment':
                if c == '}':
                    status = self.set_status(status, 'start black first digit')
                    self.proces_white_move_comment(white_move_comment)
                else:
                    white_move_comment = white_move_comment + c
            elif status == 'black move comment':
                if c == '}':
                    status = self.set_status(status, 'ply nr or result')
                    self.proces_black_move_comment(white_move_comment)
                else:
                    black_move_comment = black_move_comment + c
            elif status == 'ready':
                #self.print_game()
                #print "from ready :            :", self.bord.Games[self.bord.game_pnt].Move[self.move_pnt-1].White_start
                
                status = 'idle'
#
# here we catch the leftovers
#                   
            else: 
                status = self.set_status_false(status)




            if status == 'false':
                break    
        # end while      
        if c != None:            
            print "last status = ", self.previous_status[-20:], "last char = |", c , "| chr_pnt = ", self.chr_pnt
            print "last part: ", self.entire_file[self.chr_pnt-40:self.chr_pnt]
            print "ord(c) = ", ord(c)          
            return False
        else:
            pass
            #print "pdn analyzed"
        
        return True



if __name__=='__main__':
    
    test=ParsePDNfile()
    test.get_pdn_file(True)
    test.f.close()
    print test.bord.Games[0].Moves[0]
