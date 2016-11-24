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
#        ResultRating = '/home/feike/workspace/game records.csv'
#        ofile = open(ResultRating, 'w')
        file = open(InputFile, 'r')
        #while 1:
        for i in range(0,400000):
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

            try: FirstName = line1[line1.index(')')+2: line1.index(',')]
            except:
              pass
              #print "line1 = ", line1
            try:
              SecTemp = line1[line1.index('-') +2:-2]
              SecName = SecTemp[:SecTemp.index(',')]
            except: err=1
            print FirstName
            print SecName

            FirstNameFound = 1
            try:
                FirstValue = rating_value_list[rating_name_list.index(FirstName.lower())]
                #print FirstName, "found"

            except:
                #print FirstName, "not found"
                FirstNameFound = 0

            SecNameFound = 1
            try:
                SecValue = rating_value_list[rating_name_list.index(SecName.lower())]
                #print SecName, "found"
            except:
                #print SecName, "not found"
                SecNameFound = 0

            if FirstNameFound and SecNameFound:
                print FirstName, '-', SecName
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

