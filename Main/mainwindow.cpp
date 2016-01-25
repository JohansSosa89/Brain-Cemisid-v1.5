#include"mainwindow.h"
#include"ui_mainwindow.h"
#include<algorithm>

//PRUEBA
//prototyping methods that invoke cuda kernels
extern "C"
void boot(NeuralNetwork *&neuralSenses,const SizeNet &sizeNet, Statistic *&statistic);

extern "C"
stateNeuralNetwork recognize(NeuralNetwork *neuralSenses,const SizeNet &sizeNet,
                             unsigned char *h_pattern , Interface *interface, Statistic *&statistic);
extern "C"
void correct(NeuralNetwork *neuralSenses, unsigned char desiredOutput, int maxThreadsPerBlock ,Statistic *&statistic);

extern "C"
void reset(NeuralNetwork *neuralSenses, int maxThreadsPerBlock, Statistic *&statistic);

extern "C"
void newItemCulturalNet(CulturalNet *addNet, int protocol, int LPA, int LPT );

extern "C"
int findOrderNeuron(OrderNetwork *orderNet, unsigned char sightID);

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow){
    try{
        std::cout<<"MainWindow::MainWindows"<<endl;

        initGui();
        setNull();
        showSelectDevice();
        intitializeSenses();
        generateVectorsCharacteristic();
        createTablesCharacteristic();
        createInterfacesTeaching();
        earTraining();
        getSight();
    }
    catch(bad_alloc & exceptionMem){
        QString text(exceptionMem.what());
        showWarning("Fallo de memoria",
                    "Su sistema no posee memoria RAM suficiente para ejecutar aplicacion ERROR: "
                    +text);

        freeUi();
        freeMem();

        exit(EXIT_SUCCESS);
    }
}

MainWindow::~MainWindow(){

    freeUi();
    freeMem();
}

void MainWindow::showSelectDevice(bool isVisibleButton){

    std::cout<<"MainWindow::showSelectDevice"<<endl;

    this->setVisible(false);
    if(!multipleDevice() && isVisibleButton)
        return;
    dialogSelectHardware = new DialogSelectHardware(0,selectedDevice);
    dialogSelectHardware->setWindowModality(Qt::WindowModal);
    dialogSelectHardware->setVisibleSelectButton(isVisibleButton);
    if(dialogSelectHardware->exec()==QDialog::Rejected)
        this->setVisible(true);
    else
        selectedDevice = dialogSelectHardware->getIndexSelectecDevice();
    delete dialogSelectHardware;
    dialogSelectHardware = NULL;
}

void MainWindow::freeUi(){
    delete ui;
    delete chemicalLayerEar;
    delete chemicalLayerEye;
    freeMemory->freeGenericPtr(image);
    freeMemory->freeGenericPtr(dialogConsult);
    freeMemory->freeGenericPtr(dialogStatistics);
    freeMemory->freeGenericPtr(dialogTable);
    freeMemory->freeGenericPtr(dialogTableWord);
    freeMemory->freeGenericPtr(dialogSelectHardware);
}

void MainWindow::freeMem(){
    freeMemory->freeFormTeaching(formsTeaching, numSenses ,isInactivateSense);
    freeMemory->freeStates(stateSenses);
    freeMemory->freeVectorsCharacteristic(characteristicVectorEar, characteristicVectorEye);
    freeMemory->freeSenses(neuralSenses, numSenses);
    freeMemory->freeInterface(interface);
    freeMemory->freeGenericPtr(statistics);
}

void MainWindow::generateVectorsCharacteristic(){

    characteristicVectorEar = chemicalLayerEar->generateCharacteristic();
    characteristicVectorEye = chemicalLayerEye->generateCharacteristic();
}

void MainWindow::on_checkBox_cuento_clicked(){
    std::cout<<"MainWindow::checkbox_cuento"<<endl;

    if(ui->checkBox_cuento->isChecked() || ui->checkBox_suma->isChecked())
        ui->pushButtonBip->setEnabled(false);
    else
        ui->pushButtonBip->setEnabled(true);
}

void MainWindow::addition(struct queue &up, struct queue &down){
    std::cout<<"MainWindow::addition"<<std::endl;
    std::cout<<"Cola1"<<std::endl;
    sumQueue->showQueue(up);
    std::cout<<std::endl;
    std::cout<<"Cola2"<<std::endl;
    sumQueue->showQueue(down);
    std::cout<<std::endl;

    int i = 0;
    int l_up = sumQueue->queueLenght(up);
    int l_down = sumQueue->queueLenght(down);
    int aux = 0;
    int t_up = 0;
    int t_down = 0;
    int k = 1;
    int j = 1;
    int carry=0;
    int result_int = 0;
    int l_result = 0;
    int dif = 0;
    QString text = "";
    QString reverse = "";

    if(l_up <= l_down){
        aux = l_up;

        dif = l_down - l_up;
        for(int a = 0; a<dif; a++){
            sumQueue->enqueue(up,0);
        }
    }
    else{
        aux = l_down;

        dif = l_up - l_down;
        for(int a = 0; a<dif; a++){
            sumQueue->enqueue(down,0);
        }
    }

    std::cout<<"Cola1 modificada"<<std::endl;
    sumQueue->showQueue(up);
    std::cout<<std::endl;
    std::cout<<"Cola2 modificada"<<std::endl;
    sumQueue->showQueue(down);
    std::cout<<std::endl;

    while(i < aux){
        k=1;
        j=1;
        t_up = sumQueue->dequeue(up);
        t_down = sumQueue->dequeue(down);
        t_up = t_up+carry;
        carry=0;
        while(k <= t_up){
           sumNetwork->vectorNetworkSum[k] = 1;
           k++;
        }
        while(j <= t_down){
            if(k>9){
                k=0;
                carry=carry+1;
            }
            sumNetwork->vectorNetworkSum[k] = 1;
            j++;
            k++;
        }
        sumQueue->enqueue(result,k-1);
        sumQueue->enqueue(id_queue,obtainID(getNumberNeurons(),k-1));
        i++;
    }
    if((t_up + t_down) >= 10)
        sumQueue->enqueue(result,carry);
    sumQueue->enqueue(id_queue,obtainID(getNumberNeurons(),carry));

    ui->textBrowser->show();
    l_result = sumQueue->queueLenght(result);

    IdForm = 1;
    showPanelThinking(SIGHT,0,0,result,id_queue, IdForm);
   // paintBinaryCharacteristicAddition(SIGHT,result);
   /* for(j=l_result-1; j>=0; j--){
        result_int = sumQueue->dequeue(result);
        text = text + QString::number(result_int);
        //std::cout<<"IdNeurona nro: "<<obtainID(getNumberNeurons(), result_int)<<std::endl;
        //Addition(SIGHT, obtainID(getNumberNeurons(), result_int),sumQueue);
    }
    std::cout<<"valor de l_result: "<<l_result<<std::endl;
    for(j=l_result; j>=0; j--){
        reverse = reverse + text[j];
        sumNetwork->vectorNetworkSum[j] = 1; 
    }
    ui->textBrowser->setText("RESULTADO DE LA SUMA: \n"+reverse);*/


}

int MainWindow::obtainID(int size, int Digito){

    for(int a=0; a<size; a++){
        if(vectorCategory[a]==Digito)
            return vectorId[a];
    }
}

void MainWindow::on_pushButtonBip_clicked(){

    if(countNetwork->vectorNetworkCount[kNeuron]== 1){
        ui->textBrowser->setText("KNOWN BIP");
        std::cout<<"Cantidad Conocida"<<endl;
    }else{
        countNetwork->vectorNetworkCount[kNeuron]= 1;
        countNetwork->bipPointer[kNeuron]=kNeuron+1;
        ui->textBrowser->setText("UNKNOWN BIP");
        std::cout<<"Cantidad Desconocida"<<endl;
    }
    kNeuron++;
    if(kNeuron >= orderNeuron){
        orderNeuron = kNeuron;
    }
}

void MainWindow::processGrid(){
    std::cout<<"MainWindow::processgrid"<<endl;

    clearTables();
    int k=1;

    ////procesar el grid de la vista//

    if(!chemicalLayerEye->getNoData()){
        try{
            stateSenses[SIGHT] = recognize(&neuralSenses[SIGHT],sizeNet,characteristicVectorEye,&interface[SIGHT],statistics);
            if(countNetwork->vectorNetworkCount[kNeuron]== 1){
                if(stateSenses[SIGHT] == IS_HIT){
                    std::cout<<"Número asociado a una cantidad conocida"<<endl;

                }else{
                    countNetwork->clackPointer[kNeuron] = kNeuron;
                    orderNetwork->order[kNeuron] = 1;
                    orderNetwork->bumPointer[kNeuron] = 1;
                    kNeuron = 1;
                    if(kNeuron>orderNeuron){
                        orderNeuron = kNeuron;
                    }
                    std::cout<<"CLACK SIGHT"<<endl;
                }
            }else{
                countNetwork->vectorNetworkCount[kNeuron] = 1;
                countNetwork->bipPointer[kNeuron] = kNeuron + 1;
                countNetwork->clackPointer[kNeuron] = kNeuron;
                orderNetwork->order[kNeuron] = 1;
                orderNetwork->bumPointer[kNeuron] = 1;
                if(kNeuron >= orderNeuron){
                    orderNeuron = kNeuron;
                }
                kNeuron = 1;
                std::cout<<"CLACK HEARING"<<endl;
            }
        }catch (string text){
            QMessageBox::critical(this,"CUDA ERROR",QString(text.c_str()).append("\n Se cerrara Aplicación"));
            exit(EXIT_FAILURE);
        }
        //hace el llamado al formteaching de la vista
        //pasa como parametro la categoria y el estado de la neurona(HIT, DIFF, NO_HIT)

        if(!ui->checkBox_Teach->isChecked()){
            if(stateSenses[SIGHT]== IS_HIT){
                cout<<"neurona hizo hit"<<endl;
                showPanelThinking(SIGHT, interface[SIGHT].id[0], interface[SIGHT].arrayCategory[0],word_count,id_queue, IdForm);
            }
            else{
                cout<<"neurona hizo diff o no_hit"<<endl;
                formsTeaching[SIGHT]->setState(stateSenses[SIGHT], interface[SIGHT].arrayCategory[0]);
            }

        }
        else{
            cout<<"neurona hizo diff o no_hit"<<endl;
            formsTeaching[SIGHT]->setState(stateSenses[SIGHT], interface[SIGHT].arrayCategory[0]);
        }

    }else
        isInactivateSense[SIGHT] = true;



    /////procesar el grid del oido//////////

    if(!chemicalLayerEar->getNoData()){
        try{
            stateSenses[HEARING] = recognize(&neuralSenses[HEARING],sizeNet,characteristicVectorEar,&interface[HEARING],statistics);
            if(ui->checkBox_suma->isChecked()){
                if(stateSenses[HEARING] == IS_HIT){
                    if(sum_loop == 0){
                        if(interface[HEARING].arrayCategory[0] == '+'){
                            sum_loop = 1;
                        }else{
                            for(k; k<=orderNeuron; k++){
                                if(orderNetwork->numRelation[k] == interface[HEARING].id[0])
                                    break;
                            }
                            sumQueue->enqueue(adding_up, k-1);
                        }
                    }
                    else if(sum_loop == 1){
                        if(interface[HEARING].arrayCategory[0] == '='){
                            addition(adding_up, adding_down);
                            sum_loop = 0;
                        }else{
                            for(k; k<=orderNeuron; k++){
                                if(orderNetwork->numRelation[k] == interface[HEARING].id[0])
                                    break;
                            }
                            sumQueue->enqueue(adding_down, k-1);
                        }
                    }
                }else{
                    sumQueue->clearQueue(adding_down);
                    sumQueue->clearQueue(adding_up);
                    std::cout<<"No puedo sumar, debo de conocer ese número primero"<<endl;
                }
            }
            if(ui->checkBox_cuento->isChecked()){
                if(stateSenses[HEARING] == IS_HIT){
                    bool flag = true;
                    int k = 0;
                    for(k=1; k<=orderNeuron; k++){
                        if(orderNetwork->numRelation[k] == interface[HEARING].id[0]){
                            std::cout<<"Comienzo a contar"<<endl;
                            flag = false;
                            break;
                        }
                    }
                    std::cout<<flag<<endl;
                    if(flag){
                        std::cout<<"NO CONSEGUI NUMERO NEURONAL"<<endl;
                    }else{

                        //hace el llamado al metodo de conteo
                        paintCount(k-1);

                    }
                 }else{
                    std::cout<<"No conozco ese numero, deberia de aprenderlo primero y asociarlo a una cantidad"<<endl;
                 }
            }else{
                if(countNetwork->vectorNetworkCount[kNeuron]== 1){
                    if(stateSenses[HEARING] == IS_HIT && !ui->checkBox_suma->isChecked()){
                        orderNetwork->numRelation[kNeuron] = interface[HEARING].id[0];
                        if(interface[HEARING].id == NULL)
                            std::cout<<"este id esta null"<<std::endl;
                        else
                            std::cout<<"este id no esta null"<<std::endl;

                        std::cout<<"este es el id: "<<interface[HEARING].id<<endl;
                        std::cout<<"Número asociado a una cantidad conocida"<<endl;
                        if(kNeuron>orderNeuron){
                            orderNeuron=kNeuron;
                        }
                   }else{
                        countNetwork->clackPointer[kNeuron] = kNeuron;
                        orderNetwork->order[kNeuron] = 1;
                        orderNetwork->bumPointer[kNeuron] = 1;
                        if(kNeuron>orderNeuron){
                            orderNeuron=kNeuron;
                        }
                        kNeuron=1;
                        std::cout<<"CLACK AUDIO"<<endl;
                    }
                }else{
                    orderNetwork->order[kNeuron] = 1;
                    orderNetwork->bumPointer[kNeuron] = 1;
                    if(kNeuron>orderNeuron){
                        orderNeuron=kNeuron;
                    }
                    kNeuron=1;
                    std::cout<<"CLACK2 AUDIO"<<endl;
                }
            }
            if(stateSenses  [HEARING] == IS_HIT && !isInactivateSense[SIGHT])
                emit cross();
        }catch(string text){
            QMessageBox::critical(this,"CUDA ERROR",QString(text.c_str()).append("\n Se cerrara Aplicación"));
            exit(EXIT_FAILURE);
        }
        //hace el llamado al formteaching del oido
        //pasa como parametro la categoria y el estado de la neurona(HIT, DIFF, NO_HIT)
        if(ui->checkBox_Teach->isChecked())
            formsTeaching[HEARING]->setState(stateSenses[HEARING], interface[HEARING].arrayCategory[0]);

        if(stateSenses[HEARING]== IS_HIT && (ui->checkBox_leer->isChecked() || ui->checkBox_syl->isChecked()) && !ui->checkBox_Teach->isChecked()){
            cout<<"neurona hizo hit"<<endl;
            showPanelThinking(HEARING, returnID(word2), returnCategory(word2),word_count, id_queue, IdForm);
            chemicalLayerEye->clear();
        }
        else{
            cout<<"neurona hizo diff o no hit"<<endl;
            formsTeaching[HEARING]->setState(stateSenses[HEARING], interface[HEARING].arrayCategory[0]);
        }
    }else
        isInactivateSense[HEARING] = true;
    kNeuron = 1;
}

void MainWindow::clearTables(){
    std::cout<<"MainWindow::clearTables"<<endl;

    generateVectorsCharacteristic();
    createTablesCharacteristic();
}

void MainWindow::learnHearing(){
    std::cout<<"MainWindow::learnHearing"<<endl;
    learn(HEARING);
}


void MainWindow::learnSight(){
    std::cout<<"MainWindow::learnSight"<<endl;
    learn(SIGHT);
}

void MainWindow::resetHearing(){
    std::cout<<"MainWindow::resetHearing"<<endl;
    try{
        reset(&neuralSenses[HEARING],deviceProp.maxThreadsPerBlock,statistics);
    }catch(string text){
        QMessageBox::critical(this,"CUDA ERROR",QString(text.c_str()).append("\n Se cerrara Aplicación"));
        exit(EXIT_FAILURE);
    }
}

void MainWindow::resetSight(){
    std::cout<<"MainWindow::resetSight"<<endl;
    try{
        reset(&neuralSenses[SIGHT],deviceProp.maxThreadsPerBlock,statistics);
    }catch(string text){
        QMessageBox::critical(this,"CUDA ERROR",QString(text.c_str()).append("\n Se cerrara Aplicación"));
        exit(EXIT_FAILURE);
    }
}

void MainWindow::clickBum(){
    std::cout<<"MainWindow::clickBum"<<endl;

    /////provisional//////
    isInactivateSense[SIGHT] = true;
    isInactivateSense[HEARING] = true;
    activeLayers(true);
    word2 = "";
    //////////////////////

    bum = 1;
    chemicalLayerEye->setIsEditable(true);
    ui->pushButtonBum->setEnabled(false);
    ui->pushButtonBum->setStyleSheet("*{background-color: light gray}");
    ui->lineEditEarInput->setEnabled(true);
    connect(ui->lineEditEarInput,SIGNAL(textChanged(QString)),this,SLOT(launchWave()));
    if(!ui->checkBox_leer->isChecked() && !ui->checkBox_syl->isChecked()){
//        ui->pushButtonProcess->setStyleSheet("*{background-color: rgb(154,203,234,120)}");
        ui->pushButtonProcess->setStyleSheet("*{background-color: rgb(96,96,96,120)}");
        ui->pushButtonProcess->setEnabled(true);
    }

    if(ui->checkBox_cuento->isChecked() && !ui->checkBox_Teach->isChecked())
        ui->InputWordCount->setEnabled(true);
}

void MainWindow::activateButtonBum(){
    std::cout<<"MainWindow::activateButtomBum"<<endl;

    ui->pushButtonBum->setStyleSheet("*{background-color: light gray}");
    if((!(chemicalLayerEar->getNoData()) || !(chemicalLayerEye->getNoData())) && (!ui->checkBox_cuento->isChecked() || !ui->checkBox_suma->isChecked()) && word2.isEmpty()){
        ui->pushButtonBum->setEnabled(true);
        ui->pushButtonBum->setStyleSheet("*{background-color: rgb(96,96,96,120)}");
    }else
        ui->pushButtonBum->setEnabled(false);
}

void MainWindow::activateButtonBip(){
    std::cout<<"MainWindow::activateButtonBip"<<endl;

    ui->pushButtonBip->setStyleSheet("*{background-color: light gray}");
    if((!(chemicalLayerEar->getNoData()) || !(chemicalLayerEye->getNoData())) && (!ui->checkBox_cuento->isChecked() || !ui->checkBox_suma->isChecked()) && (!ui->checkBox_leer->isChecked() && !ui->checkBox_syl->isChecked()) && bum == 1){
        ui->pushButtonBip->show();
        ui->pushButtonBip->setEnabled(true);
        ui->pushButtonBip->setStyleSheet("*{background-color: rgb(96,96,96,120)}");
    }else
        ui->pushButtonBip->setEnabled(false);
}

void MainWindow::activateButtonBipWord(){
    std::cout<<"MainWindow::activateButtonBipWord"<<endl;

    ui->pushButtonBipWord->setStyleSheet("*{background-color: light gray}");
    if(((!(chemicalLayerEar->getNoData()) && ui->checkBox_leer->isChecked()) && (!(chemicalLayerEar->getNoData()) && (!ui->checkBox_cuento->isChecked() || !ui->checkBox_suma->isChecked()))) && bum == 1){
        ui->pushButtonBipWord->show();
        ui->pushButtonBipWord->setEnabled(true);
        ui->pushButtonBipWord->setStyleSheet("*{background-color: rgb(96,96,96,120)}");
    }else
        ui->pushButtonBipWord->setEnabled(false);
}

void MainWindow::deactivateBip(){
    std::cout<<"MainWindow::deactivateBip"<<endl;

    QString nameFile = "./Obj/net_hearing_syl.dot";
    generateDotWord(nameFile,word2);
    ui->pushButtonBip->setStyleSheet("*{background-color: light gray}");
    ui->pushButtonBip->setEnabled(false);
    ui->pushButtonBipSyl->setStyleSheet("*{background-color: light gray}");
    ui->pushButtonBipSyl->setEnabled(false);
    ui->pushButtonBipWord->setStyleSheet("*{background-color: light gray}");
    ui->pushButtonBipWord->setEnabled(false);
    ui->pushButtonBum->setStyleSheet("*{background-color: light gray}");
    ui->pushButtonBum->setEnabled(false);
    ui->lineEditEarInput->setEnabled(false);
    ui->pushButtonProcess->setEnabled(true);
    ui->pushButtonProcess->setStyleSheet("*{background-color: rgb(96,96,96,120)}");
    ui->pushButtonCheck->setEnabled(false);
    ui->pushButtonCheck->setStyleSheet("*{background-color: light gray}");
}

void MainWindow::activateButtonBipSyl(){
    std::cout<<"MainWindow::activateButtonBipSyl"<<endl;

    ui->pushButtonBipSyl->setStyleSheet("*{background-color: light gray}");
    if(!(chemicalLayerEar->getNoData()) && ui->checkBox_syl->isChecked() && bum == 1){
        ui->pushButtonBipSyl->show();
        ui->pushButtonBipSyl->setVisible(true);
        ui->pushButtonBipSyl->setEnabled(true);
        ui->pushButtonBipSyl->setStyleSheet("*{background-color: rgb(96,96,96,120)}");
    }else
        ui->pushButtonBipSyl->setEnabled(false);
}

void MainWindow::activateButtonCheck(){
    std::cout<<"MainWindow::activateButtonCheck"<<endl;

    ui->pushButtonCheck->setStyleSheet("*{background-color: light gray}");
    if((!(chemicalLayerEar->getNoData()) && (ui->checkBox_leer->isChecked() || ui->checkBox_syl->isChecked())) && bum == 1){
        ui->pushButtonCheck->show();
        ui->pushButtonCheck->setVisible(true);
        ui->pushButtonCheck->setEnabled(true);
        ui->pushButtonCheck->setStyleSheet("*{background-color: rgb(96,96,96,120)}");
    }else
        ui->pushButtonCheck->setEnabled(false);
}

void MainWindow::activateButtonBNSWord(){
    std::cout<<"MainWindow::activateButtonBNSWord"<<endl;

    if(nsyllab != 0)
        ui->pushButtonBNSWord->setEnabled(true);
    else
        ui->pushButtonBNSWord->setEnabled(false);
}

void MainWindow::activateButtonCorrectWord(){
    std::cout<<"MainWindow::activateButtonCorrectWord"<<endl;

    if(!word2.isEmpty())
        ui->pushButtonCorrectWord->setEnabled(true);
    else
        ui->pushButtonCorrectWord->setEnabled(false);
}

void MainWindow::activateButtonImage(){
    std::cout<<"MainWindow::activateButtonImage"<<endl;

    if(!word2.isEmpty())
        ui->pushButtonImage->setEnabled(true);
    else
        ui->pushButtonImage->setEnabled(false);
}

void MainWindow::activateButtonImages(){
    std::cout<<"MainWindow::activateButtonImages"<<endl;

    if(nsyllab != 0)
        ui->pushButtonImages->setEnabled(true);
    else
        ui->pushButtonImages->setEnabled(false);
}

void MainWindow::activateButtonSoundWord(){
    std::cout<<"MainWindow::activateButtonSoundWord"<<endl;

    if(!word2.isEmpty())
        ui->pushButtonSoundWord->setEnabled(true);
    else
        ui->pushButtonSoundWord->setEnabled(false);
}

void MainWindow::activeLayers(bool active){
    std::cout<<"MainWindow::activateLayers"<<endl;

    if(active){
        if(isInactivateSense[SIGHT] == true && isInactivateSense[HEARING] == true){
            ui->lineEditEarInput->clear();
            chemicalLayerEye->clear();
            chemicalLayerEar->clear();
            clearTables();
            setFormsCheck();
            activateInterface(active);
            ui->pushButtonBip->hide();
            ui->pushButtonBipSyl->hide();
            ui->pushButtonCheck->hide();
            ui->pushButtonProcess->setEnabled(false);
            ui->pushButtonProcess->setStyleSheet("*{background-color: light gray}");
            chemicalLayerEye->setIsEditable(false);
            ui->pushButtonBum->setEnabled(true);
            ui->pushButtonBum->setStyleSheet("*{background-color: rgb(96,96,96,120)}");
            ui->lineEditEarInput->setEnabled(false);
        }
    }else{
        activateInterface(active);
        word.clear();

        ///AQUI TENGO QUE BUSCAR//////
        if(ui->checkBox_leer->isChecked() || ui->checkBox_syl->isChecked()){
            if(nsyllab != 0){
                /*ui->pushButtonBNSWord->show();
                ui->pushButtonCorrectWord->show();
                ui->pushButtonImage->show();
                ui->pushButtonImages->show();
                ui->pushButtonSoundWord->show();
                ui->textBrowserWord->show();*/
                QString text(QString::number(nsyllab) + " sílaba(s): " + word2);
                ui->textBrowserWord->setText(text);
                ofstream file("./Obj/net_hearing_syl.dot",ios::app);
                file<<"}\n";
                file.close();
                ui->pushButtonCheck->setEnabled(false);
            }
        }
        bum = 0;
    }
    ui->pushButtonBum->setEnabled(true);
    ui->pushButtonBum->setStyleSheet("*{background-color: rgb(96,96,96,120)}");
    ui->pushButtonBip->setStyleSheet("*{background-color: light gray}");
    ui->pushButtonCheck->setStyleSheet("*{background-color: light gray}");
    ui->pushButtonProcess->setStyleSheet("*{background-color: light gray}");
    ui->pushButtonBip->setEnabled(false);
    ui->pushButtonProcess->setEnabled(false);
}

void MainWindow::finishGoodAnswer(senses sense){
    std::cout<<"MainWindow::finishGoodAnswerSonido"<<endl;

    isInactivateSense[sense] = true;
    activeLayers(true);
    if(nsyllab == 0){
        word.clear();
        word2.clear();
        ptr1 = 999, ptr2 = 999, ptr3 = 999, ptr4 = 999, ptr5 = 999, ptr6 = 999, ptr7 = 999, ptr8 = 999;
        nsyllab = 0;
        file();
    }
}

void MainWindow::paintNetNeuron(senses sense, bool onlyHits){
    std::cout<<"MainWindow::paintNetNeuron"<<endl;

    QString nameFile = (sense == HEARING) ? "./Obj/net_hearing.dot" : "./Obj/net_sight.dot";
    generateDot(nameFile,sense,onlyHits);
    generatePng(nameFile);
    ViewFinder &view = ViewFinder::getInstance(this);
    view.showNet();
}

void MainWindow::paintBinaryCharacteristic(senses sense, int ptr){

    unsigned short displacement = 8 * sizeof (unsigned short) -1;
    unsigned short mask = 1 << displacement;
    freeMemory->freeGenericPtr(image);
    image = new QImage(QSize(500,500), QImage::Format_MonoLSB);
    image->fill(Qt::color1);
    QPainter paint;
    paint.begin(image);
    QPen pen(QColor(Qt::color1));
    paint.setPen(pen);
    paint.setBrush(QBrush(QColor(Qt::color1), Qt::SolidPattern));
    QString text = (sense == HEARING ) ? "VOZ" : "PENSANDO";
    paint.drawText(QRect(100,20,100,100),text);
    for(int i=0; i<16 ; i++){
        unsigned short value = neuralSenses[sense].binaryCharacteristic[ptr * 16 + i];
        for(unsigned short j = 0 ; j <= displacement; j++){
            if(value &mask)
                paint.drawRect(QRect(100+(15-j)*10,50+i*10,8,8));//(x,y) x columnas y filas
            value <<= 1;
        }
    }
    paint.end();
    ViewFinder &view = ViewFinder::getInstance(this);
    view.showBinaryCharacteristic(image);
}

void MainWindow::paintStateHearing(){
    std::cout<<"MainWindow::paintStateHear"<<endl;

    paintNetNeuron(HEARING);
}

void MainWindow::paintStateSight(){
    std::cout<<"MainWindow::paintStateSigth"<<endl;

    paintNetNeuron(SIGHT);
}

void MainWindow::paintHearing(){
    std::cout<<"MainWindow::paintHearing"<<endl;

    if(*(neuralSenses[HEARING].ptr) == 0)
        showWarning("Sentido Tabula Rasa","Bloque neuronal(BNS) del oido se encuentra tabula rasa");
    else
        paintNetNeuron(HEARING,false);
}

void MainWindow::paintSight(){
    std::cout<<"MainWindow::paintSight"<<endl;

    if(*(neuralSenses[SIGHT].ptr) == 0)
        showWarning("Sentido Tabula Rasa","Bloque neuronal(BNS) de la vista se encuentra tabula rasa");
    else
        paintNetNeuron(SIGHT,false);
}

void MainWindow::activateMainWindow(bool activate){
    std::cout<<"MainWindow::activateMainWindow"<<endl;

    this->setEnabled(activate);
}

void MainWindow::showDialogConsult(){
    std::cout<<"MainWindow::showDialogConsult"<<endl;

    if((*(neuralSenses[HEARING].ptr) == 0 ) && (*(neuralSenses[SIGHT].ptr) == 0)){
        showWarning("Tabula Rasa","No existen neuronas con conocimiento");
        return;
    }
    dialogConsult = new DialogConsultNeuron(neuralSenses[HEARING].ptr,neuralSenses[SIGHT].ptr);
    this->setVisible(false);
    if(dialogConsult->exec() == QDialog::Rejected)
        this->setVisible(true);
    else{
        paintBinaryCharacteristic(dialogConsult->radioButtonActive(),dialogConsult->returnIdNeuron());
        this->setVisible(true);
    }
    freeMemory->freeGenericPtr(dialogConsult);
    dialogConsult = NULL;
}

void MainWindow::showDialogStatistic(){
    std::cout<<"MainWindow::showDialogStatistic"<<endl;

    dialogStatistics = new DialogStatistics();
    dialogStatistics->setItems(statistics);
    dialogStatistics->setWindowModality(Qt::WindowModal);
    dialogStatistics->setWindowIcon(QPixmap("icons/statistic.png"));
    this->setVisible(false);
    if(dialogStatistics->exec() == QDialog::Rejected)
        this->setVisible(true);
    freeMemory->freeGenericPtr(dialogStatistics);
    dialogStatistics = NULL;
}

void MainWindow::showDialogTableNeuron(){
    std::cout<<"MainWindow::showDialogTableNeuron"<<endl;

    dialogTable = new DialogTableBNS(0,neuralSenses,&sizeNet);
    dialogTable->setWindowModality(Qt::WindowModal);
    dialogTable->setWindowIcon(QPixmap("icons/tableNeuron.png"));
    this->setVisible(false);
    if(dialogTable->exec() == QDialog::Rejected)
        this->setVisible(true);
    freeMemory->freeGenericPtr(dialogTable);
    dialogTable = NULL;
}

