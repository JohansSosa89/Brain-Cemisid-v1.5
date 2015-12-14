#ifndef DIALOGINSTRUCTIONS_H
#define DIALOGINSTRUCTIONS_H

#include <QDialog>

namespace Ui {
class DialogInstructions;
}

class DialogInstructions : public QDialog
{
    Q_OBJECT

public:
    explicit DialogInstructions(QWidget *parent = 0);
    ~DialogInstructions();

private:
    Ui::DialogInstructions *ui;
};

#endif // DIALOGINSTRUCTIONS_H
