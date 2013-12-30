/*
 *  bmp2raw.c - convert .bmp file to .raw file compiled by gcc.
 *
 *  Copyright (C) 2007  Haiyong Zheng <zhyfly.org@gmail.com>
 *                                     http://www.zhyfly.org
 */

#include <stdio.h>
#include <stdlib.h>

typedef unsigned short int WORD;
typedef unsigned long int DWORD;
typedef signed long int LONG;
typedef unsigned char BYTE;
typedef long int UINT;
typedef struct tagBITMAPFILEHEADER{
  WORD   bfType;           /* "BM" */
  DWORD  bfSize;           /* Size of file in bytes */
  WORD   bfReserved1;      /* set to 0 */
  WORD   bfReserved2;      /* set to 0 */
  DWORD  bfOffBits;        /* Byte offset to actual bitmap data (= 54 if RGB) */
}BITMAPFILEHEADER;
typedef struct tagBITMAPINFOHEADER{
  DWORD  biSize;           /* Size of BITMAPINFOHEADER, in bytes (= 40) */   
  LONG   biWidth;          /* Width of image, in pixels */
  LONG   biHeight;         /* Height of images, in pixels */
  WORD   biPlanes;         /* Number of planes in target device (set to 1) */
  WORD   biBitCount;       /* Bits per pixel (24 in this case) */  
  DWORD  biCompression;    /* Type of compression (0 if no compression) */
  DWORD  biSizeImage;      /* Image size, in bytes (0 if no compression) */
  LONG   biXPelsPerMeter;  /* Resolution in pixels/meter of display device */
  LONG   biYPelsPerMeter;  /* Resolution in pixels/meter of display device */
  DWORD  biClrUsed;        /* Number of colors in the color table (if 0, use 
                              maximum allowed by biBitCount) */
  DWORD  biClrImportant;   /* Number of important colors.  If 0, all colors 
                              are important */
}BITMAPINFOHEADER;
/*
 * If it's not RGB image, it has an extra color palette structure.
 * (1024 bytes normally)
 */
/*
  typedef struct tagRGBQUAD{
  BYTE   rgbBlue;
  BYTE   rgbGreen;
  BYTE   rgbRed;
  BYTE   rgbReserved;
  }RGBQUAD;
*/

