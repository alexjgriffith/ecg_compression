#include "../headers/fourier.h"


/*int descript(int * data,int length)
{
  double temp;
  //double *x=(double *)calloc(length,sizeof(double));
  test_fft(size,x,1000);
  return 0;
  }*/

int test_fft(int size,double * x, double fs)
{
  long power = determine_power(size);
  int i;
  long n=2;
  for(i=0;i<power;i++)
    n*=2;
  double *x_t=(double *)calloc(n,sizeof(double));
  double *y =(double *)calloc(n,sizeof(double));
  for(i=0;i<size;i++)
    x_t[i]=x[i];
  for(i=size;i<n;i++)
    x_t[i]=x[i-size];
  for(i=0;i<n;i++)
    y[i]=0;

  /*
  //double *x =(double *)calloc(n,sizeof(double));
  double val=3.141592654*2.0/256.0;
  double power_of_two=0;
  for(i=0;i<n;i++)
    {
      y[i]=0;
      x[i]=cos((double)i*val*20);
      //printf("%lf\n",x[i]);
    }
  //printf("size=%lf\n",(double) power);
  */
  //double *x_t=(double *)calloc(100000,sizeof(double));
  //double *y_t=(double *)calloc(100000,sizeof(double));
  fft(1,power+1,x_t,y);
  /*for(i=n/2;i<n;i++)
    {
      x[i]=0;
      y[i]=0;
      }*/
  // printf("power =\t%ld\t%d\t%d\n",power,size,n);
  //fft(-1,power+1,x_t,y);
    //x[0]=0;
    //y[0]=0;
  FILE * four=fopen("fourier.data","w");
  //print("round 1\n");
  for(i=0;i<n/2;i++)
    fprintf(four,"%lf\t%lf\t%lf\n",(double)i*(double)fs/(double) n, x_t[i],y[i]); 
  fclose(four);
  double mag, phase;
  four=fopen("fourier_components.data","a");
  //print("round 1\n");
  for(i=0;i<n/2;i++)
    {
      mag=sqrt(x_t[i]*x_t[i]+y[i]*y[i]);
      phase=tan(y[i]/x_t[i]);
      fprintf(four,"%lf\t%lf\t%lf\n",(double)i*(double)fs/(double) n, mag,phase);
    }		  
  fclose(four);
  return 0;
}

long determine_power(int length)
{
  long m=0;
  while (length>1)
    {
      length*=0.5;
      m++;
    }
  return m;
}

/*
   This computes an in-place complex-to-complex FFT 
   x and y are the real and imaginary arrays of 2^m points.
   dir =  1 gives forward transform
   dir = -1 gives reverse transform 
*/
int fft(int dir,long m,double *x,double *y)
{
  long n,i,i1,j,k,i2,l,l1,l2;
  double c1,c2,tx,ty,t1,t2,u1,u2,z;
  //printf("%ld\n",m); 
  /* Calculate the number of points */
  n = 1;
  for (i=0;i<m;i++) 
    n *= 2;

  /* Do the bit reversal */
  i2 = n >> 1;
  j = 0;
  for (i=0;i<n-1;i++) {
    if (i < j) {
      tx = x[i];
      ty = y[i];
      x[i] = x[j];
      y[i] = y[j];
      x[j] = tx;
      y[j] = ty;
    }
    k = i2;
    while (k <= j) {
      j -= k;
      k >>= 1;
    }
    j += k;
  }

  /* Compute the FFT */
  c1 = -1.0; 
  c2 = 0.0;
  l2 = 1;
  for (l=0;l<m;l++) {
    l1 = l2;
    l2 <<= 1;
    u1 = 1.0; 
    u2 = 0.0;
    for (j=0;j<l1;j++) {
      for (i=j;i<n;i+=l2) {
	i1 = i + l1;
	t1 = u1 * x[i1] - u2 * y[i1];
	t2 = u1 * y[i1] + u2 * x[i1];
	x[i1] = x[i] - t1; 
	y[i1] = y[i] - t2;
	x[i] += t1;
	y[i] += t2;
      }
      z =  u1 * c1 - u2 * c2;
      u2 = u1 * c2 + u2 * c1;
      u1 = z;
    }
    c2 = sqrt((1.0 - c1) / 2.0);
    if (dir == 1) 
      c2 = -c2;
    c1 = sqrt((1.0 + c1) / 2.0);
  }

  /* Scaling for forward transform */
  if (dir == 1) {
    for (i=0;i<n;i++) {
      x[i] /= n;
      y[i] /= n;
      //printf("%lf\t%lf\t%ld\n",x[i],y[i],m); 
    }
  }
   
  return 0;
}
