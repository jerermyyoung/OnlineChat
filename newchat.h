#ifndef NEWCHAT_H
#define NEWCHAT_H

#include <QDialog>
#include <QHostAddress>
#include <QHostInfo>
#include <QNetworkInterface>
#include <QList>
#include <QProcess>
#include "addip.h"

namespace Ui {
class NewChat;
}

class NewChat : public QDialog
{
    Q_OBJECT

public:
    explicit NewChat(QWidget *parent = 0);
    ~NewChat();

signals:
    void StartChat(QString &s);

private slots:
    void ScanFinished();
    void addnew();
    void addnewIP(const QString &s);
    void ChooseOne();
    void DeleteOne();
    void ChooseFinished();

private:
    Ui::NewChat *ui;
    QProcess *scan;
    AddIP *addn;
};

#endif // NEWCHAT_H
