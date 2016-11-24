#
#  This file is Copyright (C) 2010 Feike Boomstra.
#  Distributed under the Boost Software License, Version 1.0.
#  (See accompanying file LICENSE_1_0.txt or copy at
#  http://www.boost.org/LICENSE_1_0.txt)
#


import os
import string

aZ = ['a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',\
      'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z']
aZ_ = ['a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',\
       'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '_']
d1 = [     '1', '2', '3', '4', '5', '6', '7', '8', '9']
d0 = ['0', '1', '2', '3', '4', '5', '6', '7', '8', '9']

all_key =  ["white", "black", "empty", "empty_or_white", "empty_or_black","count_white", "count_black"]
any_key =  ["any_white", "any_black", "any_empty", "any_empty_or_white", "any_empty_or_black"]
not_flip_all = ["all_white", "all_black", "all_empty", "all_empty_or_white", "all_empty_or_black", "count_white", "count_black"]
flip_all =     ["all_black", "all_white", "all_empty", "all_empty_or_black", "all_empty_or_white", "count_black", "count_white"]
not_flip_any = ["white_man", "black_man", "empty_field", "empty_or_white", "empty_or_black"]
flip_any =     ["black_man", "white_man", "empty_field", "empty_or_black", "empty_or_white"]

class ParseDraughtFile:
  def __init__(self):
    self.s = ''
    self.cp = 0
    self.inp = ''
    self.flip = False
    return

  def skip_spaces(self, cp):
    loc = cp
    while self.inp[loc] in [' ', '\t']:
      loc += 1
    return loc

  def get_single_parameter(self, cp):
    loc = self.skip_spaces(cp)
    if self.inp[loc] in d1:
      if self.inp[loc+1] == ',':
        return loc+2, self.inp[loc] , False
      elif self.inp[loc+1] == ')':
        return loc+2, self.inp[loc] , True
      elif self.inp[loc+1] in d0:
        if self.inp[loc+2] == ',':
          return loc+3, self.inp[loc:loc+2] , False
        elif self.inp[loc+2] == ')':
          return loc+3, self.inp[loc:loc+2] , True
      else:
        return loc, None, True

    return loc, None, True

  def get_params(self, cp):
    par = []
    loc = cp
    while True:
      loc, param, last = self.get_single_parameter(loc)
      if param:
        par.append(param)
        if last == True:
          return par, loc
      else:
        return [], loc
    return

  def is_to_be_expanded(self):
    # we recognize structures like: aaaa(d1,d2,...)
    # and white_, black_
    loc = self.cp
    while self.inp[loc] in aZ:
      loc += 1
    if self.inp[loc] == '_':
      r = self.inp[self.cp:loc]
      if r == 'white':
        self.cp = loc
        if self.flip == False: self.s += 'white'
        else:                  self.s += 'black'
        return True
      elif r == 'black':
        self.cp = loc
        if self.flip == True:  self.s += 'white'
        else:                  self.s += 'black'
        return True
    # we start over including _
    loc = self.cp
    while self.inp[loc] in aZ_:
      loc += 1
    if self.inp[loc] == '(':
      keyword = self.inp[self.cp:loc]
      param, loc = self.get_params(loc+1)
      if param == []:
        return False
      if self.proc_macro(keyword, param) == True:
        self.cp = loc
        return True
      else:
        return False
    return False

  def proc_macro(self, key, param):
    if key in all_key:
      if len(param) == 1:
        if self.flip == False:
          self.s += '(' + not_flip_any[all_key.index(key)] + ' & ' + self.param_to_string(param) + ')'
        else:
          self.s += '(' + flip_any[all_key.index(key)] + ' & ' + self.param_to_string(param) + ')'
      else: #len(param) > 1
        if self.flip == False:
          self.s += not_flip_all[all_key.index(key)] + '(' + self.param_to_string(param) + ')'
        else:
          self.s += flip_all[all_key.index(key)] + '(' + self.param_to_string(param) + ')'
    elif key in any_key:
      if self.flip == False:
        self.s += '(' + not_flip_any[any_key.index(key)] + ' & ' + self.param_to_string(param) + ')'
      else:
        self.s += '(' + flip_any[any_key.index(key)] + ' & ' + self.param_to_string(param) + ')'
    else:
      return False # not in any key-list
    return True

  def param_to_string(self, param):
    s = 'w' + str(len(param)) + '('
    fill = ''
    for par in param:
      if self.flip == True: par = str(51-int(par))
      s += fill + par
      fill = ','
    s += ')'
    return s



  def scan_line(self, line, flip):
    self.cp = 0
    self.inp = line + '\0'
    self.flip = flip
    self.s = ''
    while self.inp[self.cp] <> '\0':
      if self.is_to_be_expanded() == False:
        self.s += self.inp[self.cp]
        self.cp += 1
    #print self.s
    return self.s

  def expand_draught_file(self, file_name):
    #print file_name
    try:
      infile = open(file_name, 'r')
    except:
      print "can't open for read:", file_name
      return

    base_name, ext = os.path.splitext(file_name)
    try:
      outfile = open(base_name + '.dam-cpp', 'w')
    except:
      print "can't open for write:" + base_name + '.dam-cpp'
      return
    outfile.write('// ----  white part -----\n')

    # don't copy the part between /* and */ comments
    CopyFlag = True
    for line in infile:
      if string.find(line, '/*') <> -1:
        CopyFlag = False
      elif string.find(line, '*/') <> -1:
        CopyFlag = True
      else:
        if CopyFlag == True:
          outfile.write(self.scan_line(line, False))
    infile.close()
    infile = open(file_name, 'r')
    outfile.write('// ----  black part -----\n')
    # don't copy the part between /* and */ comments
    CopyFlag = True
    for line in infile:
      if string.find(line, '/*') <> -1:
        CopyFlag = False
      elif string.find(line, '*/') <> -1:
        CopyFlag = True
      else:
        if CopyFlag == True:
          outfile.write(self.scan_line(line, True))
    infile.close()
    outfile.close()
    return

  def check_updates(self, todo_list):
    for files in todo_list:
      if os.path.isfile(files[0] + ".dam"):
        if os.path.isfile(files[0] + ".dam-cpp"):
          if os.path.getmtime(files[0] + ".dam") <> os.path.getmtime(files[0] + ".dam-cpp"):
            self.expand_draught_file(files[0] + ".dam")
            self.replace_content(files[0], files[1])
        else:
          self.expand_draught_file(files[0] + ".dam")
          self.replace_content(files[0], files[1])
    return

  def replace_content(self, what,  dest):
    os.rename(dest, dest+'-save')
    infile = open(dest+'-save', 'r')
    StartFound = False
    EndFound = False
    BeginMark = '// *+* begin *+* ' + os.path.basename(what)
    EndMark = '// *+* end *+* ' + os.path.basename(what)
    for line in infile:
      if string.find(line, BeginMark) <> -1:
        if EndFound == False and StartFound == False:
          StartFound = True
        else: StartFound = False  # only one start is allowed
      if string.find(line, EndMark) <> -1:
        if StartFound == True and EndFound == False:
          EndFound = True
        else:
          EndFound = False
    if StartFound == True and EndFound == True:
      infile.close()
      infile = open(dest+'-save', 'r')
      outfile = open(dest, 'w')
      srcfile = open(what + '.dam-cpp', 'r')
      CopyFlag = True
      for line in infile:
        if string.find(line, BeginMark) <> -1:
          outfile.write(line)
          for sline in srcfile:
            outfile.write(sline)
          CopyFlag = False
        elif string.find(line, EndMark) <> -1:
          outfile.write(line)
          CopyFlag = True
        else:
          if CopyFlag == True:
            outfile.write(line)
      infile.close()
      outfile.close()
      srcfile.close()
      print os.path.basename(what), "inserted"
    else:
      infile.close()
      print "problem with begin or end ", os.path.basename(what)
      os.rename(dest+'-save', dest)
    return

todo_list = [\
["/home/feike/Horizon/patterns/one_keeps_two",   "/home/feike/Horizon/c-files/eval.v2.4.cpp"], \
["/home/feike/Horizon/patterns/two_keeps_three", "/home/feike/Horizon/c-files/eval.v2.4.cpp"]\
]

parse = ParseDraughtFile()
parse.check_updates(todo_list)