void MainWindow::showDialogInstructions(){
    std::cout<<"MainWindow::showDialogInstrucctions"<<endl;

    dialogInstructions = new DialogInstructions();
    dialogInstructions->setWindowModality(Qt::WindowModal);
    dialogInstructions->setWindowIcon(QPixmap("icons/instruction.png"));
    this->setVisible(false);
    if(dialogInstructions->exec() == QDialog::Rejected)
        this->setVisible(true);
    freeMemory->freeGenericPtr(dialogInstructions);
    dialogInstructions = NULL;
}

void MainWindow::showPanelThinking(senses sense, int ptr, int categoryNeuron, queue queue_result, queue queue_id, int idFrm){

    panelThinking = new panelthinking(0,neuralSenses);
    panelThinking->setWindowModality(Qt::WindowModal);
    panelThinking->setSenses(sense);
    panelThinking->setPtr(ptr);
    panelThinking->setCategory(categoryNeuron);
    panelThinking->setQueque(queue_result);
    panelThinking->setQueque_id(queue_id);
    panelThinking->setIdForm(idFrm);
    this->setVisible(false);
    if(panelThinking->exec() == QDialog::Rejected)
        this->setVisible(true);

    freeMemory->freeGenericPtr(panelThinking);
    panelThinking = NULL;
}

void MainWindow::aboutBrainCemisid(){
    std::cout<<"MainWindow::aboutBrainCemisid"<<endl;

    this->setVisible(false);
    QString title = "<H2>Proyecto Brain-Cemisid</H2>";
    QString version = "<H5>Versión 1.5 <br> <a rel=\"nofollow\" target=\"_blank\" href=\"https://github.com/ricardobru/BrainCemisid-v1.4\" onclick=\"s_objectID=\"https://braincemisidjj.googlecode.com/svn/trunk_1\";return this.s_oc?this.s_oc(e):true\"><font color=\"#505050\">https://github.com/ricardobru/BrainCemisid-v1.4/</font></a></H5><br>";
    QString description;
    description.append("<b>Proyecto de construcci&oacute;n de un cerebro usando arquitectura por capas, ");
    description.append("esta versi&oacute;n realiza la lectura comprensiva de palabras. A trav&eacute;s de la aplicaci&oacute;n, adaptaci&oacute;n y ampliaci&oacute;n de las capas desarrolladas en versiones anteriores de BrainCemisid.");
    description.append(" La aplicaci&oacute;n realiza:</b>");
    description.append("<ol><li>Mecanismo de captura de est&iacute;mulos de los sentidos de la vista y el o&iacute;do(sensores)</li>");
    description.append("<li>Proceso de transformaci&oacute;n del est&iacute;mulo en una caracter&iacute;stica (vector 32 pos)</li>");
    description.append("<li>Creaci&oacute;n de Bloques Neuronales Sensoriales (BNS) usando Redes Neuronales de base radial desarrolladas en CEMISID (Liliana Andrade-Carlos Rangel)</li>");
    description.append("<li>Formaci&oacute;n de palabras a trav&eacute;s del reconocimiento de s&iacute;labas</li>");
    description.append("<li>Asociaci&oacute;n de las palabras a im&aacute;genes aprendidas anteriormente o en el momento</li>");
    description.append("<li>Interfaz Gr&aacute;fica que muestre el funcionamiento</li></ol>");
    QString titleProgramer = "<b>Desarrollado por:</b>";
    QString programer = "<ul style=\"list-style-type:disc\"><li>Jonathan Monsalve, Julio Muchacho (BrainCemisid v.J&J - 2014/2015)</li> <li>Ricardo Graterol (BrainCemisid v1.3 - 2015)</li> <li>Ricardo Bruzual (BrainCemisid v1.4 - 2015)</li><li>Johan Sosa (BrainCemisid v1.5 - 2016)</li></ul>";
    QString titleEmail = "<b>Direcci&oacute;n electr&oacute;nica:</b><br>";
    QString email = "j.jmonsalveg@gmail.com, xavier.graterol@gmail.com, ricardo.bruzual.a@gmail.com, sosabjohane@gmail.com<br><br>";
    QString titleTutor = "<b>Bajo la Tutor&iacute;a:</b> <br>";
    QString tutor = "Dr. Gerard P&aacute;ez Monz&oacute;n <br><br>";
    QString university = "<b>Universidad de los Andes</b><br>";
    QString lugar = "<b>M&eacute;rida, Venezuela, 2015</b>";
    QMessageBox aboutBC;
    aboutBC.setWindowIcon(QPixmap("icons/brainn.png"));
    aboutBC.setIconPixmap(QPixmap("icons/brainlogo.png"));
    aboutBC.setWindowModality(Qt::WindowModal);
    QSpacerItem* horizontalSpacer = new QSpacerItem(700, 600, QSizePolicy::Fixed, QSizePolicy::Fixed);
    dynamic_cast<QGridLayout*>(aboutBC.layout())->addItem(horizontalSpacer, 0, 0, 0, 0);
    aboutBC.setText("<p align=\"center\">" + title + version +"</p>"+ "<p align=\"justify\">"+
                    description +"</p>"+"<p align=\"center\">"+ "<H5>"+ titleProgramer + programer +
                    titleEmail + email + titleTutor + tutor + university + lugar + "</H5>" + "</p>");
    aboutBC.exec();
    this->setVisible(true);
}

void MainWindow::aboutQt(){
    std::cout<<"MainWindow::aboutQt"<<endl;

    this->setVisible(false);
    QString title = "<H2><b>Acerca de Qt</b></H2>";
    QString version = "<H5> Este programa utiliza Qt versi&oacute;n 5.4.1. </H5>";
    QString p1= "Qt es un conjunto de herramientas de C++ para el desarrollo de aplicaciones multiplataforma."
                " Qt proporciona portabilidad de una sola fuente en todos los principales sistemas operativos."
                " Tambi&eacute;n est&aacute; disponible para otros sistemas operativos embebidos de Linux y m&oacute;viles.";
    QString p2 = "Qt est&aacute; disponible en tres opciones de licencias diferentes, dise&ntilde;ados para adaptarse a las necesidades de los diferentes usuarios.";
    QString p3 = "<li>Qt licenciado bajo nuestro contrato de licencia comercial es adecuado para el desarrollo de software propietario/comercial en el que no se desea compartir el c&oacute;digo fuente con terceros o de lo contrario no se puede cumplir con los t&eacute;rminos de la GNU LGPL versi&oacute;n 3 o GNU LGPL versi&oacute;n 2.1.</li>"
                 "<li>Qt bajo la licencia GNU LGPL versi&oacute;n 3 es apropiado para el desarrollo de aplicaciones Qt que cumplan con los t&eacute;rminos y condiciones de la versi&oacute;n GNU LGPL 3.</li>"
                 "<li>Qt bajo la licencia GNU LGPL versi&oacute;n 2.1 es apropiado para el desarrollo de aplicaciones Qt que cumplan con los t&eacute;rminos y condiciones de la versi&oacute;n GNU LGPL 2.1.</li></ol>";
    QString p4 = "<H5>Consulte "
                 "<a rel=\"nofollow\" target=\"_blank\" href=\"https://www.qt.io/licensing\" onclick=\"s_objectID=\"https://www.qt.io/licensing_1\";return this.s_oc?this.s_oc(e):true\"><font color=\"#505050\">qt.io/licensing</font></a> para una visi&oacute;n general de las licencias de Qt.</H5>"
                 "<H5>Copyright&copy; 2015 Digia Plc y/o su filial(es) y otros colaboradores.</H5>"
                 "<H5>Qt y el logo Qt son marcas comerciales de Digia Plc y/o su filial(es).</H5>"
                 "<H5>Qt es un producto Digia desarrollado como un proyecto de c&oacute;digo abierto. Consulte <a rel=\"nofollow\" target=\"_blank\" href=\"https://www.qt.io\" onclick=\"s_objectID=\"https://www.qt.io_1\";return this.s_oc?this.s_oc(e):true\"><font color=\"#505050\">qt.io</font></a> para m&aacute;s informaci&oacute;n.</H5><br>";
    QMessageBox messageAbout;
    messageAbout.setWindowTitle(tr("Acerca de Qt"));
    messageAbout.setWindowIcon(QPixmap("icons/qt.png"));
    messageAbout.setIconPixmap(QPixmap("icons/qt.png"));
    QSpacerItem* horizontalSpacer = new QSpacerItem(600, 400, QSizePolicy::Fixed, QSizePolicy::Fixed);
    dynamic_cast<QGridLayout*>(messageAbout.layout())->addItem(horizontalSpacer, 0, 0, 0, 0);
    messageAbout.setText("<p align=\"center\">" + title +
                         version +"</p>"+ "<p align=\"justify\">" + p1 +"</p>"+"<p align=\"justify\">" + p2 + "<ol>" + p3 + "</ol>" + "</p>" +
                         "<p align=\"center\">" + p4 +"</p>");
    messageAbout.exec();
    this->setVisible(true);
}

void MainWindow::aboutCuda(){
    std::cout<<"MainWindow::aboutCUDA"<<endl;

    this->setVisible(false);
    QString title = "<h1><b>PLATAFORMA DE C&Aacute;LCULO PARALELO CUDA&reg;</b></h1>";
    QString version = "<H5>Versi&oacute;n 7.0</H5>";
    QString p1 = "La plataforma de c&aacute;lculo paralelo "
            "<a rel=\"nofollow\" target=\"_blank\" href=\"http://developer.nvidia.com/cuda\" onclick=\"s_objectID=\"http://developer.nvidia.com/cuda_1\";return this.s_oc?this.s_oc(e):true\"><font color=\"#505050\">CUDA&reg;</font></a> "
            "proporciona unas cuantas extensiones de C y <a rel=\"nofollow\" target=\"_blank\" href=\"http://developer.nvidia.com/cuda-toolkit\" onclick=\"s_objectID=\"http://developer.nvidia.com/cuda-toolkit_1\";return this.s_oc?this.s_oc(e):true\"><font color=\"#505050\">C++</font></a>"
            " que permiten implementar el paralelismo en el procesamiento de tareas y datos con diferentes niveles de granularidad. El programador puede expresar ese paralelismo mediante diferentes lenguajes de alto nivel como C, "
            "C++ y Fortran o mediante est&aacute;ndares abiertos como las directivas de OpenACC"
            ". En la actualidad, la plataforma CUDA&reg; se utiliza en miles de aplicaciones aceleradas en la GPU y en miles de "
            "<a rel=\"nofollow\" target=\"_blank\" href=\"http://scholar.google.com/scholar?q=cuda+gpu\" onclick=\"s_objectID=\"http://scholar.google.com/scholar?q=cuda+gpu_1\";return this.s_oc?this.s_oc(e):true\"><font color=\"#505050\">artículos de investigaci&oacute;n publicados</font></a>.";
    QString p2 = "Los desarrolladores disponen de una gama completa de "
            "<a rel=\"nofollow\" target=\"_blank\" href=\"http://developer.nvidia.com/cuda-tools-ecosystem\" onclick=\"s_objectID=\"http://developer.nvidia.com/cuda-tools-ecosystem_1\";return this.s_oc?this.s_oc(e):true\"><font color=\"#505050\">herramientas y soluciones pertenecientes al ecosistema de CUDA&reg;</font></a>.";
    QString p3 =  "<H5>Visita "
            "<a rel=\"nofollow\" target=\"_blank\" href=\"https://developer.nvidia.com/cuda-zone\" onclick=\"s_objectID=\"https://developer.nvidia.com/cuda-zone_2\";return this.s_oc?this.s_oc(e):true\"><font color=\"#505050\">CUDA&reg; Zone</font></a> "
            "para obtener m&aacute;s informaci&oacute;n sobre el desarrollo con CUDA&reg;.</H5>";
    QMessageBox messageAbout;
    messageAbout.setWindowTitle(tr("Acerca de CUDA®"));
    messageAbout.setWindowIcon(QPixmap("icons/cuda.png"));
    messageAbout.setIconPixmap(QPixmap("icons/nvidia-logo.png"));
    QSpacerItem* horizontalSpacer = new QSpacerItem(300, 250, QSizePolicy::Fixed, QSizePolicy::Fixed);
    dynamic_cast<QGridLayout*>(messageAbout.layout())->addItem(horizontalSpacer, 0, 0, 0, 0);
    messageAbout.setText("<p align=\"center\">" + title +
                          version +"</p>"+ "<p align=\"justify\">"+p1 + "</p>" + "<p align=\"justify\">" + p2 + "</p>" + "<p align=\"center\">" + p3 +
                          "</p>");
    messageAbout.exec ();
    this->setVisible(true);
}

void MainWindow::launchWave(){
    //escucha lo que esta en input busca en las neuronas que tengan conocimientos y si esta guardado
    //pinta el patron
    std::cout<<"MainWindow::launchwave";
    chemicalLayerEar->clear();
    if(ui->lineEditEarInput->text().isEmpty())
        return;
    int categoryWave = returnCategory(ui->lineEditEarInput->text());
    int temp[16];
    for (int indexNeuron = 0; indexNeuron < *(neuralSenses[HEARING].ptr); indexNeuron++) {
        int currentCategory = neuralSenses[HEARING].vectorFlags[indexNeuron * SIZE_FLAGS + CAT];
        if(currentCategory != categoryWave)
            continue;
        for (int i = 0; i < SIZE_CHARACTERISTIC/2 ; i++)
            temp[i]=neuralSenses[HEARING].binaryCharacteristic[indexNeuron * SIZE_CHARACTERISTIC/2 + i];  //obtengo el vector desde la neurona con conocimiento
        chemicalLayerEar->paintPattern(temp,SIZE_CHARACTERISTIC/2);// pintar el patron en el grid
        break;
    }
}

void MainWindow::runCrossing(){
    std::cout<<"MainWindow::runCrossing"<<endl;

    if(interface[HEARING].arrayCategory[0] > 9)
        if(ui->checkBox_leer->isChecked() || ui->checkBox_syl->isChecked())
            formsTeaching[SIGHT]->getLineEditInput()->setText(word2);
        else
            formsTeaching[SIGHT]->getLineEditInput()->setText(ui->lineEditEarInput->text());
    else
        if(ui->checkBox_leer->isChecked() || ui->checkBox_syl->isChecked())
            formsTeaching[SIGHT]->getLineEditInput()->setText(word2);
        else
            formsTeaching[SIGHT]->getLineEditInput()->setText(QString::number(interface[HEARING].arrayCategory[0]));
    formsTeaching[SIGHT]->getPtrButtonTeach()->setEnabled(true);
}

///REVISAR MUY BIEN ESTA FUNCION//////
void MainWindow::generateDot(QString nameFile, senses sense, bool onlyHits){
    std::cout<<"MainWindow::generateDot"<<endl;

    ofstream file(nameFile.toStdString().c_str());
    int id,centinel;
    int category;
    unsigned char * vector= (onlyHits) ? interface[sense].arrayCategory : returnVectorCategory(sense);
    centinel= (onlyHits) ?*(interface[sense].hits) : *(neuralSenses[sense].ptr);
    file<<"graph net_neuron{\n";
    file<<"rankdir=LR;\n";
    for(int j=0; j <= 104;j++){
        file<<"subgraph cluster_"<<j<<"{ ";
        for(int i = 0; i < centinel ; i++){
            id=(onlyHits) ? interface[sense].id[i] : i;
            category=vector[i];
            if(category != j){
                if(j==10 && category==43)
                    file<<"\"item"<<id<<"\" [label=  \"id Neurona: "<<id<<"\\nCategor&iacute;a: '+'\"];\n";
                if(j==11 && category==61)
                    file<<"\"item"<<id<<"\" [label=  \"id Neurona: "<<id<<"\\nCategor&iacute;a: '='\"];\n";
                if(j==12 && category==65)
                    file<<"\"item"<<id<<"\" [label=  \"id Neurona: "<<id<<"\\nCategor&iacute;a: 'A'\"];\n";
                if(j==13 && category==66)
                    file<<"\"item"<<id<<"\" [label=  \"id Neurona: "<<id<<"\\nCategor&iacute;a: 'B'\"];\n";
                if(j==14 && category==67)
                    file<<"\"item"<<id<<"\" [label=  \"id Neurona: "<<id<<"\\nCategor&iacute;a: 'C'\"];\n";
                if(j==15 && category==68)
                    file<<"\"item"<<id<<"\" [label=  \"id Neurona: "<<id<<"\\nCategor&iacute;a: 'D'\"];\n";
                if(j==16 && category==69)
                    file<<"\"item"<<id<<"\" [label=  \"id Neurona: "<<id<<"\\nCategor&iacute;a: 'E'\"];\n";
                if(j==17 && category==70)
                    file<<"\"item"<<id<<"\" [label=  \"id Neurona: "<<id<<"\\nCategor&iacute;a: 'F'\"];\n";
                if(j==18 && category==71)
                    file<<"\"item"<<id<<"\" [label=  \"id Neurona: "<<id<<"\\nCategor&iacute;a: 'G'\"];\n";
                if(j==19 && category==72)
                    file<<"\"item"<<id<<"\" [label=  \"id Neurona: "<<id<<"\\nCategor&iacute;a: 'H'\"];\n";
                if(j==20 && category==73)
                    file<<"\"item"<<id<<"\" [label=  \"id Neurona: "<<id<<"\\nCategor&iacute;a: 'I'\"];\n";
                if(j==21 && category==74)
                    file<<"\"item"<<id<<"\" [label=  \"id Neurona: "<<id<<"\\nCategor&iacute;a: 'J'\"];\n";
                if(j==22 && category==75)
                    file<<"\"item"<<id<<"\" [label=  \"id Neurona: "<<id<<"\\nCategor&iacute;a: 'K'\"];\n";
                if(j==23 && category==76)
                    file<<"\"item"<<id<<"\" [label=  \"id Neurona: "<<id<<"\\nCategor&iacute;a: 'L'\"];\n";
                if(j==24 && category==77)
                    file<<"\"item"<<id<<"\" [label=  \"id Neurona: "<<id<<"\\nCategor&iacute;a: 'M'\"];\n";
                if(j==25 && category==78)
                    file<<"\"item"<<id<<"\" [label=  \"id Neurona: "<<id<<"\\nCategor&iacute;a: 'N'\"];\n";
                if(j==26 && category==79)
                    file<<"\"item"<<id<<"\" [label=  \"id Neurona: "<<id<<"\\nCategor&iacute;a: 'O'\"];\n";
                if(j==27 && category==80)
                    file<<"\"item"<<id<<"\" [label=  \"id Neurona: "<<id<<"\\nCategor&iacute;a: 'P'\"];\n";
                if(j==28 && category==81)
                    file<<"\"item"<<id<<"\" [label=  \"id Neurona: "<<id<<"\\nCategor&iacute;a: 'Q'\"];\n";
                if(j==29 && category==82)
                    file<<"\"item"<<id<<"\" [label=  \"id Neurona: "<<id<<"\\nCategor&iacute;a: 'R'\"];\n";
                if(j==30 && category==83)
                    file<<"\"item"<<id<<"\" [label=  \"id Neurona: "<<id<<"\\nCategor&iacute;a: 'S'\"];\n";
                if(j==31 && category==84)
                    file<<"\"item"<<id<<"\" [label=  \"id Neurona: "<<id<<"\\nCategor&iacute;a: 'T'\"];\n";
                if(j==32 && category==85)
                    file<<"\"item"<<id<<"\" [label=  \"id Neurona: "<<id<<"\\nCategor&iacute;a: 'U'\"];\n";
                if(j==33 && category==86)
                    file<<"\"item"<<id<<"\" [label=  \"id Neurona: "<<id<<"\\nCategor&iacute;a: 'V'\"];\n";
                if(j==34 && category==87)
                    file<<"\"item"<<id<<"\" [label=  \"id Neurona: "<<id<<"\\nCategor&iacute;a: 'W'\"];\n";
                if(j==35 && category==88)
                    file<<"\"item"<<id<<"\" [label=  \"id Neurona: "<<id<<"\\nCategor&iacute;a: 'X'\"];\n";
                if(j==36 && category==89)
                    file<<"\"item"<<id<<"\" [label=  \"id Neurona: "<<id<<"\\nCategor&iacute;a: 'Y'\"];\n";
                if(j==37 && category==90)
                    file<<"\"item"<<id<<"\" [label=  \"id Neurona: "<<id<<"\\nCategor&iacute;a: 'Z'\"];\n";
                if(j==38 && category==10)
                    file<<"\"item"<<id<<"\" [label=  \"id Neurona: "<<id<<"\\nCategor&iacute;a: 'AB'\"];\n";
                if(j==39 && category==11)
                    file<<"\"item"<<id<<"\" [label=  \"id Neurona: "<<id<<"\\nCategor&iacute;a: 'BA'\"];\n";
                if(j==40 && category==12)
                    file<<"\"item"<<id<<"\" [label=  \"id Neurona: "<<id<<"\\nCategor&iacute;a: 'CA'\"];\n";
                if(j==41 && category==13)
                    file<<"\"item"<<id<<"\" [label=  \"id Neurona: "<<id<<"\\nCategor&iacute;a: 'LLO'\"];\n";
                if(j==42 && category==14)
                    file<<"\"item"<<id<<"\" [label=  \"id Neurona: "<<id<<"\\nCategor&iacute;a: 'DA'\"];\n";
                if(j==43 && category==15)
                    file<<"\"item"<<id<<"\" [label=  \"id Neurona: "<<id<<"\\nCategor&iacute;a: 'SA'\"];\n";
                if(j==44 && category==16)
                    file<<"\"item"<<id<<"\" [label=  \"id Neurona: "<<id<<"\\nCategor&iacute;a: 'RA'\"];\n";
                if(j==45 && category==17)
                    file<<"\"item"<<id<<"\" [label=  \"id Neurona: "<<id<<"\\nCategor&iacute;a: 'PO'\"];\n";
                if(j==46 && category==18)
                    file<<"\"item"<<id<<"\" [label=  \"id Neurona: "<<id<<"\\nCategor&iacute;a: 'CE'\"];\n";
                if(j==47 && category==19)
                    file<<"\"item"<<id<<"\" [label=  \"id Neurona: "<<id<<"\\nCategor&iacute;a: 'GA'\"];\n";
                if(j==48 && category==20)
                    file<<"\"item"<<id<<"\" [label=  \"id Neurona: "<<id<<"\\nCategor&iacute;a: 'TO'\"];\n";
                if(j==49 && category==21)
                    file<<"\"item"<<id<<"\" [label=  \"id Neurona: "<<id<<"\\nCategor&iacute;a: 'TA'\"];\n";
                if(j==50 && category==22)
                    file<<"\"item"<<id<<"\" [label=  \"id NCheckBoxeurona: "<<id<<"\\nCategor&iacute;a: 'PA'\"];\n";
                if(j==51 && category==23)
                    file<<"\"item"<<id<<"\" [label=  \"id Neurona: "<<id<<"\\nCategor&iacute;a: 'DIS'\"];\n";
                if(j==52 && category==24)
                    file<<"\"item"<<id<<"\" [label=  \"id Neurona: "<<id<<"\\nCategor&iacute;a: 'RRO'\"];\n";
                if(j==53 && category==25)
                    file<<"\"item"<<id<<"\" [label=  \"id Neurona: "<<id<<"\\nCategor&iacute;a: 'JO'\"];\n";
                if(j==54 && category==26)
                    file<<"\"item"<<id<<"\" [label=  \"id Neurona: "<<id<<"\\nCategor&iacute;a: 'AR'\"];\n";
                if(j==55 && category==27)
                    file<<"\"item"<<id<<"\" [label=  \"id Neurona: "<<id<<"\\nCategor&iacute;a: 'CO'\"];\n";
                if(j==56 && category==28)
                    file<<"\"item"<<id<<"\" [label=  \"id Neurona: "<<id<<"\\nCategor&iacute;a: 'BRA'\"];\n";
                if(j==57 && category==29)
                    file<<"\"item"<<id<<"\" [label=  \"id Neurona: "<<id<<"\\nCategor&iacute;a: 'LO'\"];\n";
                if(j==58 && category==30)
                    file<<"\"item"<<id<<"\" [label=  \"id Neurona: "<<id<<"\\nCategor&iacute;a: 'CU'\"];\n";
                if(j==59 && category==31)
                    file<<"\"item"<<id<<"\" [label=  \"id Neurona: "<<id<<"\\nCategor&iacute;a: 'LE'\"];\n";
                if(j==60 && category==32)
                    file<<"\"item"<<id<<"\" [label=  \"id Neurona: "<<id<<"\\nCategor&iacute;a: 'BOL'\"];\n";
                if(j==61 && category==33)
                    file<<"\"item"<<id<<"\" [label=  \"id Neurona: "<<id<<"\\nCategor&iacute;a: 'ON'\"];\n";
                if(j==62 && category==34)
                    file<<"\"item"<<id<<"\" [label=  \"id Neurona: "<<id<<"\\nCategor&iacute;a: 'GO'\"];\n";
                if(j==63 && category==35)
                    file<<"\"item"<<id<<"\" [label=  \"id Neurona: "<<id<<"\\nCategor&iacute;a: 'VE'\"];\n";
                if(j==64 && category==36)
                    file<<"\"item"<<id<<"\" [label=  \"id Neurona: "<<id<<"\\nCategor&iacute;a: 'NA'\"];\n";
                if(j==65 && category==37)
                    file<<"\"item"<<id<<"\" [label=  \"id Neurona: "<<id<<"\\nCategor&iacute;a: 'DO'\"];\n";
                if(j==66 && category==38)
                    file<<"\"item"<<id<<"\" [label=  \"id Neurona: "<<id<<"\\nCategor&iacute;a: 'RRA'\"];\n";
                if(j==67 && category==39)
                    file<<"\"item"<<id<<"\" [label=  \"id Neurona: "<<id<<"\\nCategor&iacute;a: 'VION'\"];\n";
                if(j==68 && category==40)
                    file<<"\"item"<<id<<"\" [label=  \"id Neurona: "<<id<<"\\nCategor&iacute;a: 'DRI'\"];\n";
                if(j==69 && category==41){
                    if(sense == SIGHT){
                        file<<"\"item69CO\" [label=  \"id Neurona: 54\\nCategor&iacute;a: 'CO'\"];\n";
                        file<<"\"item69CO\" [label=  \"id Neurona: 54\\nCategor&iacute;a: 'CO'\"];\n";
                        file<<"\"item"<<id<<"\" [label=  \"id Neurona: "<<id<<"\\nCategor&iacute;a: 'COCO'\"];\n";
                    }
                }
                if(j==70 && category==42){
                    if(sense == SIGHT){
                        file<<"\"item70CA\" [label=  \"id Neurona: 40\\nCategor&iacute;a: 'CA'\"];\n";
                        file<<"\"item70BA\" [label=  \"id Neurona: 39\\nCategor&iacute;a: 'BA'\"];\n";
                        file<<"\"item70LLO\" [label=  \"id Neurona: 41\\nCategor&iacute;a: 'LLO'\"];\n";
                        file<<"\"item"<<id<<"\" [label=  \"id Neurona: "<<id<<"\\nCategor&iacute;a: 'CABALLO'\"];\n";
                    }
                }
                if(j==71 && category==44){
                    if(sense == SIGHT){
                        file<<"\"item71CA\" [label=  \"id Neurona: 40\\nCategor&iacute;a: 'CA'\"];\n";
                        file<<"\"item71SA\" [label=  \"id Neurona: 43\\nCategor&iacute;a: 'SA'\"];\n";
                        file<<"\"item"<<id<<"\" [label=  \"id Neurona: "<<id<<"\\nCategor&iacute;a: 'CASA'\"];\n";
                    }
                }
                if(j==72 && category==45){
                    if(sense == SIGHT){
                        file<<"\"item72SA\" [label=  \"id Neurona: 43\\nCategor&iacute;a: 'SA'\"];\n";
                        file<<"\"item72PO\" [label=  \"id Neurona: 45\\nCategor&iacute;a: 'PO'\"];\n";
                        file<<"\"item"<<id<<"\" [label=  \"id Neurona: "<<id<<"\\nCategor&iacute;a: 'SAPO'\"];\n";
                    }
                }
                if(j==73 && category==46){
                    if(sense == SIGHT){
                        file<<"\"item73GA\" [label=  \"id Neurona: 47\\nCategor&iacute;a: 'GA'\"];\n";
                        file<<"\"item73TO\" [label=  \"id Neurona: 48\\nCategor&iacute;a: 'TO'\"];\n";
                        file<<"\"item"<<id<<"\" [label=  \"id Neurona: "<<id<<"\\nCategor&iacute;a: 'GATO'\"];\n";
                    }
                }
                if(j==74 && category==47){
                    if(sense == SIGHT){
                        file<<"\"item74PA\" [label=  \"id Neurona: 50\\nCategor&iacute;a: 'PA'\"];\n";
                        file<<"\"item74TO\" [label=  \"id Neurona: 48\\nCategor&iacute;a: 'TO'\"];\n";
                        file<<"\"item"<<id<<"\" [label=  \"id Neurona: "<<id<<"\\nCategor&iacute;a: 'PATO'\"];\n";
                    }
                }
                if(j==75 && category==48){
                    if(sense == SIGHT){
                        file<<"\"item75CA\" [label=  \"id Neurona: 40\\nCategor&iacute;a: 'CA'\"];\n";
                        file<<"\"item75RRO\" [label=  \"id Neurona: 67\\nCategor&iacute;a: 'RRO'\"];\n";
                        file<<"\"item"<<id<<"\" [label=  \"id Neurona: "<<id<<"\\nCategor&iacute;a: 'CARRO'\"];\n";
                    }
                }
                if(j==76 && category==49){
                    if(sense == SIGHT){
                        file<<"\"item76GA\" [label=  \"id Neurona: 47\\nCategor&iacute;a: 'GA'\"];\n";
                        file<<"\"item76LLO\" [label=  \"id Neurona: 41\\nCategor&iacute;a: 'LLO'\"];\n";
                        file<<"\"item"<<id<<"\" [label=  \"id Neurona: "<<id<<"\\nCategor&iacute;a: 'GALLO'\"];\n";
                    }
                }
                if(j==77 && category==50){
                    if(sense == SIGHT){
                        file<<"\"item77CU\" [label=  \"id Neurona: 56\\nCategor&iacute;a: 'CU'\"];\n";
                        file<<"\"item77LE\" [label=  \"id Neurona: 57\\nCategor&iacute;a: 'LE'\"];\n";
                        file<<"\"item77BRA\" [label=  \"id Neurona: 55\\nCategor&iacute;a: 'BRA'\"];\n";
                        file<<"\"item"<<id<<"\" [label=  \"id Neurona: "<<id<<"\\nCategor&iacute;a: 'CULEBRA'\"];\n";
                    }
                }
                if(j==78 && category==51){
                    if(sense == SIGHT){
                        file<<"\"item78LE\" [label=  \"id Neurona: 57\\nCategor&iacute;a: 'LE'\"];\n";
                        file<<"\"item78ON\" [label=  \"id Neurona: 59\\nCategor&iacute;a: 'ON'\"];\n";
                        file<<"\"item"<<id<<"\" [label=  \"id Neurona: "<<id<<"\\nCategor&iacute;a: 'LEON'\"];\n";
                    }
                }
                if(j==79 && category==52){
                    if(sense == SIGHT){
                        file<<"\"item79VE\" [label=  \"id Neurona: 61\\nCategor&iacute;a: 'VE'\"];\n";
                        file<<"\"item79NA\" [label=  \"id Neurona: 62\\nCategor&iacute;a: 'NA'\"];\n";
                        file<<"\"item79DO\" [label=  \"id Neurona: 63\\nCategor&iacute;a: 'DO'\"];\n";
                        file<<"\"item"<<id<<"\" [label=  \"id Neurona: "<<id<<"\\nCategor&iacute;a: 'VENADO'\"];\n";
                    }
                }
                if(j==80 && category==53){
                    if(sense==SIGHT){
                        file<<"\"item80A\" [label=  \"id Neurona: 12\\nCategor&iacute;a: 'A'\"];\n";
                        file<<"\"item80VION\" [label=  \"id Neurona: 65\\nCategor&iacute;a: 'VION'\"];\n";
                        file<<"\"item"<<id<<"\" [label=  \"id Neurona: "<<id<<"\\nCategor&iacute;a: 'AVION'\"];\n";
                    }
                }
                if(j==81 && category==54){
                    if(sense == SIGHT){
                        file<<"\"item81CO\" [label=  \"id Neurona: 54\\nCategor&iacute;a: 'CO'\"];\n";
                        file<<"\"item81BRA\" [label=  \"id Neurona: 55\\nCategor&iacute;a: 'BRA'\"];\n";
                        file<<"\"item"<<id<<"\" [label=  \"id Neurona: "<<id<<"\\nCategor&iacute;a: 'COBRA'\"];\n";
                    }
                }
                ///PALABRAS AGREGADAS POR JOHAN SOSA//////////
                if(j == 82 && category == 55){
                    if(sense == SIGHT){
                        file<<"\"item"<<id<<"\" [label=  \"id Neurona: "<<id<<"\\nCategor&iacute;a: 'UN'\"];\n";
                    }
                }
                if(j==83 && category==56){
                    if(sense == SIGHT){
                        file<<"\"item83UN\" [label=  \"id Neurona: 82\\nCategor&iacute;a: 'UN'\"];\n";
                        file<<"\"item83O\" [label=  \"id Neurona: 26\\nCategor&iacute;a: 'O'\"];\n";
                        file<<"\"item"<<id<<"\" [label=  \"id Neurona: "<<id<<"\\nCategor&iacute;a: 'UNO'\"];\n";
                    }
                }
                if(j==84 && category==57){
                    if(sense == SIGHT){
                        file<<"\"item84DO\" [label=  \"id Neurona: 65\\nCategor&iacute;a: 'DO'\"];\n";
                        file<<"\"item84S\" [label=  \"id Neurona: 30\\nCategor&iacute;a: 'S'\"];\n";
                        file<<"\"item"<<id<<"\" [label=  \"id Neurona: "<<id<<"\\nCategor&iacute;a: 'DOS'\"];\n";
                    }
                }
                if(j==85 && category==58){
                    if(sense == SIGHT){
                        file<<"\"item"<<id<<"\" [label=  \"id Neurona: "<<id<<"\\nCategor&iacute;a: 'TRE'\"];\n";
                    }
                }
                if(j==86 && category==59){
                    if(sense == SIGHT){
                        file<<"\"item86TRE\" [label=  \"id Neurona: 85\\nCategor&iacute;a: 'TRE'\"];\n";
                        file<<"\"item86S\" [label=  \"id Neurona: 30\\nCategor&iacute;a: 'S'\"];\n";
                        file<<"\"item"<<id<<"\" [label=  \"id Neurona: "<<id<<"\\nCategor&iacute;a: 'TRES'\"];\n";
                    }
                }
                if(j==87 && category==60){
                    if(sense == SIGHT){
                        file<<"\"item"<<id<<"\" [label=  \"id Neurona: "<<id<<"\\nCategor&iacute;a: 'TRO'\"];\n";
                    }
                }
                if(j==88 && category==62){
                    if(sense == SIGHT){
                        file<<"\"item88CU\" [label=  \"id Neurona: 58\\nCategor&iacute;a: 'CU'\"];\n";
                        file<<"\"item88A\" [label=  \"id Neurona: 12\\nCategor&iacute;a: 'A'\"];\n";
                        file<<"\"item88TRO\" [label=  \"id Neurona: 87\\nCategor&iacute;a: 'TRO'\"];\n";
                        file<<"\"item"<<id<<"\" [label=  \"id Neurona: "<<id<<"\\nCategor&iacute;a: 'CUATRO'\"];\n";
                    }
                }
                if(j==89 && category==63){
                    if(sense == SIGHT){
                        file<<"\"item"<<id<<"\" [label=  \"id Neurona: "<<id<<"\\nCategor&iacute;a: 'CIN'\"];\n";
                    }
                }
                if(j==90 && category==64){
                    if(sense == SIGHT){
                        file<<"\"item90CIN\" [label=  \"id Neurona: 89\\nCategor&iacute;a: 'CIN'\"];\n";
                        file<<"\"item90CO\" [label=  \"id Neurona: 55\\nCategor&iacute;a: 'CO'\"];\n";
                        file<<"\"item"<<id<<"\" [label=  \"id Neurona: "<<id<<"\\nCategor&iacute;a: 'CINCO'\"];\n";
                    }
                }
                if(j==91 && category==91){
                    if(sense == SIGHT){
                        file<<"\"item"<<id<<"\" [label=  \"id Neurona: "<<id<<"\\nCategor&iacute;a: 'SE'\"];\n";
                    }
                }
                if(j==92 && category==92){
                    if(sense == SIGHT){
                        file<<"\"item"<<id<<"\" [label=  \"id Neurona: "<<id<<"\\nCategor&iacute;a: 'IS'\"];\n";
                    }
                }
                if(j==93 && category==93){
                    if(sense == SIGHT){
                        file<<"\"item93SE\" [label=  \"id Neurona: 91\\nCategor&iacute;a: 'SE'\"];\n";
                        file<<"\"item93IS\" [label=  \"id Neurona: 92\\nCategor&iacute;a: 'IS'\"];\n";
                        file<<"\"item"<<id<<"\" [label=  \"id Neurona: "<<id<<"\\nCategor&iacute;a: 'SEIS'\"];\n";
                    }
                }
                if(j==94 && category==94){
                    if(sense == SIGHT){
                        file<<"\"item"<<id<<"\" [label=  \"id Neurona: "<<id<<"\\nCategor&iacute;a: 'SI'\"];\n";
                    }
                }
                if(j==95 && category==95){
                    if(sense == SIGHT){
                        file<<"\"item"<<id<<"\" [label=  \"id Neurona: "<<id<<"\\nCategor&iacute;a: 'TE'\"];\n";
                    }
                }
                if(j==96 && category==96){
                    if(sense == SIGHT){
                        file<<"\"item96SI\" [label=  \"id Neurona: 94\\nCategor&iacute;a: 'SI'\"];\n";
                        file<<"\"item96E\" [label=  \"id Neurona: 16\\nCategor&iacute;a: 'E'\"];\n";
                        file<<"\"item96TE\" [label=  \"id Neurona: 95\\nCategor&iacute;a: 'TE'\"];\n";
                        file<<"\"item"<<id<<"\" [label=  \"id Neurona: "<<id<<"\\nCategor&iacute;a: 'SIETE'\"];\n";
                    }
                }
                if(j==97 && category==97){
                    if(sense == SIGHT){
                        file<<"\"item"<<id<<"\" [label=  \"id Neurona: "<<id<<"\\nCategor&iacute;a: 'CHO'\"];\n";
                    }
                }
                if(j==98 && category==98){
                    if(sense == SIGHT){
                        file<<"\"item98O\" [label=  \"id Neurona: 26\\nCategor&iacute;a: 'O'\"];\n";
                        file<<"\"item98CHO\" [label=  \"id Neurona: 97\\nCategor&iacute;a: 'CHO'\"];\n";
                        file<<"\"item"<<id<<"\" [label=  \"id Neurona: "<<id<<"\\nCategor&iacute;a: 'OCHO'\"];\n";
                    }
                }
                if(j==99 && category==99){
                    if(sense == SIGHT){
                        file<<"\"item"<<id<<"\" [label=  \"id Neurona: "<<id<<"\\nCategor&iacute;a: 'NU'\"];\n";
                    }
                }
                if(j==100 && category==100){
                    if(sense == SIGHT){
                        file<<"\"item"<<id<<"\" [label=  \"id Neurona: "<<id<<"\\nCategor&iacute;a: 'VE'\"];\n";
                    }
                }
                if(j==101 && category==101){
                    if(sense == SIGHT){
                        file<<"\"item101NU\" [label=  \"id Neurona: 99\\nCategor&iacute;a: 'NU'\"];\n";
                        file<<"\"item101E\" [label=  \"id Neurona: 16\\nCategor&iacute;a: 'E'\"];\n";
                        file<<"\"item101VE\" [label=  \"id Neurona: 100\\nCategor&iacute;a: 'VE'\"];\n";
                        file<<"\"item"<<id<<"\" [label=  \"id Neurona: "<<id<<"\\nCategor&iacute;a: 'NUEVE'\"];\n";
                    }
                }
                if(j==102 && category==102){
                    if(sense == SIGHT){
                        file<<"\"item"<<id<<"\" [label=  \"id Neurona: "<<id<<"\\nCategor&iacute;a: 'RO'\"];\n";
                    }
                }
                if(j==103 && category==103){
                    if(sense == SIGHT){
                        file<<"\"item103CE\" [label=  \"id Neurona: 46\\nCategor&iacute;a: 'CE'\"];\n";
                        file<<"\"item103RO\" [label=  \"id Neurona: 102\\nCategor&iacute;a: 'RO'\"];\n";
                        file<<"\"item"<<id<<"\" [label=  \"id Neurona: "<<id<<"\\nCategor&iacute;a: 'CERO'\"];\n";
                    }
                }
            }else{
                switch(category){
                case 0://if(category == 0)
                    file<<"\"item"<<id<<"\" [label=  \"id Neurona: "<<id<<"\\nCategor&iacute;a: '0'\"];\n";
                    break;
                case 1://if(category == 1)
                    file<<"\"item"<<id<<"\" [label=  \"id Neurona: "<<id<<"\\nCategor&iacute;a: '1'\"];\n";
                    break;
                case 2://if(category == 2)
                    file<<"\"item"<<id<<"\" [label=  \"id Neurona: "<<id<<"\\nCategor&iacute;a: '2'\"];\n";
                    break;
                case 3://if(category == 3)
                    file<<"\"item"<<id<<"\" [label=  \"id Neurona: "<<id<<"\\nCategor&iacute;a: '3'\"];\n";
                    break;
                case 4://if(category == 4)
                    file<<"\"item"<<id<<"\" [label=  \"id Neurona: "<<id<<"\\nCategor&iacute;a: '4'\"];\n";
                    break;
                case 5://if(category == 5)
                    file<<"\"item"<<id<<"\" [label=  \"id Neurona: "<<id<<"\\nCategor&iacute;a: '5'\"];\n";
                    break;
                case 6://if(category == 6)
                    file<<"\"item"<<id<<"\" [label=  \"id Neurona: "<<id<<"\\nCategor&iacute;a: '6'\"];\n";
                    break;
                case 7://if(category == 7)
                    file<<"\"item"<<id<<"\" [label=  \"id Neurona: "<<id<<"\\nCategor&iacute;a: '7'\"];\n";
                    break;
                case 8://if(category == 8)
                    file<<"\"item"<<id<<"\" [label=  \"id Neurona: "<<id<<"\\nCategor&iacute;a: '8'\"];\n";
                    break;
                case 9://if(category == 9)
                    file<<"\"item"<<id<<"\" [label=  \"id Neurona: "<<id<<"\\nCategor&iacute;a: '9'\"];\n";
                    break;
                }
            }
        }
        file<<"}\n";
    }
    file<<"\n}";
    file.close();
    if(!onlyHits)
        delete vector;
}

