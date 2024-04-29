#include "parserInterp.h"

map<string, bool> defVar;
map<string, Token> SymTable;

map<string, Value> TempsResults; // Container of temporary locations of Value objects for results of expressions, variables values and constants
queue<Value> *ValQue;            // declare a pointer variable to a queue of Value objects

bool _log = false; ///what does this line do????????????????????

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

// Program is: Prog = PROGRAM IDENT {Decl} {Stmt} END PROGRAM IDENT
bool Prog(istream &in, int &line)
{
    bool dl = false, sl = false; 
    LexItem tok = Parser::GetNextToken(in, line);

    if (tok.GetToken() == PROGRAM)
    {
        tok = Parser::GetNextToken(in, line);
        if (tok.GetToken() == IDENT)
        {
            dl = Decl(in, line);
            if (!dl)
            {
                ParseError(line, "Incorrect Declaration in Program");
                return false;
            }
            sl = Stmt(in, line);
            if (!sl)
            {
                ParseError(line, "Incorrect Statement in program");
                return false;
            }
            tok = Parser::GetNextToken(in, line);

            if (tok.GetToken() == END)
            {
                tok = Parser::GetNextToken(in, line);

                if (tok.GetToken() == PROGRAM)
                {
                    tok = Parser::GetNextToken(in, line);

                    if (tok.GetToken() == IDENT)
                    {
                        cout << "(DONE)" << endl;
                        return true;
                    }
                    else
                    {
                        ParseError(line, "Missing Program Name");
                        return false;
                    }
                }
                else
                {
                    ParseError(line, "Missing PROGRAM at the End");
                    return false;
                }
            }
            else
            {
                ParseError(line, "Missing END of Program");
                return false;
            }
        }
        else
        {
            ParseError(line, "Missing Program name");
            return false;
        }
    }
    else if (tok.GetToken() == ERR)
    {
        ParseError(line, "Unrecognized Input Pattern");
        cout << "(" << tok.GetLexeme() << ")" << endl;
        return false;
    }

    else
    {
        ParseError(line, "Missing Program keyword");
        return false;
    }
}

// Decl ::= Type :: VarList
// Type ::= INTEGER | REAL | CHARARACTER [(LEN = ICONST)]
bool Decl(istream &in, int &line)
{
    bool status = false;
    LexItem tok;
    LexItem idtok; ///why do we need this line????????????????????
    int strlen = 1; ///do we need this line to call the new varlist method????????????????????

    LexItem t = Parser::GetNextToken(in, line);

    if (t == INTEGER || t == REAL || t == CHARACTER)
    {
        idtok = t;

        tok = Parser::GetNextToken(in, line);
        if (tok.GetToken() == DCOLON)
        {
            status = VarList(in, line, idtok, strlen);

            if (status)
            {
                status = Decl(in, line);
                if (!status)
                {
                    ParseError(line, "Declaration Syntactic Error.");
                    return false;
                }
                return status;
            }
            else
            {
                ParseError(line, "Missing Variables List.");
                return false;
            }
        }
        else if (t == CHARACTER && tok.GetToken() == LPAREN)
        {
            tok = Parser::GetNextToken(in, line);

            if (tok.GetToken() == LEN)
            {
                tok = Parser::GetNextToken(in, line);

                if (tok.GetToken() == ASSOP)
                {
                    tok = Parser::GetNextToken(in, line);

                    if (tok.GetToken() == ICONST)
                    {
                        strlen = stoi(tok.GetLexeme());

                        tok = Parser::GetNextToken(in, line);
                        if (tok.GetToken() == RPAREN)
                        {
                            tok = Parser::GetNextToken(in, line);
                            if (tok.GetToken() == DCOLON)
                            {
                                
                                status = VarList(in, line, idtok, strlen);

                                if (status)
                                {
                                    status = Decl(in, line);
                                    if (!status)
                                    {
                                        ParseError(line, "Declaration Syntactic Error.");
                                        return false;
                                    }
                                    return status;
                                }
                                else
                                {
                                    ParseError(line, "Missing Variables List.");
                                    return false;
                                }
                            }
                        }
                        else
                        {
                            ParseError(line, "Missing Right Parenthesis for String Length definition.");
                            return false;
                        }
                    }
                    else
                    {
                        ParseError(line, "Incorrect Initialization of a String Length");
                        return false;
                    }
                }
            }
        }
        else
        {
            ParseError(line, "Missing Double Colons");
            return false;
        }
    }

    Parser::PushBackToken(t);
    return true;
} // End of Decl

