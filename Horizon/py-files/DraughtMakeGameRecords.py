#
#  This file is Copyright (C) 2010 Feike Boomstra.
#  Distributed under the Boost Software License, Version 1.0.
#  (See accompanying file LICENSE_1_0.txt or copy at
#  http://www.boost.org/LICENSE_1_0.txt)
#


from DraughtDataStructures import *
import pickle

class DraughtMakeGameRecords:
    def __init__(self, main):
        self.main = main
        return

    def MakeGameRecords(self):
        RatingFile = '/home/feike/workspace/collected/fmjd_rating_04_2009.txt'
        InputFile = '/home/feike/workspace/collected/partijen 2006.txt'
#        OutDir = '/home/feike/BoomstraDam/pdn files/game records/'
#        GameNumber = 0;
        rating = open(RatingFile, 'r')
        rating_name_list = []
        rating_value_list = []
        while 1:
            try:
                rating_line = rating.readline()
            except:
                break
            try:
                rating_name = rating_line[0:rating_line.index('\t')]
            except:
                break
            rating_value = int(rating_line[rating_line.index('\t')+2:-1])
            rating_name_list.append(rating_name)
            rating_value_list.append(rating_value)
        rating.close()
        # got the rating info per player
        ResultRating = '/home/feike/workspace/game records.csv'
        ofile = open(ResultRating, 'w')
        file = open(InputFile, 'r')
        #while 1:
        for i in range(0,4000):
            while 1:
                try: line1 = file.readline()
                except: break
                try:
                    line1.index('(')
                    break # if ( there it is ok
                except: err=1
            try: line2 = file.readline()
            except: break
            try: line3 = file.readline()
            except: break
            try: line4 = file.readline()
            except: break
            try: line5 = file.readline()
            except: break
            try: line6 = file.readline()
            except: break


            #print line1[0:-2]

            try: FirstName = line1[line1.index(')')+2: line1.index('-') -1]
            except: print "line1 = ", line1
            try: SecName = line1[line1.index('-') +2:-2]
            except: err=1
            #print FirstName
            #print SecName

            FirstNameFound = 1
            try:
                FirstValue = rating_value_list[rating_name_list.index(FirstName)]
            except:
                #print FirstName, "not found"
                FirstNameFound = 0

            SecNameFound = 1
            try:
                SecValue = rating_value_list[rating_name_list.index(SecName)]
            except:
                #print SecName, "not found"
                SecNameFound = 0

            if FirstNameFound and SecNameFound:
                #ofile.write(repr(FirstValue).rjust(4)+'\t' + repr(SecValue).rjust(4)+ '\t' + line4[0:-2] + line5[0:2] +'\n')
                self.main.read.s = line4[0:-2] + ' ' + line5[0:3] + ' \n\0'
                game = GameRecord(self.main, self.main.panel, self.main.panel)
                #self.main.read.Games.append(game)
                self.main.read.lastchrpnt = 0;
                cp = self.main.read.GetGame(0, game)
                if cp == 0:
                    #self.main.read.Games.remove(game)
                    print "Error in PDN, last char tried = ", self.main.read.lastchrpnt
                    print self.main.read.s[self.main.read.lastchrpnt-70:self.main.read.lastchrpnt]
##                    self.main.read.debug = '*'
##                    game = GameRecord(self.main, self.main.panel, self.main.panel)
##                    self.main.read.Games.append(game)
##                    cp = self.main.read.GetGame(0, game)
##                    self.main.read.debug=None
##                    break
                else:
                    # we have got a valid pdn
                    # do we want to collect this game?
                    if (FirstValue - SecValue > 150 and game.Result == '2-0') or \
                        (FirstValue - SecValue < -150 and game.Result == '0-2'):
                        game.set_begin_position()
                        GameNumber += 1
                        PDNfilename = OutDir + 'game-' + repr(GameNumber).zfill(4) + '.pdn'
                        PDNfile = open(PDNfilename, 'w')
                        PDN = self.main.save.game_to_string(game)
                        PDNfile.write(PDN)
                        PDNfile.close()
                        for i in range(0, game.get_half_ply_length()):
                            pos = game.get_next_board_layout()
                            if pos <> None: #valid move
                                ofile.write(repr(GameNumber).zfill(4)  + ' ' + repr(FirstValue).rjust(4) + ' ' + \
                                                repr(SecValue).rjust(4) + ' ' + repr(int(i/2)).rjust(2) + ' ' + \
                                                game.Result + ' ' +\
                                                pos + '\n')
                            else:
                                print "invalid move??"
        file.close()
        ofile.close()
        print "done"
        return


#if __name__=='__main__':
#    main = DraughtMakeGameRecords(main)
 #   main.MakeGameRecords()

    def MakeOpening(self):
      counter = 0
      self.hit = 0
      self.plies = 0
      file = open('dammen/openings_zetten.txt', 'r')
      self.PosList = []
      self.MoveDictList = []
      for line in file:
        if len(line) == 241:
          self.main.read.s = line + ' *' + '\0'
          game = GameRecord(self.main, self.main.panel, self.main.panel)
          self.main.read.lastchrpnt = 0;
          cp = self.main.read.GetGame(0, game)
          if cp == 0:
              #self.main.read.Games.remove(game)
              print "Error in PDN, last char tried = ", self.main.read.lastchrpnt
              print line
              return
          else:
            self.UpdateOpening(game)
            counter += 1
            if counter % 500 == 0:
              print counter, "openings read", len(self.PosList), "entries", self.hit, "hits", self.plies, "plies"

      self.MakePosDict()
      return

    def MakeMovesDict(self, game):
      ml = self.main.play.mg.valid_move(game)
      move_dict = []
      for move in ml:
        s = str(move.Start).zfill(2) + str(move.Stop).zfill(2)
        move_dict.append((s, 0))
      return dict(move_dict)

    def UpdateOpening(self, game):
      game.HalfPlyPointer = 0
      for k in range(0, game.get_half_ply_length()):
        self.plies += 1
        next_move = game.get_half_ply_pointer_record()
        valid_move = game.is_valid_move(next_move)
        if not valid_move : return None
        if game.is_color_white() == True: color = "W"
        else: color = "B"
        pos = color + game.get_50_sq_position()
        if pos in self.PosList:
#          print pos
#          print self.MoveDictList[self.PosList.index(pos)]
          self.MoveDictList[self.PosList.index(pos)][self.move_key(next_move)] += 1
          self.hit += 1
        else:
          self.PosList.append(pos)
          self.MoveDictList.append(self.MakeMovesDict(game))
        game.do_move(valid_move, False)
        game.HalfPlyPointer += 1
      return

    def move_key(self, move):
      return str(move.Start).zfill(2) + str(move.Stop).zfill(2)

    def MakePosDict(self):
      self.Dict = []
      for k in range(0, len(self.PosList)):
        self.Dict.append((self.PosList[k], self.MoveDictList[k]))
      self.Dict = dict(self.Dict)
      file = open('/home/feike/Horizon/Opening Books/PosDict.pickle', 'w')
      pickle.dump(self.Dict, file)
      file.close()
      return
