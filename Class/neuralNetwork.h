#ifndef NEURALNETWORK_H
#define NEURALNETWORK_H
#define SIZE_CHARACTERISTIC 32
#define SIZE_FLAGS 9


///Estructura llamada NeuralNetwork en ella se encuentra contenida 4 variables:
/// 1) puntero llamado vectorNeuron de tipo char sin signo
/// 2) puntero llamado vectorFlags de tipo char sin signo
/// 3) puntero llamado ptr de tipo char sin signo
/// 4) puntero llamado binaryCharacteristic de tipo short

struct NeuralNetwork{
    unsigned char *vectorNeuron;
    unsigned int *vectorFlags;
    unsigned char *ptr;
    unsigned short *binaryCharacteristic;
};

///Estructura llamada SizeNet en ella se encuentra contenida 4 variables:
///1) variable de tipo entero sin signo llamda numNeuron
///2) variable de tipo entero sin signo llamada sizeVectorNeuron
/// 3) variable de tipo entero sin signo llamada sizeVectorFlags
/// 4) variable de tipo entero sin signo llamada sizeBinaryCharacteristic

struct SizeNet{
    unsigned int numNeuron;
    unsigned int sizeVectorNeuron;
    unsigned int sizevectorFlags;
    unsigned int sizeBinaryCharacteristic;
};

enum senses{SIGHT, HEARING, SMELL, TOUCH, TASTE};
enum stateNeuralNetwork{IS_HIT,NO_HIT,DIFF};
//     0   1   2   3   4  5   6   7   8
enum{KNW,HIT,DGR,DIS,MBR,CAT,CON,RAT,RED};
#endif // NEURALNETWORK_H
