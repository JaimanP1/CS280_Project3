#include "parserInterp.h"

map<string, bool> defVar;
map<string, Token> SymTable;

map<string, Value> TempsResults; // Container of temporary locations of Value objects for results of expressions, variables values and constants
queue<Value> *ValQue;            // declare a pointer variable to a queue of Value objects


namespace Parser
{
    bool pushed_back = false;
    LexItem pushed_token;

    static LexItem GetNextToken(istream &in, int &line)
    {
        if (pushed_back)
        {
            pushed_back = false;
            return pushed_token;
        }
        return getNextToken(in, line);
    }

    static void PushBackToken(LexItem &t)
    {
        if (pushed_back)
        {
            abort();
        }
        pushed_back = true;
        pushed_token = t;
    }

}

static int error_count = 0;

int ErrCount()
{
    return error_count;
}

void ParseError(int line, string msg)
{
    ++error_count;
    cout << line << ": " << msg << endl;
}

bool IdentList(istream &in, int &line);

// Factor := ident | iconst | rconst | sconst | (Expr)
//missing case2, compare with given code and move default/error cases to top
bool factor_flag = false;
bool Factor(istream &in, int &line, int sign, Value &retVal){
    LexItem tok = Parser::GetNextToken(in, line);

    switch(tok.GetToken()){
        default:
        {
            ParseError(line, "idk");
            return false;
        }
            
        case ERR:
        {
            ParseError(line, "Unrecognized Input Patter");
            cout << "(" << tok.GetLexeme() << ")" << endl;
            return false;
        }
        
        case LPAREN:
        {
            bool expr = Expr(in, line, retVal);
            if(Parser::GetNextToken(in, line) == RPAREN){
                return true;
            }
            if(!expr){
                ParseError(line, "Missing expression after (");
                return false;
            }
            else{
                Parser::PushBackToken(tok);
                ParseError(line, "Missing ) after expression");
                return false;
            }
        }
        
        case RCONST:
        {
            if(sign == -1){
                retVal = Value(-stod(tok.GetLexeme()));
                return true;
            }
            else{
                retVal = Value(stod(tok.GetLexeme()));
                return true;
            }
        }    

        case ICONST:
        {
            if(sign != -1){
                retVal = Value(stoi(tok.GetLexeme()));
                return true;
            }
            else{
                retVal = Value(-stoi(tok.GetLexeme()));
                return true;
            }
        }
        
        case SCONST:
        {
            if (sign != 0){
                ParseError(line, "Illegal Operation on Character Type");
                return false;
            }
            else{
                retVal = Value(tok.GetLexeme());
                retVal.SetstrLen(tok.GetLexeme().length());
                return true;
            }
        }

        case IDENT:
        {
            string ident = tok.GetLexeme();

            if (!(defVar.find(ident) -> second)){
                ParseError(line, "not defined");
                return false;
            }

            switch(SymTable[ident]){
                
                case INTEGER:
                {
                    if(sign != 0){
                        retVal = Value(sign * TempsResults[ident].GetInt());
                        return false;
                    }
                    else{
                        retVal = TempsResults[ident];
                        return true;
                    }
                }
                
                case REAL:
                {
                    if(sign == 0){
                        retVal = TempsResults[ident];
                    }
                    else {
                        retVal = Value(sign*TempsResults[ident].GetReal());
                    }
                }
                
                case CHARACTER:
                {
                    if(sign == 0){
                        retVal = TempsResults[ident];
                        return true;
                    }
                    else{
                        ParseError(line, "Illegal Operation on Character Type");
                        return false;
                    }
                }
                    
                default:
                {
                    ParseError(line, "idk");
                    return false;
                }
            }
        }
            
        /*
        default:
            ParseError(line, "idk");
            return false;
        */
    }
    return false;
}

