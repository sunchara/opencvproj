#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <INIReader.h>
#include <iostream>
#include <math.h>
#include <algorithm>
#include <string>
static int X,Y;// pos of mouse
static bool moved = false;// is moved
static int radius; //radius of the blured circle in pixels
static int range; //kernel is (2*range+1,2*range+1) matrix

inline bool isInRadius(int x,int y)
{
  return (x-X)*(x-X)+(y-Y)*(y-Y)<=radius*radius;
}

//mouse move event handler
void mouseEventcalback(int event, int y, int x, int flags, void* userdata)
{
    if(event == cv::EVENT_MOUSEMOVE)
    {
        X=x;
        Y=y;
        moved=true;
    }
}


void drawBlur(const cv::Mat & frame)
{

  int n=0;
  double sum[3]={0,0,0};
  cv::Mat  blur=frame.clone(); // copy the frame we want to be blured

  for(int r=0;r<frame.rows;++r)
    for(int c=0;c<frame.cols;++c){
      if(isInRadius(r,c))
      {
        n=0; //number of components to sum
        sum[0]=sum[1]=sum[2]=0;// 3 chanels to summ

        //filtering
        for(int i=std::max(r-range, 0);i<=std::min(r+range,frame.rows);++i)
          for(int j=std::max(c-range, 0);j<=std::min(c+range,frame.cols);++j)
          {
            sum[0]+=frame.at<cv::Vec3b>(i,j)[0];
            sum[1]+=frame.at<cv::Vec3b>(i,j)[1];
            sum[2]+=frame.at<cv::Vec3b>(i,j)[2];
            ++n;
          }

        //writing the result in Mat blur
        blur.at<cv::Vec3b>(r,c)[0] = sum[0]/(double)n;
        blur.at<cv::Vec3b>(r,c)[1] = sum[1]/(double)n;
        blur.at<cv::Vec3b>(r,c)[2] = sum[2]/(double)n;

      }
    }
    blur.copyTo(frame);// cope the result back
}
int main(int argc, char *argv[])
{
    cv::String src;
    //read ini file
    INIReader inireader("gvc.ini");
    radius = inireader.GetInteger("config", "radius", 30);
    range = inireader.GetInteger("config", "range", 2);
    std::string _src = inireader.GetString("config", "source_url","");

    // get source url from shel or ini file
    if(argc < 2)
    {
      if(_src==""){
        std::cerr<<"no arguments passed"<<std::endl;
        return -1;
      }
      else
        src=_src;
    }
    else
      src = argv[1];

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
            drawBlur(frame);
          }
        imshow("vid",frame);

        //'ESC' to exit
        if(cv::waitKey(1) == 27)
            break;
    }


    v.release();
    cv::destroyAllWindows();
    return 0;
}
