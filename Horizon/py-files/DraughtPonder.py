#
#  This file is Copyright (C) 2010 Feike Boomstra.
#  Distributed under the Boost Software License, Version 1.0.
#  (See accompanying file LICENSE_1_0.txt or copy at
#  http://www.boost.org/LICENSE_1_0.txt)
#


class Ponder():
""""
pondering is done to calculate in the opponents thinking time
with pondering you prepare the hash-table for the next move
there are in general two options,
a) you guess what is the best move for your opponent, and start pondering as it's your move
b) you ponder on your opponents move.
I don't know what is best, but I choose for b).


""""
  def __init__(self):
    self.hash_state = 'not initialized'
    self.hash_ply_nr = -1
    # This is to manipulate the hash_table in the engine
    # To initialize the hash, the depth in the hashrecord is set to -1
    # To update the hash, the depth <-- depth - (this_hash_ply_nr - previous_hash_ply_nr)
