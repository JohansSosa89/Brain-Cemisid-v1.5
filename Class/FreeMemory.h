#ifndef FREEMEMORY_H
#define FREEMEMORY_H
#include<QString>
//#include<QImage>
#include"Class/neuralNetwork.h"
#include"Class/interface.h"
#include"Class/culturalNet.h"

class FreeMemory
{
public:
    FreeMemory();
    void freeCulturalNet(CulturalNet *addNet);
    void freeVectorsCharacteristic(unsigned char *characteristicVectorEar, unsigned char *characteristicVectorSight);
    void freeStates(stateNeuralNetwork *stateSenses);
    void freeSenses(NeuralNetwork *neuralSenses, int numSenses);
    void freeInterface(Interface *interface);
    template <class FT>
    void freeFormTeaching(FT **formsTeaching, int numSenses, bool *isInactivateSense);
    template <class T>
    void freeGenericPtr(T *ptr);


};

template <class FT>
void FreeMemory::freeFormTeaching(FT **formsTeaching, int numSenses, bool *isInactivateSense){
    if(formsTeaching != NULL){
        for(int i = 0; i < numSenses; i++)
            delete formsTeaching[i];
        delete formsTeaching;
    }
    freeGenericPtr(isInactivateSense);
}

template <class T>
void FreeMemory::freeGenericPtr(T *ptr){
    if(ptr!= NULL)
        delete ptr;
}

#endif // FREEMEMORY_H