unsigned char *MainWindow::returnVectorCategory(senses sense){
    std::cout<<"MainWindow::returnVectorCategory"<<endl;

    int ptr = *(neuralSenses[sense].ptr);
    unsigned char * vector= new unsigned char [ptr];
    for(register int i=0; i < ptr; i++)
        vector[i] = neuralSenses[sense].vectorFlags[i * 9 + CAT];
    return vector;
}

bool MainWindow::multipleDevice(){
    std::cout<<"MainWindow::multipleDevice"<<endl;

    int deviceCount;
    cudaError_t error_id = cudaGetDeviceCount(&deviceCount);
    if(error_id != cudaSuccess){
        QString text("ERROR: cudaGetDeviceCount returned "+QString::number((int)error_id).append(cudaGetErrorString(error_id)));
        showWarning("Device no encontrado",text);
        exit(EXIT_FAILURE);
    }
    // This function call returns 0 if there are no CUDA capable devices.
    if(deviceCount == 0){
        showWarning("No Compatibilidad","ERROR: No se han encontrado dispostivos compatibles con CUDA\n");
        exit(EXIT_FAILURE);
    }
    if (deviceCount==1){
        cudaSetDevice(0);
        selectedDevice = 0;
        return false;
    }
    return true;
}

void MainWindow::generatePng(QString nameFile){
    std::cout<<"MainWindow::generatePng"<<endl;

    QString program = "dot";
    QStringList arguments;
    //dot net_hearing.dot -o net.svg -Tsvg
    arguments <<nameFile.toStdString().c_str()<<"-o" << "./Obj/net.svg"<<"-Tsvg";
    QProcess *myProcess = new QProcess(this);
    myProcess->start(program, arguments);
}

void MainWindow::showWarning(QString windowTitle, QString message){
    std::cout<<"MainWindow::showWarning"<<endl;

    QMessageBox warning(QMessageBox::Warning,windowTitle,message,QMessageBox::Ok,0);
    warning.setIconPixmap(QPixmap(":/icons/warning.png"));
    warning.exec();
}




//funcion para aprender por medio de la vista y del oido
void MainWindow::learn(senses sense){
    std::cout<<"learn"<<endl;

    isInactivateSense[sense] = true;
    switch(stateSenses[sense]){
    case NO_HIT:
        realLearn(sense);
        break;
    case IS_HIT:
        try{
        correct(&neuralSenses[sense],formsTeaching[sense]->getLineEditInput()->text().toInt(),deviceProp.maxThreadsPerBlock,statistics);
        realLearn(sense);
        reset(&neuralSenses[sense],deviceProp.maxThreadsPerBlock,statistics);
        break;
    }catch(string text){
            QMessageBox::critical(this,"CUDA ERROR",QString(text.c_str()).append("\n Se cerrara Aplicación"));
            exit(EXIT_FAILURE);
        }
    case DIFF:
        try{
        std::cout<<"categoria numero: "<<formsTeaching[sense]->getLineEditInput()->text().toInt()<<std::cout;
        correct(&neuralSenses[sense],formsTeaching[sense]->getLineEditInput()->text().toInt(),deviceProp.maxThreadsPerBlock,statistics);
        stateSenses[sense] = recognize(&neuralSenses[sense],sizeNet,characteristicVectorEar,&interface[sense],statistics);
        break;
    }catch(string text){
            QMessageBox::critical(this,"CUDA ERROR",QString(text.c_str()).append("\n Se cerrara Aplicación"));
            exit(EXIT_FAILURE);
        }
    default:
        exit(EXIT_FAILURE);
    }
    formsTeaching[sense]->init();
    activeLayers(true);
    saveSight();
}

void MainWindow::realLearn(senses sense){

    unsigned char ptr =(*(neuralSenses[sense].ptr))++;
    if(ptr > sizeNet.numNeuron){
        showWarning("Red no dimensionable","No es posible aprender mas Patrones\nERROR:Numero de neuronas Agotadas");
        return;
    }
    neuralSenses[sense].vectorFlags[ptr * SIZE_FLAGS + KNW] = 1;
    neuralSenses[sense].vectorFlags[ptr * SIZE_FLAGS + CAT] = returnCategory(formsTeaching[sense]->getLineEditInput()->text());
    learnBinaryCharacteristic(sense, ptr);
}

int MainWindow::returnCategory(QString cad){
  std::cout<<cad.toStdString()<<std::endl;

    if(cad != "=" && cad != "+" && cad != "A" && cad != "B" && cad != "C" && cad != "D" && cad != "E" && cad != "F" && cad != "G" && cad != "H" && cad != "I" && cad != "J" && cad != "K"
            && cad != "L" && cad != "M" && cad != "N" && cad != "O" && cad != "P" && cad != "Q" && cad != "R" && cad != "S" && cad != "T" && cad != "U" && cad != "V" && cad != "W" && cad != "X"
            && cad != "Y" && cad != "Z" && cad != "AB" && cad != "BA" && cad != "CA" && cad != "LLO" && cad != "CABALLO" && cad != "SA" && cad != "CASA" && cad != "PO" && cad != "SAPO" && cad != "GA" && cad != "TO" && cad != "GATO" && cad != "PA"
            && cad != "PATO" && cad != "RRO" && cad != "CARRO" && cad != "GALLO" && cad != "CO" && cad != "BRA" && cad != "COBRA" && cad != "CU" && cad != "LE" && cad != "CULEBRA" && cad != "ON" && cad != "LEON" && cad != "VE"
            && cad != "NA" && cad != "DO" && cad != "VENADO" && cad != "VION" && cad != "AVION" && cad != "COCO" && cad != "DA" && cad != "RA" && cad != "CE" && cad != "TA" && cad != "DIS" && cad != "JO" && cad != "AR"
            && cad != "BOL" && cad != "GO" && cad != "RRA" && cad != "DRI" && cad != "LO" && cad != "UN" && cad != "UNO" && cad != "DOS" && cad != "TRE" && cad != "TRES" && cad != "TRO" && cad != "CUATRO" && cad != "CIN"
            && cad != "CINCO" && cad != "SE" && cad != "IS" && cad != "SEIS" && cad != "SI" && cad != "TE" && cad != "SIETE" && cad != "CHO" && cad != "OCHO" && cad != "NU" && cad != "VE" && cad != "NUEVE" && cad != "RO" && cad != "CERO")
        return cad.toInt();
    std::cout<<"CATEGORYA NRO"<<cad.toInt()<<std::endl;

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
    exit(1);
}

void MainWindow::learnBinaryCharacteristic(senses sense,int ptr){
    std::cout<<"MainWindow::learnBinaryCharacteristic"<<endl;

    characteristicBinaryVector = (sense==SIGHT) ? chemicalLayerEye->generateBinaryCharacteristic() : chemicalLayerEar->generateBinaryCharacteristic();
    for (int i = 0; i < 16; i++)
        neuralSenses[sense].binaryCharacteristic[ptr*16+i] = characteristicBinaryVector[i];
}

void MainWindow::initGui(){
    std::cout<<"MainWindow::initGui"<<endl;

    ui->setupUi(this);
    chemicalLayerEar= new ChemicalLayer(16,16,12,12);
    chemicalLayerEar->setIsEditable(false);
    chemicalLayerEye= new ChemicalLayer(16,16,12,12);
    chemicalLayerEye->setIsEditable(false);
    ui->lineEditEarInput->setEnabled(false);
    ui->InputWordCount->setEnabled(false);
    ui->horizontalLayoutEar->addWidget(chemicalLayerEar);
    ui->horizontalLayoutEye->addWidget(chemicalLayerEye);
    ui->pushButtonBum->setEnabled(true);
    ui->pushButtonBum->setStyleSheet("*{background-color: rgb(96,96,96,120)}");
    ui->pushButtonProcess->setEnabled(false);
    ui->pushButtonBipSyl->hide();
    ui->pushButtonBipWord->hide();
    ui->pushButtonBip->hide();
    ui->pushButtonCheck->hide();
    ui->textBrowser->hide();
    ui->pushButton_stopCount->hide();
    ui->pushButton_stopCount->hide();
    ui->pushButton_teachBip->hide();
    ui->pushButton_teachClack->hide();
    ui->pushButtonBNSWord->hide();
    ui->pushButtonSoundWord->hide();
    ui->pushButtonImage->hide();
    ui->pushButtonImages->hide();
    ui->pushButtonCorrectWord->hide();
    ui->textBrowserWord->hide();
    word.clear();
    QRegExp alphabet("[A-Z0-9+=][A-Z]*");
    ui->lineEditEarInput->setValidator(new QRegExpValidator(alphabet,this));
    ui->InputWordCount->setValidator(new QRegExpValidator(alphabet,this));
    ViewFinder::getInstance(this);
    connect(chemicalLayerEar,SIGNAL(change()),this,SLOT(activateButtonBip()));
    connect(chemicalLayerEye,SIGNAL(change()),this,SLOT(activateButtonBip()));
    connect(chemicalLayerEar,SIGNAL(change()),this,SLOT(activateButtonCheck()));
    connect(chemicalLayerEye,SIGNAL(change()),this,SLOT(activateButtonCheck()));
    connect(chemicalLayerEar,SIGNAL(change()),this,SLOT(activateButtonBipSyl()));
    connect(chemicalLayerEye,SIGNAL(change()),this,SLOT(activateButtonBipSyl()));
    connect(chemicalLayerEar,SIGNAL(change()),this,SLOT(activateButtonBipWord()));
    connect(chemicalLayerEye,SIGNAL(change()),this,SLOT(activateButtonBipWord()));
    connect(ui->pushButtonBum,SIGNAL(clicked()),this,SLOT(clickBum()));
    connect(ui->pushButtonProcess,SIGNAL(clicked()),this,SLOT(activeLayers()));
    connect(ui->pushButtonBipWord,SIGNAL(clicked()),this,SLOT(clickBipSyl()));
    connect(ui->pushButtonBipSyl,SIGNAL(clicked()),this,SLOT(protocolCheck()));
    connect(ui->pushButtonCheck,SIGNAL(clicked()),this,SLOT(clickBipSyl()));
    connect(ui->pushButtonCheck,SIGNAL(clicked()),this,SLOT(deactivateBip()));
    connect(ui->buttonClearEye,SIGNAL(clicked()),chemicalLayerEar,SLOT(clear()));
    connect(ui->buttonClearEye,SIGNAL(clicked()),chemicalLayerEye,SLOT(clear()));
    connect(ui->buttonClearEye,SIGNAL(clicked()),this,SLOT(clearWord()));
    connect(ui->pushButtonBNSWord,SIGNAL(clicked()),SLOT(paintPng()));
    connect(ui->pushButtonSoundWord,SIGNAL(clicked()),this,SLOT(paintBinaryNetWord()));
    connect(ui->pushButtonImage,SIGNAL(clicked()),this,SLOT(paintBinaryNetSyllab()));
    connect(ui->pushButtonImages,SIGNAL(clicked()),this,SLOT(fillTable()));
    connect(ui->pushButtonCorrectWord,SIGNAL(clicked()),this,SLOT(correctWord()));
    connect(ui->pushButton_stopCount,SIGNAL(clicked()),this, SLOT(stopCount()));
    connect(ui->pushButton_teachBip,SIGNAL(clicked()),this,SLOT(countProtocol()));
    connect(ui->pushButton_teachClack,SIGNAL(clicked()),this, SLOT(orderProtocol()));
    connect(this,SIGNAL(cross()),this,SLOT(runCrossing()));
    setMinimumSize(1000,700);
    setMaximumSize(1000,700);
    file();
}

void MainWindow::activateInterface(bool state){
    std::cout<<"MainWindow::activateInterface"<<endl;

    ui->lineEditEarInput->setEnabled(state);
    ui->buttonClearEye->setEnabled(state);
    chemicalLayerEar->setActiveLayer(state);
    chemicalLayerEye->setActiveLayer(state);
}

void MainWindow::setNull(){
    std::cout<<"MainWindow::setNull"<<endl;

    characteristicVectorEar = NULL;
    characteristicVectorEye = NULL;
    characteristicBinaryVector = NULL;
    neuralSenses = NULL;
    stateSenses = NULL;
    formsTeaching = NULL;
    interface = NULL;
    isInactivateSense = NULL;
    image = NULL;
    dialogConsult = NULL;
    dialogStatistics = NULL;
    dialogTable = NULL;
    dialogTableWord = NULL;
    dialogSelectHardware = NULL;
    statistics = NULL;
    selectedDevice = -1;
    adding_up.foward = NULL;
    adding_up.back = NULL;
    adding_down.foward = NULL;
    adding_down.back = NULL;
    result.foward = NULL;
    result.back = NULL;
}

void MainWindow::createTablesCharacteristic(){
    std::cout<<"MainWindow::createTablesCharacteristic"<<endl;

    initializeTable();
    createStringForTable();
}

void MainWindow::initializeTable(){
    std::cout<<"MainWindow::initializeTable "<<endl;

    ui->tableWidgetColumnEar->setRowCount(1);
    ui->tableWidgetColumnEar->setColumnCount(16);
    ui->tableWidgetColumnEar->verticalHeader()->hide();
    ui->tableWidgetRowEar->setRowCount(16);
    ui->tableWidgetRowEar->setColumnCount(1);
    ui->tableWidgetRowEar->horizontalHeader()->hide();
    ui->tableWidgetColumnEye->setRowCount(1);
    ui->tableWidgetColumnEye->setColumnCount(16);
    ui->tableWidgetColumnEye->verticalHeader()->hide();
    ui->tableWidgetRowEye->setRowCount(16);
    ui->tableWidgetRowEye->setColumnCount(1);
    ui->tableWidgetRowEye->horizontalHeader()->hide();
}

void MainWindow::intitializeSenses(int numSenses){
    std::cout<<"MainWindow::initializeSenses"<<endl;

    int calc;
    orderNeuron=1;
    kNeuron=1;
    this->numSenses=numSenses;
    int numNeuron=calculateNumNeuron();
    statistics = new Statistic [4];
    neuralSenses = new NeuralNetwork[numSenses];
    stateSenses = new stateNeuralNetwork[numSenses];
    interface = new Interface [numSenses];
    sizeNet.numNeuron=numNeuron;
    orderNetwork = new OrderNetwork;
    calc = sizeNet.numNeuron * sizeof(unsigned char);
    sizeNet.sizeVectorNeuron = calc * 32;
    sizeNet.sizevectorFlags = calc * 9;
    sizeNet.sizeBinaryCharacteristic = sizeNet.numNeuron * sizeof(unsigned short) * 16;
    orderNetwork->countNet = new CountNetwork[sizeNet.numNeuron];
    countNetwork = new CountNetwork;
    countNetwork->vectorNetworkCount = new unsigned char[sizeNet.numNeuron * 32];
    countNetwork->vectorPointerCount = new unsigned char[sizeNet.numNeuron * 9];
    countNetwork->bipPointer = new unsigned char [sizeNet.numNeuron * 9];
    countNetwork->clackPointer = new unsigned char [sizeNet.numNeuron * 9];
    countNetwork->ptr = new unsigned char (1);
    orderNetwork->bumPointer = new unsigned char [sizeNet.numNeuron * 9];
    orderNetwork->category = new unsigned char [sizeNet.numNeuron * 32];
    orderNetwork->order = new unsigned char [sizeNet.numNeuron * 32];
    orderNetwork->numRelation = new unsigned char (1);
    sumNetwork = new SumNetwork;
    sumNetwork->vectorNetworkSum = new unsigned char [sizeNet.numNeuron * 32];
    sumNetwork->vectorPointerSum = new unsigned char [sizeNet.numNeuron * 9];
    for(register int i=0; i<(int)sizeNet.numNeuron; i++){
        orderNetwork->countNet[i].vectorNetworkCount = new unsigned char [sizeNet.numNeuron * 32];
        orderNetwork->countNet[i].vectorPointerCount = new unsigned char [sizeNet.numNeuron * 9];
        orderNetwork->countNet[i].ptr = new unsigned char (1);
    }
    for(register int i = 0; i < numSenses; i++){
        neuralSenses[i].vectorNeuron = new unsigned char [sizeNet.numNeuron * 32];
        neuralSenses[i].vectorFlags = new unsigned int [sizeNet.numNeuron * 9];
        neuralSenses[i].binaryCharacteristic = new unsigned short [sizeNet.numNeuron * 16];
        neuralSenses[i].ptr = new unsigned char (0);
    }
    try{
        boot(neuralSenses,sizeNet,statistics);
    }catch(string text){
        QMessageBox::critical(this,"CUDA ERROR",QString(text.c_str()).append("\n Se cerrara Aplicación"));
        exit(EXIT_FAILURE);
    }
}

void MainWindow::createStringForTable(){
    std::cout<<"MainWindow::createStringForTable"<<endl;

    QStringList listStringRow,listStringColumn;
    QString auxRow,auxColumn;
    for(int i=0; i<16; i++){
        auxRow="v[";
        auxRow.append(QString::number(i+1));
        auxRow.append("]");
        listStringRow<<auxRow;
        auxColumn="v[";
        auxColumn.append(QString::number(i+17));
        auxColumn.append("]");
        listStringColumn<<auxColumn;
        QTableWidgetItem *newItem = new QTableWidgetItem(QString::number(characteristicVectorEar[i]));
        newItem->setTextAlignment(Qt::AlignVCenter);
        newItem->setTextAlignment(Qt::AlignHCenter);
        ui->tableWidgetRowEar->setItem(i, 0, newItem);
        newItem = new QTableWidgetItem(QString::number(characteristicVectorEar[i+16]));
        newItem->setTextAlignment(Qt::AlignVCenter);
        newItem->setTextAlignment(Qt::AlignHCenter);
        ui->tableWidgetColumnEar->setItem(0, i, newItem);
        newItem = new QTableWidgetItem(QString::number(characteristicVectorEye[i]));
        newItem->setTextAlignment(Qt::AlignVCenter);
        newItem->setTextAlignment(Qt::AlignHCenter);
        ui->tableWidgetRowEye->setItem(i, 0, newItem);
        newItem = new QTableWidgetItem(QString::number(characteristicVectorEye[i+16]));
        newItem->setTextAlignment(Qt::AlignVCenter);
        newItem->setTextAlignment(Qt::AlignHCenter);
        ui->tableWidgetColumnEye->setItem(0, i, newItem);
    }
    setDataTable(listStringRow,listStringColumn);
}

void MainWindow::earTraining(){
    std::cout<<"MainWindow::earTraining"<<endl;

    senseTraining("ear_category.o","ear_wave.o","./Obj/",104,HEARING);
}

