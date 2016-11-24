#
#  This file is Copyright (C) 2010 Feike Boomstra.
#  Distributed under the Boost Software License, Version 1.0.
#  (See accompanying file LICENSE_1_0.txt or copy at
#  http://www.boost.org/LICENSE_1_0.txt)
#


import Image, ImageDraw
from math import *

class DraughtMakeChart:
  def __init__(self, main):
    self.main = main
    self.previous_value = 0
    self.height = 124.0
    self.low_margin = 22.0
    # the x-axis at low margin
    # ______________________________
    #    |    |    |   |
    # expected move: ..
    self.vert_mark_length = 6.0
    self.vert_mark_spacing = 4.0
    self.low_text_height = int(self.height - (self.low_margin - self.vert_mark_length - self.vert_mark_spacing))
    self.white_name_height = int(self.height - self.low_margin - 13)
    self.mark_low = int(self.height - (self.low_margin - self.vert_mark_length))
    self.mark_high = int(self.height - self.low_margin)
    self.top_margin = 2.0
    self.y_canvas_height = (self.height - self.low_margin - self.top_margin)/2.0
    self.width = 250.0
    self.x_range = 125
    self.white = (255, 255, 255)
    self.black = (0, 0, 0)
    self.blue = (0, 0, 255)
    self.red = (255, 0, 0)
    self.green = (0,128,0)
    self.licht_bruin = (208, 194, 170) #"0xD4C2AA"
    self.donker_bruin = (99, 57, 9) #0x633909
    return
# total range:
# 12, 25, 60, 100, 125
# display range
# 10, 20, 30, 40, 50, 60, 75, 80, 100
# tics
# 2,   5, 10, 10, 10, 15, 15, 20,  25

  def calc_range(self, length):
    if length < 24:
      return 10, 2
    elif length < 50:
      return 20, 5
    elif length < 80:
      return 30, 10
    elif length < 100:
      return 40, 10
    elif length < 120 :
      return 50, 10
    elif length < 150:
      return 60, 15
    elif length < 180:
      return 70, 15
    elif length < 200:
      return 80, 20
    else:
      return 100, 25

  def set_marks(self, length):
    dis_range, tic_unit = self.calc_range(length)
    self.x_range = dis_range + tic_unit
    for pos in range(tic_unit, dis_range + tic_unit,  tic_unit):
      self.vert_mark(pos)
      self.canvas.text((self.lin_x_coordinate(pos) + 2, self.mark_high + 1), str(pos), self.donker_bruin)
    return



  def create_canvas(self):
    #
    self.image1 = Image.new("RGB", (int(self.width), int(self.height)), self.licht_bruin)
    self.canvas = ImageDraw.Draw(self.image1)
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
    self.canvas.text((10, self.low_text_height), s, self.black)
    return

  def write_high_text(self, s):
    self.canvas.text((10, 4), s, self.black)
    return

  def write_white_name(self, s):
    self.canvas.text((10, self.white_name_height), s, self.black)
    return

  def vert_mark(self, pos):
    x_val = self.lin_x_coordinate(pos)
    self.canvas.line([x_val, self.mark_low, x_val, self.mark_high], self.donker_bruin, width=2)
    return

  def hor_mark(self, pos):
    y_val = self.tanh_y_coordinate(pos)
    self.canvas.line([self.width - 4, y_val, self.width, y_val], self.donker_bruin, width=1)
    return

  def set_hor_marks(self):
    for pos in [-1.0, -0.5, -0.2, 0.2, 0.5, 1.0]:
      self.hor_mark(pos)
    return
