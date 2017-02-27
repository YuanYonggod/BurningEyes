#ifndef NORMLIZE_H
#define NORMLIZE_H

#include<opencv2/core/core.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/highgui/highgui.hpp>

class Normlize
{
public:
    Normlize();
    ~Normlize();

    void normlizeIris(cv::Mat &src,cv::Mat &dst,int px,int py,int pr,int ir);
    void normlizeIris2(cv::Mat &src,cv::Mat &dst,int px,int py,int pr,int ir);

    int nr = 80,theta = 360;
    cv::Mat normMat;
};

#endif // NORMLIZE_H
