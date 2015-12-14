#include "panelthinking.h"
#include "ui_panelthinking.h"

panelthinking::panelthinking(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::panelthinking)
{
    ui->setupUi(this);
}

panelthinking::~panelthinking()
{
    delete ui;
}
