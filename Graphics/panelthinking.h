#ifndef PANELTHINKING_H
#define PANELTHINKING_H

#include <QDialog>
#include"viewfinder.h"
#include"Class/neuralNetwork.h"
#include"Graphics/showthinkingwrited.h"

namespace Ui {
class panelthinking;
}

class panelthinking : public QDialog
{
    Q_OBJECT

public:
    explicit panelthinking(QWidget *parent, const NeuralNetwork *neuralSenses);
    void paintBinaryCharacteristic(senses sense, int ptr);
    void paintBinaryNetSyllab(int ID, int Category);
    void showThinkingWrited(int category);
    void setSenses(senses setsense);
    void setPtr(int setptr);
    void setCategory(int setCategory);
    senses getSenses();
    int getPtr();
    int getCategory();
    ~panelthinking();


private slots:
    void on_pushButton_3_clicked();

    void on_btnthinkword_clicked();

private:
    Ui::panelthinking *ui;
    const NeuralNetwork *neuralSenses;
    showThinkingwrited *showthinkingwrited;
    senses sense;
    int ptr;
    int categoryNeuron;
    int hit = 0;

};

#endif // PANELTHINKING_H
