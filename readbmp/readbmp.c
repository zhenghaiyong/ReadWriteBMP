#include <stdio.h>
#include <stdlib.h>

#define CAST(Class, obj) ((Class)(obj))

int main(int argc, char *argv[])
{
  unsigned char ***image;

  if( argc != 2 )
    {
      fprintf(stderr, "Usage:\t %s [input_bmp_file]\n", argv[0]);
      return 7;
    }

  read_bmp(argv[1], image);
  return 0;
}
