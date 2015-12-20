#include"formteaching.h"
#include"ui_formteaching.h"

FormTeaching::FormTeaching(QWidget *parent) : QWidget(parent), ui(new Ui::FormTeaching){
    ui->setupUi(this);
    init();
    createSignals();
    QRegExp alphabet("[A-Z0-9+=][A-Z]*");
    ui->lineEditInput->setValidator(new QRegExpValidator(alphabet,this));
}


/*
funcion para obtener el estado de la red neuronal depende de que caso sea: HIT, DIFF and NO_HIT
se llamara algun metodo que corresponda al estado no retorna ninguna salida
*/
void FormTeaching::setState(stateNeuralNetwork state, int category){
    switch(state){
    case IS_HIT:
        isHit(category);
        break;
    case NO_HIT:
        noHit();
        break;
    case DIFF:
        isDiff();
        break;
    default:
        init();
        break;
    }
}

QLineEdit *FormTeaching::getLineEditInput(){
    return ui->lineEditInput;
}

QLineEdit *FormTeaching::getLineEditOut(){
    return ui->lineEditOut;
}

QPushButton *FormTeaching::getPtrButtonTeach(){
    return ui->pushButtonTeach;
}

QPushButton *FormTeaching::getPtrButtonState(){
    return ui->pushButtonState;
}

QPushButton *FormTeaching::getPtrButtonGoodAnswer(){
    return ui->pushButtonGoodAnswer;
}

FormTeaching::~FormTeaching(){
    delete ui;
}

void FormTeaching::changeInput(){
    if(ui->lineEditInput->text().isEmpty())
        ui->pushButtonTeach->setEnabled(false);
    else
        ui->pushButtonTeach->setEnabled(true);
}

void FormTeaching::goodAnswer(){
    init();
    ui->labelEmoticon->setPixmap(QPixmap("icons/happy.png"));
    QTimer::singleShot(2000, this, SLOT(finishTime()));
}

void FormTeaching::finishTime(){
    ui->labelEmoticon->setPixmap(QPixmap("icons/sleep.png"));
    emit signalGoodAnswer(this->senseTeaching);
}

void FormTeaching::badAnswer(){
    init();
    ui->labelQuestion->setVisible(true);
    ui->lineEditInput->setVisible(true);
    ui->pushButtonTeach->setVisible(true);
    ui->pushButtonTeach->setEnabled(false);
    ui->labelEmoticon->setPixmap(QPixmap("icons/question.png"));
}

void FormTeaching::init(){
    this->setFixedHeight(155);
    ui->labelQuestion->setVisible(false);
    ui->labelSense->setVisible(false);
    ui->labelCorrectTest->setVisible(false);
    ui->lineEditInput->setVisible(false);
    ui->lineEditOut->setVisible(false);
    ui->lineEditInput->setText("");
    ui->lineEditOut->setText("");
    ui->pushButtonState->setVisible(false);
    ui->pushButtonBadAnswer->setVisible(false);
    ui->pushButtonGoodAnswer->setVisible(false);
    ui->lineEditOut->setAlignment(Qt::AlignCenter);
    ui->lineEditInput->setAlignment(Qt::AlignCenter);
    ui->pushButtonTeach->setVisible(false);
    ui->lineEditOut->setReadOnly(true);
    ui->labelEmoticon->setPixmap(QPixmap("icons/sleep.png"));
}

void FormTeaching::createSignals(){
    connect(ui->lineEditInput,SIGNAL(textEdited(QString)),this,SLOT(changeInput()));
    connect(ui->pushButtonBadAnswer,SIGNAL(clicked()),this,SLOT(badAnswer()));
    connect(ui->pushButtonGoodAnswer,SIGNAL(clicked()),this,SLOT(goodAnswer()));
}

void FormTeaching::isHit(int category){
    QString text(caracterCla(category));
    ui->labelQuestion->setVisible(false);
    ui->lineEditInput->setVisible(false);
    ui->labelSense->setVisible(true);
    ui->labelCorrectTest->setVisible(true);
    ui->lineEditOut->setVisible(true);
    ui->lineEditOut->setText(text);
    ui->pushButtonState->setVisible(true);
    ui->pushButtonBadAnswer->setVisible(true);
    ui->pushButtonGoodAnswer->setVisible(true);
    ui->labelEmoticon->setPixmap(QPixmap("icons/response.png"));
}

void FormTeaching::noHit(){
    ui->labelQuestion->setVisible(true);
    ui->labelSense->setVisible(true);
    ui->lineEditInput->setVisible(true);
    ui->lineEditOut->setVisible(true);
    ui->lineEditOut->setText("Ignoro");
    ui->pushButtonTeach->setVisible(true);
    ui->pushButtonTeach->setEnabled(false);
    ui->labelEmoticon->setPixmap(QPixmap("icons/question.png"));
}

void FormTeaching::isDiff(){
    ui->pushButtonState->setVisible(true);
    ui->labelQuestion->setVisible(true);
    ui->labelSense->setVisible(true);
    ui->lineEditInput->setVisible(true);
    ui->lineEditOut->setVisible(true);
    ui->lineEditOut->setText("Confundido!");
    ui->pushButtonTeach->setVisible(true);
    ui->pushButtonTeach->setEnabled(false);
    ui->labelEmoticon->setPixmap(QPixmap("icons/question.png"));
}

