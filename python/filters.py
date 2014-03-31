#!/usr/bin/python
import math
import matplotlib.pyplot as plt
import numpy

def signal(sample,fs,A,P):
   # print math.sin(2.0*math.pi*float(sample)/float(fs)),2.0*math.pi*float(sample)/float(fs)
    sig=math.sin(float(sample)/float(fs)*2*math.pi*P)*A
    sig_2=math.sin(float(sample)/float(fs)*2*math.pi*12)
    sig_3=math.sin(float(sample)/float(fs)*2*math.pi*18)
    sig_4=math.sin(float(sample)/float(fs)*2*math.pi*500)
    return sig

def signal_serise(sample,fs):
    A=1
    out=0
    for i in range(fs/2):
       out+= signal(sample,fs,A,i)
    return out    
        
def cheb(f,n,e):
    if abs(f)<1:
        t=math.cos(float(n)*math.acos(float(f)))
    elif f>=1:
        t=math.cosh(float(n)*math.acosh(float(f)))
    elif f<=-1:
       t=-1**n*math.cosh(float(n)*math.acosh(float(-f)))
    return 1.0/(float(float(1.0+float(e**2)*float(t**2)))**.5)

def main():
    x=[]
    y=[]
    order=4
    num=10
    fs=256
    length=fs*10
    for i in range(length/2):
        f=i*fs/length
        y.append(band_pass(18,5,order,0.1,f))
        x.append(f)
    for i in range(length/2-2):
        f=i*fs/length+fs/2
        y.append(y[length/2-i-1])
        x.append(f)
    plt.plot(x,y)
    plt.show()
    plt.close('all')
    re,im=filter_coeff(num,x,y)
    plt.stem(range(len(re)),re)
    #plt.show()
    plt.close('all')
    signal_v,time,filt,time_f=[[],[],[],[]]
    for i in range(length):
        signal_v.append(signal_serise(i,fs))
        time.append(float(i)/float(fs))
        if i>num-1:
            temp=0
            for j in range(num):
                temp+=math.sqrt((re[j]**2.0+im[j]**2.0))*signal_v[i-j]
                #temp+=re[num-j-1]*signal_v[i-j]
            filt.append(temp)
        else:
            filt.append(0)
            #time_f.append(float(i)/float(fs))
    plt.plot(time[fs:],signal_v[fs:])
    plt.plot(time[fs:],filt[fs:])
    #plt.show()
    plt.close('all')
    freq,mag=[[],[]]
    val=numpy.absolute(numpy.fft.fft(signal_v))
    for i in range(len(time)-4):
        mag.append(val[i]*y[i])
        freq.append(float(i)*float(fs)/float(len(time)))
    chev_1=numpy.fft.ifft(y)
    chev_real=numpy.array(numpy.real(chev_1))
    print chev_real[0:26:2]
    #plt.plot(freq, numpy.absolute(numpy.fft.fft(signal_v)))
    #plt.stem(freq, numpy.absolute(numpy.fft.fft(filt)))
    #plt.plot(freq,mag)
    #plt.show()

def filter_coeff(n,f,m):
    re,im=[[],[]]
    for i in range(n):
        value=0.0
        for k in range(len(f)):
            value+=math.cos(float(k*(float(i)-float(n-1.0)/2)))*m[k]
        re.append(value/(float(len(f)*2*math.pi)))
    for i in range(n):
        value=0.0
        for k in range(len(f)):
            value+=math.sin(k*(i-(n-1.0)/2))*m[k]
        im.append(1.0/float(len(f))*(value))
    print re,im
    return re,im

def band_pass(w1,w2,order,ripple,i):
        y=cheb(float (float(i+1)/(float(w1))),order,ripple)
        z=cheb(float (float(w2)/float(i+1)),order,ripple)
        return y*z

if __name__=='__main__':
    main()
