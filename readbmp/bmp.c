#include "bmp.h"

int read_bmp(const char *filename, BYTE ***image)
{
  FILE *file;
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

  if (!(file=fopen(filename,"rb"))) 
    {
      printf("Cannot open file %s.\n", filename);
      return(7);
    }

  fread(&bmfhdr.bfType, sizeof(WORD), 1, file);
  fread(&bmfhdr.bfSize, sizeof(DWORD), 1, file);
  fread(&bmfhdr.bfReserved1, sizeof(WORD), 1, file);
  fread(&bmfhdr.bfReserved2, sizeof(WORD), 1, file);
  fread(&bmfhdr.bfOffBits, sizeof(DWORD), 1, file);
  fread(&bmihdr.biSize, sizeof(DWORD), 1, file);
  fread(&bmihdr.biWidth, sizeof(LONG), 1, file);
  fread(&bmihdr.biHeight, sizeof(LONG), 1, file);
  fread(&bmihdr.biPlanes, sizeof(WORD), 1, file);
  fread(&bmihdr.biBitCount, sizeof(WORD), 1, file);
  fread(&bmihdr.biCompression, sizeof(DWORD), 1, file);
  fread(&bmihdr.biSizeImage, sizeof(DWORD), 1, file);
  fread(&bmihdr.biXPelsPerMeter, sizeof(LONG), 1, file);
  fread(&bmihdr.biYPelsPerMeter, sizeof(LONG), 1, file);
  fread(&bmihdr.biClrUsed, sizeof(DWORD), 1, file);
  fread(&bmihdr.biClrImportant, sizeof(DWORD), 1, file);

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

  printf("+------------------+\n");
  printf("| biBitCount: %d\n", bmihdr.biBitCount);
  if (bmihdr.biBitCount == 1) {
    RGBQUAD aColors[2];
    for (i=0; i<2; i++) {
      fread(&aColors[i].rgbBlue, sizeof(BYTE), 1, file);
      fread(&aColors[i].rgbGreen, sizeof(BYTE), 1, file);
      fread(&aColors[i].rgbRed, sizeof(BYTE), 1, file);
      fread(&aColors[i].rgbReserved, sizeof(BYTE), 1, file);
      printf("| rgbBlue: %d\t rgbGreen: %d\t rgbRed: %d\t rgbReserved: %d\n", aColors[i].rgbBlue, aColors[i].rgbGreen, aColors[i].rgbRed, aColors[i].rgbReserved);
    }
  } else if (bmihdr.biBitCount == 8) {
    RGBQUAD aColors[256];
    for (i=0; i<8; i++) {
      fread(&aColors[i].rgbBlue, sizeof(BYTE), 1, file);
      fread(&aColors[i].rgbGreen, sizeof(BYTE), 1, file);
      fread(&aColors[i].rgbRed, sizeof(BYTE), 1, file);
      fread(&aColors[i].rgbReserved, sizeof(BYTE), 1, file);
      printf("| rgbBlue: %d\t rgbGreen: %d\t rgbRed: %d\t rgbReserved: %d\n", aColors[i].rgbBlue, aColors[i].rgbGreen, aColors[i].rgbRed, aColors[i].rgbReserved);
    }
  } else if (bmihdr.biBitCount == 24) {
    printf(" It's rgb bmp image without palette.\n");
  } else {
    printf("Something wrong with biBitCount of image %s.\n", filename);
    return(77);
  }
  printf("+------------------+\n");

  /* display bmp image attributes. */
  biBitCount = bmihdr.biBitCount;
  if (biBitCount == 1) {
    printf("%s is Binary image.\n", filename);
  } else if (biBitCount == 8) {
    printf("%s is Gray-Level or Indexed image.\n", filename);
  } else if (biBitCount == 24) {
    printf("%s is RGB image.\n", filename);
  } else {
      printf("Something wrong with biBitCount of image %s.\n", filename);
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
    fseek(file, offset, SEEK_CUR);
    for (i=height-1; i>=0; i--) {
      for (j=0; j<width; j++) {
        image[i][j][0] = fgetc(file);
      }
    }
  } else if (bmihdr.biBitCount == 24) {
    for (i=height-1; i>=0; i--) {
      for (j=0; j<width; j++)
        for (k=level-1; k>=0; k--)
          image[i][j][k] = fgetc(file);
      /*
        if(3*width%4 != 0)
        fseek(file, 4-width*3%4, SEEK_CUR);
      */
    }
  } else {
    printf("Something wrong with biBitCount of image %s.\n", filename);
    return(77);
  }
  fclose(file);
  free(image);
  free(x);
  
  return(0);
}

