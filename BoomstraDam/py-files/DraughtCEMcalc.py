#
#  This file is Copyright (C) 2010 Feike Boomstra.
#  Distributed under the Boost Software License, Version 1.0.
#  (See accompanying file LICENSE_1_0.txt or copy at
#  http://www.boost.org/LICENSE_1_0.txt)
#


from math import *
from random import *
#from DraughtCEMadmin import *
from DraughtSwissTournamentRating import *

class ParameterRecord:
  def __init__(self, name, mean, sigma):
    self.name = name
    self.mean = mean
    self.sigma = sigma
    return
  def __str__(self):
    return self.name + " %6.4f " % self.mean + "%6.4f " % self.sigma

first = \
[["king", 0.5, 2.0],\
["man_position", 0.1, 4.0],\
["king_position", 0.1, 4.0],\
["row", 0.1, 4.0],\
["man_locked_bonus", 0.1, 4.0],\
["avoid_4641", 0.1, 4.0],\
["avoid_2324", 0.1, 4.0],\
["avoid_2335", 0.1, 4.0],\
["2_8_13", 0.1, 4.0],\
["253035", 0.1, 4.0],\
["klassiek", 0.1, 4.0],\
["tempi", 0.1, 4.0],\
["ketting_stelling", 0.1, 4.0],\
["ketting_stelling_2", 0.1, 4.0],\
["halve_hek", 0.1, 4.0],\
["active_15", 0.1, 4.0],\
["slechte_binding", 0.001, 2.0],\
["free_path", 0.1, 4.0],\
["fit", 0.1, 4.0],\
["edge_35", 0.1, 4.0],\
["edge_36", 0.1, 4.0],\
["11_16_17", 0.1, 4.0],\
["voorpost", 0.1, 4.0],\
["locked", 0.1, 4.0],\
["vleugel_opsluiting", 0.1, 4.0],\
["tandem", 0.1, 4.0],\
["mobility", 0.1, 4.0],\
["maze_penalty", 0.1, 4.0],\
["canon", 0.1, 4.0]]

def create_first_parameter_list():
  list = []
  for k in range(0, len(first)):
    name = first[k][0]
    low  = log(first[k][1])
    high = log(first[k][2])
    median = 0.5*(low + high)
    sigma = 0.1666*(high - low)
    list.append(ParameterRecord(name, median, sigma))
  return list

def create_new_player_file(tournament, player_id):
  file_name = '/home/feike/BoomstraDam/CEM/series_' + str(tournament.Series).zfill(2) + '/' + str(player_id).zfill(2) + '.weight'
  file = open(file_name, 'w')
  file.write('1.0\n')
  file.write('1.0\n')
  for i in range(0, len(tournament.ParameterList)):
    guess = gauss(tournament.ParameterList[i].mean, tournament.ParameterList[i].sigma)
    if exp(guess) < first[i][1]:
      val = first[i][1]
    elif exp(guess) > first[i][2]:
      val = first[i][2]
    else:
      val = exp(guess)
    file.write(str(val) + '\n')
  return

def create_next_parameter_list(old_swiss):
  list = []
  t_rating = []
  ratio = []
  for k in range(0, 29):
    list.append([])
  for k in range(0, 10):
    t_rating.append(old_swiss.Players[k].TournamentRating)
    file_name = '/home/feike/BoomstraDam/CEM/series_' + str(old_swiss.Series).zfill(2) + '/' + str(old_swiss.Players[k].id).zfill(2) + '.weight'
    f = open(file_name, 'r')
    f.readline()
    f.readline()
    for z in range(0, 29):
      list[z].append(log(float(f.readline())))
    f.close()
  # collected the info
  for k in range(0, 10):
    ratio.append(rating(t_rating[k]-t_rating[-1]))
  norm = sum(ratio)/float(len(ratio))
  for k in range(0, 10):
    ratio[k] = ratio[k]/norm

  param_list = []
  for z in range(0, 29):
    avg = weighted_average(list[z], ratio)
    sigma = weighted_std_dev(list[z], ratio)
    param_list.append(ParameterRecord(first[z][0], avg, sigma))

  return param_list

