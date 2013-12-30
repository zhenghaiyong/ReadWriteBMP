#include <stdio.h>
#include <stdlib.h>

typedef unsigned short int WORD;
typedef unsigned long int DWORD;
typedef signed long int LONG;
typedef unsigned char BYTE;
typedef long int UINT;
#pragma pack (1) /*指定按1字节对齐*/
typedef struct bit{
  unsigned a: 1;        //定义一个变量bit，长度为1位。
  unsigned b: 1;        //定义一个变量bit，长度为1位。
  unsigned c: 1;        //定义一个变量bit，长度为1位。
  unsigned d: 1;        //定义一个变量bit，长度为1位。
  unsigned e: 1;        //定义一个变量bit，长度为1位。
  unsigned f: 1;        //定义一个变量bit，长度为1位。
  unsigned g: 1;        //定义一个变量bit，长度为1位。
  unsigned h: 1;        //定义一个变量bit，长度为1位。
}BIT;
#pragma pack () /*取消指定对齐，恢复缺省对齐*/
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
typedef struct tagRGBQUAD{
  BYTE   rgbBlue;
  BYTE   rgbGreen;
  BYTE   rgbRed;
  BYTE   rgbReserved;
}RGBQUAD;

int read_bmp(const char *filename, BYTE ***image);
int write_binary_bmp(const char *filename, int width, int height, char *binary);
int write_graylevel_bmp(const char *filename, int width, int height, char *graylevel);
int write_rgb_bmp(const char *filename, int width, int height, char *rgb);
