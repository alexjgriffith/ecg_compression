#include "../headers/fan.h"

int fan_compress(double error)
{
  int j=0;
  double v[3];;
  int output[3];
  double su,sl;
  load_from_stdin(&v[0]);
  load_from_stdin(&v[1]);
  su=v[0]-v[1]+error;
  sl=v[0]-v[1]-error;
  output[0]= modulate(v[0] , 12,-1,1);
  output[1]= j;//modulate((double) t, 8,0,300);
  fprintf(stdout,"%04d%04d",output[0],output[1]);
     
  while(load_from_stdin(&v[2]))
    {
      if(fan_check_value(&su,&sl,v[1],v[2],error))
	{	
	  v[1]=v[2];
	  j++;
	}
      else
        {
	  output[0]= modulate(v[2] , 12,-1,1);
	  output[1]= j+1;//modulate((double) t, 8,0,300);
	  fprintf(stdout,"%04d%04d",output[0],output[1]);
	  v[0]=v[2];
	  v[1]=v[2];
	  load_from_stdin(&v[1]);
	  su=v[0]-v[1]+error;
	  sl=v[0]-v[1]-error;
	  j=0;
	}
      }
  output[0]= modulate(v[2] , 12,-1,1);
  output[1]= j;
  fprintf(stdout,"%04d%04d",output[0],output[1]);
  
  return 0;
}

int fan_decompress()
{
  char c[256];
  char b[256];
  int size=4;
  int i=0;
  int x,y;
  double y_1,y_2;
  double slope;
  fgets(c,size+1,stdin);
  fgets(b,size+1,stdin);
  sscanf(c,"%d",&y);
  y_1=demodulate(y,12,-1,1);
  fprintf(stdout,"%lf\n",y_1);
  while(fgets(c,size+1,stdin) &&fgets(b,size+1,stdin) )
    {
     sscanf(c,"%d",&y);
     sscanf(b,"%d",&x);
     y_2=demodulate(y,12,-1,1);
     slope=(y_2-y_1)/(double)x;
     for(i=0;i<x;i++)
     {
       fprintf(stdout,"%lf\n", slope*(i+1)+y_1);
     }
     fprintf (stdout,"%lf\n",y_2);
     y_1=y_2;
    }
  return 0;
}

int fan_check_value(double * su, double * sl, double point_a,double point_b,double error)
{
  double diff=point_a-point_b;
  double sl_2=diff-error;
  double su_2=diff+error;
  if(diff<* sl || diff >* su)
    return 0;
  else
    {
    if(sl_2>* sl)
      *sl=sl_2;
    if(su_2< *su)
      *su=su_2;
    return 1;
    }
}

