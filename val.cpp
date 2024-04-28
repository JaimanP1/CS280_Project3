#include "val.h"

Value Value::operator+(const Value& op) const{
    //for now limiting to only working with ints and reals
    //case of real and real
    if (op.IsReal() && IsReal()){
        return Value(GetReal() + op.GetReal());
    }
    //case of int and int
    if (op.IsInt() && IsInt()){
        return Value(GetInt() + op.GetInt());
    }
    //case of real and int
    if (op.IsReal() && IsInt()){
        return Value(GetReal() + (double)op.GetInt());
    }
    //case of int and real
    if (op.IsInt() && IsReal()){
        return Value((double)GetInt() + op.GetReal());
    }
    //improper operands
    else {
        return Value();
    }
}

Value Value::operator-(const Value& op) const{
    //for now limiting to only working with ints and reals
    //case of real and real
    if (op.IsReal() && IsReal()){
        return Value(GetReal() - op.GetReal());
    }
    //case of int and int
    if (op.IsInt() && IsInt()){
        return Value(GetInt() - op.GetInt());
    }
    //case of real and int
    if (op.IsReal() && IsInt()){
        return Value(GetReal() - (double)op.GetInt());
    }
    //case of int and real
    if (op.IsInt() && IsReal()){
        return Value((double)GetInt() - op.GetReal());
    }
    //improper operands
    else {
        return Value();
    }
}

Value Value::operator*(const Value& operand) const{
    //only works for ints and reals
    //case of int and real
    if (operand.IsInt() && IsReal()) {
        return Value(GetReal() * (double)operand.GetInt());
    }
    //case of real and int
    if (operand.IsReal() && IsInt()) {
        return Value((double)GetInt() * operand.GetReal());
    }
    //case of int and int
    if (operand.IsInt() && IsInt()) {
        return Value(GetInt() * operand.GetInt());
    }
    //case of real and real
    if (operand.IsReal() && IsReal()) {
        return Value(GetReal() * operand.GetReal());
    }
    //improper operands
    else {
        return Value();
    }
}

Value Value::operator/(const Value& op) const{
    //for now limiting to only working with ints and reals
    //case of real and real
    if (op.IsReal() && IsReal()){
        return Value(GetReal() / op.GetReal());
    }
    //case of int and int
    if (op.IsInt() && IsInt()){
        return Value(GetInt() / op.GetInt());
    }
    //case of real and int
    if (op.IsReal() && IsInt()){
        return Value(GetReal() / (double)op.GetInt());
    }
    //case of int and real
    if (op.IsInt() && IsReal()){
        return Value((double)GetInt() / op.GetReal());
    }
    //improper operands
    else {
        return Value();
    }
}

Value Value::Catenate(const Value & operand) const{ 
    //only works for strings
    //case of string and string
    if (operand.IsString() && IsString()) {
        return Value(GetString() + operand.GetString());
    }
    //improper operands
    else {
        return Value();
    }
}

Value Value::Power(const Value & operand) const{ 
    //only works for ints and reals
    //case of int and real
    if (operand.IsInt() && IsReal()) {
        return Value(pow(GetReal(), (double)operand.GetInt()));
    }
    //case of real and int
    if (operand.IsReal() && IsInt()) {
        return Value(pow((double)GetInt(), operand.GetReal()));
    }
    //case of real and real
    if (operand.IsReal() && IsReal()) {
        return Value(pow(GetReal(), operand.GetReal()));
    }
    //case of int and int
    if (operand.IsInt() && IsInt()) {
        return Value(pow((double)GetInt(), (double)operand.GetInt()));
    }   
    //improper operands
    else {
        return Value();
    }
}

Value Value::operator==(const Value& operand) const{ 
    //only works for ints and reals, if need be can add strings
    //case of real and int
    if (operand.IsReal() && IsInt()) {
        return Value((double)GetInt() == operand.GetReal());
    }
    //case of int and real
    if (operand.IsInt() && IsReal()) {
        return Value(GetReal() == (double)operand.GetInt());
    }
    //case of int and int
    if (operand.IsInt() && IsInt()) {
        return Value(GetInt() == operand.GetInt());
    }
    //case of real and real
    if (operand.IsReal() && IsReal()) {
        return Value(GetReal() == operand.GetReal());
    } 
    //improper operands
    else {
        return Value();
    }
}

Value Value::operator>(const Value& operand) const{ 
    //only works for ints and reals
    //case of real and int
    if (operand.IsReal() && IsInt()) {
        return Value((double)GetInt() > operand.GetReal());
    }
    //case of int and real
    if (operand.IsInt() && IsReal()) {
        return Value(GetReal() > (double)operand.GetInt());
    }
    //case of int and int
    if (operand.IsInt() && IsInt()) {
        return Value(GetInt() > operand.GetInt());
    }
    //case of real and real
    if (operand.IsReal() && IsReal()) {
        return Value(GetReal() > operand.GetReal());
    } 
    //improper operands
    else {
        return Value();
    }
}

Value Value::operator<(const Value& operand) const{ 
    //only works for ints and reals
    //case of real and int
    if (operand.IsReal() && IsInt()) {
        return Value((double)GetInt() < operand.GetReal());
    }
    //case of int and real
    if (operand.IsInt() && IsReal()) {
        return Value(GetReal() < (double)operand.GetInt());
    }
    //case of int and int
    if (operand.IsInt() && IsInt()) {
        return Value(GetInt() < operand.GetInt());
    }
    //case of real and real
    if (operand.IsReal() && IsReal()) {
        return Value(GetReal() < operand.GetReal());
    } 
    //improper operands
    else {
        return Value();
    }
}