int main(int argc, char *argv[])
{
  FILE *input, *output;
  BYTE ***image;
  int width, height, level;
  int i, j, k;
  int biBitCount, offset;
  int *x;
  BITMAPFILEHEADER bmfhdr;
  BITMAPINFOHEADER bmihdr;
  /*
    RGBQUAD aColors[];
    BYTE aBitmapBits[];
  */

  if (argc < 2) {
    printf("Missing Arguments.\n");
    printf("Usage:\n");
    printf("\t\t%s <input.bmp>\n", argv[0]);
    printf("\t\t%s <input.bmp> <output.raw>\n", argv[0]);
    exit(0);
  } else if (argc > 3) {
    printf("Redundant Arguments.\n");
    printf("Usage:\n");
    printf("\t\t%s <input.bmp>\n", argv[0]);
    printf("\t\t%s <input.bmp> <output.raw>\n", argv[0]);
    exit(0);
  } else {
    if (!(input=fopen(argv[1],"rb"))) {
      printf("Cannot open file %s.\n", argv[1]);
      exit(7);
    }
    
    fread(&bmfhdr.bfType, sizeof(WORD), 1, input);
    fread(&bmfhdr.bfSize, sizeof(DWORD), 1, input);
    fread(&bmfhdr.bfReserved1, sizeof(WORD), 1, input);
    fread(&bmfhdr.bfReserved2, sizeof(WORD), 1, input);
    fread(&bmfhdr.bfOffBits, sizeof(DWORD), 1, input);
    fread(&bmihdr.biSize, sizeof(DWORD), 1, input);
    fread(&bmihdr.biWidth, sizeof(LONG), 1, input);
    fread(&bmihdr.biHeight, sizeof(LONG), 1, input);
    fread(&bmihdr.biPlanes, sizeof(WORD), 1, input);
    fread(&bmihdr.biBitCount, sizeof(WORD), 1, input);
    fread(&bmihdr.biCompression, sizeof(DWORD), 1, input);
    fread(&bmihdr.biSizeImage, sizeof(DWORD), 1, input);
    fread(&bmihdr.biXPelsPerMeter, sizeof(LONG), 1, input);
    fread(&bmihdr.biYPelsPerMeter, sizeof(LONG), 1, input);
    fread(&bmihdr.biClrUsed, sizeof(DWORD), 1, input);
    fread(&bmihdr.biClrImportant, sizeof(DWORD), 1, input);

    printf("+------------------+\n");    
    printf("| bfType: %d\n", bmfhdr.bfType);
    printf("| bfSize: %ld\n", bmfhdr.bfSize);
    printf("| bfReserved1: %d\n", bmfhdr.bfReserved1);
    printf("| bfReserved2: %d\n", bmfhdr.bfReserved2);
    printf("| bfOffBits: %ld\n", bmfhdr.bfOffBits);
    printf("| biSize: %ld\n", bmihdr.biSize);
    printf("| biWidth: %ld\n", bmihdr.biWidth);
    printf("| biHeight: %ld\n", bmihdr.biHeight);
    printf("| biPlanes: %d\n", bmihdr.biPlanes);
    printf("| biBitCount: %d\n", bmihdr.biBitCount);
    printf("| biCompression: %ld\n", bmihdr.biCompression);
    printf("| biSizeImage: %ld\n", bmihdr.biSizeImage);
    printf("| biXPelsPerMeter: %ld\n", bmihdr.biXPelsPerMeter);
    printf("| biYPelsPerMeter: %ld\n", bmihdr.biYPelsPerMeter);
    printf("| biClrUsed: %ld\n", bmihdr.biClrUsed);
    printf("| biClrImportant: %ld\n", bmihdr.biClrImportant);
    printf("+------------------+\n");
    
    /* display bmp image attributes only. */
    if (argc == 2) {
      biBitCount = bmihdr.biBitCount;
      if (biBitCount == 1) {
	printf("%s is Binary image.\n", argv[1]);
      } else if (biBitCount == 8) {
	printf("%s is Gray-Level or Indexed image.\n", argv[1]);
      } else if (biBitCount == 24) {
	printf("%s is RGB image.\n", argv[1]);
      } else {
	printf("Something wrong with biBitCount of image %s.\n", argv[1]);
      }
      exit(0);
    }
    
    /* width and height of bmp image. */
    width = bmihdr.biWidth;
    height = bmihdr.biHeight;
    level = 3; /* for RGB only */
    
    /* malloc for 3-D array ***image and 1-D array *x. */
    image = (BYTE ***)malloc(height*sizeof(BYTE **));
    for (i=0; i<height; i++)
      image[i] = (BYTE **)malloc(width*sizeof(BYTE *));
    for(i=0; i<height; i++)
      for (j=0; j<width; j++)
	image[i][j] = (BYTE *)malloc(level*sizeof(BYTE));
    x = (int *)malloc(level*sizeof(int));
    
    /* initialize 3-D array ***image and 1-D array *x. */
    for (i=0; i<height; i++)
      for (j=0; j<width; j++)
	for (k=0; k<level; k++)
	  image[i][j][k] = 0;
    for (k=0; k<level; k++)
      x[k] = 0;
    
    /* read pixel from file to array image. */
    /*
      bmihdr.biBitCount = 1  -> Binary image.
      bmihdr.biBitCount = 8  -> Gray-Level or Indexed image.
      bmihdr.biBitCount = 24 -> RGB image.
    */
    if ((bmihdr.biBitCount == 1) || (bmihdr.biBitCount == 8)) {
      offset = bmfhdr.bfOffBits - 54;
      fseek(input, offset, SEEK_CUR);
      for (i=height-1; i>=0; i--) {
	for (j=0; j<width; j++) {
	  image[i][j][0] = fgetc(input);
	}
      }
      fclose(input);
      output = fopen(argv[2], "wb");
      for (i=0; i<height; i++)
	for (j=0; j<width; j++)
	  fputc(image[i][j][0], output);
      printf("It's Binary, Gray-Level or Indexed image.\n");
      printf("%s is converted to %s successfully.\n", argv[1], argv[2]);
    } else if (bmihdr.biBitCount == 24) {
      for (i=height-1; i>=0; i--) {
	for (j=0; j<width; j++)
	  for (k=level-1; k>=0; k--)
	    image[i][j][k] = fgetc(input);
	/*
	  if(3*width%4 != 0)
	  fseek(input, 4-width*3%4, SEEK_CUR);
	*/
      }
      fclose(input);
      output = fopen(argv[2], "wb");
      for (i=0; i<height; i++)
	for (j=0; j<width; j++) {
	  for (k=0; k<level; k++)
	    x[k] = image[i][j][k];
	  image[i][j][0] = 0.11*x[0]+0.59*x[1]+0.30*x[2];
	  fputc(image[i][j][0], output);
	}
      printf("It's RGB image.\n");
      printf("%s is converted to %s successfully.\n", argv[1], argv[2]);
    } else {
      printf("Something wrong with biBitCount of image %s.\n", argv[1]);
      exit(0);
    }
    fclose(output);
    free(image);
    free(x);
    
    exit(0);
  }
}
