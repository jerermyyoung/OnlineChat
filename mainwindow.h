#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "newchat.h"
#include "chatpage.h"
#include "helppage.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void StartNew();
    void StartOld();
    void GetNew(QString &s);
    void Help();

private:
    Ui::MainWindow *ui;
    NewChat *newpage;
    ChatPage *chatbox;
    HelpPage *manual;
    Client mainclient;
};

#endif // MAINWINDOW_H