void MainWindow::senseTraining(QString nameFileCategories, QString nameFileWaves,QString path,int numPatterns,senses sense){
    std::cout<<"MainWindow::senseTraining"<<endl;

    int category;
    int waveCharacteristic[SIZE_CHARACTERISTIC/2];
    bool condition;
    ChemicalLayer *layer = (sense == HEARING)? chemicalLayerEar: chemicalLayerEye;
    FormTeaching  *formTeaching= formsTeaching[sense];
    ifstream categories((path+nameFileCategories).toStdString().c_str());
    if(!categories){
        showWarning("Error abriendo Archivo","Archivo que contiene categorías para entrenar oido no se puede abrir aplicacion se cerrara");
        exit(EXIT_SUCCESS);
    }
    ifstream waves((path+nameFileWaves).toStdString().c_str());
    if(!waves){
        showWarning("Error abriendo Archivo","Archivo que contiene Ondas para entrenar oido no se puede abrir aplicacion se cerrara");
        categories.close();
        exit(EXIT_SUCCESS);
    }
    do{
        categories.seekg(0);
        waves.seekg(0);
        condition = false;
        for(int i = 0; i < numPatterns; i++){
            //interaction is simulated user-GUI
            waves.read(reinterpret_cast<char*>(&waveCharacteristic),SIZE_CHARACTERISTIC/2 *sizeof(int));
            categories.read(reinterpret_cast<char*>(&category),sizeof(int));
            layer->paintPattern(waveCharacteristic, SIZE_CHARACTERISTIC/2);
            processGrid();
            activeLayers();
            switch(stateSenses[sense]){
            case NO_HIT:
                formTeaching->getLineEditInput()->setText(QString::number(category));
                formTeaching->getPtrButtonTeach()->clicked();
                word.clear();
                break;
            case IS_HIT:
                if(returnCategory(formTeaching->getLineEditOut()->text()) != category){
                    formTeaching->badAnswer();
                    formTeaching->getLineEditInput()->setText(QString::number(category));
                    formTeaching->getPtrButtonTeach()->clicked();
                    condition = true;
                }else{
                    formTeaching->init();
                    finishGoodAnswer(sense);
                    (sense == HEARING)? resetHearing(): resetSight();
                }
                break;
            case DIFF:
                formTeaching->getLineEditInput()->setText(QString::number(category));
                formTeaching->getPtrButtonTeach()->clicked();
                condition = true;
                break;
            default:
                exit(EXIT_FAILURE);
            }
        }
    }while(condition);
    categories.close();
    waves.close();
}

void MainWindow::setDataTable(QStringList listStringRow, QStringList listStringColumn){
    std::cout<<"MainWindow::setDataTable"<<endl;

    ui->tableWidgetColumnEar->setHorizontalHeaderLabels(listStringColumn);
    ui->tableWidgetRowEar->setVerticalHeaderLabels(listStringRow);
    ui->tableWidgetColumnEye->setHorizontalHeaderLabels(listStringColumn);
    ui->tableWidgetRowEye->setVerticalHeaderLabels(listStringRow);
}

int MainWindow::calculateNumNeuron(){
    std::cout<<"MainWindow::calculateNumNeuron"<<endl;

    cudaSetDevice(selectedDevice);
    cudaGetDeviceProperties(&deviceProp,selectedDevice);
    int sizeVectorCharacteristic = sizeof(unsigned char)*32;
    int sizeVectorFlags = sizeof(unsigned int)*9;
    int sizePtr = sizeof(unsigned char);
    int sizeNeuron = sizeVectorCharacteristic + sizeVectorFlags + sizePtr;
    int numNeuron = deviceProp.sharedMemPerBlock/sizeNeuron * deviceProp.multiProcessorCount;
    numNeuron = numNeuron - (numNeuron%deviceProp.warpSize);
    printf("Caracteristica= %d banderas=%d sizeNeuron= %d numneuronas=%d \n",sizeVectorCharacteristic,sizeVectorFlags,sizeNeuron,numNeuron);
    return numNeuron;
    /*
     *calcule neuronas que puede tener un bloque porque pregunte
     *que es lo maximo que cabe en memoria compartida
     *sugerencia es mejor preguntar por los hilos y las dimensiones
     *y ver cuantas neuronas puedo correr en funcion de las dimensiones de hilos
     *releer capitulo
     */
}

void MainWindow::createInterfacesTeaching(){
    std::cout<<"MainWindow::createInterfacesTeaching"<<endl;
    ///Revisar Bien esta funcion//////////////////////
    formsTeaching = new FormTeaching*[numSenses];
    isInactivateSense = new bool[numSenses];
    for(int i=0; i<numSenses; i++){
        formsTeaching[i] = new FormTeaching;
        isInactivateSense[i] = false;
    }
    formsTeaching[SIGHT]->setSenseTeaching(SIGHT);
    formsTeaching[HEARING]->setSenseTeaching(HEARING);
    connect(formsTeaching[SIGHT],SIGNAL(signalGoodAnswer(senses)),this,SLOT(finishGoodAnswer(senses)));
    connect(formsTeaching[HEARING],SIGNAL(signalGoodAnswer(senses)),this,SLOT(finishGoodAnswer(senses)));
    connect(formsTeaching[SIGHT]->getPtrButtonTeach(),SIGNAL(clicked()),this,SLOT(learnSight()));
    connect(formsTeaching[HEARING]->getPtrButtonTeach(),SIGNAL(clicked()),this,SLOT(learnHearing()));
    connect(formsTeaching[SIGHT]->getPtrButtonGoodAnswer(),SIGNAL(clicked()),this,SLOT(resetSight()));
    connect(formsTeaching[HEARING]->getPtrButtonGoodAnswer(),SIGNAL(clicked()),this,SLOT(resetHearing()));
    connect(formsTeaching[SIGHT]->getPtrButtonState(),SIGNAL(clicked()),this,SLOT(paintStateSight()));
    connect(formsTeaching[HEARING]->getPtrButtonState(),SIGNAL(clicked()),this,SLOT(paintStateHearing()));
    ui->horizontalLayoutInterfaceEar->addWidget(formsTeaching[HEARING]);
    ui->horizontalLayoutInterfaceSight->addWidget(formsTeaching[SIGHT]);
}

void MainWindow::setFormsCheck(bool state){
    std::cout<<"MainWindow::setFormsCheck"<<endl;

    for(int i=0; i<numSenses; i++)
        isInactivateSense[i] = state;
}

void MainWindow::printAllVectorSenses(){
    std::cout<<"MainWindow::printAllVectorSenses"<<endl;

    for(register int i=0; i<this->numSenses; i++){
        printf("Sentido %d \n",i);
        for(register int s=0; s<2336; s++){
            printf("neurona %d \n",s);
            for(register int j=0; j<32; j++){
                printf("v[%d]=%d \n",j,neuralSenses[i].vectorNeuron[s*32+j]);
            }
            for(register int k=0; k<9; k++){
                printf("f[%d]=%d \n",k,neuralSenses[i].vectorFlags[s*9+k]);
            }
            for(register int l=0; l<16; l++){
                printf("VB[%d]=%d \n",l,neuralSenses[i].binaryCharacteristic[s*16+l]);
            }
        }
    }
}

void MainWindow::printVectorSenses(senses sense){
    std::cout<<"MainWindow::printVectorSenses"<<endl;

    for(register int s=0; s<*(neuralSenses[sense].ptr)+1; s++){
        printf("neurona %d \n",s);
        for(register int j=0; j<32; j++){
            printf("v[%d]=%d \n",j,neuralSenses[sense].vectorNeuron[s*32+j]);
        }
        for(register int k=0; k<9; k++){
            printf("f[%d]=%d \n",k,neuralSenses[sense].vectorFlags[s*9+k]);
        }
    }
}

void MainWindow::printIdsNeuronHit(senses sense){
    std::cout<<"MainWindow::printidsNeuronHit"<<endl;

    printSense(sense);
    printf("id hits se tienen %d hits\n",*interface[sense].hits);
    for(register int s=0; s<*interface[sense].hits; s++)
        printf("neurona %d \n",interface[sense].id[s]);
}

void MainWindow::printSense(senses sense){
    std::cout<<"MainWindow::printSense"<<endl;

    if(sense == HEARING)
        printf("Sentido OIDO \n");
    else
        printf("Sentido VISTA \n");
}

void MainWindow::printCategory(senses sense){
    std::cout<<"MainWindow::printCategory"<<endl;

    printSense(sense);
    printf("Categor&iacute;as se tienen %d Categor&iacute;as\n",*interface[sense].hits);
    for(register int s=0; s<*interface[sense].hits; s++)
        printf("Categor&iacute;as %d \n",interface[sense].arrayCategory[s]);
}

/*template <class T>
void MainWindow::freeGenericPtr(T *ptr){
    if(ptr!= NULL)
        delete ptr;
}*/

bool MainWindow::analyticsNeuron(){

    if(interface[HEARING].hits==0 || interface[SIGHT].hits==0){
        if(interface[HEARING].hits==0)
            paintBinaryCharacteristic(SIGHT,interface[SIGHT].id[0]);
        else
            paintBinaryCharacteristic(HEARING,interface[HEARING].id[0]);
        return false;
    }//  si hay hits en Oido y  Vista estan activa
    unsigned char sightCategory;
    unsigned char tempEarC;
    sightCategory = interface[SIGHT].arrayCategory[0];
    if(ambiguity(sightCategory))                                                       //  Existe la ambiguedad
        tempEarC = checkInRelationNet(); //donde llamar esto?
    return true;
}

bool MainWindow::ambiguity(unsigned char sightCat){

    for(int i=0; i<*interface[SIGHT].hits; i++){                          //  Si hay distintas Categor&iacute;as en el
        if(interface[SIGHT].arrayCategory[i] != sightCat)            //  vector de Categor&iacute;as de la vista entonces
            return true;
    }
}

void MainWindow::initializeCuturalNet(int numNeuron){                   //ESFERA CULTURAL
    std::cout<<"MainWindow::initializeCulturalNet"<<endl;

    addNet = new CulturalNet;
    addNet->data = new unsigned char[numNeuron];
    addNet->flipflopKnow = new unsigned char[numNeuron];
    addNet->valve = new unsigned char[numNeuron];
}

/*void MainWindow::freeCulturalNet(){
    std::cout<<"MainWindow::freeCulturalNet"<<endl;

    if(addNet != NULL){
        freeGenericPtr(addNet->data);
        freeGenericPtr(addNet->flipflopKnow);
        freeGenericPtr(addNet->valve);
        delete addNet;
    }
}*/

void MainWindow::buildRelation(unsigned char){
    std::cout<<"MainWindow::buildRelation"<<endl;

    relationSenses->vectorSight[relationSenses->sizeRelationNet] = interface[SIGHT].id[1];
    relationSenses->vectorEar[relationSenses->sizeRelationNet] = interface[HEARING].id[1];
    relationSenses->vectorWeight[relationSenses->sizeRelationNet] = 0;
    relationSenses->sizeRelationNet++;
}

void MainWindow::initializeRelation(int numNeuron){                        //  Reserva MEmoria para los vectores
    int tamVector;                                                         //  de la red de relaciones (ESFERA CULTURAL)
    tamVector = numNeuron *sizeof(unsigned char);                          //  Deberia ponerlos todos 0
    relationSenses->vectorEar = new unsigned char[tamVector];
    relationSenses->vectorSight = new unsigned char[tamVector];
    relationSenses->vectorWeight = new unsigned char[tamVector];
    relationSenses->sizeRelationNet = 0;

    std::cout<<"MainWindow::initializeRelation"<<endl;
}

unsigned char MainWindow::checkInRelationNet(){                                  //Chequea si existe una relacion
    std::cout<<"MainWindow::checkInRelationNet"<<endl;

    unsigned char *tempCatSight;
    unsigned char *tempCatEar;
    int *idRelationNet;
    tempCatSight = new unsigned char[*interface[SIGHT].hits];
    tempCatEar = new unsigned char[*interface[SIGHT].hits];
    idRelationNet = new int[*interface[SIGHT].hits];
    for(int i=0; i<*interface[SIGHT].hits; i++){
        tempCatSight[i] = NULL;
        tempCatEar[i] = NULL;
        idRelationNet[i] = -1;
    }
    bool ya = false;
    for(int j=0; j<*interface[SIGHT].hits; j++){
        for(int i=0; i<relationSenses->sizeRelationNet; i++){
            if(interface[SIGHT].arrayCategory[j] == relationSenses->vectorSight[i]){
                for(int k=0; k<j; k++){
                    if(tempCatSight[k] == relationSenses->vectorSight[i])
                        ya = true;
                    if(!ya){
                        tempCatSight[j] = relationSenses->vectorSight[i];
                        tempCatEar[j] = relationSenses->vectorEar[i];
                        idRelationNet[j] = i;
                        break;
                    }
                }
            }
        }
    }
    int idmax;
    int max = -1;
    for(int i=0; i<*interface[SIGHT].hits; i++){
        if(idRelationNet[i] >= 0)
            if(relationSenses->vectorWeight[idRelationNet[i]] > max){
                max = relationSenses->vectorWeight[idRelationNet[i]];
                idmax = idRelationNet[i];
            }
    }
    for(int i=0; i<*interface[SIGHT].hits; i++){
        actualiceCategory(relationSenses->vectorSight[idmax],relationSenses->vectorEar[idRelationNet[i]]);
        interface[SIGHT].arrayCategory[i] = relationSenses->vectorSight[idmax];                 //en la interfaz tmb?
        relationSenses->vectorWeight[idmax]++;
    }
}

void MainWindow::actualiceCategory(unsigned char earCat, unsigned char sightid){
    std::cout<<"MainWindow::actualizeCategory"<<endl;

    neuralSenses[SIGHT].vectorFlags[sightid * SIZE_FLAGS + CAT] = earCat;
}

void MainWindow::printCountNetwork(){
    std::cout<<"MainWindow::printCountNetwork"<<endl;

    for(register int i=0; i<kNeuron; i++){
        printf("Neurona: %d\n", countNetwork->vectorNetworkCount[i]);
    }
}

void MainWindow::startCount(){
    std::cout<<"MainWindow::startCount"<<endl;

    ui->textBrowser->show();
    ui->pushButton_stopCount->show();
    ui->pushButton_stopCount->show();
    ui->pushButton_teachBip->show();
    ui->pushButton_teachClack->show();
    ui->pushButton_teachClack->setEnabled(false);
}

void MainWindow::stopCount(){
    std::cout<<"MainWindow::stopCount"<<endl;

    ui->textBrowser->hide();
    ui->pushButton_stopCount->hide();
    ui->pushButton_stopCount->hide();
    ui->pushButton_teachBip->hide();
    ui->pushButton_teachClack->hide();
}

void MainWindow::countProtocol(){
    std::cout<<"MainWindow::countProtocol"<<endl;

    ui->pushButton_teachClack->setEnabled(true);
    if(countNetwork->vectorNetworkCount[kNeuron] == 1){
        ui->textBrowser->setText("KNOWN BIP");
        std::cout<<"Cantidad conocida"<<endl;
    }else{
        countNetwork->vectorNetworkCount[kNeuron] = 1;
        countNetwork->bipPointer[kNeuron] = kNeuron + 1;
        ui->textBrowser->setText("UNKNOWN BIP");
        std::cout<<"Cantidad Desconocida"<<endl;
    }
    kNeuron++;
}


////TRABAJO RICARDO BRUZUAL////////
void MainWindow::clearWord(){
    std::cout<<"MainWindow::clearWord"<<endl;

    ui->lineEditEarInput->clear();
    ui->lineEditEarInput->setEnabled(true);
    ui->pushButtonProcess->setEnabled(false);
    ui->pushButtonProcess->setStyleSheet("*{background-color: light gray}");
    ui->pushButtonBip->hide();
    ui->pushButtonBipSyl->hide();
    ui->pushButtonBipWord->hide();
    ui->pushButtonCheck->hide();
    file();
    word.clear();
    word2.clear();
    syllab.clear();
    nsyllab = 0;
    ptr1 = 999, ptr2 = 999, ptr3 = 999, ptr4 = 999, ptr5 = 999, ptr6 = 999, ptr7 = 999, ptr8 = 999;
    bum = 0;
    chemicalLayerEye->setIsEditable(false);
    ui->pushButtonBum->setEnabled(true);
    ui->pushButtonBum->setStyleSheet("*{background-color: rgb(96,96,96,120)}");
    ui->lineEditEarInput->setEnabled(false);
}

void MainWindow::clickBipSyl(){
    std::cout<<"MainWindow::clickBipSyl"<<endl;

    protocolCheck();
    if(word2 == "B" || word2 == "C" || word2 == "D" || word2 == "F" || word2 == "G" || word2 == "H" || word2 == "J" || word2 == "K" || word2 == "L" || word2 == "M" || word2 == "N" || word2 == "P" || word2 == "Q" ||
            word2 == "R" || word2 == "S" || word2 == "T" || word2 == "V" || word2 == "W" || word2 == "X" || word2 == "Z"){
    }else
        nsyllab = nsyllab + 1;
    getPtr();
}

void MainWindow::protocolCheck(){
    std::cout<<"MainWindow::protocolCheck"<<endl;

    int ifPlus = QString::compare(ui->lineEditEarInput->text(),"+",Qt::CaseInsensitive);
    int ifEqual = QString::compare(ui->lineEditEarInput->text(),"=",Qt::CaseInsensitive);
    QString a;
    if(ui->checkBox_leer->isChecked() || ui->checkBox_syl->isChecked()){
        if(ifPlus != 0 && ifEqual != 0)
            a = ui->lineEditEarInput->text();
        word = word + a;
        word2 = word2 + a;
        syllab = syllab + a;
    }
    std::cout<<word.toStdString()<<endl;
    if((ui->checkBox_leer->isChecked() || ui->checkBox_syl->isChecked()) && word.isEmpty())
        showWarning("Error","No se ha ingresado ningún valor.");
    if(syllab.size() == 1){
        if(syllab[0] == 'A' || syllab[0] == 'E' || syllab[0] == 'I' || syllab[0] == 'O' || syllab[0] == 'U' || syllab[0] == 'Y'){
            std::cout<<word.toStdString()<<" nsyllab "<<nsyllab<<endl;
            syllab.clear();
        }else{
            std::cout<<word.toStdString()<<" nsyllab "<<nsyllab<<endl;
            syllab.clear();
        }
    }
    if(syllab.size() == 2){
        if(syllab[0] == 'A' || syllab[0] == 'E' || syllab[0] == 'I' || syllab[0] == 'O' || syllab[0] == 'U'){
            if(syllab[1] == 'B' || syllab[1] == 'C' || syllab[1] == 'D' || syllab[1] == 'H' || syllab[1] == 'L' || syllab[1] == 'M' || syllab[1] == 'N' || syllab[1] == 'P' || syllab[1] == 'R' || syllab[1] == 'S' || syllab[1] == 'X' || syllab[1] == 'Z'){
                std::cout<<word.toStdString()<<" nsyllab "<<nsyllab<<endl;
                syllab.clear();
            }
        }
        if(syllab[0] == 'I' && syllab[1] == 'O'){
                std::cout<<word.toStdString()<<" nsyllab "<<nsyllab<<endl;
                syllab.clear();
            }
        if(syllab[0] == 'A' || syllab[0] == 'E' || syllab[0] == 'O'){
                if(syllab[1] == 'I'){
                    std::cout<<word.toStdString()<<" nsyllab "<<nsyllab<<endl;
                    syllab.clear();
                }
        }
        if(syllab[0] == 'A' || syllab[0] == 'E' || syllab[0] == 'I'){
                    if(syllab[1] == 'U'){
                        std::cout<<word.toStdString()<<" nsyllab "<<nsyllab<<endl;
                        syllab.clear();
                    }
                }
        if(syllab[0] == 'B' || syllab[0] == 'C' || syllab[0] == 'D' || syllab[0] == 'F' || syllab[0] == 'G' || syllab[0] == 'H' || syllab[0] == 'J' || syllab[0] == 'K' || syllab[0] == 'L' || syllab[0] == 'M' || syllab[0] == 'N' || syllab[0] == 'P' || syllab[0] == 'R' || syllab[0] == 'S' || syllab[0] == 'T' || syllab[0] == 'V' || syllab[0] == 'W' || syllab[0] == 'X' || syllab[0] == 'Y' || syllab[0] == 'Z'){
            if(syllab[1] == 'A' || syllab[1] == 'E' || syllab[1] == 'I' || syllab[1] == 'O' || syllab[1] == 'U'){
                std::cout<<word.toStdString()<<" nsyllab "<<nsyllab<<endl;
                syllab.clear();
            }
        }
    }
    if(syllab.size() == 3){
        if(syllab[0] == 'A'){
            if(syllab[1] == 'U'){
                if(syllab[2] == 'N'){
                    std::cout<<word.toStdString()<<" nsyllab "<<nsyllab<<endl;
                    syllab.clear();
                }
            }
        }
        if(syllab[0] == 'I'){
            if(syllab[1] == 'O'){
                if(syllab[2] == 'N'){
                    std::cout<<word.toStdString()<<" nsyllab "<<nsyllab<<endl;
                    syllab.clear();
                }
            }
        }
        if(syllab[0] == 'Q'){
            if(syllab[1] == 'U'){
                if(syllab[2] == 'A' || syllab[2] == 'E' || syllab[2] == 'I' || syllab[2] == 'O' || syllab[2] == 'U'){
                    std::cout<<word.toStdString()<<" nsyllab "<<nsyllab<<endl;
                    syllab.clear();
                }
            }
        }
        if(syllab[0] == 'C'){
            if(syllab[1] == 'H'){
                if(syllab[2] == 'A' || syllab[2] == 'E' || syllab[2] == 'I' || syllab[2] == 'O' || syllab[2] == 'U'){
                    std::cout<<word.toStdString()<<" nsyllab "<<nsyllab<<endl;
                    syllab.clear();
                }
            }
        }
        if(syllab[0] == 'L'){
            if(syllab[1] == 'L'){
                if(syllab[2] == 'A' || syllab[2] == 'E' || syllab[2] == 'I' || syllab[2] == 'O' || syllab[2] == 'U'){
                    std::cout<<word.toStdString()<<" nsyllab "<<nsyllab<<endl;
                    syllab.clear();
                }
            }
        }
        if(syllab[0] == 'R' || syllab[0] == 'B' || syllab[0] == 'T' || syllab[0] == 'D'){
            if(syllab[1] == 'R'){
                if(syllab[2] == 'A' || syllab[2] == 'E' || syllab[2] == 'I' || syllab[2] == 'O' || syllab[2] == 'U'){
                    std::cout<<word.toStdString()<<" nsyllab "<<nsyllab<<endl;
                    syllab.clear();
                }
            }
        }
        if(syllab[0] == 'B' || syllab[0] == 'C' || syllab[0] == 'D' || syllab[0] == 'F' || syllab[0] == 'G' || syllab[0] == 'H' || syllab[0] == 'J' || syllab[0] == 'K' || syllab[0] == 'L' || syllab[0] == 'M' || syllab[0] == 'N' || syllab[0] == 'P' || syllab[0] == 'R' || syllab[0] == 'S' || syllab[0] == 'T' || syllab[0] == 'V' || syllab[0] == 'W' || syllab[0] == 'X' || syllab[0] == 'Y' || syllab[0] == 'Z'){
            if(syllab[1] == 'U'){
                if(syllab[2] == 'A' || syllab[2] == 'E' || syllab[2] == 'I' || syllab[2] == 'O' || syllab[2] == 'L' || syllab[2] == 'N'){
                    std::cout<<word.toStdString()<<" nsyllab "<<nsyllab<<endl;
                    syllab.clear();
                }
            }
        }
        if(syllab[0] == 'B' || syllab[0] == 'C' || syllab[0] == 'D' || syllab[0] == 'F' || syllab[0] == 'G' || syllab[0] == 'H' || syllab[0] == 'J' || syllab[0] == 'K' || syllab[0] == 'L' || syllab[0] == 'M' || syllab[0] == 'N' || syllab[0] == 'P' || syllab[0] == 'R' || syllab[0] == 'S' || syllab[0] == 'T' || syllab[0] == 'V' || syllab[0] == 'W' || syllab[0] == 'X' || syllab[0] == 'Y' || syllab[0] == 'Z'){
            if(syllab[1] == 'O'){
                if(syllab[2] == 'L' || syllab[2] == 'S' || syllab[2] == 'R'){
                    std::cout<<word.toStdString()<<" nsyllab "<<nsyllab<<endl;
                    syllab.clear();
                }
            }
        }
    }
    if(syllab.size() == 4){
        if(syllab[0] == 'S'){
            if(syllab[1] == 'I'){
                if(syllab[2] == 'A' || syllab[2] == 'E' || syllab[2] == 'O' || syllab[2] == 'U'){
                    if(syllab[3] == 'N'){
                        std::cout<<word.toStdString()<<" nsyllab "<<nsyllab<<endl;
                        syllab.clear();
                    }
                }
            }
        }
        if(syllab[0] == 'C' || syllab[0] == 'V'){
            if(syllab[1] == 'I' || syllab[1] == 'U'){
                if(syllab[2] == 'A' || syllab[2] == 'E' || syllab[2] == 'O'){
                    if(syllab[3] == 'N' || syllab[3] == 'L'){
                        std::cout<<word.toStdString()<<" nsyllab "<<nsyllab<<endl;
                        syllab.clear();
                    }
                }
            }
        }
    }
}

QString syl;

void MainWindow::getPtr(){
    std::cout<<"MainWindow::getPtr"<<endl;

    if((word.size()>1 && word!="LL" && word!="CH" && word!="RR" && word!="BR" && word!="TR") || (word.size()==1 && (word=="A" || word=="E" || word=="I" || word=="O" || word=="U" || word=="Y"))){
        syl = word;
        if(ptr1 == 999){
            ptr1 = returnID(syl);
            word.clear();
        }else if(ptr1 != 999 && ptr2 == 999){
            ptr2 = returnID(syl);
            word.clear();
        }else if(ptr1 != 999 && ptr2 != 999 && ptr3 == 999){
            ptr3 = returnID(syl);
            word.clear();
        }else if(ptr1 != 999 && ptr2 != 999 && ptr3 != 999 && ptr4 == 999){
            ptr4 = returnID(syl);
            word.clear();
        }else if(ptr1 != 999 && ptr2 != 999 && ptr3 != 999 && ptr4 != 999 && ptr5 == 999){
            ptr5 = returnID(syl);
            word.clear();
        }else if(ptr1 != 999 && ptr2 != 999 && ptr3 != 999 && ptr4 != 999 && ptr5 != 999 && ptr6 == 999){
            ptr6 = returnID(syl);
            word.clear();
        }else if(ptr1 != 999 && ptr2 != 999 && ptr3 != 999 && ptr4 != 999 && ptr5 != 999 && ptr6 != 999 && ptr7 == 999){
            ptr7 = returnID(syl);
            word.clear();
        }else if(ptr1 != 999 && ptr2 != 999 && ptr3 != 999 && ptr4 != 999 && ptr5 != 999 && ptr6 != 999 && ptr7 != 999 && ptr8 == 999){
            ptr8 = returnID(syl);
            word.clear();
        }
    }
}

void MainWindow::paintBinaryNetWord(){
    std::cout<<"MainWindow::paintBinaryNetWord"<<endl;

    if(ptr1 != 999 || ptr2 != 999 || ptr3 != 999 || ptr4 != 999 || ptr5 != 999 || ptr6 != 999 || ptr7 != 999 || ptr8 != 999)
        paintBinaryWord(ptr1, ptr2, ptr3, ptr4, ptr5, ptr6, ptr7, ptr8);
}

void MainWindow::paintBinaryWord(int ptr1, int ptr2, int ptr3, int ptr4, int ptr5, int ptr6, int ptr7, int ptr8){ //maxima palabra en español = 8 silabas
    std::cout<<"MainWindow::paintBinaryWord"<<endl;

    unsigned short displacement = 8 * sizeof (unsigned short) -1;
    unsigned short mask = 1 << displacement;
    freeMemory->freeGenericPtr(image);
    //freeGenericPtr(image);
    if(ptr1 != 999 && ptr2 == 999 && ptr3 == 999 && ptr4 == 999 && ptr5 == 999 && ptr6 == 999 && ptr7 == 999 && ptr8 == 999)
        image = new QImage(QSize(300,240), QImage::Format_MonoLSB);
    else if(ptr1 != 999 && ptr2 != 999 && ptr3 == 999 && ptr4 == 999 && ptr5 == 999 && ptr6 == 999 && ptr7 == 999 && ptr8 == 999)
        image = new QImage(QSize(360,240), QImage::Format_MonoLSB);
    else if(ptr1 != 999 && ptr2 != 999 && ptr3 != 999 && ptr4 == 999 && ptr5 == 999 && ptr6 == 999 && ptr7 == 999 && ptr8 == 999)
        image = new QImage(QSize(540,240), QImage::Format_MonoLSB);
    else if(ptr1 != 999 && ptr2 != 999 && ptr3 != 999 && ptr4 != 999 && ptr5 == 999 && ptr6 == 999 && ptr7 == 999 && ptr8 == 999)
        image = new QImage(QSize(720,240), QImage::Format_MonoLSB);
    else if(ptr1 != 999 && ptr2 != 999 && ptr3 != 999 && ptr4 != 999 && ptr5 != 999 && ptr6 == 999 && ptr7 == 999 && ptr8 == 999)
        image = new QImage(QSize(900,240), QImage::Format_MonoLSB);
    else if(ptr1 != 999 && ptr2 != 999 && ptr3 != 999 && ptr4 != 999 && ptr5 != 999 && ptr6 != 999 && ptr7 == 999 && ptr8 == 999)
        image = new QImage(QSize(1080,240), QImage::Format_MonoLSB);
    else if(ptr1 != 999 && ptr2 != 999 && ptr3 != 999 && ptr4 != 999 && ptr5 != 999 && ptr6 != 999 && ptr7 != 999 && ptr8 == 999)
        image = new QImage(QSize(1260,240), QImage::Format_MonoLSB);
    else if(ptr1 != 999 && ptr2 != 999 && ptr3 != 999 && ptr4 != 999 && ptr5 != 999 && ptr6 != 999 && ptr7 != 999 && ptr8 != 999)
        image = new QImage(QSize(1450,240), QImage::Format_MonoLSB);
    image->fill(Qt::color1);
    QPainter paint;
    paint.begin(image);
    QPen pen(QColor(Qt::color1));
    paint.setPen(pen);
    paint.setBrush(QBrush(QColor(Qt::color1), Qt::SolidPattern));
    QString text = word2;
    paint.drawText(QRect(20,20,300,200),text);
    for(int i = 0; i < 16; i++){
        unsigned short value = neuralSenses[HEARING].binaryCharacteristic[ptr1 * 16 + i];
        for(unsigned short j = 0; j <= displacement; j++){
            if(value &mask)
                paint.drawRect(QRect((15-j)*10,50+i*10,8,8));//(x,y) x columnas y filas
            value <<= 1;
        }
    }
    for(int i = 0; i < 16; i++){
        unsigned short value = neuralSenses[HEARING].binaryCharacteristic[ptr2 * 16 + i];
        for(unsigned short j = 0; j <= displacement; j++){
            if(value &mask)
                paint.drawRect(QRect(180+(15-j)*10,50+i*10,8,8));//(x,y) x columnas y filas
            value <<= 1;
        }
    }
    for(int i = 0; i < 16; i++){
        unsigned short value = neuralSenses[HEARING].binaryCharacteristic[ptr3 * 16 + i];
        for(unsigned short j = 0; j <= displacement; j++){
            if(value &mask)
                paint.drawRect(QRect(360+(15-j)*10,50+i*10,8,8));//(x,y) x columnas y filas
            value <<= 1;
        }
    }
    for(int i = 0; i < 16; i++){
        unsigned short value = neuralSenses[HEARING].binaryCharacteristic[ptr4 * 16 + i];
        for(unsigned short j = 0; j <= displacement; j++){
            if(value &mask)
                paint.drawRect(QRect(540+(15-j)*10,50+i*10,8,8));//(x,y) x columnas y filas
            value <<= 1;
        }
    }
    for(int i = 0; i < 16; i++){
        unsigned short value = neuralSenses[HEARING].binaryCharacteristic[ptr5 * 16 + i];
        for(unsigned short j = 0; j <= displacement; j++){
            if(value &mask)
                paint.drawRect(QRect(720+(15-j)*10,50+i*10,8,8));//(x,y) x columnas y filas
            value <<= 1;
        }
    }
    for(int i = 0; i < 16; i++){
        unsigned short value = neuralSenses[HEARING].binaryCharacteristic[ptr6 * 16 + i];
        for(unsigned short j = 0; j <= displacement; j++){
            if(value &mask)
                paint.drawRect(QRect(900+(15-j)*10,50+i*10,8,8));//(x,y) x columnas y filas
            value <<= 1;
        }
    }
    for(int i = 0; i < 16; i++){
        unsigned short value = neuralSenses[HEARING].binaryCharacteristic[ptr7 * 16 + i];
        for(unsigned short j = 0; j <= displacement; j++){
            if(value &mask)
                paint.drawRect(QRect(1080+(15-j)*10,50+i*10,8,8));//(x,y) x columnas y filas
            value <<= 1;
        }
    }
    for(int i = 0; i < 16; i++){
        unsigned short value = neuralSenses[HEARING].binaryCharacteristic[ptr8 * 16 + i];
        for(unsigned short j = 0; j <= displacement; j++){
            if(value &mask)
                paint.drawRect(QRect(1260+(15-j)*10,50+i*10,8,8));//(x,y) x columnas y filas
            value <<= 1;
        }
    }
    paint.end();
    ViewFinder &view = ViewFinder::getInstance(this);
    view.showBinaryCharacteristic(image);
}

