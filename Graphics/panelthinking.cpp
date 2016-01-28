#include "panelthinking.h"
#include "ui_panelthinking.h"

panelthinking::panelthinking(QWidget *parent, const NeuralNetwork *neuralSenses) :QDialog(parent), ui(new Ui::panelthinking)
{
    ui->setupUi(this);
    this->neuralSenses = neuralSenses;
    sound = new QMediaPlayer(this);
    playList = new QMediaPlaylist(this);
    //playList->clear();
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

void panelthinking::setQueque(queue setqueque){
    this->queueThink = setqueque;
}

void panelthinking::setQueque_id(queue setqueque_id){
    this->queueIdThink = setqueque_id;
}

void panelthinking::setIdForm(int setIdForm){
    this->idForm = setIdForm;
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

queue panelthinking::getQueque(){
    return queueThink;
}

queue panelthinking::getQueque_id(){
    return queueIdThink;
}

int panelthinking::getIdForm(){
    return idForm;
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
        switch (idForm) {
        case 0:
            paintBinaryCharacteristic(getSenses(), getPtr());
            break;
        case 1:
            paintBinaryCharacteristicAddition(getSenses(), getQueque(), getQueque_id());
            break;
        case 2:
            paintBinaryCharacteristicCount(getSenses(), getPtr(),getQueque());
            break;
        default:
            break;
        }
    else
        paintBinaryNetSyllab(getPtr(), getCategory());
}

void panelthinking::on_btnthinkword_clicked()
{
    showThinkingWrited(getCategory(), getQueque());
}

void panelthinking::showThinkingWrited(int category, queue result_sum){
    showthinkingwrited = new showThinkingwrited();
    showthinkingwrited->setWindowModality(Qt::WindowModal);
    if(idForm == 0){
        showthinkingwrited->convertNumbertoWord(category);
    }
    else{
        if(idForm == 1){
            showthinkingwrited->ConvertAdditionOnWord(result_sum);
        }
    }




    this->setVisible(false);
    if(showthinkingwrited->exec() == QDialog::Rejected)
        this->setVisible(true);

    freeGenericPtr(showthinkingwrited);
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

void panelthinking::paintBinaryCharacteristicAddition(senses sense, queue result_queque, queue id_queque){

    unsigned short displacement = 8 * sizeof (unsigned short) -1;
    unsigned short mask = 1 << displacement;
    int ptr;
    QImage *image = new QImage(QSize(1350,700), QImage::Format_MonoLSB);
    image->fill(Qt::color1);
    QPainter paint;
    paint.begin(image);
    QPen pen(QColor(Qt::color1));
    paint.setPen(pen);
    paint.setBrush(QBrush(QColor(Qt::color1), Qt::SolidPattern));
    QString text = (sense == HEARING ) ? "VOZ" : "PENSANDO";
    paint.drawText(QRect(100,20,100,100),text);
    for(int a=Queue->queueLenght(result_queque)-1; a>=0; a--){
        ptr = Queue->dequeue(id_queque);
        for(int i=0; i<16 ; i++){
            unsigned short value = neuralSenses[sense].binaryCharacteristic[ptr * 16 + i];
            for(unsigned short j = 0 ; j <= displacement; j++){
                if(value &mask)
                    paint.drawRect(QRect((150*(a))+(15-j)*10,50+i*10,8,8));//(x,y) x columnas y filas
                value <<= 1;
            }
        }
    }
    paint.end();
    ViewFinder &view = ViewFinder::getInstance();
    view.showBinaryCharacteristic(image);
}

void panelthinking::paintBinaryCharacteristicCount(senses sense, int ptr, queue result_queque){

    unsigned short displacement = 8 * sizeof (unsigned short) -1;
    unsigned short mask = 1 << displacement;
    int resultado=0;
    QImage *image = new QImage(QSize(1350,700), QImage::Format_MonoLSB);
    image->fill(Qt::color1);
    QPainter paint;
    paint.begin(image);
    QPen pen(QColor(Qt::color1));
    paint.setPen(pen);
    paint.setBrush(QBrush(QColor(Qt::color1), Qt::SolidPattern));
    QString text = (sense == HEARING ) ? "VOZ" : "PENSANDO";
    paint.drawText(QRect(100,20,100,100),text);
    for(int a=Queue->queueLenght(result_queque)-1; a>=0; a--){
        for(int i=0; i<16 ; i++){
            unsigned short value = neuralSenses[sense].binaryCharacteristic[ptr * 16 + i];
            for(unsigned short j = 0 ; j <= displacement; j++){
                if(value &mask)
                    paint.drawRect(QRect((150*(a))+(15-j)*10,50+i*10,8,8));//(x,y) x columnas y filas
                value <<= 1;
            }
        }
    }
    paint.end();
    ViewFinder &view = ViewFinder::getInstance();
    view.showBinaryCharacteristic(image);
}

template <class T>
void panelthinking::freeGenericPtr(T *ptr){
    if(ptr!= NULL)
        delete ptr;
}

void panelthinking::on_btnSound_clicked()
{
    switch (idForm) {
    case 0:
        PlaySound(getCategory());
        break;
    case 1:
        playList->clear();
        playSoundAddition(getQueque());
        break;
    default:
        break;
    }
}

void panelthinking::PlaySound(int category){

    originfile="/home/johans/Escritorio/BrainCemisid-v1.5";

    switch(category){
    case 0:
        filename = originfile+"/Sounds_BrainCemisid/0.mp3";
        break;

    case 1:
        filename = originfile+"/Sounds_BrainCemisid/1.mp3";
        break;

    case 2:
        filename = originfile+"/Sounds_BrainCemisid/2.mp3";
        break;

    case 3:
        filename = originfile+"/Sounds_BrainCemisid/3.mp3";
        break;

    case 4:
        filename = originfile+"/Sounds_BrainCemisid/4.mp3";
        break;

    case 5:
        filename = originfile+"/Sounds_BrainCemisid/5.mp3";
        break;

    case 6:
        filename = originfile+"/Sounds_BrainCemisid/6.mp3";
        break;

    case 7:
        filename = originfile+"/Sounds_BrainCemisid/7.mp3";
        break;

    case 8:
        filename = originfile+"/Sounds_BrainCemisid/8.mp3";
        break;

    case 9:
        filename = originfile+"/Sounds_BrainCemisid/9.mp3";
        break;
    case 41:
        filename = originfile+"/Sounds_BrainCemisid/coco.mp3";
        break;
    case 42:
        filename = originfile+"/Sounds_BrainCemisid/caballo.mp3";
        break;
    case 43:
        filename = originfile+"/Sounds_BrainCemisid/nose.mp3";
        break;
    case 44:
        filename = originfile+"/Sounds_BrainCemisid/casa.mp3";
        break;
    case 45:
        filename = originfile+"/Sounds_BrainCemisid/sapo.mp3";
        break;
    case 46:
        filename = originfile+"/Sounds_BrainCemisid/gato.mp3";
        break;
    case 47:
        filename = originfile+"/Sounds_BrainCemisid/pato.mp3";
        break;
    case 48:
        filename = originfile+"/Sounds_BrainCemisid/carro.mp3";
        break;
    case 49:
        filename = originfile+"/Sounds_BrainCemisid/gallo.mp3";
        break;
    case 50:
        filename = originfile+"/Sounds_BrainCemisid/culebra.mp3";
        break;
    case 51:
        filename = originfile+"/Sounds_BrainCemisid/leon.mp3";
        break;
    case 52:
        filename = originfile+"/Sounds_BrainCemisid/venado.mp3";
        break;
    case 53:
        filename = originfile+"/Sounds_BrainCemisid/avion.mp3";
        break;
    case 54:
        filename = originfile+"/Sounds_BrainCemisid/cobra.mp3";
        break;
    case 56:
        filename = originfile+"/Sounds_BrainCemisid/1.mp3";
        break;
    case 57:
        filename = originfile+"/Sounds_BrainCemisid/2.mp3";
        break;
    case 59:
        filename = originfile+"/Sounds_BrainCemisid/3.mp3";
        break;
    case 61:
        filename = originfile+"/Sounds_BrainCemisid/nose.mp3";
        break;
    case 62:
        filename = originfile+"/Sounds_BrainCemisid/4.mp3";
        break;
    case 64:
        filename = originfile+"/Sounds_BrainCemisid/5.mp3";
        break;
    case 65:
        filename = originfile+"/Sounds_BrainCemisid/Letra_A.mp3";
        break;
    case 66:
        filename = originfile+"/Sounds_BrainCemisid/Letra_B.mp3";
        break;
    case 67:
        filename = originfile+"/Sounds_BrainCemisid/Letra_C.mp3";
        break;
    case 68:
        filename = originfile+"/Sounds_BrainCemisid/Letra_D.mp3";
        break;
    case 69:
        filename = originfile+"/Sounds_BrainCemisid/Letra_E.mp3";
        break;
    case 70:
        filename = originfile+"/Sounds_BrainCemisid/Letra_F.mp3";
        break;
    case 71:
        filename = originfile+"/Sounds_BrainCemisid/Letra_G.mp3";
        break;
    case 72:
        filename = originfile+"/Sounds_BrainCemisid/Letra_H.mp3";
        break;
    case 73:
        filename = originfile+"/Sounds_BrainCemisid/Letra_I.mp3";
        break;
    case 74:
        filename = originfile+"/Sounds_BrainCemisid/Letra_J.mp3";
        break;
    case 75:
        filename = originfile+"/Sounds_BrainCemisid/Letra_K.mp3";
        break;
    case 76:
        filename = originfile+"/Sounds_BrainCemisid/Letra_L.mp3";
        break;
    case 77:
        filename = originfile+"/Sounds_BrainCemisid/Letra_M.mp3";
        break;
    case 78:
        filename = originfile+"/Sounds_BrainCemisid/Letra_N.mp3";
        break;
    case 79:
        filename = originfile+"/Sounds_BrainCemisid/Letra_O.mp3";
        break;
    case 80:
        filename = originfile+"/Sounds_BrainCemisid/Letra_P.mp3";
        break;
    case 81:
        filename = originfile+"/Sounds_BrainCemisid/Letra_Q.mp3";
        break;
    case 82:
        filename = originfile+"/Sounds_BrainCemisid/Letra_R.mp3";
        break;
    case 83:
        filename = originfile+"/Sounds_BrainCemisid/Letra_S.mp3";
        break;
    case 84:
        filename = originfile+"/Sounds_BrainCemisid/Letra_T.mp3";
        break;
    case 85:
        filename = originfile+"/Sounds_BrainCemisid/Letra_U.mp3";
        break;
    case 86:
        filename = originfile+"/Sounds_BrainCemisid/Letra_V.mp3";
        break;
    case 87:
        filename = originfile+"/Sounds_BrainCemisid/Letra_W.mp3";
        break;
    case 88:
        filename = originfile+"/Sounds_BrainCemisid/Letra_X.mp3";
        break;
    case 89:
        filename = originfile+"/Sounds_BrainCemisid/Letra_Y.mp3";
        break;
    case 90:
        filename = originfile+"/Sounds_BrainCemisid/Letra_Z.mp3";
        break;
    case 93:
        filename = originfile+"/Sounds_BrainCemisid/6.mp3";
        break;
    case 96:
        filename = originfile+"/Sounds_BrainCemisid/7.mp3";
        break;
    case 98:
        filename = originfile+"/Sounds_BrainCemisid/8.mp3";
        break;
    case 101:
        filename = originfile+"/Sounds_BrainCemisid/9.mp3";
        break;
    case 103:
        filename = originfile+"/Sounds_BrainCemisid/0.mp3";
        break;
    default:
        filename = originfile+"/Sounds_BrainCemisid/nose.mp3";
        break;
    }

    sound->setMedia(QUrl::fromLocalFile(filename));
    sound->play();
}

void panelthinking::playSoundAddition(queue queue_result_sum){

    originfile = "/home/johans/Escritorio/BrainCemisid-v1.5";

    int value, longitud;

    QString text="", number;

    longitud = Queue->queueLenght(queue_result_sum);

    for(int a=0; a<longitud; a++){
        value = Queue->dequeue(queue_result_sum);
        text = text + QString::number(value);
    }

    for(int item =longitud-1; item >=0; item--){

        number = text[item];
        filename = originfile+"/Sounds_BrainCemisid/"+number+".mp3";
        playList->addMedia(QUrl::fromLocalFile(filename));
    }

    sound->setPlaylist(playList);
    sound->play();
}
