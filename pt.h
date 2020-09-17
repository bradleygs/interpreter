/*
 * pt.h
 */

#ifndef PT_H_
#define PT_H_

#include <iostream>
#include <vector>
#include <map>
#include <string>

using std::vector;
using std::map;
using std::string;
using std::cin;
using std::cout;
using std::cerr;
using std::endl;

#include "parse.h"
#include "lex.h"
#include "val.h"
//good5, hard2
class Pt {
protected:
	int	linenum;
	Pt	*left;
	Pt	*right;

public:
	Pt(int linenum, Pt *l = 0, Pt *r = 0)
		: linenum(linenum), left(l), right(r) {}

	virtual ~Pt() {
		delete left;
		delete right;
	}

  int maxDepth(Pt *node) {
    if (node == NULL) 
      return 0;
      int lDepth = maxDepth(node->left);
      int rDepth = maxDepth(node->right);
      if (lDepth > rDepth) 
        return lDepth + 1;
      else 
        return rDepth + 1;
  }
    void postOrder(Pt *node) {
        if (node == NULL) 
            return;
        postOrder(node->left);
        postOrder(node->right);
        cout << node->GetLineNumber() << endl;
    }

	int GetLineNumber() const { return linenum; }

    virtual Value Eval(map<string,Value>& symbols, Value* saved = NULL) = 0;
    

    virtual bool isPlus() const { return false; }
    
    int plusCount() const {
        int pCount = 0;
        if (left) 
            pCount += left->plusCount();
        
        if (right) 
            pCount += right->plusCount();
        
        if (isPlus()) {
            pCount++;
        }
        return pCount;
    }
            
    
    virtual bool isEq() const { return false; }
    int eqCount() const {
        int eCount = 0;
        if (left) 
            eCount += left->eqCount();
        if (right)
            eCount += right->eqCount();
        if (isEq()) {
            eCount++;
        }
        return eCount;
    }
	virtual bool isIdent() const { return false; }
	virtual bool isVar() const { return false; }
	virtual string getId() const { return ""; }
};

class StmtList : public Pt {
public:
	StmtList(Pt *l, Pt *r) : Pt(0, l, r) {}
    Value Eval(map<string,Value>& symbols, Value* saved = NULL) {
        left->Eval(symbols);
        if (right) 
            right->Eval(symbols);
        return Value();
    }
};

class Print : public Pt {
public:
	Print(int line, Pt *e) : Pt(line, e) {}
    Value Eval(map<string,Value>& symbols, Value* saved = NULL) {
        try {
        cout << left->Eval(symbols) << flush;
        } catch (const char* e) {
            cout << e << endl;
        }
        return Value();
    }
};

class Println : public Pt {
public:
    Println(int line, Pt *e) : Pt(line, e) {}
    Value Eval(map<string,Value>& symbols, Value* saved = NULL) {
        try {
        cout << left->Eval(symbols) << endl;
        } catch (const char* e) {
            cout << e << endl;
        }
        return Value();
    }
};

class Repeat : public Pt {
public:
    Repeat(int line, Pt *e, Pt *s) : Pt(line, e, s) {}
    Value Eval(map<string, Value>& symbols, Value* saved = NULL) {
        Value expr = left->Eval(symbols);
        int count = 0;
        int max = 0;
        try {
            if (expr.IsStr()) {
                cout << "RUNTIME ERROR at " + to_string(left->GetLineNumber()+1)+": control expression not an integer" << endl; 
            	return Value();
            }
            if (left->isIdent()) {
                max = symbols[left->getId()].GetInt();
            }
            else {
                max = expr.GetInt();
            }
            while (count++ < max) {
                right->Eval(symbols);
            }
        } catch (string e) {
            cout << e << endl;
        }
        return Value();
    }

}; 

class Assign : public Pt { //good5 had to use professors expr
    string id;
public:
    Assign(int line, string id, Pt *l) : Pt(line,l), id(id) {}
    Value Eval(map<string, Value>& symbols, Value* saved = NULL) {
        auto l = left->Eval(symbols);
        return symbols[id] = l;
    }
    bool isEq() const { return true;}
};

class PlusExpr : public Pt {
public:
    PlusExpr(int line, Pt *l, Pt *r) : Pt(line, l, r) {}
    Value Eval(map<string, Value>& symbols, Value* saved = NULL) {
        try {
        return left->Eval(symbols) + right->Eval(symbols);	
        } catch (const char* e) {
            throw std::string("RUNTIME ERROR at " + to_string(left->GetLineNumber()+1) + ": Type error on addition");
        }
        return Value();
    }
    bool isPlus() const { return true;}
};

class MinusExpr : public Pt {
public:
    MinusExpr(int line, Pt *l, Pt *r) : Pt(line, l, r) {}
    Value Eval(map<string, Value>& symbols, Value* saved = NULL) {
        try {
        return left->Eval(symbols) - right->Eval(symbols);
        } catch (const char* e) {
            throw std::string("RUNTIME ERROR at " + to_string(left->GetLineNumber() + 1) + ": Type error on subtraction");
        }
        return Value();
    }
};

class TimesExpr : public Pt {
public:
    TimesExpr(int line, Pt *l, Pt *r) : Pt(line, l, r) {}
    Value Eval(map<string, Value>& symbols, Value* saved = NULL) {
        try {
        	return left->Eval(symbols) * right->Eval(symbols);
        } catch (const std::string& e) {
            throw std::string(to_string(left->GetLineNumber()+1) + ": Type error on multiplication");
        }
    }
};

class DivideExpr : public Pt {
public:
    DivideExpr(int line, Pt *l, Pt *r) : Pt(line, l, r) {}
    Value Eval(map<string, Value>& symbols, Value* saved = NULL) {
        return left->Eval(symbols)/right->Eval(symbols); 
    }
};

class IConst : public Pt {
	int val;

public:
	IConst(Tok& t) : Pt(t.GetLinenum()) {
		val = stoi(t.GetLexeme());
	}
    Value Eval(map<string, Value>& symbols, Value* saved = NULL) {
        return Value(val);
    }
};

class SConst : public Pt {
	string val;

public:
	SConst(Tok& t) : Pt(t.GetLinenum()) {
		val = t.GetLexeme();
	}
    Value Eval(map<string, Value>& symbols, Value* saved = NULL) {
        return Value(val);
    }
};

class Ident : public Pt {
  string val;
    int line;

public:
  Ident(Tok& t) : Pt(t.GetLinenum()) {
    val = t.GetLexeme();
      line = t.GetLinenum();
  }
    bool isIdent() const { return true; }
    bool isVar() const { return true; }
    string getId() const { return val; }
    Value Eval(map<string, Value>& symbols, Value* saved = NULL) {
       if (symbols.find(val) != symbols.end()) 
            return symbols[val];
        throw std::string(to_string(line+1) +  ": Variable used but not assigned");
        return Value(val); 
    }
};

#endif /* PT_H_ */