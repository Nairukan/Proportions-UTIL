#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMouseEvent>
#include <QTimer>
#include <QCursor>
#include <QWidget>

QPushButton* FileDialog=nullptr;
QRect Daddy(0,0,0,0), child(0,0,0,0);
QPoint F_Daddy(0,0), S_Daddy(0,0), F_child(0,0), S_child(0,0);
QTimer* T1=nullptr; QTimer* PaintTimer=nullptr;
int control=0;
double W_left=-1, W_top=-1, W_width=-1, W_heigth=-1;
bool CTRL_ON=0;
MainWindow* mainW=nullptr;
std::vector <QImage> Zooms;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    mainW=this;
    this->resize(1275, 940);
    this->ImW=new WidgetM(this);
    ImW->setGeometry(15, 100, 1020, 700);
    T1 = new QTimer(this);
    PaintTimer = new QTimer(this);
    connect(T1, SIGNAL(timeout()),this, SLOT(Tick()));
    connect(PaintTimer, SIGNAL(timeout()),this, SLOT(paintTick()));
    FileDialog=new QPushButton(this);
    FileDialog->setText("Choose Source");
    connect(FileDialog, SIGNAL(clicked()), this, SLOT(OpenImage()));
    InfoP=new QLabel(this);
    InfoP->setGeometry(1050, 150, 200, 100);
    InfoC=new QLabel(this);
    InfoC->setGeometry(1050, 300, 200, 100);
    Info=new QLabel(this);
    Info->setGeometry(1050, 450, 200, 100);
    InfoP->setText(QString::number(Daddy.left())+" "+QString::number(Daddy.top())+"\n"+QString::number(Daddy.width())+" "+QString::number(Daddy.height()));
    InfoC->setText(QString::number(child.left())+" "+QString::number(child.top())+"\n"+QString::number(child.width())+" "+QString::number(child.height()));
    Info->setText(QString::number((double)(child.left()-Daddy.left())/Daddy.width())+" "+QString::number((double)(child.top()-Daddy.top())/Daddy.height())+"\n"+
                  QString::number((double)(child.width())/Daddy.width())+" "+QString::number((double)(child.height())/Daddy.height()));
}

WidgetM::WidgetM(QWidget *parent):QWidget(parent){}

WidgetM::~WidgetM(){
    thisIm->~QImage();
}

MainWindow::~MainWindow()
{
    ImW->~WidgetM();
    InfoP->~QLabel();
    InfoC->~QLabel();
    Info->~QLabel();
    FileDialog->~QPushButton();
    delete ui;
}



void MainWindow::paintEvent(QPaintEvent *){
    QPainter p(this);
    p.drawRect(this->ImW->geometry());
    p.drawRect(InfoP->x(), InfoP->y(), InfoP->width(), InfoP->height());
    p.drawRect(InfoC->x(), InfoC->y(), InfoC->width(), InfoC->height());
    p.drawRect(Info->x(), Info->y(), Info->width(), Info->height());
}

void WidgetM::paintEvent(QPaintEvent *){
    QPainter p(this);
    if (thisIm!=nullptr){
        QRect ImageSize((int)((1020-Zooms[Zooms.size()-1].width()/(std::max((double)Zooms[Zooms.size()-1].width()/1020, (double)Zooms[Zooms.size()-1].height()/700)))/2),
                        (int)((700-Zooms[Zooms.size()-1].height()/(std::max((double)Zooms[Zooms.size()-1].width()/1020, (double)Zooms[Zooms.size()-1].height()/700)))/2),
                        (int)(Zooms[Zooms.size()-1].width()/std::max((double)Zooms[Zooms.size()-1].width()/1020, (double)Zooms[Zooms.size()-1].height()/700)),
                        (int)(Zooms[Zooms.size()-1].height()/std::max((double)Zooms[Zooms.size()-1].width()/1020, (double)Zooms[Zooms.size()-1].height()/700)));
        p.drawImage( ImageSize, Zooms[Zooms.size()-1]);
        if (CTRL_ON){
            QRect ZoomImageSize;
            ZoomImageSize.setLeft((QCursor::pos().x()-this->x()-mainW->x())-this->width()/4);
            ZoomImageSize.setTop((QCursor::pos().y()-this->y()-mainW->y()-30)-this->height()/4);
            ZoomImageSize.setWidth(this->width()/2);
            ZoomImageSize.setHeight(this->height()/2);
            p.drawRect(ZoomImageSize);
        }
        p.setPen(QPen(Qt::green, 2));
        p.drawRect(Daddy);
        p.setPen(QPen(Qt::blue, 2));
        p.drawRect(child);

     }
}

