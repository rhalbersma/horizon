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

def FLD_list(list):
  s = '(('
  fill = ''
  for item in list:
    s += fill + FLD(item)
    fill = '|'
  s += '))'
  return s

class voorpost:
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


                                            #g1               g2
  def WSE(self,  vp):                       #  f1           f2  c5
      b = self.nw[vp]                       #     a       a2  c4
      a = self.nw[b]                        #   n   b   b2  c3
      c = self.se[vp]                       #     h   vp  c2  k2
      d = self.se[c]                        #       u   c   k1
      e = self.se[d]                        #     v       d   m
      f1 = self.nw[a]                       #               e
      g1 = self.nw[f1]
      b2 = self.ne[vp]
      a2 = self.ne[b2]
      f2 = self.ne[a2]
      g2 = self.ne[f2]
      u = self.sw[vp]
      v = self.sw[u]
      n = self.sw[a]
      h = self.sw[b]
      c2 = self.ne[c]
      c3 = self.ne[c2]
      c4 = self.ne[c3]
      c5 = self.ne[c4]
      k1 = self.ne[d]
      k2 = self.ne[k1]
      m = self.ne[e]


      backing_attack_range = (self.north_reach(a,  [], -1, 0))
      attack_range = (self.north_reach_special(b,  [], -1, 0))
      backing_attack_unique_range = []
      for i in backing_attack_range:
        if not i in attack_range:
          backing_attack_unique_range.append(i)
      backing_defence_range = (self.south_reach(e,  [], -1, 0))
      defence_range = (self.south_reach_special(d,  [], -1, 0))
      backing_defence_unique_range = []
      for i in backing_defence_range:
        if not i in defence_range:
          backing_defence_unique_range.append(i)
      close_range = (self.south_reach(c, [], -1, 0))
      close_s_range = (self.south_reach(d, [], -1, 0))

    # collect the various ranges at distance, the list contains the fields at distance 0,1,2,3,.. e.g.[FLD17, FLD11|FLD12, FLD6|FLD7|FLD8, FLD1|FLD2|FLD3]
      backing_attack_list = []
      backing_defence_list = []
      attack_list = []
      defence_list = []
      close_list = []
      close_s_list = []
      for i in range(0, 10):
        if self.north_reach(a,  [], i, 0) <> []:
          backing_attack_list.append((self.north_reach(a,  [], i, 0)))
        if self.north_reach_special(b,  [], i, 0) <> []:
          attack_list.append((self.north_reach_special(b,  [], i, 0)))
        if self.south_reach(e,  [], i, 0) <> []:
          backing_defence_list.append((self.south_reach(e,  [], i, 0)))
        if self.south_reach_special(d,  [], i, 0) <> []:
          defence_list.append((self.south_reach_special(d,  [], i, 0)))
        if self.south_reach(c, [], i, 0) <> []:
          close_list.append((self.south_reach(c, [], i, 0)))
        if self.south_reach(d, [], i, 0) <> []:
          close_s_list.append((self.south_reach(d, [], i, 0)))

      if self.white == True:
        color = 'white'
        if self.mirror == True: dir = 'sw'
        else:                   dir = 'se'
      else:
        color = 'black'
        if self.mirror == True: dir = 'ne'
        else:                   dir = 'nw'

    # the code generator itself
      print 'extern int voorpost_' + color + '_' + dir + '_' + str(vp) + '(BitArray, BitArray, BitArray, int);'
      print 'int voorpost_' + color + '_' + dir + '_' + str(vp) + '(BitArray white_man, BitArray black_man, BitArray empty_field, int inc) {'
      print 'BitArray my_pieces_used, his_pieces_used;'
      print 'int number_of_defences, number_of_attacks = 0;'
      print 'int his_min_backing_steps, my_min_backing_steps;'
      print 'int my_min_defence_steps, his_min_attack_steps;'
      print 'bool backing_defence_piece, backing_attack_piece;'
      print
      if e <> 0:
        print 'if ((' + self.MyMan + ' & ' + FLD(vp) +') && ',
        print '(' + self.AllEmptyOrHis + '((' + FLD(b) + '|' + FLD(a) + '))) && ',
        print '(' + self.AllEmptyOrMine + '((' + FLD(d) + '|' + FLD(e) + '))) && ',
        print '(empty_field & ' + FLD(c) + ')) {'
        print '//so this is what anyway has to be there to have a voorpost'
        print
      else:
        print 'if ((' + self.MyMan + ' & ' + FLD(vp) +') && ',
        print '(' + self.AllEmptyOrHis + '((' + FLD(b) + '|' + FLD(a) + '))) && ',
        print '(' + self.EmptyOrMine + ' & ' + FLD(d) +  ') && ',
        print '(empty_field & ' + FLD(c) + ')) {'
      # so this is what anyway has to be there to have a voorpost
        print '//so this is what anyway has to be there to have a voorpost'
        print

      # if the field sw of b is occupied by me, no attack is possible, so the question is, can the piece on sw(b) be exchanged.
      print '//if the field sw of b is occupied by me or can be occupied by me, no attack is possible, so the question is, can the piece on sw(b) be exchanged'
      if self.sw[self.sw[b]] <> 0:
        p1 = self.sw[self.sw[b]]
        p2 = self.nw[self.sw[b]]
        p3 = self.se[self.sw[b]]
        print 'if (('  + self.MyMan + ' & ' + FLD(self.sw[b]) + ') || ',
        print '((' + self.MyMan + " & " + FLD(p1) + ') && ((' + self.AllEmptyOrMine + '(' + FLD(p2) + '|' + FLD(p3) + ')' + ') || ',
        print '(all_occupied'  + '(' + FLD(p2) + '|' + FLD(p3) + ')' +  ')))) {'
      else:
        print 'if ('  + self.MyMan + ' & ' + FLD(self.sw[b]) + ') {'
      print 'if ((' + self.AllHis + '((' + FLD(self.ne[b]) + '|' + FLD(self.ne[self.ne[b]])  + '|' + FLD(self.ne[self.ne[self.ne[b]]]) + ')) && (empty_field & ' + FLD(b) + ')) &&'
      print '    !(((' + self.HisMan + ' & ' + FLD(b2) + ') && (empty_field & ' + FLD(self.se[b2]) + ')) ||',
      print ' ((' + self.HisMan + ' & ' + FLD(self.ne[a]) + ') && (empty_field & ' + FLD(self.nw[self.ne[a]]) + '))))'
      # can be exchanged
      print  'number_of_attacks = -1;  // can be exchanged,  start with one attack less'
      print 'else goto check_afruil; // no attacks possible'
      print '}'
      print
      # same for the defence: if ne(d) is occupied by him, we may have a problem in the defence
      if self.ne[d] <> 0:
        print '//same for the defence: if ne(d) is occupied by him, we may have a problem in the defence'
        print 'if ('  + self.HisMan + ' & ' + FLD(self.ne[d]) + ') {'
        print 'if ((' + self.AllMine + '((' + FLD(self.sw[d]) + '|' + FLD(self.se[self.se[d]])  + '|' + FLD(self.sw[self.sw[self.sw[d]]]) + ')) && (empty_field & ' + FLD(d) + ')) ||'
        print '    (' + self.AllMine + '((' + FLD(d) + '|' + FLD(self.sw[self.sw[d]])  + '|' + FLD(self.sw[self.sw[self.sw[d]]]) + ')) && (empty_field & ' + FLD(self.sw[d]) + ')))'
        print 'number_of_defences = count((' +  self.f(defence_range) + ' & ' + self.MyMan + ')) - 1;'
        print 'else number_of_defences = 0;'
        print '} else number_of_defences = count((' +  self.f(defence_range) + ' & ' + self.MyMan + '));'
      else:
        print 'number_of_defences = count((' +  self.f(defence_range) + ' & ' + self.MyMan + '));'
      print

      # check the availability of the backing pieces
      print '//check the availability of the backing pieces'
      if backing_defence_range <> []: # if the backing defence field is on the board:
        print 'backing_defence_piece = bool (' + self.f(backing_defence_range) + ' & ' + self.MyMan + ');'
      else:
        print 'backing_defence_piece = true; // no backing defence piece (edge)'
      if backing_attack_range <> []: # if the backing defence field is on the board:
        print 'backing_attack_piece = (bool) (' + self.f(backing_attack_range) + ' & ' + self.HisMan + ');'
      else:
        print 'backing_attack_piece = true; // (edge)'

      print 'number_of_attacks += count((' +  self.f(attack_range) + ' & ' + self.HisMan + '));'
      print 'if (!(' +  self.f(backing_attack_unique_range) + ' & ' + self.HisMan + ')) number_of_attacks -= 1;' # in case the backing piece has to come from the attack poule
      if backing_defence_range <> []:
        print 'if (!(' +  self.f(backing_defence_unique_range) + ' & ' + self.MyMan + ')) number_of_defences -= 1;' # in case the backing piece has to come from the defence poule
      if self.sw[self.sw[b] == 0]:
        print 'if ((' + self.HisMan + '&' + FLD(self.sw[b]) + ') && (empty_field & ' + FLD(self.ne[b]) + ')) number_of_defences++;'
      print

      # so now we know whether we have a backing piece and how many pieces for the attack/defence
      print '//so now we know whether we have a backing piece and how many pieces for the attack/defence'
      print 'if (number_of_attacks <= 0) return 180;'

      # attack(s) are possible
      if backing_defence_range <> []: # if the backing defence field is on the board:
        print 'if (!(backing_defence_piece) && (backing_attack_piece) && (number_of_attacks > 0)) { // looks bad, but maybe an escape?'
        self.maybe_escape(c, d, a2, b2, vp, u, v, f2, attack_list, backing_attack_list, defence_list, close_list, close_s_list) # if there is an escape, then a return is generated
        print ' return -700;'
        print '}'
        print

      if backing_attack_range <> []: # if the backing defence field is on the board:
        print 'if (!(backing_attack_piece)) goto check_afruil;'

      print '// both backings are available'
      print 'if (number_of_attacks > number_of_defences) {  // it looks bad, but maybe an escape?'

      self.maybe_escape(c, d, a2, b2, vp, u, v, f2, attack_list, backing_attack_list, defence_list, close_list, close_s_list) # if there is an escape, then a return is generated

      print 'return -700;'                                             # voorpost is for sure lost
      print '}'
      print
      # calc the minimal amount of steps for having the backing piece in place as well as the attack resp.defence piece
      print '//calc the minimal amount of steps for having the backing piece in place as well as the attack resp.defence piece'
      print 'his_pieces_used = 0;'
      if len(backing_attack_list) <> 0:
        print 'his_min_backing_steps = calc_dist_' + str(len(backing_attack_list)) + '(',
        for k in backing_attack_list:
          print self.f(k) + ',',
        print ' &his_pieces_used, ' + self.HisMan + ');'
      else:
        print 'his_min_backing_steps = 0; //there is a "his" man at the backing attack field'
      print 'his_min_attack_steps = calc_dist_' + str(len(attack_list)) + '(',
      for k in attack_list:
        print self.f(k) + ',',
      print ' &his_pieces_used, ' + self.HisMan + ');'
      print 'my_pieces_used = 0;'
      if len(backing_defence_list) <> 0:
        print 'my_min_backing_steps = calc_dist_' + str(len(backing_defence_list)) + '(',
        for k in backing_defence_list:
          print self.f(k) + ',',
        print ' &my_pieces_used, ' + self.MyMan + ');'
      else:
        print 'my_min_backing_steps = 0;'
      print 'my_min_defence_steps = calc_dist_' + str(len(defence_list)) + '(',
      for k in defence_list:
        print self.f(k) + ',',
      print ' &my_pieces_used, ' + self.MyMan + ');'
      print
      print '// we have calculated: his_min_backing_steps, his_min_attack_steps, my_min_defence_steps and my_min_backing steps'
      print
      print 'if ((his_min_backing_steps + his_min_attack_steps) - (my_min_backing_steps + my_min_defence_steps + inc) < -2) {'
      # if we are too late, lost, maybe escape

      self.maybe_escape(c, d, a2, b2, vp, u, v, f2, attack_list, backing_attack_list, defence_list, close_list, close_s_list) # if there is an escape, then a return is generated

      print 'return -700;'
      print '}'
      print
      print '// and a second attack??'
      print

      print 'if (number_of_attacks > 1) {'
      print 'his_min_attack_steps = calc_dist_' + str(len(attack_list)) + '(',
      for k in attack_list:
        print self.f(k) + ',',
      print ' &his_pieces_used, ' + self.HisMan + ');'
      print 'my_min_defence_steps = calc_dist_' + str(len(defence_list)) + '(',
      for k in defence_list:
        print self.f(k) + ',',
      print ' &my_pieces_used, ' + self.MyMan + ');'
      print 'if ((his_min_attack_steps) - (my_min_defence_steps + inc) < -2) { // too late for the second attempt'

      self.maybe_escape(c, d, a2, b2, vp, u, v, f2, attack_list, backing_attack_list, defence_list, close_list, close_s_list) # if there is an escape, then a return is generated

      print 'return -400;'
      print '}'

      print '}'

      print 'check_afruil:'

      if g1 <> 0:
        print 'if (' + self.AllHis + '((' + FLD(g1) + '|' + FLD(f1) + '|' + FLD(a) + '))) return 0; // voorpost wordt afgeruild'
      else:
        if f1 <> 0:
          print 'if (' + self.AllHis + '((' + FLD(f1) + '|' + FLD(a) + '))) return 0; // voorpost wordt afgeruild'
      if g2 <> 0:
        print 'if (' + self.AllHis + '((' + FLD(g2) + '|' + FLD(f2) + '|' + FLD(a2) + '))) return 0; // voorpost wordt afgeruild'
      else:
        if f2 <> 0:
          print 'if (' + self.AllHis + '((' + FLD(f2) + '|' + FLD(a2) + '))) return 0; // voorpost wordt afgeruild'
      print '// so we have hope to have a nice voorpost'
      # but for certain positions we have another check
      if c5 == 0 and c4 <> 0:
        print 'if (((' + self.AllHis + FLD_list([c2, c3, c4]) + ' && (all_empty' + FLD_list([k1, d]) +')) || ',
        print     '(' + self.AllHis + FLD_list([k2, c3, c4]) + ' && (all_empty' + FLD_list([c2, k1, d]) +'))) && ',
        print     '(' + self.MyMan + ' & ' + FLD(m) + ')) return -700; // een om twee mogelijk'
      print 'return 180;  // no problems, so a small advantage'
      print '}'
      print 'return 0;'
      print '}'
      return

