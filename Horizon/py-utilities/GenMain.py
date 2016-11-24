#
#  This file is Copyright (C) 2010 Feike Boomstra.
#  Distributed under the Boost Software License, Version 1.0.
#  (See accompanying file LICENSE_1_0.txt or copy at
#  http://www.boost.org/LICENSE_1_0.txt)
#


#  [0,1,2,3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50]
nw=[0,0,0,0, 0, 0, 0, 1, 2, 3, 4, 6, 7, 8, 9,10, 0,11,12,13,14,16,17,18,19,20, 0,21,22,23,24,26,27,28,29,30, 0,31,32,33,34,36,37,38,39,40, 0,41,42,43,44]
ne=[0,0,0,0, 0, 0, 1, 2, 3, 4, 5, 7, 8, 9,10, 0,11,12,13,14,15,17,18,19,20, 0,21,22,23,24,25,27,28,29,30, 0,31,32,33,34,35,37,38,39,40, 0,41,42,43,44,45]
se=[0,7,8,9,10, 0,11,12,13,14,15,17,18,19,20, 0,21,22,23,24,25,27,28,29,30, 0,31,32,33,34,35,37,38,39,40, 0,41,42,43,44,45,47,48,49,50, 0, 0, 0, 0, 0, 0]
sw=[0,6,7,8, 9,10, 0,11,12,13,14,16,17,18,19,20, 0,21,22,23,24,26,27,28,29,30, 0,31,32,33,34,36,37,38,39,40, 0,41,42,43,44,46,47,48,49,50, 0, 0, 0, 0, 0]

class GenMain:

  def F1(self, q):
    Q1 = se[q]
    Q2 = q
    Q3 = nw[Q2]
    E  = nw[Q3]
    A  = nw[E]
    B1 = ne[E]
    B2 = ne[B1]
    C1 = sw[E]
    C2 = sw[C1]
    #print q,  A,  B1,  C1
    if A <> 0 and B1 <> 0 and C1 <> 0 and not (B2 == 0 and C2 == 0):
      if Q1 <> 0:
        print "if (all_white((FLD" + str(Q1) + "|FLD" + str(Q2) + "|FLD" + str(Q3) + \
        ")) && (empty_field & FLD" + str(E) + ") && (black_man & FLD" + str(A) + ")) {"
      else:
        print "if (all_white((FLD" + str(Q2) + "|FLD" + str(Q3) + \
        ")) && (empty_field & FLD" + str(E) + ") && (black_man & FLD" + str(A) + ")) {"        
      if B2 == 0:
        print "\tif ((any_piece & FLD" + str(B1) + ") && (black_man & FLD" + str(C1) + ") && (empty_field & FLD" + str(C2) + ")) return 1;"
      elif C2 == 0:
        print "\tif ((any_piece & FLD" + str(C1) + ") && (black_man & FLD" + str(B1) + ") && (empty_field & FLD" + str(B2) + ")) return 1;"
      else:
        print "\tif (((any_piece & FLD" + str(B1) + ") && (black_man & FLD" + str(C1) + ") && (empty_field & FLD" + str(C2) + ")) ||"
        print "\t   ((any_piece & FLD" + str(C1) + ") && (black_man & FLD" + str(B1) + ") && (empty_field & FLD" + str(B2) + "))) return 1;"
      print "}"

  def F2(self, q):
    Q1 = sw[q]
    Q2 = q
    Q3 = ne[Q2]
    E  = ne[Q3]
    A  = ne[E]
    B1 = nw[E]
    B2 = nw[B1]
    C1 = se[E]
    C2 = se[C1]
    #print q,  A,  B1,  C1
    if A <> 0 and B1 <> 0 and C1 <> 0 and not (B2 == 0 and C2 == 0):
      if Q1 <> 0:
        print "if (all_white((FLD" + str(Q1) + "|FLD" + str(Q2) + "|FLD" + str(Q3) + \
        ")) && (empty_field & FLD" + str(E) + ") && (black_man & FLD" + str(A) + ")) {"
      else:
        print "if (all_white((FLD" + str(Q2) + "|FLD" + str(Q3) + \
        ")) && (empty_field & FLD" + str(E) + ") && (black_man & FLD" + str(A) + ")) {"        
      if B2 == 0:
        print "\tif ((any_piece & FLD" + str(B1) + ") && (black_man & FLD" + str(C1) + ") && (empty_field & FLD" + str(C2) + ")) return 1;"
      elif C2 == 0:
        print "\tif ((any_piece & FLD" + str(C1) + ") && (black_man & FLD" + str(B1) + ") && (empty_field & FLD" + str(B2) + ")) return 1;"
      else:
        print "\tif (((any_piece & FLD" + str(B1) + ") && (black_man & FLD" + str(C1) + ") && (empty_field & FLD" + str(C2) + ")) ||"
        print "\t   ((any_piece & FLD" + str(C1) + ") && (black_man & FLD" + str(B1) + ") && (empty_field & FLD" + str(B2) + "))) return 1;"
      print "}"

