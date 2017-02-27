#ifndef GABORFILTER_H
#define GABORFILTER_H

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <vector>
class Gaborfilter
{
public:
    Gaborfilter();
    ~Gaborfilter();

    bool create_kernel(int r,int theta,int alpha,int beta,double omiga,int ktype);
    cv::Mat getRealKernel();
    cv::Mat getImagKernel();
    std::vector<int> getIrisCode();
    void clearIrisCode();
    double filterGabor(cv::Mat &src,cv::Mat &kernel);
    void gaborCode(cv::Mat &src,cv::Mat &dst);

private:
    cv::Mat realKernel,imagKernel;
    std::vector<int> irisCode;
    bool isCreatedKernel = false;
};

#endif // GABORFILTER_H
