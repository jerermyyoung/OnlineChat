#include "newchat.h"
#include "ui_newchat.h"

NewChat::NewChat(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewChat)
{
    ui->setupUi(this);
    scan=new QProcess();
    scan->start("arp -a");
    connect(ui->pushButton_4,SIGNAL(clicked(bool)),this,SLOT(addnew()));
    connect(ui->pushButton,SIGNAL(clicked(bool)),this,SLOT(ChooseOne()));
    connect(ui->pushButton_2,SIGNAL(clicked(bool)),this,SLOT(DeleteOne()));
    connect(ui->pushButton_3,SIGNAL(clicked(bool)),this,SLOT(ChooseFinished()));
    connect(scan,SIGNAL(finished(int)),this,SLOT(ScanFinished()));
}

NewChat::~NewChat()
{
    delete ui;
}

void NewChat::ScanFinished()
{
    QByteArray output,ip1;
    int pos1,pos2,beg=0;
    output=scan->readAllStandardOutput();
    while(true) {
        pos1=output.indexOf('(',beg);
        pos2=output.indexOf(')',beg);
        beg=pos2+1;
        if(pos1==-1||pos2==-1) break;
        ip1.append(output.mid(pos1+1,pos2-pos1-1));
        ui->listWidget->addItem(QString::fromLocal8Bit(output.mid(pos1+1,pos2-pos1-1)));
        ip1.append('\n');
    }
}

void NewChat::addnew()
{
    addn=new AddIP(this);
    connect(addn,SIGNAL(IPadded(QString)),this,SLOT(addnewIP(QString)));
    addn->show();
}

void NewChat::addnewIP(const QString &s)
{
    ui->listWidget->addItem(s);
}

void NewChat::ChooseOne()
{
    QListWidgetItem *chosen=ui->listWidget->currentItem();
    ui->listWidget_2->addItem(chosen->text());
}

void NewChat::DeleteOne()
{
    ui->listWidget_2->takeItem(ui->listWidget_2->currentRow());
}

void NewChat::ChooseFinished()
{
    int num=ui->listWidget_2->count();
    QListWidgetItem *temp;
    QString name;
    for(int i=0;i<num;i++)
    {
        temp=ui->listWidget_2->item(i);
        if(i==0)
            name=name+temp->text();
        else
            name=name+','+temp->text();
    }
    emit StartChat(name);
    done(0);
}
