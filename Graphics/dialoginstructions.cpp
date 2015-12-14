#include "dialoginstructions.h"
#include "ui_dialoginstructions.h"

DialogInstructions::DialogInstructions(QWidget *parent) : QDialog(parent), ui(new Ui::DialogInstructions){
    ui->setupUi(this);
}

DialogInstructions::~DialogInstructions(){
    delete ui;
}
