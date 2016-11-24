#
#  This file is Copyright (C) 2010 Feike Boomstra.
#  Distributed under the Boost Software License, Version 1.0.
#  (See accompanying file LICENSE_1_0.txt or copy at
#  http://www.boost.org/LICENSE_1_0.txt)
#


#  [0,1,2,3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50]
onw=[0,0,0,0, 0, 0, 0, 1, 2, 3, 4, 6, 7, 8, 9,10, 0,11,12,13,14,16,17,18,19,20, 0,21,22,23,24,26,27,28,29,30, 0,31,32,33,34,36,37,38,39,40, 0,41,42,43,44]
one=[0,0,0,0, 0, 0, 1, 2, 3, 4, 5, 7, 8, 9,10, 0,11,12,13,14,15,17,18,19,20, 0,21,22,23,24,25,27,28,29,30, 0,31,32,33,34,35,37,38,39,40, 0,41,42,43,44,45]
ose=[0,7,8,9,10, 0,11,12,13,14,15,17,18,19,20, 0,21,22,23,24,25,27,28,29,30, 0,31,32,33,34,35,37,38,39,40, 0,41,42,43,44,45,47,48,49,50, 0, 0, 0, 0, 0, 0]
osw=[0,6,7,8, 9,10, 0,11,12,13,14,16,17,18,19,20, 0,21,22,23,24,26,27,28,29,30, 0,31,32,33,34,36,37,38,39,40, 0,41,42,43,44,46,47,48,49,50, 0, 0, 0, 0, 0]

def FLD(a):
    return'FLD' + str(a)