void MainWindow::paintNetNeuronWord(){
    std::cout<<"MainWindow::paintNeuronWord"<<endl;

    QString nameFile = "./Obj/net_hearing_syl.dot";
    generatePng(nameFile);
    ViewFinder &view = ViewFinder::getInstance(this);
    view.showNet();
}

void MainWindow::paintPng(){
    std::cout<<"MainWindow::paintPng"<<endl;

    int id = returnID(word2);
    int numNeuron = *(neuralSenses[SIGHT].ptr);
    for(int i=0; i<numNeuron;i++){
        if(neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[id]+i)*9+CAT] == (unsigned int)returnCategory(word2) && returnID(word2) != 999){
            generatePng("./Obj/net_hearing_syl.dot");
            ViewFinder &view = ViewFinder::getInstance(this);
            view.showNet();
        }else{
            generatePng("./Obj/net_hearing_syl.dot");
            ViewFinder &view = ViewFinder::getInstance(this);
            view.showNet();
        }
    }
}

void MainWindow::file(){
    std::cout<<"MainWindow::file"<<endl;

    ofstream file("./Obj/net_hearing_syl.dot");
    file<<"graph net_neuron{\n";
    file<<"rankdir=LR;\n";
    file.close();
}

void MainWindow::generateDotWord(QString nameFile, QString wo){
    std::cout<<"MainWindow::generateDotWord"<<endl;

    int id;
    int h = 0;
    cout<<word2.toStdString()<<endl;
    int numNeuron = *(neuralSenses[SIGHT].ptr);
    ofstream file(nameFile.toStdString().c_str(),ios::app);
    id = returnID(wo);
    switch(id){
    case 12:
        file<<"subgraph cluster_12{ ";
        for(int j=0; j<numNeuron; j++){
            if(neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[j]-1)*9+CAT] != (unsigned int)returnCategory(word2) && neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[j])*9+CAT] != (unsigned int)returnCategory(word2) && h != 0 && h != 3){
                h = 2;
                file<<"\"item"<<neuralSenses[SIGHT].ptr[j]+0<<"\" [label=  \"id Neurona: "<<neuralSenses[SIGHT].ptr[j]+0<<"\\nCategor&iacute;a: 'A'\"];\n";
            }
            if(neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[id]+j)*9+CAT] == (unsigned int)returnCategory(word2) && h != 2){
                h = 3;
                file<<"\"item"<<neuralSenses[SIGHT].ptr[id]+j<<"\" [label=  \"id Neurona: "<<neuralSenses[SIGHT].ptr[id]+j<<"\\nCategor&iacute;a: 'A'\"];\n";
            }
        }
        file<<"}\n";
        h = 0;
        break;
    case 16:
        file<<"subgraph cluster_16{ ";
        for(int j=0; j<numNeuron; j++){
            if(neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[j]-1)*9+CAT] != (unsigned int)returnCategory(word2) && neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[j])*9+CAT] != (unsigned int)returnCategory(word2) && h != 0 && h != 3){
                h = 2;
                file<<"\"item"<<neuralSenses[SIGHT].ptr[j]+0<<"\" [label=  \"id Neurona: "<<neuralSenses[SIGHT].ptr[j]+0<<"\\nCategor&iacute;a: 'E'\"];\n";
            }
            if(neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[id]+j)*9+CAT] == (unsigned int)returnCategory(word2) && h != 2){
                h = 3;
                file<<"\"item"<<neuralSenses[SIGHT].ptr[id]+j<<"\" [label=  \"id Neurona: "<<neuralSenses[SIGHT].ptr[id]+j<<"\\nCategor&iacute;a: 'E'\"];\n";
            }
        }
        file<<"}\n";
        h = 0;
        break;
    case 20:
        file<<"subgraph cluster_20{ ";
        for(int j=0; j<numNeuron; j++){
            if(neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[j]-1)*9+CAT] != (unsigned int)returnCategory(word2) && neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[j])*9+CAT] != (unsigned int)returnCategory(word2) && h != 0 && h != 3){
                h = 2;
                file<<"\"item"<<neuralSenses[SIGHT].ptr[j]+0<<"\" [label=  \"id Neurona: "<<neuralSenses[SIGHT].ptr[j]+0<<"\\nCategor&iacute;a: 'I'\"];\n";
            }
            if(neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[id]+j)*9+CAT] == (unsigned int)returnCategory(word2) && h != 2){
                h = 3;
                file<<"\"item"<<neuralSenses[SIGHT].ptr[id]+j<<"\" [label=  \"id Neurona: "<<neuralSenses[SIGHT].ptr[id]+j<<"\\nCategor&iacute;a: 'I'\"];\n";
            }
        }
        file<<"}\n";
        h = 0;
        break;
    case 26:
        file<<"subgraph cluster_26{ ";
        for(int j=0; j<numNeuron; j++){
            if(neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[j]-1)*9+CAT] != (unsigned int)returnCategory(word2) && neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[j])*9+CAT] != (unsigned int)returnCategory(word2) && h != 0 && h != 3){
                h = 2;
                file<<"\"item"<<neuralSenses[SIGHT].ptr[j]+0<<"\" [label=  \"id Neurona: "<<neuralSenses[SIGHT].ptr[j]+0<<"\\nCategor&iacute;a: 'O'\"];\n";
            }
            if(neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[id]+j)*9+CAT] == (unsigned int)returnCategory(word2) && h != 2){
                h = 3;
                file<<"\"item"<<neuralSenses[SIGHT].ptr[id]+j<<"\" [label=  \"id Neurona: "<<neuralSenses[SIGHT].ptr[id]+j<<"\\nCategor&iacute;a: 'O'\"];\n";
            }
        }
        file<<"}\n";
        h = 0;
        break;
    case 32:
        file<<"subgraph cluster_32{ ";
        for(int j=0; j<numNeuron; j++){
            if(neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[j]-1)*9+CAT] != (unsigned int)returnCategory(word2) && neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[j])*9+CAT] != (unsigned int)returnCategory(word2) && h != 0 && h != 3){
                h = 2;
                file<<"\"item"<<neuralSenses[SIGHT].ptr[j]+0<<"\" [label=  \"id Neurona: "<<neuralSenses[SIGHT].ptr[j]+0<<"\\nCategor&iacute;a: 'U'\"];\n";
            }
            if(neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[id]+j)*9+CAT] == (unsigned int)returnCategory(word2) && h != 2){
                h = 3;
                file<<"\"item"<<neuralSenses[SIGHT].ptr[id]+j<<"\" [label=  \"id Neurona: "<<neuralSenses[SIGHT].ptr[id]+j<<"\\nCategor&iacute;a: 'U'\"];\n";
            }
        }
        file<<"}\n";
        h = 0;
        break;
    case 36:
        file<<"subgraph cluster_36{ ";
        for(int j=0; j<numNeuron; j++){
            if(neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[j]-1)*9+CAT] != (unsigned int)returnCategory(word2) && neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[j])*9+CAT] != (unsigned int)returnCategory(word2) && h != 0 && h != 3){
                h = 2;
                file<<"\"item"<<neuralSenses[SIGHT].ptr[j]+0<<"\" [label=  \"id Neurona: "<<neuralSenses[SIGHT].ptr[j]+0<<"\\nCategor&iacute;a: 'Y'\"];\n";
            }
            if(neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[id]+j)*9+CAT] == (unsigned int)returnCategory(word2) && h != 2){
                h = 3;
                file<<"\"item"<<neuralSenses[SIGHT].ptr[id]+j<<"\" [label=  \"id Neurona: "<<neuralSenses[SIGHT].ptr[id]+j<<"\\nCategor&iacute;a: 'Y'\"];\n";
            }
        }
        file<<"}\n";
        h = 0;
        break;
    case 38:
        file<<"subgraph cluster_38{ ";
        file<<"\"item38A\" [label=  \"id Neurona: "<<returnID("A")<<"\\nCategor&iacute;a: 'A'\"];\n";
        file<<"\"item38B\" [label=  \"id Neurona: "<<returnID("B")<<"\\nCategor&iacute;a: 'B'\"];\n";
        for(int j=0; j<numNeuron; j++){
            if(neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[j]-1)*9+CAT] != (unsigned int)returnCategory(word2) && neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[j])*9+CAT] != (unsigned int)returnCategory(word2) && h != 0 && h != 3){
                h = 2;
                file<<"\"item"<<neuralSenses[SIGHT].ptr[j]+0<<"\" [label=  \"id Neurona: "<<neuralSenses[SIGHT].ptr[j]+0<<"\\nCategor&iacute;a: 'AB'\"];\n";
            }
            if(neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[id]+j)*9+CAT] == (unsigned int)returnCategory(word2) && h != 2){
                h = 3;
                file<<"\"item"<<neuralSenses[SIGHT].ptr[id]+j<<"\" [label=  \"id Neurona: "<<neuralSenses[SIGHT].ptr[id]+j<<"\\nCategor&iacute;a: 'AB'\"];\n";
            }
        }
        file<<"}\n";
        h = 0;
        break;
    case 39:
        file<<"subgraph cluster_39{ ";
        file<<"\"item39B\" [label=  \"id Neurona: "<<returnID("B")<<"\\nCategor&iacute;a: 'B'\"];\n";
        file<<"\"item39A\" [label=  \"id Neurona: "<<returnID("A")<<"\\nCategor&iacute;a: 'A'\"];\n";
        for(int j=0; j<numNeuron; j++){
            if(neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[j]-1)*9+CAT] != (unsigned int)returnCategory(word2) && neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[j])*9+CAT] != (unsigned int)returnCategory(word2) && h != 0 && h != 3){
                h = 2;
                file<<"\"item"<<neuralSenses[SIGHT].ptr[j]+0<<"\" [label=  \"id Neurona: "<<neuralSenses[SIGHT].ptr[j]+0<<"\\nCategor&iacute;a: 'BA'\"];\n";
            }
            if(neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[id]+j)*9+CAT] == (unsigned int)returnCategory(word2) && h != 2){
                h = 3;
                file<<"\"item"<<neuralSenses[SIGHT].ptr[id]+j<<"\" [label=  \"id Neurona: "<<neuralSenses[SIGHT].ptr[id]+j<<"\\nCategor&iacute;a: 'BA'\"];\n";
            }
        }
        file<<"}\n";
        h = 0;
        break;
    case 40:
        file<<"subgraph cluster_40{ ";
        file<<"\"item40C\" [label=  \"id Neurona: "<<returnID("C")<<"\\nCategor&iacute;a: 'C'\"];\n";
        file<<"\"item40A\" [label=  \"id Neurona: "<<returnID("A")<<"\\nCategor&iacute;a: 'A'\"];\n";
        for(int j=0; j<numNeuron; j++){
            if(neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[j]-1)*9+CAT] != (unsigned int)returnCategory(word2) && neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[j])*9+CAT] != (unsigned int)returnCategory(word2) && h != 3 && h != 0){
                h = 2;
                file<<"\"item"<<neuralSenses[SIGHT].ptr[j]+0<<"\" [label=  \"id Neurona: "<<neuralSenses[SIGHT].ptr[j]+0<<"\\nCategor&iacute;a: 'CA'\"];\n";
            }
            if(neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[id]+j)*9+CAT] == (unsigned int)returnCategory(word2) && h != 2){
                h = 3;
                file<<"\"item"<<neuralSenses[SIGHT].ptr[id]+j<<"\" [label=  \"id Neurona: "<<neuralSenses[SIGHT].ptr[id]+j<<"\\nCategor&iacute;a: 'CA'\"];\n";
            }
        }
        file<<"}\n";
        h = 0;
        break;
    case 41:
        file<<"subgraph cluster_41{ ";
        file<<"\"item41L\" [label=  \"id Neurona: "<<returnID("L")<<"\\nCategor&iacute;a: 'L'\"];\n";
        file<<"\"item41L\" [label=  \"id Neurona: "<<returnID("L")<<"\\nCategor&iacute;a: 'L'\"];\n";
        file<<"\"item41O\" [label=  \"id Neurona: "<<returnID("O")<<"\\nCategor&iacute;a: 'O'\"];\n";
        for(int j=0; j<numNeuron; j++){
            if(neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[j]-1)*9+CAT] != (unsigned int)returnCategory(word2) && neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[j])*9+CAT] != (unsigned int)returnCategory(word2) && h != 3 && h != 0){
                h = 2;
                file<<"\"item"<<neuralSenses[SIGHT].ptr[j]+0<<"\" [label=  \"id Neurona: "<<neuralSenses[SIGHT].ptr[j]+0<<"\\nCategor&iacute;a: 'LLO'\"];\n";
            }
            if(neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[id]+j)*9+CAT] == (unsigned int)returnCategory(word2) && h != 2){
                h = 3;
                file<<"\"item"<<neuralSenses[SIGHT].ptr[id]+j<<"\" [label=  \"id Neurona: "<<neuralSenses[SIGHT].ptr[id]+j<<"\\nCategor&iacute;a: 'LLO'\"];\n";
            }
        }
        file<<"}\n";
        h = 0;
        break;
    case 42:
        file<<"subgraph cluster_42{ ";
        file<<"\"item42D\" [label=  \"id Neurona: "<<returnID("D")<<"\\nCategor&iacute;a: 'D'\"];\n";
        file<<"\"item42A\" [label=  \"id Neurona: "<<returnID("A")<<"\\nCategor&iacute;a: 'A'\"];\n";
        for(int j=0; j<numNeuron; j++){
            if(neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[j]-1)*9+CAT] != (unsigned int)returnCategory(word2) && neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[j])*9+CAT] != (unsigned int)returnCategory(word2) && h != 3 && h != 0){
                h = 2;
                file<<"\"item"<<neuralSenses[SIGHT].ptr[j]+0<<"\" [label=  \"id Neurona: "<<neuralSenses[SIGHT].ptr[j]+0<<"\\nCategor&iacute;a: 'DA'\"];\n";
            }
            if(neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[id]+j)*9+CAT] == (unsigned int)returnCategory(word2) && h != 2){
                h = 3;
                file<<"\"item"<<neuralSenses[SIGHT].ptr[id]+j<<"\" [label=  \"id Neurona: "<<neuralSenses[SIGHT].ptr[id]+j<<"\\nCategor&iacute;a: 'DA'\"];\n";
            }
        }
        file<<"}\n";
        h = 0;
        break;
    case 43:
        file<<"subgraph cluster_43{ ";
        file<<"\"item43S\" [label=  \"id Neurona: "<<returnID("S")<<"\\nCategor&iacute;a: 'S'\"];\n";
        file<<"\"item43A\" [label=  \"id Neurona: "<<returnID("A")<<"\\nCategor&iacute;a: 'A'\"];\n";
        for(int j=0; j<numNeuron; j++){
            if(neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[j]-1)*9+CAT] != (unsigned int)returnCategory(word2) && neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[j])*9+CAT] != (unsigned int)returnCategory(word2) && h != 0 && h != 3){
                h = 2;
                file<<"\"item"<<neuralSenses[SIGHT].ptr[j]+0<<"\" [label=  \"id Neurona: "<<neuralSenses[SIGHT].ptr[j]+0<<"\\nCategor&iacute;a: 'SA'\"];\n";
            }
            if(neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[id]+j)*9+CAT] == (unsigned int)returnCategory(word2) && h != 2){
                h = 3;
                file<<"\"item"<<neuralSenses[SIGHT].ptr[id]+j<<"\" [label=  \"id Neurona: "<<neuralSenses[SIGHT].ptr[id]+j<<"\\nCategor&iacute;a: 'SA'\"];\n";
            }
        }
        file<<"}\n";
        h = 0;
        break;
    case 44:
        file<<"subgraph cluster_44{ ";
        file<<"\"item44R\" [label=  \"id Neurona: "<<returnID("R")<<"\\nCategor&iacute;a: 'R'\"];\n";
        file<<"\"item44A\" [label=  \"id Neurona: "<<returnID("A")<<"\\nCategor&iacute;a: 'A'\"];\n";
        for(int j=0; j<numNeuron; j++){
            if(neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[j]-1)*9+CAT] != (unsigned int)returnCategory(word2) && neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[j])*9+CAT] != (unsigned int)returnCategory(word2) && h != 0 && h != 3){
                h = 2;
                file<<"\"item"<<neuralSenses[SIGHT].ptr[j]+0<<"\" [label=  \"id Neurona: "<<neuralSenses[SIGHT].ptr[j]+0<<"\\nCategor&iacute;a: 'RA'\"];\n";
            }
            if(neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[id]+j)*9+CAT] == (unsigned int)returnCategory(word2) && h != 2){
                h = 3;
                file<<"\"item"<<neuralSenses[SIGHT].ptr[id]+j<<"\" [label=  \"id Neurona: "<<neuralSenses[SIGHT].ptr[id]+j<<"\\nCategor&iacute;a: 'RA'\"];\n";
            }
        }
        file<<"}\n";
        h = 0;
        break;
    case 45:
        file<<"subgraph cluster_45{ ";
        file<<"\"item45P\" [label=  \"id Neurona: "<<returnID("P")<<"\\nCategor&iacute;a: 'P'\"];\n";
        file<<"\"item45O\" [label=  \"id Neurona: "<<returnID("O")<<"\\nCategor&iacute;a: 'O'\"];\n";
        for(int j=0; j<numNeuron; j++){
            if(neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[j]-1)*9+CAT] != (unsigned int)returnCategory(word2) && neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[j])*9+CAT] != (unsigned int)returnCategory(word2) && h != 0 && h != 3){
                h = 2;
                file<<"\"item"<<neuralSenses[SIGHT].ptr[j]+0<<"\" [label=  \"id Neurona: "<<neuralSenses[SIGHT].ptr[j]+0<<"\\nCategor&iacute;a: 'PO'\"];\n";
            }
            if(neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[id]+j)*9+CAT] == (unsigned int)returnCategory(word2) && h != 2){
                h = 3;
                file<<"\"item"<<neuralSenses[SIGHT].ptr[id]+j<<"\" [label=  \"id Neurona: "<<neuralSenses[SIGHT].ptr[id]+j<<"\\nCategor&iacute;a: 'PO'\"];\n";
            }
        }
        file<<"}\n";
        h = 0;
        break;
    case 46:
        file<<"subgraph cluster_46{ ";
        file<<"\"item46C\" [label=  \"id Neurona: "<<returnID("C")<<"\\nCategor&iacute;a: 'C'\"];\n";
        file<<"\"item46E\" [label=  \"id Neurona: "<<returnID("E")<<"\\nCategor&iacute;a: 'E'\"];\n";
        for(int j=0; j<numNeuron; j++){
            if(neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[j]-1)*9+CAT] != (unsigned int)returnCategory(word2) && neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[j])*9+CAT] != (unsigned int)returnCategory(word2) && h != 0 && h != 3){
                h = 2;
                file<<"\"item"<<neuralSenses[SIGHT].ptr[j]+0<<"\" [label=  \"id Neurona: "<<neuralSenses[SIGHT].ptr[j]+0<<"\\nCategor&iacute;a: 'CE'\"];\n";
            }
            if(neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[id]+j)*9+CAT] == (unsigned int)returnCategory(word2) && h != 2){
                h = 3;
                file<<"\"item"<<neuralSenses[SIGHT].ptr[id]+j<<"\" [label=  \"id Neurona: "<<neuralSenses[SIGHT].ptr[id]+j<<"\\nCategor&iacute;a: 'CE'\"];\n";
            }
        }
        file<<"}\n";
        h = 0;
        break;
    case 47:
        file<<"subgraph cluster_47{ ";
        file<<"\"item47G\" [label= \"id Neurona: "<<returnID("G")<<"\\nCategor&iacute;a: 'G'\"];\n";
        file<<"\"item47A\" [label= \"id Neurona: "<<returnID("A")<<"\\nCategor&iacute;a: 'A'\"];\n";
        for(int j=0; j<numNeuron; j++){
            if(neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[j]-1)*9+CAT] != (unsigned int)returnCategory(word2) && neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[j])*9+CAT] != (unsigned int)returnCategory(word2) && h != 0 && h != 3){
                h = 2;
                file<<"\"item"<<neuralSenses[SIGHT].ptr[j]+0<<"\" [label=  \"id Neurona: "<<neuralSenses[SIGHT].ptr[j]+0<<"\\nCategor&iacute;a: 'GA'\"];\n";
            }
            if(neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[id]+j)*9+CAT] == (unsigned int)returnCategory(word2) && h != 2){
                h = 3;
                file<<"\"item"<<neuralSenses[SIGHT].ptr[id]+j<<"\" [label=  \"id Neurona: "<<neuralSenses[SIGHT].ptr[id]+j<<"\\nCategor&iacute;a: 'GA'\"];\n";
            }
        }
        file<<"}\n";
        h = 0;
        break;
    case 48:
        file<<"subgraph cluster_48{ ";
        file<<"\"item48T\" [label= \"id Neurona: "<<returnID("T")<<"\\nCategor&iacute;a: 'T'\"];\n";
        file<<"\"item48O\" [label= \"id Neurona: "<<returnID("O")<<"\\nCategor&iacute;a: 'O'\"];\n";
        for(int j=0; j<numNeuron; j++){
            if(neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[j]-1)*9+CAT] != (unsigned int)returnCategory(word2) && neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[j])*9+CAT] != (unsigned int)returnCategory(word2) && h != 0 && h != 3){
                h = 2;
                file<<"\"item"<<neuralSenses[SIGHT].ptr[j]+0<<"\" [label=  \"id Neurona: "<<neuralSenses[SIGHT].ptr[j]+0<<"\\nCategor&iacute;a: 'TO'\"];\n";
            }
            if(neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[id]+j)*9+CAT] == (unsigned int)returnCategory(word2) && h != 2){
                h = 3;
                file<<"\"item"<<neuralSenses[SIGHT].ptr[id]+j<<"\" [label=  \"id Neurona: "<<neuralSenses[SIGHT].ptr[id]+j<<"\\nCategor&iacute;a: 'TO'\"];\n";
            }
        }
        file<<"}\n";
        h = 0;
        break;
    case 49:
        file<<"subgraph cluster_49{ ";
        file<<"\"item49T\" [label= \"id Neurona: "<<returnID("T")<<"\\nCategor&iacute;a: 'T'\"];\n";
        file<<"\"item49A\" [label= \"id Neurona: "<<returnID("A")<<"\\nCategor&iacute;a: 'A'\"];\n";
        for(int j=0; j<numNeuron; j++){
            if(neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[j]-1)*9+CAT] != (unsigned int)returnCategory(word2) && neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[j])*9+CAT] != (unsigned int)returnCategory(word2) && h != 0 && h != 3){
                h = 2;
                file<<"\"item"<<neuralSenses[SIGHT].ptr[j]+0<<"\" [label=  \"id Neurona: "<<neuralSenses[SIGHT].ptr[j]+0<<"\\nCategor&iacute;a: 'TA'\"];\n";
            }
            if(neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[id]+j)*9+CAT] == (unsigned int)returnCategory(word2) && h != 2){
                h = 3;
                file<<"\"item"<<neuralSenses[SIGHT].ptr[id]+j<<"\" [label=  \"id Neurona: "<<neuralSenses[SIGHT].ptr[id]+j<<"\\nCategor&iacute;a: 'TA'\"];\n";
            }
        }
        file<<"}\n";
        h = 0;
        break;
    case 50:
        file<<"subgraph cluster_50{ ";
        file<<"\"item50P\" [label= \"id Neurona: "<<returnID("P")<<"\\nCategor&iacute;a: 'P'\"];\n";
        file<<"\"item50A\" [label= \"id Neurona: "<<returnID("A")<<"\\nCategor&iacute;a: 'A'\"];\n";
        for(int j=0; j<numNeuron; j++){
            if(neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[j]-1)*9+CAT] != (unsigned int)returnCategory(word2) && neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[j])*9+CAT] != (unsigned int)returnCategory(word2) && h != 0 && h != 3){
                h = 2;
                file<<"\"item"<<neuralSenses[SIGHT].ptr[j]+0<<"\" [label=  \"id Neurona: "<<neuralSenses[SIGHT].ptr[j]+0<<"\\nCategor&iacute;a: 'PA'\"];\n";
            }
            if(neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[id]+j)*9+CAT] == (unsigned int)returnCategory(word2) && h != 2){
                h = 3;
                file<<"\"item"<<neuralSenses[SIGHT].ptr[id]+j<<"\" [label=  \"id Neurona: "<<neuralSenses[SIGHT].ptr[id]+j<<"\\nCategor&iacute;a: 'PA'\"];\n";
            }
        }
        file<<"}\n";
        h = 0;
        break;
    case 51:
        file<<"subgraph cluster_51{ ";
        file<<"\"item51D\" [label= \"id Neurona: "<<returnID("D")<<"\\nCategor&iacute;a: 'D'\"];\n";
        file<<"\"item51I\" [label= \"id Neurona: "<<returnID("I")<<"\\nCategor&iacute;a: 'I'\"];\n";
        file<<"\"item51S\" [label= \"id Neurona: "<<returnID("S")<<"\\nCategor&iacute;a: 'S'\"];\n";
        for(int j=0; j<numNeuron; j++){
            if(neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[j]-1)*9+CAT] != (unsigned int)returnCategory(word2) && neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[j])*9+CAT] != (unsigned int)returnCategory(word2) && h != 0 && h != 3){
                h = 2;
                file<<"\"item"<<neuralSenses[SIGHT].ptr[j]+0<<"\" [label=  \"id Neurona: "<<neuralSenses[SIGHT].ptr[j]+0<<"\\nCategor&iacute;a: 'DIS'\"];\n";
            }
            if(neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[id]+j)*9+CAT] == (unsigned int)returnCategory(word2) && h != 2){
                h = 3;
                file<<"\"item"<<neuralSenses[SIGHT].ptr[id]+j<<"\" [label=  \"id Neurona: "<<neuralSenses[SIGHT].ptr[id]+j<<"\\nCategor&iacute;a: 'DIS'\"];\n";
            }
        }
        file<<"}\n";
        h = 0;
        break;
    case 52:
        file<<"subgraph cluster_52{ ";
        file<<"\"item52R\" [label= \"id Neurona: "<<returnID("R")<<"\\nCategor&iacute;a: 'R'\"];\n";
        file<<"\"item52R\" [label= \"id Neurona: "<<returnID("R")<<"\\nCategor&iacute;a: 'R'\"];\n";
        file<<"\"item52O\" [label= \"id Neurona: "<<returnID("O")<<"\\nCategor&iacute;a: 'O'\"];\n";
        for(int j=0; j<numNeuron; j++){
            if(neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[j]-1)*9+CAT] != (unsigned int)returnCategory(word2) && neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[j])*9+CAT] != (unsigned int)returnCategory(word2) && h != 0 && h != 3){
                h = 2;
                file<<"\"item"<<neuralSenses[SIGHT].ptr[j]+0<<"\" [label=  \"id Neurona: "<<neuralSenses[SIGHT].ptr[j]+0<<"\\nCategor&iacute;a: 'RRO'\"];\n";
            }
            if(neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[id]+j)*9+CAT] == (unsigned int)returnCategory(word2) && h != 2){
                h = 3;
                file<<"\"item"<<neuralSenses[SIGHT].ptr[id]+j<<"\" [label=  \"id Neurona: "<<neuralSenses[SIGHT].ptr[id]+j<<"\\nCategor&iacute;a: 'RRO'\"];\n";
            }
        }
        file<<"}\n";
        h = 0;
        break;
    case 53:
        file<<"subgraph cluster_53{ ";
        file<<"\"item53J\" [label= \"id Neurona: "<<returnID("J")<<"\\nCategor&iacute;a: 'J'\"];\n";
        file<<"\"item53O\" [label= \"id Neurona: "<<returnID("O")<<"\\nCategor&iacute;a: 'O'\"];\n";
        for(int j=0; j<numNeuron; j++){
            if(neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[j]-1)*9+CAT] != (unsigned int)returnCategory(word2) && neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[j])*9+CAT] != (unsigned int)returnCategory(word2) && h != 0 && h != 3){
                h = 2;
                file<<"\"item"<<neuralSenses[SIGHT].ptr[j]+0<<"\" [label=  \"id Neurona: "<<neuralSenses[SIGHT].ptr[j]+0<<"\\nCategor&iacute;a: 'JO'\"];\n";
            }
            if(neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[id]+j)*9+CAT] == (unsigned int)returnCategory(word2) && h != 2){
                h = 3;
                file<<"\"item"<<neuralSenses[SIGHT].ptr[id]+j<<"\" [label=  \"id Neurona: "<<neuralSenses[SIGHT].ptr[id]+j<<"\\nCategor&iacute;a: 'JO'\"];\n";
            }
        }
        file<<"}\n";
        h = 0;
        break;
    case 54:
        file<<"subgraph cluster_54{ ";
        file<<"\"item54A\" [label= \"id Neurona: "<<returnID("A")<<"\\nCategor&iacute;a: 'A'\"];\n";
        file<<"\"item54R\" [label= \"id Neurona: "<<returnID("R")<<"\\nCategor&iacute;a: 'R'\"];\n";
        for(int j=0; j<numNeuron; j++){
            if(neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[j]-1)*9+CAT] != (unsigned int)returnCategory(word2) && neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[j])*9+CAT] != (unsigned int)returnCategory(word2) && h != 0 && h != 3){
                h = 2;
                file<<"\"item"<<neuralSenses[SIGHT].ptr[j]+0<<"\" [label=  \"id Neurona: "<<neuralSenses[SIGHT].ptr[j]+0<<"\\nCategor&iacute;a: 'AR'\"];\n";
            }
            if(neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[id]+j)*9+CAT] == (unsigned int)returnCategory(word2) && h != 2){
                h = 3;
                file<<"\"item"<<neuralSenses[SIGHT].ptr[id]+j<<"\" [label=  \"id Neurona: "<<neuralSenses[SIGHT].ptr[id]+j<<"\\nCategor&iacute;a: 'AR'\"];\n";
            }
        }
        file<<"}\n";
        h = 0;
        break;
    case 55:
        file<<"subgraph cluster_55{ ";
        file<<"\"item55C\" [label= \"id Neurona: "<<returnID("C")<<"\\nCategor&iacute;a: 'C'\"];\n";
        file<<"\"item55O\" [label= \"id Neurona: "<<returnID("O")<<"\\nCategor&iacute;a: 'O'\"];\n";
        for(int j=0; j<numNeuron; j++){
            if(neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[j]-1)*9+CAT] != (unsigned int)returnCategory(word2) && neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[j])*9+CAT] != (unsigned int)returnCategory(word2) && h != 0 && h != 3){
                h = 2;
                file<<"\"item"<<neuralSenses[SIGHT].ptr[j]+0<<"\" [label=  \"id Neurona: "<<neuralSenses[SIGHT].ptr[j]+0<<"\\nCategor&iacute;a: 'CO'\"];\n";
            }
            if(neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[id]+j)*9+CAT] == (unsigned int)returnCategory(word2) && h != 2){
                h = 3;
                file<<"\"item"<<neuralSenses[SIGHT].ptr[id]+j<<"\" [label=  \"id Neurona: "<<neuralSenses[SIGHT].ptr[id]+j<<"\\nCategor&iacute;a: 'CO'\"];\n";
            }
        }
        file<<"}\n";
        h = 0;
        break;
    case 56:
        file<<"subgraph cluster_56{ ";
        file<<"\"item56B\" [label= \"id Neurona: "<<returnID("B")<<"\\nCategor&iacute;a: 'B'\"];\n";
        file<<"\"item56R\" [label= \"id Neurona: "<<returnID("R")<<"\\nCategor&iacute;a: 'R'\"];\n";
        file<<"\"item56A\" [label= \"id Neurona: "<<returnID("A")<<"\\nCategor&iacute;a: 'A'\"];\n";
        for(int j=0; j<numNeuron; j++){
            if(neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[j]-1)*9+CAT] != (unsigned int)returnCategory(word2) && neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[j])*9+CAT] != (unsigned int)returnCategory(word2) && h != 0 && h != 3){
                h = 2;
                file<<"\"item"<<neuralSenses[SIGHT].ptr[j]+0<<"\" [label=  \"id Neurona: "<<neuralSenses[SIGHT].ptr[j]+0<<"\\nCategor&iacute;a: 'BRA'\"];\n";
            }
            if(neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[id]+j)*9+CAT] == (unsigned int)returnCategory(word2) && h != 2){
                h = 3;
                file<<"\"item"<<neuralSenses[SIGHT].ptr[id]+j<<"\" [label=  \"id Neurona: "<<neuralSenses[SIGHT].ptr[id]+j<<"\\nCategor&iacute;a: 'BRA'\"];\n";
            }
        }
        file<<"}\n";
        h = 0;
        break;
    case 57:
        file<<"subgraph cluster_57{ ";
        file<<"\"item57L\" [label= \"id Neurona: "<<returnID("L")<<"\\nCategor&iacute;a: 'L'\"];\n";
        file<<"\"item57O\" [label= \"id Neurona: "<<returnID("O")<<"\\nCategor&iacute;a: 'O'\"];\n";
        for(int j=0; j<numNeuron; j++){
            if(neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[j]-1)*9+CAT] != (unsigned int)returnCategory(word2) && neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[j])*9+CAT] != (unsigned int)returnCategory(word2) && h != 0 && h != 3){
                h = 2;
                file<<"\"item"<<neuralSenses[SIGHT].ptr[j]+0<<"\" [label=  \"id Neurona: "<<neuralSenses[SIGHT].ptr[j]+0<<"\\nCategor&iacute;a: 'LO'\"];\n";
            }
            if(neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[id]+j)*9+CAT] == (unsigned int)returnCategory(word2) && h != 2){
                h = 3;
                file<<"\"item"<<neuralSenses[SIGHT].ptr[id]+j<<"\" [label=  \"id Neurona: "<<neuralSenses[SIGHT].ptr[id]+j<<"\\nCategor&iacute;a: 'LO'\"];\n";
            }
        }
        file<<"}\n";
        h = 0;
        break;
    case 58:
        file<<"subgraph cluster_58{ ";
        file<<"\"item58C\" [label= \"id Neurona: "<<returnID("C")<<"\\nCategor&iacute;a: 'C'\"];\n";
        file<<"\"item58U\" [label= \"id Neurona: "<<returnID("U")<<"\\nCategor&iacute;a: 'U'\"];\n";
        for(int j=0; j<numNeuron; j++){
            if(neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[j]-1)*9+CAT] != (unsigned int)returnCategory(word2) && neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[j])*9+CAT] != (unsigned int)returnCategory(word2) && h != 0 && h != 3){
                h = 2;
                file<<"\"item"<<neuralSenses[SIGHT].ptr[j]+0<<"\" [label=  \"id Neurona: "<<neuralSenses[SIGHT].ptr[j]+0<<"\\nCategor&iacute;a: 'CU'\"];\n";
            }
            if(neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[id]+j)*9+CAT] == (unsigned int)returnCategory(word2) && h != 2){
                h = 3;
                file<<"\"item"<<neuralSenses[SIGHT].ptr[id]+j<<"\" [label=  \"id Neurona: "<<neuralSenses[SIGHT].ptr[id]+j<<"\\nCategor&iacute;a: 'CU'\"];\n";
            }
        }
        file<<"}\n";
        h = 0;
        break;
    case 59:
        file<<"subgraph cluster_59{ ";
        file<<"\"item59L\" [label= \"id Neurona: "<<returnID("L")<<"\\nCategor&iacute;a: 'L'\"];\n";
        file<<"\"item59E\" [label= \"id Neurona: "<<returnID("E")<<"\\nCategor&iacute;a: 'E'\"];\n";
        for(int j=0; j<numNeuron; j++){
            if(neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[j]-1)*9+CAT] != (unsigned int)returnCategory(word2) && neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[j])*9+CAT] != (unsigned int)returnCategory(word2) && h != 0 && h != 3){
                h = 2;
                file<<"\"item"<<neuralSenses[SIGHT].ptr[j]+0<<"\" [label=  \"id Neurona: "<<neuralSenses[SIGHT].ptr[j]+0<<"\\nCategor&iacute;a: 'LE'\"];\n";
            }
            if(neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[id]+j)*9+CAT] == (unsigned int)returnCategory(word2) && h != 2){
                h = 3;
                file<<"\"item"<<neuralSenses[SIGHT].ptr[id]+j<<"\" [label=  \"id Neurona: "<<neuralSenses[SIGHT].ptr[id]+j<<"\\nCategor&iacute;a: 'LE'\"];\n";
            }
        }
        file<<"}\n";
        h = 0;
        break;
    case 60:
        file<<"subgraph cluster_60{ ";
        file<<"\"item60B\" [label= \"id Neurona: "<<returnID("B")<<"\\nCategor&iacute;a: 'B'\"];\n";
        file<<"\"item60O\" [label= \"id Neurona: "<<returnID("O")<<"\\nCategor&iacute;a: 'O'\"];\n";
        file<<"\"item60L\" [label= \"id Neurona: "<<returnID("L")<<"\\nCategor&iacute;a: 'L'\"];\n";
        for(int j=0; j<numNeuron; j++){
            if(neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[j]-1)*9+CAT] != (unsigned int)returnCategory(word2) && neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[j])*9+CAT] != (unsigned int)returnCategory(word2) && h != 0 && h != 3){
                h = 2;
                file<<"\"item"<<neuralSenses[SIGHT].ptr[j]+0<<"\" [label=  \"id Neurona: "<<neuralSenses[SIGHT].ptr[j]+0<<"\\nCategor&iacute;a: 'BOL'\"];\n";
            }
            if(neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[id]+j)*9+CAT] == (unsigned int)returnCategory(word2) && h != 2){
                h = 3;
                file<<"\"item"<<neuralSenses[SIGHT].ptr[id]+j<<"\" [label=  \"id Neurona: "<<neuralSenses[SIGHT].ptr[id]+j<<"\\nCategor&iacute;a: 'BOL'\"];\n";
            }
        }
        file<<"}\n";
        h = 0;
        break;
    case 61:
        file<<"subgraph cluster_61{ ";
        file<<"\"item61O\" [label= \"id Neurona: "<<returnID("O")<<"\\nCategor&iacute;a: 'O'\"];\n";
        file<<"\"item61N\" [label= \"id Neurona: "<<returnID("N")<<"\\nCategor&iacute;a: 'N'\"];\n";
        for(int j=0; j<numNeuron; j++){
            if(neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[j]-1)*9+CAT] != (unsigned int)returnCategory(word2) && neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[j])*9+CAT] != (unsigned int)returnCategory(word2) && h != 0 && h != 3){
                h = 2;
                file<<"\"item"<<neuralSenses[SIGHT].ptr[j]+0<<"\" [label=  \"id Neurona: "<<neuralSenses[SIGHT].ptr[j]+0<<"\\nCategor&iacute;a: 'ON'\"];\n";
            }
            if(neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[id]+j)*9+CAT] == (unsigned int)returnCategory(word2) && h != 2){
                h = 3;
                file<<"\"item"<<neuralSenses[SIGHT].ptr[id]+j<<"\" [label=  \"id Neurona: "<<neuralSenses[SIGHT].ptr[id]+j<<"\\nCategor&iacute;a: 'ON'\"];\n";
            }
        }
        file<<"}\n";
        h = 0;
        break;
    case 62:
        file<<"subgraph cluster_62{ ";
        file<<"\"item62G\" [label= \"id Neurona: "<<returnID("G")<<"\\nCategor&iacute;a: 'G'\"];\n";
        file<<"\"item62O\" [label= \"id Neurona: "<<returnID("O")<<"\\nCategor&iacute;a: 'O'\"];\n";
        for(int j=0; j<numNeuron; j++){
            if(neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[j]-1)*9+CAT] != (unsigned int)returnCategory(word2) && neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[j])*9+CAT] != (unsigned int)returnCategory(word2) && h != 0 && h != 3){
                h = 2;
                file<<"\"item"<<neuralSenses[SIGHT].ptr[j]+0<<"\" [label=  \"id Neurona: "<<neuralSenses[SIGHT].ptr[j]+0<<"\\nCategor&iacute;a: 'GO'\"];\n";
            }
            if(neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[id]+j)*9+CAT] == (unsigned int)returnCategory(word2) && h != 2){
                h = 3;
                file<<"\"item"<<neuralSenses[SIGHT].ptr[id]+j<<"\" [label=  \"id Neurona: "<<neuralSenses[SIGHT].ptr[id]+j<<"\\nCategor&iacute;a: 'GO'\"];\n";
            }
        }
        file<<"}\n";
        h = 0;
        break;
    case 63:
        file<<"subgraph cluster_63{ ";
        file<<"\"item63V\" [label= \"id Neurona: "<<returnID("V")<<"\\nCategor&iacute;a: 'V'\"];\n";
        file<<"\"item63E\" [label= \"id Neurona: "<<returnID("E")<<"\\nCategor&iacute;a: 'E'\"];\n";
        for(int j=0; j<numNeuron; j++){
            if(neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[j]-1)*9+CAT] != (unsigned int)returnCategory(word2) && neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[j])*9+CAT] != (unsigned int)returnCategory(word2) && h != 0 && h != 3){
                h = 2;
                file<<"\"item"<<neuralSenses[SIGHT].ptr[j]+0<<"\" [label=  \"id Neurona: "<<neuralSenses[SIGHT].ptr[j]+0<<"\\nCategor&iacute;a: 'VE'\"];\n";
            }
            if(neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[id]+j)*9+CAT] == (unsigned int)returnCategory(word2) && h != 2){
                h = 3;
                file<<"\"item"<<neuralSenses[SIGHT].ptr[id]+j<<"\" [label=  \"id Neurona: "<<neuralSenses[SIGHT].ptr[id]+j<<"\\nCategor&iacute;a: 'VE'\"];\n";
            }
        }
        file<<"}\n";
        h = 0;
        break;
    case 64:
        file<<"subgraph cluster_64{ ";
        file<<"\"item64N\" [label= \"id Neurona: "<<returnID("N")<<"\\nCategor&iacute;a: 'N'\"];\n";
        file<<"\"item64A\" [label= \"id Neurona: "<<returnID("A")<<"\\nCategor&iacute;a: 'A'\"];\n";
        for(int j=0; j<numNeuron; j++){
            if(neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[j]-1)*9+CAT] != (unsigned int)returnCategory(word2) && neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[j])*9+CAT] != (unsigned int)returnCategory(word2) && h != 0 && h != 3){
                h = 2;
                file<<"\"item"<<neuralSenses[SIGHT].ptr[j]+0<<"\" [label=  \"id Neurona: "<<neuralSenses[SIGHT].ptr[j]+0<<"\\nCategor&iacute;a: 'NA'\"];\n";
            }
            if(neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[id]+j)*9+CAT] == (unsigned int)returnCategory(word2) && h != 2){
                h = 3;
                file<<"\"item"<<neuralSenses[SIGHT].ptr[id]+j<<"\" [label=  \"id Neurona: "<<neuralSenses[SIGHT].ptr[id]+j<<"\\nCategor&iacute;a: 'NA'\"];\n";
            }
        }
        file<<"}\n";
        h = 0;
        break;
    case 65:
        file<<"subgraph cluster_65{ ";
        file<<"\"item65D\" [label= \"id Neurona: "<<returnID("D")<<"\\nCategor&iacute;a: 'D'\"];\n";
        file<<"\"item65O\" [label= \"id Neurona: "<<returnID("O")<<"\\nCategor&iacute;a: 'O'\"];\n";
        for(int j=0; j<numNeuron; j++){
            if(neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[j]-1)*9+CAT] != (unsigned int)returnCategory(word2) && neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[j])*9+CAT] != (unsigned int)returnCategory(word2) && h != 0 && h != 3){
                h = 2;
                file<<"\"item"<<neuralSenses[SIGHT].ptr[j]+0<<"\" [label=  \"id Neurona: "<<neuralSenses[SIGHT].ptr[j]+0<<"\\nCategor&iacute;a: 'DO'\"];\n";
            }
            if(neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[id]+j)*9+CAT] == (unsigned int)returnCategory(word2) && h != 2){
                h = 3;
                file<<"\"item"<<neuralSenses[SIGHT].ptr[id]+j<<"\" [label=  \"id Neurona: "<<neuralSenses[SIGHT].ptr[id]+j<<"\\nCategor&iacute;a: 'DO'\"];\n";
            }
        }
        file<<"}\n";
        h = 0;
        break;
    case 66:
        file<<"subgraph cluster_66{ ";
        file<<"\"item66R\" [label= \"id Neurona: "<<returnID("R")<<"\\nCategor&iacute;a: 'R'\"];\n";
        file<<"\"item66R\" [label= \"id Neurona: "<<returnID("R")<<"\\nCategor&iacute;a: 'R'\"];\n";
        file<<"\"item66A\" [label= \"id Neurona: "<<returnID("A")<<"\\nCategor&iacute;a: 'A'\"];\n";
        for(int j=0; j<numNeuron; j++){
            if(neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[j]-1)*9+CAT] != (unsigned int)returnCategory(word2) && neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[j])*9+CAT] != (unsigned int)returnCategory(word2) && h != 0 && h != 3){
                h = 2;
                file<<"\"item"<<neuralSenses[SIGHT].ptr[j]+0<<"\" [label=  \"id Neurona: "<<neuralSenses[SIGHT].ptr[j]+0<<"\\nCategor&iacute;a: 'RRA'\"];\n";
            }
            if(neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[id]+j)*9+CAT] == (unsigned int)returnCategory(word2) && h != 2){
                h = 3;
                file<<"\"item"<<neuralSenses[SIGHT].ptr[id]+j<<"\" [label=  \"id Neurona: "<<neuralSenses[SIGHT].ptr[id]+j<<"\\nCategor&iacute;a: 'RRA'\"];\n";
            }
        }
        file<<"}\n";
        h = 0;
        break;
    case 67:
        file<<"subgraph cluster_67{ ";
        file<<"\"item67V\" [label= \"id Neurona: "<<returnID("V")<<"\\nCategor&iacute;a: 'V'\"];\n";
        file<<"\"item67I\" [label= \"id Neurona: "<<returnID("I")<<"\\nCategor&iacute;a: 'I'\"];\n";
        file<<"\"item67O\" [label= \"id Neurona: "<<returnID("O")<<"\\nCategor&iacute;a: 'O'\"];\n";
        file<<"\"item67N\" [label= \"id Neurona: "<<returnID("N")<<"\\nCategor&iacute;a: 'N'\"];\n";
        for(int j=0; j<numNeuron; j++){
            if(neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[j]-1)*9+CAT] != (unsigned int)returnCategory(word2) && neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[j])*9+CAT] != (unsigned int)returnCategory(word2) && h != 0 && h != 3){
                h = 2;
                file<<"\"item"<<neuralSenses[SIGHT].ptr[j]+0<<"\" [label=  \"id Neurona: "<<neuralSenses[SIGHT].ptr[j]+0<<"\\nCategor&iacute;a: 'VION'\"];\n";
            }
            if(neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[id]+j)*9+CAT] == (unsigned int)returnCategory(word2) && h != 2){
                h = 3;
                file<<"\"item"<<neuralSenses[SIGHT].ptr[id]+j<<"\" [label=  \"id Neurona: "<<neuralSenses[SIGHT].ptr[id]+j<<"\\nCategor&iacute;a: 'VION'\"];\n";
            }
        }
        file<<"}\n";
        h = 0;
        break;
    case 68:
        file<<"subgraph cluster_68{ ";
        file<<"\"item68D\" [label= \"id Neurona: "<<returnID("D")<<"\\nCategor&iacute;a: 'D'\"];\n";
        file<<"\"item68R\" [label= \"id Neurona: "<<returnID("R")<<"\\nCategor&iacute;a: 'R'\"];\n";
        file<<"\"item68I\" [label= \"id Neurona: "<<returnID("I")<<"\\nCategor&iacute;a: 'I'\"];\n";
        for(int j=0; j<numNeuron; j++){
            if(neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[j]-1)*9+CAT] != (unsigned int)returnCategory(word2) && neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[j])*9+CAT] != (unsigned int)returnCategory(word2) && h != 0 && h != 3){
                h = 2;
                file<<"\"item"<<neuralSenses[SIGHT].ptr[j]+0<<"\" [label=  \"id Neurona: "<<neuralSenses[SIGHT].ptr[j]+0<<"\\nCategor&iacute;a: 'DRI'\"];\n";
            }
            if(neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[id]+j)*9+CAT] == (unsigned int)returnCategory(word2) && h != 2){
                h = 3;
                file<<"\"item"<<neuralSenses[SIGHT].ptr[id]+j<<"\" [label=  \"id Neurona: "<<neuralSenses[SIGHT].ptr[id]+j<<"\\nCategor&iacute;a: 'DRI'\"];\n";
            }
        }
        file<<"}\n";
        h = 0;
        break;
    case 69:
        file<<"subgraph cluster_69{ ";
        file<<"\"item69CO\" [label= \"id Neurona: "<<returnID("CO")<<"\\nCategor&iacute;a: 'CO'\"];\n";
        file<<"\"item69CO\" [label= \"id Neurona: "<<returnID("CO")<<"\\nCategor&iacute;a: 'CO'\"];\n";
        for(int j=0; j<numNeuron; j++){
            if(neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[j]-1)*9+CAT] != (unsigned int)returnCategory(word2) && neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[j])*9+CAT] != (unsigned int)returnCategory(word2) && h != 0 && h != 3){
                h = 2;
                file<<"\"item"<<neuralSenses[SIGHT].ptr[j]+0<<"\" [label=  \"id Neurona: "<<neuralSenses[SIGHT].ptr[j]+0<<"\\nCategor&iacute;a: 'COCO'\"];\n";
            }
            if(neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[id]+j)*9+CAT] == (unsigned int)returnCategory(word2) && h != 2){
                h = 3;
                file<<"\"item"<<neuralSenses[SIGHT].ptr[id]+j<<"\" [label=  \"id Neurona: "<<neuralSenses[SIGHT].ptr[id]+j<<"\\nCategor&iacute;a: 'COCO'\"];\n";
            }
        }
        file<<"}\n";
        h = 0;
        break;
    case 70:
        file<<"subgraph cluster_70{ ";
        file<<"\"item70CA\" [label= \"id Neurona: "<<returnID("CA")<<"\\nCategor&iacute;a: 'CA'\"];\n";
        file<<"\"item70BA\" [label= \"id Neurona: "<<returnID("BA")<<"\\nCategor&iacute;a: 'BA'\"];\n";
        file<<"\"item70LLO\" [label= \"id Neurona: "<<returnID("LLO")<<"\\nCategor&iacute;a: 'LLO'\"];\n";
        for(int j=0; j<numNeuron; j++){
            if(neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[j]-1)*9+CAT] != (unsigned int)returnCategory(word2) && neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[j])*9+CAT] != (unsigned int)returnCategory(word2) && h != 3 && h != 0){
                h = 2;
                file<<"\"item"<<neuralSenses[SIGHT].ptr[j]+0<<"\" [label=  \"id Neurona: "<<neuralSenses[SIGHT].ptr[j]+0<<"\\nCategor&iacute;a: 'CABALLO'\"];\n";
            }
            if(neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[id]+j)*9+CAT] == (unsigned int)returnCategory(word2) && h != 2){
                h = 3;
                file<<"\"item"<<neuralSenses[SIGHT].ptr[id]+j<<"\" [label=  \"id Neurona: "<<neuralSenses[SIGHT].ptr[id]+j<<"\\nCategor&iacute;a: 'CABALLO'\"];\n";
            }
        }
        file<<"}\n";
        h = 0;
        break;
    case 71:
        file<<"subgraph cluster_71{ ";
        file<<"\"item71CA\" [label= \"id Neurona: "<<returnID("CA")<<"\\nCategor&iacute;a: 'CA'\"];\n";
        file<<"\"item71SA\" [label= \"id Neurona: "<<returnID("SA")<<"\\nCategor&iacute;a: 'SA'\"];\n";
        for(int j=0; j<numNeuron; j++){
            if(neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[j]-1)*9+CAT] != (unsigned int)returnCategory(word2) && neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[j])*9+CAT] != (unsigned int)returnCategory(word2) && h != 0 && h != 3){
                h = 2;
                file<<"\"item"<<neuralSenses[SIGHT].ptr[j]+0<<"\" [label=  \"id Neurona: "<<neuralSenses[SIGHT].ptr[j]+0<<"\\nCategor&iacute;a: 'CASA'\"];\n";
            }
            if(neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[id]+j)*9+CAT] == (unsigned int)returnCategory(word2) && h != 2){
                h = 3;
                file<<"\"item"<<neuralSenses[SIGHT].ptr[id]+j<<"\" [label=  \"id Neurona: "<<neuralSenses[SIGHT].ptr[id]+j<<"\\nCategor&iacute;a: 'CASA'\"];\n";
            }
        }
        file<<"}\n";
        h = 0;
        break;
    case 72:
        file<<"subgraph cluster_72{ ";
        file<<"\"item72SA\" [label= \"id Neurona: "<<returnID("SA")<<"\\nCategor&iacute;a: 'SA'\"];\n";
        file<<"\"item72PO\" [label= \"id Neurona: "<<returnID("PO")<<"\\nCategor&iacute;a: 'PO'\"];\n";
        for(int j=0; j<numNeuron; j++){
            if(neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[j]-1)*9+CAT] != (unsigned int)returnCategory(word2) && neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[j])*9+CAT] != (unsigned int)returnCategory(word2) && h != 0 && h != 3){
                h = 2;
                file<<"\"item"<<neuralSenses[SIGHT].ptr[j]+0<<"\" [label=  \"id Neurona: "<<neuralSenses[SIGHT].ptr[j]+0<<"\\nCategor&iacute;a: 'SAPO'\"];\n";
            }
            if(neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[id]+j)*9+CAT] == (unsigned int)returnCategory(word2) && h != 2){
                h = 3;
                file<<"\"item"<<neuralSenses[SIGHT].ptr[id]+j<<"\" [label=  \"id Neurona: "<<neuralSenses[SIGHT].ptr[id]+j<<"\\nCategor&iacute;a: 'SAPO'\"];\n";
            }
        }
        file<<"}\n";
        h = 0;
        break;
    case 73:
        file<<"subgraph cluster_73{ ";
        file<<"\"item73GA\" [label= \"id Neurona: "<<returnID("GA")<<"\\nCategor&iacute;a: 'GA'\"];\n";
        file<<"\"item73TO\" [label= \"id Neurona: "<<returnID("TO")<<"\\nCategor&iacute;a: 'TO'\"];\n";
        for(int j=0; j<numNeuron; j++){
            if(neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[j]-1)*9+CAT] != (unsigned int)returnCategory(word2) && neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[j])*9+CAT] != (unsigned int)returnCategory(word2) && h != 0 && h != 3){
                h = 2;
                file<<"\"item"<<neuralSenses[SIGHT].ptr[j]+0<<"\" [label=  \"id Neurona: "<<neuralSenses[SIGHT].ptr[j]+0<<"\\nCategor&iacute;a: 'GATO'\"];\n";
            }
            if(neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[id]+j)*9+CAT] == (unsigned int)returnCategory(word2) && h != 2){
                h = 3;
                file<<"\"item"<<neuralSenses[SIGHT].ptr[id]+j<<"\" [label=  \"id Neurona: "<<neuralSenses[SIGHT].ptr[id]+j<<"\\nCategor&iacute;a: 'GATO'\"];\n";
            }
        }
        file<<"}\n";
        h = 0;
        break;
    case 74:
        file<<"subgraph cluster_74{ ";
        file<<"\"item74PA\" [label= \"id Neurona: "<<returnID("PA")<<"\\nCategor&iacute;a: 'PA'\"];\n";
        file<<"\"item74TO\" [label= \"id Neurona: "<<returnID("TO")<<"\\nCategor&iacute;a: 'TO'\"];\n";
        for(int j=0; j<numNeuron; j++){
            if(neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[j]-1)*9+CAT] != (unsigned int)returnCategory(word2) && neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[j])*9+CAT] != (unsigned int)returnCategory(word2) && h != 0 && h != 3){
                h = 2;
                file<<"\"item"<<neuralSenses[SIGHT].ptr[j]+0<<"\" [label=  \"id Neurona: "<<neuralSenses[SIGHT].ptr[j]+0<<"\\nCategor&iacute;a: 'PATO'\"];\n";
            }
            if(neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[id]+j)*9+CAT] == (unsigned int)returnCategory(word2) && h != 2){
                h = 3;
                file<<"\"item"<<neuralSenses[SIGHT].ptr[id]+j<<"\" [label=  \"id Neurona: "<<neuralSenses[SIGHT].ptr[id]+j<<"\\nCategor&iacute;a: 'PATO'\"];\n";
            }
        }
        file<<"}\n";
        h = 0;
        break;
    case 75:
        file<<"subgraph cluster_75{ ";
        file<<"\"item75CA\" [label= \"id Neurona: "<<returnID("CA")<<"\\nCategor&iacute;a: 'CA'\"];\n";
        file<<"\"item75RRO\" [label= \"id Neurona: "<<returnID("RRO")<<"\\nCategor&iacute;a: 'RRO'\"];\n";
        for(int j=0; j<numNeuron; j++){
            if(neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[j]-1)*9+CAT] != (unsigned int)returnCategory(word2) && neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[j])*9+CAT] != (unsigned int)returnCategory(word2) && h != 0 && h != 3){
                h = 2;
                file<<"\"item"<<neuralSenses[SIGHT].ptr[j]+0<<"\" [label=  \"id Neurona: "<<neuralSenses[SIGHT].ptr[j]+0<<"\\nCategor&iacute;a: 'CARRO'\"];\n";
            }
            if(neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[id]+j)*9+CAT] == (unsigned int)returnCategory(word2) && h != 2){
                h = 3;
                file<<"\"item"<<neuralSenses[SIGHT].ptr[id]+j<<"\" [label=  \"id Neurona: "<<neuralSenses[SIGHT].ptr[id]+j<<"\\nCategor&iacute;a: 'CARRO'\"];\n";
            }
        }
        file<<"}\n";
        h = 0;
        break;
    case 76:
        file<<"subgraph cluster_76{ ";
        file<<"\"item76GA\" [label= \"id Neurona: "<<returnID("GA")<<"\\nCategor&iacute;a: 'GA'\"];\n";
        file<<"\"item76LLO\" [label= \"id Neurona: "<<returnID("LLO")<<"\\nCategor&iacute;a: 'LLO'\"];\n";
        for(int j=0; j<numNeuron; j++){
            if(neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[j]-1)*9+CAT] != (unsigned int)returnCategory(word2) && neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[j])*9+CAT] != (unsigned int)returnCategory(word2) && h != 0 && h != 3){
                h = 2;
                file<<"\"item"<<neuralSenses[SIGHT].ptr[j]+0<<"\" [label=  \"id Neurona: "<<neuralSenses[SIGHT].ptr[j]+0<<"\\nCategor&iacute;a: 'GALLO'\"];\n";
            }
            if(neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[id]+j)*9+CAT] == (unsigned int)returnCategory(word2) && h != 2){
                h = 3;
                file<<"\"item"<<neuralSenses[SIGHT].ptr[id]+j<<"\" [label=  \"id Neurona: "<<neuralSenses[SIGHT].ptr[id]+j<<"\\nCategor&iacute;a: 'GALLO'\"];\n";
            }
        }
        file<<"}\n";
        h = 0;
        break;
    case 77:
        file<<"subgraph cluster_77{ ";
        file<<"\"item77CU\" [label= \"id Neurona: "<<returnID("CU")<<"\\nCategor&iacute;a: 'CU'\"];\n";
        file<<"\"item77LE\" [label= \"id Neurona: "<<returnID("LE")<<"\\nCategor&iacute;a: 'LE'\"];\n";
        file<<"\"item77BRA\" [label= \"id Neurona: "<<returnID("BRA")<<"\\nCategor&iacute;a: 'BRA'\"];\n";
        for(int j=0; j<numNeuron; j++){
            if(neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[j]-1)*9+CAT] != (unsigned int)returnCategory(word2) && neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[j])*9+CAT] != (unsigned int)returnCategory(word2) && h != 0 && h != 3){
                h = 2;
                file<<"\"item"<<neuralSenses[SIGHT].ptr[j]+0<<"\" [label=  \"id Neurona: "<<neuralSenses[SIGHT].ptr[j]+0<<"\\nCategor&iacute;a: 'CULEBRA'\"];\n";
            }
            if(neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[id]+j)*9+CAT] == (unsigned int)returnCategory(word2) && h != 2){
                h = 3;
                file<<"\"item"<<neuralSenses[SIGHT].ptr[id]+j<<"\" [label=  \"id Neurona: "<<neuralSenses[SIGHT].ptr[id]+j<<"\\nCategor&iacute;a: 'CULEBRA'\"];\n";
            }
        }
        file<<"}\n";
        h = 0;
        break;
    case 78:
        file<<"subgraph cluster_78{ ";
        file<<"\"item78LE\" [label= \"id Neurona: "<<returnID("LE")<<"\\nCategor&iacute;a: 'LE'\"];\n";
        file<<"\"item78ON\" [label= \"id Neurona: "<<returnID("ON")<<"\\nCategor&iacute;a: 'ON'\"];\n";
        for(int j=0; j<numNeuron; j++){
            if(neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[j]-1)*9+CAT] != (unsigned int)returnCategory(word2) && neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[j])*9+CAT] != (unsigned int)returnCategory(word2) && h != 0 && h != 3){
                h = 2;
                file<<"\"item"<<neuralSenses[SIGHT].ptr[j]+0<<"\" [label=  \"id Neurona: "<<neuralSenses[SIGHT].ptr[j]+0<<"\\nCategor&iacute;a: 'LEON'\"];\n";
            }
            if(neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[id]+j)*9+CAT] == (unsigned int)returnCategory(word2) && h != 2){
                h = 3;
                file<<"\"item"<<neuralSenses[SIGHT].ptr[id]+j<<"\" [label=  \"id Neurona: "<<neuralSenses[SIGHT].ptr[id]+j<<"\\nCategor&iacute;a: 'LEON'\"];\n";
            }
        }
        file<<"}\n";
        h = 0;
        break;
    case 79:
        file<<"subgraph cluster_79{ ";
        file<<"\"item79VE\" [label= \"id Neurona: "<<returnID("VE")<<"\\nCategor&iacute;a: 'VE'\"];\n";
        file<<"\"item79NA\" [label= \"id Neurona: "<<returnID("NA")<<"\\nCategor&iacute;a: 'NA'\"];\n";
        file<<"\"item79DO\" [label= \"id Neurona: "<<returnID("DO")<<"\\nCategor&iacute;a: 'DO'\"];\n";
        for(int j=0; j<numNeuron; j++){
            if(neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[j]-1)*9+CAT] != (unsigned int)returnCategory(word2) && neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[j])*9+CAT] != (unsigned int)returnCategory(word2) && h != 0 && h != 3){
                h = 2;
                file<<"\"item"<<neuralSenses[SIGHT].ptr[j]+0<<"\" [label=  \"id Neurona: "<<neuralSenses[SIGHT].ptr[j]+0<<"\\nCategor&iacute;a: 'VENADO'\"];\n";
            }
            if(neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[id]+j)*9+CAT] == (unsigned int)returnCategory(word2) && h != 2){
                h = 3;
                file<<"\"item"<<neuralSenses[SIGHT].ptr[id]+j<<"\" [label=  \"id Neurona: "<<neuralSenses[SIGHT].ptr[id]+j<<"\\nCategor&iacute;a: 'VENADO'\"];\n";
            }
        }
        file<<"}\n";
        h = 0;
        break;
    case 80:
        file<<"subgraph cluster_80{ ";
        file<<"\"item80A\" [label= \"id Neurona: "<<returnID("A")<<"\\nCategor&iacute;a: 'A'\"];\n";
        file<<"\"item80VION\" [label= \"id Neurona: "<<returnID("VION")<<"\\nCategor&iacute;a: 'VION'\"];\n";
        for(int j=0; j<numNeuron; j++){
            if(neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[j]-1)*9+CAT] != (unsigned int)returnCategory(word2) && neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[j])*9+CAT] != (unsigned int)returnCategory(word2) && h != 0 && h != 3){
                h = 2;
                file<<"\"item"<<neuralSenses[SIGHT].ptr[j]+0<<"\" [label=  \"id Neurona: "<<neuralSenses[SIGHT].ptr[j]+0<<"\\nCategor&iacute;a: 'AVION'\"];\n";
            }
            if(neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[id]+j)*9+CAT] == (unsigned int)returnCategory(word2) && h != 2){
                h = 3;
                file<<"\"item"<<neuralSenses[SIGHT].ptr[id]+j<<"\" [label=  \"id Neurona: "<<neuralSenses[SIGHT].ptr[id]+j<<"\\nCategor&iacute;a: 'AVION'\"];\n";
            }
        }
        file<<"}\n";
        h = 0;
        break;
    case 81:
        file<<"subgraph cluster_81{ ";
        file<<"\"item81CO\" [label= \"id Neurona: "<<returnID("CO")<<"\\nCategor&iacute;a: 'CO'\"];\n";
        file<<"\"item81BRA\" [label= \"id Neurona: "<<returnID("BRA")<<"\\nCategor&iacute;a: 'BRA'\"];\n";
        for(int j=0; j<numNeuron; j++){
            if(neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[j]-1)*9+CAT] != (unsigned int)returnCategory(word2) && neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[j])*9+CAT] != (unsigned int)returnCategory(word2) && h != 0 && h != 3){
                h = 2;
                file<<"\"item"<<neuralSenses[SIGHT].ptr[j]+0<<"\" [label=  \"id Neurona: "<<neuralSenses[SIGHT].ptr[j]+0<<"\\nCategor&iacute;a: 'COBRA'\"];\n";
            }
            if(neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[id]+j)*9+CAT] == (unsigned int)returnCategory(word2) && h != 2){
                h = 3;
                file<<"\"item"<<neuralSenses[SIGHT].ptr[id]+j<<"\" [label=  \"id Neurona: "<<neuralSenses[SIGHT].ptr[id]+j<<"\\nCategor&iacute;a: 'COBRA'\"];\n";
            }
        }
        file<<"}\n";
        h = 0;
        break;
    case 82:
        file<<"subgraph cluster_82{ ";
        file<<"\"item82U\" [label= \"id Neurona: "<<returnID("U")<<"\\nCategor&iacute;a: 'U'\"];\n";
        file<<"\"item82N\" [label= \"id Neurona: "<<returnID("N")<<"\\nCategor&iacute;a: 'N'\"];\n";
        for(int j=0; j<numNeuron; j++){
            if(neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[j]-1)*9+CAT] != (unsigned int)returnCategory(word2) && neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[j])*9+CAT] != (unsigned int)returnCategory(word2) && h != 0 && h != 3){
                h = 2;
                file<<"\"item"<<neuralSenses[SIGHT].ptr[j]+0<<"\" [label=  \"id Neurona: "<<neuralSenses[SIGHT].ptr[j]+0<<"\\nCategor&iacute;a: 'UN'\"];\n";
            }
            if(neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[id]+j)*9+CAT] == (unsigned int)returnCategory(word2) && h != 2){
                h = 3;
                file<<"\"item"<<neuralSenses[SIGHT].ptr[id]+j<<"\" [label=  \"id Neurona: "<<neuralSenses[SIGHT].ptr[id]+j<<"\\nCategor&iacute;a: 'UN'\"];\n";
            }
        }
        file<<"}\n";
        h = 0;
        break;
    case 83:
        file<<"subgraph cluster_83{ ";
        file<<"\"item83UN\" [label= \"id Neurona: "<<returnID("UN")<<"\\nCategor&iacute;a: 'UN'\"];\n";
        file<<"\"item83O\" [label= \"id Neurona: "<<returnID("O")<<"\\nCategor&iacute;a: 'O'\"];\n";
        for(int j=0; j<numNeuron; j++){
            if(neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[j]-1)*9+CAT] != (unsigned int)returnCategory(word2) && neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[j])*9+CAT] != (unsigned int)returnCategory(word2) && h != 0 && h != 3){
                h = 2;
                file<<"\"item"<<neuralSenses[SIGHT].ptr[j]+0<<"\" [label=  \"id Neurona: "<<neuralSenses[SIGHT].ptr[j]+0<<"\\nCategor&iacute;a: 'UNO'\"];\n";
            }
            if(neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[id]+j)*9+CAT] == (unsigned int)returnCategory(word2) && h != 2){
                h = 3;
                file<<"\"item"<<neuralSenses[SIGHT].ptr[id]+j<<"\" [label=  \"id Neurona: "<<neuralSenses[SIGHT].ptr[id]+j<<"\\nCategor&iacute;a: 'UNO'\"];\n";
            }
        }
        file<<"}\n";
        h = 0;
        break;
    case 84:
        file<<"subgraph cluster_84{ ";
        file<<"\"item84DO\" [label= \"id Neurona: "<<returnID("DO")<<"\\nCategor&iacute;a: 'DO'\"];\n";
        file<<"\"item84S\" [label= \"id Neurona: "<<returnID("S")<<"\\nCategor&iacute;a: 'S'\"];\n";
        for(int j=0; j<numNeuron; j++){
            if(neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[j]-1)*9+CAT] != (unsigned int)returnCategory(word2) && neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[j])*9+CAT] != (unsigned int)returnCategory(word2) && h != 0 && h != 3){
                h = 2;
                file<<"\"item"<<neuralSenses[SIGHT].ptr[j]+0<<"\" [label=  \"id Neurona: "<<neuralSenses[SIGHT].ptr[j]+0<<"\\nCategor&iacute;a: 'DOS'\"];\n";
            }
            if(neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[id]+j)*9+CAT] == (unsigned int)returnCategory(word2) && h != 2){
                h = 3;
                file<<"\"item"<<neuralSenses[SIGHT].ptr[id]+j<<"\" [label=  \"id Neurona: "<<neuralSenses[SIGHT].ptr[id]+j<<"\\nCategor&iacute;a: 'DOS'\"];\n";
            }
        }
        file<<"}\n";
        h = 0;
        break;
    case 85:
        file<<"subgraph cluster_85{ ";
        file<<"\"item85T\" [label= \"id Neurona: "<<returnID("T")<<"\\nCategor&iacute;a: 'T'\"];\n";
        file<<"\"item85R\" [label= \"id Neurona: "<<returnID("R")<<"\\nCategor&iacute;a: 'R'\"];\n";
        file<<"\"item85E\" [label= \"id Neurona: "<<returnID("E")<<"\\nCategor&iacute;a: 'E'\"];\n";
        for(int j=0; j<numNeuron; j++){
            if(neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[j]-1)*9+CAT] != (unsigned int)returnCategory(word2) && neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[j])*9+CAT] != (unsigned int)returnCategory(word2) && h != 0 && h != 3){
                h = 2;
                file<<"\"item"<<neuralSenses[SIGHT].ptr[j]+0<<"\" [label=  \"id Neurona: "<<neuralSenses[SIGHT].ptr[j]+0<<"\\nCategor&iacute;a: 'TRE'\"];\n";
            }
            if(neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[id]+j)*9+CAT] == (unsigned int)returnCategory(word2) && h != 2){
                h = 3;
                file<<"\"item"<<neuralSenses[SIGHT].ptr[id]+j<<"\" [label=  \"id Neurona: "<<neuralSenses[SIGHT].ptr[id]+j<<"\\nCategor&iacute;a: 'TRE'\"];\n";
            }
        }
        file<<"}\n";
        h = 0;
        break;
    case 86:
        file<<"subgraph cluster_86{ ";
        file<<"\"item86TRE\" [label= \"id Neurona: "<<returnID("TRE")<<"\\nCategor&iacute;a: 'TRE'\"];\n";
        file<<"\"item86S\" [label= \"id Neurona: "<<returnID("S")<<"\\nCategor&iacute;a: 'S'\"];\n";
        for(int j=0; j<numNeuron; j++){
            if(neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[j]-1)*9+CAT] != (unsigned int)returnCategory(word2) && neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[j])*9+CAT] != (unsigned int)returnCategory(word2) && h != 0 && h != 3){
                h = 2;
                file<<"\"item"<<neuralSenses[SIGHT].ptr[j]+0<<"\" [label=  \"id Neurona: "<<neuralSenses[SIGHT].ptr[j]+0<<"\\nCategor&iacute;a: 'TRES'\"];\n";
            }
            if(neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[id]+j)*9+CAT] == (unsigned int)returnCategory(word2) && h != 2){
                h = 3;
                file<<"\"item"<<neuralSenses[SIGHT].ptr[id]+j<<"\" [label=  \"id Neurona: "<<neuralSenses[SIGHT].ptr[id]+j<<"\\nCategor&iacute;a: 'TRES'\"];\n";
            }
        }
        file<<"}\n";
        h = 0;
        break;
    case 87:
        file<<"subgraph cluster_87{ ";
        file<<"\"item87T\" [label= \"id Neurona: "<<returnID("T")<<"\\nCategor&iacute;a: 'T'\"];\n";
        file<<"\"item87R\" [label= \"id Neurona: "<<returnID("R")<<"\\nCategor&iacute;a: 'R'\"];\n";
        file<<"\"item87O\" [label= \"id Neurona: "<<returnID("O")<<"\\nCategor&iacute;a: 'O'\"];\n";
        for(int j=0; j<numNeuron; j++){
            if(neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[j]-1)*9+CAT] != (unsigned int)returnCategory(word2) && neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[j])*9+CAT] != (unsigned int)returnCategory(word2) && h != 0 && h != 3){
                h = 2;
                file<<"\"item"<<neuralSenses[SIGHT].ptr[j]+0<<"\" [label=  \"id Neurona: "<<neuralSenses[SIGHT].ptr[j]+0<<"\\nCategor&iacute;a: 'TRO'\"];\n";
            }
            if(neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[id]+j)*9+CAT] == (unsigned int)returnCategory(word2) && h != 2){
                h = 3;
                file<<"\"item"<<neuralSenses[SIGHT].ptr[id]+j<<"\" [label=  \"id Neurona: "<<neuralSenses[SIGHT].ptr[id]+j<<"\\nCategor&iacute;a: 'TRO'\"];\n";
            }
        }
        file<<"}\n";
        h = 0;
        break;
    case 88:
        file<<"subgraph cluster_88{ ";
        file<<"\"item88CU\" [label= \"id Neurona: "<<returnID("CU")<<"\\nCategor&iacute;a: 'CU'\"];\n";
        file<<"\"item88A\" [label= \"id Neurona: "<<returnID("A")<<"\\nCategor&iacute;a: 'A'\"];\n";
        file<<"\"item88TRO\" [label= \"id Neurona: "<<returnID("TRO")<<"\\nCategor&iacute;a: 'TRO'\"];\n";
        for(int j=0; j<numNeuron; j++){
            if(neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[j]-1)*9+CAT] != (unsigned int)returnCategory(word2) && neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[j])*9+CAT] != (unsigned int)returnCategory(word2) && h != 0 && h != 3){
                h = 2;
                file<<"\"item"<<neuralSenses[SIGHT].ptr[j]+0<<"\" [label=  \"id Neurona: "<<neuralSenses[SIGHT].ptr[j]+0<<"\\nCategor&iacute;a: 'CUATRO'\"];\n";
            }
            if(neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[id]+j)*9+CAT] == (unsigned int)returnCategory(word2) && h != 2){
                h = 3;
                file<<"\"item"<<neuralSenses[SIGHT].ptr[id]+j<<"\" [label=  \"id Neurona: "<<neuralSenses[SIGHT].ptr[id]+j<<"\\nCategor&iacute;a: 'CUATRO'\"];\n";
            }
        }
        file<<"}\n";
        h = 0;
        break;
    case 89:
        file<<"subgraph cluster_89{ ";
        file<<"\"item89C\" [label= \"id Neurona: "<<returnID("C")<<"\\nCategor&iacute;a: 'C'\"];\n";
        file<<"\"item89I\" [label= \"id Neurona: "<<returnID("I")<<"\\nCategor&iacute;a: 'I'\"];\n";
        file<<"\"item89N\" [label= \"id Neurona: "<<returnID("N")<<"\\nCategor&iacute;a: 'N'\"];\n";
        for(int j=0; j<numNeuron; j++){
            if(neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[j]-1)*9+CAT] != (unsigned int)returnCategory(word2) && neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[j])*9+CAT] != (unsigned int)returnCategory(word2) && h != 0 && h != 3){
                h = 2;
                file<<"\"item"<<neuralSenses[SIGHT].ptr[j]+0<<"\" [label=  \"id Neurona: "<<neuralSenses[SIGHT].ptr[j]+0<<"\\nCategor&iacute;a: 'CIN'\"];\n";
            }
            if(neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[id]+j)*9+CAT] == (unsigned int)returnCategory(word2) && h != 2){
                h = 3;
                file<<"\"item"<<neuralSenses[SIGHT].ptr[id]+j<<"\" [label=  \"id Neurona: "<<neuralSenses[SIGHT].ptr[id]+j<<"\\nCategor&iacute;a: 'CIN'\"];\n";
            }
        }
        file<<"}\n";
        h = 0;
        break;
    case 90:
        file<<"subgraph cluster_90{ ";
        file<<"\"item90CIN\" [label= \"id Neurona: "<<returnID("CIN")<<"\\nCategor&iacute;a: 'CIN'\"];\n";
        file<<"\"item90CO\" [label= \"id Neurona: "<<returnID("CO")<<"\\nCategor&iacute;a: 'CO'\"];\n";
        for(int j=0; j<numNeuron; j++){
            if(neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[j]-1)*9+CAT] != (unsigned int)returnCategory(word2) && neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[j])*9+CAT] != (unsigned int)returnCategory(word2) && h != 0 && h != 3){
                h = 2;
                file<<"\"item"<<neuralSenses[SIGHT].ptr[j]+0<<"\" [label=  \"id Neurona: "<<neuralSenses[SIGHT].ptr[j]+0<<"\\nCategor&iacute;a: 'CIN'\"];\n";
            }
            if(neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[id]+j)*9+CAT] == (unsigned int)returnCategory(word2) && h != 2){
                h = 3;
                file<<"\"item"<<neuralSenses[SIGHT].ptr[id]+j<<"\" [label=  \"id Neurona: "<<neuralSenses[SIGHT].ptr[id]+j<<"\\nCategor&iacute;a: 'CO'\"];\n";
            }
        }
        file<<"}\n";
        h = 0;
        break;
    case 91:
        file<<"subgraph cluster_91{ ";
        file<<"\"item91S\" [label= \"id Neurona: "<<returnID("S")<<"\\nCategor&iacute;a: 'S'\"];\n";
        file<<"\"item91E\" [label= \"id Neurona: "<<returnID("E")<<"\\nCategor&iacute;a: 'E'\"];\n";
        for(int j=0; j<numNeuron; j++){
            if(neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[j]-1)*9+CAT] != (unsigned int)returnCategory(word2) && neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[j])*9+CAT] != (unsigned int)returnCategory(word2) && h != 0 && h != 3){
                h = 2;
                file<<"\"item"<<neuralSenses[SIGHT].ptr[j]+0<<"\" [label=  \"id Neurona: "<<neuralSenses[SIGHT].ptr[j]+0<<"\\nCategor&iacute;a: 'SE'\"];\n";
            }
            if(neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[id]+j)*9+CAT] == (unsigned int)returnCategory(word2) && h != 2){
                h = 3;
                file<<"\"item"<<neuralSenses[SIGHT].ptr[id]+j<<"\" [label=  \"id Neurona: "<<neuralSenses[SIGHT].ptr[id]+j<<"\\nCategor&iacute;a: 'SE'\"];\n";
            }
        }
        file<<"}\n";
        h = 0;
        break;
    case 92:
        file<<"subgraph cluster_92{ ";
        file<<"\"item92I\" [label= \"id Neurona: "<<returnID("I")<<"\\nCategor&iacute;a: 'I'\"];\n";
        file<<"\"item92S\" [label= \"id Neurona: "<<returnID("S")<<"\\nCategor&iacute;a: 'S'\"];\n";
        for(int j=0; j<numNeuron; j++){
            if(neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[j]-1)*9+CAT] != (unsigned int)returnCategory(word2) && neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[j])*9+CAT] != (unsigned int)returnCategory(word2) && h != 0 && h != 3){
                h = 2;
                file<<"\"item"<<neuralSenses[SIGHT].ptr[j]+0<<"\" [label=  \"id Neurona: "<<neuralSenses[SIGHT].ptr[j]+0<<"\\nCategor&iacute;a: 'IS'\"];\n";
            }
            if(neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[id]+j)*9+CAT] == (unsigned int)returnCategory(word2) && h != 2){
                h = 3;
                file<<"\"item"<<neuralSenses[SIGHT].ptr[id]+j<<"\" [label=  \"id Neurona: "<<neuralSenses[SIGHT].ptr[id]+j<<"\\nCategor&iacute;a: 'IS'\"];\n";
            }
        }
        file<<"}\n";
        h = 0;
        break;
    case 93:
        file<<"subgraph cluster_93{ ";
        file<<"\"item93SE\" [label= \"id Neurona: "<<returnID("SE")<<"\\nCategor&iacute;a: 'SE'\"];\n";
        file<<"\"item93IS\" [label= \"id Neurona: "<<returnID("IS")<<"\\nCategor&iacute;a: 'IS'\"];\n";
        for(int j=0; j<numNeuron; j++){
            if(neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[j]-1)*9+CAT] != (unsigned int)returnCategory(word2) && neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[j])*9+CAT] != (unsigned int)returnCategory(word2) && h != 0 && h != 3){
                h = 2;
                file<<"\"item"<<neuralSenses[SIGHT].ptr[j]+0<<"\" [label=  \"id Neurona: "<<neuralSenses[SIGHT].ptr[j]+0<<"\\nCategor&iacute;a: 'SEIS'\"];\n";
            }
            if(neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[id]+j)*9+CAT] == (unsigned int)returnCategory(word2) && h != 2){
                h = 3;
                file<<"\"item"<<neuralSenses[SIGHT].ptr[id]+j<<"\" [label=  \"id Neurona: "<<neuralSenses[SIGHT].ptr[id]+j<<"\\nCategor&iacute;a: 'SEIS'\"];\n";
            }
        }
        file<<"}\n";
        h = 0;
        break;
    case 94:
        file<<"subgraph cluster_94{ ";
        file<<"\"item94S\" [label= \"id Neurona: "<<returnID("S")<<"\\nCategor&iacute;a: 'S'\"];\n";
        file<<"\"item94I\" [label= \"id Neurona: "<<returnID("I")<<"\\nCategor&iacute;a: 'I'\"];\n";
        for(int j=0; j<numNeuron; j++){
            if(neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[j]-1)*9+CAT] != (unsigned int)returnCategory(word2) && neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[j])*9+CAT] != (unsigned int)returnCategory(word2) && h != 0 && h != 3){
                h = 2;
                file<<"\"item"<<neuralSenses[SIGHT].ptr[j]+0<<"\" [label=  \"id Neurona: "<<neuralSenses[SIGHT].ptr[j]+0<<"\\nCategor&iacute;a: 'SI'\"];\n";
            }
            if(neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[id]+j)*9+CAT] == (unsigned int)returnCategory(word2) && h != 2){
                h = 3;
                file<<"\"item"<<neuralSenses[SIGHT].ptr[id]+j<<"\" [label=  \"id Neurona: "<<neuralSenses[SIGHT].ptr[id]+j<<"\\nCategor&iacute;a: 'SI'\"];\n";
            }
        }
        file<<"}\n";
        h = 0;
        break;
    case 95:
        file<<"subgraph cluster_95{ ";
        file<<"\"item95T\" [label= \"id Neurona: "<<returnID("T")<<"\\nCategor&iacute;a: 'T'\"];\n";
        file<<"\"item95E\" [label= \"id Neurona: "<<returnID("E")<<"\\nCategor&iacute;a: 'E'\"];\n";
        for(int j=0; j<numNeuron; j++){
            if(neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[j]-1)*9+CAT] != (unsigned int)returnCategory(word2) && neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[j])*9+CAT] != (unsigned int)returnCategory(word2) && h != 0 && h != 3){
                h = 2;
                file<<"\"item"<<neuralSenses[SIGHT].ptr[j]+0<<"\" [label=  \"id Neurona: "<<neuralSenses[SIGHT].ptr[j]+0<<"\\nCategor&iacute;a: 'TE'\"];\n";
            }
            if(neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[id]+j)*9+CAT] == (unsigned int)returnCategory(word2) && h != 2){
                h = 3;
                file<<"\"item"<<neuralSenses[SIGHT].ptr[id]+j<<"\" [label=  \"id Neurona: "<<neuralSenses[SIGHT].ptr[id]+j<<"\\nCategor&iacute;a: 'TE'\"];\n";
            }
        }
        file<<"}\n";
        h = 0;
        break;
    case 96:
        file<<"subgraph cluster_97{ ";
        file<<"\"item97SI\" [label= \"id Neurona: "<<returnID("SI")<<"\\nCategor&iacute;a: 'SI'\"];\n";
        file<<"\"item97E\" [label= \"id Neurona: "<<returnID("E")<<"\\nCategor&iacute;a: 'E'\"];\n";
        file<<"\"item97TE\" [label= \"id Neurona: "<<returnID("TE")<<"\\nCategor&iacute;a: 'TE'\"];\n";
        for(int j=0; j<numNeuron; j++){
            if(neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[j]-1)*9+CAT] != (unsigned int)returnCategory(word2) && neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[j])*9+CAT] != (unsigned int)returnCategory(word2) && h != 0 && h != 3){
                h = 2;
                file<<"\"item"<<neuralSenses[SIGHT].ptr[j]+0<<"\" [label=  \"id Neurona: "<<neuralSenses[SIGHT].ptr[j]+0<<"\\nCategor&iacute;a: 'SIETE'\"];\n";
            }
            if(neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[id]+j)*9+CAT] == (unsigned int)returnCategory(word2) && h != 2){
                h = 3;
                file<<"\"item"<<neuralSenses[SIGHT].ptr[id]+j<<"\" [label=  \"id Neurona: "<<neuralSenses[SIGHT].ptr[id]+j<<"\\nCategor&iacute;a: 'SIETE'\"];\n";
            }
        }
        file<<"}\n";
        h = 0;
        break;
    case 97:
        file<<"subgraph cluster_97{ ";
        file<<"\"item97C\" [label= \"id Neurona: "<<returnID("C")<<"\\nCategor&iacute;a: 'C'\"];\n";
        file<<"\"item97H\" [label= \"id Neurona: "<<returnID("H")<<"\\nCategor&iacute;a: 'H'\"];\n";
        file<<"\"item970\" [label= \"id Neurona: "<<returnID("O")<<"\\nCategor&iacute;a: 'O'\"];\n";
        for(int j=0; j<numNeuron; j++){
            if(neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[j]-1)*9+CAT] != (unsigned int)returnCategory(word2) && neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[j])*9+CAT] != (unsigned int)returnCategory(word2) && h != 0 && h != 3){
                h = 2;
                file<<"\"item"<<neuralSenses[SIGHT].ptr[j]+0<<"\" [label=  \"id Neurona: "<<neuralSenses[SIGHT].ptr[j]+0<<"\\nCategor&iacute;a: 'CHO'\"];\n";
            }
            if(neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[id]+j)*9+CAT] == (unsigned int)returnCategory(word2) && h != 2){
                h = 3;
                file<<"\"item"<<neuralSenses[SIGHT].ptr[id]+j<<"\" [label=  \"id Neurona: "<<neuralSenses[SIGHT].ptr[id]+j<<"\\nCategor&iacute;a: 'CHO'\"];\n";
            }
        }
        file<<"}\n";
        h = 0;
        break;
    case 98:
        file<<"subgraph cluster_98{ ";
        file<<"\"item98O\" [label= \"id Neurona: "<<returnID("O")<<"\\nCategor&iacute;a: 'O'\"];\n";
        file<<"\"item98CHO\" [label= \"id Neurona: "<<returnID("CHO")<<"\\nCategor&iacute;a: 'CHO'\"];\n";
        for(int j=0; j<numNeuron; j++){
            if(neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[j]-1)*9+CAT] != (unsigned int)returnCategory(word2) && neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[j])*9+CAT] != (unsigned int)returnCategory(word2) && h != 0 && h != 3){
                h = 2;
                file<<"\"item"<<neuralSenses[SIGHT].ptr[j]+0<<"\" [label=  \"id Neurona: "<<neuralSenses[SIGHT].ptr[j]+0<<"\\nCategor&iacute;a: 'OCHO'\"];\n";
            }
            if(neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[id]+j)*9+CAT] == (unsigned int)returnCategory(word2) && h != 2){
                h = 3;
                file<<"\"item"<<neuralSenses[SIGHT].ptr[id]+j<<"\" [label=  \"id Neurona: "<<neuralSenses[SIGHT].ptr[id]+j<<"\\nCategor&iacute;a: 'OCHO'\"];\n";
            }
        }
        file<<"}\n";
        h = 0;
        break;
    case 99:
        file<<"subgraph cluster_99{ ";
        file<<"\"item99N\" [label= \"id Neurona: "<<returnID("N")<<"\\nCategor&iacute;a: 'N'\"];\n";
        file<<"\"item99U\" [label= \"id Neurona: "<<returnID("U")<<"\\nCategor&iacute;a: 'U'\"];\n";
        for(int j=0; j<numNeuron; j++){
            if(neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[j]-1)*9+CAT] != (unsigned int)returnCategory(word2) && neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[j])*9+CAT] != (unsigned int)returnCategory(word2) && h != 0 && h != 3){
                h = 2;
                file<<"\"item"<<neuralSenses[SIGHT].ptr[j]+0<<"\" [label=  \"id Neurona: "<<neuralSenses[SIGHT].ptr[j]+0<<"\\nCategor&iacute;a: 'NU'\"];\n";
            }
            if(neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[id]+j)*9+CAT] == (unsigned int)returnCategory(word2) && h != 2){
                h = 3;
                file<<"\"item"<<neuralSenses[SIGHT].ptr[id]+j<<"\" [label=  \"id Neurona: "<<neuralSenses[SIGHT].ptr[id]+j<<"\\nCategor&iacute;a: 'NU'\"];\n";
            }
        }
        file<<"}\n";
        h = 0;
        break;
    case 100:
        file<<"subgraph cluster_100{ ";
        file<<"\"item100V\" [label= \"id Neurona: "<<returnID("V")<<"\\nCategor&iacute;a: 'V'\"];\n";
        file<<"\"item100E\" [label= \"id Neurona: "<<returnID("E")<<"\\nCategor&iacute;a: 'E'\"];\n";
        for(int j=0; j<numNeuron; j++){
            if(neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[j]-1)*9+CAT] != (unsigned int)returnCategory(word2) && neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[j])*9+CAT] != (unsigned int)returnCategory(word2) && h != 0 && h != 3){
                h = 2;
                file<<"\"item"<<neuralSenses[SIGHT].ptr[j]+0<<"\" [label=  \"id Neurona: "<<neuralSenses[SIGHT].ptr[j]+0<<"\\nCategor&iacute;a: 'VE'\"];\n";
            }
            if(neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[id]+j)*9+CAT] == (unsigned int)returnCategory(word2) && h != 2){
                h = 3;
                file<<"\"item"<<neuralSenses[SIGHT].ptr[id]+j<<"\" [label=  \"id Neurona: "<<neuralSenses[SIGHT].ptr[id]+j<<"\\nCategor&iacute;a: 'VE'\"];\n";
            }
        }
        file<<"}\n";
        h = 0;
        break;
    case 101:
        file<<"subgraph cluster_101{ ";
        file<<"\"item101NU\" [label= \"id Neurona: "<<returnID("NU")<<"\\nCategor&iacute;a: 'NU'\"];\n";
        file<<"\"item101E\" [label= \"id Neurona: "<<returnID("E")<<"\\nCategor&iacute;a: 'E'\"];\n";
        file<<"\"item101VE\" [label= \"id Neurona: "<<returnID("VE")<<"\\nCategor&iacute;a: 'VE'\"];\n";
        for(int j=0; j<numNeuron; j++){
            if(neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[j]-1)*9+CAT] != (unsigned int)returnCategory(word2) && neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[j])*9+CAT] != (unsigned int)returnCategory(word2) && h != 0 && h != 3){
                h = 2;
                file<<"\"item"<<neuralSenses[SIGHT].ptr[j]+0<<"\" [label=  \"id Neurona: "<<neuralSenses[SIGHT].ptr[j]+0<<"\\nCategor&iacute;a: 'NUEVE'\"];\n";
            }
            if(neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[id]+j)*9+CAT] == (unsigned int)returnCategory(word2) && h != 2){
                h = 3;
                file<<"\"item"<<neuralSenses[SIGHT].ptr[id]+j<<"\" [label=  \"id Neurona: "<<neuralSenses[SIGHT].ptr[id]+j<<"\\nCategor&iacute;a: 'NUEVE'\"];\n";
            }
        }
        file<<"}\n";
        h = 0;
        break;
    case 102:
        file<<"subgraph cluster_102{ ";
        file<<"\"item102R\" [label= \"id Neurona: "<<returnID("R")<<"\\nCategor&iacute;a: 'R'\"];\n";
        file<<"\"item102O\" [label= \"id Neurona: "<<returnID("O")<<"\\nCategor&iacute;a: 'O'\"];\n";
        for(int j=0; j<numNeuron; j++){
            if(neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[j]-1)*9+CAT] != (unsigned int)returnCategory(word2) && neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[j])*9+CAT] != (unsigned int)returnCategory(word2) && h != 0 && h != 3){
                h = 2;
                file<<"\"item"<<neuralSenses[SIGHT].ptr[j]+0<<"\" [label=  \"id Neurona: "<<neuralSenses[SIGHT].ptr[j]+0<<"\\nCategor&iacute;a: 'RO'\"];\n";
            }
            if(neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[id]+j)*9+CAT] == (unsigned int)returnCategory(word2) && h != 2){
                h = 3;
                file<<"\"item"<<neuralSenses[SIGHT].ptr[id]+j<<"\" [label=  \"id Neurona: "<<neuralSenses[SIGHT].ptr[id]+j<<"\\nCategor&iacute;a: 'RO'\"];\n";
            }
        }
        file<<"}\n";
        h = 0;
        break;
    case 103:
        file<<"subgraph cluster_103{ ";
        file<<"\"item103CE\" [label= \"id Neurona: "<<returnID("CE")<<"\\nCategor&iacute;a: 'CE'\"];\n";
        file<<"\"item103RO\" [label= \"id Neurona: "<<returnID("RO")<<"\\nCategor&iacute;a: 'RO'\"];\n";
        for(int j=0; j<numNeuron; j++){
            if(neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[j]-1)*9+CAT] != (unsigned int)returnCategory(word2) && neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[j])*9+CAT] != (unsigned int)returnCategory(word2) && h != 0 && h != 3){
                h = 2;
                file<<"\"item"<<neuralSenses[SIGHT].ptr[j]+0<<"\" [label=  \"id Neurona: "<<neuralSenses[SIGHT].ptr[j]+0<<"\\nCategor&iacute;a: 'CERO'\"];\n";
            }
            if(neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[id]+j)*9+CAT] == (unsigned int)returnCategory(word2) && h != 2){
                h = 3;
                file<<"\"item"<<neuralSenses[SIGHT].ptr[id]+j<<"\" [label=  \"id Neurona: "<<neuralSenses[SIGHT].ptr[id]+j<<"\\nCategor&iacute;a: 'CERO'\"];\n";
            }
        }
        file<<"}\n";
        h = 0;
        break;
    default:
        file<<"subgraph cluster_69{ ";
        if(returnID(word2) != 999)
            file<<"\"item"<<id<<"\" [label=  \"id Neurona: "<<id<<"\\nCategor&iacute;a: "<<word2.toStdString()<<"\"];\n";
        file<<"}\n";
        break;
    }
}

