#
#  This file is Copyright (C) 2010 Feike Boomstra.
#  Distributed under the Boost Software License, Version 1.0.
#  (See accompanying file LICENSE_1_0.txt or copy at
#  http://www.boost.org/LICENSE_1_0.txt)
#


from math import *
from feike_math import *
from test_log_decay_poule_size import *

def calc_tournament_rating(self):
  min_score = rating(-400)
  max_score = rating(400)
  while True:
    tot_distance = 0
    tot_delta = 0
    tot_sign = 0
    for player in self.Players:
      player.tot_got = 0.0
      player.opp_rating = 0.0
      for k in range(0, len(player.opponents_ever)):
        player.opp_rating += self.Player_on_id[player.opponents_ever[k]].TournamentRating
        player.tot_got += player.results_ever[k]
      player.opp_rating /= len(player.opponents_ever)
      player.expected = rating(player.TournamentRating - player.opp_rating)
      player.tot_got /= len(player.opponents_ever)
      if player.tot_got > max_score: player.tot_got = max_score
      if player.tot_got < min_score: player.tot_got = min_score
      if (player.expected > player.tot_got):
        tot_sign += 1
      else: tot_sign -= 1
      tot_distance += (player.tot_got - player.expected)**2
      player.DeltaRating = calc_delta(player.TournamentRating, player.opp_rating, player.tot_got)
    for player in self.Players:
      player.TournamentRating += player.DeltaRating
      tot_delta += player.DeltaRating
    if (tot_distance*100.0)/self.PouleSize < 0.1: break
# we have updated the Tournament Rating

  for player in self.Players:
    player.sort_style = "tournament rating"
  self.Players.sort()
# players are sorted according these ratings

  for k in range(0, 1000):
    file_name = '/home/feike/BoomstraDam/CEM/series_' + str(self.Series).zfill(2) + '/' + 'stand_' + str(k).zfill(3) + '.list'
    try:
      f = open(file_name, 'r')
    except:
      break
    f.close()
  # first free fiel_name
  f = open(file_name, 'w')

  for player in self.Players:
    f.write(str(player)+ '\n')
  f.close()

  mu = 0.0
  n = float(len(self.Players))
  for player in self.Players:
    mu += float(player.TournamentRating)
  mu = mu/n
  sigma = 0.0
  for player in self.Players:
    sigma += (mu - float(player.TournamentRating))**2
  sigma = sqrt(sigma/(n-1))
# distribution is caculated

  list = []
  for player in self.Players[5:10]:
    list.append(player.TournamentRating)
  avg_05_10 = average(list)
  list = []
  for player in self.Players[10:15]:
    list.append(player.TournamentRating)
  avg_10_15 = average(list)
  self.avg_05_10_15_diff.append(avg_05_10 - avg_10_15)
  if len(self.avg_05_10_15_diff) >= 5:
    stop_because_it_is_stable = (abs(slope(self.avg_05_10_15_diff[-5:])) < 0.12 and (abs(std_dev_line(self.avg_05_10_15_diff[-5:]))) < 0.12)
  else:
    stop_because_it_is_stable = False

  list = []
  for player in self.Players[0:10]:
    list.append(player.id)
  all_players_in_previous_top_ten = True  # preset
  for id in list:
    if not (id in self.TopTen):
      all_players_in_previous_top_ten = False
      break
  self.TopTen.append(list)

# setup parameters for the next tournament
  self.TotNrOfRounds += self.NrRounds

  if self.TotNrOfRounds == 18:
    self.decay_poule_size = DecayPouleSize(sigma)
#    print '-------------'
#    for z in range(0, 100):
#      print self.Players[z].TournamentRating, self.Players[z].Rating
#    tr = []
#    for k in range(0, 100): tr.append(self.Players[k].TournamentRating)
#    print tip_dist(tr),  id_nr
#    for k in range(0, 100):
#      z = self.Players[k].TournamentRating
#      p = result[k]
#      p.append(z - 1000)





# first three series is with poule-size = 100 and nr_rounds = 6
# then depending on the obtained standard deviation the needed nr of rounds and the start poule size is calculated
  if self.TotNrOfRounds < 18:
    new_poule_size = 100
  else:
    new_poule_size = self.decay_poule_size.get_next_poule_size(self.TotNrOfRounds)

  self.PouleSize = new_poule_size


