#
#  This file is Copyright (C) 2010 Feike Boomstra.
#  Distributed under the Boost Software License, Version 1.0.
#  (See accompanying file LICENSE_1_0.txt or copy at
#  http://www.boost.org/LICENSE_1_0.txt)
#


from math import *

class DecayPouleSize:
  def __init__(self, third_std_dev):
    self.third_std_dev = third_std_dev
    self.start_poule_size = self._calc_start_poule_size()
    if self.start_poule_size < 20: self.start_poule_size = 20
    self.nr_of_rounds_needed = self._calc_nr_of_rounds_needed()
    self._calc_decay_parameters()
    return

  def _calc_start_poule_size(self):
    ps = (int(184.6154 -0.76923*self.third_std_dev)/2)*2
    if ps < 40: ps = 40
    if ps > 100: ps = 100
    return ps

  def _calc_nr_of_rounds_needed(self):
    return max(20, int(457.5 -1.96875*self.third_std_dev) - 18)

  def _calc_decay_parameters(self):
    # y = ax^2+b
    self.N0 = float(self.start_poule_size) - 20.0
    self.T =  float(self.nr_of_rounds_needed)
    self.lamba = 5
    return

  def get_next_poule_size(self, rounds_played):
#    # step_size ^nr of rounds needed = end/start ->
#    # log(step_size) = (log(end/start))/rounds_needed ->
#    # step_size = exp((log(end/start))/rounds_needed)
#    step_size = exp(log(20.0/self.start_poule_size)/self.nr_of_rounds_needed)
#    next_poule_size = self.start_poule_size*(step_size**rounds_played)
    rp = float(rounds_played) - 18.0
    next_poule_size = self.N0 * exp(-self.lamba * (rp/self.T)) + 20.0
#    if rounds_played > self.nr_of_rounds_needed: next_poule_size = 20
    next_poule_size = (int(next_poule_size)/2)*2
    return next_poule_size


#for std in range(260, 80, -10):
#  d = DecayPouleSize(std)
#  print d.start_poule_size, d.nr_of_rounds_needed
#  for k in range(0, d.nr_of_rounds_needed, 5):
#    print d.get_next_poule_size(k)
#
