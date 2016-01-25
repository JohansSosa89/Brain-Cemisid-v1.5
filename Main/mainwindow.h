#ifndef MAINWINDOW_H
#define MAINWINDOW_H
// QT includes áéíóú
#include<QMainWindow>
#include<QProcess>
#include<QMessageBox>
#include<QString>
//c++
#include<iostream>
#include<fstream>
#include<new>
// CUDA-C includes
#include</usr/local/cuda-7.5/include/cuda.h>
#include</usr/local/cuda-7.5/include/cuda_runtime.h>
#include<CUDA/helper_cuda.h>
// My includes
#include"Graphics/dialogconsultneuron.h"
#include"Graphics/dialogselecthardware.h"
#include"Graphics/dialogstatistics.h"
#include"Graphics/dialogtablebns.h"
#include"Graphics/dialogtableword.h"
#include"Graphics/formteaching.h"
#include"Graphics/chemicalLayer.h"
#include"Graphics/viewfinder.h"
#include"Graphics/dialoginstructions.h"
#include"Class/neuralNetwork.h"
#include"Class/interface.h"
#include"Class/statistic.h"
#include"Class/relationNetwork.h"
#include"Class/countNetwork.h"
#include"Class/culturalNet.h"
#include"Class/sumQueue.h"
#include"Class/sumNetwork.h"
#include"Graphics/showthinkingwrited.h"
#include"Graphics/panelthinking.h"
#include"Class/FreeMemory.h"

namespace Ui{
class MainWindow;
}

class MainWindow : public QMainWindow{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:
    void cross();

public slots :
    void showSelectDevice(bool isVisibleButton = true);
    void processGrid();
    void clearTables();
    void learnHearing();
    void learnSight();
    void resetHearing();
    void resetSight();
    void clickBum();
    void activateButtonBum();
    void activateButtonBipWord();
    void activateButtonBipSyl();
    void activateButtonCheck();
    void activateButtonBNSWord();
    void activateButtonImage();
    void activateButtonImages();
    void activateButtonCorrectWord();
    void activateButtonSoundWord();
    void activeLayers(bool active = false);
    void finishGoodAnswer(senses sense);
    void paintNetNeuron(senses sense, bool onlyHits=true);
    void paintBinaryCharacteristic(senses sense, int ptr);
    void paintStateHearing();
    void paintStateSight();
    void paintHearing();
    void paintSight();
    void activateMainWindow(bool activate = true);
    void showDialogConsult();
    void showDialogStatistic();
    void showDialogTableNeuron();
    void showDialogInstructions();
    void aboutBrainCemisid();
    void aboutQt();
    void aboutCuda();
    void launchWave();
    void runCrossing();
    void countProtocol();
    void orderProtocol();
    void activateButtonBip();
    void deactivateBip();
    void startCount();
    void stopCount();
    void clearWord();
    void clickBipSyl();
    void protocolCheck();
    void paintBinaryNetWord();
    void paintBinaryNetSyllab();
    void paintBinarySyllab(int ptr);
    void paintBinaryWord(int ptr1, int ptr2, int ptr3, int ptr4, int ptr5, int ptr6, int ptr7, int ptr8);
    void paintPng();
    void generatePng(QString nameFile);
    void getPtr();
    void paintNetNeuronWord();
    void saveSight();
    void getSight();
    void fillTable();
    void file();
    void generateDotWord(QString nameFile, QString wo);
    int returnID(QString sy);
    void correctWord();
    //johan sosa//
    void showPanelThinking(senses sense, int ptr, int categoryNeuron, queue queue_result, queue queue_id, int idFrm);
    int getNumberNeurons();
    int obtainID(int size, int Digito);


private slots:
    void on_pushButtonBip_clicked();
    void on_checkBox_cuento_clicked();

