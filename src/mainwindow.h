#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include "configuration.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    enum PixelFormat {
        PixelFormat_BG10 = 0,
        PixelFormat_RG10,
        PixelFormat_YUV422,
        PixelFormat_RGB888,
        PixelFormat_GRAY,
        PixelFormat_JPEG,
        PixelFormat_PNG,
        PixelFormat_BMP
    };
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void loadImageFile(const QString &fileName, QByteArray &data);
    void displayImage();
private:
    Ui::MainWindow *ui;
    Configuration config;
    cv::Mat rgb;
};
#endif // MAINWINDOW_H
