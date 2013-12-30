#include <stdio.h>
#include <stdlib.h>
#define w 300
#define h 300

int main()
{
    FILE *fp;
    struct Bimap_File_Header
    {
        unsigned short bftype; //文件标识
        unsigned long bfsize;  //图位文件大小,以字节为单位
        unsigned short bfreserved1;  //图位文件保留字1
        unsigned short bfreserved2;  //图位文件保留字2
        unsigned long bfoffbits; //文件开始到图位数据的偏移量,以字节为单位
    } bfh;

    bfh.bftype=0x4d42;

    bfh.bfreserved1=0;
    bfh.bfreserved2=0;
    bfh.bfoffbits=0x3e;

    struct Bitmap_File_Info_Header
    {
        unsigned long bfihsize;//图像描述信息块的大少，常为28H
        unsigned long width;   //图像宽度（以像素为单位）
        unsigned long height;  //图像高度（以像素为单位）
        unsigned short biplanes; //图像的plane总数（恒为1）
        unsigned short bibitcount;//记录颜色的位数取值有1（单色），4（16色），8（256色），16（64K色，高彩色），24（16M色，真彩色），32（4096M色，增强型真彩色）
        /*当biBitCount=1时，8个像素占1个字节;
　　      当biBitCount=4时，2个像素占1个字节;
　　      当biBitCount=8时，1个像素占1个字节;
　　      当biBitCount=24时,1个像素占3个字节; */

        unsigned long bicompression;//压缩说明：有0（不压缩），1（RLE 8，8位RLE压缩），2（RLE 4，4位RLE压缩，3（Bitfields，位域存放）
        unsigned long bisizeimage;//图像区数据的大少[数值=（≥位图宽度的最小的4的倍数）×位图高度×每个像素字节数]
        unsigned long bixpelspermeter;//水平每米有多少像素，在设备无关位图中，填写00H
        unsigned long biypelspermeter;//垂直每米有多少像素，在设备无关位图中，填写00H
        unsigned long biclrused;//此图像所用的颜色数，不用，固定为0
        unsigned long biclrimportant;//重要颜色数，不用，为0
    } bfih;

    bfih.bfihsize=0x28;
    bfih.width=w;
    bfih.height=h;
    bfih.biplanes=1;
    bfih.bibitcount=1;
    bfih.bicompression=0;
    bfih.bisizeimage=(w/32*4+(w%32?4:0))*h;
    bfih.bixpelspermeter=0;
    bfih.biypelspermeter=0;
    bfih.biclrused=0;
    bfih.biclrimportant=0;

    bfh.bfsize=0x36+8+bfih.bisizeimage;

    if((fp=fopen("a.bmp","wb+"))==NULL)
    {
        printf("error to open c:\\a.bmp!");
        exit(1);
    }

    fwrite(&bfh.bftype,2,1,fp);
    fwrite(&bfh.bfsize,4,1,fp);
    fwrite(&bfh.bfreserved1,2,1,fp);
    fwrite(&bfh.bfreserved2,2,1,fp);
    fwrite(&bfh.bfoffbits,4,1,fp);

    fwrite(&bfih.bfihsize,4,1,fp);
    fwrite(&bfih.width,4,1,fp);
    fwrite(&bfih.height,4,1,fp);
    fwrite(&bfih.biplanes,2,1,fp);
    fwrite(&bfih.bibitcount,2,1,fp);
    fwrite(&bfih.bicompression,4,1,fp);
    fwrite(&bfih.bisizeimage,4,1,fp);
    fwrite(&bfih.bixpelspermeter,4,1,fp);
    fwrite(&bfih.biypelspermeter,4,1,fp);
    fwrite(&bfih.biclrused,4,1,fp);
    fwrite(&bfih.biclrimportant,4,1,fp);

    struct RGB
    {
        unsigned short Blue;
        unsigned short Green;
        unsigned short Red;
        unsigned short Reserved;
    } rgb0,rgb1;

    rgb0.Blue=0xff;
    rgb0.Green=0xff;
    rgb0.Red=0xff;
    rgb0.Reserved=0;

    rgb1.Blue=0x00;
    rgb1.Green=0x00;
    rgb1.Red=0x00;
    rgb1.Reserved=0;

    fwrite(&rgb0.Blue,1,1,fp);
    fwrite(&rgb0.Green,1,1,fp);
    fwrite(&rgb0.Red,1,1,fp);
    fwrite(&rgb0.Reserved,1,1,fp);
    fwrite(&rgb1.Blue,1,1,fp);
    fwrite(&rgb1.Green,1,1,fp);
    fwrite(&rgb1.Red,1,1,fp);
    fwrite(&rgb1.Reserved,1,1,fp);

    int a[h][w],i,j,k=0,n=0,m=0,c,b[32];


    for(i=0;i<h;i++)
    {
        for(j=0;j<w;j++)
        {
            if(j==i||i+j==w-1)
            {
                a[i][j]=1;
            }
            else
            {
                a[i][j]=0;
            }
        }
    }



    for(i=h-1;i>=0;i--,j=0)
    {
        if(w%32==0)
        {
         for(j=0;j<w;j++)
         {
            k=a[i][j]+k*2;
            m++;
            if(m%8==0)
            {
                fwrite(&k,1,1,fp);
                k=0;
                m=0;
            }

         }
        }

        else
        {

            if(w>=32)
            {
              n=w%32;
              for(j=0;j<w-n;j++)
              {
                k=a[i][j]+k*2;
                m++;
                if(m%8==0)
                {
                    fwrite(&k,1,1,fp);
                    k=0;
                    m=0;
                }
              }


              for(c=0;c<32;c++,j++)
              {
                if(j<w)
                b[c]=a[i][j];
                else
                b[c]=0;
              }


              for(c=0;c<32;c++)
              {
                k=b[c]+k*2;
                m++;
                if(m%8==0)
                {
                    fwrite(&k,1,1,fp);
                    k=0;
                    m=0;
                }
              }

            }
            else
            {
              for(c=0;c<32;c++,j++)
              {
                if(j<w)
                b[c]=a[i][j];
                else
                b[c]=0;
              }


              for(c=0;c<32;c++)
              {
                k=b[c]+k*2;
                m++;
                if(m%8==0)
                {
                    fwrite(&k,1,1,fp);
                    k=0;
                    m=0;
                }
              }
            }


        }
    }







    fclose(fp);






    return 0;
}
