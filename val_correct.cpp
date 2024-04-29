#include "val.h"

Value Value::operator+(const Value& op) const {
    Value result;
    if (IsInt() && op.IsInt()) {
        result.SetInt(Itemp + op.Itemp);
        result.SetType(VINT);
    } else if (IsReal() && op.IsReal()) {
        result.SetReal(Rtemp + op.Rtemp);
        result.SetType(VREAL);
    } else if (IsString() && op.IsString()) {
        result.SetString(Stemp + op.Stemp);
        result.SetType(VSTRING);
        result.SetstrLen(strLen + op.strLen);
    } else if (IsString() && op.IsInt()) {
        result.SetString(Stemp + to_string(op.Itemp));
        result.SetType(VSTRING);
        result.SetstrLen(strLen + 1);
    } else if (IsInt() && op.IsString()) {
        result.SetString(to_string(Itemp) + op.Stemp);
        result.SetType(VSTRING);
        result.SetstrLen(1 + op.strLen);
    } else if (IsReal() && op.IsInt()) {
        result.SetReal(Rtemp + op.Itemp);
        result.SetType(VREAL);
    } else if (IsInt() && op.IsReal()) {
        result.SetReal(Itemp + op.Rtemp);
        result.SetType(VREAL);
    } else if (IsReal() && op.IsString()) {
        result.SetString(to_string(Rtemp) + op.Stemp);
        result.SetType(VSTRING);
        result.SetstrLen(1 + op.strLen);
    } else if (IsString() && op.IsReal()) {
        result.SetString(Stemp + to_string(op.Rtemp));
        result.SetType(VSTRING);
        result.SetstrLen(strLen + 1);
    } else
        throw "RUNTIME ERROR: Illegal Mixed Type Operands";
    return result;
}

Value Value::operator-(const Value& op) const {
    Value result;
    if (IsInt() && op.IsInt()) {
        result.SetInt(Itemp - op.Itemp);
        result.SetType(VINT);
    } else if (IsReal() && op.IsReal()) {
        result.SetReal(Rtemp - op.Rtemp);
        result.SetType(VREAL);
    } else if (IsReal() && op.IsInt()) {
        result.SetReal(Rtemp - op.Itemp);
        result.SetType(VREAL);
    } else if (IsInt() && op.IsReal()) {
        result.SetReal(Itemp - op.Rtemp);
        result.SetType(VREAL);
    } else
        throw "RUNTIME ERROR: Illegal Mixed Type Operands";
    return result;
}

Value Value::operator*(const Value& op) const {
    Value result;
    if (IsInt() && op.IsInt()) {
        result.SetInt(Itemp * op.Itemp);
        result.SetType(VINT);
    } else if (IsReal() && op.IsReal()) {
        result.SetReal(Rtemp * op.Rtemp);
        result.SetType(VREAL);
    } else if (IsReal() && op.IsInt()) {
        result.SetReal(Rtemp * op.Itemp);
        result.SetType(VREAL);
    } else if (IsInt() && op.IsReal()) {
        result.SetReal(Itemp * op.Rtemp);
        result.SetType(VREAL);
    } else
        throw "RUNTIME ERROR: Illegal Mixed Type Operands";
    return result;
}

Value Value::operator/(const Value& op) const {

    cout << "is real " << IsReal() << "op is real" << op.IsReal() << endl;
    cout << " Rtemp " << Rtemp << " op.Rtemp " << op.Rtemp << endl;

    Value result;
    if (IsInt() && op.IsInt()) {
        if (op.Itemp == 0)
            throw "RUNTIME ERROR: Illegal Division by Zero";
        result.SetInt(Itemp / op.Itemp);
        result.SetType(VINT);
    } else if (IsReal() && op.IsReal()) {
        if (op.Rtemp == 0.0)
            throw "RUNTIME ERROR: Illegal Division by Zero";
        result.SetReal(Rtemp / op.Rtemp);
        result.SetType(VREAL);
    } else if (IsReal() && op.IsInt()) {
        if (op.Itemp == 0)
            throw "RUNTIME ERROR: Illegal Division by Zero";
        result.SetReal(Rtemp / op.Itemp);
        result.SetType(VREAL);
    } else if (IsInt() && op.IsReal()) {
        if (op.Rtemp == 0.0)
            throw "RUNTIME ERROR: Illegal Division by Zero";
        result.SetReal(Itemp / op.Rtemp);
        result.SetType(VREAL);
    } else
        throw "RUNTIME ERROR: Illegal Mixed Type Operands";

    cout << "result " << result.GetReal() << endl;
    return result;
}

