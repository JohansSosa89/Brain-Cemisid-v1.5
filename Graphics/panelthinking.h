#ifndef PANELTHINKING_H
#define PANELTHINKING_H

#include <QDialog>

namespace Ui {
class panelthinking;
}

class panelthinking : public QDialog
{
    Q_OBJECT

public:
    explicit panelthinking(QWidget *parent = 0);
    ~panelthinking();

private:
    Ui::panelthinking *ui;
};

#endif // PANELTHINKING_H
