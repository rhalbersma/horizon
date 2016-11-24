#
#  This file is Copyright (C) 2010 Feike Boomstra.
#  Distributed under the Boost Software License, Version 1.0.
#  (See accompanying file LICENSE_1_0.txt or copy at
#  http://www.boost.org/LICENSE_1_0.txt)
#


class HalfPlyRecord:
    """ To store information about a move (= 1/2 ply)
    
    Info stored:    start   the begin position of the move or capture
                    stop    the end position of the move or capture
                    cap     a list with all captures (field nrs)
                    step    a list of all steps in between in case of a capture
                    Unique  start/stop provides a unique move seqence
                    MoveValue obtained from analysing the move upto a certain depth
    """
    
    def __init__(self):
        self.start = ''
        self.stop = ''
        self.cap = []
        self.step = []
        self.Unique = True
        self.MoveValue = 0
        self.move_op = '-'
        return
    
    def __str__(self):
        return "Start.:" + str(self.start) + '\n' +\
                "Stop..:" + str(self.stop) +'\n' +\
                "Cap...:" + str(self.cap) + '\n' +\
                "Step..:" + str(self.step) + '\n' +\
                "unique:" + str(self.Unique) + '\n' +\
                "MoveValue:" + str(self.MoveValue) + '\n' +\
               "move_op: " + self.move_op + '\n' 
                
    def __lt__(self, other):
        return self.MoveValue < other.MoveValue
    
    def __le__(self, other):
        return self.MoveValue <= other.MoveValue
    
    def __eq__(self, other):
        return self.MoveValue == other.MoveValue
    
    def __ge__(self, other):
        return self.MoveValue >= other.MoveValue
    
    def __gt__(self, other):
        return self.MoveValue > other.MoveValue
    
    
