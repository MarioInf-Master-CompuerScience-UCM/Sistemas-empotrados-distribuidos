#include "Message.h"


    // Método selectTypeString
    //*********************************************************************
    string Message::selectTypeString(Message::TypeMessage type){
        switch (type){
            case Message::TypeMessage::HELLO :
                return this->helloType;
                break;

            case Message::TypeMessage::EMERGENCY :
                return this->emergencyType;
                break;

            case Message::TypeMessage::ACK_EMERGENCY :
                return this->emergencyType;
                break;

            case Message::TypeMessage::UPDATE :
                return this->emergencyType;
                break;
            
            default:
                return this->unknowType;
                break;
        }
    }


    // Método CheckTipe
    //*********************************************************************
    Message::TypeMessage Message::checkType(string type){
        if(type.compare(this->helloType)==0){
            return Message::TypeMessage::HELLO;

        }else if(type.compare(emergencyType)==0){
            return Message::TypeMessage::EMERGENCY;

        }else if(type.compare(ackEmergencyType)==0){
            return Message::TypeMessage::ACK_EMERGENCY;

        }else if(type.compare(updateType)==0){
            return Message::TypeMessage::UPDATE;
            
        }else{
            return Message::TypeMessage::UNKNOWN;
        }
    }


    // Método structuredInfo
    //*********************************************************************
    string Message::structuredInfo(){
        string result;

        result =    "Mensaje: (" + this->getAllContent() + ")" +
                    "\nFuente: " + this->getSource() +
                    "\nTipo: " + this->selectTypeString(this->getType()) + 
                    "\nInformación: " + this->getData() +
                    "\n";
        return result;
    }


    //**********************************************
    //  MÉTODOS DE ACCESO A LAS VARIABLES PRIVADAS
    //**********************************************
    string Message::getAllContent(){ return this->__allContent; }
    void Message::setAllContent(string obj){ this->__allContent = obj; }

    string Message::getSource(){ return this->__source; }
    void Message::setSource(string obj){ this->__source = obj; }

    Message::TypeMessage Message::getType(){ return this->__type; }
    void Message::setType(TypeMessage obj){ this->__type = obj; }

    string Message::getData(){ return this->__data; }
    void Message::setData(string obj){ this->__data = obj; }
