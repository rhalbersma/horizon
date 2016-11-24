#
#  This file is Copyright (C) 2010 Feike Boomstra.
#  Distributed under the Boost Software License, Version 1.0.
#  (See accompanying file LICENSE_1_0.txt or copy at
#  http://www.boost.org/LICENSE_1_0.txt)
#


from DraughtDataStructures import *
import FileDialog
from Tkinter import *
import os
import copy
import symbol
from DraughtConstants import *

dc = DraughtConstants()

    # local variables
    
class ReadPDN:
    def __init__(self, main, root = None):
        if root==None:
            root=Tk()
        self.root          = root
        self.main          = main
        self.s             = ''                # the string with the input to analyse
        self.cp            = 0                 # current position pointer in this string
        self.current_color = dc.WHITE_ON_MOVE  # used to analyse begin position
        self.current_king  = False             # used to analyse begin position
        self.current_number = 0                # used in get digit
        self.Games         = []
        self.lastchrpnt    = 0
        self.debug = None
        return
    
    
    def get_pdn_file(self, test):
        self.s             = ''                # the string with the input to analyse
        self.cp            = 0                 # current position pointer in this string
        self.current_color = dc.WHITE_ON_MOVE  # used to analyse begin position
        self.current_king  = False             # used to analyse begin position
        self.current_number = 0                # used in get digit
        self.Games         = []
        self.lastchrpnt    = 0

        if test == True:
            self.fname = '/home/feike/BoomstraDam/pdn files/thijssen.pdn'
        else:
            self.fdlg = FileDialog.LoadFileDialog(self.root, title="Choose A PDN File")
            self.fname = self.fdlg.go(pattern="*.pdn", dir_or_file='/home/feike/BoomstraDam/pdn files') # opt args: dir_or_file=os.curdir, pattern="*", default="", key=None)

        self.file_open_flag = True
        try:
            self.f = open(self.fname, 'rb')
            self.s = self.f.read()
            self.s = self.s + '\0'
        except:
            self.file_open_flag = False
        
        if self.file_open_flag == True:
            self.f.close()
            
            self.main.Games = self.GetGames()

            if self.main.Games == None:
                self.main.ActiveGame = None
            else:
                self.main.ActiveGame = self.main.Games[0]

            self.main.ActiveGameNumber = 0

            if self.main.ActiveGame <> None:
                self.main.ActiveGame.refresh_display()
                self.main.ActiveGame.write_moves()
                self.main.panel.current_color_on_move.set(self.main.ActiveGame.BeginPosition.ColorOnMove)
                self.main.re_play.click(self.main.re_play.compose_tag([0, 0]))
            else:
                self.main.panel.notation.insert(END, "No valid PDN in File")
        return
    

    def GetGames(self):
        game = GameRecord(self.main, self.main.panel, self.main.panel)
        self.Games.append(game)
        cp = self.GetGame(0, game)
        if cp == 0: 
            self.Games.remove(game)
            print "Error in PDN, last char tried = ", self.lastchrpnt
            print self.s[cp-10:cp], self.s[self.lastchrpnt-70:self.lastchrpnt]
            return None

        while True:
            prev_cp = cp
            game = GameRecord(self.main, self.main.panel, self.main.panel)
            self.Games.append(game)
            cp = self.GetGame(cp, game)
            if cp == 0: 
                self.Games.remove(game)                
                break
        cp = self.SkipWhiteSpace(prev_cp)
        print "At the end: cp = ", cp, "chr = ", ord(self.s[cp])
        # print self.Games[0]
        
        if self.s[cp] == '\0':      # reached eof??
            print "reached eof"
            return self.Games
        else: 
            print "Error in PDN, last char tried = ", self.lastchrpnt
            print self.s[cp-10:cp], self.s[self.lastchrpnt-70:self.lastchrpnt]
            return None
        
            
            
    def GetGame(self, start, game):
        cp = self.GetHeader(start, game)
        if cp == 0: cp = self.GetGameBody(start, game)
        else: cp = self.GetGameBody(cp, game)
        if cp == 0: return 0
        game.CurrentPosition.WhiteMan = game.BeginPosition.WhiteMan[:]
        game.CurrentPosition.WhiteKing = game.BeginPosition.WhiteKing[:]
        game.CurrentPosition.BlackMan = game.BeginPosition.BlackMan[:]
        game.CurrentPosition.BlackKing = game.BeginPosition.BlackKing[:]
        
        return cp
    
    def GetHeader(self, start, game):
        cp = self.LogProgress("Get Header", start)
        self.InsertDefaultbeginPosition(game)
        while True:
            prev_cp = cp
            cp = self.GetHeaderLine(cp, game)
            if cp == 0: return prev_cp
            
    
    def InsertDefaultbeginPosition(self, game):
        game.BeginPosition.BlackMan = range( 1, 21)
        game.BeginPosition.Empty    = range(21, 31)
        game.BeginPosition.WhiteMan = range(31, 51)
        game.BeginPosition.ColorOnMove = dc.WHITE_ON_MOVE
        return
    
    def GetHeaderLine(self, start, game):
        cp = self.LogProgress("Get Header Line", start)
        cp = self.GetSimpleKeywordLine(cp, game)
        if cp == 0:
            cp = self.GetComment(start, game)
        return cp
                    
    def GetSimpleKeywordLine(self, start, game):
        cp = self.LogProgress("Get Simple Keyword Line", start)
        cp = self.GetOpenSquareBracket(start)
        if cp == 0: return 0
        word_start = self.SkipWhiteSpace(cp)
        word_end = self.GetWord(word_start)
        if word_end == word_start: return 0
        word = self.s[word_start:word_end]
        text_start = self.GetDoubleQuote(word_end)
        if text_start == 0: return 0
        text_end = self.GetKeywordText(text_start)
        if text_end == text_start: return 0
        kw_text = self.s[text_start:text_end]
        cp = self.GetDoubleQuote(text_end)
        if cp == 0: return 0
        cp = self.GetCloseSquareBracket(cp)
        if cp == 0: return 0
        #
        # We have collected a keyword and a keyword text:
        #
        if   word == 'Event':  game.Event       = kw_text
        elif word == 'Date':   game.Date        = kw_text
        elif word == 'Black':  game.BlackPlayer = kw_text
        elif word == 'White':  game.WhitePlayer = kw_text
        elif word == 'Site':   game. Site       = kw_text
        elif word == 'Result': game.Result      = kw_text
        elif word == 'Round':  game.Round       = kw_text
        elif word == 'GameType': game.GameType  = kw_text
        elif word == 'SetUp':  game.SetUp       = kw_text
        elif word == 'FEN':
            prev_cp = cp
            cp = self.GetBeginPosition(text_start, game)
            if cp == 0: return 0
            cp = prev_cp
        else:
            print "Found Keyword: ", word
            return 0
        return cp
    
    def GetBeginPosition(self, start, game):
        cp = self.LogProgress("Get Begin Position", start)

        game.BeginPosition.Empty = range(1, 51) # preset
        game.BeginPosition.WhiteMan = []
        game.BeginPosition.WhiteKing = []
        game.BeginPosition.BlackMan = []
        game.BeginPosition.BlackKing = []
        
        cp = self.GetColor(cp) # color of the turn
        if cp == 0: return 0        
        game.BeginPosition.ColorOnMove = self.current_color
        if self.debug: print dc.DCtoString(self.current_color)
        cp = self.GetColon(cp)
        if cp == 0: return 0
        cp = self.GetColor(cp)  # color to enter
        if cp == 0: return 0
        cp = self.GetSquare(cp, game)
        if cp == 0: return 0
        while True:
            prev_cp = cp
            cp = self.GetComma(cp)
            if cp == 0:
                cp = prev_cp
                break
            cp = self.GetSquare(cp, game)
            if cp == 0: return 0
        cp = self.GetColon(cp)
        if cp == 0: return 0
        cp = self.GetColor(cp)  # color to enter
        if cp == 0: return 0
        cp = self.GetSquare(cp, game)
        if cp == 0: return 0
        while True:
            prev_cp = cp
            cp = self.GetComma(cp)
            if cp == 0:
                cp = prev_cp
                break
            cp = self.GetSquare(cp, game)
            if cp == 0: return 0
    
        return cp
    
    def GetSquare(self, start, game):
        cp = self.GetOptionalKing(start)
        cp = self.GetDigits(cp)
        if cp == 0: return 0
        if self.current_number < 1 or self.current_number > 50 : return 0
        # valid square to enter:
        self.UpdateBeginPosition(game)
        return cp
    
    def UpdateBeginPosition(self, game):
        game.BeginPosition.Empty.remove(self.current_number)
        if self.current_color == dc.WHITE_ON_MOVE:
            if self.current_king == True:
                game.BeginPosition.WhiteKing.append(self.current_number)
            else: game.BeginPosition.WhiteMan.append(self.current_number)
        else:
            if self.current_king == True:
                game.BeginPosition.BlackKing.append(self.current_number)
            else: game.BeginPosition.BlackMan.append(self.current_number)
        return

    def GetColor(self, start):
        cp = self.LogProgress("Get Color", start)
        cp = self.SkipWhiteSpace(cp)
        if self.s[cp] == 'B' or self.s[cp] == 'b':
            self.current_color = dc.BLACK_ON_MOVE
            cp = self.Incr(cp)
            cp = self.SkipWhiteSpace(cp)
            return cp
        elif self.s[cp] == 'W' or self.s[cp] == 'w':
            self.current_color = dc.WHITE_ON_MOVE
            cp = self.Incr(cp)
            cp = self.SkipWhiteSpace(cp)
            return cp
        else:
            return 0

    def GetOptionalKing(self, start):
        cp = self.SkipWhiteSpace(start)
        if self.s[cp] == 'K' or self.s[cp] == 'k':
            self.current_king = True
            cp = self.Incr(cp)
            cp = self.SkipWhiteSpace(cp)
            return cp
        else:
            self.current_king = False
            return start

        
    def GetColon(self, start):
        cp = self.SkipWhiteSpace(start)
        if self.s[cp] == ':':
            cp = self.Incr(cp)
            cp = self.SkipWhiteSpace(cp)
            return cp
        else:
            return 0

    def GetComma(self, start):
        cp = self.SkipWhiteSpace(start)
        if self.s[cp] == ',':
            cp = self.Incr(cp)
            cp = self.SkipWhiteSpace(cp)
            return cp
        else:
            return 0

    def GetOpenSquareBracket(self, start):
        cp = self.SkipWhiteSpace(start)
        if self.s[cp] == '[':
            cp = self.Incr(cp)
            cp = self.SkipWhiteSpace(cp)
            return cp
        else:
            return 0

    def GetCloseSquareBracket(self, start):
        cp = self.SkipWhiteSpace(start)
        if self.s[cp] == ']':
            cp = self.Incr(cp)
            cp = self.SkipWhiteSpace(cp)
            return cp
        else:
            return 0

    def GetOpenCurlyBracket(self, start):
        cp = self.SkipWhiteSpace(start)
        if self.s[cp] == '{':
            cp = self.Incr(cp)
            cp = self.SkipWhiteSpace(cp)
            return cp
        else:
            return 0

    def GetCloseCurlyBracket(self, start):
        cp = self.SkipWhiteSpace(start)
        if self.s[cp] == '}':
            cp = self.Incr(cp)
            cp = self.SkipWhiteSpace(cp)
            return cp
        else:
            return 0

    def GetOpenRoundBracket(self, start):
        cp = self.SkipWhiteSpace(start)
        if self.s[cp] == '(' or self.s[cp] == '[':
            cp = self.Incr(cp)
            cp = self.SkipWhiteSpace(cp)
            return cp
        else:
            return 0

    def GetCloseRoundBracket(self, start):
        cp = self.SkipWhiteSpace(start)
        if self.s[cp] == ')' or self.s[cp] == ']':
            cp = self.Incr(cp)
            cp = self.SkipWhiteSpace(cp)
            return cp
        else:
            return 0

    def GetDoubleQuote(self, start):
        cp = self.SkipWhiteSpace(start)
        if self.s[cp] == '"':
            cp = self.Incr(cp)
            cp = self.SkipWhiteSpace(cp)
            return cp
        else:
            return 0
    
    def GetPeriod(self, start):
        cp = self.SkipWhiteSpace(start)
        if self.s[cp] == '.':
            cp = self.Incr(cp)
            cp = self.SkipWhiteSpace(cp)
            return cp
        else:
            return 0
    
    def GetMoveMoveOperator(self, start):
        cp = self.SkipWhiteSpace(start)
        if self.s[cp] == '-':
            cp = self.Incr(cp)
            cp = self.SkipWhiteSpace(cp)
            return cp
        else:
            return 0
    
    def GetMoveCaptureOperator(self, start):
        cp = self.SkipWhiteSpace(start)
        if self.s[cp] == 'x' or self.s[cp] == 'X':
            cp = self.Incr(cp)
            cp = self.SkipWhiteSpace(cp)
            return cp
        else:
            return 0
    
    def SkipWhiteSpace(self, start):
        cp = start
        while True:
            if   self.s[cp] == ' '      : cp = self.Incr(cp)
            elif self.s[cp] == '\t'     : cp = self.Incr(cp)
            elif ord(self.s[cp]) == 0xA: cp = self.Incr(cp)
            elif ord(self.s[cp]) == 0xD: cp = self.Incr(cp)
            else: return cp
                
    def GetWord(self, start):
        cp = start
        while True:
            prev_cp = cp
            if self.IsLetter(self.s[cp]) == True:
                cp = self.Incr(cp)
            elif self.IsNumber(self.s[cp]) == True:
                cp = self.Incr(cp)
            else: return prev_cp
        return
    
    def GetKeywordText(self, start):
        cp = self.LogProgress("Get Keyword Text", start)
        while True:
            prev_cp = cp
            if self.IsOkay(self.s[cp]) == True:
                cp = self.Incr(cp)
            else: return prev_cp
        return
        
    def IsOkay(self, chr):
        if chr == '"': return False
        if chr == ']': return False
        if chr == '[': return False
        if chr == '\0': return False
        return True
    
    
    def IsLetter(self, chr):
        return (ord('a') <= ord(chr) and ord(chr) <= ord('z')) or \
               (ord('A') <= ord(chr) and ord(chr) <= ord('Z'))

    def IsNumber(self, chr):
        return (ord('0') <= ord(chr) and ord(chr) <= ord('9'))
    
    def IsToken(self, chr):
        temp = False
        if chr == '-': temp = True
        if chr == '+': temp = True
        if chr == '.': temp = True
        if chr == ',': temp = True
        if chr == ':': temp = True
        if chr == ' ': temp = True
        if chr == '!': temp = True
        return temp
    
    def GetDigits(self, start):
        cp = start
        if self.IsNumber(self.s[cp]) == True:
            cp = self.Incr(cp)
        if self.IsNumber(self.s[cp]) == True:
            cp = self.Incr(cp)
        if self.IsNumber(self.s[cp]) == True:
            cp = self.Incr(cp)
        if cp == start: return 0
        self.current_number = int(self.s[start:cp])
        return cp
    
    def GetWhiteVariant(self, start, game, half_ply):
        cp = self.LogProgress("Get White Variant", start)
        cp = self.GetOpenRoundBracket(cp)
        if cp == 0: return 0
        new_game = GameRecord(self.main, '', self.main.panel, game)
        moves_end = self.GetWhiteStartMoves(cp, new_game)
        if moves_end == cp: # no moves            
            return 0
        cp = self.GetCloseRoundBracket(moves_end)
        if cp == 0: return 0
        half_ply.Variant = new_game
        return cp
    
    def GetBlackVariant(self, start, game, half_ply):
        cp = self.LogProgress("Get Black Variant", start)
        cp = self.GetOpenRoundBracket(cp)
        if cp == 0: return 0
        new_game = GameRecord(self.main, '', self.main.panel, game)
        moves_end = self.GetBlackStartMoves(cp, new_game)
        if moves_end == cp: # no moves
            return 0
        cp = self.GetCloseRoundBracket(moves_end)
        if cp == 0: return 0
        half_ply.Variant = new_game
        return cp
    
    def GetGameBody(self, start, game):
        if self.debug: print "GetGameBody:", dc.DCtoString(game.BeginPosition.ColorOnMove)
        if game.BeginPosition.ColorOnMove == dc.WHITE_ON_MOVE:
            cp = self.GetWhiteStartGameBody(start, game)
            if cp==0: return 0
            cp = self.GetTerminator(cp, game)

        else:
            cp = self.GetBlackStartGameBody(start, game)
            if cp==0: return 0
            cp = self.GetTerminator(cp, game)
        return cp
    
    def GetWhiteStartGameBody(self, start, game):
        cp = self.LogProgress("Get White Start game Body", start)
        moves_end = self.GetWhiteStartMoves(cp, game)
        #if moves_end == cp: # no moves
        #   return 0
        return moves_end 
    
    def GetBlackStartGameBody(self, start, game):
        cp = self.LogProgress("Get Black Start game Body", start)
        moves_end = self.GetBlackStartMoves(cp, game)
        return moves_end 
    
    def GetWhiteStartMoves(self, start, game):
        cp = self.LogProgress("Get White Start Moves", start)
        while True:
            prev_cp = cp
            cp = self.GetCompletePly(cp, game)
            if cp == 0:
                break
        cp = self.GetWhiteHalfPly(prev_cp, game)
        if cp == 0: 
            if prev_cp == start:  # no half ply at all
                return 0
            else: 
                return prev_cp            
        else: 
            return cp
    
    def GetBlackStartMoves(self, start, game):
        cp = self.LogProgress("Get Black Start Moves", start)
        cp = self.GetBlackHalfPly(cp, game)
        if cp == 0: return 0
        while True:
            prev_cp = cp
            cp = self.GetCompletePly(cp, game)
            if cp == 0:
                break
        cp = self.GetWhiteHalfPly(prev_cp, game)
        if cp == 0: return prev_cp
        else: return cp            
    
    def GetWhiteHalfPly(self, start, game):
        cp = self.LogProgress("Get White Half Ply", start)
        move = PlyRecord()
        game.Moves.append(move)
        prev_cp = cp
        cp = self.GetMoveSequence(cp, move)
        if cp == 0: cp = prev_cp
        cp = self.GetWhiteMove(cp, move)
        if cp == 0:
            game.Moves.remove(move)
            return 0
        last_cp = cp
        cp = self.GetComment(cp, move.WhiteMove)
        if cp == 0: cp = last_cp
        return cp

    def GetBlackHalfPly(self, start, game):
        cp = self.LogProgress("Get Black half Ply", start)
        move = PlyRecord()
        game.Moves.append(move)
        prev_cp = cp
        cp = self.GetMoveSequence(cp, move)
        if cp == 0: cp = prev_cp
        if self.s[cp:cp+2] == "..": cp = cp + 2
        cp = self.GetBlackMove(cp, move)
        if cp == 0: 
            game.Moves.remove(move)
            return 0
        return cp

    
    def GetCompletePly(self, start, game):
        cp = self.LogProgress("Get Complete Ply", start)
        move = PlyRecord()
        game.Moves.append(move)
        prev_cp = cp
        cp = self.GetMoveSequence(cp, move)
        if cp == 0: cp = prev_cp
        cp = self.GetWhiteMove(cp, move)
        if cp == 0: 
            game.Moves.remove(move)
            return 0
        cp = self.SkipWhiteSpace(cp)
        prev_cp = cp
        cp = self.GetComment(cp, move.WhiteMove)
        if cp == 0: cp = prev_cp
        prev_cp = cp
        cp = self.GetWhiteVariant(cp, game, move.WhiteMove)
        if cp == 0: cp = prev_cp
        else: cp = self.GetRestartLeader(cp, move)
        prev_cp = cp
        #
        cp = self.GetBlackMove(cp, move)
        if cp == 0:
            game.Moves.remove(move) 
            return 0
        cp = self.SkipWhiteSpace(cp)
        prev_cp = cp
        cp = self.GetComment(cp, move.BlackMove)        
        if cp == 0: cp = prev_cp
        prev_cp = cp
        cp = self.GetBlackVariant(cp, game, move.BlackMove)
        if cp == 0: cp = prev_cp
        else: cp = self.GetRestartLeader(cp, move)
        return cp
    
    def GetRestartLeader(self, start, move):
        cp = self.LogProgress("Get Restart Leader", start)
        cp = self.GetMoveSequence(cp, move)
        if self.s[cp:cp+2] == "..": cp = cp + 2
        return cp

    
    def GetMoveSequence(self, start, move):
        cp = self.LogProgress("Get Move Sequence Number", start)
        cp = self.SkipWhiteSpace(cp)
        cp = self.GetDigits(cp)
        if cp == 0: return 0
        cp = self.GetPeriod(cp)
        if cp == 0: return 0
        if self.current_number < 1: return 0
        move.Ply_nr = self.current_number
        return cp

    def GetWhiteMove(self, start, move):
        cp = self.LogProgress("Get White Move", start)
        cp = self.SkipWhiteSpace(cp)
        white_move = HalfPlyRecord()
        cp = self.GetHalfPly(cp, white_move)
        if cp == 0: return 0
        if self.s[cp:cp+2] == '+-' : cp += 2
        if self.s[cp] == '?' or self.s[cp] == '!': cp += 1
        if self.s[cp] == '?' or self.s[cp] == '!': cp += 1
        move.WhiteMove = white_move
        return cp
    
    def GetHalfPly(self, start, half_ply):
        cp = self.LogProgress("Get Half Ply", start)
        cp = self.SkipWhiteSpace(cp)
        cp = self.GetDigits(cp)
        if cp == 0: return 0
        if self.current_number < 1 or self.current_number > 50 : return 0
        half_ply.Start = self.current_number
        half_ply.Step.append(self.current_number)
        cp = self.SkipWhiteSpace(cp)
        prev_cp = cp
        cp = self.GetMoveMoveOperator(cp)
        if cp <> 0:     # it is a move
            cp = self.SkipWhiteSpace(cp)
            cp = self.GetDigits(cp)
            if cp == 0: return 0
            if self.current_number < 1 or self.current_number > 50 : return 0
            half_ply.Stop = self.current_number
            if half_ply.Start == half_ply.Stop: return 0
            half_ply.Step.append(self.current_number)
            half_ply.op = '-'
            return cp
        else:   # is a capture ??
            cp = self.GetCapLeg(prev_cp, half_ply)
            if cp == 0: return 0  # at least one leg needed
            while True:
                prev_cp = cp
                cp = self.GetCapLeg(cp, half_ply)
                if cp == 0: return prev_cp
                
                
    def GetTerminator(self, start, game):
        cp = self.LogProgress("Get Terminator", start)
        cp = self.SkipWhiteSpace(cp)
        game.Result = self.s[cp:cp+3]  # maybe
        if self.s[cp:cp+3] == '1-0': return cp+3
        if self.s[cp:cp+3] == '0-0': return cp+3
        if self.s[cp:cp+3] == '0-1': return cp+3
        if self.s[cp:cp+3] == '2-0': return cp+3
        if self.s[cp:cp+3] == '0-2': return cp+3
        if self.s[cp:cp+3] == '1-1': return cp+3
        game.Result = self.s[cp:cp+7]
        if self.s[cp:cp+7] == '1/2-1/2': return cp+7
        game.Result = ''
        if self.s[cp] == '*':        return cp + 1
        return 0
        

    def GetCapLeg(self, start, half_ply):
        cp = self.LogProgress("Get Cap Leg", start)
        cp = self.SkipWhiteSpace(start)
        cp = self.GetMoveCaptureOperator(cp)
        if cp == 0: return 0
        cp = self.SkipWhiteSpace(cp)
        cp = self.GetDigits(cp)
        if cp == 0: return 0
        if self.current_number < 1 or self.current_number > 50 : return 0
        if self.s[cp:cp+2] == '+-' : cp += 2
        if self.s[cp] == '?' or self.s[cp] == '!': cp += 1
        if self.s[cp] == '?' or self.s[cp] == '!': cp += 1
        half_ply.Stop = self.current_number
        half_ply.Step.append(self.current_number)
        half_ply.op = 'x'
        return cp
        

    def GetComment(self, start, half_ply):
        cp = self.LogProgress("Get Comment", start)
        cp = self.SkipWhiteSpace(cp)
        text_start = cp
        cp = self.GetOpenCurlyBracket(cp)
        if cp == 0: 
                cp = self.LogProgress("Verder onleesbaar?", text_start)
                if self.s[text_start:text_start+18] ==  'verder onleesbaar.': 
                    half_ply.Comment = self.s[text_start:text_start+18] 
                    return text_start+18
                else:
                    return 0
        text_start = cp
        text_end = self.GetCommentText(cp)
        cp = self.SkipWhiteSpace(text_end)
        cp = self.GetCloseCurlyBracket(cp)
        if cp == 0:  return 0
        half_ply.Comment = self.s[text_start:text_end]
        return cp
    
    def GetCommentText(self, cp):
        while True:
            prev_cp = cp
            if self.s[cp] == "}" or self.s[cp] == '\0':
                return prev_cp
            cp = self.Incr(cp)
            

    def GetBlackMove(self, start, move):
        cp = self.LogProgress("Get Black Move", start)
        cp = self.SkipWhiteSpace(cp)
        black_move = HalfPlyRecord()
        cp = self.GetHalfPly(cp, black_move)
        if cp == 0: return 0
        if self.s[cp:cp+2] == '+-' : cp += 2
        if self.s[cp] == '?' or self.s[cp] == '!'  : cp += 1
        if self.s[cp] == '?' or self.s[cp] == '!'  : cp += 1
        move.BlackMove = black_move
        return cp

    def Incr(self, cp):
        cp = cp + 1
        if cp > self.lastchrpnt:
            self.lastchrpnt = cp
        return cp
    
    def LogProgress(self, name, start):
        if self.debug:
            print name, "cp = ", start, "ts[..] = ", self.s[start:min(start+5, len(self.s))]
        return start
    
    
    
if __name__=='__main__':
    
    test=ReadPDN(self)
    s_test = '[Event "Nieuwe Parser"][FEN "W:WK1,K2,3,4,5:B50,49,48" ]1. 10-15 {gekke zet} 16-21 2. 3x15 (5 x 12 )40x30x20 1-0'
#    print test.lastchrpnt
    
    test.get_pdn_file(False)
    test_games = test.GetGames()
    for game in test_games: print game
    
    test.f.close()
#    print test.bord.Games[0].Moves[0]