void MainWindow::OpenImage(){
    QImage* temp=ImW->thisIm;
    ImW->thisIm=new QImage(QFileDialog::getOpenFileName( this, tr("Open Image"), QDir::currentPath(), tr("Image Files (*.png *.jpg *.bmp)") ) );
    delete temp;
    Zooms.clear();
    Zooms.push_back(*(ImW->thisIm));
    Daddy=QRect(0,0,0,0); child=QRect(0,0,0,0);
    F_Daddy=QPoint(0,0); S_Daddy=QPoint(0,0); F_child=QPoint(0,0); S_child=QPoint(0,0);
    control=0;
}

void WidgetM::mousePressEvent(QMouseEvent *event){
    if (thisIm!=nullptr){
        if (event->button()==Qt::RightButton){
            F_Daddy={QCursor::pos().x()-this->x()-mainW->x(), QCursor::pos().y()-this->y()-mainW->y()-30};
            S_Daddy=F_Daddy;
            control=1;
        }else{
            F_child={QCursor::pos().x()-this->x()-mainW->x(), QCursor::pos().y()-this->y()-mainW->y()-30};
            S_child=F_child;
            control=2;
        }
        T1->start(10);
    }
}

void WidgetM::mouseReleaseEvent(QMouseEvent *event){
    if (thisIm!=nullptr){
        T1->stop();
        if (event->button()==Qt::RightButton){
            S_Daddy={QCursor::pos().x()-this->x()-mainW->x(), QCursor::pos().y()-this->y()-mainW->y()-30};
            Daddy=QRect(std::min(F_Daddy.x(), S_Daddy.x()), std::min(F_Daddy.y(), S_Daddy.y()), std::abs(F_Daddy.x()-S_Daddy.x()), std::abs(F_Daddy.y()-S_Daddy.y()));
        }else{
            S_child={QCursor::pos().x()-this->x()-mainW->x(), QCursor::pos().y()-this->y()-mainW->y()-30};
            child=QRect(std::min(F_child.x(), S_child.x()), std::min(F_child.y(), S_child.y()), std::abs(F_child.x()-S_child.x()), std::abs(F_child.y()-S_child.y()));
        }
        repaint();
        mainW->InfoP->setText(QString::number(Daddy.left())+" "+QString::number(Daddy.top())+"\n"+QString::number(Daddy.width())+" "+QString::number(Daddy.height()));
        mainW->InfoC->setText(QString::number(child.left())+" "+QString::number(child.top())+"\n"+QString::number(child.width())+" "+QString::number(child.height()));
        mainW->Info->setText(QString::number((double)(child.left()-Daddy.left())/Daddy.width())+" "+QString::number((double)(child.top()-Daddy.top())/Daddy.height())+"\n"+
                      QString::number((double)(child.width())/Daddy.width())+" "+QString::number((double)(child.height())/Daddy.height()));
    }
}

