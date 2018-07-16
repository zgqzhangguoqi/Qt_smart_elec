#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<cstdlib>
#include<ctime>
#include<QLabel>
#include<QDateTime>
#include<QTimer>
#include<QString>
#define inf 0x3f3f3f3f
int a=0,b=0,c=0;
bool auto_mod=true,fan=false;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

     ui->setupUi(this);
    //更改默认窗口大小  800*600
    this->resize( QSize( 1000, 650 ));
    image = QImage(600,300,QImage::Format_RGB32);  //画布的初始化大小设为600*300，使用32位颜色
    QColor backColor = qRgb(255,255,255);    //画布初始化背景色使用白色
    image.fill(backColor);//对画布进行填充
    //开始绘图

    Paint();

    /*
     * 系统时间显示
     * 1s通过connect刷新一遍数据
     * 防止刚启动时显示失效
     */
    QDateTime time = QDateTime::currentDateTime();
    QString str = time.toString("yyyy-MM-dd hh:mm:ss dddd");
    ui->Time_Counter->setText(str);

    QTimer *timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(timerUpdate()));
    timer->start(1000);

    /*
     * 温度显示屏
     * 1s通过connect刷新一遍数据
     * 防止刚启动时显示失效
     */
     ui->lcd_temp->display(44);
     QTimer *time_lcd_temp = new QTimer(this);
     this->connect(time_lcd_temp, SIGNAL(timeout()), this, SLOT(on_lcd_temp_listen()));
     time_lcd_temp->start(1000);

     /*
      * 温度显示屏
      * 1s通过connect刷新一遍数据
      * 防止刚启动时显示失效
      */
      ui->lcd_hum->display(44);
      QTimer *time_lcd_hum = new QTimer(this);
      this->connect(time_lcd_hum, SIGNAL(timeout()), this, SLOT(on_lcd_hum_listen()));
      time_lcd_hum->start(1000);

      /*
       * 光强显示屏
       * 1s通过connect刷新一遍数据
       * 防止刚启动时显示失效
       */
       ui->edit_sun->setText("22");
       QTimer *time_edit_sun = new QTimer(this);
       this->connect(time_edit_sun, SIGNAL(timeout()), this, SLOT(on_edit_sun_listen()));
       time_edit_sun->start(1000);
       /*
        *  默认自动模式，风扇不可调
        */
       ui->lb_fan_->setEnabled(false);
       ui->combox_fan->setEnabled(false);
       ui->lb_time_fan->setEnabled(false);
       ui->combox_time->setEnabled(false);

       //combox设定监听
       connect(ui->combox_fan,SIGNAL(currentIndexChanged(QString)),this,SLOT(on_combox_fan_set()));
       connect(ui->combox_time,SIGNAL(currentIndexChanged(QString)),this,SLOT(on_combox_time_listen()));
      ui->edit_fan->setText("关闭");
      ui->edit_auto->setText("开");

}
     //温湿度显示画图
void MainWindow::Paint()
{
    QPainter painter(&image);
    painter.setRenderHint(QPainter::Antialiasing, true);//设置反锯齿模式，好看一点


    int pointx=35,pointy=280;//确定坐标轴起点坐标，这里定义(35,280)
    int width=580-pointx,height=260;//确定坐标轴宽度跟高度 上文定义画布为600X300，宽高依此而定。

    //绘制坐标轴 坐标轴原点(35，280)
    painter.drawRect(5,5,600-10,300-10);//外围的矩形，从(5,5)起，到(590,290)结束，周围留了5的间隙。

    painter.drawLine(pointx,pointy,width+pointx,pointy);//坐标轴x宽度为width
    painter.drawLine(pointx,pointy-height,pointx,pointy);//坐标轴y高度为height


    srand(time(NULL));

    //获得数据中最大值和最小值、平均数
    int n=30;       //n为数据个数
    double sum=0;
    double ave=0;
    int _ma=0;      //数组里的最大值
    int _mi=inf;

    int a[n];//数据储存在数组a中，大小为n

    for(int i=0;i<n;i++)
        a[i]=rand()%40+20;
    int maxpos=0,minpos=0;   //最大最小点
    for(int i=0;i<n;i++)
    {
        sum+=a[i];
        if(a[i]>_ma){
            _ma=a[i];
            maxpos=i;
        }
        if(a[i]<_mi){
            _mi=a[i];
            minpos=i;
        }
    }
    ave=sum/n;//平均数

    double kx=(double)width/(n-1); //x轴的系数
    double ky=(double)height/_ma;  //y方向的比例系数
    QPen pen,penPoint;
    pen.setColor(Qt::black);
    pen.setWidth(2);               //折线的宽度

    penPoint.setColor(Qt::blue);
    penPoint.setWidth(5);
    //画点算法
    for(int i=0;i<n-1;i++)
    {
        //由于y轴是倒着的，所以y轴坐标要pointy-a[i]*ky 其中ky为比例系数
        painter.setPen(pen);//黑色笔用于连线
        painter.drawLine(pointx+kx*i,pointy-a[i]*ky,pointx+kx*(i+1),pointy-a[i+1]*ky);
        painter.setPen(penPoint);//蓝色的笔，用于标记各个点
        painter.drawPoint(pointx+kx*i,pointy-a[i]*ky);
    }
    painter.drawPoint(pointx+kx*(n-1),pointy-a[n-1]*ky);//绘制最后一个点

    //绘制平均线
    QPen penAve;
    penAve.setColor(Qt::red);//选择红色
    penAve.setWidth(2);
    penAve.setStyle(Qt::DotLine);//线条类型为虚线
    painter.setPen(penAve);
    painter.drawLine(pointx,pointy-ave*ky,pointx+width,pointy-ave*ky);

    //绘制最大值和最小值
    QPen penMaxMin;
    penMaxMin.setColor(Qt::darkGreen);//暗绿色
    painter.setPen(penMaxMin);
    painter.drawText(pointx+kx*maxpos-kx,pointy-a[maxpos]*ky-5,
                     "最大值"+QString::number(_ma));
    painter.drawText(pointx+kx*minpos-kx,pointy-a[minpos]*ky+15,
                     "最小值"+QString::number(_mi));

    penMaxMin.setColor(Qt::red);
    penMaxMin.setWidth(7);
    painter.setPen(penMaxMin);
    painter.drawPoint(pointx+kx*maxpos,pointy-a[maxpos]*ky);//标记最大值点
    painter.drawPoint(pointx+kx*minpos,pointy-a[minpos]*ky);//标记最小值点


    //绘制刻度线
    QPen penDegree;
    penDegree.setColor(Qt::black);
    penDegree.setWidth(2);
    painter.setPen(penDegree);
    //画上x轴刻度线
    for(int i=0;i<10;i++)//分成10份
    {
        //选取合适的坐标，绘制一段长度为4的直线，用于表示刻度
        painter.drawLine(pointx+(i+1)*width/10,pointy,pointx+(i+1)*width/10,pointy+4);
        painter.drawText(pointx+(i+0.65)*width/10,
                         pointy+10,QString::number((int)((i+1)*((double)n/10))));
    }
    //y轴刻度线
    double _maStep=(double)_ma/10;//y轴刻度间隔需根据最大值来表示
    for(int i=0;i<10;i++)
    {
        //代码较长，但是掌握基本原理即可。
        //主要就是确定一个位置，然后画一条短短的直线表示刻度。
        painter.drawLine(pointx,pointy-(i+1)*height/10,
                         pointx-4,pointy-(i+1)*height/10);
        painter.drawText(pointx-20,pointy-(i+0.85)*height/10,
                         QString::number((int)(_maStep*(i+1))));
    }
}