#g1               g2
#  f1           f2
#     a       a2
#   n   b   b2
#     h   vp
#       u   c   k
#     v       d   m
#               e

# a: backing attack piece
# vp voorpost
# e backing defence piece


  def maybe_escape(self, c, d, a2, b2, vp, u, v, f2, attack_list, backing_attack_list, defence_list, close_list, close_s_list):
      # kan ik hem dichtzetten ??
      # als zwarte driehoek langs de kant heeft het geen zin, dichtzetten veroorzaakt dan zijn eigen opsluiting
      if self.se[a2] == 0:
        print 'if (' + self.AllHis + '((' +FLD(a2) + '|' + FLD(b2) + '|' + FLD(self.se[b2]) + '))) return -700; // vleugelopsluiting'
      # kan aanvallende schijf veilig bewegen?
      print 'if ((' + self.MyMan + ' & ' + FLD(self.se[b2]) + ') && (' + self.AllHis + '((' + FLD(b2) + '|' + FLD(self.nw[b2]) + '))) && (',
      print 'all_empty((' + FLD(self.nw[vp]) + '|' + FLD(self.nw[u]) + ')))) return 0;'
      # deze code genereert het minimale aantal stappen voordat de aanval een feit is:
      print 'his_pieces_used = 0;   // calc the minimal amount of steps for having the backing piece in place as well as the attack resp.defence piece'
      if len(backing_attack_list) <> 0:
        print 'his_min_backing_steps = calc_dist_' + str(len(backing_attack_list)) + '(',
        for k in backing_attack_list:
          print self.f(k) + ',',
        print ' &his_pieces_used, ' + self.HisMan + ');'
      else:
        print 'his_min_backing_steps = 0;'                             # there is a "his" man at the backing attack field
      print 'his_min_attack_steps = calc_dist_' + str(len(attack_list)) + '(',
      for k in attack_list:
        print self.f(k) + ',',
      print ' &his_pieces_used, ' + self.HisMan + ');'

      print 'if ((' + self.AllEmptyOrMine + '((' + FLD(self.ne[c]) + '|' + FLD(self.sw[c]) + '))) ',
      print '|| (all_occupied((' + FLD(self.ne[c]) + '|' + FLD(self.sw[c]) + ')))) {'
      #hoeveel stappen heb ik nodig om hem dicht te zetten??
      print 'my_pieces_used = 0;'
      print 'my_min_defence_steps = calc_dist_' + str(len(close_s_list)) + '(',
      for k in close_s_list:
        print self.f(k) + ',',
      print ' &my_pieces_used, ' + self.MyMan + ') + 1;'
      # ben ik op tijd met dichtzetten?
      print 'if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;'
      print '}'
      # tweede (andere) poging om hem dicht te zetten:
      print 'if (' + self.EmptyOrMine + ' & ' + FLD(self.ne[c]) + ') {'
      print 'my_pieces_used = 0;'
      print 'my_min_defence_steps = calc_dist_' + str(len(close_list)) + '(',
      for k in close_list:
        print self.f(k) + ',',
      print ' &my_pieces_used, ' + self.MyMan + ');'
      print 'if ((my_min_defence_steps + inc) <= (his_min_backing_steps + his_min_attack_steps)) return 0;'
      print '}'

      # kan ik vluchten naar b2 ??
      print 'if ((' + self.AllMine + '((' + FLD(u) + '|' + FLD(v) + '))) && (all_empty(( ' + FLD(b2) + '|' + FLD(a2) + ')))) '  ,
      # nw[b2] is vanzelf empty, daar komt de aanvaller vandaan!
      if f2 == 0:
        print 'return 50; // vlucht naar rand' # ja ik kan weg
      else:
        print 'return -200; // vlucht' # ja ik kan weg, maar het blijft eng

      # kan ik vluchten door een poortje b2 en dan naar a2?
      print 'if ((occupied & ' + FLD(self.se[b2]) + ') && (all_empty(( ' + FLD(b2) + '|' + FLD(a2) + ')))) '  ,
      # nw[b2], daar staat de aanvaller even!
      if f2 == 0:
        print 'return 50; // vlucht naar rand' # ja ik kan weg
      else:
        print 'return -200; // vlucht' # ja ik kan weg, maar het blijft eng

      # moet de aanvaller een paddestoel maken om de voorpost aan te vallen?
      print 'if ((' + self.AllMine + '((' + FLD(u) + '|' + FLD(v) + '))) && (' + self.HisMan + ' & ' + FLD(a2) + ') && (occupied & ' + FLD(self.se[b2]) + ')',
      print '&& (empty_field & ' + FLD(b2) + ')) return 0; //paddestoel'
      # kan ik de vp wegruilen ??
      print 'if ((' + self.AllMine + '((' + FLD(u) + '|' + FLD(v) + '))) && (' + self.HisMan + ' & ' + FLD(a2) + ') && (' + self.EmptyOrMine + ' &' + FLD(self.se[b2]) + ')',
      print '&& (empty_field & ' + FLD(b2) + ')) return 0; //kan wegruilen'
      return


  def south_reach_special(self,  start,  s,  wanted,  distance):
    if start <> 0:
      if wanted < 0 or wanted == distance:
        s.append(start)
      s  = ( self.south_reach(self.sw[start],  s,  wanted,  distance+1))
    return s

  def south_reach(self,  start,  s,  wanted,  distance):
    if start <> 0:
      if wanted < 0 or wanted == distance:
        s.append(start)
      s = (self.south_reach(self.se[start],  s,  wanted,  distance+1))
      s = (self.south_reach(self.sw[start],  s,  wanted,  distance+1))
    return s

  def north_reach_special(self,  start,  s,  wanted,  distance):
    if start <> 0:
      if wanted < 0 or wanted == distance:
        s.append(start)
      s  = ( self.north_reach(self.ne[start],  s,  wanted,  distance+1))
    return s

  def north_reach(self,  start,  s,  wanted,  distance):
    if start <> 0:
      if wanted < 0 or wanted == distance:
        s.append(start)
      s = (self.north_reach(self.nw[start],  s,  wanted,  distance+1))
      s = (self.north_reach(self.ne[start],  s,  wanted,  distance+1))
    return s

  def f(self,  list):
    if len(list) > 0:
      s = ''
      sep = '('
      for i in sorted(set(list)):
          s = s + sep + FLD(i)
          sep = '|'
      s = s +')'
    else:
      s = 'lijkt me niet goed'
    return s

  def make_unique_and_sort(self, list):
    if list == []:
      return []
    else:
      list.sort()
      return set(list)




