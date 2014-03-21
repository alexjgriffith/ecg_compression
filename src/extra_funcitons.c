#include <stdio.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>


int file_length(char file[20])
{
  int lines=0;
  int ch;
  FILE *f;
  f = fopen(file,"r");
  while (EOF != (ch=fgetc(f)))
  {
    if (ch=='\n')
        lines++;
    }
  fclose(f);
  return lines;
}

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

double * load_data(char file[20],int size)
{
  char buffer[256];
  int column=1;
  char token[5];
  int i;
  strcpy(token,"\n");
  FILE * read ;

  double *data = (double *) calloc(size+1,sizeof(double));
  read= fopen(file,"r");
  i=0;
  double data_temp;

  while(fgets(buffer,256, read))
  {
    split_token(buffer,token,column);

    sscanf(buffer, "%lf", &data[i]);
    i=i+1;
  }
  fclose(read);
  return data;
}
 int * modulate(double * float_data , int size,double min,double max)
{
  int  * int_data=calloc(size+1,sizeof(int));
  int i;
  int value;
  for(i=0;i<size;i++)
  {
   value=(int)((  float_data[i]-min)/(max-min)*pow(2,12));
   int_data[i]=value;
  }
  return int_data;
}
double * demodulate(int * int_data , int size,double min,double max)
{
  double * float_data=calloc(size+1,sizeof(double));
  int i;
  double value;
  for(i=0;i<size;i++)
  {
   value=(double)( int_data[i]/pow(2,12)*(max-min)+min);
   float_data[i]=value;
  }
  return float_data;
}

int main(int argc,char ** argv)
{
  int size;
  int i;
  size=file_length(argv[1]);
  double * data = calloc(size+1,sizeof(double));
  int * edata= calloc(size+1,sizeof(int));

  data=load_data(argv[1],size);

  for (i=0;i<size;i++)
    printf("%lf\n", data[i]);
  printf("test\n");
  edata=modulate(data , size,-1,1);
  data=demodulate(edata , size,-1,1);
  printf("test\n");
  for (i=0;i<size;i++)
    printf("%d\t%lf\n", edata[i],data[i]);
  return 0;

}