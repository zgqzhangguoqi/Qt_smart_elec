#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QMouseEvent>
#include<QPaintEvent>
#include<QtGui>
#include<QLabel>
#include<QDateTime>
#include<QTimer>
#include<QString>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QImage image;
     void Paint();
protected:
    /*添加自定义方法，paintEvent（）
      drawImage：设置显示位置
     */
    void paintEvent(QPaintEvent *){
        QPainter painter(this);
        painter.drawImage(10,90,image);
    }

private slots:
    void timerUpdate(void);
};

#endif // MAINWINDOW_H