int MainWindow::returnID(QString sy){
    std::cout<<"MainWindow::returnID"<<endl;

    if(sy == "A")
        return 12;
    if(sy == "B")
        return 13;
    if(sy == "C")
        return 14;
    if(sy == "D")
        return 15;
    if(sy == "E")
        return 16;
    if(sy == "F")
        return 17;
    if(sy == "G")
        return 18;
    if(sy == "H")
        return 19;
    if(sy == "I")
        return 20;
    if(sy == "J")
        return 21;
    if(sy == "K")
        return 22;
    if(sy == "L")
        return 23;
    if(sy == "M")
        return 24;
    if(sy == "N")
        return 25;
    if(sy == "O")
        return 26;
    if(sy == "P")
        return 27;
    if(sy == "Q")
        return 28;
    if(sy == "R")
        return 29;
    if(sy == "S")
        return 30;
    if(sy == "T")
        return 31;
    if(sy == "U")
        return 32;
    if(sy == "V")
        return 33;
    if(sy == "W")
        return 34;
    if(sy == "X")
        return 35;
    if(sy == "Y")
        return 36;
    if(sy == "Z")
        return 37;
    if(sy == "AB")
        return 38;
    if(sy == "BA")
        return 39;
    if(sy == "CA")
        return 40;
    if(sy == "LLO")
        return 41;
    if(sy == "DA")
        return 42;
    if(sy == "SA")
        return 43;
    if(sy == "RA")
        return 44;
    if(sy == "PO")
        return 45;
    if(sy == "CE")
        return 46;
    if(sy == "GA")
        return 47;
    if(sy == "TO")
        return 48;
    if(sy == "TA")
        return 49;
    if(sy == "PA")
        return 50;
    if(sy == "DIS")
        return 51;
    if(sy == "RRO")
        return 52;
    if(sy == "JO")
        return 53;
    if(sy == "AR")
        return 54;
    if(sy == "CO")
        return 55;
    if(sy == "BRA")
        return 56;
    if(sy == "LO")
        return 57;
    if(sy == "CU")
        return 58;
    if(sy == "LE")
        return 59;
    if(sy == "BOL")
        return 60;
    if(sy == "ON")
        return 61;
    if(sy == "GO")
        return 62;
    if(sy == "VE")
        return 63;
    if(sy == "NA")
        return 64;
    if(sy == "DO")
        return 65;
    if(sy == "RRA")
        return 66;
    if(sy == "VION")
        return 67;
    if(sy == "DRI")
        return 68;
    if(sy == "COCO")
        return 69;
    if(sy == "CABALLO")
        return 70;
    if(sy == "CASA")
        return 71;
    if(sy == "SAPO")
        return 72;
    if(sy == "GATO")
        return 73;
    if(sy == "PATO")
        return 74;
    if(sy == "CARRO")
        return 75;
    if(sy == "GALLO")
        return 76;
    if(sy == "CULEBRA")
        return 77;
    if(sy == "LEON")
        return 78;
    if(sy == "VENADO")
        return 79;
    if(sy == "AVION")
        return 80;
    if(sy == "COBRA")
        return 81;
    ////ID de silabas agregadas por Johan Sosa/////
    if(sy == "UN")
        return 82;
    if(sy == "UNO")
        return 83;
    if(sy == "DOS")
        return 84;
    if(sy == "TRE")
        return 85;
    if(sy == "TRES")
        return 86;
    if(sy == "TRO")
        return 87;
    if(sy == "CUATRO")
        return 88;
    if(sy == "CIN")
        return 89;
    if(sy == "CINCO")
        return 90;
    if(sy == "SE")
        return 91;
    if(sy == "IS")
        return 92;
    if(sy == "SEIS")
        return 93;
    if(sy == "SI")
        return 94;
    if(sy == "TE")
        return 95;
    if(sy == "SIETE")
        return 96;
    if(sy == "CHO")
        return 97;
    if(sy == "OCHO")
        return 98;
    if(sy == "NU")
        return 99;
    if(sy == "VE")
        return 100;
    if(sy == "NUEVE")
        return 101;
    if(sy == "RO")
        return 102;
    if(sy == "CERO")
        return 103;
    return 999;
}

