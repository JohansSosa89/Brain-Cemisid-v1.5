#include "showthinkingwrited.h"
#include "ui_showthinkingwrited.h"

showThinkingwrited::showThinkingwrited(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::showThinkingwrited)
{
    ui->setupUi(this);
}

showThinkingwrited::~showThinkingwrited()
{
    delete ui;
}

void showThinkingwrited::setcategory(int setcategory){
    this->category = setcategory;
}

int showThinkingwrited::getcategory(){
    return category;
}

void showThinkingwrited::setQueue(queue result){
    this->result= result;
}

queue showThinkingwrited::getQueue(){
    return result;
}

QString showThinkingwrited::returnWord(int categoria){
    switch (categoria) {
    case 0:
        return word = "CERO";
        break;

    case 1:
        return word = "UNO";
        break;

    case 2:
        return word = "DOS";
        break;

    case 3:
        return word = "TRES";
        break;

    case 4:
        return word = "CUATRO";
        break;

    case 5:
        return word = "CINCO";
        break;

    case 6:
        return word = "SEIS";
        break;

    case 7:
        return word = "SIETE";
        break;

    case 8:
        return word = "OCHO";
        break;

    case 9:
        return word = "NUEVE";
        break;
    case 41:
        return word = "COCO";
        break;
    case 42:
        return word = "CABALLO";
        break;
    case 43:
        return word = "+";
        break;
    case 44:
        return word = "CASA";
        break;
    case 45:
        return word = "SAPO";
        break;
    case 46:
        return word = "GATO";
        break;
    case 47:
        return word = "PATO";
        break;
    case 48:
        return word = "CARRO";
        break;
    case 49:
        return word = "GALLO";
        break;
    case 50:
        return word = "CULEBRA";
        break;
    case 51:
        return word = "LEON";
        break;
    case 52:
        return word = "VENADO";
        break;
    case 53:
        return word = "AVION";
        break;
    case 54:
        return word = "COBRA";
        break;
    case 56:
        return word = "UNO";
        break;
    case 57:
        return word = "DOS";
        break;
    case 59:
        return word = "TRES";
        break;
    case 61:
        return word = "=";
        break;
    case 62:
        return word = "CUATRO";
        break;
    case 64:
        return word = "CINCO";
        break;
    case 65:
        return word = "A";
        break;
    case 66:
        return word = "B";
        break;
    case 67:
        return word = "C";
        break;
    case 68:
        return word = "D";
        break;
    case 69:
        return word = "E";
        break;
    case 70:
        return word = "F";
        break;
    case 71:
        return word = "G";
        break;
    case 72:
        return word = "H";
        break;
    case 73:
        return word = "I";
        break;
    case 74:
        return word = "J";
        break;
    case 75:
        return word = "K";
        break;
    case 76:
        return word = "L";
        break;
    case 77:
        return word = "M";
        break;
    case 78:
        return word = "N";
        break;
    case 79:
        return word = "O";
        break;
    case 80:
        return word = "P";
        break;
    case 81:
        return word = "Q";
        break;
    case 82:
        return word = "R";
        break;
    case 83:
        return word = "S";
        break;
    case 84:
        return word = "T";
        break;
    case 85:
        return word = "U";
        break;
    case 86:
        return word = "V";
        break;
    case 87:
        return word = "W";
        break;
    case 88:
        return word = "X";
        break;
    case 89:
        return word = "Y";
        break;
    case 90:
        return word = "Z";
        break;
    case 93:
        return word = "SEIS";
        break;
    case 96:
        return word = "SIETE";
        break;
    case 98:
        return word = "OCHO";
        break;
    case 101:
        return word = "NUEVE";
        break;
    case 103:
        return word = "CERO";
        break;
    default:
        return word = "???";
        break;
    }
}

void showThinkingwrited::convertNumbertoWord(int category){

    font = ui->lblshowword->font();
    font.setPointSize(50);
    font.setBold(true);
    ui->lblshowword->setFont(font);
    ui->lblshowword->setText(returnWord(category));

}

void showThinkingwrited::ConvertAdditionOnWord(queue result_sum){

    longitud = Queue->queueLenght(result_sum);
    font = ui->lblshowword->font();
    font.setBold(true);
    switch (longitud) {
    case 1:
        font.setPointSize(50);
        break;
    case 2:
        font.setPointSize(40);
        break;
    case 3:
        font.setPointSize(25);
        break;
    case 4:
        font.setPointSize(20);
        break;
    default:
        font.setPointSize(50);
        break;
    }

    for(int b=0; b<longitud; b++){
        number = Queue->dequeue(result_sum);
        text = text + QString::number(number);
    }

    for(int a=longitud - 1;a>=0; a--){

        word = returnWord(text[a].digitValue());
        result_word = result_word+" "+word;
    }
    ui->lblshowword->setFont(font);
    ui->lblshowword->setText(result_word);
}

void showThinkingwrited::ConvertCountWord(queue word_count, int category){
    longitud = Queue->queueLenght(word_count);
    font = ui->lblshowword->font();
    font.setBold(true);
    font.setPointSize(30);

    value = returnWord(longitud);
    word = returnWord(category);

    if(longitud==1){
        if(word=="CASA"){
            value = "UNA";
        }
        else{
            value = "UN";
        }
        result_word = value+" "+word;
    }else{
        if(word=="AVION"){
            result_word = value+" "+word+"ES";
        }
        else{
            result_word = value+" "+word+"S";
        }

    }

    ui->lblshowword->setFont(font);
    ui->lblshowword->setText(result_word);
}
