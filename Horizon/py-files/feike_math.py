#
#  This file is Copyright (C) 2010 Feike Boomstra.
#  Distributed under the Boost Software License, Version 1.0.
#  (See accompanying file LICENSE_1_0.txt or copy at
#  http://www.boost.org/LICENSE_1_0.txt)
#


from math import *

class FeikeMathError(Exception):
  def __init__(self):
    print "vectors not the same dimension"
    return

def average(list):
   return float(sum(list))/float(len(list))

def vector_sum(list1, list2):
  if len(list1) <> len(list2):
    raise FeikeMathError
  else:
    return map(add, list1, list2)

def add(x, y):
  return x+y

def weighted_average(list, ratio):
  s = 0.0
  for z in range(0, len(list)):
    s += ratio[z]*list[z]
  return s/sum(ratio)

def std_dev(list):
   mu = average(list)
   b=[(x-mu)**2 for x in list]
   c=sum(b)
   l = len(list)
   if l > 1:
    return sqrt(c/(l-1))
   else:
    return 0

def std_dev_line(list):
  m, b = best_fit(list)
  fit_list = []
  for z in range(0, len(list)):
    fit_list.append(z*m+b)
  sqr_dist = sqr_distance(list, fit_list)
  if len(list) > 1:
    return sqrt(sqr_dist/(len(list)-1))
  else:
    return 0


def weighted_std_dev(list, ratio):
   mu = weighted_average(list, ratio)
   c = 0
   for z in range(0, len(list)):
     c += ratio[z]*(list[z] -mu)**2
   l = len(list)
   if l > 1:
    return sqrt(c/(l-1))
   else:
    return 0

def dist(x, y):
  return (x-y)**2

def sqr_distance(list1, list2):
  if len(list1) <> len(list2):
    raise FeikeMathError
  else:
    return sum(map(dist, list1, list2))

def multiply(x, y):
  return x*y

def sqr(x):
  return x**2

def best_fit(list):
  n = len(list)
  xSum     = float(sum(range(0, n)))
  ySum     = float(sum(list))
  xySum   = float(sum(map(multiply, list, range(0, n))))
  xSqSum = float(sum(map(sqr, range(0, n))))
  n = float(n)
  m = (n*xySum - xSum*ySum) / (n*xSqSum - xSum*xSum)
  b = (ySum - m*xSum) / n
  return m, b

def slope(list):
  m, b = best_fit(list)
  return m

def increasing(list, eps):
  return slope(list) - eps > 0

def decreasing(list, eps):
  return slope(list) + eps < 0

def flat(list, eps):
  return slope(list) + eps > 0 and slope(list) - eps < 0

def make_array(dim, init):
  a = []
  for k in range(0, dim):
    a.append(init)
  return a



