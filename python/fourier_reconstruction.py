#!/usr/bin/python
from math import sin , log10,pow,pi,sqrt,cos,tan
import matplotlib.pyplot as plt
import numpy as np
import sys

def load_data(filename,ret=0):
  f=open(filename)
  a_list,b_list,c_list=[[],[],[]]
  for line in f:
      if line.strip().split(' ')[0]=='':
          continue
      a=line.strip().split()
      a_list.append(a[0])
      if ret==0:
          b_list.append(a[1])
          c_list.append(a[2])
  a_list=np.array(a_list).astype('float')
  if ret==0:
      b_list=np.array(b_list).astype('float')
      c_list=np.array(c_list).astype('float')
      return a_list,b_list,c_list
  return a_list
def plot_alt(f_m):
    f,m=f_m
    plt.close('all')
    plt.plot(f,m,'o')
    plt.show()

def signal(freq,mag,phase,end,fs):
    signal=[]
    step=1/float(fs)
    length=[0,float(end)] #ms
    #print mag,freq,length[0],phase 
    while length[0]<length[1]:
        signal.append(mag*cos(float(freq*2.0*pi*length[0]+phase)))
        length[0]+=step
    return signal



def construct_signal(frequency,magnatude,phase,fs):
    frange=[]#[0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24] #hz
    step=0.5 #hz
    for i in range(50):
        frange.append(float(i)/step)
    #crit=1.0/float(frange[0])
    crit=float(len(frequency))/float(fs)
    step_l=1/float(fs) #ms
    length=[0,crit] #ms
    signals=[]
    #print frequency
    while length[0]<length[1]:
        signals.append(0)
        length[0]+=step_l
    for fr in frequency:
        i=freq_find(fr,frequency)
        m=magnatude[i]#m=interp(i,frequency,fr,magnatude)
        f=phase[i]#f=interp(i,frequency,fr,phase)
        temp=signal(fr, m,f,crit,fs)
        for k in range(len(signals)):
            signals[k]+=temp[k]        
    return signals

def interp(i,freq,a_f,value):
    return (value[i]*(float(a_f)-freq[i-1]) + value[i+1]*(freq[i]-float(a_f))) /(freq[i]-freq[i-1])

def freq_find(freq,data):
    for i in range(len(data)):
        if data[i]>=freq:
            return i
            
def find_n(f):
    i=0
    start=[]
    end=[]
    start.append(0)
    k=0
    for i in range(len(f)-1):
        if f[i+1]<f[i]:
            k+=1
            if k==1:
                end.append(i+1)
            elif k>1:
                end.append(i+1)
                start.append(end[k-2])
    end.append(i)
    return start, end

def test(data_file,start_stop,origional):
    fs=250
    garbage,start,peak=load_data(start_stop)
    data=load_data(origional, ret=1)
    a,b,c=load_data(data_file)
    start,end=find_n(a)
    x,y,fr=[[],[],[]]
    value=[]
    n=0
    #print len(start)
    for n in range(len(end)-1):
        for k in range(end[n]-start[n]):
            value.append((data[start[n]+k]-np.mean(data[start[n]:end[n]]))/max(data[start[n]:end[n]]))
        
        values_1=np.fft.fft(value)
        values_2=np.fft.fft(value)
        values_3=np.fft.fft(value)
        k=0
        freq_1,freq_2,freq_3=[[],[],[]]
        for i in range(15):
            freq_1.append(i+1)
        for i in range(21):
            freq_2.append(i+1)
        for i in range(36):
            freq_3.append(i+1)
        for i in values_1:
            x.append(sqrt(i.real**2+i.imag**2))
            y.append(tan(i.imag/i.real))
            fr.append(float(k))
            if float(k) in freq_1:
                values_1[k]=i*2
            else:
                values_1[k]=0+0j
            if float(k) in freq_2:
                values_2[k]=i*2
            else:
                values_2[k]=0+0j
            if float(k) in freq_3:
                values_3[k]=i*2
            else:
                values_3[k]=0+0j
            k+=1
        ret_1=np.fft.ifft(values_1)    
        ret_2=np.fft.ifft(values_2)    
        ret_3=np.fft.ifft(values_3)    
        for i in range(len(values_1)):
            print value[i],ret_1[i].real,ret_2[1].real,ret_3[i].real
        
    """
    width=len(np.fft.fft(value))/2
    f, axarr = plt.subplots(1,2)
    axarr[0].stem( fr[start[n]:start[n]+(end[n]-start[n])/2],x[start[n]:start[n]+(end[n]-start[n])/2], '-r')
    #axarr[0].set_title("a.)")
    axarr[0].set_xlabel("Frequency")
    axarr[0].set_ylabel("Magnatude")
    time=[]
    for i in range(len(value)):
        time.append(float(i)/fs)
    axarr[1].plot( time,value, '-b')
    axarr[1].plot( time,ret_3.real, '-r')
    #axarr[1].set_title("b.)")
    axarr[1].set_xlabel("Time")
    #axarr[1].set_ylabel("Normalized Amplitude")
   
    plt.show()
    """

def main(data_file,start_stop,origional):
    fs=250
    garbage,start,peak=load_data(start_stop)
    data=load_data(origional, ret=1)
    a,b,c=load_data(data_file)
    start_stop.append(0)
    start,end=find_n(a)
    x,y=[[],[]]
    
    
    for i,k,j in zip(a,b,c):
        x.append(float(k)*cos(float(2*pi*i+k))  +1j*float(k)*sin(float(2*pi*i+k)))
        
    for n in range(2):
        #print len(a[start[n]:end[n]])
        signal=construct_signal(a[start[n]:end[n]],b[start[n]:end[n]],c[start[n]:end[n]],fs)
        #plt.plot(range(len(signal)),signal)
        #plt.plot(np.fft.ifft(x[start[n]:end[n]]))
        #plt.show()
        #print start_stop[n],start_stop[n+1]
        value=[]
        for k in range(end[n]-start[n]):
            value.append((data[start[n]+k]-np.mean(data[start[n]:end[n]]))/max(data[start[n]:end[n]]))
        width=len(np.fft.fft(value))
        #print np.fft.fft(value)
        plt.plot(value)
        y=[]
        for i in range(width/4):
            y.append(i*4)
        plt.plot(y,np.fft.ifft(np.fft.fft(value)[0:width/4]))
        plt.show()
    
if __name__=='__main__':
    test(sys.argv[1],sys.argv[2],sys.argv[3])
