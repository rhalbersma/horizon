#
#  This file is Copyright (C) 2010 Feike Boomstra.
#  Distributed under the Boost Software License, Version 1.0.
#  (See accompanying file LICENSE_1_0.txt or copy at
#  http://www.boost.org/LICENSE_1_0.txt)
#


from random import *
from math import *
from DraughtCEMcalc import *
from DraughtSwissTournamentRating import *
from pickle import *

class Table():
  def __init__(self, playerA, playerB):
    self.playerA = playerA
    self.playerB = playerB
    self.status = "waiting"
    return
  def __str__(self):
    return str(self.playerA)+' - '+str(self.playerB) + ' (' + self.status + ')'

class ScorePoule():
  def __init__(self, score):
    self.current_members = []
    self.added_players = []
    self.previous_added = []
    self.score = score
    return


class Player():
  def __init__(self, id):
    self.TournamentRating = 1000
    self.DeltaRating = 0
    self.id = id
    self.score = 0
    self.rounds = 0
    self.wp = 0
    self.wp_hi_lo = 0
    self.sb = 0
    self.opponents = []
    self.opponents_ever = []
    self.opponents_score = []
    self.results = []
    self.results_ever = []
    self.free = True
    self.sort_style = "pairing"
    return

  def __str__(self):
    if self.sort_style == "pairing":
      return '['+"%2d"%self.id+']'+"%2d"%self.score+'/'+"%3d"%self.wp+'/'"%3d"%self.sb+'/'
    elif self.sort_style == "rating":
      return '['+"%2d"%self.id+']'+"%2d"%self.score+'/'+"%4d"%self.Rating+'/'
    elif self.sort_style == "final":
      return '['+"%2d"%self.id+']'+"%2d"%self.score+'/'+"%3d"%self.wp_hi_lo+'/'
    elif self.sort_style == "tournament rating":
      return '['+"%2d"%self.id+']'+"%2d"%self.score+'/'+"%4d"%self.TournamentRating+'/'
    elif self.sort_style == "pairing on rating":
      return '['+"%2d"%self.id+']'+"%2d"%self.score+'/'+"%4d"%self.TournamentRating+'/'
    else:
      return '['+"%2d"%self.id+']'

  def print_opp(self):
    return str(self.free) + ' ' + str(self.opponents)

  def __eq__(self, a):
    #return self.score == a.score and self.wp == a.wp and self.sb == a.sb and self.id == a.id
    return self.id == a.id

  def __gt__(self, a):
    if self.sort_style == "pairing":
      # implemented in such a way that sort
      # generate a decreasing order
      # a[0] contains the highest score,
      # a[-1] contains the lowest score
      if self.score < a.score:
        return True
      elif self.score > a.score:
        return False
      else: # score is equal
        if self.wp < a.wp:
          return True
        elif self.wp > a.wp:
          return False
        else: # wp is equal
          if self.sb < a.sb:
            return True
          elif self.sb > a.sb:
            return False
          else: # sb is equal
            if self.id > a.id: # the order for id is reversed (that's the way it is in the document
              return True
            else:
              return False
    elif self.sort_style == "rating":
      return self.Rating < a.Rating
    elif self.sort_style == "id":
      return self.id > a.id
    elif self.sort_style == "final":
      if self.score < a.score:
        return True
      elif self.score > a.score:
        return False
      else: # score is equal
        if self.wp_hi_lo < a.wp_hi_lo:
          return True
        else: # wp_hi_low >=
          return False
    elif self.sort_style == "tournament rating":
      if self.TournamentRating < a.TournamentRating:
        return True
    elif self.sort_style == "pairing on rating":
      if self.score < a.score:
        return True
      elif self.score > a.score:
        return False
      else: # score is equal
        if self.TournamentRating < a.TournamentRating:
          return True # same point, higher rating
        else:
          return False






