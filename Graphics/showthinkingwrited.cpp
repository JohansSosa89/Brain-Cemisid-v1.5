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

void showThinkingwrited::convertNumbertoWord(int category){

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
    default:
        ui->lblshowword->setText("???");
        break;
    }
}
