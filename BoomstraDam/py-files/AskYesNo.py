#
#  This file is Copyright (C) 2010 Feike Boomstra.
#  Distributed under the Boost Software License, Version 1.0.
#  (See accompanying file LICENSE_1_0.txt or copy at
#  http://www.boost.org/LICENSE_1_0.txt)
#


from Tkinter import *
from DraughtConstants import *
dc =DraughtConstants()         # get the global constants

class AskYesNo:
    
    def __init__(self, root=None):
        if root == None:
            root = Tk()
        self.root = root
        self.answer = 0
        
        self.top = Toplevel(root)
        self.frame = Frame(self.top)
        self.frame.pack()

        self.button = Button(self.frame, text="Yes", command=self.yes_clicked)
        self.button.pack(side=LEFT)

        self.hi_there = Button(self.frame, text="No", command=self.no_clicked)
        self.hi_there.pack(side=LEFT)
        
        return None
    
    def yes_clicked(self):
        self.answer = 1
        self.top.quit
        return
    
    def no_clicked(self):
        self.answer = 1
        self.top.quit
        return
    
    def ask(self, question):
        self.top.title = question
        self.top.mainloop()
        return self.answer
        
    
    
if __name__=='__main__':        
    yes = AskYesNo()
    print yes.ask("Even een vraagje")