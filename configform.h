#ifndef CONFIGFORM_H
#define CONFIGFORM_H

#include <QTabWidget>

namespace Ui {
    class configform;
}

class configform : public QTabWidget
{
    Q_OBJECT

public:
    explicit configform(QWidget *parent = 0);
    ~configform();

private:
    Ui::configform *ui;
};

#endif // CONFIGFORM_H
