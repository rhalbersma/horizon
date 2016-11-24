#
#  This file is Copyright (C) 2010 Feike Boomstra.
#  Distributed under the Boost Software License, Version 1.0.
#  (See accompanying file LICENSE_1_0.txt or copy at
#  http://www.boost.org/LICENSE_1_0.txt)
#


class DraughtConstants:
    def __init__(self):
        self.EMPTY_FIELD = 1
        self.WHITE_MAN = 2
        self.BLACK_MAN = 3
        self.WHITE_KNIGHT = 4
        self.BLACK_KNIGHT = 5
        
        self.WHITE_ON_MOVE = 6
        self.BLACK_ON_MOVE = 7
        
        self.SETUP_MODE = 8
        self.PLAY_MODE = 9
        self.ANAL_POS_MODE = 10
        self.ANAL_TIME_MODE = 14
        self.EVAL_MODE = 15
        self.RE_PLAY_MODE = 16
        
        self.COMPUTER = 11
        self.HUMAN_PLAYER = 12
        self.REMOTE_PLAYER = 13
        
        self.SOUND_OFF = 0
        self.SOUND_ON = 1
        self.SOUND_TWO = 2
        
        return
    
    def DCtoString(self, i):
        table = {self.EMPTY_FIELD   : "Empty field",
                self.WHITE_MAN      : "White man", 
                self.BLACK_MAN      : "Black man",                 
                self.WHITE_KNIGHT   : "White knight",
                self.BLACK_KNIGHT   : "Black knight",
                
                self.WHITE_ON_MOVE  : "White on move",
                self.BLACK_ON_MOVE  : "Black on move",
                
                self.SETUP_MODE     : "Setup mode",
                self.PLAY_MODE      : "Play mode",                
                self.ANAL_POS_MODE  : "Anal pos mode",
                self.ANAL_TIME_MODE : "Anal time mode",
                self.EVAL_MODE      : "Eval mode",
                self.RE_PLAY_MODE   : "Re-play mode",
                self.COMPUTER       : "Computer",
                self.HUMAN_PLAYER   : "Human player",
                self.REMOTE_PLAYER  : "Remote player" }
                
        if table.has_key(i):
            return table[i]
        else: return "can't convert" + str(i)
        
        
    