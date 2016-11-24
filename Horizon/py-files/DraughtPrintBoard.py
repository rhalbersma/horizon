#
#  This file is Copyright (C) 2010 Feike Boomstra.
#  Distributed under the Boost Software License, Version 1.0.
#  (See accompanying file LICENSE_1_0.txt or copy at
#  http://www.boost.org/LICENSE_1_0.txt)
#


class DraughtPrintBoard:
  def __init__(self, main):
    self.main = main
    self.print_row = []
    for i in range(0, 10):
      self.print_row.append('')
    return

  def clear(self):
    for i in range(0, 10):
      self.print_row[i] = ''
    return

  def add_board(self, game):
    for i in range(0, 10):
      if i % 2 == 0:
        self.print_row[i] += self.odd_row(game, i)
      else:
        self.print_row[i] += self.even_row(game, i)
    self.print_collection(game)
    return

  def print_collection(self, game):
    print '/*'
    for i in range(0, 10):
      print self.print_row[i]
    print '*/'
    return

  def pos_symbol(self, game, pos_nr):
    if pos_nr in game.CurrentPosition.WhiteMan:
      return '   o'
    elif pos_nr in game.CurrentPosition.WhiteKing:
      return '   O'
    elif pos_nr in game.CurrentPosition.BlackMan:
      return '   x'
    elif pos_nr in game.CurrentPosition.BlackKing:
      return '   X'
    else:
      return '   .'

  def odd_row(self, game, row_nr):
    s = '    '
    for i in self.calc_range(row_nr):
      s += self.pos_symbol(game, i)
    return s

  def even_row(self, game, row_nr):
    s = '  '
    for i in self.calc_range(row_nr):
      s += self.pos_symbol(game, i)
    s += '  '
    return s

  def calc_range(self, row_nr):
    low = row_nr*5 + 1
    high = low + 5
    return range(low, high)

  def clear_prints_clicked(self):
    game = self.main.ActiveGame
    self.clear()

  def add_print_clicked(self):
    game = self.main.ActiveGame
    self.add_board(game)
    return

  def print_collection_clicked(self):
    game = self.main.ActiveGame
    self.print_collection(game)
    return
