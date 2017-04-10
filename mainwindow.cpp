#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QString>
#include <QFileDialog>
#include <QImage>
#include <QDebug>
#include <fstream>
#include <sstream>

#include <windows.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->pushButton_2->setEnabled(false);
    ui->pushButton_3->setEnabled(false);
    ui->pushButton_4->setEnabled(false);
    ui->pushButton_5->setEnabled(false);
    ui->pushButton_6->setEnabled(false);
    ui->pushButton_7->setEnabled(false);
    ui->pushButton_10->setEnabled(false);
    connect(ui->pushButton,&QPushButton::clicked,this,&MainWindow::select_clicked);
    connect(ui->pushButton_2,&QPushButton::clicked,this,&MainWindow::segment_clicked);
    connect(ui->pushButton_3,&QPushButton::clicked,this,&MainWindow::normlize_clicked);
    connect(ui->pushButton_4,&QPushButton::clicked,this,&MainWindow::gabor_clickd);
    connect(ui->pushButton_5,&QPushButton::clicked,this,&MainWindow::start);
    connect(ui->pushButton_6,&QPushButton::clicked,this,&MainWindow::saveCode_clicked);
    connect(ui->pushButton_7,&QPushButton::clicked,this,&MainWindow::match_clicked);
    connect(ui->pushButton_8,&QPushButton::clicked,this,&MainWindow::load_clicked);
    connect(ui->pushButton_9,&QPushButton::clicked,this,&MainWindow::batch_clicked);
    connect(ui->pushButton_10,&QPushButton::clicked,this,&MainWindow::batchMatch_clicked);
}

MainWindow::~MainWindow()
{
    delete ui;
}

//槽函数，打开图片
void MainWindow::select_clicked()
{
    clearAll();
    iris.srcMat = selectImage();
    showImageInLabel(iris.srcMat,ui->label);
    ui->pushButton_2->setEnabled(true);
    ui->pushButton_5->setEnabled(true);
}

//槽函数，分割虹膜区域
void MainWindow::segment_clicked()
{
    ui->pushButton_2->setEnabled(false);
    ui->pushButton_5->setEnabled(false);
    cv::cvtColor(iris.srcMat,iris.grayMat,CV_BGR2GRAY);
    cv::GaussianBlur(iris.grayMat,iris.gaussMat,cv::Size(3,3),1.0);
    iris.segmentIris(iris.gaussMat,iris.segMat);
    showImageInLabel(iris.segMat,ui->label_2,QImage::Format_Grayscale8);
    ui->pushButton_3->setEnabled(true);
}

//槽函数，归一化
void MainWindow::normlize_clicked()
{
    ui->pushButton_3->setEnabled(false);
    iris.normlizeIris(iris.grayMat,iris.normMat);
    showImageInLabel(iris.normMat,ui->label_3,QImage::Format_Grayscale8,QSize(180,40));
    ui->pushButton_4->setEnabled(true);
}

//槽函数，Gabor提取特征
void MainWindow::gabor_clickd()
{
    ui->pushButton_4->setEnabled(false);
    iris.gaborFilterIris(iris.normMat,iris.codeMat);
    showImageInLabel(iris.codeMat,ui->label_4,QImage::Format_Grayscale8,QSize(128,32));
    ui->pushButton_6->setEnabled(true);
    ui->pushButton_7->setEnabled(true);
}

//槽函数，处理一张图片
void MainWindow::start()
{
    ui->pushButton_2->setEnabled(false);
    ui->pushButton_5->setEnabled(false);

    cv::cvtColor(iris.srcMat,iris.grayMat,CV_BGR2GRAY);
    cv::GaussianBlur(iris.grayMat,iris.gaussMat,cv::Size(3,3),1.0);
    iris.segmentIris(iris.gaussMat,iris.segMat);
    showImageInLabel(iris.segMat,ui->label_2,QImage::Format_Grayscale8);

    iris.normlizeIris(iris.grayMat,iris.normMat);
    showImageInLabel(iris.normMat,ui->label_3,QImage::Format_Grayscale8,QSize(180,40));

    iris.gaborFilterIris(iris.normMat,iris.codeMat);
    showImageInLabel(iris.codeMat,ui->label_4,QImage::Format_Grayscale8,QSize(128,32));
    Sleep(10);
    ui->pushButton_6->setEnabled(true);
    ui->pushButton_7->setEnabled(true);
}

//槽函数，写入虹膜码
void MainWindow::saveCode_clicked()
{
    ui->pushButton_6->setEnabled(false);
    iris.writeIrisCode();
}
//槽函数，加载虹膜码
void MainWindow::load_clicked()
{
    iris.clearLoadCode();
    QString fileName = QFileDialog::getOpenFileName(this,tr("load iris databets!"),".",tr("Image Files(*.iris)"));
    iris.codepath = fileName.toStdString();
    iris.loadIrisCode();
    ui->pushButton_10->setEnabled(true);
}

//槽函数，匹配虹膜码
void MainWindow::match_clicked()
{
    iris.matchIrisCode();
}