class Match():
  def __init__(self, tournament, playerA, playerB):
    self.PlayerA = playerA
    self.PlayerB = playerB
    if self.PlayerA.dummy == True:
      self.PlayerB.score += 2
      return
    if self.PlayerB.dummy == True:
      self.PlayerA.score += 2
      return
    self.MatchAgression = 0.5*(playerA.PlayStyle + playerB.PlayStyle)
    self.RatingDifference = float(playerA.Rating - playerB.Rating)
    # A is supposed to play white
    self.P = 1.0/(1.0 + 10**(-self.RatingDifference/400.))
    self.pw, self.pr, self.pb = self.calc_pw(self.P, self.MatchAgression)
    p = random()
    if p < self.pb:
      self.Result = 0
      self.PlayerB.score += 2
      self.PlayerA.results.append(0)
      self.PlayerA.results_ever.append(0)
      self.PlayerB.results.append(2)
      self.PlayerB.results_ever.append(2)
    elif p<self.pb + self.pr:
      self.Result = 1
      self.PlayerA.score += 1
      self.PlayerB.score += 1
      self.PlayerA.results.append(1)
      self.PlayerA.results_ever.append(1)
      self.PlayerB.results.append(1)
      self.PlayerB.results_ever.append(1)
    else:
      self.Result = 2
      self.PlayerA.score += 2
      self.PlayerA.results.append(2)
      self.PlayerA.results_ever.append(2)
      self.PlayerB.results.append(0)
      self.PlayerB.results_ever.append(0)
    tournament.NrGames += 1
    return

  def calc_pw(self, P, alpha):
     R = 2*min([P, 1-P])
     R_used = R*(0.1 + 0.8*alpha)
     pr = R_used
     pw = P - 0.5*pr
     pb = 1-pw-pr
     return pw, pr, pb


