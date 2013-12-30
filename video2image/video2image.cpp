#include <stdio.h>
 
#include <cv.h>
#include <cxcore.h>
#include <highgui.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <cstddef>

int main( int argc, char** argv )
{
  //声明IplImage指针
  IplImage* pFrame = NULL;
  IplImage* img = NULL;

  CvCapture* pCapture = NULL;
 
  int nFrmNum = 0;

  char dir[32], path[256];

  if( argc != 4 )
    {
      fprintf(stderr, "Usage:\t %s [video_file_name] [image_directory_name] [image_format]\n", argv[0]);
      return 7;
    }

  //打开视频文件
  if( !(pCapture = cvCaptureFromFile(argv[1])))
    {
      fprintf(stderr, "Can not open video file %s.\n", argv[1]);
      return 77;
    }

  //判断目录是否存在并创建
  sprintf(dir, "%s", argv[2]);
  if(NULL==opendir(dir))
    mkdir(dir,0775);

  //逐帧读取视频
  while(pFrame = cvQueryFrame( pCapture ))
    {
      nFrmNum++;
      img = cvCreateImage(cvSize(pFrame->width,pFrame->height),pFrame->depth,pFrame->nChannels);
      cvCopyImage(pFrame,img);
      sprintf(path, "%s/%d.%s", dir, nFrmNum, argv[3]);
      cvSaveImage(path, img);
      cvReleaseImage(&img);
    }
  
  cvReleaseCapture(&pCapture);
  return 0;
}