    /*void on_pushButtonBipWord_clicked();

    void on_pushButtonBipSyl_clicked();

    void on_pushButtonProcess_clicked();

    void on_actionInstrucciones_triggered();

    void on_pushButtonImage_clicked();*/

private:
    Ui::MainWindow *ui;
    QImage *image;
    ChemicalLayer *chemicalLayerEar;
    ChemicalLayer *chemicalLayerEye;
    FormTeaching **formsTeaching;
    DialogConsultNeuron *dialogConsult;
    DialogSelectHardware *dialogSelectHardware;
    DialogStatistics *dialogStatistics;
    DialogTableBNS *dialogTable;
    DialogTableWord *dialogTableWord;
    DialogInstructions *dialogInstructions;
    panelthinking *panelThinking;
    NeuralNetwork *neuralSenses;
    Statistic *statistics;
    stateNeuralNetwork *stateSenses;
    Interface *interface;
    SizeNet sizeNet;
    CountNetwork *countNetwork;
    OrderNetwork *orderNetwork;
    SumQueue *sumQueue;
    SumNetwork *sumNetwork;
    cudaDeviceProp deviceProp;
    RelationNetwork *relationSenses;
    CulturalNet *addNet;
    FreeMemory *freeMemory;
    bool *isInactivateSense;
    int numSenses;
    unsigned char selectedDevice;
    unsigned char *characteristicVectorEar;
    unsigned char *characteristicVectorEye;
    unsigned short *characteristicBinaryVector;
    int kNeuron = 1;
    int orderNeuron = 1;
    unsigned char *aux;
    int sum_loop = 0;
    int kAux = 0;
    int bum = 0;
    QString word, word2, word3;
    QString syllab;
    int nsyllab = 0, goodHit = 0;
    int finishWord = 0;
    int ptr1 = 999, ptr2 = 999, ptr3 = 999, ptr4 = 999, ptr5 = 999, ptr6 = 999, ptr7 = 999, ptr8 = 999;
    struct queue adding_up;
    struct queue adding_down;
    struct queue result;
    struct queue id_queue;
    unsigned int *vectorCategory;
    unsigned int *vectorId;
    int IdForm = 0;
    struct queue word_count;
    void initGui();
    void activateInterface(bool state);
    void setNull();
   /* void freeVectorsCharacteristic();
    void freeFormTeaching();
    void freeStates();
    void freeSenses();
    void freeInterface();*/
    void freeUi();
    void freeMem();
    void generateVectorsCharacteristic();
    void createTablesCharacteristic();
    void initializeTable();
    void intitializeSenses(int numSenses = 2); // Dos sentidos vista y oido
    void createStringForTable();
    void earTraining();
    void senseTraining(QString nameFileCategories, QString nameFileWaves, QString path, int numPatterns, senses sense);
    void setDataTable(QStringList listStringRow, QStringList listStringColumn);
    int calculateNumNeuron();
    void createInterfacesTeaching();
    void setFormsCheck(bool state = false);
    void learn(senses sense);
    void realLearn(senses sense);
    void realLearnPre(int ca, int ra, unsigned short val1, unsigned short val2, unsigned short val3, unsigned short val4, unsigned short val5, unsigned short val6, unsigned short val7, unsigned short val8, unsigned short val9, unsigned short val10, unsigned short val11, unsigned short val12, unsigned short val13, unsigned short val14, unsigned short val15, unsigned short val16, int v1, int v2, int v3, int v4, int v5, int v6, int v7, int v8, int v9, int v10, int v11, int v12, int v13, int v14, int v15, int v16, int v17, int v18, int v19, int v20, int v21, int v22, int v23, int v24, int v25, int v26, int v27, int v28, int v29, int v30, int v31, int v32);
    void learnBinaryCharacteristic(senses sense, int ptr);
    void learnBinaryCharacteristicPre(senses sense, int ptr, unsigned short val1, unsigned short val2, unsigned short val3, unsigned short val4, unsigned short val5, unsigned short val6, unsigned short val7, unsigned short val8, unsigned short val9, unsigned short val10, unsigned short val11, unsigned short val12, unsigned short val13, unsigned short val14, unsigned short val15, unsigned short val16);
    void generateDot(QString nameFile, senses sense, bool onlyHits = true);
    void generateDotSight(QString nameFile, senses sense, bool onlyHits = true);
    void showWarning(QString windowTitle, QString message);
    int returnCategory(QString cad);
    QString returnCategory(int cad);
    unsigned char *returnVectorCategory(senses sense);
    bool multipleDevice();
    //TESIS JULIO
    void actualiceCategory(unsigned char, unsigned char);
    bool ambiguity(unsigned char);
    bool analyticsNeuron();
    void buildRelation(unsigned char);
    void initializeRelation(int);
    unsigned char checkInRelationNet();
//    void think(senses senses);
    void initializeCuturalNet(int);
    void freeCulturalNet();
    /*template<class T>
    void freeGenericPtr(T *ptr);*/
    void printAllVectorSenses();
    void printVectorSenses(senses sense);
    void printIdsNeuronHit(senses sense);
    void printSense(senses sense);
    void printCategory(senses sense);
    void printCountNetwork();
    void paintCount(int times);
    char caracterCla(int category);
    void addition(struct queue &up, struct queue &down);
    //TRABAJO JOHAN SOSA//

};
#endif // MAINWINDOW_H