int write_binary_bmp(const char *filename, int width, int height, char *binary)
{
  FILE *file;
  int i, j;
  BITMAPFILEHEADER bmfhdr;
  BITMAPINFOHEADER bmihdr;
  RGBQUAD aColors[2];

  int bytesPerLine;
  BIT *line;

  /* The length of each line must be a multiple of 4 bytes */
  bytesPerLine = (width / 8 + 3) / 4 * 4;

  bmfhdr.bfType = 0x4d42;//(int)'M'*256+'B';//19778;//"BM"
  bmfhdr.bfReserved1 = 0;
  bmfhdr.bfReserved2 = 0;
  bmfhdr.bfOffBits = 54 + 2*4;
  bmfhdr.bfSize = bmfhdr.bfOffBits + bytesPerLine * height;

  bmihdr.biSize = 40;
  bmihdr.biWidth = width;
  bmihdr.biHeight = height;
  bmihdr.biPlanes = 1;
  bmihdr.biBitCount = 1;
  bmihdr.biCompression = 0;
  bmihdr.biSizeImage = bytesPerLine * height;
  bmihdr.biXPelsPerMeter = 0;
  bmihdr.biYPelsPerMeter = 0;
  bmihdr.biClrUsed = 0;
  bmihdr.biClrImportant = 0;

  for (i = 0; i < 2; i++)
    {
      aColors[i].rgbBlue = i;
      aColors[i].rgbGreen = i;
      aColors[i].rgbRed = i;
      aColors[i].rgbReserved = 0;
    }

  if (!(file=fopen(filename,"wb"))) 
    {
      printf("Cannot open file %s.\n", filename);
      return(7);
    }

  fseek(file, 0, SEEK_SET);
  fwrite(&bmfhdr.bfType, sizeof(WORD), 1, file);
  fwrite(&bmfhdr.bfSize, sizeof(DWORD), 1, file);
  fwrite(&bmfhdr.bfReserved1, sizeof(WORD), 1, file);
  fwrite(&bmfhdr.bfReserved2, sizeof(WORD), 1, file);
  fwrite(&bmfhdr.bfOffBits, sizeof(DWORD), 1, file);
  fwrite(&bmihdr.biSize, sizeof(DWORD), 1, file);

  fwrite(&bmihdr.biWidth, sizeof(LONG), 1, file);
  fwrite(&bmihdr.biHeight, sizeof(LONG), 1, file);
  fwrite(&bmihdr.biPlanes, sizeof(WORD), 1, file);
  fwrite(&bmihdr.biBitCount, sizeof(WORD), 1, file);
  fwrite(&bmihdr.biCompression, sizeof(DWORD), 1, file);
  fwrite(&bmihdr.biSizeImage, sizeof(DWORD), 1, file);
  fwrite(&bmihdr.biXPelsPerMeter, sizeof(LONG), 1, file);
  fwrite(&bmihdr.biYPelsPerMeter, sizeof(LONG), 1, file);
  fwrite(&bmihdr.biClrUsed, sizeof(DWORD), 1, file);
  fwrite(&bmihdr.biClrImportant, sizeof(DWORD), 1, file);

  fwrite(aColors, sizeof(RGBQUAD), 2, file);

  line = malloc(bytesPerLine);
  if (line == NULL)
    {
      fprintf(stderr, "Can't allocate memory for BMP file.\n");
      return(77);
    }
  
  for (i = height - 1; i >= 0; i--)
    {
      for (j = 0; j < width; j++)
        {
          //line[j].bit = binary[j+i*height];
          /*
          line[j].a=binary[j++ + i*height];
          printf("%d ",line[j].a);
          line[j].b=binary[j++ + i*height];
          printf("%d ",line[j].b);
          line[j].c=binary[j++ + i*height];
          printf("%d ",line[j].c);
          line[j].d=binary[j++ + i*height];
          printf("%d ",line[j].d);
          line[j].e=binary[j++ + i*height];
          printf("%d ",line[j].e);
          line[j].f=binary[j++ + i*height];
          printf("%d ",line[j].f);
          line[j].g=binary[j++ + i*height];
          printf("%d ",line[j].g);
          line[j].h=binary[j + i*height];
          printf("%d ",line[j].h);
          */
          line[j].a=1;
          line[j].b=1;
          line[j].c=1;
          line[j].d=1;
          line[j].e=1;
          line[j].f=1;
          line[j].g=1;
          line[j].h=1;
        }
      printf("\n");
      fwrite(line, bytesPerLine, 1, file);
    }

  free(line);
  fseek(file,0,SEEK_END);
  WORD bmp_end = 0x0000;
  fwrite(&bmp_end,1,sizeof(WORD),file);
  fclose(file);
  
  return(0);
}