# now black


  def BL1(self, q):
    Q1 = ne[q]
    Q2 = q
    Q3 = sw[Q2]
    E  = sw[Q3]
    A  = sw[E]
    B1 = se[E]
    B2 = se[B1]
    C1 = nw[E]
    C2 = nw[C1]
    #print q,  A,  B1,  C1
    if A <> 0 and B1 <> 0 and C1 <> 0 and not (B2 == 0 and C2 == 0):
      if Q1 <> 0:
        print "if (all_black((FLD" + str(Q1) + "|FLD" + str(Q2) + "|FLD" + str(Q3) + \
        ")) && (empty_field & FLD" + str(E) + ") && (white_man & FLD" + str(A) + ")) {"
      else:
        print "if (all_black((FLD" + str(Q2) + "|FLD" + str(Q3) + \
        ")) && (empty_field & FLD" + str(E) + ") && (white_man & FLD" + str(A) + ")) {"        
      if B2 == 0:
        print "\tif ((any_piece & FLD" + str(B1) + ") && (white_man & FLD" + str(C1) + ") && (empty_field & FLD" + str(C2) + ")) return 1;"
      elif C2 == 0:
        print "\tif ((any_piece & FLD" + str(C1) + ") && (white_man & FLD" + str(B1) + ") && (empty_field & FLD" + str(B2) + ")) return 1;"
      else:
        print "\tif (((any_piece & FLD" + str(B1) + ") && (white_man & FLD" + str(C1) + ") && (empty_field & FLD" + str(C2) + ")) ||"
        print "\t   ((any_piece & FLD" + str(C1) + ") && (white_man & FLD" + str(B1) + ") && (empty_field & FLD" + str(B2) + "))) return 1;"
      print "}"


  def BL2(self, q):
    Q1 = nw[q]
    Q2 = q
    Q3 = se[Q2]
    E  = se[Q3]
    A  = se[E]
    B1 = sw[E]
    B2 = sw[B1]
    C1 = ne[E]
    C2 = ne[C1]
    #print q,  A,  B1,  C1
    if A <> 0 and B1 <> 0 and C1 <> 0 and not (B2 == 0 and C2 == 0):
      if Q1 <> 0:
        print "if (all_black((FLD" + str(Q1) + "|FLD" + str(Q2) + "|FLD" + str(Q3) + \
        ")) && (empty_field & FLD" + str(E) + ") && (white_man & FLD" + str(A) + ")) {"
      else:
        print "if (all_black((FLD" + str(Q2) + "|FLD" + str(Q3) + \
        ")) && (empty_field & FLD" + str(E) + ") && (white_man & FLD" + str(A) + ")) {"        
      if B2 == 0:
        print "\tif ((any_piece & FLD" + str(B1) + ") && (white_man & FLD" + str(C1) + ") && (empty_field & FLD" + str(C2) + ")) return 1;"
      elif C2 == 0:
        print "\tif ((any_piece & FLD" + str(C1) + ") && (white_man & FLD" + str(B1) + ") && (empty_field & FLD" + str(B2) + ")) return 1;"
      else:
        print "\tif (((any_piece & FLD" + str(B1) + ") && (white_man & FLD" + str(C1) + ") && (empty_field & FLD" + str(C2) + ")) ||"
        print "\t   ((any_piece & FLD" + str(C1) + ") && (white_man & FLD" + str(B1) + ") && (empty_field & FLD" + str(B2) + "))) return 1;"
      print "}"



f = GenMain()
p = 'w'
if p=='w':
  print '//een om twee paddestoel wit'
  for i in range(1, 46):
    f.F1(i)
    f.F2(i)
else:
  print '//een om twee paddestoel zwart'
  for i in range(6, 51):
    f.BL1(i)
    f.BL2(i)
  