#    self.MakeChart(raw, expected_move, file_name)

  def MakeChart(self, list, expected_move, black_name, file_name, white_name=None):
    print list
    self.create_canvas()
    self.set_marks(len(list))
    self.set_hor_marks()
    for k in range(1, len(list)):
      self.canvas.line([int(self.lin_x_coordinate((k-1.0)/2.0+1.0)), int(self.tanh_y_coordinate(list[k-1])), \
                        int(self.lin_x_coordinate(k/2.0+1.0)), int(self.tanh_y_coordinate(list[k]))], self.donker_bruin, width=2)

    self.canvas.line([0, self.mark_high, self.width, self.mark_high], self.donker_bruin, width=1)
    self.canvas.line([0, self.top_margin + self.y_canvas_height, self.width, self.top_margin + self.y_canvas_height], self.donker_bruin, width=1)
    self.write_low_text(expected_move)
    self.write_high_text(black_name)
    if white_name: self.write_white_name(white_name)
    self.image1.save(file_name)
    return

  def move_nr_and_color(self, half_ply_counter):
    # half_ply_counter: 0..max
    move_nr = (half_ply_counter + 1) / 2 + 1
    if half_ply_counter % 2 == 0:
      color = "black"
    else:
      color = "white"
    return move_nr, color

  def create_chart(self, file_name='chart.png'):
    raw = [0, 0, 0, 0, 0, 0]
    game = self.main.ActiveGame
    game.HalfPlyPointer = 0
    expected_move = ''
    while True:
      move = game.get_half_ply_pointer_record()
      if not move: break
      if move.Comment == '':
        break
      else:
        if len(move.Comment) > 7:
          # we expect a comment in the form of value: start op stop
          # or %eval "Horizon" value: start op stop
          if move.Comment[-3] == '-' or move.Comment[-3] == 'x':
            expected_move = move.Comment[-5:]
            move_nr, color = self.move_nr_and_color(game.HalfPlyPointer)
            if color == "white": # the color of the expected (next) move
              fill = ". "
              if move.Comment[:-7] == '%eval "':
                value = -int(move.Comment[14:-7])
              else:
                value = -int(move.Comment[:-7])
            else:
              fill = ". ... "
              if move.Comment[:-7] == '%eval "':
                value = int(move.Comment[14:-7])
              else:
                value = int(move.Comment[:-7])
            if value == 0 and move.Comment[-3] == 'x':
              value = +self.previous_value
            else:
              value = round(value/1000.0, 1)
            if value > 9.9: value = 9.9
            if value < -9.9: value = -9.9
            expected_move = "expected move: " + str(move_nr) + fill + expected_move + " (" + str(value) + ")"
            #print move_nr,  color, expected_move , value
            raw.append(value)
            self.previous_value = value
      game.HalfPlyPointer += 1
    # collected all the moves with a value

    if game.HalfPlyPointer < len(raw):
      if game.half_ply_length() == 0:
        raw = [0]
      else:
        raw = raw[0:game.HalfPlyPointer]
    #print raw
    black_name = game.BlackPlayer
    white_name = game.WhitePlayer
    self.MakeChart(raw, expected_move, black_name, file_name, white_name)
    game.HalfPlyPointer += 1
    return

  def create_chart_with_illegal_move(self, file_name='chart.png'):
    raw = [0, 0, 0, 0, 0, 0]
    game = self.main.ActiveGame
    game.HalfPlyPointer = 0
    expected_move = ''
    while True:
      move = game.get_half_ply_pointer_record()
      if not move: break
      if move.Comment == '':
        break
      else:
        if len(move.Comment) > 7:
          # we expect a comment in the form of value start op stop
          if move.Comment[-3] == '-' or move.Comment[-3] == 'x':
            expected_move = move.Comment[-5:]
            move_nr, color = self.move_nr_and_color(game.HalfPlyPointer)
            if color == "white": # the color of the expected (next) move
              fill = ". "
              if move.Comment[:-7] == '%eval "':
                value = -int(move.Comment[14:-7])
              else:
                value = -int(move.Comment[:-7])
            else:
              fill = ". ... "
              if move.Comment[:-7] == '%eval "':
                value = int(move.Comment[14:-7])
              else:
                value = int(move.Comment[:-7])
            if value == 0 and move.Comment[-3] == 'x':
              value = +self.previous_value

            expected_move = "can't analyze, unrecognized move in sequence"
            #print move_nr,  color, expected_move , value
            raw.append(value)
            self.previous_value = value
      game.HalfPlyPointer += 1
    # collected all the moves with a value

    if game.HalfPlyPointer < len(raw):
      if game.half_ply_length() == 0:
        raw = [0]
      else:
        raw = raw[0:game.HalfPlyPointer]
    #print raw
    self.MakeChart(raw, expected_move, game.BlackPlayer, file_name)
    game.HalfPlyPointer += 1
    return

##raw = [10, 10, 10, 10, 20, 20, 40, 40, 60, 90, 110, 100, 100, 100, 100, 90, 100, 120, 140, 160, 110, 110, 90, 0, 0, 0, 0, -20, -40, 0, 10, 0, 100]
##raw2 = [10, 10, 10, 10, 20, 20, 40, 40, 60, 90, 110, 100, 100, 100, 100, 90, 100, 120, 140, 160, 110, 110, 90, 0, 0, 0, 0, -20, -40, 0, 10, 0, 100, 10, 10, 10, 10, 20, 20, 40, 40, 60, 90, 110, 100, 100, 100, 100, 90, 100, 120, 140, 160, 110, 110, 90, 0, 0, 0, 0, -20, -40, 0, 10, 0, 100]
##raw3 = [10, 10, 10, 10, 20, 20, 40, 40, 60, 90, 110, 100, 100, 100, 100, 90, 100, 120, 140, 160, 110, 110, 90, 0, 0, 0, 0, -20, -40, 0, 10, 0, 100, 10, 10, 10, 10, 20, 20, 40, 40, 60, 90, 110, 100, 100, 100, 100, 90, 100, 120, 140, 160, 110, 110, 90, 0, 0, 0, 0, -20, -40, 0, 10, 0, 100, 110, 120, 130, 140, 150, 160, 180, 200, 220, 240, 260, 280, 300, 340, 340, 300, 280, 270, 350, 360, 400, 450, 500, 550, 600, 610, 650, 700, 850, 850, 1020, 1100, 1400, 1400, 1800, 4000, 26000 ]
##
if __name__ == "__main__":
  chart = DraughtMakeChart(None)
  chart.MakeChart([0], '', 'chart.png')