class SwissTournament():
  def __init__(self, main):
    self.Players = []
    self.Player_on_id = []
    self.Tables = []
    self.ScorePoules = []
    self.ParameterList = []
    self.Series = 0
    self.MinLevel = 0
    self.NrPlayers = 100
    self.Round = 0
    self.NrRounds = 6
    self.TotNrOfRounds = 0
    self.NrGames = 0
    self.PouleSize = 100
    self.PairingMode = 'pairing'
    self.avg_std_dev = 0.0
    for k in range(0, 100):
      self.Players.append(Player(k))
    self.Player_on_id[:] = self.Players[:]
    for player in self.Players:
      player.sort_style = self.PairingMode
    self.Players.sort()
    self.P3 = make_array(100, [])
    self.P8 = make_array(100, [])
    self.status = "play next tournament"
    self.CurrentTable = 0
    self.main = main
    self.avg_05_10_15_diff = []
    self.TopTen = []
    return

  """
    next generates the players to be moved to the next poule. Remove_step is == -1 means that
    the move includes added players. As all but the added players have been tried already, the
    player in a[0] must be an added player, so a[0] may not be incremented. The best way to explain
    the sequence is by example: we have the poule [10,12,8,25,33,31,29], so 7 players. 10 and 12 are added.
    We are moving 3 players. a is the seed, last is the highest index to use. Assume remove_step == 1:
    the first vector a offered is [2,3,4], next generates [2,3,5]-> [2,3,6]-> [2,4,5]-> [2,4,6]-> [2,5,6]-> [3,4,5]-> [3,4,6]-> [3,5,6]-> [4,5,6]-> []
    if this failes, at a certain moment it will be called with allowance to move added players:
    if the seed is: [1,2,3] then -> [1,2,4]-> [1,2,5}-> [1,2,6]-> [1,3,4]-> [1,3,5]-> [1,3,6]-> [1,4,5]-> [1,4,6]-> [1,5,6]-> []
    and for [0,1,2]: -> [0,1,3]
  """
  def next(self, a, last, remove_step):
    if len(a) == 0: return []
    l = len(a)
    if a[-1] < last:
      a[-1] += 1
      return a
    else:
      for n in range(l-2, -1, -1): # go over a previous element to carry the overflow
        if n == 0 and remove_step == -1: # were are moving added players
          return []                      # so a[0] has to keep the added player, no increments of a[0]
        if a[n] < last - (l-1-n):
          a[n] += 1
          for m in range(n+1, l): #ripple back
            a[m] = a[n] + (m - n)
          if a[-1] > last: # we have gone too far
            return []
          else:
            return a
    return []

  def dump(self):
    print '\n-------------table dump-------------------\n'
    n = 0
    for table in self.Tables:
      print '{',"%2d"%n,'}', table
      n += 1
    print '\n-------------poule dump-------------------\n'
    n = 0
    for poule in self.ScorePoules:
      print '{',"%2d"%n,'}-----', len(poule.current_members),'-------'
      for poule in self.ScorePoules:
        print '{',"%2d"%n,'}-----', len(poule.current_members),'-------'
        for player in poule.current_members:
          print '    ', player, player.opponents
        n += 1

  def unmake_pairs(self, pairing_group):
    assert len(pairing_group) % 2 == 0
    for k in range(0, len(pairing_group)/2):
      assert self.Tables[-1].playerA in pairing_group
      assert self.Tables[-1].playerB in pairing_group
      self.Tables[-1].playerA.free = True
      self.Tables[-1].playerB.free = True
      self.Tables[-1].playerA.opponents.remove(self.Tables[-1].playerB.id)
      self.Tables[-1].playerA.opponents_ever.remove(self.Tables[-1].playerB.id)
      self.Tables[-1].playerB.opponents.remove(self.Tables[-1].playerA.id)
      self.Tables[-1].playerB.opponents_ever.remove(self.Tables[-1].playerA.id)
      self.Tables.pop()   # remove the last Tabl entry
    return



  def make_pairs(self, level, first, last, pairing_group):
    if len(pairing_group) == 0:
      return True
    for item in pairing_group:
      if len(item.opponents) <> self.Round:
        self.dump()
        assert len(item.opponents) == self.Round
    for player in pairing_group:
      assert player.free == True
    result = self.make_pair(level, first, last, pairing_group)
    if result == True:
      for item in pairing_group:
        assert len(item.opponents) == self.Round + 1
    else:
      for item in pairing_group:
        assert len(item.opponents) == self.Round
    return result


  def make_pair(self, level, first, last, pairing_group):
    if level < self.MinLevel: self.MinLevel = level
    if level == 0: return True
    for k in range(first, last):
      if pairing_group[k].free == True:
        for n in range(last, k, -1):
          if pairing_group[n].free == True:
            if (not (pairing_group[k].id in pairing_group[n].opponents)):
              self.set_pair(k, n, pairing_group)
              #next pair
              result = self.make_pair(level-1, k+1, last, pairing_group)
              if result == False:
                self.undo_pair(k, n, pairing_group)
              else:
                return True
      else:
        pass # try next start position
    #end for k
    return False

  def set_pair(self, k, n, poule):
    poule[k].free = False
    assert len(poule[k].opponents) == self.Round
    poule[k].opponents.append(poule[n].id)
    poule[k].opponents_ever.append(poule[n].id)
    poule[n].free = False
    assert len(poule[n].opponents) == self.Round
    poule[n].opponents.append(poule[k].id)
    poule[n].opponents_ever.append(poule[k].id)
#    print 'try ', k , 'with', n
    self.Tables.append(Table( poule[k], poule[n]))
    return

  def undo_pair(self, k, n, poule):
    poule[k].free = True
    poule[k].opponents.remove(poule[n].id)
    poule[k].opponents_ever.remove(poule[n].id)
    poule[n].free = True
    poule[n].opponents.remove(poule[k].id)
    poule[n].opponents_ever.remove(poule[k].id)
