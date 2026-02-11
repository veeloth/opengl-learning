#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[argc+1])
  {
  if (argc != 2)
    {
    printf("usage: %s <name of output binary>\n", argv[0]);
    return 1;
    }
  char command[512];
  sprintf(command, "clang %s.c -o build/%s -lm -lglfw -lGL", argv[1], argv[1]);
  if(system(command))
    {
    printf("error executing\n");
    return 1;
    }
  return 0;
  }
