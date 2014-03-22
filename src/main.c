#include <stdio.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <getopt.h>
#include "../headers/common_functions.h"
#include "../headers/fan.h"
#include "../headers/entropy.h"
int main(int argc,char ** argv)
{
  int ch;
  double error;
  char mode[20];
  int compress=0;

  static struct option long_options[] =
    {
      {"mode", required_argument, NULL, 'm'},
      {"error", optional_argument, NULL, 'e'},
      {"decompress",no_argument,NULL,'d'},
      {NULL, 0, NULL, 0}
    };

  while ((ch = getopt_long(argc, argv, "dm:e:", long_options, NULL)) != -1)
    {
      switch (ch)
	{
	case 'm':
	  strcpy(mode, optarg); 
	  break;
	case 'e':
	  {
	  sscanf(optarg,"%lf",&error);
          if (error<0)
          {
           printf("imporoper error value\n");
           exit(0);
           }   
	  break;
	  }
        case 'd':
          compress=1;
	}
    }

  if(!strcmp(mode,"fan"))
    {
    if(compress==0)
      {
	if(&error==NULL)
	  {
            printf("requires -e or --error value\n");
	    exit(0);
          }
      fan_compress(error);
      }
    if(compress==1)
      fan_decompress();
    }

  if (!strcmp(mode,"dpcm"))
    {
      dpcm_compress_2nd();
    }

  if (!strcmp(mode,"make-huffman"))
    {
      printf("huffman chossen\n");
      build_huffman_code(200);
    }

  return 0;
}

