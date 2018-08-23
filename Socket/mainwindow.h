#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork>
#include <QtNetwork/QTcpSocket>

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
    void newTCPConnect();

private slots:

    void revData();

    void displaySuc();

    void displayError(QAbstractSocket::SocketError);

    void on_connectButton_clicked();

    void on_sendButton_clicked();

    void on_closeButton_clicked();

private:
    Ui::MainWindow *ui;

    QTcpSocket *tcpSocket;
};

#endif // MAINWINDOW_H
