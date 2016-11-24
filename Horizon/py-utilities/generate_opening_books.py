#
#  This file is Copyright (C) 2010 Feike Boomstra.
#  Distributed under the Boost Software License, Version 1.0.
#  (See accompanying file LICENSE_1_0.txt or copy at
#  http://www.boost.org/LICENSE_1_0.txt)
#


class MoveTree_Item:
  def __init__(self,  start,  stop,  mycolor):
    self.Start = start
    self.Stop = stop
    self.Count = 1
    self.MyColor = mycolor
    self.NodeNumber = 0
    self.Next_Moves = []
    return
  def __eq__(self, a):
    return (a.Start == self.Start and a.Stop == self.Stop)
    
  def __str__(self):
    s = '['+str(self.Start).zfill(2)+','+str(self.Stop).zfill(2)+','+str(self.Count).zfill(3)+','+str(self.MyColor)+']'
    return s

class GenerateOpeningBook:
  def __init__(self,  color):
    self.Tree = MoveTree_Item(0,  0,  color == "white")
    self.NodeCounter = 0
    self.cur_num = 0
    self.max_nr_of_moves = 0;
    return
  
  def open_input(self,  color):
    if color == "white":
      file_name = '/home/feike/BoomstraDam/Opening Books/wit_ruwe_data.txt'
    else:
      file_name = '/home/feike/BoomstraDam/Opening Books/zwart_ruwe_data.txt'
    f = open(file_name,  'r')
    return f
    
  def proc_line(self,  line,  color):
    if line[0:2] == '1.':
      s = self.remove_comment(line)
      if s <>'':
        self.add_move_list(self.scan(s), 0, self.Tree.Next_Moves,  not self.Tree.MyColor)
    return
    
  def remove_comment(self,  line):
    s = ''
    r = 0
    copy = True
    while not (copy == True and line[r:r+3] == '16.'):
      if copy == True:
        if line[r] == '[':
          copy = False
        else:
          s = s + line[r]
      else: #copy == False
        if line[r] == ']':
          copy = True
      r += 1
      if r >= len(line):
        return ''
    return s + ' '
    
  def scan(self,  line):  
    start = 0
    move_list = []
    for i in range(0, 15):
      wstart, wstop, bstart, bstop,  start = self.scan_move(line, start)
      move_list.append([wstart, wstop])
      move_list.append([bstart, bstop])      
    return move_list
    
  def scan_move(self,  line,  start):
    cur_pnt = start
    while line[cur_pnt] <> '.':
      cur_pnt += 1
    cur_pnt += 1 
    #now on first white digit 
    if line[cur_pnt + 2] == '-' or line[cur_pnt + 2] == 'x':
      wstart = int(line[cur_pnt:cur_pnt+2 ])
      cur_pnt += 3
    else:
      wstart = int(line[cur_pnt:cur_pnt+1 ])
      cur_pnt += 2
    # now on first black digit
    if line[cur_pnt + 2] == ' ' :
      wstop = int(line[cur_pnt:cur_pnt+2 ])
      cur_pnt += 3
    else:
      wstop = int(line[cur_pnt:cur_pnt+1 ])
      cur_pnt += 2
    # black move
    #now on first white digit 
    if line[cur_pnt + 2] == '-' or line[cur_pnt + 2] == 'x':
      bstart = int(line[cur_pnt:cur_pnt+2 ])
      cur_pnt += 3
    else:
      bstart = int(line[cur_pnt:cur_pnt+1 ])
      cur_pnt += 2
    # now on first black digit
    if line[cur_pnt + 2] == ' ' :
      bstop = int(line[cur_pnt:cur_pnt+2 ])
      cur_pnt += 3
    else:
      bstop = int(line[cur_pnt:cur_pnt+1 ])
      cur_pnt += 2
    return wstart, wstop, bstart, bstop,  cur_pnt

  def add_move_list(self,  list,  level,  entry,  mycolor):  
    new_item = MoveTree_Item(list[level][0],  list[level][1],  mycolor)
    if not new_item in entry:
      entry.append(new_item)
    else:
      for element in entry:
        if element == new_item:
          element.Count += 1
    if level < 29:
      for element in entry:
        if element == new_item:
          self.add_move_list(list,  level+1, element.Next_Moves,  not mycolor)
      
    return
    
  def max_list(self,  list):    
    m = 0
    for a in list:
      if a.Count > m:
        m = a.Count
    return m

  def walk_tree(self, entry,  level):
    self.NodeCounter += 1
    to_be_removed = []
    if entry.MyColor == True and level > 2:
      if self.max_list(entry.Next_Moves) >= 6:
        for a in entry.Next_Moves:
          if a.Count < 0.35*self.max_list(entry.Next_Moves):
            to_be_removed.append(a)
      else:
        for a in entry.Next_Moves:
            to_be_removed.append(a)
    elif entry.MyColor == True:
      for a in entry.Next_Moves:
        if a.Count < 0.35*self.max_list(entry.Next_Moves):
          to_be_removed.append(a)
    for x in to_be_removed:
      entry.Next_Moves.remove(x)
    
    for a in entry.Next_Moves:
      self.walk_tree(a,  level+1)
    return
    
  def give_entry_numbers(self, entry):
    if len(entry.Next_Moves) > 0:
      entry.NodeNumber = self.cur_num
      self.cur_num += 1
      if  len(entry.Next_Moves) > self.max_nr_of_moves:
        self.max_nr_of_moves = len(entry.Next_Moves)
    else:
      entry.NodeNumber = -1
    for a in entry.Next_Moves:
      self.give_entry_numbers(a)
    return
    
  def print_tree(self, entry):
    if entry.NodeNumber >= 0:
      print str(entry.NodeNumber) + ','+str(len(entry.Next_Moves))
      for a in entry.Next_Moves:
        print str(a.Start)+','+str(a.Stop)+','+str(a.NodeNumber)
      for a in entry.Next_Moves:
        self.print_tree(a)
    return

  def print_first_moves(self):
    for a in z.Tree.Next_Moves:
      print a.NodeNumber,  a.Start,  a.Stop,  a.Count,  z.Tree.MyColor
      for b in a.Next_Moves:
        print '\t',  b.NodeNumber,  b.Start,  b.Stop,  b.Count,  a.MyColor
        for c in b.Next_Moves:
          print '\t\t',  c.NodeNumber,  c.Start,  c.Stop,  c.Count,  b.MyColor
    return


color = "white"  
z = GenerateOpeningBook(color)
f = z.open_input(color)
for line in f:
  z.proc_line(line,  color == "white")
#z.print_first_moves()
#print '-----------------------------------------------'
z.walk_tree(z.Tree, 0)
z.give_entry_numbers(z.Tree)
z.print_tree(z.Tree)
#z.print_first_moves()
#print '\n\n', z.cur_num,  z.max_nr_of_moves
