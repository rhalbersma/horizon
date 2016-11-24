#
#  This file is Copyright (C) 2010 Feike Boomstra.
#  Distributed under the Boost Software License, Version 1.0.
#  (See accompanying file LICENSE_1_0.txt or copy at
#  http://www.boost.org/LICENSE_1_0.txt)
#


import os
import errno
import socket
import Tkinter

class SocketReader:
    
    def __init__(self, name, main, play, mysocket, mydispatcher, root = None):
        if root == None:
            root = Tk()
        self.root = root
        self.play = play
        self.main = main
        self.socket = mysocket
        self.dispatcher = mydispatcher
        self.name = name
        self.msg = ""
        self.root.after(2000, self._select)
        return
    
    def _select(self):
        no_data = False
        try:            
            data = self.socket.recv(1024)
        except socket.error, reason:
            code, msg = reason.args
            no_data = True
            if code <> errno.EAGAIN:
                print self.name, " GUI -- socket error: ", msg
                return
        if no_data == False:
            if len(data) == 0:
                print self.name, " GUI -- socket closed by other party"
                self.socket.close()
                self.dispatcher('X')    # tell them we are closed
            else:
                for i in range(0, len(data)):
                    if data[i] == '\0':
                        #print self.name, " GUI -- received message: ", self.msg
                        self.dispatcher(self.msg)
                        self.msg = ''
                    else:
                        self.msg = self.msg + data[i]
                        #print data[i],
            
        self.root.after(200, self._select)
        return
    
