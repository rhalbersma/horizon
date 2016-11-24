#
#  This file is Copyright (C) 2010 Feike Boomstra.
#  Distributed under the Boost Software License, Version 1.0.
#  (See accompanying file LICENSE_1_0.txt or copy at
#  http://www.boost.org/LICENSE_1_0.txt)
#


from Tkinter import *
from math import *
import string

class DraughtMaketKinterChart:
  def __init__(self, main):
    self.main = main
    self.height = 124.0
    self.low_margin = 22.0
    # the x-axis at low margin
    # ______________________________
    #    |    |    |   |
    # expected move: ..
    self.vert_mark_length = 6.0
    self.vert_mark_spacing = 4.0
    self.low_text_height = int(self.height - (self.low_margin - self.vert_mark_length - self.vert_mark_spacing))
    self.mark_low = int(self.height - (self.low_margin - self.vert_mark_length))
    self.mark_high = int(self.height - self.low_margin)
    self.top_margin = 2.0
    self.y_canvas_height = (self.height - self.low_margin - self.top_margin)/2.0
    self.width = 400.0
    self.x_range = 125
    self.white = "#ffffff"
    self.black = "#000000"
    self.blue = "#0000ff"
    self.red = "#ff0000"
    self.green = "#00f000"
    self.licht_bruin = "#D4C2AA"
    self.donker_bruin = "#633909"
    return
# total range:
# 12, 25, 60, 100, 125
# display range
# 10, 20, 30, 40, 50, 60, 75, 80, 100
# tics
# 2,   5, 10, 10, 10, 15, 15, 20,  25

  def calc_range(self, length):
    if length < 12:
      return 10, 2
    elif length < 25:
      return 20, 5
    elif length < 40:
      return 30, 10
    elif length < 50:
      return 40, 10
    elif length < 60 :
      return 50, 10
    elif length < 75:
      return 60, 15
    elif length < 90:
      return 70, 15
    elif length < 100:
      return 80, 20
    else:
      return 100, 25

  def set_marks(self, length):
    dis_range, tic_unit = self.calc_range(length)
    self.x_range = dis_range + tic_unit
    for pos in range(tic_unit, dis_range + tic_unit,  tic_unit):
      self.vert_mark(pos)
      self.canvas.create_text(self.lin_x_coordinate(pos), self.mark_high + 13, text=str(pos), fill=self.donker_bruin)
    return



  def create_canvas(self):
    #
    self.toplevel = Toplevel()
    self.canvas = Canvas(self.toplevel, width = int(self.width), height = int(self.height), bg = self.licht_bruin)
    self.canvas.grid()
    return

  def lin_y_coordinate(self, lin_value):
    # in y_coordinates: 0 is the top of the picture, self.height is the absolute bottom
    # Y-range = -1 .. 1
    # y_out = a * y_in + b    with a + b = self.top_margin  and -a + b = self.top_margin + 2 * y_canvas_height ->
    # b = y_canvas_height + self.top_margin  & a = -y_canvas_height
    return int(-self.y_canvas_height * lin_value + self.y_canvas_height + self.top_margin + 0.5)

  def tanh_y_coordinate(self, pos):
    return self.lin_y_coordinate(tanh(0.9*pos))

  def lin_x_coordinate(self, lin_value):
    # in x_coordinates: 0 is left, self.width is ultimate right
    # x_range is set dependent on the input
    return int((lin_value*self.width/(self.x_range)) +0.5)

  def write_low_text(self, s):
    self.canvas.create_text(10, self.low_text_height, text=s, fill=self.black)
    return

  def write_high_text(self, s):
    self.canvas.create_text(10, 4, text=s, fill=self.black)
    return

  def vert_mark(self, pos):
    x_val = self.lin_x_coordinate(pos)
    self.canvas.create_line([x_val, self.mark_low, x_val, self.mark_high], fill=self.donker_bruin, width=2)
    return

  def hor_mark(self, pos):
    y_val = self.tanh_y_coordinate(pos)
    self.canvas.create_line([self.width - 4, y_val, self.width, y_val], fill=self.donker_bruin, width=1)
    return

  def set_hor_marks(self):
    for pos in [-1.0, -0.5, -0.2, 0.2, 0.5, 1.0]:
      self.hor_mark(pos)
    return
