/*
 * CLI.cpp
 *
 *  Author: Nimrod Gabbay 
 */

#include "CLI.h"
CLI::CLI(DefaultIO* dio) {
    this->dio=dio;
    this->ch=new CommandHandler();
    this->commands.push_back(new UploadFileCommand(this->dio,this->ch));
    this->commands.push_back(new AlgorithemSettingsCommand(this->dio,this->ch));
    this->commands.push_back(new AnomalyDetectionCommand(this->dio,this->ch));
    this->commands.push_back(new DisplayResultCommand(this->dio,this->ch));
    this->commands.push_back(new UploadAnomaliesCommand(this->dio,this->ch));
    this->commands.push_back(new ExitCommand(this->dio,this->ch));
}

void CLI::start(){
    string menu;
    menu.append("Welcome to the Anomaly Detection Server.\n");
    menu.append("Please choose an option:\n");
    this->dio->writeData(menu);
    for(Command* c: commands)
        this->dio->writeData(c->getDescription()); 

    string s;
    s=this->dio->readData();
    int opt= stoi(s);  
    switch(opt){
        case 1: 
            commands[0]->execute();
            start();
            break;
        case 2:
            commands[1]->execute();
            start();
            break;
        case 3:
            commands[2]->execute();
            start();
            break;
        case 4:
            commands[3]->execute(); 
            start();
            break;
        case 5: 
            commands[4]->execute();
            start();
            break;
        case 6: 
            commands[5]->execute(); 
            break;
    }
}


CLI::~CLI() {
    for(int i=0;i<this->commands.size();i++){
        delete commands[i];
    }
    delete this->ch;    
}


