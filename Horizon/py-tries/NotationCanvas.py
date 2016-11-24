#
#  This file is Copyright (C) 2010 Feike Boomstra.
#  Distributed under the Boost Software License, Version 1.0.
#  (See accompanying file LICENSE_1_0.txt or copy at
#  http://www.boost.org/LICENSE_1_0.txt)
#


from Tkinter import *

class PlyClicked():
  def __init__(self, main, id):
    self.id = id
    self.main = main
    return

  def __call__(self, event):
    self.main.ply_clicked(self.id)
    return


class NotationCanvas(Canvas):
  def __init__(self, master):
    Canvas.__init__(self, master)
    self.config(height=600, width=600, bg='#d0d0d0')
    self.xpos = 50
    self.ypos = 20
    self.last_id = 0
    return

  def set(self, game):
    self._add('hallo ')
    return

  def clear(self):
    self.canvas.delete('ALL')
    self.xpos = 0
    self.ypos = 0
    self.last_id = 0
    return

  def _add(self, s):

    for z in range(0, 100):
      id = self.create_text(self.xpos, self.ypos, text=s, anchor=NW)
      self.last_id = id
      self.tag_bind(id,"<Button-1>", PlyClicked(self, id))
      x1, y1, x2, y2 = self.bbox(id)
      if x2 > 600:
        self.delete(id)
        self.xpos = 50
        self.ypos += 15
        id = self.create_text(self.xpos, self.ypos, text=s, anchor=NW)
      else:
        self.xpos = x2 + 1
    return

  def ply_clicked(self, id):
    print "clicked: ", id
    for i in range(0, self.last_id ):
      self.itemconfig(i+1, fill='#000000')
    self.itemconfig(id, fill='#F00000')
    return


if __name__ == '__main__':
  root = Tk()
  root.geometry('600x800+350+70')
  canv = NotationCanvas(root)
  canv.grid()
  canv.set(None)
  root.mainloop()

