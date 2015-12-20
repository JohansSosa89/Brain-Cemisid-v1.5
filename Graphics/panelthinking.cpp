#include "panelthinking.h"
#include "ui_panelthinking.h"

panelthinking::panelthinking(QWidget *parent, const NeuralNetwork *neuralSenses) :QDialog(parent), ui(new Ui::panelthinking)
{
    ui->setupUi(this);
    this->neuralSenses = neuralSenses;
}

panelthinking::~panelthinking()
{
    delete ui;
}

void panelthinking::setSenses(senses setsense){
    this->sense = setsense;
}

void panelthinking::setPtr(int setptr){
    this->ptr = setptr;
}

void panelthinking::setCategory(int setCategory){
    this->categoryNeuron = setCategory;
}

senses panelthinking::getSenses(){
    return sense;
}

int panelthinking::getPtr(){
    return ptr;
}

int panelthinking::getCategory(){
    return categoryNeuron;
}

void panelthinking::paintBinaryCharacteristic(senses sense, int ptr){

    unsigned short displacement = 8 * sizeof (unsigned short) -1;
    unsigned short mask = 1 << displacement;
    QImage * image = new QImage(QSize(400,400), QImage::Format_MonoLSB);
    image->fill(Qt::color1);
    QPainter paint;
    paint.begin(image);
    QPen pen(QColor(Qt::color1));
    paint.setPen(pen);
    paint.setBrush(QBrush(QColor(Qt::color1), Qt::SolidPattern));
    QString text = (sense == HEARING) ? "VOZ" : "PENSANDO";
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

void panelthinking::on_pushButton_3_clicked()
{
    if(this->sense == SIGHT)
        paintBinaryCharacteristic(getSenses(), getPtr());
    else
        paintBinaryNetSyllab(getPtr(), getCategory());
}

void panelthinking::on_btnthinkword_clicked()
{
    showThinkingWrited(getCategory());
}

void panelthinking::showThinkingWrited(int category){
    showthinkingwrited = new showThinkingwrited();
    showthinkingwrited->setWindowModality(Qt::WindowModal);
    showthinkingwrited->setcategory(category);
    showthinkingwrited->convertNumbertoWord(category);
    this->setVisible(false);
    if(showthinkingwrited->exec() == QDialog::Rejected)
        this->setVisible(true);

    //freeGenericPtr(panelThinking);
    showthinkingwrited = NULL;
}

void panelthinking::paintBinaryNetSyllab(int ID, int Category){
    std::cout<<"MainWindow::paintBinarySyllab"<<endl;

    int numNeuron = *(neuralSenses[SIGHT].ptr);
    for(int id=0; id<numNeuron; id++){
        if(neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[id]-1)*9+CAT] != (unsigned int)Category && neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[id])*9+CAT] != (unsigned int)Category && neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[id]+id)*9+CAT] != (unsigned int)Category){
            //ui->pushButtonImage->setEnabled(false);
        }
        if(neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[id]-1)*9+CAT] == (unsigned int)Category && hit < 3 && ID != 999){
            hit = 1;
            paintBinaryCharacteristic(SIGHT,neuralSenses[SIGHT].ptr[id]-1);
            break;
        }
        for(int i=0; i<numNeuron;i++){
            if(neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[id]+i)*9+CAT] == (unsigned int)Category && ID != 999){
                hit = 3;
                paintBinaryCharacteristic(SIGHT,neuralSenses[SIGHT].ptr[id]+i);
            }
        }
    }
    hit = 0;
    //ui->pushButtonImage->setEnabled(true);
}