//槽函数，批量处理文件夹
void MainWindow::batch_clicked()
{
    readDir();
    for(int i = 0;i < filelist.size();i++)
    {
        clearAll();

        QString fileName = filelist.at(i).absoluteFilePath();
        QString baseName = filelist.at(i).baseName();
        iris.path = fileName.toStdString();
        iris.srcMat = cv::imread(fileName.toLatin1().data());

        cv::cvtColor(iris.srcMat,iris.grayMat,CV_BGR2GRAY);
        cv::GaussianBlur(iris.grayMat,iris.gaussMat,cv::Size(3,3),1.0);
        iris.segmentIris(iris.gaussMat,iris.segMat);
        string segPath = segDirName[i] + "/" + baseName.toStdString() + ".jpg";
        cv::imwrite(segPath,iris.segMat);

        iris.normlizeIris(iris.grayMat,iris.normMat);
        string normPath = normDirName[i] + "/" + baseName.toStdString() + ".jpg";
        cv::imwrite(normPath,iris.normMat);

        iris.gaborFilterIris(iris.normMat,iris.codeMat);
        iris.writeIrisCodeAll(dirName[i],baseName.toStdString() + ".jpg");
    }
    clearDir();
}

//槽函数，测试集
void MainWindow::batchMatch_clicked()
{
    QString dirstr = QFileDialog::getExistingDirectory();
    QDir dir(dirstr);
    QFileInfoList fl = dir.entryInfoList(QDir::Files);
    double c = 0;
    QDir *d = new QDir;
    d->mkdir("SamplesResult");
    delete d;
    for(int i = 0;i < fl.size();i++)
    {
        QString fileName = fl.at(i).absoluteFilePath();
        QString baseName = fl.at(i).baseName();
        string target = baseName.toStdString().substr(2,3);
        qDebug()<<QString::fromStdString(target);
        iris.path = fileName.toStdString();
        iris.srcMat = cv::imread(fileName.toLatin1().data());

        cv::cvtColor(iris.srcMat,iris.grayMat,CV_BGR2GRAY);
        cv::GaussianBlur(iris.grayMat,iris.gaussMat,cv::Size(3,3),1.0);
        iris.segmentIris(iris.gaussMat,iris.segMat);
        string segPath = "SamplesResult/" + baseName.toStdString() + ".jpg";
        cv::imwrite(segPath,iris.segMat);

        iris.normlizeIris(iris.grayMat,iris.normMat);
        string normPath = "SamplesResult/" + baseName.toStdString() + ".jpg";
        cv::imwrite(normPath,iris.normMat);

        iris.gaborFilterIris(iris.normMat,iris.codeMat);
        if(iris.showBatchResult("SamplesResult/MatchResult.txt",target))
            c++;
    }
    double ratio = c / fl.size();
    qDebug()<<"ratio:"<<ratio;
    stringstream ssm;
    ssm << ratio;
    string r = ssm.str();
    ofstream out("SamplesResult/MatchResult.txt",ios::app|ios::binary);
    out << "ratio:" << r << '\n';
    out.close();
}

void MainWindow::showImageInLabel(cv::Mat &image,QLabel *label,QImage::Format format,QSize showSize)
{
    QImage showImage((const unsigned char*)(image.data),image.cols,image.rows,format);
    QImage scaledImage = showImage.scaled(showSize);
    label->setPixmap(QPixmap::fromImage(scaledImage));
    label->resize(scaledImage.size());
}


cv::Mat MainWindow::selectImage()
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("open image!"),".",tr("Image Files(*.png *.jpg *bmp)"));
    iris.path = fileName.toStdString();
    cv::Mat srcMat = cv::imread(fileName.toLatin1().data());
    return srcMat;
}

void MainWindow::readDir()
{
    filelist.clear();
    QString dirstr = QFileDialog::getExistingDirectory();
    filelist = getAllFiles(dirstr);
    QDir *dir = new QDir;
    dir->mkdir("result");
    dir->mkdir("result/segment");
    dir->mkdir("result/normlize");
    for(int i = 0;i < filelist.size();i++)
    {
        QString path = filelist.at(i).absoluteFilePath();
        string pathstr = path.toStdString();
        int pos = pathstr.find_last_of('/');
        pathstr = pathstr.substr(0,pos);
        pos = pathstr.find_last_of('/');
        pathstr = pathstr.substr(0,pos);
        pos = pathstr.find_last_of('/');
        string dirname = pathstr.substr(pos+1);

        dirName.push_back(dirname);

        QString qSegDirName = "result/segment/" + QString::fromStdString(dirname);
        QString qNormDirName = "result/normlize/" + QString::fromStdString(dirname);

        bool flag = dir->exists(qSegDirName);
        if(!flag)
            dir->mkdir(qSegDirName);
        segDirName.push_back(qSegDirName.toStdString());

        flag = dir->exists(qNormDirName);
        if(!flag)
            dir->mkdir(qNormDirName);
        normDirName.push_back(qNormDirName.toStdString());

    }
    delete dir;
}

void MainWindow::clearDir()
{
    filelist.clear();
    dirName.clear();
    segDirName.clear();
    normDirName.clear();
}

QFileInfoList MainWindow::getAllFiles(QString path)
{
    QDir dir(path);
    QFileInfoList filelist = dir.entryInfoList(QDir::Files);
    QFileInfoList foderlist = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot);//不要当前目录和上级目录
    for(int i = 0;i < foderlist.size();i++)
    {
        QString name = foderlist.at(i).absoluteFilePath();
        QFileInfoList list = getAllFiles(name);
        filelist.append(list);
    }
    return filelist;
}

void MainWindow::clearAll()
{
    ui->label->setText("image");
    ui->label_2->setText("image");
    ui->label_3->setText("image");
    ui->label_4->setText("image");
    iris.init();
}