// Stmt ::= AssigStmt | BlockIfStmt | PrintStmt | SimpleIfStmt
bool Stmt(istream &in, int &line)
{
    bool status;

    LexItem t = Parser::GetNextToken(in, line);

    switch (t.GetToken())
    {

    case PRINT:
        status = PrintStmt(in, line);

        if (status)
            status = Stmt(in, line);
        break;

    case IF:
        status = BlockIfStmt(in, line);
        if (status)
            status = Stmt(in, line);
        break;

    case IDENT:
        Parser::PushBackToken(t);
        status = AssignStmt(in, line);
        if (status)
            status = Stmt(in, line);
        break;

    default:
        Parser::PushBackToken(t);
        return true;
    }

    return status;
} // End of Stmt

bool SimpleStmt(istream &in, int &line)
{
    bool status;

    LexItem t = Parser::GetNextToken(in, line);

    switch (t.GetToken())
    {

    case PRINT:
        status = PrintStmt(in, line);

        if (!status)
        {
            ParseError(line, "Incorrect Print Statement");
            return false;
        }
        cout << "Print statement in a Simple If statement." << endl;
        break;

    case IDENT:
        Parser::PushBackToken(t);
        status = AssignStmt(in, line);
        if (!status)
        {
            ParseError(line, "Incorrect Assignent Statement");
            return false;
        }
        // cout << "Assignment statement in a Simple If statement." << endl;

        break;

    default:
        Parser::PushBackToken(t);
        return true;
    }

    return status;
} // End of SimpleStmt

// VarList ::= Var [= Expr] {, Var [= Expr]}
bool VarList(istream &in, int &line, LexItem &idtok, int strlen)
{
    bool status = false, exprstatus = false;
    string identstr;
    Value retVal;

    LexItem tok = Parser::GetNextToken(in, line);
    if (tok == IDENT)
    {

        identstr = tok.GetLexeme();
        // cout << "this is the identstr " << identstr << endl;
        if (!(defVar.find(identstr)->second))
        {
            defVar[identstr] = false;
            // cout << "the result of this " << identstr << defVar[identstr] << endl;
            if (idtok == CHARACTER)
            {
                defVar[identstr] = true;
            }
        }
        else
        {
            ParseError(line, "Variable Redefinition");
            return false;
        }
        SymTable[identstr] = idtok.GetToken();
        TempsResults[identstr] = retVal;

        if (idtok == CHARACTER)
        {
            string str(strlen, ' ');
            TempsResults[identstr] = Value(str);
            TempsResults[identstr].SetstrLen(strlen);
        }
    }
    else
    {

        ParseError(line, "Missing Variable Name");
        return false;
    }

    tok = Parser::GetNextToken(in, line);
    if (tok == ASSOP)
    {

        exprstatus = Expr(in, line, retVal);
        if (!exprstatus)
        {
            ParseError(line, "Incorrect initialization for a variable.");
            return false;
        }

        if (idtok == INTEGER && retVal.IsInt() == false)
        {
            ParseError(line, "Illegal Mixed Type Operands  1 ");
            return false;
        }

        if (idtok == REAL)
        {
            if (retVal.IsInt())
            {
                // covert the retVal int to real
                retVal = Value(static_cast<double>(retVal.GetInt()));
            }
            else if (retVal.IsReal())
            {

                retVal = Value(retVal.GetReal());
            }
            else
            {
                ParseError(line, "Illegal Mixed Type Operands  [real]");
                

                return false;
            }
        }

        if (idtok == CHARACTER)
        {
            // cout << "Character Type Variable: " << identstr << " Value: " << retVal << " strlen: " << strlen << endl;
            if (retVal.IsString() == false)
            {
                ParseError(line, "Illegal Mixed Type Operands  [string]");
                // cout << "Run-Time Error: Illegal Mixed Type Operands  [string]" << endl;
                return false;
            }

            if (retVal.GetstrLen() < strlen)
            {
                string a = retVal.GetString();
                a.append(strlen - a.length(), ' ');
                retVal.SetString(a);
            }

            else
            {
                string a = retVal.GetString();
                retVal.SetString(a.substr(0, strlen));
            }
        }

        if (retVal.IsString())
        {
            retVal.SetstrLen(strlen);
        }

        TempsResults[identstr] = retVal;
        defVar[identstr] = true;

        // print what is stored in the map

        // cout<< "Initialization of the variable " << identstr <<" in the declaration statement." << endl;
        tok = Parser::GetNextToken(in, line);

        if (tok == COMMA)
        {

            status = VarList(in, line, idtok, strlen);
        }
        else
        {
            Parser::PushBackToken(tok);
            return true;
        }
    }
    else if (tok == COMMA)
    {

        status = VarList(in, line, idtok, strlen);
    }
    else if (tok == ERR)
    {
        ParseError(line, "Unrecognized Input Pattern");
        return false;
    }
    else
    {

        Parser::PushBackToken(tok);
        return true;
    }

    return status;

} // End of VarList

