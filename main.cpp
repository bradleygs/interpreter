/*
 * main.cpp
 *
 * CS280 - Spring 2020
 * Lexical Analyzer test engine
*/
//main, parse, pt
//print (3/2);

#include <fstream>
using std::ifstream;

#include <map>
using std::map;

#include "lex.h"
#include "parse.h"
#include "pt.h"
#include "val.h"
using std::cin;
using std::cout;
using std::cerr;
using std::endl;

//static map<string, Value> symbols;

int main(int argc, char *argv[]) {
	int lineNumber = 0;


	istream *in = &cin;
	ifstream file;

    if (argc == 2) {
        file.open(argv[1]);
        if (file.is_open()) {
          in = &file;	
        } else {
          cout << "CANNOT OPEN " << argv[1] << endl;
          return 0;
        }
    }

    if (argc > 2) {
       	cout << "ONLY ONE FILE NAME ALLOWED" << endl;
        return 0;
    }
    Pt *root = Prog(*in, lineNumber);
    map<string, Value> symbols;
    if (root != 0) {
        try {
      	root->Eval(symbols);
        }
        catch (std::string& e) {
            cout << "RUNTIME ERROR at " << e << endl;
        }
        catch (char const* e) {
            cout << "RUNTIME ERROR at " << e << endl;
        }
    }
    return 0;
}








//easy1 print 3;
//easy2 print 2+2;
//easy3 println 5*3;
// easy4:
//x = 5;
//println x;
//easy5 (3/2);


