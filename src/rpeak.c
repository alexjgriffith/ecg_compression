#include "../headers/rpeak.h"

int main(void)
{
  int power=12;
  int length=pow(2,power);
  int fs=250;
  int i,j,k,t;
  double * cheb_filter=(double*)calloc(length, sizeof(double));
  double * filtered_data=(double*)calloc(length, sizeof(double));
  double * garbage=(double*)calloc(length, sizeof(double));
  double value;
  double temp=0;
  double * hold=(double*)calloc(length, sizeof(double));
  double iter=0;
  int trof[300],peak[300],flag_p[300];
  int ppeak,RF[300],FN[300];
  //build cheb filter
  for(i=0;i<length;i++)
    {
      temp=(double)i*(double)fs/(double)length;
    cheb_filter[i]=band_pass(25.0,5.0,3.0,0.1,temp );
    
    // printf("%lf\t%lf\n",temp,cheb_filter[i]);
    }
  
  //load the first 4096 or less data points
   i=0;
  while(load_from_stdin(&value))
    {
   
      filtered_data[i]=value;
      if (i==length)
	{ 
	  FILE *plot = fopen("plot.data","w");
	  for(i=0;i<length-1;i++)
	    fprintf(plot,"%lf\t%lf\n",(double)iter*(double)length/(double)fs+ (double)i/(double)fs, filtered_data[i]);
	  fclose(plot);
	  for(i=0;i<length;i++)
	    hold[i]=filtered_data[i];	  

	  //if(iter==0)
	  //need to replace with a fir filter eventualy
	  fft(1,power,filtered_data,garbage);
	  for(j=0;j<length;j++)
	    {
	      temp=cheb_filter[j]*filtered_data[j];
	      filtered_data[j]=temp;
	      temp=cheb_filter[j]*garbage[j];
	      garbage[j]=temp;
	    }

	  fft(-1,power,filtered_data,garbage);	    
  
	  difference(filtered_data,length);
	  //need acess to difference later

	  //the final value of the difference comparason will be
	  //fed into the next round

	  for(i=0;i<length;i++)
	    {
	      garbage[i]=filtered_data[i];
	    }
	  normalize(filtered_data,length-1);
	  shannon_entropy_envelope(filtered_data,length-1); 

	  triangle_filter(filtered_data,length-1);

	  //printf("%lf\t%lf\n", (double)i, filtered_data[i]);
	  k=0;
	  t=0;
	  memset(flag_p,0,300);
	  memset(trof,0,300);
	  memset(peak,0,300);   
	  //for(j=ceil(2.5*fs);j<floor(length-2.5*fs);j++)
	  for(j=0;j<length-1;j++)
	    {
	      if(filtered_data[j-1]>filtered_data[j-2]&&filtered_data[j-1]>=filtered_data[j])
		{
		  peak[k]=j;
		  k++;
		}
	      if(filtered_data[j-1]<filtered_data[j-2]&&filtered_data[j-1]<=filtered_data[j])
		{
		  trof[t]=j;
		  t++;
		}
	    }
	  j=0;
	  while(filtered_data[peak[j]]<0.015)
	    {	     
	      RF[j]=1;
	      FN[j]=1;
	      j++;
	    }
	  ppeak=peak[j-1];
	  //RF[j]=1;
	  //FN[j]=0;
	  for(i=j;i<k;i++)
	    {
	      falserdetection(filtered_data,garbage,peak[i],trof[i],ppeak,fs,&FN[i],&RF[i],length);
	    if(RF[i]==0)
	      ppeak=peak[i];
	    }
	  int temp;
	  int temp_p=0;
	  for(i=0;i<k;i++)
	    {
	      //memset(filtered_data,0,length);
		  if(RF[i]==0)// || FN[i]==0)
	      {
		temp=max_abs_location(hold,peak[i]-65-fs*0.1,peak[i]-65+fs*0.1);
		for(j=temp_p;j<temp;j++)
		  garbage[j-temp_p]=hold[j];
		// if (iter<2)
		 test_fft(temp-temp_p,garbage, (double)fs );
		//printf("%lf\t%lf\n",(double)iter*(double) length/(double) fs + (double)trof[i]/(double)fs,filtered_data[trof[i]]);
		printf("%d\t%lf\t%lf\n",k,(double)iter*(double) length/(double) fs + (double)temp/(double)fs,hold[temp]);
		 memset(garbage,0,length);
		temp_p=temp;
	      }
	    }	  
	  //filtered_data[0]=hold;
	  i=-1;
	  iter++;
	}
      i++;
    }
  return 0;
}



