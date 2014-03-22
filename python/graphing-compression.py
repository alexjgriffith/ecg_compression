#!/usr/bin/python
from math import sin , log10,pow,pi,sqrt
import matplotlib.pyplot as plt
import numpy as np
import sys

def load_data(filename):
  f=open(filename)
  a_list=[]
  b_list=[]
  for line in f:
      if line.strip().split(' ')[0]=='':
          continue
      a=line.strip().split()
      a_list.append(a[0])
      b_list.append(a[1])
  return [a_list,b_list]


def plot_model(f_m):
  plt.close('all')
  for i in f_m:
   plt.plot(i[0:2000])
  #plt.plot(i[0:2000],'o')
  #plt.ylabel('DB(Z)')
  #plt.xlabel('Frequency')
  #plt.xscale('log')
  plt.show()

def main(plot_file):
  plot_model(load_data(plot_file))

if __name__=='__main__':
    main(sys.argv[1])

