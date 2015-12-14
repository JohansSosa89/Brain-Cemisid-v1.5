#ifndef DIALOGTABLEWORD_H
#define DIALOGTABLEWORD_H
#include<QDialog>
#include<QTableWidget>
#include"Class/neuralNetwork.h"
#include"viewfinder.h"

namespace Ui{
class DialogTableOfWord;
}

enum{ID1,CATEGORY1,CENTER1};

class DialogTableWord : public QDialog{
    Q_OBJECT

public slots:
    void reciveRowSight(QModelIndex index);

public:
    explicit DialogTableWord(QWidget *parent, const NeuralNetwork *neuralSenses, const SizeNet *sizeNet, QString word2);
    void fillLineEditsWord(const SizeNet *sizeNet);
    void fillAllTableWord();
    void fillTableWord(QTableWidget *&table, QString word);
    void createHeaderRow(QTableWidget *table, int rows);
    void paintBinaryCharacteristic(senses sense, int ptr);
    int returnCategory(QString cad);
    ~DialogTableWord();

private:
    Ui::DialogTableOfWord *ui;
    const NeuralNetwork *neuralSenses;
    QMainWindow *ptrMain;
};
#endif // DIALOGTABLEWORD_H