// PrintStmt:= PRINT *, ExpreList
bool PrintStmt(istream &in, int &line)
{
    LexItem t;
    ValQue = new queue<Value>; ////what is this doing???????

    t = Parser::GetNextToken(in, line);

    if (t != DEF)
    {

        ParseError(line, "Print statement syntax error.");
        return false;
    }
    t = Parser::GetNextToken(in, line);

    if (t != COMMA)
    {

        ParseError(line, "Missing Comma.");
        return false;
    }
    bool ex = ExprList(in, line);

    if (!ex)
    {
        ParseError(line, "Missing expression after Print Statement");
        return false;
    }

    while (!(*ValQue).empty()) //////what is happening here??????
    {
        Value nextVal = (*ValQue).front();
        cout << nextVal;
        ValQue->pop();
    }
    cout << endl;
    return ex;
} // End of PrintStmt

// BlockIfStmt:= if (Expr) then {Stmt} [Else Stmt]
// SimpleIfStatement := if (Expr) Stmt
bool BlockIfStmt(istream &in, int &line)
{
    bool ex = false, status;
    // static int nestlevel = 0;
    // int level;
    LexItem t;
    Value retVal; /////what is this doing???????????????????????

    t = Parser::GetNextToken(in, line);
    if (t != LPAREN)
    {

        ParseError(line, "Missing Left Parenthesis");
        return false;
    }

    ex = RelExpr(in, line, retVal);
    if (!ex)
    {
        ParseError(line, "Missing if statement condition");
        return false;
    }

    t = Parser::GetNextToken(in, line);
    if (t != RPAREN)
    {

        ParseError(line, "Missing Right Parenthesis");
        return false;
    }

    t = Parser::GetNextToken(in, line);
    if (t != THEN)
    {
        Parser::PushBackToken(t);

        if (!retVal.GetBool())  // why do we need this condition?????????????????????
        {

            t = Parser::GetNextToken(in, line);

            if (t != IDENT && t != PRINT)
            {
                ParseError(line, "Not a stmt");
                return false;
            }
            else if (t == IDENT && Parser::GetNextToken(in, line) != ASSOP)
            {
                ParseError(line, "Missing Assignment Operator");
                return false;
            }
            while (true)
            {
                LexItem newtok = Parser::GetNextToken(in, line);
                if (newtok == PRINT)
                {
                    Parser::PushBackToken(newtok);
                    break;
                }
                if (newtok == IDENT)
                {
                    LexItem newtok2 = Parser::GetNextToken(in, line);
                    if (newtok2 == ASSOP)
                    {
                        Parser::PushBackToken(newtok2);
                        Parser::PushBackToken(newtok);
                        break;
                    }
                }
            }
            return true;
        }

        status = SimpleStmt(in, line);
        if (status)
        {
            return true;
        }
        else
        {
            ParseError(line, "If-Stmt Syntax Error");
            return false;
        }
    }
    // nestlevel++;
    // level = nestlevel;
    if (retVal.GetBool()) //what is this supposed to do??????????????????
    {
        status = Stmt(in, line);
        if (!status)
        {
            ParseError(line, "Missing Statement for If-Stmt Then-Part");
            return false;
        }
        t = Parser::GetNextToken(in, line);
        if (t == ELSE)
        {
            while (true)
            {
                t = Parser::GetNextToken(in, line);
                if (t == END)
                {
                    break;
                }
            }
        }
    }
    else
    {
        while (true)
        {
            t = Parser::GetNextToken(in, line);
            if (t == ELSE || t == END)
            {
                break;
            }
        }
    }

    if (t == ELSE)
    {
        status = Stmt(in, line);
        if (!status)
        {
            ParseError(line, "Missing Statement for If-Stmt Else-Part");
            return false;
        }
        else
            t = Parser::GetNextToken(in, line);
    }

    if (t != END)
    {

        ParseError(line, "Missing END of IF");
        return false;
    }
    t = Parser::GetNextToken(in, line);
    if (t == IF)
    {
        // cout << "End of Block If statement at nesting level " << level << endl;
        return true;
    }

    Parser::PushBackToken(t);
    ParseError(line, "Missing IF at End of IF statement");
    return false;
} // End of IfStmt function

