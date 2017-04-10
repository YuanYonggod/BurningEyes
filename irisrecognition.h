#ifndef IRISRECOGNITION_H
#define IRISRECOGNITION_H

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "segment.h"
#include "normlize.h"
#include "gaborfilter.h"
#include "Match.h"

#include <vector>
#include <string>

class IrisRecognition
{
public:
    IrisRecognition();
    ~IrisRecognition();
    void init();
    void segmentIris(cv::Mat &src,cv::Mat &dst);
    void normlizeIris(cv::Mat &src,cv::Mat &dst);
    void gaborFilterIris(cv::Mat &src,cv::Mat &dst);
    void writeIrisCode();
    void loadIrisCode();
    void matchIrisCode();
    void clearLoadCode();

    void writeIrisCodeAll(string dir,string file);
    bool showBatchResult(string path,string target);

    cv::Mat srcMat,grayMat,gaussMat,segMat,normMat,codeMat;
    std::vector<char> irisCode;
    Segment segment;
    Normlize normlize;
    Gaborfilter gaborFilter;
    Match match;
    int pupil_x,pupil_y,pupil_r,iris_r;

    std::string path;
    std::string codepath;
    std::string message;

};
#endif // IRISRECOGNITION_H
