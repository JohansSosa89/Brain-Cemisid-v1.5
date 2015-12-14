#ifndef SUMNETWORK_H
#define SUMNETWORK_H
#define SIZE_CHARACTERISTIC 32
#define SIZE_FLAGS 9


///estructura llamada SumNetwork, contiene 2 variables 1) puntero de tipo char sin signo llamada vectorNetworkSum
/// 2) puntero de tipo char sin signo llamado vectorPointerSum

struct SumNetwork{
    unsigned char *vectorNetworkSum;
    unsigned char *vectorPointerSum;
};
#endif // SUMNETWORK_H
