#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QTcpServer>
#include <QTimer>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
protected:
    void init();

private slots:

    void newListen();//建立TCP监听事件

    void acceptConnection();//接受客户端连接

    void displayError(QAbstractSocket::SocketError);//显示错误信息

    void on_sendButton_clicked();

    void on_closeButton_clicked();

    void revData();

private:
    Ui::MainWindow *ui;

    QTcpSocket *tcpSocket;
    QTcpServer *tcpServer;
};


#endif // MAINWINDOW_H
