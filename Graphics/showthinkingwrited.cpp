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

void showThinkingwrited::convertNumbertoWord(int category){
    std::cout<<""<<std::endl;

    switch (category) {
    case 0:
        ui->lblshowword->setText("CERO");
        break;

    case 1:
        ui->lblshowword->setText("UNO");
        break;

    case 2:
        ui->lblshowword->setText("DOS");
        break;

    case 3:
        ui->lblshowword->setText("TRES");
        break;

    case 4:
        ui->lblshowword->setText("CUATRO");
        break;

    case 5:
        ui->lblshowword->setText("CINCO");
        break;

    case 6:
        ui->lblshowword->setText("SEIS");
        break;

    case 7:
        ui->lblshowword->setText("SIETE");
        break;

    case 8:
        ui->lblshowword->setText("OCHO");
        break;

    case 9:
        ui->lblshowword->setText("NUEVE");
        break;
    case 41:
        ui->lblshowword->setText("COCO");
        break;
    case 42:
        ui->lblshowword->setText("CABALLO");
        break;
    case 43:
        ui->lblshowword->setText("+");
        break;
    case 44:
        ui->lblshowword->setText("CASA");
        break;
    case 45:
        ui->lblshowword->setText("SAPO");
        break;
    case 46:
        ui->lblshowword->setText("GATO");
        break;
    case 47:
        ui->lblshowword->setText("PATO");
        break;
    case 48:
        ui->lblshowword->setText("CARRO");
        break;
    case 49:
        ui->lblshowword->setText("GALLO");
        break;
    case 50:
        ui->lblshowword->setText("CULEBRA");
        break;
    case 51:
        ui->lblshowword->setText("LEON");
        break;
    case 52:
        ui->lblshowword->setText("VENADO");
        break;
    case 53:
        ui->lblshowword->setText("AVION");
        break;
    case 54:
        ui->lblshowword->setText("COBRA");
        break;
    case 61:
        ui->lblshowword->setText("=");
        break;
    case 65:
        ui->lblshowword->setText("A");
        break;
    case 66:
        ui->lblshowword->setText("B");
        break;
    case 67:
        ui->lblshowword->setText("C");
        break;
    case 68:
        ui->lblshowword->setText("D");
        break;
    case 69:
        ui->lblshowword->setText("E");
        break;
    case 70:
        ui->lblshowword->setText("F");
        break;
    case 71:
        ui->lblshowword->setText("G");
        break;
    case 72:
        ui->lblshowword->setText("H");
        break;
    case 73:
        ui->lblshowword->setText("I");
        break;
    case 74:
        ui->lblshowword->setText("J");
        break;
    case 75:
        ui->lblshowword->setText("K");
        break;
    case 76:
        ui->lblshowword->setText("L");
        break;
    case 77:
        ui->lblshowword->setText("M");
        break;
    case 78:
        ui->lblshowword->setText("N");
        break;
    case 79:
        ui->lblshowword->setText("O");
        break;
    case 80:
        ui->lblshowword->setText("P");
        break;
    case 81:
        ui->lblshowword->setText("Q");
        break;
    case 82:
        ui->lblshowword->setText("R");
        break;
    case 83:
        ui->lblshowword->setText("S");
        break;
    case 84:
        ui->lblshowword->setText("T");
        break;
    case 85:
        ui->lblshowword->setText("U");
        break;
    case 86:
        ui->lblshowword->setText("V");
        break;
    case 87:
        ui->lblshowword->setText("W");
        break;
    case 88:
        ui->lblshowword->setText("X");
        break;
    case 89:
        ui->lblshowword->setText("Y");
        break;
    case 90:
        ui->lblshowword->setText("Z");
        break;
    default:
        ui->lblshowword->setText("???");
        break;
    }
}
