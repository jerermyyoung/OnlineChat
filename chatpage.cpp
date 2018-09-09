#include "chatpage.h"
#include "ui_chatpage.h"
#include <QtWidgets>

ChatPage::ChatPage(QWidget *parent, Client &cli) :
    QDialog(parent),
    ui(new Ui::ChatPage)
{
    ui->setupUi(this);
    QIcon pic(":/new/icon/pic.jpg");
    ui->toolButton->setIcon(pic);
    QIcon fil(":/new/icon/file.jpg");
    ui->toolButton_2->setIcon(fil);
    client=&cli;
    connect(ui->lineEdit,SIGNAL(returnPressed()),this,SLOT(returnPressed()));
    connect(client, SIGNAL(newMessage(QString,QString)),
            this, SLOT(appendMessage(QString,QString)));
    connect(client, SIGNAL(newImage(const QString&,QByteArray&)),
            this, SLOT(appendImage(const QString&,QByteArray&)));
    connect(client, SIGNAL(newFile(const QString&,QByteArray&)),
            this, SLOT(saveFile(const QString&,QByteArray&)));
    connect(client, SIGNAL(newParticipant(QString)),
            this, SLOT(newParticipant(QString)));
    connect(client, SIGNAL(participantLeft(QString)),
            this, SLOT(participantLeft(QString)));
    connect(ui->toolButton,SIGNAL(clicked(bool)),this,SLOT(sendPic()));
    connect(ui->toolButton_2,SIGNAL(clicked(bool)),this,SLOT(sendFile()));

    myNickName = client->nickName();
    newParticipant(myNickName);
    tableFormat.setBorder(0);
}

ChatPage::~ChatPage()
{
    delete ui;
}

void ChatPage::setChat(QString &s)
{
    ui->textBrowser->setText(s);
    if(ui->textBrowser->toPlainText().isEmpty()) client->setPrivacy(false);
    else
    {
        client->setPrivacy(true);
        QString toip=ui->textBrowser->toPlainText();
        QString tempip;
        int pos,beg=0;
        while(true) {
            pos=toip.indexOf(',',beg);
            tempip=toip.mid(beg,pos);
            QHostAddress realip(tempip);
            client->addList(realip);
            beg=pos+1;
            if(pos==-1) break;
        }
    }
}

void ChatPage::appendMessage(const QString &from, const QString &message)
{
    if (from.isEmpty() || message.isEmpty())
        return;

    QTextCursor cursor(ui->textEdit->textCursor());
    cursor.movePosition(QTextCursor::End);
    QTextTable *table = cursor.insertTable(1, 2, tableFormat);
    table->cellAt(0, 0).firstCursorPosition().insertText('<' + from + "> ");
    table->cellAt(0, 1).firstCursorPosition().insertText(message);
    QScrollBar *bar = ui->textEdit->verticalScrollBar();
    bar->setValue(bar->maximum());
}

void ChatPage::appendImage(const QString &from, QByteArray &message)
{
    if (from.isEmpty() || message.isEmpty())
        return;

    QBuffer buf(&message);
    buf.open(QIODevice::ReadOnly);
    QImageReader reader(&buf,"PNG");
    QImage image=reader.read();
    QImage tosend=image.scaledToWidth(180);
    QTextCursor cursor(ui->textEdit->textCursor());
    cursor.movePosition(QTextCursor::End);
    QTextTable *table = cursor.insertTable(1, 2, tableFormat);
    table->cellAt(0, 0).firstCursorPosition().insertText('<' + from + "> ");
    table->cellAt(0, 1).firstCursorPosition().insertImage(tosend);
    QScrollBar *bar = ui->textEdit->verticalScrollBar();
    bar->setValue(bar->maximum());
}

void ChatPage::returnPressed()
{
    QString text = ui->lineEdit->text();
    if (text.isEmpty())
        return;

    client->sendMessage(text);
    appendMessage(myNickName, text);
    ui->lineEdit->clear();
}

void ChatPage::newParticipant(const QString &nick)
{
    if (nick.isEmpty())
        return;
/*
    QColor color = ui->textEdit->textColor();
    ui->textEdit->setTextColor(Qt::gray);
    ui->textEdit->append(tr("* %1 has joined").arg(nick));
    ui->textEdit->setTextColor(color);
*/
}

void ChatPage::participantLeft(const QString &nick)
{
    if (nick.isEmpty())
        return;

    QColor color = ui->textEdit->textColor();
    ui->textEdit->setTextColor(Qt::gray);
    ui->textEdit->append(tr("* %1 has left").arg(nick));
    ui->textEdit->setTextColor(color);

}

void ChatPage::sendPic()
{
    QString file = QFileDialog::getOpenFileName(this, tr("Select an image"),
                                     ".", tr("Bitmap Files (*.bmp)\n"
                                       "JPEG (*.jpg *jpeg)\n"
                                       "GIF (*.gif)\n"
                                       "PNG (*.png)\n"));
    QImage image=QImageReader(file).read();
    QByteArray ba;
    QBuffer buffer(&ba);
    buffer.open(QIODevice::WriteOnly);
    image.save(&buffer, "PNG");
    client->sendImage(ba);
    appendImage(myNickName,ba);
}

void ChatPage::sendFile()
{
    QString filename=QFileDialog::getOpenFileName(this, tr("Select a file"),".");
    QFile file(filename);
    file.open(QIODevice::ReadOnly);
    QByteArray ba;
    ba=file.readAll();
    int pos=filename.lastIndexOf('.');
    QString extname=filename.mid(pos);
    client->sendMessage("The file extension is "+extname+"\nPlease follow it to change file extension name.");
    client->sendFile(ba);
}

void ChatPage::closeEvent(QCloseEvent *)
{
    QString towho=ui->textBrowser->toPlainText();
    QDateTime time = QDateTime::currentDateTime();
    QString when=time.toString("yyyy-MM-dd-hh-mm-ss");
    QString memoname=towho+' '+when+".htm";
    QFile memo(memoname);
    if(!memo.open(QIODevice::WriteOnly|QIODevice::Text))
    {
        return;
    }
    QTextStream out(&memo);
    out<<ui->textEdit->toHtml();
}

void ChatPage::saveFile(const QString &from, QByteArray &message)
{
    QFileDialog dialog(this);
    dialog.setWindowModality(Qt::WindowModal);
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    if (dialog.exec() != QDialog::Accepted)
        return;
    QString saveto=dialog.selectedFiles().first();
    if(!saveto.isEmpty())
    {
        QFile file(saveto);
        file.open(QIODevice::WriteOnly);
        file.close();
        file.open(QIODevice::ReadWrite);
        file.write(message);
        file.close();
    }
}
