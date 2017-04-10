#include "gaborfilter.h"
#include <QDebug>
#include <fstream>
#include <iostream>

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

std::vector<char> Gaborfilter::getIrisCode()
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
    //提取局部特征
    cv::Mat subMat1(src,cv::Rect(0,0,theta*1/6,nr*1/2));
    //cv::imwrite("sub1.jpg",subMat1);
    cv::Mat subMat2(src,cv::Rect(theta*1/6,0,theta*1/6,nr*1/2));
    //cv::imwrite("sub2.jpg",subMat2);
    cv::Mat subMat3(src,cv::Rect(theta*2/6,0,theta*1/6,nr*1/2));
    //cv::imwrite("sub3.jpg",subMat3);
    cv::Mat subMat4(src,cv::Rect(theta*3/6,0,theta*1/6,nr*1/2));
    //cv::imwrite("sub4.jpg",subMat4);
    cv::Mat subMat5(src,cv::Rect(theta*4/6,0,theta*1/6,nr*1/2));
    //cv::imwrite("sub5.jpg",subMat5);
    cv::Mat subMat6(src,cv::Rect(theta*5/6,0,theta*1/6,nr*1/2));
    //cv::imwrite("sub6.jpg",subMat6);
    cv::Mat subMat7(src,cv::Rect(0,nr*1/2,theta*1/6,nr*1/2));
    //cv::imwrite("sub7.jpg",subMat7);
    cv::Mat subMat8(src,cv::Rect(theta*1/6,nr*1/2,theta*1/6,nr*1/2));
    //cv::imwrite("sub8.jpg",subMat8);
    cv::Mat subMat9(src,cv::Rect(theta*2/6,nr*1/2,theta*1/6,nr*1/2));
    //cv::imwrite("sub9.jpg",subMat9);
    cv::Mat subMat10(src,cv::Rect(theta*3/6,nr*1/2,theta*1/6,nr*1/2));
    //cv::imwrite("sub10.jpg",subMat10);
    cv::Mat subMat11(src,cv::Rect(theta*4/6,nr*1/2,theta*1/6,nr*1/2));
    //cv::imwrite("sub11.jpg",subMat11);
    cv::Mat subMat12(src,cv::Rect(theta*5/6,nr*1/2,theta*1/6,nr*1/2));
    //cv::imwrite("sub12.jpg",subMat12);

    std::ofstream out("suitable_w.txt");
    for(int i = 0;i < 100;i++)
    {
        double maxw = 0;
        if(create_kernel(40,60,40,60,0.1+0.1*i,CV_64F))
        {
            cv::Mat realK = getRealKernel();
            cv::Mat imagK = getImagKernel();
            for(int j = 1;j < 13;j++)
            {
                cv::Mat subMat;
                switch(j)
                {
                case 1:subMat = subMat1;
                    break;
                case 2:subMat = subMat2;
                    break;
                case 3:subMat = subMat3;
                    break;
                case 4:subMat = subMat4;
                    break;
                case 5:subMat = subMat5;
                    break;
                case 6:subMat = subMat6;
                    break;
                case 7:subMat = subMat7;
                    break;
                case 8:subMat = subMat8;
                    break;
                case 9:subMat = subMat9;
                    break;
                case 10:subMat = subMat10;
                    break;
                case 11:subMat = subMat11;
                    break;
                case 12:subMat = subMat12;
                    break;
                default:break;
                }
                double real = filterGabor(subMat,realK);
                double imag = filterGabor(subMat,imagK);

                if(real >= 0)
                {irisCode.push_back('1');maxw += real;}
                else
                {irisCode.push_back('0');maxw += -real;}
                if(imag >= 0)
                {irisCode.push_back('1');maxw += imag;}
                else
                {irisCode.push_back('0');maxw += -imag;}
            }
       }
       //suitw.push_back(maxw);
       suitmap.insert({maxw,0.1+0.1*i});

       if(out.is_open())
       {
           out << "w:" << 0.1+0.1*i << " maxw:" << maxw << "\n";
       }
    }
    //quicksort(suitw,0,suitw.size()-1);
    out << "first 100:" << "\n";
    auto end = suitmap.cend();
    for(int i = 0;i < 100;i++)
    {
        end--;
        out << "maxw:" << end->first << "   w:" << end->second << "\n";

    }

    out.close();
    //qDebug()<<"irisCode size:"<<irisCode.size();
    dst = cv::Mat(30,160,CV_8U,cv::Scalar(127));
    int i = 0;
    for(int u = 0;u < 30;u++)
    {
        for(int v = 0;v < 160;)
        {
            uchar data = (irisCode[i] == '0') ? 1 : 255;
            dst.at<uchar>(u,v) = data;
            dst.at<uchar>(u,v+1) = data;
            //std::cout<<irisCode[i]<<" ";
            i++;
            v += 2;
        }
        //std::cout<<std::endl<<std::flush;
    }
}

//降序快排
void Gaborfilter::quicksort(std::vector<double> a, int left, int right)
{
    if(left < right)
    {
        int i = left;
        int j = right;
        double x = a[i];
        while(i < j)
        {
            while(i < j && a[j] < x)
                j--;
            if(i < j)
                a[i++] = a[j];
            while(i < j && a[i] > x)
                i++;
            if(i < j)
                a[j--] = a[i];
        }
        a[i] = x;
        quicksort(a,left,i-1);
        quicksort(a,i+1,right);
    }
}
