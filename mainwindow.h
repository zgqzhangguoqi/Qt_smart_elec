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
     void Paint();
     int temp_data[30]={0};
     //virtual void timerEvent( QTimerEvent *event);

private:
    Ui::MainWindow *ui;
    QImage image;

     int m_nTimerID;
protected:
    /*添加自定义方法，paintEvent（）
      drawImage：设置显示位置
     */
    void paintEvent(QPaintEvent *){

       QPainter painter(this);
       painter.drawImage(40,100,image);
    }

private slots:
    void timerUpdate(void);
    void on_lcd_temp_listen();
    void on_lcd_hum_listen();
    void on_edit_sun_listen();
    void on_radio_auto_clicked();
    void on_combox_fan_set();
    void on_combox_time_listen();
    void on_btn_temp_clicked();
    void update_cap();
};

#endif // MAINWINDOW_H
