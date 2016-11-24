#
#  This file is Copyright (C) 2010 Feike Boomstra.
#  Distributed under the Boost Software License, Version 1.0.
#  (See accompanying file LICENSE_1_0.txt or copy at
#  http://www.boost.org/LICENSE_1_0.txt)
#


from pychart import *
theme.get_options()
theme.use_color = 1
theme.reinitialize()

class DraughtLabel:
  def __init__(self):
    self.color = "b"
    self.move_nr = 0
    return
  def next(self):
    if self.color == "b":
      self.color = "w"
      self.move_nr += 1
    else:
      self.color = "b"
    return str(self.move_nr) + self.color

class DraughtMakeChart:
  def __init__(self, main):
    self.main = main
    self.previous_value = 0
    return

  def _MakeChart(self, data, expected_move, file_name):

    theme.default_font_size = 10
    theme.reinitialize()
    can = canvas.init(file_name)
    xaxis = axis.X(label='expected move: ' + expected_move, tic_interval = 10)
    yaxis = axis.Y(tic_interval = 250, label="") #"Value (White Player)")
    ar = area.T(x_axis=xaxis, y_axis=yaxis, y_range=(-9.9,9.9), x_range=(0,150), x_coord = category_coord.T(data, 0),  y_coord = tanh_coord.T(), legend=None, size=(210, 72), bg_style=fill_style.Plain(bgcolor=color.tan))#T(r=208, g=194, b=170)))
    plot = line_plot.T(data=data, ycol=1, tick_mark=tick_mark.circle1)
    ar.add_plot(plot)
    ar.draw(can)
    can.close()
#    for k in range(0, len(data)):
#      print k, data[k][0], data[k][1]
    print "created:",  file_name
    return


  def MakeChart(self, raw_data,  expected_move, file_name):

    # the raw data is a simple list of integer values between -31000 and +31000
    # it has to be converted to something like:
    #
    #data = [
    #        ["1w", 0.010],
    #        ["1b", 0.020],
    #        ["2w", 0.010],
    #        ["2b", 0.040],
    #        ["3w", 0.050],
    #        ["3b", 0.040],
    #        ["4w", 0.080],
    #        ["4b", 0.100],
    #        ["5w", 0.100],
    #        ["5b", 0.140],
    #        ["6w", 0.110],
    #        ["6b", 0.140],
    #        ["7w", 0.210],
    #        ["7b", 0.040],
    #        ["8w", -0.810],
    #        ["8b", -0.840]
    #        ]

    data = []
    label = DraughtLabel()
    for i in range(0, len(raw_data)):
      adj_data = float(raw_data[i])/1000.0
      if adj_data > 9.9: adj_data = 9.9
      if adj_data < -9.9: adj_data = -9.9
      #print "%3.1f"%adj_data, ",\\"
      data.append([label.next(), adj_data])
    self._MakeChart(data, expected_move, file_name)
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
          # we expect a comment in the form of value start op stop
          if move.Comment[-3] == '-' or move.Comment[-3] == 'x':
            expected_move = move.Comment[-5:]
            move_nr, color = self.move_nr_and_color(game.HalfPlyPointer)
            if color == "white": # the color of the expected (next) move
              fill = ". "
              value = int(move.Comment[:-7])
            else:
              fill = ". ... "
              value = -int(move.Comment[:-7])
            if value == 0 and move.Comment[-3] == 'x':
              value = +self.previous_value

            expected_move = str(move_nr) + fill + expected_move
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
    self.MakeChart(raw, expected_move, file_name)
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
              value = int(move.Comment[:-7])
            else:
              fill = ". ... "
              value = -int(move.Comment[:-7])
            if value == 0 and move.Comment[-3] == 'x':
              value = +self.previous_value

            expected_move = "can't analyze, illegal move in sequence"
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
    self.MakeChart(raw, expected_move, file_name)
    game.HalfPlyPointer += 1
    return

##raw = [10, 10, 10, 10, 20, 20, 40, 40, 60, 90, 110, 100, 100, 100, 100, 90, 100, 120, 140, 160, 110, 110, 90, 0, 0, 0, 0, -20, -40, 0, 10, 0, 100]
##raw2 = [10, 10, 10, 10, 20, 20, 40, 40, 60, 90, 110, 100, 100, 100, 100, 90, 100, 120, 140, 160, 110, 110, 90, 0, 0, 0, 0, -20, -40, 0, 10, 0, 100, 10, 10, 10, 10, 20, 20, 40, 40, 60, 90, 110, 100, 100, 100, 100, 90, 100, 120, 140, 160, 110, 110, 90, 0, 0, 0, 0, -20, -40, 0, 10, 0, 100]
##raw3 = [10, 10, 10, 10, 20, 20, 40, 40, 60, 90, 110, 100, 100, 100, 100, 90, 100, 120, 140, 160, 110, 110, 90, 0, 0, 0, 0, -20, -40, 0, 10, 0, 100, 10, 10, 10, 10, 20, 20, 40, 40, 60, 90, 110, 100, 100, 100, 100, 90, 100, 120, 140, 160, 110, 110, 90, 0, 0, 0, 0, -20, -40, 0, 10, 0, 100, 110, 120, 130, 140, 150, 160, 180, 200, 220, 240, 260, 280, 300, 340, 340, 300, 280, 270, 350, 360, 400, 450, 500, 550, 600, 610, 650, 700, 850, 850, 1020, 1100, 1400, 1400, 1800, 4000, 26000 ]
##
if __name__ == "__main__":
  chart = DraughtMakeChart(None)
  chart.MakeChart([0], '', 'chart.png')
