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

void showThinkingwrited::convertNumbertoWord(int category){

    switch (category) {
    case 0:
        word = "CERO";
        break;

    case 1:
        word = "UNO";
        break;

    case 2:
        word = "DOS";
        break;

    case 3:
        word = "TRES";
        break;

    case 4:
        word = "CUATRO";
        break;

    case 5:
        word = "CINCO";
        break;

    case 6:
        word = "SEIS";
        break;

    case 7:
        word = "SIETE";
        break;

    case 8:
        word = "OCHO";
        break;

    case 9:
        word = "NUEVE";
        break;
    case 41:
        word = "COCO";
        break;
    case 42:
        word = "CABALLO";
        break;
    case 43:
        word = "+";
        break;
    case 44:
        word = "CASA";
        break;
    case 45:
        word = "SAPO";
        break;
    case 46:
        word = "GATO";
        break;
    case 47:
        word = "PATO";
        break;
    case 48:
        word = "CARRO";
        break;
    case 49:
        word = "GALLO";
        break;
    case 50:
        word = "CULEBRA";
        break;
    case 51:
        word = "LEON";
        break;
    case 52:
        word = "VENADO";
        break;
    case 53:
        word = "AVION";
        break;
    case 54:
        word = "COBRA";
        break;
    case 56:
        word = "UNO";
        break;
    case 57:
        word = "DOS";
        break;
    case 59:
        word = "TRES";
        break;
    case 61:
        word = "=";
        break;
    case 62:
        word = "CUATRO";
        break;
    case 64:
        word = "CINCO";
        break;
    case 65:
        word = "A";
        break;
    case 66:
        word = "B";
        break;
    case 67:
        word = "C";
        break;
    case 68:
        word = "D";
        break;
    case 69:
        word = "E";
        break;
    case 70:
        word = "F";
        break;
    case 71:
        word = "G";
        break;
    case 72:
        word = "H";
        break;
    case 73:
        word = "I";
        break;
    case 74:
        word = "J";
        break;
    case 75:
        word = "K";
        break;
    case 76:
        word = "L";
        break;
    case 77:
        word = "M";
        break;
    case 78:
        word = "N";
        break;
    case 79:
        word = "O";
        break;
    case 80:
        word = "P";
        break;
    case 81:
        word = "Q";
        break;
    case 82:
        word = "R";
        break;
    case 83:
        word = "S";
        break;
    case 84:
        word = "T";
        break;
    case 85:
        word = "U";
        break;
    case 86:
        word = "V";
        break;
    case 87:
        word = "W";
        break;
    case 88:
        word = "X";
        break;
    case 89:
        word = "Y";
        break;
    case 90:
        word = "Z";
        break;
    case 93:
        word = "SEIS";
        break;
    case 96:
        word = "SIETE";
        break;
    case 98:
        word = "OCHO";
        break;
    case 101:
        word = "NUEVE";
        break;
    case 103:
        word = "CERO";
        break;
    default:
        word = "???";
        break;
    }
    font = ui->lblshowword->font();
    font.setPointSize(50);
    font.setBold(true);
    ui->lblshowword->setFont(font);
    ui->lblshowword->setText(word);

}

void showThinkingwrited::ConvertAdditionOnWord(queue result_sum){

    int value = Queue->queueLenght(result_sum);


    for(int a= value -1;a>=0; a--){
        switch (Queue->dequeue(result_sum)) {
        case 0:
            word = "CERO";
            break;
        case 1:
            word = "UNO";
            break;
        case 2:
            word = "DOS";
            break;
        case 3:
            word = "TRES";
            break;
        case 4:
            word = "CUATRO";
            break;
        case 5:
            word = "CINCO";
            break;
        case 6:
            word = "SEIS";
            break;
        case 7:
            word = "SIETE";
            break;
        case 8:
            word = "OCHO";
            break;
        case 9:
            word = "NUEVE";
            break;
        default:
            break;
        }

        result_word = result_word +" "+ word;
    }
    font = ui->lblshowword->font();
    font.setPointSize(25);
    font.setBold(true);
    ui->lblshowword->setFont(font);
    ui->lblshowword->setText(result_word);
}
