/* 
 * parserInterp.h
 * Programming Assignment 3
 * Spring 2024
*/

#ifndef PARSER_H_
#define PARSER_H_

#include <iostream>

using namespace std;

#include "lex.h"
#include "val.h"

extern bool Prog(istream& in, int& line);
extern bool Decl(istream& in, int& line);
extern bool Type(istream& in, int& line);
extern bool VarList(istream& in, int& line, LexItem & idtok, int strlen = 1 );
extern bool Stmt(istream& in, int& line);
extern bool SimpleStmt(istream& in, int& line);
extern bool PrintStmt(istream& in, int& line);
extern bool BlockIfStmt(istream& in, int& line);
extern bool SimpleIfStmt(istream& in, int& line);
extern bool AssignStmt(istream& in, int& line);
extern bool Var(istream& in, int& line, LexItem & idtok);
extern bool ExprList(istream& in, int& line);
extern bool RelExpr(istream& in, int& line, Value & retVal);
extern bool Expr(istream& in, int& line, Value & retVal);
extern bool MultExpr(istream& in, int& line, Value & retVal);
extern bool TermExpr(istream& in, int& line, Value & retVal);
extern bool SFactor(istream& in, int& line, Value & retVal);
extern bool Factor(istream& in, int& line, int sign, Value & retVal);

extern int ErrCount();

#endif /* PARSER_H_ */
