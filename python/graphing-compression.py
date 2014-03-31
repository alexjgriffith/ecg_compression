#!/usr/bin/python
from math import sin , log10,pow,pi,sqrt
import matplotlib.pyplot as plt
import numpy as np
import sys


def load_sum_data(filename):
  f=open(filename)
  a_list=[]
  b_list=[]
  i=0
  for line in f:
      if line.strip().split(' ')[0]=='':
          continue
      a=line.strip().split()
      i+=int(a[0])
      a_list.append(i)
      b_list.append(a[1])
  return a_list,b_list

def load_data(filename,ret=0):
  f=open(filename)
  a_list=[]
  b_list=[]
  for line in f:
      if line.strip().split(' ')[0]=='':
          continue
      a=line.strip().split()
      a_list.append(a[0])
      if ret==0:
          b_list.append(a[1])
  if ret!=0:
      return [a_list]
  else:
      return [a_list,b_list]

def plot_alt(f_m):
    f,m=f_m
    plt.close('all')
    plt.plot(f,m)
    plt.show()
def plot_model(f_m):
  plt.close('all')
  for i in f_m:
   #plt.plot(i,'o')
   plt.plot(i[0:2000])
  #plt.plot(i[0:2000],'o')
  #plt.ylabel('DB(Z)')
  #plt.xlabel('Frequency')
  #plt.xscale('log')
  plt.show()

def main(plot_file):
  plot_model(load_data(plot_file))
"""
def alt_1 (plot_file_1, plot_file_2):
    a,b,a_data,b_data=[[],[],[],[]]
    a_data=load_data(plot_file_1)
    b_data=load_data(plot_file_2)
"""    
def alt_2(plot_file):
    plot_model(load_data(plot_file, ret=1))
         
def plot_two(f_m_1,f_m_2,file_name):
  f_1,m_1=f_m_1
  f_2,m_2=f_m_2
  x=None
  framecount=10;
  frame=0;
  plt.plot(f_1,m_1,f_2,m_2,'o')
  plt.show()
  """plt.ion()
  fig=plt.figure()
  sp=fig.add_subplot(111)
  sp.plot(f_1,m_1,f_2,m_2,'o')
  #line, = sp.plot(range(len(data)),m_1 )
  #while True:

  plt.draw()
  while True:
    x=sys.stdin.readline().strip()
    if x=='y':
      sys.stdout.write("line\t"+sys.argv[4]+"\n")
      break
    if x=='n':
      
      break
   # frame+=1
   """  

if __name__=='__main__':
    #print sys.argv[2]
    if len(sys.argv)==3:
        if sys.argv[2]=="1":
            alt_2(sys.argv[1]);
        if sys.argv[2]=='sum':
            plot_alt(load_sum_data(sys.argv[1]))
        if sys.argv[2]=='alt':
            plot_alt(load_sum_data(sys.argv[1]))
    if len(sys.argv)==5:
        if sys.argv[2]=='alt2':
          plot_two(load_data(sys.argv[1]),load_data(sys.argv[3]),sys.argv[4])
          
      # else:
        #    alt_1(sys.argv[1],sys.argv[2])
    if len(sys.argv)==2:
            main(sys.argv[1])