void MainWindow::correctWord(){
    std::cout<<"MainWindow::correctWord"<<endl;

    isInactivateSense[HEARING] = true;
    activeLayers(true);
    ui->pushButtonBNSWord->hide();
    ui->pushButtonCorrectWord->hide();
    ui->pushButtonImage->hide();
    ui->pushButtonImages->hide();
    ui->pushButtonSoundWord->hide();
    ui->textBrowserWord->hide();
    ui->pushButtonBip->hide();
    ui->pushButtonBipSyl->hide();
    ui->pushButtonBipWord->hide();
    ui->pushButtonCheck->hide();
    word.clear();
    word2.clear();
    ptr1 = 999, ptr2 = 999, ptr3 = 999, ptr4 = 999, ptr5 = 999, ptr6 = 999, ptr7 = 999, ptr8 = 999;
    nsyllab = 0;
    file();
}

int hit = 0;


//mudar esta funcion ////
void MainWindow::paintBinaryNetSyllab(){
    std::cout<<"MainWindow::paintBinarySyllab"<<endl;

    int numNeuron = *(neuralSenses[SIGHT].ptr);
    for(int id=0; id<numNeuron; id++){
        if(neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[id]-1)*9+CAT] != (unsigned int)returnCategory(word2) && neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[id])*9+CAT] != (unsigned int)returnCategory(word2) && neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[id]+id)*9+CAT] != (unsigned int)returnCategory(word2)){
            ui->pushButtonImage->setEnabled(false);
        }
        if(neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[id]-1)*9+CAT] == (unsigned int)returnCategory(word2) && hit < 3 && returnID(word2) != 999){
            hit = 1;
            paintBinarySyllab(neuralSenses[SIGHT].ptr[id]-1);
            break;
        }
        for(int i=0; i<numNeuron;i++){
            if(neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[id]+i)*9+CAT] == (unsigned int)returnCategory(word2) && returnID(word2) != 999){
                hit = 3;
                paintBinarySyllab(neuralSenses[SIGHT].ptr[id]+i);
            }
        }
    }
    hit = 0;
    ui->pushButtonImage->setEnabled(true);
}

