#
#  This file is Copyright (C) 2010 Feike Boomstra.
#  Distributed under the Boost Software License, Version 1.0.
#  (See accompanying file LICENSE_1_0.txt or copy at
#  http://www.boost.org/LICENSE_1_0.txt)
#


from random import *
from math import *

class create:
  def gen_param(self, a, b, f):
    a = float(a)
    b = float(b)
    if a < b:
      low  = log(a)
      high = log(b)
    else:
      low  = log(b)
      high = log(a)
    r = uniform(low, high)
    f.write(str(exp(r)) + '\n')
    return



  def first_batch(self):
    for i in range(0, 60):
      file_name = '/home/feike/BoomstraDam/CEM/series_03/' + str(i).zfill(2) + '.weight'
      f = open(file_name, 'w')
      f.write('1.0\n')
      f.write('1.0\n')
      self.gen_param(0.997, 1.095, f)
      self.gen_param(0.556835273597, 0.742293730566, f)
      self.gen_param(0.56, 0.828, f)
      self.gen_param(1, 0.45, f)
      self.gen_param(3.5, 1.5, f)
      self.gen_param(0.9, 1.2, f)
      self.gen_param(0.5, 0.75, f)
      self.gen_param(0.95, 1.5, f)
      self.gen_param(2.7, 0.3, f)
      self.gen_param(0.45, 0.3, f)
      self.gen_param(2.2, 1.8, f)
      self.gen_param(0.3, 0.5, f)
      self.gen_param(0.7, 0.6, f)
      self.gen_param(0.9, 2, f)
      self.gen_param(0.75, 1.1, f)
      self.gen_param(1.6, 0.7, f)
      self.gen_param(1.3, 0.9, f)
      self.gen_param(1.3, 0.9, f)
      self.gen_param(0.65, 0.4, f)
      self.gen_param(0.6, 1.9, f)
      self.gen_param(2.5, 1.4, f)
      self.gen_param(3.5, 4.5, f)
      self.gen_param(0.55, 0.7, f)
      self.gen_param(1.1, 0.75, f)
      self.gen_param(0.7, 1, f)
      self.gen_param(0.25, 0.85, f)
      self.gen_param(2, 4, f)
      self.gen_param(0.6, 1.1, f)
      self.gen_param(2, 4, f)
      f.close()

  def second_batch(self):
    for i in range(60, 99):
      file_name = '/home/feike/BoomstraDam/CEM/series_03/' + str(i).zfill(2) + '.weight'
      f = open(file_name, 'w')
      f.write('1.0\n')
      f.write('1.0\n')
      self.gen_param(0.997, 1.095, f)
      self.gen_param(0.556835273597, 0.742293730566, f)
      self.gen_param(0.56, 0.828, f)
      self.gen_param(1, 0.45, f)
      self.gen_param(3.5, 1.5, f)
      self.gen_param(0.9, 1.2, f)
      self.gen_param(0.5, 0.75, f)
      self.gen_param(0.95, 1.5, f)
      self.gen_param(2.7, 0.3, f)
      self.gen_param(0.45, 0.3, f)
      self.gen_param(0.18, 0.28, f)
      self.gen_param(0.3, 0.5, f)
      self.gen_param(0.7, 0.6, f)
      self.gen_param(0.9, 2, f)
      self.gen_param(0.75, 1.1, f)
      self.gen_param(1.6, 0.7, f)
      self.gen_param(1.3, 0.9, f)
      self.gen_param(1.3, 0.9, f)
      self.gen_param(0.65, 0.4, f)
      self.gen_param(0.6, 1.9, f)
      self.gen_param(2.5, 1.4, f)
      self.gen_param(0.8, 1.2, f)
      self.gen_param(0.55, 0.7, f)
      self.gen_param(1.1, 0.75, f)
      self.gen_param(0.7, 1, f)
      self.gen_param(0.25, 0.85, f)
      self.gen_param(2, 4, f)
      self.gen_param(0.6, 1.1, f)
      self.gen_param(2, 4, f)
    f.close()

c = create()
c.first_batch()
c.second_batch()
