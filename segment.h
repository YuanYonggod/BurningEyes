#ifndef SEGMENT_H
#define SEGMENT_H

#include<opencv2/core/core.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/highgui/highgui.hpp>

class Segment
{
public:
    Segment();
    ~Segment();

    void findPupilEdge(cv::Mat &scr,cv::Mat &dst);
    void findIrisEdge(cv::Mat &img,cv::Mat &dst);
    int calCircleSum(cv::Mat &img,int x,int y,int r);
    void drawCircle(cv::Mat &img,int x,int y,int r);

    int pupil_x,pupil_y,pupil_r,iris_r;
    cv::Mat gaussMat;
};

struct t_attr
{
    int i0,i1;
    int j0,j1;
    int r0,r1;
    Segment seg;
};

struct r_attr
{
    int x,y,r;
    int max;
};

#endif // SEGMENT_H
