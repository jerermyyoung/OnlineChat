#ifndef HELPPAGE_H
#define HELPPAGE_H

#include <QDialog>

namespace Ui {
class HelpPage;
}

class HelpPage : public QDialog
{
    Q_OBJECT

public:
    explicit HelpPage(QWidget *parent = 0);
    ~HelpPage();

private:
    Ui::HelpPage *ui;
};

#endif // HELPPAGE_H