Value Value::operator<(const Value& op) const {
    if (IsInt() && op.IsInt()) {
        return Value(GetInt() < op.GetInt());
    }
    else if (IsReal() && op.IsReal()) {
        return Value(GetReal() < op.GetReal());
    }
    else if(IsInt() && op.IsReal()){
        return Value(GetInt() < op.GetReal());
    }
    else if(IsReal() && op.IsInt()){
        return Value(GetReal() < op.GetInt());
    }
    else if (IsReal() && op.IsString()) {
        //throw error
        throw "RUNTIME ERROR: Illegal Mixed Type Operands [Real < String]";
    }
    else if (IsString() && op.IsReal()) {
        //throw error
        throw "RUNTIME ERROR: Illegal Mixed Type Operands [String < Real]";
    }
    else if (IsInt() && op.IsString()) {
        //throw error
        throw "RUNTIME ERROR: Illegal Mixed Type Operands [Int < String]";
    }
    else if (IsString() && op.IsInt()) {
        //throw error
        throw "RUNTIME ERROR: Illegal Mixed Type Operands [String < Int]";
    }
    else {
        return Value();
    }
}

Value Value::operator>(const Value& op) const {

    cout << GetType() << " " << op.GetType() << endl;

    if (IsInt() && op.IsInt()) {
        return Value(GetInt() > op.GetInt());
    }
    else if (IsReal() && op.IsReal()) {
        return Value(GetReal() > op.GetReal());
    }
    else if(IsInt() && op.IsReal()){
        return Value(GetInt() > op.GetReal());
    }
    else if(IsReal() && op.IsInt()){
        return Value(GetReal() > op.GetInt());
    }
    else if (IsReal() && op.IsString()) {
        //throw error
        //cout << "RUNTIME ERROR: Illegal Mixed Type Operands [Real > String]" << endl;
        throw std::invalid_argument( "RUNTIME ERROR: Illegal Mixed Type Operands [Real > String]" );    
    }
    else if (IsString() && op.IsReal()) {
        //throw error
        throw std::invalid_argument( "RUNTIME ERROR: Illegal Mixed Type Operands [String > Real]" );    
    }
    else if (IsInt() && op.IsString()) {
        //throw error
        //cout << "RUNTIME ERROR: Illegal Mixed Type Operands [Real > String]" << endl;
        throw std::invalid_argument( "RUNTIME ERROR: Illegal Mixed Type Operands [Int > String]" );    
    }
    else if (IsString() && op.IsInt()) {
        //throw error
        throw std::invalid_argument( "RUNTIME ERROR: Illegal Mixed Type Operands [String < Int]" );    
    }
    else {
        return Value();
    }
}
Value Value::Catenate(const Value& op) const {
    if (IsString() && op.IsString()) {
        return Value(GetString() + op.GetString());
    }
    else {
        return Value();
    }
}

Value Value::Power(const Value& op) const {
    if ((IsInt() || IsReal()) && (op.IsInt() || op.IsReal())) {
        if (IsReal() && op.IsReal()) {
            return pow(GetReal(), op.GetReal());
        } 
        else if (IsInt() && op.IsInt()) {
            return pow(GetInt(), op.GetInt());
        }
        else if (IsInt() && op.IsReal()) {
            return pow(GetInt(), op.GetReal());
        }
        else if (IsReal() && op.IsInt()) {
            return pow(GetReal(), op.GetInt());
        }
    }
    return Value();
}

Value Value::operator==(const Value& op) const {
    Value result;
    if (IsInt() && op.IsInt()) {
        result.SetBool(Itemp == op.Itemp);
        result.SetType(VBOOL);
    } else if (IsReal() && op.IsReal()) {
        result.SetBool(Rtemp == op.Rtemp);
        result.SetType(VBOOL);


    }else if (IsReal() && op.IsInt()) {
        result.SetBool(Rtemp == op.Itemp);
        result.SetType(VBOOL);
    }else if (IsInt() && op.IsReal()) {
        result.SetBool(Itemp == op.Rtemp);
        result.SetType(VBOOL);


    } else if (IsString() && op.IsString()) {
        result.SetBool(Stemp == op.Stemp);
        result.SetType(VBOOL);
    } else if (IsBool() && op.IsBool()) {
        result.SetBool(Btemp == op.Btemp);
        result.SetType(VBOOL);
    } else
        throw "RUNTIME ERROR: Illegal Mixed Type Operands";
    return result;
}