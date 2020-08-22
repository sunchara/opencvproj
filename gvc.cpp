#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <INIReader.h>
#include <iostream>
#include <math.h>
#include <algorithm>
#include <string>
static int X,Y;
static bool moved = false;
static int radius;
static int range;

inline bool isInRadius(int x,int y)
{
  return (x-X)*(x-X)+(y-Y)*(y-Y)<=radius*radius;
}
void mouseEventcalback(int event, int y, int x, int flags, void* userdata)
{
    if(event == cv::EVENT_MOUSEMOVE)
    {
        X=x;
        Y=y;
        moved=true;
        //std::cerr<<"Mousemoved  "<<x<<' '<<y<<std::endl;
    }
}
cv::Mat drawBlur(const cv::Mat & frame)//, int x, int y)
{
  int n=0;
  double sum[3]={0,0,0};
  cv::Mat  blur=frame.clone();

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
int main(int argc, char *argv[])
{

    INIReader inireader("gvc.ini");
    radius = inireader.GetInteger("config", "radius", 30);
    range = inireader.GetInteger("config", "range", 2);

    if(argc < 2)
    {
      std::cerr<<"no arguments passed"<<std::endl;
      return -1;
    }

    cv::String src(argv[1]);// = "rtsp://wowzaec2demo.streamlock.net/vod/mp4:BigBuckBunny_115k.mov";
    std::cerr<<"source is "<<src<<std::endl;
    cv::VideoCapture v(src);

    if(!v.isOpened())
    {
        std::cerr<<"failed to open "<<src<<std::endl;
        return -1;
    }
    else
        std::cerr<<"opened "<<src<<std::endl;

    cv::Mat frame;
    cv::namedWindow("vid",cv::WINDOW_KEEPRATIO);
    std::cerr<<"press 'ESC' to quit"<<std::endl;
    cv::setMouseCallback("vid",&mouseEventcalback);
    bool exit = false;
    while(v.read(frame))
    {

        if(moved){
            moved = false;
            frame = drawBlur(frame);
          }
        imshow("vid",frame);
        if(cv::waitKey(1) == 27)
            break;
    }
    v.release();
    cv::destroyAllWindows();
    return 0;
}
