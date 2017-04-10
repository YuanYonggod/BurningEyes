#ifndef GABORFILTER_H
#define GABORFILTER_H

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <vector>
#include <map>

class Gaborfilter
{
public:
    Gaborfilter();
    ~Gaborfilter();

    bool create_kernel(int r,int theta,int alpha,int beta,double omiga,int ktype);
    cv::Mat getRealKernel();
    cv::Mat getImagKernel();
    std::vector<char> getIrisCode();
    void clearIrisCode();
    double filterGabor(cv::Mat &src,cv::Mat &kernel);
    void gaborCode(cv::Mat &src,cv::Mat &dst);
    void quicksort(std::vector<double> a,int left,int right);

private:
    cv::Mat realKernel,imagKernel;
    std::vector<char> irisCode;
    bool isCreatedKernel = false;
    //std::vector<double> suitw;
    std::multimap<double,double> suitmap;
};

#endif // GABORFILTER_H