#adjust the number of rounds according to the poule size
  if self.PouleSize > 64:
    self.NrRounds = 6
  else:
    self.NrRounds = 5
  if self.TotNrOfRounds > 100:
    self.PairingMode = "pairing on rating"

# reduce the poule_size
  self.Players[:] = self.Players[0:self.PouleSize]


  for player in self.Players:
    if self.PairingMode == "pairing":
      player.sort_style = "final"
    else:
      player.sort_style = "tournament rating"
  self.Players.sort()
# players are sorted according mode


  if self.TotNrOfRounds < 18:
    return True
  else:
    return (self.TotNrOfRounds < self.decay_poule_size.nr_of_rounds_needed + 18) \
    and not (stop_because_it_is_stable == True)\
    and not ((avg_05_10 - avg_10_15) < 10.0)
    #and not (all_players_in_previous_top_ten == True)

def calc_delta(ratingA, ratingB, got):
  # we have to reverse the rating function
  if rating( 400) < got: return 400
  if rating(-400) > got: return -400
  rating_try_high = ratingB + 400
  rating_try_low  = ratingB - 400
  while rating_try_high - rating_try_low > 1:
    rating_try = 0.5*(rating_try_high+rating_try_low)
    if rating(rating_try - ratingB) > got:
      rating_try_high = rating_try
    else:
      rating_try_low  = rating_try
  return 0.1*(rating_try - ratingA)

def rating(diff):
  return 2 * 1.0/(1.0 + 10**((-diff)/400.))

def inv_distr(alpha, mu, sigma, result):
  # find x so result = alpha/exp(-((x-mu)/sigma)**2)
  # we only want to explore the left side of the curve
  # so our start-value of x is 0.5 mu
  # in this case an x < 10 is not acceptable, so we limit x 10 < x < mu
  # if result > 100 -> return 100
  if result > 100: return 100
  if alpha*exp(-0.5*((10-mu)/sigma)**2) > result: return 10
  x_high = mu
  x_low = 10
  while x_high - x_low > 1:
    x_try = 0.5*(x_high + x_low)
    if alpha*exp(-0.5*((x_try-mu)/sigma)**2) > result:
      x_high = x_try
    else:
      x_low = x_try
  return x_try

def tip_dist(list):
  a, b = best_fit(list[20:81])
  d = 0
  for k in range(0, 10):
    d += abs((a*k+b) - list[k])
#  for k in range(90, 100):
#    d += abs((a*k+b) - list[k])
  return d

def alt_calc_tournament_rating(self):
  prev_tot_distance = 0
  min_score = rating(-400)
  max_score = rating(400)
  current_id_poule = []
  for player in self.Players:
    current_id_poule.append(player.id)
    player.TournamentRating = 1000
  while True:
    tot_distance = 0
    tot_delta = 0
    tot_sign = 0
    for player in self.Players:
      player.tot_got = 0.0
      player.opp_rating = 0.0
      player.nr_of_oponents_ever = 0
      for k in range(0, len(player.opponents_ever)):
        if player.opponents_ever[k] in current_id_poule:
          player.opp_rating += self.Player_on_id[player.opponents_ever[k]].TournamentRating
          player.tot_got += player.results_ever[k]
          player.nr_of_oponents_ever += 1
      player.opp_rating /= player.nr_of_oponents_ever
      player.expected = rating(player.TournamentRating - player.opp_rating)
      player.tot_got /= player.nr_of_oponents_ever
      if player.tot_got > max_score: player.tot_got = max_score
      if player.tot_got < min_score: player.tot_got = min_score
#      if (player.expected > player.tot_got):
#        tot_sign += 1
#      else: tot_sign -= 1
      tot_distance += (player.tot_got - player.expected)**2
      player.DeltaRating = calc_delta(player.TournamentRating, player.opp_rating, player.tot_got)
    for player in self.Players:
      player.TournamentRating += player.DeltaRating
      tot_delta += player.DeltaRating
    if (tot_distance*100.0)/self.PouleSize < 0.1: break
    if abs(prev_tot_distance - tot_distance) < 0.001: break
    prev_tot_distance = tot_distance
# we have updated the Tournament Rating

  for player in self.Players:
    player.sort_style = "tournament rating"
  self.Players.sort()
  for player in self.Players:
    print player
