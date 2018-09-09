#include "helppage.h"
#include "ui_helppage.h"

HelpPage::HelpPage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HelpPage)
{
    ui->setupUi(this);
    ui->textBrowser->setText("Introduction:\nA simple communication software.\nYou can use it to transfer text,image or small files to "
                             "devices in the same network even if not connected to the Internet.\n\n"
                             "Help:\nClick New Chat to find who is online and add some of them to a new chat."
                             "Use the IP shown below to help determine which device is the one you want to communicate with\n"
                             "If you want to chat with everyone in the same network,just click Start Chat.Otherwise,select one chat "
                             "from the Chat List and click Start Chat.");
}

HelpPage::~HelpPage()
{
    delete ui;
}