// Var:= ident
bool Var(istream &in, int &line, LexItem &idtok)
{
    string identstr;

    LexItem tok = Parser::GetNextToken(in, line);

    if (tok == IDENT)
    {
        identstr = tok.GetLexeme();

        // check if defVar contains Identstr
        if (defVar.find(identstr) == defVar.end())
        {
            ParseError(line, "Undeclared Variable");
            return false;
        }

        
        idtok = tok;

        return true;
    }
    else if (tok.GetToken() == ERR)
    {
        ParseError(line, "Unrecognized Input Pattern");
        cout << "(" << tok.GetLexeme() << ")" << endl;
        return false;
    }
    return false;
} // End of Var

// AssignStmt:= Var = Expr
bool AssignStmt(istream &in, int &line)
{

    bool varstatus = false, status = false;
    LexItem t;
    LexItem idtok;
    Value retVal;
    varstatus = Var(in, line, idtok);
    string identstr = idtok.GetLexeme();

    if (varstatus)
    {
        t = Parser::GetNextToken(in, line);

        if (t == ASSOP)
        {
            // cout << "[before] line is " << line << endl;
            status = Expr(in, line, retVal);
            // cout << "[before] line is " << line << endl;

            if (!status)
            {
                ParseError(t.GetLinenum(), "Missing Expression in Assignment Statment");
                return status;
            }

            if (SymTable[identstr] == INTEGER && retVal.IsInt() == false)
            {
                // cout << "Run-Time Error: Illegal Mixed Type Operands [Int]" << endl;
                ParseError(t.GetLinenum(), "Illegal Mixed Type Operands [Int]");
                return false;
            }

            if (SymTable[identstr] == REAL)
            {
                if (retVal.IsInt())
                {
                    retVal = Value(retVal.GetInt());
                }
                else if (retVal.IsReal())
                {
                    retVal = Value(retVal.GetReal());
                }
                else
                {
                    // cout << "Run-Time Error: Illegal Mixed Type Operands [Real]" << endl;
                    ParseError(t.GetLinenum(), "Illegal Mixed Type Operands [Real]");
                    return false;
                }
            }

            if (SymTable[identstr] == CHARACTER && retVal.IsString() == false)
            {
                // cout << "Run-Time Error: Illegal Mixed Type Operands [string]" << endl;

                ParseError(t.GetLinenum(), "Illegal Mixed Type Operands [string]");
                return false;
            }

            

            if (retVal.IsString())
            {
                int strlen = TempsResults[identstr].GetstrLen();
                // retVal.SetstrLen(strlen);

                if (retVal.GetstrLen() < strlen)
                {
                    string a = retVal.GetString();
                    a.append(strlen - a.length(), ' ');
                    retVal.SetString(a);
                }

                else
                {
                    string a = retVal.GetString();
                    retVal.SetString(a.substr(0, strlen));
                }
            }
            TempsResults[identstr] = retVal;
            defVar[identstr] = true;
        }
        else if (t.GetToken() == ERR)
        {
            ParseError(line, "Unrecognized Input Pattern");
            cout << "(" << t.GetLexeme() << ")" << endl;
            return false;
        }
        else
        {
            ParseError(line, "Missing Assignment Operator");
            return false;
        }
    }
    else
    {
        ParseError(line, "Missing Left-Hand Side Variable in Assignment statement");
        return false;
    }
    return status;
} // End of AssignStmt

// ExprList:= Expr {,Expr}
bool ExprList(istream &in, int &line)
{
    bool status = false;
    Value retVal;

    status = Expr(in, line, retVal);
    if (!status)
    {
        ParseError(line, "Missing Expression");
        return false;
    }

    ValQue->push(retVal);
    LexItem tok = Parser::GetNextToken(in, line);

    if (tok == COMMA)
    {

        status = ExprList(in, line);
    }
    else if (tok.GetToken() == ERR)
    {
        ParseError(line, "Unrecognized Input Pattern");
        cout << "(" << tok.GetLexeme() << ")" << endl;
        return false;
    }
    else
    {
        Parser::PushBackToken(tok);
        return true;
    }
    return status;
} // End of ExprList

