#include"interface.h"

//Inicializa las variables de la clase interface

Interface::Interface(){
    arrayCategory = NULL;  //vector categoria
    id  = NULL;  //id
    hits = new int;  //cantidad de neuronas que hicieron hits
}




void Interface::setHit(){
    arrayCategory = new unsigned char [*hits];  //creacion de un vector de tipo char con tamaño igual al valor de
                                                //la variable a la que apunta hits
    id = new unsigned char [*hits];
}


//liberar memoria de la clase interface

void Interface::freeMem(bool deleteAll){
    if(arrayCategory != NULL)
        delete arrayCategory;
    if(id != NULL)
        delete id;
    if(deleteAll)
        delete hits;
}