int falserdetection(double * filtered_data,double * garbage, int peak,int trof,int ppeak,int fs,int *FN,int *RF,int length)
{
  *RF=1;
  int i;
  int zone[2];
  zone[0]=(peak+trof)/2 - 2.5*fs;
  zone[1]=(peak+trof)/2 + 2.5*fs;
  //printf("%d\t%d\n",zone[0],zone[1]);
  if(zone[0]<0)
    zone[0]=0;
  if(zone[1]>length)
    zone[1]=length;
  double max_d =max(garbage,zone[0],zone[1]);
  double max_v=max(filtered_data,zone[0],zone[1]);
  double mean_v=mean(filtered_data,zone[0],zone[1]);
  *FN=1;
  if (max_d<0.015 || filtered_data[peak]<0.0015 || peak-ppeak<fs*.2)
    {
      *RF=1;
      return 1;
    }
  if(max_v>mean_v*6.0)
    max_v=mean_v*6.0;
  if (filtered_data[peak]-filtered_data[trof]<0.1*max_v)
    {
      *RF=1;
      *FN=0;
      return 2;
    }
  *RF=0;
  return 3;
       
}
		    
int normalize(double *y,int length)
{
  int i;
  double temp;
  double max=y[0];
  for(i=1;i<length;i++)
    {
      if(y[i]>max|| y[i]*-1>max)
	{	
	  max=sqrt(pow(y[i],2));
	  //printf("%lf\n",max);
	}
    }
  for(i=0;i<length;i++)
    {
      temp=y[i]/max;
      y[i]=temp;
    }
  //printf("%lf\n",max);
  return 0;
}
int triangle_filter(double *y, int length)
{
  int i,j;
  int N=65;
  static double * s=(double *)calloc(length,sizeof(double));
  static double *yI=(double *)calloc(length,sizeof(double));
  static double * yy=(double *)calloc(length,sizeof(double));
  double temp=0;
  for(i=0;i<length;i++)
    {
      if(i<2*N+1)
	{
	  for(j=0;j<i;j++)
	    temp+=y[i];
	  yI[i]=temp;
	  temp=0;
	  for(j=0;j<i;j++)
	    temp+=yI[i];
	  yy[i]=temp;
	  temp=0;
	}	
      else
	{
	  yI[i]=yI[i-1]-y[i-N]+y[i];
	  yy[i]=yy[i-1]-yI[i-N]+yI[i];
	}
      s[i]=yy[i]/(4225.0);
    }
  for(i=0;i<length;i++)
    {
      
      y[i]=s[i];
    }
  //free(s);
  //free( yI);
  //free(yy);

    
  return 0;
  
  
}
int shannon_entropy_envelope(double *y, int length)
{
  int i;
  double temp;
  for(i=0;i<length;i++)
    {
      if (y[i]==0)
	temp=0;
      else
	temp=-1*pow(y[i],2)*log(pow(y[i],2));
      y[i]=temp;
    }
  return 0;
}

int difference(double *y,int length)
{
  int i;
  double temp;
  for(i=1;i<length;i++)
    {
      temp=y[i]-y[i-1];
      y[i-1]=temp;
    }
  return 1;
    
}
double band_pass(double w1,double w2, double order, double ripple,double i)
{
  double filter_a=0;
  double filter_b=0;
  cheb((i+1.0)/w1,order,ripple,i,&filter_a);
  cheb(w2/(i+1.0),order,ripple,i,&filter_b);
  return filter_a*filter_b;
}

int cheb(double f, double order, double ripple,double i,double *filter)
{
  double t;
  if (abs(f)<1)
    t=cos(order*acos(f));
  else if (f>1)
    t=cosh(order*acosh(f));
  else if (f<1)
    t=pow(-1,order)*cosh(order*acosh(-f));
   *filter= 1.0/(1.0+ripple*ripple*t*t);
    return 1;
}


