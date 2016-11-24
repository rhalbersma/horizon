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
PORT = 27016              # Arbitrary non-privileged port

dc = DraughtConstants()

class TestSocket:            
    def __init__(self):
        s = None
        for res in socket.getaddrinfo(HOST, PORT, socket.AF_UNSPEC, socket.SOCK_STREAM):
            af, socktype, proto, canonname, sa = res
            try:
                s = socket.socket(af, socktype, proto)
            except socket.error, msg:
                s = None
                continue
            try:
                s.connect(sa)
            except socket.error, msg:
                s.close()
                s = None
                continue
            break
        if s is None:
            print 'could not open socket'
            return
        self.conn = s
        return
    
    
if __name__=='__main__':
    os.startfile("C:\\Documents and Settings\\feike\\Mijn documenten\\BoomstraDam\\project.3.1\\try_socket\\release\\try_socket.exe")
    test = TestSocket()
    test.conn.send('He daar' + '\0')
    test.conn.send('Jij daar'+ '\0')
    root = Tk()
    root.mainloop()