#    print 'undo', k , 'with', n
    self.Tables.pop()
    return

  def print_poule(self, poule):
    for player in poule:
      print poule.index(player), player.print_opp()
    return

  def print_free_poule(self, poule):
    for player in poule:
      if player.free == True:
        print poule.index(player), player.opponents
    return

  def calc_wp_and_sb(self, player):
    player.wp = 0
    player.sb = 0
    for pl in player.opponents:
      #print pl, self.Player_on_id[pl].id, self.Player_on_id[pl].score, player.results[player.opponents.index(pl)]
      player.wp += self.Player_on_id[pl].score
      player.opponents_score.append(self.Player_on_id[pl].score)
      player.sb += self.Player_on_id[pl].score * player.results[player.opponents.index(pl)]
    player.wp_hi_lo = player.wp - min(player.opponents_score) - max(player.opponents_score)
    return

  def switch_pairing_mode(self):
    self.PairingMode = 'tournament pairing'
    for player in self.Players:
      player.TournamentRating = player.score
      player.score = 0
    return



  def std_dev(self, list):
    mu = 0.0
    mu = 0.0
    n = float(len(list))
    for item in list:
      mu += float(item)
    mu = mu/n
    delta = 0.0
    for item in list:
      delta += (mu - float(item))**2
    delta = sqrt(delta/(n-1))
    return delta

    n = float(len(list))
    for item in list:
      mu += float(item)
    mu = mu/n
    delta = 0.0
    for item in list:
      delta += (mu - float(item))**2
    delta = sqrt(delta/(n-1))
    return delta

  def check_poule_structure(self, cur_poule, location):
    tot_mem = 0
    for poule in self.ScorePoules:
      tot_mem += len(poule.current_members)
    assert tot_mem == self.NrPlayers          # no players lost
    upto_mem = 0
    for poule in self.ScorePoules[0:cur_poule]:
      upto_mem += len(poule.current_members)
    assert upto_mem % 2 == 0
    if upto_mem/2 <> len(self.Tables):
      self.dump()
      assert upto_mem/2 == len(self.Tables)     # allready paired must be sitting
    for poule in self.ScorePoules:
      for item in poule.current_members:
        count = 0
        for t_item in poule.current_members:
          if t_item.id == item.id: count += 1
        #if count > 1: return False
        assert count == 1
    for poule in self.ScorePoules[0:cur_poule]:
      for item in poule.current_members:
        assert len(item.opponents) == self.Round + 1
    for poule in self.ScorePoules[cur_poule:]:
      for item in poule.current_members:
        assert len(item.opponents) == self.Round
    return True

  def prepare_pairing(self):
    for player in self.Players: player.free = True
    self.Tables = []
    self.ScorePoules = []
    # make initial distribution over scoring-poules
    for k in range(self.Players[0].score, self.Players[-1].score-1, -1):
      self.ScorePoules.append(ScorePoule(k))
    n = 0 # the index in ScorePoules
    for k in range(0, len(self.Players)): #players is sorted High to Low
      while self.ScorePoules[n].score > self.Players[k].score:
        n += 1
      pk = self.Players[k]
      self.ScorePoules[n].current_members.append(pk)
    return

  def pairing_done(self, start_poule_nr):
    PairsNeeded = len(self.ScorePoules[start_poule_nr].current_members)/2 # nrs of pairing needed
    self.MinLevel = PairsNeeded # preset high
    if PairsNeeded > 0 :self.make_pairs(PairsNeeded, 0, PairsNeeded*2-1, self.ScorePoules[start_poule_nr].current_members)
    return self.MinLevel == 0  # all remaining players in this poule have been paired

  def move_forward(self, player, start_poule_nr):
    from_poule = self.ScorePoules[start_poule_nr]
    to_poule = self.ScorePoules[start_poule_nr + 1]
    assert player in from_poule.current_members
    assert not (player in to_poule.current_members)
    assert not (player in to_poule.added_players)
    assert not (player in from_poule.previous_added)
    from_poule.current_members.remove(player)
    to_poule.current_members.append(player)
    to_poule.added_players.append(player)
    from_poule.current_members.sort()
    to_poule.current_members.sort()
    if player in from_poule.added_players:
      from_poule.added_players.remove(player)
      from_poule.previous_added.append(player)
    return

  def move_backward(self, player, start_poule_nr):
    from_poule = self.ScorePoules[start_poule_nr + 1]
    to_poule = self.ScorePoules[start_poule_nr]
    assert player in from_poule.current_members
    assert not (player in to_poule.current_members)
    assert not (player in to_poule.added_players)
    from_poule.current_members.remove(player)
    from_poule.added_players.remove(player)
    to_poule.current_members.append(player)
    from_poule.current_members.sort()
    to_poule.current_members.sort()
    if player in to_poule.previous_added:
      to_poule.added_players.append(player)
      to_poule.previous_added.remove(player)
    return



  def check_current(self, level, start_poule_nr, players_to_remove, allowed_self, allowed_next):
    # if not allowed to move added players, we start moving from the first non-added index
    # else first we start with the added one with the least score, and work upwards
    if allowed_self == True:
      if len(self.ScorePoules[start_poule_nr].added_players) > 0:
        start_remove = len(self.ScorePoules[start_poule_nr].added_players)-1 # at the lowest added player
        # does this still fit?
        if start_remove + players_to_remove > len(self.ScorePoules[start_poule_nr].current_members):
          start_remove = len(self.ScorePoules[start_poule_nr].current_members) - players_to_remove
        stop_remove = -1 # stop at the first location
        step_remove = -1 # go back ( from least score to higher score)
      else: # there are no added players
        start_remove = 0 # no added players to this poule
        stop_remove =  1
        step_remove =  1
    else:
      start_remove = len(self.ScorePoules[start_poule_nr].added_players) # at the first non-added player
      if len(self.ScorePoules[start_poule_nr].added_players) == len(self.ScorePoules[start_poule_nr].current_members):
        return False                                     # nothing to do
      else:
        stop_remove = start_remove + 1                                     # just one to do
      step_remove = +1

    for remove_starting_point in range(start_remove, stop_remove, step_remove):
      a = []
      for n in range(0, players_to_remove): a.append(remove_starting_point + n)
      z = []
      for r in a: z.append(self.ScorePoules[start_poule_nr].current_members[r])
      # z contains the players to be moved to the next poule, a contains their indices in the current poule
      while a <> []:
        for x in z:
          self.move_forward(x, start_poule_nr)
          #print 'moved forward  (', start_poule_nr, ') player', x
        #candidates are removed, try to pair
        if not (self.pairing_done(start_poule_nr) == True):
          for x in z:
            self.move_backward(x, start_poule_nr)
            #print 'moved backward (', start_poule_nr, ') player', x
          #self.check_poule_structure(start_poule_nr, 3)
        else: # try remaining
          #print "made pairs in  ", start_poule_nr
          if self.pair_remaining_poules(level-1, start_poule_nr+1, allowed_next) == True:           # try to pair remaining poules
            #print "solved", start_poule_nr
            return True                                                                   # ok
          else: # we have to restore the original poule
            self.unmake_pairs(self.ScorePoules[start_poule_nr].current_members)
            #print "unmade pairs in", start_poule_nr
            for x in z:
              self.move_backward(x, start_poule_nr)
              #print 'moved backward (', start_poule_nr, ') player', x
            #self.check_poule_structure(start_poule_nr, 2)
        a = self.next(a, len(self.ScorePoules[start_poule_nr].current_members)-1, step_remove)
        z = []
        for r in a: z.append(self.ScorePoules[start_poule_nr].current_members[r])

      # done with all z, but no solution yet
    # done with all removals of length players_to_remove
    return False


  def pair_remaining_poules(self, level, start_poule_nr, allow_here_use_of_added_players):
    #print 'enter with', level, start_poule_nr, allow_here_use_of_added_players, 'len=', len(self.ScorePoules[start_poule_nr].current_members),
    #print 'added', len(self.ScorePoules[start_poule_nr].added_players)
    #self.check_poule_structure(start_poule_nr, 1)
    if len(self.ScorePoules[start_poule_nr].current_members) == 2 and start_poule_nr == 0:
      pass
    if len(self.ScorePoules[start_poule_nr].current_members) == 0:
      #print 'solved', start_poule_nr, ' with len=0'
      return self.pair_remaining_poules(level-1, start_poule_nr+1, allow_here_use_of_added_players)
    if level == 0: # were are at the last poule, no more trics are possible
      if self.pairing_done(start_poule_nr) == True:
        #print "solved", start_poule_nr
        return True
      else: #debug
        #self.dump()
        return False
    # not level 0, so work to do:
    else:
      # calculate the min amount of players to be removed:
      if len(self.ScorePoules[start_poule_nr].current_members) % 2 == 1:
        min_players_to_remove = 1
      else:
        min_players_to_remove = 0
      if allow_here_use_of_added_players == True:
        max_players_to_remove = len(self.ScorePoules[start_poule_nr].current_members)
      else:
        max_players_to_remove = len(self.ScorePoules[start_poule_nr].current_members) - len(self.ScorePoules[start_poule_nr].added_players)
        if min_players_to_remove == 1 and max_players_to_remove < min_players_to_remove: # nothing to do
          return False

