#include "dialogconsultneuron.h"
#include "ui_dialogconsultneuron.h"

DialogConsultNeuron::DialogConsultNeuron(const unsigned char *ptrHearing,const unsigned char* ptrSight, QWidget *parent) : QDialog(parent), ui(new Ui::DialogConsultNeuron){
    std::cout<<"DialogConsultNeuron::DialogConsultNeuron"<<std::endl;

    ui->setupUi(this);
    ptrNeuron = new unsigned char [2];
    ptrNeuron[HEARING] = *ptrHearing;
    ptrNeuron[SIGHT] = *ptrSight;
    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    if(ptrNeuron[SIGHT] == 0){
        ui->radioButtonSight->setEnabled(false);
        ui->radioButtonHearing->setChecked(true);
        ui->radioButtonHearing->clicked(true);
    }else if(ptrNeuron[HEARING] == 0){
        ui->radioButtonHearing->setEnabled(false);
        ui->radioButtonSight->clicked(true);
    }else
        ui->radioButtonSight->clicked(true);
}

senses DialogConsultNeuron::radioButtonActive(){
    std::cout<<"DialogConsultNeuron::radioButtonActive"<<std::endl;
    if(ui->radioButtonSight->isChecked())
        return SIGHT;
    return HEARING;
}

int DialogConsultNeuron::returnIdNeuron(){
    std::cout<<"DialogConsultNeuron::returnIdNeuron"<<std::endl;

    return(ui->lineEdit->text().toInt());
}

void DialogConsultNeuron::setLimits(QString text){
    std::cout<<"DialogConsultNeuron::setLimits"<<std::endl;

    ui->labelLimits->setText(text);
}

void DialogConsultNeuron::setRegularExp(unsigned char number){
    std::cout<<"DialogConsultNeuron::setRegularExp"<<std::endl;

    QString exp("\\d{0,");
    QString stringNumber= QString::number(number);
    exp.append(QString::number(stringNumber.size()));
    exp.append("}");
    QRegExp regExp(exp);
    ui->lineEdit->setValidator(new QRegExpValidator(regExp,this));
}

DialogConsultNeuron::~DialogConsultNeuron(){
    delete ui;
}

QString DialogConsultNeuron::textLimits(const unsigned char number){
    std::cout<<"DialogConsultNeuron::textLimits"<<std::endl;

    QString text("<font color ='red'> Introduzca un n&uacute;mero natural menor a ");
    text.append(QString::number(number));
    text.append(" </ font>");
    return text;
}

void DialogConsultNeuron::changeSight(){
    std::cout<<"DialogConsultNeuron::changeSigth"<<std::endl;

    ui->lineEdit->clear();
    ui->labelLimits->setText(textLimits(ptrNeuron[SIGHT]));
    setRegularExp(ptrNeuron[SIGHT]);
}

void DialogConsultNeuron::changeHearing(){
    std::cout<<"DialogConsultNeuron::changeHearing"<<std::endl;

    ui->lineEdit->clear();
    ui->labelLimits->setText(textLimits(ptrNeuron[HEARING]));
    setRegularExp(ptrNeuron[HEARING]);
}

void DialogConsultNeuron::checkText(){
    std::cout<<"DialogConsultNeuron::checkText"<<std::endl;

    if(ui->lineEdit->text().isEmpty()){
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
        return;
    }
    int maxLimit;
    int value = ui->lineEdit->text().toInt();
    if(ui->radioButtonSight->isChecked())
        maxLimit=ptrNeuron[SIGHT];
    else
        maxLimit=ptrNeuron[HEARING];
    if(value < 0 || value >= maxLimit)
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    else
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
}
