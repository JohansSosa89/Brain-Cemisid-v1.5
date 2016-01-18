#ifndef SUM_QUEUE_H
#define SUM_QUEUE_H
#include<iostream>


///estructura node que contiene dos variable 1) int llamada num y 2) un puntero de tipo node llamada next
struct node{
   int num;
   struct node *next;
};


///estructura queque, contiene 2 variables 1) puntero de tipo node llamado foward y 2) puntero de tipo node llamado back
struct queue{
   node *foward;
   node *back;

   queue(){
       foward = NULL;
       back = NULL;
   }
};

class SumQueue{
    public:
       void enqueue(struct queue &q, int value);
       int dequeue(struct queue &q);
       void showQueue(struct queue q);
       int queueLenght(struct queue q);
       void clearQueue(struct queue &q);
};
#endif // SUM_QUEUE_H
