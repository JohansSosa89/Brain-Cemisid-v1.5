#ifndef INTERFACE_H
#define INTERFACE_H
#include<iostream>

struct Interface{
    unsigned char *arrayCategory;    //puntero vector categoria
    unsigned char *id;              //puntero de id
    int *hits;                     //puntero de nro de hits
    Interface();
    void setHit();
    void freeMem(bool deleteAll = false);
};
#endif // INTERFACE_H
