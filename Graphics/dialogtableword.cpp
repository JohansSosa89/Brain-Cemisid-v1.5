#include"dialogtableword.h"
#include"ui_dialogtableword.h"

QString word;
DialogTableWord::DialogTableWord(QWidget *parent, const NeuralNetwork *neuralSenses, const SizeNet *sizeNet, QString word2) : QDialog(parent), ui(new Ui::DialogTableOfWord){
   std::cout<<"DialogTableWord::DialogTableWord"<<std::endl;

    ui->setupUi(this);
    this->neuralSenses = neuralSenses;
    fillLineEditsWord(sizeNet);
    fillAllTableWord();
    word = word2;
    fillTableWord(ui->tableWidget,word2);
}

void DialogTableWord::fillLineEditsWord(const SizeNet *sizeNet){
    std::cout<<"DialogTableWord::fillLineEditsWord"<<std::endl;

    int mem = sizeNet->sizevectorFlags + sizeNet->sizeVectorNeuron + sizeNet->sizeBinaryCharacteristic;
    int maxNeuron = sizeNet->numNeuron;
    ui->lineEditNumNeuron->setText(QString::number(maxNeuron));
    ui->lineEditMem->setText(QString::number(mem));
    ui->lineEditPtr->setText(QString::number(*(neuralSenses[SIGHT].ptr)));
}

void DialogTableWord::fillAllTableWord(){
    std::cout<<"DialogTableWord::fillAllTableWord"<<std::endl;

    connect(ui->tableWidget, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(reciveRowSight(QModelIndex)));
}

void DialogTableWord::fillTableWord(QTableWidget *&table, QString word){
    std::cout<<"DialogTableWord::fillTableWord"<<std::endl;

    int numNeuron = *(neuralSenses[SIGHT].ptr);
    if(numNeuron == 0){
        table->hide();
        return;
    }
    createHeaderRow(table,numNeuron);
    for(int id=0; id<numNeuron; id++){
        if(neuralSenses[SIGHT].vectorFlags[id*9+CAT] == (unsigned int)returnCategory(word)){
            QString vector;
            QTableWidgetItem *newItem = new QTableWidgetItem(QString::number(id));
            newItem->setTextAlignment(Qt::AlignVCenter);
            newItem->setTextAlignment(Qt::AlignHCenter);
            table->setItem(id, ID1, newItem);
            newItem = new QTableWidgetItem(QString::number(neuralSenses[SIGHT].vectorFlags[id*9+CAT]));
            newItem->setTextAlignment(Qt::AlignVCenter);
            newItem->setTextAlignment(Qt::AlignHCenter);
            table->setItem(id,CATEGORY1, newItem);
            for(int j = 0; j < 32; j++){
                vector.append(QString::number(neuralSenses[SIGHT].vectorNeuron[id * 32+j]));
                if(j < 31)
                    vector.append(",");
            }
            newItem = new QTableWidgetItem(vector);
            newItem->setTextAlignment(Qt::AlignVCenter);
            newItem->setTextAlignment(Qt::AlignHCenter);
            table->setItem(id,CENTER1,newItem);
        }else{
            table->hideRow(id);
        }
    }
    table->resizeColumnToContents(ID1);
    table->resizeColumnToContents(CATEGORY1);
    table->resizeColumnToContents(CENTER1);
}

void DialogTableWord::createHeaderRow(QTableWidget *table, int rows){
    std::cout<<"DialogTableWord::createHeaderRow"<<std::endl;

    QStringList list;
    table->setRowCount(rows);
    for(int i=0; i<rows; i++)
        list << QString::number(i);
    table->setVerticalHeaderLabels(list);
}

DialogTableWord::~DialogTableWord(){
    delete ui;
}

void DialogTableWord::reciveRowSight(QModelIndex index){
    std::cout<<"DialogTableWord::reciveRowSight"<<std::endl;

    if(index.column() == CENTER1)
        paintBinaryCharacteristic(SIGHT,ui->tableWidget->item(index.row(),ID1)->text().toInt());
}

