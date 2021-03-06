#include "../headers/common_functions.h"

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
  char c[3];
  strcpy(c,"\n");
  if(fgets(buffer,256,stdin))
    {
    split_token(buffer,c,0);
    sscanf(buffer,"%lf",v);
    return 1;
    }
  else
    return 0;
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

double mean(double * data, int start, int stop)
{
  int i;
  double sum=0;
  for(i=start;i<stop; i++)
    sum=(sum+data[i]);
  return sum/(stop-start);
}

double max(double * data, int start, int stop)
{
  int i;
double  max=data[start];
 for(i=start;i<stop;i++)
   {
     if(data[i]>max)
       max=data[i];
   }
 return max;
}

int max_abs_location(double * data, int start, int stop)
{
  int i;
  int location;
double  max=data[start];
 for(i=start;i<stop;i++)
   {
     if(data[i]>max|| data[i]<-1*max)
       {
	 max=data[i];
	 location=i;
       }
   }
 return location;
}
