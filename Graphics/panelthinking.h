#ifndef PANELTHINKING_H
#define PANELTHINKING_H

#include <QDialog>
#include<QQueue>
#include"viewfinder.h"
#include"Class/neuralNetwork.h"
#include"Class/sumQueue.h"
#include"Graphics/showthinkingwrited.h"
#include<QMediaPlayer>
#include<QMediaPlaylist>


namespace Ui {
class panelthinking;
}

class QMediaPlayer;

class QMediaPlaylist;

class panelthinking : public QDialog
{
    Q_OBJECT

public:
    explicit panelthinking(QWidget *parent, const NeuralNetwork *neuralSenses);
    void paintBinaryCharacteristic(senses sense, int ptr);
    void paintBinaryNetSyllab(int ID, int Category);
    void paintBinaryCharacteristicAddition(senses sense, queue result_queque, queue id_queue);
    void paintBinaryCharacteristicCount(senses sense, int ptr, queue result_queque);
    void showThinkingWrited(int category, queue result_sum);
    void setSenses(senses setsense);
    void setPtr(int setptr);
    void setCategory(int setCategory);
    void setQueque(queue setqueque);
    void setQueque_id(queue setqueque_id);
    void setIdForm(int setIdForm);
    void setVectorId(unsigned int *setVectorId);
    void setVectorCategory(unsigned int *setVectorCategory);
    void PlaySound(int category);
    void playSoundAddition(queue queue_result_sum);
    void playCount(queue word_count, int category);
    QString returnSound(int category);
    template<class T>
    void freeGenericPtr(T *puntero);
    senses getSenses();
    int getPtr();
    int getCategory();
    queue getQueque();
    queue getQueque_id();
    int getIdForm();
    unsigned int getVectorId();
    unsigned int getVectorCategory();
    ~panelthinking();


private slots:
    void on_pushButton_3_clicked();

    void on_btnthinkword_clicked();

    void on_btnSound_clicked();

private:
    Ui::panelthinking *ui;
    const NeuralNetwork *neuralSenses;
    showThinkingwrited *showthinkingwrited;
    SumQueue *Queue;
    senses sense;
    QMediaPlayer *sound;
    int ptr, longitud, value;
    int categoryNeuron;
    int hit = 0;
    struct queue queueThink;
    struct queue queueIdThink;
    struct queue copy_result;
    int idForm;
    QString filename, originfile, text="", number;
    QMediaPlaylist *playList;

};

#endif // PANELTHINKING_H