// RelExpr ::= Expr  [ ( == | < | > ) Expr ]
bool RelExpr(istream &in, int &line, Value &retVal)
{

    bool t1 = Expr(in, line, retVal);
    LexItem tok;

    if (!t1)
    {
        return false;
    }

    tok = Parser::GetNextToken(in, line);
    if (tok.GetToken() == ERR)
    {
        ParseError(line, "Unrecognized Input Pattern");
        cout << "(" << tok.GetLexeme() << ")" << endl;
        return false;
    }
    if (tok == EQ || tok == LTHAN || tok == GTHAN)
    {
        Value newVal;
        t1 = Expr(in, line, newVal);
        if (!t1)
        {
            ParseError(line, "Missing operand after operator");
            return false;
        }
        // cout << "the value of the left expression is " << retVal << " and the value of the right expression is " << newVal << endl;
        try
        {
            //cout << "t1"
            if (tok == EQ)
            {
                retVal = retVal == newVal;
            }
            else if (tok == LTHAN)
            {

                retVal = retVal < newVal;
            }
            else if (tok == GTHAN)
            {
                retVal = retVal > newVal;
            }
        }
        catch (const std::exception &e)
        {

            ParseError(line, e.what());
            return false;
            
        }
    }

    return true;
} // End of RelExpr

// Expr ::= MultExpr { ( + | - | // ) MultExpr }
bool Expr(istream &in, int &line, Value &retVal)
{

    bool t1 = MultExpr(in, line, retVal);

    LexItem tok;

    if (!t1)
    {
        return false;
    }

    tok = Parser::GetNextToken(in, line);
    if (tok.GetToken() == ERR)
    {
        ParseError(line, "Unrecognized Input Pattern");
        cout << "(" << tok.GetLexeme() << ")" << endl;
        return false;
    }
    while (tok == PLUS || tok == MINUS || tok == CAT)
    {
        Value newVal;
        t1 = MultExpr(in, line, newVal);
        if (!t1)
        {
            ParseError(line, "Missing operand after operator");
            return false;
        }

        if (tok == PLUS)
        {
            retVal = retVal + newVal;
        }
        else if (tok == MINUS)
        {
            retVal = retVal - newVal;
        }
        else if (tok == CAT)
        {
            retVal = retVal + newVal;
        }

        tok = Parser::GetNextToken(in, line);
        if (tok.GetToken() == ERR)
        {
            ParseError(line, "Unrecognized Input Pattern");
            cout << "(" << tok.GetLexeme() << ")" << endl;
            return false;
        }
    }
    Parser::PushBackToken(tok);
    return true;
} // End of Expr

// MultExpr ::= TermExpr { ( * | / ) TermExpr }
bool MultExpr(istream &in, int &line, Value &retVal)
{

    bool t1 = TermExpr(in, line, retVal);

    LexItem tok;

    if (!t1)
    {
        return false;
    }

    tok = Parser::GetNextToken(in, line);
    if (tok.GetToken() == ERR)
    {
        ParseError(line, "Unrecognized Input Pattern");
        cout << "(" << tok.GetLexeme() << ")" << endl;
        return false;
    }
    while (tok == MULT || tok == DIV)
    {
        Value newVal;
        t1 = TermExpr(in, line, newVal);

        if (!t1)
        {
            ParseError(line, "Missing operand after operator");
            return false;
        }

        if (tok == MULT)
        {
            retVal = retVal * newVal;
        }
        else if (tok == DIV)
        {
            if (newVal.IsInt() && newVal.GetInt() == 0)
            {
                ParseError(tok.GetLinenum(), "Division by Zero");
                return false;
            }
            else if (newVal.IsReal() && newVal.GetReal() == 0.0)
            {
                ParseError(tok.GetLinenum(), "Division by Zero");
                return false;
            }
            retVal = retVal / newVal;
        }

        tok = Parser::GetNextToken(in, line);
        if (tok.GetToken() == ERR)
        {
            ParseError(line, "Unrecognized Input Pattern");
            cout << "(" << tok.GetLexeme() << ")" << endl;
            return false;
        }
    }
    Parser::PushBackToken(tok);
    return true;
} // End of MultExpr