// SFactor = Sign Factor | Factor
//no errors
bool SFactor(istream &in, int &line, Value &retVal)
{
    LexItem t = Parser::GetNextToken(in, line);

    // cout << "in SFactor: " << t.GetLexeme() << endl;
    bool status;
    int sign = 0;
    if (t == MINUS)
    {
        sign = -1;
    }
    else if (t == PLUS)
    {
        sign = 1;
    }
    else
    {
        Parser::PushBackToken(t);
    }

    status = Factor(in, line, sign, retVal);
    return status;
}

// TermExpr ::= SFactor { ** SFactor }
//missing case6, compare with given code and move default/error cases to top
bool TermExpr(istream &in, int &line, Value &retVal){

    bool t1 = SFactor(in, line, retVal);
    LexItem tok;

    if (!t1)
    {
        return false;
    }

    tok = Parser::GetNextToken(in, line);
    if (tok == POW){
        Value val;
        t1 = TermExpr(in, line, val);
        if (!t1){
            ParseError(line, "Missing exponent operand");
            return false;
        }
        retVal = retVal.Power(val);       
    }

    if (tok == ERR){
        ParseError(line, "Unrecognized Input Pattern");
        cout << "(" << tok.GetLexeme() << ")" << endl;
        return false;
    }

    Parser::PushBackToken(tok);
    return true;
}

//MultExpr ::= TermExpr { ( * | / ) TermExpr }
//no errors
bool MultExpr(istream& in, int& line, Value &retVal) {
	
	bool t1 = TermExpr(in, line, retVal);
	LexItem tok;
	
	if( !t1 ) {
		return false;
	}
	
	tok	= Parser::GetNextToken(in, line);
	if(tok.GetToken() == ERR){
			ParseError(line, "Unrecognized Input Pattern");
			cout << "(" << tok.GetLexeme() << ")" << endl;
			return false;
	}
	while ( tok == MULT || tok == DIV  )
	{
		Value val;
        t1 = TermExpr(in, line, val);
		
		if( !t1 ) {
			ParseError(line, "Missing operand after operator");
			return false;
		}

        if(tok == DIV){
            if(val.IsReal()){
                if(val.GetReal() == 0.0){
                    ParseError(line, "Division by Zero");
                    return false;
                }
                else{
                    retVal = retVal/val;
                }
            }
            if(val.IsInt()){
                if(val.GetInt() == 0){
                    ParseError(line, "Divisoin by Zero");
                    return false;
                }
                else{
                    retVal = retVal/val;
                }
            }
        }

        if(tok == MULT){
            retVal = retVal * val;
        }
		
		tok	= Parser::GetNextToken(in, line);
		if(tok.GetToken() == ERR){
			ParseError(line, "Unrecognized Input Pattern");
			cout << "(" << tok.GetLexeme() << ")" << endl;
			return false;
		}
		
	}
	Parser::PushBackToken(tok);
	return true;
}//End of MultExpr

//Expr ::= MultExpr { ( + | - | // ) MultExpr }
//missing case12, check switch statement
bool Expr(istream& in, int& line, Value &retVal) {
	
	bool t1 = MultExpr(in, line, retVal);
	LexItem tok;
	
	if( !t1 ) {
		return false;
	}
	
	tok = Parser::GetNextToken(in, line);
	if(tok.GetToken() == ERR){
		ParseError(line, "Unrecognized Input Pattern");
		cout << "(" << tok.GetLexeme() << ")" << endl;
		return false;
	}
	while ( tok == PLUS || tok == MINUS || tok == CAT) 
	{
		Value val;
        t1 = MultExpr(in, line, val);
		if( !t1 ) 
		{
			ParseError(line, "Missing operand after operator");
			return false;
		}
        switch(tok.GetToken()){
            case PLUS:{
                retVal = retVal + val;
            }
            case MINUS:{
                retVal = retVal - val;
            }
            case CAT:{
                retVal = retVal + val;
            }
            default:{
                break;
            }
        }
		
		tok = Parser::GetNextToken(in, line);
		if(tok.GetToken() == ERR){
			ParseError(line, "Unrecognized Input Pattern");
			cout << "(" << tok.GetLexeme() << ")" << endl;
			return false;
		}		
		
	}
	Parser::PushBackToken(tok);
	return true;
}//End of Expr

