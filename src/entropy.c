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
  fprintf(stdout,"%04d\n",(int)v[0]);
  load_from_stdin(&v[1]);
  while ( load_from_stdin(&v[2]))
    {
      fprintf(stdout,"%04d\n",(int)((-v[0]-v[2]+2*v[1])));
      v[0]=v[1];
      v[1]=v[2];
    }
  fprintf(stdout,"%04d\n",(int)v[2]);
  return 0;
}

int delta_code()
{
  double v[2];
  load_from_stdin(&v[0]);
  fprintf(stdout,"%04d\n",(int)v[0]);
  while ( load_from_stdin(&v[1]))
    {
      fprintf(stdout,"%04d\n",(int)((-v[0]+v[1])));
      v[0]=v[1];
    }
  return 0;
}

int dpcm_decompress ()
{
  double v[2];
  load_from_stdin(&v[0]);
  fprintf(stdout,"%04d\n", (int)(v[0]));
  while(load_from_stdin(&v[1]))
    {
      v[0]=v[0]+v[1];
      fprintf(stdout,"%04d\n", (int)v[0]);
    }
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
      fprintf(stdout,"%d\t%d\n",(int)point.value,key+depth);
    }
    else
    {
      huffman_aux_function_fixed(*point.child_0,key,depth+1);
      huffman_aux_function_fixed(*point.child_1,key+1,depth+1);
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
      huffman_aux_function_variable(*point.child_0,key,depth+1);
      key[depth]='1';
      huffman_aux_function_variable(*point.child_1,key,depth+1);
    }
}  

int build_huffman_code(int length)
{
  char key[20];
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
	  minimal_length=i+1;
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
  for (k=0;k<minimal_length;k++)
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
      list[temp_length].occurance=list[loc_0].occurance+list[loc_1].occurance;
      list[temp_length].child_0=&list[loc_0];
      list[temp_length].child_1=&list[loc_1];
      list[loc_0].occurance=0;
      list[loc_1].occurance=0;
      temp_length++;
    }
  //int depth=huffman_depth(list[temp_length-1],0);
  huffman_aux_function_variable(list[temp_length-1] ,key,0);
  //printf("depth=%d",depth);
}

int huffman_encode(char file[20])
{
  char keys[300][20];
  double values[300];
  char buffer[256];
  char c[3];
  FILE * read = fopen(file,"r");
  int code_length;
  int i=0;
  double p;
  while(fgets(buffer, 254, read))
    {
      sscanf(buffer,"%lf",&values[i]);
      strcpy(c,"\t");
      split_token(buffer,c,1);
      strcpy(c,"\n");
      split_token(buffer,c,0);
      strcpy(keys[i],buffer) ;
      i++;
    }
  code_length=i;
  while(load_from_stdin(&p))
    for(i=0;i<code_length;i++)
      {
	if (values[i]==(double)p)
	  {
	  fprintf(stdout,"%s",keys[i]);
	  break;
	  }
      }
}

int huffman_decode(char file[20])
{
  //printf("huffman_decode\n");
  char keys[300][20];
  double values[300];
  char buffer[256];
  char c[3];
  char decode_value[256];
  FILE * read = fopen(file,"r");
  int code_length;
  int i=0;
  int j=0;
  double p;
  while(fgets(buffer, 256, read))
    {
      sscanf(buffer,"%lf",&values[i]);
      strcpy(c,"\t");
      split_token(buffer,c,1);
      strcpy(c,"\n");
      split_token(buffer,c,0);
      strcpy(keys[i],buffer) ;
      i++;
    }
  code_length=i;
  while(fgets(c,2,stdin))
    {
      //printf("%s\n",c);
      decode_value[j]=c[0];
      decode_value[j+1]='\0';
      //printf("%s,%d\n",decode_value,code_length);
      for(i=0;i<code_length;i++)
      {
	if (!strcmp(keys[i],decode_value))
	  {
	  fprintf(stdout,"%lf\n",values[i]);
	  j=-1;
	  decode_value[0]='\0';
	  break;
	  }
      }
      if(j>20)
	{
	  printf("failed,%s\n",decode_value);
	  exit(0);
	}
    j++;
    }  
  return 0;
}