void DialogTableWord::paintBinaryCharacteristic(senses sense, int ptr){
    std::cout<<"DialogTableWord::paintBinaryCharacteristic"<<std::endl;

    unsigned short displacement = 8 * sizeof (unsigned short) -1;
    unsigned short mask = 1 << displacement;
    QImage * image = new QImage(QSize(400,400), QImage::Format_MonoLSB);
    image->fill(Qt::color1);
    QPainter paint;
    paint.begin(image);
    QPen pen(QColor(Qt::color1));
    paint.setPen(pen);
    paint.setBrush(QBrush(QColor(Qt::color1), Qt::SolidPattern));
    QString text = word;
    paint.drawText(QRect(100,20,100,100),text);
    for(int i = 0; i < 16; i++){
        unsigned short value = neuralSenses[sense].binaryCharacteristic[ptr * 16 +i];
        for(unsigned short j=0; j <= displacement; j++){
            if(value &mask)
                paint.drawRect(QRect(100+(15-j)*10,50+i*10,8,8));//(x,y) x columnas y filas
            value <<= 1;
        }
    }
    paint.end();
    ViewFinder &view = ViewFinder::getInstance();
    view.showBinaryCharacteristic(image);
}

int DialogTableWord::returnCategory(QString cad){
   // std::cout<<"DialogTableWord::returnCategory"<<std::endl;

    if(cad != "=" && cad != "+" && cad != "A" && cad != "B" && cad != "C" && cad != "D" && cad != "E" && cad != "F" && cad != "G" && cad != "H" && cad != "I" && cad != "J" && cad != "K"
            && cad != "L" && cad != "M" && cad != "N" && cad != "O" && cad != "P" && cad != "Q" && cad != "R" && cad != "S" && cad != "T" && cad != "U" && cad != "V" && cad != "W" && cad != "X"
            && cad != "Y" && cad != "Z" && cad != "AB" && cad != "BA" && cad != "CA" && cad != "LLO" && cad != "CABALLO" && cad != "SA" && cad != "CASA" && cad != "PO" && cad != "SAPO" && cad != "GA" && cad != "TO" && cad != "GATO" && cad != "PA"
            && cad != "PATO" && cad != "RRO" && cad != "CARRO" && cad != "GALLO" && cad != "CO" && cad != "BRA" && cad != "COBRA" && cad != "CU" && cad != "LE" && cad != "CULEBRA" && cad != "ON" && cad != "LEON" && cad != "VE"
            && cad != "NA" && cad != "DO" && cad != "VENADO" && cad != "VION" && cad != "AVION" && cad != "COCO" && cad != "DA" && cad != "RA" && cad != "CE" && cad != "TA" && cad != "DIS" && cad != "JO" && cad != "AR"
            && cad != "BOL" && cad != "GO" && cad != "RRA" && cad != "DRI" && cad != "LO" /*&& cad != "UN" && cad != "UNO" && cad != "DOS" && cad != "TRE" && cad != "TRES" && cad != "TRO" && cad != "CUATRO" && cad != "CIN"
            && cad != "CINCO" && cad != "SE" && cad != "IS" && cad != "SEIS" && cad != "SI" && cad != "TE" && cad != "SIETE" && cad != "CHO" && cad != "OCHO" && cad != "NU" && cad != "VE" && cad != "NUEVE" && cad != "RO" && cad != "CERO"*/)
        return cad.toInt();
    std::cout<<cad.toInt()<<std::endl;

    if (cad == "=")
        return '=';
    if(cad == "+")
        return'+';
    if(cad == "A")
        return 'A';
    if(cad == "B")
        return 'B';
    if(cad == "C")
        return 'C';
    if(cad == "D")
        return 'D';
    if(cad == "E")
        return 'E';
    if(cad == "F")
        return 'F';
    if(cad == "G")
        return 'G';
    if(cad == "H")
        return 'H';
    if(cad == "I")
        return 'I';
    if(cad == "J")
        return 'J';
    if(cad == "K")
        return 'K';
    if(cad == "L")
        return 'L';
    if(cad == "M")
        return 'M';
    if(cad == "N")
        return 'N';
    if(cad == "O")
        return 'O';
    if(cad == "P")
        return 'P';
    if(cad == "Q")
        return 'Q';
    if(cad == "R")
        return 'R';
    if(cad == "S")
        return 'S';
    if(cad == "T")
        return 'T';
    if(cad == "U")
        return 'U';
    if(cad == "V")
        return 'V';
    if(cad == "W")
        return 'W';
    if(cad == "X")
        return 'X';
    if(cad == "Y")
        return 'Y';
    if(cad == "Z")
        return 'Z';
    if(cad == "AB")
        return 10;
    if(cad == "BA")
        return 11;
    if(cad == "CA")
        return 12;
    if(cad == "LLO")
        return 13;
    if(cad == "DA")
        return 14;
    if(cad == "SA")
        return 15;
    if(cad == "RA")
        return 16;
    if(cad == "PO")
        return 17;
    if(cad == "CE")
        return 18;
    if(cad == "GA")
        return 19;
    if(cad == "TO")
        return 20;
    if(cad == "TA")
        return 21;
    if(cad == "PA")
        return 22;
    if(cad == "DIS")
        return 23;
    if(cad == "RRO")
        return 24;
    if(cad == "JO")
        return 25;
    if(cad == "AR")
        return 26;
    if(cad == "CO")
        return 27;
    if(cad == "BRA")
        return 28;
    if(cad == "LO")
        return 29;
    if(cad == "CU")
        return 30;
    if(cad == "LE")
        return 31;
    if(cad == "BOL")
        return 32;
    if(cad == "ON")
        return 33;
    if(cad == "GO")
        return 34;
    if(cad == "VE")
        return 35;
    if(cad == "NA")
        return 36;
    if(cad == "DO")
        return 37;
    if(cad == "RRA")
        return 38;
    if(cad == "VION")
        return 39;
    if(cad == "DRI")
        return 40;
    if(cad == "COCO")
        return 41;
    if(cad == "CABALLO")
        return 42;
    if(cad == "CASA")
        return 44;
    if(cad == "SAPO")
        return 45;
    if(cad == "GATO")
        return 46;
    if(cad == "PATO")
        return 47;
    if(cad == "CARRO")
        return 48;
    if(cad == "GALLO")
        return 49;
    if(cad == "CULEBRA")
        return 50;
    if(cad == "LEON")
        return 51;
    if(cad == "VENADO")
        return 52;
    if(cad == "AVION")
        return 53;
    if(cad == "COBRA")
        return 54;
    ////revisar estas categorias///
    if(cad == "UN")
        return 55;
    if(cad == "UNO")
        return 56;
    if(cad == "DOS")
        return 57;
    if(cad == "TRE")
        return 58;
    if(cad == "TRES")
        return 59;
    if(cad == "TRO")
        return 60;
    if(cad == "CUATRO")
        return 62;
    if(cad == "CIN")
        return 63;
    if(cad == "CINCO")
        return 64;
    if(cad == "SE")
        return 91;
    if(cad == "IS")
        return 92;
    if(cad == "SEIS")
        return 93;
    if(cad == "SI")
        return 94;
    if(cad == "TE")
        return 95;
    if(cad == "SIETE")
        return 96;
    if(cad == "CHO")
        return 97;
    if(cad == "OCHO")
        return 98;
    if(cad == "NU")
        return 99;
    if(cad == "VE")
        return 100;
    if(cad == "NUEVE")
        return 101;
    if(cad == "RO")
        return 102;
    if(cad == "CERO")
        return 103;
}
