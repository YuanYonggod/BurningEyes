#include "segment.h"

#include <QDebug>

#define THREADS_NUM 4    //线程数

static void* calMaxEdge(void *arg)
{
    t_attr *attr = (t_attr *)arg;
    int max = 0;
    r_attr *rattr = new r_attr;
    for(int i = attr->i0;i < attr->i1;i++)
    {
        for(int j = attr->j0;j < attr->j1;j++)
        {
            for(int r = attr->r0;r < attr->r1;r++)
            {
                int die = attr->seg.calCircleSum(attr->seg.gaussMat,j,i,r+1)
                        - attr->seg.calCircleSum(attr->seg.gaussMat,j,i,r-1);
                if(die > max)
                {
                    max = die;
                    rattr->x = j;
                    rattr->y = i;
                    rattr->r = r;
                    rattr->max = max;
                }
            }
        }
    }

    pthread_exit((void *)rattr);
}

Segment::Segment()
{

}

Segment::~Segment()
{

}

//计算出瞳孔中心点及半径
void Segment::findPupilEdge(cv::Mat &src, cv::Mat &dst)
{
    int px,py,pr;
    dst = src.clone();
    int height = src.rows;
    int width = src.cols;
//    int max = 0;
//    for(int i = 200;i < height-200;i++)
//    {
//        for(int j = 250;j < width-250;j++)
//        {
//            for(int r = 20;r < 60;r++)
//            {
//                int die = calCircleSum(gauMat,j,i,r+1)-calCircleSum(gauMat,j,i,r-1);
//                if(die > max)
//                {
//                    px = j;py = i;pr = r;
//                    max = die;
//                }
//            }
//        }
//    }

//多线程
    pthread_t tid[THREADS_NUM];
    t_attr attr[THREADS_NUM];
    pthread_attr_t pttr;
    pthread_attr_init(&pttr);
    pthread_attr_setdetachstate(&pttr,PTHREAD_CREATE_JOINABLE);
    for(int i = 0;i < THREADS_NUM;i++)
    {
        attr[i].i0 = 150;attr[i].i1 = height-150;
        attr[i].j0 = 200;attr[i].j1 = width-200;
        attr[i].r0 = 20+i*10;attr[i].r1 = 30+i*10;
        attr[i].seg.gaussMat = src;
        int ret = pthread_create(&tid[i],&pttr,calMaxEdge,(void*)&attr[i]);
        if(ret != 0)
            qDebug()<<"thread error!error code:"<<ret;
    }
    pthread_attr_destroy(&pttr);
    void *maxv;
    //r_attr *maxattr;
    int themax = 0;
    for(int j = 0;j < THREADS_NUM;j++)
    {
        int ret = pthread_join(tid[j],&maxv);
        if(ret != 0)
            qDebug()<<"thread error!error code:"<<ret;
        r_attr *maxattr = (r_attr*)maxv;
        if(maxattr->max > themax)
        {
            themax = maxattr->max;
            px = maxattr->x;
            py = maxattr->y;
            pr = maxattr->r;
        }
        delete maxattr;
    }
    pupil_x = px;
    pupil_y = py;
    pupil_r = pr;

    drawCircle(dst,pupil_x,pupil_y,pupil_r+1);
}

//计算出虹膜半径
void Segment::findIrisEdge(cv::Mat &src, cv::Mat &dst)
{
    dst = src.clone();
    int imax = 0;
    for(int r1 = pupil_r * 1.5;r1 < pupil_r * 3.5;r1++)
    {
        int die1 = calCircleSum(src,pupil_x,pupil_y,r1+1)-calCircleSum(src,pupil_x,pupil_y,r1-1);
        if(die1 > imax)
        {
            iris_r = r1;
            imax = die1;
        }
    }
    drawCircle(dst,pupil_x,pupil_y,iris_r-1);
}

int Segment::calCircleSum(cv::Mat &img,int x,int y,int r)
{
   int cs = 2 * M_PI * r;
   double alpha;
   int cx,cy;
   int sum = 0;
//   int I0,I1,I2,I3,I4;

   for(int i = 0;i < cs;i++)
   {
       alpha = (double)(2 * M_PI * i / cs);
       cx = x + r * cos(alpha);
       cy = y + r * sin(alpha);
       uchar *data = img.ptr<uchar>(cy);
//       I1=(int) ((img.at<uchar>(cx + 1,cy) - img.at<uchar>(cx - 1,cy)) * cos(alpha));
//       I2=(int) ((img.at<uchar>(cx,cy+1) - img.at<uchar>(cx,cy-1)) * sin(alpha));
//       I3=(int) ((img.at<uchar>(cx-1,cy+1) - img.at<uchar>(cx+1,cy-1)) * sin(M_PI*45/180 + alpha));
//       I4=(int) ((img.at<uchar>(cx+1,cy+1) - img.at<uchar>(cx-1,cy-1)) * cos(M_PI*45/180 + alpha));
       //I0 = (int) (gauMat.at<uchar>(cx,cy));
       //I1 = (int) (gauMat.at<uchar>(cx-1,cy));
       //I2 = (int) (gauMat.at<uchar>(cx+1,cy));
       //I3 = (int) (gauMat.at<uchar>(cx,cy-1));
       //I4 = (int) (gaumat.at<uchar>(cx,cy+1));
       //sum+=(I0+I1+I2+I3+I4);

       sum += data[cx-1]+data[cx]+data[cx+1];
   }
   return sum;
}

void Segment::drawCircle(cv::Mat &img, int x, int y, int r)
{
    int cs = 2 * M_PI * r;
    double alpha;
    int cx,cy;
    for(int i = 0;i < cs;i++)
    {
        alpha = (double)(2 * M_PI * i / cs);
        cx = x + r * cos(alpha);
        cy = y + r * sin(alpha);
        //cy行cx列
        img.at<uchar>(cy,cx) = 255;
    }
}
