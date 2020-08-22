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
static int X,Y;
static int radius;
static int range=4;

using namespace cv;

inline bool isInRadius(int x,int y)
{
  return (x-X)*(x-X)+(y-Y)*(y-Y)<=radius*radius;
}
void drawBlur(const cv::Mat & frame)//, int x, int y)
{
  int n=0;
  double sum[3]={0,0,0};
  cv::Mat  blur=frame.clone();

  for(int r=0;r<frame.rows;++r)
    for(int c=0;c<frame.cols;++c){
      if(isInRadius(r,c))
      {
        n=1;
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
    blur.copyTo(frame);
}
int main()
{
    Mat image = imread("1.jpg",
        IMREAD_COLOR );
    if(!image.data){
        printf("bad img");
        return 0;}
    X = image.rows/2;
    Y = image.cols/2;
    radius = min(X,Y)/2;

    namedWindow("test blur");
    drawBlur(image);
    imshow("test blur", image);
    waitKey(0);
    destroyAllWindows();
    return 0;
}