int valor = 99999;

void MainWindow::saveSight(){
    std::cout<<"MainWindow::saveSight"<<endl;

    QString vect;
    ofstream file("./Obj/sight.dot",ios::app);//ios::out | ios::trunc);
    int numNeuron = *(neuralSenses[SIGHT].ptr);
    for(int id=valor; id < numNeuron; id++){
        file<<id<<";"<<neuralSenses->vectorFlags[id*9+CAT]<<";"<<neuralSenses->vectorFlags[id*9+RAT]<<";";
        for(int i=0; i<16; i++)
            file<<characteristicBinaryVector[i]<<";";
        for(int j = 0; j < 32; j++){
            vect.append(QString::number(neuralSenses->vectorNeuron[id*32+j]));
            if(j < 31)
                vect.append(",");
        }
        file<<vect.toStdString()<<endl;
        vect.clear();
        valor++;
    }
    file.close();
}

int MainWindow::getNumberNeurons(){
    int numberLines = 0;
    QFile file("./Obj/sight.dot");
    file.open(QIODevice::ReadOnly);
    while(!file.atEnd()){
        file.readLine();
        numberLines++;
    }
    file.close();

    return numberLines;
}

void MainWindow::getSight(){
    std::cout<<"MainWindow::getSight"<<endl;


    QString line, ident, ca, ra, vec1, vec2, vec3, vec4, vec5, vec6, vec7, vec8, vec9, vec10, vec11, vec12, vec13, vec14, vec15, vec16, vect;
    QString v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15, v16, v17, v18, v19, v20, v21, v22, v23, v24, v25, v26, v27, v28, v29, v30, v31, v32;
    QFile file("./Obj/sight.dot");
    bool ok;
    int item = 0;
    int size = getNumberNeurons();
    vectorId = new unsigned int[size];
    vectorCategory = new unsigned int[size];

    file.open(QIODevice::ReadOnly);


    while(!file.atEnd()){
        line = file.readLine();
        QStringList pieces = line.split(";");
        ident = pieces.value(pieces.length() - 20);
        ca = pieces.value(pieces.length() - 19);
        vectorCategory[item] = ca.toInt();
        vectorId[item] = ident.toInt();
        ra = pieces.value(pieces.length() - 18);
        vec1 = pieces.value(pieces.length() - 17);
        vec2 = pieces.value(pieces.length() - 16);
        vec3 = pieces.value(pieces.length() - 15);
        vec4 = pieces.value(pieces.length() - 14);
        vec5 = pieces.value(pieces.length() - 13);
        vec6 = pieces.value(pieces.length() - 12);
        vec7 = pieces.value(pieces.length() - 11);
        vec8 = pieces.value(pieces.length() - 10);
        vec9 = pieces.value(pieces.length() - 9);
        vec10 = pieces.value(pieces.length() - 8);
        vec11 = pieces.value(pieces.length() - 7);
        vec12 = pieces.value(pieces.length() - 6);
        vec13 = pieces.value(pieces.length() - 5);
        vec14 = pieces.value(pieces.length() - 4);
        vec15 = pieces.value(pieces.length() - 3);
        vec16 = pieces.value(pieces.length() - 2);
        vect = pieces.value(pieces.length() - 1);
        unsigned int cate = ca.toInt(&ok, 10);
        unsigned int rad = ra.toInt(&ok, 10);
        unsigned int value1 = vec1.toUInt(&ok, 0);
        unsigned int value2 = vec2.toUInt(&ok, 0);
        unsigned int value3 = vec3.toUInt(&ok, 0);
        unsigned int value4 = vec4.toUInt(&ok, 0);
        unsigned int value5 = vec5.toUInt(&ok, 0);
        unsigned int value6 = vec6.toUInt(&ok, 0);
        unsigned int value7 = vec7.toUInt(&ok, 0);
        unsigned int value8 = vec8.toUInt(&ok, 0);
        unsigned int value9 = vec9.toUInt(&ok, 0);
        unsigned int value10 = vec10.toUInt(&ok, 0);
        unsigned int value11 = vec11.toUInt(&ok, 0);
        unsigned int value12 = vec12.toUInt(&ok, 0);
        unsigned int value13 = vec13.toUInt(&ok, 0);
        unsigned int value14 = vec14.toUInt(&ok, 0);
        unsigned int value15 = vec15.toUInt(&ok, 0);
        unsigned int value16 = vec16.toUInt(&ok, 0);
        QStringList ve = vect.split(",");
        v1 = ve.value(ve.length() - 32);
        v2 = ve.value(ve.length() - 31);
        v3 = ve.value(ve.length() - 30);
        v4 = ve.value(ve.length() - 29);
        v5 = ve.value(ve.length() - 28);
        v6 = ve.value(ve.length() - 27);
        v7 = ve.value(ve.length() - 26);
        v8 = ve.value(ve.length() - 25);
        v9 = ve.value(ve.length() - 24);
        v10 = ve.value(ve.length() - 23);
        v11 = ve.value(ve.length() - 22);
        v12 = ve.value(ve.length() - 21);
        v13 = ve.value(ve.length() - 20);
        v14 = ve.value(ve.length() - 19);
        v15 = ve.value(ve.length() - 18);
        v16 = ve.value(ve.length() - 17);
        v17 = ve.value(ve.length() - 16);
        v18 = ve.value(ve.length() - 15);
        v19 = ve.value(ve.length() - 14);
        v20 = ve.value(ve.length() - 13);
        v21 = ve.value(ve.length() - 12);
        v22 = ve.value(ve.length() - 11);
        v23 = ve.value(ve.length() - 10);
        v24 = ve.value(ve.length() - 9);
        v25 = ve.value(ve.length() - 8);
        v26 = ve.value(ve.length() - 7);
        v27 = ve.value(ve.length() - 6);
        v28 = ve.value(ve.length() - 5);
        v29 = ve.value(ve.length() - 4);
        v30 = ve.value(ve.length() - 3);
        v31 = ve.value(ve.length() - 2);
        v32 = ve.value(ve.length() - 1);
        int ve1 = v1.toInt(&ok, 10);
        int ve2 = v2.toInt(&ok, 10);
        int ve3 = v3.toInt(&ok, 10);
        int ve4 = v4.toInt(&ok, 10);
        int ve5 = v5.toInt(&ok, 10);
        int ve6 = v6.toInt(&ok, 10);
        int ve7 = v7.toInt(&ok, 10);
        int ve8 = v8.toInt(&ok, 10);
        int ve9 = v9.toInt(&ok, 10);
        int ve10 = v10.toInt(&ok, 10);
        int ve11 = v11.toInt(&ok, 10);
        int ve12 = v12.toInt(&ok, 10);
        int ve13 = v13.toInt(&ok, 10);
        int ve14 = v14.toInt(&ok, 10);
        int ve15 = v15.toInt(&ok, 10);
        int ve16 = v16.toInt(&ok, 10);
        int ve17 = v17.toInt(&ok, 10);
        int ve18 = v18.toInt(&ok, 10);
        int ve19 = v19.toInt(&ok, 10);
        int ve20 = v20.toInt(&ok, 10);
        int ve21 = v21.toInt(&ok, 10);
        int ve22 = v22.toInt(&ok, 10);
        int ve23 = v23.toInt(&ok, 10);
        int ve24 = v24.toInt(&ok, 10);
        int ve25 = v25.toInt(&ok, 10);
        int ve26 = v26.toInt(&ok, 10);
        int ve27 = v27.toInt(&ok, 10);
        int ve28 = v28.toInt(&ok, 10);
        int ve29 = v29.toInt(&ok, 10);
        int ve30 = v30.toInt(&ok, 10);
        int ve31 = v31.toInt(&ok, 10);
        int ve32 = v32.toInt(&ok, 10);
        if(ident.toInt(&ok,10) == 99999)
            valor = 0;
        else
            valor = ident.toInt(&ok, 10) + 1;
        if(ok){
            unsigned short val1 = (unsigned short) value1;
            unsigned short val2 = (unsigned short) value2;
            unsigned short val3 = (unsigned short) value3;
            unsigned short val4 = (unsigned short) value4;
            unsigned short val5 = (unsigned short) value5;
            unsigned short val6 = (unsigned short) value6;
            unsigned short val7 = (unsigned short) value7;
            unsigned short val8 = (unsigned short) value8;
            unsigned short val9 = (unsigned short) value9;
            unsigned short val10 = (unsigned short) value10;
            unsigned short val11 = (unsigned short) value11;
            unsigned short val12 = (unsigned short) value12;
            unsigned short val13 = (unsigned short) value13;
            unsigned short val14 = (unsigned short) value14;
            unsigned short val15 = (unsigned short) value15;
            unsigned short val16 = (unsigned short) value16;
            realLearnPre(cate, rad, val1, val2, val3, val4, val5, val6, val7, val8, val9, val10, val11, val12, val13, val14, val15, val16, ve1, ve2, ve3, ve4, ve5, ve6, ve7, ve8, ve9, ve10, ve11, ve12, ve13, ve14, ve15, ve16, ve17, ve18, ve19, ve20, ve21, ve22, ve23, ve24, ve25, ve26, ve27, ve28, ve29, ve30, ve31, ve32);
        }
        item++;
    }
    file.close();
}

void MainWindow::realLearnPre(int ca, int ra, unsigned short val1, unsigned short val2, unsigned short val3, unsigned short val4, unsigned short val5, unsigned short val6, unsigned short val7, unsigned short val8, unsigned short val9, unsigned short val10, unsigned short val11, unsigned short val12, unsigned short val13, unsigned short val14, unsigned short val15, unsigned short val16, int v1, int v2, int v3, int v4, int v5, int v6, int v7, int v8, int v9, int v10, int v11, int v12, int v13, int v14, int v15, int v16, int v17, int v18, int v19, int v20, int v21, int v22, int v23, int v24, int v25, int v26, int v27, int v28, int v29, int v30, int v31, int v32){

    unsigned char ptr = (*(neuralSenses[SIGHT].ptr))++;
    if(ptr > sizeNet.numNeuron){
        showWarning("Red no dimensionable","No es posible aprender mas Patrones\nERROR:Numero de neuronas Agotadas");
        return;
    }
    neuralSenses[SIGHT].vectorFlags[ptr*SIZE_FLAGS+KNW] = 1;
    neuralSenses[SIGHT].vectorFlags[ptr*SIZE_FLAGS+CAT] = (unsigned int)ca;
    neuralSenses[SIGHT].vectorFlags[ptr*SIZE_FLAGS+RAT] = (unsigned int)ra;
    learnBinaryCharacteristicPre(SIGHT, ptr, val1, val2, val3, val4, val5, val6, val7, val8, val9, val10, val11, val12, val13, val14, val15, val16);
    neuralSenses[SIGHT].vectorNeuron[ptr*32+0] = v1;
    neuralSenses[SIGHT].vectorNeuron[ptr*32+1] = v2;
    neuralSenses[SIGHT].vectorNeuron[ptr*32+2] = v3;
    neuralSenses[SIGHT].vectorNeuron[ptr*32+3] = v4;
    neuralSenses[SIGHT].vectorNeuron[ptr*32+4] = v5;
    neuralSenses[SIGHT].vectorNeuron[ptr*32+5] = v6;
    neuralSenses[SIGHT].vectorNeuron[ptr*32+6] = v7;
    neuralSenses[SIGHT].vectorNeuron[ptr*32+7] = v8;
    neuralSenses[SIGHT].vectorNeuron[ptr*32+8] = v9;
    neuralSenses[SIGHT].vectorNeuron[ptr*32+9] = v10;
    neuralSenses[SIGHT].vectorNeuron[ptr*32+10] = v11;
    neuralSenses[SIGHT].vectorNeuron[ptr*32+11] = v12;
    neuralSenses[SIGHT].vectorNeuron[ptr*32+12] = v13;
    neuralSenses[SIGHT].vectorNeuron[ptr*32+13] = v14;
    neuralSenses[SIGHT].vectorNeuron[ptr*32+14] = v15;
    neuralSenses[SIGHT].vectorNeuron[ptr*32+15] = v16;
    neuralSenses[SIGHT].vectorNeuron[ptr*32+16] = v17;
    neuralSenses[SIGHT].vectorNeuron[ptr*32+17] = v18;
    neuralSenses[SIGHT].vectorNeuron[ptr*32+18] = v19;
    neuralSenses[SIGHT].vectorNeuron[ptr*32+19] = v20;
    neuralSenses[SIGHT].vectorNeuron[ptr*32+20] = v21;
    neuralSenses[SIGHT].vectorNeuron[ptr*32+21] = v22;
    neuralSenses[SIGHT].vectorNeuron[ptr*32+22] = v23;
    neuralSenses[SIGHT].vectorNeuron[ptr*32+23] = v24;
    neuralSenses[SIGHT].vectorNeuron[ptr*32+24] = v25;
    neuralSenses[SIGHT].vectorNeuron[ptr*32+25] = v26;
    neuralSenses[SIGHT].vectorNeuron[ptr*32+26] = v27;
    neuralSenses[SIGHT].vectorNeuron[ptr*32+27] = v28;
    neuralSenses[SIGHT].vectorNeuron[ptr*32+28] = v29;
    neuralSenses[SIGHT].vectorNeuron[ptr*32+29] = v30;
    neuralSenses[SIGHT].vectorNeuron[ptr*32+30] = v31;
    neuralSenses[SIGHT].vectorNeuron[ptr*32+31] = v32;
}

void MainWindow::learnBinaryCharacteristicPre(senses sense,int ptr, unsigned short val1, unsigned short val2, unsigned short val3, unsigned short val4, unsigned short val5, unsigned short val6, unsigned short val7, unsigned short val8, unsigned short val9, unsigned short val10, unsigned short val11, unsigned short val12, unsigned short val13, unsigned short val14, unsigned short val15, unsigned short val16){

    neuralSenses[sense].binaryCharacteristic[ptr*16+0] = val1;
    neuralSenses[sense].binaryCharacteristic[ptr*16+1] = val2;
    neuralSenses[sense].binaryCharacteristic[ptr*16+2] = val3;
    neuralSenses[sense].binaryCharacteristic[ptr*16+3] = val4;
    neuralSenses[sense].binaryCharacteristic[ptr*16+4] = val5;
    neuralSenses[sense].binaryCharacteristic[ptr*16+5] = val6;
    neuralSenses[sense].binaryCharacteristic[ptr*16+6] = val7;
    neuralSenses[sense].binaryCharacteristic[ptr*16+7] = val8;
    neuralSenses[sense].binaryCharacteristic[ptr*16+8] = val9;
    neuralSenses[sense].binaryCharacteristic[ptr*16+9] = val10;
    neuralSenses[sense].binaryCharacteristic[ptr*16+10] = val11;
    neuralSenses[sense].binaryCharacteristic[ptr*16+11] = val12;
    neuralSenses[sense].binaryCharacteristic[ptr*16+12] = val13;
    neuralSenses[sense].binaryCharacteristic[ptr*16+13] = val14;
    neuralSenses[sense].binaryCharacteristic[ptr*16+14] = val15;
    neuralSenses[sense].binaryCharacteristic[ptr*16+15] = val16;
}


////mudar esta funcion////
void MainWindow::paintBinarySyllab(int ptr){
    std::cout<<"MainWindow::paintBinarySyllab"<<endl;

    unsigned short displacement = 8 * sizeof (unsigned short) -1;
    unsigned short mask = 1 << displacement;
    freeMemory->freeGenericPtr(image);
    //freeGenericPtr(image);
    image = new QImage(QSize(230,230), QImage::Format_MonoLSB);
    image->fill(Qt::color1);
    QPainter paint;
    paint.begin(image);
    QPen pen(QColor(Qt::color1));
    paint.setPen(pen);
    paint.setBrush(QBrush(QColor(Qt::color1), Qt::SolidPattern));
    QString text = word2;
    paint.drawText(QRect(1,1,80,50),text);
    for (int i = 0; i < 16; i++){
        unsigned short value = neuralSenses[SIGHT].binaryCharacteristic[ptr * 16 + i];
        for(unsigned short j = 0; j <= displacement; j++){
            if(value &mask)
                paint.drawRect(QRect((15-j)*10,50+i*10,8,8));//(x,y) x columnas y filas
            value <<= 1;
        }
    }
    paint.end();
    ViewFinder &view = ViewFinder::getInstance(this);
    view.showBinaryCharacteristic(image);
}

void MainWindow::fillTable(){
    std::cout<<"MainWindow::fillTable"<<endl;

    if(returnID(word2) != 999){
        cout<<returnCategory(word2)<<endl;
        int numNeuron = *(neuralSenses[SIGHT].ptr);
        int si = 0;
        for(int id=0; id < numNeuron; id++){
            if(neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[id]-1)*9+CAT] == (unsigned int)returnCategory(word2)){
                si = 1;
            }
            for(int i=0; i<numNeuron;i++){
                if(neuralSenses[SIGHT].vectorFlags[(neuralSenses[SIGHT].ptr[id]+i)*9+CAT] == (unsigned int)returnCategory(word2)){
                    si = 1;
                }
            }
        }
        ui->pushButtonImage->setEnabled(true);
        if(si == 1){
            dialogTableWord = new DialogTableWord(0,neuralSenses,&sizeNet,word2);
            dialogTableWord->setWindowModality(Qt::WindowModal);
            this->setVisible(false);
            if(dialogTableWord->exec() == QDialog::Rejected)
                this->setVisible(true);
            freeMemory->freeGenericPtr(dialogTableWord);
            //freeGenericPtr(dialogTableWord);
            dialogTableWord = NULL;
            si = 0;
        }
    }
}

void MainWindow::orderProtocol(){
    std::cout<<"MainWindow::orderProtocol"<<endl;

    ui->pushButton_teachClack->setEnabled(false);
    countNetwork->clackPointer[kNeuron] = kNeuron;
    orderNetwork->order[kNeuron] = 1;
    orderNetwork->bumPointer[kNeuron] = 1;
    kAux = kNeuron;
    kNeuron = 1;
    orderNeuron = 1;
    ui->textBrowser->setText("CLACK");
}

void MainWindow::paintCount(int times){
    int ptr;
    /*word_count.foward =NULL;
    word_count.back =NULL;*/

    for(int a=0; a<times; a++){
        sumQueue->enqueue(word_count, returnCategory(ui->InputWordCount->text()));
    }
    IdForm = 2;
    ptr = obtainID(getNumberNeurons(), returnCategory(ui->InputWordCount->text()));
    showPanelThinking(SIGHT,ptr,returnCategory(ui->InputWordCount->text()),word_count, id_queue,IdForm);
}




