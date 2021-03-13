#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <librealsense2/rs.hpp>
#include <librealsense2/rsutil.h>
#include "opencv.hpp"
#include <opencv2/imgproc.hpp>
#include <QString>
#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <QElapsedTimer>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