#    self.MakeChart(raw, expected_move, file_name)

  def MakeChart(self, list, list2 = None ):
    #print list
    #if list2: print list2
    self.create_canvas()
    self.set_marks(len(list))
    self.set_hor_marks()
    for k in range(1, len(list)):
      self.canvas.create_line([int(self.lin_x_coordinate((k-1.0))), int(self.tanh_y_coordinate(list[k-1])), \
                        int(self.lin_x_coordinate(k)), int(self.tanh_y_coordinate(list[k]))], fill=self.red, width=2)
      if list2:
        self.canvas.create_line([int(self.lin_x_coordinate((k-1.0))), int(self.tanh_y_coordinate(list2[k-1])), \
                        int(self.lin_x_coordinate(k)), int(self.tanh_y_coordinate(list2[k]))], fill=self.blue, width=2)

    self.canvas.create_line([0, self.mark_high, self.width, self.mark_high], fill=self.donker_bruin, width=1)
    self.canvas.create_line([0, self.top_margin + self.y_canvas_height, self.width, self.top_margin + self.y_canvas_height], fill=self.donker_bruin, width=1)
    return

  def move_nr_and_color(self, half_ply_counter):
    # half_ply_counter: 0..max
    move_nr = (half_ply_counter + 1) / 2 + 1
    if half_ply_counter % 2 == 0:
      color = "black"
    else:
      color = "white"
    return move_nr, color

  def is_all_zero(self, list):
    for item in list:
      if item != 0.0:
        print item
        return False
    return True

  def create_chart(self):
    game = self.main.ActiveGame
    game.HalfPlyPointer = 0
    self.previous_white_value = [0.0]
    self.previous_black_value = [0.0]
    white_list = []
    black_list = []
    while True:
      move = game.get_half_ply_pointer_record()
      if not move: break
      if game.HalfPlyPointer % 2 == 0:
        #print "white", game.HalfPlyPointer,  len(white_list), self.previous_white_value[0]
        self.collect_eval_info(move, self.previous_white_value, white_list)
      else:
        #print "black", game.HalfPlyPointer, len(black_list), self.previous_black_value[0]
        self.collect_eval_info(move, self.previous_black_value, black_list)
      game.HalfPlyPointer += 1
    if self.is_all_zero(white_list) == True:
      if self.is_all_zero(black_list) == True:
        print "no eval info available"
      else:
        self.MakeChart(black_list)
    else:
      if self.is_all_zero(black_list) == True:
        self.MakeChart(white_list)
      else:
        self.MakeChart(white_list, black_list)
    return

  def collect_eval_info(self, move, previous_value, list):
      if move.Comment == '':
        list.append(previous_value[0])
      else:
          value = self.get_eval_value(move, previous_value)
          if value > 9.9: value = 9.9
          if value < -9.9: value = -9.9
          list.append(value)
          previous_value[0] = value
      return

  def get_eval_value(self, move, previous_value):
    if move.Comment[0:5] == "%eval":
      if move.Comment[7:14] == "Horizon":
        if string.rfind(move.Comment, ":") != -1:
          try:
            eval_value = int(move.Comment[string.rfind(move.Comment, '"')+2:string.rfind(move.Comment, ":")])
            #print move.Comment[string.rfind(move.Comment, '"')+2:], "->", eval_value
            if eval_value == 0:
              eval_value = previous_value[0]
            else:
              eval_value = round(eval_value/1000.0, 1)

          except:
            print "error", move.Comment, '|',
            print move.Comment[string.rfind(move.Comment, '"')+2:string.rfind(move.Comment, ":")], string.rfind(move.Comment, '"')
            eval_value = 0.0
        else:
          try:
            eval_value = int(move.Comment[string.rfind(move.Comment, '"')+2:])
            #print move.Comment[string.rfind(move.Comment, '"')+2:], "->", eval_value
            if eval_value == 0:
              eval_value = previous_value[0]
            else:
              eval_value = round(eval_value/1000.0, 1)

          except:
            print "error", move.Comment, '|', move.Comment[string.rfind(move.Comment, '"')+2:], string.rfind(move.Comment, '"')
            eval_value = 0.0
      else:
        try:
          eval_value = float(move.Comment[string.rfind(move.Comment, '"')+2:])
          #print move.Comment[string.rfind(move.Comment, '"')+2:], "->", eval_value
        except:
          print "error", move.Comment, '|', move.Comment[string.rfind(move.Comment, '"')+2:], string.rfind(move.Comment, '"')
          eval_value = 0.0
    else:
      return 0.0
    return eval_value

raw = [0.01,0.01,0.01,0.01,0.01,0.01,0.01,0.02,0.04,0.08, 0.1, 0.1, 0.1, 0.12, 0.18, 0, -0.1 ]
raw2 = [-0.01,-0.01,-0.01,-0.01,-0.01,-0.01,-0.01,-0.02,-0.04,-0.08, -0.1, -0.1, -0.1, -0.12, -0.18, 0, -0.1 ]
#raw = [10, 10, 10, 10, 20, 20, 40, 40, 60, 90, 110, 100, 100, 100, 100, 90, 100, 120, 140, 160, 110, 110, 90, 0, 0, 0, 0, -20, -40, 0, 10, 0, 100]
##raw2 = [10, 10, 10, 10, 20, 20, 40, 40, 60, 90, 110, 100, 100, 100, 100, 90, 100, 120, 140, 160, 110, 110, 90, 0, 0, 0, 0, -20, -40, 0, 10, 0, 100, 10, 10, 10, 10, 20, 20, 40, 40, 60, 90, 110, 100, 100, 100, 100, 90, 100, 120, 140, 160, 110, 110, 90, 0, 0, 0, 0, -20, -40, 0, 10, 0, 100]
##raw3 = [10, 10, 10, 10, 20, 20, 40, 40, 60, 90, 110, 100, 100, 100, 100, 90, 100, 120, 140, 160, 110, 110, 90, 0, 0, 0, 0, -20, -40, 0, 10, 0, 100, 10, 10, 10, 10, 20, 20, 40, 40, 60, 90, 110, 100, 100, 100, 100, 90, 100, 120, 140, 160, 110, 110, 90, 0, 0, 0, 0, -20, -40, 0, 10, 0, 100, 110, 120, 130, 140, 150, 160, 180, 200, 220, 240, 260, 280, 300, 340, 340, 300, 280, 270, 350, 360, 400, 450, 500, 550, 600, 610, 650, 700, 850, 850, 1020, 1100, 1400, 1400, 1800, 4000, 26000 ]
##
if __name__ == "__main__":
  chart = DraughtMaketKinterChart(None)
  chart.MakeChart(raw, raw2)
  print chart.toplevel.state()
  root = Tk()
  root.mainloop()
