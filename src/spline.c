#include "../headers/spline.h"

double analyze_2_order(double d2_buffer[10000],double d1_buffer[10000],int  length,int start)
{
  double max,min;
  int i;
  int loc;
  loc=start;
  max=d1_buffer[start];
  for(i=start;i<length;i++)
    {
      if (d1_buffer[i]>max)
	{
	max=d1_buffer[i];
	loc=i;
	}    
    }
  if(loc!=start && loc< length-1)
    {
      return loc;
    }
  min=d1_buffer[start];
  loc=start;
  for(i=start;i<length;i++)
    {
      if (d1_buffer[i]<min)
	{
	min=d1_buffer[i];
	loc=i;
	}    
    }
  if(loc!=start && loc<length-1)
    {
      return loc;
    }
  max=d2_buffer[start];
  loc=start;
  for(i=start;i<length;i++)
    {
      if (d2_buffer[i]>max)
	{
	max=d2_buffer[i];
	loc=i;
	}    
    }
  if(loc!=start && loc<length-1)
    {
      return loc;
    }
  min=d2_buffer[start];
  loc=start;
  for(i=start;i<length;i++)
    {
      if (d1_buffer[i]<min)
	{
	min=d2_buffer[i];
	loc=i;
	}    
    }
  if(loc!=start && loc<length-1)
    {
      return loc;
    }
  return 0;
}

int pick_knots(int threshold,int window)
{
  double v[20];
  double d1_buffer[10000];
  double d2_buffer[10000];
  double temp;
  double state=0;
  int i=0;
  int k=0;
  int j=0;
  int t=0;
  int tt=0;
  int kk=0;
  int last_location=0;
  for(i=0;i<window-1;i++)
    {
    load_from_stdin(&v[i]);
    fprintf(stdout,"%04d\t%lf\n",1,v[i]);
    kk++;
    }
  last_location=kk;
  i=0;
  while ( load_from_stdin(&v[window-1]))
    {
      temp = -v[0]-v[window-1]+2*v[window/2];
	if (temp<0 && state<0)
	  {
	  d1_buffer[i]=temp;
	  d2_buffer[i]=v[window/2];
	  state=temp;
	  i++;
	  }
	else if (temp>0 && state >0)
	  {
	    d1_buffer[i]=temp;
	    d2_buffer[i]=v[window/2];
	    i++;
	  }
	else
	  {
	    if(i>threshold)
	      {
		t=0;
		while(i>t+threshold)
		  {
		    j=analyze_2_order(d2_buffer,d1_buffer,t+threshold,t);
		    if(j!=0)
		      {						  
			fprintf(stdout,"%04d\t%lf\n",kk-i+j-last_location,d2_buffer[i-j]);
			last_location=kk-i+j;
		      }
		    t=t+threshold;
		  }
		j=analyze_2_order(d2_buffer,d1_buffer,i,t);
		    if(j!=0)
		      {
			fprintf(stdout,"%04d\t%lf\n",kk-i+j-last_location,d2_buffer[i-j]);
			last_location=kk-i+j;
		      }
		    k=0;
	      }
	    memset(d2_buffer,0,sizeof(d2_buffer));
	    memset(d1_buffer,0,sizeof(d1_buffer));
	    state=temp;
	    i=0;
	  }
	
      for(tt=0;tt<window-1;tt++)
	{
	  v[tt]=v[tt+1];
	}
      kk++;
      k++;
    }
  fprintf(stdout,"%04d\t%lf\n",kk-last_location,v[2]);
  return 0;
}

int spline_decompression(double sigma)
{
  int *x= (int *)calloc(100000,sizeof(int));
  double * y = (double *)calloc(100000,sizeof(double));
  double * yp = (double *)calloc(100000,sizeof(double));
  char buffer[256];
  char buffer_t[256];
  int i=0;
  int temp;
  while(fgets(buffer,256,stdin))
    {
      sscanf(buffer,"%d\t%lf",&temp,&y[i]);
      if (i>0)
	x[i]=x[i-1]+temp;
      else
	x[i]=temp;
      if (i>100000-1)
	{
	  printf("error: file is to long\n");
	  exit(0);
	}
      //printf("%d\t%lf\n",x[i],y[i]);
      i++;
      //split_token(buffer,"\t", 1)
    }
  yp=curv1(11,x,y,sigma);
  int j=0;
  for(j=0;j<x[i-1];j=j+1)
  {
    printf("%lf\n",curv2(i-1,x,y,sigma,yp,j));
  }
      
  return 0;
}
int spline_test(double sigma)
{
  int i;
  int * x = (int *)calloc(11,sizeof(int));
  double * y = (double *)calloc(11,sizeof(double));
  double * yp = (double *)calloc(11,sizeof(double));
  x[0]=0;
  for(i=1;i<11;i++)
  {
    x[i]=x[i-1]+10;
    y[i-1]=pow((double)(x[i-1]-50.0)/10,2.0);
  }
  y[i-1]=pow((double)(x[i]-50.0)/10,2.0);
  yp=curv1(11,x,y,sigma);
  int j=0;
  for(j=0;j<100;j=j+1)
  {
    printf("%lf\n",curv2(11,x,y,sigma,yp,j));
  }
  return 0;
}

