/*
 * parse.cpp
 */

#include "parse.h"

namespace Parser {
bool pushed_back = false;
Tok	pushed_token;

static Tok GetNextToken(istream& in, int& line) {
	if( pushed_back ) {
		pushed_back = false;
		return pushed_token;
	}
	return getNextToken(in, line);
}

static void PushBackToken(Tok& t) {
	if( pushed_back ) {
		abort();
	}
	pushed_back = true;
	pushed_token = t;
}

}

static int error_count = 0;

void
ParseError(int line, string msg) {
	++error_count;
	cout << line << ": " << msg << endl;
}

Pt *Prog(istream& in, int& line) {
	Pt *sl = Sl(in, line);

	if( sl == 0 )
		ParseError(line, "No statements in program");

	if( error_count )
		return 0;

	return sl;
}

// Sl is a Statement followed by a Statement List
Pt *Sl(istream& in, int& line) {
	Pt *s = Stmt(in, line);
	if( s == 0 )
		return 0;

	if( Parser::GetNextToken(in, line) != SC ) {
		ParseError(line-1, "Missing semicolon");
		delete s;
		return 0;
	}

	return new StmtList(s, Sl(in,line));
}

Pt *Stmt(istream& in, int& line) {
	Pt *s;
	Tok t = Parser::GetNextToken(in, line);
	switch( t.GetToken() ) {
	case PRINT:
		s = PrintStmt(in, line);
		break;
	case PRINTLN:
		s = PrintlnStmt(in, line);
		break;
    case REPEAT:
		s = RepeatStmt(in, line);
		break;
	case IDENT:
		Parser::PushBackToken(t);
		s = Expr(in, line);
		break;
	case END:
		Parser::PushBackToken(t);
		return 0;
	case DONE:
		return 0;
	default:
		ParseError(line, "Invalid statement");
        return 0;
	}
	return s;
} 

Pt *PrintStmt(istream& in, int& line) {
	int l = line;

	Pt *ex = Expr(in, line);
	if( ex == 0 ) {
		ParseError(line, "Missing expression after print");
        return 0;
        }

	return new Print(l, ex);
}

Pt *PrintlnStmt(istream& in, int& line) {
//// FIXME add code
	int l = line;
    
    Pt *ex = Expr(in, line);
    if(ex == 0) {
    	ParseError(line, "Missing expression after Println");
        return 0;
        }
        return new Println(l, ex);
}
        



Pt *RepeatStmt(istream& in, int& line) {
//// FIXME add code
    Pt *ex = Expr(in, line);
    if (ex == 0) {
        ParseError(line, "Missing expression after Repeat");
        return 0;
    }
    Tok t = Parser::GetNextToken(in, line);
    if (t.GetToken() != BEGIN) {
        ParseError(line, "Missing BEGIN in RepeatStmt");
        return 0;
    }
    Pt *s = Stmt(in, line);
    if (s == 0) {
        ParseError(line,"Missing statement(s) in Repeat");
        return 0;
    }
    Tok t2 = Parser::GetNextToken(in, line);
    if (t2 != END) {
        ParseError(line, "Missing END in RepeatStmt");
        return 0;
    }
    return new Repeat(line, ex, s);
}
Pt *Expr(istream& in, int& line) {
    int l = line;

    Pt *lhs = Sum(in, line);

    if( lhs == 0 ) {
        ParseError(l, "Missing expression");
        return 0;
    }

    Tok t = Parser::GetNextToken(in, line);
    if( t != EQ ) {
        // no equal sign so just pass up the lhs
        Parser::PushBackToken(t);
        return lhs;
    }

    // for a valid assignment, lhs has to be an identifier
    // also remember it's right associative...

    if( ! lhs->isIdent() ) {
        ParseError(line, "Missing identifier for assignment");
        return 0;
    }

    Pt *ex = Expr(in, line); // right assoc
    if( ex == 0 ) {
        ParseError(line, "Missing expression after =");
        return 0;
    }

    return new Assign(l, lhs->getId(), ex);
}
        




Pt *Sum(istream& in, int& line) {
	Pt *t1 = Prod(in, line);
	if( t1 == 0 ) {
		return 0;
	}

	while ( true ) {
		Tok t = Parser::GetNextToken(in, line);

		if( t != PLUS && t != MINUS ) {
			Parser::PushBackToken(t);
			return t1;
		}

		Pt *t2 = Prod(in, line);
		if( t2 == 0 ) {
			ParseError(line, "Missing expression after operator");
			return 0;
		}

		
        if( t == PLUS )
        	t1 = new PlusExpr(t.GetLinenum(), t1, t2);
        else
		 	t1 = new MinusExpr(t.GetLinenum(), t1, t2);
	}
}

Pt *Prod(istream& in, int& line) {
	Pt *t1 = Primary(in, line);
	if( t1 == 0 ) {
		return 0;
	}

	while ( true ) {
		Tok t = Parser::GetNextToken(in, line);

		if( t != STAR && t != SLASH ) {
			Parser::PushBackToken(t);
			return t1;
		}

		Pt *t2 = Primary(in, line);
		if( t2 == 0 ) {
			ParseError(line, "Missing expression after operator");
			return 0;
		}

		if( t == STAR )
		 	t1 = new TimesExpr(t.GetLinenum(), t1, t2);
		 else
		 	t1 = new DivideExpr(t.GetLinenum(), t1, t2);
	}
}

Pt *Primary(istream& in, int& line) { //work on this and get a better understanding, ask Nick (TA)
	Tok t = Parser::GetNextToken(in, line);
	if( t == IDENT ) {
		return new Ident(t);
    }
    else if (t == ICONST) {
        return new IConst(t);
    }
    else if (t == SCONST) {
        return new SConst(t);
    }
    else if (t == LPAREN) { 
        Pt *ex = Expr(in, line);
        if (ex == 0) {
            ParseError(line, "Missing expression after (");
            return 0;
        }
        if (Parser::GetNextToken(in, line) == RPAREN) {
            return ex;
        }
        ParseError(line, "Missing ) after expression");
        return 0;
    }
    ParseError(line, "Primary expected");
    return 0;
}
   

