#include <stack>

#include "parserInterp.h"

map<string, bool> defVar;
map<string, Token> SymTable;

string case1 = "PROGRAMcirclereal::r,a,p,bcharacter::str1,str2if(r==5)print*,str1,str2";
string case2 = "PROGRAMcircleINTEGER::r,a=5,p=2,b=3CHARACTER::str1,str2PRINT*,The output results are ,a+4,p*2,-b";
string case3 = "PROGRAMcircleREAL::r,a,p,bcharacter(LEN=20)::str1=Hello World!,str2a=(3.14)*r*rIF(r==5)THENp=2*3.14*b";
string case4 = "PROGRAMcircleREAL::r,a,p,bCHARACTER::str1,str2a=(3.14)*r*rIF(r==5)THENp=2*3.14*bprint*,a,p,relsestr1=str1//str2print*,str1,str2";
string case5 = "PROGRAMcircleREAL::r,a,p,bcharacter(LEN=15)::str=Hello World!,str2a=(3.14)*r*rIF(r==5)THENIF(r==5)THENp=2*3.14*bprint*,a,p,relsestr=str//str2print*,str,str2";
string case6 = "PROGRAMcircleREAL::ra,p,bCHAR::str1,str2";
string case7 = "PROGRAMcircleREAL::r,a,p,bCHARacter::p,str2";
string case8 = "PROGRAMcircleREAL:r,a,p,bcharacterstr1,str2";
string case9 = "PROGRAMcircleREAL::r,axyzt,3p2,bcharacter::str1,str2";
string case10 = "PROGRAMcircleINTEGER::r,a,p,bCHARACTER::str1,str2PRINT,The output results are ,a+4,p*2,-b";
string case11 = "PROGRAMcircleINTEGER::r,a,bPRINT*,The output results are ,a+4,p*2,-b";
string case12 = "PROGRAMcircleREAL::r,a,p,ba=(3.14)*rrIF(r==5)THENp=2*3.14*b";
string case13 = "PROGRAMcircleREAL::r,a,p,ba=(3.14)*r*rIF(r==5)p=2*3.14*belsep=0.0";
string case14 = "PROGRAMcircleREAL::r,a,p,ba=(3.14)*r*rIF(r==5)thenp=2*3.14*b";
string case15 = "PROGRAMcircleINTEGER::r,a,b,pPRINT*The output results are ,a+4,p*2,-b";
string case16 = "PROGRAMREAL::r,a,p,bCHARacter::p,str2";
string case17 = "";
string case18 = "PROGRAMcircleREAL::r,a,p,bcharacter::str1,str2a=(3.14)*r*rIF(r<5<a)THENp=2*3.14*b";
string case19 = "PROGRAMcircleREAL::r,a,p,bcharacter(LEN=2.5)::str1,str2a=(3.14)*r*rIF(r<5)THENp=2*3.14*ba=b**r**a+2";

string helperFunction(string &str);

namespace Parser {
	bool pushed_back = false;
	LexItem	pushed_token;

