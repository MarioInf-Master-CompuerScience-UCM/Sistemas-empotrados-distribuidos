#include <string>
#include <vector>
#include <sstream>
#include <iostream>
using namespace std;

class Message{

    public:
        enum class TypeMessage{
            HELLO,
            EMERGENCY,
            ACK_EMERGENCY,
            UPDATE,
            UNKNOWN
        };

    private:
        string __allContent = "";
        string __source = "";
        TypeMessage __type = TypeMessage::UNKNOWN;
        string __data = "";

        const string helloType = "HELLO";
        const string emergencyType = "EMERGENCY";
        const string ackEmergencyType = "ACK EMERGENCY";
        const string updateType = "UPDATE";
        const string unknowType = "UNKNOWN";

        const string separator = "|";

    public:

        //**********************************************
        //  CONSTRUCTORES
        //**********************************************
        Message(){
            this->setAllContent("");
            this->setSource("");
            this->setData("");
            this->setType(TypeMessage::UNKNOWN);
        }

        Message(string msg){
            vector<string> listElements;
            string segment;
            stringstream test(msg);
            while(getline(test, segment, '|')){
                listElements.push_back(segment);
            }

            if(listElements.size()==3){
                this->setSource(listElements[0]);
                this->setType(checkType(listElements[1]));
                this->setData(listElements[2]);
                this->setAllContent(listElements[0] + this->separator + listElements[1] + this->separator + listElements[2]);
            }else{
                this->setAllContent(msg);
                this->setSource("");
                this->setData("");    
                this->setType(TypeMessage::UNKNOWN);          
            }
        }

        Message(string source, Message::TypeMessage type, string data){
            this->setSource(source);
            this->setType(type);
            this->setData(data);
            this->setAllContent(source + this->separator + this->selectTypeString(type) + this->separator + data);
        }
        

        //**********************************************
        //  MÉTODOS DESARROLLADOS
        //**********************************************
        string selectTypeString(Message::TypeMessage type);

        Message::TypeMessage checkType(string type);

        string structuredInfo();


        //**********************************************
        //  MÉTODOS DE ACCESO A LAS VARIABLES PRIVADAS
        //**********************************************
        string getAllContent();
        void setAllContent(string obj);
        string getSource();
        void setSource(string obj);
        TypeMessage getType();
        void setType(TypeMessage obj);
        string getData();
        void setData(string obj);


};
