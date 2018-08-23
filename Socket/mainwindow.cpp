#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QString>
#include <QMessageBox>
#include <QDateTime>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    init();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::init()
{
    this->tcpSocket = new QTcpSocket(this);

    this->setWindowTitle("Client");

    //newTCPConnect();

    connect(tcpSocket,SIGNAL(readyRead()),this,SLOT(revData()));

    connect(tcpSocket,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(displayError(QAbstractSocket::SocketError)));

    connect(tcpSocket,SIGNAL(connected()),this,SLOT(displaySuc()));
}

void MainWindow::revData()
{
    //QString datas = tcpSocket->readAll();

    QByteArray array = tcpSocket->readAll();
    QDateTime date = QDateTime::currentDateTime();
    QString str1 = date.toString("yyyy-MM-dd HH:mm:ss");
    ui->content->append("[Server:"+str1+QString("]\n")+array);
}

void MainWindow::newTCPConnect()
{
    tcpSocket->abort();
    tcpSocket->connectToHost("127.0.0.1",6666);
}

void MainWindow::displayError(QAbstractSocket::SocketError)
{
    qDebug()<<tcpSocket->errorString();

    ui->content->append("[Error:]"+tcpSocket->errorString());

    tcpSocket->close();
}

void MainWindow::on_connectButton_clicked()
{
    tcpSocket->abort();
    if(ui->IPEdit->text().count()&&ui->PortEdit->text().count())
    {
        quint16 port = ui->PortEdit->text().toInt();
        tcpSocket->connectToHost(ui->IPEdit->text(), port);
    }
    else
    {
        QMessageBox::warning(this,"Warning","IP或端口不能为空");
    }
}

void MainWindow::displaySuc()
{
    ui->content->append("The connection server is successful.");
}
void MainWindow::on_sendButton_clicked()
{
    if(ui->ContentForSending->toPlainText().count())
    {
        QString str = ui->ContentForSending->toPlainText();
        QDateTime date = QDateTime::currentDateTime();
        QString str1 = date.toString("yyyy-MM-dd HH:mm:ss");
        ui->content->append("[Client:"+str1+QString("]\n")+str);
        //给对方发送消息，当有中文时需要使用Utf8格式
        tcpSocket->write(str.toUtf8().data());
        ui->ContentForSending->clear();
    }
    else
    {
        QMessageBox::warning(this,"Warning","发送内容不能为空!");
    }
}

void MainWindow::on_closeButton_clicked()
{
    if(tcpSocket->ConnectedState==3)
        tcpSocket->disconnectFromHost();
    tcpSocket->close();
    this->close();
}
