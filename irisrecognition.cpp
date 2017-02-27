#include "irisrecognition.h"

IrisRecognition::IrisRecognition()
{

}

IrisRecognition::~IrisRecognition()
{

}

void IrisRecognition::segmentIris(cv::Mat &src, cv::Mat &dst)
{
    segment.findPupilEdge(src,dst);
    segment.findIrisEdge(dst,dst);
    pupil_x = segment.pupil_x;
    pupil_y = segment.pupil_y;
    pupil_r = segment.pupil_r;
    iris_r = segment.iris_r;
}

void IrisRecognition::normlizeIris(cv::Mat &src, cv::Mat &dst)
{
    //normlize.normlizeIris(src,dst,pupil_x,pupil_y,pupil_r,iris_r);
    normlize.normlizeIris2(src,dst,pupil_x,pupil_y,pupil_r,iris_r);
}

void IrisRecognition::gaborFilterIris(cv::Mat &src, cv::Mat &dst)
{
    gaborFilter.gaborCode(src,dst);
    irisCode = gaborFilter.getIrisCode();
    gaborFilter.clearIrisCode();
}
