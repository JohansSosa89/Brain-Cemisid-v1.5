#ifndef STATISTIC_H
#define STATISTIC_H
#include<float.h>

///Estructura llamada Statistic contiene 4 variables:
/// 1) numExecutions de tipo entera
/// 2) maxTime de tipo float
/// 3) minTime de tipo float
/// 4) accumulateTime de tipo float
/// por ultimo inicializa todas las variables dentro de la misma estructura

struct Statistic{
    int numExecutions;
    float maxTime;
    float minTime;
    float accumulateTime;
    Statistic(){
        numExecutions = 0;
        maxTime = FLT_MIN;
        minTime = FLT_MAX;
        accumulateTime = 0.0;
    }
};

///dato de tipo enum llamdo kernels los cuales poseen los nombre de los kerneles llamados por el proyecto

enum kernels{BOOT,RECOGNIZE,CORRECT,RESET,SAFERELATION};
#endif // STATISTIC_H