//更新系统时间
void MainWindow::timerUpdate(void)
{
    QDateTime time = QDateTime::currentDateTime();
    QString str = time.toString("yyyy-MM-dd hh:mm:ss dddd");
    ui->Time_Counter->setText(str);
}
MainWindow::~MainWindow()
{
    delete ui;
}


//温度显示屏刷新
void MainWindow::on_lcd_temp_listen()
{
    //QDateTime time = QDateTime::currentDateTime();
   // QString str = time.toString("ss");
    if(b==0){
        ui->lcd_temp->display(90);
        b=1;
    }else
    {
        ui->lcd_temp->display(21);
        b=0;
    }

}

void MainWindow::on_lcd_hum_listen()
{
    if(a==0){
        ui->lcd_hum->display(98);
        a=1;
    }else
    {
        ui->lcd_hum->display(13);
        a=0;
    }
}

void MainWindow::on_edit_sun_listen()
{
    if(c==0){
        ui->edit_sun->setText("45");
        c=1;
    }else
    {
        ui->edit_sun->setText("89");
        c=0;
    }
}

void MainWindow::on_radio_auto_clicked()
{
    auto_mod= ui->radio_auto->isChecked();
    if(auto_mod){
        ui->lb_fan_->setEnabled(false);
        ui->combox_fan->setEnabled(false);
        ui->lb_time_fan->setEnabled(false);
        ui->combox_time->setEnabled(false);
        ui->btn_temp->setEnabled(true);
        ui->spinBox_temp->setEnabled(true);
        ui->lb_set_temp->setEnabled(true);
        ui->edit_auto->setText("开");
    }else{
        ui->lb_fan_->setEnabled(true);
        ui->combox_fan->setEnabled(true);
        ui->lb_time_fan->setEnabled(true);
        if(fan){

            ui->combox_time->setEnabled(true);
        }
        else{

            ui->combox_time->setEnabled(false);
        }
        ui->btn_temp->setEnabled(false);
        ui->spinBox_temp->setEnabled(false);
        ui->lb_set_temp->setEnabled(false);
        ui->edit_auto->setText("关");

        }
}

void MainWindow::on_combox_fan_set()
{
    if(ui->combox_fan->currentIndex()==0)           {
                                                      ui->edit_fan->setText("关闭");
                                                     ui->combox_time->setEnabled(false);
                                                     fan=false;
                                                     }
        else if(ui->combox_fan->currentIndex()==1)   {
                                                      ui->edit_fan->setText("低速");
                                                      ui->combox_time->setEnabled(true);
                                                      fan=true;
                                                     }
        else if(ui->combox_fan->currentIndex()==2)  {
                                                      ui->edit_fan->setText("中速");
                                                      ui->combox_time->setEnabled(true);
                                                       fan=true;
                                                     }
        else if(ui->combox_fan->currentIndex()==3)   {
                                                     ui->edit_fan->setText("高速");
                                                      ui->combox_time->setEnabled(true);
                                                       fan=true;
                                                     }

}

void MainWindow::on_combox_time_listen()
{
        if(ui->combox_time->currentIndex()==0)          ui->edit_fan->setText("低速");
            else if(ui->combox_time->currentIndex()==1) ui->edit_fan->setText("低速");
            else if(ui->combox_time->currentIndex()==2) ui->edit_fan->setText("低速");
            else if(ui->combox_time->currentIndex()==3) ui->edit_fan->setText("低速");
}
