#include "../headers/compare.h"

int comparason(void)
{
  char buffer[256];
  double d,g,x,y,length=0;
  double max;
  fgets(buffer,256,stdin);
  sscanf(buffer,"%lf\t%lf\n",&d,&g);
  max=d;
  x+=pow(d-g,2);
  y+=pow(d,2);
  while(fgets(buffer,256,stdin))
    {
      sscanf(buffer,"%lf\t%lf\n",&d,&g);
      //printf("%s\t%lf\t%lf\n",buffer,d,g);
      x+=pow(d-g,2);
      y+=pow(d,2);
      if(max<d)
	max=g;
      length++;
    }
  printf("%lf\t%lf\n",20*log10(max/sqrt(x/length)),sqrt(x/y));
  return 0;
  
}

