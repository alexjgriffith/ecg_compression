#include "../headers/entropy.h"

typedef struct huffman_code huffman_code;
struct huffman_code
{
  double value;
  int occurance;
  huffman_code *child_0;
  huffman_code *child_1;
};

int dpcm_compress_2nd ()
{
  double v[3];
  double baseline;
  load_from_stdin(&v[0]);
  baseline=v[0];
  fprintf(stdout,"%04d",(int)v[0]);
  //load_from_stdin(&v[1]);
  while ( load_from_stdin(&v[2]))
    {
      fprintf(stdout,"%04d\n",(int)((v[0]+v[2]-2*v[1])));
    }
  //fprintf(stdout,"%04d",(int)v[3]);
  return 0;
}

int dcpm_decompress ()
{
  double baseline;
  double var;
  double temp;
  load_from_stdin(&baseline);
  load_from_stdin(&temp);
  fprintf(stdout,"%04d\n", (int)(baseline));
  while(load_from_stdin(&var))
    {
      fprintf(stdout,"%04d\n", (int)(temp+baseline));
      temp=var;
    }
  fprintf(stdout,"%04d\n", (int)(temp));
}

int huffman_depth(huffman_code point,int depth)
{
  int d[2];
  if ( point.child_0==NULL)
    {
      return depth;
    }
    else
    {
      d[0]=huffman_depth(*point.child_0,depth+1);
      d[1]=huffman_depth(*point.child_1,depth+1);
    }
  if (d[0]>d[1])
    return d[0];
  else
    return d[1];
}  

void huffman_aux_function_fixed(huffman_code point,int key,int depth)
{
  if ( point.child_0==NULL)
    {
      key[depth]='\0';
      fprintf(stdout,"%d\t%d\n",(int)point.value,key+depth);
    }
    else
    {
      huffman_aux_function(*point.child_0,key,depth+1);
      huffman_aux_function(*point.child_1,key+1,depth+1);
    }
}  


void huffman_aux_function_variable(huffman_code point,char key[12],int depth)
{
  if ( point.child_0==NULL)
    {
      key[depth]='\0';
      fprintf(stdout,"%d\t%s\n",(int)point.value,key);
    }
    else
    {
      key[depth]='0';
      huffman_aux_function(*point.child_0,key,depth+1);
      key[depth]='1';
      huffman_aux_function(*point.child_1,key,depth+1);
    }
}  

int build_huffman_code(int length)
{
  char key[12];
  int val_0;
  int val_1;
  int i;
  int k;
  int loc_0;
  int loc_1;
  double  p;
  int data_length;
  int minimal_length=0;
  int temp_length;
  huffman_code *list=(huffman_code *)calloc(length*2-1,sizeof(huffman_code));
  huffman_code new_node;
  for(i=0;i<length;i++)
    {
      list[i].occurance=0;
    }
  while(load_from_stdin(&p))
    {
      i=0;
      k=0;
      while(list[i].occurance>0)
	{
	  if(p==list[i].value)
	    {
	      list[i].occurance++;
              k=1;
	      break;
	    }
	  i++;
	}
      if(i>length)
	{
	  printf("The code length is too small");
	  exit(0);
	}
      if(i>minimal_length)
	{
	  minimal_length=i;
	}
      if(k==0)
	{ 
	  //printf("%d,%lf\n",list[i].occurance,list[i].value);
	  list[i].value=p;
	  list[i].occurance++;
	}
      data_length++;
      //printf("%lf\t%d\n",p,i);
    }
  temp_length=minimal_length;
  //for( i=0; i<temp_length+5;i++)
  //printf("tree built, occurance =%d\n",list[i].occurance);
  for (k=0;k<minimal_length-1;k++)
    {
      loc_0=0;
      loc_1=0;
      val_0=data_length+1;
      val_1=data_length+1;
      i=0;
      while(i<temp_length)
	{
          if(list[i].occurance==0)
	    {
	      i++;
	      continue;
	    }
	  if (list[i].occurance <val_1)
	    {
	      if(list[i].occurance <val_0)
		{	
		  val_0=list[i].occurance;
		  loc_0=i;
		}
	      else
		{
		  val_1=list[i].occurance;
		  loc_1=i;
		}	    
	    }
	  i++;
	}
      //printf("%d\t%d\n",loc_0,loc_1);
      list[temp_length].occurance=list[loc_0].occurance+list[loc_1].occurance;
      list[temp_length].child_0=&list[loc_0];
      list[temp_length].child_1=&list[loc_1];
      list[loc_0].occurance=0;
      list[loc_1].occurance=0;
      temp_length++;
    }
  int depth=huffman_depth(list[temp_length-1],0);
  huffman_aux_function(list[temp_length-1] ,key,0);
  printf("depth=%d",depth);
}

