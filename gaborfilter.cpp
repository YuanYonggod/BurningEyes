#include "gaborfilter.h"
#include <QDebug>

Gaborfilter::Gaborfilter()
{

}

Gaborfilter::~Gaborfilter()
{

}

bool Gaborfilter::create_kernel(int r, int theta, int alpha, int beta, double omiga, int ktype)
{
    int xmin,ymin,xmax,ymax;
    //int nstds = 3;

    if( theta > 0 )
        xmax = theta/2;
//    else
//        xmax = cvRound(std::max(fabs(nstds*alpha*c), fabs(nstds*beta*s)));

    if( r > 0 )
        ymax = r/2;
//    else
//        ymax = cvRound(std::max(fabs(nstds*alpha*s), fabs(nstds*beta*c)));

    xmin = -xmax;
    ymin = -ymax;

    CV_Assert( ktype == CV_32F || ktype == CV_64F );

    realKernel = cv::Mat(ymax - ymin + 1, xmax - xmin + 1, ktype);
    imagKernel = cv::Mat(ymax - ymin + 1, xmax - xmin + 1, ktype);

    for( int y = ymin; y <= ymax; y++ )
    {
        for( int x = xmin; x <= xmax; x++ )
        {
            double o = x * M_PI / 180;
            double c = cos(-omiga*o),s = sin(-omiga*o);
            double er = -((pow(y,2))/(pow(alpha,2)));
            double eo = -((pow(x,2))/(pow(beta,2)));

            double rv = std::exp(er + eo)*c;
            double iv = std::exp(er + eo)*s;
            if( ktype == CV_32F ){
                realKernel.at<float>(ymax - y, xmax - x) = (float)rv;
                imagKernel.at<float>(ymax - y, xmax - x) = (float)iv;
            }
            else{
                realKernel.at<double>(ymax - y, xmax - x) = rv;
                imagKernel.at<double>(ymax - y, xmax - x) = iv;
            }
            //std::cout<<xmax-x<<","<<ymax-y<<":"<<std::setw(10)<<rv<<std::setw(10)<<iv;
        }
        //std::cout<<endl;
    }
    isCreatedKernel = true;
    return isCreatedKernel;
}

cv::Mat Gaborfilter::getRealKernel()
{
    return realKernel;
}

cv::Mat Gaborfilter::getImagKernel()
{

    return imagKernel;

}

std::vector<int> Gaborfilter::getIrisCode()
{
    return irisCode;
}

void Gaborfilter::clearIrisCode()
{
    irisCode.clear();
}

double Gaborfilter::filterGabor(cv::Mat &src, cv::Mat &kernel)
{
    double sum = 0;
    for(int p = 0;p < src.rows;p++)
    {
        uchar *data = src.ptr<uchar>(p);
        double *k = kernel.ptr<double>(p);
        for(int q = 0;q < src.cols;q++)
        {
            sum += data[q]*k[q]*p;
        }
        //std::cout<<sum<<endl;
    }
    return sum;
}

void Gaborfilter::gaborCode(cv::Mat &src, cv::Mat &dst)
{
    int nr = src.rows;
    int theta = src.cols;
    for(int i = 0;i < 1024;i++)
    {
        if(create_kernel(nr,theta,80,360,1.0+0.1*i,CV_64F))
        {
            cv::Mat realK = getRealKernel();
            cv::Mat imagK = getImagKernel();
            double real = filterGabor(src,realK);
            double imag = filterGabor(src,imagK);
        //        for(int i = 0;i < realK.rows;i++)
        //        {
        //            double *data = realK.ptr<double>(i);
        //            for(int j = 0;j < realK.cols;j++)
        //                std::cout<<std::setw(10)<<data[j];
        //            std::cout<<endl;
        //        }
            //qDebug()<<"realSum:"<<real<<",imaginarySum:"<<imag<<endl;
            if(real >= 0)
                irisCode.push_back(1);
            else
                irisCode.push_back(0);
            if(imag >= 0)
                irisCode.push_back(1);
            else
                irisCode.push_back(0);

            //qDebug()<<"irisCode size:"<<irisCode.size();
            dst = cv::Mat(32,128,CV_8U,cv::Scalar(127));
            int i = 0;
            for(int u = 0;u < 32;u++)
            {
                for(int v = 0;v < 128;)
                {
                    uchar data = (irisCode[i] == 0) ? 1 : 255;
                    dst.at<uchar>(u,v) = data;
                    dst.at<uchar>(u,v+1) = data;
                    //std::cout<<irisCode[i]<<" ";
                    i++;
                    v += 2;
                }
                //std::cout<<std::endl<<std::flush;
            }
            cv::imwrite("code.jpg",dst);
       }
    }
}
