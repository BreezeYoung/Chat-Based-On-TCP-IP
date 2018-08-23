#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QMessageBox>
#include <QString>
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
    this->tcpServer = new QTcpServer(this);
    this->tcpSocket = new QTcpSocket(this);
    this->setWindowTitle("Server");

    newListen();

    connect(tcpServer,SIGNAL(newConnection()),this,SLOT(acceptConnection()));

    connect(tcpSocket,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(displayError(QAbstractSocket::SocketError)));

}

void MainWindow::newListen()
{
    //监听是否有客户端来访，且对任何来访者监听，端口为6666
    if(!tcpServer->listen(QHostAddress::Any,6666))
    {
        qDebug()<<tcpServer->errorString();
        close();
        return;
    }
}

void MainWindow::acceptConnection()
{
    tcpSocket = tcpServer->nextPendingConnection();
    QString ip = tcpSocket->peerAddress().toString();
    quint16 port = tcpSocket->peerPort();
    ui->content->append(QString("[%1:%2]:The new connection is established in this server.").arg(ip).arg(port));
    connect(tcpSocket,SIGNAL(readyRead()),this,SLOT(revData()));
}

void MainWindow::displayError(QAbstractSocket::SocketError)
{
    qDebug()<<tcpSocket->errorString();
    tcpSocket->close();
}

void MainWindow::on_sendButton_clicked()
{
    if(ui->contentForSending->toPlainText().count())
    {
        QString str = ui->contentForSending->toPlainText();
        QDateTime date = QDateTime::currentDateTime();
        QString str1 = date.toString("yyyy-MM-dd HH:mm:ss");
        ui->content->append("[Server:"+str1+QString("]\n")+str);
        //给对方发送消息，当有中文时需要使用Utf8格式
        tcpSocket->write(str.toUtf8().data());
        ui->contentForSending->clear();
    }
    else
    {
        QMessageBox::warning(this,"Warning","发送内容不能为空");
    }
}

void MainWindow::on_closeButton_clicked()
{
    this->close();
}

void MainWindow::revData()
{
    QByteArray array = tcpSocket->readAll();
    QDateTime date = QDateTime::currentDateTime();
    QString str1 = date.toString("yyyy-MM-dd HH:mm:ss");
    ui->content->append("[Client:"+str1+QString("]\n")+array);
}
