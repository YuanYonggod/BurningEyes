#include "irisrecognition.h"

#include <iostream>

IrisRecognition::IrisRecognition()
{

}

IrisRecognition::~IrisRecognition()
{
    irisCode.clear();
}
void IrisRecognition::init()
{
    if(!irisCode.empty())
        irisCode.clear();
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

void IrisRecognition::writeIrisCode()
{
    int pos = path.find_last_of('/');
    string file(path.substr(pos+1));
    string temp(path.substr(0,pos));
    int inx = temp.find_last_of('/');
    string dir(temp.substr(inx+1));
    cout<<dir<<" "<<file<<endl;
    match.writeCode(dir,file,irisCode);
}

void IrisRecognition::matchIrisCode()
{
    match.matchCode(irisCode);
}

void IrisRecognition::loadIrisCode()
{
    match.loadCodes(codepath);
}

void IrisRecognition::clearLoadCode()
{
    match.clearMemorry();
}