double * curv1(int n,int * x,double *y,double sigma)
{
  static double * temp =(double *)calloc(n,sizeof(double));
  double * yp=(double *)calloc(n,sizeof(double));
  int i;
  double a,b,c,slope_0,slope_n,hold,diag1,diag2,diagin,slope,delx1,dels,exps,sinhs,sinhin,sigmap,delx2;
  a=x[1]-x[0];
  b=x[2]-x[1];
  c=x[2]-x[0];
  slope_0=y[0]*-(a+c)/(a*c)+y[1]*c/(a*b)+y[2]*-a/(b*c);
  a=x[n-2]-x[n-3];
  b=x[n-1]-x[n-2];
  c=x[n-1]-x[n-3];
  slope_n=y[n-3]*-(a+c)/(a*c)+y[n-2]*c/(a*b)+y[n-1]*-a/(b*c);
  sigmap=abs(sigma)*float(n-1)/(x[n-1]-x[0]);
  delx1=x[1]-x[0];
  dels=sigmap*delx1;
  exps=exp(dels);
  sinhs=(exps-1/exps)/2.0;
  sinhin= 1/(delx1*sinhs);
  diag1=sinhin*(dels*(exps+1/exps)/2.0 -sinhs);
  diagin=1/(diag1);
  yp[0]=diagin*((y[1]-y[0])/delx1-slope_0);
  slope=sinhin*(sinhs-dels);
  temp[0]=diagin*slope;
  for(i=1;i<n-2;i++)
    {
      delx2=x[i+1]-x[i];
      dels=sigmap*delx2;
      exps=exp(dels);
      sinhs=(exps-1/exps)/2;
      sinhin= 1/(delx2*sinhs);
      diag2=sinhin*(dels*(exps+1.0/exps)/2.0 -sinhs);
      diagin=1/(diag1+diag2-slope*temp[i-1]);
      yp[i]=diagin*((y[i]-y[i-1])/delx1+(y[i+1]-y[i])/delx2-yp[i-1]*slope_0);
      slope=sinhin*(sinhs-dels);
      temp[i]=diagin*slope;
      delx1=delx2;
      diag1=diag2;
    }
  diagin=1/(diag1-slope*temp[n-2]);
  yp[n-1]=diagin*(slope_n-(y[n-1]-y[n-2])/delx2-slope*yp[n-2]);
  int j=0;
  for(i=2;i<n;i++)
    {
      j=n-i;
      hold=yp[j]-temp[j]*yp[j+1];
      yp[j]=hold;
    }
  return yp;
}

double curv2(int n,int * x,double * y,double sigma,double *yp,int t)
{
  int i;
  double v,exps2,sigmap,s,del1,del2,dels,exps1,sinhd1,sinhd2,sinhd,exps,sinhs;
  for(i=1;i<n;i++)
    {
    if (t>=x[i-1] && t<x[i])
      {
      break;
      }
    }
  s=x[n-1]-x[0];
  sigmap=abs(sigma )*(double)(n-1) /s;
  del1=t-x[i-1];
  del2=x[i]-t;
  dels=x[i]-x[i-1];
  exps1=exp(sigmap*del1);
  sinhd1 = (exps1-1.0/exps1)/2.0;
  exps2=exp(sigmap*del2);
  sinhd2 =(exps2-1.0/exps2)/2.0;
  exps=exps1*exps2;
  sinhs=(exps-1.0/exps)/2.0;
  v=(yp[i]*sinhd1+yp[i-1]*sinhd2)/sinhs + ((y[i]-yp[i])*del1+(y[i-1]-yp[i-1])*del2)/dels;
  // printf("%d\t%lf\t%lf\t%lf\n",i,y[i],y[i-1],v);
  //printf("yp[i]=\t%lf\nyp[i-1=\t%lf\nsinhd1=\t%lf\nsinhd2=\t%lf\nsinhs=\t%lf\t\ndel1=\t%lf\ndel2=\t%lf\ndels=\t%lf\nexps1=\t%lf\nexps2=\t%lf\nexps=\t%lf\ny[i]=\t%lf\ny[i-1]=\t%lf\n",yp[i],yp[i-1],sinhd1,sinhd2,sinhs,del1,del2,dels,exps1,exps2,exps,y[i],y[i-1]);
  return v;
}