# outermost loop, removing as many as players as needed
# if unsuccesfull then merge
      for allow_next_use_of_added_players in [False, True]:
        for players_to_remove in range(min_players_to_remove, max_players_to_remove + 1, 2):
          if players_to_remove == 0:  # special case
            if self.pairing_done(start_poule_nr) == True:
              # ok so far, now try next poules
              #print "made pairs in  ", start_poule_nr
              if self.pair_remaining_poules(level-1, start_poule_nr+1, allow_next_use_of_added_players) == True:
                #print "solved", start_poule_nr
                return True              # ok
              else: # failed to pair next poules
                self.unmake_pairs(self.ScorePoules[start_poule_nr].current_members) # clear previous succesfulle paired cuurent poule
                #print "unmade pairs in", start_poule_nr
                #self.check_poule_structure(start_poule_nr, 4)
            else: # pairing not succesfull
              pass
          else:  # players to remove
            if self.check_current(level, start_poule_nr, players_to_remove, allow_here_use_of_added_players, allow_next_use_of_added_players) == True:
              return True
        # next player_to_remove

      # so all things failed, we have to merge, to be done
      #self.dump()
      return False

  def pair_round_0(self):
    half_way = len(self.Players)/2
    for k in range(0, half_way):
      self.set_pair(k, k+half_way, self.Players)
    return

    """
    play one tournament,
    number of rounds: self.Rounds
    poule_size:       self.PouleSize
    pairing:          self.PairingMode
    for pairing there are two styles:
     'pairing', the pairing on score,wp and sb
     'pairing on rating', the pairing on score and rating
    a tournament is played on top of the result of the previous tournament
    so the players keep their scores, but the opponents of the previous tournament are ereased
    to be able to calc the tournament rating over all tournaments, two lists are maintained:
    opponents_ever and results_ever

    so prepare the players
    sort the players according the pairing style
    play the rounds
    calc_tournament_rating calculate the tournament rating and from that
    it updates the number of players staying for the next tournament,
    the number of players
    the pairing style
    the number of rounds
    """

  def update_status(self, status):
    self.status = status
    save_main = self.main
    self.main = None
    file_name = '/home/feike/BoomstraDam/CEM/series_' + str(self.Series).zfill(2) + '/swiss.pickle'
    f = open(file_name, 'w')
    dump(self, f)
    f.close()
    self.main = save_main
    return



  def continue_playing(self):
    while True:
      if self.status == "play next tournament":
        for player in self.Players:
          player.sort_style = self.PairingMode
          player.opponents = []
          player.opponents_score = []
          player.free = True
        self.Players.sort()
        self.Round = 0
        self.update_status ("start next round")
