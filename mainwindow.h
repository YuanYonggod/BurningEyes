#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <QMainWindow>
#include <QLabel>
#include <QDir>
#include <QFileInfoList>
#include <QFileInfo>
#include <vector>

#include <irisrecognition.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void showImageInLabel(cv::Mat &image,QLabel *label,
                          QImage::Format = QImage::Format_RGB888,QSize showSize = QSize(160,120));
    cv::Mat selectImage();
    void readDir();
    void clearDir();
    QFileInfoList getAllFiles(QString path);
    void clearAll();

public slots:
    void select_clicked();
    void segment_clicked();
    void normlize_clicked();
    void gabor_clickd();
    void start();
    void saveCode_clicked();
    void load_clicked();
    void match_clicked();
    void batch_clicked();
    void batchMatch_clicked();

private:
    Ui::MainWindow *ui;
    IrisRecognition iris;
    QFileInfoList filelist;
    std::vector<string> dirName;
    std::vector<string> segDirName;
    std::vector<string> normDirName;
};

#endif // MAINWINDOW_H
