#ifndef ADDIP_H
#define ADDIP_H

#include <QDialog>

namespace Ui {
class AddIP;
}

class AddIP : public QDialog
{
    Q_OBJECT

public:
    explicit AddIP(QWidget *parent = 0);
    ~AddIP();

signals:
    void IPadded(const QString &s);
private slots:
    void gotit();

private:
    Ui::AddIP *ui;
};

#endif // ADDIP_H
