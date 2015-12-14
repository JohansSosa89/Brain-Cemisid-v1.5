#ifndef SHOWTHINKINGWRITED_H
#define SHOWTHINKINGWRITED_H

#include <QDialog>

namespace Ui {
class showThinkingwrited;
}

class showThinkingwrited : public QDialog
{
    Q_OBJECT

public:
    explicit showThinkingwrited(QWidget *parent = 0);
    ~showThinkingwrited();

private:
    Ui::showThinkingwrited *ui;
    void convertNumbertoWord(int category);
};

#endif // SHOWTHINKINGWRITED_H
