#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    enum PixelFormat {
        PixelFormat_JPEG = 0,
        PixelFormat_PNG,
        PixelFormat_BMP,
        PixelFormat_GRAY,
        PixelFormat_RGB888,
        PixelFormat_UYVY,
        PixelFormat_BG10,
        PixelFormat_RG10
    };
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void loadImageFile(const QString &fileName, QByteArray &data);
    void displayImage();
private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
