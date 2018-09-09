#ifndef CHATPAGE_H
#define CHATPAGE_H

#include <QDialog>
#include "client.h"
#include <QTextTableFormat>

namespace Ui {
class ChatPage;
}

class ChatPage : public QDialog
{
    Q_OBJECT

public:
    explicit ChatPage(QWidget *parent,Client &cli);
    ~ChatPage();
    void setChat(QString &s);

public slots:
    void appendMessage(const QString &from, const QString &message);
    void appendImage(const QString &, QByteArray &);
    void saveFile(const QString &, QByteArray &);

private slots:
    void returnPressed();
    void newParticipant(const QString &nick);
    void participantLeft(const QString &nick);
    void sendPic();
    void sendFile();

protected:
     void closeEvent(QCloseEvent *);

private:
    Ui::ChatPage *ui;
    Client *client;
    QString myNickName;
    QTextTableFormat tableFormat;
};

#endif // CHATPAGE_H