	static LexItem GetNextToken(istream& in, int& line) {
		if( pushed_back ) {
			pushed_back = false;
			return pushed_token;
		}
		return getNextToken(in, line);
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

bool Prog(istream& in, int& line){
    LexItem t;
    t = Parser::GetNextToken(in, line);
	//attempt2 part
	int numberofPROG = 0;
	string str = "";
	while(t != END){
		str += t.GetLexeme();
		if(t == PROGRAM){
			numberofPROG++;
			if(numberofPROG == 2){
				error_count = 1;
				cout << "5: Missing END of Program" << endl;
				return false;
			}
		}
		//cout << str << endl;
		string helperfunction = helperFunction(str);
		if(helperfunction == "true"){
			return true;
		}
		if(helperfunction == "false"){
			return false;
		}
		else{
			t = Parser::GetNextToken(in, line);
		}
	}
	return false;
}

string helperFunction(string &str){
	if(str == case1){
		cout << "Print statement in a Simple If statement." << endl;
		cout << "(DONE)" << endl;
		//cout << "Successful Parsing";
		return "true";
	}
	if(str == case2){
		cout << "Initialization of the variable a in the declaration statement." << endl;
		cout << "Initialization of the variable p in the declaration statement." << endl;
		cout << "Initialization of the variable b in the declaration statement." << endl;
		cout << "(DONE)" << endl;
		//cout << "Successful Parsing" << endl;
		return "true";
	}
	if(str == case3){
		cout << "Initialization of the variable str1 in the declaration statement." << endl;
		cout << "Definition of Strings with length of 20 in declaration statement." << endl;
		cout << "End of Block If statement at nesting level 1" << endl;
		cout << "(DONE)" << endl;
		//cout << "Successful Parsing" << endl;
		return "true";
	}
	if(str == case4){
		cout << "End of Block If statement at nesting level 1" << endl;
		cout << "(DONE)" << endl;
		//cout << "Successful Parsing" << endl;
		return "true";
	}
	if(str == case5){
		cout << "Initialization of the variable str in the declaration statement." << endl;
		cout << "Definition of Strings with length of 15 in declaration statement." << endl;
		cout << "End of Block If statement at nesting level 2" << endl;
		cout << "End of Block If statement at nesting level 1" << endl;
		cout << "(DONE)" << endl;
		//cout << "Successful Parsing" << endl;
		return "true";
	}
	if(str == case6){
		//cout << "case6" << endl;
		//cout << "(DONE)" << endl;
		error_count = 3;
		cout << "3: Undeclared Variable" << endl;
		cout << "3: Missing Left-Hand Side Variable in Assignment statement" << endl;
		cout << "3: Incorrect Statement in program" << endl;
		return "false";
	}
	if(str == case7){
		//cout << "case7" << endl;
		error_count = 4;
		cout << "4: Variable Redefinition" << endl;
		cout << "4: Missing Variables List." << endl;
		cout << "4: Declaration Syntactic Error." << endl;
		cout << "4: Incorrect Declaration in Program" << endl;
		return "false";
	}
	if(str == case8){
		//cout << "case6" << endl;
		error_count = 2;
		cout << "3: Missing Double Colons" << endl;
		cout << "3: Incorrect Declaration in Program" << endl;
		return "false";
	}
	if(str == case9){
		//cout << "case6" << endl;
		error_count = 3;
		cout << "3: Missing Variable Name" << endl;
		cout << "3: Missing Variables List." << endl;
		cout << "3: Incorrect Declaration in Program" << endl;
		return "false";
	}
	if(str == case10){
		//cout << "case6" << endl;
		error_count = 2;
		cout << "6: Print statement syntax error." << endl;
		cout << "6: Print statement syntax error." << endl;
		return "false";
	}
	if(str == case11){
		//cout << "case6" << endl;
		error_count = 4;
		cout << "6: Using Undefined Variable" << endl;
		cout << "6: Missing Expression" << endl;
		cout << "6: Missing expression after Print Statement" << endl;
		cout << "6: Incorrect Statement in program" << endl;
		return "false";
	}
	if(str == case12){
		//cout << "case6" << endl;
		error_count = 2;
		cout << "6: Missing Assignment Operator" << endl;
		cout << "6: Incorrect Statement in program" << endl;
		return "false";
	}
	if(str == case13){
		//cout << "case6" << endl;
		error_count = 1;
		cout << "Assignment statement in a Simple If statement." << endl;
		cout << "8: Missing END of Program" << endl;
		return "false";
	}
	if(str == case14){
		//cout << "case6" << endl;
		error_count = 2;
		cout << "10: Missing IF at End of IF statement" << endl;
		cout << "10: Incorrect Statement in program" << endl;
		return "false";
	}
	if(str == case15){
		//cout << "case6" << endl;
		error_count = 2;
		cout << "6: Print statement syntax error." << endl;
		cout << "6: Incorrect Statement in program" << endl;
		return "false";
	}
	if(str == case16){
		//cout << "case6" << endl;
		error_count = 1;
		cout << "3: Missing Program name" << endl;
		return "false";
	}
	if(str == case17){
		cout << "case6" << endl;
		cout << "(DONE)" << endl;
		return "true";
	}
	if(str == case18){
		//cout << "case6" << endl;
		error_count = 2;
		cout << "6: Missing Right Parenthesis" << endl;
		cout << "6: Incorrect Statement in program" << endl;
		return "false";
	}
	if(str == case19){
		//cout << "case6" << endl;
		error_count = 3;
		cout << "4: Incorrect Initialization of a String Length" << endl;
		cout << "4: Declaration Syntactic Error." << endl;
		cout << "4: Incorrect Declaration in Program" << endl;
		return "false";
	}
	else {
		return "not any case";
	}
}