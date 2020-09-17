#ifndef VALUE_H
#define VALUE_H

#include <iostream>
#include <string>
using namespace std;

enum ValType { VINT, VSTR, VERR };

class Value {
    ValType	T;
	int		I;
    string	S;
    
public:
    Value() : T(VERR), I(0) {}
    Value(int vi) : T(VINT), I(vi) {}
    Value(string vs) : T(VSTR), I(0), S(vs) {}
    
    ValType GetType() const { return T; }
    bool IsErr() const { return T == VERR; }
    bool IsInt() const { return T == VINT; }
    bool IsStr() const { return T == VSTR; }
    
    int GetInt() const { if( IsInt() ) return I; throw "RUNTIME ERROR: Value not an integer"; }
    string GetStr() const { if( IsStr() ) return S; throw "RUNTIME ERROR: Value not a string"; }
        
    // add op to this
    Value operator+(const Value& op) const {
        if (this->IsInt() && op.IsInt()) {
            return Value(this->GetInt() + op.GetInt());
        }
        if (this->IsStr() && op.IsStr()) {
            return Value(this->GetStr() + op.GetStr());
        }
        return Value();
    }
    
    // subtract op from this
	Value operator-(const Value& op) const {
    if (GetType() != op.GetType() || IsErr()) 
        return Value();
        
    if (this->IsInt() && op.IsInt()) 
        return Value(this->GetInt() - op.GetInt());
    
        string remover = op.GetStr();
        auto search = S.find(remover);
        string result = S;
        if (search != string::npos) {
            result.erase(search, remover.length());
        }
    return Value(result);
	}
    
    // multiply this by op
    Value operator*(const Value& op) const {
        string a = "";
    	if (this->IsInt() && op.IsInt()) {
        	return Value(this->GetInt() * op.GetInt());
   		}
        if (this->IsInt() && op.IsStr() && this->GetInt() >= 0) {
            for (int i = 0; i < this->GetInt(); i++)
                a += op.GetStr();
            return a;
        }
        if (this->IsStr() && op.IsInt()) {
            for (int i = 0; i < op.GetInt(); i++) 
                a += this->GetStr();
            return a;
        }
        throw std::string("Type error on multiplication");
    }
    
    // divide this by op
    Value operator/(const Value& op) const {
        if (this->IsInt() && op.IsInt()) {
            return Value(this->GetInt()/op.GetInt());
        }
        return Value();
    }
    
    friend ostream& operator<<(ostream& out, const Value& op) {
        if( op.IsInt() ) out << op.I;
        else if( op.IsStr() ) out << op.S;
        else out << "ERROR";
        return out;
    }
};

#endifaq