#include "chemicalLayer.h"
#include<iostream>

ChemicalLayer::ChemicalLayer(int amountWidth,int amountHeight,int width,int height){
    setDefaultColor();
    intializeInterface(amountWidth,amountHeight,width,height);
    crateLayer();
    countRectActivate = 0;
    activeLayer = true;
    noData = true;
    setIsEditable(true);
}

ChemicalLayer::~ChemicalLayer(){
    delete m_scene;
}

void ChemicalLayer::intializeInterface(int amountWidth, int amountHeight, int width, int height){
    std::cout<<"ChemicalLayer::initializeInterface"<<std::endl;

    m_amountWidth = amountWidth;
    m_amountHeight = amountHeight;
    m_width = width;
    m_height = height;
    m_leftMousePressed = false;
    m_rightMousePressed = false;
    m_scene = new QGraphicsScene;
    setScene(m_scene);
    setAlignment(Qt::AlignLeft | Qt::AlignTop);
    setFixedSize((m_amountWidth*m_width)+3,(m_amountHeight*m_height)+3);
    setBackgroundRole(QPalette::Base);
    setAutoFillBackground(true);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

unsigned char *ChemicalLayer::generateCharacteristic(){
    std::cout<<"ChemicalLayer::initializeInterface"<<std::endl;

    unsigned char  sumRow=0,sumCol=0; //considerare que es cuadrada si no lo es no funciona
    unsigned char *vectorCharacteristic=new unsigned char[32];
    for(int i = 0; i < m_amountWidth; i++){
        for(int j = 0; j < m_amountWidth; j++){
            if(m_state[j][i])
                sumRow++;
            if(m_state[i][j])
                sumCol++;
        }
        vectorCharacteristic[i]=sumRow;
        vectorCharacteristic[i+16]=sumCol;
        sumCol=sumRow=0;
    }
    return vectorCharacteristic;
}

unsigned short * ChemicalLayer::generateBinaryCharacteristic(){
    std::cout<<"ChemicalLayer::generateBinaryCharacteristic"<<std::endl;

    unsigned short  sumRow=0; //considerare que es cuadrada si no lo es no funciona
    unsigned short * vectorCharacteristic = new unsigned short [16];
    for (int i = 0; i < m_amountWidth; i++) {
        for (int j = 0; j < m_amountWidth; j++)
            if(m_state[j][i])
                sumRow += pow(2,j);
        vectorCharacteristic[i]=sumRow;
        sumRow=0;
    }
    return vectorCharacteristic;
}

/*
Metodo para pintar el patron deseado
recibe: un puntero del vector a pintar y el numero de filas
returna: vacio
*/
void ChemicalLayer::paintPattern(const int *vector,const int &numRows){
    std::cout<<"ChemicalLayer::paintPattern"<<std::endl;

    int displacement = 8 * sizeof (int) -1;
    int mask;
    for(int i = 0; i < numRows ; i++){
        mask= 1;
        int value = vector[i];
        for(int j = 0 ; j <= displacement; j++){
            if(value &  mask){
                if(j >= numRows)
                    break;
                setState(j,i,true);
                countRectActivate++;
            }
            mask <<= 1;
        }
    }
    noData = false;
    emit change();
}

bool ChemicalLayer::getIsEditable() const{
    std::cout<<"ChemicalLayer::getIsEditable"<<std::endl;

    return isEditable;
}

void ChemicalLayer::setIsEditable(bool value){
    std::cout<<"ChemicalLayer::setIsEditable"<<std::endl;

    isEditable = value;
}
void ChemicalLayer::crateLayer(){
    std::cout<<"ChemicalLayer::crateLayer"<<std::endl;

    for(int x = 0; x < m_amountWidth; x++){
        QList<QGraphicsRectItem*> line;
        QList<bool> stateLine;
        for(int y = 0; y < m_amountHeight; y++){
            line << m_scene->addRect(m_width*x,m_height*y,m_width,m_height,QPen(),QBrush(m_inactiveColor,Qt::SolidPattern));
            stateLine << false;
        }
        m_grid << line;
        m_state << stateLine;
    }
}

QColor ChemicalLayer::activeColor(){
    std::cout<<"ChemicalLayer::activeColor"<<std::endl;

    return m_activeColor;
}

QColor ChemicalLayer::inactiveColor(){
    std::cout<<"ChemicalLayer::inactiveColor"<<std::endl;
    return m_inactiveColor;
}

void ChemicalLayer::newActiveColor(){
    std::cout<<"ChemicalLayer::newActiveColor"<<std::endl;

    QColor color = QColorDialog::getColor(m_activeColor);
    if (color.isValid() && color != m_activeColor) {
        m_activeColor = color;
        for (int y = 0; y < m_amountHeight; y++)
            for (int x = 0; x < m_amountWidth; x++)
                setState(x,y,m_state[x][y]);
    }
}

void ChemicalLayer::newInactiveColor(){
    std::cout<<"ChemicalLayer::newInactiveColor"<<std::endl;

    QColor color = QColorDialog::getColor(m_inactiveColor);
    if (color.isValid() && color != m_inactiveColor) {
        m_inactiveColor = color;
        for (int y = 0; y < m_amountHeight; y++)
            for (int x = 0; x < m_amountWidth; x++)
                setState(x,y,m_state[x][y]);
    }
}

void ChemicalLayer::setDefaultColor(){
    std::cout<<"ChemicalLayer::setDefaultColor"<<std::endl;

    m_activeColor = QColor("gray");
    m_inactiveColor = QColor("white");
}

void ChemicalLayer::setActiveLayer(bool activeLayer){
    std::cout<<"ChemicalLayer::setActiveLayer"<<std::endl;
    this->activeLayer = activeLayer;
}

bool ChemicalLayer::getNoData(){
    std::cout<<"ChemicalLayer::getNoData"<<std::endl;
    return noData;
}

void ChemicalLayer::clear(){
    std::cout<<"ChemicalLayer::clear"<<std::endl;

    if(countRectActivate == 0)
        return;
    for (int y = 0; y < m_amountHeight; y++)
        for (int x = 0; x < m_amountWidth; x++)
            setState(x,y,false);
    noData=true;
    countRectActivate=0;
    emit change();
}

QList<QList<bool> >& ChemicalLayer::state(){
    std::cout<<"ChemicalLayer::state"<<std::endl;
    return m_state;
}

void ChemicalLayer::setState(int x,int y,bool s){
    std::cout<<"ChemicalLayer::setState"<<std::endl;

    if (s)
        m_grid[x][y]->setBrush(QBrush(m_activeColor,Qt::SolidPattern));
    else
        m_grid[x][y]->setBrush(QBrush(m_inactiveColor,Qt::SolidPattern));
    m_state[x][y] = s;
}

int ChemicalLayer::width(){
    std::cout<<"ChemicalLayer::width"<<std::endl;

    return m_amountWidth;
}

int ChemicalLayer::height(){
    std::cout<<"ChemicalLayer::height"<<std::endl;

    return m_amountHeight;
}

void ChemicalLayer::mouseMoveEvent(QMouseEvent* event){
    std::cout<<"ChemicalLayer::mouseMoveEvent"<<std::endl;

    if((!m_leftMousePressed && !m_rightMousePressed) || !activeLayer || !isEditable)
        return;
    QGraphicsRectItem* item = getNode(event->pos());
    if(item == 0)
        return;
    bool* state = getNodeState(event->pos());
    if(m_leftMousePressed && !(*state)){
        item->setBrush(QBrush(m_activeColor,Qt::SolidPattern));
        countRectActivate++;
    }
    if(m_rightMousePressed && (*state)){
        item->setBrush(QBrush(m_inactiveColor,Qt::SolidPattern));
        countRectActivate--;
    }
    *state = m_leftMousePressed;
    if(noData && countRectActivate > 0){
        noData=false;
        emit change();
    }
    else if(!noData && countRectActivate == 0){
        noData=true;
        emit change();
    }
}

void ChemicalLayer::mousePressEvent(QMouseEvent* event){
    std::cout<<"ChemicalLayer::mousePressEvent"<<std::endl;

    if(!activeLayer || !isEditable)
        return;
    if(event->button() == Qt::LeftButton)
        m_leftMousePressed = true;
    if(event->button() == Qt::RightButton)
        m_rightMousePressed = true;
    QGraphicsRectItem* item = getNode(event->pos());
    if(item == 0)
        return;
    bool* state = getNodeState(event->pos());
    if(m_leftMousePressed && !(*state)){
        item->setBrush(QBrush(m_activeColor,Qt::SolidPattern));
        countRectActivate++;
    }
    if(m_rightMousePressed && (*state)){
        item->setBrush(QBrush(m_inactiveColor,Qt::SolidPattern));
        countRectActivate--;
    }
    *state = m_leftMousePressed;
    if(noData && countRectActivate > 0){
        noData=false;
        emit change();
    }
    else if(!noData && countRectActivate == 0){
        noData=true;
        emit change();
    }
}

void ChemicalLayer::mouseReleaseEvent(QMouseEvent* event){
    std::cout<<"ChemicalLayer::mouseReleaseEvent"<<std::endl;

    if (event->button() == Qt::LeftButton)
        m_leftMousePressed = false;
    if (event->button() == Qt::RightButton)
        m_rightMousePressed = false;
}

QGraphicsRectItem* ChemicalLayer::getNode(const QPoint& pos){
    std::cout<<"ChemicalLayer::getNode"<<std::endl;

    int x = pos.x();
    int y = pos.y();
    if (x <= 0 || x >= m_width*m_amountWidth || y <= 0 || y >= m_height*m_amountHeight)
        return 0;
    x = x/m_width;
    y = y/m_height;
    return m_grid[x][y];
}

bool* ChemicalLayer::getNodeState(const QPoint& pos){
    std::cout<<"ChemicalLayer::getNodeState"<<std::endl;

    int x = pos.x();
    int y = pos.y();
    if (x <= 0 || x >= m_width*m_amountWidth || y <= 0 || y >= m_height*m_amountHeight)
        return 0;
    x = x/m_width;
    y = y/m_height;
    return &m_state[x][y];
}
