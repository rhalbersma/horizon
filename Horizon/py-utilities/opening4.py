#
#  This file is Copyright (C) 2010 Feike Boomstra.
#  Distributed under the Boost Software License, Version 1.0.
#  (See accompanying file LICENSE_1_0.txt or copy at
#  http://www.boost.org/LICENSE_1_0.txt)
#


#from DraughtDataStructures import *

class DraughtMakeOpeningBook:
    def __init__(self, main):
        self.main = main
        return

    def MakeBook(self):
        RatingFile = '/home/feike/workspace/collected/fmjd_rating_04_2009.txt'
        InputFile = '/home/feike/dammen/alle_partijen.doc'
#        OutDir = '/home/feike/BoomstraDam/pdn files/game records/'
#        GameNumber = 0;

        rating = open(RatingFile, 'r')
        rating_name_list = []
        rating_value_list = []
        while 1:
            try:
                rating_line = rating.readline()
                if rating == "": break #eof
            except:
                break
            if '\t' in rating_line:
                rating_name = rating_line[0:rating_line.index('\t')]
            else:
                break
            rating_value = int(rating_line[rating_line.index('\t')+1:-1])
            rating_name_list.append(rating_name)
            rating_value_list.append(rating_value)
        rating.close()
        # got the rating info per player
#        ResultRating = '/home/feike/workspace/game records.csv'
#        ofile = open(ResultRating, 'w')
        file = open(InputFile, 'r')
        PhaseFlag = "wait for header"
        while 1:
        #for i in range(0,100000):
          #print PhaseFlag
          if PhaseFlag == "wait for header":
            header = file.readline()
            if header == "": break # reached eof
            if len(header) > 1:
              if header[0].isupper() == True: # this starts the header
                if ',' in header[0:26]:
                  stop = header.index(',')
                else:
                  stop = header.index(' ')
                FirstName = header[:stop]
                if '-' in header:
                  second_start = header.index('-') + 2
                  if ',' in header[second_start:]:
                    second_stop = header[second_start:].index(',')
                  else:
                    second_stop = header[second_start:].index(' ')
                  SecName = header[second_start:second_start+second_stop]
                  #print FirstName,  SecName, header[60:70]
                  PhaseFlag = "collecting moves"
                  moves = ''
            else:
              print "header <= 1"
              break # eof ??
          elif PhaseFlag == "collecting moves":
            try: move_line = file.readline()
            except: break # eof ??
            if move_line == '\015\012' or move_line == '\40\015\012':
              if self.valid_players(FirstName, SecName, rating_name_list) == True:
                print moves[:240]
              PhaseFlag = "wait for header"
            else:
              #moves = moves + move_line
              for k in range(0, len(move_line)):
                s = move_line[k]
                if s != '\015' and s != '\012':
                  moves += s

    def valid_players(self, first_player, second_player, rating_list):
      if first_player.lower() in rating_list and second_player.lower() in rating_list:
        return True
      else:
        return False


#        FirstNameFound = 1
#        try:
#            FirstValue = rating_value_list[rating_name_list.index(FirstName.lower())]
#            #print FirstName, "found"
#
#        except:
#            #print FirstName, "not found"
#            FirstNameFound = 0
#
#        SecNameFound = 1
#        try:
#            SecValue = rating_value_list[rating_name_list.index(SecName.lower())]
#            #print SecName, "found"
#        except:
#            #print SecName, "not found"
#            SecNameFound = 0
#
#        if FirstNameFound and SecNameFound:
#            print FirstName, '-', SecName, moves
#                #ofile.write(repr(FirstValue).rjust(4)+'\t' + repr(SecValue).rjust(4)+ '\t' + line4[0:-2] + line5[0:2] +'\n')
#                self.main.read.s = line4[0:-2] + ' ' + line5[0:3] + ' \n\0'
#                game = GameRecord(self.main, self.main.panel, self.main.panel)
#                #self.main.read.Games.append(game)
#                self.main.read.lastchrpnt = 0;
#                cp = self.main.read.GetGame(0, game)
#                if cp == 0:
#                    #self.main.read.Games.remove(game)
#                    print "Error in PDN, last char tried = ", self.main.read.lastchrpnt
#                    print self.main.read.s[self.main.read.lastchrpnt-70:self.main.read.lastchrpnt]
###                    self.main.read.debug = '*'
###                    game = GameRecord(self.main, self.main.panel, self.main.panel)
###                    self.main.read.Games.append(game)
###                    cp = self.main.read.GetGame(0, game)
###                    self.main.read.debug=None
###                    break
#                else:
#                    # we have got a valid pdn
#                    # do we want to collect this game?
#                    if (FirstValue - SecValue > 150 and game.Result == '2-0') or \
#                        (FirstValue - SecValue < -150 and game.Result == '0-2'):
#                        game.set_begin_position()
#                        GameNumber += 1
#                        PDNfilename = OutDir + 'game-' + repr(GameNumber).zfill(4) + '.pdn'
#                        PDNfile = open(PDNfilename, 'w')
#                        PDN = self.main.save.game_to_string(game)
#                        PDNfile.write(PDN)
#                        PDNfile.close()
#                        for i in range(0, game.get_half_ply_length()):
#                            pos = game.get_next_board_layout()
#                            if pos <> None: #valid move
#                                ofile.write(repr(GameNumber).zfill(4)  + ' ' + repr(FirstValue).rjust(4) + ' ' + \
#                                                repr(SecValue).rjust(4) + ' ' + repr(int(i/2)).rjust(2) + ' ' + \
#                                                game.Result + ' ' +\
#                                                pos + '\n')
#                            else:
#                                print "invalid move??"
#        file.close()
#        ofile.close()
#        print "done"
#        return


if __name__=='__main__':
    main = DraughtMakeOpeningBook(None)
    main.MakeBook()

