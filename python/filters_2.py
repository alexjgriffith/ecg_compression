#!/usr/bin/python
import math
import matplotlib.pyplot as plt
import numpy

def signal(sample,fs,A,P):
    sig=math.sin(float(sample)/float(fs)*2*math.pi*P)*A
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


def band_pass(w1,w2,order,ripple,i):
        y=cheb(float (float(i+1)/(float(w1))),order,ripple)
        z=cheb(float (float(w2)/float(i+1)),order,ripple)
        return y*z

def make_filter(order,fs,length):
    x,y=[[],[]]
    for i in range(length/2):
        f=i*fs/length
        y.append(band_pass(18,5,order,0.1,f))
        x.append(f)
    for i in range(length/2):
        f=i*fs/length+fs/2
        y.append(y[length/2-i-1])
        x.append(f)
    return x,y

def create_signals(length,fs):
    time,signal_v=[[],[]]
    for i in range(length):
        signal_v.append(signal_serise(i,fs))
        time.append(float(i)/float(fs))
    return time, signal_v

def main():
    length=2**12
    fs=100
    filter_f,filter_m=make_filter(6,fs,length)
    chev_1=numpy.fft.ifft(filter_m)
    chev_coeff=numpy.array(numpy.real(chev_1))
    chev_coeff=chev_coeff[0:30]
    time,signal_v=create_signals(length,30)
    filt,time,out=[[],[],[]]
    
    temp=0
    for j in range(2*len(chev_coeff)):
        if j<len(chev_coeff):
            temp=chev_coeff[len(chev_coeff)-j-1]
        else:
            temp=chev_coeff[j-len(chev_coeff)]
        filt.append(temp)
    
    temp=0
    for i in range(length):
        if i>len(filt):
            temp=0
            for j in range(len(filt)):
                    temp+=filt[j]*signal_v[i-j]
        time.append(i/fs)
        out.append(temp)
    out=numpy.real(numpy.fft.fft(out))
    #print out.shape
    out_2,out_3=[[],[]]
    value=numpy.real(numpy.fft.fft(signal_v))
    for i in range(length):
        out_2.append(value[i]*filter_m[i])
        out_3.append(out_2[i]-out[i])
    #plt.stem(range(len(filt)),filt)
    plt.plot(out_3,'ro')
    #print len(numpy.fft.fft(out)),len(filter_f)
    #plt.stem(filter_f,numpy.real( numpy.fft.fft(out)))
    plt.show()
    

if __name__=='__main__':
    main()
