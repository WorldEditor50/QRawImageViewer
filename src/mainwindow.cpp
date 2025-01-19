#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QFileInfo>
#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("QRawImageViewer");
    ui->fileSizeLineEdit->setReadOnly(true);
    ui->pixelFormatComboBox->addItems(QStringList{"BG10", "RG10", "YUV422", "RGB888", "GRAY", "JPEG", "PNG", "BMP"});
    /* load config */
    int ret = config.load("./viewer.ini");
    if (ret != 0) {
        config.setPath(".");
    } else {
        ui->widthLineEdit->setText(QString::number(config.getWidth()));
        ui->heightLineEdit->setText(QString::number(config.getHeight()));
        ui->pixelFormatComboBox->setCurrentIndex(config.getFormat());
    }
    connect(ui->pixelFormatComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, [=](int index) {config.setFormat(index);});
    connect(ui->openBtn, &QPushButton::clicked, this, [=](){
        QString filePath = QFileDialog::getOpenFileName(this, "select an image", config.getPath());
        if (filePath.isEmpty()) {
            return;
        }
        QFileInfo info(filePath);
        config.setPath(info.absolutePath());
        ui->pathLineEdit->setText(info.absoluteFilePath());
    });
    connect(ui->decodeBtn, &QPushButton::clicked, this, &MainWindow::displayImage);
}

MainWindow::~MainWindow()
{
    config.save("./viewer.ini");
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
    config.setWidth(w);
    config.setHeight(h);

    loadImageFile(path, rawImg);
    if (rawImg.isEmpty()) {
        QMessageBox::warning(this, "Error", "failed to load image file.");
        return;
    }
    ui->fileSizeLineEdit->setText(QString::number(rawImg.size()));

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
        if (h*w*3 > rawImg.size()) {
            QMessageBox::warning(this, "Error", "invalid height or width");
            return;
        }
        QImage img((uchar*)rawImg.data(), w, h, QImage::Format_RGB888);
        ui->imageLabel->setPixmap(QPixmap::fromImage(img.scaled(ui->imageLabel->size())));
        break;
    }
    case PixelFormat_YUV422: {
        if (h*w*2 > rawImg.size()) {
            QMessageBox::warning(this, "Error", "invalid height or width");
            return;
        }
        cv::Mat yuv422(h, w*2, CV_8UC1, (uchar*)rawImg.data());
        cv::cvtColor(yuv422, rgb, cv::COLOR_YUV2RGB_YUYV);
        if (rgb.empty()) {
            QImage img(rgb.data, w, h, QImage::Format_RGB888);
            ui->imageLabel->setPixmap(QPixmap::fromImage(img.scaled(ui->imageLabel->size())));
        }
        break;
    }
    case PixelFormat_BG10: {
        if (h*w*5/4 > rawImg.size()) {
            QMessageBox::warning(this, "Error", "invalid height or width");
            return;
        }
        cv::Mat raw10(h, w, CV_16SC1, (uchar*)rawImg.data());
        cv::Mat raw8;
        cv::convertScaleAbs(raw10, raw8, 0.25);
        cv::cvtColor(raw10, rgb, cv::COLOR_BayerBGGR2RGB);
        QImage img(rgb.data, w, h, QImage::Format_RGB888);
        ui->imageLabel->setPixmap(QPixmap::fromImage(img.scaled(ui->imageLabel->size())));
        break;
    }
    case PixelFormat_RG10: {
        if (h*w*5/4 > rawImg.size()) {
            QMessageBox::warning(this, "Error", "invalid height or width");
            return;
        }
        cv::Mat raw10(h, w, CV_16SC1, (uchar*)rawImg.data());
        cv::Mat raw8;
        cv::convertScaleAbs(raw10, raw8, 0.25);
        cv::cvtColor(raw8, rgb, cv::COLOR_BayerRGGB2RGB);
        QImage img(rgb.data, w, h, QImage::Format_RGB888);
        ui->imageLabel->setPixmap(QPixmap::fromImage(img.scaled(ui->imageLabel->size())));
        break;
    }
    default:
        break;
    }
    return;
}


