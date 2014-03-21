#include <stdio.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>



void split_token(char trim[256], char del[],  int i)
{
  char *s;
  int j=0;
    s = strtok (trim, del);
    while (s != NULL && j<=i)
  {
    strcpy(trim,s);
    s = strtok (NULL, del);
    j=j+1;
  }
}

int load_from_stdin(double * v)
{
  char buffer[256];
  if(fgets(buffer,256,stdin))
    {
    split_token(buffer,"/n",0);
    sscanf(buffer,"%lf",v);
    return 1;
    }
  else
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
int modulate(double float_data , int size,double min,double max)
{
  int int_data;
      int_data=(int)((  float_data-min)/(max-min)*pow(2,size));    
  return int_data;
}

double demodulate(int int_data , int size,double min,double max)
{
  double float_data;
  float_data=(double)(int_data/pow(2,size)*(max-min)+min);    
  return float_data;
}

int fan_compress(double error)
{
  int i=2;
  double v[3];
  int t;
  int output[3];
  double su,sl;
  load_from_stdin(&v[0]);
  load_from_stdin(&v[1]);
  su=v[0]-v[1]+error;
  sl=v[0]-v[1]-error;
  t=0;

  while(load_from_stdin(&v[2]))
    {
      if(fan_check_value(&su,&sl,v[1],v[2],error))
	v[1]=v[2];
      else
        {
	output[0]= modulate(v[0] , 8,-1,1);
	output[1]= t;//modulate((double) t, 8,0,300);
	//output[2]= modulate((v[1]-v[0])/(i-t) , 12,-1,1);
	fprintf(stdout,"%03d%03d",output[0],output[1]);
	v[0]=v[2];
        v[1]=v[2];
        load_from_stdin(&v[1]);
        su=v[0]-v[1]+error;
        sl=v[0]-v[1]-error;
	t=i-t;
	}

	i++; 
	}
  return 0;
  
}

int fan_decompress()
{
  char c[256];
  char b[256];
  int size=3;
  int i=0;
  int x,y;
  double y_1,y_2;
  double slope;
  fgets(c,size+1,stdin);
  fgets(b,size+1,stdin);
  sscanf(c,"%d",&y);
  y_1=demodulate(y,8,-1,1);
  fprintf(stdout,"%lf\n",y_1);
  while(fgets(c,size+1,stdin) &&fgets(b,size+1,stdin) )
    {
     sscanf(c,"%d",&y);
     sscanf(b,"%d",&x);
     y_2=demodulate(y,8,-1,1);
     slope=(y_2-y_1)/(double)x;
     for(i=1;i<x;i++)
     {
       fprintf(stdout,"%lf\n", slope*i+y_1);
     }
     fprintf (stdout,"%lf\n",y_2);
     y_1=y_2;
    }
  return 0;
}

int main(int argc,char ** argv)
{
  double error;
  if (argc!=2)
  {
    printf("enter single argument for error\n");
    exit(0);
  }
  sscanf(argv[1],"%lf",&error);
  if (error<0)
  {
    printf("imporoper error value\n");
    exit(0);
  }  
  fan_compress(error);
  //fan_decompress();
  return 0;
}

