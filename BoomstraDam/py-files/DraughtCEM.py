#
#  This file is Copyright (C) 2010 Feike Boomstra.
#  Distributed under the Boost Software License, Version 1.0.
#  (See accompanying file LICENSE_1_0.txt or copy at
#  http://www.boost.org/LICENSE_1_0.txt)
#


from DraughtSwissTournamentRating import *
from DraughtCEMcalc import *
from DraughtCEMadmin import *
from pickle import *

class DraughtCEM:
  def __init__(self, main):
    self.main = main
    self.runs = 0
    self.score = 0
    self.win = 0
    self.draw = 0
    self.loss = 0
    return

  def start_learning(self):
    self.main.play.startup_engine()
    self.main.play.engine.send_self_play(4, 97, 99)
    return

  def receive_result(self, msg):
    print msg[1:]
    p = int(msg[1])
    self.score += p
    if p == 2:
      self.win += 1
    elif p == 1:
      self.draw += 1
    else:
      self.loss += 1

    self.runs += 1
    self.main.play.panel.result_text.set(str(self.runs) + ' (' + str(self.score) + ': ' + str(self.win) + '-'+ str(self.draw) + '-'+str(self.loss)+')')
    if self.runs == 100:
      print str(self.runs) + ' (' + str(self.score) + ': ' + str(self.win) + '-'+ str(self.draw) + '-'+str(self.loss)+')'
      return
    self.main.play.engine.send_self_play(4, 97, 99)
    return


  def start_learning2(self):
    f = self.open_latest_pickle_file()
    self.Swiss = load(f)
    f.close()
    self.main.play.RescueProcess = self.start_learning
    self.Swiss.main = self.main
    self.Swiss.continue_playing()
    return



  def receive_result2(self, msg):
    print msg[1:]
    p = int(msg[1])
    if p == 0:
      self.Swiss.Tables[self.Swiss.CurrentTable].playerB.score += 2
      self.Swiss.Tables[self.Swiss.CurrentTable].playerA.results.append(0)
      self.Swiss.Tables[self.Swiss.CurrentTable].playerA.results_ever.append(0)
      self.Swiss.Tables[self.Swiss.CurrentTable].playerB.results.append(2)
      self.Swiss.Tables[self.Swiss.CurrentTable].playerB.results_ever.append(2)
    elif p == 1:
      self.Swiss.Tables[self.Swiss.CurrentTable].playerA.score += 1
      self.Swiss.Tables[self.Swiss.CurrentTable].playerB.score += 1
      self.Swiss.Tables[self.Swiss.CurrentTable].playerA.results.append(1)
      self.Swiss.Tables[self.Swiss.CurrentTable].playerA.results_ever.append(1)
      self.Swiss.Tables[self.Swiss.CurrentTable].playerB.results.append(1)
      self.Swiss.Tables[self.Swiss.CurrentTable].playerB.results_ever.append(1)
    else:
      self.Swiss.Tables[self.Swiss.CurrentTable].playerA.score += 2
      self.Swiss.Tables[self.Swiss.CurrentTable].playerA.results.append(2)
      self.Swiss.Tables[self.Swiss.CurrentTable].playerA.results_ever.append(2)
      self.Swiss.Tables[self.Swiss.CurrentTable].playerB.results.append(0)
      self.Swiss.Tables[self.Swiss.CurrentTable].playerB.results_ever.append(0)
    self.Swiss.CurrentTable += 1
    self.Swiss.status = "received result"
    self.Swiss.NrGames += 1
    self.main.play.panel.result_text.set(str(self.Swiss.NrGames) + ' (' + str(self.Swiss.TotNrOfRounds) + ') [' + str(self.Swiss.Round) + ']')
    self.Swiss.continue_playing()
    return

  def open_latest_pickle_file(self):
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

  def anal_swiss(self):
    f = self.open_latest_pickle_file()
    self.Swiss = load(f)
    f.close()
    self.Swiss.main = self.main
    pass
    alt_calc_tournament_rating(self.Swiss)

#    for player in self.Swiss.Players:
#      player.sort_style = "tournament rating"
#    self.Swiss.Players.sort()
#
#    self.Swiss.status = "start next series"
#    self.Swiss.continue_playing()
    return

  def init_swiss(self):
    self.Swiss = SwissTournament(None)
    self.Swiss.ParameterList = create_first_parameter_list()
    file_name = '/home/feike/BoomstraDam/CEM/series_00/param.list'
    f = open(file_name, 'w')
    for z in range(0, 29):
      f.write(str(self.Swiss.ParameterList[z]) + '\n')
    f.close()

    file_name = '/home/feike/BoomstraDam/CEM/series_00/swiss.pickle'
    f = open(file_name, 'w')
    dump(self.Swiss, f)
    f.close()
    return



if __name__=='__main__':
  main = None
  cem = DraughtCEM(main)
  cem.anal_swiss()
