//#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#define UNIT long int
#define DWORD unsigned long int
#define LONG signed long int
#define WORD unsigned short int
#define TYPE unsigned char
#define thresh 128
//#define yuantu test2.bmp
//#define rawtu  test2gray.raw
//#define rawtuerzhi test2gray1.raw 
main()
{
	
	typedef int hell;
	typedef struct tagFile{
		UNIT bftype;
		DWORD bfsize;
		UNIT bfreserved1;
		UNIT bfreserved2;
		DWORD bfoffbits;
		} BITMAPFILEHEADER;
	typedef struct tagMap{
		DWORD bisize;
		LONG biwidth;
		LONG biheight;
		WORD biplanes;
		WORD bibitcount;
		DWORD bicompression;
		DWORD bisizeimage;
		LONG bixpelspermeter;
		LONG biypelspermeter;
		DWORD biclrused;
		DWORD biclrimportant;
		} BITMAPINFOHEADER;
	
	BITMAPFILEHEADER exam1={1,2,3,4,5};
	BITMAPINFOHEADER exam2={6,7,8,9,10,11,12,13,14,15,16};
	BITMAPFILEHEADER bmfhdr;
	BITMAPINFOHEADER bmihdr;
	hell i;
	//hello k;
	for(i=0;i<10;i++)
	printf("Hello World the %d times\n",i);	
	//k.j=100;
	printf("the last is %u\n",exam1.bfreserved1);
	printf("the rist is %ld\n",exam2.bixpelspermeter);
	TYPE ***yy;
	FILE *fp,*fp1,*fp2;
	int k;
	LONG height=1,width=2;//图象的高宽
	LONG high=3,wide=4;//xiangsu
	//printf("the height is %ld\n",height);
	//printf("the width is %ld\n",width);
	int x[3];
	if(!(fp=fopen("Lena256.bmp","rb")))
	{
		printf("cannot open file \n");
	}	
	else printf("OK\n");
	fread(&bmfhdr.bftype,sizeof(WORD),1,fp);
	printf("the length %ld\n",bmfhdr.bftype);
	fread(&bmfhdr.bfsize,sizeof(DWORD),1,fp);
	fread(&bmfhdr.bfreserved1,sizeof(WORD),1,fp);
	fread(&bmfhdr.bfreserved2,sizeof(WORD),1,fp);
	fread(&bmfhdr.bfoffbits,sizeof(DWORD),1,fp);
	printf("\n%u\n",bmfhdr.bfoffbits);
	fread(&bmihdr.bisize,sizeof(DWORD),1,fp);
	fread(&bmihdr.biwidth,sizeof(LONG),1,fp);
	fread(&bmihdr.biheight,sizeof(LONG),1,fp);
	fread(&bmihdr.biplanes,sizeof(WORD),1,fp);
	fread(&bmihdr.bibitcount,sizeof(WORD),1,fp);
	fread(&bmihdr.bicompression,sizeof(DWORD),1,fp);
	fread(&bmihdr.bisizeimage,sizeof(DWORD),1,fp);
	fread(&bmihdr.bixpelspermeter,sizeof(LONG),1,fp);
	fread(&bmihdr.biypelspermeter,sizeof(LONG),1,fp);
	fread(&bmihdr.biclrused,sizeof(DWORD),1,fp);
	fread(&bmihdr.biclrimportant,sizeof(DWORD),1,fp);
	
	width=bmihdr.biwidth;
	height=bmihdr.biheight;
	printf("%ld   %ld\n",width,height);
	printf("the xiangsuweishu is %u\n",bmihdr.bibitcount);	
	yy=(TYPE ***)malloc(height*sizeof(TYPE **));
	for(high=0;high<height;high++)
	{
		yy[high]=(TYPE **)malloc(width*sizeof(TYPE *));
	}
	
	for(high=0;high<height;high++)
	{
		for(wide=0;wide<width;wide++)
		{
			yy[high][wide]=(TYPE *)malloc(3*sizeof(TYPE));
		}
	}
	
	printf("\n");
	
	for(high=0;high<height;high++)
	{
		for(wide=0;wide<width;wide++)
		{
			for(k=0;k<3;k++)
			{
				yy[high][wide][k]=0;
			}
		}
	}	
printf("yy is %d\n\n",yy[2][2][1]);

	for(high=height-1;high>=0;high--)
	{
		for(wide=0;wide<width;wide++)
		{
			for(k=2;k>=0;k--)
			{
				yy[high][wide][k]=fgetc(fp);
			}
		}
		if(3*width%4!=0)
			fseek(fp,4-width*3%4,SEEK_CUR);
	}
printf("yy is %d\n\n",yy[2][2][1]);
	
	fclose(fp);

	fp1=fopen("rawtu2.raw","wb");
	fp2=fopen("rawtuerzhi2.raw","wb");	
	for(high=0;high<height;high++)
	{
		for(wide=0;wide<width;wide++)
		{
			for(k=0;k<3;k++)
			{
				x[k]=yy[high][wide][k];
			}
			yy[high][wide][0]=0.11*x[0]+0.59*x[1]+0.30*x[2];
			fputc(yy[high][wide][0],fp1);
			
			if(yy[high][wide][0]>thresh)
				yy[high][wide][0]=255;
			else yy[high][wide][0]=0;
			fputc(yy[high][wide][0],fp2);
		}
	}
	
	fclose(fp1);
	
	free(yy);

}