#
      elif self.status == "start next round":
        self.CurrentTable = 0
        self.prepare_pairing()
        if self.Round == 0 or self.Round == 1:
          self.pair_round_0()
        else:
          if not self.pair_remaining_poules(len(self.ScorePoules)-1, 0, False) == True:
            print "Can't pair these poules, try merge"
            self.dump()
            return
        self.update_status ("play next game")
#
      elif self.status == "play next game":
        self.main.play.startup_engine()
        self.main.play.engine.send_self_play(self.Series, self.Tables[self.CurrentTable].playerA.id, self.Tables[self.CurrentTable].playerB.id)
        return
#
      elif self.status == "received result":
        if self.CurrentTable == len(self.Tables):
          self.Round += 1
          if self.PairingMode == "pairing":
            for player in self.Players:
              self.calc_wp_and_sb(player)
          self.Players.sort()
          if self.Round == self.NrRounds:
            result = calc_tournament_rating(self)
            if result == True:
              self.update_status("play next tournament")
            else:
              self.update_status("start next series")
          else: #not all rounds played yet
            self.update_status("start next round")
        else:
          self.update_status("play next game")

#
      elif self.status == "start next series":
        # new fresh tournament
        new_swiss = SwissTournament(None)
        new_swiss.ParameterList = create_next_parameter_list(self)
        new_swiss.Series = self.Series + 1
        new_swiss.main = self.main
        new_swiss.update_status("play next tournament")
        return
#        new_swiss.continue_playing()
      else:
        print "impossible status: ", self.status
    return




