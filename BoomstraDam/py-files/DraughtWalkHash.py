#
#  This file is Copyright (C) 2010 Feike Boomstra.
#  Distributed under the Boost Software License, Version 1.0.
#  (See accompanying file LICENSE_1_0.txt or copy at
#  http://www.boost.org/LICENSE_1_0.txt)
#


from DraughtConstants import *
from DraughtDataStructures import *
from DraughtWalkHashPanel import *
from DraughtDebugInfoPanel import *
dc = DraughtConstants()

class HashRecord:
    def __init__(self, white_man,  white_king,  black_man,  black_king,  maxnode,  whitecolor,  eval,  useless,  exact,  cycle,  prom,  ext,  hasext,  depth,  dist,  lower,  upper, rdepth):
        self.board = DraughtBoard()
        self.board.WhiteMan = white_man
        self.board.WhiteKing = white_king
        self.board.BlackMan = black_man
        self.board.BlackKing = black_king
        if (whitecolor == False):
            self.board.ColorOnMove = dc.BLACK_ON_MOVE
        self.max_node = maxnode
        self.white_color = whitecolor
        self.evaluated = eval
        self.useless = useless
        self.exact = exact
        self.cycle = cycle
        self.prom = prom
        self.ext = ext
        self.hasext = hasext
        self.search_depth = depth
        self.dist = dist
        self.lower = lower
        self.upper = upper
        self.depth = rdepth
        self.quiescence = None
        self.has_to_capture = None
        self.move_status = None
        self.moves = []
        return
        
    def __str__(self):
        s = ''
        for i in range(0, len(self.moves)):
            s = s + str(self.moves[i]) + '\n'
        return s
        
        
class HashMoveRecord:
    def __init__(self,  start,  stop,  target,  empty,  lower,  upper,  hash_key):
        self.start = start
        self.stop = stop
        self.target = target  # bool true is target extists in hash
        self.empty = empty
        self.lower = lower
        self.upper = upper
        self.hash_key = hash_key
        return
        
    def __str__(self):
        s = str(self.start).zfill(2) + str(self.stop).zfill(2) + ' ' + self.lower + ' ' + self.upper + '    (' + self.hash_key + ')'
        return s
                
class DraughtWalkHash:
    def __init__(self,  main,  root):
        self.main = main
        self.root = root
        self.CurrentSearchDepth = 20
        self.CurrentNumberOfThreads = 4
        self.InitHash = 1
        self.Positions = []
        self.AltPositions = []
        return
        
    def setup_hash(self):
        game = self.main.ActiveGame
        self.main.play.startup_engine()
        game.CurrentPosition.ColorOnMove = self.main.panel.current_color_on_move.get()
        self.main.play.engine.send_setup_hash(game , self.CurrentSearchDepth,  self.CurrentNumberOfThreads,  self.InitHash)
        return
        
    def set_depth(self):
        self.info =  DraughtDebugInfoPanel(self,  self.CurrentSearchDepth, self.CurrentNumberOfThreads,  self.InitHash)
        self.info.root.mainloop()
        return
     
    def Read_Walk_Info(self):
        self.CurrentSearchDepth = int(self.info.search_depth_text.get())
        self.CurrentNumberOfThreads = int(self.info.num_threads_text.get())
        self.InitHash = self.info.Init_Hash.get()
        print self.InitHash
        self.info.root.destroy()
        return
        
    def set_threads(self):
        self.info =  DraughtDebugInfoPanel(self,  self.CurrentSearchDepth, self.CurrentNumberOfThreads ,  self.InitHash)
        self.root.mainloop()
        return
        
    def walk_hash_dump(self):
        return

    def walk_hash_proc(self):
        self.Positions = []  # reset stack
        self.panel =  DraughtWalkHashPanel(self)
        self.panel.LowerBound.set("-")
        self.panel.UpperBound.set("-")
        self.panel.Node.set("-")
        self.panel.Color.set("-")
        self.panel.IsEvaluated.set("-")
        self.panel.Useless.set("-")
        self.panel.ExactValue.set("-")
        self.panel.CyclePossible.set("-")
        self.panel.JustPromoted.set("-")
        self.panel.IsExtended.set("-")
        self.panel.HasExtCondition.set("-")
        game = self.main.ActiveGame
        self.main.play.startup_engine()
        game.CurrentPosition.ColorOnMove = self.main.panel.current_color_on_move.get()
        self.main.play.engine.send_get_from_hash(game )
        self.root.mainloop()         
        return
        
    def show_home_node_pressed(self):
        for k in range(0,  len(self.Positions) - 1):
            self.Positions.pop()
        self.show_pos(-1,  self.Positions,  self.panel)
        return
  
    def show_prev_node_pressed(self):
        if len(self.Positions) > 1:
            self.Positions.pop()
        self.show_pos(-1,  self.Positions,  self.panel)
        return
  
    def show_next_node_pressed(self):
        selection = self.panel.all_moves.curselection()
        if selection==(): 
            return
        move_nr = int(selection[0])
        hash_key = self.Positions[-1].moves[move_nr].hash_key
        self.main.play.engine.send_get_with_key(hash_key)

        return
        
    def show_eval_node_pressed(self):
        self.main.play.startup_engine()
        self.main.play.engine.send_evaluate_from_board_position(self.Positions[-1].board, True)
        return
  
    def walk_hash_script(self):
        self.main.read.get_pdn_file(True,  '/home/feike/BoomstraDam/pdn files/aaa.pdn') #open the test pdn file
        game = self.main.ActiveGame
        game.HalfPlyPointer = game.half_ply_length() #end of file
        game.build_till_pointer()
        game.refresh_display()
        self.main.play.startup_engine()
        self.main.play.engine.send_setup_hash(game , 8,  1,  1)
        self.main.play.engine.send_setup_hash(game , 8,  4,  1)
        self.walk_hash_proc()
        return

