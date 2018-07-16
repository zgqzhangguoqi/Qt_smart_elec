#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QMouseEvent>
#include<QPaintEvent>
#include<QtGui>

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
        painter.drawImage(80,80,image);
    }

};

#endif // MAINWINDOW_H