void MainWindow::Tick(){
    if (control==1){
        S_Daddy={QCursor::pos().x()-this->x()-ImW->x(), QCursor::pos().y()-this->y()-ImW->y()-30};
        Daddy=QRect(std::min(F_Daddy.x(), S_Daddy.x()), std::min(F_Daddy.y(), S_Daddy.y()), std::abs(F_Daddy.x()-S_Daddy.x()), std::abs(F_Daddy.y()-S_Daddy.y()));

    }else if (control==2){
        S_child={QCursor::pos().x()-this->x()-ImW->x(), QCursor::pos().y()-this->y()-ImW->y()-30};
        child=QRect(std::min(F_child.x(), S_child.x()), std::min(F_child.y(), S_child.y()), std::abs(F_child.x()-S_child.x()), std::abs(F_child.y()-S_child.y()));
    }
    repaint();
    InfoP->setText(QString::number(Daddy.left())+" "+QString::number(Daddy.top())+"\n"+QString::number(Daddy.width())+" "+QString::number(Daddy.height()));
    InfoC->setText(QString::number(child.left())+" "+QString::number(child.top())+"\n"+QString::number(child.width())+" "+QString::number(child.height()));
    Info->setText(QString::number((double)(child.left()-Daddy.left())/Daddy.width())+" "+QString::number((double)(child.top()-Daddy.top())/Daddy.height())+"\n"+
                  QString::number((double)(child.width())/Daddy.width())+" "+QString::number((double)(child.height())/Daddy.height()));
}

void MainWindow::keyPressEvent(QKeyEvent *e){
    if (ImW->thisIm!=nullptr){
        switch (e->key())
        {
            case 16777249:{
               CTRL_ON = true;
               PaintTimer->start(15);
               break;
            }
            default:
                break;
        }
    }
}

void MainWindow::keyReleaseEvent(QKeyEvent *e){
    if (ImW->thisIm!=nullptr){
        switch (e->key())
        {
            case 16777249:{
               CTRL_ON = false;
               PaintTimer->stop();
               repaint();
               break;
            }
            default:
                break;
        }
    }
}

void MainWindow::wheelEvent(QWheelEvent *event)
{
    if (ImW->thisIm!=nullptr && CTRL_ON){
        mainW->Info->setText("Wheel");
        if(event->delta() > 0)
        {
            child={0,0,0,0}; Daddy={0,0,0,0};
            QRect ZoomImageSize;
            ZoomImageSize.setLeft(std::max((QCursor::pos().x()-this->x()-ImW->x()-(int)((1020-Zooms[Zooms.size()-1].width()/(std::max((double)Zooms[Zooms.size()-1].width()/1020, (double)Zooms[Zooms.size()-1].height()/700)))/2))-ImW->width()/4, 0)*std::max((double)Zooms[Zooms.size()-1].width()/1020, (double)Zooms[Zooms.size()-1].height()/700));
            ZoomImageSize.setTop(std::max(((QCursor::pos().y()-this->y()-ImW->y()-30-(int)((700-Zooms[Zooms.size()-1].height()/(std::max((double)Zooms[Zooms.size()-1].width()/1020, (double)Zooms[Zooms.size()-1].height()/700)))/2))-ImW->height()/4), 0)*std::max((double)Zooms[Zooms.size()-1].width()/1020, (double)Zooms[Zooms.size()-1].height()/700));
            ZoomImageSize.setWidth(std::min((int)(ImW->width()*std::max((double)Zooms[Zooms.size()-1].width()/1020, (double)Zooms[Zooms.size()-1].height()/700)/2), Zooms[Zooms.size()-1].width()-ZoomImageSize.left()));
            ZoomImageSize.setHeight(std::min((int)(ImW->height()*std::max((double)Zooms[Zooms.size()-1].width()/1020, (double)Zooms[Zooms.size()-1].height()/700)/2), Zooms[Zooms.size()-1].height()-ZoomImageSize.top()));
            QImage ZoomImage=Zooms[Zooms.size()-1].copy(ZoomImageSize);
            Zooms.push_back(ZoomImage);
            repaint();
        }
        else if (Zooms.size()!=1){
            child={0,0,0,0}; Daddy={0,0,0,0};
            Zooms.erase(Zooms.begin()+Zooms.size()-1);
            repaint();
        }
    }
}

void MainWindow::paintTick(){
    repaint();
}
