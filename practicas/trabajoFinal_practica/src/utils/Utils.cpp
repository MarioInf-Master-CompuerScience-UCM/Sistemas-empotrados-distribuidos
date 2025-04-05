#include "Utils.h"

     void Utils::print( string message ){
          cout <<  "SYSTEM..: " << getDate() << message << endl;
     }

     void Utils::printSuccess( string message ){
          cout << "\033[1;32m" << getDate() << "SYSTEM SUCCESS..: " << message << "\033[0m";
     }

     void Utils::printWarning( string message ){
          cout << "\033[1;33m" << getDate() << "SYSTEM WARNING..: " << message << "\033[0m";
     }

     void Utils::printEvent( string message ){
          cout << "\033[1;35m" << getDate() << "SYSTEM EVENT..: " << message << "\033[0m";
     }

     void Utils::printError( string message ){
          cout << "\033[1;31m" << getDate() << "SYSTEM ERROR..: " << message << "\033[0m";
     }




     string Utils::getDate() {
          time_t     now = time(0);
          struct tm  tstruct;
          char       buf[80];
          
          tstruct = *localtime(&now);
          strftime(buf, sizeof(buf), "%Y/%m/%d %X -> ", &tstruct);
          return buf;
     }



