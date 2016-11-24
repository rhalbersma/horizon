#
#  This file is Copyright (C) 2010 Feike Boomstra.
#  Distributed under the Boost Software License, Version 1.0.
#  (See accompanying file LICENSE_1_0.txt or copy at
#  http://www.boost.org/LICENSE_1_0.txt)
#


from DraughtConstants import *
from socket import *
import sys
from Tkinter import *
from types import *
from DraughtSocketInterface import *


HOST = '127.000.000.001'  # Symbolic name meaning the local host
#HOST = '010.000.002.015'
PORT = 27531              # Arbitrary non-privileged port

dc = DraughtConstants()

class DamExchangeLevel1:
    def __init__(self, Server=True):
        if Server == True:
            s = None
            for res in socket.getaddrinfo(HOST, PORT, socket.AF_UNSPEC, socket.SOCK_STREAM, 0, socket.AI_PASSIVE):
                af, socktype, proto, canonname, sa = res
                try:
                    print "make socket"
                    s = socket.socket(af, socktype, proto)
                except socket.error, msg:
                    print msg
                    s = None
                    continue
                try:
                    print "bind it"
                    s.bind(sa)
                except socket.error, msg:
                    print msg
                    s.close()
                    s = None
                    continue
                try:
                    print "make it listen"
                    s.listen(1)
                except socket.error, msg:
                    print msg
                    s.close()
                    s = None
                    continue
                break
            if s is None:
                print 'could not open socket'
                return
            print "wait for other site"
            self.conn, self.addr = s.accept()
            print 'Connected by', self.addr
        else: #server is False
            s = None
            print "making socket"
            try:
                s = socket.socket(AF_INET, SOCK_STREAM)
            except socket.error, msg:
                print "Remote player -- could not make socket"
                s = None
            print "trying to make a connection"
            try:
                s.connect((HOST, PORT))
            except socket.error, msg:
                print "Remote player -- could not connect to socket"
                s.close()
                s = None
            if s is None:
                print 'could not open socket'
                return
            print "connected"
            self.conn = s
        return

    def send(self, msg, print_message = None):
        if len(msg) > 127:
            print "message too long, truncated"
            msg = msg[:128]
        msg = msg + '\0'
        if print_message:
          print "Send to remote: ", print_message
        self.conn.send(msg)
        return

    def recv(self, socket, mask):
        print "called back"
        msg = socket.recv(128)
        if not msg:
            socket.close()
            print "socket closed"
            return
        if msg[-1] == '\0':
            # we have collected all info
            return msg[:-1] # strip the trailing \0
        else:
            while True:
                msg2 = socket.recv(128)
                if not msg2:
                    socket.close()
                    return ''
                msg = msg + msg2
                if msg[-1] == '\0':
                    # we have collected all info
                    print msg[:-1] # strip the trailing \0
                    return
        return




class DamExchange:
    def __init__(self, Server=True):
        self.level1 = DamExchangeLevel1(Server)
        self.RemoteGameCounter = 0
        self.RemoteWhiteResult = 0
        self.RemoteBlackResult = 0
        return

    def send_game_req_message(self, ColorRemotePlayer, ThinkingTime, NumberOfPlies, BeginPositionID='B', ColorOnMove='W', \
                              BeginPosition = 'zzzzzzzzzzzzzzzzezzzeeeezeeeeewwwwwwwwwwwwwwwwwwww'):
                 # 0        1         2         3         4         5         6         7         8
                 # 123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890
        header  = 'R'
        version = '01'
        program = 'EenhelelangeNaamvanWeBoomstraDam'
        if len(program) <> 32:
            print "He Feike, fout geteld"
            return

        color   = ColorRemotePlayer
        if (color <> 'W') and (color <> 'Z'):
            print "Invalid color_remote_player in send_game_req_message"
            return False
        if (ThinkingTime > 999) or (ThinkingTime <= 0):
            print "Invalid thinkingtime in send_game_req_message"
            return False
        think = str(ThinkingTime).zfill(3)
        if (NumberOfPlies > 999) or (NumberOfPlies <= 0):
            print "Invalid NumberOfPlies in send_game_req_message"
            return False
        plies = str(NumberOfPlies).zfill(3)
        if (BeginPositionID <> 'A') and (BeginPositionID <> 'B'):
            print "Invalid BeginPosition in send_game_req_message"
            return False
        begID = BeginPositionID
        color2   = ColorOnMove
        if (color2 <> 'W') and (color2 <> 'Z'):
            print "Invalid color_on_move in send_game_req_message"
            return False
        if len(BeginPosition) <> 50:
            print "Incorrect Beginposition length in send_game_req_message"
            return False
        begin = BeginPosition

        # all parameters screened

        msg = header + version + program + color + think + plies + begID

        # if begID == 'B':
        msg = msg + color2 + begin
        self.level1.send(msg, "request new game")
        return

    def send_game_acc_message(self):
        header = 'A'
        program = 'BoomstraDam                             '
        accept = '0'
        msg = header + program + accept
        self.level1.send(msg, "game accepted")
        return

    def send_back_acc_message(self):
        # always reject
        header = 'K'
        acc = '1'
        msg = header + acc
        self.level1.send(msg, "reject take back")
        return


    def send_move(self, start, stop, caps=[]):
        header = 'M'
        tijd = '0000'
        l_start = str(start).zfill(2)
        l_stop = str(stop).zfill(2)
        l_cnt = str(len(caps)).zfill(2)

        msg = header + tijd + l_start + l_stop + l_cnt

        for cap in caps:
            msg = msg + str(cap).zfill(2)

        self.level1.send(msg, "send move")
        return

    def send_end_of_game(self):
        header = 'E'
        reden = '0'
        stop_code = '0'
        msg = header + reden + stop_code
        self.level1.send(msg, "end of game")
        return

if __name__=='__main__':

##    dam = DamExchange(Server=False)
##    dam.send_game_req_message('Z', 25, 75, 'A')
##    dam.send_move(31, 26)
##    dam.get_move()



    dam = DamExchange(Server=False)
    root = Tk()
    root.mainloop()
    #dam.get_game_req_message()
    #dam.get_move()

