#
#  This file is Copyright (C) 2010 Feike Boomstra.
#  Distributed under the Boost Software License, Version 1.0.
#  (See accompanying file LICENSE_1_0.txt or copy at
#  http://www.boost.org/LICENSE_1_0.txt)
#


from DraughtConstants import *
import socket
import sys
from Tkinter import *
from types import *
import os


HOST = '127.000.000.001'  # Symbolic name meaning the local host
PORT = 27013              # Arbitrary non-privileged port

dc = DraughtConstants()

class EngineSocket:
    def __init__(self):
        #s = None
        #af, socktype, proto, canonname, sa = socket.getaddrinfo(HOST, PORT, socket.AF_UNSPEC, socket.SOCK_STREAM, 0, socket.AI_PASSIVE)
        #print af, socktype, proto
        try:
            print "make socket"
            s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        except socket.error, reason:
            code, msg = reason.args
            print msg
            self.flag =  False
            return
        try:
            print "bind it"
            s.bind((HOST, PORT))
        except socket.error, reason:
            code, msg = reason.args
            print msg
            s.close()
            self.flag =  False
            return
        try:
            print "make it listen"
            s.listen(1)
        except socket.error, reason:
            code, msg = reason.args
            print msg
            s.close()
            self.flag =  False
            return
        try:
            print "trying to accept"
            self.conn, self.addr = s.accept()
        except socket.error, reason:
            code, msg = reason.args
            print msg
            s.close()
            self.flag =  False
            return
        print 'Connected by', self.addr
        s.close()
        self.flag = True
        return

    def send_init_game(self, game, nr_of_moves = 75, time_in_seconds = 450):
        header = 'I'
        nr_of_moves = str(nr_of_moves).zfill(3)
        time = str(time_in_seconds).zfill(5)
        msg = header + nr_of_moves + " " + time + '\0'

        self.conn.send(msg)
        return

    def send_final_report(self):
        self.conn.send("R\0")
        return

    def send_get_next_move(self, game,  remaining_engine_time_in_seconds = -1):
        header = 'G'
        if game.CurrentPosition.ColorOnMove == dc.WHITE_ON_MOVE:
            color = 'W'
        else: color = 'Z'
        pos = game.get_50_sq_position()
        ply = str(game.HalfPlyPointer).zfill(3)
        secs = '%05d' % remaining_engine_time_in_seconds
        msg = header + color + pos + ply + secs + '\0'
        self.conn.send(msg)
        return

    def send_get_next_move_from_opening_book(self, game):
        header = 'B'
        pos = game.get_move_string()
        msg = header + pos + '\0'

        self.conn.send(msg)
        return

    def send_evaluate_position(self, game, display=True):
        header = 'E'
        if game.CurrentPosition.ColorOnMove == dc.WHITE_ON_MOVE:
            color = 'W'
        else: color = 'Z'
        if display == True:
            dis = str(1)
        else: dis = str(0)
        pos = game.get_50_sq_position()
        ply = str(game.HalfPlyPointer).zfill(3)
        msg = header + color + pos + ply + dis +'\0'

        self.conn.send(msg)
        return

    def get_50_sq_position(self, board):
        s = ''
        for i in range(1,51):
            if i in board.WhiteMan:
                s = s + 'w'
            elif i in board.WhiteKing:
                s = s + 'W'
            elif i in board.BlackMan:
                s = s + 'z'
            elif i in board.BlackKing:
                s = s + 'Z'
            else: s = s + 'e'
        return s


    def send_evaluate_from_board_position(self, board, display=True):
        header = 'E'
        if board.ColorOnMove == dc.WHITE_ON_MOVE:
            color = 'W'
        else: color = 'Z'
        if display == True:
            dis = str(1)
        else: dis = str(0)
        pos = self.get_50_sq_position(board)
        ply = '000'
        msg = header + color + pos + ply + dis +'\0'

        self.conn.send(msg)
        return

    def send_evaluate_position_2(self, game, display=True):
        header = 'L'
        if game.CurrentPosition.ColorOnMove == dc.WHITE_ON_MOVE:
            color = 'W'
        else: color = 'Z'
        if display == True:
            dis = str(1)
        else: dis = str(0)
        pos = game.get_50_sq_position()
        ply = str(game.HalfPlyPointer).zfill(3)
        msg = header + color + pos + ply + dis +'\0'

        self.conn.send(msg)
        return

    def send_analyse_position(self, game, depth=10):
        header = 'A'
        if game.CurrentPosition.ColorOnMove == dc.WHITE_ON_MOVE:
            color = 'W'
        else: color = 'Z'
        pos = game.get_50_sq_position()
        depth = str(depth).zfill(2)
        ply = str(game.HalfPlyPointer).zfill(3)
        msg = header + color + pos + ply + depth + '\0'

        self.conn.send(msg)
        return

    def send_analyse_move(self, game, move, depth=10):
        header = 'M'
        if game.CurrentPosition.ColorOnMove == dc.WHITE_ON_MOVE:
            color = 'W'
        else: color = 'Z'
        pos = game.get_50_sq_position()
        depth = str(depth).zfill(2)
        move_str = move.get_damexchange_move_string()
        ply = str(game.HalfPlyPointer).zfill(3)
        msg = header + color + pos + ply + depth + move_str + '\0'

        self.conn.send(msg)
        return

    def send_analyse_timing(self, game):
        header = 'T'
        if game.CurrentPosition.ColorOnMove == dc.WHITE_ON_MOVE:
            color = 'W'
        else: color = 'Z'
        pos = game.get_50_sq_position()
        msg = header + color + pos + '\0'
        self.conn.send(msg)
        return

    def send_close_engine(self):
        header = 'Q'
        msg = header + '\0'
        self.conn.send(msg)
        return

    def send_get_fixed_depth_move(self, game, depth, value, orig_color, display=True, FirstMove=False):
        header = 'F'
        if game.CurrentPosition.ColorOnMove == dc.WHITE_ON_MOVE:
            color = 'W'
        else: color = 'Z'
        pos = game.get_50_sq_position()
        depth = str(depth).zfill(2)
        if display == True:
            dis = str(1)
        else: dis = str(0)
        if FirstMove == True:
            fm = str(1)
        else: fm = str(0)
        val = str(value).zfill(6)
        ply = str(game.HalfPlyPointer).zfill(3)
        msg = header + color + pos + ply + depth + dis + val + fm + orig_color + '\0'
        self.conn.send(msg)
        return

    def send_move_now(self):
        header = 'S'
        msg = header + '\0'
        self.conn.send(msg)
        return

    def send_setup_hash(self, game , depth,  threads,  init_hash) :
        header = 'H'
        if game.CurrentPosition.ColorOnMove == dc.WHITE_ON_MOVE:
            color = 'W'
        else: color = 'Z'
        pos = game.get_50_sq_position()
        depth = str(depth).zfill(2)
        threads = str(threads).zfill(2)
        init_hash = str(init_hash).zfill(1)
        ply = str(game.HalfPlyPointer).zfill(3)
        msg = header + color + pos + ply + depth + threads + init_hash +'\0'
        self.conn.send(msg)
        return

    def send_get_from_hash(self,  game):
        header = "C"
        if game.CurrentPosition.ColorOnMove == dc.WHITE_ON_MOVE:
            color = 'W'
        else: color = 'Z'
        pos = game.get_50_sq_position()
        msg = header + color + pos + '\0'
        self.conn.send(msg)
        return

    def send_get_with_key(self,  hash_key):
        header = 'D'
        msg = header + hash_key + '\0'
        self.conn.send(msg)
        return

    def send_ponder(self, game,  ply = '000', max_node = True):
        header = 'P'
        if game.CurrentPosition.ColorOnMove == dc.WHITE_ON_MOVE:
            color = 'W'
        else: color = 'Z'
        pos = game.get_50_sq_position()
        if max_node == True: max = "1"
        else:                max = "0"
        msg = header + color + pos + ply + max + '\0'
        self.conn.send(msg)
        return

    def send_follow_up_move(self, board, move_nr):
        header = 'K'
        if board.ColorOnMove == dc.WHITE_ON_MOVE:
            color = 'W'
        else: color = 'Z'
        pos = self.get_50_sq_position(board)
        ply = str(move_nr).zfill(3)
        msg = header + color + pos + ply + '\0'
        self.conn.send(msg)
        return
