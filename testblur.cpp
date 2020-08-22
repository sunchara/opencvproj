#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/imgcodecs.hpp>


#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <math.h>
#include <algorithm>
using namespace cv;
static int X=200,Y=200;
static bool moved = false;
static int radius=100;
static int range=2;

inline bool isInRadius(int x,int y)
{
  return (x-X)*(x-X)+(y-Y)*(y-Y)<=radius*radius;
}
cv::Mat drawBlur(const cv::Mat & frame)//, int x, int y)
{
  int n=0;
  double sum[3]={0,0,0};
  cv::Mat  blur(frame);

  for(int r=0;r<frame.rows;++r)
    for(int c=0;c<frame.cols;++c){
      if(isInRadius(r,c))
      {
        n=0;
        sum[0]=sum[1]=sum[2]=0;
        for(int i=std::max(r-range, 0);i<=std::min(r+range,frame.rows);++i)
          for(int j=std::max(c-range, 0);j<=std::min(c+range,frame.cols);++j)
          {
            sum[0]+=frame.at<cv::Vec3b>(i,j)[0];
            sum[1]+=frame.at<cv::Vec3b>(i,j)[1];
            sum[2]+=frame.at<cv::Vec3b>(i,j)[2];
            ++n;
          }

          blur.at<cv::Vec3b>(r,c)[0] = sum[0]/(double)n;
          blur.at<cv::Vec3b>(r,c)[1] = sum[1]/(double)n;
          blur.at<cv::Vec3b>(r,c)[2] = sum[2]/(double)n;

      }
    }
    return blur;
}
int main(int argc, char** argv )
{
    Mat image = imread("1.jpg",
        IMREAD_COLOR );
    if(!image.data){
        printf("bad img");
        return 0;}
    namedWindow("test");
    /*for(int r=0;r<image.rows;++r)
      for(int c=0;c<image.cols;++c){
        image.at<Vec3b>(r,c)[0]*=2;
        image.at<Vec3b>(r,c)[1]*=2;
    }*/
    Mat blur=drawBlur(image);
    imshow("test", blur);
    waitKey(0);
    return 0;
}