// TermExpr ::= SFactor { ** SFactor }
bool TermExpr(istream &in, int &line, Value &retVal)
{

    bool t1 = SFactor(in, line, retVal);

    LexItem tok;

    if (!t1)
    {
        return false;
    }

    tok = Parser::GetNextToken(in, line);
    if (tok.GetToken() == ERR)
    {
        ParseError(line, "Unrecognized Input Pattern");
        cout << "(" << tok.GetLexeme() << ")" << endl;
        return false;
    }
    if (tok == POW)
    {
        Value newVal;
        t1 = TermExpr(in, line, newVal);

        if (!t1)
        {
            ParseError(line, "Missing exponent operand");
            return false;
        }

        retVal = retVal.Power(newVal);

        // tok = Parser::GetNextToken(in, line);

       
    }
    else
    {
        Parser::PushBackToken(tok);
    }
    return true;
} // End of TermExpr

// SFactor = Sign Factor | Factor
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
} // End of SFactor

// Factor := ident | iconst | rconst | sconst | (Expr)
bool Factor(istream &in, int &line, int sign, Value &retVal)
{

    LexItem tok = Parser::GetNextToken(in, line);
    // cout << "the tok in Factor is " << tok.GetLexeme() << endl;

    if (tok == IDENT)
    {

        string lexeme = tok.GetLexeme();
        // print defvar map at this stage
        // cout << "the value of the defVar map is " << defVar[lexeme] << endl;

        if (!(defVar.find(lexeme)->second))
        {
            ParseError(line, "Using Undefined Variable");
            return false;
        }

        // if the variable defined return its value
        if (SymTable[lexeme] == CHARACTER)
        {
            if (sign != 0)
            {
                ParseError(line, "Illegal Operation on Character Type");
                return false;
            }
            retVal = TempsResults[lexeme];
            return true;
        }

        if (SymTable[lexeme] == INTEGER)
        {
            // cout << "the sign is " << sign << " and the value is " << TempsResults[lexeme].GetInt() << endl;
            if (sign != 0)
            {
                retVal = Value(sign * TempsResults[lexeme].GetInt());
            }
            else
            {
                retVal = TempsResults[lexeme];
            }
            // retVal = Value(sign * TempsResults[lexeme].GetInt());
            return true;
        }

        if (SymTable[lexeme] == REAL)
        {
            if (sign != 0)
            {
                retVal = Value(sign * TempsResults[lexeme].GetReal());
            }
            else
            {
                retVal = TempsResults[lexeme];
            }
            return true;
        }

        // cout << " the tok in Factor [IDENT] is " << tok.GetLexeme() << endl;
        if (_log)
        {
            cout << "the token is " << tok.GetLexeme() << " and the value is " << TempsResults[lexeme] << endl;
        }

        retVal = TempsResults[lexeme];

        return true;
    }
    else if (tok == ICONST)
    {
        // cout << " the tok in Factor [ICONST] is " << tok.GetLexeme() << endl;
        if (sign == -1)
        {
            // cout << "[before] " << tok.GetLexeme() << " the sign is " << sign  << endl;
            retVal = Value(-stoi(tok.GetLexeme()));
            // cout << "[after] " << retVal << endl;
        }
        else
        {
            retVal = Value(stoi(tok.GetLexeme()));
        }
        // cout << "after conversion: " << retVal << endl;
        return true;
    }
    else if (tok == SCONST)
    {
        if (sign != 0)
        {
            ParseError(line, "Illegal Operation on Character Type");
            return false;
        }
        // return the char as retval
        retVal = Value(tok.GetLexeme());
        retVal.SetstrLen(retVal.GetString().length());
        return true;
    }
    else if (tok == RCONST)
    {

        if (sign == -1)
        {
            retVal = Value(-stod(tok.GetLexeme()));
        }
        else
        {
            retVal = Value(stod(tok.GetLexeme()));
        }

        // cout << " the tok in Factor [RCONST] is " << retVal << endl;
        return true;
    }
    else if (tok == LPAREN)
    {
        bool ex = Expr(in, line, retVal);
        if (!ex)
        {
            ParseError(line, "Missing expression after (");
            return false;
        }
        if (Parser::GetNextToken(in, line) == RPAREN)
            return ex;
        else
        {
            Parser::PushBackToken(tok);
            ParseError(line, "Missing ) after expression");
            return false;
        }
    }
    else if (tok.GetToken() == ERR)
    {
        ParseError(line, "Unrecognized Input Pattern");
        cout << "(" << tok.GetLexeme() << ")" << endl;
        return false;
    }

    return false;
}