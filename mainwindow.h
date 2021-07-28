#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>
#include <QFileDialog>
#include <QPainter>
#include <QPushButton>
#include <QWidget>
#include <iostream>
#include <cmath>
#include <vector>
#include <QLabel>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class WidgetM : public QWidget
{
    Q_OBJECT

public:
    WidgetM(QWidget *parent = nullptr);
    ~WidgetM();
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    QImage* thisIm=nullptr;
private:
    Ui::MainWindow *ui;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void paintEvent(QPaintEvent *);
    void keyPressEvent(QKeyEvent *e);
    void keyReleaseEvent(QKeyEvent *e);
    void wheelEvent(QWheelEvent *event);
    QLabel* InfoP=nullptr; QLabel* InfoC=nullptr; QLabel* Info=nullptr;
    WidgetM* ImW=nullptr;
public slots:
    void OpenImage();
    void Tick();
    void paintTick();
private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
