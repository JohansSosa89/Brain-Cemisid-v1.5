#include "FreeMemory.h"

FreeMemory::FreeMemory()
{

}

void FreeMemory::freeVectorsCharacteristic(unsigned char *characteristicVectorEar, unsigned char *characteristicVectorSight){

    freeGenericPtr(characteristicVectorEar);
    freeGenericPtr(characteristicVectorSight);
}

void FreeMemory::freeStates(stateNeuralNetwork *stateSenses){
    freeGenericPtr(stateSenses);
}

void FreeMemory::freeSenses(NeuralNetwork *neuralSenses, int numSenses){
    if(neuralSenses != NULL){
        for(register int i = 0; i < numSenses; i++){
            freeGenericPtr(neuralSenses[i].vectorNeuron);
            freeGenericPtr(neuralSenses[i].vectorFlags);
            freeGenericPtr(neuralSenses[i].binaryCharacteristic);
        }
        delete neuralSenses;
    }
}

void FreeMemory::freeInterface(Interface *interface){
    if(interface!= NULL){
        interface[HEARING].freeMem(true);
        interface[SIGHT].freeMem(true);
        delete interface;
    }
}

void FreeMemory::freeCulturalNet(CulturalNet *addNet){

    if(addNet != NULL){
        freeGenericPtr(addNet->data);
        freeGenericPtr(addNet->flipflopKnow);
        freeGenericPtr(addNet->valve);
        delete addNet;
    }
}