class Orient:
  def __init__(self, mirror, white):
    if mirror == False and white == True:
      self.nw = onw
      self.ne = one
      self.se = ose
      self.sw = osw
      self.MyMan          = 'white_man'
      self.EmptyOrMine    = 'empty_or_white'
      self.AllEmptyOrMine = 'all_empty_or_white'
      self.AllMine        = 'all_white'
      self.HisMan         = 'black_man'
      self.AllEmptyOrHis  = 'all_empty_or_black'
      self.AllHis         = 'all_black'
    elif mirror == True and white == True:
      self.nw = one
      self.ne = onw
      self.se = osw
      self.sw = ose
      self.MyMan          = 'white_man'
      self.EmptyOrMine    = 'empty_or_white'
      self.AllEmptyOrMine = 'all_empty_or_white'
      self.AllMine        = 'all_white'
      self.HisMan         = 'black_man'
      self.AllEmptyOrHis  = 'all_empty_or_black'
      self.AllHis         = 'all_black'
    elif mirror == False and white == False:
      self.nw = ose
      self.ne = osw
      self.se = onw
      self.sw = one
      self.MyMan          = 'black_man'
      self.EmptyOrMine    = 'empty_or_black'
      self.AllEmptyOrMine = 'all_empty_or_black'
      self.AllMine        = 'all_black'
      self.HisMan         = 'white_man'
      self.AllEmptyOrHis  = 'all_empty_or_white'
      self.AllHis         = 'all_white'
    else:
      self.nw = osw
      self.ne = ose
      self.se = one
      self.sw = onw
      self.MyMan          = 'black_man'
      self.EmptyOrMine    = 'empty_or_black'
      self.AllEmptyOrMine = 'all_empty_or_black'
      self.AllMine        = 'all_black'
      self.HisMan         = 'white_man'
      self.AllEmptyOrHis  = 'all_empty_or_white'
      self.AllHis         = 'all_white'
    self.white  = white
    self.mirror = mirror
    return

  def all_white(self, N1, N2, N3=0, N4=0, N5=0, N6=0):
    s = self.AllMine + '('
    if N1 <> 0:
      s = s + FLD(N1) + '|' + FLD(N2)
    else:
      s = s + FLD(N2)
    if N3 <> 0:
      s = s + '|' + FLD(N3)
      if N4 <> 0:
        s = s + '|' + FLD(N4)
        if N5 <> 0:
          s = s + '|' + FLD(N5)
          if N6 <> 0:
            s = s + '|' + FLD(N6)
    s = s + ')'
    return s

  def all_empty(self, N1, N2, N3=0, N4=0, N5=0, N6=0):
    s = 'all_empty('
    if N1 <> 0:
      s = s + FLD(N1) + '|' + FLD(N2)
    else:
      s = s + FLD(N2)
    if N3 <> 0:
      s = s + '|' + FLD(N3)
      if N4 <> 0:
        s = s + '|' + FLD(N4)
        if N5 <> 0:
          s = s + '|' + FLD(N5)
          if N6 <> 0:
            s = s + '|' + FLD(N6)
    s = s + ')'
    return s



  def F1(self, q, color, search_extension = False): #
    Q2 = q         #
    Q1 = self.sw[Q2]   #P14     P6      P2
    Q3 = self.ne[Q2]   #  P13 P12 P5  P1
    E2 = self.ne[Q3]    #   E5      E4
    T2 = self.ne[E2]    # T1  O2  T2  P3
    E4 = self.ne[T2]    #   E1  E2      P4
    O1 = self.nw[Q2]    # O1  Q3  T3  P7
    E1 = self.nw[Q3]    #   Q2  B1  E3
    T1 = self.nw[E1]    # Q1      P10  P8
    O2 = self.nw[E2]    #       P11      P9
    E5 = self.nw[O2]
    T3 = self.se[E2]
    E3 = self.se[T3]
    P1 = self.ne[E4]
    P2 = self.ne[P1]
    P3 = self.se[E4]
    P4 = self.se[P3]
    P5 = self.nw[E4]
    P6 = self.nw[P5]
    P7 = self.ne[E3]
    P8 = self.se[E3]
    P9 = self.se[P8]
    P10= self.sw[E3]
    P11= self.sw[P10]
    P12= self.ne[E5]
    P13= self.nw[E5]
    P14= self.nw[P13]
    B1 = self.se[Q3]
    B2 = self.se[B1]

    if T1<> 0 and E2 <> 0 and (E3 <> 0 or E4 <>0 or E5 <> 0):
      # basic structure
      print "if (" + self.all_white(Q1, Q2, Q3),
      print " && " + self.all_empty(E1, E2) + " && (" + self.HisMan + ' & ' + FLD(T1) + "))"
      # can I move piece Q1 safely?
      print "\tif ((all_occupied(" + FLD(O1) + '|' + FLD(O2) + '))',
      print " || (" + self.AllEmptyOrMine + "(" + FLD(O1) + '|' + FLD(O2) + ")))"
      if B2 <> 0:
        print '\t\tif ((empty_field & ' + FLD(B1) + ') || (' + self.AllMine + '((' + FLD(B1) + "|" + FLD(B2) + ')))) {'
      else:
        print '\t\tif (' + self.EmptyOrMine + ' & ' + FLD(B1) + ') {'
      if search_extension == True:
        # yes, is there a second piece to capture?
        if E4 <> 0: print "\t\tif ((" + self.HisMan + " & FLD" + str(T2) + ") && (empty_field & FLD" + str(E4) + ")) return 1;"
        if E3 <> 0: print "\t\tif ((" + self.HisMan + " & FLD" + str(T3) + ") && (empty_field & FLD" + str(E3) + ")) return 1;"
        if E5 <> 0: print "\t\tif ((" + self.HisMan + " & FLD" + str(O2) + ") && (empty_field & FLD" + str(E5) + ")) return 1;"
      else:
        print '\t\t\t' + color + '_row += 200;'
      print "\t\t}"

      #alternative structure
      print "if (" + self.all_white(Q1, Q2, O1),
      print " && " + self.all_empty(E1, E2, Q3) + " && (" + self.HisMan + ' & ' + FLD(T1) + "))"
      # can I move piece Q1 safely?
      print "\tif (" + self.EmptyOrMine + '&' + FLD(O2) + ')'
      if B2 <> 0:
        print '\t\tif ((empty_field & ' + FLD(B1) + ') || (' + self.AllMine + '((' + FLD(B1) + "|" + FLD(B2) + ')))) {'
      else:
        print '\t\tif (' + self.EmptyOrMine + ' & ' + FLD(B1) + ') {'
      if search_extension == True:
        # yes, is there a second piece to capture?
        if E4 <> 0: print "\t\tif ((" + self.HisMan + " & FLD" + str(T2) + ") && (empty_field & FLD" + str(E4) + ")) return 1;"
        if E3 <> 0: print "\t\tif ((" + self.HisMan + " & FLD" + str(T3) + ") && (empty_field & FLD" + str(E3) + ")) return 1;"
        if E5 <> 0: print "\t\tif ((" + self.HisMan + " & FLD" + str(O2) + ") && (empty_field & FLD" + str(E5) + ")) return 1;"
      else:
        print '\t\t\t' + color + '_row += 200;'
      print "\t\t}"

print "\n// white pootje\n\n"
f = Orient(True,  True)
print 'if (node->nr_of_black_king == 0) {'
for i in range(16, 35):
  f.F1(i, "white")
f = Orient(False,  True)
for i in range(16, 35):
  f.F1(i, "white")
print '}'
print "\n// zwart pootje\n\n"
print 'if (node->nr_of_white_king == 0) {'
f = Orient(True,  False)
for i in range(16, 35):
  f.F1(i, "black")
f = Orient(False,  False)
for i in range(16, 35):
  f.F1(i, "black")
print '}'

