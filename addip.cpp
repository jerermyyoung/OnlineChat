#include "addip.h"
#include "ui_addip.h"

AddIP::AddIP(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddIP)
{
    ui->setupUi(this);
    connect(ui->buttonBox,SIGNAL(accepted()),this,SLOT(gotit()));
}

AddIP::~AddIP()
{
    delete ui;
}

void AddIP::gotit()
{
    emit IPadded(ui->lineEdit->text());
}
