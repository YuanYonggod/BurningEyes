#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QString>
#include <QFileDialog>
#include <QImage>
#include <QDebug>

#include <windows.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->pushButton_2->setEnabled(false);
    ui->pushButton_3->setEnabled(false);
    ui->pushButton_3->setEnabled(false);
    ui->pushButton_4->setEnabled(false);
    ui->pushButton_5->setEnabled(false);
    ui->pushButton_6->setEnabled(false);
    ui->pushButton_7->setEnabled(false);
    connect(ui->pushButton,&QPushButton::clicked,this,&MainWindow::select_clicked);
    connect(ui->pushButton_2,&QPushButton::clicked,this,&MainWindow::segment_clicked);
    connect(ui->pushButton_3,&QPushButton::clicked,this,&MainWindow::normlize_clicked);
    connect(ui->pushButton_4,&QPushButton::clicked,this,&MainWindow::gabor_clickd);
    connect(ui->pushButton_5,&QPushButton::clicked,this,&MainWindow::start);
    connect(ui->pushButton_6,&QPushButton::clicked,this,&MainWindow::saveCode_clicked);
    connect(ui->pushButton_7,&QPushButton::clicked,this,&MainWindow::match_clicked);
    connect(ui->pushButton_8,&QPushButton::clicked,this,&MainWindow::load_clicked);
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

//槽函数，批处理
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
}

//槽函数，匹配虹膜码
void MainWindow::match_clicked()
{
    iris.matchIrisCode();
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
    if(srcMat.channels() == 3)
    {
        //qDebug()<<"3 channels image"<<endl;
        cv::cvtColor(srcMat,srcMat,CV_BGR2RGB);
        return srcMat;
    }
    else
    {
        //qDebug()<<"1 channel image"<<endl;
        return srcMat;
    }
}

void MainWindow::clearAll()
{
    ui->label->setText("image");
    ui->label_2->setText("image");
    ui->label_3->setText("image");
    ui->label_4->setText("image");
    iris.init();
}
