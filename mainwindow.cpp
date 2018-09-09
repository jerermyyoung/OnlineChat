#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->listWidget->setFocusPolicy(Qt::NoFocus);
    ui->lineEdit->setFocusPolicy(Qt::NoFocus);

    connect(ui->pushButton,SIGNAL(clicked(bool)),this,SLOT(StartNew()));
    connect(ui->pushButton_2,SIGNAL(clicked(bool)),this,SLOT(StartOld()));
    connect(ui->pushButton_3,SIGNAL(clicked(bool)),this,SLOT(Help()));

    mainclient.setPrivacy(true);
    QString add;
    QList<QHostAddress> addList=QNetworkInterface::allAddresses();
    int n=addList.size();
    for(int i=0;i<n;i++)
    {
        if(addList.at(i)!=QHostAddress::LocalHost&&addList.at(i).toIPv4Address())
        {
            add=addList.at(i).toString();
            ui->lineEdit->setText(add);
            break;
        }
    }
    if(add.isEmpty())
    {
        add=QHostAddress(QHostAddress::LocalHost).toString();
        ui->lineEdit->setText(add);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::StartNew()
{
    newpage=new NewChat(this);
    connect(newpage,SIGNAL(StartChat(QString &)),this,SLOT(GetNew(QString &)));
    newpage->show();
}

void MainWindow::StartOld()
{
    QListWidgetItem *select=ui->listWidget->currentItem();
    if(select!=nullptr)
    {
        mainclient.setPrivacy(true);
        chatbox=new ChatPage(this,mainclient);
        QString title=select->text();
        chatbox->setChat(title);
        chatbox->show();
    }
    else {
        mainclient.setPrivacy(false);
        chatbox=new ChatPage(this,mainclient);
        chatbox->show();
    }
}

void MainWindow::GetNew(QString &s)
{
    ui->listWidget->addItem(s);
}

void MainWindow::Help()
{
    manual=new HelpPage(this);
    manual->show();
}
