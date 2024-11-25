#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QFileInfo>
#include <QFileDialog>
#include <QMessageBox>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("QRawImageViewer");
    ui->pixelFormatComboBox->addItems(QStringList{"JPEG", "PNG", "BMP", "GRAY", "RGB888", "UYVY", "BG10", "RG10"});
    connect(ui->openBtn, &QPushButton::clicked, this, [=](){
        QString filePath = QFileDialog::getOpenFileName(this, "select an image", ".");
        if (filePath.isEmpty()) {
            return;
        }
        QFileInfo info(filePath);
        ui->pathLineEdit->setText(info.absoluteFilePath());
    });
    connect(ui->decodeBtn, &QPushButton::clicked, this, &MainWindow::displayImage);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::loadImageFile(const QString &fileName, QByteArray &data)
{
    QFile file(fileName);
    if (file.open(QIODevice::ReadOnly)){
        data = file.readAll();
        file.close();
    }
    return;
}

void MainWindow::displayImage()
{
    QString path = ui->pathLineEdit->text();
    if (path.isEmpty()) {
        QMessageBox::information(this, "Notice", "Please select an image.");
        return;
    }
    int h = 0;
    int w = 0;
    QByteArray rawImg;
    int index = ui->pixelFormatComboBox->currentIndex();
    if (index == PixelFormat_JPEG ||
            index == PixelFormat_PNG ||
            index == PixelFormat_BMP) {
        QImage img(path);
        ui->imageLabel->setPixmap(QPixmap::fromImage(img.scaled(ui->imageLabel->size())));
        return;
    }
    QString heightText = ui->heightLineEdit->text();
    QString widthText = ui->widthLineEdit->text();
    if (heightText.isEmpty()||heightText.isEmpty()) {
        QMessageBox::warning(this, "Error", "emty height or width");
        return;
    }
    h = heightText.toInt();
    w = widthText.toInt();

    loadImageFile(path, rawImg);
    if (rawImg.isEmpty()) {
        QMessageBox::warning(this, "Error", "failed to load image file.");
        return;
    }

    switch (index) {
    case PixelFormat_GRAY: {
        if (w*h != rawImg.size()) {
            QMessageBox::warning(this, "Error", "invalid height or width");
            return;
        }
        QImage img((uchar*)rawImg.data(), w, h, QImage::Format_Grayscale8);
        ui->imageLabel->setPixmap(QPixmap::fromImage(img.scaled(ui->imageLabel->size())));
        break;
    }
    case PixelFormat_RGB888: {
        if (w*h*3 > rawImg.size()) {
            QMessageBox::warning(this, "Error", "invalid height or width");
            return;
        }
        QImage img((uchar*)rawImg.data(), w, h, QImage::Format_RGB888);
        ui->imageLabel->setPixmap(QPixmap::fromImage(img.scaled(ui->imageLabel->size())));
        break;
    }
    case PixelFormat_UYVY: {
        if (w*h*3/2 > rawImg.size()) {
            QMessageBox::warning(this, "Error", "invalid height or width");
            return;
        }
        cv::Mat yuyv(h, w, CV_8UC1, (uchar*)rawImg.data());
        cv::Mat rgb;
        cv::cvtColor(yuyv, rgb, cv::COLOR_YUV2RGB_UYVY);
        QImage img(rgb.data, w, h, QImage::Format_RGB888);
        ui->imageLabel->setPixmap(QPixmap::fromImage(img.scaled(ui->imageLabel->size())));
        break;
    }
    case PixelFormat_BG10: {
        cv::Mat bg10(h, w, CV_8UC1, (uchar*)rawImg.data());
        cv::Mat rgb;
        cv::cvtColor(bg10, rgb, cv::COLOR_BayerBG2RGB);
        QImage img(rgb.data, w, h, QImage::Format_RGB888);
        ui->imageLabel->setPixmap(QPixmap::fromImage(img.scaled(ui->imageLabel->size())));
        break;
    }
    case PixelFormat_RG10: {
        cv::Mat rg10(h, w, CV_8UC1, (uchar*)rawImg.data());
        cv::Mat rgb;
        cv::cvtColor(rg10, rgb, cv::COLOR_BayerRG2RGB);
        QImage img(rgb.data, w, h, QImage::Format_RGB888);
        ui->imageLabel->setPixmap(QPixmap::fromImage(img.scaled(ui->imageLabel->size())));
        break;
    }
    default:
        break;
    }
    return;
}


