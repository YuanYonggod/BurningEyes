#ifndef IRISRECOGNITION_H
#define IRISRECOGNITION_H

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "segment.h"
#include "normlize.h"
#include "gaborfilter.h"

#include <vector>

class IrisRecognition
{
public:
    IrisRecognition();
    ~IrisRecognition();

    void segmentIris(cv::Mat &src,cv::Mat &dst);
    void normlizeIris(cv::Mat &src,cv::Mat &dst);
    void gaborFilterIris(cv::Mat &src,cv::Mat &dst);


    cv::Mat srcMat,grayMat,gaussMat,segMat,normMat,codeMat;
    std::vector<int> irisCode;
    Segment segment;
    Normlize normlize;
    Gaborfilter gaborFilter;
    int pupil_x,pupil_y,pupil_r,iris_r;

};
#endif // IRISRECOGNITION_H