f = voorpost(False, True)
print '#include "BoomstraDam.v2.5.h"'
print "extern int calc_dist_9(BitArray, BitArray, BitArray, BitArray, BitArray, BitArray, BitArray, BitArray, BitArray, BitArray *, BitArray);"
print "extern int calc_dist_8(BitArray, BitArray, BitArray, BitArray, BitArray, BitArray, BitArray, BitArray, BitArray *, BitArray);"
print "extern int calc_dist_7(BitArray, BitArray, BitArray, BitArray, BitArray, BitArray, BitArray, BitArray *, BitArray);"
print "extern int calc_dist_6(BitArray, BitArray, BitArray, BitArray, BitArray, BitArray, BitArray *, BitArray);"
print "extern int calc_dist_5(BitArray, BitArray, BitArray, BitArray, BitArray, BitArray *, BitArray);"
print "extern int calc_dist_4(BitArray, BitArray, BitArray, BitArray, BitArray *, BitArray);"
print "extern int calc_dist_3(BitArray, BitArray, BitArray, BitArray *, BitArray);"
print "extern int calc_dist_2(BitArray, BitArray, BitArray *, BitArray);"
print "extern int calc_dist_1(BitArray, BitArray *, BitArray);"


print '// witte voorposten se'
for i in [17, 18, 19, 22, 23, 24]:
  f.WSE(i)
f = voorpost(True, True)
print '// witte voorposten sw'
for i in [17, 18, 19, 22, 23, 24]:
  f.WSE(i)
f = voorpost(True, False)
print '// zwarte voorposten ne'
for i in [27, 28, 29, 32, 33, 34]:
  f.WSE(i)
f = voorpost(False, False)
print '// zwarte voorposten nw'
for i in [27, 28, 29, 32, 33, 34]:
  f.WSE(i)

