#include <string>
#include <iostream>
using namespace std;

class Utils{

    public:
        static void print( string message );
        static void printSuccess( string message );
        static void printEvent( string message );
        static void printWarning( string message );
        static void printError( string message );

        static string getDate();
};
