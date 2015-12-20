#ifndef SHOWTHINKINGWRITED_H
#define SHOWTHINKINGWRITED_H

#include <QDialog>
#include<iostream>

namespace Ui {
class showThinkingwrited;
}

class showThinkingwrited : public QDialog
{
    Q_OBJECT

public:

    explicit showThinkingwrited(QWidget *parent = 0);
    void convertNumbertoWord(int category);
    int getcategory();
    void setcategory(int setcategory);
    ~showThinkingwrited();

private:
    Ui::showThinkingwrited *ui;
    int category;

};

#endif // SHOWTHINKINGWRITED_H
