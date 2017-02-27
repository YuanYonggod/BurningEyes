#include "normlize.h"

Normlize::Normlize()
{

}

Normlize::~Normlize()
{

}

void Normlize::normlizeIris(cv::Mat &src,cv::Mat &dst,int px,int py,int pr,int ir)
{
    int iw = ir - pr;
    dst = cv::Mat(nr,theta,CV_8U,cv::Scalar(255));
    normMat = cv::Mat(nr,theta,CV_8U,cv::Scalar(255));
    for(int i = 0;i < theta;i++)
    {
        double alpha = 2 * M_PI * i / theta;
        for(int j = 0;j < nr;j++)
        {
            double r = pr + iw*(j+1) / nr;
            int x = (int) px + r * cos(alpha);
            int y = (int) py + r * sin(alpha);
            normMat.at<uchar>(j,i) = src.at<uchar>(y,x);
            dst.at<uchar>(j,i) = src.at<uchar>(y,x);
            //dst.at<uchar>(y,x) = 255;
            //qDebug()<<"x:"<<x<<",y:"<<y<<",r:"<<r<<",alpha:"<<alpha<<endl;
        }
    }
}

void Normlize::normlizeIris2(cv::Mat &src,cv::Mat &dst,int px,int py,int pr,int ir)
{
    dst = cv::Mat(nr,theta,CV_8U,cv::Scalar(255));
    normMat = cv::Mat(nr,theta,CV_8U,cv::Scalar(255));
    for(int i = 0;i < 360;i++)
    {
        double alpha = 2 * M_PI * i / theta;
        for(int j = 0;j < nr;j++)
        {
            double r = 1.0*j / nr;
            int x = (int)((1-r)*(px+pr*cos(alpha)) + r*(px+ir*cos(alpha)));
            int y = (int)((1-r)*(py+pr*sin(alpha)) + r*(py+ir*sin(alpha)));
            //qDebug()<<"x:"<<x<<",y:"<<y<<",r:"<<r<<",alpha:"<<alpha<<endl;
            normMat.at<uchar>(j,i) = src.at<uchar>(y,x);
            dst.at<uchar>(j,i) = src.at<uchar>(y,x);
            //grayMat.at<uchar>(y,x) = 255;
        }
    }
}