QString FormTeaching::caracterCla(int category){
    switch(category){
    case 0:
        return("0");
        break;
    case 1:
        return("1");
        break;
    case 2:
        return("2");
        break;
    case 3:
        return("3");
        break;
    case 4:
        return("4");
        break;
    case 5:
        return("5");
        break;
    case 6:
        return("6");
        break;
    case 7:
        return("7");
        break;
    case 8:
        return("8");
        break;
    case 9:
        return("9");
        break;
    case '=':
        return("=");
        break;
    case '+':
        return("+");
        break;
    case 'A':
        return("A");
        break;
    case 'B':
        return("B");
        break;
    case 'C':
        return("C");
        break;
    case 'D':
        return("D");
        break;
    case 'E':
        return("E");
        break;
    case 'F':
        return("F");
        break;
    case 'G':
        return("G");
        break;
    case 'H':
        return("H");
        break;
    case 'I':
        return("I");
        break;
    case 'J':
        return("J");
        break;
    case 'K':
        return("K");
        break;
    case 'L':
        return("L");
        break;
    case 'M':
        return("M");
        break;
    case 'N':
        return("N");
        break;
    case 'O':
        return("O");
        break;
    case 'P':
        return("P");
        break;
    case 'Q':
        return("Q");
        break;
    case 'R':
        return("R");
        break;
    case 'S':
        return("S");
        break;
    case 'T':
        return("T");
        break;
    case 'U':
        return("U");
        break;
    case 'V':
        return("V");
        break;
    case 'W':
        return("W");
        break;
    case 'X':
        return("X");
        break;
    case 'Y':
        return("Y");
        break;
    case 'Z':
        return("Z");
        break;
    case 10:
        return("AB");
        break;
    case 11:
        return("BA");
        break;
    case 12:
        return("CA");
        break;
    case 13:
        return("LLO");
        break;
    case 14:
        return("DA");
        break;
    case 15:
        return("SA");
        break;
    case 16:
        return("RA");
        break;
    case 17:
        return("PO");
        break;
    case 18:
        return("CE");
        break;
    case 19:
        return("GA");
        break;
    case 20:
        return("TO");
        break;
    case 21:
        return("TA");
        break;
    case 22:
        return("PA");
        break;
    case 23:
        return("DIS");
        break;
    case 24:
        return("RRO");
        break;
    case 25:
        return("JO");
        break;
    case 26:
        return("AR");
        break;
    case 27:
        return("CO");
        break;
    case 28:
        return("BRA");
        break;
    case 29:
        return("LO");
        break;
    case 30:
        return("CU");
        break;
    case 31:
        return("LE");
        break;
    case 32:
        return("BOL");
        break;
    case 33:
        return("ON");
        break;
    case 34:
        return("GO");
        break;
    case 35:
        return("VE");
        break;
    case 36:
        return("NA");
        break;
    case 37:
        return("DO");
        break;
    case 38:
        return("RRA");
        break;
    case 39:
        return("VION");
        break;
    case 40:
        return("DRI");
        break;
    case 41:
        return("COCO");
        break;
    case 42:
        return("CABALLO");
        break;
    case 44:
        return("CASA");
        break;
    case 45:
        return("SAPO");
        break;
    case 46:
        return("GATO");
        break;
    case 47:
        return("PATO");
        break;
    case 48:
        return("CARRO");
        break;
    case 49:
        return("GALLO");
        break;
    case 50:
        return("CULEBRA");
        break;
    case 51:
        return("LEON");
        break;
    case 52:
        return("VENADO");
        break;
    case 53:
        return("AVION");
        break;
    case 54:
        return("COBRA");
        break;
    case 55:
        return("UN");
        break;
    case 56:
        return("UNO");
        break;
    case 57:
        return("DOS");
        break;
    case 58:
        return("TRE");
        break;
    case 59:
        return("TRES");
        break;
    case 60:
        return("TRO");
        break;
    case 62:
        return("CUATRO");
        break;
    case 63:
        return("CIN");
        break;
    case 64:
        return("CINCO");
        break;
    case 91:
        return("SE");
        break;
    case 92:
        return("IS");
        break;
    case 93:
        return("SEIS");
        break;
    case 94:
        return("SI");
        break;
    case 95:
        return("TE");
        break;
    case 96:
        return("SIETE");
        break;
    case 97:
        return("CHO");
        break;
    case 98:
        return("OCHO");
        break;
    case 99:
        return("NU");
        break;
    case 100:
        return("VE");
        break;
    case 101:
        return("NUEVE");
        break;
    case 102:
        return("RO");
        break;
    case 103:
        return("CERO");
        break;
    default:
        return("e");
    }
}

senses FormTeaching::getSenseTeaching() const{
    return senseTeaching;
}

void FormTeaching::setSenseTeaching(const senses &value){
    senseTeaching = value;
}