# msg format
# 01-50 50sq position
# 51 max node
# 52 white color
# 53 is evaluated
# 54 useless sacrifice
# 55 is exact value
# 56 cycle possible
# 57 just promoted
# 58 is extended
# 59 has extention condition
# 60-62 search depth (3)
# 63-65 distance from root (3)
# 66-71 lower bound(6)
# 72-77 upper bound (6)
# 78-79 nr of moves
# movelist:
# 00-01 start
# 02-03 stop
# 04 target exists in hash                                           if not
# 05-10 lower bound target ( if any) (6)                      # 05 target is empty
# 11-16 upper bound target (6)                                   
# 17-23 hash key target (7)

# at the end
# best move index (2)

    def receive_hash_node(self, msg):
        if msg[1] <> 'x':
            new_hash_node,  next_start = self.get_hash_node(msg,  0)
            self.Positions.append(new_hash_node)
            self.show_pos(-1,  self.Positions,  self.panel)        
        return
        
    def get_hash_node(self,  msg,  p):      
        white_man,  white_king,  black_man,  black_king = self.get_50_square(msg, p)
        maxnode      = (msg[p+51] == '1')
        color      = (msg[p+52] == '1')
        eval       =  (msg[p+53] == '1')
        useless   =  (msg[p+54] == '1')
        exact     = (msg[p+55] == '1')
        cycle     = (msg[p+56] == '1')
        prom     = (msg[p+57] == '1')
        ext        = (msg[p+58] == '1')
        hasext   = (msg[p+59] == '1')
        depth    = msg[p+60:p+63]
        dist        = msg[p+63:p+66]
        low         = msg[p+66:p+72]
        up         = msg[p+72:p+78]
        new_rec = HashRecord(white_man,  white_king,  black_man,  black_king,  \
                                            maxnode,  color,  eval,  useless,  exact,  cycle,  prom,  ext,  hasext,  depth,  dist,  low,  up, "-1")
        nr_moves = int(msg[p+78:p+80])
        for i in range(0, nr_moves):
            self.insert_move(msg[p+80 +24*i : p+104 +24*i],  new_rec) 
        tail_start = p+104+24*(nr_moves-1)
        new_rec.bestmove = int(msg[tail_start:tail_start+2])
        new_rec.quiescence = (msg[tail_start+2] == '1')
        new_rec.has_to_capture = (msg[tail_start+3] == '1')
        st = msg[tail_start+4:tail_start+6]
        if st == " 0" : new_rec.move_status = "max node free move"
        elif st == " 1": new_rec.move_status = "min node captured"
        elif st == " 2": new_rec.move_status = "min node first freed"
        elif st == " 3": new_rec.move_status = "min node second freed"
        elif st == "10" : new_rec.move_status = "min node free move"
        elif st == "11": new_rec.move_status = "max node captured"
        elif st == "12": new_rec.move_status = "max node first freed"
        elif st == "13": new_rec.move_status = "max node second freed"
        else: new_rec.move_status = "Unknown"            
        new_rec.depth = msg[tail_start+6:tail_start+9]
        return new_rec,  tail_start + 9
        
    def insert_move(self,  msg,  new_rec):
        start = msg[0:2]
        stop = msg[2:4]
        target = (msg[4] == '1')
        if target == True:
            lower = msg[5:11]
            upper = msg[11:17]
            hash_key = msg[17:24]
            empty = False
        else:
            empty = (msg[5] == '1')
            lower = '  --  '
            upper = '  --  '
            hash_key = '0'
        new_move_record = HashMoveRecord(start,  stop,  target,  empty,  lower,  upper,  hash_key)
        new_rec.moves.append(new_move_record)
        return

    def get_50_square(self,  msg,  p):
        # it is not so handy but the send proc includes the color
        # get handles just the 50 squares
        white_man = []
        white_king = []
        black_man = []
        black_king = []
        
        for i in range(p+1, p+51):
            if msg[i]  == 'w': white_man.append(i-p)
            elif msg[i]  == 'W': white_king.append(i-p)
            elif msg[i]  == 'z': black_man.append(i-p)
            elif msg[i]  == 'Z': black_king.append(i-p)
            
        return white_man,  white_king,  black_man,  black_king

    def show_pos(self,  num,  Position,  Panel):
        pos = Position[num]
        self.display_pieces(pos,  Panel)
        if pos.max_node == True:  Panel.Node.set("Max") 
        else: Panel.Node.set("Min")
        if pos.white_color == True:  Panel.Color.set("White") 
        else: Panel.Color.set("Black")
        if pos.evaluated == True:  Panel.IsEvaluated.set("Evaluated") 
        else: Panel.IsEvaluated.set("")
        if pos.useless == True:  Panel.Useless.set("Useless Sacrifice") 
        else: Panel.Useless.set("")
        if pos.exact == True:  Panel.ExactValue.set("Is Exact Value") 
        else: Panel.ExactValue.set("")
        if pos.cycle == True:  Panel.CyclePossible.set("Cycle Possible") 
        else: Panel.CyclePossible.set("")
        if pos.prom == True:  Panel.JustPromoted.set("Just Promoted") 
        else: Panel.JustPromoted.set("")
        if pos.ext == True:  Panel.IsExtended.set("Is Extended") 
        else: Panel.IsExtended.set("")
        if pos.quiescence == True:  Panel.IsQuiescence.set("Quiescence") 
        else: Panel.IsQuiescence.set("")
        if pos.has_to_capture == True:  Panel.HasToCapture.set("Has to Capture") 
        else: Panel.HasToCapture.set("")
        if pos.hasext == True:  Panel.HasExtCondition.set("Has Ext Condition") 
        else: Panel.HasExtCondition.set("")
        Panel.SearchDepth.set(pos.search_depth)
        Panel.DistanceFromRoot.set(pos.dist)
        Panel.LowerBound.set(pos.lower)
        Panel.UpperBound.set(pos.upper)
        Panel.BestMoveIndex.set(str(pos.bestmove))
        Panel.MoveStatus.set(pos.move_status)
        Panel.Depth.set(pos.depth)
        # now the list of moves
        Panel.all_moves.delete(first=0, last=Panel.all_moves.size()-1)
        for j in range(0, len(pos.moves)):
            Panel.all_moves.insert(END, str(pos.moves[j]))
        Panel.all_moves.selection_set(pos.bestmove)
        return
        
    def display_pieces_old(self,  pos,  Panel):
        for i in range(1, 51):
            if i in pos.white_man:
                Panel.board3.set_white_man(i)
            elif i in pos.white_king:
                Panel.board3.set_white_king(i)
            elif i in pos.black_man:
                Panel.board3.set_black_man(i)
            elif i in pos.black_king:
                Panel.board3.set_black_king(i)
            else:
                Panel.board3.set_empty_field(i)
        return
        
    def display_pieces(self,  pos,  Panel):
        Panel.board3.display(pos.board)
        return
