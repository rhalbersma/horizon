#
#  This file is Copyright (C) 2010 Feike Boomstra.
#  Distributed under the Boost Software License, Version 1.0.
#  (See accompanying file LICENSE_1_0.txt or copy at
#  http://www.boost.org/LICENSE_1_0.txt)
#


from pickle import *
from DraughtCEMadmin import *


def nr_same(topten, a, b):
  nr_same_players_in_top_ten = 0
  for player_id in topten[a]:
    if player_id in topten[b]:
      nr_same_players_in_top_ten += 1
  return nr_same_players_in_top_ten


def open_latest_pickle_file():
  for k in range(0, 20):
    file_name = '/home/feike/BoomstraDam/CEM/series_' + str(k).zfill(2) + '/swiss.pickle'
    try:
      f = open(file_name, 'r')
    except:
      break
    f.close()
  k = k - 1
  file_name = '/home/feike/BoomstraDam/CEM/series_' + str(k).zfill(2)+ '/swiss.pickle'
  return open(file_name, 'r')

def anal_swiss():
  f = open_latest_pickle_file()
  swiss = load(f)
  f.close()

  print
  if swiss.TotNrOfRounds >= 18:
    print "start dev:    ", "%4.0f"%swiss.decay_poule_size.third_std_dev
    print "rnds needed:  ", "%4d"%swiss.decay_poule_size.nr_of_rounds_needed
  print "Nr of Games:  ", "%4d"%swiss.NrGames
  print "Nr of Rounds: ", "%4d"%(swiss.TotNrOfRounds + swiss.Round)
  print "Nr of Tourmnt:", "%4d"%len(swiss.TopTen)
  print "Poule Size:   ", "%4d"%swiss.PouleSize
  print

  if len(swiss.avg_05_10_15_diff) >= 5:
    print "same:",'\t',
    for z in range(len(swiss.avg_05_10_15_diff)-1, max(4, len(swiss.avg_05_10_15_diff)-8), -1):
      print "%5d"%nr_same(swiss.TopTen, z, z-1),'\t',
    print
    print "slope:",'\t',
    for z in range(len(swiss.avg_05_10_15_diff)-1, max(4, len(swiss.avg_05_10_15_diff)-8), -1):
      print "%5.2f"%slope(swiss.avg_05_10_15_diff[z-5:z]),'\t',
    print
    print "line",'\t',
    for z in range(len(swiss.avg_05_10_15_diff)-1, max(4, len(swiss.avg_05_10_15_diff)-8), -1):
      print "%5.2f"%std_dev_line(swiss.avg_05_10_15_diff[z-5:z]),'\t',
    print
    print "diff:",'\t',
    for z in range(len(swiss.avg_05_10_15_diff)-1, max(4, len(swiss.avg_05_10_15_diff)-8), -1):
      print "%5.2f"%swiss.avg_05_10_15_diff[z],'\t',
    print
  print

  for player in swiss.Players:
    player.sort_style = "tournament rating"
  swiss.Players.sort()
# players are sorted according these ratings

  p99_found = False
  for z in range(0, len(swiss.Players)):
    if swiss.Players[z].id == 99:
      p99_found = True
      p99_location = z
      break
  if p99_found == True:
    print "99 at location:", p99_location
    print "delta rating:", "%4.0f"%(swiss.Players[0].TournamentRating - swiss.Players[p99_location].TournamentRating)
  else:
    print "99 disappeared"
    print "delta rating >", "%4.0f"%(swiss.Players[0].TournamentRating - swiss.Players[-1].TournamentRating)
  print

  pl_00_10 = []
  pl_00_15 = []
  for player in swiss.Players[0:10]:
    pl_00_10.append(player.TournamentRating)
  for player in swiss.Players[0:5]:
    pl_00_15.append(player.TournamentRating)
  for player in swiss.Players[10:15]:
    pl_00_15.append(player.TournamentRating)
  print "delta 10 15:  ", "%4.1f"%(average(pl_00_10) - average(pl_00_15))


  for z in range(0, 5):
    print swiss.Players[z]
  print
  for z in range(5, 10):
    print swiss.Players[z]
  print
  for z in range(10, 15):
    print swiss.Players[z]
  print


  return

anal_swiss()
