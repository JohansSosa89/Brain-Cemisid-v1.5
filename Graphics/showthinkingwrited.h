#ifndef SHOWTHINKINGWRITED_H
#define SHOWTHINKINGWRITED_H

#include <QDialog>
#include<iostream>
#include"Class/sumQueue.h"

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
    queue getQueue();
    void setQueue(queue result);
    void ConvertAdditionOnWord(queue result_sum);
    QString returnWord(int categoria);
    void ConvertCountWord(queue word_count, int category);
    ~showThinkingwrited();

private:
    Ui::showThinkingwrited *ui;
    int category, longitud, number;
    queue result;
    SumQueue *Queue;
    QString word="", result_word="", value="", text="";
    QFont font;

};

#endif // SHOWTHINKINGWRITED_H
